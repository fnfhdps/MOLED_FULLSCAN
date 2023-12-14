#pragma once

#include "CommThread.h"
// CommSerial
class CommSerial : public CWnd
{
	DECLARE_DYNAMIC(CommSerial)

public:
	CommSerial(CWnd* pParent = NULL);
	virtual ~CommSerial();

public :
	CCommThread     CComPort;
	int			    m_nComPort;
	int			    m_nBaudRate;
	int			    m_nDataBit;
	int			    m_nStopBit;
	int			    m_nParityBit;

	CString OnSendData(int type, BOOL bStx, CString MSG, BOOL bEtx, BOOL CRLF);
	CString ASCII2Data(BOOL bStx, CString MSG, BOOL bEtx, BOOL CRLF);
	CString HEX2Data(BOOL bStx, CString MSG, BOOL bEtx, BOOL CRLF);
	BYTE    byCode2AsciiValue(char cData);
	
	BOOL	OnPortOpen(CString port, int baudrate, int databit, int stopbit, int paritybit);
	BOOL	OnPortClose();
	
	CString GetComPort(int iPort);
	DWORD	GetBaudRate(int iBaud);
	BYTE    GetDataBit(int iData);
	BYTE    GetStopBit(int iStop);
	BYTE    GetParityBit(int iParity);
	
	BOOL	IsConnected() {return CComPort.IsConnected(); }
	CString OnCommunication(int type);
protected:
// 	afx_msg long OnCommunication(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommunication(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
extern CommSerial pCommSerial;


