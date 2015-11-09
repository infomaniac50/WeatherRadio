#ifndef _MESSAGE_H
#define _MESSAGE_H
// SD Card Globals

const char FREQ_LABEL[] PROGMEM = "FREQ: ";
const char RSSI_LABEL[] PROGMEM = "RSSI: ";
const char SNR_LABEL[] PROGMEM = "SNR: ";
const char FREQOFF_LABEL[] PROGMEM = "FREQOFF: ";

const char ORIGINATOR_LABEL[] PROGMEM = "Originator: ";
const char EVENT_LABEL[] PROGMEM = "Event: ";
const char LOCATIONS_LABEL[] PROGMEM = "Locations: ";
const char LOCATION_CODES_LABEL[] PROGMEM = "Location Codes: ";
const char DURATION_LABEL[] PROGMEM = "Duration: ";
const char DAY_LABEL[] PROGMEM = " Day: ";
const char TIME_LABEL[] PROGMEM = " Time: ";
const char CALLSIGN_LABEL[] PROGMEM = "Callsign: ";

const char VOLUME_LABEL[] PROGMEM = "Volume: ";
const char MUTE_ON_LABEL[] PROGMEM = "Mute: On\n";
const char MUTE_OFF_LABEL[] PROGMEM = "Mute: Off\n";
const char RADIO_ON_LABEL[] PROGMEM = "Radio: On\n";
const char RADIO_OFF_LABEL[] PROGMEM = "Radio: Off\n";
const char CHANNEL_UP_LABEL[] PROGMEM = "Channel Up\n";
const char CHANNEL_DOWN_LABEL[] PROGMEM = "Channel Down\n";

const char WAT_ON_LABEL[] PROGMEM = "WAT: On";
const char WAT_OFF_LABEL[] PROGMEM = "WAT: Off";

const char STARTING_SI4707_LABEL[] PROGMEM = "Starting up the Si4707.......";
const char DEFAULTS_SET_LABEL[] PROGMEM = "Defaults set.";
const char CONFIG_SAVED_LABEL[] PROGMEM = "Config saved.";
const char SCANNING_LABEL[] PROGMEM = "Scanning.....";
const char EOM_DETECTED[] PROGMEM = "EOM detected";
const char ERROR_OCCURRED_LABEL[] PROGMEM = "An error occured!";

const char VERSION_INFO_LABEL[] PROGMEM = "Version Information";
const char PART_NUMBER_LABEL[] PROGMEM = "PN: ";
const char PATCH_HIGH_LABEL[] PROGMEM = "PatchH: ";
const char PATCH_LOW_LABEL[] PROGMEM = "PatchL: ";
const char CHIP_REVISION_LABEL[] PROGMEM = "Chip Rev: ";
const char COMPONENT_MAJOR_LABEL[] PROGMEM = "CMP Major: ";
const char COMPONENT_MINOR_LABEL[] PROGMEM = "CMP Minor: ";
const char FIRMWARE_MAJOR_LABEL[] PROGMEM = "FW Major: ";
const char FIRMWARE_MINOR_LABEL[] PROGMEM = "FW Minor: ";

const char MENU[] PROGMEM = "\nDisplay this menu =\t 'h' or '?'\n" \
                            "Channel down =\t\t 'd'\n" \
                            "Channel up =\t\t 'u'\n" \
                            "Scan =\t\t\t 's'\n" \
                            "Volume + =\t\t '+'\n" \
                            "Volume - =\t\t '-'\n" \
                            "Mute / Unmute =\t\t 'm'\n" \
                            "On / Off =\t\t 'o'\n" \
                            "Save Settings = \t 'e'\n\n" \
                            ;

#endif