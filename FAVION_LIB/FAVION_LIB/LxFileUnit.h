// LxFileUnit.h: interface for the CFileUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXFILEUNIT_H__D00D9DE9_D069_434B_ADBA_1DA09312B35E__INCLUDED_)
#define AFX_LXFILEUNIT_H__D00D9DE9_D069_434B_ADBA_1DA09312B35E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRING_BUFFER_SIZE		0xFF
#define BACK_SLASH				0x5C
#define SECONDS_OF_ONE_DAY     (60*60*24)

typedef enum __TFILE_TIME_TYPE
{
	     ftCreation,
		ftLastAccess,
		ftLastWrite
}TFILE_TIME_TYPE;

//--------------------------------------------------------
#include "LxHeader.h"
//--------------------------------------------------------
class AFX_EXT_CLASS CFileUnit //: public CFileFind
{
private:
protected:
	
public:
	
    //-----------------------------------
    // Constructor & Destructor
    //-----------------------------------
	CFileUnit();
	~CFileUnit();
	
	
    //-----------------------------------
    // Get File Path
    //-----------------------------------
	BOOL ExistPath(LPCTSTR lpszPath);
	CString GetExeFileName();
	CString GetFolderPath(LPCTSTR lpszFolder);
	CString GetCurrentDir(void);
	CString ExtractExeFilePath(void);
	
	
    //-----------------------------------
    // Get System
    //-----------------------------------
	CString GetWindowsDir(void);
	CString GetSystemDir(void);
	float GetHddDiskUsedSpaceRate(TCHAR chDir = NULL);
	BOOL BrowseDir(CString &strPath);
	
	
    //-----------------------------------
    // File Control
    //-----------------------------------
	BOOL FileExist(LPCTSTR lpszFileName);
	CString FindFile(LPCTSTR lpszPath, LPCTSTR lpszFileName);
	BOOL FileCopy(LPCTSTR lpszFromFile, LPCTSTR lpszToFile);
	BOOL FileDelete(LPCTSTR lpszFileName);
	BOOL FileRename(LPCTSTR lpszFromFile, LPCTSTR lpszToFile);
	BOOL FileMove(LPCTSTR lpszFromFile, LPCTSTR lpszToFile);
	BOOL AllFileDelete(LPCTSTR lpszPath);
	DWORD GetFileSize(LPCTSTR lpszFileName);
	
    
	//-----------------------------------
    // Directory Control
    //-----------------------------------
	BOOL ExistDir(LPCTSTR lpszPath);
	BOOL CreateDir(LPCTSTR lpszPath);
	BOOL DeleteDir(LPCTSTR lpszPath);
	BOOL CopyDir(LPCTSTR lpszFromPath, LPCTSTR lpszToPath);
	BOOL MoveDir(LPCTSTR lpszFromPath, LPCTSTR lpszToPath);
	BOOL ReNameDir(LPCTSTR FromPath, LPCTSTR lpszToPath);
	BOOL MakePathDir(LPCTSTR lpszPath);
	BOOL MakeFullPathDir(LPCTSTR lpszPath);
	
	
	int  GetFileCount(LPCTSTR lpszPath);
	int  GetSubDirectoryCount(LPCTSTR lpszPath);
	BOOL ClearDirectoryFiles(LPCTSTR lpszPath);
	BOOL ClearDirectoryByDayCount(LPCTSTR lpszPath, UINT nDayCount, TFILE_TIME_TYPE TFileTimeType = ftCreation);
	
	
	//-----------------------------------
    // Get Path Data
    //-----------------------------------
	int GetPathData(LPCTSTR lpszPath, CStringArray *pName, CStringArray *pTime, BOOL bFolder);
	int GetFileList(LPCTSTR lpszPath, CStringArray *pName);
	int GetFileList(LPCTSTR lpszPath, CStringArray *pName, CStringArray *pTime);
	int GetFolderList(LPCTSTR lpszPath, CStringArray *pName);
	int GetFolderList(LPCTSTR lpszPath, CStringArray *pName, CStringArray *pTime);
	
};
class AFX_EXT_CLASS CFolder  
{
public:
	CFolder();
	~CFolder();
	
public :
	BOOL 	ExistsDir(CString Path);
	void 	CreateDir(CString Path) { CreateDirectory(Path,NULL); }
	BOOL 	RemoveDir(CString Path) { return RemoveDirectory(Path); }
	BOOL 	RemoveFile(CString Path) { return DeleteFile(Path); }
	BOOL 	RemoveDirAll(CString Path);
	void 	ClearDir(CString Path);
	CString GetExeDir();
	//bool    CopyDir        (AnsiString FileName, AnsiString DestName);
	//GOO ?
	BOOL    CopyFile(CString Src, CString Dest);
	//int     SearchDir(CString Path, TStringGrid *Grid, int DispTyep, bool DispDate)
};

class AFX_EXT_CLASS CIFile  
{
public:
	CIFile();
	~CIFile();
	
public :
protected: 
	void  Load(CString Path, CString Caption, CString Id, bool*       Value);
	void  Save(CString Path, CString Caption, CString Id, bool        Data );
	void  Load(CString Path, CString Caption, CString Id, double*     Value);
	void  Save(CString Path, CString Caption, CString Id, double      Data );
	void  Load(CString Path, CString Caption, CString Id, float*      Value);
	void  Save(CString Path, CString Caption, CString Id, float       Data );
	void  Load(CString Path, CString Caption, CString Id, int*        Value);
	void  Save(CString Path, CString Caption, CString Id, int         Data );
	void  Load(CString Path, CString Caption, CString Id, CString*    Value);
	void  Save(CString Path, CString Caption, CString Id, CString     Data );
	
public :
	void  Load(bool IsLoad, CString Path, CString Caption, CString Id, double*     Value);
	void  Load(bool IsLoad, CString Path, CString Caption, CString Id, float*      Value);
	void  Load(bool IsLoad, CString Path, CString Caption, CString Id, bool*       Value);
	void  Load(bool IsLoad, CString Path, CString Caption, CString Id, int*        Value);
	void  Load(bool IsLoad, CString Path, CString Caption, CString Id, CString*    Value);
	
};

class AFX_EXT_CLASS KFileDialog
{
public :
	KFileDialog() { strFilter = ""; }
	void	SetFileFilter(bool bBmp, bool bJpg, bool bAll);
	CString GetPathOpenFileDlg();
	CString GetPathSaveFileDlg();
	CString GetFolderPath();

private :
	CString strFilter;
};


#endif // !defined(AFX_LXFILEUNIT_H__D00D9DE9_D069_434B_ADBA_1DA09312B35E__INCLUDED_)
