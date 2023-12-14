// FAVION_LIB.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "StackWalker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
// Debug Error LogFile Start 
//-----------------------------------------------------------------------------
#define UNHANDLED_EXCEPTION_TEST
#define EXCEPTION_FILTER_TEST

// secure-CRT_functions are only available starting with VC8
#if _MSC_VER < 1400
#define _tcscpy_s _tcscpy
#define _tcscat_s _tcscat
#define _stprintf_s _stprintf
#define strcpy_s(a, b, c) strcpy(a, c)
#endif


// Specialized stackwalker-output classes
// Console (printf):
// class StackWalkerToConsole : public StackWalker
//  {
//  protected:
//  	virtual void OnOutput(LPCSTR szText)
//  	{
//  		printf("%s", szText);
//  	}
//  };

void Func5()
{
}
void Func4()
{
	Func5();
}
void Func3()
{
	Func4();
}
void Func2()
{
	Func3();
}
void Func1()
{
	Func2();
}

void StackWalkTest()
{
	Func1();
}

#ifdef UNHANDLED_EXCEPTION_TEST

// For more info about "PreventSetUnhandledExceptionFilter" see:
// "SetUnhandledExceptionFilter" and VC8
// http://blog.kalmbachnet.de/?postid=75
// and
// Unhandled exceptions in VC8 and above?for x86 and x64
// http://blog.kalmbach-software.de/2008/04/02/unhandled-exceptions-in-vc8-and-above-for-x86-and-x64/
// Even better: http://blog.kalmbach-software.de/2013/05/23/improvedpreventsetunhandledexceptionfilter/

#if defined _M_X64 || defined _M_IX86
static BOOL PreventSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
	if (hKernel32 == NULL) return FALSE;
	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if (pOrgEntry == NULL) return FALSE;

#ifdef _M_IX86
	// Code for x86:
	// 33 C0                xor         eax,eax  
	// C2 04 00             ret         4 
	unsigned char szExecute[] = { 0x33, 0xC0, 0xC2, 0x04, 0x00 };
#elif _M_X64
	// 33 C0                xor         eax,eax 
	// C3                   ret  
	unsigned char szExecute[] = { 0x33, 0xC0, 0xC3 };
#else
#error "The following code only works for x86 and x64!"
#endif

	DWORD dwOldProtect = 0;
	BOOL bProt = VirtualProtect(pOrgEntry, sizeof(szExecute),
		PAGE_EXECUTE_READWRITE, &dwOldProtect);

	SIZE_T bytesWritten = 0;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
		pOrgEntry, szExecute, sizeof(szExecute), &bytesWritten);

	if ((bProt != FALSE) && (dwOldProtect != PAGE_EXECUTE_READWRITE))
	{
		DWORD dwBuf;
		VirtualProtect(pOrgEntry, sizeof(szExecute), dwOldProtect, &dwBuf);
	}
	return bRet;
}
#else
#pragma message("This code works only for x86 and x64!")
#endif

static TCHAR s_szExceptionLogFileName[_MAX_PATH] = _T("\\exceptions.log");  // default
static BOOL s_bUnhandledExeptionFilterSet = FALSE;
static LONG __stdcall CrashHandlerExceptionFilter(EXCEPTION_POINTERS* pExPtrs)
{
#ifdef _M_IX86
	if (pExPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
	{
		static char MyStack[1024 * 128];  // be sure that we have enought space...
		// it assumes that DS and SS are the same!!! (this is the case for Win32)
		// change the stack only if the selectors are the same (this is the case for Win32)
		//__asm push offset MyStack[1024*128];
		//__asm pop esp;
		__asm mov eax, offset MyStack[1024 * 128];
		__asm mov esp, eax;
	}
#endif

	StackWalker sw;  // output to console
	sw.ShowCallstack(GetCurrentThread(), pExPtrs->ContextRecord);
	TCHAR lString[500];
	_stprintf_s(lString,
		_T("*** Unhandled Exception! See console output for more infos!\n")
		_T("   ExpCode: 0x%8.8X\n")
		_T("   ExpFlags: %d\n")
		_T("   ExpAddress: 0x%8.8X\n")
		_T("   Please report!"),
		pExPtrs->ExceptionRecord->ExceptionCode,
		pExPtrs->ExceptionRecord->ExceptionFlags,
		pExPtrs->ExceptionRecord->ExceptionAddress);
	FatalAppExit(-1, lString);
	return EXCEPTION_CONTINUE_SEARCH;
}

static void InitUnhandledExceptionFilter()
{
	TCHAR szModName[_MAX_PATH];
	if (GetModuleFileName(NULL, szModName, sizeof(szModName) / sizeof(TCHAR)) != 0)
	{
		_tcscpy_s(s_szExceptionLogFileName, szModName);
		_tcscat_s(s_szExceptionLogFileName, _T(".exp.log"));
	}
	if (s_bUnhandledExeptionFilterSet == FALSE)
	{
		// set global exception handler (for handling all unhandled exceptions)
		SetUnhandledExceptionFilter(CrashHandlerExceptionFilter);
#if defined _M_X64 || defined _M_IX86
		PreventSetUnhandledExceptionFilter();
#endif
		s_bUnhandledExeptionFilterSet = TRUE;
	}
}
#endif  // UNHANDLED_EXCEPTION_TEST

#ifdef EXCEPTION_FILTER_TEST
LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode)
{
	//StackWalker sw;  // output to default (Debug-Window)
	StackWalker sw;  // output to the console
	sw.ShowCallstack(GetCurrentThread(), pExp->ContextRecord);
	return EXCEPTION_EXECUTE_HANDLER;
}
void ExpTest5() { char *p = NULL; p[0] = 0; printf(p); }
void ExpTest4() { ExpTest5(); }
void ExpTest3() { ExpTest4(); }
void ExpTest2() { ExpTest3(); }
void ExpTest1() { ExpTest2(); }
void TestExceptionWalking()
{
	__try
	{
		ExpTest1();
	}
	__except (ExpFilter(GetExceptionInformation(), GetExceptionCode()))
	{
		printf("\n\nException-Handler called\n\n\n");
	}
}

int f(int i)
{
	if (i < 0) return i;
	return f(i + 1);
}
#endif  // EXCEPTION_FILTER_TEST
//----------------------------------------------------------------------------------
// Debug Error LogFile End 
//---------------------------------------------------------------------------------

static AFX_EXTENSION_MODULE FAVION_LIBDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("FAVION_LIB.DLL Initializing!\n");
		InitUnhandledExceptionFilter();
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(FAVION_LIBDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(FAVION_LIBDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("FAVION_LIB.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(FAVION_LIBDLL);
	}
	return 1;   // ok
}
