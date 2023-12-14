// Trace.h: interface for the CTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACE_H__85D3A693_81CC_4363_8CBC_D76B4FBB258B__INCLUDED_)
#define AFX_TRACE_H__85D3A693_81CC_4363_8CBC_D76B4FBB258B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LxHeader.h"
#include "TheLogger.h"
//-----------------------------------------
#define  MAX_DEFECT_COUNT

enum TSort{stLeft=0, stTop, stRight, stBtm};
class  AFX_EXT_CLASS CBlob  : public CFilter{

    public :
	    CBlob();
	    ~CBlob();
	    
// 	    void *TAddress;
	    //TPoint *FPoint;
	    int PixelCount;
	    
	    JRect MgRect;
	    
	    //Trace Result
	    //------------------------	`-------------------------------------------
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
        int FMinX;
        int FMinY;
        int FHiThHold;
        int FLoThHold;
        int OnExecute();
		void OnLoad(bool IsLoad, CString Folder);
};

// Euresys Defect Blob
class AFX_EXT_CLASS CEBlob : public CFilter
{
	public:
		CEBlob();
		~CEBlob();
	protected:
	private:

	public:
		//---------------------------------------------------------------------
		// Property
		//---------------------------------------------------------------------
		// Euresys 
		EImageBW8 BinaryImg;

		// Parameter 
		struct TParameter{
			int m_iThresHold    ;
			int m_iMergeCount   ;
			int m_iRoiOffset    ;
			int m_iDarkThresHold;

			float m_fMinWidth ;
			float m_fMaxWidth ;
			float m_fMinHeight;
			float m_fMaxHeight;

			bool m_bDarkDefect;
			bool m_bBinaryInsp;
			bool m_bMerge     ;

		}Parameter;
		
		int m_iPixelValue ;
		int m_iPixelCount ;
		int m_iBlobCount  ; 
		int m_iDefectCount;

		CString PvPath;

		// Pixel ?? ??? ??? ??? Buffer 
// 		void * IAddress;

		//---------------------------------------------------------------------
		// Result
		//---------------------------------------------------------------------


        //---------------------------------------------------------------------
		// Architecture
		// ---------------------------------------------------------------------
		struct TBlob {double Height; double Width; int Round; TFPoint P; JRect Rect;TFPoint Center;};

		TBlob Blob[MAX_DEFECT_POINT];
		TSort SortMode;

		//---------------------------------------------------------------------
		// Functions
		//---------------------------------------------------------------------
		bool OnExecute()                      ;
		void OnLoad(bool IsLoad, CString Path, int ItemId);
        void OnPaint(TDrawTool Tool,EImageBW8*lpImage, int RColor);
// 		void OnPaint(HWND Handle,EImageBW8*lpImage, int RColor);
		void OnClearData()                    ;
		JRect OnMerge(JRect ar, JRect br)     ;
};

// Euresys Blob(Gray Image?? ?? ? Blob???? ??, ?? ??? ????...?? ????)
class AFX_EXT_CLASS CFBlob : public CFilter
{
public:
	CFBlob(void);
	~CFBlob(void);

	bool OnInspection(HRect Rect, int SortCount);
	void OnPaint(TDrawTool Tool, int Mode=0);

	int Width;
	int Height;
// 	void *IAddress;
// 	void *MAddress;
	//BYTE IAddress;
	//BYTE MAddress;
	HRect Rect;
// 	EImageBW8 Image;
	EImageBW8 EBW8Image1;   // EImageBW8 instance
// 	EROIBW8 EBW8Image1Roi1; // EROIBW8 instance

	struct TParameter {
		int MinWidth ;
		int MinHeight;
		int MaxWidth ;
		int MaxHeight;
		int AutoThHold;
		int ThHold;
		int IsDarkBackGround;
	} Parameter;

	struct TResult {
		int FindBlobCount;
		TPoint WPoint;
		HRect RsRect;
	} Result;
};
// ??? ?? ?? EdgePoint ?? Contour
class AFX_EXT_CLASS CPolygon : public CFilter
{
public: 
	CPolygon(void);
	~CPolygon(void);

	int IWidth, IHeight;
// 	void *IAddress;
// 	void *TAddress;
	CRect IRoi;

	// LogUnit 
	CTheLogger LogUnit;
	
	struct TPoPa {
		int IgInSide       ;
		int IsSeparate     ;
		int IsInToOut      ;
		int MaxBbCnt       ;
		int MinPxCnt       ;
		int MaxPxCnt       ;
		int HiThHold       ;
		int LoThHold       ;
		int FMinWidth      ;
		int FMinHeight     ;
		int FMaxWidth      ;
		int FMaxHeight     ;
		int IsVerSort      ;
		int IsHorSort      ;
		int IsInToLeft     ;
		int LeftFirstPoint ;
		int TopFirstPoint  ;
		int RightFirstPoint;
		int BtmFirstPoint  ;
	} PoPa;

	//Wall Result
	//-------------------------------------------------------------------
	int WallCount;
	struct TWall {
		int StaId, EndId;
		TPoint SPoint, EPoint;

	} Wall[10];

	//Trace Result
	//-------------------------------------------------------------------
	int BlobCount;
	struct TBlob {
		short n, Tag;
		JRect Rect;
		TFPoint *Point;
		int OutCnt;
		int InCnt ;

		// Blob ?? ??? ?? 
		TFPoint LeftPoint  ;
		TFPoint TopPoint   ;
		TFPoint RightPoint ;
		TFPoint BottomPoint;

		TFPoint*OutPoint;  //??? ?? OutEdgePoint
		TFPoint*InPoint ;  //??? ?? InEdgePoint 

		TBlob();
		void OnClear();

		~TBlob() {
			if(OutPoint!=NULL) {delete[] OutPoint; OutPoint=NULL;}
			if(InPoint !=NULL) {delete[] InPoint ; InPoint =NULL;}
			if(Point   !=NULL) {delete[] Point   ; Point   =NULL;}
		}
	} *Blob;

	int OnExecute(TPoint p1, TPoint p2, TPoint p3, TPoint p4);
	int OnExecute(JRect Rect);
	void ClearData();
};


#endif // !defined(AFX_TRACE_H__85D3A693_81CC_4363_8CBC_D76B4FBB258B__INCLUDED_)