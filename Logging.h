class Logging {
private:
  // SD Card Globals
  File logFile;

public:
  bool begin(void);
  void end(void);
  void logEom(void);
  void logErrorOccurred(void);
  void logFrequency(void);
  void logLeadingZero(int value);
  void logSameMessage(void);
  void printBooting(void);
  void printChannelDown(void);
  void printChannelUp(void);
  void printConfigSaved(void);
  void printDefaultsSet(void);
  void printEom(void);
  void printErrorOccurred(void);
  void printFrequency(void);
  void printFrequencyOffset(void);
  void printHex(byte value);
  void printMuteOff(void);
  void printMuteOn(void);
  void printRadioOff(void);
  void printRadioOn(void);
  void printRadioVersion(void);
  void printRssi(void);
  void printSameMessage(void);
  void printScanning(void);
  void printSnr(void);
  void printVolume(void);
  void printWatOff(void);
  void printWatOn(void);
  void showMenu(void);
  void timestamp(void);
};

