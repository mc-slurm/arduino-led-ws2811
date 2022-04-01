#include "StreamEEPROM.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "Logger.h"

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
		LED_LOG("failed to initialise EEPROM");
	}

	LED_LOG("StreamEEPROMWrite");
	uint8_t uiSize = m_bytes.size();
	LED_LOG("- size: " + String(m_bytes.size()));
	EEPROM.write(uiPosition++, uiSize);
	//EEPROM.put(uiPosition++, uiSize);
	for (uint32_t i = 0; i < m_bytes.size(); ++i)
	{
		LED_LOG("- writing pos " + String(i) + " [val " + String(m_bytes[i]) + "]");
		EEPROM.write(uiPosition++, m_bytes[i]);
		//EEPROM.put(uiPosition++, m_bytes[i]);
	}
	LED_LOG("- commit");
	EEPROM.commit();
	EEPROM.end();
	LED_LOG("- size in EEPROM: " + String(EEPROM.length()));
	LED_LOG("- done.");
}

void StreamEEPROMRead::ReadEEPROM(uint32_t uiPosition)
{
	if (!EEPROM.begin(EEPROM_SIZE))
	{
		LED_LOG("failed to initialise EEPROM");
	}
	
	LED_LOG("StreamEEPROMRead");
	uint8_t uiSize = 0;
	uiSize = EEPROM.read(uiPosition++);
	//EEPROM.get(uiPosition++, uiSize);
	m_bytes.resize((size_t)uiSize);
	LED_LOG("- size: " + String(m_bytes.size()));
	for (uint32_t i = 0; i < uiSize; ++i)
	{
		m_bytes[i] = EEPROM.read(uiPosition++);	
		//EEPROM.get(uiPosition++, m_bytes[i]);	
	}
	LED_LOG("- done.");
	EEPROM.end();
}
