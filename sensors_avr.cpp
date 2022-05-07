/* trackuino copyright (C) 2010  EA5HAV Javi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* Credit to:
 *
 * cathedrow for this idea on using the ADC as a volt meter:
 * http://code.google.com/p/tinkerit/wiki/SecretVoltmeter
 */

#ifdef AVR

#include "config.h"
#include "pin.h"
#include "sensors_avr.h"
#if (ARDUINO + 1) >= 100
#  include <Arduino.h>
#else
#  include <WProgram.h>
#endif

/*
 * sensors_aref: measure an external voltage hooked up to the AREF pin,
 * optionally (and recommendably) through a pull-up resistor. This is
 * incompatible with all other functions that use internal references
 * (see config.h)
 */
#ifdef USE_AREF
void sensors_setup()
{
  // Nothing to set-up when AREF is in use
}

unsigned long sensors_aref()
{
  unsigned long result;
  // Read 1.1V reference against AREF (p. 262)
  ADMUX = _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = (ADCH << 8) | ADCL;

  // millivolts = 1.1 * 1024 * 1000 / result
  result = 1126400 / result;

  // aref = read aref * (32K + AREF_PULLUP) / 32K
  result = result * (32000UL + AREF_PULLUP) / 32000;

  return result;
}
#endif

#ifndef USE_AREF
void sensors_setup()
{
}

int sensors_vin()
{
  analogReference(DEFAULT);      // Ref=5V
  analogRead(VMETER_PIN);        // Disregard the 1st conversion after changing ref (p.256)
  delay(10);                     // This is needed when switching references

  uint16_t adc = analogRead(VMETER_PIN); 
  uint16_t mV = 5000L * adc / 1024;
   
  // Vin = mV * R2 / (R1 + R2)
  int vin = (uint32_t)mV;
  return vin;
}


#endif
#endif // ifdef AVR
