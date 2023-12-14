#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LxHeader.h"
#include "LxLine.h"
#include "LxBlob.h"
#include "LxFind.h"
#include "TheLogger.h"
#include "CSystemMon.h"
#include "LxDoubleLine.h"
#include "LxMatch.h"
#include "LxFitting.h"
#include "Open_eVision_1_2.h"
#include "LxCircle.h"

using namespace Euresys::Open_eVision_1_2;
// using namespace The;

class AFX_EXT_CLASS CFM3xLineDefect : public CFilter { //신규 프로젝트(LineScan4k X 8)

private :

protected :
	//----------------------------------------------------------------------
	// Architecture
	// ---------------------------------------------------------------------
	CFind        *Find1     ;
	CFind        *Find2     ;
	CSingleLine  *SingleLine;
	CRectEdge    *RectEdge  ;
	CEdge        *Edge      ;
	CCircle      *Circle    ;
	CFind        *Find      ;
	CCircleFit   *CircleFit ;
	CPolygon     *Polygon   ; //사용 
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// Struct Define  
	// ---------------------------------------------------------------------
	struct TBlob   {double Height; double Width; int Round; TFPoint P; CRect Rect; double Hor_Dis; double Ver_Dis; int LineAttach;};
	struct TCorner {float Dv; TFPoint P;};
	struct TCircle {float Dv; float Dis; TFPoint P;};
	struct TSpec   {double DvX; double DvY;};
	// Polygon Defect 
	struct TBadRect{CRect Rect; TFPoint SPoint; TFPoint EPoint; int BadPixelCount; TFPoint Pixel[MAX_POLYGON_DEFECT_POINT];};
	// GrindMark 
	struct TGrindMark{CRect Rect; TFPoint Point; TFPoint RectPoint; int MarkID; double Range; double Gradient; double Score; double degree; int DvX; int DvY;double GrindWidth; double GrindMea;};
	struct TPatternDis{double Dis;};
	struct TPolygonROI{int StaX; int StaY; int Width; int Height;};
	struct TGrindLine { TPoint LinePoint; bool IsGrindLine;};
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// Property
	// ---------------------------------------------------------------------
	TFPoint FP0, FP1 ; // 기준이 되는 좌표
	bool FLeftWall   ;
	bool FTopWall    ;
	bool FRightWall  ;
	bool FBottomWall ;
	bool FLeftTop    ;
	bool FRightTop   ;
	bool FRightBottom;
	bool FLeftBottom ;
	bool FBevelIn    ;
	int FShape       ;
	int FProcessMode ;
	int FInspectMode ;
	int FCutMode     ;
	int FLineScanType;
	int FGlassType   ;
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// Euresys Data 
	// ---------------------------------------------------------------------
	// Euresys Class
	EImageBW8 BinaryImg             ; // 라인을 찾기 위한 이미지 
	EImageBW8 DefectImg             ; // Defect검사 목적 이미지 
	EImageBW8 MaskDarkDefectImg     ; // DarkDefect검사 목적 이미지 
	EImageBW8 BrightDefectImg       ; // BrightDefect검사 목적 이미지

// 	EImageBW8 RotateImg             ;
// 	EImageBW8 RotateBinaryImg       ;
// 	EImageBW8 RotateGrindBinaryImg  ;
// 	EImageBW8 RotateGrindInBinaryImg;
// 	EImageBW8 RotateBrokenImg       ;
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
public    :
	//----------------------------------------------------------------------
	// Architecture
	// ---------------------------------------------------------------------
	CTheLogger   LogUnit  ;
	CSystemMon   SystemMon;
	CTimer       Time     ;
	CFind        MarkFind[MAX_GRINDMARK_ID];
	//----------------------------------------------------------------------
	// Define
	//----------------------------------------------------------------------
	enum TShape       { shRectH     =0, shRectC        , shRectS                    };
	enum TProcessMode { pmAutoBevelIn , pmManualBevelIn, pmAutoOutLine              };
	enum TInspectMode { Insp_Track  =0, Insp_Mark      , Insp_Polygon  ,Insp_Surface}; 
	enum TGlassType   { Mobile_Glass=0, VR_Glass                                    };

	//----------------------------------------------------------------------
	// Struct 
	//----------------------------------------------------------------------
	struct TLine          { TFPoint FP0; TFPoint FP1; } Line          [20] ; // 기본라인 
	struct TLeftOutLine   { TFPoint FP0; TFPoint FP1; } LeftOutLine   [1 ] ;
	struct TLeftInLine    { TFPoint FP0; TFPoint FP1; } LeftInLine    [1 ] ;
	struct TLeftStdLine   { TFPoint FP0; TFPoint FP1; } LeftStdLine   [1 ] ;
	struct TBtmOutLine    { TFPoint FP0; TFPoint FP1; } BtmOutLine    [1 ] ;
	struct TTopOutLine    { TFPoint FP0; TFPoint FP1; } TopOutLine    [1 ] ;

	// InLine or OutLine Data가 없을경우 PreInLine,PreOutLine 데이터 적용함 
	struct TPreLeftOutLine  { TFPoint FP0; TFPoint FP1; } PreLeftOutLine [1];
	struct TPreLeftInLine   { TFPoint FP0; TFPoint FP1; } PreLeftInLine  [1];
	//----------------------------------------------------------------------
	//Defect Count 
	// ---------------------------------------------------------------------
	int DefectPointCount; // 전체 불량 포인트 개수
	int ACount          ; // A_SIDE  불량 포인트 개수
	int BCount          ; // B_SIDE  불량 포인트 개수
	int CCount          ; // C_SIDE  불량 포인트 개수
	int DCount          ; // D_SIDE  불량 포인트 개수
	int LTCorCount      ; // C1_SIDE 불량 포인트 개수
	int LBCorCount      ; // C2_SIDE 불량 포인트 개수
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
	int LTCirDeftectCnt ;
	int LBCirDeftectCnt ;

	// White Defect 
	int LTCirCrackCnt  ;
	int LBCirCrackCnt  ;

	// 전체 연마시 DarkDefect 
	int LeftDefectCnt     ;
	int FrameLeftDefectCnt;
	// 전체 연마시 BrightDefect 
	int LeftCrackCnt     ;
	int FrameLeftCrackCnt;
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// Defect Data  
	// ---------------------------------------------------------------------
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

	// Corner Defect(Dark_Defect)
	TBlob LTCorDefectPoint [MAX_DEFECT_POINT];  
	TBlob LBCorDefectPoint [MAX_DEFECT_POINT];  

	// Dark Circle Defect 
	// Circle Defect 
	TBlob LTCirDefectPoint  [MAX_DEFECT_POINT];
	TBlob LBCirDefectPoint  [MAX_DEFECT_POINT];

	// Bright Circle Defect 
	// Circle Defect 
	TBlob LTCirCrackPoint  [MAX_DEFECT_POINT];
	TBlob LBCirCrackPoint  [MAX_DEFECT_POINT];

	// 전체 연마시(단축 AllGrind) 
	// Left Dark Defect 
	TBlob LeftDefectPoint  [MAX_DEFECT_POINT];
	// Left Bright Defect 
	TBlob LeftCrackPoint   [MAX_DEFECT_POINT];

	// Polygon Defect Data 
	TBadRect BadRect[MAX_POLYGON_DEFECT_COUNT];

	//Burr Data 
	//현재는 필요 없으나...나중에 필요할지모 모름!! 
	TBlob ABurrPoint   [MAX_DEFECT_POINT];
	TBlob BBurrPoint   [MAX_DEFECT_POINT];
	TBlob CBurrPoint   [MAX_DEFECT_POINT];
	TBlob DBurrPoint   [MAX_DEFECT_POINT];
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// Data Memory (Real Edge Point Data)
	// ---------------------------------------------------------------------
	// Corner Edge Point Data(Normal)
	TFPoint *LTCorPoint;
	TFPoint *LBCorPoint;

	// Corner Edge Point Data(Corner 연마검사 Data) NOTCH 
	TFPoint *LTCorInPoint;
	TFPoint *LBCorInPoint;

	// Total 연마 In/Out Data 
	TFPoint AllCornerInData [MAX_ALL_EDGE_COUNT];
	TFPoint AllCornerOutData[MAX_ALL_EDGE_COUNT];

// 	TFPoint AllCornerInData [1];
// 	TFPoint AllCornerOutData[1];

	// Corner Edge Point Data(Corner 연마검사 Data) NOTCH 
	TFPoint *LTCorMeaPoint;
	TFPoint *LBCorMeaPoint;
	// Corner Calculate 
	TFPoint P1;
	TFPoint P2;
	// Circle In Coordinate Data
	TFPoint *LTCirInPoint;
	TFPoint *LBCirInPoint;
	// Circle Out Coordinate Data
	TFPoint *LTCirOutPoint;
	TFPoint *LBCirOutPoint;

	// Total 연마 In/Out Data 
	TFPoint AllCircleInData [MAX_ALL_EDGE_COUNT];
	TFPoint AllCircleOutData[MAX_ALL_EDGE_COUNT];

// 	TFPoint AllCircleInData [1];
// 	TFPoint AllCircleOutData[1];

	// AllGrind경우 개별 Frame기준 데이터 취합
	TFPoint *LeftInPoint ;
	TFPoint *LeftOutPoint;

	// Cutting 단차로 인해서 발생하는 문제 해결 목적 변수 
	TFPoint *FrameLeftInPoint ;
	TFPoint *FrameLeftOutPoint;

	// Total 연마 In/Out Data 
	TFPoint AllLeftInData [MAX_ALL_EDGE_COUNT];
	TFPoint AllLeftOutData[MAX_ALL_EDGE_COUNT];

	// Circle 박리검사에 필요한 데이터 취합(Circle Fitting 알고리즘 정용시 사용)
	// Circle Fitting 알고리즘 적용시 Bright_Defect검출을 위해서 필요한 데이터 
// 	TFPoint *LTCirInCrackPoint;
// 	TFPoint *LBCirInCrackPoint;

	// ---------------------------------------------------------------------
	// Compare Data(중간 데이터)
	// ---------------------------------------------------------------------
	// 검사를 위해서 필요한 중간 데이터 
	TCorner LTCorner[MAX_CORNER_POINT];
	TCorner LBCorner[MAX_CORNER_POINT];
// 	TCorner C3Corner[MAX_CORNER_POINT];
// 	TCorner C4Corner[MAX_CORNER_POINT];

	// Bright Defect 검출에 필요한 EdgePoint 데이터(Polygon 알고리즘 사용시 필요한 데이터)
	// Polygon Algorithm 
	TCircle LTCircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle LBCircleData[MAX_CIRCLE_EDGEPOINT];

	// Bright Defect 검출에 필요한 EdgePoint 데이터(Polygon 알고리즘 사용시 필요한 데이터)
	// Polygon Algorithm 
	TCircle LeftCircleData[MAX_CIRCLE_EDGEPOINT];
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// GrindMark Data && ROI Daata 
	// ---------------------------------------------------------------------
	//8_Camera 검사시 필요한 GrindMark Data 
	TGrindMark GrindMark    [MAX_GRIND_MARK_COUNT];
	// Hole Grind Mark 
	TGrindMark HoleGrindMark[MAX_HOLE_GRIND_MARK_COUNT];
	// EnCap ASide Pattern Distance 
	TPatternDis PatternDis  [MAX_GRIND_MARK_COUNT];

	// CutLine vs GrindLine 구별목적 
	TGrindLine GrindLine[MAX_ROI_ID];

	// ChamberCut or Circle 가공에 필요한 ROI영역 
	CRect   ASideRect[1];
	CRect   CSideRect[1];
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------

	// 직선영역 검사목적 교차점 포인트 
// 	TPoint BtmEdgePoint[2];
// 	TPoint TopEdgePoint[2];
	// ---------------------------------------------------------------------
	// Data Count Memory(EdgePoint)
	// ---------------------------------------------------------------------
	// Corner Edge Point Count
	int LTCorEdgeCount;
	int LBCorEdgeCount;

	// Corner InEdge Point Count
	int LTCorInEdgeCount;
	int LBCorInEdgeCount;

	// 부분연마시 전체 EdgePoint Count
	int AllCorInEdgeCount  ;
	int AllCorOutEdgeCount ;

	// Corner Grind Measure StandCount 
	int LTCorMeaEdgeCount;
	int LBCorMeaEdgeCount;

	// Circle In_Edge Point Count 
	int LTCirInEdgeCount;
	int LBCirInEdgeCount;

	// Circle Out_Edge Point Count 
	int LTCirOutEdgeCount;
	int LBCirOutEdgeCount;

	// 부분연마시 전체 EdgePoint Count
	int AllCirInEdgeCount  ;
	int AllCirOutEdgeCount ;

	// New 8_Camera  
	// 단축,장축 검사시 전체 연마검사시 필요한 Count_Data 
	int LeftInEdgeCount     ;
	int LeftOutEdgeCount    ;
	int AllLeftInEdgeCount  ;
	int AllLeftOutEdgeCount ;

	// Cutting 단차로 인해서 발생하는 문제 해결 목적 변수 
	int FrameLeftInEdgeCount ;
	int FrameLeftOutEdgeCount;
	int StaLeftInEdgeCount   ;
	int StaLeftOutEdgeCount  ;

	// 단축 부분연마시 필요한 Count_Data 

	// 박리불량 검출전 Edge Point Count
	int LTCirInCrackCount;
	int LBCirInCrackCount;

	// 박리불량 검출전 Edge Point Count
    int LeftCrackCount;
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------

	// ---------------------------------------------------------------------
	// ThresHold 
	// ---------------------------------------------------------------------
	int ECThresHold_Sta        ; // TopCorner ThresHold 신규 TCG 이미지 기준 상단 코너 
	int ECThresHold_End        ; // BTMCorner ThresHold 신규 TCG 이미지 기준 하단 코너 
	int CorGrindInThresHold_Sta; // Corner 안쪽 검출 ThresHold 신규 TCG 이미지 기준 상단 코너
	int CorGrindInThresHold_End; // Corner 안쪽 검출 ThresHold 신규 TCG 이미지 기준 하단
	int CorGrindMeaThresHold; // Corner 3Point 검출시 사용되는 ThresHold(가공량 측정 목적)

	// Defect ThresHold 
// 	int EnCapTopThresHold  ; // Top Defect Find
// 	int EnCapMidThresHold  ; // Mid Defect Find
// 	int EnCapBtmThresHold  ; // Bot Defect Find
	int DarkThresHold      ;
	int BrokenThresHold    ; // Bright Defect Find 
	int MaskDefectThresHold; // Mask Dark Defect ThresHold 

	// Mask Image 생성을 위한 Offset 
	int MaskDefectOffset   ; // Mask Dark Defect Offset  
	int MaskOffsetDistance ; // Mask Defect Offset Distance 

	// 외곽라인 찾는 기본 ThresHold 
	int LeftEdgeThresHold  ;
	int TopEdgeThresHold   ;
	int RightEdgeThresHold ;
	int BtmEdgeThresHold   ;

    bool LeftEdgeBTWhite ; //Black To White Edge 검출 
	bool TopEdgeBTWhite  ; //Black To White Edge 검출 
	bool RightEdgeBTWhite; //Black To White Edge 검출 
	bool BtmEdgeBTWhite  ; //Black To White Edge 검출 

	// 연마라인 찾는 ThresHold 
	int LeftGrindThresHold ;
	int TopGrindThresHold  ;
	int RightGrindThresHold;
	int BtmGrindThresHold  ;

	// Cutting라인 찾는 ThresHold 
	int LeftCutThresHold   ;

	//Pattern Distacne 안쪽라인 검출 ThresHold 
	int PatternThresHold   ;
	int PatternOffSet      ; //가공상태가 크기때문에 PatternDistance측정기준 Offset적용 및 계측데이터 재산출  

	int CirGrindInThresHold ; // MarkMode  ==> Circle안쪽 검출 ThresHold
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// EdgeLine 교차점 Data (TFPoint)
	// ---------------------------------------------------------------------
	// 실측 Edge검출 외곽라인 데이터 
	TFPoint FLTPoint; //좌측라인과 상단라인의 교차점 좌표
	TFPoint FLBPoint; //좌측라인과 하단라인의 교차점 좌표

	TFPoint FLTLPoint; //좌측라인과   첫번째 코너 교차점 좌표 
	TFPoint FLTTPoint; //위쪽라인과   첫번째 코너 교차점 좌표 
	TFPoint FLBLPoint; //좌측라인과   네번째 코너 교차점 좌표 
	TFPoint FLBBPoint; //아래쪽라인과 네번째 코너 교차점 좌표 

	// NOTCH 검사시 필요한 Corner 연마 안쪽라인 교차점 좌표
	TFPoint FLTLInPoint;
	TFPoint FLTTInPoint;
	TFPoint FLBLInPoint;
	TFPoint FLBBInPoint;

	// NOTCH 검사시 필요한 Corner 연마량 기준 라인 교차점 
	TFPoint FLTLMeaPoint;
	TFPoint FLTTMeaPoint;
	TFPoint FLBLMeaPoint;
	TFPoint FLBBMeaPoint;

	//각도 계산시 필요한 기준 Mark좌표 
	TFPoint AlignMark1;
	TFPoint AlignMark2;

	// Grind검사시 ROI영역 좌표 
	TPoint PolyP1[MAX_ROI_ID];
	TPoint PolyP2[MAX_ROI_ID];
	TPoint PolyP3[MAX_ROI_ID];
	TPoint PolyP4[MAX_ROI_ID];

	//전체 Buffer기준 좌표 
	TFPoint BuffLTPoint;

	// 
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------

	// ---------------------------------------------------------------------
	// Cell Information && 기타 
	// ---------------------------------------------------------------------
	// Corner Dx, Dy 데이터 
	double LTDx; 
	double LTDy; 
	double LBDx;
	double LBDy;

	// Match 파일 경로 
	CString FindDir ;
	CString MatchDir;

	// Corner 연마검사 여부 확인 
	bool IsCornerTop;
	bool IsCornerBtm;
	// Corner 연마검사 여부 확인 
	bool IsRectTop  ;
	bool IsRectBtm  ;
	// Cell Circle Grind 검사 유무 결정
	bool IsCircleTop;
	bool IsCircleBtm;

	// Frame기준 GrindMark 존재유무 결정 
	bool IsFrameGrindMark;

	// Inspection Blob Spec 
	float FMinWidth   ;
	float FMaxWidth   ;
	float FMinHeight  ;
	float FMaxHeight  ;

	// Defect Line Position 
	float LinePosition ;
	float LeftLineData ;
	float RightLineData;

	// Image Rotate Degree
	float ImgDegree;
	// GrindMark 영역설정시 필요한 각도 데이터 
	double dSideDeg; 
	// Display 목적 회전 중심 좌표 
	TFPoint CenterPoint;
	// Pixel 내부 데이터 취합시 필요한 Buffer 
// 	void * IAddress;

	// 외곽기준 vs Size기준 선택 
	bool InspectMode;    // 0: 외곽기준  1:Size 기준(BMLine 기준)

	//LHJ 추가 RQ_Model 진행하면서 RQ_Model 일경우 A2 SetAngleBias를 135 -> 150으로 상향함. A1은 잘잡혀서 안함
	bool RqMode;
	//LHJ 추가 WK_Model 진행하면서 WK_Model 일경우 C3,C4 (Cir8,Cir12) SetAngleBias를 315 -> 290 / 45 -> 85 으로 조절함
	bool WkMode;

	// 개별 Frame기준 Image정보 
	int FrameImgW;
	int FrameImgH;
	// 전체 Frame 기준 ImageHeight
	int ImageHeight;
// 	int ImageWidth ;

	// ImageHeightEnd
	int ImageHEnd  ;

	// Mark Rotate Degree 
	float MarkDegree;

	// ROI영역 범위 제한 목적 
	int BtmLimitLine        ;
	int TopLimitLine        ;

	bool IsMarkBtm  ; 
	bool IsAllGrind ;
	bool IsShortCell;

	TFPoint MarkPos   ;
	TFPoint MarkStrPos;
	TFPoint MarkEndPos;
	TFPoint MarkPos1  ;
	TFPoint MarkPos2  ;
	float   MarkAng   ; 

	int FStartLine    ;
	int FEndLine      ;

	//CoreRoi
	int LTCornerWidth ;
	int LTCornerHeight;
	int LBCornerWidth ;
	int LBCornerHeight;

	//CornerROI Offset HSKIM
	int LTCornerWidthOffset ;
	int LTCornerHeightOffset;
	int LBCornerWidthOffset ;
	int LBCornerHeightOffset;

	// Log기록시 분류 목적(분석)
	int    CellId      ;
	int    CamId       ;
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	//-------------------------------------------------------------------
	// Result(Dx,Dy,Round,Diameter,Center) 및 치수,연마,가공
	//-------------------------------------------------------------------
	// Circle Dx, Dy, Round Data, Grind_Width, Grind_Measure 
	double LTCirDx      ;
	double LTCirDy      ;
	double LTCirRound   ;
	double LBCirDx      ;
	double LBCirDy      ;
	double LBCirRound   ;

	// 셀 패턴기준 연마,가공 측정 데이터 
	double LeftGrindWidth ;
	double LeftGrindMea   ;

	// Corner Chamber_Cut 3Point기준 연마,가공 측정시 데이터 
	double LTCorGrindWd ;
	double LTCorGrindMea;
	double LBCorGrindWd ;
	double LBCorGrindMea;

	//Circle Center Point
	// 교차점 기준 CenterPoint 
	TFPoint LTCirCenter;
	TFPoint LBCirCenter;

	// Real Circle Center Point 
	// Euresys Circle Fitting Center 
	////////////////////////////////////////////////
	TFPoint LTRealCirCenter;
	TFPoint LBRealCirCenter;
	////////////////////////////////////////////////

	// FAVION Circle Fitting Center 
	///////////////////////////////////////////////
	TFPoint LTCircleFitCenter;
	TFPoint LBCircleFitCenter;
	//All Grind 연마시 필요한 데이터(비선형 곡선을 찾기때문에 센터값이 없다)
	TFPoint AllGrindCriFitCenter;
	////////////////////////////////////////////////

	// 반원형 박리 및 꽃박리 구별목적 Diameter
	double LTCrackCirDiameter;
	double LBCrackCirDiameter;
	//  GOO 20180909
	// 	double CrackCirDiameter3 ;
	// 	double CrackCirDiameter4 ;
	// 	double CrackCirDiameter5 ;
	// 	double CrackCirDiameter6 ;
	// Euresys Circle Fitting Diameter 
	double LTCirDiameter;
	double LBCirDiameter;

	// FAVION Circle Fitting Diameter
	double LTCirFitDiameter;
	double LBCirFitDiameter;
	//////////////////////////////////////////////

	// 개별 Circle Spec Width, Height 
	// A면 Circle Dx, Dy 
	double LTSideCircleWidth  ;
	double LTSideCircleHeight ;

	// C면 Circle Dx, Dy 
	double LBSideCircleWidth  ;
	double LBSideCircleHeight ;
	
	// A면 Align Mark에서 GrindMark까지 거리 편차값 
	// PatternFinder ROI영역 자동 설정 목적 
	double AMarkToGrindMarkDvX;
	double AMarkToGrindMarkDvY;

	// C면 교차점 기준 GrindMakr까지 거리 편차값 
	// PatternFinder ROI영역 자동 설정 목적 
	double CMarkToGrindMarkDvX;
	double CMarkToGrindMarkDvY;

	//  GOO 20180909
	// 	double CenterCircleWidth ;
	// 	double CenterCircleHeight;

	// 연마 각도 (Circle)
// 	int SideCirDeg   ;
// 	int CenterCirDeg ;

	// TrackLine 기준 Width, Height 
	//  GOO 20180909
	// 	double TrackCircleWidth ;
	// 	double TrackCircleHeight;

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

	// TrackLine Data OverLap 
// 	int AvgLeftTrackLineDis    ;
// 	int AvgRightTrackLineDis   ;
// 	int AvgLeftBtmTrackLineDis ;
// 	int AvgRightBtmTrackLineDis;
// 	int LeftTrackLineCount     ;
// 	int RightTrackLineCount    ;
// 	int TotalLeftTrackDis      ;
// 	int TotalRightTrackDis     ;

	// Cell Size측정 결과 데이터(Notch_Cell)
	double LeftCellSizeHeight  ;  
// 	double CenCellSizeHeight   ;  
	double RightCellSizeHeight ;  

	double CenCellSizeWidth    ;
	double NotchCellSizeWidth  ;
	double NotchCellSizeHeight ;

	double LeftMarkToTopHeight ; //// Cell Mark To TopSide    ==>M1
	double CenMarkToTopHeight  ; //// Cell Mark To TopSide    ==>M2
	double RightMarkToTopHeight; //// Cell Mark To TopSide    ==>M3

	double LeftGrindToBtmHeight;
	double CenGrindToBtmHeight ;
	double RightGrindBtmHeight ;

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

	// 외곽라인에서 AlignMark까지 거리  
	double LDistance; 
	double RDistance; 
	double TDistance;
	double BDistance;

	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// Functions - Execute
	// ---------------------------------------------------------------------
	int OnExecute();
	int OnExecute(TShape Shape);
	int OnExecute(bool l, bool t, bool r, bool b, bool lt, bool rt, bool rb, bool lb);
	int GetSize(TCorner *lpCorner,TBlob *lpDefect, int Count, int Dv);
	int GetCircleSize     (TCircle *lpCircle,TBlob *lpDefect, int Count, int Dv );
	int GetPolygonSize    (TFPoint *PolygonData, TBlob *lpDefect,int TotalCount,int Range, int DefectDv, int PreDefectCnt, int ThresHold);
	// 	int GetCircleCrackSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dis);
	int GetCircleCrackSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dis, double CirDiameter, TFPoint Center, int PreCrackCnt, bool IsInCircle=true);
	int GetAvgCellSize(int TopCount, int MidCount, int BtmCount, int AvgMode); // AvgMode==0, TopAvg, AvgMode==1, MidAvg, AvgMode==2, BtmAvg, AvgMode==4, AllAvg
	int GetCellSize   (int Count                                            ); 
	void OnClearData();
	CRect OnMerge(CRect ar, CRect br);
	int OnAutoBevelIn();
	float GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);
	void OnLoad(bool IsLoad, CString Folder);
	void OnExecuteGrind(TFPoint StaX, TFPoint StaY, int InspMode);
// 	void OnExecuteGrind(TFPoint StaX, TFPoint StaY);

	//-------------------------------------------------------------------
	// 불필요 데이터 확인해야함 
	//-------------------------------------------------------------------
	TFPoint FCellPoint [MAX_DEFECT_POINT]; // IsMarkLine CellSize 계산시 필요한 좌표값 

	CFM3xLineDefect();
	~CFM3xLineDefect();

	//---------------------------------------------------------------------
	// Functions - Set
	// ---------------------------------------------------------------------
	// Set 함수 
	void SetProcessMode    (TProcessMode ProcessMode); 
	void SetInspectMode    (TInspectMode InspectMode); 
	void SetShape          (TShape Shape)            ;
	void SetLineScanType   (int LineScanType        );
	void SetGlassType      (TGlassType GlassType    );

	// AlignMark Y값기준 편차 적용하여 ROI영역 보정 
	void SetCrossLeftPoint  (int x, int y)           ;  
	void SetCrossRightPoint (int x, int y)           ;  

	// 검사 Range 설정 
	void SetSpecWidth       (float MinWidth , float MaxWidth );
	void SetSpecHeight      (float MinHeight, float MaxHeight);

	//---------------------------------------------------------------------
	// Functions - Get
	// ---------------------------------------------------------------------
	// Get 함수 
	CFind*  GetFindPointer(int n);


	// Corner 연마 여부 결정 
	// 	void SetCornerTop(bool IsEnable);
	// 	void SetCornerBtm(bool IsEnable);

	// Corner 연마 여부 결정 
	// 	void SetRectTop(bool IsEnable);
	// 	void SetRectBtm(bool IsEnable);

	// Circle 연마 여부 결정 
	// 	void SetCircleTop    (bool IsEnable);
	// 	void SetCircleBtm    (bool IsEnable);

	// NarrowTrench 선택 결정 
	// 	void SetNarrowTrench (bool IsEnable);

	// EdgeInspection
	// 	void SetBrokenThresHold  (int ThHold)  ;
	//-----------------------------------------------------------------------------
};

