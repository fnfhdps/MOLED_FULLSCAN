#include "stdafx.h"
#include "CSystemMon.h"
#include <algorithm>


typedef enum ETH_QUERY_TYPE
{
	EQT_CURRENT_BANDWIDTH = 0,
	EQT_BYTES_RECV_PER_SEC = 1,
	EQT_BYTES_SENT_PER_SEC = 2,
	EQT_PACKET_RECV_PER_SEC = 3,
	EQT_PACKET_SENT_PER_SEC = 4,
	EQT_COUNT = 5
} ETH_QUERY_TYPE;

typedef struct Eth
{
	TCHAR *m_pszName;
	HQUERY m_hQuery;
	HCOUNTER m_hCounter[EQT_COUNT];
	unsigned long m_ulBandwidth;
	unsigned long m_ulBytesRecvPerSec;
	unsigned long m_ulBytesSentPerSec;
	unsigned long m_ulPacketRecvPerSec;
	unsigned long m_ulPacketSentPerSec;
} Eth;

typedef struct EthList
{
	int m_nCount;
	Eth *m_pEth;
} EthList;

bool  OpenEthList(EthList *ethList)
{
	int i = 0;
	bool bErr = false;
	TCHAR *szCur = NULL;
	TCHAR *szBuff = NULL;
	TCHAR *szNet = NULL;
	DWORD dwBuffSize = 0, dwNetSize = 0;
	TCHAR szCounter[256];

	if (ethList == NULL)
		return false;

	memset(ethList, 0x00, sizeof(EthList));

	// calculate buffer size
	PdhEnumObjectItems(NULL, NULL, TEXT("Network Interface"), szBuff, &dwBuffSize, szNet, &dwNetSize, PERF_DETAIL_WIZARD, 0);

	if ((szBuff = (TCHAR *)malloc(sizeof(TCHAR) * dwBuffSize)) == NULL)
	{
		return false;
	}

	if ((szNet = (TCHAR *)malloc(sizeof(TCHAR) * dwNetSize)) == NULL)
	{
		free(szBuff);
		return false;
	}

	if (PdhEnumObjectItems(NULL, NULL, TEXT("Network Interface"), szBuff, &dwBuffSize, szNet, &dwNetSize, PERF_DETAIL_WIZARD, 0) != ERROR_SUCCESS)
	{
		free(szBuff);
		free(szNet);
		return false;
	}

	for (szCur = szNet, i = 0; *szCur != 0; szCur += _tcslen(szCur) + 1, i++)
	{
		// Just count how many network interface is exist.
	}

	if (i <= 0)
	{
		free(szBuff);
		free(szNet);
		return false;
	}

	ethList->m_nCount = i;
	ethList->m_pEth = (Eth *)malloc(sizeof(Eth) * i);
	memset(ethList->m_pEth, 0x00, sizeof(Eth) * i);

	for (szCur = szNet, i = 0; *szCur != 0; szCur += _tcslen(szCur) + 1, i++)
	{
		Eth *eth = &ethList->m_pEth[i];
		eth->m_pszName = _tcsdup(szCur);

		if (PdhOpenQuery(NULL, 0, &eth->m_hQuery))
		{
			bErr = true;
			break;
		}

		_stprintf(szCounter, TEXT("\\Network Interface(%s)\\Current Bandwidth"), eth->m_pszName);
		if (PdhAddCounter(eth->m_hQuery, szCounter, 0, &eth->m_hCounter[EQT_CURRENT_BANDWIDTH]))
		{
			bErr = true;
			break;
		}

		_stprintf(szCounter, TEXT("\\Network Interface(%s)\\Bytes Received/sec"), eth->m_pszName);
		if (PdhAddCounter(eth->m_hQuery, szCounter, 0, &eth->m_hCounter[EQT_BYTES_RECV_PER_SEC]))
		{
			bErr = true;
			break;
		}

		_stprintf(szCounter, TEXT("\\Network Interface(%s)\\Bytes Sent/sec"), eth->m_pszName);
		if (PdhAddCounter(eth->m_hQuery, szCounter, 0, &eth->m_hCounter[EQT_BYTES_SENT_PER_SEC]))
		{
			bErr = true;
			break;
		}

		_stprintf(szCounter, TEXT("\\Network Interface(%s)\\Packets Received/sec"), eth->m_pszName);
		if (PdhAddCounter(eth->m_hQuery, szCounter, 0, &eth->m_hCounter[EQT_PACKET_RECV_PER_SEC]))
		{
			bErr = true;
			break;
		}

		_stprintf(szCounter, TEXT("\\Network Interface(%s)\\Packets Sent/sec"), eth->m_pszName);
		if (PdhAddCounter(eth->m_hQuery, szCounter, 0, &eth->m_hCounter[EQT_PACKET_SENT_PER_SEC]))
		{
			bErr = true;
			break;
		}
	}

	if (szBuff != NULL)
		free(szBuff);

	if (szNet != NULL)
		free(szNet);

	if (bErr)
		return false;

	return true;
}

bool  QueryEthUsage(Eth *eth)
{
	HCOUNTER hCounter = NULL;

	PDH_FMT_COUNTERVALUE v;

	if (eth == NULL)
		return false;

	if (PdhCollectQueryData(eth->m_hQuery))
	{
		return false;
	}

	v.longValue = 0;
	PdhGetFormattedCounterValue(eth->m_hCounter[EQT_CURRENT_BANDWIDTH], PDH_FMT_LONG, 0, &v);
	eth->m_ulBandwidth = v.longValue;

	v.longValue = 0;
	PdhGetFormattedCounterValue(eth->m_hCounter[EQT_BYTES_RECV_PER_SEC], PDH_FMT_LONG, 0, &v);
	eth->m_ulBytesRecvPerSec = v.longValue;

	v.longValue = 0;
	PdhGetFormattedCounterValue(eth->m_hCounter[EQT_BYTES_SENT_PER_SEC], PDH_FMT_LONG, 0, &v);
	eth->m_ulBytesSentPerSec = v.longValue;

	v.longValue = 0;
	PdhGetFormattedCounterValue(eth->m_hCounter[EQT_PACKET_RECV_PER_SEC], PDH_FMT_LONG, 0, &v);
	eth->m_ulPacketRecvPerSec = v.longValue;

	v.longValue = 0;
	PdhGetFormattedCounterValue(eth->m_hCounter[EQT_PACKET_SENT_PER_SEC], PDH_FMT_LONG, 0, &v);
	eth->m_ulPacketSentPerSec = v.longValue;

	return true;
}

void  CloseEthList(EthList *ethList)
{
	if (ethList != NULL)
	{
		int i = 0, j = 0;
		for (i = 0; i < ethList->m_nCount; i++)
		{
			Eth *eth = &ethList->m_pEth[i];
			free(eth->m_pszName);

			for (j = 0; j < EQT_COUNT; j++)
			{
				if (eth->m_hCounter[j] != NULL)
				{
					PdhRemoveCounter(eth->m_hCounter[j]);
					eth->m_hCounter[j] = NULL;
				}
			}

			PdhCloseQuery(eth->m_hQuery);
			eth->m_hQuery = NULL;
		}

		free(ethList->m_pEth);
		memset(ethList, 0x00, sizeof(EthList));
	}
}


void MonitorNetworkLoad()
{
	int i = 0;
	time_t tLast = time(0) + 20;
	EthList ethList;

	if (OpenEthList(&ethList) <= 0)
	{
		printf("fail to load ethernet list\n");
	}

	while (time(0) < tLast)
	{
		for (i = 0; i < ethList.m_nCount; i++)
		{
			Eth *e = &ethList.m_pEth[i];

			if (!QueryEthUsage(e))
				break;
#if(0)
			SetLog(TEXT("%s"), e->m_pszName);
			SetLog(TEXT("bandwidth %ld"), e->m_ulBandwidth);
			SetLog(TEXT("bytes recv/sec %ld"), e->m_ulBytesRecvPerSec);
			SetLog(TEXT("bytes sent/sec %ld"), e->m_ulBytesSentPerSec);
			SetLog(TEXT("packet recv/sec %ld"), e->m_ulPacketRecvPerSec);
			SetLog(TEXT("packet sent/sec %ld"), e->m_ulPacketSentPerSec);
#endif
		}

		Sleep(100);
	}

	CloseEthList(&ethList);
}


DWORD WINAPI CSystemMon::StartUp(LPVOID pTheThread)
{
	MSG	msg;
	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);	//	to make create message queue

	CSystemMon *pClass = (CSystemMon*)pTheThread;
	pClass->ThreadTask();
	return 0;
}

BOOL CSystemMon::Run()
{
	m_thRuning = true;
	DWORD dwThreadId;

	m_hThread = CreateThread(NULL, 0xa00000, StartUp, this, 0, &dwThreadId);

	if (m_hThread != NULL)
	{
		return TRUE;
	}


	return FALSE;
}



CSystemMon::CSystemMon()
	:m_hQuery(NULL), m_reQuery(true), m_hRefresh(NULL), m_thRuning(false)
{
	CTheLogger::Create(_TEXT("CSystemMon"));
}


CSystemMon::~CSystemMon()
{
	DestroySystemMon();
}


void CSystemMon::InitSystemMon()
{
	// ������ �����ش�.
	if (m_hQuery == NULL)
	{
		m_hRefresh = CreateEvent(NULL, TRUE, FALSE, NULL);

		PDH_STATUS status = PdhOpenQuery(NULL, 0, &m_hQuery);
		if (ERROR_SUCCESS != status)
			return;

		UpdateCpuList();
		UpdateDiskList();
		UpdateNetworkLisk();

		m_thRuning = true;

#if(0)
		m_ThMonitor = std::thread(&CSystemMon::ThreadFunc, this);
#else
		Run();
#endif


	}
}

void	CSystemMon::DestroySystemMon()
{
	m_thRuning = false;	  //thread terminate
	SetEvent(m_hRefresh); //working thread awake

#if(0)
	if (m_ThMonitor.joinable())
		m_ThMonitor.join();
#endif
	for (auto it = m_CpuList.begin(); it != m_CpuList.end(); it++)
		PdhRemoveCounter(it->counter);
	m_CpuList.clear();


	for (auto it = m_DiskList.begin(); it != m_DiskList.end(); it++)
		PdhRemoveCounter(it->counter);
	m_DiskList.clear();

	for (auto it = m_NetworkList.begin(); it != m_NetworkList.end(); it++)
		PdhRemoveCounter(it->counter);
	m_NetworkList.clear();

	for (auto it = m_ProcssTime.begin(); it != m_ProcssTime.end(); it++)
		PdhRemoveCounter(it->counter);
	m_ProcssTime.clear();

	//
	for (auto it = m_ProcssIoReadByte.begin(); it != m_ProcssIoReadByte.end(); it++)
		PdhRemoveCounter(it->counter);
	m_ProcssIoReadByte.clear();

	for (auto it = m_ProcssIoWriteByte.begin(); it != m_ProcssIoWriteByte.end(); it++)
		PdhRemoveCounter(it->counter);
	m_ProcssIoWriteByte.clear();

	for (auto it = m_ProcssIoDataByte.begin(); it != m_ProcssIoDataByte.end(); it++)
		PdhRemoveCounter(it->counter);
	m_ProcssIoDataByte.clear();

	for (auto it = m_ProcssIoOtherByte.begin(); it != m_ProcssIoOtherByte.end(); it++)
		PdhRemoveCounter(it->counter);
	m_ProcssIoOtherByte.clear();

	for (auto it = m_ProcssIoReadOper.begin(); it != m_ProcssIoReadOper.end(); it++)
		PdhRemoveCounter(it->counter);
	m_ProcssIoReadOper.clear();

	for (auto it = m_ProcssIoWriteOper.begin(); it != m_ProcssIoWriteOper.end(); it++)
		PdhRemoveCounter(it->counter);
	m_ProcssIoWriteOper.clear();

	for (auto it = m_ProcssIoDataOper.begin(); it != m_ProcssIoDataOper.end(); it++)
		PdhRemoveCounter(it->counter);
	m_ProcssIoDataOper.clear();


	if (m_hQuery) {
		PdhCloseQuery(m_hQuery);
		m_hQuery = NULL;
	}

	CloseHandle(m_hRefresh);
}

void CSystemMon::Update()
{
	SetEvent(m_hRefresh);
}

bool CSystemMon::IsPhdList(const tstring& name, bool full)
{
	PDH_STATUS status;
	PDH_FMT_COUNTERVALUE CounterValue;

	if (full) {
		for (auto it = m_ProcssTime.begin(); it != m_ProcssTime.end();) {
			status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &CounterValue);

			if (ERROR_SUCCESS != status) {
				PdhRemoveCounter(it->counter);
				it = m_ProcssTime.erase(it);
			}
			else {++it;}
		}

		for (auto it = m_ProcssIoReadByte.begin(); it != m_ProcssIoReadByte.end();) {
			status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &CounterValue);

			if (ERROR_SUCCESS != status) {
				PdhRemoveCounter(it->counter);
				it = m_ProcssIoReadByte.erase(it);
			}
			else { ++it; }
		}

		for (auto it = m_ProcssIoWriteByte.begin(); it != m_ProcssIoWriteByte.end();) {
			status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &CounterValue);

			if (ERROR_SUCCESS != status) {
				PdhRemoveCounter(it->counter);
				it = m_ProcssIoWriteByte.erase(it);
			}
			else { ++it; }
		}


		for (auto it = m_ProcssIoDataByte.begin(); it != m_ProcssIoDataByte.end();) {
			status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &CounterValue);

			if (ERROR_SUCCESS != status) {
				PdhRemoveCounter(it->counter);
				it = m_ProcssIoDataByte.erase(it);
			}
			else { ++it; }
		}

		for (auto it = m_ProcssIoOtherByte.begin(); it != m_ProcssIoOtherByte.end();) {
			status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &CounterValue);

			if (ERROR_SUCCESS != status) {
				PdhRemoveCounter(it->counter);
				it = m_ProcssIoOtherByte.erase(it);
			}
			else { ++it; }
		}


		for (auto it = m_ProcssIoReadOper.begin(); it != m_ProcssIoReadOper.end();) {
			status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &CounterValue);

			if (ERROR_SUCCESS != status) {
				PdhRemoveCounter(it->counter);
				it = m_ProcssIoReadOper.erase(it);
			}
			else { ++it; }
		}

		for (auto it = m_ProcssIoWriteOper.begin(); it != m_ProcssIoWriteOper.end();) {
			status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &CounterValue);

			if (ERROR_SUCCESS != status) {
				PdhRemoveCounter(it->counter);
				it = m_ProcssIoWriteOper.erase(it);
			}
			else { ++it; }
		}

		for (auto it = m_ProcssIoDataOper.begin(); it != m_ProcssIoDataOper.end();) {
			status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &CounterValue);

			if (ERROR_SUCCESS != status) {
				PdhRemoveCounter(it->counter);
				it = m_ProcssIoDataOper.erase(it);
			}
			else { ++it; }
		}
		return true;
	}

	bool isExist = false;

	for (auto it = m_ProcssTime.begin(); it != m_ProcssTime.end(); ++it) {
		if (it->name == name) {
			isExist = true;	break;
		}
	}

	for (auto it = m_ProcssIoReadByte.begin(); it != m_ProcssIoReadByte.end(); ++it) {
		if (it->name == name) {
			isExist = true;	break;
		}
	}

	for (auto it = m_ProcssIoWriteByte.begin(); it != m_ProcssIoWriteByte.end(); ++it) {
		if (it->name == name) {
			isExist = true;	break;
		}
	}

	for (auto it = m_ProcssIoDataByte.begin(); it != m_ProcssIoDataByte.end(); ++it) {
		if (it->name == name) {
			isExist = true;	break;
		}
	}

	for (auto it = m_ProcssIoOtherByte.begin(); it != m_ProcssIoOtherByte.end(); ++it) {
		if (it->name == name) {
			isExist = true;	break;
		}
	}

	for (auto it = m_ProcssIoReadOper.begin(); it != m_ProcssIoReadOper.end(); ++it) {
		if (it->name == name) {
			isExist = true;	break;
		}
	}

	for (auto it = m_ProcssIoWriteOper.begin(); it != m_ProcssIoWriteOper.end(); ++it) {
		if (it->name == name) {
			isExist = true;	break;
		}
	}

	for (auto it = m_ProcssIoDataOper.begin(); it != m_ProcssIoDataOper.end(); ++it) {
		if (it->name == name) {
			isExist = true;	break;
		}
	}
	return isExist;
}

void CSystemMon::UpdateCpuList()
{
	PDH_STATUS status;
	HCOUNTER *phCounter_Core = NULL; // CPU �ھ� ������ ���ϱ� ����
	TCHAR szFullCounterPath[1024] = { 0 };

	DWORD i;
	SYSTEM_INFO si;


	// �ھ��� ������ ���Ѵ�.
	GetSystemInfo(&si); // si.dwNumberOfProcessors


	// CPU �ھ�� ī���� �ڵ��� ������ �޸𸮸� �Ҵ��Ѵ�. ��Ż �κе� ��� +1�Ѵ�.
	phCounter_Core = (HCOUNTER *)malloc(sizeof(HCOUNTER) * (si.dwNumberOfProcessors + 1));
	if (!phCounter_Core) {
		SetLog(TEXT("phCounter_Core �޸� �Ҵ�(malloc)�� �����Ͽ����ϴ�.- InitSystemMon"));
		return;
	}

	memset(phCounter_Core, 0, sizeof(HCOUNTER) * (si.dwNumberOfProcessors + 1));

	// CPU �ھ Processor Time ī���͸� �߰��ϰ� �ڵ��� ���Ѵ�. Processor ���ڿ� �����Ѵ�.(Process�� �ƴϴ�.)
	for (i = 0; i < si.dwNumberOfProcessors; i++) {
		_stprintf(szFullCounterPath, TEXT("\\Processor(%d)\\%% Processor Time"), i);

		status = PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Core + i);
		if (ERROR_SUCCESS != status) {
			SetLog(TEXT("PdhAddCounter - InitSystemMon"));
			goto error_pdh;
		}

		m_CpuList.push_back(PhdItem(phCounter_Core[i], szFullCounterPath));
	}

	// CPU ��Ż Processor Time ī���͸� �߰��ϰ� �ڵ��� ���Ѵ�.
	lstrcpy(szFullCounterPath, TEXT("\\Processor(_Total)\\% Processor Time"));

	status = PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Core + si.dwNumberOfProcessors);
	if (ERROR_SUCCESS == status)
	{
		m_CpuList.push_back(PhdItem(phCounter_Core[si.dwNumberOfProcessors], szFullCounterPath));
	}

error_pdh:

	free(phCounter_Core);

}

void CSystemMon::UpdateProcessList()
{
	LPCTSTR szMachine = NULL;
	LPCTSTR szObject = TEXT("Process");

	PDH_STATUS status;
	DWORD cchBuffer = 0;
	DWORD cchCounterList = 0;
	DWORD cchInstanceList = 0;
	LPTSTR mszCounterList = NULL; // msz�� ��Ƽ��Ʈ���� �ǹ��Ѵ�. ���� ���ڿ��� ������ NULL�� �ϰ� �� ������ NULL�� ���� 2���� �´�.
	LPTSTR mszInstanceList = NULL;
	LPTSTR szInstance = NULL;
	LPTSTR szCounter = NULL;

	DWORD cInstance = 0;

	HCOUNTER *phCounter_Instance = NULL;
	TCHAR szFullCounterPath[1024] = { 0 };
	DWORD i, j;

	int *pIndexOfInstance = NULL;  // �ν��Ͻ� �̸��� ���� ��� �ε��� �۾��� ����
	LPTSTR pszCur = NULL;
	int index;

	LPTSTR lpszErrorBuf; // ���� �޽����ڽ� ����(FormatMessage ����)

	// ���� ���μ����� üũ�Ѵ�.  
	// Ư�� ������Ʈ�� �����۵��� �����Ѵ�.
	// �ϴ� �ʿ��� ���� ����� ���Ѵ�.
	mszCounterList = NULL; // m�� ��Ƽ�� �ǹ�
	mszInstanceList = NULL;

	// ���� ������ �μ��� 0���� �����ϸ� �Ʒ� �Լ� ����� �ʿ��� ���� ����� �ش� �μ��� �ùٸ��� �Է����ش�.
	cchCounterList = 0;
	cchInstanceList = 0;
	status = PdhEnumObjectItems(NULL, szMachine, szObject,
		NULL, &cchCounterList,
		NULL, &cchInstanceList, PERF_DETAIL_WIZARD, 0);

	// ������ ���� ���̰� 2�� ���(NULL ���� 2��)�� ��ü�� �������� �ʴ´ٴ� �ǹ̴�. ���� ������ ������ �ʿ� ����.
	if (cchInstanceList == 2) {
		goto finish;
	}

	// �޸𸮸� �Ҵ��Ѵ�. ������ ���� ���� ������ ���� ���ڿ��� ����(NULL ����)�̹Ƿ� �Ҵ����� �����Ѵ�.
	mszCounterList = (LPTSTR)malloc(cchCounterList * sizeof(TCHAR));
	if (!mszCounterList) {
		//SetLog(TEXT("mszCounterList �޸� �Ҵ�(malloc)�� �����Ͽ����ϴ�. - CoreAndProcessUsageByPDH"));
		goto finish;
	}

	mszInstanceList = (LPTSTR)malloc(cchInstanceList * sizeof(TCHAR));
	if (!mszInstanceList) {
		//SetLog(TEXT("mszInstanceList �޸� �Ҵ�(malloc)�� �����Ͽ����ϴ�. - CoreAndProcessUsageByPDH"));
		goto finish;
	}

	// �Ҵ�� ���ۿ� ����� �ְ� �����Լ��� �ٽ� �����Ѵ�.
	status = PdhEnumObjectItems(NULL, szMachine, szObject,
		mszCounterList, &cchCounterList,
		mszInstanceList, &cchInstanceList, PERF_DETAIL_WIZARD, 0);

	if (status != ERROR_SUCCESS) {
		//SetLog(TEXT("PdhEnumObjectItems - CoreAndProcessUsageByPDH"));
		goto error_pdh;
	}

	// �ν��Ͻ��� �� ������ ���Ѵ�.
	cInstance = 0;
	szInstance = mszInstanceList;
	while (*szInstance) {
		cInstance++;
		szInstance += _tcslen(szInstance) + 1;
	}

	// �� �ν��Ͻ� �̸��� ���� ��� �ε��� �۾��� ����� �ȴ�. �ε����� 0���� �����Ѵ�. ex) iexplorer.exe�� 2�� �̻� �����ϴ� ��� ��
	// �� �ν��Ͻ����� �ε����� ������ �޸� �Ҵ�
	pIndexOfInstance = (int *)malloc(sizeof(int) * cInstance);
	if (!pIndexOfInstance) {
		//SetLog(TEXT("pIndexOfInstance �޸� �Ҵ�(malloc)�� �����Ͽ����ϴ�. - CoreAndProcessUsageByPDH"));
		goto finish;
	}

	// �� �ν��Ͻ�����  �˻�
	for (i = 0; i < cInstance; i++) {

		// i��° �ν��Ͻ� �̸��� ���Ѵ�.
		pszCur = mszInstanceList;
		for (j = 0; j < i; j++) {
			pszCur += _tcslen(pszCur) + 1;
		}

		// ù �κк��� ���� �̸��� �ִ��� �����Ѵ�. �ش� �ν��Ͻ� �κп� �����ϱ� �������� �˻��ϸ� �ȴ�.
		// ���� ���ڿ��� �߰ߵǸ� �ε����� 1 ������Ű�� �ȴ�.
		index = 0;
		szInstance = mszInstanceList;
		for (j = 0; j < i; j++) {
			if (0 == _tcsicmp(szInstance, pszCur)) { // ���ڿ��� ������ �ε����� 1 ������Ų��.
				index++;
			}
			szInstance += _tcslen(szInstance) + 1;
		}

		// �����Ѵ�.
		pIndexOfInstance[i] = index;
	}

	// �ν��Ͻ����� ī���� �ڵ��� ������ �޸𸮸� �Ҵ��Ѵ�. 
	phCounter_Instance = (HCOUNTER *)malloc(sizeof(HCOUNTER) * cInstance * ITEM_PER_PROC);
	if (!phCounter_Instance) {
		//SetLog(TEXT("phCounter_Instance �޸� �Ҵ�(malloc)�� �����Ͽ����ϴ�. - CoreAndProcessUsageByPDH"));
		goto finish;
	}

	memset(phCounter_Instance, 0, sizeof(HCOUNTER) * cInstance * ITEM_PER_PROC);

	// �ν��Ͻ����� �ش� ���Ŀ� �°� ī���� �̸�(Processor Time), �ε����� ������(#index) ī���͸� �߰��ϰ� �ڵ��� ���Ѵ�.
	szInstance = mszInstanceList;
	for (i = 0; i < cInstance; i++) {

		// �� FullCounterPath ����: \������Ʈ �̸�(�ν��Ͻ� �̸�#�ε���)\% ī���� �̸�  ex) \Process(iexplorer#1)\% Processor Time

		//Process Time �׸� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s#%d)\\%% Processor Time"), szObject, szInstance, pIndexOfInstance[i]);

		if (!IsPhdList(szFullCounterPath, false))
		{
			if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2))
				m_ProcssTime.push_back(PhdItem(phCounter_Instance[i * 2], szFullCounterPath));
		}

		//Process I/O read �׸� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s#%d)\\IO Read Bytes/sec"), szObject, szInstance, pIndexOfInstance[i]);
		if (!IsPhdList(szFullCounterPath, false))
		{
			if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 1))
				m_ProcssIoReadByte.push_back(PhdItem(phCounter_Instance[i * 2 + 1], szFullCounterPath));
		}

		//Process I/O Write �׸� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s#%d)\\IO Write Bytes/sec"), szObject, szInstance, pIndexOfInstance[i]);

		if (!IsPhdList(szFullCounterPath, false))
		{
			if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 2))
				m_ProcssIoWriteByte.push_back(PhdItem(phCounter_Instance[i * 2 + 2], szFullCounterPath));
		}

		//Process I/O Data �׸� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s#%d)\\IO Data Bytes/sec"), szObject, szInstance, pIndexOfInstance[i]);

		if (!IsPhdList(szFullCounterPath, false))
		{
			if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 3))
				m_ProcssIoDataByte.push_back(PhdItem(phCounter_Instance[i * 2 + 3], szFullCounterPath));
		}

		//Process I/O Other �׸� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s#%d)\\IO Other Bytes/sec"), szObject, szInstance, pIndexOfInstance[i]);

		if (!IsPhdList(szFullCounterPath, false))
		{
			if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 4))
				m_ProcssIoOtherByte.push_back(PhdItem(phCounter_Instance[i * 2 + 4], szFullCounterPath));
		}



		//Process I/O Read operation

		_stprintf(szFullCounterPath, TEXT("\\%s(%s#%d)\\IO Read Operations/sec"), szObject, szInstance, pIndexOfInstance[i]);

		if (!IsPhdList(szFullCounterPath, false))
		{
			if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 5))
				m_ProcssIoReadOper.push_back(PhdItem(phCounter_Instance[i * 2 + 5], szFullCounterPath));
		}

		//Process I/O Write operation

		_stprintf(szFullCounterPath, TEXT("\\%s(%s#%d)\\IO Write Operations/sec"), szObject, szInstance, pIndexOfInstance[i]);

		if (!IsPhdList(szFullCounterPath, false))
		{
			if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 6))
				m_ProcssIoWriteOper.push_back(PhdItem(phCounter_Instance[i * 2 + 6], szFullCounterPath));
		}

		//Process I/O Data operation

		_stprintf(szFullCounterPath, TEXT("\\%s(%s#%d)\\IO Data Operations/sec"), szObject, szInstance, pIndexOfInstance[i]);

		if (!IsPhdList(szFullCounterPath, false))
		{
			if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 7))
				m_ProcssIoDataOper.push_back(PhdItem(phCounter_Instance[i * 2 + 7], szFullCounterPath));
		}

		szInstance += _tcslen(szInstance) + 1;
	}

error_pdh:
	// pdh.dll ���κ��� �����޽����� �����ؾ� �ϹǷ� �Ϲ� �ý��� �����޽����ʹ� �μ��� �ٸ��� �����Ѵ�.(�ٸ� �κ� �ּ� ó��) 

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE, //FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		GetModuleHandle(TEXT("pdh.dll")), //NULL,
		status, //GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpszErrorBuf,
		0,
		NULL))
	{
		//SetLog((LPCTSTR)lpszErrorBuf);
		LocalFree(lpszErrorBuf);
	}

finish:
	if (mszCounterList) free(mszCounterList);
	if (mszInstanceList) free(mszInstanceList);
	if (pIndexOfInstance) free(pIndexOfInstance);
	if (phCounter_Instance) free(phCounter_Instance);
}

void CSystemMon::UpdateDiskList()
{
	LPCTSTR szMachine = NULL;
	LPCTSTR szObject = TEXT("PhysicalDisk");

	PDH_STATUS status;
	DWORD cchBuffer = 0;
	DWORD cchCounterList = 0;
	DWORD cchInstanceList = 0;
	LPTSTR mszCounterList = NULL; // msz�� ��Ƽ��Ʈ���� �ǹ��Ѵ�. ���� ���ڿ��� ������ NULL�� �ϰ� �� ������ NULL�� ���� 2���� �´�.
	LPTSTR mszInstanceList = NULL;
	LPTSTR szInstance = NULL;
	LPTSTR szCounter = NULL;

	DWORD cInstance = 0;

	HCOUNTER *phCounter_Instance = NULL;
	TCHAR szFullCounterPath[1024] = { 0 };
	DWORD i, j;

	int *pIndexOfInstance = NULL;  // �ν��Ͻ� �̸��� ���� ��� �ε��� �۾��� ����
	LPTSTR pszCur = NULL;
	int index;

	LPTSTR lpszErrorBuf; // ���� �޽����ڽ� ����(FormatMessage ����)

	// ���� ���μ����� üũ�Ѵ�.  
	// Ư�� ������Ʈ�� �����۵��� �����Ѵ�.
	// �ϴ� �ʿ��� ���� ����� ���Ѵ�.
	mszCounterList = NULL; // m�� ��Ƽ�� �ǹ�
	mszInstanceList = NULL;

	// ���� ������ �μ��� 0���� �����ϸ� �Ʒ� �Լ� ����� �ʿ��� ���� ����� �ش� �μ��� �ùٸ��� �Է����ش�.
	cchCounterList = 0;
	cchInstanceList = 0;
	status = PdhEnumObjectItems(NULL, szMachine, szObject,
		NULL, &cchCounterList,
		NULL, &cchInstanceList, PERF_DETAIL_WIZARD, 0);

	// ������ ���� ���̰� 2�� ���(NULL ���� 2��)�� ��ü�� �������� �ʴ´ٴ� �ǹ̴�. ���� ������ ������ �ʿ� ����.
	if (cchInstanceList == 2) {
		goto finish;
	}

	// �޸𸮸� �Ҵ��Ѵ�. ������ ���� ���� ������ ���� ���ڿ��� ����(NULL ����)�̹Ƿ� �Ҵ����� �����Ѵ�.
	mszCounterList = (LPTSTR)malloc(cchCounterList * sizeof(TCHAR));
	if (!mszCounterList) {
		//SetLog(TEXT("mszCounterList �޸� �Ҵ�(malloc)�� �����Ͽ����ϴ�. - CoreAndProcessUsageByPDH"));
		goto finish;
	}

	mszInstanceList = (LPTSTR)malloc(cchInstanceList * sizeof(TCHAR));
	if (!mszInstanceList) {
		//SetLog(TEXT("mszInstanceList �޸� �Ҵ�(malloc)�� �����Ͽ����ϴ�. - CoreAndProcessUsageByPDH"));
		goto finish;
	}

	// �Ҵ�� ���ۿ� ����� �ְ� �����Լ��� �ٽ� �����Ѵ�.
	status = PdhEnumObjectItems(NULL, szMachine, szObject,
		mszCounterList, &cchCounterList,
		mszInstanceList, &cchInstanceList, PERF_DETAIL_WIZARD, 0);

	if (status != ERROR_SUCCESS) {
		//SetLog(TEXT("PdhEnumObjectItems - CoreAndProcessUsageByPDH"));
		goto error_pdh;
	}

	// �ν��Ͻ��� �� ������ ���Ѵ�.
	cInstance = 0;
	szInstance = mszInstanceList;
	while (*szInstance) {
		cInstance++;
		szInstance += _tcslen(szInstance) + 1;
	}

	// �� �ν��Ͻ� �̸��� ���� ��� �ε��� �۾��� ����� �ȴ�. �ε����� 0���� �����Ѵ�. ex) iexplorer.exe�� 2�� �̻� �����ϴ� ��� ��
	// �� �ν��Ͻ����� �ε����� ������ �޸� �Ҵ�
	pIndexOfInstance = (int *)malloc(sizeof(int) * cInstance);
	if (!pIndexOfInstance) {
		//SetLog(TEXT("pIndexOfInstance �޸� �Ҵ�(malloc)�� �����Ͽ����ϴ�. - CoreAndProcessUsageByPDH"));
		goto finish;
	}

#if(0)
	// �� �ν��Ͻ�����  �˻�
	for (i = 0; i < cInstance; i++) {

		// i��° �ν��Ͻ� �̸��� ���Ѵ�.
		pszCur = mszInstanceList;
		for (j = 0; j < i; j++) {
			pszCur += _tcslen(pszCur) + 1;
		}

		// ù �κк��� ���� �̸��� �ִ��� �����Ѵ�. �ش� �ν��Ͻ� �κп� �����ϱ� �������� �˻��ϸ� �ȴ�.
		// ���� ���ڿ��� �߰ߵǸ� �ε����� 1 ������Ű�� �ȴ�.
		index = 0;
		szInstance = mszInstanceList;
		for (j = 0; j < i; j++) {
			if (0 == _tcsicmp(szInstance, pszCur)) { // ���ڿ��� ������ �ε����� 1 ������Ų��.
				index++;
			}
			szInstance += _tcslen(szInstance) + 1;
		}

		// �����Ѵ�.
		pIndexOfInstance[i] = index;
	}
#endif

	// �ν��Ͻ����� ī���� �ڵ��� ������ �޸𸮸� �Ҵ��Ѵ�. 
	phCounter_Instance = (HCOUNTER *)malloc(sizeof(HCOUNTER) * cInstance * ITEM_PER_PROC);
	if (!phCounter_Instance) {
		//SetLog(TEXT("phCounter_Instance �޸� �Ҵ�(malloc)�� �����Ͽ����ϴ�. - CoreAndProcessUsageByPDH"));
		goto finish;
	}

	memset(phCounter_Instance, 0, sizeof(HCOUNTER) * cInstance * ITEM_PER_PROC);

	// �ν��Ͻ����� �ش� ���Ŀ� �°� ī���� �̸�(Processor Time), �ε����� ������(#index) ī���͸� �߰��ϰ� �ڵ��� ���Ѵ�.
	szInstance = mszInstanceList;
	for (i = 0; i < cInstance; i++) {

		// �� FullCounterPath ����: \������Ʈ �̸�(�ν��Ͻ� �̸�#�ε���)\% ī���� �̸�  ex) \Process(iexplorer#1)\% Processor Time

		//Disk Time �׸� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s)\\Avg. Disk Bytes/Transfer"), szObject, szInstance);

		if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2))
			m_DiskList.push_back(PhdItem(phCounter_Instance[i * 2], szFullCounterPath));

		//Disk read �ð� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s)\\Avg. Disk Bytes/Read"), szObject, szInstance);
		if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 1))
			m_DiskList.push_back(PhdItem(phCounter_Instance[i * 2 + 1], szFullCounterPath));

		//Disk Write �ð� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s)\\Avg. Disk Bytes/Write"), szObject, szInstance);

		if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 2))
			m_DiskList.push_back(PhdItem(phCounter_Instance[i * 2 + 2], szFullCounterPath));


		//Disk Time �׸� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s)\\Disk Bytes/sec"), szObject, szInstance);

		if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 3))
			m_DiskList.push_back(PhdItem(phCounter_Instance[i * 2 + 3], szFullCounterPath));


		//Disk read �ð� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s)\\Disk Read Bytes/sec"), szObject, szInstance);
		if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 4))
			m_DiskList.push_back(PhdItem(phCounter_Instance[i * 2 + 4], szFullCounterPath));

		//Disk Write �ð� ������Ʈ

		_stprintf(szFullCounterPath, TEXT("\\%s(%s)\\Disk Write Bytes/sec"), szObject, szInstance);

		if (ERROR_SUCCESS == PdhAddCounter(m_hQuery, szFullCounterPath, 0, phCounter_Instance + i * 2 + 5))
			m_DiskList.push_back(PhdItem(phCounter_Instance[i * 2 + 5], szFullCounterPath));

		szInstance += _tcslen(szInstance) + 1;
	}

error_pdh:
	// pdh.dll ���κ��� �����޽����� �����ؾ� �ϹǷ� �Ϲ� �ý��� �����޽����ʹ� �μ��� �ٸ��� �����Ѵ�.(�ٸ� �κ� �ּ� ó��) 

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE, //FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		GetModuleHandle(TEXT("pdh.dll")), //NULL,
		status, //GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpszErrorBuf,
		0,
		NULL))
	{
		//SetLog((LPCTSTR)lpszErrorBuf);
		LocalFree(lpszErrorBuf);
	}

finish:
	if (mszCounterList) free(mszCounterList);
	if (mszInstanceList) free(mszInstanceList);
	if (pIndexOfInstance) free(pIndexOfInstance);
	if (phCounter_Instance) free(phCounter_Instance);
}

void  CSystemMon::UpdateNetworkLisk()
{
	int N = 0;
	bool bErr = false;
	TCHAR *szCur = NULL;
	TCHAR *szBuff = NULL;
	TCHAR *szNet = NULL;
	DWORD dwBuffSize = 0, dwNetSize = 0;
	TCHAR szCounter[256];
	PDH_STATUS status;


	// calculate buffer size
	PdhEnumObjectItems(NULL, NULL, TEXT("Network Interface"), szBuff, &dwBuffSize, szNet, &dwNetSize, PERF_DETAIL_WIZARD, 0);

	if ((szBuff = (TCHAR *)malloc(sizeof(TCHAR) * dwBuffSize)) == NULL)
	{
		return;
	}

	if ((szNet = (TCHAR *)malloc(sizeof(TCHAR) * dwNetSize)) == NULL)
	{
		free(szBuff);
		return;
	}

	if (PdhEnumObjectItems(NULL, NULL, TEXT("Network Interface"), szBuff, &dwBuffSize, szNet, &dwNetSize, PERF_DETAIL_WIZARD, 0) != ERROR_SUCCESS)
	{
		free(szBuff);
		free(szNet);
		return;
	}

	szCur = szNet;
	while (*szCur) {
		N++;
		szCur += _tcslen(szCur) + 1;
	}

	szCur = szNet;
	HCOUNTER hCounter;

	for (int i = 0; i < N; i++)
	{
		_stprintf(szCounter, TEXT("\\Network Interface(%s)\\Current Bandwidth"), szCur);

		status = PdhAddCounter(m_hQuery, szCounter, 0, &hCounter);
		if (ERROR_SUCCESS == status)
			m_NetworkList.push_back(PhdItem(hCounter, szCounter));


		_stprintf(szCounter, TEXT("\\Network Interface(%s)\\Bytes Received/sec"), szCur);

		status = PdhAddCounter(m_hQuery, szCounter, 0, &hCounter);
		if (ERROR_SUCCESS == status)
			m_NetworkList.push_back(PhdItem(hCounter, szCounter));


		_stprintf(szCounter, TEXT("\\Network Interface(%s)\\Bytes Sent/sec"), szCur);

		status = PdhAddCounter(m_hQuery, szCounter, 0, &hCounter);
		if (ERROR_SUCCESS == status)
			m_NetworkList.push_back(PhdItem(hCounter, szCounter));


		_stprintf(szCounter, TEXT("\\Network Interface(%s)\\Packets Received/sec"), szCur);

		status = PdhAddCounter(m_hQuery, szCounter, 0, &hCounter);
		if (ERROR_SUCCESS == status)
			m_NetworkList.push_back(PhdItem(hCounter, szCounter));


		_stprintf(szCounter, TEXT("\\Network Interface(%s)\\Packets Sent/sec"), szCur);

		status = PdhAddCounter(m_hQuery, szCounter, 0, &hCounter);
		if (ERROR_SUCCESS == status)
			m_NetworkList.push_back(PhdItem(hCounter, szCounter));

		szCur += _tcslen(szCur) + 1;
	}

	if (szBuff != NULL)
		free(szBuff);

	if (szNet != NULL)
		free(szNet);
}

void CSystemMon::ThreadTask()
{
	while (m_thRuning) 
	{
		WaitForSingleObject(m_hRefresh, INFINITE);

		if (m_thRuning) {
			ResetEvent(m_hRefresh);
			Monitor();
		}
	}
}

DWORD CSystemMon::Monitor()
{
	LPCTSTR szMachine = NULL;
	LPCTSTR szObject = TEXT("Process");

	PDH_STATUS status;
	DWORD cchBuffer = 0;
	PDH_FMT_COUNTERVALUE CounterValue;

	LPTSTR lpszErrorBuf; // ���� �޽����ڽ� ����(FormatMessage ����)

	ULONGLONG s = GetTickCount64();

	UpdateProcessList();

	SetLog(TEXT("Start %ld"), GetTickCount64() - s);

	// ���� ���Ѵ�.
	/*
	�� PdhCollectQueryData �ѹ� ȣ�⸸���δ� ���� ����� ���� �� ����.
	�ι� �̻� ȣ���ؾ� �ȴ�.(���� ���ð� ���� ���� ���ؼ� ���� �����Ѵ�.)
	�ѹ��� ȣ���ϸ� 99.9999�� 0.0000���� �� ���� ���̴�.

	����� �ھ� ����ó�� ���ϴ� ī���Ͱ� ������ ���
	if (k == 0) PdhCollectQueryData(hQuery);
	Sleep(1000);
	PdhCollectQueryData(hQuery);
	���� ���� ������ �׻� 2�� ȣ���� �ƴ϶� ù ������ ��츸 ��ȣ�� ó�����ָ� �� ���̴�.
	(�� ��� �ۼ��� �Ը��� �°� �ڵ��ؼ� Sleep�� ������ �α� �� �ٸ� ��ġ�ε� ���� ���� ���� ���̴�.)

	�׷��� ���μ��� �ν��Ͻ��� �ֱ������� �������õǹǷ� �������� ���� ī���͸� ����(PdhRemoveCounter)�ϰ� ���� �߰�(PdhAddCounter)�ؾ� �Ѵ�.
	������ ���� �߰�(���)�ϴ� ���� ���� ������ ���� �����̹Ƿ�  �׻� PdhCollectQueryData�� 2�� �̻� ȣ������� ����� ���� ���� �� �ִ�.
	*/
	if (m_reQuery) {
		PdhCollectQueryData(m_hQuery);
		Sleep(1000);
		m_reQuery = false;
	}
	PdhCollectQueryData(m_hQuery);

	SetLog(TEXT("---------------------CPU Load ---------------------"));
	// �� CPU �ν��Ͻ��� ������ ǥ���Ѵ�.
	for (auto it = m_CpuList.begin(); it != m_CpuList.end(); ++it) {

		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS == status) {
			if (0 < it->value.doubleValue)
				SetLog(TEXT("%s: %.1f"), it->name.c_str(), it->value.doubleValue);
		}
	}

	SetLog(TEXT("---------------------HDD Load ---------------------"));
	// �� ��ũ ������ ǥ���Ѵ�.
	for (auto it = m_DiskList.begin(); it != m_DiskList.end(); ++it) {

		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS == status) {
			if (0 < it->value.doubleValue)
				SetLog(TEXT("%s: %.1f"), it->name.c_str(), it->value.doubleValue);
		}
	}

	SetLog(TEXT("---------------------Network Load ---------------------"));
	// �� ��Ʈ��ũ ������ ǥ���Ѵ�.
	for (auto it = m_NetworkList.begin(); it != m_NetworkList.end(); ++it) {

		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS == status) {
			if (0 < it->value.doubleValue)
				SetLog(TEXT("%s: %.1f"), it->name.c_str(), it->value.doubleValue);
		}
	}


	bool result = true;

	// �� ���μ��� �ν��Ͻ��� ������ ǥ���Ѵ�.
	for (auto it = m_ProcssTime.begin(); it != m_ProcssTime.end(); ++it) {
		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS != status)
			result = false;
	}

	for (auto it = m_ProcssIoReadByte.begin(); it != m_ProcssIoReadByte.end(); ++it) {
		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS != status)
			result = false;
	}

	for (auto it = m_ProcssIoWriteByte.begin(); it != m_ProcssIoWriteByte.end(); ++it) {
		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS != status)
			result = false;
	}

	for (auto it = m_ProcssIoDataByte.begin(); it != m_ProcssIoDataByte.end(); ++it) {
		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS != status)
			result = false;
	}

	for (auto it = m_ProcssIoOtherByte.begin(); it != m_ProcssIoOtherByte.end(); ++it) {
		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS != status)
			result = false;
	}

	for (auto it = m_ProcssIoReadOper.begin(); it != m_ProcssIoReadOper.end(); ++it) {
		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS != status)
			result = false;
	}

	for (auto it = m_ProcssIoWriteOper.begin(); it != m_ProcssIoWriteOper.end(); ++it) {
		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS != status)
			result = false;
	}

	for (auto it = m_ProcssIoDataOper.begin(); it != m_ProcssIoDataOper.end(); ++it) {
		status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, NULL, &it->value);

		if (ERROR_SUCCESS != status)
			result = false;
	}

	std::sort(m_ProcssTime.begin(), m_ProcssTime.end());
	std::sort(m_ProcssIoReadByte.begin(), m_ProcssIoReadByte.end());
	std::sort(m_ProcssIoWriteByte.begin(), m_ProcssIoWriteByte.end());
	std::sort(m_ProcssIoDataByte.begin(), m_ProcssIoDataByte.end());
	std::sort(m_ProcssIoOtherByte.begin(), m_ProcssIoOtherByte.end());
	std::sort(m_ProcssIoReadOper.begin(), m_ProcssIoReadOper.end());
	std::sort(m_ProcssIoWriteOper.begin(), m_ProcssIoWriteOper.end());
	std::sort(m_ProcssIoDataOper.begin(), m_ProcssIoDataOper.end());

	int N = 0;

	SetLog(TEXT("---------------------Process time Top 10---------------------"));

	for (auto it = m_ProcssTime.rbegin(); it != m_ProcssTime.rend() && N < 10; ++it, ++N)
		SetLog(TEXT("%02d => %s : %.1f"), N, it->name.c_str(), it->value.doubleValue);

	N = 0;
	SetLog(TEXT("---------------------I/O Read Bytes Top 10---------------------"));
	for (auto it = m_ProcssIoReadByte.rbegin(); it != m_ProcssIoReadByte.rend() && N < 10; ++it, ++N)
		SetLog(TEXT("%02d => %s : %.1f"), N, it->name.c_str(), it->value.doubleValue);

	N = 0;
	SetLog(TEXT("---------------------I/O Write Bytes Top 10---------------------"));
	for (auto it = m_ProcssIoWriteByte.rbegin(); it != m_ProcssIoWriteByte.rend() && N < 10; ++it, ++N)
		SetLog(TEXT("%02d => %s : %.1f"), N, it->name.c_str(), it->value.doubleValue);

	N = 0;
	SetLog(TEXT("---------------------I/O Data Bytes Top 10---------------------"));
	for (auto it = m_ProcssIoDataByte.rbegin(); it != m_ProcssIoDataByte.rend() && N < 10; ++it, ++N)
		SetLog(TEXT("%02d => %s : %.1f"), N, it->name.c_str(), it->value.doubleValue);

	N = 0;
	SetLog(TEXT("---------------------I/O Other Bytes Top 10---------------------"));
	for (auto it = m_ProcssIoOtherByte.rbegin(); it != m_ProcssIoOtherByte.rend() && N < 10; ++it, ++N)
		SetLog(TEXT("%02d => %s : %.1f"), N, it->name.c_str(), it->value.doubleValue);

	N = 0;
	SetLog(TEXT("---------------------I/O Read Operation Top 10---------------------"));
	for (auto it = m_ProcssIoReadOper.rbegin(); it != m_ProcssIoReadOper.rend() && N < 10; ++it, ++N)
		SetLog(TEXT("%02d => %s : %.1f"), N, it->name.c_str(), it->value.doubleValue);

	N = 0;
	SetLog(TEXT("---------------------I/O Write Opertation Top 10---------------------"));
	for (auto it = m_ProcssIoWriteOper.rbegin(); it != m_ProcssIoWriteOper.rend() && N < 10; ++it, ++N)
		SetLog(TEXT("%02d => %s : %.1f"), N, it->name.c_str(), it->value.doubleValue);

	N = 0;
	SetLog(TEXT("---------------------I/O Operation Bytes Top 10---------------------"));
	for (auto it = m_ProcssIoDataOper.rbegin(); it != m_ProcssIoDataOper.rend() && N < 10; ++it, ++N)
		SetLog(TEXT("%02d => %s : %.1f"), N, it->name.c_str(), it->value.doubleValue);


	//����� ���μ��� ����Ʈ���� ����
	if (!result) {
		IsPhdList(tstring(), true);
		m_reQuery = true;
	}

	ULONGLONG e0 = GetTickCount64();
	
//	SetLog(_T("Process data tact (%ld Items) : %ld"), m_PhdList.size(),  e0 - s);

	// ������ �ӽ��� ���������Ѵ�.(������Ʈ ���� ������ �ƴ�) NULL���� ��� ���� ��ǻ��. 
	// �Ʒ� ������ ���� ������ ���μ��� ����� ������Ʈ���� �ʰ� ������ ���ߴ� �� �״���̴�.
	cchBuffer = 0;
	PdhEnumObjects(NULL, szMachine, NULL, &cchBuffer, PERF_DETAIL_WIZARD, TRUE); // �� ������ �ý��� ���ϰ� �� �ɸ��� ��,,,

	ULONGLONG e1 = GetTickCount64();

	SetLog(_T("PdhEnumObjects tact : %ld"), e1 - s);

	return 1;


error_pdh:

	// pdh.dll ���κ��� �����޽����� �����ؾ� �ϹǷ� �Ϲ� �ý��� �����޽����ʹ� �μ��� �ٸ��� �����Ѵ�.(�ٸ� �κ� �ּ� ó��) 

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE, //FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		GetModuleHandle(TEXT("pdh.dll")), //NULL,
		status, //GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpszErrorBuf,
		0,
		NULL))
	{
		SetLog((LPCTSTR)lpszErrorBuf);
		LocalFree(lpszErrorBuf);
	}

	return 0;
}
