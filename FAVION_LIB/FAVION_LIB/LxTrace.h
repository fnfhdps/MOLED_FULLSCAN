#pragma once
#include "LxHeader.h"
//---------------------------------------------------------------------------
#define MAX_IGNORE_RECT  500
#define MAX_JUNC 10
//---------------------------------------------------------------------------
enum TSortMode {smRightBtm, smLeftBtm, smRightTop, smLeftTop,
	smBtmRight, smBtmLeft, smTopRight, smTopLeft};


class AFX_EXT_CLASS JTrace : public CFilter
{
public :
	int FShape; // 0:Rect, 1:Circle
	bool IgSort;
	TSortMode SortMode;
	int IgBlobCount;
	JRect IgBlob[MAX_IGNORE_RECT];

public :
	bool OpJuncCount, LtJunc;
	int JuncCount;
	TPoint Junc[MAX_JUNC];

public :
// 	void *TAddress;
// 	void *MAddress;
	//TPoint *FPoint;
	int PixelCount;
	EROIBW8 DRoi;
	JRect MgRect;

	//Trace Result
	//-------------------------------------------------------------------
	int BlobCount;
	struct TBlob {
		BYTE *Pixel; TPoint *Point; TPoint lPoint; TPoint tPoint; TPoint rPoint; TPoint bPoint;
		TPoint ltPoint; TPoint rtPoint; TPoint rbPoint; TPoint lbPoint;
		short n; JRect Rect;
		TFPoint C; double Radius; double MinRadius; double MaxRadius;
		TPoint WPoint;
		void OnClear();
	} *Blob;

public :
	int FMinArea;
	int FMaxArea;
	int FMinHeight;	
	int FMaxHeight;
	int FMinWidth;
	int FMaxWidth;
	int FMinX;
	int FMinY;
	int FHiThHold;
	int FLoThHold;
	int FUseIgnoreBlob;

	int FDarkPixelCount;
	int FBrightPixelCount;

public :
	JTrace();
	~JTrace();

	void OnSortBlob();
	void OnMasking(JRect Rect, int Mask);
	int OnExecute();
	void OnPaintBrightPixel (HWND Handle, HRect Rect, COLORREF FColor, int ThHold);
	void OnPaintDarkPixel   (HWND Handle, HRect Rect, COLORREF FColor, int ThHold);
	int GetDarkPixelCount   (int ThHold=-1);
	int GetBrightPixelCount (int ThHold=-1);
	void OnPaintRectPoint   (TDrawTool Tool);
	void OnPaintJuncPoint   (TDrawTool Tool);
	void OnPaintIRoiByCircle(TDrawTool Tool);
	void OnPaintDRoiByCircle(TDrawTool Tool);

	//OnPaint
	//-------------------------------------------------------------------
	void OnPaintIRoi           (TDrawTool Tool);
	void OnPaintDRoi           (TDrawTool Tool);
	void OnPaintBlobBrightPixel(TDrawTool Tool, int ThHold);
	void OnPaintBlobId         (TDrawTool Tool);
	void OnPaintBlobRect       (TDrawTool Tool);
	void OnPaintBlobMgRect     (TDrawTool Tool);
	void OnPaintBlobWeight     (TDrawTool Tool);
	void OnPaintBlobRect       (TDrawTool Tool, TPoint P1, TPoint P2, TPoint P3, TPoint P4, COLORREF Color=AQUA);
	void OnPaintBlobPixel      (TDrawTool Tool, COLORREF FColor, COLORREF BColor, int LoThHold, int HiThHold);
	void OnPaintBlobCircle     (TDrawTool Tool);
	void OnPaintBlobPixel      (JRect Rect, HWND Handle, bool IsBright, bool IsDark, int ThHold, COLORREF BrightColor=BLUE_COLOR, COLORREF DarkColor=RED_COLOR);
	void OnPaintBlobRectConer  (TDrawTool Tool);
	void OnPaintPoint          (TDrawTool Tool);

	void OnLoad(bool IsLoad, CString Path);
	int GetMeanThHold (JRect Rect);

};

