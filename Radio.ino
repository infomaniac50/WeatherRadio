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
    printf_P(PSTR("FREQ: %.3f  RSSI: %u  SNR: %u\n"), frequency, rssi, snr);
    Radio.sameFlush();             //  This should be done after any tune function.
    //intStatus |= RSQINT;         //  We can force it to get rsqStatus on any tune.
  }

  if (intStatus & RSQINT)
  {
    Radio.getRsqStatus(INTACK);
    printf_P(PSTR("RSSI: %u  SNR: %u  FREQOFF: %u\n"), rssi, snr, freqoff);
  }

  if (intStatus & SAMEINT)
  {
    Radio.getSameStatus(INTACK);

    if (sameStatus & EOMDET)
    {
      Radio.sameFlush();
      printf_P(PSTR("EOM detected.\n"));
      //  More application specific code could go here. (Mute audio, turn something on/off, etc.)
      return;
    }

    if (msgStatus & MSGAVL && (!(msgStatus & MSGUSD)))  // If a message is available and not already used,
      Radio.sameParse();                                // parse it.

    if (msgStatus & MSGPAR)
    {
      msgStatus &= ~MSGPAR;                         // Clear the parse status, so that we don't print it again.

      printf_P(PSTR("Originator: %.4s\nEvent: %.4s\nLocations: %u\nLocation Codes: "), sameOriginatorName, sameEventName, sameLocations);

      for (int i = 0; i < sameLocations; i++)
      {
        printf_P("%lu ", sameLocationCodes[i]);
      }

      printf_P(PSTR("Duration: %u Day: %u Time: %u\nCallsign: %.9s\n"), sameDuration, sameDay, sameTime, sameCallSign);
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
      printf_P(PSTR("WAT is on.\n"));
      //  More application specific code could go here.  (Unmute audio, turn something on/off, etc.)
    }

    if (asqStatus == 0x02)
    {
      printf_P(PSTR("WAT is off.\n"));
      //  More application specific code could go here.  (Mute audio, turn something on/off, etc.)
    }

    sameWat = asqStatus;
  }

  if (intStatus & ERRINT)
  {
    intStatus &= ~ERRINT;
    printf_P(PSTR("An error occured!\n\n"));
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
    printf_P(PSTR("Channel down.\n"));
    break;

  case 'u':
    if (!tuneUp())
      break;
    printf_P(PSTR("Channel up.\n"));
    break;

  case 's':
    printf_P(PSTR("Scanning.....\n"));
    Radio.scan();
    break;

  case '-':
    if (!volumeDown())
      break;
    printf_P(PSTR("Volume: %u\n"), volume);
    break;

  case '+':
    if (!volumeUp())
      break;
    printf_P(PSTR("Volume: %u\n"), volume);
    break;

  case 'm':
    if (mute)
    {
      Radio.setMute(OFF);
      printf_P(PSTR("Mute: Off\n"));
      break;
    }
    else
    {
      Radio.setMute(ON);
      printf_P(PSTR("Mute: On\n"));
      break;
    }

  case 'o':
    if (power)
    {
      Radio.off();
      printf_P(PSTR("Radio powered off.\n"));
      break;
    }
    else
    {
      Radio.on();
      printf_P(PSTR("Radio powered on.\n"));
      Radio.tune();
      break;
    }
  case 'e':
    saveConfig();
    printf_P(PSTR("Config saved.\n"));
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
  printf_P(PSTR("\nDisplay this menu =\t 'h' or '?'\nChannel down =\t\t 'd'\nChannel up =\t\t 'u'\nScan =\t\t\t 's'\nVolume - =\t\t '-'\nVolume + =\t\t '+'\nMute / Unmute =\t\t 'm'\nOn / Off =\t\t 'o'\nSave Settings = \t 'e'\n\n"));
}
//
//  Simple Hex print utility - Prints a Byte with a leading zero and trailing space.
//
void printHex(byte value)
{
  printf_P(PSTR("0x%X "), value);
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