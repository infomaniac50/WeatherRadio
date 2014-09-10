#include <Arduino.h>

#include <Streaming.h>
#include <Flash.h>

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SI4707.h>
#include <SPI.h>
#include <SD.h>

#include "Logging.h"

#define stdout Serial

FLASH_STRING(FREQ_LABEL, "FREQ: ");
FLASH_STRING(RSSI_LABEL, "RSSI: ");
FLASH_STRING(SNR_LABEL, "SNR: ");
FLASH_STRING(FREQOFF_LABEL, "FREQOFF: ");

FLASH_STRING(ORIGINATOR_LABEL, "Originator: ");
FLASH_STRING(EVENT_LABEL, "Event: ");
FLASH_STRING(LOCATIONS_LABEL, "Locations: ");
FLASH_STRING(LOCATION_CODES_LABEL, "Location Codes: ");
FLASH_STRING(DURATION_LABEL, "Duration: ");
FLASH_STRING(DAY_LABEL, " Day: ");
FLASH_STRING(TIME_LABEL, " Time: ");
FLASH_STRING(CALLSIGN_LABEL, "Callsign: ");

FLASH_STRING(VOLUME_LABEL, "Volume: ");
FLASH_STRING(MUTE_ON_LABEL, "Mute: On\n");
FLASH_STRING(MUTE_OFF_LABEL, "Mute: Off\n");
FLASH_STRING(RADIO_ON_LABEL, "Radio: On\n");
FLASH_STRING(RADIO_OFF_LABEL, "Radio: Off\n");
FLASH_STRING(CHANNEL_UP_LABEL, "Channel Up\n");
FLASH_STRING(CHANNEL_DOWN_LABEL, "Channel Down\n");

FLASH_STRING(WAT_ON_LABEL, "WAT: On\n");
FLASH_STRING(WAT_OFF_LABEL, "WAT: Off\n");

FLASH_STRING(STARTING_SI4707_LABEL, "Starting up the Si4707.......\n\n");
FLASH_STRING(DEFAULTS_SET_LABEL, "Defaults set.\n");
FLASH_STRING(CONFIG_SAVED_LABEL, "Config saved.\n");
FLASH_STRING(SCANNING_LABEL, "Scanning.....\n");
FLASH_STRING(EOM_DETECTED, "EOM detected\n");
FLASH_STRING(ERROR_OCCURRED_LABEL, "An error occured!\n\n");

FLASH_STRING(VERSION_INFO_LABEL, "Version Information\n");
FLASH_STRING(PART_NUMBER_LABEL, "PN: ");
FLASH_STRING(PATCH_HIGH_LABEL, "PatchH: ");
FLASH_STRING(PATCH_LOW_LABEL, "PatchL: ");
FLASH_STRING(CHIP_REVISION_LABEL, "Chip Rev: ");
FLASH_STRING(COMPONENT_MAJOR_LABEL, "CMP Major: ");
FLASH_STRING(COMPONENT_MINOR_LABEL, "CMP Minor: ");
FLASH_STRING(FIRMWARE_MAJOR_LABEL, "FW Major: ");
FLASH_STRING(FIRMWARE_MINOR_LABEL, "FW Minor: ");

FLASH_STRING(MENU, "Display this menu =\t 'h' or '?'\n"
  "Channel down =\t\t 'd'\n"
  "Channel up =\t\t 'u'\n"
  "Scan =\t\t\t 's'\n"
  "Volume + =\t\t '+'\n"
  "Volume - =\t\t '-'\n"
  "Mute / Unmute =\t\t 'm'\n"
  "On / Off =\t\t 'o'\n"
  "Save Settings = \t 'e'\n\n"
);

bool Logging::begin()
{
  // Setup Clock
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet)
    return false;

  // Now using the good old Adafruit Data Logging Shield
  if (!SD.begin(10))
    return false;

  logFile = SD.open("log.txt", FILE_WRITE);

  if (!logFile)
    return false;

  return true;
}

void Logging::end(void)
{
  logFile.close();
}
void Logging::logLeadingZero(int value) {
  if (value < 10) {
    logFile << '0';
  }

  logFile << value;
}

void Logging::timestamp(void) {
  logFile << endl << EVENT_LABEL;
  if (timeStatus() == timeSet) {
    logLeadingZero(month());
    logFile << '-';
    logLeadingZero(day());
    logFile << '-';
    logLeadingZero(year());
    logFile << ' ';
    logLeadingZero(hour());
    logFile << ':';
    logLeadingZero(minute());
    logFile << ':';
    logLeadingZero(second());
  }
  else {
    logFile << '?';
  }

  logFile << endl;
}

void Logging::printRadioVersion(void)
{
  Radio.getRevision();  //  Only captured on the logic analyzer - not displayed.
  // The hell it ain't :p
  uint8_t pn = response[1];
  uint8_t fwmajor = response[2];
  uint8_t fwminor = response[3];
  uint8_t patchH = response[4];
  uint8_t patchL = response[5];
  uint8_t cmpmajor = response[6];
  uint8_t cmpminor = response[7];
  uint8_t chiprev = response[8];

  stdout << VERSION_INFO_LABEL
  << PART_NUMBER_LABEL << pn << endl
  << FIRMWARE_MAJOR_LABEL << fwmajor << endl
  << FIRMWARE_MINOR_LABEL << fwminor << endl
  << PATCH_HIGH_LABEL << patchH << endl
  << PATCH_LOW_LABEL << patchL << endl
  << COMPONENT_MAJOR_LABEL << cmpmajor << endl
  << COMPONENT_MINOR_LABEL << cmpminor << endl
  << CHIP_REVISION_LABEL << chiprev << endl << endl;
}
//
//  Prints the Function Menu.
//
void Logging::showMenu(void)
{
  stdout << MENU;
}

void Logging::printBooting(void)
{
  stdout << STARTING_SI4707_LABEL;
}

void Logging::printDefaultsSet(void)
{
  stdout << DEFAULTS_SET_LABEL;
}
void Logging::printRadioOff(void)
{
  stdout << RADIO_OFF_LABEL;
}

void Logging::printRadioOn(void)
{
  stdout << RADIO_ON_LABEL;
}

void Logging::printConfigSaved(void)
{
  stdout << CONFIG_SAVED_LABEL;
}

void Logging::printFrequency(void)
{
  stdout << FREQ_LABEL << _FLOAT(frequency, 3) << endl;
}

void Logging::printRssi(void)
{
  stdout << RSSI_LABEL << rssi << endl;
}

void Logging::printSnr(void)
{
  stdout << SNR_LABEL << snr << endl;
}

void Logging::printFrequencyOffset(void)
{
  stdout << FREQOFF_LABEL << freqoff << endl;
}

void Logging::printEom(void)
{
  stdout << EOM_DETECTED;
}

void Logging::printWatOn(void)
{
  stdout << WAT_ON_LABEL;
}

void Logging::printWatOff(void)
{
  stdout << WAT_OFF_LABEL;
}

void Logging::printErrorOccurred(void)
{
  stdout << ERROR_OCCURRED_LABEL;
}

//
//  Simple Hex print utility - Prints a Byte with a leading zero and trailing space.
//
void Logging::printHex(byte value)
{
  stdout << "0x" << _HEX(value) << ' ';
}

void Logging::printChannelDown(void)
{
  stdout << CHANNEL_DOWN_LABEL;
}

void Logging::printChannelUp(void)
{
  stdout << CHANNEL_UP_LABEL;
}

void Logging::printScanning(void)
{
  stdout << SCANNING_LABEL;
}

void Logging::printVolume(void)
{
  stdout << VOLUME_LABEL << volume << endl;
}

void Logging::printMuteOff(void)
{
  stdout << MUTE_OFF_LABEL;
}

void Logging::printMuteOn(void)
{
  stdout << MUTE_ON_LABEL;
}

void Logging::printSameMessage(void)
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


void Logging::logFrequency(void)
{
  timestamp();
  logFile << FREQ_LABEL << _FLOAT(frequency, 3) << endl;
  logFile.flush();
}

void Logging::logEom(void)
{
  timestamp();
  logFile << EOM_DETECTED;
  logFile.flush();
}

void Logging::logErrorOccurred(void)
{
  timestamp();
  logFile << ERROR_OCCURRED_LABEL;
  logFile.flush();
}

void Logging::logSameMessage(void) {
  timestamp();
  logFile << ORIGINATOR_LABEL << sameOriginatorName << endl;
  logFile << EVENT_LABEL << sameEventName << endl;
  logFile << LOCATIONS_LABEL << sameLocations << endl;

  logFile << LOCATION_CODES_LABEL << endl;

  for (int i = 0; i < sameLocations; i++)
  {
    logFile << sameLocationCodes[i] << endl;
  }

  logFile << DURATION_LABEL << sameDuration;
  logFile << DAY_LABEL << sameDay;
  logFile << TIME_LABEL << sameTime << endl;
  logFile << CALLSIGN_LABEL << sameCallSign << endl;
  logFile.flush();
}
