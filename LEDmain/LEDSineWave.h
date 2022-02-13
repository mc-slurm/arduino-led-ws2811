#ifndef LEDSINEWAVE_H
#define LEDSINEWAVE_H

#pragma once

#include "LEDBase.h"

/** y(x,t) = A * sin(kx-wt+o)+D
	k = 2*pi / wavelength
	w = 2*pi * f, (f = angular frequency in radians per sec)
	o = phase angle (in radians)
*/
class LEDSineWave : public LEDBase
{
	public:
		LEDSineWave(void);
		virtual ~LEDSineWave(void);
		
	public:
		virtual String GetSubPage(void) const override;
		virtual String GetSubPageLink(void) const override;
		virtual void CreateHTML(String& rHTMLString) override;
		virtual void UpdateLEDs(CRGB* leds, int iNumLEDs) override;
		
	protected:
		virtual void onEvent(std::vector<std::pair<String, String>>& rArguments) override;
	
	private:
		float m_fk = 0.6f;
		float m_fw = 0.7f;
		float m_fo = 0.f;
		float m_fD = 0.f;
		
		float m_fdT = 0.3f;
		float m_fTime = 0.f;
		
	private:
		uint8_t m_iRedHigh = 255;
		uint8_t m_iBlueHigh = 0;
		uint8_t m_iGreenHigh = 0;		

		uint8_t m_iRedLow = 0;
		uint8_t m_iBlueLow = 255;
		uint8_t m_iGreenLow = 0;		
};
#endif