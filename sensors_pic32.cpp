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

#ifdef PIC32MX

#include "config.h"
#include "pin.h"
#include "sensors_pic32.h"
#include <WProgram.h>

void sensors_setup()
{
  pinMode(EXTERNAL_LM60_VS_PIN, OUTPUT);
  pinMode(INTERNAL_LM60_VS_PIN, OUTPUT);
  analogReference(DEFAULT);       // Ref=AVDD, AVSS
}

int sensors_vin()
{
   uint16_t adc = analogRead(VMETER_PIN); 
   uint16_t mV = 3300L * adc / 1024;
   
   // Vin = mV * R2 / (R1 + R2)
   int vin = (uint32_t)mV;
   return vin;
}


#endif // ifdef PIC32MX
