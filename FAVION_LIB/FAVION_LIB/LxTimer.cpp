// LxTimer.cpp: implementation of the CTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LxTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimer::CTimer()
{
    Clear();
	
}
CTimer::~CTimer()
{
	
}
void __fastcall CTimer::Clear()
{
	Enable = false;
	End = 0.0;
}

double __fastcall CTimer::Update() {
	if (Start != 0) {
		if (GetCount() > Start) {
			Pass = ((GetCount() - Start) + Pass);
		}
		if (GetCount() < Start) {
			Pass = (((MAX_COUNT - Start) + GetCount()) + Pass ) ;
		}
		Start = GetCount();
	}
	
	else Start = GetCount();
	return Pass;
}

bool __fastcall CTimer::On(bool IsStatus, double mSecond)
{
	Update();
	if (IsStatus) {
		if (!End) {
			End = Pass + mSecond;
			Check();
		}
		else {
			Check();
		}
	}
	else Clear();
	
	return Enable;
}

void __fastcall CTimer::Check()
{
	if (End<=Pass) {
		Enable = true;
	}
	else {
		Enable = false;
	}
}

//---------------------------------------------------
// Class KTime 
//--------------------------------------------------
//KTime Time;
CString KTime::GetCurrentTime()
{
	CString strTime;
	SYSTEMTIME time;
	GetLocalTime(&time);

	strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);

	return strTime;
}

WORD KTime::GetCurrentYear()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wYear;
}

WORD KTime::GetCurrentMonth()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wMonth;
}

WORD KTime::GetCurrentDay()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wDay;
}

WORD KTime::GetCurrentHour()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wHour;
}

WORD KTime::GetCurrentMinute()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wMinute;
}

WORD KTime::GetCurrentSecond()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wSecond;
}

WORD KTime::GetCurrentMilliSecond()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	return time.wMilliseconds;
}
//---------------------------------------------------
// 
//--------------------------------------------------


