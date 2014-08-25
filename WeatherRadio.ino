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
#include <Streaming.h>

#include <EEPROMex.h>

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SI4707.h>
#include <SPI.h>
#include <SD.h>

#define ERROR_PIN 7
#define STATUS_PIN 8

// 20 + 1 characters 
#define VERSION __DATE__ " " __TIME__
// EEPROM Globals
// #define CONFIG_VERSION "wr1"
const char CONFIG_VERSION[21] PROGMEM = VERSION;
#define MEMORY_BASE 32

#define stdout Serial

int configAddress = 0;

struct StoreStruct
{
  char version[21];
  unsigned long frequency;
  unsigned char volume;
} storage;

// SD Card Globals
File logFile;

// Si4707 Globals
byte function = 0x00;           //  Function to be performed.

void setup()
{
  // On recent versions of Arduino the LED pin likes to turn on for no apparent reason
  pinMode(13, OUTPUT);
  pinMode(STATUS_PIN, OUTPUT);
  pinMode(ERROR_PIN, OUTPUT);

  Serial.begin(115200);

  // Setup EEPROM with defaults
  EEPROM.setMemPool(MEMORY_BASE, EEPROMSizeMega);
  configAddress = EEPROM.getAddress(sizeof(StoreStruct));

  if (!loadConfig())
  {
    setDefaults();
    stdout << F("Defaults set.\n");
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

  // Now using the good old Adafruit Data Logging Shield
  if (!SD.begin(SS))
    errorLoop();

  // sprintf_P(filename, name_format, tm.Month, tm.Day, tm.Year + 1970, tm.Hour, tm.Minute, tm.Second);
  // logFile = SD.open(filename, FILE_WRITE);
  logFile = SD.open("log.txt", FILE_WRITE);

  if (!logFile)
    errorLoop();

  stdout << F("Starting up the Si4707.......\n\n");
  delay(100);

  // Setup Radio
  Radio.begin(22);
  Radio.patch();          //  Use this one to to include the 1050 Hz patch.
  //Radio.on();           //  Use this one if not using the patch.
  Radio.getRevision();  //  Only captured on the logic analyzer - not displayed.
  uint8_t pn = response[1];
  uint8_t fwmajor = response[2];
  uint8_t fwminor = response[3];
  uint8_t patchH = response[4];
  uint8_t patchL = response[5];
  uint8_t cmpmajor = response[6];
  uint8_t cmpminor = response[7];
  uint8_t chiprev = response[8];

  stdout << F("Version Information\n")
  << F("PN: ") << pn << endl
  << F("FW Major: ") << fwmajor << endl
  << F("FW Minor: ") << fwminor << endl
  << F("PatchH: ") << patchH << endl
  << F("PatchL: ") << patchL << endl
  << F("CMP Major: ") << cmpmajor << endl
  << F("CMP Minor: ") << cmpminor << endl
  << F("Chip Rev: ") << chiprev << endl << endl;

  //
  //  All useful interrupts are enabled here.
  //
  Radio.setProperty(GPO_IEN, (CTSIEN | ERRIEN | RSQIEN | SAMEIEN | ASQIEN | STCIEN));
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
  Radio.setProperty(WB_SAME_INTERRUPT_SOURCE, (EOMDETIEN | HDRRDYIEN));
  //
  //  ASQ Interrupt Sources.
  //
  Radio.setProperty(WB_ASQ_INT_SOURCE, (ALERTOFIEN | ALERTONIEN));

  // Apply EEPROM settings
  applyConfig();

  // Setup Other Stuff

  showMenu();
}

void loop()
{
  if (intStatus & INTAVL)
    getStatus();

  if (!Serial.available())
    getFunction();
}

void errorLoop()
{
  digitalWrite(ERROR_PIN, LOW);
  // Blink forever
  while (true)
  {
    delay(1000);
    blink(ERROR_PIN, 250);
    blink(ERROR_PIN, 250);
  }
}

void statusLoop()
{
  digitalWrite(STATUS_PIN, LOW);
  // Blink forever
  while (true)
  {
    delay(1000);
    blink(STATUS_PIN, 250);
    blink(STATUS_PIN, 250);
  }
}

void blink(int pin, int blinkDelay) {
  digitalWrite(pin, HIGH);
  delay(blinkDelay);
  digitalWrite(pin, LOW);
  delay(blinkDelay);
}