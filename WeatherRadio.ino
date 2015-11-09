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
// #include <Streaming.h>
// #include <Flash.h>

#include <EEPROMex.h>

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SI4707.h>
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>

#include "Settings.h"
#include "Logging.h"
#include "Message.h"

#define ERROR_PIN 7
#define STATUS_PIN 8
#define SD_PIN 10

byte intStatusCopy;

Settings settings;
Logging logging;
File logFile;
PrintEx serial = Serial;

bool log_begin()
{
  if (!logging.begin())
    return false;

  // Now using the good old Adafruit Data Logging Shield
  if (!SD.begin(SD_PIN))
    return false;
  logFile = SD.open("log.txt", FILE_WRITE);

  if (!logFile)
    return false;
  return true;
}

void log_end(void)
{
  logging.end();
  logFile.flush();
  logFile.close();
}

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

  Serial.begin(9600);

  // Setup Other Stuff
  if (!log_begin()) {
    errorLoop();
  }

  settings.loadConfig();
  if (!settings.checkVersion())
  {
    settings.setDefaults();
    serial.printf("%p\n", DEFAULTS_SET_LABEL);
  }

  serial.printf("%p\n", STARTING_SI4707_LABEL);

  delay(100);

  // Setup Radio
  Radio.begin(22);

  togglePower();
  serial.printf("%p\n", MENU);
}

void loop()
{
  // Atomic copy
  intStatusCopy = intStatus;

  // If all that came back after we fetch the status bits are CTSINT and INTAVL
  // Clear to Send
  // Interrupt Available
  if (intStatusCopy == (CTSINT | INTAVL)) {
    // Clear the whole shebang so we dont check again until the next interrupt.
    // Defined as uint8_t
    intStatusCopy = 0;
  }

  if (intStatusCopy & INTAVL) {
    blink(ERROR_PIN, 25);
    Radio.getIntStatus();

    if (intStatusCopy & STCINT)
    {
      Radio.getTuneStatus(INTACK);  //  Using INTACK clears STCINT, CHECK preserves it.
      logging.printFrequencyTo(Serial);
      logging.printRssiTo(Serial);
      logging.printSnrTo(Serial);
      logging.printFrequencyTo(logFile);
      Radio.sameFlush();             //  This should be done after any tune function.
      //intStatusCopy |= RSQINT;         //  We can force it to get rsqStatus on any tune.
    }

    if (intStatusCopy & RSQINT)
    {
      Radio.getRsqStatus(INTACK);
      logging.printRssiTo(Serial);
      logging.printSnrTo(Serial);
      logging.printFrequencyOffsetTo(Serial);
    }

    if (intStatusCopy & SAMEINT)
    {
      Radio.getSameStatus(INTACK);

      if (sameStatus & EOMDET)
      {
        Radio.sameFlush();
        logging.printEomTo(Serial);
        logging.printEomTo(logFile);
        //  More application specific code could go here. (Mute audio, turn something on/off, etc.)
        return;
      }

      if (msgStatus & MSGAVL && (!(msgStatus & MSGUSD)))  // If a message is available and not already used,
        Radio.sameParse();                                // parse it.

      if (msgStatus & MSGPAR)
      {
        msgStatus &= ~MSGPAR;                         // Clear the parse status, so that we don't logging.print it again.
        logging.printSameMessageTo(Serial);
        logging.printSameMessageTo(logFile);
      }

      if (msgStatus & MSGPUR)  //  Signals that the third header has been received.
        Radio.sameFlush();
    }

    if (intStatusCopy & ASQINT)
    {
      Radio.getAsqStatus(INTACK);

      if (sameWat == asqStatus)
        return;

      if (asqStatus == 0x01)
      {
        Radio.sameFlush();
        serial.printf("%p\n", WAT_ON_LABEL);
        //  More application specific code could go here.  (Unmute audio, turn something on/off, etc.)
      }

      if (asqStatus == 0x02)
      {
        serial.printf("%p\n", WAT_OFF_LABEL);
        //  More application specific code could go here.  (Mute audio, turn something on/off, etc.)
      }

      sameWat = asqStatus;
    }

    if (intStatusCopy & ERRINT)
    {
      intStatusCopy &= ~ERRINT;
      serial.printf("%p\n", ERROR_OCCURRED_LABEL);
      logFile.println(ERROR_OCCURRED_LABEL);
    }
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
      serial.printf("%p\n", MENU);
      break;

    case 'd':
      if (tuneDown()) serial.printf("%p\n", CHANNEL_DOWN_LABEL);
      break;

    case 'u':
      if (tuneUp()) serial.printf("%p\n", CHANNEL_UP_LABEL);
      break;

    case 's':
      serial.printf("%p\n", SCANNING_LABEL);
      Radio.scan();
      break;

    case '-':
      if (volumeDown()) logging.printVolumeTo(Serial);
      break;

    case '+':
      if (volumeUp()) logging.printVolumeTo(Serial);
      break;

    case 'm':
      toggleMute();
      break;

    case 'o':
      togglePower();
      break;

    case 'e':
      settings.saveConfig();
      serial.printf("%p\n", CONFIG_SAVED_LABEL);
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


inline void toggleMute(void) {
  if (mute)
  {
    Radio.setMute(OFF);
    serial.printf("%p\n", MUTE_OFF_LABEL);
  }
  else
  {
    Radio.setMute(ON);
    serial.printf("%p\n", MUTE_ON_LABEL);
  }
}

inline void togglePower(void) {
  if (power)
  {
    Radio.off();
    serial.printf("%p\n", RADIO_OFF_LABEL);
    logging.end();
  }
  else
  {
    logging.begin();
    Radio.patch();          //  Use this one to to include the 1050 Hz patch.
    //Radio.on();           //  Use this one if not using the patch.
    logging.printRadioVersionTo(Serial);
    //
    //  All useful interrupts are enabled here.
    //
    // Disable CTSIEN as it was getting to be a pain.
    // CTSI assert -> getStatus() -> Si CMD -> CTSI assert -> forever and ever and ever ..........
    // I believe the Si4707 library already delays a proper amount of time after each command.
    // Radio.setProperty(GPO_IEN, (CTSIEN | ERRIEN | RSQIEN | SAMEIEN | ASQIEN | STCIEN));
    Radio.setProperty(GPO_IEN, (ERRIEN | RSQIEN | SAMEIEN | ASQIEN | STCIEN));
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

    serial.printf("%p\n", RADIO_ON_LABEL);
  }
}

inline bool tuneUp() {
  if (channel >= WB_MAX_FREQUENCY)
    return false;

  channel += WB_CHANNEL_SPACING;
  Radio.tune();
  return true;
}

inline bool tuneDown() {
  if (channel <= WB_MIN_FREQUENCY)
    return false;

  channel -= WB_CHANNEL_SPACING;
  Radio.tune();
  return true;
}

inline bool volumeUp() {
  return volumeUp(1);
}

inline bool volumeUp(int increment) {
  if (volume >= 0x003F)
    return false;
  volume += increment;
  constrain(volume, 0x0000, 0x003F);
  Radio.setVolume(volume);
  return true;
}

inline bool volumeDown() {
  return volumeDown(1);
}

inline bool volumeDown(int increment) {
  if (volume <= 0x0000)
    return false;
  volume -= increment;
  constrain(volume, 0x0000, 0x003F);
  Radio.setVolume(volume);
  return true;
}
