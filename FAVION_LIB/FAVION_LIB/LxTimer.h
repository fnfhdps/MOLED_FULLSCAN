// LxTimer.h: interface for the CTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXTIMER_H__BAADF3B9_3E66_4ADB_9CFA_59367C7566D6__INCLUDED_)
#define AFX_LXTIMER_H__BAADF3B9_3E66_4ADB_9CFA_59367C7566D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CTimer  
{
public:
	CTimer();
	~CTimer();
	
protected :
#define MAX_COUNT 99999999
	bool Enable;
	double End;
	double Start;
	double Pass;
	void   __fastcall Check();
	double __fastcall Update();
	
public :
	
	void __fastcall Clear();
	bool __fastcall On(bool IsStatus, double mSecond);
	double __fastcall GetCount()
	{
		LARGE_INTEGER Counter;
		LARGE_INTEGER Frequency;
		QueryPerformanceCounter  (&Counter);
		QueryPerformanceFrequency(&Frequency);
		double Clock = (((double)Counter.QuadPart/(double)Frequency.QuadPart)*1000.0);
		return Clock;
	}
};
//----------------------------------------------------------------------------------------------------------------------
class AFX_EXT_CLASS KTime
{
public :
	void SetFrequency()				{ QueryPerformanceFrequency(&Freq);}
	void StartCounter()				{ QueryPerformanceCounter(&Start) ;}
	void StopCounter()				{ QueryPerformanceCounter(&Stop)  ;}
	double GetCounterTimeMS()		{ return ((double)(Stop.QuadPart - Start.QuadPart)/Freq.QuadPart*1000); } //Get ms time
	double GetCount() {
		LARGE_INTEGER Counter;
		LARGE_INTEGER Frequency;
		QueryPerformanceCounter  (&Counter);
		QueryPerformanceFrequency(&Frequency);
		double Clock = (((double)Counter.QuadPart/(double)Frequency.QuadPart)*1000.0);
		return Clock;
	}

	CString GetCurrentTime();
	WORD	GetCurrentYear();
	WORD	GetCurrentMonth();
	WORD	GetCurrentWeek();
	WORD	GetCurrentDay();
	WORD	GetCurrentHour();
	WORD	GetCurrentMinute();
	WORD	GetCurrentSecond();
	WORD	GetCurrentMilliSecond();

private:

	LARGE_INTEGER Start;
	LARGE_INTEGER Stop;
	LARGE_INTEGER Freq;
};
//extern KTime Time;

template<typename T>
inline T limit8bit(const T& value)
{
	return ( (value > 255) ? 255 : ((value < 0) ? 0 : value) );
}
//----------------------------------------------------------------------------------------------------------------------

#endif // !defined(AFX_LXTIMER_H__BAADF3B9_3E66_4ADB_9CFA_59367C7566D6__INCLUDED_)
