
inline void printRadioOff(void)
{
  stdout << RADIO_OFF_LABEL;
}

inline void printRadioOn(void)
{
  stdout << RADIO_ON_LABEL;
}

inline void printConfigSaved(void)
{
  stdout << CONFIG_SAVED_LABEL;
}

inline void printFrequency(void)
{
  stdout << FREQ_LABEL << _FLOAT(frequency, 3) << endl;
}

inline void printRssi(void)
{
  stdout << RSSI_LABEL << rssi << endl;
}

inline void printSnr(void)
{
  stdout << SNR_LABEL << snr << endl;
}

inline void printFrequencyOffset(void)
{
  stdout << FREQOFF_LABEL << freqoff << endl;
}

inline void printEom(void)
{
  stdout << EOM_DETECTED;
}

inline void printWatOn(void)
{
  stdout << WAT_ON_LABEL;
}

inline void printWatOff(void)
{
  stdout << WAT_OFF_LABEL;
}

inline void printErrorOccurred(void)
{
  stdout << ERROR_OCCURRED_LABEL;
}

//
//  Simple Hex print utility - Prints a Byte with a leading zero and trailing space.
//
inline void printHex(byte value)
{
  stdout << "0x" << _HEX(value) << ' ';
}

//
//  Prints the Function Menu.
//
inline void showMenu(void)
{
  stdout << MENU;
}

inline void printChannelDown(void)
{
  stdout << CHANNEL_DOWN_LABEL;
}

inline void printChannelUp(void)
{
  stdout << CHANNEL_UP_LABEL;
}

inline void printScanning(void)
{
  stdout << SCANNING_LABEL;
}

inline void printVolume(void)
{
  stdout << VOLUME_LABEL << volume << endl;
}

inline void printMuteOff(void)
{
  stdout << MUTE_OFF_LABEL;
}

inline void printMuteOn(void)
{
  stdout << MUTE_ON_LABEL;
}

inline void printSameMessage(void)
{
  stdout << ORIGINATOR_LABEL << sameOriginatorName << endl;
  stdout << EVENT_LABEL << sameEventName << endl;
  stdout << LOCATIONS_LABEL << sameLocations << endl;

  stdout << LOCATION_CODES_LABEL << endl;

  for (int i = 0; i < sameLocations; i++)
  {
    stdout << sameLocationCodes[i] << endl;
  }

  stdout << DURATION_LABEL << sameDuration;
  stdout << DAY_LABEL << sameDay;
  stdout << TIME_LABEL << sameTime << endl;
  stdout << CALLSIGN_LABEL << sameCallSign << endl;
}
