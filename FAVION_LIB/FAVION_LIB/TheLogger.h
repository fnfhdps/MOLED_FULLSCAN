// TheLogger.h: interface for the CTheLogger class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

//#include "Dshow.h"
#include "TheLoggerComm.h"


//#pragma comment(lib, "psapi.lib")

//
#define USER_ERROR_FLAG	0x20000000

typedef enum
{
	unit_sec,
	unit_msec,
	unit_usec,
	unit_nsec
} ThePhdUnit;

typedef struct ThePhd
{
	HANDLE		hProcess;
	__int64		Begin;
	__int64		End;
	__int64		Frequency;
	BOOL		bAffinity;
	DWORD		OrgAffinityMask;
	DWORD		AffinityMask;

	//////////////////////////////////////////////////////////////////////////
	//	Initialize member variable
	//	specially get high performance timer frequency & look for highest
	//	processor affinity
	ThePhd::ThePhd(BOOL	bAffine = FALSE)
	{
		Begin		= 0;
		End			= 0;
		bAffinity	= bAffine;
		AffinityMask= 1;

		//	Get frequency of timer
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency))
		{
			if (bAffinity)
			{
				DWORD systemAffinityMask;
				hProcess	= GetCurrentProcess();
#ifdef _M_IX86
				if (GetProcessAffinityMask(hProcess, &OrgAffinityMask, &systemAffinityMask))
#else
				if (GetProcessAffinityMask(hProcess, (PDWORD_PTR)&OrgAffinityMask, (PDWORD_PTR)&systemAffinityMask))
#endif
				{
					while(OrgAffinityMask & (AffinityMask << 1))
						AffinityMask <<= 1;	//	look for highest processor affinity
				}
			}
		}
	}

	ThePhd::~ThePhd()
	{
		// Reset the processor affinity(set to original processor affinity mask
		if (bAffinity) SetProcessAffinityMask(hProcess, OrgAffinityMask);
	}

	//////////////////////////////////////////////////////////////////////////
	//	get/set start timer & set to highest processor affinity
	BOOL ThePhd::start()
	{
		End = 0;

		if (!bAffinity)
		{
			if (QueryPerformanceCounter((LARGE_INTEGER*)&Begin))
				return TRUE;
		}
		else if (OrgAffinityMask)
		{
			//	set highest processor to  be a affinity mask processor
			if (SetProcessAffinityMask(hProcess, AffinityMask))
			{
				if (QueryPerformanceCounter((LARGE_INTEGER*)&Begin))
					return TRUE;
			}
		}
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//	get/set stop timer & set back to original processor affinity mask
	//	every time when it called, set stop timer again
	//	this means you can get elapse time from start()
	BOOL ThePhd::stop()
	{
		if (Begin)	// Get STOP time
		{
			if (QueryPerformanceCounter((LARGE_INTEGER*)&End))
			{
				if (!bAffinity) return TRUE;

				// Reset the processor affinity(set to original processor affinity mask
				if (SetProcessAffinityMask(hProcess, OrgAffinityMask))
					return TRUE;
			}
		}
		return FALSE;
	}

	//	get elapse time in second
	double ThePhd::sec()
	{
		if (!End) stop();
		return (double)(End - Begin) / (double)Frequency;
	}

	// get elapse time in millisecond
	double ThePhd::msec()
	{
		if (!End) stop();
		return (double)(End - Begin) / (double)Frequency * 1000.0;
	}

	//	get elapse time in micro second
	double ThePhd::usec()
	{
		if (!End) stop();
		return (double)(End - Begin) / (double)Frequency * 1000000.0;
	}

	//	get elapse time in nano-second
	double ThePhd::nsec()
	{
		if (!End) stop();
		return (double)(End - Begin) / (double)Frequency * 1000000000.0;
	}

	double ThePhd::elapse(ThePhdUnit unit = unit_msec)
	{
		if (!QueryPerformanceCounter((LARGE_INTEGER*)&End)) return 0.0;

		switch(unit)
		{
		case unit_msec:
			return (double)(End - Begin) / (double)Frequency * 1000.0;
		case unit_usec:
			return (double)(End - Begin) / (double)Frequency * 1000000.0;
		case unit_nsec:
			return (double)(End - Begin) / (double)Frequency * 1000000000.0;
		case unit_sec:
		default:
			return (double)(End - Begin) / (double)Frequency;
		}
	}

	//	delay 
	void ThePhd::delay(double msec)
	{
		if (start())
		{
			double r;
			do
			{
				if (!QueryPerformanceCounter((LARGE_INTEGER*)&End))
					goto ERROR_PROC;

				r = double(End - Begin) / (double)Frequency * 1000.0;
				if ((msec-r) > 5.0) Sleep(0);
			} while(r < msec);

			stop(); 
			return;
		}
ERROR_PROC:
		Sleep((DWORD)msec);
	}
} ThePhd;


PVOID TheDebuggerStart();
ULONG TheDebuggerStop();

// String manipulation functions

#define THELOG_TRACE

#ifndef THELOG_TRACE
	#define	LOG
#else
	#define	LOG this->SetLog
#endif


#ifndef THELOG_TRACE
	#define THEPHD_SEC(func, this) func
#else
	#define THEPHD_SEC(func, this)	{	ThePhd phd; if (phd.start()) { func; if (phd.stop()) { SetLog(#func " tact = %.9f[ms]", phd.msec()); } else {func; }} else{ func;}}
#endif

typedef	union __THE_VERSION
{
	BYTE	m_Version[2];
	BYTE	m_byMajor;		// major version number
	BYTE	m_byMinor;		// minor version number

} THE_VERSION;


class AFX_EXT_CLASS CTheLogger
{
public:
//	CTheLogger(const CTheLogger &_thelogger);
//	CTheLogger &operator=(const CTheLogger &_thelogger);

public:
	CTheLogger();
	virtual ~CTheLogger();

//	FILE* debugfile;

	/************************************************************************/
	/* szIPCName length + lpServerName length can't be over 80 bytes		*/
	/* lpszModuleName can't be over MAX_MOD_NAME_LEN, 						*/
	/************************************************************************/
	
	BOOL	SetModuleName(LPCTSTR lpszModuleName = TEXT("UnknownModule"));

	// If call CTheLogger::Crate more than one time only module name will updated
	// but, in case called CTheLogger::Create() on a while logging under multi-threading environment
	// can causes access violation

	BOOL	Create(LPCTSTR lpszModuleName, LPCTSTR lpszServerName = NULL);
	BOOL	Create(LPCTSTR lpszIPCName, LPCTSTR lpszModuleName, LPCTSTR lpszServerName /*= NULL*/);

	void	HexDump(void *ptr, size_t buflen);
	// longer than MAX_MSG_LEN will causes failure.
	BOOL	SetLog(LPCTSTR szFormatString, ...);
	void	SystemErrorLog();
	void	SystemErrorLog(int gdiStatus);
	void	UserErrorLog(DWORD dwdMsgID, LPCTSTR szFormatString, ...);
//	void	SystemErrorLog(HRESULT hr);
// 	bool    MomoryUsageLog(int check_point);
	bool    MomoryUsageLog(int check_point, bool OnlyMemCheck= false);
	bool    ResourceMonitor(int check_point);  

//private:
	BOOL	SetLog(DWORD dwMsgID, LPCTSTR szFormatString, ...);
	BOOL	Close(void);
	BOOL	GetProcessName();
	BOOL	GetProcessName2();

private:
//	LogMsg			m_Log;

	TCHAR	m_lpszAppName[MAX_PATH];			// application name
	TCHAR	m_lpszModuleName[MAX_MOD_NAME_LEN];	// Optional, module name
// 	size_t  m_physMemUsedByMe;
	static size_t  m_physMemUsedByMe;
	DWORD   m_nHandleCount;

	HANDLE	m_hClient;

public:
	void CPUID();

	/************************************************************************/
	/* return -1 : exception occured(cpuid not supporting),					*/
	/* 0 : no simd not supporting, 1 ~ 4 : simd extention 1 ~ 4				*/
	/************************************************************************/
	int	GetSimdExtNo();
	unsigned int GenuineIntel();
	unsigned int CpuIDSupported();
	static size_t  m_physMemWorkSize;
};
