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