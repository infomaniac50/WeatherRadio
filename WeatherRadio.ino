// WeatherRadio.ino

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

  // Setup Radio

  // Setup Other Stuff
}

void loop()
{

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