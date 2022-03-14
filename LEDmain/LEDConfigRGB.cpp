#include "LEDConfigRGB.h"
#include "StreamEEPROM.h"

const uint32_t c_uiVersion = 0;

void LEDConfigRGB::Serialize(StreamEEPROM& rStream) const
{
	rStream.Write(c_uiVersion);
	rStream.Write(m_uiRed);
	rStream.Write(m_uiGreen);
	rStream.Write(m_uiBlue);
}

void LEDConfigRGB::Serialize(const StreamEEPROM& rStream)
{
	uint32_t uiVersion = 0;
	rStream.Read(uiVersion);
	if (uiVersion >= 0)
	{
		rStream.Read(m_uiRed);
		rStream.Read(m_uiGreen);
		rStream.Read(m_uiBlue);	
	}
}

uint32_t LEDConfigRGB::GetSize(void) const
{
	return sizeof(c_uiVersion) + sizeof(m_uiRed) + sizeof(m_uiGreen) + sizeof(m_uiBlue);
}

uint32_t LEDConfigRGB::getVersion(void) const
{
	return c_uiVersion;
}

String LEDConfigRGB::GetClassName(void)
{
	return "LEDConfigRGB";
}