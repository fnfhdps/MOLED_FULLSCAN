// LxLine.h: interface for the CSingleLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXLINE_H__2B186375_7F4F_41A8_9CC1_6B99A12AA2A1__INCLUDED_)
#define AFX_LXLINE_H__2B186375_7F4F_41A8_9CC1_6B99A12AA2A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LxEdge.h"
#include "LxHeader.h"

enum TWall       { waLeftWall, waTopWall, waRightWall, waBottomWall, waLeftTop, waRightTop, waRightBottom, waLeftBottom, waBottomLeft, waBottomCenter, waBottomRight };
enum TOrientaion { orWest, orNorth, orEast, orSouth };


// class AFX_EXT_CLASS  CSingleLine : public CEdge {
// ???? ??? ?? ?? ??? ??? LineFitting ? ?? ?? 
class AFX_EXT_CLASS  CSingleLine : public CFilter{

    private :

    protected :
        // Functions
        // ---------------------------------------------------------------------
        double  __fastcall GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);
        double __fastcall OnLineDistance(double x1, double y1, double x2, double y2, double x, double y);
//         bool   __fastcall OnLineFitRANSAC_2(bool IsHor, TFPoint *Q1, int Count, double Deviation, int max_iter, int MinCount);
        int OnFindExecute(bool IsFindVerticalLne, bool IsFwdDirection);

        // Architecture
        // ---------------------------------------------------------------------

        // Property
        // ---------------------------------------------------------------------

        // Result
        // ---------------------------------------------------------------------
        double FAngle;
        TFPoint FP0;
        TFPoint FP1;
        TFPoint FCP;

    public :
        // Creat & Destory
        // ---------------------------------------------------------------------
        CSingleLine();
         ~CSingleLine();

		 HMath Math;
		 CTimer Time;

        // Functions - Set
        // ---------------------------------------------------------------------
		void SetOrientation	 (int Orientation); 
        void SetFindLineId	 (int FindLineId );
        void SetIncrement	 (int Increment  );
		void SetEdgeType	 (int EdgeType   );
// 		void SetMinEdgeCount (int MinEdgeCount);


        // Functions - Get
        // ---------------------------------------------------------------------
        int GtOrientation()   { return FOrientation; }
        int GetFindLineId()   { return FFindLineId ; }
        int GetIncrement ()   { return FIncrement  ; }
// 		int GetMinEdgeCount() { return FMinEdgeCount; }

        // Functions - Execute
        // ---------------------------------------------------------------------
        int OnExecute();

        // Functions - Result
        // ---------------------------------------------------------------------
        double GetResultAngle() {return FAngle; }
        TFPoint GetResultP0() ;
        TFPoint GetResultP1() ;
        TFPoint GetResultCP() ; 

        // Functions - Paint
        // ---------------------------------------------------------------------
        void OnPaint             (TDrawTool Tool);
        void OnPaintLine         (TDrawTool Tool);
        void OnPaintPoint        (TDrawTool Tool);
        void OnPaintText         (TDrawTool Tool);
        void OnPaintRoi          (TDrawTool Tool);
		void OnPaintLineWidthIRoi(TDrawTool Tool);
		void OnPaintEdgePoint    (TDrawTool Tool);

		bool   __fastcall OnLineFitRANSAC_2(bool IsHor, TFPoint *Q1, int Count, double Deviation, int max_iter, int MinCount);

        // Functions - Load & Save
        // ---------------------------------------------------------------------
        //void OnLoad(bool IsLoad, AnsiString Folder);
        void OnLoad(bool IsLoad, CString Folder);

        //Value
		int FThickness   ;
		int FSmoothing   ;
		int FThreshold   ;
		int FMinAmplitude;
		int FMinArea     ;
		int FEdgeType    ;

		int FOrientation  ;
		int FFindLineId   ;
		int   FIncrement  ;
		double RansacScore;

		int FEdgePointCount;
        TFPoint *FEdgePoint;
};

//------------------------------------------------------------------------------
// ??? ?? Count?? EdgeSeach ? LineFittign ?? ??(Count?? ??? ??)
// MultiLine EdgeSearch ? LineFitting ?? 
class AFX_EXT_CLASS CLine : public CFilter {

public:
	CLine(void);
	~CLine(void);

	// Architecture
public :
	TFPoint P0;
	TFPoint P1;
	float  __fastcall GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);
	double __fastcall OnLineDistance(double x1, double y1, double x2, double y2, double x, double y);
	bool   __fastcall OnLineFitRANSAC_2(bool IsHor, TFPoint *Q1, int Count, double Deviation, int max_iter, int MinCount);

public :
#define MAX_LINE_COUNT 10
#define MAX_EDGE_COUNT 1000

	// 	EWorldShape EWorldShape1; // EWorldShape instance
	// 	EImageBW8 *Image; // BW8 Image to work on
	EROIBW8  HRoi   ;
	EROIBW8  VRoi   ;
	EPointGauge EPointGauge1; // EPointGauge instance
	EImageBW8 BinaryImg;

	TFPoint HFindEdge[MAX_LINE_COUNT][MAX_EDGE_COUNT];
	int HFindEdgeCount[MAX_LINE_COUNT];

	TFPoint VFindEdge[MAX_LINE_COUNT][MAX_EDGE_COUNT];
	int VFindEdgeCount[MAX_LINE_COUNT];

	int HFEdgeCount   ;
	int VFEdgeCount   ;
	int HFindLineCount;
	int VFindLineCount;

	// 	HMath Math;

	struct TFindLine {
		TFPoint P1;
		TFPoint P2;
	} HFindLine[MAX_LINE_COUNT], VFindLine[MAX_LINE_COUNT];

	bool DwIpRoiCrossLine;
	bool DwHoRoi;
	bool DwVtRoi;
	bool DwAllLine;
	bool DwPoint;
	bool DwLine;
	bool DwIntersect;
	bool DwRectangle;
	bool DwText;
	bool DwEdgePoint;
	bool IsInOut    ;

	CString PvPath;
	int Width;
	int Height;
// 	void *IAddress;
// 	void *MAddress;
	HRect HoRect;
	HRect VtRect;

	// Paint Function
	void OnPaint(TDrawTool Tool,TFPoint CP, COLORREF Color, EImageBW8 *lpImage);
	void OnPaintRectangle(TDrawTool Tool, EImageBW8*lpImage);

	int OnExecute();
	int OnFindExecute   (bool IsHorEdge, bool FwdDirection, int LineId);
	int OnFindExecuteAll(bool IsHorEdge, bool FwdDirection, int LineId);

	struct TParameter {
		bool OpFindTopLine   ;
		bool OpFindRightLine ;
		bool OpFindBottomLine;
		bool OpFindLeftLine  ;
		int  OpBinary        ;
		int  ThHold          ;
		int  MinEdgeCount    ;
		int  Angle           ;
		int  MaxEdges        ;
		int  EdgeType        ;
		int  Increment       ;

		int MinAmplitude;
		int MinArea     ;
		int Threshold   ;
		int Thickness   ;
		int Smoothing   ;

		int JDThreshold ;
		int JDThickness ;
		int JDSmoothing ;

		float FitDistance;
		int LeftLineId  ;
		int TopLineId   ;
		int RightLineId ;
		int BottomLineId;
	} Parameter;

	struct TResult {
		bool IsFindLeftLine;
		bool IsFindTopLine;
		bool IsFindRightLine;
		bool IsFindBottomLine;
		double Angle;
		double LAG;
		double TAG;
		double RAG;
		double BAG;

		TFPoint CP ;
		TFPoint LT ;
		TFPoint TR ;
		TFPoint RB ;
		TFPoint LB ;
		TFPoint LP1;
		TFPoint LP2;
		TFPoint TP1;
		TFPoint TP2;
		TFPoint RP1;
		TFPoint RP2;
		TFPoint BP1;
		TFPoint BP2;
	} Result;

	void OnLoad(bool IsLoad, CString Folder,int ItemId);
};
//--------------------------------------------------------------------------
//ROI???? SingleLine Algorithm ?? ? LT,RT,RB,LB ??? ??? ???  
class CRectLine : public CFilter {
private :
protected :
	// Architecture
	// ---------------------------------------------------------------------


public :
	// Result
	// ---------------------------------------------------------------------
	TFPoint FCPoint; // Center Position
	TFPoint FIPoint[4]; // 0 : LT, 1 : RT, 2 : RB, 3 : LB
	struct TLine { TFPoint FP0; TFPoint FP1; } Line[4];

	// Property
	// ---------------------------------------------------------------------
	bool DwIRoi;
	bool DwSingleLine;
	bool DwTwoPoint;
	bool DwFindRect;

	int FRoiRange[4];
	JRect LRect, TRect, RRect, BRect; //????

	// Architecture
	// ---------------------------------------------------------------------
	CSingleLine *SLLine;
	CSingleLine *STLine;
	CSingleLine *SRLine;
	CSingleLine *SBLine;

	// Creat & Destory
	// ---------------------------------------------------------------------
	CRectLine();
	~CRectLine();

	// Functions - Set
	// ---------------------------------------------------------------------

	// Functions - Get
	// ---------------------------------------------------------------------

	// Functions - Execute
	// ---------------------------------------------------------------------
	int OnExecute();

	// Functions - Result
	// ---------------------------------------------------------------------
	TFPoint GetResultIntersectPoint(int n) { return FIPoint[n]; }
	TFPoint GetResultLinePointP0(int n) { return Line[n].FP0; }
	TFPoint GetResultLinePointP1(int n) { return Line[n].FP1; }
	TFPoint GetResultCenterPoint() { return FCPoint; }
	float   GetResultAngle() { return Math.GetAngle(FIPoint[0].x, FIPoint[0].y, FIPoint[1].x, FIPoint[1].y); }

	// Functions - Paint
	// ---------------------------------------------------------------------
	void OnPaint(TDrawTool Tool);

	// Functions - Load & Save
	// ---------------------------------------------------------------------
	void OnLoad(bool IsLoad, CString Folder);

	float GetLength(TFPoint p1, TFPoint p2);
};
//------------------------------------------------------------------------------

// ??? ?? Shape?? EdgeSearch ? LineFitting ?? ??
class AFX_EXT_CLASS CPolygoneLine : public CSingleLine {
    #define MAX_POLYGONE_LINE 10

    private :

    protected :
        // Architecture
        // ---------------------------------------------------------------------

        // Property
        // ---------------------------------------------------------------------
        int FMaxLines;

        // Result
        // ---------------------------------------------------------------------
        TFPoint FIPoint[MAX_POLYGONE_LINE];
        struct TLine { TFPoint FP0; TFPoint FP1; } Line[MAX_POLYGONE_LINE];

    public :
        // Architecture
        // ---------------------------------------------------------------------
        EROIBW8 *ARoi[MAX_POLYGONE_LINE];

        // Creat & Destory
        // ---------------------------------------------------------------------
        CPolygoneLine();
        ~CPolygoneLine();

		HMath Math;

        // Functions - Set
        // ---------------------------------------------------------------------
        void SetMaxLines(int MaxLines) { FMaxLines = MaxLines; }

        // Functions - Get
        // ---------------------------------------------------------------------
        int GetMaxLines() { return FMaxLines; }

        // Functions - Execute
        // ---------------------------------------------------------------------
        int OnExecute();

        // Functions - Result
        // ---------------------------------------------------------------------
        TFPoint GetResultIntersectPoint(int n) { return FIPoint[n]; }
        TFPoint GetResultLinePointP0(int n) { return Line[n].FP0; }
        TFPoint GetResultLinePointP1(int n) { return Line[n].FP1; }

        // Functions - Paint
        // ---------------------------------------------------------------------
        //void OnPaint(HWND Handle, TColor RColor=clRed);
        void OnPaint(TDrawTool Tool);

        // Functions - Load & Save
        // ---------------------------------------------------------------------
        //void OnLoad(bool IsLoad, AnsiString Folder);
        void OnLoad(bool IsLoad, CString Folder);

        float GetLength(TFPoint p1, TFPoint p2);
};
#endif // !defined(AFX_LXLINE_H__2B186375_7F4F_41A8_9CC1_6B99A12AA2A1__INCLUDED_)
