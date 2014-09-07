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
#include <Flash.h>

#include "Strings.h"

#include <EEPROMex.h>

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SI4707.h>
#include <SPI.h>
#include <SD.h>

#include "Settings.h"

#define ERROR_PIN 7
#define STATUS_PIN 8

#define stdout Serial

// SD Card Globals
File logFile;

Settings settings;

// Si4707 Globals
byte function = 0x00;           //  Function to be performed.

void setup()
{
  // On recent versions of Arduino the LED pin likes to turn on for no apparent reason
  // pinMode(13, OUTPUT);
  // but don't let the SD card clock be pushed or pulled by this pin
  // we are driving SCK with the Mega hardware SPI so just go high-z
  // no status pin though :(
  pinMode(13, INPUT);
  //but thats ok ;)
  pinMode(STATUS_PIN, OUTPUT);
  pinMode(ERROR_PIN, OUTPUT);

  Serial.begin(115200);

  settings.loadConfig();
  if (!settings.checkVersion())
  {
    settings.setDefaults();
    stdout << DEFAULTS_SET_LABEL;
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
  if (!SD.begin(10))
    errorLoop();

  logFile = SD.open("log.txt", FILE_WRITE);

  if (!logFile)
    errorLoop();

  stdout << STARTING_SI4707_LABEL;
  delay(100);

  // Setup Radio
  Radio.begin(22);
  Radio.patch();          //  Use this one to to include the 1050 Hz patch.
  //Radio.on();           //  Use this one if not using the patch.
  Radio.getRevision();  //  Only captured on the logic analyzer - not displayed.
  // The hell it ain't :p
  uint8_t pn = response[1];
  uint8_t fwmajor = response[2];
  uint8_t fwminor = response[3];
  uint8_t patchH = response[4];
  uint8_t patchL = response[5];
  uint8_t cmpmajor = response[6];
  uint8_t cmpminor = response[7];
  uint8_t chiprev = response[8];

  stdout << VERSION_INFO_LABEL
  << PART_NUMBER_LABEL << pn << endl
  << FIRMWARE_MAJOR_LABEL << fwmajor << endl
  << FIRMWARE_MINOR_LABEL << fwminor << endl
  << PATCH_HIGH_LABEL << patchH << endl
  << PATCH_LOW_LABEL << patchL << endl
  << COMPONENT_MAJOR_LABEL << cmpmajor << endl
  << COMPONENT_MINOR_LABEL << cmpminor << endl
  << CHIP_REVISION_LABEL << chiprev << endl << endl;

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

  //
  //  Tune to the desired frequency.
  //
  Radio.setVolume(volume);
  Radio.tune();  //  6 digits only.


  // Setup Other Stuff

  showMenu();
}

void loop()
{
  if (intStatus & INTAVL)
  {
    blink(ERROR_PIN, 50);
    getStatus();
    blink(STATUS_PIN, 25);
    blink(STATUS_PIN, 25);
  }

  if (Serial.available()) {
    blink(STATUS_PIN, 25);
    getFunction();
  }
}

//
//  Functions are performed here.
//
inline void getFunction()
{
  function = Serial.read();

  switch (function)
  {
  case 'h':
  case '?':
    showMenu();
    break;

  case 'd':
    if (tuneDown()) printChannelDown();
    break;

  case 'u':
    if (tuneUp()) printChannelUp();
    break;

  case 's':
    printScanning();
    Radio.scan();
    break;

  case '-':
    if (volumeDown()) printVolume();
    break;

  case '+':
    if (volumeUp()) printVolume();
    break;

  case 'm':
    toggleMute();
    break;

  case 'o':
    togglePower();
    break;

  case 'e':
    settings.saveConfig();
    printConfigSaved();
    break;
  default:
    blink(ERROR_PIN, 25);
    break;
  }

  blink(STATUS_PIN, 25);

  Serial.flush();
  function = 0x00;
}
//
//  The End.
//


inline void errorLoop()
{
  // Blink forever
  while (true)
  {
    delay(1000);
    blink(ERROR_PIN, 250);
    blink(ERROR_PIN, 250);
  }
}

inline void blink(int pin, int blinkDelay) {
  digitalWrite(pin, HIGH);
  delay(blinkDelay);
  digitalWrite(pin, LOW);
  delay(blinkDelay);
}
