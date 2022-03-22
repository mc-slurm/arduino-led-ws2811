#ifndef LED_CONFIG_RGB_H
#define LED_CONFIG_RGB_H

#pragma once

#include "LEDConfigBase.h"


class LEDConfigRGB : public LEDConfigBase
{
	public:
		LEDConfigRGB(const String& rName) : LEDConfigBase(rName) {}
		LEDConfigRGB(const String& rName, uint8_t uiRed, uint8_t uiGreen, uint8_t uiBlue) : LEDConfigBase(rName), m_uiRed(uiRed), m_uiGreen(uiGreen), m_uiBlue(uiBlue) {}
		virtual ~LEDConfigRGB(void) {}

	public:
		virtual void Serialize(StreamEEPROMWrite& rStream) const;
		virtual void Serialize(const StreamEEPROMRead& rStream);	
	public:
		virtual uint32_t GetSize(void) const;
		virtual String GetClassName(void);
	protected:
		virtual uint32_t getVersion(void) const;
		
	public:
		uint8_t m_uiRed = 128;
		uint8_t m_uiGreen = 128;
		uint8_t m_uiBlue = 128;
};
#endif