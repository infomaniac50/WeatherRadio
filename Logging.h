#ifndef _LOGGING_H
#define _LOGGING_H

#include <Stream.h>
#include <avr/pgmspace.h>

class Logging {
private:
  // SD Card Globals
  Stream* printer;

  const PROGMEM char* FREQ_LABEL = "FREQ: ";
  const PROGMEM char* RSSI_LABEL = "RSSI: ";
  const PROGMEM char* SNR_LABEL = "SNR: ";
  const PROGMEM char* FREQOFF_LABEL = "FREQOFF: ";

  const PROGMEM char* ORIGINATOR_LABEL = "Originator: ";
  const PROGMEM char* EVENT_LABEL = "Event: ";
  const PROGMEM char* LOCATIONS_LABEL = "Locations: ";
  const PROGMEM char* LOCATION_CODES_LABEL = "Location Codes: ";
  const PROGMEM char* DURATION_LABEL = "Duration: ";
  const PROGMEM char* DAY_LABEL = " Day: ";
  const PROGMEM char* TIME_LABEL = " Time: ";
  const PROGMEM char* CALLSIGN_LABEL = "Callsign: ";

  const PROGMEM char* VOLUME_LABEL = "Volume: ";
  const PROGMEM char* MUTE_ON_LABEL = "Mute: On\n";
  const PROGMEM char* MUTE_OFF_LABEL = "Mute: Off\n";
  const PROGMEM char* RADIO_ON_LABEL = "Radio: On\n";
  const PROGMEM char* RADIO_OFF_LABEL = "Radio: Off\n";
  const PROGMEM char* CHANNEL_UP_LABEL = "Channel Up\n";
  const PROGMEM char* CHANNEL_DOWN_LABEL = "Channel Down\n";

  const PROGMEM char* WAT_ON_LABEL = "WAT: On\n";
  const PROGMEM char* WAT_OFF_LABEL = "WAT: Off\n";

  const PROGMEM char* STARTING_SI4707_LABEL = "Starting up the Si4707.......\n\n";
  const PROGMEM char* DEFAULTS_SET_LABEL = "Defaults set.\n";
  const PROGMEM char* CONFIG_SAVED_LABEL = "Config saved.\n";
  const PROGMEM char* SCANNING_LABEL = "Scanning.....\n";
  const PROGMEM char* EOM_DETECTED = "EOM detected\n";
  const PROGMEM char* ERROR_OCCURRED_LABEL = "An error occured!\n\n";

  const PROGMEM char* VERSION_INFO_LABEL = "Version Information\n";
  const PROGMEM char* PART_NUMBER_LABEL = "PN: ";
  const PROGMEM char* PATCH_HIGH_LABEL = "PatchH: ";
  const PROGMEM char* PATCH_LOW_LABEL = "PatchL: ";
  const PROGMEM char* CHIP_REVISION_LABEL = "Chip Rev: ";
  const PROGMEM char* COMPONENT_MAJOR_LABEL = "CMP Major: ";
  const PROGMEM char* COMPONENT_MINOR_LABEL = "CMP Minor: ";
  const PROGMEM char* FIRMWARE_MAJOR_LABEL = "FW Major: ";
  const PROGMEM char* FIRMWARE_MINOR_LABEL = "FW Minor: ";

  const PROGMEM char* MENU = "\nDisplay this menu =\t 'h' or '?'\n" \
    "Channel down =\t\t 'd'\n" \
    "Channel up =\t\t 'u'\n" \
    "Scan =\t\t\t 's'\n" \
    "Volume + =\t\t '+'\n" \
    "Volume - =\t\t '-'\n" \
    "Mute / Unmute =\t\t 'm'\n" \
    "On / Off =\t\t 'o'\n" \
    "Save Settings = \t 'e'\n\n" \
  ;

public:
  Logging(Stream* _printer) {
    printer = _printer;
  }
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

#endif
