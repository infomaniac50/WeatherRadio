#ifndef _LOGGING_H
#define _LOGGING_H

#ifndef _PRINTEX_H
#define _PRINTEX_H
#include <PrintEx.h>
#endif

#include <avr/pgmspace.h>

class Logging {
  public:
    bool begin(void);
    void end(void);
    void printEomTo(StreamEx printer);
    void printErrorOccurredTo(StreamEx printer);
    void printFrequencyOffsetTo(StreamEx printer);
    void printFrequencyTo(StreamEx printer);
    void printRadioVersionTo(StreamEx printer);
    void printRssiTo(StreamEx printer);
    void printSameMessageTo(StreamEx printer);
    void printSnrTo(StreamEx printer);
    void printVolumeTo(StreamEx printer);
    void timestampTo(StreamEx printer);
};

#endif
