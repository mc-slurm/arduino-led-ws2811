#include "NTPTime.h"
#include "Arduino.h"


NTPTime::NTPTime(void)
{
	
}

NTPTime::~NTPTime(void)
{
	
}

void NTPTime::Init(const long& iGMTOffsetSec, const int& iDaylightOffsetSec, const char* pNTPServer)
{
  // Timeserver: init and get the time
  configTime(iGMTOffsetSec, iDaylightOffsetSec, pNTPServer);
}

tm NTPTime::GetTime(void) const
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    return timeinfo;
  }
  return timeinfo;
}