#pragma once

#include "multicam.h"
#include "Open_eVision_1_2.h"
#pragma comment (lib,"multicam.lib")

//-----------------------------------------------
using namespace Euresys::Open_eVision_1_2;
//-----------------------------------------------

//#include "C:\\Program Files (x86)\\Euresys\\MultiCam\\Include\\multicam.h"
//#include "C:\\Program Files\\Euresys\\MultiCam\\Include\\multicam.h"
//#pragma comment (lib,"C:\\Program Files (x86)\\Euresys\\MultiCam\\lib\\multicam")

#define		SURFACE_COUNT 4
#define		MAX_BUFFER	  10

#define		MC_NO_BOARD_FOUND				-1	
#define		MC_BAD_PARAMETER				-2	
#define		MC_IO_ERROR						-3	
#define		MC_INTERNAL_ERROR				-4	
#define		MC_NO_MORE_RESOURCES			-5	
#define		MC_IN_USE						-6	
#define		MC_NOT_SUPPORTED				-7	
#define		MC_DATABASE_ERROR				-8	
#define		MC_OUT_OF_BOUND					-9	
#define		MC_INSTANCE_NOT_FOUND			-10
#define		MC_INVALID_HANDLE				-11
#define		MC_TIMEOUT						-12
#define		MC_INVALID_VALUE				-13
#define		MC_RANGE_ERROR					-14
#define		MC_BAD_HW_CONFIG				-15
#define		MC_NO_EVENT						-16
#define		MC_LICENSE_NOT_GRANTED			-17
#define		MC_FATAL_ERROR					-18
#define		MC_HW_EVENT_CONFLICT			-19
#define		MC_FILE_NOT_FOUND				-20
#define		MC_OVERFLOW						-21
#define		MC_INVALID_PARAMETER_SETTING	-22
#define		MC_PARAMETER_ILLEGAL_ACCESS		-23
#define		MC_CLUSTER_BUSY					-24
#define		MC_SERVICE_ERROR				-25
#define		MC_INVALID_SURFACE				-26

class CBoardInfo
{
public :
	CBoardInfo();
	~CBoardInfo();

public :
	int GetBoardCount	  ();
	int	GetPCIePayloadSize(int num);
	int	GetPCIeLinkWidth  (int num);
	CString GetBoardName  (int num);
	CString GetBoardType  (int num);
	CString GetBoardInformation(int num);
private :
	void	ShowErrorMessage(CString message, int error);
};

class CEuresys
{
public:
	CEuresys(void);
	~CEuresys(void);

	EImageBW8 IImage;
	int m_nSizeX, m_nSizeY,m_nBufferPitch;
	/*
	BYTE* GetPointer() { 
		if (m_nGrabCount == 0)
			return m_pImage[MAX_BUFFER-1]; 
		else
			return m_pImage[m_nGrabCount-1];
	}
	*/
	bool  IsOpened()  { return m_bOpened; }
	bool		IsTrigger;
private:
	int			m_nGrabCount;
	MCHANDLE	m_hChannel;
	PVOID		m_pBuffer;
	MCSTATUS    m_Status ;
	char		m_pTmp[MAX_PATH];
	//BYTE*		m_pImage[10];
	//int			m_nSize;
	bool		m_bOpened;

	char* CStringToPChar(CString str);
	//---------- Callback Function ------------//
	static void WINAPI	GlobalCallback(PMCSIGNALINFO SigInfo);
	void 	Callback(PMCSIGNALINFO SigInfo);

	//---------- Channel manager ------------//
	void	DeleteChannel();

	//---------- Error Report ------------//
	void	ShowErrorMessage(CString message, int error);

	//---------- Multicam Driver ------------//
	bool	OpenDriver();
	bool	CloseDriver();

	//---------- Output Control ------------//
	bool	SetOutputConfig(int channel);
public:
	//---------- Grabber Setting ------------//
	void	SetErrorHandling(int num);
	/*0:NONE, 1:MSGBOX, 2:EXCEPTION, 3:MSGEXCEPTION*/

	//---------- Initialize Grabber ------------//
	bool 	InitBoard(int index , CString connector, CString topology, CString camfilePath, int ExposeTime);

	//---------- Acquisition Control ------------//
	bool	SetActive();
	bool	SetIdle();
	bool	OnTrigger();	

	//---------- Output Control ------------//
	bool	SetOutputStatus(int channel, bool status);

	//---------- Get & Set Parameter ------------//
	int		GetHactive_Px			();
	int		GetHsyncAft_Tk			();
	int		GetVactive_Ln			();
	int		GetVsyncAft_Ln			();
	int	 	GetImageSizeX			();
	int	 	GetImageSizeY			();
	int	 	GetBufferPitch			();
	int		GetSurfaceCount			();
	int		GetExpose_us			();
	int		GetSeqLength_Fr			();
	int		GetFrameRate_mHz		();
	int		GetExposeRecovery_us	();
	int		GetReadoutRecovery_us	();
	int		GetBufferSize			() { return m_nBufferPitch; }
	int		GetBitPerPixel			();
	CString	GetColorFormat			();
	CString GetTrigMode				();
	CString GetNextTrigMode			();
	CString GetCamfilePath			();

	bool	SetFrameRate_mHz		(int value);
	bool	SetSeqLength_Fr			(int value);
	bool	SetSurfaceCount			(int value);
	bool	SetExpose_us			(int value);
	bool 	SetImageSizeX			(int value);
	bool 	SetImageSizeY			(int value);
	bool	SetExposeRecovery_us	(int time);
	bool	SetReadoutRecovery_us	(int time);
	bool	SetWindowX_Px			(int value);
	bool	SetWindowY_Ln			(int value);
	bool	SetGrabWindow			(bool enable);
	bool	SetFlipX				(bool enable);
	bool	SetFlipY				(bool enable);
	bool	SetTriggerLine			(CString line);
	bool	SetColorFormat			(CString format);	
	bool	SetSoftwareTriggerMode	();
	bool	SetHardwareTriggerMode	();
	bool	SetImmediateMode		();
	bool	SetCombinedMode			();
	
	bool    OnSetExposeTime         (int ExpTime);
	bool    OnLoadBoard             ();
};

