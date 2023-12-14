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

class AFX_EXT_CLASS CFM14xLineDefect : public CFilter { //CELL ������(LineScan4k X 4)

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
	// Match ���� ��� 
	CString FindDir  ;

	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// Euresys Data 
	// ---------------------------------------------------------------------
	// Euresys Class
	EImageBW8 BinaryImg             ; // ������ ã�� ���� �̹��� 
	EImageBW8 DefectImg             ; // Defect�˻� ���� �̹��� 
	EImageBW8 BrightDefectImg       ; // BrightDefect�˻� ���� �̹���
	EImageBW8 MaskDarkDefectImg     ; // Making DarkDefect  �˻� ���� �̹��� 
	EImageBW8 MaskBrightDefectImg   ; // Making BrightDefect�˻� ���� �̹��� 

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
	struct TLine          { TFPoint FP0; TFPoint FP1; } Line          [20] ; // �⺻���� 
	struct TLeftOutLine   { TFPoint FP0; TFPoint FP1; } LeftOutLine   [1 ] ; // �������� 
	struct TBtmOutLine    { TFPoint FP0; TFPoint FP1; } BtmOutLine    [1 ] ; // �������� 
	struct TTopOutLine    { TFPoint FP0; TFPoint FP1; } TopOutLine    [1 ] ; // �������� 
	struct TRealOutLine   { TFPoint FP0; TFPoint FP1; } RealOutLine   [MAX_REAL_FRAME_COUNT] ; //TopCam���� Real �ܰ�����  
	

	// InLine or OutLine Data�� ������� PreInLine,PreOutLine ������ ������ 
	struct TPreLeftOutLine  { TFPoint FP0; TFPoint FP1; } PreLeftOutLine [1];
	struct TPreLeftInLine   { TFPoint FP0; TFPoint FP1; } PreLeftInLine  [1];
	//----------------------------------------------------------------------
	//Defect Count 
	// ---------------------------------------------------------------------
	int DefectPointCount; // ��ü �ҷ� ����Ʈ ����
	int ACount          ; // A_SIDE Dark   �ҷ� ����Ʈ ����
	int BCount          ; // B_SIDE Dark   �ҷ� ����Ʈ ����
	int CCount          ; // C_SIDE Dark   �ҷ� ����Ʈ ����
	int DCount          ; // D_SIDE Dark   �ҷ� ����Ʈ ����

	int ACrackCount     ; // A_SIDE Bright �ҷ� ����Ʈ ����
	int BCrackCount     ; // B_SIDE Bright �ҷ� ����Ʈ ����
	int CCrackCount     ; // C_SIDE Bright �ҷ� ����Ʈ ����
	int DCrackCount     ; // D_SIDE Bright �ҷ� ����Ʈ ����

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

	// Total ���� In/Out Data 
	TFPoint AllCornerOutData[MAX_ALL_EDGE_COUNT];

	// Circle Out Coordinate Data
	TFPoint *LTCirOutPoint;
	TFPoint *LBCirOutPoint;

	// Total ���� In/Out Data 
	TFPoint AllCircleOutData[MAX_ALL_EDGE_COUNT];

	// All Etching ���� Frame���� ������ ����
	TFPoint *LeftInPoint ;
	TFPoint *LeftOutPoint;

	// All Etching ���� In/Out Data 
	TFPoint AllLeftInData [MAX_ALL_EDGE_COUNT];
	TFPoint AllLeftOutData[MAX_ALL_EDGE_COUNT];

	// Print Screen ���� Frame���� Data 
	TFPoint *PrintScPoint;

	// All Print Screen Data 
	TFPoint AllPrintScData[MAX_ALL_EDGE_COUNT];

	// Masking Pointer 
	POINT *MaskPoint;

	// ---------------------------------------------------------------------
	// Compare Data(�߰� ������)
	// ---------------------------------------------------------------------
	// �˻縦 ���ؼ� �ʿ��� �߰� ������ 
	TCorner LTCorner[MAX_CORNER_POINT];
	TCorner LBCorner[MAX_CORNER_POINT];
	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------


	// ---------------------------------------------------------------------
	// PrintMark Data && ROI Daata 
	// ---------------------------------------------------------------------
	//8_Camera �˻�� �ʿ��� PrintMark Data 
	TGrindMark PrintMark    [MAX_PRINT_MARK_COUNT];

	//Display ROI���� 
	CRect  LTROI   ; //LTPoint ChamberCut or CircleCut ROI
	CRect  LBROI   ; //LBPoint ChamberCut or CircleCut ROI
	CRect  PrintROI; //Print Screen ROI

	// ---------------------------------------------------------------------
	// ---------------------------------------------------------------------

	// �������� �˻���� ������ ����Ʈ 
// 	TPoint BtmEdgePoint[2];
// 	TPoint TopEdgePoint[2];
	// ---------------------------------------------------------------------
	// Data Count Memory(EdgePoint)
	// ---------------------------------------------------------------------
	// Corner Edge Point Count
	int LTCorOutEdgeCount;
	int LBCorOutEdgeCount;

	// ��ü EdgePoint Count
	int AllCorOutEdgeCount ;

	// Circle Out_Edge Point Count 
	int LTCirOutEdgeCount;
	int LBCirOutEdgeCount;

	// ��ü EdgePoint Count
	int AllCirOutEdgeCount ;

	// Etching �� Cutting ���� ����� �ʿ��� ������  
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

	// Mask Image ������ ���� Offset 
	int MaskDefectOffset   ; // Mask Dark Defect Offset  

	// �ܰ����� ã�� �⺻ ThresHold 
	int LeftEdgeThresHold  ;
	int TopEdgeThresHold   ;
	int RightEdgeThresHold ;
	int BtmEdgeThresHold   ;

    bool LeftEdgeBTWhite ; //Black To White Edge ���� 
	bool TopEdgeBTWhite  ; //Black To White Edge ���� 
	bool RightEdgeBTWhite; //Black To White Edge ���� 
	bool BtmEdgeBTWhite  ; //Black To White Edge ���� 

	//  CutLine ThresHold 
	int LeftCutThresHold   ;
	//  Print Screen ThresHold 
	int LeftPrintThresHold ;

	//  Print Screen ���� ���� Value  
	int PrintScOffset; //�ܰ����� ���� Offset
	int PrintScRange ; //PrintScreen ���� Range 

	// Real Out Line Count 
	int RealOutCnt;
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

	//���� ���� �ʿ��� ���� Mark��ǥ 
	TFPoint AlignMark1;
	TFPoint AlignMark2;

	// Grind�˻�� ROI���� ��ǥ 
	TPoint PolyP1[1];
	TPoint PolyP2[1];
	TPoint PolyP3[1];
	TPoint PolyP4[1];

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
	bool bCutLine   ; //Etching �� Cutt Line ���� ���� 0:Cut Line ����, 1:Cut Line ���� 
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
	int    ScanID      ;
	int    CamId       ;

	//�˻� ���� Width 
	int AllCutWidth    ;

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
	double LTCircleWidth ;
	double LTCircleHeight;

	// C�� Circle Dx, Dy 
	double LBCircleWidth  ;
	double LBCircleHeight ;
	
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

	// Cell Size���� ��� ������(Notch_Cell)
	double LeftCellSizeHeight  ;  

	double CenCellSizeWidth    ;

	// Cell Size ���� Value 
	double PrintEdge1;
	double PrintEdge2;
	double PrintEdge3;

	// Cell Size ���� ����(������ǥ) 
	TFPoint LeftStaPos      ;
	TFPoint LeftEndPos      ;
	TFPoint CenterStaPos    ;
	TFPoint CenterEndPos    ;

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
	void OnExecutePrint(TFPoint StaX, TFPoint StaY, int InspMode, bool bCutLine=false);
// 	void OnExecuteGrind(TFPoint StaX, TFPoint StaY);

	//-------------------------------------------------------------------
	// ���ʿ� ������ Ȯ���ؾ��� 
	//-------------------------------------------------------------------
	TFPoint FCellPoint [MAX_DEFECT_POINT]; // IsMarkLine CellSize ���� �ʿ��� ��ǥ�� 

	CFM14xLineDefect();
	~CFM14xLineDefect();

	//---------------------------------------------------------------------
	// Functions - Set
	// ---------------------------------------------------------------------
	// Set �Լ� 
	void SetProcessMode    (TProcessMode ProcessMode); 
	void SetInspectMode    (TInspectMode InspectMode); 
	void SetShape          (TShape Shape)            ;
	void SetLineScanType   (int LineScanType        );

	// AlignMark Y������ ���� �����Ͽ� ROI���� ���� 
	void SetCrossLeftPoint  (int x, int y)           ;  
	void SetCrossRightPoint (int x, int y)           ;  

	// �˻� Range ���� 
	void SetSpecWidth       (float MinWidth , float MaxWidth );
	void SetSpecHeight      (float MinHeight, float MaxHeight);

	//Find ��� ���� 
	void SetPath            (CString Path);

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

