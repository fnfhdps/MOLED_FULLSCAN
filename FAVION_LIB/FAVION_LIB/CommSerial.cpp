// CommSerial.cpp : implementation file
//

#include "stdafx.h"
#include "CommSerial.h"


CommSerial pCommSerial;
// CommSerial
IMPLEMENT_DYNAMIC(CommSerial, CWnd)
//HWND hCommWnd;
CommSerial::CommSerial(CWnd* pParent)
{
	m_nComPort	 = 0;
	m_nBaudRate	 = 0;
	m_nDataBit	 = 0;
	m_nStopBit	 = 0;
	m_nParityBit = 0;

	//pCommSerial = this;
}

CommSerial::~CommSerial()
{
}


BEGIN_MESSAGE_MAP(CommSerial, CWnd)
	ON_MESSAGE(WM_COMM_READ, OnCommunication)
END_MESSAGE_MAP()



// CommSerial message handlers


// long CommSerial::OnCommunication(WPARAM wParam, LPARAM lParam)	
LRESULT CommSerial::OnCommunication(WPARAM wParam, LPARAM lParam)	
{
	CString str = _T("");
	CString result;
	CString strTmp;
	char size[128];
	BYTE aByte; 
	int iCount = 0;

	int iSize =  (CComPort.m_QueueRead).GetSize(); //��Ʈ�� ���� ������ ����

	iSize += iSize;	
	sprintf_s(size,"Receive %d characters\r\n",iSize);

	if(iSize != 0)
	{
		strTmp.Format(_T("%s"),(size));
	}
	else
	{
		strTmp.Format(_T("�����͸� ���� ���߽��ϴ�.\n"));
	}

	for(int i  = 0 ; i < iSize/2; i++)//���� ���� ��ŭ �����͸� �о� �� ȭ�鿡 ������
	{
		(CComPort.m_QueueRead).GetByte(&aByte);//ť���� ������ �Ѱ��� �о��

		switch (0)//ctrlComboReceiveType.GetCurSel())
		{
		case 0:
			str.Format(_T("%02X"), aByte);	

			iCount++;//������ ���� ����

			if((iCount % (iSize/2)) == 0) 
			{
				str += _T("\r\n");
			}
			else if((iCount % 1) ==0)
			{
				str += _T(" ");
			}
			result += str;
			break;

		case 1:
			str.Format(_T("%c"),aByte);
			result += str;
			break;
		}
	}	
	iCount = 0;
	CComPort.m_QueueRead.Clear();
	//ctrlEditReceive.ReplaceSel(result);//ȭ�鿡 ���� ������ ������
	return (LRESULT)1;
}

CString CommSerial::OnCommunication(int type)	
{
	CString str = _T("");
	CString result;
	CString strTmp;
	char size[128];
	BYTE aByte; 
	int iCount = 0;

	int iSize =  (CComPort.m_QueueRead).GetSize(); //��Ʈ�� ���� ������ ����

	iSize += iSize;	
	sprintf_s(size,"Receive %d characters\r\n",iSize);

	if(iSize != 0)
	{
		//strTmp.Format(_T("%s"),CA2T(size));
		strTmp.Format(_T("%s"),(size));
	}
	else
	{
		strTmp.Format(_T("�����͸� ���� ���߽��ϴ�.\n"));
	}

	for(int i  = 0 ; i < iSize/2; i++)//���� ���� ��ŭ �����͸� �о� �� ȭ�鿡 ������
	{
		(CComPort.m_QueueRead).GetByte(&aByte);//ť���� ������ �Ѱ��� �о��

		switch (type)//ctrlComboReceiveType.GetCurSel())
		{
		case 0:
			str.Format(_T("%02X"), aByte);	

			iCount++;//������ ���� ����

			if((iCount % (iSize/2)) == 0) 
			{
				str += _T("\r\n");
			}
			else if((iCount % 1) ==0)
			{
				str += _T(" ");
			}
			result += str;
			break;

		case 1:
			str.Format(_T("%c"),aByte);
			result += str;
			break;
		}
	}	
	iCount = 0;
	CComPort.m_QueueRead.Clear();
	//ctrlEditReceive.ReplaceSel(result);//ȭ�鿡 ���� ������ ������
	return result;
}
CString CommSerial::GetComPort(int iPort)
{
	CString strPortName;
	switch(iPort)
	{
	case 0 : strPortName = _T("COM1");
		break;
	case 1 : strPortName = _T("COM2");
		break;
	case 2 : strPortName = _T("COM3");
		break;
	case 3 : strPortName = _T("COM4");
		break;
	case 4 : strPortName = _T("COM5");
		break;
	case 5 : strPortName = _T("COM6");
		break;
	case 6 : strPortName = _T("COM7");
		break;
	case 7 : strPortName = _T("COM8");
		break;
	case 8 : strPortName = _T("COM9");
		break;
	case 9  : strPortName = _T("\\\\.\\COM10");
		break;
	case 10 : strPortName = _T("\\\\.\\COM11");
		break;
	case 11 : strPortName = _T("\\\\.\\COM12");
		break;
	case 12 : strPortName = _T("\\\\.\\COM13");
		break;
	case 13 : strPortName = _T("\\\\.\\COM14");
		break;
	case 14 : strPortName = _T("\\\\.\\COM15");
		break;
	case 15 : strPortName = _T("\\\\.\\COM16");
		break;
	case 16 : strPortName = _T("\\\\.\\COM17");
		break;
	case 17 : strPortName = _T("\\\\.\\COM18");
		break;
	case 18 : strPortName = _T("\\\\.\\COM19");
		break;
	}

	return strPortName;
}

DWORD CommSerial::GetBaudRate(int iBaud)
{
	DWORD dwBaud;
	switch(iBaud)
	{
	case 0:		dwBaud = CBR_4800;		
		break;
	case 1:		dwBaud = CBR_9600;		
		break;
	case 2:		dwBaud = CBR_14400;		
		break;
	case 3:		dwBaud = CBR_19200;		
		break;
	case 4:		dwBaud = CBR_38400;		
		break;
	case 5:		dwBaud = CBR_56000;		
		break;
	case 6:		dwBaud = CBR_57600;		
		break;
	case 7:		dwBaud = CBR_115200;	
		break;
	}

	return dwBaud;
}

BYTE CommSerial::GetDataBit(int iData)
{
	BYTE byData;
	switch(iData)
	{
	case 0 :	byData = 4;			
		break;
	case 1 :	byData = 5;			
		break;
	case 2 :	byData = 6;			
		break;
	case 3 :	byData = 7;			
		break;
	case 4 :	byData = 8;			
		break;
	}
	return byData;
}

BYTE CommSerial::GetStopBit(int iStop)
{
	BYTE byStop;
	switch(iStop)
	{
	case 0 :	byStop = ONESTOPBIT;	
		break;
	case 1 :	byStop = ONE5STOPBITS;	
		break;
	case 2 :	byStop = TWOSTOPBITS;	
		break;
	}
	return byStop;
}
BYTE CommSerial::GetParityBit(int iParity)
{
	BYTE byParity;
	switch(iParity)
	{
	case 0 :	byParity = NOPARITY;	
		break;
	case 1 :	byParity = ODDPARITY;	
		break;
	case 2 :	byParity = EVENPARITY;	
		break;
	}

	return byParity;
}


BOOL CommSerial::OnPortOpen(CString port, int baudrate, int databit, int stopbit, int paritybit) 
{
	if(CComPort.IsConnected() == FALSE)										
	{
		if(CComPort.OpenPort(port, baudrate, databit, stopbit, paritybit) == TRUE)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		AfxMessageBox(_T("Already Port Open!"));
		return FALSE;
	}
}

BOOL CommSerial::OnPortClose() 
{
	if(CComPort.IsConnected() == TRUE)		
	{	
		CComPort.ClosePort();				
		return TRUE;	
	}
	else
	{
		return FALSE;
	}
}

//int
CString CommSerial::OnSendData(int type, BOOL bStx, CString MSG, BOOL bEtx, BOOL CRLF) 
{
	//int iSize;
	CString str;
	if(type) //������  �����Ͱ� ascii or hex�� ���� 
	{
		ASCII2Data(bStx,MSG,bEtx,CRLF);// �����͸� ascii�� ���� ��� 
		return MSG;
	}
	else
	{
		str = HEX2Data(bStx,MSG,bEtx,CRLF); //�����͸�  hex�� ���� ���
		return str;
	}

	//return iSize;
}

CString CommSerial::ASCII2Data(BOOL bStx, CString MSG, BOOL bEtx, BOOL CRLF)
{
	CString strHex, strTmp, strMsg;
	int iSize;
	int iBufPos = 0;
	BYTE byHigh, byLow;
	BYTE* Send_buff = NULL;
	int iDatasize,iBufsize;

	strHex = MSG;
	strMsg = strHex;
	iSize = strHex.GetLength();

	Send_buff = new BYTE[iSize*2+4+1];

	for(int i = 0; i < iSize; i++)
	{
		//strHex.Format(_T("%X"),strSend[i]);
		strHex.Format(_T("%X"),strMsg[i]);       
		strTmp += strHex;
	}

	if(bStx == TRUE)
		strTmp = _T("02") + strTmp; // [STX] + Msg

	if(bEtx == TRUE)
		strTmp += _T("03"); // Msg + [ETX]

	if(CRLF == TRUE)
		strTmp += _T("0D0A"); // Msg + [CR] + [LF]


	iDatasize = strTmp.GetLength();  

	if(iDatasize %2 == 0)
	{
		iBufsize = iDatasize;
	}
	else
	{
		iBufsize = iDatasize -1; 
	}


	for(int i = 0 ; i < iDatasize; i+=2)
	{

		byHigh = byCode2AsciiValue((char)strTmp[i]);
		byLow  = byCode2AsciiValue((char)strTmp[i+1]);
		Send_buff[iBufPos++] = (byHigh <<4) | byLow;
	}

	//������ ���ڰ� 1�ڸ��� �϶� ó�� �ϱ� ����  ��) 1 -> 01
	if(iDatasize %2 !=0)
	{	
		Send_buff[iBufPos++] = byCode2AsciiValue((char)strTmp[iDatasize-1]);
	}

	int etc = iBufPos % 8;
	//��Ʈ�� �����͸� 8���� ���� ����
	//�������� ���̰� 8�� ����� �ƴϸ� ������ �����ʹ� ���� ������
	for(int j =0; j < iBufPos-etc; j+= 8)//8�� ��� ����
	{
		CComPort.WriteComm(&Send_buff[j], 8) ;
	}
	if(etc != 0)//������ ������ ����
	{
		CComPort.WriteComm(&Send_buff[iBufPos -etc], etc) ;// ��Ʈ�� ������ ���� 
	}

	delete [] Send_buff;
	//return iBufPos;
	return strTmp;
}
BYTE CommSerial::byCode2AsciiValue(char cData)
{
	//char���ڸ� hex������ ����
	BYTE byAsciiValue;
	if( ( '0' <= cData ) && ( cData <='9' ) )
	{
		byAsciiValue = cData - '0';
	}
	else if( ( 'A' <= cData ) && ( cData <= 'F' ) )
	{
		byAsciiValue = (cData - 'A') + 10;
	}
	else if( ( 'a' <= cData ) && ( cData <= 'f' ) )
	{
		byAsciiValue = (cData - 'a') + 10;
	}
	else
	{
		byAsciiValue = 0;
	}
	return byAsciiValue;
}
CString CommSerial::HEX2Data(BOOL bStx, CString MSG, BOOL bEtx, BOOL CRLF)
{
	int iBufPos = 0;
	int iDatasize = 0,iBufsize = 0;
	BYTE *Send_buff = NULL;
	BYTE byHigh, byLow;
	CString strGetData;

	strGetData = MSG;
	strGetData.Replace(_T(" "),_T(""));	// ������ ����.
	strGetData.Replace(_T("\r\n"),_T(""));	//�ٳѱ��� ����.
	//iDatasize = strGetData.GetLength();		//���ڿ��� ���̸� ����.

	if(bStx == TRUE)
		strGetData = _T("02") + strGetData; // [STX] + Msg

	if(bEtx == TRUE)
		strGetData += _T("03"); // Msg + [ETX]

	if(CRLF == TRUE)
		strGetData += _T("0D0A"); // Msg + [CR] + [LF]

	iDatasize = strGetData.GetLength();		//���ڿ��� ���̸� ����.

	if(iDatasize %2 == 0)
	{
		iBufsize = iDatasize;
	}
	else
	{
		iBufsize = iDatasize -1; 
	}

	Send_buff = new BYTE[iBufsize];

	for(int i = 0; i < iBufsize ; i+=2)
	{
		byHigh = byCode2AsciiValue((char)strGetData[i]);
		byLow  = byCode2AsciiValue((char)strGetData[i+1]);
		Send_buff[iBufPos++] = (byHigh <<4) | byLow;

	}
	//������ ���ڰ� 1�ڸ��� �϶� ó�� �ϱ� ����  ��) 1 -> 01
	if(iDatasize %2 !=0)
	{	
		Send_buff[iBufPos++] = byCode2AsciiValue((char)strGetData[iDatasize-1]);
	}

	int etc = iBufPos % 8;
	//��Ʈ�� �����͸� 8���� ���� ����
	//�������� ���̰� 8�� ����� �ƴϸ� ������ �����ʹ� ���� ������
	for(int j =0; j < iBufPos-etc; j+= 8)//8�� ��� ����
	{
		CComPort.WriteComm(&Send_buff[j], 8) ;
	}
	if(etc != 0)//������ ������ ����
	{
		CComPort.WriteComm(&Send_buff[iBufPos -etc], etc) ;// ��Ʈ�� ������ ���� 
	}

	delete [] Send_buff;
	//return iBufPos;
	return strGetData;
}

