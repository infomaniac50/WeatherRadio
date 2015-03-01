#include <SPI.h>
#include <SD.h>

#include "LoggingSD.h"

bool LoggingSD::begin()
{
  if (!Logging::begin())
    return false;

  // Now using the good old Adafruit Data Logging Shield
  if (!SD.begin(SD_PIN))
    return false;

  logFile = SD.open("log.txt", FILE_WRITE);

  if (!logFile)
    return false;

  return true;
}

void LoggingSD::end(void)
{
  logFile.close();

  Logging::end();
}
