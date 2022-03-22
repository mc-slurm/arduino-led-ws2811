#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#pragma once

class StreamEEPROMRead;
class StreamEEPROMWrite;

class Serializable
{
	public:
		virtual void Serialize(StreamEEPROMWrite& rStream) const = 0;
		virtual void Serialize(const StreamEEPROMRead& rStream) = 0;	
	protected:
		virtual uint32_t getVersion(void) const = 0;
};
#endif