#include <Arduino.h>

#include <Streaming.h>

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SI4707.h>

#include "Logging.h"

bool Logging::begin(void)  {
  // Setup Clock
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet)
    return false;

  return true;
}

void Logging::end(void) {

}

void Logging::logLeadingZero(int value) {
  if (value < 10) {
    (*printer) << '0';
  }

  (*printer) << value;
}

void Logging::timestamp(void) {
  (*printer) << endl << EVENT_LABEL;
  if (timeStatus() == timeSet) {
    logLeadingZero(month());
    (*printer) << '-';
    logLeadingZero(day());
    (*printer) << '-';
    logLeadingZero(year());
    (*printer) << ' ';
    logLeadingZero(hour());
    (*printer) << ':';
    logLeadingZero(minute());
    (*printer) << ':';
    logLeadingZero(second());
  }
  else {
    (*printer) << '?';
  }
  (*printer).flush();
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

  Serial << VERSION_INFO_LABEL
  << PART_NUMBER_LABEL << pn << '\n'
  << FIRMWARE_MAJOR_LABEL << fwmajor << '\n'
  << FIRMWARE_MINOR_LABEL << fwminor << '\n'
  << PATCH_HIGH_LABEL << patchH << '\n'
  << PATCH_LOW_LABEL << patchL << '\n'
  << COMPONENT_MAJOR_LABEL << cmpmajor << '\n'
  << COMPONENT_MINOR_LABEL << cmpminor << '\n'
  << CHIP_REVISION_LABEL << chiprev << "\n\n";
}
//
//  Prints the Function Menu.
//
void Logging::showMenu(void)
{
  Serial << MENU;
}

void Logging::printBooting(void)
{
  Serial << STARTING_SI4707_LABEL;
}

void Logging::printDefaultsSet(void)
{
  Serial << DEFAULTS_SET_LABEL;
}
void Logging::printRadioOff(void)
{
  Serial << RADIO_OFF_LABEL;
}

void Logging::printRadioOn(void)
{
  Serial << RADIO_ON_LABEL;
}

void Logging::printConfigSaved(void)
{
  Serial << CONFIG_SAVED_LABEL;
}

void Logging::printFrequency(void)
{
  Serial << FREQ_LABEL << _FLOAT(frequency, 3) << '\n';
}

void Logging::printRssi(void)
{
  Serial << RSSI_LABEL << rssi << '\n';
}

void Logging::printSnr(void)
{
  Serial << SNR_LABEL << snr << '\n';
}

void Logging::printFrequencyOffset(void)
{
  Serial << FREQOFF_LABEL << freqoff << '\n';
}

void Logging::printEom(void)
{
  Serial << EOM_DETECTED;
}

void Logging::printWatOn(void)
{
  Serial << WAT_ON_LABEL;
}

void Logging::printWatOff(void)
{
  Serial << WAT_OFF_LABEL;
}

void Logging::printErrorOccurred(void)
{
  Serial << ERROR_OCCURRED_LABEL;
}

//
//  Simple Hex print utility - Prints a Byte with a leading zero and trailing space.
//
void Logging::printHex(byte value)
{
  Serial << "0x" << _HEX(value) << ' ';
}

void Logging::printChannelDown(void)
{
  Serial << CHANNEL_DOWN_LABEL;
}

void Logging::printChannelUp(void)
{
  Serial << CHANNEL_UP_LABEL;
}

void Logging::printScanning(void)
{
  Serial << SCANNING_LABEL;
}

void Logging::printVolume(void)
{
  Serial << VOLUME_LABEL << volume << '\n';
}

void Logging::printMuteOff(void)
{
  Serial << MUTE_OFF_LABEL;
}

void Logging::printMuteOn(void)
{
  Serial << MUTE_ON_LABEL;
}

void Logging::printSameMessage(void)
{
  Serial << ORIGINATOR_LABEL << sameOriginatorName << '\n';
  Serial << EVENT_LABEL << sameEventName << '\n';
  Serial << LOCATIONS_LABEL << sameLocations << '\n';

  Serial << LOCATION_CODES_LABEL << '\n';

  for (int i = 0; i < sameLocations; i++)
  {
    Serial << sameLocationCodes[i] << '\n';
  }

  Serial << DURATION_LABEL << sameDuration;
  Serial << DAY_LABEL << sameDay;
  Serial << TIME_LABEL << sameTime << '\n';
  Serial << CALLSIGN_LABEL << sameCallSign << '\n';
}


void Logging::logFrequency(void)
{
  timestamp();
  (*printer) << FREQ_LABEL << _FLOAT(frequency, 3) << '\n';
  (*printer).flush();
}

void Logging::logEom(void)
{
  timestamp();
  (*printer) << EOM_DETECTED;
  (*printer).flush();
}

void Logging::logErrorOccurred(void)
{
  timestamp();
  (*printer) << ERROR_OCCURRED_LABEL;
  (*printer).flush();
}

void Logging::logSameMessage(void) {
  timestamp();
  (*printer) << ORIGINATOR_LABEL << sameOriginatorName << '\n';
  (*printer) << EVENT_LABEL << sameEventName << '\n';
  (*printer) << LOCATIONS_LABEL << sameLocations << '\n';

  (*printer) << LOCATION_CODES_LABEL << '\n';

  for (int i = 0; i < sameLocations; i++)
  {
    (*printer) << sameLocationCodes[i] << '\n';
  }

  (*printer) << DURATION_LABEL << sameDuration;
  (*printer) << DAY_LABEL << sameDay;
  (*printer) << TIME_LABEL << sameTime << '\n';
  (*printer) << CALLSIGN_LABEL << sameCallSign << '\n';
  (*printer).flush();
}
