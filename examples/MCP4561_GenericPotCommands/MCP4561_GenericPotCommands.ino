// Copyright 2018 Steve Quinn
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
#include "MCP4561_DIGI_POT.h"
#include <Wire.h>


//MCP4561 digitalPot(0x2E);                      // Instance of MCP4561 pot with address supplied. A0 = 0
MCP4561 digitalPot;                              // Instance of MCP4561 pot with default address. A0 = 0

const char *cRegNames[MCP4561_GP_EEPROM_9+1] = {
                     "Volatile Wiper 0",
                     "Volatile Wiper 1",
                     "Non-volatile Wiper 0",
                     "Non-volatile Wiper 1",
                     "Volatile TCON Register",
                     "Status Register",
                     "Data EEPROM 0",
                     "Data EEPROM 1",     
                     "Data EEPROM 2",      
                     "Data EEPROM 3",      
                     "Data EEPROM 4",      
                     "Data EEPROM 5",      
                     "Data EEPROM 6",      
                     "Data EEPROM 7",      
                     "Data EEPROM 8",      
                     "Data EEPROM 9"      
};

uint16_t u16Result = 0;
#define MAX_MESSAGE_SIZE 150
char cMessage[MAX_MESSAGE_SIZE];
char cAddr;
uint16_t u16readValue;
uint16_t u16PotVal;
uint16_t u16ADCVal;
float f;

void setup(void)
{
  Wire.begin();                                   
  Serial.begin(115200);                           
  putMenu();                                  
} 


void loop(void)
{
  char c;

  if (Serial.available())
    c = Serial.read();

  //delay(5);
  switch(c) {
    case 'm':
        putMenu();
        Serial.flush();
      break;
    
    case 'r':
        cAddr = Serial.parseInt();
        if (cAddr <= MCP4561_GP_EEPROM_9) {
          u16readValue = digitalPot.readVal(cAddr);  
          sprintf(cMessage," r - MCP4561 memory at [%s] 0x%02X/%d : 0x%04X/%d",cRegNames[cAddr],cAddr,cAddr,u16readValue,u16readValue);               
          Serial.println(cMessage);
        } else {
          Serial.println(F(" r - Memory out of bounds"));
        }
        Serial.flush();
      break;

    case 'p':
        for (uint8_t x = 0; x <= MCP4561_GP_EEPROM_9; x++) {
          u16readValue = digitalPot.readVal(x);  
          sprintf(cMessage," MCP4561 memory at [%s] 0x%02X/%d : 0x%04X/%d",cRegNames[x],x,x,u16readValue,u16readValue);               
          Serial.println(cMessage);
        }
        Serial.flush();
      break;

    case 'v':
        uint16_t u16PotVal;
        u16PotVal = Serial.parseInt();
        u16Result = digitalPot.writeVal(MCP4561_VOL_WIPER_0,u16PotVal);
         if (u16Result == MCP4561_SUCCESS)             
         {
           Serial.print(F(" v - MCP4561 set to : "));
           Serial.println(u16PotVal);
         } 
         else
           Serial.println(F(" v - MCP4561 failed to set new volatile value"));  
         Serial.flush();
      break;

    case 'n':
        u16PotVal = Serial.parseInt();
        u16Result = digitalPot.writeVal(MCP4561_NON_VOL_WIPER_0,u16PotVal);
          if (u16Result == MCP4561_SUCCESS)             
          {
            Serial.print(F(" n - MCP4561 set to : "));
            Serial.println(u16PotVal);
          } 
          else
            Serial.println(F(" n - MCP4561 failed to set new non-volatile value"));  
         Serial.flush();
      break;

    case 'd':
        u16Result = digitalPot.potDisconnectAll(MCP4561_WIPER_0);
        if (u16Result == MCP4561_SUCCESS)             
           Serial.println(F(" d - MCP4561 Pot 0 disconnected"));
        else   
           Serial.println(F(" d - MCP4561 Failed to disconnect Pot 0"));
         Serial.flush();
      break;
          
    case 'c':
        u16Result = digitalPot.potConnectAll(MCP4561_WIPER_0);
        if (u16Result == MCP4561_SUCCESS)             
          Serial.println(F(" c - MCP4561 Pot 0 connected"));
        else  
          Serial.println(F(" c - MCP4561 Failed to connect Pot 0"));
         Serial.flush();
      break;
    
    case 'g':
         u16ADCVal = analogRead(A0);
         f = (float)(((float)u16ADCVal/1023.0) * 5.0);
         sprintf(cMessage," g - Analogue Pin A0 Value : %d, %d.%02dv (assuming supply rail is 5v)",u16ADCVal,  (int)f, abs((int)(f*100)%100));               
         Serial.println(cMessage);
         Serial.flush();
      break;
      
    case 'a':
        u16Result = digitalPot.potDisconnectSelective(MCP4561_TERM_R0A);
        if (u16Result == MCP4561_SUCCESS)             
          Serial.println(F(" a - MCP4561 Pot 0 R0A High Side disconnected"));
        else  
          Serial.println(F(" a - MCP4561 Failed to disconnect Pot 0 R0A High Side"));
         Serial.flush();
      break;
    
    case 'w':
        u16Result = digitalPot.potDisconnectSelective(MCP4561_TERM_R0W);
        if (u16Result == MCP4561_SUCCESS)             
          Serial.println(F(" w - MCP4561 Pot 0 R0W Wiper disconnected"));
        else  
          Serial.println(F(" w - MCP4561 Failed to disconnect Pot 0 R0W Wiper"));
         Serial.flush();
      break;
    
    case 'b':
        u16Result = digitalPot.potDisconnectSelective(MCP4561_TERM_R0B);
        if (u16Result == MCP4561_SUCCESS)             
          Serial.println(F(" b - MCP4561 Pot 0 R0B Low Side disconnected"));
        else  
          Serial.println(F(" b - MCP4561 Failed to disconnect Pot 0 R0B Low Side"));
         Serial.flush();
      break;
    
  }
}


void putMenu(void)
{
  Serial.println(F("Microchip MCP4561_DIGI_POT Arduino Library Example"));
  Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
  Serial.println(F("m     : This Menu"));
  Serial.println(F("rXX   : Read memory at XX (00 - 16)"));
  Serial.println(F("p     : Dump all memory (00 - 16)"));
  Serial.println(F("vXXX  : Set Volatile Wiper 0 with XXX (000 - 257)"));
  Serial.println(F("nXXX  : Set Non-volatile Wiper 0 with XXX (000 - 257)"));
  Serial.println(F("d     : Disconnect Pot 0"));
  Serial.println(F("c     : Connect Pot 0"));
  Serial.println(F("g     : Read ADC A0 Value"));
  Serial.println(F("a     : Float Pot 0 High Side (R0A). ADC should read 0v"));
  Serial.println(F("w     : Float Pot 0 Wiper (R0W). ADC value will fluctuate as it is now floating"));
  Serial.println(F("b     : Float Pot 0 Low Side (R0B). ADC should read Vcc"));
  Serial.println(F("--------------------------------------------------"));
}
