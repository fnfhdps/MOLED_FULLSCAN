// LxFileUnit.cpp: implementation of the CFileUnit class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LxFileUnit.h"

#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFolder::CFolder()
{

}

CFolder::~CFolder()
{

}
BOOL CFolder::ExistsDir(CString Path)
{
	if (GetFileAttributes(Path) == -1) return FALSE; // -1 is No Exist
	else return TRUE;
}

BOOL CFolder::RemoveDirAll(CString Path)
{
	if( Path == _T("") )  
	{  
		return FALSE;  
	}  

	BOOL bResult = FALSE; 
	int nRval = 0;

	CString szNextDirPath   = _T(""); 
	CString szRoot = _T("");

	szRoot.Format(_T("%s\\*.*"), Path);

	CFileFind find; 

	bResult = find.FindFile( szRoot );

	if( bResult == FALSE )  
	{        
		return bResult;  
	}

	while( bResult )  
	{  
		bResult = find.FindNextFile();

		if( find.IsDots() == FALSE )   
		{   
			if( find.IsDirectory() )   
			{   
				RemoveDirAll(find.GetFilePath());   
			}   
			else   
			{    
				bResult = DeleteFile(find.GetFilePath()); 
			}  
		}
	} 
	find.Close();
	bResult = RemoveDirectory( Path );          
	return bResult;
}

void CFolder::ClearDir(CString Path)
{
	if( Path == _T("") )  
		return;  

	BOOL bResult = FALSE; 
	int nRval = 0; 

	CString szNextDirPath   = _T(""); 
	CString szRoot = _T("");

	szRoot.Format(_T("%s\\*.*"), Path);

	CFileFind find; 

	bResult = find.FindFile( szRoot );

	if( bResult == FALSE )  
		return ;  

	while( bResult )  
	{  
		bResult = find.FindNextFile();

		if( find.IsDots() == FALSE )   
		{   
			if( find.IsDirectory() )   
			{   
				ClearDir(find.GetFilePath());   
			}   
			else   
			{    
				bResult = DeleteFile(find.GetFilePath()); 
			}  
		}
	} 
	find.Close();
}

CString CFolder::GetExeDir()
{
	TCHAR pszPathName[_MAX_PATH];

	::GetModuleFileName(::AfxGetInstanceHandle(), pszPathName, _MAX_PATH); 

	CString strPathName = pszPathName;

	int i = strPathName.ReverseFind('\\');

	strPathName = strPathName.Left(i);

	return strPathName;
}
/*
int HFolder::SearchDir(AnsiString Path, TStringGrid *Grid, int DispType, bool DispDate)
{
	TSearchRec sr;
	int Count=0;
	AnsiString Item [MAX_FILE] ;
	int        IDate[MAX_FILE];

	for(int i=0;i<Grid->RowCount;i++)
	{
		Grid->Rows[i]->Clear();
	}
	Grid->RowCount = 2;

	AnsiString SercHFolder = ExpandFileName(NormalDir(Path)+"*.*");
	if(FindFirst(SercHFolder, faAnyFile, sr) == 0){
		do{
			if((sr.Attr & faDirectory) == sr.Attr){
				if((sr.Name == ".") || (sr.Name == "..")) continue;
				if(Count>=MAX_FILE) break;
				Item [Count] = sr.Name;
				IDate[Count] = SearchFileDate(NormalDir(Path) + sr.Name +"\\*.*", sr.Time);
				Count ++;
			}
		}while(FindNext(sr) == 0);
	}
	FindClose(sr);
	if(Count == 0) return 0;
	AnsiString tempName;
	int        tempTime;

	//내림차순
	if (DispType == 1) {
		for(int i=0; i<Count; i++)
		{
			for(int j=i; j<Count; j++)
			{
				if(IDate[i]<IDate[j])
				{
					tempName = Item [i]; Item [i] = Item [j]; Item[ j] = tempName;
					tempTime = IDate[i]; IDate[i] = IDate[j]; IDate[j] = tempTime;
				}
			}
		}
	}
	Grid->RowCount = Count;
	if(Grid->ColCount < 3) DispDate = false;
	for(int i=0; i<Count; i++)
	{
		Grid->Cells[0][i] = i+1;
		Grid->Cells[1][i] = Item [i];
		if(DispDate) Grid->Cells[2][i] = FileDateToDateTime(IDate [i]);
	}
	Grid->Row = 0;	
	return Count;
}
*/

//////////////////////////////////////////////////////////////////////
// HIFile Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIFile::CIFile()
{

}
CIFile::~CIFile()
{	
	
}
void CIFile::Load(CString Path, CString Caption, CString Id, float* Value)
{
	if (Path   .IsEmpty()) return;
	if (Caption.IsEmpty()) return;
	if (Id     .IsEmpty()) return;

	WCHAR sDir[MAX_PATH] = {0,};

	GetPrivateProfileString(Caption, Id, L"0", sDir, MAX_PATH, Path);
	//*Value = (FLOAT) _tstof(sDir);
	*Value = (FLOAT) _wtof(sDir);
}

void CIFile::Load(CString Path, CString Caption, CString Id, double* Value)
{
	if (Path   .IsEmpty()) return;
	if (Caption.IsEmpty()) return;
	if (Id     .IsEmpty()) return;

	WCHAR sDir[MAX_PATH] = {0,};

	GetPrivateProfileString(Caption, Id, L"0", sDir, MAX_PATH, Path);
	//*Value = (double) _tstof(sDir);
	*Value = (double) _wtof(sDir);
}

void CIFile::Save(CString Path, CString Caption, CString Id, float Data)
{
	if (Path   .IsEmpty()) return;
	if (Caption.IsEmpty()) return;
	if (Id     .IsEmpty()) return;

	TCHAR sDir[MAX_PATH] = {0,};
	CString strTmp;
	strTmp.Format(_T("%.5f"),Data);
	WritePrivateProfileString(Caption, Id, strTmp, Path);
}

void CIFile::Save(CString Path, CString Caption, CString Id, double Data)
{
	if (Path   .IsEmpty()) return;
	if (Caption.IsEmpty()) return;
	if (Id     .IsEmpty()) return;

	TCHAR sDir[MAX_PATH] = {0,};
	CString strTmp;
	strTmp.Format(_T("%.5f"),Data);
	WritePrivateProfileString(Caption, Id, strTmp, Path);
}

void CIFile::Load(CString Path, CString Caption, CString Id, bool* Value)
{
	if (Path   .IsEmpty()) return;
	if (Caption.IsEmpty()) return;
	if (Id     .IsEmpty()) return;

	TCHAR sDir[MAX_PATH] = {0,};
	CString str;

	GetPrivateProfileString(Caption, Id, _T("false"), sDir, MAX_PATH, Path);
	str.Format(_T("%s"),sDir);
	*Value = (str == _T("true")) ? true : false;

}

void CIFile::Save(CString Path, CString Caption, CString Id, bool Data)
{
	if (Path   .IsEmpty()) return;
	if (Caption.IsEmpty()) return;
	if (Id     .IsEmpty()) return;

	TCHAR sDir[MAX_PATH] = {0,};
	CString str = (Data) ? _T("true") : _T("false");

	WritePrivateProfileString(Caption, Id, str, Path);
}

void CIFile::Load(CString Path, CString Caption, CString Id, int* Value)
{
	if (Path   .IsEmpty()) return;
	if (Caption.IsEmpty()) return;
	if (Id     .IsEmpty()) return;

	TCHAR sDir[MAX_PATH] = {0,};

//	GetPrivateProfileStringW(Caption, Id, _T("0"), sDir, MAX_PATH, Path);
	GetPrivateProfileString(Caption, Id, _T("0"), sDir, MAX_PATH, Path);
	*Value = _ttoi(sDir);
}

void CIFile::Save(CString Path, CString Caption, CString Id, int Data)
{
	if (Path   .IsEmpty()) return;
	if (Caption.IsEmpty()) return;
	if (Id     .IsEmpty()) return;

	TCHAR sDir[MAX_PATH] = {0,};
	CString strTmp;
	strTmp.Format(_T("%d"),Data);
	WritePrivateProfileString(Caption, Id, strTmp, Path);
}

void CIFile::Load(CString Path, CString Caption, CString Id, CString* Value)
{
	if (Path   .IsEmpty()) return;
	if (Caption.IsEmpty()) return;
	if (Id     .IsEmpty()) return;

	TCHAR sDir[MAX_PATH] = {0,};

	GetPrivateProfileString(Caption, Id, _T("0"), sDir, MAX_PATH, Path);
	*Value = (CString)sDir;
}

void CIFile::Save(CString Path, CString Caption, CString Id, CString Value)
{
	if (Path   .IsEmpty()) return;
	if (Caption.IsEmpty()) return;
	if (Id     .IsEmpty()) return;

	TCHAR sDir[MAX_PATH] = {0,};
	WritePrivateProfileString(Caption, Id, Value, Path);
}

void CIFile::Load(bool IsLoad, CString Path, CString Caption, CString Id, CString* Value)
{
	if (IsLoad) CIFile::Load(Path, Caption, Id, Value);
	else        CIFile::Save(Path, Caption, Id, *Value);
}

void CIFile::Load(bool IsLoad, CString Path, CString Caption, CString Id, float* Value)
{
	if (IsLoad) CIFile::Load(Path, Caption, Id, Value);
	else        CIFile::Save(Path, Caption, Id, *Value);
}

void CIFile::Load(bool IsLoad, CString Path, CString Caption, CString Id, double* Value)
{
	if (IsLoad) CIFile::Load(Path, Caption, Id, Value);
	else        CIFile::Save(Path, Caption, Id, *Value);
}

void CIFile::Load(bool IsLoad, CString Path, CString Caption, CString Id, int* Value)
{
	if (IsLoad) CIFile::Load(Path, Caption, Id, Value);
	else        CIFile::Save(Path, Caption, Id, *Value);
}

void CIFile::Load(bool IsLoad, CString Path, CString Caption, CString Id, bool* Value)
{
	if (IsLoad) CIFile::Load(Path, Caption, Id, Value);
	else        CIFile::Save(Path, Caption, Id, *Value);
}
//////////////////////////////////////////////////////////////////////
// CFileUnit Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFileUnit::CFileUnit(void)
{
}
//---------------------------------------------------------------------------
CFileUnit::~CFileUnit(void)
{
}
//---------------------------------------------------------------------------
BOOL CFileUnit::ExistPath(LPCTSTR lpszPath)
{
    if (_access(LPSTR(lpszPath), 0) == -1)
		return FALSE;
	else
	    return TRUE;
}
//---------------------------------------------------------------------------
CString CFileUnit::GetExeFileName(void)
{
	return AfxGetApp()->m_pszAppName;
}
//---------------------------------------------------------------------------
CString CFileUnit::GetFolderPath(LPCTSTR lpszFolder)
{
	CString strBuff;
	strBuff.Format(_T("%s\\%s"), ExtractExeFilePath(), lpszFolder);
	return strBuff;
}
//---------------------------------------------------------------------------
CString CFileUnit::GetCurrentDir(void)
{
	CString strBuff;
	::GetCurrentDirectory(STRING_BUFFER_SIZE, strBuff.GetBuffer(STRING_BUFFER_SIZE));
	strBuff.ReleaseBuffer();
	return strBuff;
}
//---------------------------------------------------------------------------
CString CFileUnit::ExtractExeFilePath(void)
{
	CString strFilePath;
	
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFname[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	int nPos;
	
	_tsplitpath(strFilePath, szDrive, szDir, szFname, szExt);

	if (!szDrive[0])
	{
		CString strModule;
		GetModuleFileName(NULL, strModule.GetBuffer(STRING_BUFFER_SIZE), STRING_BUFFER_SIZE);
		strModule.ReleaseBuffer();

		_tsplitpath(strModule, szDrive, szDir, szFname, szExt);

		strModule = szDrive;
		strModule += szDir;
		
		strFilePath = strModule;
		
		strModule.MakeUpper();
		nPos = strModule.Find(_T("DEBUG"));
		if (nPos >= 0)
		{
			strFilePath = strFilePath.Left(nPos);
		}
		else
		{
			nPos = strModule.Find(_T("RELEASE"));
			if (nPos >= 0)
			{
				strFilePath = strFilePath.Left(nPos);
			}
		}


	}
	
	strFilePath.TrimRight(_T("\\"));
	
	return strFilePath;
}
//---------------------------------------------------------------------------
CString CFileUnit::GetWindowsDir(void)
{
	CString strBuff;
	::GetWindowsDirectory(strBuff.GetBuffer(STRING_BUFFER_SIZE), STRING_BUFFER_SIZE);
	strBuff.ReleaseBuffer();
	return strBuff;
}
//---------------------------------------------------------------------------
CString CFileUnit::GetSystemDir(void)
{
	CString strBuff;
	::GetSystemDirectory(strBuff.GetBuffer(STRING_BUFFER_SIZE), STRING_BUFFER_SIZE);
	strBuff.ReleaseBuffer();
	return strBuff;
}
//---------------------------------------------------------------------------
float CFileUnit::GetHddDiskUsedSpaceRate(TCHAR chDir)
{
	float fUsedSpaceRate = 0.f;
	TCHAR szDrive[10];
	ULARGE_INTEGER i64FreeBytesToCaller, i64TotalBytes,i64FreeBytes;
    FARPROC pGetDiskFreeSpaceEx = GetProcAddress( GetModuleHandle(_T("kernel32.dll")), "GetDiskFreeSpaceExA");
	
	if (!pGetDiskFreeSpaceEx)
		return fUsedSpaceRate;


	if (chDir)
	{
		wsprintf(szDrive, TEXT("%c:\\"), chDir);

		GetDiskFreeSpaceEx (szDrive,(PULARGE_INTEGER)&i64FreeBytesToCaller,
            (PULARGE_INTEGER)&i64TotalBytes,(PULARGE_INTEGER)&i64FreeBytes);
	}
	else
	{
        GetDiskFreeSpaceEx (NULL,(PULARGE_INTEGER)&i64FreeBytesToCaller,
            (PULARGE_INTEGER)&i64TotalBytes,(PULARGE_INTEGER)&i64FreeBytes);
	}

	
	double dFreeByte = (double)((_int64)(i64FreeBytes.QuadPart));
	double dTotalByte = (double)((_int64)(i64TotalBytes.QuadPart));
	
	fUsedSpaceRate = (float)(100 - (dFreeByte/dTotalByte)*100);
	
	return fUsedSpaceRate;
}
//---------------------------------------------------------------------------
BOOL CFileUnit::BrowseDir(CString &strPath)
{
	return 0;
	//     return SelectDirectory(AFolderName, TSelectDirOpts() << sdAllowCreate << sdPerformCreate << sdPrompt
	//            ,0);
}
//---------------------------------------------------------------------------
BOOL CFileUnit::FileExist(LPCTSTR lpszFileName)
{
	return ExistPath(lpszFileName);
}
//---------------------------------------------------------------------------
CString CFileUnit::FindFile(LPCTSTR lpszPath, LPCTSTR lpszFileName)
{
	CString strFileName = _T("");

	HANDLE hFile = NULL;
	TCHAR szBuff[STRING_BUFFER_SIZE];
	WIN32_FIND_DATA TFindData;
	
	CString strPath;
	
	strPath.Format(_T("%s"), lpszPath);
	strPath.TrimRight();
	strPath.TrimRight(BACK_SLASH);
	
	wsprintf(szBuff, TEXT("%s\\*.*"), strPath);
	
	hFile = ::FindFirstFile(szBuff, &TFindData);
	
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		return strFileName;
	
	while(FindNextFile(hFile, &TFindData))
	{
		wsprintf(szBuff, TFindData.cFileName);
		if (_tcscmp(szBuff, _T(".")) == 0 || _tcscmp(szBuff, _T("..")) == 0)
			continue;

		if (_tcsstr(szBuff, lpszFileName))
		{
			strFileName.Format(_T("%s"), szBuff);
			int nExpIndex = strFileName.FindOneOf(_T("."));
			int nLength = strFileName.GetLength();
			strFileName.Delete(nExpIndex, nLength - nExpIndex);

			break;
		}
	}
	
	FindClose(hFile);



	return strFileName;
}
//---------------------------------------------------------------------------
BOOL CFileUnit::FileCopy(LPCTSTR szFromFile, LPCTSTR lpszToFile)
{
	return ((CopyFile(szFromFile, lpszToFile, FALSE))?TRUE:FALSE);
}
//---------------------------------------------------------------------------
BOOL CFileUnit::FileDelete(LPCTSTR lpszFileName)
{
	return DeleteFile(lpszFileName);
}
//---------------------------------------------------------------------------
BOOL CFileUnit::FileRename(LPCTSTR szFromFile, LPCTSTR lpszToFile)
{
	if (FileCopy(szFromFile, lpszToFile))
	{
		if (FileDelete(szFromFile))
			return TRUE;
	}
	
	return FALSE;
}
//---------------------------------------------------------------------------
BOOL CFileUnit::FileMove(LPCTSTR szFromFile, LPCTSTR lpszToFile)
{
	return ((MoveFile(szFromFile, lpszToFile))?TRUE:FALSE);
}
//---------------------------------------------------------------------------
BOOL CFileUnit::AllFileDelete(LPCTSTR lpszPath)
{
	HANDLE hFile = NULL;
	TCHAR szBuff[STRING_BUFFER_SIZE];
	WIN32_FIND_DATA TFindData;
	
	CString strPath;
	strPath.Format(_T("%s"), lpszPath);
	strPath.TrimRight();
	strPath.TrimRight(BACK_SLASH);

	wsprintf(szBuff, TEXT("%s\\*.*"), strPath);
	
	
	hFile = ::FindFirstFile(szBuff, &TFindData);
	
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		return FALSE;
	
	while(FindNextFile(hFile, &TFindData))
	{
		wsprintf(szBuff, TFindData.cFileName);
		if (_tcscmp(szBuff, _T(".")) == 0 || _tcscmp(szBuff, _T("..")) == 0)
			continue;
		
		wsprintf(szBuff, TEXT("%s\\%s"), strPath, TFindData.cFileName);
		
		DeleteFile(szBuff);
	}
	
	FindClose(hFile);
	
	return TRUE;
}
//---------------------------------------------------------------------------
DWORD CFileUnit::GetFileSize(LPCTSTR lpszFileName)
{
	HANDLE hFile;
	WIN32_FIND_DATA TFindData;
	
	hFile = ::FindFirstFile(lpszFileName, &TFindData);
	
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		return 0;
	
	FindClose(hFile);
	
	return TFindData.nFileSizeHigh * MAXDWORD + TFindData.nFileSizeLow; 
}
//---------------------------------------------------------------------------
BOOL CFileUnit::ExistDir(LPCTSTR lpszPath)
{
	return ExistPath(lpszPath);
}
//---------------------------------------------------------------------------
BOOL CFileUnit::CreateDir(LPCTSTR lpszPath)
{
	if (!ExistDir(lpszPath))
	{
		if (::CreateDirectory(lpszPath, NULL)) 
			return TRUE;
		else                       
			return FALSE;
	}
	
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL CFileUnit::DeleteDir(LPCTSTR lpszPath)
{
	HANDLE hFile = NULL;
	TCHAR szBuff[STRING_BUFFER_SIZE];
	WIN32_FIND_DATA TFindData;
	BOOL bResult = TRUE;
	CString strPath;
	
	strPath.Format(_T("%s"), lpszPath);
	strPath.TrimRight();
	strPath.TrimRight(BACK_SLASH);
	
	wsprintf(szBuff, TEXT("%s\\*.*"), strPath);
	
	hFile = ::FindFirstFile(szBuff, &TFindData);
	
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		return FALSE;
	
	while(FindNextFile(hFile, &TFindData))
	{
		wsprintf(szBuff, TFindData.cFileName);
		if (_tcscmp(szBuff, _T(".")) == 0 || _tcscmp(szBuff, _T("..")) == 0)
			continue;

		if (TFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	
		{
			wsprintf(szBuff, TEXT("%s\\%s"), strPath, TFindData.cFileName);
			if (DeleteDir(szBuff) == FALSE)
				bResult = FALSE;
		}
		else
		{
			wsprintf(szBuff, TEXT("%s\\%s"), strPath, TFindData.cFileName);
			if (FileDelete(szBuff) == FALSE)
				bResult = FALSE;
		}
	}
	
	FindClose(hFile);
	
	if (bResult) 
		return RemoveDirectory(lpszPath);
	
	return FALSE;
}
//---------------------------------------------------------------------------
BOOL CFileUnit::CopyDir(LPCTSTR lpszFromPath, LPCTSTR lpszToPath)
{
	HANDLE hFile = NULL;
	TCHAR szBuff[STRING_BUFFER_SIZE];
	TCHAR szFromSubPath[STRING_BUFFER_SIZE];
	TCHAR szToSubPath[STRING_BUFFER_SIZE];
	WIN32_FIND_DATA TFindData;
	BOOL bResult = TRUE;
	
	CString strFromPath;
	strFromPath.Format(_T("%s"), lpszFromPath);
	strFromPath.TrimRight();
	strFromPath.TrimRight(BACK_SLASH);
	
	wsprintf(szBuff, TEXT("%s\\*.*"), strFromPath);
	
	hFile = ::FindFirstFile(szBuff, &TFindData);
	
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		return FALSE;
	
	if (CreateDir(lpszToPath) == FALSE)
		return FALSE;
	
	while(FindNextFile(hFile, &TFindData))
	{
		wsprintf(szBuff, TFindData.cFileName);
		if (_tcscmp(szBuff, _T(".")) == 0 || _tcscmp(szBuff, _T("..")) == 0)
			continue;
		
		if (TFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	
		{
			wsprintf(szFromSubPath, TEXT("%s\\%s"), lpszFromPath, TFindData.cFileName);
			wsprintf(szToSubPath, TEXT("%s\\%s"), lpszToPath, TFindData.cFileName);
			
			if (CopyDir(szFromSubPath, szToSubPath) == FALSE)
				bResult = FALSE;
		}
		else
		{
			wsprintf(szFromSubPath, TEXT("%s\\%s"), lpszFromPath, szBuff);
			wsprintf(szToSubPath, TEXT("%s\\%s"), lpszToPath, szBuff);

			if (FileCopy(szFromSubPath, szToSubPath) == FALSE)
				bResult = FALSE;
		}
	}
	
	FindClose(hFile);
	
	return bResult;
}
//---------------------------------------------------------------------------
BOOL CFileUnit::MoveDir(LPCTSTR lpszFromPath, LPCTSTR lpszToPath)
{
	if (CopyDir(lpszFromPath, lpszToPath))
	{
		DeleteDir(lpszFromPath);
		return TRUE;
	}
	
	return FALSE;
}
//---------------------------------------------------------------------------
BOOL CFileUnit::ReNameDir(LPCTSTR lpszFromPath, LPCTSTR lpszToPath)
{
	return MoveDir(lpszFromPath, lpszToPath);
}
//---------------------------------------------------------------------------
BOOL CFileUnit::MakePathDir(LPCTSTR lpszPath)
{
	TCHAR szBuff[STRING_BUFFER_SIZE];
	int nCount = 0;
	int nPathSize;
	
	CString strPath;
	strPath.Format(_T("%s"), lpszPath);
	strPath.TrimRight();
	strPath.TrimRight(BACK_SLASH);
	nPathSize = strPath.GetLength();

	while(nCount < nPathSize)
	{
		nCount ++;
		if (lpszPath[nCount] == _T('\\'))
		{
			wsprintf(szBuff, TEXT("%s\\%s"), ExtractExeFilePath(), strPath.Mid(0, nCount));
			if (CreateDir(szBuff) == FALSE)
				return FALSE;
		}
	}

	wsprintf(szBuff, TEXT("%s\\%s"), ExtractExeFilePath(), strPath);
	
	return CreateDir(szBuff);
}
//---------------------------------------------------------------------------
BOOL CFileUnit::MakeFullPathDir(LPCTSTR lpszPath)
{
	int nSize = _tcslen(lpszPath);
	if (nSize <= 0)
		return FALSE;

	TCHAR szBuff[STRING_BUFFER_SIZE];
	int nCount = 0;
	int nPathSize;
	
	CString strPath;
	strPath.Format(_T("%s"), lpszPath);
	strPath.TrimRight();
	strPath.TrimRight(BACK_SLASH);
	nPathSize = strPath.GetLength();
	
	while(nCount < nPathSize)
	{
		nCount ++;
		if (lpszPath[nCount] == _T('\\'))
		{
			wsprintf(szBuff, TEXT("%s"), strPath.Mid(0, nCount));
			if (CreateDir(szBuff) == FALSE)
				return FALSE;
		}
	}
	
	wsprintf(szBuff, TEXT("%s"), strPath);
	
	return CreateDir(szBuff);
}
//---------------------------------------------------------------------------
int  CFileUnit::GetFileCount(LPCTSTR lpszPath)
{
	HANDLE hFile = NULL;
	TCHAR szBuff[STRING_BUFFER_SIZE];
	WIN32_FIND_DATA TFindData;
	int nCount = 0;
	
	CString strPath;
	strPath.Format(_T("%s"), lpszPath);
	strPath.TrimRight();
	strPath.TrimRight(BACK_SLASH);
	
	wsprintf(szBuff, TEXT("%s\\*.*"), strPath);
	hFile = ::FindFirstFile(szBuff, &TFindData);
	
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		return FALSE;
	
	while(FindNextFile(hFile, &TFindData))
	{
		if (_tcscmp(szBuff, _T(".")) == 0 || _tcscmp(szBuff, _T("..")) == 0)
			continue;

		if (!(TFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))	
			nCount ++;
	}
	
	FindClose(hFile);
	
	return nCount;
}
//---------------------------------------------------------------------------
int  CFileUnit::GetSubDirectoryCount(LPCTSTR lpszPath)
{
	HANDLE hFile = NULL;
	TCHAR szBuff[STRING_BUFFER_SIZE];
	WIN32_FIND_DATA TFindData;
	int nCount = 0;
	
	CString strPath;
	strPath.Format(_T("%s"), lpszPath);
	strPath.TrimRight();
	strPath.TrimRight(BACK_SLASH);
	
	wsprintf(szBuff, TEXT("%s\\*.*"), strPath);
	
	hFile = ::FindFirstFile(szBuff, &TFindData);
	
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		return FALSE;
	
	while(FindNextFile(hFile, &TFindData))
	{
		wsprintf(szBuff, TFindData.cFileName);
		if (_tcscmp(szBuff, _T(".")) == 0 || _tcscmp(szBuff, _T("..")) == 0)
			continue;
		if (TFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	
			nCount ++;
	}
	
	FindClose(hFile);
	
	return nCount;
}
//---------------------------------------------------------------------------
BOOL CFileUnit::ClearDirectoryFiles(LPCTSTR lpszPath)
{
	HANDLE hFile = NULL;
	TCHAR szBuff[STRING_BUFFER_SIZE];
	WIN32_FIND_DATA TFindData;
	BOOL bResult = TRUE;
	
	CString strPath;
	strPath.Format(_T("%s"), lpszPath);
	strPath.TrimRight();
	strPath.TrimRight(BACK_SLASH);
	
	wsprintf(szBuff, TEXT("%s\\*.*"), strPath);
	
	hFile = ::FindFirstFile(szBuff, &TFindData);
	
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		return FALSE;
	
	while(FindNextFile(hFile, &TFindData))
	{
		wsprintf(szBuff, TFindData.cFileName);
		if (_tcscmp(szBuff, _T(".")) == 0 || _tcscmp(szBuff, _T("..")) == 0)
			continue;

		if (TFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	
		{
			wsprintf(szBuff, TEXT("%s\\%s"), strPath, TFindData.cFileName);
			if (ClearDirectoryFiles(szBuff) == FALSE)
				bResult = FALSE;
		}
		else
		{
			wsprintf(szBuff, TEXT("%s\\%s"), lpszPath, TFindData.cFileName);
			if (FileDelete(szBuff) == FALSE)
				bResult = FALSE;
			
		}
	}
	
	FindClose(hFile);
	
	return bResult;
}
//---------------------------------------------------------------------------
BOOL CFileUnit::ClearDirectoryByDayCount(LPCTSTR lpszPath, UINT nDayCount, TFILE_TIME_TYPE TFileTimeType)
{
	HANDLE hFile = NULL;
	BOOL bResult = TRUE;
	TCHAR szBuff[STRING_BUFFER_SIZE];
	TCHAR szAttri[STRING_BUFFER_SIZE];

	
	WIN32_FIND_DATA TFindData;
	FILETIME FileTime;
	
	time_t Today;
	time_t CutLine;
	
	CString strPath;
	strPath.Format(_T("%s"), lpszPath);
	strPath.TrimRight();
	strPath.TrimRight(BACK_SLASH);
	
	wsprintf(szBuff, TEXT("%s\\*.*"), strPath);
	
	hFile = ::FindFirstFile(szBuff, &TFindData);
	
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		return FALSE;
	
	while(FindNextFile(hFile, &TFindData))
	{
		wsprintf(szBuff, TFindData.cFileName);
		if (_tcscmp(szBuff, _T(".")) == 0 || _tcscmp(szBuff, _T("..")) == 0)
			continue;
		
		if (TFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	
		{
			wsprintf(szBuff, TEXT("%s\\%s"), strPath, TFindData.cFileName);
			if (ClearDirectoryByDayCount(szBuff, nDayCount, TFileTimeType) == FALSE)
				bResult = FALSE;
		}
		else
		{
			switch (TFileTimeType)
			{
			case ftCreation:
				FileTime = TFindData.ftCreationTime;
				break;
			case ftLastAccess:
				FileTime = TFindData.ftLastAccessTime;
				break;
			case ftLastWrite:
				FileTime = TFindData.ftLastWriteTime;
				break;
			}
			
			time(&Today);
			CutLine = Today - (SECONDS_OF_ONE_DAY * nDayCount);
			CTime *pTimeFile = new CTime(FileTime);


			//-----------------------------------------------
			// Delete Read Only File
			//-----------------------------------------------
			DWORD dwFileAttri = TFindData.dwFileAttributes;
			if (dwFileAttri & FILE_ATTRIBUTE_READONLY)
			{

				dwFileAttri &= ~FILE_ATTRIBUTE_READONLY;
				dwFileAttri |= FILE_ATTRIBUTE_NORMAL;
				
				wsprintf(szAttri, TEXT("%s\\%s"), strPath, szBuff); 
				SetFileAttributes(szAttri, dwFileAttri);
			}
			
			if (pTimeFile->GetTime() < CutLine) 
			{
				wsprintf(szBuff, TEXT("%s\\%s"), strPath, TFindData.cFileName);
				if (FileDelete(szBuff) == FALSE)
					bResult = FALSE;
			}
			
			delete pTimeFile;
		}
	}
	
	FindClose(hFile);
	
	return bResult;
}
//---------------------------------------------------------------------------
int CFileUnit::GetPathData(LPCTSTR lpszPath, CStringArray *pName, CStringArray *pTime, BOOL bFolder)
{
	HANDLE hFile = NULL;
	BOOL bStatus;
	TCHAR szBuff[STRING_BUFFER_SIZE];
	int nCount = 0;
	
	WIN32_FIND_DATA TFindData;
	SYSTEMTIME SystemTime;


	CString strPath;
	strPath.Format(_T("%s"), lpszPath);
	strPath.TrimRight();
	strPath.TrimRight(BACK_SLASH);
	
	wsprintf(szBuff, TEXT("%s\\*.*"), strPath);
	
	hFile = ::FindFirstFile(szBuff, &TFindData);
	
	if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		return FALSE;
	
	while(FindNextFile(hFile, &TFindData))
	{
		wsprintf(szBuff, TFindData.cFileName);
		if (_tcscmp(szBuff, _T(".")) == 0 || _tcscmp(szBuff, _T("..")) == 0)
			continue;
			
		bStatus = TFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;	
		bStatus = (bFolder && bStatus) || (!bFolder && !bStatus);
		
		if (bStatus)
		{
			FileTimeToSystemTime(&TFindData.ftLastWriteTime, &SystemTime);
			
			pName->Add(szBuff);
			
			wsprintf(szBuff, TEXT("%04d:%02d:%02d"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay);
			pTime->Add(szBuff);
			nCount ++;
		}
	}
	
	FindClose(hFile);
	
	return nCount;
}
//---------------------------------------------------------------------------
int CFileUnit::GetFileList(LPCTSTR lpszPath, CStringArray *pName)
{
	CStringArray arTime;
	return GetPathData(lpszPath, pName, &arTime, FALSE);
}
//---------------------------------------------------------------------------
int CFileUnit::GetFileList(LPCTSTR lpszPath, CStringArray *pName, CStringArray *pTime)
{
	return GetPathData(lpszPath, pName, pTime, FALSE);
}
//---------------------------------------------------------------------------
int CFileUnit::GetFolderList(LPCTSTR lpszPath, CStringArray *pName)
{
	CStringArray arTime;
	return GetPathData(lpszPath, pName, &arTime, TRUE);
}
//---------------------------------------------------------------------------
int CFileUnit::GetFolderList(LPCTSTR lpszPath, CStringArray *pName, CStringArray *pTime)
{
	return GetPathData(lpszPath, pName, pTime, TRUE);
}

//------------------------------------------------------------------
// class KFileDialog
//--------------------------------------------------------------------
void KFileDialog::SetFileFilter(bool bBmp, bool bJpg, bool bAll)
{

	if (bBmp == true)
		strFilter += "bmp Files (*.BMP)|*.BMP|";

	if (bJpg == true)
		strFilter += "jpg Files (*.JPG)|*.JPG|";

	if (bAll == true)
		strFilter += "All Files (*.*)|*.*||";
}
//--------------------------------------------------------------------------
CString KFileDialog::GetPathOpenFileDlg()
{
	if (strFilter == "")
		strFilter ="All Files (*.*)|*.*||";

	//	CFileDialog FileDlg(TRUE, ".BMP", NULL, 0, strFilter);
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, strFilter);

	if( FileDlg.DoModal() == IDOK )
	{
		CString str  =  FileDlg.GetPathName();
		return  str;
		//return FileDlg.GetPathName();
	}

	return "";
}
//--------------------------------------------------------------------------
CString KFileDialog::GetPathSaveFileDlg()
{
	if (strFilter =="")
		strFilter = "All Files (*.*)|*.*||";

	CFileDialog FileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

	if( FileDlg.DoModal() == IDOK )
	{
		return FileDlg.GetPathName();
	}

	return "";
}
//--------------------------------------------------------------------------
CString KFileDialog::GetFolderPath()
{
	CFolderPickerDialog dlg(NULL,OFN_FILEMUSTEXIST);

	if(IDOK == dlg.DoModal())
	{
		return dlg.GetFolderPath();
	}

	return "";
}
//--------------------------------------------------------------------------