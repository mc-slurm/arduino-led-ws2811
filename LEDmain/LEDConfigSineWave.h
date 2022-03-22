#ifndef LED_CONFIG_SINE_WAVE_H
#define LED_CONFIG_SINE_WAVE_H

#pragma once

#include "LEDConfigBase.h"


class LEDConfigSineWave : public LEDConfigBase
{
	public:
		LEDConfigSineWave(const String& rName) : LEDConfigBase(rName) {}
		LEDConfigSineWave(const String& rName,
						  float fk, float fw, float fo, float fD, float fdT,
						  uint8_t uiRedHigh, uint8_t uiGreenHigh, uint8_t uiBlueHigh,
						  uint8_t uiRedLow, uint8_t uiGreenLow, uint8_t uiBlueLow)
						  : LEDConfigBase(rName),
						    m_fk(fk),
							m_fw(fw),
							m_fo(fo),
							m_fD(fD),
							m_fdT(fdT),
							m_uiRedHigh(uiRedHigh),
							m_uiGreenHigh(uiGreenHigh),
							m_uiBlueHigh(uiBlueHigh),
							m_uiRedLow(uiRedLow),
							m_uiGreenLow(uiGreenLow),
							m_uiBlueLow(uiBlueLow) {}
		virtual ~LEDConfigSineWave(void) {}

	public:
		virtual void Serialize(StreamEEPROMWrite& rStream) const;
		virtual void Serialize(const StreamEEPROMRead& rStream);	
	public:
		virtual uint32_t GetSize(void) const;
		virtual String GetClassName(void);
	protected:
		virtual uint32_t getVersion(void) const;
		
	public:
		float m_fk = 0.6f;
		float m_fw = 0.7f;
		float m_fo = 0.f;
		float m_fD = 0.f;
		
		float m_fdT = 0.3f;
		
		uint8_t m_uiRedHigh = 255;
		uint8_t m_uiGreenHigh = 0;		
		uint8_t m_uiBlueHigh = 0;

		uint8_t m_uiRedLow = 0;
		uint8_t m_uiGreenLow = 0;		
		uint8_t m_uiBlueLow = 255;

};
#endif