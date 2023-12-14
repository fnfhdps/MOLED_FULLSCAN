#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LxHeader.h"
#include "LxLine.h"
#include "LxBlob.h"
#include "LxFind.h"
#include "TheLogger.h"
#include "LxDoubleLine.h"
#include "LxMatch.h"
#include "LxFitting.h"
#include "Open_eVision_1_2.h"
#include "LxCircle.h"


using namespace Euresys::Open_eVision_1_2;
// using namespace The;

class AFX_EXT_CLASS CFM1xLineDefect : public CFilter { // CNC_6HEAD New Version(LineScan16k X 2)

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
	CPolygon     *Polygon   ;
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
	struct TGrindMark{CRect Rect; TFPoint Point; TFPoint RectPoint; double Gradient; double Score; double degree; int DvX; int DvY;double GrindWidth; double GrindMea;};
	struct TPolygonROI{int StaX; int StaY; int Width; int Height;};

	// ---------------------------------------------------------------------
	// Property
	// ---------------------------------------------------------------------
	TFPoint FP0, FP1    ; // ������ �Ǵ� ��ǥ
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
	int FCompanyMode    ;

	// ---------------------------------------------------------------------
	// Euresys Data 
	// ---------------------------------------------------------------------
	// Euresys Class
	EImageBW8 BinaryImg             ;
	EImageBW8 DefectImg             ;
	EImageBW8 RotateImg             ;
	EImageBW8 RotateBinaryImg       ;
	EImageBW8 RotateGrindBinaryImg  ;
	EImageBW8 RotateGrindInBinaryImg;
	EImageBW8 RotateBrokenImg       ;

	// ---------------------------------------------------------------------
public    :
	//----------------------------------------------------------------------
	// Architecture
	// ---------------------------------------------------------------------
	CTheLogger   LogUnit;
	CTimer       Time   ;
	//----------------------------------------------------------------------
	// Define
	//----------------------------------------------------------------------
	enum TShape       { shRect8=0, shRectH, shRectHL, shRectHR, shRectC, shRectCL, shRectCR, shRectS, shRectSL, shRectSR, shRectTOP, shRectBTM, shRectCC};
	enum TProcessMode { pmAutoBevelIn, pmManualBevelIn, pmAutoOutLine                                                                                   };
	enum TCompany     { COM_KWON =0  ,COM_TIANMA , COM_BOE      , COM_ONCELL , COM_SLBRAIN, COM_NOTCH, COM_BMDT                                         };
	enum TInspectMode { Insp_Track=0 ,Insp_Mark  , Insp_Polygon ,Insp_Surface                                                                           }; 
	enum TCutMode     { Normal_Cut=0 ,CAI_Cut    , Rough_Cut                                                                                            };

	//----------------------------------------------------------------------
	// Struct 
	//----------------------------------------------------------------------
	struct TLine          { TFPoint FP0; TFPoint FP1; } Line          [20]           ; // �⺻���� 
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

	// LeftTrackLine or RightTrackLine or BtmTrackLine Data ������� PreLeftTrackLine, PreRightTrackLine, PreBtmTrackLine ������ ����� 
	struct TPreLeftTrackLine { TFPoint FP0; TFPoint FP1; } PreLeftTrackLine [1]      ;
	struct TPreRightTrackLine{ TFPoint FP0; TFPoint FP1; } PreRightTrackLine[1]      ;

	// InLine or OutLine Data�� ������� PreInLine,PreOutLine ������ ������ 
	struct TPreLeftOutLine  { TFPoint FP0; TFPoint FP1; } PreLeftOutLine [1]         ;
	struct TPreLeftInLine   { TFPoint FP0; TFPoint FP1; } PreLeftInLine  [1]         ;
	struct TPreRightOutLine { TFPoint FP0; TFPoint FP1; } PreRightOutLine[1]         ;
	struct TPreRightInLine  { TFPoint FP0; TFPoint FP1; } PreRightInLine [1]         ;

	//----------------------------------------------------------------------
	//Defect Count 
	// ---------------------------------------------------------------------
	int DefectPointCount; // ��ü �ҷ� ����Ʈ ����
	int ACount          ; // A_SIDE  �ҷ� ����Ʈ ����
	int BCount          ; // B_SIDE  �ҷ� ����Ʈ ����
	int CCount          ; // C_SIDE  �ҷ� ����Ʈ ����
	int DCount          ; // D_SIDE  �ҷ� ����Ʈ ����
	int C1Count         ; // C1_SIDE �ҷ� ����Ʈ ����
	int C2Count         ; // C2_SIDE �ҷ� ����Ʈ ����
	int C3Count         ; // C3_SIDE �ҷ� ����Ʈ ����
	int C4Count         ; // C4_SIDE �ҷ� ����Ʈ ����
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

	// ---------------------------------------------------------------------
	// Data Memory  
	// ---------------------------------------------------------------------
	// Corner Edge Point Data(Normal)
	TFPoint *C1Point;
	TFPoint *C2Point;
	TFPoint *C3Point;
	TFPoint *C4Point;

	// Corner Edge Point Data(Corner �����˻� Data) NOTCH 
	TFPoint *C1InPoint;
	TFPoint *C2InPoint;
	TFPoint *C3InPoint;
	TFPoint *C4InPoint;

	// Corner Edge Point Data(Corner �����˻� Data) NOTCH 
	TFPoint *C1MeaPoint;
	TFPoint *C2MeaPoint;
	TFPoint *C3MeaPoint;
	TFPoint *C4MeaPoint;

	// Corner Calculate 
	TFPoint P1;
	TFPoint P2;

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

	// Circle �ڸ��˻翡 �ʿ��� ������ ����
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

	// Compare Corner Data 
	TCorner C1Corner[MAX_CORNER_POINT];
	TCorner C2Corner[MAX_CORNER_POINT];
	TCorner C3Corner[MAX_CORNER_POINT];
	TCorner C4Corner[MAX_CORNER_POINT];

	TCircle A1CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle A2CircleData[MAX_CIRCLE_EDGEPOINT];

	TCircle C1CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle C2CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle C3CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle C4CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle C5CircleData[MAX_CIRCLE_EDGEPOINT];
	TCircle C6CircleData[MAX_CIRCLE_EDGEPOINT];

	TCircle Btm3CircleData[MAX_CIRCLE_EDGEPOINT];

	// Grind Mark ROI ������ (Find�˰��� ����)
	CRect    GrindMarkRect      [MAX_FIND_MARK_COUNT];
	TFPoint  GrindMarkPoint     [MAX_FIND_MARK_COUNT];
	TFPoint  GrindMarkRectCenter[MAX_FIND_MARK_COUNT];
	double   GrindMarkGradient  [MAX_FIND_MARK_COUNT];
	double   GrindMarkScore     [MAX_FIND_MARK_COUNT];

	// NotchROI_Rect
	CRect    NotchPolyRect[6];
	CRect    PadPolyRect  [2];

	// Ginrd Mark Rect Spec DvX, DvY
	TSpec GrindRectSpec[9];

	// �������� �˻���� ������ ����Ʈ 
	TPoint BtmEdgePoint[MAX_BTM_LINE_POINT];
	TPoint TopEdgePoint[MAX_TOP_LINE_POINT];

	// ---------------------------------------------------------------------
	// Data Count Memory 
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

	// �ڸ��ҷ� ������ Edge Point Count
	int A1CirInCrackCount;
	int A2CirInCrackCount;

	int C1CirInCrackCount;
	int C2CirInCrackCount;
	int C3CirInCrackCount;
	int C4CirInCrackCount;
	int C5CirInCrackCount;
	int C6CirInCrackCount;

	int Btm3CirInCrackCount;

	// ---------------------------------------------------------------------
    // ThresHold 
	// ---------------------------------------------------------------------
	int ECThresHold        ; // Corner ThresHold 
	int EnCapTopThresHold  ; // Top Defect Find
	int EnCapMidThresHold  ; // Mid Defect Find
	int EnCapBtmThresHold  ; // Bot Defect Find
	int BrokenThresHold    ; // Bright Defect Find 
	// �ܰ����� ã�� �⺻ ThresHold 
	int LeftEdgeThresHold  ;
	int TopEdgeThresHold   ;
	int RightEdgeThresHold ;
	int BtmEdgeThresHold   ;

	int CirGrindMeaThresHold; // TrackMode ==> TrackLine���� ThresHold 
	int CorGrindMeaThresHold; // Corner 3Point ����� ���Ǵ� ThresHold
	int CirGrindInThresHold ; // MarkMode  ==> Circle���� ���� ThresHold
	int CorGrindInThresHold ; // Corner ���� ���� ThresHold 

	// ---------------------------------------------------------------------
	// EdgeLine ������ Data  
	// ---------------------------------------------------------------------
	// TrakLine ���� ��� 
	// ���� Edge���� �ܰ����� ������ 
	TFPoint FLTPoint; //�������ΰ� ��ܶ����� ������ ��ǥ
	TFPoint FRTPoint; //�������ΰ� ��ܶ����� ������ ��ǥ
	TFPoint FRBPoint; //�������ΰ� �ϴܶ����� ������ ��ǥ
	TFPoint FLBPoint; //�������ΰ� �ϴܶ����� ������ ��ǥ

	// Notch Cell ���� TrackLine EdgePoint ���� ������ ��ǥ 
	TFPoint FTRLBPoint; //����Ʈ�����ΰ� �ϴ�Ʈ�������� ������ ��ǥ
	TFPoint FTRRBPoint; //����Ʈ�����ΰ� �ϴ�Ʈ�������� ������ ��ǥ

	TFPoint FLTLPoint; //�������ΰ�   ù��° �ڳ� ������ ��ǥ 
	TFPoint FLTTPoint; //���ʶ��ΰ�   ù��° �ڳ� ������ ��ǥ 
	TFPoint FRTRPoint; //�������ΰ�   �ι�° �ڳ� ������ ��ǥ 
	TFPoint FRTTPoint; //���ʶ��ΰ�   �ι�° �ڳ� ������ ��ǥ 
	TFPoint FRBRPoint; //�������ΰ�   ����° �ڳ� ������ ��ǥ 
	TFPoint FRBBPoint; //�Ʒ��ʶ��ΰ� ����° �ڳ� ������ ��ǥ 
	TFPoint FLBLPoint; //�������ΰ�   �׹�° �ڳ� ������ ��ǥ 
	TFPoint FLBBPoint; //�Ʒ��ʶ��ΰ� �׹�° �ڳ� ������ ��ǥ 

	// NOTCH �˻�� �ʿ��� Corner ���� ���ʶ��� ������ ��ǥ
	TFPoint FLTLInPoint;
	TFPoint FLTTInPoint;
	TFPoint FRTRInPoint;
	TFPoint FRTTInPoint;
	TFPoint FLBLInPoint;
	TFPoint FLBBInPoint;
	TFPoint FRBRInPoint;
	TFPoint FRBBInPoint;

	// NOTCH �˻�� �ʿ��� Corner ������ ���� ���� ������ 
	TFPoint FLTLMeaPoint;
	TFPoint FLTTMeaPoint;
	TFPoint FRTRMeaPoint;
	TFPoint FRTTMeaPoint;
	TFPoint FLBLMeaPoint;
	TFPoint FLBBMeaPoint;
	TFPoint FRBRMeaPoint;
	TFPoint FRBBMeaPoint;

	//���� ���� �ʿ��� ���� Mark��ǥ 
	TFPoint AlignMark1;
	TFPoint AlignMark2;

	// ---------------------------------------------------------------------
	// Cell Information && ��Ÿ 
	// ---------------------------------------------------------------------
	// Mark �������� ���� �缳�� �� Corner Size �˻� 
	int CELL_A;
	int CELL_B;
	int CELL_C;
	int CELL_D;
	int CELL_E;
	int CELL_F;
	int CELL_G;
	int CELL_H;
	// Corner Dx, Dy ������ 
	double LTDx; 
	double LTDy; 
	double RTDx;
	double RTDy;
	double RBDx;
	double RBDy;
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

	// Narrow Trench ���� 
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

	// Inspection ROI����(�������� �˻�� �ʿ���)
	int FLeftWidth     ;
	int FRightWidth    ;
	int FTopHeight     ;
	int FBtmHeight     ;

	// Image Rotate Degree
	float ImgDegree;
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

	// ImageHeight
	int ImageHeight;
	int ImageWidth ;

	// ImageHeightEnd
	int ImageHEnd  ;

	// Mark Rotate Degree 
	float MarkDegree;

	// ROI���� ���� ���� ���� 
	int BtmLimitLine        ;
	int TopLimitLine        ;

	bool IsMarkBtm  ; 
	bool IsAllGrind ;

	TFPoint MarkPos   ;
	TFPoint MarkStrPos;
	TFPoint MarkEndPos;
	TFPoint MarkPos1  ;
	TFPoint MarkPos2  ;
	float   MarkAng   ; 

	int FStartLine      ;
	int FEndLine        ;


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

	// Log��Ͻ� �з� ����(�м�)
	int    CellId      ;
	int    CamId       ;

	//-------------------------------------------------------------------
	// Result(Dx,Dy,Round,Diameter,Center) �� ġ��,����,����
	//-------------------------------------------------------------------
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

	// Btm������ ���������� Grind_Width, Grind_Measure
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
	// ������ ���� CenterPoint 
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

	// �ݿ��� �ڸ� �� �ɹڸ� �������� Diameter
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

	// ���� Circle Spec Width, Height 
	// A�� Circle Dx, Dy 
	double ASideCircleWidth  ;
	double ASideCircleHeight ;

	// A�� Align Mark���� GrindMark���� �Ÿ� ������ 
	// PatternFinder ROI���� �ڵ� ���� ���� 
	double AMarkToGrindMarkDvX;
	double AMarkToGrindMarkDvY;

	double AMarkToGrindMarkDvX2;
	double AMarkToGrindMarkDvY2;

	// C�� Circle Dx, Dy 
	double CSideCircleWidth  ;
	double CSideCircleHeight ;
	double CenterCircleWidth ;
	double CenterCircleHeight;

	// Narrow Trench ����� �˻�� �ʿ��� 
	// Cir2,Cir3 Dx�� �������� ���� �˰��� ROI������ �����Ͽ�����..�̶��� GrindMark���� �� ������ ��ȭ�� ���ؼ� 
	// �ε����ϰ� �߰� ������Ʈ �ʿ��� 
	// ���� �ҽ��� ���������� ����....
	double Center1CircleWidth;
	double Center2CircleWidth;

	// ���� ���� (Circle)
	int SideCirDeg   ;
	int CenterCirDeg ;

	// TrackLine ���� Width, Height 
	double TrackCircleWidth ;
	double TrackCircleHeight;

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

	// Grind Width ���� ������ 
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

	// Cell Size���� ��� ������(Notch_Cell)
	double LeftCellSizeHeight  ;  
	double CenCellSizeHeight   ;  
	double RightCellSizeHeight ;  

	double CenCellSizeWidth    ;
	double NotchCellSizeWidth  ;
	double NotchCellSizeHeight ;

	double LeftMarkToTopHeight ; //// Cell Mark To TopSide    ==>M1
	double CenMarkToTopHeight  ; //// Cell Mark To TopSide    ==>M2
	double RightMarkToTopHeight; //// Cell Mark To TopSide    ==>M3

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
	int OnAutoBevelIn();
	float GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);
	void OnLoad(bool IsLoad, CString Folder);

	//-------------------------------------------------------------------
    // ���ʿ� ������ Ȯ���ؾ��� 
	//-------------------------------------------------------------------
	TFPoint FCellPoint [MAX_DEFECT_POINT]; // IsMarkLine CellSize ���� �ʿ��� ��ǥ�� 

	//Burr Data 
	TBlob ABurrPoint   [MAX_DEFECT_POINT];
	TBlob BBurrPoint   [MAX_DEFECT_POINT];
	TBlob CBurrPoint   [MAX_DEFECT_POINT];
	TBlob DBurrPoint   [MAX_DEFECT_POINT];


	CFM1xLineDefect();
	~CFM1xLineDefect();

	//---------------------------------------------------------------------
	// Functions - Set
	// ---------------------------------------------------------------------
	// Set �Լ� 
	void SetProcessMode    (TProcessMode ProcessMode); 
	void SetInspectMode    (TInspectMode InspectMode); 
	void SetCutMode        (TCutMode     CutMode    );
	void SetCompanyMode    (TCompany     Company    );
	void SetShape          (TShape Shape)            ;

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


