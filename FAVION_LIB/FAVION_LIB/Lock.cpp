// Lock.cpp: implementation of the CLock class.
//
//////////////////////////////////////////////////////////////////////

#pragma hdrstop

#include "Lock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLock::CLock()
	: m_tid(0), m_entry(0)
{ 
	InitializeCriticalSection(&m_cs);
//	TRACE("create %x\n", &m_cs);
}

CLock::~CLock()
{
//	TRACE("delete %x\n", &m_cs);
	DeleteCriticalSection(&m_cs);
}
void CLock::Lock()
{
	//	dead-lock search 시 유용함.
// 	DWORD tick = GetTickCount();
// 	while (!IsLock())
// 	{
// 		ASSERT(GetTickCount() - tick < 5000);
// 		Sleep(0);
// 	}

	EnterCriticalSection(&m_cs);
	m_tid = GetCurrentThreadId();
	++m_entry;

}
void CLock::Unlock()
{
//	ASSERT(m_tid == GetCurrentThreadId());

	if (--m_entry == 0)
		m_tid = 0;

//	동일 thread에서 복수회 호출하는 경우가 있음.
//	ASSERT(m_entry <= 1);

	LeaveCriticalSection(&m_cs);
}				
BOOL CLock::IsLock()
{
	if (TryEnterCriticalSection(&m_cs))
	{
		m_tid = GetCurrentThreadId();
		++m_entry;
		return TRUE;
	}
	return FALSE;
}

BOOL CLock::IsLock(DWORD timeout)
{
	ULONGLONG t0 = GetTickCount();
	BOOL locked = FALSE;

	do 
	{
		locked = IsLock();

	} while (!locked && (GetTickCount() - t0 < timeout));

	return locked;
}

BOOL CLock::IsOwnerThread()
{
//	return (m_cs.LockCount && GetThreadId(m_cs.OwningThread) == GetCurrentThreadId());
	return (m_entry && m_tid == GetCurrentThreadId());
}
