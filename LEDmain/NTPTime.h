#ifndef NPTTIME_H
#define NPTTIME_H

#pragma once

#include "Singleton.h"
#include "time.h"

class NTPTime : public Singleton<NTPTime>
{
	public:
		NTPTime(void);
		virtual ~NTPTime(void);
		
	public:

	public:
		void Init(const long& iGMTOffsetSec, const int& iDaylightOffsetSec, const char* pNTPServer);
		tm GetTime(void) const; 
};
#endif