#pragma once
#include <cassert>
#include "Misc.h"

template<class T>
class Singleton
{
protected:
	Singleton()
	{
		assert(instance == nullptr);
		instance = static_cast<T*>(this);
	}

public:
	virtual ~Singleton()
	{
		instance = nullptr;
	}

	bool Initialize()
	{
		if (!initializeFlag)
		{
			initializeFlag = InitializeElements();
		}
		return initializeFlag;
	}
private:
	virtual bool InitializeElements() = 0;
public:
	virtual bool Finalize() { return true; }

	Singleton(Singleton&) = delete;
	Singleton& operator=(Singleton&) = delete;

	static inline T* GetInstance() { return instance; }
private:
	static inline T* instance = nullptr;
protected:
	bool initializeFlag = false;
};