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
    Serial.print(F("FREQ: "));
    Serial.print(frequency, 3);
    Serial.print(F("  RSSI: "));
    Serial.print(rssi);
    Serial.print(F("  SNR: "));
    Serial.println(snr);
    Radio.sameFlush();             //  This should be done after any tune function.
    //intStatus |= RSQINT;         //  We can force it to get rsqStatus on any tune.
  }

  if (intStatus & RSQINT)
  {
    Radio.getRsqStatus(INTACK);
    Serial.print(F("RSSI: "));
    Serial.print(rssi);
    Serial.print(F("  SNR: "));
    Serial.print(snr);
    Serial.print(F("  FREQOFF: "));
    Serial.println(freqoff);
  }

  if (intStatus & SAMEINT)
  {
    Radio.getSameStatus(INTACK);

    if (sameStatus & EOMDET)
    {
      Radio.sameFlush();
      Serial.println(F("EOM detected."));
      Serial.println();
      //  More application specific code could go here. (Mute audio, turn something on/off, etc.)
      return;
    }

    if (msgStatus & MSGAVL && (!(msgStatus & MSGUSD)))  // If a message is available and not already used,
      Radio.sameParse();                                // parse it.

    if (msgStatus & MSGPAR)
    {
      msgStatus &= ~MSGPAR;                         // Clear the parse status, so that we don't print it again.
      Serial.print(F("Originator: "));
      Serial.println(sameOriginatorName);
      Serial.print(F("Event: "));
      Serial.println(sameEventName);
      Serial.print(F("Locations: "));
      Serial.println(sameLocations);
      Serial.print(F("Location Codes: "));

      for (int i = 0; i < sameLocations; i++)
      {
        Serial.print(sameLocationCodes[i]);
        Serial.print(' ');
      }

      Serial.println();
      Serial.print(F("Duration: "));
      Serial.println(sameDuration);
      Serial.print(F("Day: "));
      Serial.println(sameDay);
      Serial.print(F("Time: "));
      Serial.println(sameTime);
      Serial.print(F("Callsign: "));
      Serial.println(sameCallSign);
      Serial.println();
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
      Serial.println(F("WAT is on."));
      Serial.println();
      //  More application specific code could go here.  (Unmute audio, turn something on/off, etc.)
    }

    if (asqStatus == 0x02)
    {
      Serial.println(F("WAT is off."));
      Serial.println();
      //  More application specific code could go here.  (Mute audio, turn something on/off, etc.)
    }

    sameWat = asqStatus;
  }

  if (intStatus & ERRINT)
  {
    intStatus &= ~ERRINT;
    Serial.println(F("An error occured!"));
    Serial.println();
  }
}
//
//  Functions are performed here.
//
void getFunction()
{
  function = Serial.read();

  switch (function)
  {
  case 'h':
  case '?':
    showMenu();
    break;

  case 'd':
    if (!tuneDown())
      break;
    Serial.println(F("Channel down."));
    break;

  case 'u':
    if (!tuneUp())
      break;
    Serial.println(F("Channel up."));
    break;

  case 's':
    Serial.println(F("Scanning....."));
    Radio.scan();
    break;

  case '-':
    if (!volumeDown())
      break;
    Serial.print(F("Volume: "));
    Serial.println(volume);
    break;

  case '+':
    if (!volumeUp())
      break;
    Serial.print(F("Volume: "));
    Serial.println(volume);
    break;

  case 'm':
    if (mute)
    {
      Radio.setMute(OFF);
      Serial.println(F("Mute: Off"));
      break;
    }
    else
    {
      Radio.setMute(ON);
      Serial.println(F("Mute: On"));
      break;
    }

  case 'o':
    if (power)
    {
      Radio.off();
      Serial.println(F("Radio powered off."));
      break;
    }
    else
    {
      Radio.on();
      Serial.println(F("Radio powered on."));
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
  Serial.println();
  Serial.println(F("Display this menu =\t 'h' or '?'"));
  Serial.println(F("Channel down =\t\t 'd'"));
  Serial.println(F("Channel up =\t\t 'u'"));
  Serial.println(F("Scan =\t\t\t 's'"));
  Serial.println(F("Volume - =\t\t '-'"));
  Serial.println(F("Volume + =\t\t '+'"));
  Serial.println(F("Mute / Unmute =\t\t 'm'"));
  Serial.println(F("On / Off =\t\t 'o'"));
  Serial.println(F("Save Settings = \t 'e'"));
  Serial.println();
}
//
//  Simple Hex print utility - Prints a Byte with a leading zero and trailing space.
//
void printHex(byte value)
{
  Serial.print(F("0x"));
  Serial.print(value >> 4 & 0x0F, HEX);
  Serial.print(value >> 0 & 0x0F, HEX);
  Serial.print("  ");
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