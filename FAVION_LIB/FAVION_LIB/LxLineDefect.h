	// LxLineDefect.h: interface for the CLineDefect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXLINEDEFECT_H__02D37965_8899_4373_830E_A5A6C9E513F1__INCLUDED_)
#define AFX_LXLINEDEFECT_H__02D37965_8899_4373_830E_A5A6C9E513F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LxLine.h"
#include "LxHeader.h"
#include "LxBlob.h"
#include "LxFind.h"
#include "TheLogger.h"
#include "LxDoubleLine.h"
#include "LxMatch.h"
#include "LxFitting.h"
#include "LxCircle.h"
// #include "Open_eVision_1_2.h"

// using namespace Euresys::Open_eVision_1_2;
class AFX_EXT_CLASS CLineDefect : public CSingleLine {

    private :
        // Architecture
        // ---------------------------------------------------------------------

    protected :
        // Property
        //----------------------------------------------------------------------
        float FEscapeDeviation;

        // Result
        //----------------------------------------------------------------------
        int FDefectPointCount;
        TFPoint *FDefectPoint;

    public :
        // Creat & Destory
        // ---------------------------------------------------------------------
        CLineDefect();
        ~CLineDefect();

        // Functions - Set
        // ---------------------------------------------------------------------
        void SetRoi(TPoint FP, TPoint SP);
        void SetEscapeDeviation(float EscapeDeviation) { FEscapeDeviation = EscapeDeviation; }

        // Functions - Get
        // ---------------------------------------------------------------------
        float GetEscapeDeviation() {return FEscapeDeviation; }

        // Functions - Execute
        // ---------------------------------------------------------------------
        int OnExecute();

        // Functions - Result
        // ---------------------------------------------------------------------
        int GetResultDefectPointCount() {return FDefectPointCount; }
        TFPoint GetResultDefectPoint(int n) {return FDefectPoint[n]; }

        // Functions - Paint
        // ---------------------------------------------------------------------
        void OnPaint(TDrawTool Tool            );
        void OnPaintEdgePoint(TDrawTool Tool   );
        void OnPaintEscapePoint(TDrawTool Tool );

        // Functions Load & Save
        // ---------------------------------------------------------------------
        void OnLoad(bool IsLoad, CString Folder);

};

//------------------------------------------------------------------------------

class AFX_EXT_CLASS  CPolygoneDefect : public CPolygoneLine {

    private :

    protected :
        // Architecture
        // ---------------------------------------------------------------------
        CRectEdge *RectEdge;

        // Property
        // ---------------------------------------------------------------------
        bool FLeftWall   ;
        bool FTopWall    ;
        bool FRightWall  ;
        bool FBottomWall ;
        int  FEscapeDeviation;

        // Result
        // ---------------------------------------------------------------------
        int FDefectPointCount;
        TFPoint *FDefectPoint;

    public :
        // Creat & Destory
        // ---------------------------------------------------------------------
        CPolygoneDefect();
        ~CPolygoneDefect();

        // Functions - Set
        // ---------------------------------------------------------------------
        void SetLeftWall  (bool LeftWall  ) { FLeftWall   = LeftWall  ;}
        void SetTopWall   (bool TopWall   ) { FTopWall    = TopWall   ;}
        void SetRightWall (bool RightWall ) { FRightWall  = RightWall ;}
        void SetBottomWall(bool BottomWall) { FBottomWall = BottomWall;}
        void SetEscapeDeviation(int EscapeDeviation) { FEscapeDeviation = EscapeDeviation; }

        // Functions - Get
        // ---------------------------------------------------------------------
        bool GetLeftWall  () { return FLeftWall  ;}
        bool GetTopWall   () { return FTopWall   ;}
        bool GetRightWall () { return FRightWall ;}
        bool GetBottomWall() { return FBottomWall;}
        int  GetEscapeDeviation() { return FEscapeDeviation; }
        float  GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);

        // Functions - Execute
        // ---------------------------------------------------------------------
        int OnExecute();
        int OnExecute(bool LeftWall, bool TopWall, bool RightWall, bool BottomWall);

        // Functions - Result
        // ---------------------------------------------------------------------
        int GetResultDefectPointCount() {return FDefectPointCount; }
        TFPoint GetResultDefectPoint(int n) {return FDefectPoint[n]; }

        // Functions - Paint
        // ---------------------------------------------------------------------
        void OnPaint(TDrawTool Tool);

        // Functions - Load & Save
        // ---------------------------------------------------------------------
        void OnLoad(bool IsLoad, CString Folder);
};

//------------------------------------------------------------------------------

class AFX_EXT_CLASS  CMultiLineDefect : public CFilter {

private :
	// Result - For Paint
	// 내부 수학 계산용으로 사용 되는 변수 이며, Paint를 위해 사용되기도 함.
	// ---------------------------------------------------------------------
	TFPoint InLTPoint;
	TFPoint InRTPoint;
	TFPoint InRBPoint;
	TFPoint InLBPoint;

	TFPoint PLW0; TFPoint InLTLPoint;
	TFPoint PLW1; TFPoint InLTTPoint;
	TFPoint PTW0; TFPoint InRTRPoint;
	TFPoint PTW1; TFPoint InRTTPoint;
	TFPoint PRW0; TFPoint InRBRPoint;
	TFPoint PRW1; TFPoint InRBBPoint;
	TFPoint PBW0; TFPoint InLBLPoint;
	TFPoint PBW1; TFPoint InLBBPoint;

protected :
	/////////////////////////////////////////////////////////////////////////
	// CELL INSPECTION 
	/////////////////////////////////////////////////////////////////////////
	// Result
	// ---------------------------------------------------------------------
	int DefectPointCount; // 전체 불량 포인트 개수
	int ACount          ; // A_SIDE  불량 포인트 개수
	int BCount          ; // B_SIDE  불량 포인트 개수
	int CCount          ; // C_SIDE  불량 포인트 개수
	int DCount          ; // D_SIDE  불량 포인트 개수
	int C1Count         ; // C1_SIDE 불량 포인트 개수
	int C2Count         ; // C2_SIDE 불량 포인트 개수
	int C3Count         ; // C3_SIDE 불량 포인트 개수
	int C4Count         ; // C4_SIDE 불량 포인트 개수
	int ABurrCount      ; 
	int BBurrCount      ;
	int CBurrCount      ;
	int DBurrCount      ;
	int ACrackCount     ;
	int BCrackCount     ;
	int CCrackCount     ;
	int DCrackCount     ;
	// White Defect 

	// Dark Defect
	int A1CirDeftectCnt ;
	int A2CirDeftectCnt ;
	int C1CirDeftectCnt ;
	int C2CirDeftectCnt ;
	int C3CirDeftectCnt ;
	int C4CirDeftectCnt ;
	int C5CirDeftectCnt ;
	int C6CirDeftectCnt ;
	int Btm3CirDefectCnt;

	// White Defect 
	int A1CirCrackCnt  ;
	int A2CirCrackCnt  ;
	int C1CirCrackCnt  ;
	int C2CirCrackCnt  ;
	int C3CirCrackCnt  ;
	int C4CirCrackCnt  ;
	int C5CirCrackCnt  ;
	int C6CirCrackCnt  ;
	int Btm3CirCrackCnt;


	// MultiThresHold 
	int EThresHold       ;
	int ECThresHold      ;
	int LtpsThresHold    ;
	int EnCapTopThresHold;
	int EnCapMidThresHold;
	int EnCapBtmThresHold;

	// Corner 연마검사 여부 확인 
	bool IsCornerTop;
	bool IsCornerBtm;

	// Corner 연마검사 여부 확인 
	bool IsRectTop  ;
	bool IsRectBtm  ;

	// Cell Circle Grind 검사 유무 결정
	bool IsCircleTop;
	bool IsCircleBtm;

	// Narrow Trench 선택 
	bool IsNarrowTrench;

	// Inspection Blob Spec 
	float FMinWidth   ;
	float FMaxWidth   ;
	float FMinHeight  ;
	float FMaxHeight  ;

	// Defect Line Position 
	float LinePosition ;
	float LeftLineData ;
	float RightLineData;

	// Inspection ROI 
	int FLeftWidth     ;
	int FRightWidth    ;
	int FTopHeight     ;
	int FBtmHeight     ;

	// Architecture
	// ---------------------------------------------------------------------
	CFind        *Find1     ;
	CFind        *Find2     ;
	CSingleLine  *SingleLine;
	CRectEdge    *RectEdge  ;
	CEdge        *Edge      ;
	CCircle      *Circle    ;
// 	CMatch       *Match     ;
	CFind        *Find      ;
	CCircleFit   *CircleFit ;
	CPolygon     *Polygon   ;
// 	ClipperOffset*Cliper    ;

	
// 	JTrace      *Trace     ;

	/////////////////////////////////////////////////////////////////////////
	// EDGE INSPECTION 
	////////////////////////////////////////////////////////////////////////
	int EBMLinThresHold  ;
	int BurrThresHold    ;
	int BrokenThresHold  ;
	int CrackThresHold   ;
	int GrindThresHold   ;
	int BurrCount        ;
	int BrokenCount      ;
	int CrackCount       ;
	int BurrOffset       ;
	int BurrROIWidth     ; 
	double BmToMarkDis   ;

	// Inspection Broken Spec
	float FBrokenMinWidth ;
	float FBrokenMaxWidth ;
	float FBrokenMinHeight;
	float FBrokenMaxHeight;

	// Inspection Burr Spec
	float FBurrMinWidth ;
	float FBurrMaxWidth ;
	float FBurrMinHeight;
	float FBurrMaxHeight;

	// Inspection Chipping Spec
	float FChippingMinWidth ;
	float FChippingMaxWidth ;
	float FChippingMinHeight;
	float FChippingMaxHeight;

	// Top,Btm Ignore roi
	int IgnoreOffset; 
	int GrindOutIn  ; 
	int GrindCheck  ; 

	// Inspect Roi Offst 
	int InspOffset;

	// Image Rotate Degree
	float ImgDegree;
	// Display 목적 회전 중심 좌표 
	TFPoint CenterPoint;
	//	임시로 public 선언함.
public:

	/////////////////////////////////////////////////////////////////////
	// 공통 Data 
	/////////////////////////////////////////////////////////////////////
	// Defect Blob 
	struct TBlob   {double Height; double Width; int Round; TFPoint P; CRect Rect; double Hor_Dis; double Ver_Dis; int LineAttach;};
	struct TCorner {float Dv; TFPoint P;};
	struct TCircle {float Dv; float Dis; TFPoint P;};
	struct TSpec   {double DvX; double DvY;};
	// Polygon Defect 
	struct TBadRect{CRect Rect; TFPoint SPoint; TFPoint EPoint; int BadPixelCount; TFPoint Pixel[MAX_POLYGON_DEFECT_POINT];};
	// GrindMark 
	struct TGrindMark{CRect Rect; TFPoint Point; TFPoint RectPoint; double Gradient; double Score; double degree; int DvX; int DvY;double GrindWidth; double GrindMea;};
	struct TPolygonROI{int StaX; int StaY; int Width; int Height;};

	// Pixel 내부 데이터 취합시 필요한 Buffer 
// 	void * IAddress;

	// 외곽기준 vs Size기준 선택 
	bool InspectMode;    // 0: 외곽기준  1:Size 기준(BMLine 기준)
    // TrakLine 생성 방법 
	// 실측 Edge검출 외곽라인 데이터 
	TFPoint FLTPoint; //좌측라인과 상단라인의 교차점 좌표
	TFPoint FRTPoint; //우측라인과 상단라인의 교차점 좌표
	TFPoint FRBPoint; //우측라인과 하단라인의 교차점 좌표
	TFPoint FLBPoint; //좌측라인과 하단라인의 교차점 좌표

	// Notch Cell 내부 TrackLine EdgePoint 기준 교차점 좌표 
	TFPoint FTRLBPoint; //좌측트랙라인과 하단트랙라인의 교차점 좌표
	TFPoint FTRRBPoint; //우측트랙라인과 하단트랙라인의 교차점 좌표

	TFPoint FLTLPoint; //좌측라인과   첫번째 코너 교차점 좌표 
	TFPoint FLTTPoint; //위쪽라인과   첫번째 코너 교차점 좌표 
	TFPoint FRTRPoint; //우측라인과   두번째 코너 교차점 좌표 
	TFPoint FRTTPoint; //위쪽라인과   두번째 코너 교차점 좌표 
	TFPoint FRBRPoint; //우측라인과   세번째 코너 교차점 좌표 
	TFPoint FRBBPoint; //아래쪽라인과 세번째 코너 교차점 좌표 
	TFPoint FLBLPoint; //좌측라인과   네번째 코너 교차점 좌표 
	TFPoint FLBBPoint; //아래쪽라인과 네번째 코너 교차점 좌표 

	// NOTCH 검사시 필요한 Corner 연마 안쪽라인 교차점 좌표
	TFPoint FLTLInPoint;
	TFPoint FLTTInPoint;
	TFPoint FRTRInPoint;
	TFPoint FRTTInPoint;
	TFPoint FLBLInPoint;
	TFPoint FLBBInPoint;
	TFPoint FRBRInPoint;
	TFPoint FRBBInPoint;

	// NOTCH 검사시 필요한 Corner 연마량 기준 라인 교차점 
	TFPoint FLTLMeaPoint;
	TFPoint FLTTMeaPoint;
	TFPoint FRTRMeaPoint;
	TFPoint FRTTMeaPoint;
	TFPoint FLBLMeaPoint;
	TFPoint FLBBMeaPoint;
	TFPoint FRBRMeaPoint;
	TFPoint FRBBMeaPoint;

	// AlingKey 기준 사용자가 설정한 외곽라인 데이터  
	TFPoint LTLPoint; 
	TFPoint LTTPoint; 
	TFPoint RTRPoint; 
	TFPoint LBBPoint; 

	TFPoint LBLPoint; 
	TFPoint RTTPoint;
	TFPoint RBRPoint;
	TFPoint RBBPoint;
	
	//각도 계산시 필요한 기준 Mark좌표 
	TFPoint MarkPoint1;
	TFPoint MarkPoint2;
	TFPoint AlignMark1;
	TFPoint AlignMark2;

	// ImageHeight
	int ImageHeight;
	int ImageWidth ;

	// ImageHeightEnd
	int ImageHEnd  ;

	// Mark Rotate Degree 
	float MarkDegree;

	// ThresHold 분류 (EnCapTop과 EnCapMid분류 위해서 필요함)
	bool IsEnCapTop ;

	// Mark 기준으로 영역 재설정 및 Corner Size 검사 
	int CELL_A;
	int CELL_B;
	int CELL_C;
	int CELL_D;
	int CELL_E;
	int CELL_F;
	int CELL_G;
	int CELL_H;
	
	double MarkLTDx;
	double MarkLTDy;
	double MarkRTDx;
	double MarkRTDy;
	double MarkRBDx;
	double MarkRBDy;
	double MarkLBDx;
	double MarkLBDy;

	// Match 파일 경로 
	CString FindDir ;
	CString MatchDir;
	
	//-------------------------------------------------------------------
	//////////////////////////////////////////////////////////////////////
	// Defect Data 
	/////////////////////////////////////////////////////////////////////
	// Side Defect(Dark)
	TBlob ADefectPoint    [MAX_DEFECT_POINT];
	TBlob BDefectPoint    [MAX_DEFECT_POINT];
	TBlob CDefectPoint    [MAX_DEFECT_POINT];
	TBlob DDefectPoint    [MAX_DEFECT_POINT];

	// Side Defect(Bright)
	TBlob ACrackPoint    [MAX_DEFECT_POINT];
	TBlob BCrackPoint    [MAX_DEFECT_POINT];
	TBlob CCrackPoint    [MAX_DEFECT_POINT];
	TBlob DCrackPoint    [MAX_DEFECT_POINT];

	// Corner Defect 
	TBlob C1DefectPoint   [MAX_DEFECT_POINT];
	TBlob C2DefectPoint   [MAX_DEFECT_POINT];
	TBlob C3DefectPoint   [MAX_DEFECT_POINT];
	TBlob C4DefectPoint   [MAX_DEFECT_POINT];

	// Dark Circle Defect 
	// Circle Defect 
	TBlob A1CirDefectPoint  [MAX_DEFECT_POINT];
	TBlob A2CirDefectPoint  [MAX_DEFECT_POINT];
	TBlob C1CirDefectPoint  [MAX_DEFECT_POINT];
	TBlob C2CirDefectPoint  [MAX_DEFECT_POINT];
	TBlob C3CirDefectPoint  [MAX_DEFECT_POINT];
	TBlob C4CirDefectPoint  [MAX_DEFECT_POINT];
	TBlob C5CirDefectPoint  [MAX_DEFECT_POINT];
	TBlob C6CirDefectPoint  [MAX_DEFECT_POINT];
	TBlob Btm3CirDefectPoint[MAX_DEFECT_POINT];

	// Bright Circle Defect 
	// Circle Defect 
	TBlob A1CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob A2CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob C1CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob C2CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob C3CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob C4CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob C5CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob C6CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob Btm3CirCrackPoint[MAX_DEFECT_POINT];

	// Polygon Defect Data 
	TBadRect BadRect[MAX_POLYGON_DEFECT_COUNT];

	//-------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////
	// CELL INSPECTION (외관검사기)
	/////////////////////////////////////////////////////////////////////
	struct TLine        { TFPoint FP0; TFPoint FP1; } Line       [20];
	struct TLeftBMLine  { TFPoint FP0; TFPoint FP1; } LeftBMLine [1 ];
	struct TRightBmLine { TFPoint FP0; TFPoint FP1; } RightBMLine[1 ];
	struct TTopBMLine   { TFPoint FP0; TFPoint FP1; } TopBMLine  [1 ];
	struct TBtmBmLine   { TFPoint FP0; TFPoint FP1; } BtmBMLine  [1 ];
	struct TMarkLine    { TFPoint FP0; TFPoint FP1; } MarkLine   [4 ]; 
	struct TPreLine     { TFPoint FP0; TFPoint FP1; } PreLine    [4 ];

	// Corner Edge Point Data(Normal)
	TFPoint *C1Point;
	TFPoint *C2Point;
	TFPoint *C3Point;
	TFPoint *C4Point;

	// Corner Edge Point Data(Corner 연마검사 Data) NOTCH 
	TFPoint *C1InPoint;
	TFPoint *C2InPoint;
	TFPoint *C3InPoint;
	TFPoint *C4InPoint;

	// Corner Edge Point Data(Corner 연마검사 Data) NOTCH 
	TFPoint *C1MeaPoint;
	TFPoint *C2MeaPoint;
	TFPoint *C3MeaPoint;
	TFPoint *C4MeaPoint;

	// Corner Edge Point Count
	int C1EdgeCount;
	int C2EdgeCount;
	int C3EdgeCount;
	int C4EdgeCount;

	// Corner InEdge Point Count
	int C1InEdgeCount;
	int C2InEdgeCount;
	int C3InEdgeCount;
	int C4InEdgeCount;

	// Corner Grind Measure StandCount 
	int C1MeaEdgeCount;
	int C2MeaEdgeCount;
	int C3MeaEdgeCount;
	int C4MeaEdgeCount;

	// Clipper Algotithm Test
	int ClipperMode;
	int ClipperRange;

	TFPoint P1;
	TFPoint P2;

	// Compare Corner Data 
	TCorner C1Corner[MAX_CORNER_POINT];
	TCorner C2Corner[MAX_CORNER_POINT];
	TCorner C3Corner[MAX_CORNER_POINT];
	TCorner C4Corner[MAX_CORNER_POINT];

	// 광원 Parameter 
	// Corner Dx, Dy Mark기준으로 산출하기 위해서 필요한 좌표 
	TFPoint MarkFLTLPoint ; //좌측라인과   첫번째 코너 교차점 좌표 
	TFPoint MarkFLTTPoint ; //위쪽라인과   첫번째 코너 교차점 좌표 
	TFPoint MarkFRTRPoint ; //우측라인과   두번째 코너 교차점 좌표 
	TFPoint MarkFRTTPoint ; //위쪽라인과   두번째 코너 교차점 좌표 
	TFPoint MarkFRBRPoint ; //좌측라인과   첫번째 코너 교차점 좌표 
	TFPoint MarkFRBBPoint ; //위쪽라인과   첫번째 코너 교차점 좌표 
	TFPoint MarkFLBLPoint ; //우측라인과   두번째 코너 교차점 좌표 
	TFPoint MarkFLBBPoint ; //위쪽라인과   두번째 코너 교차점 좌표 
	TFPoint MarkCrossPoint; //Corner 교차점 좌표 
	//---------------------------------------------

	// Remain 검사시 필요한 Parameter 
	// BMLine to EdgeLine Offset 
	int LeftOffset          ;
	int RightOffset         ;
	int TopOffset           ;
	int BtmOffset           ;

	// Remain(Burr) Defect Offset 
	int BurrLeftOffset      ;
	int BurrRightOffset     ;
	int BurrTopOffset       ;
	int BurrBtmOffset       ;

	// BMLine Find ThresHold 
	int LeftBmLineThresHold ;
	int RightBmLineThresHold;
	int BtmBmLineThresHold  ;
	int BurrFindThresHold   ;
	int BtmLimitLine        ;
	int TopLimitLine        ;

	// EdgeLine Defect
	// OverKill 감소를 위해서 분리함 
	int LeftEdgeThresHold   ;
	int TopEdgeThresHold    ;
	int RightEdgeThresHold  ;
	int BtmEdgeThresHold    ;
	int CirGrindMeaThresHold;
	int CorGrindMeaThresHold;
	int CirGrindInThresHold ;
	int CorGrindInThresHold ;

	// BM Line 2개중 한곳을 못찾을 경우 데이터 변경함 
	int BMLineDistance      ;

	TFPoint FDefectPoint [MAX_DEFECT_POINT]; // 육각형 포인트 퐈표 리스트 
	TFPoint FCellPoint   [MAX_DEFECT_POINT]; // IsMarkLine CellSize 계산시 필요한 좌표값 

	//Burr Data 
	TBlob ABurrPoint   [MAX_DEFECT_POINT];
	TBlob BBurrPoint   [MAX_DEFECT_POINT];
	TBlob CBurrPoint   [MAX_DEFECT_POINT];
	TBlob DBurrPoint   [MAX_DEFECT_POINT];

	// Corner Dx, Dy 데이터 
	double LTDx; 
	double LTDy; 
	double RTDx;
	double RTDy;
	double RBDx;
	double RBDy;
	double LBDx;
	double LBDy;

	// 외곽라인에서 AlignMark까지 거리  
	double LDistance; 
	double RDistance; 
	double TDistance;
	double BDistance;
	//-------------------------------------------------------------------


	/////////////////////////////////////////////////////////////////////
	//NOTCH CELL INSPECTION 
	/////////////////////////////////////////////////////////////////////
	struct TLeftOutLine   { TFPoint FP0; TFPoint FP1; } LeftOutLine   [1]            ;
	struct TLeftInLine    { TFPoint FP0; TFPoint FP1; } LeftInLine    [1]            ;
	struct TRightOutLine  { TFPoint FP0; TFPoint FP1; } RightOutLine  [1]            ;
	struct TRightInLine   { TFPoint FP0; TFPoint FP1; } RightInLine   [1]            ;
	struct TBtmOutLine    { TFPoint FP0; TFPoint FP1; } BtmOutLine    [MAX_BTM_COUNT];
	struct TBtmInLine     { TFPoint FP0; TFPoint FP1; } BtmInLine     [MAX_BTM_COUNT];
	struct TBtmStandLine  { TFPoint FP0; TFPoint FP1; } BtmStandLine  [MAX_BTM_COUNT];
	struct TTopOutLine    { TFPoint FP0; TFPoint FP1; } TopOutLine    [1]            ;
	struct TTopInLine     { TFPoint FP0; TFPoint FP1; } TopInLine     [1]            ;

	// Cell Pattern Line 
	struct TLeftTrackLine { TFPoint FP0; TFPoint FP1; } LeftTrackLine [1]            ;
	struct TRightTrackLine{ TFPoint FP0; TFPoint FP1; } RightTrackLine[1]            ;
	struct TBtmTrackLine  { TFPoint FP0; TFPoint FP1; } BtmTrackLine  [1]            ;

	// LeftTrackLine or RightTrackLine or BtmTrackLine Data 없을경우 PreLeftTrackLine, PreRightTrackLine, PreBtmTrackLine 데이터 적용됨 
	struct TPreLeftTrackLine { TFPoint FP0; TFPoint FP1; } PreLeftTrackLine [1]      ;
	struct TPreRightTrackLine{ TFPoint FP0; TFPoint FP1; } PreRightTrackLine[1]      ;

	// InLine or OutLine Data가 없을경우 PreInLine,PreOutLine 데이터 적용함 
	struct TPreLeftOutLine  { TFPoint FP0; TFPoint FP1; } PreLeftOutLine [1]         ;
	struct TPreLeftInLine   { TFPoint FP0; TFPoint FP1; } PreLeftInLine  [1]         ;
	struct TPreRightOutLine { TFPoint FP0; TFPoint FP1; } PreRightOutLine[1]         ;
	struct TPreRightInLine  { TFPoint FP0; TFPoint FP1; } PreRightInLine [1]         ;

	// Circle Data 
    //----------------------------------------------------
	TCircle A1CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle A2CircleData[MAX_CIRCLE_EDGEPOINT];

	TCircle C1CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle C2CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle C3CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle C4CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle C5CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle C6CircleData[MAX_CIRCLE_EDGEPOINT];

	TCircle Btm3CircleData[MAX_CIRCLE_EDGEPOINT];

	// Grind Mark ROI 데이터 (Find알고리즘 적용)
	CRect    GrindMarkRect      [MAX_FIND_MARK_COUNT];
	TFPoint  GrindMarkPoint     [MAX_FIND_MARK_COUNT];
	TFPoint  GrindMarkRectCenter[MAX_FIND_MARK_COUNT];
	double   GrindMarkGradient  [MAX_FIND_MARK_COUNT];
	double   GrindMarkScore     [MAX_FIND_MARK_COUNT];

	CRect PadPolyRect  [2];
	CRect NotchPolyRect[6];
	// Ginrd Mark Rect Spec DvX, DvY
	TSpec GrindRectSpec[9];

	// Circle In Coordinate Data
	TFPoint *A1CirInPoint;
	TFPoint *A2CirInPoint;
// 	TFPoint *A1PolyInPoint;

	TFPoint *C1CirInPoint;
	TFPoint *C2CirInPoint;
	TFPoint *C3CirInPoint;
	TFPoint *C4CirInPoint;
	TFPoint *C5CirInPoint;
	TFPoint *C6CirInPoint;

	// Circle Out Coordinate Data
	TFPoint *A1CirOutPoint;
	TFPoint *A2CirOutPoint;

	TFPoint *C1CirOutPoint;
	TFPoint *C2CirOutPoint;
	TFPoint *C3CirOutPoint;
	TFPoint *C4CirOutPoint;
	TFPoint *C5CirOutPoint;
	TFPoint *C6CirOutPoint;

	// Narrow Trench In/Out Data 
	TFPoint *Btm3InPoint ;
	TFPoint *Btm3OutPoint;

	// Circle 박리검사 데이터 
	TFPoint *A1CirInCrackPoint;
	TFPoint *A2CirInCrackPoint;

	TFPoint *C1CirInCrackPoint;
	TFPoint *C2CirInCrackPoint;
	TFPoint *C3CirInCrackPoint;
	TFPoint *C4CirInCrackPoint;
	TFPoint *C5CirInCrackPoint;
	TFPoint *C6CirInCrackPoint;

	// Circle(Grind_Measure) Stand Edge Coordinate Data 
	TFPoint *C1CirStandPoint;
	TFPoint *C2CirStandPoint;
	TFPoint *C3CirStandPoint;
	TFPoint *C4CirStandPoint;
	TFPoint *C5CirStandPoint;
	TFPoint *C6CirStandPoint;

	// Circle In_Edge Point Count 
	int A1CirInEdgeCount;
	int A2CirInEdgeCount;
// 	int A1PolyInEdgeCount;

	int C1CirInEdgeCount;
	int C2CirInEdgeCount;
	int C3CirInEdgeCount;
	int C4CirInEdgeCount;
	int C5CirInEdgeCount;
	int C6CirInEdgeCount;

	// Circle Out_Edge Point Count 
	int A1CirOutEdgeCount;
	int A2CirOutEdgeCount;

	int C1CirOutEdgeCount;
	int C2CirOutEdgeCount;
	int C3CirOutEdgeCount;
	int C4CirOutEdgeCount;
	int C5CirOutEdgeCount;
	int C6CirOutEdgeCount;

	// Narrow Trench Data Count
	int Btm3InEdgeCount ;
	int Btm3OutEdgeCount;

	// Circle(Grind_Measure) Stand Edge Point Count
	int C1CirStandEdgeCount;
	int C2CirStandEdgeCount;
	int C3CirStandEdgeCount;
	int C4CirStandEdgeCount;
	int C5CirStandEdgeCount;
	int C6CirStandEdgeCount;

	// 박리불량 검출 Count
	// Circle Out_Edge Point Count 
	int A1CirInCrackCount;
	int A2CirInCrackCount;

	int C1CirInCrackCount;
	int C2CirInCrackCount;
	int C3CirInCrackCount;
	int C4CirInCrackCount;
	int C5CirInCrackCount;
	int C6CirInCrackCount;

	int Btm3CirInCrackCount;

	// Circle Dx, Dy, Round Data, Grind_Width, Grind_Measure 
	double A1CirDx          ;
	double A1CirDy          ;
	double A1CirRound       ;
	double A1CirGrindWd [4] ;
	double A1CirGrindMea[4] ;
	double A2CirDx          ;
	double A2CirDy          ;
	double A2CirRound       ;
	double A2CirGrindWd [4] ;
	double A2CirGrindMea[4] ;
	double ASideGrindWd [MAX_ASIDE_GRIND_POINT];
	double ASideGrindMea[MAX_ASIDE_GRIND_POINT];

	double C0CirGrindWd ; 
	double C0CirGrindMea; 
	double C1CirDx      ;
	double C1CirDy      ;
	double C1CirRound   ;
	double C1CirGrindWd ;
	double C1CirGrindMea;
	double C2CirDx      ;
	double C2CirDy      ;
	double C2CirRound   ;
	double C2CirGrindWd ;
	double C2CirGrindMea;
	double C3CirDx      ;
	double C3CirDy      ;
	double C3CirGrindWd ;
	double C3CirGrindMea;
	double C3CirRound   ;
	double C4CirDx      ;
	double C4CirDy      ;
	double C4CirRound   ;
	double C4CirGrindWd ;
	double C4CirGrindMea;
	double C5CirDx      ;
	double C5CirDy      ;
	double C5CirRound   ;
	double C5CirGrindWd ;
	double C5CirGrindMea;
	double C6CirDx      ;
	double C6CirDy      ;
	double C6CirRound   ;
	double C6CirGrindWd ;
	double C6CirGrindMea;
	double C7CirGrindWd ; 
	double C7CirGrindMea; 

	// Trench Circle CrossPoint
	double TrenchLeftGrindWd  ;
	double TrenchLeftGrindMea ;
	double TrenchRightGrindWd ;
	double TrenchRightGrindMea;

	// Corner Grind_Width, Grind_Measure 
	double C1CorGrindWd  ;
	double C1CorGrindMea ;
	double C2CorGrindWd  ;
	double C2CorGrindMea ;
	double C3CorGrindWd  ;
	double C3CorGrindMea ;
	double C4CorGrindWd  ;
	double C4CorGrindMea ;

	// Btm영역의 직선영역의 Grind_Width, Grind_Measure
	double BtmGrindWidth1  ;
	double BtmGrindWidth2  ;
	double BtmGrindWidth3  ;
	double BtmGrindWidth4  ;
	double BtmGrindWidth5  ;
	double BtmGrindWidth6  ;
	double BtmGrindWidth7  ;
	double BtmGrindWidth8  ;
	double BtmGrindWidth9  ;
	double BtmGrindMeasure1;
	double BtmGrindMeasure2;
	double BtmGrindMeasure3;
	double BtmGrindMeasure4;
	double BtmGrindMeasure5;
	double BtmGrindMeasure6;
	double BtmGrindMeasure7;
	double BtmGrindMeasure8;
	double BtmGrindMeasure9;

	//Circle Center Point
	// 교차점 기준 CenterPoint 
	TFPoint A1CirCenter;
	TFPoint A2CirCenter;

	TFPoint C1CirCenter;
	TFPoint C2CirCenter;
	TFPoint C3CirCenter;
	TFPoint C4CirCenter;
	TFPoint C5CirCenter;
	TFPoint C6CirCenter;

	// Real Circle Center Point 
	// Euresys Circle Fitting Center 
	////////////////////////////////////////////////
	TFPoint RealA1CirCenter;
	TFPoint RealA2CirCenter;

	TFPoint RealC1CirCenter;
	TFPoint RealC2CirCenter;
	TFPoint RealC3CirCenter;
	TFPoint RealC4CirCenter;
	TFPoint RealC5CirCenter;
	TFPoint RealC6CirCenter;
	////////////////////////////////////////////////

	// FAVION Circle Fitting Center 
	///////////////////////////////////////////////
	TFPoint ACircleFit1Center;
	TFPoint ACircleFit2Center;

	TFPoint CircleFit1Center;
	TFPoint CircleFit2Center;
	TFPoint CircleFit3Center;
	TFPoint CircleFit4Center;
	TFPoint CircleFit5Center;
	TFPoint CircleFit6Center;

	// Narrow Trench 
	TFPoint CircleFitBtm3Center;
	////////////////////////////////////////////////

	// 반원형 박리 및 꽃박리 구별목적 Diameter
	double CrackACirDiameter1;
	double CrackACirDiameter2;
	double CrackCirDiameter1 ;
	double CrackCirDiameter2 ;
	double CrackCirDiameter3 ;
	double CrackCirDiameter4 ;
	double CrackCirDiameter5 ;
	double CrackCirDiameter6 ;
	// Euresys Circle Fitting Diameter 
	double ACirDiameter1;
	double ACirDiameter2;
	double CirDiameter1 ;
	double CirDiameter2 ;
	double CirDiameter3 ;
	double CirDiameter4 ;
	double CirDiameter5 ;
	double CirDiameter6 ;

	// FAVION Circle Fitting Diameter
	double ACirFitDiameter1;
	double ACirFitDiameter2;
	double CirFitDiameter1 ;
	double CirFitDiameter2 ;
	double CirFitDiameter3 ;
	double CirFitDiameter4 ;
	double CirFitDiameter5 ;
	double CirFitDiameter6 ;
	// Narrow Trench 
	double CirFitDiameterBtm3;
	//////////////////////////////////////////////

	// Track Line Circle Center Point 
	TFPoint TrackC1CirCenter;
	TFPoint TrackC2CirCenter;
	TFPoint TrackC3CirCenter;
	TFPoint TrackC4CirCenter;
	TFPoint TrackC5CirCenter;
	TFPoint TrackC6CirCenter;

	TPoint BtmEdgePoint[MAX_BTM_LINE_POINT];
	TPoint TopEdgePoint[MAX_TOP_LINE_POINT];

	// 개별 Circle Spec Width, Height 
	// A면 Circle Dx, Dy 
	double ASideCircleWidth  ;
	double ASideCircleHeight ;

	// A면 Align Mark에서 GrindMark까지 거리 편차값 
	// PatternFinder ROI영역 자동 설정 목적 
	double AMarkToGrindMarkDvX;
	double AMarkToGrindMarkDvY;

	double AMarkToGrindMarkDvX2;
	double AMarkToGrindMarkDvY2;

	// C면 Circle Dx, Dy 
	double CSideCircleWidth  ;
	double CSideCircleHeight ;
	double CenterCircleWidth ;
	double CenterCircleHeight;

	// Narrow Trench 비션형 검사시 필요함 
	// Cir2,Cir3 Dx값 기준으로 비선형 알고리즘 ROI영역을 설정하였으나..이또한 GrindMark수량 및 포지션 변화로 인해서 
	// 부득이하게 추가 업데이트 필요함 
	// 점점 소스가 더러워지고 있음....
	double Center1CircleWidth;
	double Center2CircleWidth;

	// 연마 각도 (Circle)
	int SideCirDeg   ;
	int CenterCirDeg ;

	// TrackLine 기준 Width, Height 
	double TrackCircleWidth ;
	double TrackCircleHeight;

	// 외곽라인에서 TrackLine까지 거리 
	double OutToTrackLineDis;

	// 외곽라인에서 개별 GrindMark까지 Width,Height 거리
	double OutToGrindMarkDis1;
	double OutToGrindMarkDis2;

	// 박리 검출 목적으로 EdgeLine에서 Trap까지 거리 
	double dEdgeToTrapLineDis;
	// Circle Offset 
	double dCirOffsetX;
	double dCirOffsetY;

	// Grind Width 측정 데이터 
	float Left_Grind_Width ;
	float Left_Grind_Mea   ;
	float Right_Grind_Width;
	float Right_Grind_Mea  ;
	float Btm_Grind_Width  ;
// 	float Top_Grind_Width  ;
// 	float Top_Grind_Mea    ;

	// TrackLine Data OverLap 
	int AvgLeftTrackLineDis    ;
	int AvgRightTrackLineDis   ;
	int AvgLeftBtmTrackLineDis ;
	int AvgRightBtmTrackLineDis;
	int LeftTrackLineCount     ;
	int RightTrackLineCount    ;
	int TotalLeftTrackDis      ;
	int TotalRightTrackDis     ;

	// Cell Size측정 결과 데이터(Notch_Cell)
	double LeftCellSizeHeight  ;  
	double CenCellSizeHeight   ;  
	double RightCellSizeHeight ;  

	double CenCellSizeWidth    ;
	double NotchCellSizeWidth  ;
	double NotchCellSizeHeight ;

	double LeftMarkToTopHeight ; //// Cell Mark To TopSide    ==>M1
	double CenMarkToTopHeight  ; //// Cell Mark To TopSide    ==>M2
	double RightMarkToTopHeight; //// Cell Mark To TopSide    ==>M3

	// Cell Size 측정 목적(기준좌표) 
	TFPoint LeftStaPos      ;
	TFPoint LeftEndPos      ;
	TFPoint CenterStaPos    ;
	TFPoint CenterEndPos    ;

	// Center Cell Size  측정시 필요한 데이터 
	float TotalCellSizeWidth;
	float CenterCellSize    ;
	int   TotalCellSizeCount;

	// Grind Mark Find Score 
	double GrindMarkMinScore;
	double GrindPadMarkMinScore;

	// Align 심하게 틀어질경우 발생하는 Defect오버킬 
	int LeftDegMargin ;
	int RightDegMargin;
	//-------------------------------------------------------------------
	/////////////////////////////////////////////////////////////////////////
	// EDGE INSPECTION 
	/////////////////////////////////////////////////////////////////////////
	// InLine, OutLine, BMLine 
	struct TInLine  { TFPoint FP0; TFPoint FP1; } InLine [1];
	struct TOutLine { TFPoint FP0; TFPoint FP1; } OutLine[1];
	struct TBMLine  { TFPoint FP0; TFPoint FP1; } BMLine [1];

	// Broken Line을 찾지 못할경우 
	struct TPreInLine  { TFPoint FP0; TFPoint FP1; } PreInLine [1];
	struct TPreOutLine { TFPoint FP0; TFPoint FP1; } PreOutLine[1];
	struct TPreBMLine  { TFPoint FP0; TFPoint FP1; } PreBMLine [1];

	// Line Data 
	struct TBMLineData{int Dis1; int Dis2; int yPos1; int yPos2;};

	float  Cell_Size ;
	float  GrindSize ;
	float  Grind_Spec;

	// Defect Data 
	TBlob BurrPoint    [MAX_DEFECT_POINT];
	TBlob BrokenPoint  [MAX_DEFECT_POINT];
	TBlob CrackPoint   [MAX_DEFECT_POINT];

	// Line Data 
	TBMLineData BMLineData[MAX_LINE_COUNT];

	// Frit Remove
	int PixelData[256] ;
	double BrokenScore ;
	double CrkackScore ;
	double TotalScore  ;
	int    CellId      ;
	int    CamId       ;
	CRect BrokenRect   ;
	CString CellName   ;

	// LogUnit 
	CTheLogger LogUnit;

	// BmLine 유무, MarkLine 유무 
	bool IsBmLine   ;
	bool IsMarkLine ;
	bool IsMarkBtm  ; 
	bool IsLongTop  ; 
	bool IsShortCell;
	bool IsAllGrind ;

	// Inspection End
	bool IsInspEnd  ;

	// BMLine Data Overlap
	bool IsBMLineData;

	TFPoint MarkPos   ;
	TFPoint MarkStrPos;
	TFPoint MarkEndPos;
	TFPoint MarkPos1  ;
	TFPoint MarkPos2  ;
	float   MarkAng   ; 

	// StartLeft Line
	float StartLeft;

	// Short Cell Size Count
	int nCell_Count;
	int nTopBtmStart;

	int BMLineMargin;

	// BMLine Data Overlap
	int InspectCount;
	//-------------------------------------------------------------------

protected:
	// Property
	// ---------------------------------------------------------------------
	TFPoint FP0, FP1    ; // 기준이 되는 좌표
	bool FLeftWall      ;
	bool FTopWall       ;
	bool FRightWall     ;
	bool FBottomWall    ;
	bool FLeftTop       ;
	bool FRightTop      ;
	bool FRightBottom   ;
	bool FLeftBottom    ;
	bool FBevelIn       ;
	int FEscapeDeviation;
	int FShape          ;
	int FProcessMode    ;
	int FInspectMode    ;
	int FCutMode        ;
	int FCompanyMode    ;
	int FStartLine      ;
	int FEndLine        ;
	bool FDrawOutLine   ;
	bool FDrawBevelIn   ;
	bool FDrawRectEdge  ;
	bool FDrawDefect    ;

	//GOO
	int GrabCount   ;
	int LineIndex   ;
	int PreGrabCount;
	int NexGrabCount;
	int TotalCount  ;
	double LeftDv, RightDv;

	// 심자가 기준 a, b, c, d, e, f
	int LeftDistance ;
	int TopDistance  ;
	int RightDistance;

	//CoreRoi
	int CornerWidth ;
	int CornerHeight;

	//CoreRoi
	int LTCornerWidth ;
	int LTCornerHeight;
	int RTCornerWidth ;
	int RTCornerHeight;
	int LBCornerWidth ;
	int LBCornerHeight;
	int RBCornerWidth ;
	int RBCornerHeight;

	// Align Mark YLength
	int Mark1YLength;
	int Mark2YLength;

	// Result
	// ---------------------------------------------------------------------
	//int DefectPointCount; // 전체 불량 포인트 개수
	//int ECount       ; // 육각형 불량 포인트 개수
	//int HCount       ; // Top 불량 포인트 개수
	//int SCount       ; // Mid 불량 포인트 개수
	//int CCount       ; // Btm 불량 포인트 개수


	// Functions
	// ---------------------------------------------------------------------
	int OnAutoBevelIn();
	int OnManualBevelIn();
	float GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);

public :

	CMultiLineDefect();
	~CMultiLineDefect();

	// Define
	//----------------------------------------------------------------------
	enum TShape       { shRect8=0, shRectH, shRectHL, shRectHR, shRectC, shRectCL, shRectCR, shRectS, shRectSL, shRectSR, shRectTOP, shRectBTM, shRectCC};
	enum TProcessMode { pmAutoBevelIn, pmManualBevelIn, pmAutoOutLine                                                                                   };
	enum TCompany     { COM_KWON =0  ,COM_TIANMA , COM_BOE      , COM_ONCELL , COM_SLBRAIN, COM_NOTCH, COM_BMDT                                         };
	enum TInspectMode { Insp_Track=0 ,Insp_Mark  , Insp_Polygon ,Insp_Surface                                                                           }; 
	enum TCutMode     { Normal_Cut=0 ,CAI_Cut    , Rough_Cut                                                                                            };

	// Creat & Destory
	// ---------------------------------------------------------------------

	// Inspetion Sequence 
	bool OnInspection();

	//---------------------------------------------------------------------
	// Functions - Set
	// ---------------------------------------------------------------------
	void SetOrgP0(TFPoint P0) {FP0 = P0;}
	void SetOrgP1(TFPoint P1) {FP1 = P1;}

	// Class
	CTimer Time;

	// Euresys Class
	EImageBW8 BinaryImg             ;
	EImageBW8 DefectImg             ;
	EImageBW8 BMLineImg             ;
	EImageBW8 RotateImg             ;
	EImageBW8 RotateBinaryImg       ;
	EImageBW8 RotateGrindBinaryImg  ;
	EImageBW8 RotateGrindInBinaryImg;
	EImageBW8 RotateBrokenImg       ;
	

	// 사용자 설정 Parameter 
	// Set 함수 
	void SetProcessMode    (TProcessMode ProcessMode); 
	void SetInspectMode    (TInspectMode InspectMode); 
	void SetCutMode        (TCutMode     CutMode    );
	void SetCompanyMode    (TCompany     Company    );
	void SetShape          (TShape Shape)            ;
	void SetEscapeDeviation(int EscapeDeviation)	 ;
	void SetLeftDistance   (int Dv)					 ;
	void SetTopDistance    (int Dv)					 ;
	void SetRightDistance  (int Dv)					 ;

	// ThresHold Setting
	void SetEdgeThresHold   (int ThHold)             ;
	void SetCornerThresHold (int ThHold)             ;
	void SetLtpsThHold(int ThHold)                   ;
	void SetEnCapTopThHold(int ThHold)               ;
	void SetEnCapMidThHold(int ThHold)               ;
	void SetEnCapBtmThHold(int ThHold)               ;

	void SetStartLine      (int StartLine)           ;
	void SetEndLine        (int EndLine  )           ;
	void SetCornerWidth     (int Width   )           ;
	void SetCornerHeight    (int Height  )           ; 


	void SetLTCornerWidth     (int Width )           ;
	void SetLTCornerHeight    (int Height)           ; 
	void SetRTCornerWidth     (int Width )           ;
	void SetRTCornerHeight    (int Height)           ; 
	void SetLBCornerWidth     (int Width )           ;
	void SetLBCornerHeight    (int Height)           ; 
	void SetRBCornerWidth     (int Width )           ;
	void SetRBCornerHeight    (int Height)           ; 
	void SetCrossLeftPoint  (int x, int y)           ;  
	void SetCrossRightPoint (int x, int y)           ;  

	// 검사 Range 설정 
	void SetSpecWidth       (float MinWidth , float MaxWidth );
	void SetSpecHeight      (float MinHeight, float MaxHeight);

	// Corner 연마 여부 결정 
	void SetCornerTop(bool IsEnable);
	void SetCornerBtm(bool IsEnable);

	// Corner 연마 여부 결정 
	void SetRectTop(bool IsEnable);
	void SetRectBtm(bool IsEnable);

	// Circle 연마 여부 결정 
	void SetCircleTop    (bool IsEnable);
	void SetCircleBtm    (bool IsEnable);
	void SetSideCircleBtm(bool IsEnable);

	// NarrowTrench 선택 결정 
	void SetNarrowTrench (bool IsEnable);

	// Inspect ROI Offset
	void SetInspRoiOffset(int Offset);

	// EdgeInspection
	void SetBMLineThresHold  (int ThHold)  ;
	void SetBurrThresHold    (int ThHold)  ;
	void SetBrokenThresHold  (int ThHold)  ;
	void SetCrackThresHold   (int ThHold)  ;
	void SetGrindThresHold   (int ThHold)  ;
	void SetBurrOffset       (int Offset)  ;
	void SetIgnoreOffset     (int Offset)  ;
	void SetGrindDirection   (int Dir   )  ;
	void SetGrindInspection  (int GrindInsp);

	void SetBmToMarkDistance (double Distance);
	void SetMinBrokenWidth   (float MinWidth );
	void SetMinBrokenHeight  (float MinHeight);
	void SetMinBurrWidth     (float MinWidth );
	void SetMinBurrHeight    (float MinHeight);
	void SetMinChippingWidth (float MinWidth );
	void SetMinChippingHeight(float MinHeight);

	// TEST 
	void OnWriteLogFile();

	//-----------------------------------------------------------------------------
	// Set Functions End 
	//-----------------------------------------------------------------------------


	//---------------------------------------------------------------------
	// Functions - Get
	// ---------------------------------------------------------------------
	int  GetProcessMode()        ;
	TFPoint GetOrgP0()			 ;
	TFPoint GetOrgP1()			 ;
	int     GetShape()	         ;
	CFind*  GetFindPointer(int n);
// 	JTrace* GetTracePointer()    ;
	CEdge * GetEdgePointer ()    ;
	int     GetEscapeDeviation() ;


	// Functions - Execute
	// ---------------------------------------------------------------------
	int OnExecute();
	int OnExecute(TShape Shape);
	int OnExecute(bool l, bool t, bool r, bool b, bool lt, bool rt, bool rb, bool lb);
	int GetSize(TCorner *lpCorner,TBlob *lpDefect, int Count, int Dv);
	int GetCircleSize     (TCircle *lpCircle,TBlob *lpDefect, int Count, int Dv );
	int GetPolygonSize    (TFPoint *PolygonData, TBlob *lpDefect,int TotalCount,int Range, int DefectDv);
// 	int GetCircleCrackSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dis);
	int GetCircleCrackSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dis, double CirDiameter, TFPoint Center, bool IsInCircle=true);
	int GetAvgCellSize(int TopCount, int MidCount, int BtmCount, int AvgMode); // AvgMode==0, TopAvg, AvgMode==1, MidAvg, AvgMode==2, BtmAvg, AvgMode==4, AllAvg
	int GetCellSize   (int Count                                            ); 
	void OnClearData();

	CRect OnMerge(CRect ar, CRect br);

	//----------------------------------------------------------------------
	// Get Functions End 
	//----------------------------------------------------------------------

	// Dark Defect 
	int GetResultDefectPointCount()      ;
	int GetResultACountPointCount()      ;
	int GetResultBCountPointCount()      ;
	int GetResultCCountPointCount()      ;
	int GetResultDCountPointCount()      ;
	int GetResultC1CountPointCount()     ;
	int GetResultC2CountPointCount()     ;
	int GetResultC3CountPointCount()     ;
	int GetResultC4CountPointCount()     ;
	int GetResultABurrPointCount  ()     ;
	int GetResultBBurrPointCount  ()     ;
	int GetResultCBurrPointCount  ()     ;
	int GetResultDBurrPointCount  ()     ;

	int GetResultA1CirPointCount  ()     ;
	int GetResultA2CirPointCount  ()     ;
	int GetResultC1CirPointCount  ()     ;
	int GetResultC2CirPointCount  ()     ;
	int GetResultC3CirPointCount  ()     ;
	int GetResultC4CirPointCount  ()     ;
	int GetResultC5CirPointCount  ()     ;
	int GetResultC6CirPointCount  ()     ;
	int GetResultBtm3CirPointCount()     ;

	// Bright Defect 
	int GetResultACrackPointCount()      ;
	int GetResultBCrackPointCount()      ;
	int GetResultCCrackPointCount()      ;
	int GetResultDCrackPointCount()      ;
	int GetResultA1CirCrackPointCount()  ;
	int GetResultA2CirCrackPointCount()  ;
	int GetResultC1CirCrackPointCount()  ;
	int GetResultC2CirCrackPointCount()  ;
	int GetResultC3CirCrackPointCount()  ;
	int GetResultC4CirCrackPointCount()  ;
	int GetResultC5CirCrackPointCount()  ;
	int GetResultC6CirCrackPointCount()  ;
	int GetResultBtm3CirCrackPointCount();


	TFPoint GetResultDefectPoint(int n) ;

	TFPoint GetResultLTPoint () ;
	TFPoint GetResultRTPoint () ;
	TFPoint GetResultRBPoint () ;
	TFPoint GetResultLBPoint () ;
	TFPoint GetResultLTLPoint() ; 
	TFPoint GetResultLTTPoint() ; 
	TFPoint GetResultRTRPoint() ; 
	TFPoint GetResultRTTPoint() ; 
	TFPoint GetResultRBRPoint() ; 
	TFPoint GetResultRBBPoint() ; 
	TFPoint GetResultLBLPoint() ; 
	TFPoint GetResultLBBPoint() ; 

	float GetResultDistanceFromLTLToLTT() ;
	float GetResultDistanceFromRTRToRTT() ;
	float GetResultDistanceFromRBRToRBB() ;
	float GetResultDistanceFromLBLToLBB() ;

	// GOO   Corner DisX, DisY
	double GetResultC1DisX();
	double GetResultC1DisY();
	double GetResultC2DisX();
	double GetResultC2DisY();
	double GetResultC3DisX();
	double GetResultC3DisY();
	double GetResultC4DisX();
	double GetResultC4DisY();

	// GOO Device Height, Width
	double GetResultSizeH();
	double GetResultSizeW();

	//EDGE INSPECTION
	int GetResultCrackPointCount ();
	int GetResultBrokenPointCount();
	int GetResultBurrPointCount  ();

	//---------------------------------------------------------------------
	// Get Functions End
	// ---------------------------------------------------------------------



	// Functions - Paint
	// ---------------------------------------------------------------------
	void OnPaint       (TDrawTool Tool);
	void OnPaintOutLine(TDrawTool Tool);
	void OnPaintBevelIn(TDrawTool Tool);
	void OnPaintDefect (TDrawTool Tool);
	void OnPaintRectEdge(TDrawTool Tool);

	// Functions - Laod & Save
	// ---------------------------------------------------------------------
	void OnLoad(bool IsLoad, CString Folder);

	void Train(int n, BYTE* pImg, int imgW, int imgH, int roi_x, int roi_y, int roi_w, int roi_h);
};

#endif // !defined(AFX_LXLINEDEFECT_H__02D37965_8899_4373_830E_A5A6C9E513F1__INCLUDED_)
