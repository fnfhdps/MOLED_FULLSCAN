
#pragma once

//#include "StdAfx.h"	// VS 6.0에서는 필요함.
#include <Windows.h>

#define MAX_MSG_LEN			424	// 총 size는 424이하 일것.
#define MAX_MOD_NAME_LEN	51
#define MAX_LOG_THREAD		1

typedef struct tagLogMsg
{
	DWORD		m_msgID;							// Optional,
	SYSTEMTIME	m_time;
	TCHAR		m_lpszModuleName[MAX_MOD_NAME_LEN];	// Optional, module name
	TCHAR		m_lpszLogMsg[MAX_MSG_LEN];
} LogMsg, *LPLogMsg;

//typedef DWORD  (CALLBACK *LogFuncPtrType)(LPLogMsg pLog);
typedef DWORD  (*LogFuncPtrType)(LPLogMsg pLog);

#define CACHE_LINE      16
#define CACHE_ALIGN    __declspec(align(CACHE_LINE))
