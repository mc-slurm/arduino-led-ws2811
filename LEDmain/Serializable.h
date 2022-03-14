#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#pragma once

class StreamEEPROM;

class Serializable
{
	public:
		virtual void Serialize(StreamEEPROM& rStream) const = 0;
		virtual void Serialize(const StreamEEPROM& rStream) = 0;	
	protected:
		virtual uint32_t getVersion(void) const = 0;
};
#endif