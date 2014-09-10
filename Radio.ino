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
    logging.printFrequency();
    logging.printRssi();
    logging.printSnr();
    logging.logFrequency();
    Radio.sameFlush();             //  This should be done after any tune function.
    //intStatus |= RSQINT;         //  We can force it to get rsqStatus on any tune.
  }

  if (intStatus & RSQINT)
  {
    Radio.getRsqStatus(INTACK);
    logging.printRssi();
    logging.printSnr();
    logging.printFrequencyOffset();
  }

  if (intStatus & SAMEINT)
  {
    Radio.getSameStatus(INTACK);

    if (sameStatus & EOMDET)
    {
      Radio.sameFlush();
      logging.printEom();
      logging.logEom();
      //  More application specific code could go here. (Mute audio, turn something on/off, etc.)
      return;
    }

    if (msgStatus & MSGAVL && (!(msgStatus & MSGUSD)))  // If a message is available and not already used,
      Radio.sameParse();                                // parse it.

    if (msgStatus & MSGPAR)
    {
      msgStatus &= ~MSGPAR;                         // Clear the parse status, so that we don't logging.print it again.
      logging.printSameMessage();
      logging.logSameMessage();
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
      logging.printWatOn();
      //  More application specific code could go here.  (Unmute audio, turn something on/off, etc.)
    }

    if (asqStatus == 0x02)
    {
      logging.printWatOff();
      //  More application specific code could go here.  (Mute audio, turn something on/off, etc.)
    }

    sameWat = asqStatus;
  }

  if (intStatus & ERRINT)
  {
    intStatus &= ~ERRINT;
    logging.printErrorOccurred();
    logging.logErrorOccurred();
  }
}

inline void toggleMute(void) {
  if (mute)
  {
    Radio.setMute(OFF);
    logging.printMuteOff();
  }
  else
  {
    Radio.setMute(ON);
    logging.printMuteOn();
  }
}

inline void togglePower(void) {
  if (power)
  {
    Radio.off();
    logging.printRadioOff();
    logging.end();
  }
  else
  {
    logging.begin();
    Radio.patch();          //  Use this one to to include the 1050 Hz patch.
    //Radio.on();           //  Use this one if not using the patch.
    logging.printRadioVersion();
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

    logging.printRadioOn();
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
