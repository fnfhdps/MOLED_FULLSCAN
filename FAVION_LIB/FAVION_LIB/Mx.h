#pragma once

#include "CActUtlType.h"
#include "LxHeader.h"


extern CString BLOCK_READ_BIT   ;
extern CString BLOCK_READ_CHAR  ;
extern CString BLOCK_WRITE_BIT  ;
extern CString BLOCK_WRITE_CHAR ;
extern CString BLOCK_WRITE_CHECK;

//extern int BLOCK_READ_ADDRESS_B ;
//extern int BLOCK_READ_ADDRESS_C ;
//extern int BLOCK_WRITE_ADDRESS_B;
//extern int BLOCK_WRITE_ADDRESS_C;
//extern int BLOCK_WRITE_ADDRESS_CH;

class TQJ71E71TCP
{

private :
	//TActQJ71E71TCP *TCP;
	//CActUtlType *TCP;

public:
	bool IsConnect;
	long lRet;
	long CpuType;
	BSTR CpuName;
	CString HostAddress;
	int IONumber           ; //1023
	int NetworkNumber      ; //1
	int SourceNetworkNumber; //2
	int SourceStationNumber; //2
	int StationNumber      ; //1
	CString PvPath       ;
	CString ReadAddrBit  ;
	CString ReadAddrChar ;
	CString WriteAddrBit ;
	CString WriteAddrChar;
	CString WriteCheck   ;


public:
	TQJ71E71TCP(void);
	~TQJ71E71TCP(void);

	//GOO 
	bool Open (CActUtlType *lpQNUDECPUTCP);
	bool Close();

	bool ReadBlock  (CString Address, int  Count, long *Data);
	bool ReadRandom (CString Address, long Count, long *Data);
	bool WriteBlock (CString Address, int  Count, long *Data);
	bool WriteRandom(CString Address, long Count, long *Data);

	bool GetDevice(CString Address, long *lData);
	bool SetDevice(CString Address, long  lData);

	bool SetDevice16Bit(CString Address, long Value);
	bool SetString(CString Address, CString  Value, int iLen = 0);

	void OnLoad(bool IsLoad, CString Path);

public:
	CActUtlType *TCP;
};

extern TQJ71E71TCP QJ71E71;

