#include "StdAfx.h"
#include "AjinIO.h"

#pragma comment (lib,"AXL.lib")
CAjinIO AjinIO;

CAjinIO::CAjinIO(void)
{
}
CAjinIO::~CAjinIO(void)
{
}
bool CAjinIO::Init()
{
	if( AxlOpen(7) == AXT_RT_SUCCESS){ // Init
		if (AxdInfoIsDIOModule(&dwStatus) == AXT_RT_SUCCESS){// Module Fine
			if (dwStatus == STATUS_EXIST){
				if (AxdInfoGetModuleCount(&ModuleCount) == AXT_RT_SUCCESS){// Module Count Find
					if(NUMBEROF_IO_BOARD!=ModuleCount){
						//AxlInterruptEnable();
						//AxdiInterruptSetModule()
						MessageBox(NULL,L"Board Count Different", L"Error",MB_ICONERROR||MB_OK);
						return false;
					}
				}
			}
			else {
				MessageBox(NULL, L"AJin AXT_SIO_DB32P Board를 찾을수 없습니다", L"Error",MB_ICONERROR||MB_OK);

				return false;
			}
		}
		else {
			MessageBox(NULL, L"Board Count Different", L"Error",MB_ICONERROR||MB_OK);

			return false;
		}
	}
	else {
		MessageBox(NULL, L"I/0 Board Init Fail!", L"Error",MB_ICONERROR||MB_OK);

		return false;
	}
	return	true;
}
//----------------------------------------------------------------------------
bool CAjinIO::Input(int BoardId, int BitId)
{
	if (dwStatus != STATUS_EXIST) return false;
	DWORD Value;
	bool  IsOn ;

	AxdiReadInportBit(BoardId, BitId, &Value);

	if   (Value) IsOn = true ;
	else         IsOn = false;

	return IsOn;
}
//----------------------------------------------------------------------------
// Only AJin Board Status Check Function
//----------------------------------------------------------------------------
bool CAjinIO::Output(int BoardId, int BitId)
{
	if (dwStatus != STATUS_EXIST) return false;

	DWORD Value;
	bool  IsOn ;

	AxdoReadOutportBit(BoardId, BitId, &Value);

	if   (Value) IsOn = true ;
	else         IsOn = false;

	return IsOn;
}
//----------------------------------------------------------------------------
bool CAjinIO::Output(int BoardId, int BitId, bool On)
{
	if (dwStatus != STATUS_EXIST) return false;

	DWORD Value;
	bool  IsOn = true;

	AxdoWriteOutportBit(BoardId, BitId, On);

	return IsOn;
}
//---------------------------------------------------------------------------
