#ifndef _LOGGINGSD_H
#define _LOGGINGSD_H
#include "Logging.h"

#define SD_PIN 10

class LoggingSD : public Logging {
private:
  File logFile;
public:
  LoggingSD() : Logging((Stream*)&logFile) {

  }
  bool begin(void);
  void end(void);
};

#endif
