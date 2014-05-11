// WeatherRadio Copyright (C) 2014 Derek Chafin

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <EEPROMex.h>

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <Si4707.h>
#include <SPI.h>
#include <SD.h>

#define CONFIG_VERSION "wr1"

#define memoryBase 32

bool ok = true;
int configAddress = 0;
struct StoreStruct
{
  char version[4];

} storage;

tmElements_t tm;
File log;

void setup()
{
  // Setup EEPROM with defaults
  EEPROM.setMemPool(memoryBase, EEPROMSizeUno);
  configAddress = EEPROM.getAddress(sizeof(StoreStruct));

  ok = loadConfig();
  if (!ok)
  {
    setDefaults();
    saveConfig();
  }

  // Setup Clock
  if (!RTC.read(tm))
    loopError();

  // Setup SD Card
  pinMode(10, OUTPUT);

  // Setup Radio


  // Setup Other Stuff

}

void loop()
{

}

void loopError()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  // Blink forever
  while (true)
  {
    delay(1000);
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
    delay(250);
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
  }
}

bool loadConfig()
{
  EEPROM.readBlock(configAddress, storage);
  return (storage.version == CONFIG_VERSION);
}

void saveConfig()
{
  EEPROM.writeBlock(configAddress, storage);
}

void setDefaults()
{
  storage.version == CONFIG_VERSION;
}