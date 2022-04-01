#ifndef LOGGER_H
#define LOGGER_H

#pragma once

#include "Singleton.h"

#if 1
	#define LED_LOG(X) Logger::GetInstance().Log(X);
#else
	#define LED_LOG(X) /##/
#endif

class Logger : public Singleton<Logger>
{
	public:
		Logger(void) {}
		virtual ~Logger(void) {}
		
	public:
		void Log(const String& rString) { if (m_printFunc != nullptr) m_printFunc(rString); }
		
	public:
		void RegisterPrintFunction(std::function<void(const String&)> rFunc) { m_printFunc = rFunc; }
		
	private:
		std::function<void(const String&)> m_printFunc;

};
#endif