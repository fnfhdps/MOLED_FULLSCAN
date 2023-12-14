// Lock.h: interface for the CLock class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#ifdef THEDLL_EXPORTS
	#define DllExport   __declspec( dllexport )
#else
//	#define DllExport   __declspec( dllimport )
	#define DllExport
#endif

class DllExport CLock
{
private:
	CRITICAL_SECTION	m_cs;

public:
	CLock();
	~CLock();
	void Lock();
	void Unlock();
	BOOL IsLock();
};

