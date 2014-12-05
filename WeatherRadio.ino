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

#include <EEPROMex.h>

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SI4707.h>
#include <SPI.h>
#include <SD.h>

#include "Settings.h"
#include "Logging.h"

#define ERROR_PIN 7
#define STATUS_PIN 8


Settings settings;
Logging logging;

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

  // Setup SD Card
  // Set hardware SS pin as output
  // Uno SS pin = 10
  // Mega SS pin = 53
  // SS is defined in the pins_arduino.h file
  pinMode(SS, OUTPUT);

  Serial.begin(115200);

  // Setup Other Stuff
  if (!logging.begin())
    errorLoop();

  settings.loadConfig();
  if (!settings.checkVersion())
  {
    settings.setDefaults();
    logging.printDefaultsSet();
  }

  logging.printBooting();
  delay(100);

  // Setup Radio
  Radio.begin(22);

  togglePower();
  logging.showMenu();
}

void loop()
{
  // If all that came back after we fetch the status bits are CTSINT and INTAVL
  if (intStatus == (CTSINT | INTAVL)) {
    // Clear the whole shebang so we dont check again until the next interrupt.
    // Defined as uint8_t so no half reads/writes and is atomic except for the conditional.
    // On a rare occasion the ISR could set INTAVL right here.
    // Then it would be cleared here.
    intStatus = 0;
  }

  if (intStatus & INTAVL) {
    blink(ERROR_PIN, 25);
    getStatus();
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
  char function = Serial.read();

  switch (function)
  {
  case 'h':
  case '?':
    logging.showMenu();
    break;

  case 'd':
    if (tuneDown()) logging.printChannelDown();
    break;

  case 'u':
    if (tuneUp()) logging.printChannelUp();
    break;

  case 's':
    logging.printScanning();
    Radio.scan();
    break;

  case '-':
    if (volumeDown()) logging.printVolume();
    break;

  case '+':
    if (volumeUp()) logging.printVolume();
    break;

  case 'm':
    toggleMute();
    break;

  case 'o':
    togglePower();
    break;

  case 'e':
    settings.saveConfig();
    logging.printConfigSaved();
    break;
  default:
    blink(ERROR_PIN, 25);
    break;
  }

  blink(STATUS_PIN, 25);
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
