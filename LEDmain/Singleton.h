#ifndef SINGLETON_H
#define SINGLETON_H

#pragma once

#include <mutex>
template<typename TYPE>
class Singleton
{
	public:
	   static TYPE& GetInstance(void);
	   virtual ~Singleton(void) {}

	protected:
	   //! Konstruktor.
	   Singleton(void) {}

	private:
	   //! Konstruktor.
	   Singleton(Singleton const&) {}
	   //! Zuweisungsoperator.
	   void operator=(Singleton const&) {}

	private:
	   //! Wächter.
	   struct Guardian { virtual ~Guardian(void); };

	   static TYPE* sm_pInstance;
};
#endif
 
// Initialisierung.
template<class TYPE>
TYPE *Singleton<TYPE>::sm_pInstance = nullptr;
 
template<typename TYPE>
TYPE& Singleton<TYPE>::GetInstance(void)
{
	static std::mutex s_mutex;
	std::lock_guard<std::mutex> lock(s_mutex);

	if (sm_pInstance == nullptr)
	{
		static Guardian guardian;
		sm_pInstance = new TYPE();
	}
	return *sm_pInstance;
}
 
template<typename TYPE>
Singleton<TYPE>::Guardian::~Guardian(void)
{
	if (sm_pInstance != nullptr)
	{
		delete sm_pInstance;
	}
}
