
#pragma once

//#include "StdAfx.h"	// VS 6.0������ �ʿ���.
#include <Windows.h>

#define MAX_MSG_LEN			424	// �� size�� 424���� �ϰ�.
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
