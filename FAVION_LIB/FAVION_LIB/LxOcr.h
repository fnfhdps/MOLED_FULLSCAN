#pragma once

#define USING_MATCH 1

#include "LxDraw.h"

#ifdef USING_FIND
#include "LxFind.h"
#endif

#ifdef USING_MATCH
#include "LxMatch.h"
#endif

#include "LxTrace.h"
#include "LxMatch.h"

//---------------------------------------------------------------------------
#define MAX_OCR_FONT 50
//---------------------------------------------------------------------------

class AFX_EXT_CLASS COcr  : public CFilter
{
public:
	COcr(void);
	~COcr(void);

public :

	bool IsPass;
	bool DwTrainId;
	bool DwScore;
	bool DwIRoi;
	bool DwFRoi;

	CString PathFont;
	CString ReadString;
	CString ReferString;

	int ResultFindCount;
	int PrevFindCount;
	struct TResult {
		bool IsPass;
		float Score;
		int TrainId;
		JRect Rect ;
		CString Simbol;
	};

	TResult Result[MAX_OCR_FONT];
	TResult Prev[MAX_OCR_FONT];

	EROIBW8 FontRoi;
	JTrace Trace;

#ifdef USING_FIND
	CFind FontFind[MAX_OCR_FONT];
#endif

#ifdef USING_MATCH
	CMatch FontLoad;
	CMatch FontTrain;
	CMatch FontFind[MAX_OCR_FONT];
#endif

public :
	bool ReferEnable0;
	bool ReferEnable1;
	bool ReferEnable2;
	int FindReferStringId;

public :
	struct TProperty {
		int Mode; // 0:ByBlob, 1:BySort, 2:ByRoi
		int TrMargin;
		int ShMarginX;
		int ShMarginY;
		int ShMove;
		int ShMinusX;
		int ShMinusY;
		float ShBlobScore;
		float PassBlobScore;
		CString Simbol;
		CString ReferString0;
		CString ReferString1;
		CString ReferString2;
	} Property;


	int IpIndex;
	bool IsTrain, IsMaunalTrain;

	EROIBW8 ORoi[10];
	EROIBW8 FRoi[10];

	struct TArea {
		int Count;
		float Gx[MAX_OCR_FONT];
		float Gy[MAX_OCR_FONT];
		JRect Body;
		JRect Rect[MAX_OCR_FONT];
	} Area;

	struct TTrain {
		int Count;
		CString ReferString;
		//CRect FontRect[MAX_OCR_FONT];
		CString Simbol[MAX_OCR_FONT];
	} Train;


public :

	EROIBW8 DRoi; // For Display
	EROIBW8 AreaRoi;
	CMatch AreaFind;

	bool IsEnabled;
	int  OnRoiSetting(CString Simbol);
	int  OnManualRoiSetting(CString Simbol);
	int  OnExecute();
	int  OnExecuteBySrh();
	int  OnExecuteSort(CString ReferStr);
	int  OnExecuteBlob(CString ReferStr);
	int  OnExecuteRoi (CString ReferStr);
	void OnLoadRoi(bool IsLoad, CString Path, EROIBW8 *lpRoi);
	void OnLoad(bool IsLoad, CString Folder);
	void OnPaint(TDrawTool Tool);

	TSortMode SortMode;
	void OnSortRect();


	int OnTrainByTRoi(CString Simbol);
	void OnLoadByPath(CString Path);
	void OnLoadByReferString();
	void OnSaveByReferString(CString Char);

};

