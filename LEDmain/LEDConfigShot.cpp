#include "LEDConfigShot.h"
#include "StreamEEPROM.h"

const uint32_t c_uiVersion = 0;

void LEDConfigShot::Serialize(StreamEEPROMWrite& rStream) const
{
	rStream.Write(c_uiVersion);
	rStream.Write(m_uiSpeed);
	rStream.Write(m_uiSize);
	rStream.Write(m_uiRed);
	rStream.Write(m_uiGreen);
	rStream.Write(m_uiBlue);
}

void LEDConfigShot::Serialize(const StreamEEPROMRead& rStream)
{
	uint32_t uiVersion = 0;
	rStream.Read(uiVersion);
	if (uiVersion >= 0)
	{
		rStream.Read(m_uiSpeed);
		rStream.Read(m_uiSize);
		rStream.Read(m_uiRed);
		rStream.Read(m_uiGreen);
		rStream.Read(m_uiBlue);	
	}
}

uint32_t LEDConfigShot::GetSize(void) const
{
	return sizeof(c_uiVersion) + sizeof(m_uiSpeed) + sizeof(m_uiSize) + sizeof(m_uiRed) + sizeof(m_uiGreen) + sizeof(m_uiBlue);
}

uint32_t LEDConfigShot::getVersion(void) const
{
	return c_uiVersion;
}

String LEDConfigShot::GetClassName(void)
{
	return "LEDConfigShot";
}
