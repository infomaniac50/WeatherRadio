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
#include <SI4707.h>
#include <SPI.h>
#include <SD.h>

#define ERROR_PIN 13
// EEPROM Globals
// #define CONFIG_VERSION "wr1"
const char CONFIG_VERSION[4] PROGMEM = "wr1";
#define MEMORY_BASE 32

int configAddress = 0;

struct StoreStruct
{
  char version[4];
  unsigned long frequency;
} storage;


// SD Card Globals
File logFile;

// Si4707 Globals
byte function = 0x00;           //  Function to be performed.

void setup()
{
  // On recent versions of Arduino the LED pin likes to turn on for no apparent reason
  pinMode(13, OUTPUT);

  Serial.begin(115200);
  // Setup EEPROM with defaults

  EEPROM.setMemPool(MEMORY_BASE, EEPROMSizeMega);
  configAddress = EEPROM.getAddress(sizeof(StoreStruct));

  if (!loadConfig())
  {
    blink(13, 1500);
    blink(13, 1500);
    blink(13, 1500);
    setDefaults();
    saveConfig();
  }

  // Setup Clock
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet)
    errorLoop();

  // Setup SD Card
  // Set hardware SS pin as output
  // Uno SS pin = 10
  // Mega SS pin = 53
  // SS is defined in the pins_arduino.h file
  pinMode(SS, OUTPUT);

  // Currently using the Ethernet Shield. Not efficient but it works.
  if (!SD.begin(4))
    errorLoop();

  // sprintf_P(filename, name_format, tm.Month, tm.Day, tm.Year + 1970, tm.Hour, tm.Minute, tm.Second);
  // logFile = SD.open(filename, FILE_WRITE);
  logFile = SD.open("log.txt", FILE_WRITE);

  if (!logFile)
    errorLoop();

  // Setup Radio

  delay(100);
  Serial.println(F("Starting up the Si4707......."));
  Serial.println();
  delay(1000);
  showMenu();
  delay(1000);
  Radio.begin(8);
  Radio.patch();          //  Use this one to to include the 1050 Hz patch.
  //Radio.on();           //  Use this one if not using the patch.
  //Radio.getRevision();  //  Only captured on the logic analyzer - not displayed.
  //
  //  All useful interrupts are enabled here.
  //
  // Radio.setProperty(GPO_IEN, (CTSIEN | ERRIEN | RSQIEN | SAMEIEN | ASQIEN | STCIEN));
  //
  //  RSQ Interrupt Sources.
  //
  Radio.setProperty(WB_RSQ_SNR_HIGH_THRESHOLD, 0x007F);   // 127 dBuV for testing..want it high
  Radio.setProperty(WB_RSQ_SNR_LOW_THRESHOLD, 0x0001);    // 1 dBuV for testing
  Radio.setProperty(WB_RSQ_RSSI_HIGH_THRESHOLD, 0x004D);  // -30 dBm for testing
  Radio.setProperty(WB_RSQ_RSSI_LOW_THRESHOLD, 0x0007);   // -100 dBm for testing
  //Radio.setProperty(WB_RSQ_INT_SOURCE, (SNRHIEN | SNRLIEN | RSSIHIEN | RSSILIEN));
  //
  //  SAME Interrupt Sources.
  //
  // Radio.setProperty(WB_SAME_INTERRUPT_SOURCE, (EOMDETIEN | HDRRDYIEN));
  //
  //  ASQ Interrupt Sources.
  //
  // Radio.setProperty(WB_ASQ_INT_SOURCE, (ALERTOFIEN | ALERTONIEN));

  // Apply EEPROM settings
  applyConfig();

  // Setup Other Stuff

}

void loop()
{
  if (intStatus & INTAVL)
    getStatus();

  if (Serial.available() > 0)
    getFunction();
}

void errorLoop()
{
  pinMode(ERROR_PIN, OUTPUT);
  digitalWrite(ERROR_PIN, LOW);
  // Blink forever
  while (true)
  {
    delay(1000);
    blink(ERROR_PIN, 250);
    blink(ERROR_PIN, 250);
  }
}

void blink(int pin, int blinkDelay) {
  digitalWrite(pin, HIGH);
  delay(blinkDelay);
  digitalWrite(pin, LOW);
  delay(blinkDelay);
}