#include <Arduino.h>

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SI4707.h>

#include "Logging.h"
#include "Message.h"


bool Logging::begin(void)  {
  // Setup Clock
  setSyncProvider(RTC.get);
  return (timeStatus() == timeSet);
}

void Logging::end(void) {

}

void Logging::printRadioVersionTo(StreamEx printer)
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

  printer.printf("%p\n", VERSION_INFO_LABEL);
  printer.printf("%p%d\n", PART_NUMBER_LABEL, pn);
  printer.printf("%p%d\n", FIRMWARE_MAJOR_LABEL, fwmajor);
  printer.printf("%p%d\n", FIRMWARE_MINOR_LABEL, fwminor);
  printer.printf("%p%d\n", PATCH_HIGH_LABEL, patchH);
  printer.printf("%p%d\n", PATCH_LOW_LABEL, patchL);
  printer.printf("%p%d\n", COMPONENT_MAJOR_LABEL, cmpmajor);
  printer.printf("%p%d\n", COMPONENT_MINOR_LABEL, cmpminor);
  printer.printf("%p%d\n", CHIP_REVISION_LABEL, chiprev);
}
//
//  Prints the Function Menu.
//

//
//  Global Radio Variables.
//
// uint16_t channel = WB_MIN_FREQUENCY;
// float frequency;
// uint16_t volume = RADIO_VOLUME;
// uint8_t mute = OFF;
// uint8_t rssi;
// uint8_t snr;
// int freqoff;
// uint8_t power = OFF;
// //
// //  Global SAME Variables.
// //
// char sameOriginatorName[4];
// char sameEventName[4];
// char sameCallSign[9];
// //
// uint8_t sameHeaderCount;
// uint8_t sameState;
// uint8_t sameLength;
// uint8_t samePlusIndex;
// uint8_t sameLocations;
// uint32_t sameLocationCodes[SAME_LOCATION_CODES];
// uint16_t sameDuration;
// uint16_t sameDay;
// uint16_t sameTime;
// uint8_t sameWat = 0x02;
// uint8_t response[15];
void Logging::printSameMessageTo(StreamEx printer)
{
  timestampTo(printer);

  char _sameOriginatorName[4];
  char _sameEventName[4];
  char _sameCallSign[9];

  strcpy(_sameOriginatorName, sameOriginatorName);
  strcpy(_sameEventName, sameEventName);
  strcpy(_sameCallSign, sameCallSign);

  printer.printf("%p%s\n%p%s\n%p%s\n%p\n",
    ORIGINATOR_LABEL, _sameOriginatorName,
    EVENT_LABEL, _sameEventName,
    LOCATIONS_LABEL, _sameCallSign,
    LOCATION_CODES_LABEL
  );

  for (int i = 0; i < sameLocations; i++)
  {
    printer.printf("%d\n", sameLocationCodes[i]);
  }

  printer.printf("%p%d %p%d %p%d\n%p%s\n",
    DURATION_LABEL, sameDuration,
    DAY_LABEL, sameDay,
    TIME_LABEL, sameTime,
    CALLSIGN_LABEL, _sameCallSign
  );
}

void Logging::printRssiTo(StreamEx printer)
{
  printer.printf("%p%d\n", RSSI_LABEL, rssi);
}

void Logging::printSnrTo(StreamEx printer)
{
  printer.printf("%p%d\n", SNR_LABEL, snr);
}

void Logging::printFrequencyOffsetTo(StreamEx printer)
{
  printer.printf("%p%d\n", FREQOFF_LABEL, freqoff);
}

void Logging::printVolumeTo(StreamEx printer)
{
  printer.printf("%p%d\n", VOLUME_LABEL, volume);
}

void Logging::printFrequencyTo(StreamEx printer)
{
  timestampTo(printer);

  printer.printf("%p%0.3f\n", FREQ_LABEL, frequency);
}

void Logging::printEomTo(StreamEx printer)
{
  timestampTo(printer);
  printer.printf("%p\n", EOM_DETECTED);
}

void Logging::printErrorOccurredTo(StreamEx printer)
{
  timestampTo(printer);
  printer.printf("%p\n", ERROR_OCCURRED_LABEL);
}

void Logging::timestampTo(StreamEx printer) {
  if (timeStatus() == timeSet) {
    printer.printf("%p %02d-%02d-%04d %02d:%02d:%02d\n", EVENT_LABEL, month(), day(), year(), hour(), minute(), second());
  }
  else {
    printer.printf("%p %02d-%02d-%04d %02d:%02d:%02d\n", EVENT_LABEL, 0, 0, 0, 0, 0, 0);
  }
}
