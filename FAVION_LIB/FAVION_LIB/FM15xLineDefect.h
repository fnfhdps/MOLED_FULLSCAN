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
	struct TLine          { TFPoint FP0; TFPoint FP1; } Line          [20]           ; // �⺻���� 
	struct TLeftOutLine   { TFPoint FP0; TFPoint FP1; } LeftOutLine   [1]            ;
	struct TLeftInLine    { TFPoint FP0; TFPoint FP1; } LeftInLine    [1]            ;
	struct TRightOutLine  { TFPoint FP0; TFPoint FP1; } RightOutLine  [1]            ;
	struct TRightInLine   { TFPoint FP0; TFPoint FP1; } RightInLine   [1]            ;
	struct TBtmOutLine    { TFPoint FP0; TFPoint FP1; } BtmOutLine    [MAX_BTM_COUNT];
	struct TBtmInLine     { TFPoint FP0; TFPoint FP1; } BtmInLine     [MAX_BTM_COUNT];
	struct TTopOutLine    { TFPoint FP0; TFPoint FP1; } TopOutLine    [1]            ;
	struct TTopInLine     { TFPoint FP0; TFPoint FP1; } TopInLine     [1]            ;

	struct TPatternLine   { TFPoint FP0; TFPoint FP1; } PatternLine   [1]            ;

	// InLine or OutLine Data�� ������� PreInLine,PreOutLine ������ ������ 
	struct TPreLeftOutLine  { TFPoint FP0; TFPoint FP1; } PreLeftOutLine [1]         ;
	struct TPreLeftInLine   { TFPoint FP0; TFPoint FP1; } PreLeftInLine  [1]         ;
	struct TPreRightOutLine { TFPoint FP0; TFPoint FP1; } PreRightOutLine[1]         ;
	struct TPreRightInLine  { TFPoint FP0; TFPoint FP1; } PreRightInLine [1]         ;

	//----------------------------------------------------------------------
	//Defect Count 
	// ---------------------------------------------------------------------
	// ����Side Dark_Defect Count 
	int DefectPointCount; // ��ü �ҷ� ����Ʈ ����
	int ACount          ; // A_SIDE  �ҷ� ����Ʈ ����
	int BCount          ; // B_SIDE  �ҷ� ����Ʈ ����
	int CCount          ; // C_SIDE  �ҷ� ����Ʈ ����
	int DCount          ; // D_SIDE  �ҷ� ����Ʈ ����

	// ����Side Bright_Defect Count
	int ACrackCount     ;
	int BCrackCount     ;
	int CCrackCount     ;
	int DCrackCount     ;

	// Corner Dark_Defect Count 
	int C1Count         ; // C1_SIDE �ҷ� ����Ʈ ����
	int C2Count         ; // C2_SIDE �ҷ� ����Ʈ ����
	int C3Count         ; // C3_SIDE �ҷ� ����Ʈ ����
	int C4Count         ; // C4_SIDE �ҷ� ����Ʈ ����

	// ���� Circle Dark_Defect Count 
	int A1CirDeftectCnt ;
	int A2CirDeftectCnt ;
	int C1CirDeftectCnt ;
	int C2CirDeftectCnt ;

	// ���� Circle Bright_Defect Count 
	int A1CirCrackCnt  ;
	int A2CirCrackCnt  ;
	int C1CirCrackCnt  ;
	int C2CirCrackCnt  ;

	// ��ü���� Dark_Defect Count
	int LeftDefectCount ;
	int RIghtDefectCount;
	int TopDefectCount  ;
	int BtmDefectCount  ;

	// ��ü���� Bright_Defect Count
	int LeftCrackCount ;
	int RIghtCrackCount;
	int TopCrackCount  ;
	int BtmCrackCount  ;
	// ---------------------------------------------------------------------
	// Defect Data  
	// ---------------------------------------------------------------------
	// ����Side Defect(Dark)
	TBlob ADefectPoint    [MAX_DEFECT_POINT];
	TBlob BDefectPoint    [MAX_DEFECT_POINT];
	TBlob CDefectPoint    [MAX_DEFECT_POINT];
	TBlob DDefectPoint    [MAX_DEFECT_POINT];

	// ���� Side Defect(Bright)
	TBlob ACrackPoint     [MAX_DEFECT_POINT];
	TBlob BCrackPoint     [MAX_DEFECT_POINT];
	TBlob CCrackPoint     [MAX_DEFECT_POINT];
	TBlob DCrackPoint     [MAX_DEFECT_POINT];

	// Corner Defect 
	TBlob C1DefectPoint   [MAX_DEFECT_POINT];
	TBlob C2DefectPoint   [MAX_DEFECT_POINT];
	TBlob C3DefectPoint   [MAX_DEFECT_POINT];
	TBlob C4DefectPoint   [MAX_DEFECT_POINT];

	//�κп��� 
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

	//��ü������
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
	// Compare Corner Data(�˻縦 ���� �߰� ������) 
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

	// Total ���� In/Out Data 
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
	// EdgePoint Data Memory(��������)
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

	// AllGrind��� ���� Frame���� ������ ����
	TFPoint *LeftInPoint  ;
	TFPoint *LeftOutPoint ;
	TFPoint *TopInPoint   ;
	TFPoint *TopOutPoint  ;
	TFPoint *RightInPoint ;
	TFPoint *RightOutPoint;
	TFPoint *BtmInPoint   ;
	TFPoint *BtmOutPoint  ;

	// NarrowTrench �������� ���������� 
	TGrindLine *GrindLine;

	// Masking test 
	// ��ü���� ���� 
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
	// EdgePoint Data Count Memory(�������� Count����)
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

	// �ڸ��ҷ� ������ Edge Point Count
	int A1CirInCrackCount;
	int A2CirInCrackCount;

	int C1CirInCrackCount;
	int C2CirInCrackCount;

	//�ڸ��ҷ� ������ Edge Point Count 
	int LeftInCrackCount ;
	int RIghtInCrackCount;
	int TopInCrackCount  ;
	int BtmInCrackCount  ;

	// ��ü ������ �ʿ��� ��ü Count���� 
	int AllLeftInEdgeCount  ;
	int AllLeftOutEdgeCount ;
	int AllRightInEdgeCount ;
	int AllRightOutEdgeCount;
	int AllTopInEdgeCount   ;
	int AllTopOutEdgeCount  ;
	int AllBtmInEdgeCount   ;
	int AllBtmOutEdgeCount  ;

	// ��ü ������ �ʿ��� ���� Count���� 
	int LeftInEdgeCount     ;
	int LeftOutEdgeCount    ;
	int RightInEdgeCount    ;
	int RightOutEdgeCount   ;
	int TopInEdgeCount      ;
	int TopOutEdgeCount     ;
	int BtmInEdgeCount      ;
	int BtmOutEdgeCount     ;

	// NarrowTrench �������� ���������� 
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
	// GrindMark ���� ������ 
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

	//ROI_Rect(�ܼ� Display ����)
// 	CRect       NotchPolyRect[5];
	CRect       NotchPolyRect[MAX_CUT_GRIND_ROI   ];
	CRect       PadPolyRect  [MAX_GRIND_ROI_COUNT ];
	CRect       DefectRoiRect[MAX_DEFECT_ROI_COUNT];

	// �������� �˻���� ������ ����Ʈ 
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

	// �ܰ����� ã�� �⺻ ThresHold 
	int LeftEdgeThresHold  ;
	int TopEdgeThresHold   ;
	int RightEdgeThresHold ;
	int BtmEdgeThresHold   ;

    bool LeftEdgeBTWhite   ; //Black To White Edge ���� 
	bool TopEdgeBTWhite    ; //Black To White Edge ���� 
	bool RightEdgeBTWhite  ; //Black To White Edge ���� 
	bool BtmEdgeBTWhite    ; //Black To White Edge ���� 

	// �������� ã�� ThresHold
	int PadSideThresHold   ; // Corner,Pad Circle(A1,A2) ThresHold 
	int PadCenThresHold    ; // Pad ������(CAI) �������� ThresHold
	int NotchSideThresHold ; // Notch Side �������� ThresHold 
	int NotchCenThresHold  ; // Notch Center �������� ThresHold 
	int BSideGrindThresHold; // B�� ���� ���� 20221014
	int DSideGrindThresHold; // D�� ���� ���� 20221014

	// Chamber Cut Corner Grind Search ThresHold 
	int CorGrindOutThresHold; // Corner �ٱ��� ���� ThresHold 
	int CorGrindInThresHold ; // Corner ����   ���� ThresHold 
	int CorGrindMeaThresHold; // Corner 3Point ����� ���Ǵ� ThresHold

	// Cutting���� ã�� ThresHold 
	int CutThresHold   ;

	int PatternThresHold    ; // Pattern Distance ���ʶ��� ���� ThresHold 
	int PatternOffSet       ; // Pattern Distance ���ʶ��� ���� OffSet 

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

	// ������ Mask Offset 
	int LeftMaskOffset ;
	int TopMaskOffset  ;
	int RightMaskOffset;
	int BtmMaskOffset  ;

	// ������ Mask Distance 
	int LeftMaskDis ;
	int TopMaskDis  ;
	int RightMaskDis;
	int BtmMaskDis  ;

	//���� ���ʶ��� Pos Count
	int LeftGrindInCnt ;
	int TopGrindInCnt  ;
	int RightGrindInCnt;
	int BtmGrindInCnt  ;
	//���� ���ʶ��� Pos Data 
	int LeftGrindInPos ;
	int TopGrindInPos  ;
	int RightGrindInPos;
	int BtmGrindInPos  ;
	//���� �������� Defect �˻���� Range 
	int LeftDefectRange ;
	int TopDefectRange  ;
	int RightDefectRange;
	int BtmDefectRange  ;

	// ---------------------------------------------------------------------
	// EdgeLine ������ Data  
	// ---------------------------------------------------------------------
	// TrakLine ���� ��� 
	// ���� Edge���� �ܰ����� ������ 
	TFPoint FLTPoint; //�������ΰ� ��ܶ����� ������ ��ǥ
	TFPoint FRTPoint; //�������ΰ� ��ܶ����� ������ ��ǥ
	TFPoint FRBPoint; //�������ΰ� �ϴܶ����� ������ ��ǥ
	TFPoint FLBPoint; //�������ΰ� �ϴܶ����� ������ ��ǥ

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
	CString AlignDir;

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
	float LeftLineData ;
	float RightLineData;

	// Inspection ROI����(�������� �˻�� �ʿ���)
	int FLeftWidth     ;
	int FRightWidth    ;
	int FTopHeight     ;
	int FBtmHeight     ;

	// Image Rotate Degree
	float ImgDegree;
	// GrindMark ���������� �ʿ��� ���ذ��� ������ 
	double dASideDeg; //���� ���� - Glass����
	double dBSideDeg; //Glass���� - ���� ���� 
	double dCSideDeg; //���� ���� - Glass����
	double dDSideDeg; //Glass���� - ���� ���� 

	// Display ���� ȸ�� �߽� ��ǥ 
	TFPoint CenterPoint;
	// Pixel ���� ������ ���ս� �ʿ��� Buffer 
	void * IAddress;

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
	//int ImageWidth ;

	// ImageHeightEnd
	int ImageHEnd  ;

	// Mark Rotate Degree 
	float MarkDegree;

	// ROI���� ���� ���� ���� 
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

	//4�� ������ Corner ROI 
	int LTRectWidth ;
	int LTRectHeight;
	int RTRectWidth ;
	int RTRectHeight;
	int LBRectWidth ;
	int LBRectHeight;
	int RBRectWidth ;
	int RBRectHeight;

	//4�� ������ Corner ROI 
	int LTCornerWidth ;
	int LTCornerHeight;
	int RTCornerWidth ;
	int RTCornerHeight;
	int LBCornerWidth ;
	int LBCornerHeight;
	int RBCornerWidth ;
	int RBCornerHeight;

	//4�� ������ Circle ROI 
	int LTCircleWidth ;
	int LTCircleHeight;
	int RTCircleWidth ;
	int RTCircleHeight;
	int LBCircleWidth ;
	int LBCircleHeight;
	int RBCircleWidth ;
	int RBCircleHeight;

	// Log��Ͻ� �з� ����(�м�)
	int    CellId      ;
	int    CamId       ;

	//-------------------------------------------------------------------
	// Result(Dx,Dy,Round,Diameter,Center) �� ġ��,����,����
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
	// ���� Circle Spec Width, Height 
	// A�� Circle Dx, Dy 
	double ASideCircleWidth  ;
	double ASideCircleHeight ;

	// C�� Circle Dx, Dy 
	double CSideCircleWidth  ;
	double CSideCircleHeight ;
	double CenterCircleWidth ;
	double CenterCircleHeight;

	// ���� ���� (Circle)
	int SideCirDeg   ;
	int CenterCirDeg ;

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
    // ���ʿ� ������ Ȯ���ؾ��� 
	//-------------------------------------------------------------------
	TFPoint FCellPoint [MAX_DEFECT_POINT]; // IsMarkLine CellSize ���� �ʿ��� ��ǥ�� 

	CFM15xLineDefect();
	~CFM15xLineDefect();

	//---------------------------------------------------------------------
	// Functions - Set
	// ---------------------------------------------------------------------
	// Set �Լ� 
	void SetProcessMode    (TProcessMode ProcessMode); 
	void SetInspectMode    (TInspectMode InspectMode); 
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
	CFind* GetFindPointer();

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


