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
#include "clipper.h"

using namespace Euresys::Open_eVision_1_2;
// using namespace The;

class AFX_EXT_CLASS CFM14xLineDefect : public CFilter { //CELL 계측기(LineScan4k X 4)

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
	// PrintMark 
	struct TGrindMark {CRect Rect; TFPoint Point; TFPoint RectPoint; int MarkID; double Range; double Gradient; double Score; double degree; int DvX; int DvY;double PrintEdge; double PrintShift;};
	struct TPatternDis{double Dis;};
	struct TPolygonROI{int StaX; int StaY; int Width; int Height;};
	struct TGrindLine { TPoint LinePoint; bool IsGrindLine;};
	struct TRealLine  { TPoint StaPos   ;};
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
	// Match 파일 경로 
	CString FindDir  ;

	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// Euresys Data 
	// ---------------------------------------------------------------------
	// Euresys Class
	EImageBW8 BinaryImg             ; // 라인을 찾기 위한 이미지 
	EImageBW8 DefectImg             ; // Defect검사 목적 이미지 
	EImageBW8 BrightDefectImg       ; // BrightDefect검사 목적 이미지
	EImageBW8 MaskDarkDefectImg     ; // Making DarkDefect  검사 목적 이미지 
	EImageBW8 MaskBrightDefectImg   ; // Making BrightDefect검사 목적 이미지 

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
	enum TShape       { shRectH     =0, shRectC        , shRectS                     };
	enum TProcessMode { pmAutoBevelIn , pmManualBevelIn, pmAutoOutLine               };
	enum TInspectMode { Insp_Track  =0, Insp_Mark      , Insp_Polygon  ,Insp_Surface }; 
	enum TCameraMode  { Left_Btm_Cam=0, Left_Top_Cam   , Right_Btm_Cam ,Right_Top_Cam};
	enum TCellSahpe   { Rect_Shape  =0, Corner_Shape   , Circle_Shape                };

	//----------------------------------------------------------------------
	// Struct 
	//----------------------------------------------------------------------
	struct TLine          { TFPoint FP0; TFPoint FP1; } Line          [20] ; // 기본라인 
	struct TLeftOutLine   { TFPoint FP0; TFPoint FP1; } LeftOutLine   [1 ] ; // 개별라인 
	struct TBtmOutLine    { TFPoint FP0; TFPoint FP1; } BtmOutLine    [1 ] ; // 개별라인 
	struct TTopOutLine    { TFPoint FP0; TFPoint FP1; } TopOutLine    [1 ] ; // 개별라인 
	struct TRealOutLine   { TFPoint FP0; TFPoint FP1; } RealOutLine   [MAX_REAL_FRAME_COUNT] ; //TopCam기준 Real 외곽라인  
	

	// InLine or OutLine Data가 없을경우 PreInLine,PreOutLine 데이터 적용함 
	struct TPreLeftOutLine  { TFPoint FP0; TFPoint FP1; } PreLeftOutLine [1];
	struct TPreLeftInLine   { TFPoint FP0; TFPoint FP1; } PreLeftInLine  [1];
	//----------------------------------------------------------------------
	//Defect Count 
	// ---------------------------------------------------------------------
	int DefectPointCount; // 전체 불량 포인트 개수
	int ACount          ; // A_SIDE Dark   불량 포인트 개수
	int BCount          ; // B_SIDE Dark   불량 포인트 개수
	int CCount          ; // C_SIDE Dark   불량 포인트 개수
	int DCount          ; // D_SIDE Dark   불량 포인트 개수

	int ACrackCount     ; // A_SIDE Bright 불량 포인트 개수
	int BCrackCount     ; // B_SIDE Bright 불량 포인트 개수
	int CCrackCount     ; // C_SIDE Bright 불량 포인트 개수
	int DCrackCount     ; // D_SIDE Bright 불량 포인트 개수

	//Corner Dark Defect
	int LTCorDefectCnt; 
	int LBCorDefectCnt; 

	//Corner Bright Defect
	int LTCorCrackCnt;
	int LBCorCrackCnt;

	//Circle Dark Defect
	int LTCirDeftectCnt ;
	int LBCirDeftectCnt ;

	//Circle White Defect 
	int LTCirCrackCnt  ;
	int LBCirCrackCnt  ;

	// All Etching Dark Defect(Static) 
	int LeftDefectCnt     ;
	int FrameLeftDefectCnt;

	// All Etching Bright Defect(Static) 
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

	// Corner Defect(Bright_Defect)
	TBlob LTCorCrackPoint  [MAX_DEFECT_POINT];
	TBlob LBCorCrackPoint  [MAX_DEFECT_POINT];

	// Dark Circle Defect 
	TBlob LTCirDefectPoint  [MAX_DEFECT_POINT];
	TBlob LBCirDefectPoint  [MAX_DEFECT_POINT];

	// Bright Circle Defect 
	TBlob LTCirCrackPoint  [MAX_DEFECT_POINT];
	TBlob LBCirCrackPoint  [MAX_DEFECT_POINT];

	// All Etching Defect  
	TBlob LeftDefectPoint  [MAX_DEFECT_POINT]; // Left Dark   Defect(Static)
	TBlob LeftCrackPoint   [MAX_DEFECT_POINT]; // Left Bright Defect(Static) 

	// Polygon Defect Data 
	TBadRect BadRect[MAX_POLYGON_DEFECT_COUNT];
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// Data Memory (Real Edge Point Data)
	// ---------------------------------------------------------------------
	// Corner Calculate 
	TFPoint P1;
	TFPoint P2;

	// Corner Edge Point Data(Normal)
	TFPoint *LTCorOutPoint;
	TFPoint *LBCorOutPoint;

	// Total 연마 In/Out Data 
	TFPoint AllCornerOutData[MAX_ALL_EDGE_COUNT];

	// Circle Out Coordinate Data
	TFPoint *LTCirOutPoint;
	TFPoint *LBCirOutPoint;

	// Total 연마 In/Out Data 
	TFPoint AllCircleOutData[MAX_ALL_EDGE_COUNT];

	// All Etching 개별 Frame기준 데이터 취합
	TFPoint *LeftInPoint ;
	TFPoint *LeftOutPoint;

	// All Etching 연마 In/Out Data 
	TFPoint AllLeftInData [MAX_ALL_EDGE_COUNT];
	TFPoint AllLeftOutData[MAX_ALL_EDGE_COUNT];

	// Print Screen 개별 Frame기준 Data 
	TFPoint *PrintScPoint;

	// All Print Screen Data 
	TFPoint AllPrintScData[MAX_ALL_EDGE_COUNT];

	// Masking Pointer 
	POINT *MaskPoint;

	// ---------------------------------------------------------------------
	// Compare Data(중간 데이터)
	// ---------------------------------------------------------------------
	// 검사를 위해서 필요한 중간 데이터 
	TCorner LTCorner[MAX_CORNER_POINT];
	TCorner LBCorner[MAX_CORNER_POINT];
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// PrintMark Data && ROI Daata 
	// ---------------------------------------------------------------------
	//8_Camera 검사시 필요한 PrintMark Data 
	TGrindMark PrintMark    [MAX_PRINT_MARK_COUNT];

	//Display ROI영역 
	CRect  LTROI   ; //LTPoint ChamberCut or CircleCut ROI
	CRect  LBROI   ; //LBPoint ChamberCut or CircleCut ROI
	CRect  PrintROI; //Print Screen ROI

	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------

	// 직선영역 검사목적 교차점 포인트 
// 	TPoint BtmEdgePoint[2];
// 	TPoint TopEdgePoint[2];
	// ---------------------------------------------------------------------
	// Data Count Memory(EdgePoint)
	// ---------------------------------------------------------------------
	// Corner Edge Point Count
	int LTCorOutEdgeCount;
	int LBCorOutEdgeCount;

	// 전체 EdgePoint Count
	int AllCorOutEdgeCount ;

	// Circle Out_Edge Point Count 
	int LTCirOutEdgeCount;
	int LBCirOutEdgeCount;

	// 전체 EdgePoint Count
	int AllCirOutEdgeCount ;

	// Etching 후 Cutting 단차 존재시 필요한 데이터  
	int LeftInEdgeCount     ;
	int LeftOutEdgeCount    ;

	int AllLeftInEdgeCount  ;
	int AllLeftOutEdgeCount ;

	// Print Screen EdgePoint Count 
	int PrintScEdgeCount   ;
	int AllPrintScEdgeCount;

	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------

	// ---------------------------------------------------------------------
	// ThresHold 
	// ---------------------------------------------------------------------
	int CornerThresHold    ; // Chamber Cut Find ThresHold 
	int CircleThresHold    ; // Circle  Cut Find ThredHold 

	// Defect ThresHold 
	int DarkThresHold      ; // Dark        Defect ThresHold
	int BrokenThresHold    ; // Bright      Defect ThresHold 
	int MaskDefectThresHold; // Mask Dark   Defect ThresHold 
	int MaskBrokenThresHold; // Mask Bright Defect ThresHold 

	// Mask Image 생성을 위한 Offset 
	int MaskDefectOffset   ; // Mask Dark Defect Offset  

	// 외곽라인 찾는 기본 ThresHold 
	int LeftEdgeThresHold  ;
	int TopEdgeThresHold   ;
	int RightEdgeThresHold ;
	int BtmEdgeThresHold   ;

    bool LeftEdgeBTWhite ; //Black To White Edge 검출 
	bool TopEdgeBTWhite  ; //Black To White Edge 검출 
	bool RightEdgeBTWhite; //Black To White Edge 검출 
	bool BtmEdgeBTWhite  ; //Black To White Edge 검출 

	//  CutLine ThresHold 
	int LeftCutThresHold   ;
	//  Print Screen ThresHold 
	int LeftPrintThresHold ;

	//  Print Screen 검출 목적 Value  
	int PrintScOffset; //외곽라인 기준 Offset
	int PrintScRange ; //PrintScreen 검축 Range 

	// Real Out Line Count 
	int RealOutCnt;
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

	//각도 계산시 필요한 기준 Mark좌표 
	TFPoint AlignMark1;
	TFPoint AlignMark2;

	// Grind검사시 ROI영역 좌표 
	TPoint PolyP1[1];
	TPoint PolyP2[1];
	TPoint PolyP3[1];
	TPoint PolyP4[1];

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
	bool bCutLine   ; //Etching 후 Cutt Line 존재 유무 0:Cut Line 없음, 1:Cut Line 있음 
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
	int    ScanID      ;
	int    CamId       ;

	//검사 영역 Width 
	int AllCutWidth    ;

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
	double LTCircleWidth ;
	double LTCircleHeight;

	// C면 Circle Dx, Dy 
	double LBCircleWidth  ;
	double LBCircleHeight ;
	
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

	// Cell Size측정 결과 데이터(Notch_Cell)
	double LeftCellSizeHeight  ;  

	double CenCellSizeWidth    ;

	// Cell Size 관련 Value 
	double PrintEdge1;
	double PrintEdge2;
	double PrintEdge3;

	// Cell Size 측정 목적(기준좌표) 
	TFPoint LeftStaPos      ;
	TFPoint LeftEndPos      ;
	TFPoint CenterStaPos    ;
	TFPoint CenterEndPos    ;

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
	void OnExecutePrint(TFPoint StaX, TFPoint StaY, int InspMode, bool bCutLine=false);
// 	void OnExecuteGrind(TFPoint StaX, TFPoint StaY);

	//-------------------------------------------------------------------
	// 불필요 데이터 확인해야함 
	//-------------------------------------------------------------------
	TFPoint FCellPoint [MAX_DEFECT_POINT]; // IsMarkLine CellSize 계산시 필요한 좌표값 

	CFM14xLineDefect();
	~CFM14xLineDefect();

	//---------------------------------------------------------------------
	// Functions - Set
	// ---------------------------------------------------------------------
	// Set 함수 
	void SetProcessMode    (TProcessMode ProcessMode); 
	void SetInspectMode    (TInspectMode InspectMode); 
	void SetShape          (TShape Shape)            ;
	void SetLineScanType   (int LineScanType        );

	// AlignMark Y값기준 편차 적용하여 ROI영역 보정 
	void SetCrossLeftPoint  (int x, int y)           ;  
	void SetCrossRightPoint (int x, int y)           ;  

	// 검사 Range 설정 
	void SetSpecWidth       (float MinWidth , float MaxWidth );
	void SetSpecHeight      (float MinHeight, float MaxHeight);

	//Find 경로 설정 
	void SetPath            (CString Path);

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

