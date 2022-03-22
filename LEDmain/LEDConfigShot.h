#ifndef LED_CONFIG_SHOT_H
#define LED_CONFIG_SHOT_H

#pragma once

#include "LEDConfigBase.h"


class LEDConfigShot : public LEDConfigBase
{
	public:
		LEDConfigShot(const String& rName) : LEDConfigBase(rName) {}
		LEDConfigShot(const String& rName, uint8_t uiSpeed, uint8_t uiSize, uint8_t uiRed, uint8_t uiGreen, uint8_t uiBlue) : LEDConfigBase(rName), m_uiSpeed(uiSpeed), m_uiSize(uiSize), m_uiRed(uiRed), m_uiGreen(uiGreen), m_uiBlue(uiBlue) {}
		virtual ~LEDConfigShot(void) {}

	public:
		virtual void Serialize(StreamEEPROMWrite& rStream) const;
		virtual void Serialize(const StreamEEPROMRead& rStream);	
	public:
		virtual uint32_t GetSize(void) const;
		virtual String GetClassName(void);
	protected:
		virtual uint32_t getVersion(void) const;
		
	public:
		uint8_t m_uiSpeed = 1;
		uint8_t m_uiSize = 3;
		
		uint8_t m_uiRed = 180;
		uint8_t m_uiGreen = 180;		
		uint8_t m_uiBlue = 0;
};
#endif