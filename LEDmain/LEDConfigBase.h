#ifndef LED_CONFIG_BASE_H
#define LED_CONFIG_BASE_H

#pragma once

#include "Arduino.h"
#include "Serializable.h"
class StreamEEPROM;

class LEDConfigBase : public Serializable
{
	public:
		LEDConfigBase(const String& rName) : m_name(rName) {}
		virtual ~LEDConfigBase(void) {}

	public:
		virtual void Serialize(StreamEEPROM& rStream) const = 0;
		virtual void Serialize(const StreamEEPROM& rStream) = 0;	
	public:
		virtual void SetName(const String& rName) { m_name = rName; }
		virtual String GetName(void) const { return m_name; }
		virtual uint32_t GetSize(void) const = 0;
	public:
		virtual String GetClassName(void) = 0;

	protected:
		virtual uint32_t getVersion(void) const = 0;
	private:
		String m_name;
};
#endif