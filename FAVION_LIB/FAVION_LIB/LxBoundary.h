#pragma once

#include "LxHeader.h"
#include "LxFitting.h"
#include "math.h"
//---------------------------------------------------------------------------
enum TSMode {sbRightBtm, sbLeftBtm, sbRightTop, sbLeftTop,
	sbBtmRight, sbBtmLeft, sbTopRight, sbTopLeft, sbMatrix};

//---------------------------------------------------------------------------

#define MAX_IGNORE_RECT   500
#define MAX_INSPECT_PIXEL 20000
#define MAX_WEIGHT_PIXEL  40

//----------------------------------------------------------------------------
extern int IpPixelCount;
extern TPoint IpPixel[MAX_INSPECT_PIXEL];

//Image Pixel 추출하는곳이 많음....호출시..Image Pointer 넘겨줌 
class AFX_EXT_CLASS CBoundary : public CFilter
{
    public:
    	CBoundary ();
    	~CBoundary();
    
    	int IWidth, IHeight;
//     	void *IAddress;
//     	void *TAddress;
//     	void *MAddress;
    	JRect IpRect;
    	CCircleFit CircleFit;
    
    public :
    	void OnMaskIpArea(JRect Rect, int Mask);
    	void OnMaskIpArea(TPoint p1, TPoint p2, TPoint p3, TPoint p4, int Mask);
    	void OnPaintMkPixel(TDrawTool Tool, COLORREF FColor = RED_COLOR);
    
    public :
    	int IgBlobCount;
    	int IgBlobMargin;
    	JRect IgBlob[MAX_IGNORE_RECT];
    
    public :
    	//int IpPixelCount;
    	//TFPoint IpPixel[MAX_INSPECT_PIXEL];
    	int OnFindBlobWidthMElement(JRect rect ,bool IsBlobClear=true); //return value is BlobCount
    	int OnFindBlobWidthMElement(TPoint p1, TPoint p2, TPoint p3, TPoint p4, bool IsBlobClear=true);
    
    public :
    	int GetPixelCount    (int BlobId   , bool IsBright, int ThHold, TFPoint C, float r);
    	int GetPixelCount    (JRect *lpRect, bool IsBright, int ThHold, TFPoint C, float r);
    	int GetAvgPixelCount (int BlobId   , bool IsBright, int ThHold, TFPoint C, int r, bool IsValue=false);
    	int GetAvgPixelCount (JRect *lpRect, bool IsBright, int ThHold, TFPoint C, int r, bool IsValue=false);
    	int GetMaskPixelCount(JRect *lpRect);
    
    	enum TTag {tgBurr=500, tgDeform};
    
    	//-------------------------------------------------------------------
    	//Trace Result
    	//-------------------------------------------------------------------
    	int BlobCount;
    	struct TBlob {
    		BYTE *Pixel; TPoint *Point; TPoint lPoint; TPoint tPoint; TPoint rPoint; TPoint bPoint;
    		TPoint ltPoint; TPoint rtPoint; TPoint rbPoint; TPoint lbPoint;
    		short n; JRect Rect; int Quantity2; int Quantity8; int Tag;
    		TFPoint C; double Radius; double MinRadius; double MaxRadius;
    		double Diameter; double MinDiameter; double MaxDiameter;
    		TPoint WPoint;
    
    #define PElement(x, y) (*(Pixel + x + Rect.xLength() * y))
    		//[Functions] Basic
    		TBlob::TBlob();
    		void OnCopy(TBlob *lpBlob);
    		void OnClear();
    		void OnLoad(bool IsLoad, CString Path);
    		//[Functions] OnPaint
    		void OnPaintBlobId  (TDrawTool Tool, COLORREF Color, CString Id="");
    		void OnPaintBlobRect(TDrawTool Tool, COLORREF Color, int lbStyle, int lbHatch);
    	} *Blob;
    
    	int MBlobCount;
    	struct TMBlob : public TBlob {
    		//TMBlob::MTBlob() {}
    	} *MBlob;
    
    public :
    	void OnPaintIgBlob (TDrawTool Tool, int lbStyle=PS_DOT, int lbHatch=HS_CROSS);
    	void OnPaintIgBlobAsCircle(TDrawTool Tool, bool IsFull);
    	//----------------------------------------------------------------------
    	// END : Ignore Area By Rect
    	//----------------------------------------------------------------------
    
    	class TTrPa {
    	    public :
    	    	TTrPa();
    	    	~TTrPa();
    	    	int UseIgnoreBlob   ;
    	    	int IgSort          ;
    	    	int SMode           ;
    	    	int IgInSide        ;
    	    	int MaxBbCnt        ;
    	    	int MinPxCnt        ;
    	    	int MaxPxCnt        ;
    	    	int OffsetY         ;
    	    	int StepY           ;
    	    	int OffsetX         ;
    	    	int StepX           ;
    	    	int MgCount         ;
    	    	float xMgPxCnt      ;
    	    	float yMgPxCnt      ;
    	    	float MgTgHorLength ;
    	    	float MgTgVerLength ;
    	    	int   HiThHold      ;
    	    	int   LoThHold      ;
    	    	double MinVerLength;
    	    	double MinHorLength;
    	    	double MaxVerLength;
    	    	double MaxHorLength;
    	    	int   MinPxBlob   ;
    	    	float MinHorPitch ;
    	    	float MinVerPitch ;
    	    	float IgBlobFarX  ;
    	    	float IgBlobFarY  ;
    	    	int IsQuantity2   ;
    	    	int IsQuantity8   ;
    	    	int OpCircleFit   ;
    	    	int yIgPixelBleed ;
    	    	int xIgPixelBleed ;
    	    	int  OpGrayWeight ;
    	    	bool OpMinMaxRadious;
    	    	int OpLocalThHold;
    	    	int OpfiMax33;
    	    	int xMinLength; 
    	    	int yMinLength;
    	    	int IsFindAnd ;
    	    
    	} TrPa;
    
    	void OnSortBlob();
    	void OnSortBlob(int StartId);
    
    public :
    	int OnExecute(bool IsRotation=false);
    	int OnFindBubble(JRect rect, bool IsBlobClear=true);
    	int OnFindBubble(TPoint p1, TPoint p2, TPoint p3, TPoint p4, bool IsBlobClear);
    	int OnFindBlob(TPoint p1, TPoint p2, TPoint p3, TPoint p4, bool IsBlobClear=true);
    	int OnFindBlob(JRect rect, bool IsBlobClear=true);
    	int OnFindBlobFast(JRect Rect);
    	JRect OnFindRectOne(JRect Rect, int MinPxCnt=5, int MaxPxCnt=1000);
    	void  OnWeightBga();
    
    	double GetMinLength() {
    		return (TrPa.MinVerLength<TrPa.MinHorLength) ? TrPa.MinVerLength:TrPa.MinHorLength;
    	}
    
		
    	//OnPaint
    	//-------------------------------------------------------------------
    	void OnPaint          (TDrawTool Tool);
    	void OnPaintBlobTag   (TDrawTool Tool);
    	void OnPaintBlobId    (TDrawTool Tool);
    	void OnPaintBlobRect  (TDrawTool Tool);
    	void OnPaintBlobWeight(TDrawTool Tool);
    	void OnPaintBlobRect  (TDrawTool Tool, TPoint P1, TPoint P2, TPoint P3, TPoint P4, COLORREF Color=AQUA);
    	void OnPaintBlobPixel (TDrawTool Tool, COLORREF FColor=WHITE, COLORREF BColor=BLACK);
    	void OnPaintBlobCircle(TDrawTool Tool);
    	void OnPaintBlobPixel (JRect Rect, TDrawTool Tool, bool IsBright, bool IsDark, int ThHold, COLORREF BrightColor=BLUE_COLOR, COLORREF DarkColor=RED_COLOR);
    	void OnPaintBlobRectConer(TDrawTool Tool);
    
    	void OnLoad(bool IsLoad, CString Path);
    
    	//-------------------------------------------------------------------
    	//Find ThresHold
    	//-------------------------------------------------------------------
    	struct THisto {
    		int Avg;
    		int Mount;
    		int Valley;
    	};
    
    	THisto OnBlobHistoAnalysis(JRect Rect, bool IsOnlyDefect);
    	THisto OnRectHistoAnalysis(TPoint p1, TPoint p2, TPoint p3, TPoint p4);
    
    protected :
    	int GetMountThHold(JRect Rect);
    	int GetThresHold  (JRect Rect);
    	int GetMeanThHold (JRect Rect);
    
};

class AFX_EXT_CLASS CBlobTable : public CBoundary {

    private :

    protected :

    public :
    	CBlobTable ();
    	~CBlobTable();
    
    	int Width(TBlob* Blob);
    	int Height(TBlob* Blob);
    	int Demension(TBlob* Blob);
    
    	TBlob* lpMinWidthHeightRatio();
    	TBlob* lpBigWidth(TBlob* b1, TBlob* b2);
    	TBlob* lpBigHeight(TBlob* b1, TBlob* b2);
    	TBlob* lpMaxWidthBlob();
    	TBlob* lpMaxHeightBlob();
    	TBlob* lpMaxDemensionBlob();
    	//TBlob* lpMergeBlob();
    
    	int OnDemensionRatio(JRect R1, JRect R2);
    	int OnCompareRatio(JRect R1, JRect R2);
    
    	int GetParentBlob(int x, int y); //Find blob id, If return value is -1, not exist.
    	int GetParentBlob(TPoint Point); //Find blob id, If return value is -1, not exist.
    
    	int GetPxCount(JRect rect, int HiThHold=0, int LoThHold=0);
    	int GetPxAvg  (JRect rect, int HiThHold=0, int LoThHold=0);
    	int GetPxMax  (JRect rect, int HiThHold=0, int LoThHold=0);
    	int GetPxMin  (JRect rect, int HiThHold=0, int LoThHold=0);
    
    	//[Functions] Compare
    	bool OnDemension(int Min, int Max                    ); int GetMaxDemension();
    
    	bool OnQuantity2(int Min, int Max                    ); int GetMaxQuantity2();
    	bool OnQuantity8(int Min, int Max                    ); int GetMaxQuantity8();
    	bool OnBlobCount(int Min, int Max                    ); int GetMaxBlobCount();
    	bool OnHorAlign (int Min, int Max, bool IsSingle=true); int GetMaxHorAlign ();
    	bool OnVerAlign (int Min, int Max, bool IsSingle=true); int GetMaxVerAlign ();
    	bool OnHorPitch (int Min, int Max                    ); int GetMaxHorPitch ();
    	bool OnVerPitch (int Min, int Max                    ); int GetMaxVerPitch ();
    	bool OnHorLength(int Min, int Max                    ); int GetMaxHorLength();
    	bool OnVerLength(int Min, int Max                    ); int GetMaxVerLength();
    	bool OnBorderCnt(int Min, int Max                    ); int GetMaxBorderCnt();
    	bool OnAngle    (int Min, int Max                    ); int GetMaxAngle    ();
    	bool OnHorPos   (int Min, int Max, JRect IpRt        ); int GetMaxHorPos   ();
    	bool OnVerPos   (int Min, int Max, JRect IpRt        ); int GetMaxVerPos   ();
    
    	//[Functions] Match Score
    	int GetMatchPxScore (TBlob *CuBlob, TBlob *TrBlob, int OutLineArea, bool IsCorner);
    	int GetMatchQtyScore(TBlob *CuBlob, TBlob *TrBlob, int OutLineArea, bool IsCorner);
    
    	JRect OnMerge();
    	void OnLoad(bool IsLoad, CString Path);
};
