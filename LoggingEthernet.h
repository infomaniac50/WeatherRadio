#ifndef _LOGGINGETHERNET_H
#define _LOGGINGETHERNET_H

#include "Logging.h"


class LoggingEthernet : public Logging {
private:
  uint8_t* mac;
  EthernetClient client;
public:
  LoggingEthernet() : Logging((Stream*)&client) {

  }
  bool begin(void);
  void end(void);
};

#endif
