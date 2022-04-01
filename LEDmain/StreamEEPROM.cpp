#include "StreamEEPROM.h"
#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 256

StreamEEPROM::StreamEEPROM(void)
{
	
}
StreamEEPROM::~StreamEEPROM(void)
{
	
}

void StreamEEPROMWrite::WriteEEPROM(uint32_t uiPosition)
{
	if (!EEPROM.begin(EEPROM_SIZE))
	{
		m_printFunc("failed to initialise EEPROM");
	}

	m_printFunc("StreamEEPROMWrite");
	uint8_t uiSize = m_bytes.size();
	m_printFunc("- size: " + String(m_bytes.size()));
	EEPROM.write(uiPosition++, uiSize);
	//EEPROM.put(uiPosition++, uiSize);
	for (uint32_t i = 0; i < m_bytes.size(); ++i)
	{
		m_printFunc("- writing pos " + String(i) + " [val " + String(m_bytes[i]) + "]");
		EEPROM.write(uiPosition++, m_bytes[i]);
		//EEPROM.put(uiPosition++, m_bytes[i]);
	}
	m_printFunc("- commit");
	EEPROM.commit();
	EEPROM.end();
	m_printFunc("- size in EEPROM: " + String(EEPROM.length()));
	m_printFunc("- done.");
}

void StreamEEPROMRead::ReadEEPROM(uint32_t uiPosition)
{
	if (!EEPROM.begin(EEPROM_SIZE))
	{
		m_printFunc("failed to initialise EEPROM");
	}
	
	m_printFunc("StreamEEPROMRead");
	uint8_t uiSize = 0;
	uiSize = EEPROM.read(uiPosition++);
	//EEPROM.get(uiPosition++, uiSize);
	m_bytes.resize((size_t)uiSize);
	m_printFunc("- size: " + String(m_bytes.size()));
	for (uint32_t i = 0; i < uiSize; ++i)
	{
		m_bytes[i] = EEPROM.read(uiPosition++);	
		//EEPROM.get(uiPosition++, m_bytes[i]);	
	}
	m_printFunc("- done.");
	EEPROM.end();
}
