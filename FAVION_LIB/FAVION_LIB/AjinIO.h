#pragma once
#pragma comment (lib, "AXL.lib")
//--------------------------------------------------------------------
#include "AXL.h"
#include "AXD.h"
//---------------------------------------------------------------------
#define NUMBEROF_IO_BOARD 1
#define NUMBEROF_IO_CHANNEL 16
//---------------------------------------------------------------------
class CAjinIO
{
private  :
protected:
public   :

	CAjinIO(void);
	~CAjinIO(void);
	bool Init  ();
	bool Input (int BoardId, int BitId           );
	bool Output(int BoardId, int BitId           );
	bool Output(int BoardId, int BitId, bool IsOn);
	//bool OutPut();

	DWORD dwStatus  ;
	long ModuleCount;

};
//---------------------------------------------------------------------------
extern CAjinIO AjinIO;


