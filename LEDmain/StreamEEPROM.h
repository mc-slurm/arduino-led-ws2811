#ifndef STREAM_EEPROM_H
#define STREAM_EEPROM_H

#pragma once

#include <vector>

class StreamEEPROM
{
	public:
		StreamEEPROM(void);
		~StreamEEPROM(void);
		
	public:
		void WriteEEPROM(uint32_t uiPosition);
		void ReadEEPROM(uint32_t uiPosition);
		
	public:
		uint32_t GetSize(void) const { return static_cast<uint32_t>(m_bytes.size()); }

	public:
		template<typename T>
		void Write(const T& rVal)
		{
			const uint8_t* pBytes = reinterpret_cast<const uint8_t*>(&rVal);
			for (uint32_t i = 0; i < (uint32_t)sizeof(rVal); i++)
			{
				m_bytes.push_back(*pBytes++);
			}
		}

		template<typename T>
		void Read(T& rVal) const
		{
			uint8_t* pBytes = reinterpret_cast<uint8_t*>(&rVal);
			for (uint32_t i = 0; i < (uint32_t)sizeof(rVal); i++)
			{
				*pBytes++ = m_bytes[const_cast<uint32_t&>(m_uiReadPos)++];
			}
		}
		
	protected:
		std::vector<uint8_t> m_bytes;
		uint32_t m_uiReadPos = 0;
		
		// public:
		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const bool& rVal);
		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const char& rVal);

		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const uint8_t& rVal);
		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const uint16_t& rVal);
		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const uint32_t& rVal);
		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const uint64_t& rVal);

		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const int8_t& rVal);
		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const int16_t& rVal);
		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const int32_t& rVal);
		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const int64_t& rVal);

		// StreamEEPROM& operator<<(StreamEEPROM& rStreamEEPROM, const String& rVal);
		
	// public:
		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, bool& rVal);
		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, char& rVal);

		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, uint8_t& rVal);
		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, uint16_t& rVal);
		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, uint32_t& rVal);
		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, uint64_t& rVal);

		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, int8_t& rVal);
		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, int16_t& rVal);
		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, int32_t& rVal);
		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, int64_t& rVal);

		// StreamEEPROM& operator>>(StreamEEPROM& rStreamEEPROM, String& rVal);
};
#endif