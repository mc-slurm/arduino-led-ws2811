#ifndef STREAM_EEPROM_H
#define STREAM_EEPROM_H

#pragma once

#include <vector>
#include <functional>
#include <Arduino.h>

class StreamEEPROM
{
	public:
		StreamEEPROM(void);
		virtual ~StreamEEPROM(void);
		
	public:
		uint32_t GetSize(void) const { return static_cast<uint32_t>(m_bytes.size()); }
		void RegisterPrintFunction(std::function<void(const String&)> rFunc) { m_printFunc = rFunc; }

	protected:
		std::function<void(const String&)> m_printFunc;
		
	protected:
		std::vector<uint8_t> m_bytes;
		uint32_t m_uiReadPos = 0;
};

class StreamEEPROMRead : public StreamEEPROM
{
	public:
		StreamEEPROMRead(void) : StreamEEPROM() {}
		virtual ~StreamEEPROMRead(void) {}
	public:
		void ReadEEPROM(uint32_t uiPosition);
	public:
		template<typename T>
		void Read(T& rVal) const
		{
			uint8_t* pBytes = reinterpret_cast<uint8_t*>(&rVal);
			for (uint32_t i = 0; i < (uint32_t)sizeof(rVal); i++)
			{
				*pBytes++ = m_bytes[const_cast<uint32_t&>(m_uiReadPos)++];
			}
		}
};

class StreamEEPROMWrite : public StreamEEPROM
{
	public:
		StreamEEPROMWrite(void) : StreamEEPROM() {}
		virtual ~StreamEEPROMWrite(void) {}
	public:
		void WriteEEPROM(uint32_t uiPosition);
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

};
#endif