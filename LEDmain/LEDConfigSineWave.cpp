#include "LEDConfigSineWave.h"
#include "StreamEEPROM.h"

const uint32_t c_uiVersion = 0;

void LEDConfigSineWave::Serialize(StreamEEPROMWrite& rStream) const
{
	rStream.Write(c_uiVersion);
	rStream.Write(m_fk);
	rStream.Write(m_fw);
	rStream.Write(m_fo);
	rStream.Write(m_fD);
	rStream.Write(m_fdT);
	rStream.Write(m_uiRedHigh);
	rStream.Write(m_uiGreenHigh);
	rStream.Write(m_uiBlueHigh);
	rStream.Write(m_uiRedLow);
	rStream.Write(m_uiGreenLow);
	rStream.Write(m_uiBlueLow);
}

void LEDConfigSineWave::Serialize(const StreamEEPROMRead& rStream)
{
	uint32_t uiVersion = 0;
	rStream.Read(uiVersion);
	if (uiVersion >= 0)
	{
		rStream.Read(m_fk);
		rStream.Read(m_fw);
		rStream.Read(m_fo);
		rStream.Read(m_fD);
		rStream.Read(m_fdT);
		rStream.Read(m_uiRedHigh);
		rStream.Read(m_uiGreenHigh);
		rStream.Read(m_uiBlueHigh);
		rStream.Read(m_uiRedLow);
		rStream.Read(m_uiGreenLow);
		rStream.Read(m_uiBlueLow);
	}
}

LEDConfigSineWave& LEDConfigSineWave::operator=(const LEDConfigSineWave& rConfig)
{
	LEDConfigBase::operator=(rConfig);
	m_fk = rConfig.m_fk;
	m_fw = rConfig.m_fw;
	m_fo = rConfig.m_fo;
	m_fD = rConfig.m_fD;
	m_fdT = rConfig.m_fdT;
	m_uiRedHigh = rConfig.m_uiRedHigh;
	m_uiGreenHigh = rConfig.m_uiGreenHigh;
	m_uiBlueHigh = rConfig.m_uiBlueHigh;
	m_uiRedLow = rConfig.m_uiRedLow;
	m_uiGreenLow = rConfig.m_uiGreenLow;
	m_uiBlueLow = rConfig.m_uiBlueLow;
}

uint32_t LEDConfigSineWave::GetSize(void) const
{
	return sizeof(c_uiVersion) + sizeof(m_fk) + sizeof(m_fw) + sizeof(m_fo) + sizeof(m_fD) + sizeof(m_fdT) +
		   sizeof(m_uiRedHigh) + sizeof(m_uiGreenHigh) + sizeof(m_uiBlueHigh) +
		   sizeof(m_uiRedLow) + sizeof(m_uiGreenLow) + sizeof(m_uiBlueLow);
}

uint32_t LEDConfigSineWave::getVersion(void) const
{
	return c_uiVersion;
}

String LEDConfigSineWave::GetClassName(void)
{
	return "LEDConfigSineWave";
}
