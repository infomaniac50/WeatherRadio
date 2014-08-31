/*
 shamelessly copied from:
 Si4707 Basic Demonstration Program.
 https://github.com/tomn46037/Si4707/blob/master/examples/SAME/Si4707/Si4707.ino
 16 JUN 2013
*/

inline void getStatus()
{
  Radio.getIntStatus();

  if (intStatus & STCINT)
  {
    Radio.getTuneStatus(INTACK);  //  Using INTACK clears STCINT, CHECK preserves it.
    printFrequency();
    printRssi();
    printSnr();
    Radio.sameFlush();             //  This should be done after any tune function.
    //intStatus |= RSQINT;         //  We can force it to get rsqStatus on any tune.
  }

  if (intStatus & RSQINT)
  {
    Radio.getRsqStatus(INTACK);
    printRssi();
    printSnr();
    printFrequencyOffset();
  }

  if (intStatus & SAMEINT)
  {
    Radio.getSameStatus(INTACK);

    if (sameStatus & EOMDET)
    {
      Radio.sameFlush();
      printEom();
      //  More application specific code could go here. (Mute audio, turn something on/off, etc.)
      return;
    }

    if (msgStatus & MSGAVL && (!(msgStatus & MSGUSD)))  // If a message is available and not already used,
      Radio.sameParse();                                // parse it.

    if (msgStatus & MSGPAR)
    {
      msgStatus &= ~MSGPAR;                         // Clear the parse status, so that we don't print it again.
      printSameMessage();
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
      printWatOn();
      //  More application specific code could go here.  (Unmute audio, turn something on/off, etc.)
    }

    if (asqStatus == 0x02)
    {
      printWatOff();
      //  More application specific code could go here.  (Mute audio, turn something on/off, etc.)
    }

    sameWat = asqStatus;
  }

  if (intStatus & ERRINT)
  {
    intStatus &= ~ERRINT;
    printErrorOccurred();
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
    saveConfig();
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

inline void toggleMute(void) {
  if (mute)
  {
    Radio.setMute(OFF);
    printMuteOff();
  }
  else
  {
    Radio.setMute(ON);
    printMuteOn();
  }
}

inline void togglePower(void) {
  if (power)
  {
    Radio.off();
    printRadioOff();
  }
  else
  {
    Radio.on();
    printRadioOn();
    Radio.tune();
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
