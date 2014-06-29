/*
 shamelessly copied from:
 Si4707 Basic Demonstration Program.
 https://github.com/tomn46037/Si4707/blob/master/examples/SAME/Si4707/Si4707.ino
 16 JUN 2013
*/
void getStatus()
{
  Radio.getIntStatus();

  if (intStatus & STCINT)
  {
    Radio.getTuneStatus(INTACK);  //  Using INTACK clears STCINT, CHECK preserves it.
    MENU_PRINT(F("FREQ: "));
    MENU_PRINT(frequency, 3);
    MENU_PRINT(F("  RSSI: "));
    MENU_PRINT(rssi);
    MENU_PRINT(F("  SNR: "));
    MENU_PRINTLN(snr);
    Radio.sameFlush();             //  This should be done after any tune function.
    //intStatus |= RSQINT;         //  We can force it to get rsqStatus on any tune.
  }

  if (intStatus & RSQINT)
  {
    Radio.getRsqStatus(INTACK);
    MENU_PRINT(F("RSSI: "));
    MENU_PRINT(rssi);
    MENU_PRINT(F("  SNR: "));
    MENU_PRINT(snr);
    MENU_PRINT(F("  FREQOFF: "));
    MENU_PRINTLN(freqoff);
  }

  if (intStatus & SAMEINT)
  {
    Radio.getSameStatus(INTACK);

    if (sameStatus & EOMDET)
    {
      Radio.sameFlush();
      MENU_PRINTLN(F("EOM detected."));
      MENU_PRINTLN();
      //  More application specific code could go here. (Mute audio, turn something on/off, etc.)
      return;
    }

    if (msgStatus & MSGAVL && (!(msgStatus & MSGUSD)))  // If a message is available and not already used,
      Radio.sameParse();                                // parse it.

    if (msgStatus & MSGPAR)
    {
      msgStatus &= ~MSGPAR;                         // Clear the parse status, so that we don't print it again.
      MENU_PRINT(F("Originator: "));
      MENU_PRINTLN(sameOriginatorName);
      MENU_PRINT(F("Event: "));
      MENU_PRINTLN(sameEventName);
      MENU_PRINT(F("Locations: "));
      MENU_PRINTLN(sameLocations);
      MENU_PRINT(F("Location Codes: "));

      for (int i = 0; i < sameLocations; i++)
      {
        MENU_PRINT(sameLocationCodes[i]);
        MENU_PRINT(' ');
      }

      MENU_PRINTLN();
      MENU_PRINT(F("Duration: "));
      MENU_PRINTLN(sameDuration);
      MENU_PRINT(F("Day: "));
      MENU_PRINTLN(sameDay);
      MENU_PRINT(F("Time: "));
      MENU_PRINTLN(sameTime);
      MENU_PRINT(F("Callsign: "));
      MENU_PRINTLN(sameCallSign);
      MENU_PRINTLN();
    }

    if (msgStatus & MSGPUR)  //  Signals that the third header has been received.
      Radio.sameFlush();
  }

  if (intStatus & ASQINT)
  {
    Radio.getAsqStatus(INTACK);

    if (sameWat == asqStatus)
      return;

    if (asqStatus == 0x01)
    {
      Radio.sameFlush();
      MENU_PRINTLN(F("WAT is on."));
      MENU_PRINTLN();
      //  More application specific code could go here.  (Unmute audio, turn something on/off, etc.)
    }

    if (asqStatus == 0x02)
    {
      MENU_PRINTLN(F("WAT is off."));
      MENU_PRINTLN();
      //  More application specific code could go here.  (Mute audio, turn something on/off, etc.)
    }

    sameWat = asqStatus;
  }

  if (intStatus & ERRINT)
  {
    intStatus &= ~ERRINT;
    MENU_PRINTLN(F("An error occured!"));
    MENU_PRINTLN();
  }
}
//
//  Functions are performed here.
//
void getFunction()
{
  function = MENU_READ();

  switch (function)
  {
  case 'h':
  case '?':
    showMenu();
    break;

  case 'd':
    if (!tuneDown())
      break;
    MENU_PRINTLN(F("Channel down."));
    break;

  case 'u':
    if (!tuneUp())
      break;
    MENU_PRINTLN(F("Channel up."));
    break;

  case 's':
    MENU_PRINTLN(F("Scanning....."));
    Radio.scan();
    break;

  case '-':
    if (!volumeDown())
      break;
    MENU_PRINT(F("Volume: "));
    MENU_PRINTLN(volume);
    break;

  case '+':
    if (!volumeUp())
      break;
    MENU_PRINT(F("Volume: "));
    MENU_PRINTLN(volume);
    break;

  case 'm':
    if (mute)
    {
      Radio.setMute(OFF);
      MENU_PRINTLN(F("Mute: Off"));
      break;
    }
    else
    {
      Radio.setMute(ON);
      MENU_PRINTLN(F("Mute: On"));
      break;
    }

  case 'o':
    if (power)
    {
      Radio.off();
      MENU_PRINTLN(F("Radio powered off."));
      break;
    }
    else
    {
      Radio.on();
      MENU_PRINTLN(F("Radio powered on."));
      Radio.tune();
      break;
    }
  case 'e':
    saveConfig();

  default:
    break;
  }

  Serial.flush();
  function = 0x00;
}
//
//  Prints the Function Menu.
//
void showMenu()
{
  MENU_PRINTLN();
  MENU_PRINTLN(F("Display this menu =\t 'h' or '?'"));
  MENU_PRINTLN(F("Channel down =\t\t 'd'"));
  MENU_PRINTLN(F("Channel up =\t\t 'u'"));
  MENU_PRINTLN(F("Scan =\t\t\t 's'"));
  MENU_PRINTLN(F("Volume - =\t\t '-'"));
  MENU_PRINTLN(F("Volume + =\t\t '+'"));
  MENU_PRINTLN(F("Mute / Unmute =\t\t 'm'"));
  MENU_PRINTLN(F("On / Off =\t\t 'o'"));
  MENU_PRINTLN(F("Save Settings = \t 'e'"));
  MENU_PRINTLN();
}
//
//  Simple Hex print utility - Prints a Byte with a leading zero and trailing space.
//
void printHex(byte value)
{
  MENU_PRINT(F("0x"));
  MENU_PRINT(value >> 4 & 0x0F, HEX);
  MENU_PRINT(value >> 0 & 0x0F, HEX);
  MENU_PRINT("  ");
}
//
//  The End.
//

bool tuneUp() {
  if (channel >= WB_MAX_FREQUENCY)
    return false;

  channel += WB_CHANNEL_SPACING;
  Radio.tune();
  return true;
}

bool tuneDown() {
  if (channel <= WB_MIN_FREQUENCY)
    return false;

    channel -= WB_CHANNEL_SPACING;
    Radio.tune();
    return true;
}

bool volumeUp() {
  return volumeUp(1);
}

bool volumeUp(int increment) {
  if (volume >= 0x003F)
    return false;
  volume += increment;
  constrain(volume, 0x0000, 0x003F);
  Radio.setVolume(volume);
  return true;
}

bool volumeDown() {
  return volumeDown(1);
}

bool volumeDown(int increment) {
  if (volume <= 0x0000)
    return false;
  volume -= increment;
  constrain(volume, 0x0000, 0x003F);
  Radio.setVolume(volume);
  return true;
}