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

class AFX_EXT_CLASS CFM3xLineDefect : public CFilter { //�ű� ������Ʈ(LineScan4k X 8)

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
	CPolygon     *Polygon   ; //��� 
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
	TFPoint FP0, FP1 ; // ������ �Ǵ� ��ǥ
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
	EImageBW8 BinaryImg             ; // ������ ã�� ���� �̹��� 
	EImageBW8 DefectImg             ; // Defect�˻� ���� �̹��� 
	EImageBW8 MaskDarkDefectImg     ; // DarkDefect�˻� ���� �̹��� 
	EImageBW8 BrightDefectImg       ; // BrightDefect�˻� ���� �̹���

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
	struct TLine          { TFPoint FP0; TFPoint FP1; } Line          [20] ; // �⺻���� 
	struct TLeftOutLine   { TFPoint FP0; TFPoint FP1; } LeftOutLine   [1 ] ;
	struct TLeftInLine    { TFPoint FP0; TFPoint FP1; } LeftInLine    [1 ] ;
	struct TLeftStdLine   { TFPoint FP0; TFPoint FP1; } LeftStdLine   [1 ] ;
	struct TBtmOutLine    { TFPoint FP0; TFPoint FP1; } BtmOutLine    [1 ] ;
	struct TTopOutLine    { TFPoint FP0; TFPoint FP1; } TopOutLine    [1 ] ;

	// InLine or OutLine Data�� ������� PreInLine,PreOutLine ������ ������ 
	struct TPreLeftOutLine  { TFPoint FP0; TFPoint FP1; } PreLeftOutLine [1];
	struct TPreLeftInLine   { TFPoint FP0; TFPoint FP1; } PreLeftInLine  [1];
	//----------------------------------------------------------------------
	//Defect Count 
	// ---------------------------------------------------------------------
	int DefectPointCount; // ��ü �ҷ� ����Ʈ ����
	int ACount          ; // A_SIDE  �ҷ� ����Ʈ ����
	int BCount          ; // B_SIDE  �ҷ� ����Ʈ ����
	int CCount          ; // C_SIDE  �ҷ� ����Ʈ ����
	int DCount          ; // D_SIDE  �ҷ� ����Ʈ ����
	int LTCorCount      ; // C1_SIDE �ҷ� ����Ʈ ����
	int LBCorCount      ; // C2_SIDE �ҷ� ����Ʈ ����
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

	// ��ü ������ DarkDefect 
	int LeftDefectCnt     ;
	int FrameLeftDefectCnt;
	// ��ü ������ BrightDefect 
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

	// ��ü ������(���� AllGrind) 
	// Left Dark Defect 
	TBlob LeftDefectPoint  [MAX_DEFECT_POINT];
	// Left Bright Defect 
	TBlob LeftCrackPoint   [MAX_DEFECT_POINT];

	// Polygon Defect Data 
	TBadRect BadRect[MAX_POLYGON_DEFECT_COUNT];

	//Burr Data 
	//����� �ʿ� ������...���߿� �ʿ������� ��!! 
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

	// Corner Edge Point Data(Corner �����˻� Data) NOTCH 
	TFPoint *LTCorInPoint;
	TFPoint *LBCorInPoint;

	// Total ���� In/Out Data 
	TFPoint AllCornerInData [MAX_ALL_EDGE_COUNT];
	TFPoint AllCornerOutData[MAX_ALL_EDGE_COUNT];

// 	TFPoint AllCornerInData [1];
// 	TFPoint AllCornerOutData[1];

	// Corner Edge Point Data(Corner �����˻� Data) NOTCH 
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

	// Total ���� In/Out Data 
	TFPoint AllCircleInData [MAX_ALL_EDGE_COUNT];
	TFPoint AllCircleOutData[MAX_ALL_EDGE_COUNT];

// 	TFPoint AllCircleInData [1];
// 	TFPoint AllCircleOutData[1];

	// AllGrind��� ���� Frame���� ������ ����
	TFPoint *LeftInPoint ;
	TFPoint *LeftOutPoint;

	// Cutting ������ ���ؼ� �߻��ϴ� ���� �ذ� ���� ���� 
	TFPoint *FrameLeftInPoint ;
	TFPoint *FrameLeftOutPoint;

	// Total ���� In/Out Data 
	TFPoint AllLeftInData [MAX_ALL_EDGE_COUNT];
	TFPoint AllLeftOutData[MAX_ALL_EDGE_COUNT];

	// Circle �ڸ��˻翡 �ʿ��� ������ ����(Circle Fitting �˰��� ����� ���)
	// Circle Fitting �˰��� ����� Bright_Defect������ ���ؼ� �ʿ��� ������ 
// 	TFPoint *LTCirInCrackPoint;
// 	TFPoint *LBCirInCrackPoint;

	// ---------------------------------------------------------------------
	// Compare Data(�߰� ������)
	// ---------------------------------------------------------------------
	// �˻縦 ���ؼ� �ʿ��� �߰� ������ 
	TCorner LTCorner[MAX_CORNER_POINT];
	TCorner LBCorner[MAX_CORNER_POINT];
// 	TCorner C3Corner[MAX_CORNER_POINT];
// 	TCorner C4Corner[MAX_CORNER_POINT];

	// Bright Defect ���⿡ �ʿ��� EdgePoint ������(Polygon �˰��� ���� �ʿ��� ������)
	// Polygon Algorithm 
	TCircle LTCircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle LBCircleData[MAX_CIRCLE_EDGEPOINT];

	// Bright Defect ���⿡ �ʿ��� EdgePoint ������(Polygon �˰��� ���� �ʿ��� ������)
	// Polygon Algorithm 
	TCircle LeftCircleData[MAX_CIRCLE_EDGEPOINT];
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// GrindMark Data && ROI Daata 
	// ---------------------------------------------------------------------
	//8_Camera �˻�� �ʿ��� GrindMark Data 
	TGrindMark GrindMark    [MAX_GRIND_MARK_COUNT];
	// Hole Grind Mark 
	TGrindMark HoleGrindMark[MAX_HOLE_GRIND_MARK_COUNT];
	// EnCap ASide Pattern Distance 
	TPatternDis PatternDis  [MAX_GRIND_MARK_COUNT];

	// CutLine vs GrindLine �������� 
	TGrindLine GrindLine[MAX_ROI_ID];

	// ChamberCut or Circle ������ �ʿ��� ROI���� 
	CRect   ASideRect[1];
	CRect   CSideRect[1];
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------

	// �������� �˻���� ������ ����Ʈ 
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

	// �κп����� ��ü EdgePoint Count
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

	// �κп����� ��ü EdgePoint Count
	int AllCirInEdgeCount  ;
	int AllCirOutEdgeCount ;

	// New 8_Camera  
	// ����,���� �˻�� ��ü �����˻�� �ʿ��� Count_Data 
	int LeftInEdgeCount     ;
	int LeftOutEdgeCount    ;
	int AllLeftInEdgeCount  ;
	int AllLeftOutEdgeCount ;

	// Cutting ������ ���ؼ� �߻��ϴ� ���� �ذ� ���� ���� 
	int FrameLeftInEdgeCount ;
	int FrameLeftOutEdgeCount;
	int StaLeftInEdgeCount   ;
	int StaLeftOutEdgeCount  ;

	// ���� �κп����� �ʿ��� Count_Data 

	// �ڸ��ҷ� ������ Edge Point Count
	int LTCirInCrackCount;
	int LBCirInCrackCount;

	// �ڸ��ҷ� ������ Edge Point Count
    int LeftCrackCount;
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------

	// ---------------------------------------------------------------------
	// ThresHold 
	// ---------------------------------------------------------------------
	int ECThresHold_Sta        ; // TopCorner ThresHold �ű� TCG �̹��� ���� ��� �ڳ� 
	int ECThresHold_End        ; // BTMCorner ThresHold �ű� TCG �̹��� ���� �ϴ� �ڳ� 
	int CorGrindInThresHold_Sta; // Corner ���� ���� ThresHold �ű� TCG �̹��� ���� ��� �ڳ�
	int CorGrindInThresHold_End; // Corner ���� ���� ThresHold �ű� TCG �̹��� ���� �ϴ�
	int CorGrindMeaThresHold; // Corner 3Point ����� ���Ǵ� ThresHold(������ ���� ����)

	// Defect ThresHold 
// 	int EnCapTopThresHold  ; // Top Defect Find
// 	int EnCapMidThresHold  ; // Mid Defect Find
// 	int EnCapBtmThresHold  ; // Bot Defect Find
	int DarkThresHold      ;
	int BrokenThresHold    ; // Bright Defect Find 
	int MaskDefectThresHold; // Mask Dark Defect ThresHold 

	// Mask Image ������ ���� Offset 
	int MaskDefectOffset   ; // Mask Dark Defect Offset  
	int MaskOffsetDistance ; // Mask Defect Offset Distance 

	// �ܰ����� ã�� �⺻ ThresHold 
	int LeftEdgeThresHold  ;
	int TopEdgeThresHold   ;
	int RightEdgeThresHold ;
	int BtmEdgeThresHold   ;

    bool LeftEdgeBTWhite ; //Black To White Edge ���� 
	bool TopEdgeBTWhite  ; //Black To White Edge ���� 
	bool RightEdgeBTWhite; //Black To White Edge ���� 
	bool BtmEdgeBTWhite  ; //Black To White Edge ���� 

	// �������� ã�� ThresHold 
	int LeftGrindThresHold ;
	int TopGrindThresHold  ;
	int RightGrindThresHold;
	int BtmGrindThresHold  ;

	// Cutting���� ã�� ThresHold 
	int LeftCutThresHold   ;

	//Pattern Distacne ���ʶ��� ���� ThresHold 
	int PatternThresHold   ;
	int PatternOffSet      ; //�������°� ũ�⶧���� PatternDistance�������� Offset���� �� ���������� �����  

	int CirGrindInThresHold ; // MarkMode  ==> Circle���� ���� ThresHold
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// EdgeLine ������ Data (TFPoint)
	// ---------------------------------------------------------------------
	// ���� Edge���� �ܰ����� ������ 
	TFPoint FLTPoint; //�������ΰ� ��ܶ����� ������ ��ǥ
	TFPoint FLBPoint; //�������ΰ� �ϴܶ����� ������ ��ǥ

	TFPoint FLTLPoint; //�������ΰ�   ù��° �ڳ� ������ ��ǥ 
	TFPoint FLTTPoint; //���ʶ��ΰ�   ù��° �ڳ� ������ ��ǥ 
	TFPoint FLBLPoint; //�������ΰ�   �׹�° �ڳ� ������ ��ǥ 
	TFPoint FLBBPoint; //�Ʒ��ʶ��ΰ� �׹�° �ڳ� ������ ��ǥ 

	// NOTCH �˻�� �ʿ��� Corner ���� ���ʶ��� ������ ��ǥ
	TFPoint FLTLInPoint;
	TFPoint FLTTInPoint;
	TFPoint FLBLInPoint;
	TFPoint FLBBInPoint;

	// NOTCH �˻�� �ʿ��� Corner ������ ���� ���� ������ 
	TFPoint FLTLMeaPoint;
	TFPoint FLTTMeaPoint;
	TFPoint FLBLMeaPoint;
	TFPoint FLBBMeaPoint;

	//���� ���� �ʿ��� ���� Mark��ǥ 
	TFPoint AlignMark1;
	TFPoint AlignMark2;

	// Grind�˻�� ROI���� ��ǥ 
	TPoint PolyP1[MAX_ROI_ID];
	TPoint PolyP2[MAX_ROI_ID];
	TPoint PolyP3[MAX_ROI_ID];
	TPoint PolyP4[MAX_ROI_ID];

	//��ü Buffer���� ��ǥ 
	TFPoint BuffLTPoint;

	// 
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------

	// ---------------------------------------------------------------------
	// Cell Information && ��Ÿ 
	// ---------------------------------------------------------------------
	// Corner Dx, Dy ������ 
	double LTDx; 
	double LTDy; 
	double LBDx;
	double LBDy;

	// Match ���� ��� 
	CString FindDir ;
	CString MatchDir;

	// Corner �����˻� ���� Ȯ�� 
	bool IsCornerTop;
	bool IsCornerBtm;
	// Corner �����˻� ���� Ȯ�� 
	bool IsRectTop  ;
	bool IsRectBtm  ;
	// Cell Circle Grind �˻� ���� ����
	bool IsCircleTop;
	bool IsCircleBtm;

	// Frame���� GrindMark �������� ���� 
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
	// GrindMark ���������� �ʿ��� ���� ������ 
	double dSideDeg; 
	// Display ���� ȸ�� �߽� ��ǥ 
	TFPoint CenterPoint;
	// Pixel ���� ������ ���ս� �ʿ��� Buffer 
// 	void * IAddress;

	// �ܰ����� vs Size���� ���� 
	bool InspectMode;    // 0: �ܰ�����  1:Size ����(BMLine ����)

	//LHJ �߰� RQ_Model �����ϸ鼭 RQ_Model �ϰ�� A2 SetAngleBias�� 135 -> 150���� ������. A1�� �������� ����
	bool RqMode;
	//LHJ �߰� WK_Model �����ϸ鼭 WK_Model �ϰ�� C3,C4 (Cir8,Cir12) SetAngleBias�� 315 -> 290 / 45 -> 85 ���� ������
	bool WkMode;

	// ���� Frame���� Image���� 
	int FrameImgW;
	int FrameImgH;
	// ��ü Frame ���� ImageHeight
	int ImageHeight;
// 	int ImageWidth ;

	// ImageHeightEnd
	int ImageHEnd  ;

	// Mark Rotate Degree 
	float MarkDegree;

	// ROI���� ���� ���� ���� 
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

	// Log��Ͻ� �з� ����(�м�)
	int    CellId      ;
	int    CamId       ;
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	//-------------------------------------------------------------------
	// Result(Dx,Dy,Round,Diameter,Center) �� ġ��,����,����
	//-------------------------------------------------------------------
	// Circle Dx, Dy, Round Data, Grind_Width, Grind_Measure 
	double LTCirDx      ;
	double LTCirDy      ;
	double LTCirRound   ;
	double LBCirDx      ;
	double LBCirDy      ;
	double LBCirRound   ;

	// �� ���ϱ��� ����,���� ���� ������ 
	double LeftGrindWidth ;
	double LeftGrindMea   ;

	// Corner Chamber_Cut 3Point���� ����,���� ������ ������ 
	double LTCorGrindWd ;
	double LTCorGrindMea;
	double LBCorGrindWd ;
	double LBCorGrindMea;

	//Circle Center Point
	// ������ ���� CenterPoint 
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
	//All Grind ������ �ʿ��� ������(���� ��� ã�⶧���� ���Ͱ��� ����)
	TFPoint AllGrindCriFitCenter;
	////////////////////////////////////////////////

	// �ݿ��� �ڸ� �� �ɹڸ� �������� Diameter
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

	// ���� Circle Spec Width, Height 
	// A�� Circle Dx, Dy 
	double LTSideCircleWidth  ;
	double LTSideCircleHeight ;

	// C�� Circle Dx, Dy 
	double LBSideCircleWidth  ;
	double LBSideCircleHeight ;
	
	// A�� Align Mark���� GrindMark���� �Ÿ� ������ 
	// PatternFinder ROI���� �ڵ� ���� ���� 
	double AMarkToGrindMarkDvX;
	double AMarkToGrindMarkDvY;

	// C�� ������ ���� GrindMakr���� �Ÿ� ������ 
	// PatternFinder ROI���� �ڵ� ���� ���� 
	double CMarkToGrindMarkDvX;
	double CMarkToGrindMarkDvY;

	//  GOO 20180909
	// 	double CenterCircleWidth ;
	// 	double CenterCircleHeight;

	// ���� ���� (Circle)
// 	int SideCirDeg   ;
// 	int CenterCirDeg ;

	// TrackLine ���� Width, Height 
	//  GOO 20180909
	// 	double TrackCircleWidth ;
	// 	double TrackCircleHeight;

	// �ܰ����ο��� TrackLine���� �Ÿ� 
	double OutToTrackLineDis;

	// �ܰ����ο��� ���� GrindMark���� Width,Height �Ÿ�
	double OutToGrindMarkDis1;
	double OutToGrindMarkDis2;

	// �ڸ� ���� �������� EdgeLine���� Trap���� �Ÿ� 
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

	// Cell Size���� ��� ������(Notch_Cell)
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

	// Cell Size ���� ����(������ǥ) 
	TFPoint LeftStaPos      ;
	TFPoint LeftEndPos      ;
	TFPoint CenterStaPos    ;
	TFPoint CenterEndPos    ;

	// Center Cell Size  ������ �ʿ��� ������ 
	float TotalCellSizeWidth;
	float CenterCellSize    ;
	int   TotalCellSizeCount;

	// Grind Mark Find Score 
	double GrindMarkMinScore;
	double GrindPadMarkMinScore;

	// Align ���ϰ� Ʋ������� �߻��ϴ� Defect����ų 
	int LeftDegMargin ;
	int RightDegMargin;

	// �ܰ����ο��� AlignMark���� �Ÿ�  
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
	// ���ʿ� ������ Ȯ���ؾ��� 
	//-------------------------------------------------------------------
	TFPoint FCellPoint [MAX_DEFECT_POINT]; // IsMarkLine CellSize ���� �ʿ��� ��ǥ�� 

	CFM3xLineDefect();
	~CFM3xLineDefect();

	//---------------------------------------------------------------------
	// Functions - Set
	// ---------------------------------------------------------------------
	// Set �Լ� 
	void SetProcessMode    (TProcessMode ProcessMode); 
	void SetInspectMode    (TInspectMode InspectMode); 
	void SetShape          (TShape Shape)            ;
	void SetLineScanType   (int LineScanType        );
	void SetGlassType      (TGlassType GlassType    );

	// AlignMark Y������ ���� �����Ͽ� ROI���� ���� 
	void SetCrossLeftPoint  (int x, int y)           ;  
	void SetCrossRightPoint (int x, int y)           ;  

	// �˻� Range ���� 
	void SetSpecWidth       (float MinWidth , float MaxWidth );
	void SetSpecHeight      (float MinHeight, float MaxHeight);

	//---------------------------------------------------------------------
	// Functions - Get
	// ---------------------------------------------------------------------
	// Get �Լ� 
	CFind*  GetFindPointer(int n);


	// Corner ���� ���� ���� 
	// 	void SetCornerTop(bool IsEnable);
	// 	void SetCornerBtm(bool IsEnable);

	// Corner ���� ���� ���� 
	// 	void SetRectTop(bool IsEnable);
	// 	void SetRectBtm(bool IsEnable);

	// Circle ���� ���� ���� 
	// 	void SetCircleTop    (bool IsEnable);
	// 	void SetCircleBtm    (bool IsEnable);

	// NarrowTrench ���� ���� 
	// 	void SetNarrowTrench (bool IsEnable);

	// EdgeInspection
	// 	void SetBrokenThresHold  (int ThHold)  ;
	//-----------------------------------------------------------------------------
};

