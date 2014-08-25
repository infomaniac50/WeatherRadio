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
    stdout << F("FREQ: ") << _FLOAT(frequency, 3);
    stdout << F("RSSI: ") << rssi;
    stdout << F("SNR: ") << snr << endl;

    Radio.sameFlush();             //  This should be done after any tune function.
    //intStatus |= RSQINT;         //  We can force it to get rsqStatus on any tune.
  }

  if (intStatus & RSQINT)
  {
    Radio.getRsqStatus(INTACK);
    stdout << F("RSSI: ") << rssi;
    stdout << F("SNR: ") << snr;
    stdout << F("FREQOFF: ") << freqoff << endl;
  }

  if (intStatus & SAMEINT)
  {
    Radio.getSameStatus(INTACK);

    if (sameStatus & EOMDET)
    {
      Radio.sameFlush();
      stdout << F("EOM detected.") << endl;
      //  More application specific code could go here. (Mute audio, turn something on/off, etc.)
      return;
    }

    if (msgStatus & MSGAVL && (!(msgStatus & MSGUSD)))  // If a message is available and not already used,
      Radio.sameParse();                                // parse it.

    if (msgStatus & MSGPAR)
    {
      msgStatus &= ~MSGPAR;                         // Clear the parse status, so that we don't print it again.

      stdout << F("Originator: ") << sameOriginatorName << endl;
      stdout << F("Event: ") << sameEventName << endl;
      stdout << F("Locations: ") << sameLocations << endl;
      
      stdout << F("Location Codes: ") << endl;

      for (int i = 0; i < sameLocations; i++)
      {
        stdout << sameLocationCodes[i] << endl;
      }

      stdout << F("Duration: ") << sameDuration;
      stdout << F("Day: ") << sameDay;
      stdout << F("Time: ") << sameTime << endl;
      stdout << F("Callsign: ") << sameCallSign << endl;
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
      stdout << F("WAT is on.") << endl;
      //  More application specific code could go here.  (Unmute audio, turn something on/off, etc.)
    }

    if (asqStatus == 0x02)
    {
      stdout << F("WAT is off.") << endl;
      //  More application specific code could go here.  (Mute audio, turn something on/off, etc.)
    }

    sameWat = asqStatus;
  }

  if (intStatus & ERRINT)
  {
    intStatus &= ~ERRINT;
    stdout << F("An error occured!") << endl << endl;
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
    stdout << F("Channel down.") << endl;
    break;

  case 'u':
    if (!tuneUp())
      break;
    stdout << F("Channel up.") << endl;
    break;

  case 's':
    stdout << F("Scanning.....") << endl;
    Radio.scan();
    break;

  case '-':
    if (!volumeDown())
      break;
    stdout << F("Volume: ") << volume << endl;
    break;

  case '+':
    if (!volumeUp())
      break;
    stdout << F("Volume: ") << volume << endl;
    break;

  case 'm':
    if (mute)
    {
      Radio.setMute(OFF);
      stdout << F("Mute: Off") << endl;
      break;
    }
    else
    {
      Radio.setMute(ON);
      stdout << F("Mute: On") << endl;
      break;
    }

  case 'o':
    if (power)
    {
      Radio.off();
      stdout << F("Radio powered off.") << endl;
      break;
    }
    else
    {
      Radio.on();
      stdout << F("Radio powered on.") << endl;
      Radio.tune();
      break;
    }
  case 'e':
    saveConfig();
    stdout << F("Config saved.") << endl;
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
  stdout << F("Display this menu =\t 'h' or '?'") << endl;
  stdout << F("Channel down =\t\t 'd'") << endl;
  stdout << F("Channel up =\t\t 'u'") << endl;
  stdout << F("Scan =\t\t\t 's'") << endl;
  stdout << F("Volume - =\t\t '-'") << endl;
  stdout << F("Volume + =\t\t '+'") << endl;
  stdout << F("Mute / Unmute =\t\t 'm'") << endl;
  stdout << F("On / Off =\t\t 'o'") << endl;
  stdout << F("Save Settings = \t 'e'") << endl << endl;
  
}
//
//  Simple Hex print utility - Prints a Byte with a leading zero and trailing space.
//
void printHex(byte value)
{
  stdout << F("0x") << _HEX(value) << " ";
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