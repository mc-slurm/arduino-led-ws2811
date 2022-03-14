#include "StreamEEPROM.h"
#include <Arduino.h>
#include <EEPROM.h>

StreamEEPROM::StreamEEPROM(void)
{
	
}
StreamEEPROM::~StreamEEPROM(void)
{
	
}

void StreamEEPROM::WriteEEPROM(uint32_t uiPosition)
{
	EEPROM.write(uiPosition++, m_bytes.size());
	for (uint32_t i = 0; i < m_bytes.size(); ++i)
	{
		EEPROM.write(uiPosition++, m_bytes[i]);
	}
}

void StreamEEPROM::ReadEEPROM(uint32_t uiPosition)
{
	size_t uiSize = EEPROM.read(uiPosition++);
	for (uint32_t i = 0; i < uiSize; ++i)
	{
		m_bytes[i] = EEPROM.read(uiPosition++);	
	}
}


// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, const bool& rVal)		{ write<bool>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, const char& rVal)		{ write<char>(rVal); return *this; }


// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, const uint8_t& rVal)		{ write<uint8_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, const uint16_t& rVal)	{ write<uint16_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, const uint32_t& rVal)	{ write<uint32_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, const uint64_t& rVal)	{ write<uint64_t>(rVal); return *this; }

// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, const int8_t& rVal)		{ write<int8_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, const int16_t& rVal)		{ write<int16_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, const int32_t& rVal)		{ write<int32_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, const int64_t& rVal)		{ write<int64_t>(rVal); return *this; }

// StreamEEPROM& StreamEEPROM::operator<<(StreamEEPROM& rStreamEEPROM, String& rVal)			{ write<String>(rVal.c_str()); return *this; }



// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, bool& rVal)			{ read<bool>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, char& rVal)			{ read<char>(rVal); return *this; }


// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, uint8_t& rVal)		{ read<uint8_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, uint16_t& rVal)		{ read<uint16_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, uint32_t& rVal)		{ read<uint32_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, uint64_t& rVal)		{ read<uint64_t>(rVal); return *this; }

// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, int8_t& rVal)		{ read<int8_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, int16_t& rVal)		{ read<int16_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, int32_t& rVal)		{ read<int32_t>(rVal); return *this; }
// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, int64_t& rVal)		{ read<int64_t>(rVal); return *this; }

// StreamEEPROM& StreamEEPROM::operator>>(StreamEEPROM& rStreamEEPROM, String& rVal)		{ read<String>(rVal.c_str()); return *this; }
