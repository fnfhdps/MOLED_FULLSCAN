// TheLogger.cpp: implementation of the CTheLogger class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "StdAfx.h"
#include <Winsock2.h>
#include "strsafe.h"
//#include "thelogger.h"
#include "TheLogger.h"
#include "Dbghelp.h"
#include "Psapi.h"
#include "wininet.h"
#include <string>
#pragma comment(lib, "Psapi.lib") 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
size_t CTheLogger::m_physMemUsedByMe=0;
size_t CTheLogger::m_physMemWorkSize=0;

CTheLogger::CTheLogger()
{
	m_lpszAppName[0]	= TEXT('\0');
	m_lpszModuleName[0]	= TEXT('\0');

	m_hClient = INVALID_HANDLE_VALUE;
	//Memory Check Value
	m_physMemUsedByMe =0;
	m_nHandleCount    =0;
//	_wfopen_s(&debugfile, TEXT("debugout.txt"), TEXT("a+"));
}

CTheLogger::~CTheLogger()
{
//	fclose(debugfile);
}

BOOL CTheLogger::Close(void)
{
	if(m_hClient != INVALID_HANDLE_VALUE)
		CloseHandle(m_hClient);

	return 0;
}

BOOL CTheLogger::SetModuleName(LPCTSTR lpszModuleName)
{
	if (0 < _tcslen(lpszModuleName))
	{
		_tcscpy_s(m_lpszModuleName, MAX_MOD_NAME_LEN, lpszModuleName);
		return TRUE;
	}

	UserErrorLog(0, _T("Too long module name"));
	return FALSE;
}

BOOL CTheLogger::Create(LPCTSTR lpszModuleName, LPCTSTR lpszServerName)
{
//	fwprintf_s(debugfile, TEXT("Create #1 module : %s, server : %s\n"), lpszModuleName, lpszServerName);

	BOOL bRet = Create( TEXT("TheLogger"), lpszModuleName, lpszServerName);
	return bRet;
}


BOOL CTheLogger::Create(LPCTSTR lpszIPCName, LPCTSTR lpszModuleName, LPCTSTR lpszServerName)
{
	TCHAR svrName[100];
	size_t len, len2;

//	fwprintf_s(debugfile, TEXT("Create #2 ipcname : %s, module : %s, server : %s\n"), lpszIPCName, lpszModuleName, lpszServerName);

	if (lpszServerName == NULL)
		len = _tcslen(lpszIPCName);
	else
	{
		len  = _tcslen(lpszIPCName   );
		len2 = _tcslen(lpszServerName);
		len += len2;
	}

	if(len > 80) return FALSE;

	if(!SetModuleName(lpszModuleName)) return FALSE;

	if (m_hClient != INVALID_HANDLE_VALUE) return TRUE;

	_stprintf_s(svrName, TEXT("\\\\%s\\mailslot\\%s"), (lpszServerName == NULL) ? TEXT(".") : lpszServerName, lpszIPCName);

//	fwprintf_s(debugfile, TEXT("svrName : %s\n"), svrName);

	m_hClient = CreateFile(svrName, 
		GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,  // required to write to a mailslot 
		(LPSECURITY_ATTRIBUTES) NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		(HANDLE) NULL);

//	fwprintf_s(debugfile, TEXT("m_hClient : %x"), m_hClient);


	GetProcessName2();

	if (m_hClient != INVALID_HANDLE_VALUE)	return TRUE;

	return FALSE;
}

BOOL CTheLogger::GetProcessName2()
{
	TCHAR appName[MAX_PATH];

	DWORD dwLen = GetModuleFileName(NULL, appName, MAX_PATH);
	if (!dwLen) return FALSE;

	DWORD dwtmp;
	// search first '\' from full path string to extract process name only
	for(dwtmp = dwLen ; dwtmp > 0 && appName[dwtmp] != TEXT('\\'); dwtmp--);
	// extract process string only
	_tcsnccpy_s(m_lpszAppName, &appName[dwtmp + 1], (dwLen - dwtmp + 1));
	return TRUE;
}
// 
// BOOL CTheLogger::GetProcessName()
// {
// 	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, GetCurrentProcessId());
// 
// 	if (hProcess)
// 	{
// 		HMODULE hMod;
// 		DWORD	cdNeeded;
// 
// 		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cdNeeded))
// 			GetModuleBaseName(hProcess, hMod, m_lpszAppName, sizeof(m_lpszAppName));
// 
// 		CloseHandle(hProcess);
// 		return TRUE;
// 	}
// 	return FALSE;
// }


void CTheLogger::SystemErrorLog()
{
	LPVOID lpMsgBuf = NULL;

	int nErr = GetLastError();	// Windows system error

// 	if (!nErr)	// socket error 
// 	{
// 		if (!(nErr = WSAGetLastError())) return;
// 	}

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, nErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf, 0, NULL);

	if (lpMsgBuf)
	{
		CTheLogger::SetLog(nErr, (LPCTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
	}
	else
	{
		SetLog(TEXT("System Error Code \"%d\" cannot create message format"), nErr);
	}
}

TCHAR		   *g_WeekOfDay[7] = {TEXT("Sun"), TEXT("Mon"), TEXT("Tue"), TEXT("Wed"), TEXT("Thu"), TEXT("Fri"), TEXT("Sat")};

#define MULTI_BUF	25

//void CTheLogger::UserErrorLog(DWORD dwMsgID, LPCTSTR szFormatString, ...)
//{
//	if(m_hClient == INVALID_HANDLE_VALUE) return;
//
//	SYSTEMTIME tmNow;
//	GetLocalTime(&tmNow);
//	
//	CString log;
//
//	log.Format(TEXT("%04d.%02d.%02d`%02d:%02d:%02d:%03d`%s`%s`%s`") , 
//		tmNow.wYear, tmNow.wMonth, tmNow.wDay,
//		tmNow.wHour, tmNow.wMinute, tmNow.wSecond, tmNow.wMilliseconds,
//		g_WeekOfDay[tmNow.wDayOfWeek], m_lpszAppName, m_lpszModuleName);
//
//	char buf[1000];
//	va_list	args;
//	va_start(args, szFormatString);
//	vsprintf(buf, szFormatString, args);
//	va_end(args);
//
//	log += buf;
//
//	CString log2;
//	log2.Format(TEXT("`%d"), dwMsgID|USER_ERROR_FLAG);
//	log += log2;
//
//	CString length;
//	length.Format(TEXT("#LENGTH=%08d"), log.GetLength());
//	log.Insert(0, length);
//
//	DWORD	cbToWrite = log.GetLength() * sizeof(TCHAR), cbWritten;
//
//	int	from = 0;
//
//	//	split the log string
//	if (log.GetLength() * sizeof(TCHAR) > 420)
//		cbToWrite = 420;
//
//	BOOL bResult = TRUE;
//	do
//	{
//		if (log.GetLength() * sizeof(TCHAR) - from <= 420)
//			cbToWrite = log.GetLength() * sizeof(TCHAR) - from;
//
//#ifdef _UNICODE
//		char mbcs[500];
//		size_t bytes = 0;
//		wcstombs_s(&bytes, mbcs, 500, log.Mid(from / sizeof(TCHAR), cbToWrite), cbToWrite / 2);
//		bResult = WriteFile(m_hClient, mbcs, bytes, &cbWritten, (LPOVERLAPPED)NULL);
//#else
//
//		bResult = WriteFile(m_hClient, (LPCTSTR)log.Mid(from / sizeof(TCHAR), cbToWrite), cbToWrite, &cbWritten, (LPOVERLAPPED)NULL);
//
//#endif
//
//		from += cbWritten;
//
//	} while (from < log.GetLength()/* * sizeof(TCHAR)*/ && bResult);
//}
//
void CTheLogger::UserErrorLog(DWORD dwMsgID, LPCTSTR szFormatString, ...)
{
	if (m_hClient == INVALID_HANDLE_VALUE) return;


	SYSTEMTIME tmNow;
	GetLocalTime(&tmNow);

	const int H = 16;
	const int N = MAX_PATH * 4;
	TCHAR log[N];
	int leng = H;

	leng += _stprintf_s(&log[leng], N - leng, TEXT("%04d.%02d.%02d`%02d:%02d:%02d:%03d`%s`%s`%s`"),
		tmNow.wYear, tmNow.wMonth, tmNow.wDay,
		tmNow.wHour, tmNow.wMinute, tmNow.wSecond, tmNow.wMilliseconds,
		g_WeekOfDay[tmNow.wDayOfWeek], m_lpszAppName, m_lpszModuleName);

	va_list	args;
	va_start(args, szFormatString);
	leng += vswprintf_s(&log[leng], N - leng, szFormatString, args);
	va_end(args);

	leng += _stprintf_s(&log[leng], N - leng, TEXT("`%d"), dwMsgID | USER_ERROR_FLAG);

	TCHAR header[MAX_PATH];
	_stprintf_s(header, MAX_PATH, TEXT("#LENGTH=%08d"), leng - H);
	_tcsncpy(log, header, H);

	DWORD	cbToWrite = leng, cbWritten;

	int	from = 0;

	//	split the log string
	if (leng > 420)
		cbToWrite = 420;

	BOOL bResult = TRUE;
	do
	{
		if (leng - from <= 420)
			cbToWrite = leng - from;

#ifdef _UNICODE
		std::string str_mid(CW2A(std::wstring(&log[from], cbToWrite).c_str()).m_psz);

		bResult = WriteFile(m_hClient, str_mid.c_str(), (DWORD)str_mid.size(), &cbWritten, (LPOVERLAPPED)NULL);
#else
		bResult = WriteFile(m_hClient, (LPCTSTR)log.Mid(from / sizeof(TCHAR), cbToWrite), cbToWrite, &cbWritten, (LPOVERLAPPED)NULL);

#endif

		from += cbWritten;

	} while (from < leng && bResult);
}


//BOOL CTheLogger::SetLog(DWORD dwMsgID, LPCTSTR szFormatString, ...)
//{
//	if(m_hClient == INVALID_HANDLE_VALUE) return FALSE;
//
//	SYSTEMTIME tmNow;
//	GetLocalTime(&tmNow);
//
//	CString log;
//
//	log.Format(TEXT("%04d.%02d.%02d`%02d:%02d:%02d:%03d`%s`%s`%s`") , 
//		tmNow.wYear, tmNow.wMonth, tmNow.wDay,
//		tmNow.wHour, tmNow.wMinute, tmNow.wSecond, tmNow.wMilliseconds,
//		g_WeekOfDay[tmNow.wDayOfWeek], m_lpszAppName, m_lpszModuleName);
//
//	char buf[1000];
//	va_list	args;
//	va_start(args, szFormatString);
//	vsprintf(buf, szFormatString, args);
//	va_end(args);
//
//	log += buf;
//
//// 	bool debug = false;
//// 	if (log.Find(TEXT("사용 가능한")) != -1)
//// 		debug = true;
//
//	CString log2;
//	log2.Format(TEXT("`%d"), dwMsgID);
//
//	log += log2;
//
//	CString length;
//	length.Format(TEXT("#LENGTH=%08d"), log.GetLength());
//	log.Insert(0, length);
//
//	DWORD	cbToWrite = log.GetLength() * sizeof(TCHAR), cbWritten;
//	int		from = 0;
//
//	//	split the log string
//	if (log.GetLength() * sizeof(TCHAR) > 420)
//		cbToWrite = 420;
//
//	BOOL bResult = TRUE;
//	do
//	{
//		if (log.GetLength() * sizeof(TCHAR) - from <= 420)
//			cbToWrite = log.GetLength() * sizeof(TCHAR) - from;
//
//#ifdef _UNICODE
//		char mbcs[500];
//		size_t bytes = 0;
//		wcstombs_s(&bytes, mbcs, 500, log.Mid(from / sizeof(TCHAR), cbToWrite), cbToWrite / 2);
//		bResult = WriteFile(m_hClient, mbcs, bytes, &cbWritten, (LPOVERLAPPED)NULL);
//#else
//
//		bResult = WriteFile(m_hClient, (LPCTSTR)log.Mid(from / sizeof(TCHAR), cbToWrite), cbToWrite, &cbWritten, (LPOVERLAPPED)NULL);
//	
//#endif
//
//		from += cbWritten;
//
//	} while (from < log.GetLength()/* * sizeof(TCHAR)*/ && bResult);
//
//	return bResult;
//}
BOOL CTheLogger::SetLog(DWORD dwMsgID, LPCTSTR szFormatString, ...)
{
	if (m_hClient == INVALID_HANDLE_VALUE) return FALSE;

	SYSTEMTIME tmNow;
	GetLocalTime(&tmNow);

	const int H = 16;
	const int N = MAX_PATH * 4;
	TCHAR log[N] = { 0, };
	int leng = H;

	leng += _stprintf_s(&log[leng], N - leng, TEXT("%04d.%02d.%02d`%02d:%02d:%02d:%03d`%s`%s`%s`"),
		tmNow.wYear, tmNow.wMonth, tmNow.wDay,
		tmNow.wHour, tmNow.wMinute, tmNow.wSecond, tmNow.wMilliseconds,
		g_WeekOfDay[tmNow.wDayOfWeek], m_lpszAppName, m_lpszModuleName);

	va_list	args;
	va_start(args, szFormatString);
	leng += vswprintf_s(&log[leng], N - leng, szFormatString, args);
	va_end(args);

	leng += _stprintf_s(&log[leng], N - leng, TEXT("`%d"), dwMsgID);

	TCHAR header[MAX_PATH] = { 0, };
	_stprintf_s(header, MAX_PATH, TEXT("#LENGTH=%08d"), leng - H);
	_tcsncpy(log, header, H);

	DWORD	cbToWrite = leng, cbWritten;
	int		from = 0;

	//	split the log string
	if (leng > 420)
		cbToWrite = 420;

	BOOL bResult = TRUE;
	do
	{
		if (leng - from <= 420)
			cbToWrite = leng - from;

		try
		{
#ifdef _UNICODE
			std::string str_mid(CW2A(std::wstring(&log[from], cbToWrite).c_str()).m_psz);
			bResult = WriteFile(m_hClient, str_mid.c_str(), (DWORD)str_mid.size(), &cbWritten, (LPOVERLAPPED)NULL);
#else
			bResult = WriteFile(m_hClient, (LPCTSTR)log.Mid(from / sizeof(TCHAR), cbToWrite), cbToWrite, &cbWritten, (LPOVERLAPPED)NULL);
#endif
		}
		catch (...)
		{
			break;
		}

		from += cbWritten;

	} while (from < leng && bResult);

	return bResult;
}

//BOOL CTheLogger::SetLog(LPCTSTR szFormatString, ...)
//{
//	if(m_hClient == INVALID_HANDLE_VALUE) return FALSE;
//
//	SYSTEMTIME tmNow;
//	GetLocalTime(&tmNow);
//
//	CString log;
//
//	log.Format(TEXT("%04d.%02d.%02d`%02d:%02d:%02d:%03d`%s`%s`%s`"),
//		tmNow.wYear, tmNow.wMonth, tmNow.wDay,
//		tmNow.wHour, tmNow.wMinute, tmNow.wSecond, tmNow.wMilliseconds,
//		g_WeekOfDay[tmNow.wDayOfWeek], m_lpszAppName, m_lpszModuleName);
//
//	char buf[1000];
//	va_list	args;
//	va_start(args, szFormatString);
//	vsprintf(buf, szFormatString, args);
//	va_end(args);
//
//	log += buf;
//
//	CString log2;
//	log2.Format(TEXT("`0"));
//
//	log += log2;
//
//	CString length;
//	length.Format(TEXT("#LENGTH=%08d"), log.GetLength());
//	log.Insert(0, length);
//
//	DWORD	cbToWrite = log.GetLength() * sizeof(TCHAR), cbWritten;
//	int		from = 0;
//
//	//	split the log string
//	if (log.GetLength() * sizeof(TCHAR) > 420)
//		cbToWrite = 420;
//
//	BOOL bResult = TRUE;
//	do
//	{
//		if (log.GetLength() * sizeof(TCHAR) - from <= 420)
//			cbToWrite = log.GetLength() * sizeof(TCHAR) - from;
//
//#ifdef _UNICODE
//		char mbcs[500];
//		size_t bytes = 0;
//		wcstombs_s(&bytes, mbcs, 500, log.Mid(from / sizeof(TCHAR), cbToWrite), cbToWrite / 2);
//		bResult = WriteFile(m_hClient, mbcs, bytes, &cbWritten, (LPOVERLAPPED)NULL);
//#else
//
//		bResult = WriteFile(m_hClient, (LPCTSTR)log.Mid(from / sizeof(TCHAR), cbToWrite), cbToWrite, &cbWritten, (LPOVERLAPPED)NULL);
//
//#endif
//
//		from += cbWritten;
//
//	} while (from < log.GetLength()/* * sizeof(TCHAR)*/ && bResult);
//
//	return bResult;
//}
BOOL CTheLogger::SetLog(LPCTSTR szFormatString, ...)
{
	if (m_hClient == INVALID_HANDLE_VALUE) return FALSE;

	SYSTEMTIME tmNow;
	GetLocalTime(&tmNow);

	const int H = 16;
	const int N = MAX_PATH * 4;
	TCHAR log[N] = { 0, };
	int leng = H;

	leng += _stprintf_s(&log[leng], N - leng, TEXT("%04d.%02d.%02d`%02d:%02d:%02d:%03d`%s`%s`%s`"),
		tmNow.wYear, tmNow.wMonth, tmNow.wDay,
		tmNow.wHour, tmNow.wMinute, tmNow.wSecond, tmNow.wMilliseconds,
		g_WeekOfDay[tmNow.wDayOfWeek], m_lpszAppName, m_lpszModuleName);

	va_list	args;
	va_start(args, szFormatString);
	leng += vswprintf_s(&log[leng], N - leng, szFormatString, args);
	va_end(args);

	leng += _stprintf_s(&log[leng], N - leng, TEXT("`0"));

	TCHAR header[MAX_PATH] = { 0, };
	_stprintf_s(header, MAX_PATH, TEXT("#LENGTH=%08d"), leng - H);
	_tcsncpy(log, header, H);

	DWORD	cbToWrite = leng, cbWritten;
	int		from = 0;

	//	split the log string
	if (leng > 420)
		cbToWrite = 420;

	BOOL bResult = TRUE;
	do
	{
		if (leng - from <= 420)
			cbToWrite = leng - from;

		try
		{

#ifdef _UNICODE
			std::string str_mid(CW2A(std::wstring(&log[from], cbToWrite).c_str()).m_psz);
			bResult = WriteFile(m_hClient, str_mid.c_str(), (DWORD)str_mid.size(), &cbWritten, (LPOVERLAPPED)NULL);
#else
			bResult = WriteFile(m_hClient, (LPCTSTR)log.Mid(from / sizeof(TCHAR), cbToWrite), cbToWrite, &cbWritten, (LPOVERLAPPED)NULL);
#endif
		}
		catch (...) { break; }

		from += cbWritten;

	} while (from < leng && bResult);

	return bResult;
}

// bool CTheLogger::MomoryUsageLog(int check_point)
// {
// #if(0)
// 	MEMORYSTATUSEX memInfo;
// 	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
// 	GlobalMemoryStatusEx(&memInfo);
// 	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
// 
// 	//Same code as in "Total Virtual Memory" and then
// 	DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
// #endif
// 
// 	HANDLE   hcur_process = GetCurrentProcess();
// 
// 	if (hcur_process)
// 	{
// 
// // 		   PROCESS_MEMORY_COUNTERS_EX pmc;
// 		PROCESS_MEMORY_COUNTERS   pmc;
// 
// 		GetProcessMemoryInfo(hcur_process, &pmc, sizeof(pmc));
// 		SIZE_T physMemUsedByMe = pmc.PeakWorkingSetSize;
// 
// 		if (pmc.PeakWorkingSetSize > m_physMemUsedByMe)
// 		{
// // 			SetLog(TEXT("_________*Currently Memory usage %lld (%lld) @ %d"), (pmc.PeakWorkingSetSize/1024), (pmc.PeakWorkingSetSize - m_physMemUsedByMe)/1024, check_point);
// 			SetLog(TEXT("_________*Currently Memory Useage Max_%dKB Dv(%dByte) @ %d"), (pmc.PeakWorkingSetSize/1024), (pmc.PeakWorkingSetSize - m_physMemUsedByMe), check_point);
// 
// 			m_physMemUsedByMe = pmc.PeakWorkingSetSize;
// 			CloseHandle(hcur_process);
// 			return true;
// 		}
// 		CloseHandle(hcur_process);
// 		return false;
// 	}
// }
bool CTheLogger::MomoryUsageLog(int check_point,bool OnlyMemCheck)
{
#if(0)
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;

	//Same code as in "Total Virtual Memory" and then
	DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
#endif

	HANDLE   hcur_process = GetCurrentProcess();
	bool     IsMemoryLeak = false;

	if (hcur_process)
	{
		// 		   PROCESS_MEMORY_COUNTERS_EX pmc;
		PROCESS_MEMORY_COUNTERS   pmc;

		GetProcessMemoryInfo(hcur_process, &pmc, sizeof(pmc));
// 		SIZE_T physMemUsedByMe = pmc.PeakWorkingSetSize;

		if(OnlyMemCheck){
			if(pmc.WorkingSetSize>m_physMemWorkSize){
				SetLog(TEXT("_________*Currently Memory WorkSize Min_%dKB Dv(%dKB) @ %d"), (pmc.WorkingSetSize/1024)    , ((pmc.WorkingSetSize -     m_physMemWorkSize)/1024), check_point);
// 				SetLog(TEXT("_________*Currently Memory PeakSize Max_%dKB Dv(%dKB) @ %d"), (pmc.PeakWorkingSetSize/1024), ((pmc.PeakWorkingSetSize - m_physMemUsedByMe)/1024), check_point);
				m_physMemWorkSize = pmc.WorkingSetSize;
// 				m_physMemUsedByMe = pmc.PeakWorkingSetSize;
			}
			// 작업메모리가 증가했을경우 최대작업 메모리 동시 체크 
			if (pmc.PeakWorkingSetSize > m_physMemUsedByMe){
				SetLog(TEXT("_________*Currently Memory PeakSize Max_%dKB Dv(%dKB) @ %d"), (pmc.PeakWorkingSetSize/1024), ((pmc.PeakWorkingSetSize - m_physMemUsedByMe)/1024), check_point);
				m_physMemUsedByMe = pmc.PeakWorkingSetSize;
			}
			CloseHandle(hcur_process);
			IsMemoryLeak = true;
		}
		else            {
			if (pmc.PeakWorkingSetSize > m_physMemUsedByMe)
			{
				// 			SetLog(TEXT("_________*Currently Memory usage %lld (%lld) @ %d"), (pmc.PeakWorkingSetSize/1024), (pmc.PeakWorkingSetSize - m_physMemUsedByMe)/1024, check_point);
				// 			SetLog(TEXT("_________*Currently Memory Useage Max_%dKB Dv(%dByte) @ %d"), (pmc.PeakWorkingSetSize/1024), (pmc.PeakWorkingSetSize - m_physMemUsedByMe), check_point);
				SetLog(TEXT("_________*Currently Memory PeakSize Max_%dKB Dv(%dKB) @ %d"), (pmc.PeakWorkingSetSize/1024), ((pmc.PeakWorkingSetSize - m_physMemUsedByMe)/1024), check_point);

				m_physMemUsedByMe = pmc.PeakWorkingSetSize;
				CloseHandle(hcur_process);
				IsMemoryLeak = true;
				// 			return true;
			}
			// WorkSize Memory Check 
			// 			if(m_physMemWorkSize!=0){
			// 				if(pmc.WorkingSetSize>m_physMemWorkSize){
			// 					SetLog(TEXT("_________*Currently Memory WorkSize Max_%dKB Dv(%dByte) @ %d"), (pmc.WorkingSetSize/1024), (pmc.WorkingSetSize - m_physMemWorkSize), check_point);
			// 
			// 					m_physMemWorkSize = pmc.WorkingSetSize;
			// 					CloseHandle(hcur_process);
			// 					IsMemoryLeak = true;
			// 					// 				return true;
			// 				}
			// 			}
		}

		CloseHandle(hcur_process);
		// 		return false;
		return IsMemoryLeak;
	}
}

bool CTheLogger::ResourceMonitor(int check_point)
{
#if(1)
	MomoryUsageLog(check_point);
#endif


	DWORD   handle_cnt = 0;
	HANDLE   hcur_process = GetCurrentProcess();

	if (hcur_process) {
		GetProcessHandleCount(hcur_process, &handle_cnt);

		if (handle_cnt > m_nHandleCount)
		{
// 			SetLog(TEXT("_________*Inspection Handle count %d (%d) @ %d"), handle_cnt, handle_cnt - m_nHandleCount, check_point);
			SetLog(TEXT("_________*Inspection Handle count Max_%d Dv_(%d) @ %d"), handle_cnt, handle_cnt - m_nHandleCount, check_point);
			m_nHandleCount = handle_cnt;
			CloseHandle(hcur_process);
			return true;
		}
		CloseHandle(hcur_process);
		return false;
	}
}



// void CTheLogger::HexDump(void *ptr, size_t buflen)
// {  
// 	TCHAR outBuf[200];
// 	TCHAR *buf = (TCHAR*)ptr;
// 	
// 	size_t i, j, pos;  
// 
// 	for (i = 0; i < buflen; i += 16)
// 	{    
// 		// address & offset
// #ifdef _UNICODE
// 		pos = swprintf_s(outBuf, 200, TEXT("0x%08X, x0%06X: "), &buf[i], i);
// #else
// 		pos = sprintf(outBuf, TEXT("0x%08X, x0%06X: "), &buf[i], i);
// #endif
// 		// hexadecimal dump
// 		for (j = 0; j < 16; j++)
// 		{
// 			if (i+j < buflen)
// #ifdef _UNICODE
// 				pos += swprintf_s(&outBuf[pos], 200 - pos, TEXT("%02X "), buf[i + j]);
// 			else        
// 				pos += swprintf_s(&outBuf[pos], 200-pos, TEXT("   "));
// #else
// 				pos += sprintf_s(&outBuf[pos], 200 - pos, TEXT("%02X "), buf[i + j]);
// 			else
// 				pos += sprintf_s(&outBuf[pos], 200 - pos, "   ");
// #endif
// 		}
// #ifdef _UNICODE
// 		pos += wprintf_s(&outBuf[pos], 200-pos, TEXT(" "));
// #else
// 		pos += printf_s(&outBuf[pos], 200 - pos, TEXT(" "));
// #endif
// 
// 
// 		// ASCII dump
// 		for (j = 0; j < 16; j++)
// 		{
// 			if (i+j < buflen)
// #ifdef _UNICODE
// 				pos += swprintf_s(&outBuf[pos], 200-pos, TEXT("%c"), isprint(buf[i+j]) ? buf[i+j] : TEXT('.'));
// #else
// 				pos += sprintf_s(&outBuf[pos], 200 - pos, TEXT("%c"), isprint(buf[i + j]) ? buf[i + j] : TEXT('.'));
// #endif
// 		}
// 
// 		SetLog(outBuf);
// 	}
// }

//void CTheLogger::SystemErrorLog(HRESULT hr)
//{
//	if (FAILED(hr))
//	{
//		TCHAR szErr[MAX_ERROR_TEXT_LEN];
//		DWORD res = AMGetErrorText(hr, szErr, MAX_ERROR_TEXT_LEN);
//		if (res == 0)
//		{
//			StringCbPrintf(szErr, MAX_ERROR_TEXT_LEN, "Unknown Error: 0x%x", hr);
//		}
//		CTheLogger::SetLog(hr, szErr);
//	}
//}

void CTheLogger::SystemErrorLog(int gdiStatus)
{
	switch(gdiStatus)
	{
	case 0/* Ok*/: return;
	case 1/*GenericError  = 1,*/:
		CTheLogger::SetLog(1, TEXT("GDI+,Indicates that there was an error on the method call, which is identified as something other than those defined by the other elements of this enumeration."));
		break;
	case 2/*InvalidParameter = 2,*/:
		CTheLogger::SetLog(2, TEXT("GDI+,Indicates that one of the arguments passed to the method was not valid."));
		break;
	case 3/*OutOfMemory = 3,*/:
		CTheLogger::SetLog(3, TEXT("GDI+,Indicates that the operating system is out of memory and could not allocate memory to process the method call. For an explanation of how constructors use the OutOfMemory status, see the Remarks section at the end of this topic."));
		break;
	case 4/*ObjectBusy = 4,*/:
		CTheLogger::SetLog(4, TEXT("GDI+,Indicates that one of the arguments specified in the application programming interface (API) call is already in use in another thread."));
		break;
	case 5/*InsufficientBuffer = 5,*/:
		CTheLogger::SetLog(5, TEXT("GDI+,Indicates that a buffer specified as an argument in the API call is not large enough to hold the data to be received."));
		break;
	case 6/*NotImplemented = 6,*/:
		CTheLogger::SetLog(6, TEXT("GDI+,Indicates that the method is not implemented."));
		break;
	case 7/*Win32Error = 7,*/:
		CTheLogger::SetLog(7, TEXT("GDI+,Indicates that the method generated a Microsoft Win32 error."));
		SystemErrorLog();
		break;
	case 8/*WrongState = 8,*/:
		CTheLogger::SetLog(8, TEXT("GDI+,Indicates that the object is in an invalid state to satisfy the API call. For example, calling Pen::GetColor from a pen that is not a single, solid color results in a WrongState status."));
		break;
	case 9/*Aborted = 9,*/:
		CTheLogger::SetLog(9, TEXT("GDI+,Indicates that the method was aborted."));
		break;
	case 10/*FileNotFound = 10,*/:
		CTheLogger::SetLog(10, TEXT("GDI+,Indicates that the specified image file or metafile cannot be found."));
		break;
	case 11/*ValueOverflow = 11,*/:
		CTheLogger::SetLog(11, TEXT("GDI+,Indicates that the method performed an arithmetic operation that produced a numeric overflow."));
		break;
	case 12/*AccessDenied = 12,*/:
		CTheLogger::SetLog(12, TEXT("GDI+,Indicates that a write operation is not allowed on the specified file."));
		break;
	case 13/*UnknownImageFormat = 13,*/:
		CTheLogger::SetLog(13, TEXT("GDI+,Indicates that the specified image file format is not known."));
		break;
	case 14/*FontFamilyNotFound = 14,*/:
		CTheLogger::SetLog(14, TEXT("GDI+,Indicates that the specified font family cannot be found. Either the font family name is incorrect or the font family is not installed."));
		break;
	case 15/*FontStyleNotFound = 15,*/:
		CTheLogger::SetLog(15, TEXT("GDI+,Indicates that the specified style is not available for the specified font family."));
		break;
	case 16/*NotTrueTypeFont = 16,*/:
		CTheLogger::SetLog(16, TEXT("GDI+,Indicates that the font retrieved from an HDC or LOGFONT is not a TrueType font and cannot be used with GDI+."));
		break;
	case 17/*UnsupportedGdiplusVersion = 17,*/:
		CTheLogger::SetLog(17, TEXT("GDI+,Indicates that the version of GDI+ that is installed on the system is incompatible with the version with which the application was compiled."));
		break;
	case 18/*GdiplusNotInitialized = 18,*/:
		CTheLogger::SetLog(18, TEXT("GDI+,Indicates that the GDI+API is not in an initialized state. To function, all GDI+ objects require that GDI+ be in an initialized state. Initialize GDI+ by calling GdiplusStartup."));
		break;
	case 19/*PropertyNotFound = 19,*/:
		CTheLogger::SetLog(19, TEXT("GDI+,Indicates that the specified property does not exist in the image."));
		break;
	case 20/*PropertyNotSupported = 20,*/:
		CTheLogger::SetLog(20, TEXT("GDI+,Indicates that the specified property is not supported by the format of the image and, therefore, cannot be set."));
		break;
	case 21/*ProfileNotFound = 21*/:
		CTheLogger::SetLog(21, TEXT("GDI+,Indicates that the color profile required to save an image in CMYK format was not found."));
		break;
	default:
		CTheLogger::SetLog(22, TEXT("GDI+,Unknown GDI+ Error!!!"));
		break;
	}
}

/*
unsigned int CTheLogger::CpuIDSupported(void)
{
	unsigned int MaxInputValue = 0;
	__try
	{
#if defined (_M_IX86)
		__asm
		{
			xor eax, eax;	// call cpuid with eax = 0
			cpuid;
			mov MaxInputValue, eax;
		}
#else
#pragma message(TEXT("x64 CpuIDSupported 함수가 구현되지 않았습니다."))
#endif
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return (0);
	}
	return MaxInputValue;
}

// GenuineIntel will return 0 if the processor is not gunuine intel processor
unsigned int CTheLogger::GenuineIntel(void)
{
	unsigned int MaxInputVal = 0;
	unsigned int VendorID[3] = {0, 0, 0};
	__try	// if cpuid instruction is supported
	{
#if defined (_M_IX86)
		__asm
		{
			xor eax, eax;	// call cpuid with eax = 0
			cpuid;			// get vendor id string
			mov MaxInputVal, eax;	// cpu type
			mov VendorID, ebx;
			mov VendorID + 4, edx;
			mov VendorID + 8, ecx;
		}
#else
#pragma message(TEXT("x64용 GenuineIntel() 함수가 구현되지 않았습니다."))
#endif
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return(0);	// cpuid instruction is unavailable
	}
	//switch(MaxInputVal)
	//{
	//case 0x1:	SetLog("Higher than 486 or pentium");	break;
	//case 0x2:	SetLog("Pentium Pro & pentium II processor, Intel Celelon processor");
	//	SetLog("Pentium 4, Xeon, pentium M processor");	break;
	//	break;
	//case 0x3:	SetLog("Pentium III processor");	break;
	//case 0x5:	SetLog("Pentium 4 Processor supporting with HT technology");
	//			SetLog("Pentium D Processor(8xx)");	break;
	//case 0x6:	SetLog("Pentium D Processor(9xx)");	break;
	//case 0xa:	SetLog("Intel core Duo processor");
	//			SetLog("Intel core Duo 2 processor");
	//			SetLog("Intel Xeon processor 3000, 5100, 5300");	break;
	//}
	// 'GenuineIntel'
	return ((VendorID[0] == 0x756e6547) && (VendorID[1] == 0x49656e69) && (VendorID[2] == 0x6c65746e));
}

#define HWD_MT_BIT (0x10000000)	// bit 28
void CTheLogger::CPUID(void)
{
	unsigned int regEDX = 0;

	if ((CpuIDSupported() >= 1) && GenuineIntel())
	{
#if defined (_M_IX86)
		__asm
		{
			mov eax, 1;
			cpuid;
			mov regEDX, edx;
		}
#else
#pragma message(TEXT("x64용 CPUID() 함수가 구현되지 않았습니다."))
#endif
	}
	(regEDX & HWD_MT_BIT) ? SetLog(TEXT("Hyper threading support")) : SetLog(TEXT("Does not support Hyper threading"));
}

int CTheLogger::GetSimdExtNo()
{
	unsigned int regECX = 0, regEDX = 0;
	__try
	{
#if defined (_M_IX86)
		__asm
		{
			mov		eax, 1;
			cpuid;
			mov		regECX, ecx;
			mov		regEDX, edx;
		}
#else
#pragma message(TEXT("x64용 GetSimdExtNo() 함수가 구현되지 않았습니다."))
#endif
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return(-1);
	}

	if (regECX & 0x201)	// bit 0 or bit 9 1 means SSE3 supporting.
	{
		if (regECX & 0x180000)	return(4); // bit 19, 20 1 means SS4 supporting.
		else					return(3);
	}
	else if (regEDX & 0x2000000) return 2;	// bit 26 SSE2 support
	else if (regEDX & 0x1000000) return 1;	// bit 25 SSE suppot

	return(0);
}
*/


















