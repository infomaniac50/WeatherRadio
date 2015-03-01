#include <SPI.h>
#include <Ethernet.h>

#include "LoggingEthernet.h"

bool LoggingEthernet::begin(void) {
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    IPAddress ip(192, 168, 1, 50);
    Ethernet.begin(mac, ip);
  }
}
