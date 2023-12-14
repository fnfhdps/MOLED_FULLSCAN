#include "StdAfx.h"
#include "Euresys.h"


CBoardInfo::CBoardInfo()
{
	McOpenDriver(NULL);
}

CBoardInfo::~CBoardInfo()
{
	McCloseDriver();
}

int CBoardInfo::GetBoardCount()
{
	int nBoardCount;
	MCSTATUS result;
	result = McGetParamNmInt(MC_CONFIGURATION, "BoardCount", &nBoardCount);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get Board count"),result);
		return 0;
	}
	return nBoardCount;
}

int CBoardInfo::GetPCIePayloadSize(int num)
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(MC_BOARD + num, MC_PCIePayloadSize, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get PCIePayloadSize"),result);
		return 0;
	}
	return nValue;
}

int CBoardInfo::GetPCIeLinkWidth(int num)
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(MC_BOARD + num, MC_PCIeLinkWidth, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get PCIeLinkWidth"),result);
		return 0;
	}
	return nValue;
}

CString CBoardInfo::GetBoardName(int num)
{
	char tmp[MAX_PATH];
	MCSTATUS result;
	result = McGetParamStr(MC_BOARD + num,MC_BoardName,tmp,MAX_PATH);;
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get BoardName"),result);
		return _T("");
	}

	return(CString)tmp;
}

CString CBoardInfo::GetBoardType(int num)
{
	char tmp[MAX_PATH];
	MCSTATUS result;
	result = McGetParamStr(MC_BOARD + num,MC_BoardType,tmp,MAX_PATH);;
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get BoardType"),result);
		return _T("");
	}

	return(CString)tmp;
}

CString CBoardInfo::GetBoardInformation(int num)
{
	MCSTATUS result;
	CString Info, name, identi;
	int index;
	char tmp[MAX_PATH];
	result = McGetParamNmInt(MC_CONFIGURATION, "DriverIndex", &index);

	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get Driver Index"),result);
		//return 0;
	}
	result = McGetParamNmStr(MC_CONFIGURATION, "BoardName", tmp, sizeof(tmp));
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get Board Name"),result);
		//return 0;
	}
	name = (CString)tmp;

	result = McGetParamNmStr(MC_CONFIGURATION, "BoardIdentifier", tmp, sizeof(tmp));
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get Board Identifier"),result);
		//return 0;
	}
	identi = (CString)tmp;

	Info.Format(_T("Index:%d"),index);
	Info = Info + _T(", Name:") + name + _T(", Identifier:") + identi;
	return Info;
}

void CBoardInfo::ShowErrorMessage(CString message, int error)
{
	CString errorMsg;
	errorMsg.Format(_T("%s: Error = %d: "), message, error);

	switch(error)
	{
	case MC_NO_BOARD_FOUND				:	errorMsg += _T("No Board Found"					);		break;
	case MC_BAD_PARAMETER				:	errorMsg += _T("Bad Parameter"					);		break;
	case MC_IO_ERROR					:	errorMsg += _T("I/O Error"						);		break;
	case MC_INTERNAL_ERROR				:	errorMsg += _T("Internal Error"					);		break;
	case MC_NO_MORE_RESOURCES			:	errorMsg += _T("No More Resources"				);		break;
	case MC_IN_USE						:	errorMsg += _T("Object still in use"			);		break;
	case MC_NOT_SUPPORTED				:	errorMsg += _T("Operation not supported"		);		break;
	case MC_DATABASE_ERROR				:	errorMsg += _T("Parameter database error"		);		break;
	case MC_OUT_OF_BOUND				:	errorMsg += _T("Value out of bound"				);		break;
	case MC_INSTANCE_NOT_FOUND			:	errorMsg += _T("Object instance not found"		);		break;
	case MC_INVALID_HANDLE				:	errorMsg += _T("Invalid Handle"					);		break;
	case MC_TIMEOUT						:	errorMsg += _T("Timeout"						);		break;
	case MC_INVALID_VALUE				:	errorMsg += _T("Invalid Value"					);		break;
	case MC_RANGE_ERROR					:	errorMsg += _T("Value not in range"				);		break;
	case MC_BAD_HW_CONFIG				:	errorMsg += _T("Invalid hardware configuration"	);		break;
	case MC_NO_EVENT					:	errorMsg += _T("No	Event"						);		break;
	case MC_LICENSE_NOT_GRANTED			:	errorMsg += _T("License not granted"			);		break;
	case MC_FATAL_ERROR					:	errorMsg += _T("Fatal error"					);		break;
	case MC_HW_EVENT_CONFLICT			:	errorMsg += _T("Hardware event conflict"		);		break;
	case MC_FILE_NOT_FOUND				:	errorMsg += _T("File not found"					);		break;
	case MC_OVERFLOW					:	errorMsg += _T("Overflow"						);		break;
	case MC_INVALID_PARAMETER_SETTING	:	errorMsg += _T("Parameter inconsistency"		);		break;
	case MC_PARAMETER_ILLEGAL_ACCESS	:	errorMsg += _T("Illegal operation"				);		break;
	case MC_CLUSTER_BUSY				:	errorMsg += _T("Cluster busy"					);		break;
	case MC_SERVICE_ERROR				:	errorMsg += _T("MultiCam service error"			);		break;
	case MC_INVALID_SURFACE				:	errorMsg += _T("Invalid surface"				);		break;
	}
	AfxMessageBox(errorMsg, MB_OKCANCEL | MB_ICONINFORMATION);
}


CEuresys::CEuresys(void)
{
	m_nGrabCount= 0;
	m_hChannel	= 0;
	//m_nSize		= 0;
	m_pBuffer	= NULL;
	m_bOpened	= false;
	IsTrigger   = false;

	//memset(m_pImage, 0, sizeof(m_pImage));

	OpenDriver();
}
CEuresys::~CEuresys(void)
{
	m_bOpened = false;

	SetIdle();
	DeleteChannel();
	CloseDriver();

	/*
	for (int i=0; i<MAX_BUFFER; i++)
	{
		if (m_pImage[i] != NULL)
		{
			delete []m_pImage[i];
			m_pImage[i] = NULL;
		}
	}
	*/
}

char* CEuresys::CStringToPChar(CString str)
{
	//GOO 
	WCHAR  *wchar_str; 
// 	CHAR   * char_mul;
// 	int char_str_len;  

	wchar_str = str.GetBuffer(0);  //CString =>WCHAR 변환 
// 	strcpy(m_pTmp, CW2A(str));
	strcpy(m_pTmp, CW2A(wchar_str));
	// 	char_str = CW2A(str); //유니코드를 멀티바이트 변환 
// 	m_pTmp = *char_str;
	// 
	//char_str_len = WideCharToMultiByte(CP_ACP, 0, LPWSTR(LPCWSTR(wchar_str)) , -1, NULL, 0, NULL, NULL);
	//WideCharToMultiByte(CP_ACP, 0,LPWSTR(LPCWSTR(wchar_str)), -1, m_pTmp, char_str_len, 0,0);  
	//char *char_str;
	//char_str = LPSTR(LPCSTR(str));
	//m_pTmp = char_str;

// 	char* char_str;
// 	char_str = LPSTR(LPCSTR(str));
// 	m_pTmp = char_str;
	return m_pTmp;
}
void CEuresys::SetErrorHandling(int num)
{
	switch(num)
	{
	case 0 :
		McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_NONE);
		break;
	case 1 :
		McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_MSGBOX);
		break;
	case 2 :
		McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_EXCEPTION);
		break;
	case 3 :
		McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_MSGEXCEPTION);
		break;
	}
	McSetParamStr (MC_CONFIGURATION, MC_ErrorLog, "error.log");
}

bool CEuresys::OpenDriver()
{
	MCSTATUS result;
	result = McOpenDriver(NULL);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("McOpenDriver Error"),result);
		return false;
	}

	SetErrorHandling(0);

	return true;
}

bool CEuresys::CloseDriver()
{
	MCSTATUS result;
	result = McCloseDriver();
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("McCloseDriver Error"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetOutputConfig(int channel)
{
	MCSTATUS result;
	result = McSetParamInt(MC_BOARD, MC_OutputConfig + channel, MC_OutputConfig_SOFT);
	if (result != MC_OK)
	{
		TRACE(_T("Could not set Output config for SOFT\n"));
		return false;
	}
	result = McSetParamInt(MC_BOARD, MC_OutputStyle + channel, MC_OutputStyle_OPTO );	
	if (result != MC_OK)
	{
		TRACE(_T("Could not set Output style for OPTO\n"));
		return false;
	}

	return true;
}

bool CEuresys::SetOutputStatus(int channel, bool status)
{
	bool ret = SetOutputConfig(channel);
	if (ret == TRUE)
	{
		if (status == true)
		{
			MCSTATUS result = McSetParamInt(MC_BOARD, MC_OutputState + channel, MC_OutputState_HIGH);
			if (result != MC_OK)
			{
				TRACE(_T("Could not set output signal HIGH\n"));
				return false;
			}
		}
		else
		{
			MCSTATUS result = McSetParamInt(MC_BOARD, MC_OutputState + channel, MC_OutputState_LOW);
			if (result != MC_OK)
			{
				TRACE(_T("Could not set output signal LOW\n"));
				return false;
			}
		}
	}
	else
	{
		TRACE(_T("Could not set Output config\n"));
		return false;
	}

	return true;
}

bool CEuresys::InitBoard(int index, CString connector, CString topology, CString camfilePath, int ExposeTime)
{
	MCSTATUS result;

	char* tmp = CStringToPChar(topology);

	//result = McSetParamStr(MC_BOARD + index, MC_BoardTopology, topology);
	result = McSetParamInt(MC_BOARD + index, MC_BoardTopology, MC_BoardTopology_1_1_1_1);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set BoardTopology"),result);
		return false;
	}

	McCreate(MC_CHANNEL, &m_hChannel);
	result = McSetParamInt(m_hChannel, MC_DriverIndex, index);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set DriverIndex"),result);
		return false;
	}

	tmp = CStringToPChar(connector);

	result = McSetParamStr(m_hChannel, MC_Connector, CW2A(connector));
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set Connector"),result);
		return false;
	}

	tmp = CStringToPChar(camfilePath);

	result = McSetParamStr(m_hChannel, MC_CamFile, CW2A(camfilePath));
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not load camfile"),result);
		return false;
	}

	result = McSetParamInt(m_hChannel, MC_Expose_us, ExposeTime);
	
	result = McSetParamInt(m_hChannel, MC_SurfaceCount, 4);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set SurfaceCount"),result);
		return false;
	}

	McGetParamInt(m_hChannel,MC_ImageSizeX, &m_nSizeX);
	McGetParamInt(m_hChannel,MC_ImageSizeY, &m_nSizeY);
	//McSetParamInt(m_hChannel,MC_BufferSize, 1632     );
	McGetParamInt(m_hChannel,MC_BufferPitch, &m_nBufferPitch);

	try
	{
		/*
		for (int i=0; i<MAX_BUFFER; i++)
		{
			if (m_pImage[i] != NULL)
			{
				delete []m_pImage[i];
				m_pImage[i] = NULL;
			}

			m_pImage[i] = new BYTE[m_nSize];
			memset(m_pImage[i],0,m_nSize);
		}
		*/
	}
	catch (CMemoryException* e)
	{
		LPTSTR lptstr = NULL;
		e->GetErrorMessage(lptstr,MAX_PATH);

		CString msg;
		msg = (CString)lptstr;
		AfxMessageBox(msg);
		return false;
	}

	McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_START_ACQUISITION_SEQUENCE, MC_SignalEnable_ON);
	McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING		 , MC_SignalEnable_ON);
	McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_END_CHANNEL_ACTIVITY		 , MC_SignalEnable_ON);
	McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE		 , MC_SignalEnable_ON);

	McRegisterCallback(m_hChannel, GlobalCallback, this);

	m_bOpened = true;
	IsTrigger = false;

	return true;	
}

void WINAPI CEuresys::GlobalCallback(PMCSIGNALINFO SigInfo)
{
	if (SigInfo && SigInfo->Context)
	{
		CEuresys* pApp = (CEuresys*) SigInfo->Context;
		pApp->Callback (SigInfo);
	}
}


void CEuresys::Callback(PMCSIGNALINFO SigInfo)
{
	switch(SigInfo->Signal)
	{
	case MC_SIG_SURFACE_PROCESSING:
		McGetParamInt (SigInfo->SignalInfo, MC_SurfaceAddr, (PINT32) &m_pBuffer); 
	    IImage.SetImagePtr(m_nSizeX, m_nSizeY,m_pBuffer, m_nBufferPitch*8);
		IsTrigger = true;
		//memcpy(m_pImage[m_nGrabCount++], m_pBuffer, m_nSize);

		if (m_nGrabCount == MAX_BUFFER)
			m_nGrabCount = 0;
		break;

	case MC_SIG_END_CHANNEL_ACTIVITY :
		break;

	case MC_SIG_ACQUISITION_FAILURE:
		break;
	case MC_SIG_START_ACQUISITION_SEQUENCE :
		m_nGrabCount = 0;
		break;
	default:
		break;
	}
}

bool CEuresys::SetActive()
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_ChannelState, MC_ChannelState_ACTIVE);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set ACTIVE"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetIdle()
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_ChannelState, MC_ChannelState_IDLE);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set IDLE"),result);
		return false;
	}
	return true;
}

bool CEuresys::OnTrigger()
{
	MCSTATUS result;
	int chState;

	result = McGetParamInt(m_hChannel, MC_ChannelState, &chState);

	if(chState == MC_ChannelState_ACTIVE){
		result = McSetParamInt(m_hChannel, MC_ChannelState, MC_ChannelState_IDLE);
	}
	result = McSetParamInt(m_hChannel, MC_ChannelState, MC_ChannelState_ACTIVE);
	result = McSetParamInt(m_hChannel, MC_ForceTrig, MC_ForceTrig_TRIG);
	//result = McSetParamInt(m_hChannel, MC_ChannelState, MC_ChannelState_IDLE);

	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set Force Trigger"),result);
		return false;
	}
	return true;
}

int CEuresys::GetImageSizeX()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_ImageSizeX, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get ImageSizeX"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetImageSizeY()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_ImageSizeY, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get ImageSizeY"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetHactive_Px()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_Hactive_Px, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get Hactive_Px"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetHsyncAft_Tk()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_HsyncAft_Tk, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get HsyncAft_Tk"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetVactive_Ln()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_Vactive_Ln, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get Vactive_Ln"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetVsyncAft_Ln()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_VsyncAft_Ln, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get VsyncAft_Ln"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetBufferPitch()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_BufferPitch, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get BufferPitch"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetSurfaceCount()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_SurfaceCount, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get SurfaceCount"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetFrameRate_mHz()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_FrameRate_mHz, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get FrameRate_mHz"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetExpose_us()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_Expose_us, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get Expose_us"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetExposeRecovery_us()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_ExposeRecovery_us, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get ExposeRecovery_us"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetReadoutRecovery_us()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_ReadoutRecovery_us, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get ReadoutRecovery_us"),result);
		return 0;
	}
	return nValue;
}

int CEuresys::GetBitPerPixel()
{
	CString value = GetColorFormat();
	if (value == _T("Y8"))
		return 8;
	else if (value == _T("RGB24"))
		return 24;
	else
		return 0;
}

CString CEuresys::GetColorFormat()
{
	char tmp[10];
	MCSTATUS result;
	result = McGetParamStr(m_hChannel,MC_ColorFormat,tmp,10);;
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get ColorFormat"),result);
		return _T("");
	}

	return(CString)tmp;
}

CString CEuresys::GetTrigMode()
{
	char tmp[20];
	MCSTATUS result;
	result = McGetParamStr(m_hChannel,MC_TrigMode,tmp,20);;
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get TrigMode"),result);
		return _T("");
	}

	return(CString)tmp;
}

CString CEuresys::GetNextTrigMode()
{
	char tmp[50];
	MCSTATUS result;
	result = McGetParamStr(m_hChannel,MC_NextTrigMode,tmp,50);;
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get NextTrigMode"),result);
		return _T("");
	}

	return(CString)tmp;
}

CString CEuresys::GetCamfilePath()
{
	char tmp[MAX_PATH];
	MCSTATUS result;
	result = McGetParamStr(m_hChannel,MC_CamFile,tmp,MAX_PATH);;
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get Camfile Path"),result);
		return _T("");
	}

	return(CString)tmp;
}

int CEuresys::GetSeqLength_Fr()
{
	int nValue;
	MCSTATUS result;
	result = McGetParamInt(m_hChannel, MC_SeqLength_Fr, &nValue);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not get SeqLength_Fr"),result);
		return 0;
	}
	return nValue;
}

bool CEuresys::SetWindowX_Px(int value)
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_WindowX_Px, value);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set WindowX_Px"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetFrameRate_mHz(int value)
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_FrameRate_mHz, value);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set FrameRate_mHz"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetWindowY_Ln(int value)
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_WindowY_Ln, value);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set WindowY_Ln"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetSeqLength_Fr(int value)
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_SeqLength_Fr, value);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set SeqLength_Fr"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetSoftwareTriggerMode()
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_TrigMode, MC_TrigMode_SOFT);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set SOFT mode"),result);
		return false;
	}

	result = McSetParamInt(m_hChannel, MC_NextTrigMode, MC_NextTrigMode_SOFT);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set Next SOFT mode"),result);
		return false;
	}

	return true;
}

bool CEuresys::SetHardwareTriggerMode()
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_TrigMode, MC_TrigMode_HARD);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set HARD mode"),result);
		return false;
	}

	result = McSetParamInt(m_hChannel, MC_NextTrigMode, MC_NextTrigMode_HARD);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set Next HARD mode"),result);
		return false;
	}

	return true;
}

bool CEuresys::SetImmediateMode()
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_TrigMode, MC_TrigMode_IMMEDIATE);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set IMMEDIATE mode"),result);
		return false;
	}

	result = McSetParamInt(m_hChannel, MC_NextTrigMode, MC_NextTrigMode_IMMEDIATE);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set Next IMMEDIATE mode"),result);
		return false;
	}

	return true;
}

bool CEuresys::SetTriggerLine(CString line)
{
	MCSTATUS result;

	if (line == _T("DIN1"))
		result = McSetParamInt(m_hChannel, MC_TrigLine, MC_TrigLine_DIN1);
	else if (line == _T("DIN2"))
		result = McSetParamInt(m_hChannel, MC_TrigLine, MC_TrigLine_DIN2);
	else if (line == _T("IIN4"))
		result = McSetParamInt(m_hChannel, MC_TrigLine, MC_TrigLine_IIN4);
	else if (line == _T("IIN3"))
		result = McSetParamInt(m_hChannel, MC_TrigLine, MC_TrigLine_IIN3);
	else if (line == _T("IIN2"))
		result = McSetParamInt(m_hChannel, MC_TrigLine, MC_TrigLine_IIN2);
	else 
		result = McSetParamInt(m_hChannel, MC_TrigLine, MC_TrigLine_IIN1);

	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set Trigger line"),result);
		return false;
	}

	return true;
}

bool CEuresys::SetCombinedMode()
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_TrigMode, MC_TrigMode_COMBINED);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set COMBINED mode"),result);
		return false;
	}

	result = McSetParamInt(m_hChannel, MC_NextTrigMode, MC_NextTrigMode_COMBINED);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set Next COMBINED mode"),result);
		return false;
	}

	return true;
}

bool CEuresys::SetColorFormat(CString format)
{
	MCSTATUS result;

	if (format == _T("BAYER8"))
		result = McSetParamInt(m_hChannel,MC_ColorFormat,MC_ColorFormat_BAYER8);
	else if (format == _T("RGB24"))
		result = McSetParamInt(m_hChannel,MC_ColorFormat,MC_ColorFormat_RGB24);
	else
		result = McSetParamInt(m_hChannel,MC_ColorFormat,MC_ColorFormat_Y8);

	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set Color Format"),result);
		return false;
	}

	return true;
}

bool CEuresys::SetExpose_us(int value)
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_Expose_us, value);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set Expose_us"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetSurfaceCount(int value)
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_SurfaceCount, value);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set SurfaceCount"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetImageSizeX(int value)
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_ImageSizeX, value);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set ImageSizeX"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetImageSizeY(int value)
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_ImageSizeY, value);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set ImageSizeY"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetGrabWindow(bool enable)
{
	MCSTATUS result;

	if (enable == true)
	{
		result = McSetParamInt(m_hChannel, MC_GrabWindow, MC_GrabWindow_MAN);
		if(result != MC_OK)
		{
			ShowErrorMessage(_T("Could not set GrabWindow On"),result);
			return false;
		}
	}
	else
	{
		result = McSetParamInt(m_hChannel, MC_GrabWindow, MC_GrabWindow_NOBLACK);
		if(result != MC_OK)
		{
			ShowErrorMessage(_T("Could not set GrabWindow Off"),result);
			return false;
		}
	}
	return true;
}

bool CEuresys::SetFlipX(bool enable)
{
	MCSTATUS result;

	if (enable == true)
	{
		result = McSetParamInt(m_hChannel, MC_ImageFlipX, MC_ImageFlipX_ON);
		if(result != MC_OK)
		{
			ShowErrorMessage(_T("Could not set FlipX On"),result);
			return false;
		}
	}
	else
	{
		result = McSetParamInt(m_hChannel, MC_ImageFlipX, MC_ImageFlipX_OFF);
		if(result != MC_OK)
		{
			ShowErrorMessage(_T("Could not set FlipX Off"),result);
			return false;
		}
	}
	return true;
}

bool CEuresys::SetFlipY(bool enable)
{
	MCSTATUS result;

	if (enable == true)
	{
		result = McSetParamInt(m_hChannel, MC_ImageFlipY, MC_ImageFlipY_ON);
		if(result != MC_OK)
		{
			ShowErrorMessage(_T("Could not set FlipY On"),result);
			return false;
		}
	}
	else
	{
		result = McSetParamInt(m_hChannel, MC_ImageFlipY, MC_ImageFlipY_OFF);
		if(result != MC_OK)
		{
			ShowErrorMessage(_T("Could not set FlipY Off"),result);
			return false;
		}
	}
	return true;
}

bool CEuresys::SetExposeRecovery_us(int value)
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_ExposeRecovery_us, value);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set ExposeRecovery_us"),result);
		return false;
	}
	return true;
}

bool CEuresys::SetReadoutRecovery_us(int value)
{
	MCSTATUS result;
	result = McSetParamInt(m_hChannel, MC_ReadoutRecovery_us, value);
	if(result != MC_OK)
	{
		ShowErrorMessage(_T("Could not set ReadoutRecovery_us"),result);
		return false;
	}
	return true;
}

void CEuresys::DeleteChannel()
{
	McDelete(m_hChannel);
}

void CEuresys::ShowErrorMessage(CString message, int error)
{
	CString errorMsg;
	errorMsg.Format(_T("%s: Error = %d: "), message, error);

	switch(error)
	{
	    case MC_NO_BOARD_FOUND				:	errorMsg += _T("No Board Found"				    );		break;
	    case MC_BAD_PARAMETER				:	errorMsg += _T("Bad Parameter"					);		break;
	    case MC_IO_ERROR					:	errorMsg += _T("I/O Error"						);		break;
	    case MC_INTERNAL_ERROR				:	errorMsg += _T("Internal Error"				    );		break;
	    case MC_NO_MORE_RESOURCES			:	errorMsg += _T("No More Resources"				);		break;
	    case MC_IN_USE						:	errorMsg += _T("Object still in use"			);		break;
	    case MC_NOT_SUPPORTED				:	errorMsg += _T("Operation not supported"		);		break;
	    case MC_DATABASE_ERROR				:	errorMsg += _T("Parameter database error"		);		break;
	    case MC_OUT_OF_BOUND				:	errorMsg += _T("Value out of bound"			    );		break;
	    case MC_INSTANCE_NOT_FOUND			:	errorMsg += _T("Object instance not found"		);		break;
	    case MC_INVALID_HANDLE				:	errorMsg += _T("Invalid Handle"				    );		break;
	    case MC_TIMEOUT						:	errorMsg += _T("Timeout"						);		break;
	    case MC_INVALID_VALUE				:	errorMsg += _T("Invalid Value"					);		break;
	    case MC_RANGE_ERROR					:	errorMsg += _T("Value not in range"			    );		break;
	    case MC_BAD_HW_CONFIG				:	errorMsg += _T("Invalid hardware configuration" );		break;
	    case MC_NO_EVENT					:	errorMsg += _T("No	Event"						);		break;
	    case MC_LICENSE_NOT_GRANTED			:	errorMsg += _T("License not granted"			);		break;
	    case MC_FATAL_ERROR					:	errorMsg += _T("Fatal error"					);		break;
	    case MC_HW_EVENT_CONFLICT			:	errorMsg += _T("Hardware event conflict"		);		break;
	    case MC_FILE_NOT_FOUND				:	errorMsg += _T("File not found"				    );		break;
	    case MC_OVERFLOW					:	errorMsg += _T("Overflow"						);		break;
	    case MC_INVALID_PARAMETER_SETTING	:	errorMsg += _T("Parameter inconsistency"		);		break;
	    case MC_PARAMETER_ILLEGAL_ACCESS	:	errorMsg += _T("Illegal operation"				);		break;
	    case MC_CLUSTER_BUSY				:	errorMsg += _T("Cluster busy"					);		break;
	    case MC_SERVICE_ERROR				:	errorMsg += _T("MultiCam service error"		    );  	break;
	    case MC_INVALID_SURFACE				:	errorMsg += _T("Invalid surface"				);		break;
	}
	AfxMessageBox(errorMsg, MB_OKCANCEL | MB_ICONINFORMATION);
}

bool CEuresys::OnSetExposeTime(int ExpTime)
{
	m_Status = McOpenDriver(NULL);

	m_Status = McSetParamInt(m_hChannel, MC_Expose_us, ExpTime);

	if (m_Status!=0) {
		ShowErrorMessage("Fail to set expose time!", m_Status);
        return false;
	}

	return true;
}

bool CEuresys::OnLoadBoard()
{
	int EBoardCount;

	m_Status = McOpenDriver(NULL);

	m_Status = McGetParamInt(MC_CONFIGURATION, MC_BoardCount, &EBoardCount);

	if (EBoardCount==0) {
		ShowErrorMessage("Could not find vision board!", m_Status);
		return false;
	}

	return true;
}


