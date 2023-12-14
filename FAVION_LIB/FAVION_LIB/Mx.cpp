
#include "StdAfx.h"
#include "Mx.h"

TQJ71E71TCP QJ71E71;

CString BLOCK_READ_BIT   ;
CString BLOCK_READ_CHAR  ;
CString BLOCK_WRITE_BIT  ;
CString BLOCK_WRITE_CHAR ;
CString BLOCK_WRITE_CHECK;

//int BLOCK_READ_ADDRESS_B ;
//int BLOCK_READ_ADDRESS_C ;
//int BLOCK_WRITE_ADDRESS_B;
//int BLOCK_WRITE_ADDRESS_C;

//---------------------------------------------------------------------------

TQJ71E71TCP::TQJ71E71TCP(void)
{
}
//---------------------------------------------------------------------------

TQJ71E71TCP::~TQJ71E71TCP(void)
{

}
//---------------------------------------------------------------------------

//bool TQJ71E71TCP::Open(TActQJ71E71TCP *lpQJ71E71TCP)
bool TQJ71E71TCP::Open(CActUtlType* lpQNUDECPUTCP)
{
    //TCP = lpQJ71E71TCP;
    TCP = lpQNUDECPUTCP;
    
    if (TCP               ==NULL) return false;
    if (HostAddress.Trim()== "" ) return false;
    if (CpuType           ==0   ) return false;

    TCP->put_ActLogicalStationNumber(1);

	//GOO 
	//TCP->ActLogicalStationNumber = 1;

    /*
    TCP->ActCpuType = CpuType;
    TCP->ActHostAddress = (WideString)HostAddress;
    TCP->ActIONumber            = IONumber           ; //1023
    TCP->ActNetworkNumber       = NetworkNumber      ; //1
    TCP->ActStationNumber       = StationNumber      ; //1
    TCP->ActSourceNetworkNumber = SourceNetworkNumber; //1
    TCP->ActSourceStationNumber = SourceStationNumber; //2
   */



    lRet = TCP->Open();
    IsConnect = (lRet == 0x00);
    TCP->GetCpuType(&CpuName, &CpuType);

    return IsConnect;
}
//---------------------------------------------------------------------------
bool TQJ71E71TCP::Close(void)
{
    if (!IsConnect) return false;

    lRet = TCP->Close();
    IsConnect = false;
    return (lRet == 0x00);
}

//---------------------------------------------------------------------------

bool TQJ71E71TCP::ReadBlock(CString Address, int Count, long *Data)
{
    if (!IsConnect) return false;

    lRet  = TCP->ReadDeviceBlock(LPTSTR(LPCTSTR(Address)), Count, Data);
    return (lRet == 0x00);
}
//---------------------------------------------------------------------------

bool TQJ71E71TCP::ReadRandom(CString Address, long Count, long  *Data)
{
    if (!IsConnect) return false;

    lRet = TCP->ReadDeviceRandom(LPTSTR(LPCTSTR(Address)), Count, Data);
    return (lRet == 0x00);
}
//---------------------------------------------------------------------------

bool TQJ71E71TCP::WriteBlock(CString Address, int Count, long  *Data)
{
    if (!IsConnect) return false;

    lRet = TCP->WriteDeviceBlock(LPTSTR(LPCTSTR(Address)), Count, Data);
    return (lRet == 0x00);
}
//---------------------------------------------------------------------------

bool TQJ71E71TCP::WriteRandom(CString Address, long Count, long  *Data)
{
    if (!IsConnect) return false;

    lRet = TCP->WriteDeviceRandom(LPTSTR(LPCTSTR(Address)), Count, Data);
    return (lRet == 0x00);
}
//---------------------------------------------------------------------------

bool TQJ71E71TCP::SetDevice(CString Address, long Data)
{
    if (!IsConnect) return false;

    lRet = TCP->SetDevice(LPTSTR(LPCTSTR(Address)),  Data);
    return (lRet == 0x00);
}
//---------------------------------------------------------------------------

bool TQJ71E71TCP::GetDevice(CString Address, long *Data)
{

    if (!IsConnect) return false;

    lRet = TCP->GetDevice(LPTSTR(LPCTSTR(Address)),  Data);
    return (lRet == 0x00);
}
//---------------------------------------------------------------------------
bool TQJ71E71TCP::SetDevice16Bit(CString Address, long Value)
{
	/*
    long lData = Value;

    CString sType = Address.SubString(1,1).UpperCase();

    //lData =  Value & 0xffffffff;
    //lData = (lData & 0xffff)<<16;

    SetDevice(Address, lData);

    return true;
	*/
	return true;
}
//---------------------------------------------------------------------------

bool TQJ71E71TCP::SetString(CString Address, CString Value, int iLen)
{

	/*
    long  lData[100];
    char  sData[100];

    memset(sData, 0, sizeof(sData));
    if (Address.Length()!=4) return false;

    if (iLen == 0) iLen = Value.Length();
    int  iWriteLen = iLen/2 +(iLen%2);

    strcpy(sData, Value.c_str());

    for(int i=0;i<iWriteLen;i++)
    {
        lData[i] = sData[i*2+1];
        lData[i] = lData[i]<<8  | sData[i*2];
    }

    WriteBlock(Address, iWriteLen, lData);

    return true;
	*/
	return true;
}
//---------------------------------------------------------------------------

void  TQJ71E71TCP::OnLoad(bool IsLoad, CString Path)
{
    if (Path.Trim()=="") Path = PvPath;
    PvPath = Path;
    if (Path=="") return;

    //HIFile Ini;
	CIFile Ini;

    if (IsLoad) {
        Ini.Load(IsLoad, Path, "TCP_CONFIG", "IONumber           ", &IONumber           );
        Ini.Load(IsLoad, Path, "TCP_CONFIG", "NetworkNumber      ", &NetworkNumber      );
        Ini.Load(IsLoad, Path, "TCP_CONFIG", "SourceNetworkNumber", &SourceNetworkNumber);
        Ini.Load(IsLoad, Path, "TCP_CONFIG", "SourceStationNumber", &SourceStationNumber);
        Ini.Load(IsLoad, Path, "TCP_CONFIG", "StationNumber      ", &StationNumber      );

        if (IONumber           <1) IONumber           = 1023;
        if (NetworkNumber      <1) NetworkNumber      = 10;
        if (SourceNetworkNumber<1) SourceNetworkNumber= 1;
        if (SourceStationNumber<1) SourceStationNumber= 2;
        if (StationNumber      <1) StationNumber      = 1;

        CString CpuTypeStr = "";
        Ini.Load(IsLoad, Path, CString("TCP_CONFIG"), CString("CpuType"    ), &CpuTypeStr);
        //CpuType = CString("0x"+CpuTypeStr).ToIntDef(0);
		//CpuType = _ttoi(CString("0x"+CpuTypeStr));
		CpuType = _ttoi(CpuTypeStr);

        Ini.Load(IsLoad, Path, CString("TCP_CONFIG"), CString("HostAddress"), &HostAddress);
        Ini.Load(IsLoad, Path, "MEMORY", "BLOCK_READ_CHAR"  , &BLOCK_READ_CHAR );
        Ini.Load(IsLoad, Path, "MEMORY", "BLOCK_READ_BIT"   , &BLOCK_READ_BIT  );
		Ini.Load(IsLoad, Path, "MEMORY", "BLOCK_WRITE_CHAR" , &BLOCK_WRITE_CHAR);
		Ini.Load(IsLoad, Path, "MEMORY", "BLOCK_WRITE_BIT"  , &BLOCK_WRITE_BIT );

        Ini.Load(IsLoad, Path, "MEMORY", "BLOCK_READ_ADDRESS_Bit " , &ReadAddrBit  );
        Ini.Load(IsLoad, Path, "MEMORY", "BLOCK_READ_ADDRESS_Char" , &ReadAddrChar );
        Ini.Load(IsLoad, Path, "MEMORY", "BLOCK_WRITE_ADDRESS_Bit" , &WriteAddrBit );
        Ini.Load(IsLoad, Path, "MEMORY", "BLOCK_WRITE_ADDRESS_Char", &WriteAddrChar);
        Ini.Load(IsLoad, Path, "MEMORY", "CHECK_WRITE_ADDRESS "    , &WriteCheck   );

		/*
        BLOCK_READ_ADDRESS1  = CString("0x"+ReadAddr1 ).ToIntDef(0);
        BLOCK_READ_ADDRESS2  = CString("0x"+ReadAddr2 ).ToIntDef(0);
        BLOCK_READ_ADDRESS3  = CString("0x"+ReadAddr3 ).ToIntDef(0);
        BLOCK_READ_ADDRESS4  = CString("0x"+ReadAddr4 ).ToIntDef(0);
        BLOCK_WRITE_ADDRESS1 = CString("0x"+WriteAddr1).ToIntDef(0);
        BLOCK_WRITE_ADDRESS2 = CString("0x"+WriteAddr2).ToIntDef(0);
        BLOCK_WRITE_ADDRESS3 = CString("0x"+WriteAddr3).ToIntDef(0);
        BLOCK_WRITE_ADDRESS4 = CString("0x"+WriteAddr4).ToIntDef(0);
        CHECK_WRITE_ADDRESS  = CString("0x"+WriteCheck).ToIntDef(0);
		*/

		// 추가 확인필요 
		//BLOCK_READ_ADDRESS1  = _ttoi(CString("0x"+ReadAddr1 ));
		//BLOCK_READ_ADDRESS2  = _ttoi(CString("0x"+ReadAddr2 ));
		//BLOCK_READ_ADDRESS3  = _ttoi(CString("0x"+ReadAddr3 ));
		//BLOCK_READ_ADDRESS4  = _ttoi(CString("0x"+ReadAddr4 ));
		//BLOCK_WRITE_ADDRESS1 = _ttoi(CString("0x"+WriteAddr1));
		//BLOCK_WRITE_ADDRESS2 = _ttoi(CString("0x"+WriteAddr2));
		//BLOCK_WRITE_ADDRESS3 = _ttoi(CString("0x"+WriteAddr3));
		//BLOCK_WRITE_ADDRESS4 = _ttoi(CString("0x"+WriteAddr4));
		//CHECK_WRITE_ADDRESS  = _ttoi(CString("0x"+WriteCheck));



        if (BLOCK_READ_BIT  .Trim ()=="") BLOCK_READ_BIT        = "B";
        if (BLOCK_READ_CHAR .Trim ()=="") BLOCK_READ_CHAR       = "W";
		if (BLOCK_WRITE_BIT .Trim ()=="") BLOCK_WRITE_BIT       = "B";
		if (BLOCK_WRITE_CHAR.Trim()=="")  BLOCK_WRITE_CHAR      = "W";
        //if (BLOCK_READ_ADDRESS_B  <0   ) BLOCK_READ_ADDRESS_B   = 0;
        //if (BLOCK_READ_ADDRESS_C  <0   ) BLOCK_READ_ADDRESS_C   = 0;
        //if (BLOCK_WRITE_ADDRESS_B <0   ) BLOCK_WRITE_ADDRESS_B  = 0;
        //if (BLOCK_WRITE_ADDRESS_C <0   ) BLOCK_WRITE_ADDRESS_C  = 0;
    } 
    else {
        Ini.Load(IsLoad, Path, "TCP_CONFIG", "IONumber           ", &IONumber           );
        Ini.Load(IsLoad, Path, "TCP_CONFIG", "NetworkNumber      ", &NetworkNumber      );
        Ini.Load(IsLoad, Path, "TCP_CONFIG", "SourceNetworkNumber", &SourceNetworkNumber);
        Ini.Load(IsLoad, Path, "TCP_CONFIG", "SourceStationNumber", &SourceStationNumber);
        Ini.Load(IsLoad, Path, "TCP_CONFIG", "StationNumber      ", &StationNumber      );
    }
}
