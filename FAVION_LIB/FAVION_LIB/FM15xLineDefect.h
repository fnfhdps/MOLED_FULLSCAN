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
#include "Open_eVision_1_2.h"
#include "clipper.h"

using namespace Euresys::Open_eVision_1_2;
// using namespace The;

class AFX_EXT_CLASS CFM15xLineDefect : public CFilter { // CNC_6HEAD New Version(NOTCH)

private :

protected :
	//----------------------------------------------------------------------
	// Architecture
	// ---------------------------------------------------------------------
	CSingleLine  *SingleLine;
	CRectEdge    *RectEdge  ;
	CEdge        *Edge      ;
	CCircleFit   *CircleFit ;
	CPolygon     *Polygon   ;
	CFind        *Find1     ;
	
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
	struct TGrindMark { CRect Rect; TFPoint Point; TFPoint RectPoint; int MarkID; int Range; double Gradient; double Score; double degree; int DvX; int DvY; double GrindWidth; double GrindMea;};
	//EnCap ASide Pattern Distance Struct 
	struct TPatternDis{double Dis;};
	struct TPolygonROI{int DvX; int DvY;int Width; int Height;};
	struct TGrindLine { TPoint LinePoint; bool IsGrindLine;};

	// ---------------------------------------------------------------------
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
	int FShape          ;
	int FProcessMode    ;
	int FInspectMode    ;
	int FCutMode        ;

	// ---------------------------------------------------------------------
	// Euresys Data 
	// ---------------------------------------------------------------------
	// Euresys Class
	EImageBW8 BinaryImg             ;
	EImageBW8 DefectImg             ;
	EImageBW8 BMLineImg             ;
	EImageBW8 RotateImg             ;
	EImageBW8 RotateBinaryImg       ;
	EImageBW8 RotateGrindBinaryImg  ;
	EImageBW8 RotateGrindInBinaryImg;
	EImageBW8 RotateBrokenImg       ;
	EImageBW8 BrightDefectImg       ;
	EImageBW8 MaskDarkDefectImg     ;

	// ---------------------------------------------------------------------
public    :
	//----------------------------------------------------------------------
	// Architecture
	// ---------------------------------------------------------------------
	CTheLogger   LogUnit;
	CTimer       Time   ;
	CFind        MarkFind[MAX_GRINDMARK_ID];
	//----------------------------------------------------------------------
	// Define
	//----------------------------------------------------------------------
	enum TShape       { shRectH      =0, shRectS        , shRectC                            };
	enum TProcessMode { pmAutoBevelIn=0, pmManualBevelIn, pmAutoOutLine                      };
	enum TInspectMode { Insp_Track   =0,Insp_Mark       , Insp_Polygon ,Insp_Surface         }; 
	enum TSideDefine  { ASide        =0, BSide          , CSide        , DSide               };
	enum TASideDefine { PadROI1     = 0, PadROI2        , PadROI3      , PadROI4    , PadROI5};
	enum TCSideDefine { NotROI1     = 0, NotROI2        , NotROI3      , NotROI4    , NotROI5};
	enum TCamDefine   { Cam_Btm     = 0, Cam_Top                                             };
	enum TDfRoiDefine { Left_Roi    = 0, TopBtm_Roi     , Right_Roi                          };

	//----------------------------------------------------------------------
	// Struct 
	//----------------------------------------------------------------------
	struct TLine          { TFPoint FP0; TFPoint FP1; } Line          [20]           ; // 기본라인 
	struct TLeftOutLine   { TFPoint FP0; TFPoint FP1; } LeftOutLine   [1]            ;
	struct TLeftInLine    { TFPoint FP0; TFPoint FP1; } LeftInLine    [1]            ;
	struct TRightOutLine  { TFPoint FP0; TFPoint FP1; } RightOutLine  [1]            ;
	struct TRightInLine   { TFPoint FP0; TFPoint FP1; } RightInLine   [1]            ;
	struct TBtmOutLine    { TFPoint FP0; TFPoint FP1; } BtmOutLine    [MAX_BTM_COUNT];
	struct TBtmInLine     { TFPoint FP0; TFPoint FP1; } BtmInLine     [MAX_BTM_COUNT];
	struct TTopOutLine    { TFPoint FP0; TFPoint FP1; } TopOutLine    [1]            ;
	struct TTopInLine     { TFPoint FP0; TFPoint FP1; } TopInLine     [1]            ;

	struct TPatternLine   { TFPoint FP0; TFPoint FP1; } PatternLine   [1]            ;

	// InLine or OutLine Data가 없을경우 PreInLine,PreOutLine 데이터 적용함 
	struct TPreLeftOutLine  { TFPoint FP0; TFPoint FP1; } PreLeftOutLine [1]         ;
	struct TPreLeftInLine   { TFPoint FP0; TFPoint FP1; } PreLeftInLine  [1]         ;
	struct TPreRightOutLine { TFPoint FP0; TFPoint FP1; } PreRightOutLine[1]         ;
	struct TPreRightInLine  { TFPoint FP0; TFPoint FP1; } PreRightInLine [1]         ;

	//----------------------------------------------------------------------
	//Defect Count 
	// ---------------------------------------------------------------------
	// 개별Side Dark_Defect Count 
	int DefectPointCount; // 전체 불량 포인트 개수
	int ACount          ; // A_SIDE  불량 포인트 개수
	int BCount          ; // B_SIDE  불량 포인트 개수
	int CCount          ; // C_SIDE  불량 포인트 개수
	int DCount          ; // D_SIDE  불량 포인트 개수

	// 개별Side Bright_Defect Count
	int ACrackCount     ;
	int BCrackCount     ;
	int CCrackCount     ;
	int DCrackCount     ;

	// Corner Dark_Defect Count 
	int C1Count         ; // C1_SIDE 불량 포인트 개수
	int C2Count         ; // C2_SIDE 불량 포인트 개수
	int C3Count         ; // C3_SIDE 불량 포인트 개수
	int C4Count         ; // C4_SIDE 불량 포인트 개수

	// 개별 Circle Dark_Defect Count 
	int A1CirDeftectCnt ;
	int A2CirDeftectCnt ;
	int C1CirDeftectCnt ;
	int C2CirDeftectCnt ;

	// 개별 Circle Bright_Defect Count 
	int A1CirCrackCnt  ;
	int A2CirCrackCnt  ;
	int C1CirCrackCnt  ;
	int C2CirCrackCnt  ;

	// 전체연마 Dark_Defect Count
	int LeftDefectCount ;
	int RIghtDefectCount;
	int TopDefectCount  ;
	int BtmDefectCount  ;

	// 전체연마 Bright_Defect Count
	int LeftCrackCount ;
	int RIghtCrackCount;
	int TopCrackCount  ;
	int BtmCrackCount  ;
	// ---------------------------------------------------------------------
	// Defect Data  
	// ---------------------------------------------------------------------
	// 개별Side Defect(Dark)
	TBlob ADefectPoint    [MAX_DEFECT_POINT];
	TBlob BDefectPoint    [MAX_DEFECT_POINT];
	TBlob CDefectPoint    [MAX_DEFECT_POINT];
	TBlob DDefectPoint    [MAX_DEFECT_POINT];

	// 개별 Side Defect(Bright)
	TBlob ACrackPoint     [MAX_DEFECT_POINT];
	TBlob BCrackPoint     [MAX_DEFECT_POINT];
	TBlob CCrackPoint     [MAX_DEFECT_POINT];
	TBlob DCrackPoint     [MAX_DEFECT_POINT];

	// Corner Defect 
	TBlob C1DefectPoint   [MAX_DEFECT_POINT];
	TBlob C2DefectPoint   [MAX_DEFECT_POINT];
	TBlob C3DefectPoint   [MAX_DEFECT_POINT];
	TBlob C4DefectPoint   [MAX_DEFECT_POINT];

	//부분연마 
	///////////////////////////////////////////////
	// Dark Circle Defect 
	TBlob A1CirDefectPoint[MAX_DEFECT_POINT];
	TBlob A2CirDefectPoint[MAX_DEFECT_POINT];
	TBlob C1CirDefectPoint[MAX_DEFECT_POINT];
	TBlob C2CirDefectPoint[MAX_DEFECT_POINT];

	// Bright Circle Defect 
	TBlob A1CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob A2CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob C1CirCrackPoint  [MAX_DEFECT_POINT];
	TBlob C2CirCrackPoint  [MAX_DEFECT_POINT];
	///////////////////////////////////////////////

	//전체연마시
	///////////////////////////////////////////////
	//Dark_Defect  
	TBlob LeftDefectPoint  [MAX_DEFECT_POINT];
	TBlob TopDefectPoint   [MAX_DEFECT_POINT];
	TBlob RightDefectPoint [MAX_DEFECT_POINT];
	TBlob BtmDefectPoint   [MAX_DEFECT_POINT];

	//Bright_Defect  
	TBlob LeftCrackPoint   [MAX_DEFECT_POINT];
	TBlob TopCrackPoint    [MAX_DEFECT_POINT];
	TBlob RightCrackPoint  [MAX_DEFECT_POINT];
	TBlob BtmCrackPoint    [MAX_DEFECT_POINT];
	///////////////////////////////////////////////

	// Polygon Defect Data 
	TBadRect BadRect[MAX_POLYGON_DEFECT_COUNT];

	// ---------------------------------------------------------------------
	// Compare Corner Data(검사를 위한 중간 데이터) 
	// ---------------------------------------------------------------------
	TCorner C1Corner[MAX_CORNER_POINT];
	TCorner C2Corner[MAX_CORNER_POINT];
	TCorner C3Corner[MAX_CORNER_POINT];
	TCorner C4Corner[MAX_CORNER_POINT];

	//Pad 
	TCircle A1CircleData [MAX_CIRCLE_EDGEPOINT];
	TCircle A2CircleData [MAX_CIRCLE_EDGEPOINT];

	//Notch 
	TCircle C1CircleData [MAX_CIRCLE_EDGEPOINT];
	TCircle C2CircleData [MAX_CIRCLE_EDGEPOINT];

	TCircle TopCircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle BtmCircleData[MAX_CIRCLE_EDGEPOINT];

	// Total 연마 In/Out Data 
	TFPoint AllLeftInData  [MAX_ALL_EDGE_COUNT];
	TFPoint AllLeftOutData [MAX_ALL_EDGE_COUNT];
	TFPoint AllTopInData   [MAX_ALL_EDGE_COUNT];
	TFPoint AllTopOutData  [MAX_ALL_EDGE_COUNT];
	TFPoint AllRightInData [MAX_ALL_EDGE_COUNT];
	TFPoint AllRightOutData[MAX_ALL_EDGE_COUNT];
	TFPoint AllBtmInData   [MAX_ALL_EDGE_COUNT];
	TFPoint AllBtmOutData  [MAX_ALL_EDGE_COUNT];
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// EdgePoint Data Memory(동적생성)
	// ---------------------------------------------------------------------
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

	// Corner Calculate 
	TFPoint CornerP1;
	TFPoint CornerP2;

	// Circle In Coordinate Data
	TFPoint *A1CirInPoint;
	TFPoint *A2CirInPoint;
	// 	TFPoint *A1PolyInPoint;

	TFPoint *C1CirInPoint;
	TFPoint *C2CirInPoint;

	// Circle Out Coordinate Data
	TFPoint *A1CirOutPoint;
	TFPoint *A2CirOutPoint;

	TFPoint *C1CirOutPoint;
	TFPoint *C2CirOutPoint;

	// Circle(Grind_Measure) Stand Edge Coordinate Data 
	TFPoint *C1CirStandPoint;
	TFPoint *C2CirStandPoint;

	// AllGrind경우 개별 Frame기준 데이터 취합
	TFPoint *LeftInPoint  ;
	TFPoint *LeftOutPoint ;
	TFPoint *TopInPoint   ;
	TFPoint *TopOutPoint  ;
	TFPoint *RightInPoint ;
	TFPoint *RightOutPoint;
	TFPoint *BtmInPoint   ;
	TFPoint *BtmOutPoint  ;

	// NarrowTrench 직선영역 영상밝기차이 
	TGrindLine *GrindLine;

	// Masking test 
	// 전체연마 기준 
	POINT *MaskPoint;
	POINT *MaskPoint2;

	//Left Grind 
	POINT *LeftMaskPoint;
	POINT *LeftMaskPoint2;

	//Right Grind 
	POINT *RightMaskPoint;
	POINT *RightMaskPoint2;

	// ---------------------------------------------------------------------

	// ---------------------------------------------------------------------
	// EdgePoint Data Count Memory(동적생성 Count정보)
	// ---------------------------------------------------------------------
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

	// Circle In_Edge Point Count 
	int A1CirInEdgeCount;
	int A2CirInEdgeCount;
	// 	int A1PolyInEdgeCount;

	int C1CirInEdgeCount;
	int C2CirInEdgeCount;

	// Circle Out_Edge Point Count 
	int A1CirOutEdgeCount;
	int A2CirOutEdgeCount;

	int C1CirOutEdgeCount;
	int C2CirOutEdgeCount;

	// 박리불량 검출전 Edge Point Count
	int A1CirInCrackCount;
	int A2CirInCrackCount;

	int C1CirInCrackCount;
	int C2CirInCrackCount;

	//박리불량 검출전 Edge Point Count 
	int LeftInCrackCount ;
	int RIghtInCrackCount;
	int TopInCrackCount  ;
	int BtmInCrackCount  ;

	// 전체 연마시 필요한 전체 Count정보 
	int AllLeftInEdgeCount  ;
	int AllLeftOutEdgeCount ;
	int AllRightInEdgeCount ;
	int AllRightOutEdgeCount;
	int AllTopInEdgeCount   ;
	int AllTopOutEdgeCount  ;
	int AllBtmInEdgeCount   ;
	int AllBtmOutEdgeCount  ;

	// 전체 연마시 필요한 개별 Count정보 
	int LeftInEdgeCount     ;
	int LeftOutEdgeCount    ;
	int RightInEdgeCount    ;
	int RightOutEdgeCount   ;
	int TopInEdgeCount      ;
	int TopOutEdgeCount     ;
	int BtmInEdgeCount      ;
	int BtmOutEdgeCount     ;

	// NarrowTrench 직선영역 영상밝기차이 
	int GrindLineCount;

	// 20190720
	// Masking Image Test 
	int AllMaskCount1;
	int AllMaskCount2;
	int AllLeftMaskCount1;
	int AllLeftMaskCount2;
	int AllRightMaskCount1;
	int AllRightMaskCount2;

	// ---------------------------------------------------------------------

	// ---------------------------------------------------------------------
	// GrindMark 관련 데이터 
	// ---------------------------------------------------------------------
	TGrindMark  AGrindMark [MAX_GRINDMARK_COUNT ];
	TGrindMark  BGrindMark [MAX_GRINDMARK_COUNT ];
	TGrindMark  CGrindMark [MAX_GRINDMARK_COUNT ];
	TGrindMark  DGrindMark [MAX_GRINDMARK_COUNT ];
	TGrindMark  H1GrindMark[MAX_HOLE_GRIND_COUNT];
	TGrindMark  H2GrindMark[MAX_HOLE_GRIND_COUNT];

	// EnCap ASide Pattern Distance 
	TPatternDis PatternDis[MAX_GRINDMARK_COUNT];

	int nASideRoiCenterLength;
	int nCSideRoiCenterLength;

	//ROI_Rect(단순 Display 목적)
// 	CRect       NotchPolyRect[5];
	CRect       NotchPolyRect[MAX_CUT_GRIND_ROI   ];
	CRect       PadPolyRect  [MAX_GRIND_ROI_COUNT ];
	CRect       DefectRoiRect[MAX_DEFECT_ROI_COUNT];

	// 직선영역 검사목적 교차점 포인트 
	TPoint BtmEdgePoint[MAX_BTM_LINE_POINT];
	TPoint TopEdgePoint[MAX_TOP_LINE_POINT];

	bool IsLeftAllGrind ;
	bool IsTopAllGrind  ;
	bool IsRightAllGrind;
	bool IsBtmAllGrind  ;
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
    // ThresHold 
	// ---------------------------------------------------------------------
	// Defect ThresHold 
	int TopDarkThresHold  ; // Top Defect Find
	int LeftDarkThresHold ; // 20206004 HSKIM
	int RightDarkThresHold; // 20206004 HSKIM
	int BtmDarkThresHold  ; // Bot Defect Find
	int BrokenThresHold    ; // Bright Defect Find 

	// 외곽라인 찾는 기본 ThresHold 
	int LeftEdgeThresHold  ;
	int TopEdgeThresHold   ;
	int RightEdgeThresHold ;
	int BtmEdgeThresHold   ;

    bool LeftEdgeBTWhite   ; //Black To White Edge 검출 
	bool TopEdgeBTWhite    ; //Black To White Edge 검출 
	bool RightEdgeBTWhite  ; //Black To White Edge 검출 
	bool BtmEdgeBTWhite    ; //Black To White Edge 검출 

	// 연마라인 찾는 ThresHold
	int PadSideThresHold   ; // Corner,Pad Circle(A1,A2) ThresHold 
	int PadCenThresHold    ; // Pad 직선부(CAI) 연마라인 ThresHold
	int NotchSideThresHold ; // Notch Side 연마라인 ThresHold 
	int NotchCenThresHold  ; // Notch Center 연마라인 ThresHold 
	int BSideGrindThresHold; // B면 연마 라인 20221014
	int DSideGrindThresHold; // D면 연마 라인 20221014

	// Chamber Cut Corner Grind Search ThresHold 
	int CorGrindOutThresHold; // Corner 바깥쪽 검출 ThresHold 
	int CorGrindInThresHold ; // Corner 안쪽   검출 ThresHold 
	int CorGrindMeaThresHold; // Corner 3Point 검출시 사용되는 ThresHold

	// Cutting라인 찾는 ThresHold 
	int CutThresHold   ;

	int PatternThresHold    ; // Pattern Distance 안쪽라인 검출 ThresHold 
	int PatternOffSet       ; // Pattern Distance 안쪽라인 검출 OffSet 

	// White Masking DarkDefect ThresHold  
	int LeftMaskDarkDefectTh ;
	int TopMaskDarkDefectTh  ;
	int RightMaskDarkDefectTh;
	int BtmMaskDarkDefectTh  ;

	// Black Masking BirhtDefect ThresHold  
	int LeftMaskBrightDefectTh ;
	int TopMaskBrightDefectTh  ;
	int RightMaskBrightDefectTh;
	int BtmMaskBrightDefectTh  ;

	// 영역별 Mask Offset 
	int LeftMaskOffset ;
	int TopMaskOffset  ;
	int RightMaskOffset;
	int BtmMaskOffset  ;

	// 영역별 Mask Distance 
	int LeftMaskDis ;
	int TopMaskDis  ;
	int RightMaskDis;
	int BtmMaskDis  ;

	//연마 안쪽라인 Pos Count
	int LeftGrindInCnt ;
	int TopGrindInCnt  ;
	int RightGrindInCnt;
	int BtmGrindInCnt  ;
	//연마 안쪽라인 Pos Data 
	int LeftGrindInPos ;
	int TopGrindInPos  ;
	int RightGrindInPos;
	int BtmGrindInPos  ;
	//연마 직선구간 Defect 검사목적 Range 
	int LeftDefectRange ;
	int TopDefectRange  ;
	int RightDefectRange;
	int BtmDefectRange  ;

	// ---------------------------------------------------------------------
	// EdgeLine 교차점 Data  
	// ---------------------------------------------------------------------
	// TrakLine 생성 방법 
	// 실측 Edge검출 외곽라인 데이터 
	TFPoint FLTPoint; //좌측라인과 상단라인의 교차점 좌표
	TFPoint FRTPoint; //우측라인과 상단라인의 교차점 좌표
	TFPoint FRBPoint; //우측라인과 하단라인의 교차점 좌표
	TFPoint FLBPoint; //좌측라인과 하단라인의 교차점 좌표

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

	//각도 계산시 필요한 기준 Mark좌표 
	TFPoint AlignMark1;
	TFPoint AlignMark2;

	// ---------------------------------------------------------------------
	// Cell Information && 기타 
	// ---------------------------------------------------------------------
	// Mark 기준으로 영역 재설정 및 Corner Size 검사 
	int CELL_A;
	int CELL_B;
	int CELL_C;
	int CELL_D;
	int CELL_E;
	int CELL_F;
	int CELL_G;
	int CELL_H;
	// Corner Dx, Dy 데이터 
	double LTDx; 
	double LTDy; 
	double RTDx;
	double RTDy;
	double RBDx;
	double RBDy;
	double LBDx;
	double LBDy;

	// Match 파일 경로 
	CString FindDir ;
	CString MatchDir;
	CString AlignDir;

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
	float LeftLineData ;
	float RightLineData;

	// Inspection ROI영역(직선영역 검사시 필요함)
	int FLeftWidth     ;
	int FRightWidth    ;
	int FTopHeight     ;
	int FBtmHeight     ;

	// Image Rotate Degree
	float ImgDegree;
	// GrindMark 영역설정시 필요한 기준각도 데이터 
	double dASideDeg; //기준 각도 - Glass각도
	double dBSideDeg; //Glass각도 - 기준 각도 
	double dCSideDeg; //기준 각도 - Glass각도
	double dDSideDeg; //Glass각도 - 기준 각도 

	// Display 목적 회전 중심 좌표 
	TFPoint CenterPoint;
	// Pixel 내부 데이터 취합시 필요한 Buffer 
	void * IAddress;

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
	//int ImageWidth ;

	// ImageHeightEnd
	int ImageHEnd  ;

	// Mark Rotate Degree 
	float MarkDegree;

	// ROI영역 범위 제한 목적 
	int BtmLimitLine  ;
	int TopLimitLine  ;

	TFPoint MarkPos   ;
	TFPoint MarkStrPos;
	TFPoint MarkEndPos;
	TFPoint MarkPos1  ;
	TFPoint MarkPos2  ;
	float   MarkAng   ; 

	int FStartLine    ;
	int FEndLine      ;


	//CoreRoi
	int CornerWidth   ;
	int CornerHeight  ;

	//4개 교차점 Corner ROI 
	int LTRectWidth ;
	int LTRectHeight;
	int RTRectWidth ;
	int RTRectHeight;
	int LBRectWidth ;
	int LBRectHeight;
	int RBRectWidth ;
	int RBRectHeight;

	//4개 교차점 Corner ROI 
	int LTCornerWidth ;
	int LTCornerHeight;
	int RTCornerWidth ;
	int RTCornerHeight;
	int LBCornerWidth ;
	int LBCornerHeight;
	int RBCornerWidth ;
	int RBCornerHeight;

	//4개 교차점 Circle ROI 
	int LTCircleWidth ;
	int LTCircleHeight;
	int RTCircleWidth ;
	int RTCircleHeight;
	int LBCircleWidth ;
	int LBCircleHeight;
	int RBCircleWidth ;
	int RBCircleHeight;

	// Log기록시 분류 목적(분석)
	int    CellId      ;
	int    CamId       ;

	//-------------------------------------------------------------------
	// Result(Dx,Dy,Round,Diameter,Center) 및 치수,연마,가공
	//-------------------------------------------------------------------
	// Corner Grind_Width, Grind_Measure 
	double C1CorGrindWd  ;
	double C1CorGrindMea ;
	double C2CorGrindWd  ;
	double C2CorGrindMea ;
	double C3CorGrindWd  ;
	double C3CorGrindMea ;
	double C4CorGrindWd  ;
	double C4CorGrindMea ;

	// FAVION Circle Fitting Center 
	///////////////////////////////////////////////
	TFPoint ACircleFit1Center;
	TFPoint ACircleFit2Center;
	TFPoint CircleFit1Center;
	TFPoint CircleFit2Center;
	///////////////////////////////////////////////

	// FAVION Circle Fitting Diameter
	double ACirFitDiameter1;
	double ACirFitDiameter2;
	double CirFitDiameter1 ;
	double CirFitDiameter2 ;
	//////////////////////////////////////////////
	// 개별 Circle Spec Width, Height 
	// A면 Circle Dx, Dy 
	double ASideCircleWidth  ;
	double ASideCircleHeight ;

	// C면 Circle Dx, Dy 
	double CSideCircleWidth  ;
	double CSideCircleHeight ;
	double CenterCircleWidth ;
	double CenterCircleHeight;

	// 연마 각도 (Circle)
	int SideCirDeg   ;
	int CenterCirDeg ;

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

	// 외곽라인에서 AlignMark까지 거리  
	double LDistance; 
	double RDistance; 
	double TDistance;
	double BDistance;

	// ---------------------------------------------------------------------
	// Functions - Execute
	// ---------------------------------------------------------------------
	int OnExecute();
	int OnExecute(TShape Shape);
	int OnExecute(bool l, bool t, bool r, bool b, bool lt, bool rt, bool rb, bool lb);
	int GetSize(TCorner *lpCorner,TBlob *lpDefect, int Count, int Dv, int ThresHold);
	int GetCircleSize     (TCircle *lpCircle,TBlob *lpDefect, int Count, int Dv );
	int GetPolygonSize    (TFPoint *PolygonData, TBlob *lpDefect,int TotalCount,int Range, int DefectDv, int ThresHold, int PreDefectCnt=0);
	// 	int GetCircleCrackSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dis);
	int GetCircleCrackSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dis, double CirDiameter, TFPoint Center, int PreDefectCnt=0, bool IsInCircle=true);
	int GetAvgCellSize(int TopCount, int MidCount, int BtmCount, int AvgMode); // AvgMode==0, TopAvg, AvgMode==1, MidAvg, AvgMode==2, BtmAvg, AvgMode==4, AllAvg
	int GetCellSize   (int Count                                            ); 
	void OnClearData();
	CRect OnMerge(CRect ar, CRect br);
	int OnAutoBevelIn();
	float GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);
	void OnLoad(bool IsLoad, CString Folder);
	void OnExecuteGrind(TFPoint StaPos, TFPoint EndPos, int nSide);

	//-------------------------------------------------------------------
    // 불필요 데이터 확인해야함 
	//-------------------------------------------------------------------
	TFPoint FCellPoint [MAX_DEFECT_POINT]; // IsMarkLine CellSize 계산시 필요한 좌표값 

	CFM15xLineDefect();
	~CFM15xLineDefect();

	//---------------------------------------------------------------------
	// Functions - Set
	// ---------------------------------------------------------------------
	// Set 함수 
	void SetProcessMode    (TProcessMode ProcessMode); 
	void SetInspectMode    (TInspectMode InspectMode); 
	void SetShape          (TShape Shape)            ;

	// AlignMark Y값기준 편차 적용하여 ROI영역 보정 
	void SetCrossLeftPoint  (int x, int y)           ;  
	void SetCrossRightPoint (int x, int y)           ;  

	// 검사 Range 설정 
	void SetSpecWidth       (float MinWidth , float MaxWidth );
	void SetSpecHeight      (float MinHeight, float MaxHeight);

	//---------------------------------------------------------------------
	// Functions - Get
	// ---------------------------------------------------------------------
	CFind* GetFindPointer();

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


