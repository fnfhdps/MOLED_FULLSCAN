#pragma once

#include "TheLogger.h"
#include <string>
#include <vector>

// 추가해야 할 라이브러리와 헤더 파일
#pragma comment(lib, "pdh.lib")
#include <pdh.h>
#include <pdhmsg.h>


#ifndef tstring
typedef std::basic_string<TCHAR> tstring;
#endif

typedef struct PhdItem
{
	HCOUNTER counter;

	tstring  name;
	tstring  group;

	PDH_FMT_COUNTERVALUE value;

	PhdItem(const HCOUNTER c, tstring n)
		: counter(c), name(n)
	{
		value.doubleValue = 0;

		size_t pos = n.find(tstring(TEXT("\\")));
		
		if (tstring::npos != pos)
			group = n.substr(pos);
	}

	bool operator <(const PhdItem& b)
	{
		return value.doubleValue < b.value.doubleValue;
	}

	bool operator >(const PhdItem& b)
	{
		return value.doubleValue > b.value.doubleValue;
	}

	bool operator ==(const PhdItem& b)
	{
		return (group == b.group
			&& value.doubleValue == b.value.doubleValue);
	}

} PhdItem;

#define ITEM_PER_PROC		8

class AFX_EXT_CLASS CSystemMon : public CTheLogger
{
public:
	CSystemMon();
	~CSystemMon();

	void	InitSystemMon();
	void	Update();

	//연속 호출시 Delay 2초
	DWORD	Monitor();

public:

private:
	void	UpdateCpuList();
	void	UpdateProcessList();
	void	UpdateDiskList();
	void	UpdateNetworkLisk();
	void	DestroySystemMon();
	void	ThreadTask();

	BOOL	Run();
	static DWORD WINAPI	StartUp(LPVOID p);

	bool	IsPhdList(const tstring& name, bool full);

	HQUERY					m_hQuery;
	bool					m_reQuery;

	std::vector<PhdItem>	m_CpuList;
	std::vector<PhdItem>	m_DiskList;
	std::vector<PhdItem>	m_NetworkList;

	std::vector<PhdItem>	m_ProcssTime;
	std::vector<PhdItem>	m_ProcssIoReadByte;
	std::vector<PhdItem>	m_ProcssIoWriteByte;
	std::vector<PhdItem>	m_ProcssIoDataByte;
	std::vector<PhdItem>	m_ProcssIoOtherByte;

	std::vector<PhdItem>	m_ProcssIoReadOper;
	std::vector<PhdItem>	m_ProcssIoWriteOper;
	std::vector<PhdItem>	m_ProcssIoDataOper;

	HANDLE					m_hRefresh;
	HANDLE					m_hThread;
	bool					m_thRuning;

};

