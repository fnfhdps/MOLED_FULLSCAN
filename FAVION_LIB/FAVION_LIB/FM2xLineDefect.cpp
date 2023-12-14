#include "stdafx.h"
#include "FM2xLineDefect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// #if LINESCAN_CAMERA_MODE == 0
// #define IElement(x, y)  *((BYTE *)IAddress + x + 4096 * y)  // 4K  LineScan
// #elif LINESCAN_CAMERA_MODE ==1
// #define IElement(x, y)  *((BYTE *)IAddress + x + 16384 * y) // 16K LineScan
// #elif LINESCAN_CAMERA_MODE ==2
// #define IElement(x, y)  *((BYTE *)IAddress + x + 23360 * y) // 23K LineScan
// #endif

// �ܰ��˻�� ����(LineScan23k X 2)
////////////////////////////////////////////////////////////
CFM2xLineDefect::CFM2xLineDefect()
{
	SingleLine  = new CSingleLine();
	RectEdge    = new CRectEdge();
	Edge        = new CEdge();
	Find1       = new CFind();
	Find2       = new CFind();
	Circle      = new CCircle();
	Find        = new CFind();
	CircleFit   = new CCircleFit();
	Polygon     = new CPolygon();

	FLeftWall    = true ; 
	FTopWall     = true ; 
	FRightWall   = true ; 
	FBottomWall  = true ; 
	FLeftTop     = true ; 
	FRightTop    = true ; 
	FRightBottom = true ; 
	FLeftBottom  = true ; 
	IsMarkBtm    = false;
	IsAllGrind   = false;

	MarkPos.x    = 0.0f;
	MarkPos.y    = 0.0f;

	MarkEndPos.x = 0.0f;
	MarkEndPos.y = 0.0f;

	MarkStrPos.x = 0.0f;
	MarkStrPos.y = 0.0f;

	MarkAng     = 0.0f;

	FBevelIn = false;

	FProcessMode = pmAutoOutLine;

	IRoi = new EROIBW8();

	// Dark Defect 
	DefectPointCount = 0; // ��ü �ҷ� ���� �ʱ�ȭ
	ACount           = 0; // ���ڸ�� �ҷ� ���� �ʱ�ȭ
	BCount           = 0; // ��������� �ҷ� ���� �ʱ�ȭ
	CCount           = 0; // �Ÿ�� �ҷ� ���� �ʱ�ȭ
	DCount           = 0;
	C1Count          = 0;
	C2Count          = 0;
	C3Count          = 0;
	C4Count          = 0;
	ABurrCount       = 0;
	BBurrCount       = 0;
	CBurrCount       = 0;
	DBurrCount       = 0;

	// Bridht Defect 
	ACrackCount      = 0;
	BCrackCount      = 0;
	CCrackCount      = 0;
	DCrackCount      = 0;

	// Dark Defect 
	A1CirDeftectCnt  = 0;
	A2CirDeftectCnt  = 0;
	C1CirDeftectCnt  = 0;
	C2CirDeftectCnt  = 0;

	// Bright Defect 
	A1CirCrackCnt    =0;
	A2CirCrackCnt    =0;
	C1CirCrackCnt    =0;
	C2CirCrackCnt    =0;

	// Cell ���� 
	CELL_A =0;
	CELL_B =0;
	CELL_C =0;
	CELL_D =0;
	CELL_E =0;
	CELL_F =0;
	CELL_G =0;
	CELL_H =0;

	LTDx=0.0f, RTDx=0.0f, RBDx=0.0f, LBDx=0.0f;
	LTDy=0.0f, RTDy=0.0f, RBDy=0.0f, LBDy=0.0f;

	// Corner Data OutPoint Init
	C1Point = NULL;
	C2Point = NULL;
	C3Point = NULL;
	C4Point = NULL;

	// Corner Data InPoint Init
	C1InPoint = NULL;
	C2InPoint = NULL;
	C3InPoint = NULL;
	C4InPoint = NULL;

	// Corner Grind Meaasure StandPoint
	C1MeaPoint = NULL;
	C2MeaPoint = NULL;
	C3MeaPoint = NULL;
	C4MeaPoint = NULL;

	// Circle Data InPoint Init
	A1CirInPoint   = NULL;
	A2CirInPoint   = NULL;
	// 	A1PolyInPoint  = NULL;

	C1CirInPoint   = NULL;
	C2CirInPoint   = NULL;

	// Circle Data OutPoint Init
	A1CirOutPoint   = NULL;
	A2CirOutPoint   = NULL;

	C1CirOutPoint   = NULL;
	C2CirOutPoint   = NULL;

	// �ڸ� Circle �˻� InPoint Init 
	A1CirInCrackPoint = NULL;
	A2CirInCrackPoint = NULL;

	C1CirInCrackPoint = NULL;
	C2CirInCrackPoint = NULL;

	// Btm �������� EdgePoint Data Init 
	BtmInPoint        = NULL;
	BtmOutPoint       = NULL;

	// MultiThresHold 
	ECThresHold       =0;
	EnCapTopThresHold =0;
	EnCapMidThresHold =0;
	EnCapBtmThresHold =0;

	//�⺻ EdgeThresHold Init 
	LeftEdgeThresHold    =0;
	TopEdgeThresHold     =0;
	RightEdgeThresHold   =0;
	BtmEdgeThresHold     =0;

	// �������� ã�� ThresHold 
	LeftGrindThresHold   =0;
	RightGrindThresHold  =0;
	BtmGrindThresHold    =0;

	// Standard Line ã�� ThresHold 
	LeftStdThresHold     =0;
	RightStdThresHold    =0;
	BtmStdThresHold      =0;

	CirGrindMeaThresHold =0;
	CorGrindMeaThresHold =0;
	CirGrindInThresHold  =0;
	CorGrindInThresHold  =0;

	//���α׷� ����� �ѹ� �ʱ�ȭ  
	// NOTCH CELL INSPECTION LineData Init
	PreLeftOutLine [0].FP0.x = 0.0f;
	PreLeftOutLine [0].FP0.y = 0.0f;
	PreLeftOutLine [0].FP1.x = 0.0f;
	PreLeftOutLine [0].FP1.y = 0.0f;
	PreLeftInLine  [0].FP0.x = 0.0f;
	PreLeftInLine  [0].FP0.y = 0.0f;
	PreLeftInLine  [0].FP1.x = 0.0f;
	PreLeftInLine  [0].FP1.y = 0.0f;
	PreRightOutLine[0].FP0.x = 0.0f;
	PreRightOutLine[0].FP0.y = 0.0f;
	PreRightOutLine[0].FP1.x = 0.0f;
	PreRightOutLine[0].FP1.y = 0.0f;
	PreRightInLine [0].FP0.x = 0.0f;
	PreRightInLine [0].FP0.y = 0.0f;
	PreRightInLine [0].FP1.x = 0.0f;
	PreRightInLine [0].FP1.y = 0.0f;

	//Notch Circle Dx,Dy 
	ASideCircleWidth    =0.0f;
	ASideCircleHeight   =0.0f;

	CSideCircleWidth    =0.0f;
	CSideCircleHeight   =0.0f;

	// �ܰ����ο��� ���� GrindMark���� Width,Height �Ÿ�
	OutToGrindMarkDis1=0.0f;
	OutToGrindMarkDis2=0.0f;

	// �ڸ� ���� �������� EdgeLine���� Trap���� �Ÿ� 
	dEdgeToTrapLineDis=0.0f;

	// Notch Cell Mark Init 
	dCirOffsetX        =0.0f;
	dCirOffsetY        =0.0f;

	LogUnit.Create(_T("FAVION"));

	FInspectMode =0; //InspectMode Classify Parameter

	//LHJ �߰� 20180808
	RqMode = false;
	WkMode = false;

	// Corner �����˻� ���� Ȯ�� 
	IsCornerTop = false;
	IsCornerBtm = false;

	// Rect �˻� ���� Ȯ�� 
	IsRectTop = false;
	IsRectBtm = false;

	// Cell Circle Grind �˻� ���� ����
	IsCircleTop = false;
	IsCircleBtm = false;

	// Find Dir 
	FindDir  ="";
	MatchDir =""; 

	// Grind Mark Find Score 
	GrindMarkMinScore    =0.0f;
	GrindPadMarkMinScore =0.0f;

	// A_Side AlignMark���� GrindMark���� �Ÿ� ������ Init
	AMarkToGrindMarkDvX =0.0f;
	AMarkToGrindMarkDvY =0.0f;

	CMarkToGrindMarkDvX =0.0f;
	CMarkToGrindMarkDvY =0.0f;

// 	for(int k=0; k<9; k++){
// 		GrindRectSpec[k].DvX =0.0f;
// 		GrindRectSpec[k].DvX =0.0f;
// 	}
	OnClearData();
}
CFM2xLineDefect::~CFM2xLineDefect()
{
	delete SingleLine    ;
	delete RectEdge      ;
	delete Edge          ;
	delete Find1         ;
	delete Find2         ;
	delete Circle        ;
	delete Find          ;
	delete CircleFit     ;
}

float CFM2xLineDefect::GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3)
{
	//--------------------------------------------------------------------------
	// ������ ������ ������ �������� ���� ġ�� ����
	// ���� : ax + by + c = 0, (m, n) -> Distance = (ma + nb +c) / sqrt(a*a + b*b)
	//--------------------------------------------------------------------------
	double xDiff = P2.x-P1.x;
	double yDiff = P2.y-P1.y;

	if      (xDiff == 0) return (float)(fabs(P3.x-P1.x));
	else if (yDiff == 0) return (float)(fabs(P3.y-P1.y));

	float m    = (float)(yDiff/xDiff);
	float dist = (float)(fabs((-m*P3.x + P3.y + m*P1.x - P1.y)/sqrt(m*m + 1)));

	return dist;
}
int CFM2xLineDefect::OnExecute(TShape Shape)
{
	FShape = Shape;
	return OnExecute();
}
int CFM2xLineDefect::OnExecute()
{
	int Index = 0;
	// ���� LineEdge ������ ����Cell���� �������� ������ �˻����� �����ؾ���   
	// NOTCH CELL �˻� ������ 
	if (FShape==shRectH) Index = 0;
	if (FShape==shRectS) Index = 1;
	if (FShape==shRectC) Index = 2;
	// ������������ Cell���°� ������ ��� Company �߰��ؾ��� 
	bool l  = false;
	bool t  = false;
	bool r  = false;
	bool b  = false;
	bool lt = false;
	bool rt = false;
	bool rb = false;
	bool lb = false;

	switch (Index) {

	case 0 :
		// RectH ���ڸ�� 
		//------------------------------------------------------------------
		l  = FLeftWall    = true ;
		t  = FTopWall     = true ;
		r  = FRightWall   = true ;
		b  = FBottomWall  = false;
		lt = FLeftTop     = true ;
		rt = FRightTop    = true ;
		rb = FRightBottom = false;
		lb = FLeftBottom  = false;
		break;

	case 1 :
		// RectS : ������ ���
		//------------------------------------------------------------------
		l  = FLeftWall    = true ;
		t  = FTopWall     = false;
		r  = FRightWall   = true ;
		b  = FBottomWall  = false;
		lt = FLeftTop     = false;
		rt = FRightTop    = false;
		rb = FRightBottom = false;
		lb = FLeftBottom  = false;
		break;

	case 2 :
		// RectC : �Ÿ��
		//------------------------------------------------------------------
		l  = FLeftWall    = true ;
		t  = FTopWall     = false;
		r  = FRightWall   = true ;
		b  = FBottomWall  = true ;
		lt = FLeftTop     = false;
		rt = FRightTop    = false;
		rb = FRightBottom = true ;
		lb = FLeftBottom  = true ;
		break;
	}
	if(!OnExecute(l, t, r, b, lt, rt, rb, lb)) return false;
	return true;
}
CRect CFM2xLineDefect::OnMerge( CRect ar, CRect br)
{
	CRect r;
	r.left   = (ar.left   < br.left  ) ? ar.left   : br.left  ;
	r.top    = (ar.top    < br.top   ) ? ar.top    : br.top   ;
	r.right  = (ar.right  > br.right ) ? ar.right  : br.right ;
	r.bottom = (ar.bottom > br.bottom) ? ar.bottom : br.bottom;

	return r;
}
/////////////////////////////////////////////////////////////////////////////////////
int CFM2xLineDefect::GetSize(TCorner *lpCorner,TBlob *lpDefect, int Count, int Dv)
{
	bool IsRoof  = false;
	bool IsStart = false;
	CRect Rect;
	Rect.left   = 0;
	Rect.right  = 0;
	Rect.top    = 0;
	Rect.bottom = 0;
	int DefectCount=0;
	int DefectPoint=0;
	const double MinDv =2;
	int PixelValue  =0;
	int PixelCount  =0;
	int PixelPoint  =0;
	int PixelScore  =0;
	float Score     =0.0f;

	// Defect Data �������� �Ǵ��� 
	for(int i=0; i<Count; i++){
		if(lpCorner[i].Dv>Dv){
			IsStart = true;
			break;
		}
	}

	if(IsStart){	
		for (register int k=0; k<Count; k++) {
			if ( lpCorner[k].Dv >1               ) { IsRoof = true; }
			if ( IsRoof && lpCorner[k].Dv <=MinDv) { 
				if(DefectPoint>3){
					lpDefect[DefectCount].Height = (Rect.bottom - Rect.top);
					lpDefect[DefectCount].Width  = (Rect.right - Rect.left);

					// ���� Pixel Data ���� 
					PixelCount =0; PixelPoint=0; 
					for(int n=Rect.top+1 ; n<Rect.bottom-1; n++){
						for(int m=Rect.left+1; m<Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < ECThresHold) PixelCount++;
							PixelPoint++;
						}
					}
					Score  = (((float)PixelCount/(float)PixelPoint)*100);
					if(Score>10){
						lpDefect[DefectCount].P.x    =  (float)Rect.left;
						lpDefect[DefectCount].P.y    =  (float)(Rect.top +ImageHeight);
						lpDefect[DefectCount].Round  =  PixelCount;
						DefectCount++;
					}
				}
				DefectPoint=0;
				IsRoof = false; 
			}

			if (IsRoof) {
				Rect.left   = Rect.left  <lpCorner[k].P.x && Rect.left!=0 ? Rect.left   :(long) lpCorner[k].P.x;
				Rect.top    = Rect.top   <lpCorner[k].P.y && Rect.top !=0 ? Rect.top    :(long) lpCorner[k].P.y;
				Rect.right  = Rect.right >lpCorner[k].P.x                 ? Rect.right  :(long) lpCorner[k].P.x;
				Rect.bottom = Rect.bottom>lpCorner[k].P.y                 ? Rect.bottom :(long) lpCorner[k].P.y;
				DefectPoint++;
			}
			else {
				Rect.left   = 0;
				Rect.top    = 0;
				Rect.right  = 0;
				Rect.bottom = 0;
			}

		}
	}
	return DefectCount;
}
int CFM2xLineDefect::GetCircleCrackSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dis, double CirDiameter, TFPoint Center, bool IsInCircle)
{
	bool IsRoof   = false;
	bool IsStart  = false;
	bool IsFlagCh = true ; 
	CRect Rect;
	Rect.left   = 0;
	Rect.right  = 0;
	Rect.top    = 0;
	Rect.bottom = 0;
	int DefectCount=0;
	int DefectPoint=0;
	// 	const double MinDv =0.5;
	int MinDis = Dis;
	int PixelValue  =0;
	int PixelCount  =0;
	int PixelPoint  =0;
	int PixelScore  =0;
	float Score     =0.0f;

	int DefectWidth  =0;
	int DefectHeight =0;
	double MaxDis    =0.0f;

	TFPoint StaPos(0.0f, 0.0f);
	TFPoint EndPos(0.0f, 0.0f);
	TFPoint MaxPos(0.0f, 0.0f);
	int     MaxCount=0;
	float   PointDis =0.0f;

	// �� �ڸ��� �ݿ��� �ڸ� ���� ���� �ʿ��� ����  
	TFPoint LTPoint(0.0f, 0.0f);
	TFPoint RTPoint(0.0f, 0.0f);
	TFPoint LBPoint(0.0f, 0.0f);
	TFPoint RBPoint(0.0f, 0.0f);
	double CenDisLTPos =0.0f;
	double CenDisRTPos =0.0f;
	double CenDisLBPos =0.0f;
	double CenDisRBPos =0.0f;

	// Defect Data �������� �Ǵ�g 
	if(Count>5) IsStart = true;

	if(IsStart){	
		for (register int k=0; k<Count; k++) {
			if ( lpCircle[k].P.x!=0.0f && lpCircle[k].P.y!=0.0f && IsFlagCh) {   // 1�̻� ū �������� ������ ��....2����.....
				IsRoof = true;
				IsFlagCh = false ;
				StaPos.x = lpCircle[k].P.x;
				StaPos.y = lpCircle[k].P.y;
				PointDis =0.0f;
			} 
			if(k>1       ) PointDis = Math.GetLength(lpCircle[k-1].P,lpCircle[k].P);
			if(k==Count-1) PointDis = MinDis+1;

			if (IsRoof && PointDis >MinDis) { 
				// ���� Pixel Data ���� 
				PixelCount =0, PixelPoint=0, PixelScore=5, Score =0.0f;
				for(int n=Rect.top ; n<Rect.bottom; n++){
					for(int m=Rect.left; m<Rect.right; m++){
						PixelValue = IElement(m,n);
						if(PixelValue > BrokenThresHold) PixelCount++;
						PixelPoint++;
					}
				}
				EndPos.x = lpCircle[k].P.x;
				EndPos.y = lpCircle[k].P.y;

				Score  = (((float)PixelCount/(float)PixelPoint)*100);
				// Circle Defect ������ PixelCount ������ üũ�Ѵ� => ����ų ���� ���� 
				if(Score>PixelScore){
					// 						MaxPos.x = lpCircle[MaxCount].P.x;
					// 						MaxPos.y = lpCircle[MaxCount].P.y;
					// �� �ڸ��� �ݿ��� �ڸ� ��������
					LTPoint.x = (float)Rect.left ;  LTPoint.y = (float)Rect.top   ;
					RTPoint.x = (float)Rect.right;  RTPoint.y = (float)Rect.top   ;
					LBPoint.x = (float)Rect.left ;  LBPoint.y = (float)Rect.bottom;
					RBPoint.x = (float)Rect.right;  RBPoint.y = (float)Rect.bottom;

					CenDisLTPos =0.0f;
					CenDisRTPos =0.0f;
					CenDisLBPos =0.0f;
					CenDisRBPos =0.0f;

					// �ɹڸ�(Peeling) vs �ݿ����ڸ�(Broken)���� ������ 
					if(CirDiameter!=0.0f){
						if(IsInCircle){ // Cir1, Cir2, Cir5, Cir6
							CenDisLTPos = Math.GetLength(Center,LTPoint);
							CenDisRTPos = Math.GetLength(Center,RTPoint);
							CenDisLBPos = Math.GetLength(Center,LBPoint);						
							CenDisRBPos = Math.GetLength(Center,RBPoint);		
							if(CenDisLTPos>=CirDiameter || CenDisRTPos>=CirDiameter || CenDisLBPos>=CirDiameter || CenDisRBPos>=CirDiameter) lpDefect[DefectCount].LineAttach =1;
						}
						else          {// Cir3, Cir4 
							CenDisLTPos = Math.GetLength(Center,LTPoint);
							CenDisRTPos = Math.GetLength(Center,RTPoint);
							CenDisLBPos = Math.GetLength(Center,LBPoint);						
							CenDisRBPos = Math.GetLength(Center,RBPoint);		
							if(CenDisLTPos<=CirDiameter || CenDisRTPos<=CirDiameter || CenDisLBPos<=CirDiameter || CenDisRBPos<=CirDiameter) lpDefect[DefectCount].LineAttach =1;
						}
					}

					lpDefect[DefectCount].Rect.left    = Rect.left  ;
					lpDefect[DefectCount].Rect.top     = Rect.top   ;
					lpDefect[DefectCount].Rect.right   = Rect.right ; 
					lpDefect[DefectCount].Rect.bottom  = Rect.bottom;

					lpDefect[DefectCount].Height       = (Rect.bottom - Rect.top );
					lpDefect[DefectCount].Width        = (Rect.right  - Rect.left);

					// 					lpDefect[DefectCount].Ver_Dis      = lpCircle[MaxCount].Dv        ;
					// 					lpDefect[DefectCount].Hor_Dis      = Math.GetLength(StaPos,EndPos);

					lpDefect[DefectCount].Ver_Dis      = lpDefect[DefectCount].Height;
					lpDefect[DefectCount].Hor_Dis      = lpDefect[DefectCount].Width ;

					lpDefect[DefectCount].P.x       =  (float) Rect.left;
					lpDefect[DefectCount].P.y       =  (float)(Rect.top +ImageHeight);

					lpDefect[DefectCount].Rect.top     = lpDefect[DefectCount].P.y;
					lpDefect[DefectCount].Rect.bottom  = lpDefect[DefectCount].Rect.top+lpDefect[DefectCount].Height;
					lpDefect[DefectCount].Round        =  PixelCount;
					DefectCount++;
				}
				DefectPoint=0   ;
				IsRoof   = false; 
				IsFlagCh = true ;
				MaxDis   =0.0f  ;
				MaxCount =0     ; 
			}

			if (IsRoof) { // Bright Point ���� Rect���� 
				// 				if(lpCircle[k].Dv>MaxDis){
				// 					MaxDis   = lpCircle[k].Dv;
				// 					MaxCount = k             ;
				// 				}
				Rect.left   = Rect.left  <lpCircle[k].P.x && Rect.left!=0 ? Rect.left   :(long) lpCircle[k].P.x;
				Rect.top    = Rect.top   <lpCircle[k].P.y && Rect.top !=0 ? Rect.top    :(long) lpCircle[k].P.y;
				Rect.right  = Rect.right >lpCircle[k].P.x                 ? Rect.right  :(long) lpCircle[k].P.x;
				Rect.bottom = Rect.bottom>lpCircle[k].P.y                 ? Rect.bottom :(long) lpCircle[k].P.y;
				DefectPoint++;
			}
			else {
				Rect.left   = 0;
				Rect.top    = 0;
				Rect.right  = 0;
				Rect.bottom = 0;
			}

		}
	}
	return DefectCount;
}
int CFM2xLineDefect::GetCircleSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dv) //Mark_Mode Dark Defect 
{
	bool IsRoof   = false;
	bool IsStart  = false;
	bool IsFlagCh = true ; 
	CRect Rect;
	Rect.left   = 0;
	Rect.right  = 0;
	Rect.top    = 0;
	Rect.bottom = 0;
	int DefectCount=0;
	int DefectPoint=0;
	// 	const double MinDv =0.5;
	const double MinDv =Dv/2;
	int PixelValue  =0;
	int PixelCount  =0;
	int PixelPoint  =0;
	int PixelScore  =0;
	float Score     =0.0f;

	int DefectWidth  =0;
	int DefectHeight =0;
	double MaxDis    =0.0f;

	TFPoint StaPos(0.0f, 0.0f);
	TFPoint EndPos(0.0f, 0.0f);
	TFPoint MaxPos(0.0f, 0.0f);
	int     MaxCount=0;

	// Defect Data �������� �Ǵ��� 
	for(int i=0; i<Count; i++){
		if(lpCircle[i].Dv>Dv){
			IsStart = true;
			break;
		}
	}

	if(IsStart){	
		for (register int k=0; k<Count; k++) {
			if ( lpCircle[k].Dv >1 && IsFlagCh) {   // 1�̻� ū �������� ������ ��....2����.....
				IsRoof = true;
				IsFlagCh = false ;
				StaPos.x = lpCircle[k].P.x;
				StaPos.y = lpCircle[k].P.y;
			} 

			if ( IsRoof && lpCircle[k].Dv <=1 && DefectPoint>3) { 
				if(DefectPoint>3){
					// ���� Pixel Data ���� 
					PixelCount =0, PixelPoint=0, PixelScore=45, Score =0.0f;
					for(int n=Rect.top+1 ; n<Rect.bottom-1; n++){
						for(int m=Rect.left+1; m<Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
							PixelPoint++;
						}
					}
					EndPos.x = lpCircle[k].P.x;
					EndPos.y = lpCircle[k].P.y;

					Score  = (((float)PixelCount/(float)PixelPoint)*100);
					// Circle Defect ������ PixelCount ������ üũ�Ѵ� => ����ų ���� ���� 
					if(Score>PixelScore){
						MaxPos.x = lpCircle[MaxCount].P.x;
						MaxPos.y = lpCircle[MaxCount].P.y;

						lpDefect[DefectCount].Rect.left    = Rect.left  ;
						lpDefect[DefectCount].Rect.top     = Rect.top   ;
						lpDefect[DefectCount].Rect.right   = Rect.right ; 
						lpDefect[DefectCount].Rect.bottom  = Rect.bottom;

						lpDefect[DefectCount].Height      = (Rect.bottom - Rect.top );
						lpDefect[DefectCount].Width       = (Rect.right  - Rect.left);

						lpDefect[DefectCount].Ver_Dis      = lpCircle[MaxCount].Dv        ;
						lpDefect[DefectCount].Hor_Dis      = Math.GetLength(StaPos,EndPos);

						lpDefect[DefectCount].P.x       =  (float)Rect.left;
						lpDefect[DefectCount].P.y       =  (float)(Rect.top +ImageHeight);

						lpDefect[DefectCount].Rect.top     = lpDefect[DefectCount].P.y;
						lpDefect[DefectCount].Rect.bottom  = lpDefect[DefectCount].Rect.top+lpDefect[DefectCount].Height;

						lpDefect[DefectCount].Round  =  PixelCount;
						DefectCount++;
					}
				}
				DefectPoint=0   ;
				IsRoof   = false; 
				IsFlagCh = true ;
				MaxDis   =0.0f  ;
				MaxCount =0     ; 
			}

			if (IsRoof) {
				if(lpCircle[k].Dv>MaxDis){
					MaxDis   = lpCircle[k].Dv;
					MaxCount = k             ;
				}
				Rect.left   = Rect.left  <lpCircle[k].P.x && Rect.left!=0 ? Rect.left   :(long) lpCircle[k].P.x;
				Rect.top    = Rect.top   <lpCircle[k].P.y && Rect.top !=0 ? Rect.top    :(long) lpCircle[k].P.y;
				Rect.right  = Rect.right >lpCircle[k].P.x                 ? Rect.right  :(long) lpCircle[k].P.x;
				Rect.bottom = Rect.bottom>lpCircle[k].P.y                 ? Rect.bottom :(long) lpCircle[k].P.y;
				DefectPoint++;

				// 				if(DefectPoint>10 && IsFlagCh){
				// 					PixelCount =0, PixelPoint=0, PixelScore=15, Score =0.0f;
				// 					for(int n=Rect.top ; n<Rect.bottom; n++){
				// 						for(int m=Rect.left; m<Rect.right ; m++){
				// 							PixelValue = IElement(m,n);
				// 							if(PixelValue < EnCapBtmThresHold) PixelCount++;
				// 							PixelPoint++;
				// 						}
				// 					}
				// 					Score  = (((float)PixelCount/(float)PixelPoint)*100);
				// 					if(Score<PixelScore){
				// 						Rect.left   = 0;
				// 						Rect.top    = 0;
				// 						Rect.right  = 0;
				// 						Rect.bottom = 0;
				// 						DefectPoint = 0;
				// 						IsFlagCh    = true;
				// 					}
				// 					else IsFlagCh = false;
				// 				}
			}
			else {
				Rect.left   = 0;
				Rect.top    = 0;
				Rect.right  = 0;
				Rect.bottom = 0;
			}

		}
	}
	return DefectCount;
}
int CFM2xLineDefect::GetPolygonSize(TFPoint *PolygonData, TBlob *lpDefect,int TotalCount,int Range, int DefectDv)// Polygon Mode Dark Defect 
{
	bool IsRoof   = false;
	bool IsBadRect= false;
	CRect Rect;
	Rect.left   = 0;
	Rect.right  = 0;
	Rect.top    = 0;
	Rect.bottom = 0;

	int DefectCount=0;
	int DefectPoint=0;

	// Defect ����� ���� Quantity Score Calc 
	int PixelValue   =0;
	int PixelCount   =0;
	int PixelPoint   =0;
	int PixelScore   =0;
	float Score      =0.0f;

	int BadRectCount  =0;

	// Defect Merge Value
	const int MgCount=3 ;
	const int xMerge =10;
	const int yMerge =10;

	// 3���� ���� Point ��ǥ 
	TFPoint f1(0.0f,0.0f), f2(0.0f, 0.0f), f3(0.0f,0.0f);

	// Polygon Vlaue 
	TFPoint *RangePoint     =NULL;
	float Dv1 =0.0f, Dv2=0.0F, Dv3=0.0f;

	//BadRect Init 
	for(int k=0; k<MAX_POLYGON_DEFECT_COUNT; k++){
		// Polygon Defect Data 
		BadRect[k].Rect.left          =0   ;
		BadRect[k].Rect.top           =0   ;
		BadRect[k].Rect.right         =0   ;
		BadRect[k].Rect.bottom        =0   ;
		BadRect[k].SPoint.x           =0.0f;
		BadRect[k].SPoint.y           =0.0f;
		BadRect[k].EPoint.x           =0.0f;
		BadRect[k].EPoint.y           =0.0f;
		BadRect[k].BadPixelCount      =0   ;

		for(int i=0; i<MAX_POLYGON_DEFECT_POINT; i++){
			BadRect[k].Pixel[i].x =0.0f;
			BadRect[k].Pixel[i].y =0.0f;
		}
	}

	// Range������ŭ Polygon Data Point ��Ȱ�Ͽ� Defect ���� Calc
	for(int k=0; k<TotalCount; k+=Range){
		TFPoint *Q1 = &PolygonData[k];
		int v = (TotalCount-k)/Range;  // Range������ŭ ��Ȱ�� ������ ��
		int r = TotalCount%Range    ;  // ��ü �ȼ��� Range�������� ��Ȱ�� Range�������� ���������� ������� 
		int n = (v>=1)? Range: r    ;  // ��ü �ȼ��� Range�������� ��Ȱ�� Range�������� ���������� ������� 

		if(v<0) break;

		// Range ������ŭ ��Ȱ�Ͽ� ���� �����Ѵ� 
		if(RangePoint!=NULL) delete [] RangePoint;
		RangePoint = new TFPoint[n];

		// Range�������� ���� Polygon Pos ��ǥ �� BadPixel Pos��ǥ Init 
		for(int z=0; z<n; z++){
			RangePoint[z].x    = Q1[z].x;
			RangePoint[z].y    = Q1[z].y;
		}

		// Find Defct Pixel 
		f1 = RangePoint[0         ]; //Range�������� ù��° Point ��ǥ
		f2 = RangePoint[(int)(n/2)]; //Range�������� ��� Point ��ǥ
		f3 = RangePoint[n-1       ]; //Range�������� ������ Point ��ǥ

		// Rect Data Init 
		Rect.left   =0;
		Rect.top    =0;
		Rect.right  =0;
		Rect.bottom =0;

		IsBadRect  = false;
		PixelCount =0     ;
		for(int i=0; i<n; i++){ // Range�������� 3�� Point���� BadRect,BadPixel ������ �����Ѵ� 
			Dv1 = GetLengthFrLineToPoint(f1, f3, RangePoint[i]);
			Dv2 = GetLengthFrLineToPoint(f1, f2, RangePoint[i]);
			Dv3 = GetLengthFrLineToPoint(f2, f3, RangePoint[i]);

			if(Dv1>DefectDv || Dv2>DefectDv || Dv3>DefectDv){ // Defect Point �����Ұ�� 
				if(!IsBadRect){ // Defect Point StaPos Data ���� ����
					IsBadRect = true;
					BadRect[BadRectCount].SPoint.x  = RangePoint[i].x;
					BadRect[BadRectCount].SPoint.y  = RangePoint[i].y;
				}

				if(IsBadRect){
					Rect.left   = Rect.left  <RangePoint[i].x && Rect.left!=0 ? Rect.left   :(long) RangePoint[i].x;
					Rect.top    = Rect.top   <RangePoint[i].y && Rect.top !=0 ? Rect.top    :(long) RangePoint[i].y;
					Rect.right  = Rect.right >RangePoint[i].x                 ? Rect.right  :(long) RangePoint[i].x;
					Rect.bottom = Rect.bottom>RangePoint[i].y                 ? Rect.bottom :(long) RangePoint[i].y;

					BadRect[BadRectCount].Pixel[BadRect[BadRectCount].BadPixelCount] = RangePoint[i]  ;
					BadRect[BadRectCount].EPoint.x                                   = RangePoint[i].x; // End Pos ���� ������Ʈ 
					BadRect[BadRectCount].EPoint.y                                   = RangePoint[i].y; // End Pos ���� ������Ʈ 
					BadRect[BadRectCount].BadPixelCount++                                             ;
				}
			}
		}

		if(IsBadRect){
			BadRect[BadRectCount].Rect = Rect;

			float Distance=0.0f;

			if(BadRectCount>0){ // �ι�° BadRect���� ���� BadRect�� ���Ͽ� �Ÿ��� ���� 
				Distance = Math.GetLength(BadRect[BadRectCount].SPoint, BadRect[BadRectCount-1].EPoint);
			}

			// ���� BadRect���� �����Ÿ� �����̸� ����BadRect�����Ϳ� Merge�Ѵ� 
			if(((Rect.right - Rect.left)>2) && ((Rect.bottom- Rect.top)>2) && Distance && (Distance<20)){
				BadRect[BadRectCount-1].Rect.left   = BadRect[BadRectCount-1].Rect.left  <Rect.left   ? BadRect[BadRectCount-1].Rect.left   : Rect.left  ;
				BadRect[BadRectCount-1].Rect.top    = BadRect[BadRectCount-1].Rect.top   <Rect.top    ? BadRect[BadRectCount-1].Rect.top    : Rect.top   ;
				BadRect[BadRectCount-1].Rect.right  = BadRect[BadRectCount-1].Rect.right >Rect.right  ? BadRect[BadRectCount-1].Rect.right  : Rect.right ;
				BadRect[BadRectCount-1].Rect.bottom = BadRect[BadRectCount-1].Rect.bottom>Rect.bottom ? BadRect[BadRectCount-1].Rect.bottom : Rect.bottom;

				BadRect[BadRectCount-1].EPoint = BadRect[BadRectCount].EPoint;

				for(int b=0; b<BadRect[BadRectCount].BadPixelCount; b++){ // ���� BadPixelData���� ���� BadRect�� �Ѱ��ش� 
					BadRect[BadRectCount-1].Pixel[BadRect[BadRectCount-1].BadPixelCount] = BadRect[BadRectCount].Pixel[b];
					BadRect[BadRectCount-1].BadPixelCount++;

					// ���� Data Init 
					BadRect[BadRectCount].Pixel[b].x =0;
					BadRect[BadRectCount].Pixel[b].y =0;
				}
				BadRect[BadRectCount].BadPixelCount =0;
				BadRect[BadRectCount].EPoint.x      =0;
				BadRect[BadRectCount].EPoint.y      =0;
				BadRect[BadRectCount].SPoint.x      =0;
				BadRect[BadRectCount].SPoint.y      =0;
			}
			else  BadRectCount++;
		}
	}

	// Range ������ŭ ��Ȱ�Ͽ� ���� �����Ѵ� 
	// Range �������� ������ ������ ���� 
	if(RangePoint!=NULL) delete [] RangePoint;

	// BadRect Data ���� 1�� ���͸� �� ���� lpDefect�� ������ �Ѱ���
	if(BadRectCount>0){
		TFPoint Target(0.0f,0.0f);

		for(int k=0; k<BadRectCount; k++){
			IsRoof = false;
			for(int i=0; i<BadRect[k].BadPixelCount; i++){
				f1     = BadRect[k].SPoint  ; // BadRect ������ ��ǥ
				f2     = BadRect[k].EPoint  ; // BadRect ������ ��ǥ 
				Target = BadRect[k].Pixel[i]; // BddRect ����   ��ǥ 
				Dv1    = GetLengthFrLineToPoint(f1, f2, Target);

				if(Dv1>3){
					if(!IsRoof){ // Defect Point StaPos Data ���� ����
						IsRoof = true;
					}

					if(IsRoof){
						Rect.left   = Rect.left  <Target.x && Rect.left!=0 ? Rect.left   :(long) Target.x;
						Rect.top    = Rect.top   <Target.y && Rect.top !=0 ? Rect.top    :(long) Target.y;
						Rect.right  = Rect.right >Target.x                 ? Rect.right  :(long) Target.x;
						Rect.bottom = Rect.bottom>Target.y                 ? Rect.bottom :(long) Target.y;
						DefectPoint++;
					}
				}
				else                                            { // Defect Point ������� 
					if(IsRoof){
						if(DefectPoint>10){
							// ���� Pixel Data ���� 
							PixelCount =0, PixelPoint=0, PixelScore=5, Score =0.0f;
							for(int n=Rect.top ; n<Rect.bottom; n++){
								for(int m=Rect.left; m<Rect.right; m++){
									PixelValue = IElement(m,n);
									if(PixelValue < EnCapBtmThresHold) PixelCount++;
									PixelPoint++;
								}
							}
							Score  = (((float)PixelCount/(float)PixelPoint)*100);
							// Circle Defect ������ PixelCount ������ üũ�Ѵ� => ����ų ���� ���� 
							if(Score>PixelScore){
								lpDefect[DefectCount].Rect.left    = Rect.left  ;
								lpDefect[DefectCount].Rect.top     = Rect.top   ;
								lpDefect[DefectCount].Rect.right   = Rect.right ; 
								lpDefect[DefectCount].Rect.bottom  = Rect.bottom;

								lpDefect[DefectCount].Height       = (Rect.bottom - Rect.top );
								lpDefect[DefectCount].Width        = (Rect.right  - Rect.left);

								lpDefect[DefectCount].Ver_Dis      = lpDefect[DefectCount].Height;
								lpDefect[DefectCount].Hor_Dis      = lpDefect[DefectCount].Width ;

								lpDefect[DefectCount].P.x          =  (float)(Rect.left            );
								lpDefect[DefectCount].P.y          =  (float)(Rect.top +ImageHeight); // ��ü Frame���� Real��ǥ �������� ��ȯ 

								lpDefect[DefectCount].Rect.top     = lpDefect[DefectCount].P.y;
								lpDefect[DefectCount].Rect.bottom  = lpDefect[DefectCount].Rect.top+lpDefect[DefectCount].Height;
								lpDefect[DefectCount].Round        =  PixelCount;
								DefectCount++;

								// Data Init 
								DefectPoint = 0;
								Rect.left   = 0;
								Rect.top    = 0;
								Rect.right  = 0;
								Rect.bottom = 0;
								IsRoof      = false; 
							}
							else {
								DefectPoint = 0;
								Rect.left   = 0;
								Rect.top    = 0;
								Rect.right  = 0;
								Rect.bottom = 0;
								IsRoof      = false; 
							}
						}
						else { // Defect Count ������� �ʱ�ȭ 
							DefectPoint = 0;
							Rect.left   = 0;
							Rect.top    = 0;
							Rect.right  = 0;
							Rect.bottom = 0;
							IsRoof      = false; 
						}
					}
					else      { // �ּ� Defect Distance ������� 
						Rect.left   = 0;
						Rect.top    = 0;
						Rect.right  = 0;
						Rect.bottom = 0;
					}
				}
			}
		}
	}

	// Defect Data���� Merge ������ 
	if(DefectCount>1){ // Defect������ �ּ� 2���̻� �����Ұ�� Merge ������ ������
		bool IsxMg=false, IsyMg=false, IsInMg=false;
		CRect r1, r2;
		int sort =0;
		int tmpDDefectCount = DefectCount;

		for(int k=0; k<tmpDDefectCount; k++){
			if(lpDefect[k].Rect.left==0 && lpDefect[k].Rect.top==0 && lpDefect[k].Rect.right==0 && lpDefect[k].Rect.bottom==0) continue;

			for(int l=k+1; l<k+MgCount; l++ ){
				IsxMg = false; IsyMg = false;
				if(l>(tmpDDefectCount-1)) break;
				if(lpDefect[l].Rect.left==0 && lpDefect[l].Rect.top==0 && lpDefect[k].Rect.right==0 && lpDefect[k].Rect.bottom==0) continue;

				r1 = lpDefect[k].Rect;
				r2 = lpDefect[l].Rect; 

				register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
				register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
				IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

				if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

				IsxMg = (HorDist<xMerge) ? true: false;
				IsyMg = (VerDist<yMerge) ? true: false;

				if((IsxMg && IsyMg) || IsInMg){
					lpDefect[k].Round    += lpDefect[l].Round;
					lpDefect[k].Rect      = OnMerge(lpDefect[k].Rect, lpDefect[l].Rect);
					lpDefect[k].Width     = lpDefect[k].Rect.right  - lpDefect[k].Rect.left;
					lpDefect[k].Height    = lpDefect[k].Rect.bottom - lpDefect[k].Rect.top ;
					lpDefect[k].Hor_Dis   = lpDefect[k].Width ; 
					lpDefect[k].Ver_Dis   = lpDefect[k].Height;
					lpDefect[k].P.x       = (float)(lpDefect[k].Rect.left);
					lpDefect[k].P.y       = (float)(lpDefect[k].Rect.top ); 
					lpDefect[l].Rect.left = lpDefect[l].Rect.top = lpDefect[l].Rect.right = lpDefect[l].Rect.bottom =0;
					DefectCount--;
					k=-1;
					break;
				}
			}
		}
		for(int j=0; j<tmpDDefectCount; j++){
			if(lpDefect[j].Rect.left==0 && lpDefect[j].Rect.top==0 && lpDefect[j].Rect.right==0 && lpDefect[j].Rect.bottom==0) continue;
			lpDefect[sort] = lpDefect[j];
			sort++;
		}
	}
	return DefectCount;
}
int CFM2xLineDefect::GetAvgCellSize(int TopCount, int MidCount, int BtmCount, int AvgMode)
{
	if(MarkStrPos.x==0 || MarkStrPos.y==0 || MarkEndPos.x==0 || MarkEndPos.y==0) return false;

	int nTotalCellSize =0;
	int nCellSize      =0;
	int nAvgCellSize   =0;


	int nCellSizeCount =0;

	if     (AvgMode==0){  // TopCellSizeAvg
		for(int k1=0; k1<TopCount; k1++){
			if(FCellPoint[k1].x==0 || FCellPoint[k1].y==0) continue;
			nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkStrPos, MarkEndPos, FCellPoint[k1]));     
			nTotalCellSize +=nCellSize;
			nCellSizeCount++;
		}
		if(nTotalCellSize>0 && nCellSizeCount>0) nAvgCellSize = nTotalCellSize/nCellSizeCount;
	}
	else if(AvgMode==1){  // MidCellSizeAvg
		for(int k2=TopCount; k2<TopCount+MidCount; k2++){
			if(FCellPoint[k2].x==0 || FCellPoint[k2].y==0) continue;
			nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkStrPos, MarkEndPos, FCellPoint[k2]));     
			nTotalCellSize +=nCellSize;
			nCellSizeCount++;
		}
		if(nTotalCellSize>0 && nCellSizeCount>0) nAvgCellSize = nTotalCellSize/nCellSizeCount;

	}
	else if(AvgMode==2){  // BtmCellSizeAvg
		for(int k3=TopCount+MidCount; k3<TopCount+MidCount+BtmCount; k3++){
			if(FCellPoint[k3].x==0 || FCellPoint[k3].y==0) continue;
			nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkStrPos, MarkEndPos, FCellPoint[k3]));     
			nTotalCellSize +=nCellSize;
			nCellSizeCount++;
		}
		if(nTotalCellSize>0 && nCellSizeCount>0) nAvgCellSize = nTotalCellSize/nCellSizeCount;

	}
	else if(AvgMode==3){  // AllCellSizeAvg
		for(int k4=0; k4<TopCount+MidCount+BtmCount; k4++){
			if(FCellPoint[k4].x==0 || FCellPoint[k4].y==0) continue;
			nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkPos, MarkEndPos, FCellPoint[k4]));     
			nTotalCellSize +=nCellSize;
			nCellSizeCount++;
		}
		if(nTotalCellSize>0 && nCellSizeCount>0) nAvgCellSize = nTotalCellSize/nCellSizeCount;
	}
	return  nAvgCellSize;
}
int CFM2xLineDefect::GetCellSize(int Count)
{
	if(MarkStrPos.x==0 || MarkStrPos.y==0 || MarkEndPos.x==0 || MarkEndPos.y==0) return false;
	if( FCellPoint[Count].x==0 || FCellPoint[Count].y==0)                        return false;
	if(Count>=MAX_DEFECT_POINT)                                                  return false;

	int nCellSize      =0;

	nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkStrPos, MarkEndPos, FCellPoint[Count]));     
	return  nCellSize;
}
void CFM2xLineDefect::OnClearData()
{
	for(int k=0; k<MAX_DEFECT_POINT; k++){
		// Cell Inspection Init
		// Dark Defect 
		ADefectPoint[k].Height    =0;
		ADefectPoint[k].Width     =0;
		ADefectPoint[k].Round     =0;
		ADefectPoint[k].P.x       =0;
		ADefectPoint[k].P.y       =0;
		ADefectPoint[k].Ver_Dis   =0;
		ADefectPoint[k].Hor_Dis   =0;
		ADefectPoint[k].LineAttach=0;

		BDefectPoint[k].Height    =0;
		BDefectPoint[k].Width     =0;
		BDefectPoint[k].Round     =0;
		BDefectPoint[k].P.x       =0;
		BDefectPoint[k].P.y       =0;
		BDefectPoint[k].Ver_Dis   =0;
		BDefectPoint[k].Hor_Dis   =0;
		BDefectPoint[k].LineAttach=0;

		CDefectPoint[k].Height    =0;
		CDefectPoint[k].Width     =0;
		CDefectPoint[k].Round     =0;
		CDefectPoint[k].P.x       =0;
		CDefectPoint[k].P.y       =0;
		CDefectPoint[k].Ver_Dis   =0;
		CDefectPoint[k].Hor_Dis   =0;
		CDefectPoint[k].LineAttach=0;


		DDefectPoint[k].Height    =0;
		DDefectPoint[k].Width     =0;
		DDefectPoint[k].Round     =0;
		DDefectPoint[k].P.x       =0;
		DDefectPoint[k].P.y       =0;
		DDefectPoint[k].Ver_Dis   =0;
		DDefectPoint[k].Hor_Dis   =0;
		DDefectPoint[k].LineAttach=0;


		// Bright Defect 
		ACrackPoint[k].Height    =0;
		ACrackPoint[k].Width     =0;
		ACrackPoint[k].Round     =0;
		ACrackPoint[k].P.x       =0;
		ACrackPoint[k].P.y       =0;
		ACrackPoint[k].Ver_Dis   =0;
		ACrackPoint[k].Hor_Dis   =0;
		ACrackPoint[k].LineAttach=0;

		BCrackPoint[k].Height    =0;
		BCrackPoint[k].Width     =0;
		BCrackPoint[k].Round     =0;
		BCrackPoint[k].P.x       =0;
		BCrackPoint[k].P.y       =0;
		BCrackPoint[k].Ver_Dis   =0;
		BCrackPoint[k].Hor_Dis   =0;
		BCrackPoint[k].LineAttach=0;

		CCrackPoint[k].Height    =0;
		CCrackPoint[k].Width     =0;
		CCrackPoint[k].Round     =0;
		CCrackPoint[k].P.x       =0;
		CCrackPoint[k].P.y       =0;
		CCrackPoint[k].Ver_Dis   =0;
		CCrackPoint[k].Hor_Dis   =0;
		CCrackPoint[k].LineAttach=0;

		DCrackPoint[k].Height    =0;
		DCrackPoint[k].Width     =0;
		DCrackPoint[k].Round     =0;
		DCrackPoint[k].P.x       =0;
		DCrackPoint[k].P.y       =0;
		DCrackPoint[k].Ver_Dis   =0;
		DCrackPoint[k].Hor_Dis   =0;
		DCrackPoint[k].LineAttach=0;

		C1DefectPoint[k].Height    =0;
		C1DefectPoint[k].Width     =0;
		C1DefectPoint[k].Round     =0;
		C1DefectPoint[k].P.x       =0;
		C1DefectPoint[k].P.y       =0;
		C1DefectPoint[k].Ver_Dis   =0;
		C1DefectPoint[k].Hor_Dis   =0;
		C1DefectPoint[k].LineAttach=0;

		C2DefectPoint[k].Height    =0;
		C2DefectPoint[k].Width     =0;
		C2DefectPoint[k].Round     =0;
		C2DefectPoint[k].P.x       =0;
		C2DefectPoint[k].P.y       =0;
		C2DefectPoint[k].Ver_Dis   =0;
		C2DefectPoint[k].Hor_Dis   =0;
		C2DefectPoint[k].LineAttach=0;

		C3DefectPoint[k].Height    =0;
		C3DefectPoint[k].Width     =0;
		C3DefectPoint[k].Round     =0;
		C3DefectPoint[k].P.x       =0;
		C3DefectPoint[k].P.y       =0;
		C3DefectPoint[k].Ver_Dis   =0;
		C3DefectPoint[k].Hor_Dis   =0;
		C3DefectPoint[k].LineAttach=0;


		C4DefectPoint[k].Height    =0;
		C4DefectPoint[k].Width     =0;
		C4DefectPoint[k].Round     =0;
		C4DefectPoint[k].P.x       =0;
		C4DefectPoint[k].P.y       =0;
		C4DefectPoint[k].Ver_Dis   =0;
		C4DefectPoint[k].Hor_Dis   =0;
		C4DefectPoint[k].LineAttach=0;


		ABurrPoint[k].Height    =0;
		ABurrPoint[k].Width     =0;
		ABurrPoint[k].Round     =0;
		ABurrPoint[k].P.x       =0;
		ABurrPoint[k].P.y       =0;
		ABurrPoint[k].Ver_Dis   =0;
		ABurrPoint[k].Hor_Dis   =0;
		ABurrPoint[k].LineAttach=0;

		BBurrPoint[k].Height    =0;
		BBurrPoint[k].Width     =0;
		BBurrPoint[k].Round     =0;
		BBurrPoint[k].P.x       =0;
		BBurrPoint[k].P.y       =0;
		BBurrPoint[k].Ver_Dis   =0;
		BBurrPoint[k].Hor_Dis   =0;
		BBurrPoint[k].LineAttach=0;

		CBurrPoint[k].Height    =0;
		CBurrPoint[k].Width     =0;
		CBurrPoint[k].Round     =0;
		CBurrPoint[k].P.x       =0;
		CBurrPoint[k].P.y       =0;
		CBurrPoint[k].Ver_Dis   =0;
		CBurrPoint[k].Hor_Dis   =0;
		CBurrPoint[k].LineAttach=0;


		DBurrPoint[k].Height    =0;
		DBurrPoint[k].Width     =0;
		DBurrPoint[k].Round     =0;
		DBurrPoint[k].P.x       =0;
		DBurrPoint[k].P.y       =0;
		DBurrPoint[k].Ver_Dis   =0;
		DBurrPoint[k].Hor_Dis   =0;
		DBurrPoint[k].LineAttach=0;

		// Circle Defect Data Init
		// Dark Defect 
		A1CirDefectPoint[k].Height      =0;
		A1CirDefectPoint[k].Width       =0;
		A1CirDefectPoint[k].Round       =0;
		A1CirDefectPoint[k].P.x         =0;
		A1CirDefectPoint[k].P.y         =0;
		A1CirDefectPoint[k].Rect.left   =0;
		A1CirDefectPoint[k].Rect.top    =0;
		A1CirDefectPoint[k].Rect.right  =0;
		A1CirDefectPoint[k].Rect.bottom =0;
		A1CirDefectPoint[k].Ver_Dis     =0;
		A1CirDefectPoint[k].Hor_Dis     =0;
		A1CirDefectPoint[k].LineAttach  =0;

		A2CirDefectPoint[k].Height      =0;
		A2CirDefectPoint[k].Width       =0;
		A2CirDefectPoint[k].Round       =0;
		A2CirDefectPoint[k].P.x         =0;
		A2CirDefectPoint[k].P.y         =0;
		A2CirDefectPoint[k].Rect.left   =0;
		A2CirDefectPoint[k].Rect.top    =0;
		A2CirDefectPoint[k].Rect.right  =0;
		A2CirDefectPoint[k].Rect.bottom =0;
		A2CirDefectPoint[k].Ver_Dis     =0;
		A2CirDefectPoint[k].Hor_Dis     =0;
		A2CirDefectPoint[k].LineAttach  =0;

		C1CirDefectPoint[k].Height      =0;
		C1CirDefectPoint[k].Width       =0;
		C1CirDefectPoint[k].Round       =0;
		C1CirDefectPoint[k].P.x         =0;
		C1CirDefectPoint[k].P.y         =0;
		C1CirDefectPoint[k].Rect.left   =0;
		C1CirDefectPoint[k].Rect.top    =0;
		C1CirDefectPoint[k].Rect.right  =0;
		C1CirDefectPoint[k].Rect.bottom =0;
		C1CirDefectPoint[k].Ver_Dis     =0;
		C1CirDefectPoint[k].Hor_Dis     =0;
		C1CirDefectPoint[k].LineAttach  =0;

		C2CirDefectPoint[k].Height      =0;
		C2CirDefectPoint[k].Width       =0;
		C2CirDefectPoint[k].Round       =0;
		C2CirDefectPoint[k].P.x         =0;
		C2CirDefectPoint[k].P.y         =0;
		C2CirDefectPoint[k].Rect.left   =0;
		C2CirDefectPoint[k].Rect.top    =0;
		C2CirDefectPoint[k].Rect.right  =0;
		C2CirDefectPoint[k].Rect.bottom =0;
		C2CirDefectPoint[k].Ver_Dis     =0;
		C2CirDefectPoint[k].Hor_Dis     =0;
		C2CirDefectPoint[k].LineAttach  =0;

		// Bright Defect 
		A1CirCrackPoint[k].Height     =0;
		A1CirCrackPoint[k].Width      =0;
		A1CirCrackPoint[k].Round      =0;
		A1CirCrackPoint[k].P.x        =0;
		A1CirCrackPoint[k].P.y        =0;
		A1CirCrackPoint[k].Rect.left  =0;
		A1CirCrackPoint[k].Rect.top   =0;
		A1CirCrackPoint[k].Rect.right =0;
		A1CirCrackPoint[k].Rect.bottom=0;
		A1CirCrackPoint[k].Ver_Dis    =0;
		A1CirCrackPoint[k].Hor_Dis    =0;
		A1CirCrackPoint[k].LineAttach =0;

		A2CirCrackPoint[k].Height     =0;
		A2CirCrackPoint[k].Width      =0;
		A2CirCrackPoint[k].Round      =0;
		A2CirCrackPoint[k].P.x        =0;
		A2CirCrackPoint[k].P.y        =0;
		A2CirCrackPoint[k].Rect.left  =0;
		A2CirCrackPoint[k].Rect.top   =0;
		A2CirCrackPoint[k].Rect.right =0;
		A2CirCrackPoint[k].Rect.bottom=0;
		A2CirCrackPoint[k].Ver_Dis    =0;
		A2CirCrackPoint[k].Hor_Dis    =0;
		A2CirCrackPoint[k].LineAttach =0;

		C1CirCrackPoint[k].Height     =0;
		C1CirCrackPoint[k].Width      =0;
		C1CirCrackPoint[k].Round      =0;
		C1CirCrackPoint[k].P.x        =0;
		C1CirCrackPoint[k].P.y        =0;
		C1CirCrackPoint[k].Rect.left  =0;
		C1CirCrackPoint[k].Rect.top   =0;
		C1CirCrackPoint[k].Rect.right =0;
		C1CirCrackPoint[k].Rect.bottom=0;
		C1CirCrackPoint[k].Ver_Dis    =0;
		C1CirCrackPoint[k].Hor_Dis    =0;
		C1CirCrackPoint[k].LineAttach =0;

		C2CirCrackPoint[k].Height     =0;
		C2CirCrackPoint[k].Width      =0;
		C2CirCrackPoint[k].Round      =0;
		C2CirCrackPoint[k].P.x        =0;
		C2CirCrackPoint[k].P.y        =0;
		C2CirCrackPoint[k].Rect.left  =0;
		C2CirCrackPoint[k].Rect.top   =0;
		C2CirCrackPoint[k].Rect.right =0;
		C2CirCrackPoint[k].Rect.bottom=0;
		C2CirCrackPoint[k].Ver_Dis    =0;
		C2CirCrackPoint[k].Hor_Dis    =0;
		C2CirCrackPoint[k].LineAttach =0;

		FCellPoint[k].x         =0.0f;
		FCellPoint[k].y         =0.0f;
	}
	// TOP/BTM Broken Inspection StartLine

	// Line Position
	LinePosition = 0.0f;

	// Line Data 
	LeftLineData  =0.0f;
	RightLineData =0.0f;

	// NOTCH CELL INSPECTION LineData Init
	LeftOutLine [0].FP0.x = 0.0f;
	LeftOutLine [0].FP0.y = 0.0f;
	LeftOutLine [0].FP1.x = 0.0f;
	LeftOutLine [0].FP1.y = 0.0f;
	LeftInLine  [0].FP0.x = 0.0f;
	LeftInLine  [0].FP0.y = 0.0f;
	LeftInLine  [0].FP1.x = 0.0f;
	LeftInLine  [0].FP1.y = 0.0f;
	LeftStdLine [0].FP0.x = 0.0f;
	LeftStdLine [0].FP0.y = 0.0f;
	LeftStdLine [0].FP1.x = 0.0f;
	LeftStdLine [0].FP1.y = 0.0f;

	TopOutLine [0].FP0.x  = 0.0f;
	TopOutLine [0].FP0.y  = 0.0f;
	TopOutLine [0].FP1.x  = 0.0f;
	TopOutLine [0].FP1.y  = 0.0f;

	RightOutLine[0].FP0.x = 0.0f;
	RightOutLine[0].FP0.y = 0.0f;
	RightOutLine[0].FP1.x = 0.0f;
	RightOutLine[0].FP1.y = 0.0f;
	RightInLine [0].FP0.x = 0.0f;
	RightInLine [0].FP0.y = 0.0f;
	RightInLine [0].FP1.x = 0.0f;
	RightInLine [0].FP1.y = 0.0f;
	RightStdLine[0].FP0.x = 0.0f;
	RightStdLine[0].FP0.y = 0.0f;
	RightStdLine[0].FP1.x = 0.0f;
	RightStdLine[0].FP1.y = 0.0f;

	BtmOutLine  [0].FP0.x = 0.0f;
	BtmOutLine  [0].FP0.y = 0.0f;
	BtmOutLine  [0].FP1.x = 0.0f;
	BtmOutLine  [0].FP1.y = 0.0f;
	BtmInLine   [0].FP0.x = 0.0f;
	BtmInLine   [0].FP0.y = 0.0f;
	BtmInLine   [0].FP1.x = 0.0f;
	BtmInLine   [0].FP1.y = 0.0f;
	BtmStdLine  [0].FP0.x = 0.0f;
	BtmStdLine  [0].FP0.y = 0.0f;
	BtmStdLine  [0].FP1.x = 0.0f;
	BtmStdLine  [0].FP1.y = 0.0f;


	//Line Data Init
	for(int i=0; i<MAX_LINE_POINT; i++){
		Line[i].FP0.x =0.0f;
		Line[i].FP0.y =0.0f;
		Line[i].FP1.x =0.0f;
		Line[i].FP1.y =0.0f;
	}

	// Pad Roi Rect 
	for(int k=0; k<2; k++){
		PadPolyRect[k].left   =0;
		PadPolyRect[k].top    =0;
		PadPolyRect[k].right  =0;
		PadPolyRect[k].bottom =0;

		NotchPolyRect[k].left   =0;
		NotchPolyRect[k].top    =0;
		NotchPolyRect[k].right  =0;
		NotchPolyRect[k].bottom =0;
	}

	// CornerData Init
	C1EdgeCount =0;
	C2EdgeCount =0;
	C3EdgeCount =0;
	C4EdgeCount =0;

	// Corner InEdge Count
	C1InEdgeCount =0;
	C2InEdgeCount =0;
	C3InEdgeCount =0;
	C4InEdgeCount =0;

	// Corner Grind Measure Stand Edge Count 
	C1MeaEdgeCount =0;
	C2MeaEdgeCount =0;
	C3MeaEdgeCount =0;
	C4MeaEdgeCount =0;

	// Inspection End
	TopLimitLine = 0 ;
	BtmLimitLine = 0 ;

	// Circle Data Init
	//----------------------------------------------------------------
	// Circle Data 
	for(int k=0; k<MAX_CIRCLE_EDGEPOINT; k++){
		A1CircleData[k].Dis = 0.0f;
		A1CircleData[k].Dv  = 0.0f;
		A1CircleData[k].P.x = 0.0f;
		A1CircleData[k].P.y = 0.0f;

		A2CircleData[k].Dis = 0.0f;
		A2CircleData[k].Dv  = 0.0f;
		A2CircleData[k].P.x = 0.0f;
		A2CircleData[k].P.y = 0.0f;

		C1CircleData[k].Dis = 0.0f;
		C1CircleData[k].Dv  = 0.0f;
		C1CircleData[k].P.x = 0.0f;
		C1CircleData[k].P.y = 0.0f;

		C2CircleData[k].Dis = 0.0f;
		C2CircleData[k].Dv  = 0.0f;
		C2CircleData[k].P.x = 0.0f;
		C2CircleData[k].P.y = 0.0f;
	}
	// Circle InEdge Point Count
	A1CirInEdgeCount = 0;
	A2CirInEdgeCount = 0;

	C1CirInEdgeCount = 0;
	C2CirInEdgeCount = 0;

	// Circle InEdge Point Count
	A1CirOutEdgeCount = 0;
	A2CirOutEdgeCount = 0;

	C1CirOutEdgeCount = 0;
	C2CirOutEdgeCount = 0;

	BtmInEdgeCount    = 0;
	BtmOutEdgeCount   = 0;

	// �ڸ��ҷ� ���� Point Count
	A1CirInCrackCount = 0;
	A2CirInCrackCount = 0;

	C1CirInCrackCount = 0;
	C2CirInCrackCount = 0;

	// Bridht Defect 
	ACrackCount      = 0;
	BCrackCount      = 0;
	CCrackCount      = 0;
	DCrackCount      = 0;

	// Circle Calculate Data Init 
	A1CirDx      = 0.0f;
	A1CirDy      = 0.0f;
	A1CirRound   = 0.0f;
	A2CirDx      = 0.0f;
	A2CirDy      = 0.0f;
	A2CirRound   = 0.0f;

	C1CirDx      = 0.0f;
	C1CirDy      = 0.0f;
	C1CirRound   = 0.0f;
	C2CirDx      = 0.0f;
	C2CirDy      = 0.0f;
	C2CirRound   = 0.0f;

	// Circle Center Data Init 
	A1CirCenter.x = 0.0f;
	A1CirCenter.y = 0.0f;
	A2CirCenter.x = 0.0f;
	A2CirCenter.y = 0.0f;

	C1CirCenter.x = 0.0f;
	C1CirCenter.y = 0.0f;
	C2CirCenter.x = 0.0f;
	C2CirCenter.y = 0.0f;

	// Real Circle Center Data Init 
	RealA1CirCenter.x = 0.0f;
	RealA1CirCenter.y = 0.0f;
	RealA2CirCenter.x = 0.0f;
	RealA2CirCenter.y = 0.0f;

	RealC1CirCenter.x = 0.0f;
	RealC1CirCenter.y = 0.0f;
	RealC2CirCenter.x = 0.0f;
	RealC2CirCenter.y = 0.0f;

	ACircleFit1Center.x = 0.0f;
	ACircleFit1Center.y = 0.0f;
	ACircleFit2Center.x = 0.0f;
	ACircleFit2Center.y = 0.0f;

	CircleFit1Center.x = 0.0f;
	CircleFit1Center.y = 0.0f;
	CircleFit2Center.x = 0.0f;
	CircleFit2Center.y = 0.0f;

	CrackACirDiameter1 = 0.0f;
	CrackACirDiameter2 = 0.0f;
	CrackCirDiameter1  = 0.0f;
	CrackCirDiameter2  = 0.0f;

	ACirDiameter1 = 0.0f;
	ACirDiameter2 = 0.0f;

	CirDiameter1 = 0.0f;
	CirDiameter2 = 0.0f;

	ACirFitDiameter1 = 0.0f;
	ACirFitDiameter2 = 0.0f;

	CirFitDiameter1 = 0.0f;
	CirFitDiameter2 = 0.0f;

	// NOTCH CELL Btm EdgePoint Data Init 
	for(int k=0; k<MAX_BTM_LINE_POINT; k++){
		BtmEdgePoint[k].x =0.0f;
		BtmEdgePoint[k].y =0.0f;
	}

	// NOTCH CELL Top EdgePoint Data Init 
	for(int k=0; k<MAX_TOP_LINE_POINT; k++){
		TopEdgePoint[k].x =0.0f;
		TopEdgePoint[k].y =0.0f;
	}

	// Corner Data Init 
	C1CorGrindWd  = 0.0f;
	C1CorGrindMea = 0.0f;
	C2CorGrindWd  = 0.0f;
	C2CorGrindMea = 0.0f;
	C3CorGrindWd  = 0.0f;
	C3CorGrindMea = 0.0f;
	C4CorGrindWd  = 0.0f;
	C4CorGrindMea = 0.0f;

	//Left,Right ����,���� Init
	LeftGrindWidth =0.0f;
	LeftGrindMea   =0.0f;
	RightGrindWidth=0.0f;
	RightGrindMea  =0.0f;

	// Notch Cell TrackLine Data Overlap Init 
	AvgLeftTrackLineDis     =0;
	AvgRightTrackLineDis    =0;
	AvgLeftBtmTrackLineDis  =0;
	AvgRightBtmTrackLineDis =0;
	LeftTrackLineCount      =0;
	RightTrackLineCount     =0;
	TotalLeftTrackDis       =0;
	TotalRightTrackDis      =0;

	// NOTCH 
	// Btm Line Search �ð� ���� ���� 
	FEndLine =0;

	// Notch Cell Mark Degree 
	MarkDegree =0.0f;

	// ���� ���� (Circle)
	SideCirDeg   =0;
	CenterCirDeg =0;

	// 	Find->OnLoad(true,FindDir);

	// Notch Cell Size Data Init 
	LeftCellSizeHeight  =0.0f;
	CenCellSizeHeight   =0.0f;
	CenCellSizeWidth    =0.0f;
	NotchCellSizeWidth  =0.0f;
	NotchCellSizeHeight =0.0f;

	// MarkToTopSide Height
	LeftMarkToTopHeight  =0.0f;
	CenMarkToTopHeight   =0.0f;
	RightMarkToTopHeight =0.0f;

	// Center Cell Size  ������ �ʿ��� ������ 
	TotalCellSizeWidth  =0.0f;
	CenterCellSize      =0.0f;
	TotalCellSizeCount  =0   ;

	// Cell Size ���� ����
	LeftStaPos.x   =0.0f;
	LeftStaPos.y   =0.0f;
	LeftEndPos.x   =0.0f;
	LeftEndPos.y   =0.0f;
	CenterStaPos.x =0.0f;
	CenterStaPos.y =0.0f;
	CenterEndPos.x =0.0f;
	CenterEndPos.y =0.0f;

	// Align ���ϰ� Ʋ������� �߻��ϴ� Defect����ų 
	int LeftDegMargin =0;
	int RightDegMargin=0;

	AlignMark1.x=0.0f;
	AlignMark1.y=0.0f;
	AlignMark2.x=0.0f;
	AlignMark2.y=0.0f;

	// A1,A2 Data Init 
	for(int k=0; k<2; k++){
		AGrindMark[k].GrindMea =0.0f;
		AGrindMark[k].GrindWidth =0.0f;
		AGrindMark[k].Score      =0.0f;
		AGrindMark[k].Point.x    =0;
		AGrindMark[k].Point.y    =0;
		AGrindMark[k].Rect.left  =0;
		AGrindMark[k].Rect.top   =0;
		AGrindMark[k].Rect.right =0;
		AGrindMark[k].Rect.bottom=0;
	}

	// C1,C2 Data Init 
	for(int k=0; k<2; k++){
		CGrindMark[k].GrindMea   =0.0f;
		CGrindMark[k].GrindWidth =0.0f;
		CGrindMark[k].Score      =0.0f;
		CGrindMark[k].Point.x    =0;
		CGrindMark[k].Point.y    =0;
		CGrindMark[k].Rect.left  =0;
		CGrindMark[k].Rect.top   =0;
		CGrindMark[k].Rect.right =0;
		CGrindMark[k].Rect.bottom=0;
	}

	// Bottom Data Init 
	for(int k=0; k<7; k++){
		BtmGrindMark[k].GrindMea =0.0f;
		BtmGrindMark[k].GrindWidth =0.0f;
		BtmGrindMark[k].Score      =0.0f;
		BtmGrindMark[k].Point.x    =0;
		BtmGrindMark[k].Point.y    =0;
		BtmGrindMark[k].Rect.left  =0;
		BtmGrindMark[k].Rect.top   =0;
		BtmGrindMark[k].Rect.right =0;
		BtmGrindMark[k].Rect.bottom=0;
	}
}
//---------------------------------------------------------------
int CFM2xLineDefect::OnAutoBevelIn()// Line���� ���� �˻� �Լ� 
{
	EImageEncoder CodeImageEncoder     ;
	ECodedImage2  CodeImage            ;
	EObjectSelection CodeImageSelection;
	EROIBW8   DefectROI                ;

	int ROI_X =0;
	int ROI_Y =0;
	int ROI_W =0;
	int ROI_H =0;

	// Circle Dis, Dv TEST 
	double MaxDis =0.0f     ;
	double MinDis =10000.0f ;
	double MaxDv  =0.0f     ;
	double MinDv  =10000.0f ;
	double StdMaxDis =0.0f     ;
	double StdMinDis =10000.0f ;
	int    MinCirCount =0;


	int Index = 0;
	// NOTCH Defect �˻� ������ 
	if( FShape==shRectH ){ // NOTCH Edge Inspection(All Inspection     )  ImageCopy -> ImageConversion -> ROI Attatch 
		if(ImgDegree==0.0f) EasyImage::Threshold(GImage    ,&DefectImg, EnCapTopThresHold);
		else                EasyImage::Threshold(&RotateImg,&DefectImg, EnCapTopThresHold); //DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_HAP.bmp");
		Index =0;   
	}                                                                                                     
	if( FShape==shRectS ){
		EasyImage::Threshold(GImage    ,&DefectImg, EnCapMidThresHold);
		Index =1;                                                                              // NOTCH Edge Inspection(Only Top Inspection)  Short Cell Cam0
	}
	if( FShape==shRectC ){ // NOTCH Edge Inspection(All Inspection     )  ImageCopy -> ImageConversion -> ROI Attatch 
		if(ImgDegree==0.0f) EasyImage::Threshold(GImage    ,&DefectImg, EnCapBtmThresHold);
		else                EasyImage::Threshold(&RotateImg,&DefectImg, EnCapBtmThresHold); //DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_HAP.bmp");
		Index =2;   
	}    

	// Pixel ����Data ���� Buffer�Ѱ��� 
	//IAddress = DefectImg.GetImagePtr();
	int PixelValue;
	int PixelCount;

	// ���� Btm OverKill���� ���� 
	int BTMLine =0;
	int TotalPixel =0;

	// Frit OverKill Remove Value
	int    MaxPixel   =0;
	int    MaxData    =0;
	int    CurData    =0;
	int    BrokenPixel =0;
	int    CrackPixel  =0;
	const int LongBrokenMargin   =7500;
	const int LongCrackMargin    =10  ;
	// 	const int ShortBrokenMargin  =7500;
	const int ShortCrackMargin   =25  ;

	// Burr & Remain
	int BurrWidth =100;

	// Left Line Dv 
	double LeftDv =0.0f;

	bool l  = false; bool lt = false;
	bool t  = false; bool rt = false;
	bool r  = false; bool rb = false;
	bool b  = false; bool lb = false;

	// LeftLimit Remove Blob
	bool IsRemoveBlob = true;

	int ImgW   = GImage->GetWidth();
	int ImgH   = GImage->GetHeight();
	ImageWidth = GImage->GetWidth();;

	int Count1=0, Count2=0, Count3=0, Count4=0, Count5=0;

	//Euresys Blob Parameter Setting 
	//////////////////////////////////////////////////
	//CNC_6HEAD �̹��� ��ȯ �� �⺻���� ���� 
	if((FShape==shRectH || FShape==shRectS || FShape==shRectC)){
		DefectROI.Attach(&DefectImg);
		CodeImageSelection.SetFeretAngle(0.00f);
		CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
		CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );

		IAddress = DefectImg.GetImagePtr();
	}
	//////////////////////////////////////////////////

	TFPoint LT(0          ,           0);
	TFPoint RT((float)ImgW,           0);
	TFPoint RB((float)ImgW, (float)ImgH);
	TFPoint LB(0          , (float)ImgH);
	TFPoint SP, EP, LSP, LEP, RSP, REP  ;

	// Notch Cell Size ���� 
	TFPoint NotchCenterPos(0.0f, 0.0f);

	// ROI ���� ����
	int LeftHeight ;
	int RightHeight;
	int TopWidth   ;
	int BtmWidth   ;
	int BlobCount  ;
	const int Margin =15;

	// ImageRotate �ҷ� �߻��� OverKill ���� ����
	const int TopMargin =10;
	const int BtmMargin =30;

	// �ſ��� ���簢�� �����ϰ�� ������ ���ؼ� �ҷ��� ���ϰ� �پ
	// �˻�� ���� �Ұ���...�̷����ؼ� Offset�߰��� 
	const int BtmInspOffset = 6;

	// Corner Data 
	int CornerCount  =0;
	int MarktoBmLine =0;

	//Circle Count,Avg Round 
	int CircleCount=0;
	float AvgCirDis   =0.0f;
	float TotalCirDis =0.0f;

	// Merge Margin
	const int xMerge  =30;
	const int yMerge  =30;
	const int MgCount =3;

	//Remain Offset 
	const int RemainMargin = 30;

	// BM Line Check
	float RightBMGab=0;
	float LeftBMGab =0;

	// Side Offset Margin
	int SideMargin =1;

	// Margin Filter
	const int BroMarginWidth  = 200;
	const int BroMarginHeight = 200;

	// GrindMark ���� �����Ͽ� ȸ������(�˻�ȭ�� ���÷���)
	TFPoint LtPos, RbPos;

	// �˻� ROI ���� 
	// Notch Cell ���� �˻� 
	if(FShape==shRectH ) {
		SP = Math.OnIntersect(LeftOutLine [0].FP0, LeftOutLine [0].FP1, LB, RB);
		EP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LB, RB);

		FLeftWidth  =200;
		FRightWidth =200;
		FTopHeight  =200;

		if(IsCornerTop)     { //Corner Cutting�ϰ�� Defect�˻� ���� ���� 
			TopWidth    =(int)(FRTTInPoint.x - FLTTInPoint.x); // TOP�˻� ����� ���� ���� ����ų ���� ���� 
			LeftHeight  =(int)(SP.y - FLTLInPoint.y);
			RightHeight =(int)(EP.y - FRTRInPoint.y);
		}
		else if(IsRectTop)  {//Corner Cutting ������� Defect�˻� ���� ���� 
			TopWidth    =(int)(FRTPoint.x - FLTPoint.x); // TOP�˻� ����� ���� ���� ����ų ���� ���� 
			LeftHeight  =(int)(SP.y - FLTPoint.y);
			RightHeight =(int)(EP.y - FRTPoint.y);
		}
		else if(IsCircleTop){//Circle Cutting�ϰ�� Defect �˻� ���� ���� 
			if(TopEdgePoint[0].x==0 || TopEdgePoint[0].y==0) {
				TopEdgePoint[0].x = (int)FLTPoint.x                     ; 
				TopEdgePoint[0].y = (int)FLTPoint.y+ASideCircleHeight+10;
			}
			if(TopEdgePoint[1].x==0 || TopEdgePoint[1].y==0) {
				TopEdgePoint[1].x = (int)FLTPoint.x+ASideCircleWidth+10; 
				TopEdgePoint[1].y = (int)FLTPoint.y                    ;
			}
			if(TopEdgePoint[2].x==0 || TopEdgePoint[2].y==0) {
				TopEdgePoint[2].x = (int)FRTPoint.x -ASideCircleWidth-10; 
				TopEdgePoint[2].y = (int)FRTPoint.y                     ;
			}
			if(TopEdgePoint[3].x==0 || TopEdgePoint[3].y==0) {
				TopEdgePoint[3].x = (int)FRTPoint.x                     ; 
				TopEdgePoint[3].y = (int)FRTPoint.y+ASideCircleHeight+10;
			}
			TopWidth    =(int)(TopEdgePoint[2].x - TopEdgePoint[1].x); // TOP�˻� ����� ���� ���� ����ų ���� ����   
			// 			LeftHeight  =(int)(SP.y - TopEdgePoint[0].y);                                                             
			// 			RightHeight =(int)(EP.y - TopEdgePoint[3].y);
			if(IsMarkBtm){
				LeftHeight  =(int)(SP.y - TopEdgePoint[0].y);                                                             
				RightHeight =(int)(EP.y - TopEdgePoint[3].y);
			}
			else         {
				TopEdgePoint[0].y = (int)FLTPoint.y+ASideCircleHeight+10;
				TopEdgePoint[3].y = (int)FRTPoint.y+ASideCircleHeight+10;
				LeftHeight        = (int)(SP.y - TopEdgePoint[0].y);                                                             
				RightHeight       = (int)(EP.y - TopEdgePoint[3].y);
			}
		}

		// Defect Data Init 
		DefectPointCount = 0; // ��ü �ҷ� ���� �ʱ�ȭ
		BCount           = 0; // B_SIDE  �ҷ� ����Ʈ ����
		ACount           = 0; // A_SIDE  �ҷ� ����Ʈ ����
		CCount           = 0; // C_SIDE  �ҷ� ����Ʈ ����
		DCount           = 0; // D_SIDE  �ҷ� ����Ʈ ����
		// 		C1CirDeftectCnt  = 0;
		C2CirDeftectCnt  = 0;
		ABurrCount       = 0;
		BBurrCount       = 0;
		CBurrCount       = 0;
		DBurrCount       = 0;

		// BSide, DSide Defect �˻�� Margin�� ���� 
		if(AlignMark1.y!=0.0f && AlignMark2.y!=0.0f){
			if(AlignMark1.y>AlignMark2.y){
				LeftDegMargin  = (int)((AlignMark1.y - AlignMark2.y)+5);
				RightDegMargin = 5;
			}
			else if(AlignMark1.y<AlignMark2.y) {
				LeftDegMargin  = 5;
				RightDegMargin = (int)((AlignMark2.y - AlignMark1.y)+5);
			}
			else if(AlignMark1.y==AlignMark2.y) {
				LeftDegMargin  = 5;
				RightDegMargin = 5;
			}
		}
	}
	// Notch Cell ��ƽ �˻� 
	if(FShape==shRectS) {
		LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
		LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);

		RSP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LT, RT);
		REP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LB, RB);

		// ����ų ������������ ������ ���� ���� ���� �˻� 
		if((fabs(LSP.x - LEP.x)>Margin) || (fabs(RSP.x - REP.x)>Margin) ){
			LSP.x = (LSP.x>LEP.x) ? LSP.x : LEP.x;
			RSP.x = (RSP.x>REP.x) ? REP.x : RSP.x;
		}
		// Blob
		FLeftWidth   = 200;
		FRightWidth  = 200;
		LeftHeight   = (int)(LEP.y - LSP.y);
		RightHeight  = (int)(REP.y - RSP.y);
	}
	// Notch Cell �� �˻� 
	if(FShape==shRectC) {
		SP = Math.OnIntersect(LeftOutLine [0].FP0, LeftOutLine [0].FP1, LT, RT);
		EP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LT, RT);

		// BtmWidth �������� Circle��� �� �Ŀ� ���� ������
		FLeftWidth  = 200;
		FRightWidth = 200;
		FBtmHeight  = 50;

		if(IsCornerBtm)     {//Corner Cutting�ϰ�� Defect�˻� ���� ���� 
			LeftHeight  = (int)((FLBLInPoint.y - SP.y));
			RightHeight = (int)((FRBRInPoint.y - EP.y));
			BtmWidth    = (int)((FRBBInPoint.x - FLBBInPoint.x));
		}				    
		else if(IsRectBtm)  {//Corner Cutting ������� Defect�˻� ���� ���� 
			LeftHeight  = (int)((FLBPoint.y - SP.y      ));
			RightHeight = (int)((FRBPoint.y - EP.y      ));
			BtmWidth    = (int)((FRBPoint.x - FLBPoint.x));
		}
		else if(IsCircleBtm){//Circle Cutting�ϰ�� Defect �˻� ���� ���� 
			LeftHeight  = (int)((FLBPoint.y - SP.y) - CSideCircleHeight - dCirOffsetY);
			RightHeight = (int)((FRBPoint.y - EP.y) - CSideCircleHeight - dCirOffsetY);
		}

		// BSide, DSide Defect �˻�� Margin�� ���� 
		if(AlignMark1.y!=0.0f && AlignMark2.y!=0.0f){
			if(AlignMark1.y>AlignMark2.y){
				LeftDegMargin  = 5                                     ;
				RightDegMargin = (int)((AlignMark1.y - AlignMark2.y)+5);
			}
			else if(AlignMark1.y<AlignMark2.y) {
				LeftDegMargin  = (int)((AlignMark2.y - AlignMark1.y)+5);
				RightDegMargin = 5                                     ;
			}
			else if(AlignMark1.y==AlignMark2.y) {
				LeftDegMargin  = 5;
				RightDegMargin = 5;
			}
		}
	}
	float Dis    =0.0f;
	float StdDis =0.0f;

	// Side Offset TEST 
	double  Width   =0.0f;
	long    Left    =0;
	float   CenterX =0.0F;


	switch (Index) {

		// shRectH NOTCH 
		//----------------------------------------------------------------------
	case 0 :
		// Corner Defect �˻� 
		if(IsCornerTop){
			if(C1InEdgeCount!=0){
				CornerCount=0;
				for(int k=0; k<C1InEdgeCount; k++){
					if(C1InPoint[k].y>FLTLPoint.y-1) continue;
					Dis = Math.GetLengthFrLineToPoint(FLTLInPoint, FLTTInPoint, C1InPoint[k]);
					C1Corner[CornerCount].Dv   = Dis         ;
					C1Corner[CornerCount].P.x  = C1InPoint[k].x;
					C1Corner[CornerCount].P.y  = C1InPoint[k].y;
					CornerCount++;
					if(Dis>MaxDv) MaxDv = Dis;
					if(Dis<MinDv) MinDv = Dis;
				}
				if(LTCornerWidth>(LTCornerHeight*2)) C1Count = GetSize(C1Corner, C1DefectPoint, CornerCount, 10);
				else                                 C1Count = GetSize(C1Corner, C1DefectPoint, CornerCount, 5 );
			}

			MaxDv =0.0f;
			MinDv =10000.0f;

			// Corner 2 
			if(C2InEdgeCount!=0){
				CornerCount=0;
				for(int k=0; k<C2InEdgeCount; k++){
					if(C2InPoint[k].y>FRTRPoint.y-1) continue;
					Dis = Math.GetLengthFrLineToPoint(FRTTInPoint, FRTRInPoint, C2InPoint[k]);
					C2Corner[CornerCount].Dv   = Dis         ;
					C2Corner[CornerCount].P.x  = C2InPoint[k].x;
					C2Corner[CornerCount].P.y  = C2InPoint[k].y;
					CornerCount++;
					if(Dis>MaxDv) MaxDv = Dis;
					if(Dis<MinDv) MinDv = Dis;
				}
				if(RTCornerWidth >(RTCornerHeight*1.5)) C2Count = GetSize(C2Corner, C2DefectPoint, CornerCount, 10);
				else                                    C2Count = GetSize(C2Corner, C2DefectPoint, CornerCount, 5 );

			}
			//Side A
			////////////////////////////////////////////////////////////////////////////////////////////////
			// 			LogUnit.SetLog("CAP_Side_A_Start_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			// 		DefectROI.SetPlacement((int)(FLTTPoint.x+SideMargin), (int)((FLTTPoint.y+FRTTPoint.y)/2), TopWidth-(2*SideMargin), FTopHeight);
			DefectROI.SetPlacement((int)(FLTTInPoint.x+SideMargin), (int)((FLTTInPoint.y+FRTTInPoint.y)/2), TopWidth-(2*SideMargin), FTopHeight);
			if((FLTTInPoint.x+SideMargin) <=0          || ((FLTTInPoint.y+FRTTInPoint.y)/2)<=0           || TopWidth-(2*SideMargin)<=0          || FTopHeight<=0          )return false;
			if((FLTTInPoint.x+SideMargin) >=ImageWidth || ((FLTTInPoint.y+FRTTInPoint.y)/2)>TopLimitLine || TopWidth-(2*SideMargin)>=ImageWidth || FTopHeight>TopLimitLine) return false;
			if((FLTTInPoint.x+SideMargin)+(TopWidth-(2*SideMargin)) >=ImageWidth || ((FRTTInPoint.y+FRTTPoint.y)/2)+ FTopHeight>TopLimitLine) return false;
			// Blob �˻� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_TopLimit         ,0         , 3          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_LeftLimit ,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(ACount>=MAX_DEFECT_POINT) break;
					ADefectPoint[ACount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					ADefectPoint[ACount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					ADefectPoint[ACount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width/2 ) +FLTTInPoint.x+SideMargin      );
					ADefectPoint[ACount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2) +((FLTTInPoint.y+FRTTInPoint.y)/2));
					ADefectPoint[ACount].Rect.right  =(long)(ADefectPoint[ACount].Rect.left+ ADefectPoint[ACount].Width );
					ADefectPoint[ACount].Rect.bottom =(long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=ADefectPoint[ACount].Rect.top+1 ; n<ADefectPoint[ACount].Rect.bottom-1; n++){
						for(int m=ADefectPoint[ACount].Rect.left+1; m<ADefectPoint[ACount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}

					ADefectPoint[ACount].Round  = PixelCount;
					ADefectPoint[ACount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width/2 )+FLTTInPoint.x+SideMargin       );
					ADefectPoint[ACount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2)+((FLTTInPoint.y+FRTTInPoint.y)/2));

					// Image Rotate ���� ��ǥ�� �ٽ� Rotate�Ѵ� _ Display ���� 
					if(ImgDegree!=0.0f){
						ADefectPoint[ACount].P = Math.OnRotate(CenterPoint, ADefectPoint[ACount].P, (ImgDegree));
					}
					// Image Rotate ���� ��ǥ�� ȸ�� ���Ŀ� ImageHeight���� ���Ͽ� ���� ��ǥ�� �����Ѵ� 
					ADefectPoint[ACount].P.y    =(float)(ADefectPoint[ACount].P.y+ImageHeight);

					//Merget���ؼ� �ʿ��� 
					ADefectPoint[ACount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2) +((FLTTInPoint.y+FRTTInPoint.y)/2)+ImageHeight);
					ADefectPoint[ACount].Rect.bottom =(long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);
					ACount++;
				}
			}

			//Side B
			////////////////////////////////////////////////////////////////////////////////////////////////
			// 			LogUnit.SetLog("CAP_Side_B_Start_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			// 			DefectROI.SetPlacement((int)(FRTRInPoint.x-FRightWidth-SideMargin), (int)(FRTRInPoint.y+Margin), FRightWidth, (RightHeight-(2*Margin)-TopMargin));
			// 			DefectROI.SetPlacement((int)(FRTRInPoint.x-FRightWidth-SideMargin), (int)(FRTRInPoint.y+Margin), FRightWidth, (RightHeight-(2*Margin)-TopMargin));
			DefectROI.SetPlacement((int)(FRTRInPoint.x-FRightWidth-SideMargin), (int)(FRTRInPoint.y+Margin), FRightWidth, (RightHeight-(2*Margin)-RightDegMargin));
			if((FRTRInPoint.x-FRightWidth-SideMargin) <=0          ||(FRTRInPoint.y+Margin)<=0           || FRightWidth<=0          || (RightHeight-(2*Margin)-RightDegMargin)<=0)           return false;
			if((FRTRInPoint.x-FRightWidth-SideMargin) >=ImageWidth ||(FRTRInPoint.y+Margin)>TopLimitLine || FRightWidth>=ImageWidth || (RightHeight-(2*Margin)-RightDegMargin)>TopLimitLine) return false;
			if((FRTRInPoint.x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth ||((FRTRInPoint.y+Margin)+(RightHeight-(2*Margin)-RightDegMargin))>TopLimitLine) return false;
			//  Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth ,FMinWidth     , FMaxWidth    , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight,FMinHeight    , FMaxHeight   , EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit     ,FRightWidth-2 , FRightWidth+1, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit     ,FRightWidth-1 , FRightWidth+1, EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(BCount>=MAX_DEFECT_POINT) break;
					BDefectPoint[BCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					BDefectPoint[BCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					BDefectPoint[BCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +(FRTRInPoint.x-FRightWidth-SideMargin));
					BDefectPoint[BCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(FRTRInPoint.y+Margin)                );
					BDefectPoint[BCount].Rect.right  = (long)(BDefectPoint[BCount].Rect.left+ BDefectPoint[BCount].Width );
					BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
						for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}
					BDefectPoint[BCount].Round  = PixelCount;
					BDefectPoint[BCount].P.x    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +(FRTRInPoint.x-FRightWidth-SideMargin));
					BDefectPoint[BCount].P.y    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(FRTRInPoint.y+Margin)+ImageHeight    );
					BDefectPoint[BCount].Width  = BDefectPoint[BCount].Width+SideMargin; 

					// Merge ���ؼ� ������ �ʿ��� 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) + FRTRInPoint.y+Margin+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount =0;

			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = ((Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x);
			else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP0.x - FP1.x);
			else                                                      RDistance = ((Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x);

			MarktoBmLine = (IsMarkBtm) ? 5: 50;  
			if(IsMarkBtm) RDistance = RDistance-MarktoBmLine;
			else          RDistance = RDistance+MarktoBmLine;

			// Side D	
			////////////////////////////////////////////////////////////////////////////////////////////////
			// 			LogUnit.SetLog("CAP_Side_D_Start_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			// 			DefectROI.SetPlacement((int)(FLTLInPoint.x+SideMargin), (int)(FLTLInPoint.y+Margin), FLeftWidth, (LeftHeight-(2*Margin)-TopMargin));
			// 			if((FLTLInPoint.x+SideMargin) <=0          ||(FLTLInPoint.y+Margin)<=0           || FLeftWidth<=0          || (LeftHeight-(2*Margin)-TopMargin)<=0          ) return false;
			// 			if((FLTLInPoint.x+SideMargin) >=ImageWidth ||(FLTLInPoint.y+Margin)>TopLimitLine || FLeftWidth>=ImageWidth || (LeftHeight-(2*Margin)-TopMargin)>TopLimitLine) return false;
			// 			if((FLTLInPoint.x+SideMargin)+FLeftWidth >=ImageWidth ||(FLTLInPoint.y+Margin)+(LeftHeight-(2*Margin)-TopMargin)>TopLimitLine) return false;

			DefectROI.SetPlacement((int)(FLTLInPoint.x+SideMargin), (int)(FLTLInPoint.y+Margin), FLeftWidth, (LeftHeight-(2*Margin)-LeftDegMargin));
			if((FLTLInPoint.x+SideMargin) <=0          ||(FLTLInPoint.y+Margin)<=0           || FLeftWidth<=0          || (LeftHeight-(2*Margin)-LeftDegMargin)<=0          ) return false;
			if((FLTLInPoint.x+SideMargin) >=ImageWidth ||(FLTLInPoint.y+Margin)>TopLimitLine || FLeftWidth>=ImageWidth || (LeftHeight-(2*Margin)-LeftDegMargin)>TopLimitLine) return false;
			if((FLTLInPoint.x+SideMargin)+FLeftWidth >=ImageWidth ||(FLTLInPoint.y+Margin)+(LeftHeight-(2*Margin)-LeftDegMargin)>TopLimitLine) return false;

			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);

			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 3          , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(DCount>=MAX_DEFECT_POINT) break;
					DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +(FLTLInPoint.x+SideMargin));
					DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(FLTLInPoint.y+Margin)    );
					DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
					DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

					PixelCount = 0;
					PixelValue = 0;
					// ���� Pixel Data ���� 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}
					// Display ������ �ʿ��� 
					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
					DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(FLTLInPoint.y+Margin)+ImageHeight );
					DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

					// Merge ���ؼ� ������ �ʿ��� 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(FLTLInPoint.y+Margin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount =0;

			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = (FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2));
			else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = (FP0.x -  Line[waLeftWall].FP0.x)                                                     ;
			else                                                    LDistance = (FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2)); 
		}
		else if(IsRectTop) {  // Corner Cutting�� ������� 
			//Side A
			////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(FLTPoint.x+SideMargin), (int)((FLTPoint.y+FRTPoint.y)/2), TopWidth-(2*SideMargin), FTopHeight);
			if((FLTPoint.x+SideMargin) <=0          || ((FLTPoint.y+FRTPoint.y)/2)<=0           || TopWidth-(2*SideMargin)<=0          || FTopHeight<=0          )return false;
			if((FLTPoint.x+SideMargin) >=ImageWidth || ((FLTPoint.y+FRTPoint.y)/2)>TopLimitLine || TopWidth-(2*SideMargin)>=ImageWidth || FTopHeight>TopLimitLine) return false;
			if((FLTPoint.x+SideMargin)+(TopWidth-(2*SideMargin)) >=ImageWidth || ((FLTPoint.y+FRTPoint.y)/2)+ FTopHeight>TopLimitLine) return false;
			// Blob �˻� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_TopLimit         ,0         , 3          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_LeftLimit ,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(ACount>=MAX_DEFECT_POINT) break;
					ADefectPoint[ACount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					ADefectPoint[ACount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					ADefectPoint[ACount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width/2 ) +FLTPoint.x+SideMargin      );
					ADefectPoint[ACount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2) +((FLTPoint.y+FRTPoint.y)/2));
					ADefectPoint[ACount].Rect.right  =(long)(ADefectPoint[ACount].Rect.left+ ADefectPoint[ACount].Width );
					ADefectPoint[ACount].Rect.bottom =(long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=ADefectPoint[ACount].Rect.top+1 ; n<ADefectPoint[ACount].Rect.bottom-1; n++){
						for(int m=ADefectPoint[ACount].Rect.left+1; m<ADefectPoint[ACount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}

					ADefectPoint[ACount].Round  = PixelCount;
					ADefectPoint[ACount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width/2 )+FLTPoint.x+SideMargin      );
					ADefectPoint[ACount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2)+((FLTPoint.y+FRTPoint.y)/2));

					// Image Rotate ���� ��ǥ�� �ٽ� Rotate�Ѵ� _ Display ���� 
					if(ImgDegree!=0.0f){
						ADefectPoint[ACount].P = Math.OnRotate(CenterPoint, ADefectPoint[ACount].P, (ImgDegree));
					}
					// Image Rotate ���� ��ǥ�� ȸ�� ���Ŀ� ImageHeight���� ���Ͽ� ���� ��ǥ�� �����Ѵ� 
					ADefectPoint[ACount].P.y    =(float)(ADefectPoint[ACount].P.y+ImageHeight);

					//Merget���ؼ� �ʿ��� 
					ADefectPoint[ACount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2) +((FLTPoint.y+FRTPoint.y)/2)+ImageHeight);
					ADefectPoint[ACount].Rect.bottom =(long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);
					ACount++;
				}
			}
			BlobCount =0;

			//Side B
			////////////////////////////////////////////////////////////////////////////////////////////////
			// 			LogUnit.SetLog("CAP_Side_B_Start_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			// 			DefectROI.SetPlacement((int)(FRTPoint.x-FRightWidth-SideMargin), (int)(FRTPoint.y+Margin), FRightWidth, (RightHeight-(2*Margin)-TopMargin));
			// 			if((FRTPoint.x-FRightWidth-SideMargin) <=0          ||(FRTPoint.y+Margin)<=0           || FRightWidth<=0          || (RightHeight-(2*Margin)-TopMargin)<=0)           return false;
			// 			if((FRTPoint.x-FRightWidth-SideMargin) >=ImageWidth ||(FRTPoint.y+Margin)>TopLimitLine || FRightWidth>=ImageWidth || (RightHeight-(2*Margin)-TopMargin)>TopLimitLine) return false;
			// 			if((FRTPoint.x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth ||((FRTPoint.y+Margin)+(RightHeight-(2*Margin)-TopMargin))>TopLimitLine) return false;

			DefectROI.SetPlacement((int)(FRTPoint.x-FRightWidth-SideMargin), (int)(FRTPoint.y+Margin), FRightWidth, (RightHeight-(2*Margin)-RightDegMargin));
			if((FRTPoint.x-FRightWidth-SideMargin) <=0          ||(FRTPoint.y+Margin)<=0           || FRightWidth<=0          || (RightHeight-(2*Margin)-RightDegMargin)<=0)           return false;
			if((FRTPoint.x-FRightWidth-SideMargin) >=ImageWidth ||(FRTPoint.y+Margin)>TopLimitLine || FRightWidth>=ImageWidth || (RightHeight-(2*Margin)-RightDegMargin)>TopLimitLine) return false;
			if((FRTPoint.x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth ||((FRTPoint.y+Margin)+(RightHeight-(2*Margin)-RightDegMargin))>TopLimitLine) return false;

			//  Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth ,FMinWidth     , FMaxWidth    , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight,FMinHeight    , FMaxHeight   , EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit     ,FRightWidth-2 , FRightWidth+1, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit     ,FRightWidth-1 , FRightWidth+1, EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(BCount>=MAX_DEFECT_POINT) break;
					BDefectPoint[BCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					BDefectPoint[BCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					BDefectPoint[BCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +(FRTPoint.x-FRightWidth-SideMargin));
					BDefectPoint[BCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(FRTPoint.y+Margin)                );
					BDefectPoint[BCount].Rect.right  = (long)(BDefectPoint[BCount].Rect.left+ BDefectPoint[BCount].Width );
					BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
						for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}
					BDefectPoint[BCount].Round  = PixelCount;
					BDefectPoint[BCount].P.x    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +(FRTPoint.x-FRightWidth-SideMargin));
					BDefectPoint[BCount].P.y    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(FRTPoint.y+Margin)+ImageHeight    );
					BDefectPoint[BCount].Width  = BDefectPoint[BCount].Width+SideMargin; 

					// Merge ���ؼ� ������ �ʿ��� 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) + FRTPoint.y+Margin+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount =0;

			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = ((Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x);
			else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP0.x - FP1.x);
			else                                                      RDistance = ((Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x);

			// 			MarktoBmLine = (IsMarkBtm) ? 5: 50;  
			// 			if(IsMarkBtm) RDistance = RDistance-MarktoBmLine;
			// 			else          RDistance = RDistance+MarktoBmLine;

			// Side D	
			////////////////////////////////////////////////////////////////////////////////////////////////
			// 			LogUnit.SetLog("CAP_Side_D_Start_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			// 			DefectROI.SetPlacement((int)(FLTPoint.x+SideMargin), (int)(FLTPoint.y+Margin), FLeftWidth, (LeftHeight-(2*Margin)-TopMargin));
			// 			if((FLTPoint.x+SideMargin) <=0          ||(FLTPoint.y+Margin)<=0           || FLeftWidth<=0          || (LeftHeight-(2*Margin)-TopMargin)<=0          ) return false;
			// 			if((FLTPoint.x+SideMargin) >=ImageWidth ||(FLTPoint.y+Margin)>TopLimitLine || FLeftWidth>=ImageWidth || (LeftHeight-(2*Margin)-TopMargin)>TopLimitLine) return false;
			// 			if((FLTPoint.x+SideMargin)+FLeftWidth >=ImageWidth ||(FLTPoint.y+Margin)+(LeftHeight-(2*Margin)-TopMargin)>TopLimitLine) return false;

			DefectROI.SetPlacement((int)(FLTPoint.x+SideMargin), (int)(FLTPoint.y+Margin), FLeftWidth, (LeftHeight-(2*Margin)-LeftDegMargin));
			if((FLTPoint.x+SideMargin) <=0          ||(FLTPoint.y+Margin)<=0           || FLeftWidth<=0          || (LeftHeight-(2*Margin)-LeftDegMargin)<=0          ) return false;
			if((FLTPoint.x+SideMargin) >=ImageWidth ||(FLTPoint.y+Margin)>TopLimitLine || FLeftWidth>=ImageWidth || (LeftHeight-(2*Margin)-LeftDegMargin)>TopLimitLine) return false;
			if((FLTPoint.x+SideMargin)+FLeftWidth >=ImageWidth ||(FLTPoint.y+Margin)+(LeftHeight-(2*Margin)-LeftDegMargin)>TopLimitLine) return false;

			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);

			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 3          , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(DCount>=MAX_DEFECT_POINT) break;
					DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +(FLTPoint.x+SideMargin));
					DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(FLTPoint.y+Margin)    );
					DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
					DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

					PixelCount = 0;
					PixelValue = 0;
					// ���� Pixel Data ���� 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}
					// Display ������ �ʿ��� 
					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
					DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(FLTPoint.y+Margin)+ImageHeight );
					DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

					// Merge ���ؼ� ������ �ʿ��� 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(FLTPoint.y+Margin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount =0;

			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = (FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2));
			else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = (FP0.x -  Line[waLeftWall].FP0.x)                                                     ;
			else                                                    LDistance = (FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2)); 
		}
		else if(IsCircleTop){ // Circle Cutting�ϰ�� Defect�˻� ���� 
			if(FInspectMode==Insp_Polygon){ // A_Side ���� Arc Polygon Mode �˻� ������ 
				// 				LogUnit.SetLog("Polygon_Defect_START");     
				if(A1CirInEdgeCount!=0){    // Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 

					A1CirDeftectCnt  = GetPolygonSize(A1CirInPoint, A1CirDefectPoint, A1CirInEdgeCount, 50, 5); // Dark Defect 
					// �ڸ� �ҷ� �˻� 
					if(!IsMarkBtm && A1CirInCrackCount>0 && BrokenThresHold!=0){
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
						A1CirCrackCnt  = GetCircleCrackSize(A1CircleData, A1CirCrackPoint, A1CirInCrackCount, 30,ACirFitDiameter1,ACircleFit1Center); //Bright Defect 
					}
				}

				if(A2CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
					A2CirDeftectCnt  = GetPolygonSize(A2CirInPoint, A2CirDefectPoint, A2CirInEdgeCount, 50, 5); // Dark Defect 

					// �ڸ� �ҷ� �˻� 
					if(!IsMarkBtm && A2CirInCrackCount>0 && BrokenThresHold!=0){
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
						A2CirCrackCnt  = GetCircleCrackSize(A2CircleData, A2CirCrackPoint, A2CirInCrackCount, 30,ACirFitDiameter2,ACircleFit2Center); // Bright Defect 
					}
				}
				// 				LogUnit.SetLog("Polygon_Defect_END");     
			}
			else                          { // A_Side ���� Arc Mark Mode �˻� ������ (A_Side������ Only MarkMode�� ������)
				// 				LogUnit.SetLog("Mark_Defect_START");     
				if(A1CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
					MaxDis =0.0f     ;
					MinDis =10000.0f ;
					MaxDv  =0.0f     ;
					MinDv  =10000.0f ;

					CircleCount=0;TotalCirDis=0.0f; AvgCirDis=0.0f;
					for(int k=0; k<A1CirInEdgeCount; k++){
						if(A1CirInPoint[k].x==0 || A1CirInPoint[k].y==0 || k>=MAX_CIRCLE_EDGEPOINT) continue;

						// 					Dis    = Math.GetLengthFrPointToPoint(A1CirCenter,A1CirInPoint[k]);
						Dis    = Math.GetLengthFrPointToPoint(ACircleFit1Center,A1CirInPoint[k]);

						if(Dis>MaxDis) MaxDis = Dis;
						if(Dis<MinDis) MinDis = Dis;

						if(k==0){ // BTM Defect �˻縦 ���� ��ǥ�� �� Dx,Dy ���� 
							A1CirDx = Math.GetLengthFrPointToPoint(ACircleFit1Center,A1CirInPoint[k]);
						} 
						if(k==(A1CirInEdgeCount-1)){
							A1CirDy = Math.GetLengthFrPointToPoint(ACircleFit1Center,A1CirInPoint[k]);
						}

						A1CircleData[CircleCount].Dis  = Dis              ;
						A1CircleData[CircleCount].P.x  = A1CirInPoint[k].x;
						A1CircleData[CircleCount].P.y  = A1CirInPoint[k].y;
						CircleCount++   ;
						TotalCirDis+=Dis;
					}
					if(CircleCount!=0 || TotalCirDis!=0){ // Circle AvgRound Cal
						AvgCirDis = (TotalCirDis/CircleCount);
						A1CirRound = AvgCirDis;
					}

					for(int i=0; i<CircleCount; i++){
						// 					A1CircleData[i].Dv = fabs(AvgCirDis - A1CircleData[i].Dis);
						A1CircleData[i].Dv = (ACirFitDiameter1 - A1CircleData[i].Dis) >0 ?(ACirFitDiameter1 - A1CircleData[i].Dis):0;
						if(A1CircleData[i].Dv>MaxDv) MaxDv = A1CircleData[i].Dv;
						if(A1CircleData[i].Dv<MinDv) MinDv = A1CircleData[i].Dv;

					}
					A1CirDeftectCnt  = GetCircleSize(A1CircleData, A1CirDefectPoint, CircleCount, 5); // Dark Defect 

					// �ڸ��ҷ� �˻� ������ 
					if(!IsMarkBtm && A1CirInCrackCount>0 && BrokenThresHold!=0){
						for(int i=0; i<A1CirInCrackCount; i++){
							A1CircleData[i].Dv  =0.0f;
							A1CircleData[i].Dis =0.0f;
							A1CircleData[i].P.x  = A1CirInCrackPoint[i].x;
							A1CircleData[i].P.y  = A1CirInCrackPoint[i].y;
						}
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
						A1CirCrackCnt  = GetCircleCrackSize(A1CircleData, A1CirCrackPoint, A1CirInCrackCount, 30,CrackACirDiameter1,ACircleFit1Center);
					}
				}

				if(A2CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
					MaxDis =0.0f     ;
					MinDis =10000.0f ;
					MaxDv  =0.0f     ;
					MinDv  =10000.0f ;

					CircleCount=0;TotalCirDis=0.0f; AvgCirDis=0.0f;
					for(int k=0; k<A2CirInEdgeCount; k++){
						if(A2CirInPoint[k].x==0 || A2CirInPoint[k].y==0 || k>=MAX_CIRCLE_EDGEPOINT) continue;

						// 					Dis    = Math.GetLengthFrPointToPoint(A2CirCenter,A2CirInPoint[k]);
						Dis    = Math.GetLengthFrPointToPoint(ACircleFit2Center,A2CirInPoint[k]);

						if(Dis>MaxDis) MaxDis = Dis;
						if(Dis<MinDis) MinDis = Dis;

						if(k==0){ // BTM Defect �˻縦 ���� ��ǥ�� �� Dx,Dy ���� 
							A2CirDy = Math.GetLengthFrPointToPoint(ACircleFit2Center,A2CirInPoint[k]);
						} 
						if(k==(A2CirInEdgeCount-1)){
							A2CirDx = Math.GetLengthFrPointToPoint(ACircleFit2Center,A2CirInPoint[k]);
						}

						A2CircleData[CircleCount].Dis  = Dis              ;
						A2CircleData[CircleCount].P.x  = A2CirInPoint[k].x;
						A2CircleData[CircleCount].P.y  = A2CirInPoint[k].y;
						CircleCount++   ;
						TotalCirDis+=Dis;
					}
					if(CircleCount!=0 || TotalCirDis!=0){ // Circle AvgRound Cal
						AvgCirDis = (TotalCirDis/CircleCount);
						A2CirRound = AvgCirDis;
					}

					for(int i=0; i<CircleCount; i++){
						// 					A2CircleData[i].Dv = fabs(AvgCirDis - A2CircleData[i].Dis);
						A2CircleData[i].Dv = (ACirFitDiameter2 - A2CircleData[i].Dis) >0 ?(ACirFitDiameter2 - A2CircleData[i].Dis):0;
						if(A2CircleData[i].Dv>MaxDv) MaxDv = A2CircleData[i].Dv;
						if(A2CircleData[i].Dv<MinDv) MinDv = A2CircleData[i].Dv;
					}
					A2CirDeftectCnt  = GetCircleSize(A2CircleData, A2CirDefectPoint, CircleCount, 5);

					// �ڸ��ҷ� �˻� ������ 
					if(!IsMarkBtm && A2CirInCrackCount>0 && BrokenThresHold!=0){
						for(int i=0; i<A2CirInCrackCount; i++){
							A2CircleData[i].Dv  =0.0f;
							A2CircleData[i].Dis =0.0f;
							A2CircleData[i].P.x  = A2CirInCrackPoint[i].x;
							A2CircleData[i].P.y  = A2CirInCrackPoint[i].y;
						}
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
						A2CirCrackCnt  = GetCircleCrackSize(A2CircleData, A2CirCrackPoint, A2CirInCrackCount, 20,CrackACirDiameter2,ACircleFit2Center);
					}
				}
				// 				LogUnit.SetLog("Mark_Defect_END");     
			}

			// SIDE A �˻� 
			////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(TopEdgePoint[1].x+SideMargin), (int)((TopEdgePoint[1].y+TopEdgePoint[2].y)/2), TopWidth-(2*SideMargin), FTopHeight);
			if((TopEdgePoint[1].x+SideMargin) <=0          || ((TopEdgePoint[1].y+TopEdgePoint[2].y)/2)<=0           || TopWidth-(2*SideMargin)<=0          || FTopHeight<=0          ) return false;
			if((TopEdgePoint[1].x+SideMargin) >=ImageWidth || ((TopEdgePoint[1].y+TopEdgePoint[2].y)/2)>TopLimitLine || TopWidth-(2*SideMargin)>=ImageWidth || FTopHeight>TopLimitLine) return false;
			if((TopEdgePoint[1].x+SideMargin)+(TopWidth-(2*SideMargin)) >=ImageWidth || ((TopEdgePoint[1].y+TopEdgePoint[2].y)/2)+ FTopHeight>TopLimitLine                            ) return false;
			// Blob �˻� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_TopLimit         ,0         , 3          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_LeftLimit ,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(ACount>=MAX_DEFECT_POINT) break;
					ADefectPoint[ACount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					ADefectPoint[ACount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					ADefectPoint[ACount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width/2 ) +(TopEdgePoint[1].x+SideMargin));
					ADefectPoint[ACount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2) +((TopEdgePoint[1].y+TopEdgePoint[2].y)/2));
					ADefectPoint[ACount].Rect.right  =(long)(ADefectPoint[ACount].Rect.left+ ADefectPoint[ACount].Width );
					ADefectPoint[ACount].Rect.bottom =(long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=ADefectPoint[ACount].Rect.top+1 ; n<ADefectPoint[ACount].Rect.bottom-1; n++){
						for(int m=ADefectPoint[ACount].Rect.left+1; m<ADefectPoint[ACount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}

					ADefectPoint[ACount].Round  = PixelCount;
					ADefectPoint[ACount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width/2 )+(TopEdgePoint[1].x+SideMargin));
					ADefectPoint[ACount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2)+((TopEdgePoint[1].y+TopEdgePoint[2].y)/2));


					// Image Rotate ���� ��ǥ�� �ٽ� Rotate�Ѵ� _ Display ���� 
					if(ImgDegree!=0.0f){
						ADefectPoint[ACount].P = Math.OnRotate(CenterPoint, ADefectPoint[ACount].P, (ImgDegree));
					}
					// Image Rotate ���� ��ǥ�� ȸ�� ���Ŀ� ImageHeight���� ���Ͽ� ���� ��ǥ�� �����Ѵ� 
					ADefectPoint[ACount].P.y    =(float)(ADefectPoint[ACount].P.y+ImageHeight);

					//Merget���ؼ� �ʿ��� 
					ADefectPoint[ACount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2) +((TopEdgePoint[1].y+TopEdgePoint[2].y)/2)+ImageHeight);
					ADefectPoint[ACount].Rect.bottom =(long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);
					ACount++;
				}
			}
			BlobCount =0;

			// SIDE B �˻� 
			//Side B
			////////////////////////////////////////////////////////////////////////////////////////////////
			// 			LogUnit.SetLog("CAP_Side_B_Start_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			// 			DefectROI.SetPlacement((int)(TopEdgePoint[3].x-FRightWidth-SideMargin), (int)(TopEdgePoint[3].y+Margin), FRightWidth, (RightHeight-(2*Margin)-TopMargin));
			// 			if((TopEdgePoint[3].x-FRightWidth-SideMargin) <=0          ||(TopEdgePoint[3].y+Margin)<=0           || FRightWidth<=0          || (RightHeight-(2*Margin)-TopMargin)<=0)           return false;
			// 			if((TopEdgePoint[3].x-FRightWidth-SideMargin) >=ImageWidth ||(TopEdgePoint[3].y+Margin)>TopLimitLine || FRightWidth>=ImageWidth || (RightHeight-(2*Margin)-TopMargin)>TopLimitLine) return false;
			// 			if((TopEdgePoint[3].x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth ||((TopEdgePoint[3].y+Margin)+(RightHeight-(2*Margin)-TopMargin))>TopLimitLine) return false;

			DefectROI.SetPlacement((int)(TopEdgePoint[3].x-FRightWidth-SideMargin), (int)(TopEdgePoint[3].y+Margin), FRightWidth, (RightHeight-(2*Margin)-RightDegMargin));
			if((TopEdgePoint[3].x-FRightWidth-SideMargin) <=0          ||(TopEdgePoint[3].y+Margin)<=0           || FRightWidth<=0          || (RightHeight-(2*Margin)-RightDegMargin)<=0)           return false;
			if((TopEdgePoint[3].x-FRightWidth-SideMargin) >=ImageWidth ||(TopEdgePoint[3].y+Margin)>TopLimitLine || FRightWidth>=ImageWidth || (RightHeight-(2*Margin)-RightDegMargin)>TopLimitLine) return false;
			if((TopEdgePoint[3].x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth ||((TopEdgePoint[3].y+Margin)+(RightHeight-(2*Margin)-RightDegMargin))>TopLimitLine) return false;

			//  Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth ,FMinWidth     , FMaxWidth    , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight,FMinHeight    , FMaxHeight   , EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit     ,FRightWidth-2 , FRightWidth+1, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit     ,FRightWidth-1 , FRightWidth+1, EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(BCount>=MAX_DEFECT_POINT) break;
					BDefectPoint[BCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					BDefectPoint[BCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					BDefectPoint[BCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +(TopEdgePoint[3].x-FRightWidth-SideMargin));
					BDefectPoint[BCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(TopEdgePoint[3].y+Margin)                );
					BDefectPoint[BCount].Rect.right  = (long)(BDefectPoint[BCount].Rect.left+ BDefectPoint[BCount].Width );
					BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
						for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}
					BDefectPoint[BCount].Round  = PixelCount;
					BDefectPoint[BCount].P.x    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +(TopEdgePoint[3].x-FRightWidth-SideMargin));
					BDefectPoint[BCount].P.y    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(TopEdgePoint[3].y+Margin)+ImageHeight    );
					BDefectPoint[BCount].Width  = BDefectPoint[BCount].Width+SideMargin; 

					// Merge ���ؼ� ������ �ʿ��� 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) + (TopEdgePoint[3].y+Margin)+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount =0;

			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = ((Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x);
			else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP0.x - FP1.x);
			else                                                      RDistance = ((Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x);

			// SIDE D �˻� 
			////////////////////////////////////////////////////////////////////////////////////////////////
			// 			LogUnit.SetLog("CAP_Side_D_Start_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			// 			DefectROI.SetPlacement((int)(TopEdgePoint[0].x+SideMargin), (int)(TopEdgePoint[0].y+Margin), FLeftWidth, (LeftHeight-(2*Margin)-TopMargin));
			// 			if((TopEdgePoint[0].x+SideMargin) <=0          ||(TopEdgePoint[0].y+Margin)<=0           || FLeftWidth<=0          || (LeftHeight-(2*Margin)-TopMargin)<=0          ) return false;
			// 			if((TopEdgePoint[0].x+SideMargin) >=ImageWidth ||(TopEdgePoint[0].y+Margin)>TopLimitLine || FLeftWidth>=ImageWidth || (LeftHeight-(2*Margin)-TopMargin)>TopLimitLine) return false;
			// 			if((TopEdgePoint[0].x+SideMargin)+FLeftWidth >=ImageWidth ||(TopEdgePoint[0].y+Margin)+(LeftHeight-(2*Margin)-TopMargin)>TopLimitLine) return false;

			DefectROI.SetPlacement((int)(TopEdgePoint[0].x+SideMargin), (int)(TopEdgePoint[0].y+Margin), FLeftWidth, (LeftHeight-(2*Margin)-LeftDegMargin));
			if((TopEdgePoint[0].x+SideMargin) <=0          ||(TopEdgePoint[0].y+Margin)<=0           || FLeftWidth<=0          || (LeftHeight-(2*Margin)-LeftDegMargin)<=0          ) return false;
			if((TopEdgePoint[0].x+SideMargin) >=ImageWidth ||(TopEdgePoint[0].y+Margin)>TopLimitLine || FLeftWidth>=ImageWidth || (LeftHeight-(2*Margin)-LeftDegMargin)>TopLimitLine) return false;
			if((TopEdgePoint[0].x+SideMargin)+FLeftWidth >=ImageWidth ||(TopEdgePoint[0].y+Margin)+(LeftHeight-(2*Margin)-LeftDegMargin)>TopLimitLine) return false;

			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);

			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 3          , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(DCount>=MAX_DEFECT_POINT) break;
					DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +(TopEdgePoint[0].x+SideMargin));
					DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(TopEdgePoint[0].y+Margin)    );
					DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
					DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

					PixelCount = 0;
					PixelValue = 0;
					// ���� Pixel Data ���� 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}
					// Display ������ �ʿ��� 
					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
					DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(TopEdgePoint[0].y+Margin)+ImageHeight );
					DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

					// Merge ���ؼ� ������ �ʿ��� 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(TopEdgePoint[0].y+Margin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount =0;

			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = (FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2));
			else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = (FP0.x -  Line[waLeftWall].FP0.x)                                                     ;
			else                                                    LDistance = (FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2)); 
		}


		// Cell Size ���� ������ �Ѱ���(Image ȸ�� �������)
		LeftStaPos.x   =  FLTPoint.x            ;
		LeftStaPos.y   =  FLTPoint.y+ImageHeight;
		CenterStaPos.x =  FLTPoint.x + (FRTPoint.x - FLTPoint.x)/2            ; 
		CenterStaPos.y = (FLTPoint.y + FRTPoint.y)              /2+ImageHeight; 

		// FLTPoint,FRTPoint ���� Image Rotate �����Ͽ� Display �������� �����̹��� ���� ��ǥ�� ������Ų
		if(ImgDegree!=0.0f){
			// �˻翵�� �и��� ������� ������ ���� �����Ͽ� �ٽ� ����ؾ� �ϴ� ��ǥ���� 
			// TopLine 
			TopOutLine[0].FP0   = Math.OnRotate(CenterPoint , TopOutLine[0].FP0 , (ImgDegree));
			TopOutLine[0].FP1   = Math.OnRotate(CenterPoint , TopOutLine[0].FP1 , (ImgDegree));
							    
			// LeftOutLIne 	    
			LeftOutLine[0].FP0  = Math.OnRotate(CenterPoint, LeftOutLine[0].FP0 , (ImgDegree));
			LeftOutLine[0].FP1  = Math.OnRotate(CenterPoint, LeftOutLine[0].FP1 , (ImgDegree));
							    
			// LeftInLIne 	    
			LeftInLine[0].FP0   = Math.OnRotate(CenterPoint , LeftInLine[0].FP0 , (ImgDegree));
			LeftInLine[0].FP1   = Math.OnRotate(CenterPoint , LeftInLine[0].FP1 , (ImgDegree));
							    
			// LeftStdLIne 	    
			LeftStdLine[0].FP0  = Math.OnRotate(CenterPoint, LeftStdLine[0].FP0 , (ImgDegree));
			LeftStdLine[0].FP1  = Math.OnRotate(CenterPoint, LeftStdLine[0].FP1 , (ImgDegree));

			// RightOutLIne 
			RightOutLine[0].FP0 = Math.OnRotate(CenterPoint, RightOutLine[0].FP0, (ImgDegree));
			RightOutLine[0].FP1 = Math.OnRotate(CenterPoint, RightOutLine[0].FP1, (ImgDegree));

			// RightOutLIne 
			RightInLine[0].FP0  = Math.OnRotate(CenterPoint, RightInLine[0].FP0 , (ImgDegree));
			RightInLine[0].FP1  = Math.OnRotate(CenterPoint, RightInLine[0].FP1 , (ImgDegree));

			// RightStdLIne 
			RightStdLine[0].FP0 = Math.OnRotate(CenterPoint, RightStdLine[0].FP0, (ImgDegree));
			RightStdLine[0].FP1 = Math.OnRotate(CenterPoint, RightStdLine[0].FP1, (ImgDegree));

			FRTPoint    = Math.OnRotate(CenterPoint, FRTPoint     , (ImgDegree));
			FLTPoint    = Math.OnRotate(CenterPoint, FLTPoint     , (ImgDegree));

			// Cell Size ���� ������ �Ѱ���(Image  ȸ��)
			LeftStaPos.x   =  FLTPoint.x            ;
			LeftStaPos.y   =  FLTPoint.y+ImageHeight;
			CenterStaPos.x =  FLTPoint.x + (FRTPoint.x - FLTPoint.x)/2            ; 
			CenterStaPos.y = (FLTPoint.y + FRTPoint.y)              /2+ImageHeight; 

			if(IsCornerTop){
				FLTTPoint   = Math.OnRotate(CenterPoint, FLTTPoint    , (ImgDegree));
				FLTLPoint   = Math.OnRotate(CenterPoint, FLTLPoint    , (ImgDegree));
				FLTTInPoint = Math.OnRotate(CenterPoint, FLTTInPoint  , (ImgDegree));
				FLTLInPoint = Math.OnRotate(CenterPoint, FLTLInPoint  , (ImgDegree));
				FLTTMeaPoint= Math.OnRotate(CenterPoint, FLTTMeaPoint , (ImgDegree));
				FLTLMeaPoint= Math.OnRotate(CenterPoint, FLTLMeaPoint , (ImgDegree));

				FRTTPoint   = Math.OnRotate(CenterPoint, FRTTPoint    , (ImgDegree));
				FRTRPoint   = Math.OnRotate(CenterPoint, FRTRPoint    , (ImgDegree));
				FRTTInPoint = Math.OnRotate(CenterPoint, FRTTInPoint  , (ImgDegree));
				FRTRInPoint = Math.OnRotate(CenterPoint, FRTRInPoint  , (ImgDegree));
				FRTTMeaPoint= Math.OnRotate(CenterPoint, FRTTMeaPoint , (ImgDegree));
				FRTRMeaPoint= Math.OnRotate(CenterPoint, FRTRMeaPoint , (ImgDegree));

				// Corner Defect ��ǥ�� ���� �����Ͽ� �����ؾ��� 
				if(C1Count!=0){
					for(int k=0; k<C1Count; k++){
						// Fram���� Y��ǥ������ �����Ѵ� 
						C1DefectPoint[k].P.y = C1DefectPoint[k].P.y - ImageHeight;
						if(C1DefectPoint[k].P.y>0) {
							C1DefectPoint[k].P   = Math.OnRotate(CenterPoint,C1DefectPoint[k].P,(ImgDegree)); 
							C1DefectPoint[k].P.y = C1DefectPoint[k].P.y + ImageHeight;
						}
						else {
							C1DefectPoint[k].P.y = C1DefectPoint[k].P.y + ImageHeight;
						}
					}
				}

				if(C2Count!=0){
					for(int k=0; k<C2Count; k++){
						// Fram���� Y��ǥ������ �����Ѵ� 
						C2DefectPoint[k].P.y = C2DefectPoint[k].P.y - ImageHeight;
						if(C2DefectPoint[k].P.y>0) {
							C2DefectPoint[k].P   = Math.OnRotate(CenterPoint,C2DefectPoint[k].P,(ImgDegree)); 
							C2DefectPoint[k].P.y = C2DefectPoint[k].P.y + ImageHeight;
						}
						else {
							C2DefectPoint[k].P.y = C2DefectPoint[k].P.y + ImageHeight;
						}
					}
				}
			}
			else if(IsCircleTop) { // Display �������� ���� ���� �������� ��ǥ���� �ٽ� ȸ�� ���Ѿ� �Ѵ�   
				// A���� Circle_Defect ��ǥ �̹��� ���� ȸ����
				if(A1CirDeftectCnt!=0){
					for(int k=0; k<A1CirDeftectCnt; k++){
						A1CirDefectPoint[k].P.y = A1CirDefectPoint[k].P.y - ImageHeight;
						if(A1CirDefectPoint[k].P.y>0) {
							A1CirDefectPoint[k].P   = Math.OnRotate(CenterPoint,A1CirDefectPoint[k].P,(ImgDegree)); 
							A1CirDefectPoint[k].P.y = A1CirDefectPoint[k].P.y + ImageHeight;
						}
						else {
							A1CirDefectPoint[k].P.y = A1CirDefectPoint[k].P.y + ImageHeight;
						}
					}
				}

				if(A2CirDeftectCnt!=0){
					// A���� Circle_Defect ��ǥ �̹��� ���� ȸ����
					for(int k=0; k<A2CirDeftectCnt; k++){
						A2CirDefectPoint[k].P.y = A2CirDefectPoint[k].P.y - ImageHeight;
						if(A2CirDefectPoint[k].P.y>0) {
							A2CirDefectPoint[k].P   = Math.OnRotate(CenterPoint,A2CirDefectPoint[k].P,(ImgDegree)); 
							A2CirDefectPoint[k].P.y = A2CirDefectPoint[k].P.y + ImageHeight;
						}
						else {
							A2CirDefectPoint[k].P.y = A2CirDefectPoint[k].P.y + ImageHeight;
						}
					}
				}

				// Circle Defect Data 
				// Bright Defect Data 
				////////////////////////////////////////////////////////////////////////////////////////////////
				if(A1CirCrackCnt!=0){
					for(int k=0; k<A1CirCrackCnt; k++){
						A1CirCrackPoint[k].P.y = A1CirCrackPoint[k].P.y - ImageHeight;
						if(A1CirCrackPoint[k].P.y>0) {
							A1CirCrackPoint[k].P   = Math.OnRotate(CenterPoint,A1CirCrackPoint[k].P,(ImgDegree)); 
							A1CirCrackPoint[k].P.y = A1CirCrackPoint[k].P.y + ImageHeight;
						}
						else {
							A1CirCrackPoint[k].P.y = A1CirCrackPoint[k].P.y + ImageHeight;
						}
					}
				}

				if(A2CirCrackCnt!=0){
					for(int k=0; k<A2CirCrackCnt; k++){
						A2CirCrackPoint[k].P.y = A2CirCrackPoint[k].P.y - ImageHeight;
						if(A2CirCrackPoint[k].P.y>0) {
							A2CirCrackPoint[k].P   = Math.OnRotate(CenterPoint,A2CirCrackPoint[k].P,(ImgDegree)); 
							A2CirCrackPoint[k].P.y = A2CirCrackPoint[k].P.y + ImageHeight;
						}
						else {
							A2CirCrackPoint[k].P.y = A2CirCrackPoint[k].P.y + ImageHeight;
						}
					}
				}
				//////////////////////////////////////////////////////////////////////////////////////

				if(A1CirInEdgeCount!=0){
					for(int k=0; k<A1CirInEdgeCount; k++){
						if(A1CirInPoint[k].x==0.0f || A1CirInPoint[k].y==0.0f) continue;
						A1CirInPoint[k] = Math.OnRotate(CenterPoint, A1CirInPoint[k], ImgDegree);
					}
				}
				if(A1CirOutEdgeCount!=0){
					for(int k=0; k<A1CirOutEdgeCount; k++){
						if(A1CirOutPoint[k].x==0.0f || A1CirOutPoint[k].y==0.0f) continue;
						A1CirOutPoint[k] = Math.OnRotate(CenterPoint, A1CirOutPoint[k], ImgDegree);
					}
				}

				if(A1CirCenter      .x!=0.0f && A1CirCenter      .y!=0.0f) A1CirCenter        = Math.OnRotate(CenterPoint, A1CirCenter       , (ImgDegree)); // �ܰ� ������ ����  
				if(RealA1CirCenter  .x!=0.0f && RealA1CirCenter  .y!=0.0f) RealA1CirCenter    = Math.OnRotate(CenterPoint, RealA1CirCenter   , (ImgDegree)); // Euresys 
				if(ACircleFit1Center.x!=0.0f && ACircleFit1Center.y!=0.0f) ACircleFit1Center  = Math.OnRotate(CenterPoint, ACircleFit1Center , (ImgDegree)); // FAVION

				if(A2CirInEdgeCount!=0){
					for(int k=0; k<A2CirInEdgeCount; k++){
						if(A2CirInPoint[k].x==0.0f || A2CirInPoint[k].y==0.0f) continue;
						A2CirInPoint[k] = Math.OnRotate(CenterPoint, A2CirInPoint[k], ImgDegree);
					}
				}
				if(A2CirOutEdgeCount!=0){
					for(int k=0; k<A2CirOutEdgeCount; k++){
						if(A2CirOutPoint[k].x==0.0f || A2CirOutPoint[k].y==0.0f) continue;
						A2CirOutPoint[k] = Math.OnRotate(CenterPoint, A2CirOutPoint[k], ImgDegree);
					}
				}

				if(A2CirCenter      .x!=0.0f && A2CirCenter      .y!=0.0f) A2CirCenter        = Math.OnRotate(CenterPoint, A2CirCenter       , (ImgDegree)); // �ܰ� ������ ����  
				if(RealA2CirCenter  .x!=0.0f && RealA2CirCenter  .y!=0.0f) RealA2CirCenter    = Math.OnRotate(CenterPoint, RealA2CirCenter   , (ImgDegree)); // Euresys 
				if(ACircleFit2Center.x!=0.0f && ACircleFit2Center.y!=0.0f) ACircleFit2Center  = Math.OnRotate(CenterPoint, ACircleFit2Center , (ImgDegree)); // FAVION
			}

			// GrindMarkPoint Rotate (Display ����) 
			if(IsMarkBtm){
				for(int k=0; k<2; k++){
					LtPos.x =0.0f, LtPos.y=0.0f,RbPos.x=0.0f,RbPos.y=0.0f;
					if(AGrindMark[k].Rect.left==0 || AGrindMark[k].Rect.top==0 || AGrindMark[k].Rect.right==0 || AGrindMark[k].Rect.bottom==0 ) continue;

					LtPos.x = (float)AGrindMark[k].Rect.left  ;
					LtPos.y = (float)AGrindMark[k].Rect.top   ;
					RbPos.x = (float)AGrindMark[k].Rect.right ;
					RbPos.y = (float)AGrindMark[k].Rect.bottom;
					LtPos = Math.OnRotate(CenterPoint, LtPos,ImgDegree);
					RbPos = Math.OnRotate(CenterPoint, RbPos,ImgDegree);

					AGrindMark[k].Rect.left   = (long)LtPos.x;
					AGrindMark[k].Rect.top    = (long)LtPos.y;
					AGrindMark[k].Rect.right  = (long)RbPos.x;
					AGrindMark[k].Rect.bottom = (long)RbPos.y;
					if(AGrindMark[k].Point.x!=0.0f && AGrindMark[k].Point.y!=0.0f) {
						AGrindMark[k].Point= Math.OnRotate(CenterPoint, AGrindMark[k].Point,ImgDegree);
					}
				}

				// Pad ROI Rect Rotate (Display ����) 
				for(int k=0; k<2; k++){
					LtPos.x =0.0f, LtPos.y=0.0f,RbPos.x=0.0f,RbPos.y=0.0f;
					if(PadPolyRect[k].left==0 || PadPolyRect[k].top==0 || PadPolyRect[k].right==0 || PadPolyRect[k].bottom==0 ) continue;

					LtPos.x = (float)PadPolyRect[k].left  ;
					LtPos.y = (float)PadPolyRect[k].top   ;
					RbPos.x = (float)PadPolyRect[k].right ;
					RbPos.y = (float)PadPolyRect[k].bottom;
					LtPos = Math.OnRotate(CenterPoint, LtPos,ImgDegree);
					RbPos = Math.OnRotate(CenterPoint, RbPos,ImgDegree);

					PadPolyRect[k].left   = (long)LtPos.x;
					PadPolyRect[k].top    = (long)LtPos.y;
					PadPolyRect[k].right  = (long)RbPos.x;
					PadPolyRect[k].bottom = (long)RbPos.y;
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
		Index = 0;
		return true;

		// NOTCH RectC : ������ ���
		//------------------------------------------------------------------
	case 1 :
		// Side B
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//LogUnit.SetLog("STATIC_Side_B_START_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
		DefectROI.SetPlacement((int)(RSP.x-FRightWidth-SideMargin), (int)RSP.y, FRightWidth, RightHeight);
		if((RSP.x-FRightWidth-SideMargin) <=0          ||(RSP.y)<0           || FRightWidth<=0          || RightHeight<0         )return false;
		if((RSP.x-FRightWidth-SideMargin) >=ImageWidth ||(RSP.y)>ImageHeight || FRightWidth>=ImageWidth || RightHeight>ImageHeight)return false;
		if((RSP.x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth ||(RSP.y)+RightHeight>ImageHeight                             )return false;
		//Line Position
		LinePosition = RSP.x-FRightWidth-SideMargin;
		// Blob �˻� ������ 
		CodeImageSelection.Clear();
		CodeImageEncoder.Encode(DefectROI,CodeImage);
		CodeImageSelection.Clear();
		CodeImageSelection.AddObjects(CodeImage);
		// Blob ���͸� 
		CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth    , FMaxWidth     , EDoubleThresholdMode_Outside);
		CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight   , FMaxHeight    , EDoubleThresholdMode_Outside);
		CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit       ,FRightWidth-1, FRightWidth+1 , EDoubleThresholdMode_Outside);

		BlobCount = CodeImageSelection.GetElementCount();
		CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

		if(BlobCount!=0){
			for(int k=0; k<BlobCount; k++){
				if(BCount>=MAX_DEFECT_POINT) break; 
				BDefectPoint[BCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
				BDefectPoint[BCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

				BDefectPoint[BCount].Rect.left   =(long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2) +LinePosition));
				BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)               );
				BDefectPoint[BCount].Rect.right  =(long)(BDefectPoint[BCount].Rect.left+ BDefectPoint[BCount].Width );
				BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);

				PixelCount = 0;
				PixelValue = 0;

				// ���� Pixel Data ���� 
				for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
					for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
						PixelValue = IElement(m,n);
						if(PixelValue < EnCapMidThresHold) PixelCount++;
					}
				}

				BDefectPoint[BCount].Round  = PixelCount;
				BDefectPoint[BCount].P.x    =(float)(BDefectPoint[BCount].Rect.left);
				BDefectPoint[BCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +ImageHeight );
				BDefectPoint[BCount].Width  = (InspectMode)? BDefectPoint[BCount].Width :BDefectPoint[BCount].Width+SideMargin; 

				// Merge ���ؼ� ������ �ʿ��� 
				BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +ImageHeight);
				BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
				BCount++;
			}
		}
		BlobCount =0;
		//  		LogUnit.SetLog("STATIC_Side_B_END_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
		// �ܰ����ο��� Mark������ �Ÿ� ���� 
		// CellSize 
		if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x;
		else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance =  Line[waRightWall].FP0.x - FP1.x;
		else                                                      RDistance = (Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x;

		// SIDE-D
		DefectROI.SetPlacement((int)(LSP.x+SideMargin),(int)LSP.y, FLeftWidth, LeftHeight);
		if((LSP.x+SideMargin) <=0          || (LSP.y)<0           || FLeftWidth<=0          || LeftHeight<0          ) return false;
		if((LSP.x+SideMargin) >=ImageWidth || (LSP.y)>ImageHeight || FLeftWidth>=ImageWidth || LeftHeight>ImageHeight) return false;
		if((LSP.x+SideMargin)+FLeftWidth >=ImageWidth || (LSP.y)+LeftHeight>ImageHeight                              ) return false;
		// Line Position
		LinePosition = LSP.x+SideMargin;
		// Blob �˻� ���� 
		CodeImageSelection.Clear();
		CodeImageEncoder.Encode(DefectROI,CodeImage);
		CodeImageSelection.Clear();
		CodeImageSelection.AddObjects(CodeImage);
		// Blob ���͸� 
		CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
		CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
		CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 1          , EDoubleThresholdMode_Outside);

		BlobCount = CodeImageSelection.GetElementCount();
		CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

		if(BlobCount!=0){
			for(int k=0; k<BlobCount; k++){
				if(DCount>=MAX_DEFECT_POINT) break; 
				DDefectPoint[DCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
				DDefectPoint[DCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

				DDefectPoint[DCount].Rect.left   =(long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +LinePosition));
				DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)                );
				DDefectPoint[DCount].Rect.right  =(long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width) ; 
				DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

				PixelCount =0;
				PixelValue =0;

				// ���� Pixel Data ���� 
				for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
					for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
						PixelValue = IElement(m,n);
						if(PixelValue < EnCapMidThresHold) PixelCount++;
					}
				}

				DDefectPoint[DCount].Round = PixelCount;
				DDefectPoint[DCount].P.x    =(float)(DDefectPoint[DCount].Rect.left); 
				DDefectPoint[DCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ImageHeight  );
				DDefectPoint[DCount].Width  = (InspectMode)? DDefectPoint[DCount].Width :DDefectPoint[DCount].Width+SideMargin; 

				// Merge ���ؼ� ������ �ʿ��� 
				DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ImageHeight);
				DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
				DCount++;
			}
		}
		BlobCount =0;
		//  		LogUnit.SetLog("STATIC_Side_D_END_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
		// Notch Cell Size Calc
		if(LSP.x!=0.0f  && RSP.x!=0.0f ){
			CenterCellSize = Math.GetLengthFrPointToPoint(LSP,RSP);
			TotalCellSizeWidth+=CenterCellSize;
			TotalCellSizeCount++;
		}
		// �ܰ����ο��� Mark������ �Ÿ� ���� 
		// CellSize 
		if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2);
		else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = FP0.x -  Line[waLeftWall].FP0.x;
		else                                                    LDistance = FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2); 
		////////////////////////////////////////////////////////////////////////////////////////////////

		// Broken �� �ڸ� �ҷ� �˻� ������(EnCap�� ������)
		if(!IsMarkBtm && BrokenThresHold!=0){
			RotateBrokenImg.SetSize(GImage->GetWidth(), GImage->GetHeight())       ;
			EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&RotateBrokenImg);
			EasyImage::Threshold(GImage    ,&RotateBrokenImg, BrokenThresHold)     ;

			DefectROI.Attach(&RotateBrokenImg);
			CodeImageSelection.SetFeretAngle(0.00f);
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
			IAddress = RotateBrokenImg.GetImagePtr();

			FRightWidth = 60; // �׽�Ʈ ����
			FLeftWidth  = 60; // �׽�Ʈ ����

			// Side_B Bright Defect ���� �� �з�(Broken,�ݿ��� �ڸ�___�ɹڸ�)
			/////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(RSP.x-FRightWidth-SideMargin), (int)RSP.y, FRightWidth, RightHeight);
			if((RSP.x-FRightWidth-SideMargin) <=0          ||(RSP.y)<0           || FRightWidth<=0          || RightHeight<0          )return false;
			if((RSP.x-FRightWidth-SideMargin) >=ImageWidth ||(RSP.y)>ImageHeight || FRightWidth>=ImageWidth || RightHeight>ImageHeight)return false;
			if((RSP.x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth ||(RSP.y)+RightHeight>ImageHeight                              )return false;
			//Line Position
			LinePosition = RSP.x-FRightWidth-SideMargin;
			// Blob �˻� ������ 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth    , FMaxWidth     , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight   , FMaxHeight    , EDoubleThresholdMode_Outside);
			// 			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit       ,FRightWidth-1, FRightWidth+1 , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(BCount>=MAX_DEFECT_POINT) break; 
					BCrackPoint[BCrackCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
					BCrackPoint[BCrackCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					BCrackPoint[BCrackCount].Rect.left   =(long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BCrackPoint[BCrackCount].Width/2) +LinePosition));
					BCrackPoint[BCrackCount].Rect.top    =(long)(( CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BCrackPoint[BCrackCount].Height/2)              );
					BCrackPoint[BCrackCount].Rect.right  =(long)(BCrackPoint[BCrackCount].Rect.left+ BCrackPoint[BCrackCount].Width );
					BCrackPoint[BCrackCount].Rect.bottom =(long)(BCrackPoint[BCrackCount].Rect.top + BCrackPoint[BCrackCount].Height);
					BCrackPoint[BCrackCount].LineAttach  =((CodeImageSelection.GetElement(k).GetRightLimit() > (FRightWidth-5))>0)? 1:0; //LineAttach:1 �ݿ��� �ڸ�, 0: �ɹڸ� 

					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=BCrackPoint[BCrackCount].Rect.top+1 ; n<BCrackPoint[BCrackCount].Rect.bottom-1; n++){
						for(int m=BCrackPoint[BCrackCount].Rect.left+1; m<BCrackPoint[BCrackCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue > BrokenThresHold) PixelCount++;
						}
					}

					BCrackPoint[BCrackCount].Round  = PixelCount;
					BCrackPoint[BCrackCount].P.x    =(float)(BCrackPoint[BCrackCount].Rect.left);
					BCrackPoint[BCrackCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BCrackPoint[BCrackCount].Height/2) +ImageHeight );
					BCrackPoint[BCrackCount].Width  = (InspectMode)? BCrackPoint[BCrackCount].Width :BCrackPoint[BCrackCount].Width+SideMargin; 

					// Merge ���ؼ� ������ �ʿ��� 
					BCrackPoint[BCrackCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BCrackPoint[BCrackCount].Height/2) +ImageHeight);
					BCrackPoint[BCrackCount].Rect.bottom =(long)(BCrackPoint[BCrackCount].Rect.top + BCrackPoint[BCrackCount].Height);
					BCrackCount++;
				}
			}
			BlobCount =0;

			// Side_D Bright Defect ���� �� �з�(Broken,�ݿ��� �ڸ�___�ɹڸ�)
			/////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(LSP.x+SideMargin),(int)LSP.y, FLeftWidth, LeftHeight);
			if((LSP.x+SideMargin) <=0          || (LSP.y)<0           || FLeftWidth<=0          || LeftHeight<0          ) return false;
			if((LSP.x+SideMargin) >=ImageWidth || (LSP.y)>ImageHeight || FLeftWidth>=ImageWidth || LeftHeight>ImageHeight) return false;
			if((LSP.x+SideMargin)+FLeftWidth >=ImageWidth || (LSP.y)+LeftHeight>ImageHeight                              ) return false;
			// Line Position
			LinePosition = LSP.x+SideMargin;
			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			// 			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 1          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(BCount>=MAX_DEFECT_POINT) break; 
					DCrackPoint[DCrackCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
					DCrackPoint[DCrackCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DCrackPoint[DCrackCount].Rect.left   =(long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width/2) +LinePosition));
					DCrackPoint[DCrackCount].Rect.top    =(long)(( CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/2)              );
					DCrackPoint[DCrackCount].Rect.right  =(long)(DCrackPoint[DCrackCount].Rect.left+ DCrackPoint[DCrackCount].Width );
					DCrackPoint[DCrackCount].Rect.bottom =(long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
					DCrackPoint[DCrackCount].LineAttach  =(CodeImageSelection.GetElement(k).GetLeftLimit() < 5)? 1:0; //LineAttach:1 �ݿ��� �ڸ�, 0: �ɹڸ� 

					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=DCrackPoint[DCrackCount].Rect.top+1 ; n<DCrackPoint[DCrackCount].Rect.bottom-1; n++){
						for(int m=DCrackPoint[DCrackCount].Rect.left+1; m<DCrackPoint[DCrackCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue > BrokenThresHold) PixelCount++;
						}
					}

					DCrackPoint[DCrackCount].Round  = PixelCount;
					DCrackPoint[DCrackCount].P.x    =(float)(DCrackPoint[DCrackCount].Rect.left);
					DCrackPoint[DCrackCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/2) +ImageHeight );
					DCrackPoint[DCrackCount].Width  = (InspectMode)? DCrackPoint[DCrackCount].Width :DCrackPoint[DCrackCount].Width+SideMargin; 

					// Merge ���ؼ� ������ �ʿ��� 
					DCrackPoint[DCrackCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/2) +ImageHeight);
					DCrackPoint[DCrackCount].Rect.bottom =(long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
					DCrackCount++;
				}
			}
			BlobCount =0;
		}
		Index = 0;
		return true;

		// NOTCH RectC : �Ÿ��
		//------------------------------------------------------------------
	case 2 :

		if(IsCornerBtm)     { //Corner Cutting �ϰ�� Defect �˻� ������ 
			if(C3InEdgeCount!=0){
				CornerCount=0;
				for(int k=0; k<C3InEdgeCount; k++){
					if(C3InPoint[k].y<FLBLPoint.y-1) continue;
					Dis = Math.GetLengthFrLineToPoint(FLBLInPoint, FLBBInPoint, C3InPoint[k]);
					C3Corner[CornerCount].Dv   = Dis         ;
					C3Corner[CornerCount].P.x  = C3InPoint[k].x;
					C3Corner[CornerCount].P.y  = C3InPoint[k].y;
					CornerCount++;
				}
				C3Count = GetSize(C3Corner, C3DefectPoint, CornerCount, 5);
			}

			if(C4InEdgeCount!=0){
				CornerCount=0;
				for(int k=0; k<C4InEdgeCount; k++){
					if(C4InPoint[k].y<FRBRPoint.y-1) continue;
					Dis = Math.GetLengthFrLineToPoint(FRBRInPoint, FRBBInPoint, C4InPoint[k]);
					C4Corner[CornerCount].Dv   = Dis         ;
					C4Corner[CornerCount].P.x  = C4InPoint[k].x;
					C4Corner[CornerCount].P.y  = C4InPoint[k].y;
					CornerCount++;
				}
				C4Count = GetSize(C4Corner, C4DefectPoint, CornerCount, 5);
			}
			// Side D 
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//LogUnit.SetLog("CUP_Side_D_Start_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			DefectROI.SetPlacement((int)(SP.x+SideMargin),(int)(SP.y+BtmMargin), FLeftWidth, LeftHeight-(2*BtmMargin));
			if((SP.x+SideMargin) <=0          || SP.y+BtmMargin< 0            || FLeftWidth<=0          || LeftHeight-(2*BtmMargin)< 0           ) return false;
			if((SP.x+SideMargin) >=ImageWidth || SP.y+BtmMargin> BtmLimitLine || FLeftWidth>=ImageWidth || LeftHeight-(2*BtmMargin)> BtmLimitLine) return false;
			if((SP.x+SideMargin)+FLeftWidth >=ImageWidth || (SP.y+BtmMargin)+(LeftHeight-(2*BtmMargin))> BtmLimitLine                            ) return false;

			// Line Position 
			LinePosition = SP.x+SideMargin;
			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 3          , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 1          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(DCount>=MAX_DEFECT_POINT) break;
					DDefectPoint[DCount].Width  =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					DDefectPoint[DCount].Height =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DDefectPoint[DCount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +LinePosition );
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(SP.y+BtmMargin));
					DDefectPoint[DCount].Rect.right  =(long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					PixelCount =0;
					PixelValue =0;

					// ���� Pixel Data ���� 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
						}
					}

					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +LinePosition);
					DDefectPoint[DCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(SP.y+BtmMargin)+ImageHeight);
					DDefectPoint[DCount].Width  = (InspectMode)?DDefectPoint[DCount].Width: DDefectPoint[DCount].Width+SideMargin;

					// Merge ���ؼ� ������ �ʿ��� 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(SP.y+BtmMargin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount=0;
			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2);
			else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = FP0.x -  Line[waLeftWall].FP0.x;
			else                                                    LDistance = FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2); 
			////////////////////////////////////////////////////////////////////////////////////////////////


			// Side B
			////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(EP.x-FRightWidth-SideMargin),(int)(EP.y+BtmMargin), FRightWidth,  RightHeight-(2*BtmMargin));
			if((EP.x-FRightWidth-SideMargin) <=0          || (EP.y+BtmMargin)< 0            || FRightWidth<=0           || RightHeight-(2*BtmMargin)< 0           ) return false;
			if((EP.x-FRightWidth-SideMargin) >=ImageWidth || (EP.y+BtmMargin)> BtmLimitLine || FRightWidth>=ImageWidth  || RightHeight-(2*BtmMargin)> BtmLimitLine) return false;
			if((EP.x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth || (EP.y+BtmMargin)+(RightHeight-(2*BtmMargin))> BtmLimitLine                               ) return false;

			// Line Position 
			LinePosition = EP.x - FRightWidth- SideMargin;
			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth               , FMaxWidth                , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight              , FMaxHeight               , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit     ,FRightWidth -1            , FRightWidth+1            , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(BCount>=MAX_DEFECT_POINT) break;
					BDefectPoint[BCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ; 
					BDefectPoint[BCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					BDefectPoint[BCount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +(LinePosition)  );
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(EP.y+BtmMargin));
					BDefectPoint[BCount].Rect.right  =(long)(BDefectPoint[BCount].Rect.left+ BDefectPoint[BCount].Width );
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
						for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
						}
					}
					BDefectPoint[BCount].Round  = PixelCount;
					BDefectPoint[BCount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 )+(LinePosition));
					BDefectPoint[BCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)+(EP.y+BtmMargin)+ImageHeight   );
					BDefectPoint[BCount].Width  =(InspectMode)? BDefectPoint[BCount].Width: BDefectPoint[BCount].Width+SideMargin;

					// Merge ���ؼ� ������ �ʿ��� 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(EP.y+BtmMargin)+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount =0;
			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x;
			else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance =  Line[waRightWall].FP0.x - FP1.x;
			else                                                      RDistance = (Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x;
			////////////////////////////////////////////////////////////////////////////////////////////////


			// SIDE C 
			// Btm�˻�� ����ų�� ������ ���ؼ� 
			if(fabs(FLBBPoint.y - FRBBPoint.y)>3) BTMLine = (FLBBPoint.y > FRBBPoint.y) ?(int)FRBBPoint.y : (int)FLBBPoint.y ;
			else                                  BTMLine = (FLBBPoint.y > FRBBPoint.y) ?(int)FLBBPoint.y : (int)FRBBPoint.y ;

			DefectROI.SetPlacement((int)FLBBPoint.x+SideMargin, (int)(BTMLine-FBtmHeight), BtmWidth -(2*SideMargin), FBtmHeight);
			if((FLBBPoint.x+SideMargin) <=0          ||(BTMLine-FBtmHeight)< 0            || BtmWidth -(2*SideMargin)<=0          || FBtmHeight< 0           ) return false;
			if((FLBBPoint.x+SideMargin) >=ImageWidth ||(BTMLine-FBtmHeight)> BtmLimitLine || BtmWidth -(2*SideMargin)>=ImageWidth || FBtmHeight> BtmLimitLine) return false;
			if((FLBBPoint.x+SideMargin)+(BtmWidth -(2*SideMargin)) >=ImageWidth ||(BTMLine-FBtmHeight)+FBtmHeight> BtmLimitLine                              ) return false;

			//if(BtmLimitLine!=0){
			//	if(((BtmBMLine[0].FP0.y+BtmBMLine[0].FP1.y)/2+BtmOffset)>BtmLimitLine) return false;
			//}

			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth      , FMaxWidth   , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight     , FMaxHeight  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_BottomLimit      ,FBtmHeight-1   , FBtmHeight+1, EDoubleThresholdMode_Outside);


			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_LeftLimit ,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(CCount>=MAX_DEFECT_POINT) break; 
					CDefectPoint[CCount].Width  =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					CDefectPoint[CCount].Height =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					CDefectPoint[CCount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CDefectPoint[CCount].Width/2 ) +FLBBPoint.x+SideMargin);
					// 						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +((FLBBPoint.y+FRBBPoint.y)/2-FBtmHeight-1));
					CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BTMLine-FBtmHeight));
					CDefectPoint[CCount].Rect.right  =(long)(CDefectPoint[CCount].Rect.left+ CDefectPoint[CCount].Width );
					CDefectPoint[CCount].Rect.bottom =(long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=CDefectPoint[CCount].Rect.top+1 ; n<CDefectPoint[CCount].Rect.bottom-1; n++){
						for(int m=CDefectPoint[CCount].Rect.left+1; m<CDefectPoint[CCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
						}
					}
					CDefectPoint[CCount].Round  =  PixelCount;
					CDefectPoint[CCount].P.x    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CDefectPoint[CCount].Width/2 )+FLBBPoint.x+SideMargin);
					CDefectPoint[CCount].P.y    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2)+(BTMLine-FBtmHeight)  );

					if(ImgDegree!=0.0f){
						CDefectPoint[CCount].P = Math.OnRotate(CenterPoint,CDefectPoint[CCount].P, ImgDegree);
					}
					CDefectPoint[CCount].P.y    = (float)(CDefectPoint[CCount].P.y +ImageHeight);

					// Merge ���ؼ� ������ �ʿ��� 
					// 						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +((FLBBPoint.y+FRBBPoint.y)/2-FBtmHeight-1)+ImageHeight);
					CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BTMLine-FBtmHeight)+ImageHeight);
					CDefectPoint[CCount].Rect.bottom =(long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
					CCount++;
				}
			}
			BlobCount =0;
		}
		else if(IsRectBtm)  { //���簢�� ��� Defect �˻� ������ 
			// SIDE D 
			////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(SP.x+SideMargin),(int)(SP.y+BtmMargin), FLeftWidth, LeftHeight-(2*BtmMargin));
			if((SP.x+SideMargin) <=0          || (SP.y+BtmMargin)<0            || FLeftWidth<=0          || LeftHeight-(2*BtmMargin)<  0           ) return false;
			if((SP.x+SideMargin) >=ImageWidth || (SP.y+BtmMargin)>BtmLimitLine || FLeftWidth>=ImageWidth || LeftHeight-(2*BtmMargin)>  BtmLimitLine) return false;
			if((SP.x+SideMargin)+FLeftWidth >=ImageWidth || (SP.y+BtmMargin)+(LeftHeight-(2*BtmMargin))> BtmLimitLine                              ) return false;
			// Line Position
			LinePosition = SP.x+SideMargin;

			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			// 			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 3          , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 2          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(DCount>=MAX_DEFECT_POINT) break; 
					DDefectPoint[DCount].Width  =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					DDefectPoint[DCount].Height =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DDefectPoint[DCount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +LinePosition);
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(SP.y+BtmMargin));
					DDefectPoint[DCount].Rect.right  =(long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					PixelCount =0;
					PixelValue =0;

					// ���� Pixel Data ���� 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
						}
					}

					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +LinePosition   );
					DDefectPoint[DCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(SP.y+BtmMargin)+ImageHeight     );

					// Merge ���ؼ� ������ �ʿ��� 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(SP.y+BtmMargin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount=0;
			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			//if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2);
			//else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = FP0.x -  Line[waLeftWall].FP0.x;
			//else                                                    LDistance = FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2); 

			if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2);
			else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = FP0.x -  Line[waLeftWall].FP0.x;
			else                                                    LDistance = FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2); 
			////////////////////////////////////////////////////////////////////////////////////////////////

			// SIDE B
			////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(EP.x-FRightWidth-SideMargin),(int)(EP.y+BtmMargin), FRightWidth, RightHeight-(2*BtmMargin));
			if((EP.x-FRightWidth-SideMargin) <=0          || (EP.y+BtmMargin)<0            || FRightWidth<=0          || RightHeight-(2*BtmMargin)< 0           ) return false;
			if((EP.x-FRightWidth-SideMargin) >=ImageWidth || (EP.y+BtmMargin)>BtmLimitLine || FRightWidth>=ImageWidth || RightHeight-(2*BtmMargin)> BtmLimitLine) return false;
			if((EP.x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth || (EP.y+BtmMargin)+(RightHeight-(2*BtmMargin))>BtmLimitLine                              ) return false;

			// Line Position
			LinePosition = EP.x-FRightWidth;

			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth ,FMinWidth     , FMaxWidth    , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight,FMinHeight    , FMaxHeight   , EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit     ,FRightWidth-2             , FRightWidth+1            , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit     ,FRightWidth-1             , FRightWidth+1            , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(BCount>=MAX_DEFECT_POINT) break; 
					BDefectPoint[BCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					BDefectPoint[BCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					BDefectPoint[BCount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +(LinePosition));
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(EP.y+BtmMargin));
					BDefectPoint[BCount].Rect.right  =(long)(BDefectPoint[BCount].Rect.left+ BDefectPoint[BCount].Width );
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
						for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
						}
					}
					BDefectPoint[BCount].Round  = PixelCount;
					BDefectPoint[BCount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 )+(LinePosition));
					BDefectPoint[BCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)+(EP.y+BtmMargin)+ImageHeight   );

					// Merge ���ؼ� ������ �ʿ��� 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(EP.y+BtmMargin)+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}

			BlobCount =0;
			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			//if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x;
			//else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance =  Line[waRightWall].FP0.x - FP1.x;
			//else                                                      RDistance = (Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x;

			if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x;
			else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance =  Line[waRightWall].FP0.x - FP1.x;
			else                                                      RDistance = (Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x;
			////////////////////////////////////////////////////////////////////////////////////////////////

			//SIDE C
			////////////////////////////////////////////////////////////////////////////////////////////////
			// Btm�˻�� ����ų�� ������ ���ؼ� 
			if(fabs(FLBPoint.y - FRBPoint.y)>3) BTMLine = (FLBPoint.y > FRBPoint.y) ?(int)FRBPoint.y : (int)FLBPoint.y ;
			else                                BTMLine = (FLBPoint.y > FRBPoint.y) ?(int)FLBPoint.y : (int)FRBPoint.y ;

			if(IsMarkBtm){
				DefectROI.SetPlacement((int)FLBPoint.x+SideMargin, (int)(BTMLine-FBtmHeight), BtmWidth-(2*SideMargin), FBtmHeight);
				if((FLBPoint.x+SideMargin) <=0          ||(BTMLine-FBtmHeight)<0            || BtmWidth-(2*SideMargin)<=0          || FBtmHeight<0           ) return false;
				if((FLBPoint.x+SideMargin) >=ImageWidth ||(BTMLine-FBtmHeight)>BtmLimitLine || BtmWidth-(2*SideMargin)>=ImageWidth || FBtmHeight>BtmLimitLine) return false;
				if((FLBPoint.x+SideMargin)+(BtmWidth-(2*SideMargin))>=ImageWidth ||(BTMLine-FBtmHeight)+FBtmHeight>BtmLimitLine                              ) return false;
			}
			else         {
				DefectROI.SetPlacement((int)FLBPoint.x+SideMargin, (int)(BTMLine-FBtmHeight), BtmWidth-(2*SideMargin), FBtmHeight-BtmInspOffset);
				if((FLBPoint.x+SideMargin) <=0          ||(BTMLine-FBtmHeight)<0            || BtmWidth-(2*SideMargin)<=0          || FBtmHeight-BtmInspOffset<0           ) return false;
				if((FLBPoint.x+SideMargin) >=ImageWidth ||(BTMLine-FBtmHeight)>BtmLimitLine || BtmWidth-(2*SideMargin)>=ImageWidth || FBtmHeight-BtmInspOffset>BtmLimitLine) return false;
				if((FLBPoint.x+SideMargin)+(BtmWidth-(2*SideMargin))>=ImageWidth ||(BTMLine-FBtmHeight)+FBtmHeight-BtmInspOffset>BtmLimitLine                              ) return false;
			}

			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth      , FMaxWidth   , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight     , FMaxHeight  , EDoubleThresholdMode_Outside);
			if(IsMarkBtm) CodeImageSelection.RemoveUsingIntegerFeature(EFeature_BottomLimit      ,FBtmHeight-1                 , FBtmHeight+1              , EDoubleThresholdMode_Outside);
			else          CodeImageSelection.RemoveUsingIntegerFeature(EFeature_BottomLimit      ,FBtmHeight-BtmInspOffset-1   , FBtmHeight-BtmInspOffset+1, EDoubleThresholdMode_Outside);


			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_LeftLimit ,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(CCount>=MAX_DEFECT_POINT) break; 
					CDefectPoint[CCount].Width  =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					CDefectPoint[CCount].Height =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					CDefectPoint[CCount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CDefectPoint[CCount].Width/2 ) +FLBPoint.x+SideMargin);
					// 						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +((FLBBPoint.y+FRBBPoint.y)/2-FBtmHeight-1));
					CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BTMLine-FBtmHeight));
					CDefectPoint[CCount].Rect.right  =(long)(CDefectPoint[CCount].Rect.left+ CDefectPoint[CCount].Width );
					CDefectPoint[CCount].Rect.bottom =(long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=CDefectPoint[CCount].Rect.top+1 ; n<CDefectPoint[CCount].Rect.bottom-1; n++){
						for(int m=CDefectPoint[CCount].Rect.left+1; m<CDefectPoint[CCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
						}
					}
					CDefectPoint[CCount].Round  =  PixelCount;
					CDefectPoint[CCount].P.x    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CDefectPoint[CCount].Width/2 )+FLBPoint.x+SideMargin);
					CDefectPoint[CCount].P.y    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2)+(BTMLine-FBtmHeight) );

					if(ImgDegree!=0.0f){
						CDefectPoint[CCount].P = Math.OnRotate(CenterPoint,CDefectPoint[CCount].P, ImgDegree);
					}
					CDefectPoint[CCount].P.y    = (float)(CDefectPoint[CCount].P.y+ImageHeight);

					// Merge ���ؼ� ������ �ʿ��� 
					// 						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +((FLBBPoint.y+FRBBPoint.y)/2-FBtmHeight-1)+ImageHeight);
					CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BTMLine-FBtmHeight)+ImageHeight);
					CDefectPoint[CCount].Rect.bottom =(long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
					CCount++;
				}
			}
			BlobCount =0;
			////////////////////////////////////////////////////////////////////////////////////////////////
		}
		else if(IsCircleBtm){ //Circle Cutting�ϰ�� Defect �˻� ������ 
			if(FInspectMode==Insp_Polygon){ // ���� �˻翡���� Dx,Dy,R �̰˻� 
				if(C1CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
					C1CirDeftectCnt  = GetPolygonSize(C1CirInPoint, C1CirDefectPoint, C1CirInEdgeCount, 50, 5); // Dark Defect 

					if(!IsMarkBtm && C1CirInCrackCount>0 && BrokenThresHold!=0){
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
						C1CirCrackCnt  = GetCircleCrackSize(C1CircleData, C1CirCrackPoint, C1CirInCrackCount, 30,CirFitDiameter1,CircleFit1Center); // Bright Defect 
					}
				}

				if(C2CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
					C2CirDeftectCnt  = GetPolygonSize(C2CirInPoint, C2CirDefectPoint, C2CirInEdgeCount, 50, 5); // Dark Defect 

					if(!IsMarkBtm && C2CirInCrackCount>0 && BrokenThresHold!=0){
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
						C2CirCrackCnt  = GetCircleCrackSize(C2CircleData, C2CirCrackPoint, C2CirInCrackCount, 30,CirFitDiameter2,CircleFit2Center); // Bright Defect 
					}
				}
			}
			else { // MarkMode, TrackMode 
				if(C1CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
					MaxDis =0.0f        ;
					MinDis =10000.0f    ;
					MaxDv  =0.0f        ;
					MinDv  =10000.0f    ;
					StdMaxDis =0.0f     ;
					StdMinDis =10000.0f ;
					MinCirCount =0      ;
					// ���� ������ �ּҰ� ���ϱ�  
					MinCirCount = C1CirInEdgeCount;

					CircleCount=0;TotalCirDis=0.0f; AvgCirDis=0.0f;
					for(int k=0; k<MinCirCount; k++){
						if(FInspectMode==Insp_Mark){ // MarkMode 
							if(C1CirInPoint[k].x==0    || C1CirInPoint[k].y==0    || k>=MAX_CIRCLE_EDGEPOINT) continue;

							if(IsMarkBtm){ // RealC1CirCneter���� �������� EdgePoint���� Circle_Fitting ����� 
								// 							Dis    = Math.GetLengthFrPointToPoint(RealC1CirCenter,C1CirInPoint[k] ); // Euresys Circle Fitting
								Dis = Math.GetLengthFrPointToPoint(CircleFit1Center,C1CirInPoint[k]   ); // FAVION  Circle Fitting 
							}
							else {
								Dis    = Math.GetLengthFrPointToPoint(CircleFit1Center,C1CirInPoint[k]   );
							}

							if(Dis>MaxDis) MaxDis = Dis;
							if(Dis<MinDis) MinDis = Dis;

							if(k==0){ // BTM Defect �˻縦 ���� ��ǥ�� �� Dx,Dy ���� 
								// 						    C1CirDy = Math.GetLengthFrPointToPoint(RealC1CirCenter,C1CirInPoint[k]);
								C1CirDy = Math.GetLengthFrPointToPoint(CircleFit1Center,C1CirInPoint[k]);
							} 
							if(k==(C1CirInEdgeCount-1)){
								// 						    C1CirDx = Math.GetLengthFrPointToPoint(RealC1CirCenter,C1CirInPoint[k]);
								C1CirDx = Math.GetLengthFrPointToPoint(CircleFit1Center,C1CirInPoint[k]);
							}
						}
						//Data ���� 
						C1CircleData[CircleCount].Dis  = Dis              ; 
						C1CircleData[CircleCount].P.x  = C1CirInPoint[k].x;
						C1CircleData[CircleCount].P.y  = C1CirInPoint[k].y;
						CircleCount++   ;
						TotalCirDis+=Dis;
					}

					if(CircleCount!=0 || TotalCirDis!=0){ // Circle AvgRound Cal
						AvgCirDis = (TotalCirDis/CircleCount);
						C1CirRound = AvgCirDis;
					}

					for(int i=0; i<CircleCount; i++){
						// 					C1CircleData[i].Dv = fabs(AvgCirDis - C1CircleData[i].Dis); // Avg EdgePoint ���� ������ 
						C1CircleData[i].Dv = (CirFitDiameter1 - C1CircleData[i].Dis) >0 ?(CirFitDiameter1 - C1CircleData[i].Dis):0;
						if(C1CircleData[i].Dv>MaxDv) MaxDv = C1CircleData[i].Dv;
						if(C1CircleData[i].Dv<MinDv) MinDv = C1CircleData[i].Dv;
					}
					C1CirDeftectCnt  = GetCircleSize(C1CircleData, C1CirDefectPoint, CircleCount, 5); // Dark Defect 

					// �ڸ��ҷ� �˻� ������ 
					if(!IsMarkBtm && C1CirInCrackCount>0 && BrokenThresHold!=0){
						for(int i=0; i<C1CirInCrackCount; i++){
							C1CircleData[i].Dv  =0.0f;
							C1CircleData[i].Dis =0.0f;
							C1CircleData[i].P.x  = C1CirInCrackPoint[i].x;
							C1CircleData[i].P.y  = C1CirInCrackPoint[i].y;
						}
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
						C1CirCrackCnt  = GetCircleCrackSize(C1CircleData, C1CirCrackPoint, C1CirInCrackCount, 30, CrackCirDiameter1,CircleFit1Center); // Bright Defect 
					}
				}

				// C2Circle Calculate 
				if(C2CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
					MaxDis =0.0f     ;
					MinDis =10000.0f ;
					MaxDv  =0.0f     ;
					MinDv  =10000.0f ;
					StdMaxDis =0.0f     ;
					StdMinDis =10000.0f ;
					MinCirCount =0      ;
					// ���� ������ �ּҰ� ���ϱ�  
					MinCirCount =  C2CirInEdgeCount;

					CircleCount=0;TotalCirDis=0.0f; AvgCirDis=0.0f;
					for(int k=0; k<MinCirCount; k++){
						if(FInspectMode==Insp_Mark){ // MarkMode 
							if(C2CirInPoint[k].x==0    || C2CirInPoint[k].y==0    || k>=MAX_CIRCLE_EDGEPOINT) continue;

							if(IsMarkBtm){ // RealC2CirCneter���� �������� EdgePoint���� Circle_Fitting ����� 
								// 							Dis    = Math.GetLengthFrPointToPoint(RealC2CirCenter,C2CirInPoint[k]   );
								Dis = Math.GetLengthFrPointToPoint(CircleFit2Center,C2CirInPoint[k]     );
								// 							Dis    = Math.GetLengthFrPointToPoint(C2CirCenter,C2CirInPoint[k]   );
							}
							else {         // RealC2CirCneter���� �������� EdgePoint���� Circle_Fitting ����� 
								Dis    = Math.GetLengthFrPointToPoint(CircleFit2Center,C2CirInPoint[k]   );
							}

							if(Dis>MaxDis) MaxDis = Dis;
							if(Dis<MinDis) MinDis = Dis;

							if(k==0){ // BTM Defect �˻縦 ���� ��ǥ�� �� Dx,Dy ���� 
								// 						    C2CirDx = Math.GetLengthFrPointToPoint(RealC2CirCenter,C2CirInPoint[k]);
								C2CirDx = Math.GetLengthFrPointToPoint(CircleFit2Center,C2CirInPoint[k]);
							} 
							if(k==(C2CirInEdgeCount-1)) {
								// 							C2CirDy = Math.GetLengthFrPointToPoint(RealC2CirCenter,C2CirInPoint[k]);
								C2CirDy = Math.GetLengthFrPointToPoint(CircleFit2Center,C2CirInPoint[k]);
							}
						}

						C2CircleData[CircleCount].Dis  = Dis              ; // FAVION  Circle Fitting 
						C2CircleData[CircleCount].P.x  = C2CirInPoint[k].x;
						C2CircleData[CircleCount].P.y  = C2CirInPoint[k].y;
						CircleCount++   ;
						TotalCirDis+=Dis;
					}
					if(CircleCount!=0 || TotalCirDis!=0){ // Circle AvgRound Cal
						AvgCirDis = (TotalCirDis/CircleCount);
						C2CirRound = AvgCirDis;
					}

					for(int i=0; i<CircleCount; i++){
						// 					C2CircleData[i].Dv = fabs(AvgCirDis - C2CircleData[i].Dis);      // Euresys Circle Fitting 
						C2CircleData[i].Dv = (CirFitDiameter2 - C2CircleData[i].Dis) >0 ?(CirFitDiameter2 - C2CircleData[i].Dis):0;
						if(C2CircleData[i].Dv>MaxDv) MaxDv = C2CircleData[i].Dv;
						if(C2CircleData[i].Dv<MinDv) MinDv = C2CircleData[i].Dv;
					}
					C2CirDeftectCnt  = GetCircleSize(C2CircleData, C2CirDefectPoint, CircleCount, 5);

					if(!IsMarkBtm && C2CirInCrackCount>0 && BrokenThresHold!=0){
						for(int i=0; i<C2CirInCrackCount; i++){
							C2CircleData[i].Dv  =0.0f;
							C2CircleData[i].Dis =0.0f;
							C2CircleData[i].P.x  = C2CirInCrackPoint[i].x;
							C2CircleData[i].P.y  = C2CirInCrackPoint[i].y;
						}
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
						C2CirCrackCnt  = GetCircleCrackSize(C2CircleData, C2CirCrackPoint, C2CirInCrackCount, 30, CrackCirDiameter2,CircleFit2Center);
					}
				}
			}


			// Side D 
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
			DefectROI.SetPlacement((int)(SP.x+SideMargin),(int)(SP.y+LeftDegMargin), FLeftWidth, LeftHeight-(2*LeftDegMargin));
			if((SP.x+SideMargin) <=0          || SP.y+LeftDegMargin<0            || FLeftWidth<=0          || LeftHeight-(2*LeftDegMargin)< 0          ) return false;
			if((SP.x+SideMargin) >=ImageWidth || SP.y+LeftDegMargin>BtmLimitLine || FLeftWidth>=ImageWidth || LeftHeight-(2*LeftDegMargin)>BtmLimitLine) return false;
			if((SP.x+SideMargin)+FLeftWidth >=ImageWidth || (SP.y+LeftDegMargin)+(LeftHeight-(2*LeftDegMargin))>BtmLimitLine                           ) return false;

			// Line Position 
			LinePosition = SP.x+SideMargin;
			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 3          , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 1          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(DCount>=MAX_DEFECT_POINT) break;
					DDefectPoint[DCount].Width  =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					DDefectPoint[DCount].Height =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DDefectPoint[DCount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +LinePosition );
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(SP.y+LeftDegMargin));
					DDefectPoint[DCount].Rect.right  =(long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					PixelCount =0;
					PixelValue =0;

					// ���� Pixel Data ���� 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
						}
					}

					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +LinePosition);
					DDefectPoint[DCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(SP.y+LeftDegMargin)+ImageHeight);
					DDefectPoint[DCount].Width  = (InspectMode)?DDefectPoint[DCount].Width: DDefectPoint[DCount].Width+SideMargin;

					// Merge ���ؼ� ������ �ʿ��� 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(SP.y+LeftDegMargin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount=0;
			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2);
			else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = FP0.x -  Line[waLeftWall].FP0.x;
			else                                                    LDistance = FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2); 
			////////////////////////////////////////////////////////////////////////////////////////////////


			// Side B
			////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(EP.x-FRightWidth-SideMargin),(int)(EP.y+RightDegMargin), FRightWidth,  RightHeight-(2*RightDegMargin));
			if((EP.x-FRightWidth-SideMargin) <=0          || (EP.y+RightDegMargin)<0            || FRightWidth<=0          || RightHeight-(2*RightDegMargin)<0           ) return false;
			if((EP.x-FRightWidth-SideMargin) >=ImageWidth || (EP.y+RightDegMargin)>BtmLimitLine || FRightWidth>=ImageWidth || RightHeight-(2*RightDegMargin)>BtmLimitLine) return false;
			if((EP.x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth || (EP.y+RightDegMargin)+(RightHeight-(2*RightDegMargin))>BtmLimitLine                             ) return false;

			// Line Position 
			LinePosition = EP.x - FRightWidth- SideMargin;
			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth               , FMaxWidth                , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight              , FMaxHeight               , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit       ,FRightWidth -1          , FRightWidth+1            , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(BCount>=MAX_DEFECT_POINT) break;
					BDefectPoint[BCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ; 
					BDefectPoint[BCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					BDefectPoint[BCount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +(LinePosition)  );
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(EP.y+RightDegMargin));
					BDefectPoint[BCount].Rect.right  =(long)(BDefectPoint[BCount].Rect.left+ BDefectPoint[BCount].Width );
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
						for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
						}
					}
					BDefectPoint[BCount].Round  = PixelCount;
					BDefectPoint[BCount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 )+(LinePosition));
					BDefectPoint[BCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)+(EP.y+RightDegMargin)+ImageHeight   );
					BDefectPoint[BCount].Width  =(InspectMode)? BDefectPoint[BCount].Width: BDefectPoint[BCount].Width+SideMargin;

					// Merge ���ؼ� ������ �ʿ��� 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(EP.y+RightDegMargin)+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount =0;
			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x;
			else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance =  Line[waRightWall].FP0.x - FP1.x;
			else                                                      RDistance = (Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x;
			////////////////////////////////////////////////////////////////////////////////////////////////

			// Side C 
			// ���� ������Ʈ������ �⺻���� BTM���� 1���� �˻� �Ѵ� 
			///////////////////////////////////////////////////////////////////////////////////////////////
			// ù��° Btm������ �������� ���� �˻� 
			// Dark �ҷ� �˻� ������ 
			if(BtmInLine[0].FP0.x!=0 && BtmInLine[0].FP0.y!=0 && BtmInLine[0].FP1.x!=0 && BtmInLine[0].FP1.y!=0){
				DefectROI.SetPlacement((int)(BtmInLine[0].FP0.x+SideMargin), (int)(BtmInLine[0].FP0.y-FBtmHeight), (BtmInLine[0].FP1.x - BtmInLine[0].FP0.x)-(2*SideMargin), FBtmHeight);
				if((BtmInLine[0].FP0.x+SideMargin) <=0          ||(BtmInLine[0].FP0.y-FBtmHeight)< 0           || (BtmInLine[0].FP1.x - BtmInLine[0].FP0.x)-(2*SideMargin)<=0          || FBtmHeight< 0          ) return false;
				if((BtmInLine[0].FP0.x+SideMargin) >=ImageWidth ||(BtmInLine[0].FP0.y-FBtmHeight)>BtmLimitLine || (BtmInLine[0].FP1.x - BtmInLine[0].FP0.x)-(2*SideMargin)>=ImageWidth || FBtmHeight>BtmLimitLine) return false;
				if((BtmInLine[0].FP0.x+SideMargin)+((BtmInLine[0].FP1.x - BtmInLine[0].FP0.x)-(2*SideMargin)) >=ImageWidth ||(BtmInLine[0].FP0.y-FBtmHeight)+FBtmHeight>BtmLimitLine                             ) return false;
				//Blob �˻� ������ 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);
				// Blob ���͸� 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth      , FMaxWidth   , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight     , FMaxHeight  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_BottomLimit      ,FBtmHeight-1   , FBtmHeight+1, EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_LeftLimit ,ESortDirection_Ascending);

				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(CCount>=MAX_DEFECT_POINT) break; 
						CDefectPoint[CCount].Width  =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						CDefectPoint[CCount].Height =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						CDefectPoint[CCount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CDefectPoint[CCount].Width/2 ) +(BtmInLine[0].FP0.x+SideMargin));
						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BtmInLine[0].FP0.y-FBtmHeight));
						CDefectPoint[CCount].Rect.right  =(long)(CDefectPoint[CCount].Rect.left+ CDefectPoint[CCount].Width );
						CDefectPoint[CCount].Rect.bottom =(long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
						PixelCount = 0;
						PixelValue = 0;

						// ���� Pixel Data ���� 
						for(int n=CDefectPoint[CCount].Rect.top+1 ; n<CDefectPoint[CCount].Rect.bottom-1; n++){
							for(int m=CDefectPoint[CCount].Rect.left+1; m<CDefectPoint[CCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < EnCapBtmThresHold) PixelCount++;
							}
						}
						CDefectPoint[CCount].Round  =  PixelCount;
						CDefectPoint[CCount].P.x    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CDefectPoint[CCount].Width/2 ) +(BtmInLine[0].FP0.x+SideMargin));
						CDefectPoint[CCount].P.y    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BtmInLine[0].FP0.y-FBtmHeight));

						// Image Rotate ���� ��ǥ�� �ٽ� Rotate�Ѵ� _ Display ���� 
						if(ImgDegree!=0.0f){
							CDefectPoint[CCount].P = Math.OnRotate(CenterPoint, CDefectPoint[CCount].P, (ImgDegree));
						}
						// Image Rotate ���� ��ǥ�� ȸ�� ���Ŀ� ImageHeight���� ���Ͽ� ���� ��ǥ�� �����Ѵ� 
						CDefectPoint[CCount].P.y    =(float)(CDefectPoint[CCount].P.y+ImageHeight);

						// Merge ���ؼ� ������ �ʿ��� 
						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BtmInLine[0].FP0.y-FBtmHeight)+ImageHeight);
						CDefectPoint[CCount].Rect.bottom =(long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
						CCount++;
					}
				}
			}
			BlobCount =0;

			// ���� �� ���� �۾��� ���� ����� �߻��ϴ� �ҷ� ���� ���� 
			// ���� �ڸ��ҷ�, �� �ڸ��ҷ� ���� ������.....EnCap���������� �˻縦 ������ 
			// Bright Defect �˻� ���� 
			if(BrokenThresHold!=0 && !IsMarkBtm){
				// �̹��� ��ȯ 
				// 				EasyImage::Threshold(&RotateImg,&DefectImg, BrokenThresHold); 
				DefectROI.Attach(&RotateBrokenImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
				IAddress = RotateBrokenImg.GetImagePtr();

				// TEST ���� 
				FBtmHeight =70;

				if(BtmEdgePoint[0].x!=0 && BtmInLine[0].FP0.y!=0 && BtmEdgePoint[1].x!=0 && BtmInLine[0].FP1.y!=0){
					DefectROI.SetPlacement((int)(BtmEdgePoint[0].x)-50, (int)(BtmInLine[0].FP0.y-FBtmHeight), (BtmEdgePoint[1].x - BtmEdgePoint[0].x)+100, FBtmHeight);
					if((BtmEdgePoint[0].x-50) <=0          ||(BtmInLine[0].FP0.y-FBtmHeight)< 0           || ((BtmEdgePoint[1].x - BtmEdgePoint[0].x)+100)<=0          || FBtmHeight< 0          ) return false;
					if((BtmEdgePoint[0].x-50) >=ImageWidth ||(BtmInLine[0].FP0.y-FBtmHeight)>BtmLimitLine || ((BtmEdgePoint[1].x - BtmEdgePoint[0].x)+100)>=ImageWidth || FBtmHeight>BtmLimitLine) return false;
					if((BtmEdgePoint[0].x-50)+((BtmEdgePoint[1].x - BtmEdgePoint[0].x)) >=ImageWidth ||(BtmInLine[0].FP0.y-FBtmHeight)+FBtmHeight>BtmLimitLine                             ) return false;

					//Blob �˻� ������ 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);
					// Blob ���͸� 
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,3     , FMaxWidth   , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,3     , FMaxHeight  , EDoubleThresholdMode_Outside);
					// 					CodeImageSelection.RemoveUsingIntegerFeature(EFeature_BottomLimit      ,FBtmHeight-1   , FBtmHeight+1, EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_LeftLimit ,ESortDirection_Ascending);

					if(BlobCount!=0){
						for(int k=0; k<BlobCount; k++){
							if(CCrackCount>=MAX_DEFECT_POINT) break; 
							CCrackPoint[CCrackCount].Width  =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
							CCrackPoint[CCrackCount].Height =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

							CCrackPoint[CCrackCount].Rect.left   =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CCrackPoint[CCrackCount].Width/2 ) +(BtmEdgePoint[0].x-50));
							CCrackPoint[CCrackCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CCrackPoint[CCrackCount].Height/2) +(BtmInLine[0].FP0.y-FBtmHeight));
							CCrackPoint[CCrackCount].Rect.right  =(long)(CCrackPoint[CCrackCount].Rect.left+ CCrackPoint[CCrackCount].Width );
							CCrackPoint[CCrackCount].Rect.bottom =(long)(CCrackPoint[CCrackCount].Rect.top + CCrackPoint[CCrackCount].Height);
							CCrackPoint[CCrackCount].LineAttach  =(CodeImageSelection.GetElement(k).GetBottomLimit()>(FBtmHeight-5)) ? 1: 0;
							PixelCount = 0;
							PixelValue = 0;

							// ���� Pixel Data ���� 
							for(int n=CCrackPoint[CCrackCount].Rect.top+1 ; n<CCrackPoint[CCrackCount].Rect.bottom-1; n++){
								for(int m=CCrackPoint[CCrackCount].Rect.left+1; m<CCrackPoint[CCrackCount].Rect.right-1 ; m++){
									PixelValue = IElement(m,n);
									if(PixelValue > BrokenThresHold) PixelCount++;
								}
							}
							CCrackPoint[CCrackCount].Round  =  PixelCount;
							CCrackPoint[CCrackCount].P.x    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CCrackPoint[CCrackCount].Width/2 ) +(BtmEdgePoint[0].x-50));
							CCrackPoint[CCrackCount].P.y    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CCrackPoint[CCrackCount].Height/2) +(BtmInLine[0].FP0.y-FBtmHeight));

							// Image Rotate ���� ��ǥ�� �ٽ� Rotate�Ѵ� _ Display ���� 
							if(ImgDegree!=0.0f){
								CCrackPoint[CCrackCount].P = Math.OnRotate(CenterPoint, CCrackPoint[CCrackCount].P, (ImgDegree));
							}
							// Image Rotate ���� ��ǥ�� ȸ�� ���Ŀ� ImageHeight���� ���Ͽ� ���� ��ǥ�� �����Ѵ� 
							CCrackPoint[CCrackCount].P.y    =(float)(CCrackPoint[CCrackCount].P.y+ImageHeight);

							// Merge ���ؼ� ������ �ʿ��� 
							CCrackPoint[CCrackCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CCrackPoint[CCrackCount].Height/2) +(BtmInLine[0].FP0.y-FBtmHeight)+ImageHeight);
							CCrackPoint[CCrackCount].Rect.bottom =(long)(CCrackPoint[CCrackCount].Rect.top + CCrackPoint[CCrackCount].Height);
							CCrackCount++;
						}
					}
				}
				BlobCount =0;
				// Notch�� �������� Bright Defect �˻� ������ ����  
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		// ���� �̹��� ȸ���Ͽ� �˻� ���� ȭ�鿡 ���÷��� �������� ��ǥ���� �ٽ� ����Ѵ� 
		////////////////////////////////////////////////////////////////////////////////////////////////
		// Cell Size ���� ������ �Ѱ���(Image ȸ�� �������)
// 		if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f && FRBPoint.x!=0.0f && FRBPoint.y!=0.0f){
// 			LeftEndPos.x     =  FLBPoint.x            ;
// 			LeftEndPos.y     =  FLBPoint.y+ImageHeight;
// 
// 			CenterEndPos.x =  BtmOutLine[2].FP0.x + (BtmOutLine[2].FP0.x - BtmOutLine[2].FP1.x)/2; 
// 			CenterEndPos.y =  (BtmOutLine[2].FP0.y + BtmOutLine[2].FP1.y)/2+ImageHeight; 
// 		}
		// Notch Cell Height Calc (Image  ȸ������)
// 		if(BtmOutLine[2].FP0.x!=0.0f && BtmOutLine[2].FP0.y!=0.0f && BtmOutLine[2].FP1.x!=0.0f && BtmOutLine[2].FP1.y!=0.0f){
// 			NotchCenterPos.x =  BtmOutLine[2].FP0.x + (BtmOutLine[2].FP1.x - BtmOutLine[2].FP0.x)/2.0;
// 			NotchCenterPos.y = (BtmOutLine[2].FP0.y + BtmOutLine[2].FP1.y)/2.0;
// 			NotchCellSizeHeight = Math.GetLengthFrLineToPoint(FLBPoint, FRBPoint, NotchCenterPos);
// 		}
		// Notch Cell Width Calc(Image ȸ������)
		////////////////////////////////////////////////////////////////////////////////////////////////
// 		if(BtmEdgePoint[2].x!=0.0f && BtmEdgePoint[2].y!=0.0f && BtmEdgePoint[7].x!=0.0f && BtmEdgePoint[7].y!=0.0f){
// 			NotchCellSizeWidth = Math.GetLengthFrPointToPoint((float)BtmEdgePoint[2].x,(float)BtmEdgePoint[2].y, (float)BtmEdgePoint[7].x,(float)BtmEdgePoint[7].y);
// 		}
		// Center Cell Size Width Calc(Image ȸ���� �������)
// 		if(TotalCellSizeWidth!=0.0f && TotalCellSizeCount!=0) CenCellSizeWidth = TotalCellSizeWidth/TotalCellSizeCount;

		if(ImgDegree!=0.0f ){
			FLBPoint    = Math.OnRotate(CenterPoint, FLBPoint     , (ImgDegree));
			FRBPoint    = Math.OnRotate(CenterPoint, FRBPoint     , (ImgDegree));

			// BtmLine 
			Line[3].FP0 = Math.OnRotate(CenterPoint, Line[3].FP0  , (ImgDegree));
			Line[3].FP1 = Math.OnRotate(CenterPoint, Line[3].FP1  , (ImgDegree));

			// BtmOutLIne 	    
			BtmOutLine[0].FP0   = Math.OnRotate(CenterPoint, BtmOutLine[0].FP0  , (ImgDegree));
			BtmOutLine[0].FP1   = Math.OnRotate(CenterPoint, BtmOutLine[0].FP1  , (ImgDegree));

			// BtmInLIne 	     											    
			BtmInLine[0].FP0    = Math.OnRotate(CenterPoint , BtmInLine[0].FP0  , (ImgDegree));
			BtmInLine[0].FP1    = Math.OnRotate(CenterPoint , BtmInLine[0].FP1  , (ImgDegree));

			// BtmStdLIne 	     											    
			BtmStdLine[0].FP0   = Math.OnRotate(CenterPoint, BtmStdLine[0].FP0  , (ImgDegree));
			BtmStdLine[0].FP1   = Math.OnRotate(CenterPoint, BtmStdLine[0].FP1  , (ImgDegree));

			// LeftOutLIne 	    
			LeftOutLine[0].FP0  = Math.OnRotate(CenterPoint, LeftOutLine[0].FP0 , (ImgDegree));
			LeftOutLine[0].FP1  = Math.OnRotate(CenterPoint, LeftOutLine[0].FP1 , (ImgDegree));

			// LeftInLIne 	    
			LeftInLine[0].FP0   = Math.OnRotate(CenterPoint , LeftInLine[0].FP0 , (ImgDegree));
			LeftInLine[0].FP1   = Math.OnRotate(CenterPoint , LeftInLine[0].FP1 , (ImgDegree));

			// LeftStdLIne 	    
			LeftStdLine[0].FP0  = Math.OnRotate(CenterPoint, LeftStdLine[0].FP0 , (ImgDegree));
			LeftStdLine[0].FP1  = Math.OnRotate(CenterPoint, LeftStdLine[0].FP1 , (ImgDegree));

			// RightOutLIne 
			RightOutLine[0].FP0 = Math.OnRotate(CenterPoint, RightOutLine[0].FP0, (ImgDegree));
			RightOutLine[0].FP1 = Math.OnRotate(CenterPoint, RightOutLine[0].FP1, (ImgDegree));

			// RightOutLIne 
			RightInLine[0].FP0  = Math.OnRotate(CenterPoint, RightInLine[0].FP0 , (ImgDegree));
			RightInLine[0].FP1  = Math.OnRotate(CenterPoint, RightInLine[0].FP1 , (ImgDegree));

			// RightStdLIne 
			RightStdLine[0].FP0 = Math.OnRotate(CenterPoint, RightStdLine[0].FP0, (ImgDegree));
			RightStdLine[0].FP1 = Math.OnRotate(CenterPoint, RightStdLine[0].FP1, (ImgDegree));

			// Cell Size ���� ������ �Ѱ���(Image ȸ�� ����)
// 			if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f && FRBPoint.x!=0.0f && FRBPoint.y!=0.0f){
// 				LeftEndPos.x     =  FLBPoint.x            ;
// 				LeftEndPos.y     =  FLBPoint.y+ImageHeight;
// 				CenterEndPos.x =  BtmOutLine[2].FP0.x + (BtmOutLine[2].FP0.x - BtmOutLine[2].FP1.x)/2; 
// 				CenterEndPos.y =  (BtmOutLine[2].FP0.y + BtmOutLine[2].FP1.y)/2+ImageHeight; 
// 			}

			if(IsCornerBtm){
				FLBBPoint   = Math.OnRotate(CenterPoint, FLBBPoint    , (ImgDegree));
				FLBLPoint   = Math.OnRotate(CenterPoint, FLBLPoint    , (ImgDegree));
				FLBBInPoint = Math.OnRotate(CenterPoint, FLBBInPoint  , (ImgDegree));
				FLBLInPoint = Math.OnRotate(CenterPoint, FLBLInPoint  , (ImgDegree));
				FLBBMeaPoint= Math.OnRotate(CenterPoint, FLBBMeaPoint , (ImgDegree));
				FLBLMeaPoint= Math.OnRotate(CenterPoint, FLBLMeaPoint , (ImgDegree));

				FRBBPoint   = Math.OnRotate(CenterPoint, FRBBPoint    , (ImgDegree));
				FRBRPoint   = Math.OnRotate(CenterPoint, FRBRPoint    , (ImgDegree));
				FRBBInPoint = Math.OnRotate(CenterPoint, FRBBInPoint  , (ImgDegree));
				FRBRInPoint = Math.OnRotate(CenterPoint, FRBRInPoint  , (ImgDegree));
				FRBBMeaPoint= Math.OnRotate(CenterPoint, FRBBMeaPoint , (ImgDegree));
				FRBRMeaPoint= Math.OnRotate(CenterPoint, FRBRMeaPoint , (ImgDegree));

				// Corner Defect ��ǥ�� ���� �����Ͽ� �����ؾ��� 
				if(C3Count!=0){
					for(int k=0; k<C3Count; k++){
						// Fram���� Y��ǥ������ �����Ѵ� 
						C3DefectPoint[k].P.y = C3DefectPoint[k].P.y - ImageHeight;
						if(C3DefectPoint[k].P.y>0) {
							C3DefectPoint[k].P   = Math.OnRotate(CenterPoint,C3DefectPoint[k].P,(ImgDegree)); 
							C3DefectPoint[k].P.y = C3DefectPoint[k].P.y + ImageHeight;
						}
						else {
							C3DefectPoint[k].P.y = C3DefectPoint[k].P.y + ImageHeight;
						}
					}
				}

				if(C4Count!=0){
					for(int k=0; k<C4Count; k++){
						// Fram���� Y��ǥ������ �����Ѵ� 
						C4DefectPoint[k].P.y = C4DefectPoint[k].P.y - ImageHeight;
						if(C4DefectPoint[k].P.y>0) {
							C4DefectPoint[k].P   = Math.OnRotate(CenterPoint,C4DefectPoint[k].P,(ImgDegree)); 
							C4DefectPoint[k].P.y = C4DefectPoint[k].P.y + ImageHeight;
						}
						else {
							C4DefectPoint[k].P.y = C4DefectPoint[k].P.y + ImageHeight;
						}
					}
				}
			}
			else if(IsCircleBtm){
				// Circle Defect Data 
				// Dark Defect Data 
				////////////////////////////////////////////////////////////////////////////////////////////////
				if(C1CirDeftectCnt!=0){
					for(int k=0; k<C1CirDeftectCnt; k++){
						C1CirDefectPoint[k].P.y = C1CirDefectPoint[k].P.y - ImageHeight;
						if(C1CirDefectPoint[k].P.y>0) {
							C1CirDefectPoint[k].P   = Math.OnRotate(CenterPoint,C1CirDefectPoint[k].P,(ImgDegree)); 
							C1CirDefectPoint[k].P.y = C1CirDefectPoint[k].P.y + ImageHeight;
						}
						else {
							C1CirDefectPoint[k].P.y = C1CirDefectPoint[k].P.y + ImageHeight;
						}
					}
				}

				if(C2CirDeftectCnt!=0){
					for(int k=0; k<C2CirDeftectCnt; k++){
						C2CirDefectPoint[k].P.y = C2CirDefectPoint[k].P.y - ImageHeight;
						if(C2CirDefectPoint[k].P.y>0) {
							C2CirDefectPoint[k].P   = Math.OnRotate(CenterPoint,C2CirDefectPoint[k].P,(ImgDegree)); 
							C2CirDefectPoint[k].P.y = C2CirDefectPoint[k].P.y + ImageHeight;
						}
						else {
							C2CirDefectPoint[k].P.y = C2CirDefectPoint[k].P.y + ImageHeight;
						}
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////////

				// Circle Defect Data 
				// Bright Defect Data e
				////////////////////////////////////////////////////////////////////////////////////////////////
				if(C1CirCrackCnt!=0){
					for(int k=0; k<C1CirCrackCnt; k++){
						C1CirCrackPoint[k].P.y = C1CirCrackPoint[k].P.y - ImageHeight;
						if(C1CirCrackPoint[k].P.y>0) {
							C1CirCrackPoint[k].P   = Math.OnRotate(CenterPoint,C1CirCrackPoint[k].P,(ImgDegree)); 
							C1CirCrackPoint[k].P.y = C1CirCrackPoint[k].P.y + ImageHeight;
						}
						else {
							C1CirCrackPoint[k].P.y = C1CirCrackPoint[k].P.y + ImageHeight;
						}
					}
				}

				if(C2CirCrackCnt!=0){
					for(int k=0; k<C2CirCrackCnt; k++){
						C2CirCrackPoint[k].P.y = C2CirCrackPoint[k].P.y - ImageHeight;
						if(C2CirCrackPoint[k].P.y>0) {
							C2CirCrackPoint[k].P   = Math.OnRotate(CenterPoint,C2CirCrackPoint[k].P,(ImgDegree)); 
							C2CirCrackPoint[k].P.y = C2CirCrackPoint[k].P.y + ImageHeight;
						}
						else {
							C2CirCrackPoint[k].P.y = C2CirCrackPoint[k].P.y + ImageHeight;
						}
					}
				}
				/////////////////////////////////////////////////////////////////////////////////////////////////////////
				if(C1CirInEdgeCount!=0){
					for(int k=0; k<C1CirInEdgeCount; k++){
						if(C1CirInPoint[k].x==0.0f || C1CirInPoint[k].y==0.0f) continue;
						C1CirInPoint[k] = Math.OnRotate(CenterPoint, C1CirInPoint[k], ImgDegree);
					}
				}
				if(C1CirOutEdgeCount!=0){
					for(int k=0; k<C1CirOutEdgeCount; k++){
						if(C1CirOutPoint[k].x==0.0f || C1CirOutPoint[k].y==0.0f) continue;
						C1CirOutPoint[k] = Math.OnRotate(CenterPoint, C1CirOutPoint[k], ImgDegree);
					}
				}
				if(C1CirCenter     .x!=0.0f && C1CirCenter     .y!=0.0f) C1CirCenter        = Math.OnRotate(CenterPoint, C1CirCenter         , (ImgDegree)); // �ܰ� ������ ����  
				if(RealC1CirCenter .x!=0.0f && RealC1CirCenter .y!=0.0f) RealC1CirCenter    = Math.OnRotate(CenterPoint, RealC1CirCenter     , (ImgDegree)); // Euresys 
				if(CircleFit1Center.x!=0.0f && CircleFit1Center.y!=0.0f) CircleFit1Center   = Math.OnRotate(CenterPoint, CircleFit1Center    , (ImgDegree)); // FAVION

				// C2 Circle Edge Point Rotate 
				if(C2CirInEdgeCount!=0){
					for(int k=0; k<C2CirInEdgeCount; k++){
						if(C2CirInPoint[k].x==0.0f || C2CirInPoint[k].y==0.0f) continue;
						C2CirInPoint[k] = Math.OnRotate(CenterPoint, C2CirInPoint[k], ImgDegree);
					}
				}
				if(C2CirOutEdgeCount!=0){
					for(int k=0; k<C2CirOutEdgeCount; k++){
						if(C2CirOutPoint[k].x==0.0f || C2CirOutPoint[k].y==0.0f) continue;
						C2CirOutPoint[k] = Math.OnRotate(CenterPoint, C2CirOutPoint[k], ImgDegree);
					}
				}
				if(C2CirCenter     .x!=0.0f && C2CirCenter     .y!=0.0f) C2CirCenter        = Math.OnRotate(CenterPoint, C2CirCenter         , (ImgDegree)); // �ܰ� ������ ����  
				if(RealC2CirCenter .x!=0.0f && RealC2CirCenter .y!=0.0f) RealC2CirCenter    = Math.OnRotate(CenterPoint, RealC2CirCenter     , (ImgDegree));
				if(CircleFit2Center.x!=0.0f && CircleFit2Center.y!=0.0f) CircleFit2Center   = Math.OnRotate(CenterPoint, CircleFit2Center    , (ImgDegree));
			}

			// GrindMarkPoint Rotate (Display ����) 
			if(IsMarkBtm){
				// C1,C2 Grind Mark ���� ȸ��
				for(int k=0; k<2; k++){
					LtPos.x =0.0f, LtPos.y=0.0f,RbPos.x=0.0f,RbPos.y=0.0f;
					if(CGrindMark[k].Rect.left==0 || CGrindMark[k].Rect.top==0 || CGrindMark[k].Rect.right==0 || CGrindMark[k].Rect.bottom==0 ) continue;

					LtPos.x = (float)CGrindMark[k].Rect.left  ;
					LtPos.y = (float)CGrindMark[k].Rect.top   ;
					RbPos.x = (float)CGrindMark[k].Rect.right ;
					RbPos.y = (float)CGrindMark[k].Rect.bottom;
					LtPos = Math.OnRotate(CenterPoint, LtPos,ImgDegree);
					RbPos = Math.OnRotate(CenterPoint, RbPos,ImgDegree);

					CGrindMark[k].Rect.left   = (long)LtPos.x;
					CGrindMark[k].Rect.top    = (long)LtPos.y;
					CGrindMark[k].Rect.right  = (long)RbPos.x;
					CGrindMark[k].Rect.bottom = (long)RbPos.y;
					if(CGrindMark[k].Point.x!=0.0f && CGrindMark[k].Point.y!=0.0f) {
						CGrindMark[k].Point= Math.OnRotate(CenterPoint, CGrindMark[k].Point,ImgDegree);
					}
				}

				// Btm ���� ����,���� ���� ���� Display���� 
				for(int k=0; k<7; k++){
					LtPos.x =0.0f, LtPos.y=0.0f,RbPos.x=0.0f,RbPos.y=0.0f;
					if(BtmGrindMark[k].Rect.left==0 || BtmGrindMark[k].Rect.top==0 || BtmGrindMark[k].Rect.right==0 || BtmGrindMark[k].Rect.bottom==0 ) continue;

					LtPos.x = (float)BtmGrindMark[k].Rect.left  ;
					LtPos.y = (float)BtmGrindMark[k].Rect.top   ;
					RbPos.x = (float)BtmGrindMark[k].Rect.right ;
					RbPos.y = (float)BtmGrindMark[k].Rect.bottom;
					LtPos = Math.OnRotate(CenterPoint, LtPos,ImgDegree);
					RbPos = Math.OnRotate(CenterPoint, RbPos,ImgDegree);

					BtmGrindMark[k].Rect.left   = (long)LtPos.x;
					BtmGrindMark[k].Rect.top    = (long)LtPos.y;
					BtmGrindMark[k].Rect.right  = (long)RbPos.x;
					BtmGrindMark[k].Rect.bottom = (long)RbPos.y;
					if(BtmGrindMark[k].Point.x!=0.0f && BtmGrindMark[k].Point.y!=0.0f) {
						BtmGrindMark[k].Point= Math.OnRotate(CenterPoint, BtmGrindMark[k].Point,ImgDegree);
					}
				}

				// Notch ROI Rect Rotate (Display ����) 
				for(int k=0; k<2; k++){
					LtPos.x =0.0f, LtPos.y=0.0f,RbPos.x=0.0f,RbPos.y=0.0f;
					if(NotchPolyRect[k].left==0 || NotchPolyRect[k].top==0 || NotchPolyRect[k].right==0 || NotchPolyRect[k].bottom==0 ) continue;

					LtPos.x = (float)NotchPolyRect[k].left  ;
					LtPos.y = (float)NotchPolyRect[k].top   ;
					RbPos.x = (float)NotchPolyRect[k].right ;
					RbPos.y = (float)NotchPolyRect[k].bottom;
					LtPos = Math.OnRotate(CenterPoint, LtPos,ImgDegree);
					RbPos = Math.OnRotate(CenterPoint, RbPos,ImgDegree);

					NotchPolyRect[k].left   = (long)LtPos.x;
					NotchPolyRect[k].top    = (long)LtPos.y;
					NotchPolyRect[k].right  = (long)RbPos.x;
					NotchPolyRect[k].bottom = (long)RbPos.y;
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
		DefectPointCount = A1CirDeftectCnt+A2CirDeftectCnt+ACount+BCount+CCount+DCount+C1CirDeftectCnt+C2CirDeftectCnt+ABurrCount+BBurrCount+CBurrCount
			               +DBurrCount+ACrackCount+BCrackCount+CCrackCount+DCrackCount+A1CirCrackCnt+A2CirCrackCnt+C1CirCrackCnt+C2CirCrackCnt; // ��ü �ҷ����� ���

		// BTM ���� �˻� ���� �� Cell Size ���� 
		// �˻� ���� �� Cell Size ������ 
		if(LeftStaPos.x!=0.0f && LeftStaPos.y!=0.0f && LeftEndPos.x!=0.0f && LeftEndPos.y!=0.0f){
			LeftCellSizeHeight = Math.GetLengthFrPointToPoint(LeftStaPos, LeftEndPos);
		}
		if(CenterStaPos.x!=0.0f && CenterStaPos.y!=0.0f && CenterEndPos.x!=0.0f && CenterEndPos.y!=0.0f){
			CenCellSizeHeight  = Math.GetLengthFrPointToPoint(CenterStaPos, CenterEndPos);
		}

		// Dark Defect Blob Merge
		///////////////////////////////////////////////////////////////////////////////////////////////
		// Circle_1 �ҷ� Merge ��� 
		if(C1CirDeftectCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = C1CirDeftectCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(C1CirDefectPoint[k].Rect.left==0 && C1CirDefectPoint[k].Rect.top==0 && C1CirDefectPoint[k].Rect.right==0 && C1CirDefectPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(C1CirDefectPoint[l].Rect.left==0 && C1CirDefectPoint[l].Rect.top==0 && C1CirDefectPoint[k].Rect.right==0 && C1CirDefectPoint[k].Rect.bottom==0) continue;

					r1 = C1CirDefectPoint[k].Rect;
					r2 = C1CirDefectPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						C1CirDefectPoint[k].Round    += C1CirDefectPoint[l].Round;
						C1CirDefectPoint[k].Rect      = OnMerge(C1CirDefectPoint[k].Rect, C1CirDefectPoint[l].Rect);
						C1CirDefectPoint[k].Width     = C1CirDefectPoint[k].Rect.right  - C1CirDefectPoint[k].Rect.left;
						C1CirDefectPoint[k].Height    = C1CirDefectPoint[k].Rect.bottom - C1CirDefectPoint[k].Rect.top ;
						C1CirDefectPoint[k].Hor_Dis   = C1CirDefectPoint[k].Width ; 
						C1CirDefectPoint[k].Ver_Dis   = C1CirDefectPoint[k].Height;
						C1CirDefectPoint[k].P.x       = (float)(C1CirDefectPoint[k].Rect.left);
						C1CirDefectPoint[k].P.y       = (float)(C1CirDefectPoint[k].Rect.top ); 
						C1CirDefectPoint[l].Rect.left = C1CirDefectPoint[l].Rect.top = C1CirDefectPoint[l].Rect.right = C1CirDefectPoint[l].Rect.bottom =0;
						C1CirDeftectCnt--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(C1CirDefectPoint[j].Rect.left==0 && C1CirDefectPoint[j].Rect.top==0 && C1CirDefectPoint[j].Rect.right==0 && C1CirDefectPoint[j].Rect.bottom==0) continue;
				C1CirDefectPoint[sort] = C1CirDefectPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Circle_2 �ҷ� Merge ��� 
		if(C2CirDeftectCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = C2CirDeftectCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(C2CirDefectPoint[k].Rect.left==0 && C2CirDefectPoint[k].Rect.top==0 && C2CirDefectPoint[k].Rect.right==0 && C2CirDefectPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(C2CirDefectPoint[l].Rect.left==0 && C2CirDefectPoint[l].Rect.top==0 && C2CirDefectPoint[k].Rect.right==0 && C2CirDefectPoint[k].Rect.bottom==0) continue;

					r1 = C2CirDefectPoint[k].Rect;
					r2 = C2CirDefectPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						C2CirDefectPoint[k].Round    += C2CirDefectPoint[l].Round;
						C2CirDefectPoint[k].Rect      = OnMerge(C2CirDefectPoint[k].Rect, C2CirDefectPoint[l].Rect);
						C2CirDefectPoint[k].Width     = C2CirDefectPoint[k].Rect.right  - C2CirDefectPoint[k].Rect.left;
						C2CirDefectPoint[k].Height    = C2CirDefectPoint[k].Rect.bottom - C2CirDefectPoint[k].Rect.top ;
						C2CirDefectPoint[k].Hor_Dis   = C2CirDefectPoint[k].Width ; 
						C2CirDefectPoint[k].Ver_Dis   = C2CirDefectPoint[k].Height;
						C2CirDefectPoint[k].P.x       = (float)(C2CirDefectPoint[k].Rect.left);
						C2CirDefectPoint[k].P.y       = (float)(C2CirDefectPoint[k].Rect.top ); 
						C2CirDefectPoint[l].Rect.left = C2CirDefectPoint[l].Rect.top = C2CirDefectPoint[l].Rect.right = C2CirDefectPoint[l].Rect.bottom =0;
						C2CirDeftectCnt--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(C2CirDefectPoint[j].Rect.left==0 && C2CirDefectPoint[j].Rect.top==0 && C2CirDefectPoint[j].Rect.right==0 && C2CirDefectPoint[j].Rect.bottom==0) continue;
				C2CirDefectPoint[sort] = C2CirDefectPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Side C Merge (�ڸ��ҷ�)
		if(CCrackCount!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = CCrackCount;

			for(int k=0; k<tmpDDefectCount; k++){
				if(CCrackPoint[k].Rect.left==0 && CCrackPoint[k].Rect.top==0 && CCrackPoint[k].Rect.right==0 && CCrackPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(CCrackPoint[l].Rect.left==0 && CCrackPoint[l].Rect.top==0 && CCrackPoint[k].Rect.right==0 && CCrackPoint[k].Rect.bottom==0) continue;

					r1 = CCrackPoint[k].Rect;
					r2 = CCrackPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						CCrackPoint[k].Round    += CCrackPoint[l].Round;
						CCrackPoint[k].Rect      = OnMerge(CCrackPoint[k].Rect, CCrackPoint[l].Rect);
						CCrackPoint[k].Width     = CCrackPoint[k].Rect.right  - CCrackPoint[k].Rect.left;
						CCrackPoint[k].Height    = CCrackPoint[k].Rect.bottom - CCrackPoint[k].Rect.top ;
						CCrackPoint[k].P.x       = (float)(CCrackPoint[k].Rect.left);
						CCrackPoint[k].P.y       = (float)(CCrackPoint[k].Rect.top ); 
						CCrackPoint[l].Rect.left = CCrackPoint[l].Rect.top = CCrackPoint[l].Rect.right = CCrackPoint[l].Rect.bottom =0;
						CCrackCount--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(CCrackPoint[j].Rect.left==0 && CCrackPoint[j].Rect.top==0 && CCrackPoint[j].Rect.right==0 && CCrackPoint[j].Rect.bottom==0) continue;
				CCrackPoint[sort] = CCrackPoint[j];
				sort++;
			}
		}

		// Side D Merge (Crack & Chipping)
		if(DCount!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = DCount;

			for(int k=0; k<tmpDDefectCount; k++){
				if(DDefectPoint[k].Rect.left==0 && DDefectPoint[k].Rect.top==0 && DDefectPoint[k].Rect.right==0 && DDefectPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(DDefectPoint[l].Rect.left==0 && DDefectPoint[l].Rect.top==0 && DDefectPoint[k].Rect.right==0 && DDefectPoint[k].Rect.bottom==0) continue;

					r1 = DDefectPoint[k].Rect;
					r2 = DDefectPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						DDefectPoint[k].Round    += DDefectPoint[l].Round;
						DDefectPoint[k].Rect      = OnMerge(DDefectPoint[k].Rect, DDefectPoint[l].Rect);
						DDefectPoint[k].Width     = DDefectPoint[k].Rect.right  - DDefectPoint[k].Rect.left;
						DDefectPoint[k].Height    = DDefectPoint[k].Rect.bottom - DDefectPoint[k].Rect.top ;
						DDefectPoint[k].P.x       = (float)(DDefectPoint[k].Rect.left);
						DDefectPoint[k].P.y       = (float)(DDefectPoint[k].Rect.top ); 
						DDefectPoint[l].Rect.left = DDefectPoint[l].Rect.top = DDefectPoint[l].Rect.right = DDefectPoint[l].Rect.bottom =0;
						DCount--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(DDefectPoint[j].Rect.left==0 && DDefectPoint[j].Rect.top==0 && DDefectPoint[j].Rect.right==0 && DDefectPoint[j].Rect.bottom==0) continue;
				DDefectPoint[sort] = DDefectPoint[j];
				sort++;
			}
		}

		// Side D Merge(Burr & Remain)
		if(DBurrCount!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = DBurrCount;

			for(int k=0; k<tmpDDefectCount; k++){
				if(DBurrPoint[k].Rect.left==0 && DBurrPoint[k].Rect.top==0 && DBurrPoint[k].Rect.right==0 && DBurrPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(DBurrPoint[l].Rect.left==0 && DBurrPoint[l].Rect.top==0 && DBurrPoint[k].Rect.right==0 && DBurrPoint[k].Rect.bottom==0) continue;

					r1 = DBurrPoint[k].Rect;
					r2 = DBurrPoint[l].Rect; 

					register int HorDist   =abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   =abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						DBurrPoint[k].Round    += DBurrPoint[l].Round;
						DBurrPoint[k].Rect      = OnMerge(DBurrPoint[k].Rect, DBurrPoint[l].Rect);
						DBurrPoint[k].Width     = DBurrPoint[k].Rect.right  - DBurrPoint[k].Rect.left;
						DBurrPoint[k].Height    = DBurrPoint[k].Rect.bottom - DBurrPoint[k].Rect.top ;
						DBurrPoint[k].P.x       = (float)(DBurrPoint[k].Rect.left);
						DBurrPoint[k].P.y       = (float)(DBurrPoint[k].Rect.top ); 
						DBurrPoint[l].Rect.left = DBurrPoint[l].Rect.top = DBurrPoint[l].Rect.right = DBurrPoint[l].Rect.bottom =0;
						DBurrCount--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(DBurrPoint[j].Rect.left==0 && DBurrPoint[j].Rect.top==0 && DBurrPoint[j].Rect.right==0 && DBurrPoint[j].Rect.bottom==0) continue;
				DBurrPoint[sort] = DBurrPoint[j];
				sort++;
			}
		}


		// Side B Merge (Crack & Chipping)
		if(BCount!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpBDefectCount = BCount;

			for(int k=0; k<tmpBDefectCount; k++){
				if(BDefectPoint[k].Rect.left==0 && BDefectPoint[k].Rect.top==0 && BDefectPoint[k].Rect.right==0 && BDefectPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpBDefectCount-1)) break;
					if(BDefectPoint[l].Rect.left==0 && BDefectPoint[l].Rect.top==0 && BDefectPoint[k].Rect.right==0 && BDefectPoint[k].Rect.bottom==0) continue;

					r1 = BDefectPoint[k].Rect;
					r2 = BDefectPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >=r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						BDefectPoint[k].Round    += BDefectPoint[l].Round;
						BDefectPoint[k].Rect      = OnMerge(BDefectPoint[k].Rect, BDefectPoint[l].Rect);
						BDefectPoint[k].Width     = BDefectPoint[k].Rect.right  - BDefectPoint[k].Rect.left;
						BDefectPoint[k].Height    = BDefectPoint[k].Rect.bottom - BDefectPoint[k].Rect.top ;
						BDefectPoint[k].P.x       = (float)(BDefectPoint[k].Rect.left);
						BDefectPoint[k].P.y       = (float)(BDefectPoint[k].Rect.top ); 
						BDefectPoint[l].Rect.left = BDefectPoint[l].Rect.top = BDefectPoint[l].Rect.right = BDefectPoint[l].Rect.bottom =0;
						BCount--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpBDefectCount; j++){
				if(BDefectPoint[j].Rect.left==0 && BDefectPoint[j].Rect.top==0 && BDefectPoint[j].Rect.right==0 && BDefectPoint[j].Rect.bottom==0) continue;
				BDefectPoint[sort] = BDefectPoint[j];
				sort++;
			}
		}


		// Side B Merge (Burr & Remain)
		if(BBurrCount!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpBDefectCount = BBurrCount;

			for(int k=0; k<tmpBDefectCount; k++){
				if(BBurrPoint[k].Rect.left==0 && BBurrPoint[k].Rect.top==0 && BBurrPoint[k].Rect.right==0 && BBurrPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpBDefectCount-1)) break;
					if(BBurrPoint[l].Rect.left==0 && BBurrPoint[l].Rect.top==0 && BBurrPoint[k].Rect.right==0 && BBurrPoint[k].Rect.bottom==0) continue;

					r1 = BBurrPoint[k].Rect;
					r2 = BBurrPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >=r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						BBurrPoint[k].Round    += BBurrPoint[l].Round;
						BBurrPoint[k].Rect      = OnMerge(BBurrPoint[k].Rect, BBurrPoint[l].Rect);
						BBurrPoint[k].Width     = BBurrPoint[k].Rect.right  - BBurrPoint[k].Rect.left;
						BBurrPoint[k].Height    = BBurrPoint[k].Rect.bottom - BBurrPoint[k].Rect.top ;
						BBurrPoint[k].P.x       = (float)(BBurrPoint[k].Rect.left);
						BBurrPoint[k].P.y       = (float)(BBurrPoint[k].Rect.top ); 
						BBurrPoint[l].Rect.left = BBurrPoint[l].Rect.top = BBurrPoint[l].Rect.right = BBurrPoint[l].Rect.bottom =0;
						BBurrCount--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpBDefectCount; j++){
				if(BBurrPoint[j].Rect.left==0 && BBurrPoint[j].Rect.top==0 && BBurrPoint[j].Rect.right==0 && BBurrPoint[j].Rect.bottom==0) continue;
				BBurrPoint[sort] = BBurrPoint[j];
				sort++;
			}

		}

		if(!IsCornerTop){
			if(ACount!=0){ // Left 
				bool IsxMg, IsyMg;
				CRect r1, r2;
				int sort =0;
				int tmpBDefectCount = ACount;

				// SIDE A + SIDE D Merge 
				for(int k=0; k<tmpBDefectCount; k++){
					if(ADefectPoint[k].Rect.left==0 && ADefectPoint[k].Rect.top==0 && ADefectPoint[k].Rect.right==0 && ADefectPoint[k].Rect.bottom==0) continue;
					IsxMg = false; IsyMg = false;
					r1 = ADefectPoint[k].Rect;
					r2 = DDefectPoint[0].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);

					if(HorDist>xMerge && VerDist>yMerge) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if(IsxMg && IsyMg){
						DDefectPoint[0].Round     = (ADefectPoint[k].Round>DDefectPoint[0].Round) ? ADefectPoint[k].Round :DDefectPoint[0].Round; 
						DDefectPoint[0].Rect      = OnMerge(ADefectPoint[k].Rect, DDefectPoint[0].Rect);
						DDefectPoint[0].Width     = DDefectPoint[0].Rect.right  - DDefectPoint[0].Rect.left;
						DDefectPoint[0].Height    = DDefectPoint[0].Rect.bottom - DDefectPoint[0].Rect.top ;
						ADefectPoint[k].Rect.left = ADefectPoint[k].Rect.top = ADefectPoint[k].Rect.right = ADefectPoint[k].Rect.bottom =0;
						ACount--;
						k=-1;
						//break;
					}
				}

				for(int j=0; j<tmpBDefectCount; j++){
					if(ADefectPoint[j].Rect.left==0 && ADefectPoint[j].Rect.top==0 && ADefectPoint[j].Rect.right==0 && ADefectPoint[j].Rect.bottom==0) continue;
					ADefectPoint[sort] = ADefectPoint[j];
					sort++;
				}
			}

			if(ACount!=0){ // Right 
				bool IsxMg, IsyMg;
				CRect r1, r2;
				int sort =0;
				int tmpBDefectCount = ACount;

				// SIDE A + SIDE B Merge 
				for(int k=0; k<tmpBDefectCount; k++){
					if(ADefectPoint[k].Rect.left==0 && ADefectPoint[k].Rect.top==0 && ADefectPoint[k].Rect.right==0 && ADefectPoint[k].Rect.bottom==0) continue;
					IsxMg = false; IsyMg = false;
					r1 = ADefectPoint[k].Rect;
					r2 = BDefectPoint[0].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);

					if(HorDist>xMerge && VerDist>yMerge) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if(IsxMg && IsyMg){
						BDefectPoint[0].Round     = (ADefectPoint[k].Round>BDefectPoint[0].Round) ? ADefectPoint[k].Round :BDefectPoint[0].Round; 
						BDefectPoint[0].Rect      = OnMerge(ADefectPoint[k].Rect, BDefectPoint[0].Rect);
						BDefectPoint[0].Width     = BDefectPoint[0].Rect.right  - BDefectPoint[0].Rect.left;
						BDefectPoint[0].Height    = BDefectPoint[0].Rect.bottom - BDefectPoint[0].Rect.top ;
						ADefectPoint[k].Rect.left = ADefectPoint[k].Rect.top = ADefectPoint[k].Rect.right = ADefectPoint[k].Rect.bottom =0;
						ACount--;
						k=-1;
						//break;
					}
				}

				for(int j=0; j<tmpBDefectCount; j++){
					if(ADefectPoint[j].Rect.left==0 && ADefectPoint[j].Rect.top==0 && ADefectPoint[j].Rect.right==0 && ADefectPoint[j].Rect.bottom==0) continue;
					ADefectPoint[sort] = ADefectPoint[j];
					sort++;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////
		// Blob Merge End
		Index = 0;
		return true;
	}
	return false;
}
int CFM2xLineDefect::OnExecute(bool l, bool t, bool r, bool b, bool lt, bool rt, bool rb, bool lb)
{

	int x = IRoi->GetOrgX();
	int y = IRoi->GetOrgY();
	int w = IRoi->GetWidth();
	int h = IRoi->GetHeight();

	int szX = IRoi->GetWidth();
	int szY = IRoi->GetHeight();
	int szBuffer = szX > szY ? szX : szY;

	int nLRoiOffset =0;
	int nRRoiOffset =0;
	int CornerCount =0;

	int ROI_X       =0;
	int ROI_Y       =0;
	int ROI_H       =0;
	int ROI_W       =0;

	const int MAX_SIDE     = 8  ;
	//const int BMWidth      = 500;
	const int BMHeight     = 200;
	const int GRWidth      = 250;
	const int GROffset     = 20 ;
	const int ROIOffset    = 10 ;
	const int CornerMargin = 2  ;
	int BMWidth      = 0  ;

// 	if(FCompanyMode==COM_TIANMA) BMWidth =500;
// 	if(FCompanyMode==COM_KWON  ) BMWidth =300;

	// ���ۺκ� Broken�߻��� 
	int LineMargin =30;
	int LineRange  =80;

	// Euresys ROI 
	// Median Filtter ���� ���� ROI   
	EROIBW8   FilterROI                ;

	// Circle ������ �ʿ��� ������ 
	const int   TrackLineMargin = 5    ;
	const int   CirAngleMargin  = 10   ;
	const int   CirCenterMargin = 0    ;
	const float CirCrackMargin  = 30.0f;  
	const float CirOffsetMargin = 7.0f ;

	//Polygon Value Test 
	int   PolygonMarginCnt= 0 ;

	// Circle Notch�� Trench_Width ���� ������ 
	const int TrenchWidthMargin =2000;

	// ��Mark Find �˻�� �ʿ��� ROI���� ������ 
	int GrindRectWidth=0, GrindRectHeight=0;

	// Value Init 
	double AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;

	// FAVION Circle Fitting �ʿ��� Data 
	float SampleSize   = 0.0f; //0.5~0.8���Ϸ� �Է��Ұ�
	float DistDevRatio = 0.15; //0.1~0.25 ���Ϸ� �Է��Ұ�

	//Circle Count,Avg Round 
	int CircleCount      =0   ;
	int EdgeCount        =0   ;
	double Dis           =0.0f;
	float AvgCirDis      =0.0f;
	float TotalCirDis    =0.0f;
	double MaxDis        =0.0f;
	double MinDis        =100000.0f; 
	double GrindMeaDis   =0.0f;
	double GrindWidthDis =0.0f;

	//Polygon Algorithm ����� OutData,InData ���� �ִܰŸ� ���� �ʿ��� Count
	int GrindMarkOutCnt=0, GrindMarkInCnt =0; 

	// GOO 20150819
	//if (FDefectPoint != NULL) delete[] FDefectPoint;
	//FDefectPoint = new TFPoint[szBuffer];

	FLeftWall = l;
	FTopWall = t;
	FRightWall = r;
	FBottomWall = b;
	FLeftTop = lt;
	FRightTop = rt;
	FRightBottom = rb;
	FLeftBottom = lb;

	// TOP, BTM Cell_Size, Grind_Size �˻�� �ʿ� ����

	int ImgW = GImage->GetWidth();
	int ImgH = GImage->GetHeight();

	TFPoint LT(0          ,           0)  ;
	TFPoint RT((float)ImgW,           0)  ;
	TFPoint RB((float)ImgW, (float)ImgH)  ;
	TFPoint LB(0          , (float)ImgH)  ;

	TFPoint LSP(0.0f,0.0f), LEP(0.0f,0.0f);
	TFPoint RSP(0.0f,0.0f), REP(0.0f,0.0f);

	double GrindWd1 =0.0f, GrindWd2 =0.0f;
	double GrindMea1=0.0f, GrindMea2=0.0f;

	ImgDegree        =0.0f;

	// Data Init
	FLTPoint.x = 0.0f; //�������ΰ� ��ܶ����� ������ ��ǥ
	FLTPoint.y = 0.0f; //�������ΰ� ��ܶ����� ������ ��ǥ
	FRTPoint.x = 0.0f; //�������ΰ� ��ܶ����� ������ ��ǥ
	FRTPoint.y = 0.0f; //�������ΰ� ��ܶ����� ������ ��ǥ
	FRBPoint.x = 0.0f; //�������ΰ� �ϴܶ����� ������ ��ǥ
	FRBPoint.y = 0.0f; //�������ΰ� �ϴܶ����� ������ ��ǥ
	FLBPoint.x = 0.0f; //�������ΰ� �ϴܶ����� ������ ��ǥ
	FLBPoint.y = 0.0f; //�������ΰ� �ϴܶ����� ������ ��ǥ

	FLTLPoint.x = 0.0f; //�������ΰ�   ù��° �ڳ� ������ ��ǥ 
	FLTLPoint.y = 0.0f; //�������ΰ�   ù��° �ڳ� ������ ��ǥ 
	FLTTPoint.x = 0.0f; //���ʶ��ΰ�   ù��° �ڳ� ������ ��ǥ 
	FLTTPoint.y = 0.0f; //���ʶ��ΰ�   ù��° �ڳ� ������ ��ǥ 
	FRTRPoint.x = 0.0f; //�������ΰ�   �ι�° �ڳ� ������ ��ǥ 
	FRTRPoint.y = 0.0f; //�������ΰ�   �ι�° �ڳ� ������ ��ǥ 
	FRTTPoint.x = 0.0f; //���ʶ��ΰ�   �ι�° �ڳ� ������ ��ǥ 
	FRTTPoint.y = 0.0f; //���ʶ��ΰ�   �ι�° �ڳ� ������ ��ǥ 
	FRBRPoint.x = 0.0f; //�������ΰ�   ����° �ڳ� ������ ��ǥ 
	FRBRPoint.y = 0.0f; //�������ΰ�   ����° �ڳ� ������ ��ǥ 
	FRBBPoint.x = 0.0f; //�Ʒ��ʶ��ΰ� ����° �ڳ� ������ ��ǥ 
	FRBBPoint.y = 0.0f; //�Ʒ��ʶ��ΰ� ����° �ڳ� ������ ��ǥ 
	FLBLPoint.x = 0.0f; //�������ΰ�   �׹�° �ڳ� ������ ��ǥ 
	FLBLPoint.y = 0.0f; //�������ΰ�   �׹�° �ڳ� ������ ��ǥ 
	FLBBPoint.x = 0.0f; //�Ʒ��ʶ��ΰ� �׹�° �ڳ� ������ ��ǥ 
	FLBBPoint.y = 0.0f; //�Ʒ��ʶ��ΰ� �׹�° �ڳ� ������ ��ǥ 

	//NOTCH Inspection
	FLTLInPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLTLInPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLTTInPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLTTInPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FRTRInPoint.x = 0.0f; //�����ʶ��ΰ� �ڳ� �������� ���ΰ� ������ ��ǥ
	FRTRInPoint.y = 0.0f; //�����ʶ��ΰ� �ڳ� �������� ���ΰ� ������ ��ǥ
	FRTTInPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FRTTInPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLBLInPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLBLInPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLBBInPoint.x = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLBBInPoint.y = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FRBRInPoint.x = 0.0f; //�����ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FRBRInPoint.y = 0.0f; //�����ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FRBBInPoint.x = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FRBBInPoint.y = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ

	FLTLMeaPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLTLMeaPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLTTMeaPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLTTMeaPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FRTRMeaPoint.x = 0.0f; //�����ʶ��ΰ� �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FRTRMeaPoint.y = 0.0f; //�����ʶ��ΰ� �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FRTTMeaPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FRTTMeaPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ

	FLBLMeaPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLBLMeaPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLBBMeaPoint.x = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLBBMeaPoint.y = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FRBRMeaPoint.x = 0.0f; //�����ʶ��ΰ� �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FRBRMeaPoint.y = 0.0f; //�����ʶ��ΰ� �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FRBBMeaPoint.x = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FRBBMeaPoint.y = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ

	// Notch Cell Track ������ ��ǥ 
	FTRLBPoint.x = 0.0f; //����Ʈ�����ΰ� �ϴ�Ʈ�������� ������ ��ǥ
	FTRLBPoint.y = 0.0f; //����Ʈ�����ΰ� �ϴ�Ʈ�������� ������ ��ǥ
	FTRRBPoint.x = 0.0f; //����Ʈ�����ΰ� �ϴ�Ʈ�������� ������ ��ǥ
	FTRRBPoint.y = 0.0f; //����Ʈ�����ΰ� �ϴ�Ʈ�������� ������ ��ǥ

	// Mark���� TrackLine���� �Ÿ� �� 
	float MarkToTrackLineDis1 =0.0f;
	float MarkToTrackLineDis2 =0.0f;

	// �ܰ����ο��� TrackLine���� �Ÿ� �� 
	float LeftToTrackLineDis1  =0.0f;
	float LeftToTrackLineDis2  =0.0f;
	float RightToTrackLineDis1 =0.0f;
	float RightToTrackLineDis2 =0.0f;

	for(int i=0; i<MAX_SIDE; i++){
		Line[i].FP0.x = 0.0f;
		Line[i].FP0.y = 0.0f;
		Line[i].FP1.x = 0.0f;
		Line[i].FP1.y = 0.0f;
	}

	TFPoint OrgPoint;

	// BtmTrackLine ��ǥ
	TFPoint LeftBtmTrackPoint (0.0f, 0.0f);
	TFPoint RightBtmTrackPoint(0.0f, 0.0f);
	//int CornerWidth = 100;	//	
	//int CornerHeight =100;

	int Index = 10;

	bool IsMiddle = false;

	// ThresHold Image Copy 
	if(GImage->GetImagePtr()){
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
	}
	else {
		return false;
	}

	switch (Index) {
		// Find Line
		//----------------------------------------------------------------------
	case 10 :
		if (FLeftWall) { //SoulBrain ���� ==> �⺻ �ܰ����� ���� 
			EasyImage::Threshold(GImage,&BinaryImg, LeftEdgeThresHold);
			if(LeftEdgeThresHold==0) {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
			else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

			if(FShape==shRectC){
				SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()                                );
				SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()                                );
				SingleLine->IRoi->SetWidth (IRoi->GetWidth()                               );
				// ChamberCut VS Circle VS Rect��쿡 ���� Left���� Fitting���� ���̰� �޶��� 
				if     (IsCornerBtm) SingleLine->IRoi->SetHeight(FEndLine-LBCornerHeight   );
				else if(IsCircleBtm) SingleLine->IRoi->SetHeight(FEndLine-CSideCircleHeight);
				else                 SingleLine->IRoi->SetHeight(IRoi->GetHeight()         );
				SingleLine->SetIncrement(1);
			}
			else if(FShape==shRectH) {
				if(IsCircleTop){ // Circle �˻�� 
					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()  );
					SingleLine->IRoi->SetOrgY  (ASideCircleHeight+50);
					SingleLine->IRoi->SetWidth (IRoi->GetWidth() );
					SingleLine->IRoi->SetHeight(ImgH-(ASideCircleHeight+50));
					SingleLine->SetIncrement(1);
				}
				else {
					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()  );
					SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()  );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth() );
					SingleLine->IRoi->SetHeight(IRoi->GetHeight());
					SingleLine->SetIncrement(1);
				}
			}
			else {
				SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()  );
				SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()  );
				SingleLine->IRoi->SetWidth (IRoi->GetWidth() );
				SingleLine->IRoi->SetHeight(IRoi->GetHeight());

				ROI_X = IRoi->GetOrgX()  ;
				ROI_Y = IRoi->GetOrgY()  ;
				ROI_W = IRoi->GetWidth() ;
				ROI_H = IRoi->GetHeight();
				if(ROI_H<700)  SingleLine->SetIncrement(50 );
				else 	       SingleLine->SetIncrement(100);
			}
			SingleLine->SetOrientation(orWest);
			SingleLine->SetFindLineId(0);
            SingleLine->SetEdgeType(1); // White to Black 

// 			if(IsMarkBtm) SingleLine->SetEdgeType(1); // Black to White 
// 			else          SingleLine->SetEdgeType(1); // White to Black 

			SingleLine->OnExecute();
			Line[0].FP0 = SingleLine->GetResultP0();
			Line[0].FP1 = SingleLine->GetResultP1();

			// Btm,Top ������� ù��°
			LeftOutLine[0].FP0.x = Line[0].FP0.x;
			LeftOutLine[0].FP0.y = Line[0].FP0.y;
			LeftOutLine[0].FP1.x = Line[0].FP1.x;
			LeftOutLine[0].FP1.y = Line[0].FP1.y;

			// LeftOutLine....make OutLine Data 
			// ������ �����͸� �Ѱ��ش� 
			// �˻��� ���ض����� ã�� ���Ұ�� ���������� �Ѱ��༭ ���ض��� ������ 
			if(LeftOutLine[0].FP0.x!=0.0f && LeftOutLine[0].FP0.y!=0.0f && LeftOutLine[0].FP1.x!=0.0f && LeftOutLine[0].FP1.y!=0.0f){
				PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
				PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
			} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
			else if(LeftOutLine[0].FP0.x==0.0f || LeftOutLine[0].FP0.y==0.0f || LeftOutLine[0].FP1.x==0.0f || LeftOutLine[0].FP1.y==0.0f){
				if(PreLeftOutLine[0].FP0.x==0.0f && PreLeftOutLine[0].FP0.y==0.0f && PreLeftOutLine[0].FP1.x==0.0f && PreLeftOutLine[0].FP1.y==0.0f){// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
					// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
					ROI_H = IRoi->GetHeight();
					ROI_W = IRoi->GetOrgX()  ;
					LeftOutLine[0].FP0.x = ROI_W;    LeftOutLine[0].FP0.y = 0.0f        ;
					LeftOutLine[0].FP1.x = ROI_W;    LeftOutLine[0].FP1.y = (float)ROI_H;

					//Line �����͸� PreLine�� �Ѱ��ش� 
					PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
					PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
				}
				else { // �߰��κ� Broken�߻����� �����Ͱ� ���°�� 
					// PreLine �����͸� �Ѱ��� 
					LeftOutLine[0].FP0.x = PreLeftOutLine[0].FP0.x;  LeftOutLine[0].FP1.x = PreLeftOutLine[0].FP1.x;  
					LeftOutLine[0].FP0.y = PreLeftOutLine[0].FP0.y;  LeftOutLine[0].FP1.y = PreLeftOutLine[0].FP1.y;  
				}
			}

			// LeftGrindIn ���� ���� 
			if(LeftGrindThresHold!=0){
				EasyImage::Threshold(GImage,&BinaryImg, LeftGrindThresHold);
				SingleLine->GImage = &BinaryImg;
				LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;
				// ROI���� ������  ���  
				if(fabs(LeftOutLine[0].FP0.x - LeftOutLine[0].FP1.x) > 2){
					LSP.x = (LeftOutLine[0].FP0.x + LeftOutLine[0].FP1.x)/2;
					LSP.y =  LeftOutLine[0].FP0.y;
					LEP.x = (LeftOutLine[0].FP0.x + LeftOutLine[0].FP1.x)/2;
					LEP.y =  LeftOutLine[0].FP1.y;
				}
				else {
					LSP.x =  LeftOutLine[0].FP0.x;
					LSP.y =  LeftOutLine[0].FP0.y;
					LEP.x =  LeftOutLine[0].FP1.x;
					LEP.y =  LeftOutLine[0].FP1.y;
				}
				// ���� ���� 
				if(FShape==shRectS){// ��Ƽ���� ���� 
					SingleLine->IRoi->SetOrgX  ((int)LSP.x+3);
					SingleLine->IRoi->SetOrgY  (0           );
					SingleLine->IRoi->SetWidth (100         );
					SingleLine->IRoi->SetHeight(ImgH        );
					if((LSP.x+3)<=0 ||(LSP.x+3)>=ImgW) break;
				}
				else               {// ����,�� �������� 
					SingleLine->IRoi->SetOrgX  ((int)LSP.x+3        );
					SingleLine->IRoi->SetOrgY  ((int)LSP.y          );
					SingleLine->IRoi->SetWidth (100                 );
					SingleLine->IRoi->SetHeight((int)(LEP.y - LSP.y));
					if((LSP.x+3)<=0 || LSP.y<0 || (LEP.y - LSP.y)<=0 || (LSP.x+3)>=ImgW || LSP.y>ImgH || (LEP.y - LSP.y)>ImgH) break;
				}

				ROI_X = (int)LSP.x+3        ;
				ROI_Y = (int)LSP.y          ;
				ROI_W = 100                 ;
				ROI_H = (int)(LEP.y - LSP.y);

				SingleLine->SetFindLineId(0);
				SingleLine->SetOrientation(orWest);
				SingleLine->SetEdgeType(0); // Black to White 
				SingleLine->OnExecute();
				LeftInLine[0].FP0 = SingleLine->GetResultP0();
				LeftInLine[0].FP1 = SingleLine->GetResultP1();

				// ������ ��� 
				GrindWd1=0.0f,GrindWd2=0.0f;
				GrindWd1 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP0);
				GrindWd2 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP1);
				LeftGrindWidth = (GrindWd1+GrindWd2)/2.0;
			}

			// ������ ���� ���ض��� ���� ������ 
			if(LeftStdThresHold!=0 && LeftInLine[0].FP0.x!=0 && LeftInLine[0].FP0.y!=0 && LeftInLine[0].FP1.x!=0 && LeftInLine[0].FP1.y!=0){
				EasyImage::Threshold(GImage,&BinaryImg, LeftStdThresHold);
				SingleLine->GImage = &BinaryImg;
// 				BinaryImg.Save("D:\\ROTATE_IMG\\SoulBrain_MID.bmp");
				LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;

				// �������� ���  
				if(fabs(LeftInLine[0].FP0.x - LeftInLine[0].FP1.x) > 2){
					LSP.x = (LeftInLine[0].FP0.x + LeftInLine[0].FP1.x)/2;
					LSP.y =  LeftInLine[0].FP0.y;
					LEP.x = (LeftInLine[0].FP0.x + LeftInLine[0].FP1.x)/2;
					LEP.y =  LeftInLine[0].FP1.y;
				}
				else {
					LSP.x =  LeftInLine[0].FP0.x;
					LSP.y =  LeftInLine[0].FP0.y;
					LEP.x =  LeftInLine[0].FP1.x;
					LEP.y =  LeftInLine[0].FP1.y;
				}
				// ���� ���� 
				if(FShape==shRectS){// ��Ƽ���� ���� 
					SingleLine->IRoi->SetOrgX  ((int)LSP.x+10);
					SingleLine->IRoi->SetOrgY  (0            );
					SingleLine->IRoi->SetWidth (100          );
					SingleLine->IRoi->SetHeight(ImgH         );
					if((LSP.x+3)<=0 ||(LSP.x+3)>=ImgW) break;
				}
				else               {// ����,�� �������� 
					SingleLine->IRoi->SetOrgX  ((int)LSP.x+10       );
					SingleLine->IRoi->SetOrgY  ((int)LSP.y          );
					SingleLine->IRoi->SetWidth (100                 );
					SingleLine->IRoi->SetHeight((int)(LEP.y - LSP.y));
					if((LSP.x+3)<=0 || LSP.y<0 || (LEP.y - LSP.y)<=0 || (LSP.x+3)>=ImgW || LSP.y>ImgH || (LEP.y - LSP.y)>ImgH) break;
				}

				ROI_X = (int)LSP.x+10       ;
				ROI_X = (int)LSP.y          ;
				ROI_W = 100                 ;
				ROI_H = (int)(LEP.y - LSP.y);

				SingleLine->SetFindLineId(0);
				SingleLine->SetOrientation(orWest);
				SingleLine->SetEdgeType(1); // White To Black  
				SingleLine->OnExecute();
				LeftStdLine[0].FP0 = SingleLine->GetResultP0();
				LeftStdLine[0].FP1 = SingleLine->GetResultP1();

				//������ ��� 
				GrindMea1=0.0f,GrindMea2=0.0f;
				GrindMea1 = Math.GetLengthFrLineToPoint(LeftStdLine[0].FP0,LeftStdLine[0].FP1,LeftOutLine[0].FP0);
				GrindMea2 = Math.GetLengthFrLineToPoint(LeftStdLine[0].FP0,LeftStdLine[0].FP1,LeftOutLine[0].FP1);
				LeftGrindMea = (GrindMea1+GrindMea2)/2.0;
			}
		}
		if (FTopWall) { // SoulBrain ����  
			EasyImage::Threshold(GImage,&BinaryImg, TopEdgeThresHold);
			if(TopEdgeThresHold==0)  {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
			else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

			SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX());
			SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY());
			SingleLine->IRoi->SetWidth (IRoi->GetWidth());
			SingleLine->IRoi->SetHeight(IRoi->GetHeight());
			if(IRoi->GetOrgX()<=0 || IRoi->GetWidth()<=0 || IRoi->GetHeight()<=0){
				return false;
			}

			SingleLine->SetOrientation(orNorth);
			SingleLine->FIncrement =100;
			SingleLine->SetFindLineId(0);
			SingleLine->FThreshold =5;

			SingleLine->SetEdgeType(0);  // Black To White
			SingleLine->OnExecute();

			Line[1].FP0 = SingleLine->GetResultP0();
			Line[1].FP1 = SingleLine->GetResultP1();

			TopOutLine[0].FP0 = Line[1].FP0;
			TopOutLine[0].FP1 = Line[1].FP1;
		}
		if (FRightWall) { //SoulBrain ���� ==> �⺻ �ܰ����� ���� 
			EasyImage::Threshold(GImage,&BinaryImg, RightEdgeThresHold);
			if(LeftEdgeThresHold==0) {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
			else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

			if(FShape==shRectC){
				SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()                                );
				SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()                                );
				SingleLine->IRoi->SetWidth (IRoi->GetWidth()                               );
				// ChamberCut VS Circle VS Rect��쿡 ���� Left���� Fitting���� ���̰� �޶��� 
				if     (IsCornerBtm) SingleLine->IRoi->SetHeight(FEndLine-RBCornerHeight   );
				else if(IsCircleBtm) SingleLine->IRoi->SetHeight(FEndLine-CSideCircleHeight);
				else                 SingleLine->IRoi->SetHeight(IRoi->GetHeight()         );
				SingleLine->SetIncrement(1 );

				ROI_X = IRoi->GetOrgX()           ;
				ROI_Y = IRoi->GetOrgY()           ;
				ROI_W = IRoi->GetWidth()          ;
				ROI_H = FEndLine-CSideCircleHeight;
			}
			else if(FShape==shRectH){
				if(IsCircleTop){
					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()  );
					SingleLine->IRoi->SetOrgY  (ASideCircleHeight+50);
					SingleLine->IRoi->SetWidth (IRoi->GetWidth() );
					SingleLine->IRoi->SetHeight(ImgH-(ASideCircleHeight+50));
					SingleLine->SetIncrement(1 );
				}
				else {
					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()  );
					SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()  );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth() );
					SingleLine->IRoi->SetHeight(IRoi->GetHeight());
					SingleLine->SetIncrement(1 );
				}
				ROI_X = IRoi->GetOrgX()  ;
				ROI_Y = IRoi->GetOrgY()  ;
				ROI_W = IRoi->GetWidth() ;
				ROI_H = IRoi->GetHeight();
// 				BinaryImg.Save("D:\\ROTATE_IMG\\SoulBrain_TOP.bmp");
			}
			else {
				SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX());
				SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY());
				SingleLine->IRoi->SetWidth (IRoi->GetWidth());
				SingleLine->IRoi->SetHeight(IRoi->GetHeight());
				ROI_H = IRoi->GetHeight();
				if(ROI_H<700)   SingleLine->SetIncrement(50 );
				else 	        SingleLine->SetIncrement(100);
			}

			SingleLine->SetOrientation(orEast);
			SingleLine->SetFindLineId(0);
			SingleLine->FThreshold =10;
			SingleLine->SetEdgeType(1); // White to Black 

// 			if(IsMarkBtm) SingleLine->SetEdgeType(0); // Black to White 
// 			else          SingleLine->SetEdgeType(1); // White to Black  

			SingleLine->OnExecute();
			Line[2].FP0 = SingleLine->GetResultP0();
			Line[2].FP1 = SingleLine->GetResultP1();

			// Btm,Top ������� ù��°
			RightOutLine[0].FP0.x = Line[2].FP0.x;
			RightOutLine[0].FP0.y = Line[2].FP0.y;
			RightOutLine[0].FP1.x = Line[2].FP1.x;
			RightOutLine[0].FP1.y = Line[2].FP1.y;

			// LeftOutLine....make OutLine Data 
			// ������ �����͸� �Ѱ��ش� 
			// �˻��� ���ض����� ã�� ���Ұ�� ���������� �Ѱ��༭ ���ض��� ������ 
			if(RightOutLine[0].FP0.x!=0.0f && RightOutLine[0].FP0.y!=0.0f && RightOutLine[0].FP1.x!=0.0f && RightOutLine[0].FP1.y!=0.0f){
				PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
				PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
			} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
			else if(RightOutLine[0].FP0.x==0.0f || RightOutLine[0].FP0.y==0.0f || RightOutLine[0].FP1.x==0.0f || RightOutLine[0].FP1.y==0.0f){
				if(PreRightOutLine[0].FP0.x==0.0f && PreRightOutLine[0].FP0.y==0.0f && PreRightOutLine[0].FP1.x==0.0f && PreRightOutLine[0].FP1.y==0.0f){// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
					// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
					ROI_H = IRoi->GetHeight();
					RightOutLine[0].FP0.x = Line[2].FP0.x;    RightOutLine[0].FP0.y = 0.0f        ;
					RightOutLine[0].FP1.x = Line[2].FP1.x;    RightOutLine[0].FP1.y = (float)ROI_H;

					//Line �����͸� PreLine�� �Ѱ��ش� 
					PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
					PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
				}
				else { // �߰��κ� Broken�߻����� �����Ͱ� ���°�� 
					// PreLine �����͸� �Ѱ��� 
					RightOutLine[0].FP0.x = PreRightOutLine[0].FP0.x;  RightOutLine[0].FP1.x = PreRightOutLine[0].FP1.x;  
					RightOutLine[0].FP0.y = PreRightOutLine[0].FP0.y;  RightOutLine[0].FP1.y = PreRightOutLine[0].FP1.y;  
				}
			}

			// RightGrindIn���� ���� 
			if(RightGrindThresHold!=0){
				EasyImage::Threshold(GImage,&BinaryImg, RightGrindThresHold);
				SingleLine->GImage = &BinaryImg;

				RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;
				// ROI���� ������  ���  
				if(fabs(RightOutLine[0].FP0.x - RightOutLine[0].FP1.x) > 2){
					RSP.x = (RightOutLine[0].FP0.x + RightOutLine[0].FP1.x)/2;
					RSP.y =  RightOutLine[0].FP0.y;
					REP.x = (RightOutLine[0].FP0.x + RightOutLine[0].FP1.x)/2;
					REP.y =  RightOutLine[0].FP1.y;
				}
				else {
					RSP.x =  RightOutLine[0].FP0.x;
					RSP.y =  RightOutLine[0].FP0.y;
					REP.x =  RightOutLine[0].FP1.x;
					REP.y =  RightOutLine[0].FP1.y;
				}
				// ���� ���� 
				if(FShape==shRectS){// ��Ƽ���� ���� 
					SingleLine->IRoi->SetOrgX  ((int)RSP.x-100 );
					SingleLine->IRoi->SetOrgY  (0              );
					SingleLine->IRoi->SetWidth (100            );
					SingleLine->IRoi->SetHeight(ImgH           );
					if((RSP.x-100)<=0 ||(RSP.x-100)>=ImgW) break;
				}
				else               {// ����,�� �������� 
					SingleLine->IRoi->SetOrgX  ((int)RSP.x-100      );
					SingleLine->IRoi->SetOrgY  ((int)RSP.y          );
					SingleLine->IRoi->SetWidth (100                 );
					SingleLine->IRoi->SetHeight((int)(REP.y - RSP.y));
					if((RSP.x-100)<=0 || RSP.y<0 || (REP.y - RSP.y)<=0 || (RSP.x-100)>=ImgW || RSP.y>ImgH || (REP.y - RSP.y)>ImgH) break;
				}

				SingleLine->SetFindLineId(0);
				SingleLine->SetOrientation(orEast);
				SingleLine->SetEdgeType(0); // Black to White 
				SingleLine->OnExecute();
				RightInLine[0].FP0 = SingleLine->GetResultP0();
				RightInLine[0].FP1 = SingleLine->GetResultP1();

				// ������ ��� 
				GrindWd1=0.0f,GrindWd2=0.0f;
				GrindWd1 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP0);
				GrindWd2 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP1);
				RightGrindWidth = (GrindWd1+GrindWd2)/2.0;
			}

			// ������ ���� ���ض��� ���� ������ 
			if(RightStdThresHold!=0 && RightInLine[0].FP0.x!=0 && RightInLine[0].FP0.y!=0 && RightInLine[0].FP1.x!=0 && RightInLine[0].FP1.y!=0){
				EasyImage::Threshold(GImage,&BinaryImg, RightStdThresHold);
				SingleLine->GImage = &BinaryImg;
				RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;

				// �������� ���  
				if(fabs(RightInLine[0].FP0.x - RightInLine[0].FP1.x) > 2){
					RSP.x = (RightInLine[0].FP0.x + RightInLine[0].FP1.x)/2;
					RSP.y =  RightInLine[0].FP0.y;
					REP.x = (RightInLine[0].FP0.x + RightInLine[0].FP1.x)/2;
					REP.y =  RightInLine[0].FP1.y;
				}
				else {
					RSP.x =  RightInLine[0].FP0.x;
					RSP.y =  RightInLine[0].FP0.y;
					REP.x =  RightInLine[0].FP1.x;
					REP.y =  RightInLine[0].FP1.y;
				}
				// ���� ���� 
				if(FShape==shRectS){// ��Ƽ���� ���� 
					SingleLine->IRoi->SetOrgX  ((int)RSP.x-100 );
					SingleLine->IRoi->SetOrgY  (0              );
					SingleLine->IRoi->SetWidth (80             );
					SingleLine->IRoi->SetHeight(ImgH           );
					if((RSP.x-100)<=0 ||(RSP.x-100)>=ImgW) break;
				}
				else               {// ����,�� �������� 
					SingleLine->IRoi->SetOrgX  ((int)RSP.x-100      );
					SingleLine->IRoi->SetOrgY  ((int)RSP.y          );
					SingleLine->IRoi->SetWidth (80                  );
					SingleLine->IRoi->SetHeight((int)(REP.y - RSP.y));
					if((RSP.x-100)<=0 || RSP.y<0 || (REP.y - RSP.y)<=0 || (RSP.x-100)>=ImgW || RSP.y>ImgH || (REP.y - RSP.y)>ImgH) break;
				}

				SingleLine->SetFindLineId(0);
				SingleLine->SetOrientation(orEast);
				SingleLine->SetEdgeType(1); // White To Black  
				SingleLine->OnExecute();
				RightStdLine[0].FP0 = SingleLine->GetResultP0();
				RightStdLine[0].FP1 = SingleLine->GetResultP1();

				//������ ��� 
				GrindMea1=0.0f,GrindMea2=0.0f;
				GrindMea1 = Math.GetLengthFrLineToPoint(RightStdLine[0].FP0,RightStdLine[0].FP1,RightOutLine[0].FP0);
				GrindMea2 = Math.GetLengthFrLineToPoint(RightStdLine[0].FP0,RightStdLine[0].FP1,RightOutLine[0].FP1);
				RightGrindMea = (GrindMea1+GrindMea2)/2.0;
			}
		}
		if (FBottomWall) { //SoulBrain 
			EasyImage::Threshold(GImage,&BinaryImg, BtmEdgeThresHold);
			if(BtmEdgeThresHold==0)    {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
			else                       {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

			SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX());
			SingleLine->IRoi->SetOrgY  (FEndLine-100   );
			SingleLine->IRoi->SetWidth (IRoi->GetWidth());
			SingleLine->IRoi->SetHeight(200             );

			SingleLine->SetOrientation(orSouth);
			SingleLine->FIncrement =50;
			SingleLine->SetFindLineId(0);
			SingleLine->FThreshold =5;
            SingleLine->SetEdgeType(1);  // White To Black 

			// �⺻ BtmLine Search 
			SingleLine->OnExecute();
			Line[3].FP0 = SingleLine->GetResultP0();
			Line[3].FP1 = SingleLine->GetResultP1();

			BtmOutLine[0].FP0 =  Line[3].FP0;
			BtmOutLine[0].FP1 =  Line[3].FP1;
		}
		if (FLeftTop    ){ //SoulBrain ==>�̹��� ȸ���� �ݺ��˻� �����ϹǷ� �ʿ���� 

		}
		if (FRightTop   ){ //SoulBrain ==>�̹��� ȸ���� �ݺ��˻� �����ϹǷ� �ʿ���� 

		}
		if (FRightBottom){ //SoulBrain ==>�̹��� ȸ���� �ݺ��˻� �����ϹǷ� �ʿ���� 

		}
		if (FLeftBottom ){ //SoulBrain ==>�̹��� ȸ���� �ݺ��˻� �����ϹǷ� �ʿ���� 

		}

		// SoulBrain �ܰ��˻�� �� ����� �Ҿ����Ͽ� ���� Ʋ��������� 
		// �˻� ROI������ Ʋ���� ���� �������� ȸ���Ͽ��� �ܰ����� ��ó�� �����ϴ� �������� ���ؼ� ����ų �߻��� 
		// �̷� ���ؼ� �ε����ϰ� ���� �̹����� �����Ͽ� Ʋ���� ������ŭ ȸ���Ͽ� �⺻ Edge�����͸� �ٽ� ���� �� Defect�˻�
		// ȸ���� �̹����������� �˻� ������ 
		if(FShape==shRectH ){ //SoulBrain ROTATE IMAGE
			// Image Roate �� �����̹��� ���� ������ ���� 
			FLTPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
			FRTPoint = Math.OnIntersect(Line[waRightWall].FP0, Line[waRightWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);

			// Align Mark �������� �̹��� ȸ��
			if(FP0.x!=0 && FP0.y!=0 && FP1.x!=0 && FP1.y!=0) ImgDegree = -(Math.GetDegree(FP0.x , FP0.y, FP1.x, FP1.y));

			// ���� RotateImg Size ���� �� ���� 
			LogUnit.SetLog(L"shRectH_RotateImg_Start");     
			RotateImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
			EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&RotateImg);

			// ���� �����Ͽ� �̹��� ȸ�� 
			EasyImage::ScaleRotate(GImage, (float)ImgW/2, (float)ImgH/2, (float)ImgW/2, (float)ImgH/2, 1.0f, 1.0f, ImgDegree, &RotateImg, 4);
			CenterPoint.x = ImgW/2;
			CenterPoint.y = ImgH/2;
// 			RotateImg.Save("D:\\ROTATE_IMG\\SoulBrain_TOP.bmp");

			// Binary RotateImg Size ���� �� ���� 
			RotateBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
			EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBinaryImg);
			// Grind Measure Corner, Circle �˻� ���� 
			RotateGrindBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
			EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateGrindBinaryImg);
			LogUnit.SetLog(L"shRectH_RotateImg_End");     

			if(FLeftWall){
				EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftEdgeThresHold); // �⺻ LeftEdgeLine
				if(LeftEdgeThresHold==0) {SingleLine->GImage = &RotateImg      ; SingleLine->FThreshold =5 ;}
				else                     {SingleLine->GImage = &RotateBinaryImg; SingleLine->FThreshold =10;}

				if(IsCircleTop){
					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()  );
					SingleLine->IRoi->SetOrgY  (ASideCircleHeight+50);
					SingleLine->IRoi->SetWidth (IRoi->GetWidth() );
					SingleLine->IRoi->SetHeight(ImgH-(ASideCircleHeight+50));
					SingleLine->SetIncrement(1 );
				}
				else {
					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()  );
					SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()  );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth() );
					SingleLine->IRoi->SetHeight(IRoi->GetHeight());
					SingleLine->SetIncrement(5 );
				}
				SingleLine->SetOrientation(orWest);
				SingleLine->SetFindLineId(0);
				SingleLine->SetEdgeType(1); // White to Black 
				SingleLine->OnExecute();

				Line[0].FP0 = SingleLine->GetResultP0();
				Line[0].FP1 = SingleLine->GetResultP1();

				// Btm,Top ������� ù��°
				LeftOutLine[0].FP0.x = Line[0].FP0.x;
				LeftOutLine[0].FP0.y = Line[0].FP0.y;
				LeftOutLine[0].FP1.x = Line[0].FP1.x;
				LeftOutLine[0].FP1.y = Line[0].FP1.y;

				// LeftOutLine....make OutLine Data 
				// ������ �����͸� �Ѱ��ش� 
				// �˻��� ���ض����� ã�� ���Ұ�� ���������� �Ѱ��༭ ���ض��� ������ 
				if(LeftOutLine[0].FP0.x!=0.0f && LeftOutLine[0].FP0.y!=0.0f && LeftOutLine[0].FP1.x!=0.0f && LeftOutLine[0].FP1.y!=0.0f){
					PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
					PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
				} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
				else if(LeftOutLine[0].FP0.x==0.0f || LeftOutLine[0].FP0.y==0.0f || LeftOutLine[0].FP1.x==0.0f || LeftOutLine[0].FP1.y==0.0f){
					if(PreLeftOutLine[0].FP0.x==0.0f && PreLeftOutLine[0].FP0.y==0.0f && PreLeftOutLine[0].FP1.x==0.0f && PreLeftOutLine[0].FP1.y==0.0f){// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
						// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
						ROI_H = IRoi->GetHeight();
						LeftOutLine[0].FP0.x = Line[0].FP0.x;    LeftOutLine[0].FP0.y = 0.0f        ;
						LeftOutLine[0].FP1.x = Line[0].FP1.x;    LeftOutLine[0].FP1.y = (float)ROI_H;

						//Line �����͸� PreLine�� �Ѱ��ش� 
						PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
						PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
					}
					else { // �߰��κ� Broken�߻����� �����Ͱ� ���°�� 
						// PreLine �����͸� �Ѱ��� 
						LeftOutLine[0].FP0.x = PreLeftOutLine[0].FP0.x;  LeftOutLine[0].FP1.x = PreLeftOutLine[0].FP1.x;  
						LeftOutLine[0].FP0.y = PreLeftOutLine[0].FP0.y;  LeftOutLine[0].FP1.y = PreLeftOutLine[0].FP1.y;  
					}
				}

				// LeftGrindIn ���� ���� 
				if(LeftGrindThresHold!=0){
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftGrindThresHold);
					SingleLine->GImage = &RotateBinaryImg;
					LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;
					// ROI���� ������  ���  
					if(fabs(LeftOutLine[0].FP0.x - LeftOutLine[0].FP1.x) > 2){
						LSP.x = (LeftOutLine[0].FP0.x + LeftOutLine[0].FP1.x)/2;
						LSP.y =  LeftOutLine[0].FP0.y;
						LEP.x = (LeftOutLine[0].FP0.x + LeftOutLine[0].FP1.x)/2;
						LEP.y =  LeftOutLine[0].FP1.y;
					}
					else {
						LSP.x =  LeftOutLine[0].FP0.x;
						LSP.y =  LeftOutLine[0].FP0.y;
						LEP.x =  LeftOutLine[0].FP1.x;
						LEP.y =  LeftOutLine[0].FP1.y;
					}
					// ���� ���� 
					SingleLine->IRoi->SetOrgX  ((int)LSP.x+3        );
					SingleLine->IRoi->SetOrgY  ((int)LSP.y          );
					SingleLine->IRoi->SetWidth (100                 );
					SingleLine->IRoi->SetHeight((int)(LEP.y - LSP.y));
					if((LSP.x+3)<=0 || LSP.y<0 || (LEP.y - LSP.y)<=0 || (LSP.x+3)>=ImgW || LSP.y>ImgH || (LEP.y - LSP.y)>ImgH) break;

					SingleLine->SetFindLineId(0);
					SingleLine->SetOrientation(orWest);
					SingleLine->SetEdgeType(0); // Black to White 
					SingleLine->OnExecute();
					LeftInLine[0].FP0 = SingleLine->GetResultP0();
					LeftInLine[0].FP1 = SingleLine->GetResultP1();

					// ������ ��� 
					GrindWd1=0.0f,GrindWd2=0.0f;
					GrindWd1 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP0);
					GrindWd2 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP1);
					LeftGrindWidth = (GrindWd1+GrindWd2)/2.0;
				}

				// ������ ���� ���ض��� ���� ������ 
				if(LeftStdThresHold!=0 && LeftInLine[0].FP0.x!=0 && LeftInLine[0].FP0.y!=0 && LeftInLine[0].FP1.x!=0 && LeftInLine[0].FP1.y!=0){
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftStdThresHold);
					SingleLine->GImage = &RotateBinaryImg;
					LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;

					// �������� ���  
					if(fabs(LeftInLine[0].FP0.x - LeftInLine[0].FP1.x) > 2){
						LSP.x = (LeftInLine[0].FP0.x + LeftInLine[0].FP1.x)/2;
						LSP.y =  LeftInLine[0].FP0.y;
						LEP.x = (LeftInLine[0].FP0.x + LeftInLine[0].FP1.x)/2;
						LEP.y =  LeftInLine[0].FP1.y;
					}
					else {
						LSP.x =  LeftInLine[0].FP0.x;
						LSP.y =  LeftInLine[0].FP0.y;
						LEP.x =  LeftInLine[0].FP1.x;
						LEP.y =  LeftInLine[0].FP1.y;
					}
					// ���� ���� 
					SingleLine->IRoi->SetOrgX  ((int)LSP.x+10       );
					SingleLine->IRoi->SetOrgY  ((int)LSP.y          );
					SingleLine->IRoi->SetWidth (100                 );
					SingleLine->IRoi->SetHeight((int)(LEP.y - LSP.y));
					if((LSP.x+3)<=0 || LSP.y<0 || (LEP.y - LSP.y)<=0 || (LSP.x+3)>=ImgW || LSP.y>ImgH || (LEP.y - LSP.y)>ImgH) break;

					SingleLine->SetFindLineId(0);
					SingleLine->SetOrientation(orWest);
					SingleLine->SetEdgeType(1); // White To Black  
					SingleLine->OnExecute();
					LeftStdLine[0].FP0 = SingleLine->GetResultP0();
					LeftStdLine[0].FP1 = SingleLine->GetResultP1();

					//������ ��� 
					GrindMea1=0.0f,GrindMea2=0.0f;
					GrindMea1 = Math.GetLengthFrLineToPoint(LeftStdLine[0].FP0,LeftStdLine[0].FP1,LeftOutLine[0].FP0);
					GrindMea2 = Math.GetLengthFrLineToPoint(LeftStdLine[0].FP0,LeftStdLine[0].FP1,LeftOutLine[0].FP1);
					LeftGrindMea = (GrindMea1+GrindMea2)/2.0;
				}
			}
			if(FTopWall ){ // ImageRotate 
				EasyImage::Threshold(&RotateImg,&RotateBinaryImg, TopEdgeThresHold);
				if(TopEdgeThresHold==0)  {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5;}
				else                     {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

				SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()  );
				SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()  );
				SingleLine->IRoi->SetWidth (IRoi->GetWidth() );
				SingleLine->IRoi->SetHeight(IRoi->GetHeight());

				if(IRoi->GetOrgX()<=0 || IRoi->GetWidth()<=0 || IRoi->GetHeight()<=0){
					return false;
				}

				SingleLine->SetOrientation(orNorth);
				SingleLine->FIncrement =100;
				SingleLine->SetFindLineId(0);
				SingleLine->FThreshold =5;
				SingleLine->SetEdgeType(0);  // Black To White
				SingleLine->OnExecute();

				Line[1].FP0 = SingleLine->GetResultP0();
				Line[1].FP1 = SingleLine->GetResultP1();

				TopOutLine[0].FP0 = Line[1].FP0;
				TopOutLine[0].FP1 = Line[1].FP1;
			}
			if(FRightWall){
				EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightEdgeThresHold); //�⺻ RightEdgeLine 
				if(LeftEdgeThresHold==0) {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
				else                     {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

				if(IsCircleTop){
					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()  );
					SingleLine->IRoi->SetOrgY  (ASideCircleHeight+50);
					SingleLine->IRoi->SetWidth (IRoi->GetWidth() );
					SingleLine->IRoi->SetHeight(ImgH-(ASideCircleHeight+50));
					SingleLine->SetIncrement(1 );
				}
				else {
					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()  );
					SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()  );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth() );
					SingleLine->IRoi->SetHeight(IRoi->GetHeight());
					SingleLine->SetIncrement(5 );
				}

				SingleLine->SetOrientation(orEast);
				SingleLine->SetFindLineId(0);
				SingleLine->FThreshold =10;
				SingleLine->SetEdgeType(1); // White To Black 
				SingleLine->OnExecute();

				Line[2].FP0 = SingleLine->GetResultP0();
				Line[2].FP1 = SingleLine->GetResultP1();

				// Btm,Top ������� ù��°
				RightOutLine[0].FP0.x = Line[2].FP0.x;
				RightOutLine[0].FP0.y = Line[2].FP0.y;
				RightOutLine[0].FP1.x = Line[2].FP1.x;
				RightOutLine[0].FP1.y = Line[2].FP1.y;

				// RightOutLine....make OutLine Data 
				// ������ �����͸� �Ѱ��ش� 
				// �˻��� ���ض����� ã�� ���Ұ�� ���������� �Ѱ��༭ ���ض��� ������ 
				if(RightOutLine[0].FP0.x!=0.0f && RightOutLine[0].FP0.y!=0.0f && RightOutLine[0].FP1.x!=0.0f && RightOutLine[0].FP1.y!=0.0f){
					PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
					PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
				} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
				else if(RightOutLine[0].FP0.x==0.0f || RightOutLine[0].FP0.y==0.0f || RightOutLine[0].FP1.x==0.0f || RightOutLine[0].FP1.y==0.0f){
					if(PreRightOutLine[0].FP0.x==0.0f && PreRightOutLine[0].FP0.y==0.0f && PreRightOutLine[0].FP1.x==0.0f && PreRightOutLine[0].FP1.y==0.0f){// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
						// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
						ROI_H = IRoi->GetHeight();
						RightOutLine[0].FP0.x = Line[2].FP0.x;    RightOutLine[0].FP0.y = 0.0f        ;
						RightOutLine[0].FP1.x = Line[2].FP1.x;    RightOutLine[0].FP1.y = (float)ROI_H;

						//Line �����͸� PreLine�� �Ѱ��ش� 
						PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
						PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
					}
					else { // �߰��κ� Broken�߻����� �����Ͱ� ���°�� 
						// PreLine �����͸� �Ѱ��� 
						RightOutLine[0].FP0.x = PreRightOutLine[0].FP0.x;  RightOutLine[0].FP1.x = PreRightOutLine[0].FP1.x;  
						RightOutLine[0].FP0.y = PreRightOutLine[0].FP0.y;  RightOutLine[0].FP1.y = PreRightOutLine[0].FP1.y;  
					}
				}

				// RightGrindIn���� ���� 
				if(RightGrindThresHold!=0){ // ImageRotate 
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightGrindThresHold);
					SingleLine->GImage = &RotateBinaryImg;
					RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;
					// ROI���� ������  ���  
					if(fabs(RightOutLine[0].FP0.x - RightOutLine[0].FP1.x) > 2){
						RSP.x = (RightOutLine[0].FP0.x + RightOutLine[0].FP1.x)/2;
						RSP.y =  RightOutLine[0].FP0.y;
						REP.x = (RightOutLine[0].FP0.x + RightOutLine[0].FP1.x)/2;
						REP.y =  RightOutLine[0].FP1.y;
					}
					else {
						RSP.x =  RightOutLine[0].FP0.x;
						RSP.y =  RightOutLine[0].FP0.y;
						REP.x =  RightOutLine[0].FP1.x;
						REP.y =  RightOutLine[0].FP1.y;
					}
					// ���� ���� 
					SingleLine->IRoi->SetOrgX  ((int)RSP.x-100      );
					SingleLine->IRoi->SetOrgY  ((int)RSP.y          );
					SingleLine->IRoi->SetWidth (100                 );
					SingleLine->IRoi->SetHeight((int)(REP.y - RSP.y));
					if((RSP.x-100)<=0 || RSP.y<0 || (REP.y - RSP.y)<=0 || (RSP.x-100)>=ImgW || RSP.y>ImgH || (REP.y - RSP.y)>ImgH) break;

					SingleLine->SetFindLineId(0);
					SingleLine->SetOrientation(orEast);
					SingleLine->SetEdgeType(0); // Black to White 
					SingleLine->OnExecute();
					RightInLine[0].FP0 = SingleLine->GetResultP0();
					RightInLine[0].FP1 = SingleLine->GetResultP1();

					// ������ ��� 
					GrindWd1=0.0f,GrindWd2=0.0f;
					GrindWd1 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP0);
					GrindWd2 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP1);
					RightGrindWidth = (GrindWd1+GrindWd2)/2.0;
				}

				// ������ ���� ���ض��� ���� ������ 
				if(RightStdThresHold!=0 && RightInLine[0].FP0.x!=0 && RightInLine[0].FP0.y!=0 && RightInLine[0].FP1.x!=0 && RightInLine[0].FP1.y!=0){
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightStdThresHold);
					SingleLine->GImage = &RotateBinaryImg;
					RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;

					// �������� ���  
					if(fabs(RightInLine[0].FP0.x - RightInLine[0].FP1.x) > 2){
						RSP.x = (RightInLine[0].FP0.x + RightInLine[0].FP1.x)/2;
						RSP.y =  RightInLine[0].FP0.y;
						REP.x = (RightInLine[0].FP0.x + RightInLine[0].FP1.x)/2;
						REP.y =  RightInLine[0].FP1.y;
					}
					else {
						RSP.x =  RightInLine[0].FP0.x;
						RSP.y =  RightInLine[0].FP0.y;
						REP.x =  RightInLine[0].FP1.x;
						REP.y =  RightInLine[0].FP1.y;
					}
					// ���� ���� 
					SingleLine->IRoi->SetOrgX  ((int)RSP.x-100      );
					SingleLine->IRoi->SetOrgY  ((int)RSP.y          );
					SingleLine->IRoi->SetWidth (80                  );
					SingleLine->IRoi->SetHeight((int)(REP.y - RSP.y));
					if((RSP.x-100)<=0 || RSP.y<0 || (REP.y - RSP.y)<=0 || (RSP.x-100)>=ImgW || RSP.y>ImgH || (REP.y - RSP.y)>ImgH) break;

					SingleLine->SetFindLineId(0);
					SingleLine->SetOrientation(orEast);
					SingleLine->SetEdgeType(1); // White To Black  
					SingleLine->OnExecute();
					RightStdLine[0].FP0 = SingleLine->GetResultP0();
					RightStdLine[0].FP1 = SingleLine->GetResultP1();

					//������ ��� 
					GrindMea1=0.0f,GrindMea2=0.0f;
					GrindMea1 = Math.GetLengthFrLineToPoint(RightStdLine[0].FP0,RightStdLine[0].FP1,RightOutLine[0].FP0);
					GrindMea2 = Math.GetLengthFrLineToPoint(RightStdLine[0].FP0,RightStdLine[0].FP1,RightOutLine[0].FP1);
					RightGrindMea = (GrindMea1+GrindMea2)/2.0;
				}
			}

			//Image Rotate �� ������ �ٽ� ��� 
			FLTPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
			FRTPoint = Math.OnIntersect(Line[waRightWall].FP0, Line[waRightWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);

			if(FLeftTop){ // SoulBrain ������ ã�´� 
				EasyImage::Threshold(&RotateImg,&RotateBinaryImg     , ECThresHold         ); //Corner �ܰ����� ����     
				EasyImage::Threshold(&RotateImg,&RotateGrindBinaryImg, CorGrindMeaThresHold); //Corner 3Point������ ������ ���� ���� ���� 

				if(ECThresHold==0)  {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
				else                {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

				OrgPoint = Math.OnIntersect(Line[waLeftWall].FP0, Line[waLeftWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
				FLTPoint = OrgPoint;

				// Mark���� Top���α��� �Ÿ����� 
				if(IsMarkBtm){
					if(FP0.x!=0.0f && FP0.y!=0.0f && Line[1].FP0.x!=0.0f && Line[1].FP0.y!=0.0f && Line[1].FP1.x!=0.0f && Line[1].FP1.y!=0.0f ) {
						FP0 = Math.OnRotate(CenterPoint,FP0, -ImgDegree);
						LeftMarkToTopHeight = GetLengthFrLineToPoint(Line[1].FP0, Line[1].FP1, FP0);
					}
					// Right Mark To TopSide Height 
					if(FP1.x!=0.0f && FP1.y!=0.0f && Line[1].FP0.x!=0.0f && Line[1].FP0.y!=0.0f && Line[1].FP1.x!=0.0f && Line[1].FP1.y!=0.0f ) {
						FP1 = Math.OnRotate(CenterPoint,FP1, -ImgDegree);
						RightMarkToTopHeight = GetLengthFrLineToPoint(Line[1].FP0, Line[1].FP1, FP1);
					}
					// Center Mark To TopSide Height
					if(LeftMarkToTopHeight!=0.0f && RightMarkToTopHeight!=0.0f){
						CenMarkToTopHeight = ((FP0.y+FP1.y)/2.0) - ((Line[1].FP0.y+Line[1].FP1.y)/2.0);
					}
				}

				if(IsCornerTop){ //�ڳ� ������ �����Ǿ� ������� �ڳ� �˻�������(Dx,Dy,R,GW,GM ������)
					// LeftTop Corner OutLine Data 
					SingleLine->IRoi->SetOrgX  ((int)OrgPoint.x-10    );
					SingleLine->IRoi->SetOrgY  ((int)OrgPoint.y       );
					SingleLine->IRoi->SetWidth ((int)LTCornerWidth+20 );
					SingleLine->IRoi->SetHeight((int)LTCornerHeight   );

					SingleLine->SetOrientation(orWest);
					SingleLine->FIncrement =1;
					SingleLine->FThreshold =15;
					SingleLine->SetFindLineId(0);
					SingleLine->SetEdgeType(1);  //White To Black 
					SingleLine->OnExecute();

					Line[4].FP0 = SingleLine->GetResultP0();
					Line[4].FP1 = SingleLine->GetResultP1();

					// ORG ������ 
					FLTLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
					FLTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

					// 3Point �������� ������ ���� ��� ���� ������(GrindMark �������)
					if(AMarkToGrindMarkDvX==0.0f && AMarkToGrindMarkDvY==0.0f){ 
						if((FLTLPoint.x==0 || FLTLPoint.y==0 || FLTTPoint.x==0 || FLTTPoint.y==0) || (SingleLine->FEdgePointCount <30)){
							LTDx = 0.0f;
							LTDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FLTLPoint.x = FLTPoint.x;
							FLTLPoint.y = FLTPoint.y;
							FLTTPoint.x = FLTPoint.x;
							FLTTPoint.y = FLTPoint.y;
						}
						else { 
							LTDx = Math.GetLengthFrPointToPoint(FLTPoint, FLTTPoint);
							LTDy = Math.GetLengthFrPointToPoint(FLTPoint, FLTLPoint);

							// OutLine C1Corner Data 
							if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
								if(C1Point!=NULL) delete[] C1Point;
								C1Point  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
								C1EdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

								CornerCount =0;  
								for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
									if(CornerCount>=MAX_CORNER_POINT) break;
									C1Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
									C1Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
									CornerCount++;
								}
							}
							// Corner Grind In Gray Level ������ ���ؼ� Corner Grind Out/In ThresHold���� �з��ϴ� 
							// Corner Grind In Start 
							if(CorGrindInThresHold!=0){ // Corner InLine Search Sequence 
								EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
								SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

								// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
								// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
								// if(LTCornerWidth >(LTCornerHeight*3)){
								if(LTCornerWidth >(LTCornerHeight*1.5)){
									SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x );
									SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y );
									SingleLine->IRoi->SetWidth ((int)((FLTTPoint.x - FLTLPoint.x+5 )));
									SingleLine->IRoi->SetHeight((int)((FLTLPoint.y - FLTTPoint.y+10)));

									if(FLTPoint.x<=0    || FLTPoint.y<=0    || (FLTTPoint.x - FLTLPoint.x)<=0    || (FLTLPoint.y - FLTTPoint.y)<=0   ) return false;
									if(FLTPoint.x>=ImgW || FLTPoint.y>=ImgH || (FLTTPoint.x - FLTLPoint.x)>=ImgW || (FLTLPoint.y - FLTTPoint.y)>=ImgH) return false;

// 									SingleLine->SetOrientation(orNorth);
									SingleLine->SetOrientation(orSouth);
									SingleLine->FIncrement =2;
									SingleLine->SetFindLineId(0);
									//SingleLine->SetEdgeType(0); //Black To White 
									SingleLine->SetEdgeType(1);   //White To Black 
									SingleLine->OnExecute();

									// InLine C1Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C1InPoint!=NULL) delete[] C1InPoint;
										C1InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C1InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C1InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C1InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
								}
								else { // Corner ���� ���ʶ��� ã�´� 
									SingleLine->SetOrientation(orWest);
// 									SingleLine->SetOrientation(orSouth);
									SingleLine->SetFindLineId(0);
									SingleLine->FIncrement =1;
 									SingleLine->SetEdgeType(0); //Black To White 
// 									SingleLine->SetEdgeType(1); //White To Black 
									SingleLine->OnExecute();

									// InLine C1Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C1InPoint!=NULL) delete[] C1InPoint;
										C1InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C1InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C1InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C1InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
								}

								//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
								FLTLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
								FLTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

								//C1 �ڳ� ������(LTPS��쿡�� ������ ����)
								if(IsMarkBtm){
									Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
									for(int k=0; k<C1InEdgeCount; k++){
										Dis = Math.GetLengthFrLineToPoint(FLTLPoint, FLTTPoint, C1InPoint[k]);
										TotalGrindWidth+=Dis;
									}
									if(C1InEdgeCount!=0){
										AvgGrindWidth = (TotalGrindWidth/C1InEdgeCount);
										C1CorGrindWd  = AvgGrindWidth;
									}
								}
							}

							if(CorGrindMeaThresHold!=0){ // Corner Grind ThresHold���� �����Ǿ� ������츸 ������ �˻縦 �����Ѵ� 
								// �ڳ� ������ ���� ������ ã�´� 
								// ROI ���� ����(3���� ��� ���� ���� ������ ã�´�)
								if(CorGrindMeaThresHold==0){SingleLine->GImage = &RotateImg           ;  SingleLine->FThreshold =5 ;}
								else                       {SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold =10;}

								// ���� ������....�ϴ� ������ �ޱ������� �ӽ÷� ������ �з��Ͽ� CornerEdgePoint ã�� 
								if(LTCornerWidth >(LTCornerHeight*1.5)){
									SingleLine->IRoi->SetOrgX  ((int)OrgPoint.x       );
									SingleLine->IRoi->SetOrgY  ((int)OrgPoint.y+30    );
									SingleLine->IRoi->SetWidth (LTCornerWidth -50     );
									SingleLine->IRoi->SetHeight((int)LTCornerHeight-40);

									SingleLine->SetOrientation(orNorth);
									SingleLine->SetFindLineId(0);
									SingleLine->FIncrement =1;
									SingleLine->SetEdgeType(0);  //Black To White 
									SingleLine->OnExecute();

									// C1Corner Grind Measure Stand Data 
									if((SingleLine->FEdgePointCount)>0){
										if(C1MeaPoint!=NULL) delete[] C1MeaPoint;
										C1MeaPoint     = new TFPoint [(SingleLine->FEdgePointCount)];
										C1MeaEdgeCount = (SingleLine->FEdgePointCount);

										CornerCount =0;  
										for(int k=0; k<(SingleLine->FEdgePointCount); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C1MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C1MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									FLTLMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
									FLTTMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);


									//�ڳ� ������ ���  
									Dis =0.0f, AvgGrindMea=0.0f, TotalGrindMeasure=0.0f;
									for(int k=0; k<C1EdgeCount; k++){
										Dis = Math.GetLengthFrLineToPoint(FLTLMeaPoint, FLTTMeaPoint, C1Point[k]);
										TotalGrindMeasure+=Dis;
									}
									if(C1EdgeCount!=0){
										AvgGrindMea    = (TotalGrindMeasure/C1EdgeCount);
										C1CorGrindMea  = AvgGrindMea;
									}
								}
								else {
									SingleLine->IRoi->SetOrgX  ((int)OrgPoint.x+15 );
									SingleLine->IRoi->SetOrgY  ((int)OrgPoint.y+15 );
									SingleLine->IRoi->SetWidth ((int)LTCornerWidth );
									SingleLine->IRoi->SetHeight((int)LTCornerHeight);

									SingleLine->SetOrientation(orWest);
									SingleLine->SetFindLineId(0);
									SingleLine->FIncrement =1;
									SingleLine->SetEdgeType(0);  //Black To White 
									SingleLine->OnExecute();

									//�ڳ� ������ ���� ���ΰ� ���ο� ������ ���� 
									FLTLMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
									FLTTMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

									// C1Corner Grind Measure Stand Data 
									if((SingleLine->FEdgePointCount)>0){
										if(C1MeaPoint!=NULL) delete[] C1MeaPoint;
										C1MeaPoint     = new TFPoint [(SingleLine->FEdgePointCount)];
										C1MeaEdgeCount = (SingleLine->FEdgePointCount);

										CornerCount =0;  
										for(int k=0; k<(SingleLine->FEdgePointCount); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C1MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C1MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
									//�ڳ� ������ ���  
									Dis =0.0f, AvgGrindMea=0.0f, TotalGrindMeasure=0.0f;
									for(int k=0; k<C1EdgeCount; k++){
										Dis = Math.GetLengthFrLineToPoint(FLTLMeaPoint, FLTTMeaPoint, C1Point[k]);
										TotalGrindMeasure+=Dis;
									}
									if(C1EdgeCount!=0){
										AvgGrindMea    = (TotalGrindMeasure/C1EdgeCount);
										C1CorGrindMea  = AvgGrindMea;
									}
								}
							}
						}		
					}
					else { // ���ڰ� ��ũ ���� ������,������ ���� ������(LeftTop)
						TFPoint C1GrindRectPoint(0.0f, 0.0f);
						if((FLTLPoint.x==0 || FLTLPoint.y==0 || FLTTPoint.x==0 || FLTTPoint.y==0) || (SingleLine->FEdgePointCount <30)){
							LTDx = 0.0f;
							LTDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FLTLPoint.x = FLTPoint.x;
							FLTLPoint.y = FLTPoint.y;
							FLTTPoint.x = FLTPoint.x;
							FLTTPoint.y = FLTPoint.y;
						}
						else { 
							LTDx = Math.GetLengthFrPointToPoint(FLTPoint, FLTTPoint);
							LTDy = Math.GetLengthFrPointToPoint(FLTPoint, FLTLPoint);

							// OutLine C1Corner Data 
							if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
								if(C1Point!=NULL) {
									delete[] C1Point;
								}
								C1Point  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
								C1EdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

								CornerCount =0;  
								for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
									if(CornerCount>=MAX_CORNER_POINT) break;
									C1Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
									C1Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
									CornerCount++;
								}
							}

							// Corner Grind In Gray Level ������ ���ؼ� Corner Grind Out/In ThresHold���� �з��ϴ� 
							// Corner Grind In Start 
							if(CorGrindInThresHold!=0){ // Corner InLine Search Sequence 
								EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
								SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

								// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
								// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
								// if(LTCornerWidth >(LTCornerHeight*3)){
								if(LTCornerWidth >(LTCornerHeight*1.5)){
									SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x );
									SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y );
									SingleLine->IRoi->SetWidth ((int)((FLTTPoint.x - FLTLPoint.x+5 )));
									SingleLine->IRoi->SetHeight((int)((FLTLPoint.y - FLTTPoint.y+10)));

									if(FLTPoint.x<=0    || FLTPoint.y<=0    || (FLTTPoint.x - FLTLPoint.x)<=0    || (FLTLPoint.y - FLTTPoint.y)<=0   ) return false;
									if(FLTPoint.x>=ImgW || FLTPoint.y>=ImgH || (FLTTPoint.x - FLTLPoint.x)>=ImgW || (FLTLPoint.y - FLTTPoint.y)>=ImgH) return false;

// 									SingleLine->SetOrientation(orNorth);
									SingleLine->SetOrientation(orSouth);
									SingleLine->FIncrement =1;
									SingleLine->SetFindLineId(0);
									//SingleLine->SetEdgeType(0); //Black To White 
									SingleLine->SetEdgeType(1);   //White To Black 
									SingleLine->OnExecute();

									// InLine C1Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C1InPoint!=NULL) delete[] C1InPoint;
										C1InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C1InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C1InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C1InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
								}
								else {
									// Corner ���� ���ʶ��� ã�´� 
									SingleLine->SetOrientation(orWest);
									SingleLine->SetFindLineId(0);
									SingleLine->FIncrement =1;
									SingleLine->SetEdgeType(0);  //Black To White 
									SingleLine->OnExecute();

									// InLine C1Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C1InPoint!=NULL){
											delete[] C1InPoint;
										}
										C1InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C1InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C1InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C1InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
								}
								//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
								FLTLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
								FLTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);
							}

							// Corner Chamber Cut GrindMark���� ������,������ ������ 
							// LeftTop 
							if(IsMarkBtm && FP0.x!=0.0f && FP0.y!=0.0f){
								AGrindMark[0].Rect.left   = (FP0.x-AMarkToGrindMarkDvX)-40;
								AGrindMark[0].Rect.top    = (FP0.y+AMarkToGrindMarkDvY)-40;
								AGrindMark[0].Rect.right  = (AGrindMark[0].Rect.left + 80);
								AGrindMark[0].Rect.bottom = (AGrindMark[0].Rect.top  + 80);

								GrindRectWidth  = (AGrindMark[0].Rect.right  - AGrindMark[0].Rect.left);
								GrindRectHeight = (AGrindMark[0].Rect.bottom - AGrindMark[0].Rect.top );

								// �����̹������� Mark ã�´� 
								Find->OnLoad(true,FindDir,1)                   ;
								Find->GImage  = &RotateImg                      ;
								//Find->SetContrastMode(EFindContrastMode_Any)   ;
								Find->SetContrastMode(EFindContrastMode_Normal);
								//Find->SetMinScore(0.8f)                        ;
								Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind �˻� �ּ� Score
								Find->SetZeroScore(GrindPadMarkMinScore)       ; // �˻� �� �ּ� Score 
								//ind->SetMinScore (0.75)                       ; // PatternrFind �˻� �ּ� Score
								//ind->SetZeroScore(0.75)                       ; // �˻� �� �ּ� Score 
								Find->SetMaxInstances(1)                       ;
								Find->SetAngleTolerance(20.0f)                 ;
								Find->SetAngleBias(45.0f)                      ;
								Find->SetScaleBias(1.00f)                      ;
								Find->SetScaleTolerance(0.0f)                  ;
								Find->Property.Ox = (float)ImgW/2              ;
								Find->Property.Oy = (float)ImgH/2              ;

								Find->IRoi->SetPlacement(AGrindMark[0].Rect.left,AGrindMark[0].Rect.top,GrindRectWidth,GrindRectHeight);
								if(AGrindMark[0].Rect.left<=0 || AGrindMark[0].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

								// �˻� �� Match Pos ��ǥ �޾�´� 
								// Find �˻� ������ ������ �޾ƿ� 
								if(Find->OnExecute()){
									AGrindMark[0].Point.x = Find->Result[0].Px;
									AGrindMark[0].Point.y = Find->Result[0].Py;
									AGrindMark[0].Score   = Find->Result[0].Score;
								}

								// Corner GrindMakrã����� GrinaMark�������� ����,������ ����
								if(AGrindMark[0].Point.x!=0.0f && AGrindMark[0].Point.y!=0.0f){
									// GrindMark���� OutData �ִܰŸ� Count���� Calc 
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
									for(int k=0; k<C1EdgeCount; k++){
										if(C1Point[k].x  ==0.0f       || C1Point[k].y          ==0.0f) continue;
										if(AGrindMark[0].Point.x==0.0f || AGrindMark[0].Point.y==0.0f) continue;

										GrindMeaDis = Math.GetLength(AGrindMark[0].Point, C1Point[k]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkOutCnt = k;
										}
									}
									// OutData���� InData �ִܰŸ� Count���� Calc 
									if(GrindMarkOutCnt!=0){
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
										for(int k=0; k<C1InEdgeCount; k++){
											if(C1InPoint[k].x   ==0.0f          || C1InPoint[k].y             ==0.0f) continue;
											if(C1Point[GrindMarkOutCnt].x==0.0f || C1Point[GrindMarkOutCnt].y ==0.0f) continue;

											GrindMeaDis = Math.GetLength(C1Point[GrindMarkOutCnt], C1InPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkInCnt = k;
											}
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
									// ������ ���� 
									if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5 && (GrindMarkOutCnt+5)<C1EdgeCount){
										CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
										for(int k=GrindMarkOutCnt-5; k<GrindMarkOutCnt+5; k++){
											if(C1Point[k].x==0.0f || C1Point[k].y==0.0f) continue;

											GrindMeaDis = Math.GetLength(AGrindMark[0].Point, C1Point[k]);
											TotalCirDis+= GrindMeaDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											AGrindMark[0].GrindMea = TotalCirDis/CircleCount;
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
									if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5 && (GrindMarkOutCnt+5)<C1EdgeCount) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5 && (GrindMarkInCnt+5)<C1InEdgeCount)){
										CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
										for(int k=-5; k<5; k++){
											if(C1Point  [GrindMarkOutCnt+k].x==0.0f && C1Point  [GrindMarkOutCnt+k].y==0.0f) continue;
											if(C1InPoint[GrindMarkInCnt +k].x==0.0f && C1InPoint[GrindMarkInCnt +k].y==0.0f) continue;
											GrindWidthDis = Math.GetLength(C1InPoint[GrindMarkInCnt+k], C1Point[GrindMarkOutCnt+k]);
											TotalCirDis+=   GrindWidthDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											AGrindMark[0].GrindWidth  = TotalCirDis/CircleCount;
										}
									}
								}
								else { // GrindMrk ��ã����� �������� ������ 
									// OutData���� �ּҰ� ������ Count���� 
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
									for(int k=0; k<C1EdgeCount; k++){
										if(C1Point[k].x==0.0f || C1Point[k].y==0.0f) continue;

										C1GrindRectPoint.x = AGrindMark[0].Rect.left + (AGrindMark[0].Rect.right  - AGrindMark[0].Rect.left)/2; 
										C1GrindRectPoint.y = AGrindMark[0].Rect.top  + (AGrindMark[0].Rect.bottom - AGrindMark[0].Rect.top )/2; 

										GrindMeaDis = Math.GetLength(C1GrindRectPoint, C1Point[k]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkOutCnt = k;
										}
									}

									// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
									if(GrindMarkOutCnt!=0){
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
										for(int k=0; k<C1InEdgeCount; k++){
											if(C1InPoint[k].x            ==0.0f || C1InPoint[k].y             ==0.0f) continue;
											if(C1Point[GrindMarkOutCnt].x==0.0f || C1Point[GrindMarkOutCnt].y ==0.0f) continue;

											GrindMeaDis = Math.GetLength(C1Point[GrindMarkOutCnt], C1InPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkInCnt = k;
											}
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
									if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
										CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
										for(int k=-5; k<5; k++){
											if(C1Point  [GrindMarkOutCnt+k].x==0.0f && C1Point  [GrindMarkOutCnt+k].y==0.0f) continue;
											if(C1InPoint[GrindMarkInCnt +k].x==0.0f && C1InPoint[GrindMarkInCnt +k].y==0.0f) continue;
											GrindWidthDis = Math.GetLength(C1InPoint[GrindMarkInCnt+k], C1Point[GrindMarkOutCnt+k]);
											TotalCirDis+=   GrindWidthDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											AGrindMark[0].GrindWidth = TotalCirDis/CircleCount;
										}
									}
								}
							}
						}		
					}
				}
				else if(IsRectTop){ // �ڳ� �������� ������� ������ ���� �˻� Rect�˻�  
					LTDx = 0.0f;
					LTDy = 0.0f;
					//ȭ�鿡 ���÷��� ���ؼ� 
					FLTLPoint.x = FLTPoint.x;
					FLTLPoint.y = FLTPoint.y;
					FLTTPoint.x = FLTPoint.x;
					FLTTPoint.y = FLTPoint.y;
				}
				else if(IsCircleTop){ // TOP���� LeftTop Circle �˻� (A1Circle �����˻�),
					// A_Side Pad�������� TrackLine�������� ����, �� GrindMark���� ������,������ ������ ������ 
					// Org A1CircleData 
					int OrgA1CirInEdgeCount =0;		  
					int OrgA1CirOutEdgeCount=0;	    
					int A1GrindWidthCount   =0;		  
					int A1GrindMarkCount    =0;
					int A1Diameter          =0;
					GrindRectWidth          =0;
					GrindRectHeight         =0;

					TFPoint *OrgA1CirInPoint =NULL;	 
					TFPoint *OrgA1CirOutPoint=NULL;

					TFPoint AGrindMarkRectPoint1(0.0f, 0.0f);

					// Find Mark Search Start
					////////////////////////////////////////////////////////////////////////////
					if(IsMarkBtm){
						if(FP0.x!=0.0f && FP0.y!=0.0f){
							// �����̹������� Mark ã�´� 
							Find->OnLoad(true,FindDir,1)                   ;
							Find->GImage = &RotateImg                      ;
							// 										 Find->SetContrastMode(EFindContrastMode_Any)   ;
							Find->SetContrastMode(EFindContrastMode_Normal);
							// 										 Find->SetMinScore(0.8f)                        ;
							Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind �˻� �ּ� Score
							Find->SetZeroScore(GrindPadMarkMinScore)       ; // �˻� �� �ּ� Score 
							// 										 Find->SetMinScore (0.75)                       ; // PatternrFind �˻� �ּ� Score
							// 										 Find->SetZeroScore(0.75)                       ; // �˻� �� �ּ� Score 
							Find->SetMaxInstances(1)                       ;
							Find->SetAngleTolerance(20.0f)                 ;
							// 										 Find->SetAngleBias(45.0f)                      ;
							Find->SetScaleBias(1.00f)                      ;
							Find->SetScaleTolerance(0.0f)                  ;
							Find->Property.Ox = (float)ImgW/2              ;
							Find->Property.Oy = (float)ImgH/2              ;

							// ù��° GrindMark���� ���� A1 Search 
							if(AMarkToGrindMarkDvX!=0.0f && AMarkToGrindMarkDvY!=0.0f){
								AGrindMark[0].Rect.left   = (FP0.x-AMarkToGrindMarkDvX)-40;
								AGrindMark[0].Rect.top    = (FP0.y+AMarkToGrindMarkDvY)-40;
								AGrindMark[0].Rect.right  = (AGrindMark[0].Rect.left + 80);
								AGrindMark[0].Rect.bottom = (AGrindMark[0].Rect.top  + 80);

								GrindRectWidth  = (AGrindMark[0].Rect.right  - AGrindMark[0].Rect.left);
								GrindRectHeight = (AGrindMark[0].Rect.bottom - AGrindMark[0].Rect.top );

								Find->IRoi->SetPlacement(AGrindMark[0].Rect.left,AGrindMark[0].Rect.top,GrindRectWidth,GrindRectHeight);
								if(AGrindMark[0].Rect.left<=0 || AGrindMark[0].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

								// �˻� �� Match Pos ��ǥ �޾�´� 
								// Find �˻� ������ ������ �޾ƿ� 
								if(RqMode == true)
									Find->SetAngleBias(35.0f);
								else
									Find->SetAngleBias(45.0f);
								//Find->SetAngleBias(45.0f);
								if(Find->OnExecute()){
									AGrindMark[0].Point.x = Find->Result[0].Px;
									AGrindMark[0].Point.y = Find->Result[0].Py;
									AGrindMark[0].Score   = Find->Result[0].Score;
								}
								// Contrast Gradient Pixel Value
// 								GrindMarkGradient[25] = EasyImage::Focusing(Find->IRoi);
							}
						}
					}

					// SourBrain LeftTop 
					if(FInspectMode==Insp_Polygon){ // ���� � �˰��� 
						TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);

						//P1.x = (int)(FLTPoint.x-20);    P1.y =(int)(FLTPoint.y-20            ); P2.x = (int)(P1.x+ASideCircleWidth+50);    P2.y = (int)(P1.y);
						//P3.x = (int)(FLTPoint.x-20);    P3.y =(int)(P1.y+ASideCircleHeight+50); P4.x = (int)(P2.x                    );    P4.y = (int)(P3.y);

						//P1.x = (int)(FLTPoint.x-20);    P1.y =(int)(FLTPoint.y-20            ); P2.x = (int)(P1.x+ASideCircleWidth+40);    P2.y = (int)(P1.y);
						//P3.x = (int)(FLTPoint.x-20);    P3.y =(int)(P1.y+ASideCircleHeight+40); P4.x = (int)(P2.x                    );    P4.y = (int)(P3.y);

						P1.x = (int)(FLTPoint.x-20);    P1.y =(int)(FLTPoint.y-20            ); P2.x = (int)(P1.x+ASideCircleWidth+20);    P2.y = (int)(P1.y);
						P3.x = (int)(FLTPoint.x-20);    P3.y =(int)(P1.y+ASideCircleHeight+20); P4.x = (int)(P2.x                    );    P4.y = (int)(P3.y);

						// �˻�ȭ�� Display ���� 
						PadPolyRect[0].left  = P1.x;   PadPolyRect[0].top    = P1.y;
						PadPolyRect[0].right = P2.x;   PadPolyRect[0].bottom = P3.y;

						int PixelCnt =0;

						// Median Filter ���� 
						if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
						if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
						FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
						FilterROI.Attach(&RotateBinaryImg)                                         ;
						FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y))     ;
						LogUnit.SetLog(L"LeftTop ==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
						LogUnit.SetLog(L"LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
						EasyImage::Median(&FilterROI, &FilterROI)                                  ;
						//  									 RotateBinaryImg.Save("D:\\ROTATE_IMG\\Polygon_LeftTop_����.bmp");

						Polygon->GImage = &RotateBinaryImg                          ;
						Polygon->IWidth = ImgW                                      ;
						Polygon->IHeight= ImgH                                      ;
						Polygon->PoPa.FMinWidth  =(int)(ASideCircleWidth/2)         ;                       
						// 									 Polygon->PoPa.FMaxWidth  =(int)((FRTPoint.x - FLTPoint.x)/2);
						Polygon->PoPa.FMaxWidth  =(int)(ASideCircleWidth+100)       ;
						Polygon->PoPa.FMinHeight =(int)(ASideCircleHeight/2)        ;
						// 									 Polygon->PoPa.FMaxHeight =1000                              ;
						Polygon->PoPa.FMaxHeight =(int)(ASideCircleHeight+100)      ;
						Polygon->PoPa.MinPxCnt   =100                               ;
						Polygon->PoPa.MaxPxCnt   =20000                             ;
						Polygon->PoPa.MaxBbCnt   =10                                ;
						Polygon->PoPa.IsSeparate =true                              ;
						Polygon->PoPa.IsInToOut  =false                             ;
						Polygon->PoPa.HiThHold   = ECThresHold                      ;
						Polygon->PoPa.LoThHold   = 0                                ;

						if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
							Polygon->OnExecute(P1,P2,P3,P4);
						}

						// Polygon Algorithm InEdgeData/OutEdgeData Separate 
						if(Polygon->BlobCount==1){
							PolygonMarginCnt = (IsMarkBtm)? 5:20;
							if(Polygon->Blob[0].InCnt>(PolygonMarginCnt*2)){
								A1CirInEdgeCount=0;
								A1CirInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt*2));
								if(A1CirInEdgeCount>0){
									if(A1CirInPoint!=NULL) delete[] A1CirInPoint;
									A1CirInPoint = new TFPoint[A1CirInEdgeCount];

									PixelCnt=0;
									for(int k=0; k<Polygon->Blob[0].InCnt; k++){
										if(k<PolygonMarginCnt || k>=(Polygon->Blob[0].InCnt-PolygonMarginCnt)) continue;
										A1CirInPoint[PixelCnt].x   = Polygon->Blob[0].InPoint[k].x ;
										A1CirInPoint[PixelCnt].y   = Polygon->Blob[0].InPoint[k].y ;
										PixelCnt++;
									}
								}
							}

							if(Polygon->Blob[0].OutCnt>(PolygonMarginCnt*2)){
								A1CirOutEdgeCount=0;
								A1CirOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt*2));
								if(A1CirOutEdgeCount>0){
									if(A1CirOutPoint!=NULL) delete[] A1CirOutPoint;
									A1CirOutPoint = new TFPoint[A1CirOutEdgeCount];

									PixelCnt=0;
									for(int k=0; k<Polygon->Blob[0].OutCnt; k++){
										if(k<PolygonMarginCnt || k>=(Polygon->Blob[0].OutCnt-PolygonMarginCnt)) continue;
										A1CirOutPoint[PixelCnt].x  = Polygon->Blob[0].OutPoint[k].x ;
										A1CirOutPoint[PixelCnt].y  = Polygon->Blob[0].OutPoint[k].y ;
										PixelCnt++;
									}
								}
							}
						}

						// InEdgeData/OutEdgeData ���� ������ �� ������ ���
						// Polygon �˰��򿡼��� GrindMark���� OutData,InData �ִܰŸ� 2�� Tag�� �����ؾ��� 
						if(IsMarkBtm){ // LTPS ������,������ ��� 
							if(A1CirOutEdgeCount!=0 && A1CirInEdgeCount!=0){ // MarkMode��� ������ �˻縦 ���� ���� �� ������ ���� 
								// 25��° GrindMark Search �� ����,���� ������ ���
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								if(AGrindMark[0].Point.x!=0.0f && AGrindMark[0].Point.y!=0.0f){  //GrindMark ã������� 
									// GrindMark���� OutData �ִܰŸ� Count���� Calc 
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
									for(int k=0; k<A1CirOutEdgeCount; k++){
										if(A1CirOutPoint[k].x   ==0.0f || A1CirOutPoint[k].y   ==0.0f) continue;
										if(AGrindMark[0].Point.x==0.0f || AGrindMark[0].Point.y==0.0f) continue;

										GrindMeaDis = Math.GetLength(AGrindMark[0].Point, A1CirOutPoint[k]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkOutCnt = k; 
										}
									}
// 									// GrindMark���� InData �ִܰŸ� Count���� Calc 
// 									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
// 									for(int k=0; k<A1CirInEdgeCount; k++){
// 										if(A1CirInPoint[k].x    ==0.0f || A1CirInPoint[k].y     ==0.0f) continue;
// 										if(AGrindMark[0].Point.x==0.0f || AGrindMark[0].Point.y ==0.0f) continue;
// 
// 										GrindMeaDis = Math.GetLength(AGrindMark[0].Point, A1CirInPoint[k]);
// 										if(GrindMeaDis<MinDis){
// 											MinDis = GrindMeaDis;
// 											GrindMarkInCnt = k;
// 										}
// 									}

									// CircleOutData ���� InData �ִܰŸ� Count���� Calc 
									if(GrindMarkOutCnt!=0){
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
										for(int k=0; k<A1CirInEdgeCount; k++){
											if(A1CirInPoint[k].x   ==0.0f             || A1CirInPoint[k].y                ==0.0f) continue;
											if(A1CirOutPoint[GrindMarkOutCnt].x==0.0f || A1CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

											GrindMeaDis = Math.GetLength(A1CirOutPoint[GrindMarkOutCnt], A1CirInPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkInCnt = k;
											}
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
									if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5 &&(GrindMarkOutCnt+5<A1CirOutEdgeCount)){
										CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
										for(int k=GrindMarkOutCnt-5; k<GrindMarkOutCnt+5; k++){
											if(A1CirOutPoint[k].x==0.0f || A1CirOutPoint[k].y==0.0f) continue;

											GrindMeaDis = Math.GetLength(AGrindMark[0].Point, A1CirOutPoint[k]);
											TotalCirDis+= GrindMeaDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											AGrindMark[0].GrindMea = TotalCirDis/CircleCount;
											AGrindMark[0].GrindMea = AGrindMark[0].GrindMea+0.5; //SubPixel 
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
									if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
										CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
										for(int k=-5; k<5; k++){
											if(A1CirOutPoint[GrindMarkOutCnt+k].x==0.0f && A1CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
											if(A1CirInPoint [GrindMarkInCnt +k].x==0.0f && A1CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
											GrindWidthDis = Math.GetLength(A1CirInPoint[GrindMarkInCnt+k], A1CirOutPoint[GrindMarkOutCnt+k]);
											TotalCirDis+=   GrindWidthDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											AGrindMark[0].GrindWidth = TotalCirDis/CircleCount;
											AGrindMark[0].GrindWidth = AGrindMark[0].GrindWidth+0.5; //SubPixel
										}
									}
								}
								else if(AGrindMark[0].Rect.left!=0 && AGrindMark[0].Rect.top!=0 && AGrindMark[0].Rect.right!=0 && AGrindMark[0].Rect.bottom!=0){  //GrindMark �� ã�������
									// OutData���� �ּҰ� ������ Count���� 
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
									for(int k=0; k<A1CirOutEdgeCount; k++){
										if(A1CirOutPoint[k].x==0.0f || A1CirOutPoint[k].y==0.0f) continue;

										AGrindMarkRectPoint1.x = AGrindMark[0].Rect.left + (AGrindMark[0].Rect.right  - AGrindMark[0].Rect.left)/2; 
										AGrindMarkRectPoint1.y = AGrindMark[0].Rect.top  + (AGrindMark[0].Rect.bottom - AGrindMark[0].Rect.top )/2; 

										GrindMeaDis = Math.GetLength(AGrindMarkRectPoint1, A1CirOutPoint[k]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkOutCnt = k;
										}
									}

									// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
									if(GrindMarkOutCnt!=0){
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
										for(int k=0; k<A1CirInEdgeCount; k++){
											if(A1CirInPoint[k].x   ==0.0f             || A1CirInPoint[k].y                ==0.0f) continue;
											if(A1CirOutPoint[GrindMarkOutCnt].x==0.0f || A1CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

											GrindMeaDis = Math.GetLength(A1CirOutPoint[GrindMarkOutCnt], A1CirInPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkInCnt = k;
											}
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
									if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
										CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
										for(int k=-5; k<5; k++){
											if(A1CirOutPoint[GrindMarkOutCnt+k].x==0.0f && A1CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
											if(A1CirInPoint [GrindMarkInCnt +k].x==0.0f && A1CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
											GrindWidthDis = Math.GetLength(A1CirInPoint[GrindMarkInCnt+k], A1CirOutPoint[GrindMarkOutCnt+k]);
											TotalCirDis+=   GrindWidthDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											AGrindMark[0].GrindWidth = TotalCirDis/CircleCount;
											AGrindMark[0].GrindWidth = AGrindMark[0].GrindWidth+0.5; //SubPixel
										}
									}
								}
							}
						}
						else         { //Polygon EnCap ���� OutPoint �߰��� ���� ���� �Ÿ� ��հ��� ����Ѵ�(LeftTop) 
							if(A1CirInEdgeCount!=0 && A1CirOutEdgeCount!=0){
								GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
								GrindMarkOutCnt = (int)(A1CirOutEdgeCount/2);
								// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
								if(GrindMarkOutCnt!=0){
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
									for(int k=0; k<A1CirInEdgeCount; k++){
										if(A1CirInPoint[k].x   ==0.0f             || A1CirInPoint[k].y                ==0.0f) continue;
										if(A1CirOutPoint[GrindMarkOutCnt].x==0.0f || A1CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

										GrindMeaDis = Math.GetLength(A1CirOutPoint[GrindMarkOutCnt], A1CirInPoint[k]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkInCnt = k;
										}
									}
								}

								// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
								if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
									CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
									for(int k=-5; k<5; k++){
										if(A1CirOutPoint[GrindMarkOutCnt+k].x==0.0f && A1CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
										if(A1CirInPoint [GrindMarkInCnt +k].x==0.0f && A1CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
										GrindWidthDis = Math.GetLength(A1CirInPoint[GrindMarkInCnt+k], A1CirOutPoint[GrindMarkOutCnt+k]);
										TotalCirDis+=   GrindWidthDis;
										CircleCount++;
									}
									if(TotalCirDis!=0.0f && CircleCount!=0){
										AGrindMark[0].GrindWidth = TotalCirDis/CircleCount;
										AGrindMark[0].GrindWidth = AGrindMark[0].GrindWidth+0.5; //SubPixel
									}
								}
							}
						}

						// FAVION Circle Fitting �����(LeftTop)
						SampleSize = A1CirInEdgeCount*0.75; //0.5~0.8���Ϸ� �Է��Ұ�
						DistDevRatio = 0.15; //0.1~0.25 ���Ϸ� �Է��Ұ�
						CircleFit->OnRansac(A1CirInPoint,A1CirInEdgeCount,SampleSize,DistDevRatio,&ACircleFit1Center,&ACirFitDiameter1);

						// Polygon �ڸ� �˻� ������(LeftTop) 
						if(!IsMarkBtm && BrokenThresHold!=0){
							RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
							EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBrokenImg);
							EasyImage::Threshold(&RotateImg,&RotateBrokenImg, BrokenThresHold);
							Edge->GImage = &RotateBrokenImg;
							Edge->SetEdgeType(0);
							// 										 RotateBrokenImg.Save("D:\\ROTATE_IMG\\Polygon_LeftTop_�ڸ�.bmp");

							int DvCnt =0, DataCount=0;
							double deg =0.0f; 
							TFPoint Target(0.0f, 0.0f);

							if(GrindMarkInCnt>GrindMarkOutCnt){
								DvCnt = GrindMarkInCnt - GrindMarkOutCnt;
								for(int k=0; k<A1CirOutEdgeCount; k++){
									deg =0.0f, Target.x =0.0f, Target.y =0.0f;
									if(((k+DvCnt)<A1CirInEdgeCount) && A1CirInPoint[k+DvCnt].x>0  && A1CirInPoint[k+DvCnt].y>0){
										deg = Math.GetDegree(A1CirOutPoint[k],A1CirInPoint[k+DvCnt]);

										Target  = Math.OnFindPerpendicularPointXY(A1CirOutPoint[k],dEdgeToTrapLineDis,dEdgeToTrapLineDis,deg);
										Edge->OnExecute(A1CirOutPoint[k],Target); // �����ܰ����ο��� Target���� White Point Search 
										if(Edge->GetResultEdgePointCount()>0){
											A1CircleData[DataCount].Dis =0.0f;
											A1CircleData[DataCount].Dv  =0.0f;
											A1CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
											A1CircleData[DataCount].P.y = A1CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
											DataCount++;
										}
									}
								}
								A1CirInCrackCount = DataCount;
							}
							else if(GrindMarkInCnt<GrindMarkOutCnt)   {
								DvCnt = GrindMarkOutCnt - GrindMarkInCnt;
								for(int k=DvCnt; k<A1CirOutEdgeCount; k++){
									deg =0.0f, Target.x =0.0f, Target.y =0.0f;
									if(((k-DvCnt)<A1CirInEdgeCount) && A1CirInPoint[k-DvCnt].x>0  && A1CirInPoint[k-DvCnt].y>0 && k-DvCnt>=0){
										deg = Math.GetDegree(A1CirOutPoint[k],A1CirInPoint[k-DvCnt]);

										Target  = Math.OnFindPerpendicularPointXY(A1CirOutPoint[k],dEdgeToTrapLineDis,dEdgeToTrapLineDis,deg);
										Edge->OnExecute(A1CirOutPoint[k],Target); // �����ܰ����ο��� Target���� White Point Search 
										if(Edge->GetResultEdgePointCount()>0){
											A1CircleData[DataCount].Dis =0.0f;
											A1CircleData[DataCount].Dv  =0.0f;
											A1CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
											A1CircleData[DataCount].P.y = A1CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
											DataCount++;
										}
									}
								}
								A1CirInCrackCount = DataCount;
							}
						}
					}
					else if(FInspectMode==Insp_Mark){ // LeftTop
						// ������ �ܰ����� ������ ���� CirCenter ������ 
						if(ASideCircleWidth!=0.0f && ASideCircleHeight!=0.0f) A1CirCenter.x = FLTPoint.x + ASideCircleWidth;  A1CirCenter.y = FLTPoint.y + ASideCircleHeight;  
						// Circle Fitting ����� �������� ����ϴ� ������
						Circle->SetSamplingStep(1)                              ; // A1Circle Edge ���� 
						//A1 In Circle EdgePoint Data ���� 
						if(A1CirCenter.x!=0 && A1CirCenter.y!=0){
							// A_Side Circle �˻��� Median ���� ���� 
							TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
							P1.x = (int)(FLTPoint.x-20);    P1.y =(int)(FLTPoint.y-20            ); P2.x = (int)(P1.x+ASideCircleWidth+40);    P2.y = (int)(P1.y);
							P3.x = (int)(FLTPoint.x-20);    P3.y =(int)(P1.y+ASideCircleHeight+40); P4.x = (int)(P2.x                    );    P4.y = (int)(P3.y);

							// Median Filter ���� 
							if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
							if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
							FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
							FilterROI.Attach(&RotateBinaryImg)                                    ;
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"Px : %d, Py : %d, Width : %d, Height : %d => MedianFilter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"P1.x : %d, P1.y : %d, P2.x : %d, P2.y : %d, P3.x : %d, P3.y : %d, P4.x : %d, P4.y : %d => MedianFilter", P1.x, P1.y, P2.x, P2.y, P3.x, P3.y, P4.x, P4.y);
							EasyImage::Median(&FilterROI, &FilterROI)                             ;

							if(ECThresHold==0) Circle->GImage = &RotateImg      ; //Org Image �Ѱ��� 
							else               Circle->GImage = &RotateBinaryImg; //Binary Image �Ѱ��� 

							Circle->SetCenterXY(A1CirCenter.x , A1CirCenter.y)  ; // Circle Center ��ǥ���� 
							// 									 A1Diameter = ((ASideCircleWidth*2)+((ASideCircleWidth-OutToTrackLineDis)*2))/2;
							A1Diameter = (ASideCircleWidth*2);

							Circle->SetDiameter(A1Diameter)             ;
							Circle->SetTolerance(60.0f )                ; // A1Circle Tolerance 
							Circle->SetCircleChoice(1)                  ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
							Circle->SetCircleIndex (1)                  ; // Circle EdgePoint Index 
							Circle->SetAngle(180+CirAngleMargin)        ; // Cirlce Angle 
							Circle->SetAmplitude(90-(2*CirAngleMargin)) ; // Circle Amplitude

							// Circle InData EdgePoint 
							//////////////////////////////////////////////////////////////////////////////////////////////////////
							Circle->SetCircleType(2)                          ; // 0:Black to White, 1:White to Black 2: Both Black, White 
							Circle->OnExecute()                               ;

							// Circle In Data ���� ���ο� Circle Center ��ǥ �޾ƿ� 
							// Euresys Circle Fitting Center & Diameter  
							RealA1CirCenter.x = Circle->FCP.x;
							RealA1CirCenter.y = Circle->FCP.y;
							ACirDiameter1     = (Circle->Diameter)/2.0;

							// Circle In Data���� 
							OrgA1CirInEdgeCount = Circle->FCirclePointCount;
							if(OrgA1CirInEdgeCount>0){
								if(OrgA1CirInPoint!=NULL) delete[] OrgA1CirInPoint;
								OrgA1CirInPoint = new TFPoint[OrgA1CirInEdgeCount];

								for(int k=0; k<OrgA1CirInEdgeCount; k++){
									if(k>MAX_CIRCLE_EDGEPOINT) break;
									OrgA1CirInPoint[k].x = Circle->FCirclePoint[k].x;
									OrgA1CirInPoint[k].y = Circle->FCirclePoint[k].y;
									if(OrgA1CirInPoint[k].x==0.0f && OrgA1CirInPoint[k].y==0.0f) A1CirInEdgeCount++;
								}
							}
							//////////////////////////////////////////////////////////////////////////////////////////////////////

							// Circle Out Data �ٽ� �˻�  
							//////////////////////////////////////////////////////////////////////////////////////////////////////
							Circle->SetTolerance(60.0f )      ; // Circle Tolerance 
							Circle->SetDiameter (A1Diameter)  ; // Circle Diameter 
							Circle->SetCircleChoice(1)        ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
							Circle->SetCircleType(2)          ; // 0:Black to White, 1:White to Black 2: Both Black, White 
							Circle->SetCircleIndex (0)        ; // Circle EdgePoint Index 

							Circle->OnExecute()               ;

							// Circle Out Data���� 
							OrgA1CirOutEdgeCount = Circle->FCirclePointCount;
							if(OrgA1CirOutEdgeCount>0){
								if(OrgA1CirOutPoint!=NULL) delete[] OrgA1CirOutPoint;
								OrgA1CirOutPoint = new TFPoint[OrgA1CirOutEdgeCount];

								for(int k=0; k<OrgA1CirOutEdgeCount; k++){
									if(k>MAX_CIRCLE_EDGEPOINT) break;
									OrgA1CirOutPoint[k].x = Circle->FCirclePoint[k].x;
									OrgA1CirOutPoint[k].y = Circle->FCirclePoint[k].y;
									// BTM���� Circle�� ������ ���� Defect�˻� ������ ��ǥ��� 
									if(k==0                     ) {TopEdgePoint[0].x = OrgA1CirOutPoint[k].x; TopEdgePoint[0].y = OrgA1CirOutPoint[k].y;}
									if(k==OrgA1CirOutEdgeCount-1) {TopEdgePoint[1].x = OrgA1CirOutPoint[k].x; TopEdgePoint[1].y = OrgA1CirOutPoint[k].y;}
									if(OrgA1CirOutPoint[k].x==0.0f && OrgA1CirOutPoint[k].y==0.0f) A1CirOutEdgeCount++;
								}
							}

							// A1Circle Grind_Width Calculate (������ ���)
							if(IsMarkBtm){ // LTPS��� ������ ��� ==> ������ ��� 
								if(OrgA1CirOutEdgeCount!=0 && OrgA1CirInEdgeCount!=0){ // MarkMode��� ������ �˻縦 ���� ���� �� ������ ����  
									GrindMeaDis =0.0f, MinDis =10000.0f; A1GrindMarkCount=0;
									if(AGrindMark[0].Point.x!=0.0f && AGrindMark[0].Point.y!=0.0f){  //GrindMark ã������� 
										for(int k=0; k<OrgA1CirOutEdgeCount; k++){
											if(OrgA1CirOutPoint[k].x==0.0f || OrgA1CirOutPoint[k].y==0.0f) continue;
											if(AGrindMark[0].Point.x==0.0f || AGrindMark[0].Point.y==0.0f) continue;

											GrindMeaDis = Math.GetLength(AGrindMark[0].Point, OrgA1CirOutPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												A1GrindMarkCount = k;
											}
										}
										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ���� 
										if(A1GrindMarkCount!=0 && A1GrindMarkCount>5){
											CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
											for(int k=A1GrindMarkCount-5; k<A1GrindMarkCount+5; k++){
												if(OrgA1CirOutPoint[k].x==0.0f || OrgA1CirOutPoint[k].y==0.0f) continue;

												GrindMeaDis = Math.GetLength(AGrindMark[0].Point, OrgA1CirOutPoint[k]);
												TotalCirDis+= GrindMeaDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												AGrindMark[0].GrindMea = TotalCirDis/CircleCount;
											}
										}

										// GrindMark���� ���� ��հ� ������ ���� 
										if(A1GrindMarkCount!=0 && A1GrindMarkCount>5){
											CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
											for(int k=A1GrindMarkCount-5; k<A1GrindMarkCount+5; k++){
												if(OrgA1CirOutPoint[k].x==0.0f && OrgA1CirOutPoint[k].y==0.0f) continue;
												if(OrgA1CirInPoint [k].x==0.0f && OrgA1CirInPoint [k].y==0.0f) continue;
												GrindWidthDis = Math.GetLength(OrgA1CirInPoint[k], OrgA1CirOutPoint[k]);
												TotalCirDis+=   GrindWidthDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												AGrindMark[0].GrindWidth = TotalCirDis/CircleCount;
											}
										}
									}
									else {  //GrindMark �� ã�������
										for(int k=0; k<OrgA1CirOutEdgeCount; k++){
											if(OrgA1CirOutPoint[k].x==0.0f || OrgA1CirOutPoint[k].y==0.0f) continue;

											AGrindMarkRectPoint1.x = AGrindMark[0].Rect.left + (AGrindMark[0].Rect.right  - AGrindMark[0].Rect.left)/2; 
											AGrindMarkRectPoint1.y = AGrindMark[0].Rect.top  + (AGrindMark[0].Rect.bottom - AGrindMark[0].Rect.top )/2; 

											GrindMeaDis = Math.GetLength(AGrindMarkRectPoint1, OrgA1CirOutPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												A1GrindMarkCount = k;
											}
										}
										// GrindMark���� ���� ��հ� ������ ���� 
										if(A1GrindMarkCount!=0 && A1GrindMarkCount>5 && (A1GrindMarkCount+5<OrgA1CirOutEdgeCount)){
											CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
											for(int k=A1GrindMarkCount-5; k<A1GrindMarkCount+5; k++){
												if(OrgA1CirOutPoint[k].x==0.0f && OrgA1CirOutPoint[k].y==0.0f) continue;
												if(OrgA1CirInPoint [k].x==0.0f && OrgA1CirInPoint [k].y==0.0f) continue;
												GrindWidthDis = Math.GetLength(OrgA1CirInPoint[k], OrgA1CirOutPoint[k]);
												TotalCirDis+=   GrindWidthDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												AGrindMark[0].GrindWidth = TotalCirDis/CircleCount;
											}
										}
									}
								}
							}
							else     { // MarkMode ==> EnCap��� �������� ����Ѵ� 
								if(OrgA1CirInEdgeCount==OrgA1CirOutEdgeCount){
									double OutDis=0.0f, InDis=0.0f, TotalGrindWidth=0.0f, AvgGrindWidth=0.0f;A1GrindWidthCount=0;
									for(int k=0; k<OrgA1CirInEdgeCount; k++){
										if(OrgA1CirOutPoint[k].x==0.0f && OrgA1CirOutPoint[k].y==0.0f){
											OutDis =0.0f;
										}
										else {
											OutDis = Math.GetLengthFrPointToPoint(A1CirCenter,OrgA1CirOutPoint[k]);
										}

										if(OrgA1CirInPoint[k].x==0.0f && OrgA1CirInPoint[k].y==0.0f){
											InDis =0.0f;
										}
										else {
											InDis = Math.GetLengthFrPointToPoint(A1CirCenter,OrgA1CirInPoint[k]);
										}

										// CircleFitting ����� ���� EdgePoint �����Ͱ� ���� �� ��츸 �����Ͽ� ����Ѵ�
										if(OutDis!=0.0f && InDis!=0.0f){
											TotalGrindWidth+=(OutDis - InDis);
											A1GrindWidthCount++;
										}
									}
									if(A1GrindWidthCount!=0){
										AvgGrindWidth = (TotalGrindWidth/A1GrindWidthCount);
										AGrindMark[0].GrindWidth = AvgGrindWidth;
									}
								}
							}

							//////////////////////////////////////////////////////////////////////////////////////////////////////
							// Org Circle�������� ��ȣ���� ���� ���� ������ ���� �����ϴ� ������ �������� ���� �޸� ������
							// Circle Defect�˻翡 �ʿ��� Edge������ ��
							int Count=0;
							if(OrgA1CirInEdgeCount>0){ // A1 CircleInData
								A1CirInEdgeCount = OrgA1CirInEdgeCount - A1CirInEdgeCount;
								if(A1CirInEdgeCount>0){
									if(A1CirInPoint!=NULL) delete[] A1CirInPoint;
									A1CirInPoint = new TFPoint[A1CirInEdgeCount];

									for(int k=0; k<OrgA1CirInEdgeCount; k++){
										if(k>MAX_CIRCLE_EDGEPOINT) break;
										if(OrgA1CirInPoint[k].x==0.0f && OrgA1CirInPoint[k].y==0.0f) continue;
										A1CirInPoint[Count].x = OrgA1CirInPoint[k].x;
										A1CirInPoint[Count].y = OrgA1CirInPoint[k].y;
										Count++;
									}
								}
								// Circle ���� ������ �޸� ����
								if(OrgA1CirInPoint!=NULL) delete[] OrgA1CirInPoint;
							}
							// FAVION Circle Fitting ����� 
							SampleSize = A1CirInEdgeCount*0.75; //0.5~0.8���Ϸ� �Է��Ұ�
							DistDevRatio = 0.15; //0.1~0.25 ���Ϸ� �Է��Ұ�
							CircleFit->OnRansac(A1CirInPoint,A1CirInEdgeCount,SampleSize,DistDevRatio,&ACircleFit1Center,&ACirFitDiameter1);

							if(OrgA1CirOutEdgeCount>0){// A1 CircleOutData 
								A1CirOutEdgeCount = OrgA1CirOutEdgeCount - A1CirOutEdgeCount;
								Count =0;
								if(A1CirOutEdgeCount>0){
									if(A1CirOutPoint!=NULL) delete[] A1CirOutPoint;
									A1CirOutPoint = new TFPoint[A1CirOutEdgeCount];

									for(int k=0; k<OrgA1CirOutEdgeCount; k++){
										if(k>MAX_CIRCLE_EDGEPOINT) break;
										if(OrgA1CirOutPoint[k].x==0.0f && OrgA1CirOutPoint[k].y==0.0f) continue;
										A1CirOutPoint[Count].x = OrgA1CirOutPoint[k].x;
										A1CirOutPoint[Count].y = OrgA1CirOutPoint[k].y;
										Count++;
									}
								}
								// Circle ���� ������ �޸� ����
								if(OrgA1CirOutPoint!=NULL) delete[] OrgA1CirOutPoint;
							}
							// Circle ���� �ɹڸ�, �����ڸ� ���� ���� ������(EnCap)
							// ���� ��Ī�� ����.....�ϴ� Crack���� ���� 
							// SoulBrain LeftTop �ڸ� �˻� 
							if(!IsMarkBtm && BrokenThresHold!=0){
								RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
								EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBrokenImg);
								EasyImage::Threshold(&RotateImg,&RotateBrokenImg, BrokenThresHold);
								// 										 RotateBrokenImg.Save("D:\\ROTATE_IMG\\RotateBrokenImg_HAP.bmp");

								if(ACirFitDiameter1!=0.0f && ACircleFit1Center.x!=0.0f && ACircleFit1Center.y!=0.0f){ // FAVION Circle Fitting ������ �ڸ��˻� ���� 
									Circle->GImage = &RotateBrokenImg;                                             //Binary Image �Ѱ��� 

									Circle->SetCenterXY(ACircleFit1Center.x,ACircleFit1Center.y);
									// 											 Circle->SetTolerance(CirCrackMargin)                      ; // Circle Tolerance 
									// 											 Circle->SetDiameter ((ACirFitDiameter1-CirCrackMargin)*2)  ; // Circle Diameter 
									Circle->SetTolerance((dEdgeToTrapLineDis/2)-CirOffsetMargin)      ; // Circle Tolerance 
									Circle->SetDiameter ((ACirFitDiameter1-(dEdgeToTrapLineDis/2))*2) ; // Circle Diameter 
									Circle->SetCircleChoice(0)                                ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
									Circle->SetCircleType  (0)                                ; // 0:Black to White, 1:White to Black 2: Both Black, White 
									Circle->SetCircleIndex (0)                                ; // Circle EdgePoint Index 
									Circle->SetAmplitude(25)                                  ; // �������� ���ؼ� ����ų �߻�...������ �����ش� 
									Circle->OnExecute(true)                                   ;
									CrackACirDiameter1 = (ACirFitDiameter1-(dEdgeToTrapLineDis/2))+((dEdgeToTrapLineDis/2)-CirOffsetMargin);

									A1CirInCrackCount = Circle->FCirclePointCount;

									if(A1CirInCrackCount>0){
										if(A1CirInCrackPoint!=NULL) delete[] A1CirInCrackPoint;
										A1CirInCrackPoint = new TFPoint[A1CirInCrackCount];

										for(int k=0; k<A1CirInCrackCount; k++){
											if(k>MAX_CIRCLE_EDGEPOINT) break;
											A1CirInCrackPoint[k].x = Circle->FCirclePoint[k].x;
											A1CirInCrackPoint[k].y = Circle->FCirclePoint[k].y;
										}
									}
								}
							}
						}
					}
				}
			}

			if(FRightTop){
				EasyImage::Threshold(&RotateImg,&RotateBinaryImg     , ECThresHold         ); // Corner �ܰ����� ���� 
				EasyImage::Threshold(&RotateImg,&RotateGrindBinaryImg, CorGrindMeaThresHold); // Corner 3Point������ ������ ���� ���� ���� 
				//							 RotateBinaryImg.Save("D:\\ROTATE_IMG\\RotateBinaryImgȸ��_HAP.bmp");

				if(ECThresHold==0)  {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
				else                {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

				OrgPoint = Math.OnIntersect(Line[waRightWall].FP0, Line[waRightWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
				FRTPoint = OrgPoint;

				if(IsCornerTop){
					SingleLine->IRoi->SetOrgX  ((int)(OrgPoint.x-RTCornerWidth));
					SingleLine->IRoi->SetOrgY  ((int) OrgPoint.y               );
					SingleLine->IRoi->SetWidth (RTCornerWidth +10              );
					SingleLine->IRoi->SetHeight(RTCornerHeight                 );

					SingleLine->SetOrientation(orEast);
					SingleLine->FIncrement =1;
					SingleLine->FThreshold =15;
					SingleLine->SetFindLineId(0);
					SingleLine->SetEdgeType(1);  //White To Black 
					SingleLine->OnExecute();
					Line[waRightTop].FP0 = SingleLine->GetResultP0();
					Line[waRightTop].FP1 = SingleLine->GetResultP1();

					// ORG 
					FRTRPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
					FRTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall  ].FP0, Line[waTopWall].FP1  );

					if(AMarkToGrindMarkDvX==0.0f && AMarkToGrindMarkDvY==0.0f){ // 3Point �������� ������ ���� ��� ���� ������ 
						if((FRTRPoint.x==0 || FRTRPoint.y==0 || FRTTPoint.x==0 || FRTTPoint.y==0) || (SingleLine->FEdgePointCount <30)){
							RTDx = 0.0f;
							RTDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FRTRPoint.x = FRTPoint.x;
							FRTRPoint.y = FRTPoint.y;
							FRTTPoint.x = FRTPoint.x;
							FRTTPoint.y = FRTPoint.y;
						}
						else {
							RTDx = Math.GetLengthFrPointToPoint(FRTPoint, FRTTPoint);
							RTDy = Math.GetLengthFrPointToPoint(FRTPoint, FRTRPoint);

							// OutLine C2Corner Data 
							if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
								if(C2Point!=NULL) delete[] C2Point;
								C2Point  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
								C2EdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

								CornerCount =0;  
								for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
									if(CornerCount>=MAX_CORNER_POINT) break;
									C2Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
									C2Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
									CornerCount++;
								}
							}

							// Corner Grind In Start 
							if(CorGrindInThresHold!=0){
								EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
								SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

								// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
								// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
								// if(RTCornerWidth >(RTCornerHeight*3)){
								if(RTCornerWidth >(RTCornerHeight*1.5)){
									SingleLine->IRoi->SetOrgX  ((int)(FRTPoint.x-(FRTRPoint.x-FRTTPoint.x)+5));
									SingleLine->IRoi->SetOrgY  ((int) FRTPoint.y                             );
									SingleLine->IRoi->SetWidth ((int)((FRTRPoint.x-FRTTPoint.x)+5)           );
									SingleLine->IRoi->SetHeight((int)((FRTRPoint.y - FRTTPoint.y)+10)        );

									if(FRTTPoint.x<=0    || FRTTPoint.y<=0    || (FRTRPoint.x - FRTTPoint.x)<=0    || (FRTRPoint.y - FRTTPoint.y)<=0   ) return false;
									if(FRTTPoint.x>=ImgW || FRTTPoint.y>=ImgH || (FRTRPoint.x - FRTTPoint.x)>=ImgW || (FRTRPoint.y - FRTTPoint.y)>=ImgH) return false;

									//SingleLine->SetOrientation(orNorth);
									SingleLine->SetOrientation(orSouth);
									SingleLine->FIncrement =2;
									SingleLine->SetFindLineId(0);
									//SingleLine->SetEdgeType(0);  //Black To White 
									SingleLine->SetEdgeType(1);  //Black To White 
									SingleLine->OnExecute();

									// InLine C2Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C2InPoint!=NULL) delete[] C2InPoint;
										C2InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C2InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C2InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C2InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
								}
								else {
									// Corner ���� ���ʶ��� ã�´� 
									SingleLine->SetOrientation(orEast);
									SingleLine->SetFindLineId(0);
									SingleLine->FIncrement =1;
									SingleLine->SetEdgeType(0);  //Black To White 
									SingleLine->OnExecute();

									// InLine C2Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C2InPoint!=NULL) delete[] C2InPoint;
										C2InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C2InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C2InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C2InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
								}

								//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
								FRTRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
								FRTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall  ].FP0, Line[waTopWall].FP1  );

								//�ڳ� ������, ������ ��� 
								Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
								for(int k=0; k<C2InEdgeCount; k++){
									Dis = Math.GetLengthFrLineToPoint(FRTRPoint, FRTTPoint, C2InPoint[k]);
									TotalGrindWidth+=Dis;
								}
								if(C2InEdgeCount!=0){
									AvgGrindWidth = (TotalGrindWidth/C2InEdgeCount);
									C2CorGrindWd  = AvgGrindWidth;
								}
							}

							if(CorGrindMeaThresHold!=0){ // Corner Grind ThresHold���� �����Ǿ� ������츸 ������ �˻縦 �����Ѵ� 
								// �ڳ� ������ ���� ������ ã�´� 
								// ROI ���� ���� 
								if(CorGrindMeaThresHold==0){SingleLine->GImage = &RotateImg           ;  SingleLine->FThreshold =5 ;}
								else                       {SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold =10;}

								// ���� ������....�ϴ� ������ �ޱ������� �ӽ÷� ������ �з��Ͽ� CornerEdgePoint ã�� 
								if(RTCornerWidth >(RTCornerHeight*1.5)){
									SingleLine->IRoi->SetOrgX  ((int) OrgPoint.x-(RTCornerWidth-50));
									SingleLine->IRoi->SetOrgY  ((int) OrgPoint.y+30                );
									SingleLine->IRoi->SetWidth (RTCornerWidth -50                  );
									SingleLine->IRoi->SetHeight(RTCornerHeight-40                  );

									SingleLine->SetOrientation(orNorth);
									SingleLine->SetFindLineId(0);
									SingleLine->FIncrement =1;
									SingleLine->SetEdgeType(0);  //Black To White 
									SingleLine->OnExecute();

// 									P1 = SingleLine->GetResultP0();
// 									P2 = SingleLine->GetResultP1();

									// C1Corner Grind Measure Stand Data 
									if((SingleLine->FEdgePointCount)>0){
										if(C2MeaPoint!=NULL) delete[] C2MeaPoint;
										C2MeaPoint     = new TFPoint [(SingleLine->FEdgePointCount)];
										C2MeaEdgeCount = (SingleLine->FEdgePointCount);

										CornerCount =0;  
										for(int k=0; k<(SingleLine->FEdgePointCount); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C2MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C2MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
									//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
									FRTRMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
									FRTTMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall  ].FP0, Line[waTopWall].FP1  );

									//�ڳ� ������ ���  
									Dis =0.0f, AvgGrindMea=0.0f, TotalGrindMeasure=0.0f;
									for(int k=0; k<C2EdgeCount; k++){
										Dis = Math.GetLengthFrLineToPoint(FRTRMeaPoint, FRTTMeaPoint, C2Point[k]);
										TotalGrindMeasure+=Dis;
									}
									if(C2EdgeCount!=0){
										AvgGrindMea    = (TotalGrindMeasure/C2EdgeCount);
										C2CorGrindMea  = AvgGrindMea;
									}
								}
								else {
									// 											 SingleLine->IRoi->SetOrgX  ((int) OrgPoint.x-RTCornerWidth    );
									SingleLine->IRoi->SetOrgX  ((int) OrgPoint.x-RTCornerWidth+20 );
									SingleLine->IRoi->SetOrgY  ((int) OrgPoint.y+15               );
									SingleLine->IRoi->SetWidth (RTCornerWidth                     );
									// 											 SingleLine->IRoi->SetHeight(RTCornerHeight                    );
									SingleLine->IRoi->SetHeight(RTCornerHeight+10                 );

									SingleLine->SetOrientation(orEast);
									SingleLine->SetFindLineId(0);
									SingleLine->FIncrement =1;
									SingleLine->SetEdgeType(0);  //Black To White 
									SingleLine->OnExecute();

// 									P1 = SingleLine->GetResultP0();
// 									P2 = SingleLine->GetResultP1();

									// C1Corner Grind Measure Stand Data 
									if((SingleLine->FEdgePointCount)>0){
										if(C2MeaPoint!=NULL) delete[] C2MeaPoint;
										C2MeaPoint     = new TFPoint [(SingleLine->FEdgePointCount)];
										C2MeaEdgeCount = (SingleLine->FEdgePointCount);

										CornerCount =0;  
										for(int k=0; k<(SingleLine->FEdgePointCount); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C2MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C2MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
									FRTRMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
									FRTTMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall  ].FP0, Line[waTopWall].FP1  );

									//�ڳ� ������ ���  
									Dis =0.0f, AvgGrindMea=0.0f, TotalGrindMeasure=0.0f;
									for(int k=0; k<C2EdgeCount; k++){
										Dis = Math.GetLengthFrLineToPoint(FRTRMeaPoint, FRTTMeaPoint, C2Point[k]);
										TotalGrindMeasure+=Dis;
									}
									if(C2EdgeCount!=0){
										AvgGrindMea    = (TotalGrindMeasure/C2EdgeCount);
										C2CorGrindMea  = AvgGrindMea;
									}
								}
							}
						}
					}
					else { //���ڰ� ��ũ ���� ������,������ ���� ������(RightTop)
						TFPoint C2GrindRectPoint(0.0f, 0.0f);
						if((FRTRPoint.x==0 || FRTRPoint.y==0 || FRTTPoint.x==0 || FRTTPoint.y==0) || (SingleLine->FEdgePointCount <30)){
							RTDx = 0.0f;
							RTDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FRTRPoint.x = FRTPoint.x;
							FRTRPoint.y = FRTPoint.y;
							FRTTPoint.x = FRTPoint.x;
							FRTTPoint.y = FRTPoint.y;
						}
						else {
							RTDx = Math.GetLengthFrPointToPoint(FRTPoint, FRTTPoint);
							RTDy = Math.GetLengthFrPointToPoint(FRTPoint, FRTRPoint);

							// OutLine C2Corner Data 
							if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
								if(C2Point!=NULL) delete[] C2Point;
								C2Point  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
								C2EdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

								CornerCount =0;  
								for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
									if(CornerCount>=MAX_CORNER_POINT) break;
									C2Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
									C2Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
									CornerCount++;
								}
							}

							// Corner Grind In Start 
							if(CorGrindInThresHold!=0){
								EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
								SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

								// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
								// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
								// if(RTCornerWidth >(RTCornerHeight*3)){
								if(RTCornerWidth >(RTCornerHeight*1.5)){
									SingleLine->IRoi->SetOrgX  ((int)(FRTPoint.x-(FRTRPoint.x-FRTTPoint.x)+5));
									SingleLine->IRoi->SetOrgY  ((int) FRTPoint.y                             );
									SingleLine->IRoi->SetWidth ((int)((FRTRPoint.x-FRTTPoint.x)+5)           );
									SingleLine->IRoi->SetHeight((int)((FRTRPoint.y - FRTTPoint.y)+10)        );

									if(FRTTPoint.x<=0    || FRTTPoint.y<=0    || (FRTRPoint.x - FRTTPoint.x)<=0    || (FRTRPoint.y - FRTTPoint.y)<=0   ) return false;
									if(FRTTPoint.x>=ImgW || FRTTPoint.y>=ImgH || (FRTRPoint.x - FRTTPoint.x)>=ImgW || (FRTRPoint.y - FRTTPoint.y)>=ImgH) return false;

									SingleLine->SetOrientation(orNorth);
									SingleLine->FIncrement =1;
									SingleLine->SetFindLineId(0);
									SingleLine->SetEdgeType(0);  //Black To White 
									SingleLine->OnExecute();

									// InLine C2Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C2InPoint!=NULL) delete[] C2InPoint;
										C2InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C2InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C2InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C2InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
								}
								else {
									// Corner ���� ���ʶ��� ã�´� 
									SingleLine->SetOrientation(orEast);
									SingleLine->SetFindLineId(0);
									SingleLine->FIncrement =1;
									SingleLine->SetEdgeType(0);  //Black To White 
									SingleLine->OnExecute();

									// InLine C2Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C2InPoint!=NULL) delete[] C2InPoint;
										C2InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C2InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C2InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C2InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
								}

								//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
								FRTRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
								FRTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall  ].FP0, Line[waTopWall].FP1  );
							}

							// GrindMark���� ������,������ ������ 
							// RightTop 
							if(IsMarkBtm && FP1.x!=0.0f && FP1.y!=0.0f){
								AGrindMark[1].Rect.left   = (FP1.x+AMarkToGrindMarkDvX)-40;
								AGrindMark[1].Rect.top    = (FP1.y+AMarkToGrindMarkDvY)-40;
								AGrindMark[1].Rect.right  = (AGrindMark[1].Rect.left + 80);
								AGrindMark[1].Rect.bottom = (AGrindMark[1].Rect.top  + 80);

								GrindRectWidth  = (AGrindMark[1].Rect.right  - AGrindMark[1].Rect.left);
								GrindRectHeight = (AGrindMark[1].Rect.bottom - AGrindMark[1].Rect.top );

								// �����̹������� Mark ã�´� 
								Find->OnLoad(true,FindDir,1)                   ;
								Find->GImage = &RotateImg                      ;
								//Find->SetContrastMode(EFindContrastMode_Any)   ;
								Find->SetContrastMode(EFindContrastMode_Normal);
								//Find->SetMinScore(0.8f)                        ;
								Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind �˻� �ּ� Score
								Find->SetZeroScore(GrindPadMarkMinScore)       ; // �˻� �� �ּ� Score 
								//ind->SetMinScore (0.75)                       ; // PatternrFind �˻� �ּ� Score
								//ind->SetZeroScore(0.75)                       ; // �˻� �� �ּ� Score 
								Find->SetMaxInstances(1)                       ;
								Find->SetAngleTolerance(20.0f)                 ;
								Find->SetAngleBias(135.0f)                     ;
								Find->SetScaleBias(1.00f)                      ;
								Find->SetScaleTolerance(0.0f)                  ;
								Find->Property.Ox = (float)ImgW/2              ;
								Find->Property.Oy = (float)ImgH/2              ;

								Find->IRoi->SetPlacement(AGrindMark[1].Rect.left,AGrindMark[1].Rect.top,GrindRectWidth,GrindRectHeight);
								if(AGrindMark[1].Rect.left<=0 || AGrindMark[1].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

								// �˻� �� Match Pos ��ǥ �޾�´� 
								// Find �˻� ������ ������ �޾ƿ� 
								if(Find->OnExecute()){
									AGrindMark[1].Point.x = Find->Result[0].Px;
									AGrindMark[1].Point.y = Find->Result[0].Py;
									AGrindMark[1].Score   = Find->Result[0].Score;
								}
								// Contrast Gradient Pixel Value
// 								GrindMarkGradient[20] = EasyImage::Focusing(Find->IRoi);
							}

							// GrindMakrã����� ������,������ ������
							if(AGrindMark[1].Point.x!=0.0f && AGrindMark[1].Point.y!=0.0f){
								// GrindMark���� OutData �ִܰŸ� Count���� Calc 
								GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
								for(int k=0; k<C2EdgeCount; k++){
									if(C2Point[k].x  ==0.0f        || C2Point[k].y        ==0.0f) continue;
									if(AGrindMark[1].Point.x==0.0f || AGrindMark[1].Point.y==0.0f) continue;

									GrindMeaDis = Math.GetLength(AGrindMark[1].Point, C2Point[k]);
									if(GrindMeaDis<MinDis){
										MinDis = GrindMeaDis;
										GrindMarkOutCnt = k;
									}
								}
								// OutData���� InData �ִܰŸ� Count���� Calc 
								if(GrindMarkOutCnt!=0){
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
									for(int k=0; k<C2InEdgeCount; k++){
										if(C2InPoint[k].x   ==0.0f          || C2InPoint[k].y             ==0.0f) continue;
										if(C2Point[GrindMarkOutCnt].x==0.0f || C2Point[GrindMarkOutCnt].y ==0.0f) continue;

										GrindMeaDis = Math.GetLength(C2Point[GrindMarkOutCnt], C2InPoint[k]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkInCnt = k;
										}
									}
								}

								// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
								if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5 && (GrindMarkOutCnt+5)<C2EdgeCount){
									CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
									for(int k=GrindMarkOutCnt-5; k<GrindMarkOutCnt+5; k++){
										if(C2Point[k].x==0.0f || C2Point[k].y==0.0f) continue;

										GrindMeaDis = Math.GetLength(AGrindMark[1].Point, C2Point[k]);
										TotalCirDis+= GrindMeaDis;
										CircleCount++;
									}
									if(TotalCirDis!=0.0f && CircleCount!=0){
										AGrindMark[1].GrindMea = TotalCirDis/CircleCount;
									}
								}

								// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
								if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5 && (GrindMarkOutCnt+5)<C2EdgeCount) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5 && (GrindMarkInCnt+5)<C2InEdgeCount)){
									CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
									for(int k=-5; k<5; k++){
										if(C2Point  [GrindMarkOutCnt+k].x==0.0f && C2Point  [GrindMarkOutCnt+k].y==0.0f) continue;
										if(C2InPoint[GrindMarkInCnt +k].x==0.0f && C2InPoint[GrindMarkInCnt +k].y==0.0f) continue;
										GrindWidthDis = Math.GetLength(C2InPoint[GrindMarkInCnt+k], C2Point[GrindMarkOutCnt+k]);
										TotalCirDis+=   GrindWidthDis;
										CircleCount++;
									}
									if(TotalCirDis!=0.0f && CircleCount!=0){
										AGrindMark[1].GrindWidth  = TotalCirDis/CircleCount;
									}
								}
							}
							else { // GrindMrk ��ã����� �������� ������ 
								// OutData���� �ּҰ� ������ Count���� 
								GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
								for(int k=0; k<C2EdgeCount; k++){
									if(C2Point[k].x==0.0f || C2Point[k].y==0.0f) continue;

									C2GrindRectPoint.x = AGrindMark[1].Rect.left + (AGrindMark[1].Rect.right  - AGrindMark[1].Rect.left)/2; 
									C2GrindRectPoint.y = AGrindMark[1].Rect.top  + (AGrindMark[1].Rect.bottom - AGrindMark[1].Rect.top )/2; 

									GrindMeaDis = Math.GetLength(C2GrindRectPoint, C2Point[k]);
									if(GrindMeaDis<MinDis){
										MinDis = GrindMeaDis;
										GrindMarkOutCnt = k;
									}
								}

								// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
								if(GrindMarkOutCnt!=0){
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
									for(int k=0; k<C2InEdgeCount; k++){
										if(C2InPoint[k].x            ==0.0f || C2InPoint[k].y             ==0.0f) continue;
										if(C2Point[GrindMarkOutCnt].x==0.0f || C2Point[GrindMarkOutCnt].y ==0.0f) continue;

										GrindMeaDis = Math.GetLength(C2Point[GrindMarkOutCnt], C2InPoint[k]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkInCnt = k;
										}
									}
								}

								// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
								if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
									CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
									for(int k=-5; k<5; k++){
										if(C2Point  [GrindMarkOutCnt+k].x==0.0f && C2Point  [GrindMarkOutCnt+k].y==0.0f) continue;
										if(C2InPoint[GrindMarkInCnt +k].x==0.0f && C2InPoint[GrindMarkInCnt +k].y==0.0f) continue;
										GrindWidthDis = Math.GetLength(C2InPoint[GrindMarkInCnt+k], C2Point[GrindMarkOutCnt+k]);
										TotalCirDis+=   GrindWidthDis;
										CircleCount++;
									}
									if(TotalCirDis!=0.0f && CircleCount!=0){
										AGrindMark[1].GrindWidth = TotalCirDis/CircleCount;
									}
								}
							}
						}
					}
				}
				else if(IsRectTop){
					RTDx = 0.0f;
					RTDy = 0.0f;
					//ȭ�鿡 ���÷��� ���ؼ� 
					FRTRPoint.x = FRTPoint.x;
					FRTRPoint.y = FRTPoint.y;
					FRTTPoint.x = FRTPoint.x;
					FRTTPoint.y = FRTPoint.y;
				}
				else if(IsCircleTop){ // TOP���� RightTop Circle �˻�  A2Circle 
					// Org A1CircleData 
					int OrgA2CirInEdgeCount =0;		  
					int OrgA2CirOutEdgeCount=0;	    
					int A2GrindWidthCount   =0;		  
					int A2GrindMarkCount    =0;
					int A2Diameter          =0;
					GrindRectWidth          =0;
					GrindRectHeight         =0;

					TFPoint *OrgA2CirInPoint =NULL;	 
					TFPoint *OrgA2CirOutPoint=NULL;

					TFPoint AGrindMarkRectPoint2(0.0f, 0.0f);

					// Find Mark Search Start
					////////////////////////////////////////////////////////////////////////////
					if(IsMarkBtm){
						if(FP1.x!=0.0f && FP1.y!=0.0f){
							Find->OnLoad(true,FindDir,1)                   ;
							Find->GImage = &RotateImg                      ;
							Find->SetContrastMode(EFindContrastMode_Normal);
							// 										 Find->SetContrastMode(EFindContrastMode_Any    );
							// 										 Find->SetMinScore(0.8f)                        ;
							Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind �˻� �ּ� Score
							Find->SetZeroScore(GrindPadMarkMinScore)       ; // �˻� �� �ּ� Score 
							// 										 Find->SetMinScore (0.75)                       ; // PatternrFind �˻� �ּ� Score
							// 										 Find->SetZeroScore(0.75)                       ; // �˻� �� �ּ� Score 
							Find->SetMaxInstances(1)                       ;
							Find->SetAngleTolerance(20.0f)                 ;
							// 										 Find->SetAngleBias(135.0f)                     ;
							Find->SetScaleBias(1.00f)                      ;
							Find->SetScaleTolerance(0.0f)                  ;
							Find->Property.Ox = (float)ImgW/2              ;
							Find->Property.Oy = (float)ImgH/2              ;

							// ù��° GrindMark���� ���� A2 Search
							if(AMarkToGrindMarkDvX!=0.0f && AMarkToGrindMarkDvX!=0.0f){
								AGrindMark[1].Rect.left   = (FP1.x+AMarkToGrindMarkDvX)-40;
								AGrindMark[1].Rect.top    = (FP1.y+AMarkToGrindMarkDvY)-40;
								AGrindMark[1].Rect.right  = (AGrindMark[1].Rect.left + 80);
								AGrindMark[1].Rect.bottom = (AGrindMark[1].Rect.top  + 80);

								GrindRectWidth  = (AGrindMark[1].Rect.right  - AGrindMark[1].Rect.left);
								GrindRectHeight = (AGrindMark[1].Rect.bottom - AGrindMark[1].Rect.top );

								Find->IRoi->SetPlacement(AGrindMark[1].Rect.left,AGrindMark[1].Rect.top,GrindRectWidth,GrindRectHeight);
								if(AGrindMark[1].Rect.left<=0 ||AGrindMark[1].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

								// �˻� �� Match Pos ��ǥ �޾�´� 
								// Find �˻� ������ ������ �޾ƿ� 

								if(RqMode == true)
									Find->SetAngleBias(150.0f);
								else
									Find->SetAngleBias(135.0f);

								if(Find->OnExecute()){
									AGrindMark[1].Point.x = Find->Result[0].Px;
									AGrindMark[1].Point.y = Find->Result[0].Py;
									AGrindMark[1].Score   = Find->Result[0].Score;
								}
								// Contrast Gradient Pixel Value
// 								GrindMarkGradient[20] = EasyImage::Focusing(Find->IRoi);
							}
						}
					}
					// Find Mark Search Start
					////////////////////////////////////////////////////////////////////////////

					// SoulBrain RightTop
					if(FInspectMode==Insp_Polygon){
						TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);

						// 									 P1.x = (int)(FRTPoint.x-ASideCircleWidth-30);    P1.y =(int)(FRTPoint.y-20            );    P2.x = (int)(FRTPoint.x+20);    P2.y = (int)(P1.y);
						// 									 P3.x = (int)(P1.x)                          ;    P3.y =(int)(P1.y+ASideCircleHeight+50);    P4.x = (int)(P2.x         );    P4.y = (int)(P3.y);
						P1.x = (int)(FRTPoint.x-ASideCircleWidth   );    P1.y =(int)(FRTPoint.y-20            );    P2.x = (int)(FRTPoint.x+20);    P2.y = (int)(P1.y);
						P3.x = (int)(P1.x)                          ;    P3.y =(int)(P1.y+ASideCircleHeight+20);    P4.x = (int)(P2.x         );    P4.y = (int)(P3.y);

						// �˻�ȭ�� Display ���� 
						PadPolyRect[1].left  = P1.x;   PadPolyRect[1].top    = P1.y;
						PadPolyRect[1].right = P2.x;   PadPolyRect[1].bottom = P3.y;

						int PixelCnt =0;

						// Median Filter ���� 
						if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
						if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
						FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
						FilterROI.Attach(&RotateBinaryImg)                                    ;
						FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
						LogUnit.SetLog(L"RightTop ==>Px :%d, Py :%d, Width:%d, Height:%d ==>Median Filter",P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
						LogUnit.SetLog(L"RightTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
						EasyImage::Median(&FilterROI, &FilterROI)                             ;
						// 									 RotateBinaryImg.Save("D:\\ROTATE_IMG\\Polygon_RightTop_����.bmp");

						Polygon->GImage = &RotateBinaryImg                          ;
						Polygon->IWidth = ImgW                                      ;
						Polygon->IHeight= ImgH                                      ;
						Polygon->PoPa.FMinWidth  =(int)(ASideCircleWidth/2)         ;                       
						// 									 Polygon->PoPa.FMaxWidth  =(int)((FRTPoint.x - FLTPoint.x)/2);
						Polygon->PoPa.FMaxWidth  =(int)(ASideCircleWidth+100)       ;
						Polygon->PoPa.FMinHeight =(int)(ASideCircleHeight/2)        ;
						// 									 Polygon->PoPa.FMaxHeight =1000                              ;
						Polygon->PoPa.FMaxHeight =(int)(ASideCircleHeight+100)      ;
						Polygon->PoPa.MinPxCnt   =100                               ;
						Polygon->PoPa.MaxPxCnt   =20000                             ;
						Polygon->PoPa.MaxBbCnt   =10                                ;
						Polygon->PoPa.IsSeparate =true                              ;
						Polygon->PoPa.IsInToOut  =false                             ;
						Polygon->PoPa.HiThHold   = ECThresHold                      ;
						Polygon->PoPa.LoThHold   = 0                                ;

						if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
							Polygon->OnExecute(P1,P2,P3,P4);
						}

						// Polygon Algorithm InEdgeData/OutEdgeData Separate 
						if(Polygon->BlobCount==1){
							PolygonMarginCnt = (IsMarkBtm)? 5:20;
							if(Polygon->Blob[0].InCnt>(PolygonMarginCnt*2)){
								A2CirInEdgeCount=0;
								A2CirInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt*2));
								if(A2CirInEdgeCount>0){
									if(A2CirInPoint!=NULL) delete[] A2CirInPoint;
									A2CirInPoint = new TFPoint[A2CirInEdgeCount];

									PixelCnt=0;
									for(int k=0; k<Polygon->Blob[0].InCnt; k++){
										if(k<PolygonMarginCnt || k>=(Polygon->Blob[0].InCnt-PolygonMarginCnt)) continue;
										A2CirInPoint[PixelCnt].x = Polygon->Blob[0].InPoint[k].x ;
										A2CirInPoint[PixelCnt].y = Polygon->Blob[0].InPoint[k].y ;
										PixelCnt++;
									}
								}
							}

							if(Polygon->Blob[0].OutCnt>(PolygonMarginCnt*2)){
								A2CirOutEdgeCount=0;
								A2CirOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt*2));
								if(A2CirOutEdgeCount>0){
									if(A2CirOutPoint!=NULL) delete[] A2CirOutPoint;
									A2CirOutPoint = new TFPoint[A2CirOutEdgeCount];

									PixelCnt=0;
									for(int k=0; k<Polygon->Blob[0].OutCnt; k++){
										if(k<PolygonMarginCnt || k>=(Polygon->Blob[0].OutCnt-PolygonMarginCnt)) continue;
										A2CirOutPoint[PixelCnt].x = Polygon->Blob[0].OutPoint[k].x ;
										A2CirOutPoint[PixelCnt].y = Polygon->Blob[0].OutPoint[k].y ;
										PixelCnt++;
									}
								}
							}
						}


						// InEdgeData/OutEdgeData ���� ������ �� ������ ���
						if(IsMarkBtm){ // LTPS ������,������ ��� 
							if(A2CirOutEdgeCount!=0 && A2CirInEdgeCount!=0){ // MarkMode��� ������ �˻縦 ���� ���� �� ������ ����
								////////////////////////////////////////////////////////////////////////////////////////////////////
								if(AGrindMark[1].Point.x!=0.0f && AGrindMark[1].Point.y!=0.0f){  //GrindMark ã������� 
									// GrindMark���� OutData �ִܰŸ� Count���� Calc 
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
									for(int k=0; k<A2CirOutEdgeCount; k++){
										if(A2CirOutPoint[k].x  ==0.0f  || A2CirOutPoint[k].y   ==0.0f) continue;
										if(AGrindMark[1].Point.x==0.0f || AGrindMark[1].Point.y==0.0f) continue;

										GrindMeaDis = Math.GetLength(AGrindMark[1].Point, A2CirOutPoint[k]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkOutCnt = k;
										}
									}
									// GrindMark���� InData �ִܰŸ� Count���� Calc 
// 									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
// 									for(int k=0; k<A2CirInEdgeCount; k++){
// 										if(A2CirInPoint[k].x   ==0.0f  || A2CirInPoint[k].y     ==0.0f) continue;
// 										if(AGrindMark[1].Point.x==0.0f || AGrindMark[1].Point.y ==0.0f) continue;
// 
// 										GrindMeaDis = Math.GetLength(AGrindMark[1].Point, A2CirInPoint[k]);
// 										if(GrindMeaDis<MinDis){
// 											MinDis = GrindMeaDis;
// 											GrindMarkInCnt = k;
// 										}
// 									}

									if(GrindMarkOutCnt!=0){
										// GrindMark���� InData �ִܰŸ� Count���� Calc 
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
										for(int k=0; k<A2CirInEdgeCount; k++){
											if(A2CirInPoint[k].x               ==0.0f || A2CirInPoint[k].y                ==0.0f) continue;
											if(A2CirOutPoint[GrindMarkOutCnt].x==0.0f || A2CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

											GrindMeaDis = Math.GetLength(A2CirOutPoint[GrindMarkOutCnt], A2CirInPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkInCnt = k;
											}
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
									if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5 &&(GrindMarkOutCnt+5<A2CirOutEdgeCount)){
										CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
										for(int k=GrindMarkOutCnt-5; k<GrindMarkOutCnt+5; k++){
											if(A2CirOutPoint[k].x==0.0f || A2CirOutPoint[k].y==0.0f) continue;

											GrindMeaDis = Math.GetLength(AGrindMark[1].Point, A2CirOutPoint[k]);
											TotalCirDis+= GrindMeaDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											AGrindMark[1].GrindMea = TotalCirDis/CircleCount;
											AGrindMark[1].GrindMea = AGrindMark[1].GrindMea+0.5; //SubPixel 
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
									if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
										CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
										for(int k=-5; k<5; k++){
											if(A2CirOutPoint[GrindMarkOutCnt+k].x==0.0f && A2CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
											if(A2CirInPoint [GrindMarkInCnt +k].x==0.0f && A2CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
											GrindWidthDis = Math.GetLength(A2CirInPoint[GrindMarkInCnt+k], A2CirOutPoint[GrindMarkOutCnt+k]);
											TotalCirDis+=   GrindWidthDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											AGrindMark[1].GrindWidth = TotalCirDis/CircleCount     ;
											AGrindMark[1].GrindWidth = AGrindMark[1].GrindWidth+0.5; //SubPixel
										}
									}
								}
								else if(AGrindMark[1].Rect.left!=0 && AGrindMark[1].Rect.top!=0 && AGrindMark[1].Rect.right!=0 && AGrindMark[1].Rect.bottom!=0){  //GrindMark �� ã�������
									// OutData���� �ּҰ� ������ Count���� 
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
									for(int k=0; k<A2CirOutEdgeCount; k++){
										if(A2CirOutPoint[k].x==0.0f || A2CirOutPoint[k].y==0.0f) continue;

										AGrindMarkRectPoint2.x = AGrindMark[1].Rect.left + (AGrindMark[1].Rect.right  - AGrindMark[1].Rect.left)/2; 
										AGrindMarkRectPoint2.y = AGrindMark[1].Rect.top  + (AGrindMark[1].Rect.bottom - AGrindMark[1].Rect.top )/2; 

										GrindMeaDis = Math.GetLength(AGrindMarkRectPoint2, A2CirOutPoint[k]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkOutCnt = k;
										}
									}

									// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
									if(GrindMarkOutCnt!=0){
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
										for(int k=0; k<A2CirInEdgeCount; k++){
											if(A2CirInPoint[k].x   ==0.0f             || A2CirInPoint[k].y                ==0.0f) continue;
											if(A2CirOutPoint[GrindMarkOutCnt].x==0.0f || A2CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

											GrindMeaDis = Math.GetLength(A2CirOutPoint[GrindMarkOutCnt], A2CirInPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkInCnt = k;
											}
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
									if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
										CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
										for(int k=-5; k<5; k++){
											if(A2CirOutPoint[GrindMarkOutCnt+k].x==0.0f && A2CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
											if(A2CirInPoint [GrindMarkInCnt +k].x==0.0f && A2CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
											GrindWidthDis = Math.GetLength(A2CirInPoint[GrindMarkInCnt+k], A2CirOutPoint[GrindMarkOutCnt+k]);
											TotalCirDis+=   GrindWidthDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											AGrindMark[1].GrindWidth = TotalCirDis/CircleCount     ;
											AGrindMark[1].GrindWidth = AGrindMark[1].GrindWidth+0.5; //SubPixel
										}
									}
								}
							}
						}
						else         { //Polygon EnCap ���� OutPoint �߰��� ���� ���� �Ÿ� ��հ��� ����Ѵ�(RightTop)
							if(A2CirInEdgeCount!=0 &&A2CirOutEdgeCount!=0){
								GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
								GrindMarkOutCnt = (int)(A2CirOutEdgeCount/2);
								// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
								if(GrindMarkOutCnt!=0){
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
									for(int k=0; k<A2CirInEdgeCount; k++){
										if(A2CirInPoint[k].x   ==0.0f             || A2CirInPoint[k].y                ==0.0f) continue;
										if(A2CirOutPoint[GrindMarkOutCnt].x==0.0f || A2CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

										GrindMeaDis = Math.GetLength(A2CirOutPoint[GrindMarkOutCnt], A2CirInPoint[k]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkInCnt = k;
										}
									}
								}

								// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
								if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
									CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
									for(int k=-5; k<5; k++){
										if(A2CirOutPoint[GrindMarkOutCnt+k].x==0.0f && A2CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
										if(A2CirInPoint [GrindMarkInCnt +k].x==0.0f && A2CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
										GrindWidthDis = Math.GetLength(A2CirInPoint[GrindMarkInCnt+k], A2CirOutPoint[GrindMarkOutCnt+k]);
										TotalCirDis+=   GrindWidthDis;
										CircleCount++;
									}
									if(TotalCirDis!=0.0f && CircleCount!=0){
										AGrindMark[1].GrindWidth = TotalCirDis/CircleCount     ;
										AGrindMark[1].GrindWidth = AGrindMark[1].GrindWidth+0.5; //SubPixel
									}
								}
							}
						}
						// FAVION Circle Fitting �����(RightTop)
						SampleSize = A2CirInEdgeCount*0.75; //0.5~0.8���Ϸ� �Է��Ұ�
						DistDevRatio = 0.15; //0.1~0.25 ���Ϸ� �Է��Ұ�
						CircleFit->OnRansac(A2CirInPoint,A2CirInEdgeCount,SampleSize,DistDevRatio,&ACircleFit2Center,&ACirFitDiameter2);

						// Polygon �ڸ� �˻� ������(RightTop) 
						if(!IsMarkBtm && BrokenThresHold!=0){
							Edge->GImage = &RotateBrokenImg;
							Edge->SetEdgeType(0);

							int DvCnt =0, DataCount=0;
							double deg =0.0f; 
							TFPoint Target(0.0f, 0.0f);

							if(GrindMarkInCnt>GrindMarkOutCnt){
								DvCnt = GrindMarkInCnt - GrindMarkOutCnt;
								for(int k=0; k<A2CirOutEdgeCount; k++){
									deg =0.0f, Target.x =0.0f, Target.y =0.0f;
									if(((k+DvCnt)<A2CirInEdgeCount) && A2CirInPoint[k+DvCnt].x>0  && A2CirInPoint[k+DvCnt].y>0){
										deg = Math.GetDegree(A2CirOutPoint[k],A2CirInPoint[k+DvCnt]);

										Target  = Math.OnFindPerpendicularPointXY(A2CirOutPoint[k],-dEdgeToTrapLineDis,dEdgeToTrapLineDis,deg);
										Edge->OnExecute(A2CirOutPoint[k],Target); // �����ܰ����ο��� Target���� White Point Search 
										if(Edge->GetResultEdgePointCount()>0){
											A2CircleData[DataCount].Dis =0.0f;
											A2CircleData[DataCount].Dv  =0.0f;
											A2CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
											A2CircleData[DataCount].P.y = A2CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
											DataCount++;
										}
									}
								}
								A2CirInCrackCount = DataCount;
							}
							else if(GrindMarkInCnt<GrindMarkOutCnt)   {
								DvCnt = GrindMarkOutCnt - GrindMarkInCnt;
								for(int k=DvCnt; k<A2CirOutEdgeCount; k++){
									deg =0.0f, Target.x =0.0f, Target.y =0.0f;
									if(((k-DvCnt)<A2CirInEdgeCount) && A2CirInPoint[k-DvCnt].x>0  && A2CirInPoint[k-DvCnt].y>0 && k-DvCnt>=0){
										deg = Math.GetDegree(A2CirOutPoint[k],A2CirInPoint[k-DvCnt]);

										Target  = Math.OnFindPerpendicularPointXY(A2CirOutPoint[k],-dEdgeToTrapLineDis,dEdgeToTrapLineDis,deg);
										Edge->OnExecute(A2CirOutPoint[k],Target); // �����ܰ����ο��� Target���� White Point Search 
										if(Edge->GetResultEdgePointCount()>0){
											A2CircleData[DataCount].Dis =0.0f;
											A2CircleData[DataCount].Dv  =0.0f;
											A2CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
											A2CircleData[DataCount].P.y = A2CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
											DataCount++;
										}
									}
								}
								A2CirInCrackCount = DataCount;
							}
						}
					}
					else if(FInspectMode==Insp_Mark){ // RightTop
						// ������ �ܰ����� ������ ���� CirCenter ������ 
						if(ASideCircleWidth!=0.0f && ASideCircleHeight!=0.0f)  A2CirCenter.x = FRTPoint.x - ASideCircleWidth;  A2CirCenter.y = FRTPoint.y + ASideCircleHeight;  

						// Circle Fitting ����� �������� ����ϴ� ������
						Circle->SetSamplingStep(1)                              ; // A2Circle Edge ���� 

						//A1 In Circle EdgePoint Data ���� 
						if(A2CirCenter.x!=0 && A2CirCenter.y!=0){
							TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
							P1.x = (int)(FRTPoint.x-ASideCircleWidth   );    P1.y =(int)(FRTPoint.y-20            );    P2.x = (int)(FRTPoint.x+20);    P2.y = (int)(P1.y);
							P3.x = (int)(P1.x)                          ;    P3.y =(int)(P1.y+ASideCircleHeight+20);    P4.x = (int)(P2.x         );    P4.y = (int)(P3.y);
							// Median Filter ���� 
							if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
							if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
							FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
							FilterROI.Attach(&RotateBinaryImg)                                    ;
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"Px : %d, Py : %d, Width : %d, Height : %d => MedianFilter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"P1.x : %d, P1.y : %d, P2.x : %d, P2.y : %d, P3.x : %d, P3.y : %d, P4.x : %d, P4.y : %d => MedianFilter", P1.x, P1.y, P2.x, P2.y, P3.x, P3.y, P4.x, P4.y);
							EasyImage::Median(&FilterROI, &FilterROI)                             ;
							// 										 RotateBinaryImg.Save("D:\\ROTATE_IMG\\RightTop_����.bmp");


							if(ECThresHold==0) Circle->GImage = &RotateImg      ; //Org Image �Ѱ��� 
							else               Circle->GImage = &RotateBinaryImg; //Binary Image �Ѱ��� 

							Circle->SetCenterXY(A2CirCenter.x , A2CirCenter.y)  ; // Circle Center ��ǥ���� 
							// 									 A2Diameter = ((ASideCircleWidth*2)+((ASideCircleWidth-OutToTrackLineDis)*2))/2;
							A2Diameter = (ASideCircleWidth*2);

							Circle->SetDiameter(A2Diameter)             ;
							Circle->SetTolerance(60.0f )                ; // A1Circle Tolerance 
							Circle->SetCircleChoice(1)                  ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
							Circle->SetCircleIndex (1)                  ; // Circle EdgePoint Index 
							Circle->SetAngle(270+CirAngleMargin)        ; // Cirlce Angle 
							Circle->SetAmplitude(90-(2*CirAngleMargin)) ; // Circle Amplitude

							// Circle InData EdgePoint 
							//////////////////////////////////////////////////////////////////////////////////////////////////////
							Circle->SetCircleType(2)                    ; // 0:Black to White, 1:White to Black 2: Both Black, White 
							Circle->OnExecute()                         ;

							// Circle In Data ���� ���ο� Circle Center ��ǥ �޾ƿ� 
							// Euresys Circle Fitting Center 
							RealA2CirCenter.x = Circle->FCP.x;
							RealA2CirCenter.y = Circle->FCP.y;
							ACirDiameter2     = (Circle->Diameter)/2.0;

							// Circle In Data���� 
							OrgA2CirInEdgeCount = Circle->FCirclePointCount;
							if(OrgA2CirInEdgeCount>0){
								if(OrgA2CirInPoint!=NULL) delete[] OrgA2CirInPoint;
								OrgA2CirInPoint = new TFPoint[OrgA2CirInEdgeCount];

								for(int k=0; k<OrgA2CirInEdgeCount; k++){
									if(k>MAX_CIRCLE_EDGEPOINT) break;
									OrgA2CirInPoint[k].x = Circle->FCirclePoint[k].x;
									OrgA2CirInPoint[k].y = Circle->FCirclePoint[k].y;
									if(OrgA2CirInPoint[k].x==0.0f && OrgA2CirInPoint[k].y==0.0f) A2CirInEdgeCount++;
								}
							}
							//////////////////////////////////////////////////////////////////////////////////////////////////////

							// Circle Out Data �ٽ� �˻�  
							//////////////////////////////////////////////////////////////////////////////////////////////////////
							Circle->SetTolerance(60.0f )      ; // Circle Tolerance 
							Circle->SetDiameter (A2Diameter)  ; // Circle Diameter 
							Circle->SetCircleChoice(1)        ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
							Circle->SetCircleType(2)          ; // 0:Black to White, 1:White to Black 2: Both Black, White 
							Circle->SetCircleIndex (0)        ; // Circle EdgePoint Index 

							Circle->OnExecute()               ;

							// Circle Out Data���� 
							OrgA2CirOutEdgeCount = Circle->FCirclePointCount;
							if(OrgA2CirOutEdgeCount>0){
								if(OrgA2CirOutPoint!=NULL) delete[] OrgA2CirOutPoint;
								OrgA2CirOutPoint = new TFPoint[OrgA2CirOutEdgeCount];

								for(int k=0; k<OrgA2CirOutEdgeCount; k++){
									if(k>MAX_CIRCLE_EDGEPOINT) break;
									OrgA2CirOutPoint[k].x = Circle->FCirclePoint[k].x;
									OrgA2CirOutPoint[k].y = Circle->FCirclePoint[k].y;
									// BTM���� Circle�� ������ ���� Defect�˻� ������ ��ǥ��� 
									if(k==0                     ) {TopEdgePoint[2].x = OrgA2CirOutPoint[k].x; TopEdgePoint[2].y = OrgA2CirOutPoint[k].y;}
									if(k==OrgA2CirOutEdgeCount-1) {TopEdgePoint[3].x = OrgA2CirOutPoint[k].x; TopEdgePoint[3].y = OrgA2CirOutPoint[k].y;}
									if(OrgA2CirOutPoint[k].x==0.0f && OrgA2CirOutPoint[k].y==0.0f) A2CirOutEdgeCount++;
								}
							}
							// A2Circle Grind_Width Calculate (������ ���)
							if(IsMarkBtm){ // LTPS��� ������ ��� ==> ������ ��� 
								if(OrgA2CirOutEdgeCount!=0 && OrgA2CirInEdgeCount!=0){// MarkMode��� ������ �˻縦 ���� ���� �� ������ ����  
									GrindMeaDis =0.0f, MinDis =10000.0f; A2GrindMarkCount=0;
									if(AGrindMark[1].Point.x!=0.0f && AGrindMark[1].Point.y!=0.0f){  // GrindMark ã������� 
										for(int k=0; k<OrgA2CirOutEdgeCount; k++){
											if(OrgA2CirOutPoint[k].x ==0.0f || OrgA2CirOutPoint[k].y ==0.0f) continue;
											if(AGrindMark[1].Point.x ==0.0f || AGrindMark[1].Point.y ==0.0f) continue;

											GrindMeaDis = Math.GetLength(AGrindMark[1].Point, OrgA2CirOutPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												A2GrindMarkCount = k;
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ���� 
										if(A2GrindMarkCount!=0 && A2GrindMarkCount>5){
											CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
											for(int k=A2GrindMarkCount-5; k<A2GrindMarkCount+5; k++){
												if(OrgA2CirOutPoint[k].x==0.0f || OrgA2CirOutPoint[k].y==0.0f) continue;

												GrindMeaDis = Math.GetLength(AGrindMark[1].Point, OrgA2CirOutPoint[k]);
												TotalCirDis+= GrindMeaDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												AGrindMark[1].GrindMea = TotalCirDis/CircleCount;
											}
										}

										// GrindMark���� ���� ��հ� ������ ���� 
										if(A2GrindMarkCount!=0 && A2GrindMarkCount>5){
											CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
											for(int k=A2GrindMarkCount-5; k<A2GrindMarkCount+5; k++){
												if(OrgA2CirOutPoint[k].x==0.0f && OrgA2CirOutPoint[k].y==0.0f) continue;
												if(OrgA2CirInPoint [k].x==0.0f && OrgA2CirInPoint [k].y==0.0f) continue;
												GrindWidthDis = Math.GetLength(OrgA2CirInPoint[k], OrgA2CirOutPoint[k]);
												TotalCirDis+=   GrindWidthDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												AGrindMark[1].GrindWidth = TotalCirDis/CircleCount;
											}
										}
									}
									else { // GrindMark �� ã���� ��� 
										for(int k=0; k<OrgA2CirOutEdgeCount; k++){
											if(OrgA2CirOutPoint[k].x==0.0f || OrgA2CirOutPoint[k].y==0.0f) continue;

											AGrindMarkRectPoint2.x = AGrindMark[1].Rect.left + (AGrindMark[1].Rect.right  - AGrindMark[1].Rect.left)/2; 
											AGrindMarkRectPoint2.y = AGrindMark[1].Rect.top  + (AGrindMark[1].Rect.bottom - AGrindMark[1].Rect.top )/2; 

											GrindMeaDis = Math.GetLength(AGrindMarkRectPoint2, OrgA2CirOutPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												A2GrindMarkCount = k;
											}
										}
										// GrindMark���� ���� ��հ� ������ ���� 
										if(A2GrindMarkCount!=0 && A2GrindMarkCount>5 && (A2GrindMarkCount+5<OrgA2CirOutEdgeCount)){
											CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
											for(int k=A2GrindMarkCount-5; k<A2GrindMarkCount+5; k++){
												if(OrgA2CirOutPoint[k].x==0.0f && OrgA2CirOutPoint[k].y==0.0f) continue;
												if(OrgA2CirInPoint [k].x==0.0f && OrgA2CirInPoint [k].y==0.0f) continue;
												GrindWidthDis = Math.GetLength(OrgA2CirInPoint[k], OrgA2CirOutPoint[k]);
												TotalCirDis+=   GrindWidthDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												AGrindMark[1].GrindWidth = TotalCirDis/CircleCount;
											}
										}
									}
								}
							}
							else     { // MarkMode ==> EnCap��� �������� ����Ѵ� 
								if(OrgA2CirInEdgeCount==OrgA2CirOutEdgeCount){
									double OutDis=0.0f, InDis=0.0f, TotalGrindWidth=0.0f, AvgGrindWidth=0.0f;A2GrindWidthCount=0;
									for(int k=0; k<OrgA2CirInEdgeCount; k++){
										if(OrgA2CirOutPoint[k].x==0.0f && OrgA2CirOutPoint[k].y==0.0f){
											OutDis =0.0f;
										}
										else {
											OutDis = Math.GetLengthFrPointToPoint(A2CirCenter,OrgA2CirOutPoint[k]);
										}

										if(OrgA2CirInPoint[k].x==0.0f && OrgA2CirInPoint[k].y==0.0f){
											InDis =0.0f;
										}
										else {
											InDis = Math.GetLengthFrPointToPoint(A2CirCenter,OrgA2CirInPoint[k]);
										}

										// CircleFitting ����� ���� EdgePoint �����Ͱ� ���� �� ��츸 �����Ͽ� ����Ѵ�
										if(OutDis!=0.0f && InDis!=0.0f){
											TotalGrindWidth+=(OutDis - InDis);
											A2GrindWidthCount++;
										}
									}
									if(A2GrindWidthCount!=0){
										AvgGrindWidth = (TotalGrindWidth/A2GrindWidthCount);
										AGrindMark[1].GrindWidth = AvgGrindWidth;
									}
								}
							}

							//////////////////////////////////////////////////////////////////////////////////////////////////////
							// Org Circle�������� ��ȣ���� ���� ���� ������ ���� �����ϴ� ������ �������� ���� �޸� ������
							// Circle Defect�˻翡 �ʿ��� Edge������ ��
							int Count=0;
							if(OrgA2CirInEdgeCount>0){ // A2 CircleInData
								A2CirInEdgeCount = OrgA2CirInEdgeCount - A2CirInEdgeCount;
								if(A2CirInEdgeCount>0){
									if(A2CirInPoint!=NULL) delete[] A2CirInPoint;
									A2CirInPoint = new TFPoint[A2CirInEdgeCount];

									for(int k=0; k<OrgA2CirInEdgeCount; k++){
										if(k>MAX_CIRCLE_EDGEPOINT) break;
										if(OrgA2CirInPoint[k].x==0.0f && OrgA2CirInPoint[k].y==0.0f) continue;
										A2CirInPoint[Count].x = OrgA2CirInPoint[k].x;
										A2CirInPoint[Count].y = OrgA2CirInPoint[k].y;
										Count++;
									}
								}
								// Circle ���� ������ �޸� ����
								if(OrgA2CirInPoint!=NULL) delete[] OrgA2CirInPoint;
							}
							// FAVION Circle Fitting ����� 
							SampleSize = A2CirInEdgeCount*0.75; //0.5~0.8���Ϸ� �Է��Ұ�
							DistDevRatio = 0.15; //0.1~0.25 ���Ϸ� �Է��Ұ�
							CircleFit->OnRansac(A2CirInPoint,A2CirInEdgeCount,SampleSize,DistDevRatio,&ACircleFit2Center,&ACirFitDiameter2);

							if(OrgA2CirOutEdgeCount>0){// A1 CircleOutData 
								A2CirOutEdgeCount = OrgA2CirOutEdgeCount - A2CirOutEdgeCount;
								Count =0;
								if(A2CirOutEdgeCount>0){
									if(A2CirOutPoint!=NULL) delete[] A2CirOutPoint;
									A2CirOutPoint = new TFPoint[A2CirOutEdgeCount];

									for(int k=0; k<OrgA2CirOutEdgeCount; k++){
										if(k>MAX_CIRCLE_EDGEPOINT) break;
										if(OrgA2CirOutPoint[k].x==0.0f && OrgA2CirOutPoint[k].y==0.0f) continue;
										A2CirOutPoint[Count].x = OrgA2CirOutPoint[k].x;
										A2CirOutPoint[Count].y = OrgA2CirOutPoint[k].y;
										Count++;
									}
								}
								// Circle ���� ������ �޸� ����
								if(OrgA2CirOutPoint!=NULL) delete[] OrgA2CirOutPoint;
							}
							// Circle ���� �ɹڸ�, �����ڸ� ���� ���� ������(EnCap)
							// ���� ��Ī�� ����.....�ϴ� Crack���� ���� 
							if(!IsMarkBtm && BrokenThresHold!=0){
								if(ACirFitDiameter2!=0.0f && ACircleFit2Center.x!=0.0f && ACircleFit2Center.y!=0.0f){ // FAVION Circle Fitting ������ �ڸ��˻� ���� 
									Circle->GImage = &RotateBrokenImg;                                             //Binary Image �Ѱ��� 

									Circle->SetCenterXY(ACircleFit2Center.x,ACircleFit2Center.y);
									// 											 Circle->SetTolerance(CirCrackMargin)                      ; // Circle Tolerance 
									// 											 Circle->SetDiameter ((ACirFitDiameter2-CirCrackMargin)*2) ; // Circle Diameter 
									Circle->SetTolerance((dEdgeToTrapLineDis/2)-CirOffsetMargin); // Circle Tolerance 
									Circle->SetDiameter ((ACirFitDiameter2-(dEdgeToTrapLineDis/2))*2) ; // Circle Diameter 
									Circle->SetCircleChoice(0)                                ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
									Circle->SetCircleType  (0)                                ; // 0:Black to White, 1:White to Black 2: Both Black, White 
									Circle->SetCircleIndex (0)                                ; // Circle EdgePoint Index 
									Circle->SetAngle(325)                                     ; // �������� ���ؼ� ����ų �߻�...������ �����ش� 
									Circle->SetAmplitude(25)                                  ; // �������� ���ؼ� ����ų �߻�...������ �����ش� 
									Circle->OnExecute(true)                                   ;
									CrackACirDiameter2 = (ACirFitDiameter2-(dEdgeToTrapLineDis/2))+((dEdgeToTrapLineDis/2)-CirOffsetMargin);

									A2CirInCrackCount = Circle->FCirclePointCount;

									if(A2CirInCrackCount>0){
										if(A2CirInCrackPoint!=NULL) delete[] A2CirInCrackPoint;
										A2CirInCrackPoint = new TFPoint[A2CirInCrackCount];

										for(int k=0; k<A2CirInCrackCount; k++){
											if(k>MAX_CIRCLE_EDGEPOINT) break;
											A2CirInCrackPoint[k].x = Circle->FCirclePoint[k].x;
											A2CirInCrackPoint[k].y = Circle->FCirclePoint[k].y;
										}
									}
								}
							}
						}
						LogUnit.SetLog(L"RightTop_Mark_END");     
					}
				}
				LogUnit.SetLog(L"shRectH_Rotate_RightTop_End");     
			}
		}

		if(FShape==shRectC ){ // SoulBrain Notch Image Rotate  
			// Circle �˻翡 �ʿ��� ���ú��� �ʱ�ȭ 
			const int GrindMarkRange =40;    const int GrindMarkLength=80;
			int OrgC1CirInEdgeCount=0;		  int OrgC1CirOutEdgeCount=0;	    int OrgC1CirStandEdgeCount=0;
			int OrgC2CirInEdgeCount=0;		  int OrgC2CirOutEdgeCount=0;	    int OrgC2CirStandEdgeCount=0;

			int OrgBtm1InEdgeCount =0;		  
			int OrgBtm1OutEdgeCount=0;		  
			int Btm1GrindMarkCount =0;        

			int C1GrindWidthCount=0;		 int C1GrindMeasureCount=0;
			int C2GrindWidthCount=0;		 int C2GrindMeasureCount=0;

			int C1GrindMarkCount =0;         int C2GrindMarkCount =0;

			// Circle Diameter 
			int C1Diameter       =0;         int C2Diameter         =0; 

			TFPoint *OrgC1CirInPoint=NULL;	  TFPoint *OrgC1CirOutPoint=NULL;	TFPoint *OrgC1CirStandPoint=NULL;
			TFPoint *OrgC2CirInPoint=NULL;	  TFPoint *OrgC2CirOutPoint=NULL;	TFPoint *OrgC2CirStandPoint=NULL;

			// Notch �������� ������ ���տ� �ʿ��� 
			TFPoint *OrgBtm1InPoint =NULL;	  
			TFPoint *OrgBtm1OutPoint=NULL;	  

			TFPoint PreC1CirStandPoint[1];    TFPoint PreC2CirStandPoint[1];	  

			// Polygon Value 
			int PixelCnt =0;
			if(FInspectMode==Insp_Polygon){
				for(int k=0; k<2; k++){
					NotchPolyRect[k].left   =0;
					NotchPolyRect[k].top    =0;
					NotchPolyRect[k].right  =0;
					NotchPolyRect[k].bottom =0;
				}
			}

			// Image Roate �� �����̹��� ���� ������ ���� 
			FLBPoint = Math.OnIntersect(Line[0].FP0 , Line[0].FP1 , Line[3].FP0, Line[3].FP1);
			FRBPoint = Math.OnIntersect(Line[2].FP0 , Line[2].FP1 , Line[3].FP0, Line[3].FP1);

			if(FLBPoint.x!=0 && FLBPoint.y!=0 && FRBPoint.x!=0 && FRBPoint.y!=0) ImgDegree = -(Math.GetDegree(FLBPoint.x , FLBPoint.y, FRBPoint.x, FRBPoint.y));

			// Left_Mark�� Right_Mark������ ��Ȯ�� 0���ΰ�� �߻��Ͽ� ������ �ٲ�
			// 						 if(ImgDegree!=0.0f){ // �����̹��� ������ ���� AlignƲ��������� �̹��� ȸ�� 
			if(FLBPoint.x!=0 && FLBPoint.y!=0 && FRBPoint.x!=0 && FRBPoint.y!=0){ // �����̹��� ������ ���� AlignƲ��������� �̹��� ȸ�� 
				// ���� RotateImg Size ���� �� ���� 
				LogUnit.SetLog(L"shRectC_RotateImg_Start");     
				RotateImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&RotateImg);

				// ���� �����Ͽ� �̹��� ȸ�� 
				EasyImage::ScaleRotate(GImage, (float)ImgW/2, (float)ImgH/2, (float)ImgW/2, (float)ImgH/2, 1.0f, 1.0f, ImgDegree, &RotateImg, 4);
				CenterPoint.x = ImgW/2;
				CenterPoint.y = ImgH/2;
// 				RotateImg.Save("D:\\ROTATE_IMG\\SoulBrain_BTM.bmp");

				// Binary RotateImg Size ���� �� ���� 
				RotateBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBinaryImg);
				// 							 RotateBinaryImg.Save("D:\\ROTATE_IMG\\RotateImg_CUP_����.bmp");

				// Circle Grind In EdgePoint ������� �̹��� 
				RotateGrindInBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateGrindInBinaryImg);

				// Grind Measure Corner, Circle �˻� ���� 
				RotateGrindBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateGrindBinaryImg);
				//							 RotateImg.Save("D:\\ROTATE_IMG\\RotateImgȸ��_CUP.bmp");
				LogUnit.SetLog(L"shRectC_RotateImg_End");    

				if(FLeftWall){ // shRectC Image Rotate Sequence  
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftEdgeThresHold);
					if(LeftEdgeThresHold==0) {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
					else                     {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()          );
					SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()          );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth()         );

					// ChamberCut VS Circle VS Rect��쿡 ���� Left���� Fitting���� ���̰� �޶��� 
					if     (IsCornerBtm) SingleLine->IRoi->SetHeight(FEndLine-LBCornerHeight   );
					else if(IsCircleBtm) SingleLine->IRoi->SetHeight(FEndLine-CSideCircleHeight);
					else                 SingleLine->IRoi->SetHeight(IRoi->GetHeight()         ); 

					SingleLine->SetIncrement(5 );
					SingleLine->SetOrientation(orWest);
					SingleLine->SetFindLineId(0);
					SingleLine->SetEdgeType(1); // White to Black  
					SingleLine->OnExecute();

					Line[0].FP0 = SingleLine->GetResultP0();
					Line[0].FP1 = SingleLine->GetResultP1();

					if((fabs(Line[0].FP0.x - Line[0].FP1.x)>2) && (Line[0].FP1.x >Line[0].FP0.x)) Line[0].FP1.x = Line[0].FP0.x;

					// Btm,Top ������� ù��°
					LeftOutLine[0].FP0.x = Line[0].FP0.x;
					LeftOutLine[0].FP0.y = Line[0].FP0.y;
					LeftOutLine[0].FP1.x = Line[0].FP1.x;
					LeftOutLine[0].FP1.y = Line[0].FP1.y;

					if(fabs(LeftOutLine[0].FP0.x - LeftOutLine[0].FP1.x)>=0.5){
						LeftOutLine[0].FP0.x = (LeftOutLine[0].FP0.x+LeftOutLine[0].FP1.x)/2.0;
						LeftOutLine[0].FP1.x = (LeftOutLine[0].FP0.x+LeftOutLine[0].FP1.x)/2.0;
					}
					// LeftOutLine....make OutLine Data 
					// ������ �����͸� �Ѱ��ش� 
					// �˻��� ���ض����� ã�� ���Ұ�� ���������� �Ѱ��༭ ���ض��� ������ 
					if(LeftOutLine[0].FP0.x!=0.0f && LeftOutLine[0].FP0.y!=0.0f && LeftOutLine[0].FP1.x!=0.0f && LeftOutLine[0].FP1.y!=0.0f){
						PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
						PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
					} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
					else if(LeftOutLine[0].FP0.x==0.0f || LeftOutLine[0].FP0.y==0.0f || LeftOutLine[0].FP1.x==0.0f || LeftOutLine[0].FP1.y==0.0f){
						if(PreLeftOutLine[0].FP0.x==0.0f && PreLeftOutLine[0].FP0.y==0.0f && PreLeftOutLine[0].FP1.x==0.0f && PreLeftOutLine[0].FP1.y==0.0f){// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
							// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
							ROI_H = IRoi->GetHeight();
							LeftOutLine[0].FP0.x = Line[0].FP0.x;    LeftOutLine[0].FP0.y = 0.0f        ;
							LeftOutLine[0].FP1.x = Line[0].FP1.x;    LeftOutLine[0].FP1.y = (float)ROI_H;

							//Line �����͸� PreLine�� �Ѱ��ش� 
							PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
							PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
						}
						else { // �߰��κ� Broken�߻����� �����Ͱ� ���°�� 
							// PreLine �����͸� �Ѱ��� 
							LeftOutLine[0].FP0.x = PreLeftOutLine[0].FP0.x;  LeftOutLine[0].FP1.x = PreLeftOutLine[0].FP1.x;  
							LeftOutLine[0].FP0.y = PreLeftOutLine[0].FP0.y;  LeftOutLine[0].FP1.y = PreLeftOutLine[0].FP1.y;  
						}
					}

					// LeftInLine ����
					// LeftGrindIn ���� ���� 
					if(LeftGrindThresHold!=0){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftGrindThresHold);
						SingleLine->GImage = &RotateBinaryImg;
						LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;
						// ROI���� ������  ���  
						if(fabs(LeftOutLine[0].FP0.x - LeftOutLine[0].FP1.x) > 2){
							LSP.x = (LeftOutLine[0].FP0.x + LeftOutLine[0].FP1.x)/2;
							LSP.y =  LeftOutLine[0].FP0.y;
							LEP.x = (LeftOutLine[0].FP0.x + LeftOutLine[0].FP1.x)/2;
							LEP.y =  LeftOutLine[0].FP1.y;
						}
						else {
							LSP.x =  LeftOutLine[0].FP0.x;
							LSP.y =  LeftOutLine[0].FP0.y;
							LEP.x =  LeftOutLine[0].FP1.x;
							LEP.y =  LeftOutLine[0].FP1.y;
						}
						// ���� ���� 
						SingleLine->IRoi->SetOrgX  ((int)LSP.x+3        );
						SingleLine->IRoi->SetOrgY  ((int)LSP.y          );
						SingleLine->IRoi->SetWidth (100                 );
						SingleLine->IRoi->SetHeight((int)(LEP.y - LSP.y));
						if((LSP.x+3)<=0 || LSP.y<0 || (LEP.y - LSP.y)<=0 || (LSP.x+3)>=ImgW || LSP.y>ImgH || (LEP.y - LSP.y)>ImgH) break;

						SingleLine->SetFindLineId(0);
						SingleLine->SetOrientation(orWest);
						SingleLine->SetEdgeType(0); // Black to White 
						SingleLine->OnExecute();
						LeftInLine[0].FP0 = SingleLine->GetResultP0();
						LeftInLine[0].FP1 = SingleLine->GetResultP1();

						// ������ ��� 
						GrindWd1=0.0f,GrindWd2=0.0f;
						GrindWd1 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP0);
						GrindWd2 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP1);
						LeftGrindWidth = (GrindWd1+GrindWd2)/2.0;
					}

					// ������ ���� ���ض��� ���� ������ 
					if(LeftStdThresHold!=0 && LeftInLine[0].FP0.x!=0 && LeftInLine[0].FP0.y!=0 && LeftInLine[0].FP1.x!=0 && LeftInLine[0].FP1.y!=0){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftStdThresHold);
						SingleLine->GImage = &RotateBinaryImg;
						LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;

						// �������� ���  
						if(fabs(LeftInLine[0].FP0.x - LeftInLine[0].FP1.x) > 2){
							LSP.x = (LeftInLine[0].FP0.x + LeftInLine[0].FP1.x)/2;
							LSP.y =  LeftInLine[0].FP0.y;
							LEP.x = (LeftInLine[0].FP0.x + LeftInLine[0].FP1.x)/2;
							LEP.y =  LeftInLine[0].FP1.y;
						}
						else {
							LSP.x =  LeftInLine[0].FP0.x;
							LSP.y =  LeftInLine[0].FP0.y;
							LEP.x =  LeftInLine[0].FP1.x;
							LEP.y =  LeftInLine[0].FP1.y;
						}
						// ���� ���� 
						SingleLine->IRoi->SetOrgX  ((int)LSP.x+10       );
						SingleLine->IRoi->SetOrgY  ((int)LSP.y          );
						SingleLine->IRoi->SetWidth (100                 );
						SingleLine->IRoi->SetHeight((int)(LEP.y - LSP.y));
						if((LSP.x+3)<=0 || LSP.y<0 || (LEP.y - LSP.y)<=0 || (LSP.x+3)>=ImgW || LSP.y>ImgH || (LEP.y - LSP.y)>ImgH) break;

						SingleLine->SetFindLineId(0);
						SingleLine->SetOrientation(orWest);
						SingleLine->SetEdgeType(1); // Black to White 
						SingleLine->OnExecute();
						LeftStdLine[0].FP0 = SingleLine->GetResultP0();
						LeftStdLine[0].FP1 = SingleLine->GetResultP1();

						//������ ��� 
						GrindMea1=0.0f,GrindMea2=0.0f;
						GrindMea1 = Math.GetLengthFrLineToPoint(LeftStdLine[0].FP0,LeftStdLine[0].FP1,LeftOutLine[0].FP0);
						GrindMea2 = Math.GetLengthFrLineToPoint(LeftStdLine[0].FP0,LeftStdLine[0].FP1,LeftOutLine[0].FP1);
						LeftGrindMea = (GrindMea1+GrindMea2)/2.0;
					}
				}
				if(FRightWall){ // SoulBrain Image Rotate ���� 
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightEdgeThresHold);
// 					RotateBinaryImg.Save("D:\\ROTATE_IMG\\SoulBrain_BTM.bmp");
					if(RightEdgeThresHold==0) {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
					else                      {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()           );
					SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()           );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth()          );
					// ChamberCut VS Circle VS Rect��쿡 ���� Left���� Fitting���� ���̰� �޶��� 
					if     (IsCornerBtm) SingleLine->IRoi->SetHeight(FEndLine-RBCornerHeight   );
					else if(IsCircleBtm) SingleLine->IRoi->SetHeight(FEndLine-CSideCircleHeight);
					else                 SingleLine->IRoi->SetHeight(IRoi->GetHeight()         );

					SingleLine->SetIncrement(5 );
					SingleLine->SetOrientation(orEast);
					SingleLine->SetFindLineId(0);
					SingleLine->FThreshold =10;

                    SingleLine->SetEdgeType(1); // White To Black  
					SingleLine->OnExecute();

					Line[2].FP0 = SingleLine->GetResultP0();
					Line[2].FP1 = SingleLine->GetResultP1();

					if((fabs(Line[2].FP0.x - Line[2].FP1.x)>2) && (Line[2].FP1.x <Line[2].FP0.x)) Line[2].FP1.x = Line[2].FP0.x;

					// Btm,Top ������� ù��°
					RightOutLine[0].FP0.x = Line[2].FP0.x;
					RightOutLine[0].FP0.y = Line[2].FP0.y;
					RightOutLine[0].FP1.x = Line[2].FP1.x;
					RightOutLine[0].FP1.y = Line[2].FP1.y;

					if(fabs(RightOutLine[0].FP0.x - RightOutLine[0].FP1.x)>=0.5){
						RightOutLine[0].FP0.x = (RightOutLine[0].FP0.x+RightOutLine[0].FP1.x)/2.0;
						RightOutLine[0].FP1.x = (RightOutLine[0].FP0.x+RightOutLine[0].FP1.x)/2.0;
					}

					// LeftOutLine....make OutLine Data 
					// ������ �����͸� �Ѱ��ش� 
					// �˻��� ���ض����� ã�� ���Ұ�� ���������� �Ѱ��༭ ���ض��� ������ 
					if(RightOutLine[0].FP0.x!=0.0f && RightOutLine[0].FP0.y!=0.0f && RightOutLine[0].FP1.x!=0.0f && RightOutLine[0].FP1.y!=0.0f){
						PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
						PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
					} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
					else if(RightOutLine[0].FP0.x==0.0f || RightOutLine[0].FP0.y==0.0f || RightOutLine[0].FP1.x==0.0f || RightOutLine[0].FP1.y==0.0f){
						if(PreRightOutLine[0].FP0.x==0.0f && PreRightOutLine[0].FP0.y==0.0f && PreRightOutLine[0].FP1.x==0.0f && PreRightOutLine[0].FP1.y==0.0f){// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
							// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
							ROI_H = IRoi->GetHeight();
							RightOutLine[0].FP0.x = Line[2].FP0.x;    RightOutLine[0].FP0.y = 0.0f        ;
							RightOutLine[0].FP1.x = Line[2].FP1.x;    RightOutLine[0].FP1.y = (float)ROI_H;

							//Line �����͸� PreLine�� �Ѱ��ش� 
							PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
							PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
						}
						else { // �߰��κ� Broken�߻����� �����Ͱ� ���°�� 
							// PreLine �����͸� �Ѱ��� 
							RightOutLine[0].FP0.x = PreRightOutLine[0].FP0.x;  RightOutLine[0].FP1.x = PreRightOutLine[0].FP1.x;  
							RightOutLine[0].FP0.y = PreRightOutLine[0].FP0.y;  RightOutLine[0].FP1.y = PreRightOutLine[0].FP1.y;  
						}
					}

					// RightGrindIn���� ���� 
					if(RightGrindThresHold!=0){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightGrindThresHold);
						SingleLine->GImage = &RotateBinaryImg;
						RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;
						// ROI���� ������  ���  
						if(fabs(RightOutLine[0].FP0.x - RightOutLine[0].FP1.x) > 2){
							RSP.x = (RightOutLine[0].FP0.x + RightOutLine[0].FP1.x)/2;
							RSP.y =  RightOutLine[0].FP0.y;
							REP.x = (RightOutLine[0].FP0.x + RightOutLine[0].FP1.x)/2;
							REP.y =  RightOutLine[0].FP1.y;
						}
						else {
							RSP.x =  RightOutLine[0].FP0.x;
							RSP.y =  RightOutLine[0].FP0.y;
							REP.x =  RightOutLine[0].FP1.x;
							REP.y =  RightOutLine[0].FP1.y;
						}
						// ���� ���� 
// 						SingleLine->IRoi->SetOrgX  ((int)RSP.x-100      );
// 						SingleLine->IRoi->SetOrgY  ((int)RSP.y          );
// 						SingleLine->IRoi->SetWidth (97                  );
// 						SingleLine->IRoi->SetHeight((int)(REP.y - RSP.y));
// 						if((RSP.x-100)<=0 || RSP.y<0 || (REP.y - RSP.y)<=0 || (RSP.x-100)>=ImgW || RSP.y>ImgH || (REP.y - RSP.y)>ImgH) break;

						SingleLine->IRoi->SetOrgX  ((int)RSP.x-100      );
						SingleLine->IRoi->SetOrgY  ((int)RSP.y          );
						SingleLine->IRoi->SetWidth (100                 );
						SingleLine->IRoi->SetHeight((int)(REP.y - RSP.y));
						if((RSP.x-100)<=0 || RSP.y<0 || (REP.y - RSP.y)<=0 || (RSP.x-100)>=ImgW || RSP.y>ImgH || (REP.y - RSP.y)>ImgH) break;


						SingleLine->SetFindLineId(0);
						SingleLine->SetOrientation(orEast);
						SingleLine->SetEdgeType(0); // Black to White 
						SingleLine->OnExecute();
						RightInLine[0].FP0 = SingleLine->GetResultP0();
						RightInLine[0].FP1 = SingleLine->GetResultP1();

						// ������ ��� 
						GrindWd1=0.0f,GrindWd2=0.0f;
						GrindWd1 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP0);
						GrindWd2 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP1);
						RightGrindWidth = (GrindWd1+GrindWd2)/2.0;
					}

					// ������ ���� ���ض��� ���� ������ 
					if(RightStdThresHold!=0 && RightInLine[0].FP0.x!=0 && RightInLine[0].FP0.y!=0 && RightInLine[0].FP1.x!=0 && RightInLine[0].FP1.y!=0){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightStdThresHold);
						SingleLine->GImage = &RotateBinaryImg;
						RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;

						// �������� ���  
						if(fabs(RightInLine[0].FP0.x - RightInLine[0].FP1.x) > 2){
							RSP.x = (RightInLine[0].FP0.x + RightInLine[0].FP1.x)/2;
							RSP.y =  RightInLine[0].FP0.y;
							REP.x = (RightInLine[0].FP0.x + RightInLine[0].FP1.x)/2;
							REP.y =  RightInLine[0].FP1.y;
						}
						else {
							RSP.x =  RightInLine[0].FP0.x;
							RSP.y =  RightInLine[0].FP0.y;
							REP.x =  RightInLine[0].FP1.x;
							REP.y =  RightInLine[0].FP1.y;
						}
						// ���� ���� 
						SingleLine->IRoi->SetOrgX  ((int)RSP.x-100      );
						SingleLine->IRoi->SetOrgY  ((int)RSP.y          );
						SingleLine->IRoi->SetWidth (80                  );
						SingleLine->IRoi->SetHeight((int)(REP.y - RSP.y));
						if((RSP.x-100)<=0 || RSP.y<0 || (REP.y - RSP.y)<=0 || (RSP.x-100)>=ImgW || RSP.y>ImgH || (REP.y - RSP.y)>ImgH) break;

						SingleLine->SetFindLineId(0);
						SingleLine->SetOrientation(orEast);
						SingleLine->SetEdgeType(1); // Black to White 
						SingleLine->OnExecute();
						RightStdLine[0].FP0 = SingleLine->GetResultP0();
						RightStdLine[0].FP1 = SingleLine->GetResultP1();

						//������ ��� 
						GrindMea1=0.0f,GrindMea2=0.0f;
						GrindMea1 = Math.GetLengthFrLineToPoint(RightStdLine[0].FP0,RightStdLine[0].FP1,RightOutLine[0].FP0);
						GrindMea2 = Math.GetLengthFrLineToPoint(RightStdLine[0].FP0,RightStdLine[0].FP1,RightOutLine[0].FP1);
						RightGrindMea = (GrindMea1+GrindMea2)/2.0;
					}
				}
				if(FBottomWall){ //SoulBrain �̹��� ȸ�� �� �������� ��Ȯ�� ã�´� 
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, BtmEdgeThresHold);
					if(BtmEdgeThresHold==0)    {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
					else                       {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX());
					SingleLine->IRoi->SetOrgY  (FEndLine-100   );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth());
					SingleLine->IRoi->SetHeight(200             );

					SingleLine->SetOrientation(orSouth);
					SingleLine->FIncrement =20;
					SingleLine->SetFindLineId(0);
					SingleLine->FThreshold =5;
					SingleLine->OnExecute();

					if(IsMarkBtm) SingleLine->SetEdgeType(1);  //White To Black
					else          SingleLine->SetEdgeType(1);  //White To Black

					SingleLine->OnExecute();
					Line[3].FP0 = SingleLine->GetResultP0();
					Line[3].FP1 = SingleLine->GetResultP1();

					BtmOutLine[0].FP0 = Line[3].FP0;
					BtmOutLine[0].FP1 = Line[3].FP1;

					// Image Roate �� ���� ������ �ٽ� ���� 
					FLBPoint = Math.OnIntersect(LeftOutLine [0].FP0 , LeftOutLine [0].FP1 , Line[3].FP0, Line[3].FP1);
					FRBPoint = Math.OnIntersect(RightOutLine[0].FP0 , RightOutLine[0].FP1 , Line[3].FP0, Line[3].FP1);
				}
				if(FLeftBottom){
					if(IsCornerBtm){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, ECThresHold);               // Corner Out GinrdThresHold 
						EasyImage::Threshold(&RotateImg,&RotateGrindBinaryImg, CorGrindMeaThresHold); // Corner Measure GrindThresHold(3Point)

						if(ECThresHold==0)    {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
						else                  {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

						SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x-10            );
						SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y-LBCornerHeight);
						SingleLine->IRoi->SetWidth ((int)LBCornerWidth+20         );
						SingleLine->IRoi->SetHeight((int)LBCornerHeight           );

						SingleLine->SetOrientation(orWest);
						SingleLine->FIncrement =1;
						SingleLine->FThreshold =15;
						SingleLine->SetFindLineId(0);
						SingleLine->SetEdgeType(1);  //White To Black 
						SingleLine->OnExecute();

						Line[7].FP0 = SingleLine->GetResultP0();
						Line[7].FP1 = SingleLine->GetResultP1();

						// �̹��� ȸ�� �� �����ͱ��� ������ ���� 
						FLBLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
						FLBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);
						BtmEdgePoint[0].x = FLBBPoint.x;
						BtmEdgePoint[0].y = FLBBPoint.y;

						if(CMarkToGrindMarkDvX==0.0f && CMarkToGrindMarkDvY==0.0f){ // Corner Chamber Cut 3Point 
							if((FLBLPoint.x==0 || FLBLPoint.y==0 || FLBBPoint.x==0 || FLBBPoint.y==0) || (SingleLine->FEdgePointCount <30)){
								LBDx = 0.0f;
								LBDy = 0.0f;
								//ȭ�鿡 ���÷��� ���ؼ� 
								FLBLPoint.x = FLBPoint.x;
								FLBLPoint.y = FLBPoint.y;
								FLBBPoint.x = FLBPoint.x;
								FLBBPoint.y = FLBPoint.y;
							}
							else { 
								LBDx = Math.GetLengthFrPointToPoint(FLBPoint, FLBBPoint);
								LBDy = Math.GetLengthFrPointToPoint(FLBPoint, FLBLPoint);

								// OutLine C3Corner Data 
								if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
									if(C3Point!=NULL) delete[] C3Point;
									C3Point  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
									C3EdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

									CornerCount =0;  
									for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
										if(CornerCount>=MAX_CORNER_POINT) break;
										C3Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
										C3Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
										CornerCount++;
									}
								}

								// Corner ���� ���ʶ��� 
								if(CorGrindInThresHold!=0){
									EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
									SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

									// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
									// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
									if(LBCornerWidth >(LBCornerHeight*1.5)){
										SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x );
										SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y - LBCornerHeight    );
										SingleLine->IRoi->SetWidth ((int)((FLBBPoint.x - FLBPoint.x)+20));
										SingleLine->IRoi->SetHeight((int)((FLBPoint.y  - FLBLPoint.y)  ));

										if(FLBPoint.x<=0    || FLBPoint.y - LBCornerHeight<=0    || (FLBBPoint.x - FLBPoint.x )<=0    || ((FLBPoint.y  - FLBLPoint.y)+20)<=0   ) return false;
										if(FLBPoint.x>=ImgW || FLBPoint.y - LBCornerHeight>=ImgH || (FLBBPoint.x - FLBPoint.x )>=ImgW || ((FLBPoint.y  - FLBLPoint.y)+20)>=ImgH) return false;

										// 										SingleLine->SetOrientation(orSouth);
										SingleLine->SetOrientation(orNorth);
										SingleLine->FIncrement =1;
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(1);  //White To Black 
										SingleLine->OnExecute();
									}
									else {
										// Corner ���� ���ʶ��� ã�´� 
										SingleLine->SetOrientation(orWest);
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(1);  //White To Black 
										SingleLine->OnExecute();
									}

									// InLine C3Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C3InPoint!=NULL) delete[] C3InPoint;
										C3InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C3InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C3InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C3InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
									FLBLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
									FLBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);
									BtmEdgePoint[0].x = FLBBInPoint.x;
									BtmEdgePoint[0].y = FLBBInPoint.y;

									//C3 �ڳ� ������ 
									if(IsMarkBtm){
										Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
										for(int k=0; k<C3InEdgeCount; k++){
											Dis = Math.GetLengthFrLineToPoint(FLBLPoint, FLBBPoint, C3InPoint[k]);
											TotalGrindWidth+=Dis;
										}
										if(C3InEdgeCount!=0){
											AvgGrindWidth = (TotalGrindWidth/C3InEdgeCount);
											C3CorGrindWd  = AvgGrindWidth;
										}
									}
								}
								// �ڳ� ������ ���� ������ ã�´� 
								// ROI ���� ����(3���� ��� ���� ���� ������ ã�´�)
								if(CorGrindMeaThresHold!=0){
									if(CorGrindMeaThresHold==0){SingleLine->GImage = &RotateImg           ;  SingleLine->FThreshold =5 ;}
									else                       {SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold =10;}

									if(LBCornerWidth >(LBCornerHeight*1.5)){ // ���Ī ���� Chamber Cut 
										SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x                   );
										SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y-LBCornerHeight +20);
										SingleLine->IRoi->SetWidth ((int)LBCornerWidth -50            );
										SingleLine->IRoi->SetHeight((int)LBCornerHeight-30            );

										SingleLine->SetOrientation(orSouth);
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();
									}
									else                                   { //�Ϲ����� Chamber CUt
										SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x+15               );
										SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y-LBCornerHeight+15);
										SingleLine->IRoi->SetWidth ((int)LBCornerWidth               );
										SingleLine->IRoi->SetHeight((int)LBCornerHeight-15           );

										SingleLine->SetOrientation(orWest);
										SingleLine->SetFindLineId(0);
										SingleLine->FIncrement =1;
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();
									}
									//�ڳ� ������ ���� ���ΰ� ���ο� ������ ���� 
									FLBLMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
									FLBBMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);

									// C3Corner Grind Measure Stand Data 
									if((SingleLine->FEdgePointCount)>0){
										if(C3MeaPoint!=NULL) delete[] C3MeaPoint;
										C3MeaPoint     = new TFPoint [(SingleLine->FEdgePointCount)];
										C3MeaEdgeCount = (SingleLine->FEdgePointCount);

										CornerCount =0;  
										for(int k=0; k<(SingleLine->FEdgePointCount); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C3MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C3MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
									//�ڳ� ������ ���  
									Dis =0.0f, AvgGrindMea=0.0f, TotalGrindMeasure=0.0f;
									for(int k=0; k<C3MeaEdgeCount; k++){
										Dis = Math.GetLengthFrLineToPoint(FLBLPoint, FLBBPoint, C3MeaPoint[k]);
										TotalGrindMeasure+=Dis;
									}
									if(C3MeaEdgeCount!=0){
										AvgGrindMea    = (TotalGrindMeasure/C3MeaEdgeCount);
										C3CorGrindMea  = AvgGrindMea;
									}
								}
							}	
						}
						else                                                      { // Corner Chamber Cut GrindMark 
							TFPoint C3GrindRectPoint(0.0f, 0.0f);
							if((FLBLPoint.x==0 || FLBLPoint.y==0 || FLBBPoint.x==0 || FLBBPoint.y==0) || (SingleLine->FEdgePointCount <30)){
								LBDx = 0.0f;
								LBDy = 0.0f;
								//ȭ�鿡 ���÷��� ���ؼ� 
								FLBLPoint.x = FLBPoint.x;
								FLBLPoint.y = FLBPoint.y;
								FLBBPoint.x = FLBPoint.x;
								FLBBPoint.y = FLBPoint.y;
							}
							else { 
								LBDx = Math.GetLengthFrPointToPoint(FLBPoint, FLBBPoint);
								LBDy = Math.GetLengthFrPointToPoint(FLBPoint, FLBLPoint);

								// OutLine C3Corner Data 
								if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
									if(C3Point!=NULL) delete[] C3Point;
									C3Point  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
									C3EdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

									CornerCount =0;  
									for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
										if(CornerCount>=MAX_CORNER_POINT) break;
										C3Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
										C3Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
										CornerCount++;
									}
								}

								// Corner ���� ���ʶ��� 
								if(CorGrindInThresHold!=0){
									EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
									SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

									// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
									// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
									if(LBCornerWidth >(LBCornerHeight*1.5)){
										SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x );
										SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y - LBCornerHeight    );
										SingleLine->IRoi->SetWidth ((int)((FLBBPoint.x - FLBPoint.x)+20));
										SingleLine->IRoi->SetHeight((int)((FLBPoint.y  - FLBLPoint.y)  ));

										if(FLBPoint.x<=0    || FLBPoint.y - LBCornerHeight<=0    || (FLBBPoint.x - FLBPoint.x )<=0    || ((FLBPoint.y  - FLBLPoint.y)+20)<=0   ) return false;
										if(FLBPoint.x>=ImgW || FLBPoint.y - LBCornerHeight>=ImgH || (FLBBPoint.x - FLBPoint.x )>=ImgW || ((FLBPoint.y  - FLBLPoint.y)+20)>=ImgH) return false;

										SingleLine->SetOrientation(orSouth);
										SingleLine->FIncrement =1;
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();
									}
									else {
										// Corner ���� ���ʶ��� ã�´� 
										SingleLine->SetOrientation(orWest);
										SingleLine->SetFindLineId(0);
										SingleLine->FIncrement =1;
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();
									}
									// InLine C3Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C3InPoint!=NULL) delete[] C3InPoint;
										C3InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C3InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C3InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C3InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
									FLBLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
									FLBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);
									BtmEdgePoint[0].x = FLBBInPoint.x;
									BtmEdgePoint[0].y = FLBBInPoint.y;
								}

								// GrindMark���� ������,������ ������ 
								// LeftBtm 
								if(IsMarkBtm){
									CGrindMark[0].Rect.left = (FLBPoint.x + CMarkToGrindMarkDvX) -GrindMarkRange;
									CGrindMark[0].Rect.top  = (FLBPoint.y - CMarkToGrindMarkDvY) -GrindMarkRange;
									CGrindMark[0].Rect.right  = CGrindMark[0].Rect.left+GrindMarkLength;
									CGrindMark[0].Rect.bottom = CGrindMark[0].Rect.top +GrindMarkLength;

									// �����̹������� Mark ã�´� 
									Find->OnLoad(true,FindDir)                     ;
									Find->GImage = &RotateImg                      ;
									Find->SetContrastMode(EFindContrastMode_Normal);
									Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind �˻� �ּ� Score
									Find->SetZeroScore(GrindPadMarkMinScore)       ; // �˻� �� �ּ� Score 
									Find->SetMaxInstances(1)                       ;
									Find->SetAngleTolerance(20.0f)                 ;
									Find->SetAngleBias(45.0f)                      ;
									Find->SetScaleBias(1.00f)                      ;
									Find->SetScaleTolerance(0.0f)                  ;
									Find->Property.Ox = (float)ImgW/2              ;
									Find->Property.Oy = (float)ImgH/2              ;

									GrindRectWidth  = (CGrindMark[0].Rect.right  - CGrindMark[0].Rect.left);
									GrindRectHeight = (CGrindMark[0].Rect.bottom - CGrindMark[0].Rect.top );
									Find->IRoi->SetPlacement(CGrindMark[0].Rect.left,CGrindMark[0].Rect.top,GrindRectWidth,GrindRectHeight);
									if(CGrindMark[0].Rect.left<=0    || CGrindMark[0].Rect.top<=0    || GrindRectWidth<=0    || GrindRectHeight<=0   ) return false;
									if(CGrindMark[0].Rect.left>=ImgW || CGrindMark[0].Rect.top>=ImgH || GrindRectWidth>=ImgW || GrindRectHeight>=ImgH) return false;

									// �˻� �� Match Pos ��ǥ �޾�´� 
									// Find �˻� ������ ������ �޾ƿ� 
									if(Find->OnExecute()){
										CGrindMark[0].Point.x = Find->Result[0].Px;
										CGrindMark[0].Point.y = Find->Result[0].Py;
										CGrindMark[0].Score   = Find->Result[0].Score;
									}

									// GrindMakrã����� ������,������ ������
									if(CGrindMark[0].Point.x !=0.0f && CGrindMark[0].Point.y!=0.0f){
										// GrindMark���� OutData �ִܰŸ� Count���� Calc 
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
										for(int k=0; k<C3EdgeCount; k++){
											if(C3Point[k].x  ==0.0f         || C3Point[k].y         ==0.0f) continue;
											if(CGrindMark[0].Point.x==0.0f  || CGrindMark[0].Point.y==0.0f) continue;

											GrindMeaDis = Math.GetLength(CGrindMark[0].Point, C3Point[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkOutCnt = k;
											}
										}
										// OutData���� InData �ִܰŸ� Count���� Calc 
										if(GrindMarkOutCnt!=0){
											GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
											for(int k=0; k<C3InEdgeCount; k++){
												if(C3InPoint[k].x   ==0.0f          || C3InPoint[k].y             ==0.0f) continue;
												if(C3Point[GrindMarkOutCnt].x==0.0f || C3Point[GrindMarkOutCnt].y ==0.0f) continue;

												GrindMeaDis = Math.GetLength(C3Point[GrindMarkOutCnt], C3InPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													GrindMarkInCnt = k;
												}
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
										if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5 && (GrindMarkOutCnt+5<C3EdgeCount)){
											CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
											for(int k=GrindMarkOutCnt-5; k<GrindMarkOutCnt+5; k++){
												if(C3Point[k].x==0.0f || C3Point[k].y==0.0f) continue;
												if(k>=C3EdgeCount                          ) continue;

												GrindMeaDis = Math.GetLength(CGrindMark[0].Point, C3Point[k]);
												TotalCirDis+= GrindMeaDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												CGrindMark[0].GrindMea = TotalCirDis/CircleCount;
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
										if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
											CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
											for(int k=-5; k<5; k++){
												if(C3Point  [GrindMarkOutCnt+k].x==0.0f && C3Point  [GrindMarkOutCnt+k].y==0.0f) continue;
												if(C3InPoint[GrindMarkInCnt +k].x==0.0f && C3InPoint[GrindMarkInCnt +k].y==0.0f) continue;
												if((GrindMarkOutCnt+k)>=C3EdgeCount                                            ) continue;
												if((GrindMarkInCnt +k)>=C3InEdgeCount                                          ) continue;

												GrindWidthDis = Math.GetLength(C3InPoint[GrindMarkInCnt+k], C3Point[GrindMarkOutCnt+k]);
												TotalCirDis+=   GrindWidthDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												CGrindMark[0].GrindWidth  = TotalCirDis/CircleCount;
											}
										}
									}
									else { // GrindMrk ��ã����� �������� ������ 
										// OutData���� �ּҰ� ������ Count���� 
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
										for(int k=0; k<C3EdgeCount; k++){
											if(C3Point[k].x==0.0f || C3Point[k].y==0.0f) continue;

											C3GrindRectPoint.x = CGrindMark[0].Rect.left + (CGrindMark[0].Rect.right  - CGrindMark[0].Rect.left)/2; 
											C3GrindRectPoint.y = CGrindMark[0].Rect.top  + (CGrindMark[0].Rect.bottom - CGrindMark[0].Rect.top )/2; 

											GrindMeaDis = Math.GetLength(C3GrindRectPoint, C3Point[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkOutCnt = k;
											}
										}

										// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
										if(GrindMarkOutCnt!=0){
											GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
											for(int k=0; k<C3InEdgeCount; k++){
												if(C3InPoint[k].x            ==0.0f || C3InPoint[k].y             ==0.0f) continue;
												if(C3Point[GrindMarkOutCnt].x==0.0f || C3Point[GrindMarkOutCnt].y ==0.0f) continue;

												GrindMeaDis = Math.GetLength(C3Point[GrindMarkOutCnt], C3InPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													GrindMarkInCnt = k;
												}
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
										if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
											CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
											for(int k=-5; k<5; k++){
												if(C3Point  [GrindMarkOutCnt+k].x==0.0f && C3Point  [GrindMarkOutCnt+k].y==0.0f) continue;
												if(C3InPoint[GrindMarkInCnt +k].x==0.0f && C3InPoint[GrindMarkInCnt +k].y==0.0f) continue;
												if((GrindMarkOutCnt+k)>=C3EdgeCount                                            ) continue;
												if((GrindMarkInCnt +k)>=C3InEdgeCount                                          ) continue;

												GrindWidthDis = Math.GetLength(C3InPoint[GrindMarkInCnt+k], C3Point[GrindMarkOutCnt+k]);
												TotalCirDis+=   GrindWidthDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												C3CorGrindWd = TotalCirDis/CircleCount;
											}
										}
									}
								}
							}	
						}
					}
					else if(IsRectBtm){
						LBDx = 0.0f;
						LBDy = 0.0f;

						//ȭ�鿡 ���÷��� ���ؼ� 
						FLBLPoint.x = FLBPoint.x;
						FLBLPoint.y = FLBPoint.y;
						FLBBPoint.x = FLBPoint.x;
						FLBBPoint.y = FLBPoint.y;
						// Btm�������� �������� ����,���� ���� �� Defect�˻翡 �ʿ��� ���� ��ǥ 
						BtmEdgePoint[0].x =FLBPoint.x;
						BtmEdgePoint[0].y =FLBPoint.y;
					}
					else if(IsCircleBtm){ // Circle Cut(LeftBtm)
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg       , BtmEdgeThresHold    ); // Circle Grind Edge(Polygon_Mode, Mark_Mode���)
						EasyImage::Threshold(&RotateImg,&RotateGrindInBinaryImg, CirGrindInThresHold ); // Circle Grind In_EdgePoint ���� (Mark_Modr���)
						TFPoint Cir1GrindRectPoint(0.0f, 0.0f);
						if(IsMarkBtm){
							if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f){
								// �����̹������� Mark ã�´� 
								Find->OnLoad(true,FindDir)                     ;
								Find->GImage = &RotateImg                      ;
								Find->SetContrastMode(EFindContrastMode_Normal);
								Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind �˻� �ּ� Score
								Find->SetZeroScore(GrindPadMarkMinScore)       ; // �˻� �� �ּ� Score 
								Find->SetMaxInstances(1)                       ;
								Find->SetAngleTolerance(20.0f)                 ;
								Find->SetScaleBias(1.00f)                      ;
								Find->SetScaleTolerance(0.0f)                  ;
								Find->Property.Ox = (float)ImgW/2              ;
								Find->Property.Oy = (float)ImgH/2              ;

								// ù��° GrindMark���� ���� C1 Search 
								if(CMarkToGrindMarkDvX!=0.0f && CMarkToGrindMarkDvY!=0.0f){
									CGrindMark[0].Rect.left   = (FLBPoint.x+CMarkToGrindMarkDvX)-40;
									CGrindMark[0].Rect.top    = (FLBPoint.y-CMarkToGrindMarkDvY)-40;
									CGrindMark[0].Rect.right  = (CGrindMark[0].Rect.left + 80);
									CGrindMark[0].Rect.bottom = (CGrindMark[0].Rect.top  + 80);

									GrindRectWidth  = (CGrindMark[0].Rect.right  - CGrindMark[0].Rect.left);
									GrindRectHeight = (CGrindMark[0].Rect.bottom - CGrindMark[0].Rect.top );

									Find->IRoi->SetPlacement(CGrindMark[0].Rect.left,CGrindMark[0].Rect.top,GrindRectWidth,GrindRectHeight);
									if(CGrindMark[0].Rect.left<=0 || CGrindMark[0].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

									// �˻� �� Match Pos ��ǥ �޾�´� 
									// Find �˻� ������ ������ �޾ƿ� 
									if(RqMode == true)
										Find->SetAngleBias(35.0f);
									else
										Find->SetAngleBias(45.0f);
									//Find->SetAngleBias(45.0f);
									if(Find->OnExecute()){
										CGrindMark[0].Point.x = Find->Result[0].Px;
										CGrindMark[0].Point.y = Find->Result[0].Py;
										CGrindMark[0].Score   = Find->Result[0].Score;
									}
								}
							}
						}

						if(FInspectMode==Insp_Polygon){ // LeftBottom 
							if(CSideCircleWidth!=0.0f && CSideCircleHeight!=0.0f  ){ // Notch�� �¿� �������� 
								NotchPolyRect[0].left  = (long)(FLBPoint.x                 );  NotchPolyRect[0].top    = (long)(FLBPoint.y - CSideCircleHeight);
								NotchPolyRect[0].right = (long)(FLBPoint.x+CSideCircleWidth);  NotchPolyRect[0].bottom = (long)(FLBPoint.y                    );
							}
							TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
							// Polygon Algorithm �⺻ ����  
							Polygon->IWidth          = ImgW;
							Polygon->IHeight         = ImgH;
							Polygon->PoPa.MinPxCnt   =300  ;
							Polygon->PoPa.MaxPxCnt   =20000;
							Polygon->PoPa.MaxBbCnt   =10   ;

							if(NotchPolyRect[0].left!=0 && NotchPolyRect[0].top!=0 && NotchPolyRect[0].right!=0 && NotchPolyRect[0].bottom!=0){
								P1.x=0, P1.y=0, P2.x=0, P2.y=0, P3.x=0, P3.y=0, P4.x=0, P4.y=0;
								LogUnit.SetLog(L"CellID:%d, CamID:%d ==>Polygon1_Insp_Start",CellId,CamId);     

								P1.x = (NotchPolyRect[0].left); P1.y =(NotchPolyRect[0].top             ); P2.x = (P1.x+NotchPolyRect[0].Width()); P2.y = (P1.y);
								P3.x = (NotchPolyRect[0].left); P3.y =(P1.y+NotchPolyRect[0].Height()+50); P4.x = (P2.x                         ); P4.y = (P3.y);

								// Median Filter ���� 
								if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
								if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
								if( (P1.x + int(P2.x - P1.x)) >= ImgW || (P1.y + int(P3.y - P1.y)) >= ImgH ) return false;
								FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
								FilterROI.Attach(&RotateBinaryImg)                                    ;
								FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
								LogUnit.SetLog(L"Polygon1 ==>Px :%d, Py :%d, Width:%d, Height:%d ==>Median Filter",P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
								LogUnit.SetLog(L"Polygon1 ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
								EasyImage::Median(&FilterROI, &FilterROI)                             ;
								// 										 RotateBinaryImg.Save("D:\\ROTATE_IMG\\Polygon_BinaryImg_����1.bmp");

								Polygon->GImage          = &RotateBinaryImg           ;
								Polygon->PoPa.FMaxWidth  =(int)(CSideCircleWidth+100 );
								Polygon->PoPa.FMaxHeight =(int)(CSideCircleHeight+100);
								Polygon->PoPa.IsSeparate =true                        ;
								Polygon->PoPa.IsInToOut  =true                        ;
								Polygon->PoPa.HiThHold   = BtmEdgeThresHold           ;
								Polygon->PoPa.LoThHold   = 0                          ;
								Polygon->PoPa.FMinWidth  =(int)(CSideCircleWidth/2)   ;
								Polygon->PoPa.FMinHeight =(int)(CSideCircleHeight/2)  ;

								if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
									Polygon->OnExecute(P1,P2,P3,P4);
								}

								// Polygon Algorithm InEdgeData/OutEdgeData Separate �� ������ ���� 
								if(Polygon->BlobCount==1){
									PolygonMarginCnt =10;
									if(Polygon->Blob[0].InCnt>(PolygonMarginCnt*2)){
										C1CirInEdgeCount=0;
										C1CirInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt*2));
										if(C1CirInEdgeCount>0){
											if(C1CirInPoint!=NULL){
												delete[] C1CirInPoint;
											}
											C1CirInPoint = new TFPoint[C1CirInEdgeCount];

											PixelCnt=0;
											for(int k=0; k<Polygon->Blob[0].InCnt; k++){
												if(k<PolygonMarginCnt || k>=(Polygon->Blob[0].InCnt-PolygonMarginCnt)) continue;
												C1CirInPoint[PixelCnt].x   = Polygon->Blob[0].InPoint[k].x ;
												C1CirInPoint[PixelCnt].y   = Polygon->Blob[0].InPoint[k].y ;
												PixelCnt++;
											}
										}
									}

									if(Polygon->Blob[0].OutCnt>(PolygonMarginCnt*2)){
										C1CirOutEdgeCount=0;
										C1CirOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt*2));
										if(C1CirOutEdgeCount>0){
											if(C1CirOutPoint!=NULL) delete[] C1CirOutPoint;
											C1CirOutPoint = new TFPoint[C1CirOutEdgeCount];

											PixelCnt=0;
											for(int k=0; k<Polygon->Blob[0].OutCnt; k++){
												if(k<PolygonMarginCnt || k>=(Polygon->Blob[0].OutCnt-PolygonMarginCnt)) continue;
												C1CirOutPoint[PixelCnt].x  = Polygon->Blob[0].OutPoint[k].x ;
												C1CirOutPoint[PixelCnt].y  = Polygon->Blob[0].OutPoint[k].y ;
												if(k==(Polygon->Blob[0].OutCnt-PolygonMarginCnt-1)){ // ���������� �ʿ��� ������ ������ �·� 
													BtmEdgePoint[0].x = C1CirOutPoint[PixelCnt].x;
													BtmEdgePoint[0].y = C1CirOutPoint[PixelCnt].y;
												}
												PixelCnt++;
											}
										}
									}
								}
								/////////////////////////////////////////////////////////////////////////////////////////////

								// InEdgeData/OutEdgeData ���� ������ �� ������ ���
								// Polygon �˰��򿡼��� GrindMark���� OutData,InData �ִܰŸ� 2�� Tag�� �����ؾ��� 
								if(IsMarkBtm){ // LTPS ������,������ ��� 
									if(C1CirOutEdgeCount!=0 && C1CirInEdgeCount!=0){ // MarkMode��� ������ �˻縦 ���� ���� �� ������ ����
										// GrindMark[0] �������� ������,������ ���� ������ 
										if(CGrindMark[0].Point.x!=0.0f && CGrindMark[0].Point.y!=0.0f){  //GrindMark ã������� 
											// GrindMark���� OutData �ִܰŸ� Count���� Calc 
											GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
											for(int k=0; k<C1CirOutEdgeCount; k++){
												if(C1CirOutPoint[k].x ==0.0f   || C1CirOutPoint[k].y  ==0.0f ) continue;
												if(CGrindMark[0].Point.x==0.0f || CGrindMark[0].Point.y==0.0f) continue;

												GrindMeaDis = Math.GetLength(CGrindMark[0].Point, C1CirOutPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													GrindMarkOutCnt = k;
												}
											}

											// GrindMark���� InData �ִܰŸ� Count���� Calc 
											if(GrindMarkOutCnt!=0){
												GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
												for(int k=0; k<C1CirInEdgeCount; k++){
													if(C1CirInPoint[k].x   ==0.0f             || C1CirInPoint[k].y                ==0.0f) continue;
													if(C1CirOutPoint[GrindMarkOutCnt].x==0.0f || C1CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

													GrindMeaDis = Math.GetLength(C1CirOutPoint[GrindMarkOutCnt], C1CirInPoint[k]);
													if(GrindMeaDis<MinDis){
														MinDis = GrindMeaDis;
														GrindMarkInCnt = k;
													}
												}
											}

											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
											if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5 && (GrindMarkOutCnt+5<C1CirOutEdgeCount)){
												CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
												for(int k=GrindMarkOutCnt-5; k<GrindMarkOutCnt+5; k++){
													if(C1CirOutPoint[k].x==0.0f || C1CirOutPoint[k].y==0.0f) continue;
													if(k>=C1CirOutEdgeCount                                ) continue;

													GrindMeaDis = Math.GetLength(CGrindMark[0].Point, C1CirOutPoint[k]);
													TotalCirDis+= GrindMeaDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[0].GrindMea = TotalCirDis/CircleCount;
													CGrindMark[0].GrindMea = CGrindMark[0].GrindMea+0.5; //SubPixel 
												}
											}

											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
											if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
												CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
												for(int k=-5; k<5; k++){
													if(C1CirOutPoint[GrindMarkOutCnt+k].x==0.0f && C1CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
													if(C1CirInPoint [GrindMarkInCnt +k].x==0.0f && C1CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
													if((GrindMarkOutCnt+k)>=C1CirOutEdgeCount                                              ) continue;
													if((GrindMarkInCnt +k)>=C1CirInEdgeCount                                               ) continue;

													GrindWidthDis = Math.GetLength(C1CirInPoint[GrindMarkInCnt+k], C1CirOutPoint[GrindMarkOutCnt+k]);
													TotalCirDis+=   GrindWidthDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[0].GrindWidth = TotalCirDis/CircleCount;
													CGrindMark[0].GrindWidth = CGrindMark[0].GrindWidth+0.5; //SubPixel
												}
											}
										}
										else if(CGrindMark[0].Rect.left!=0 && CGrindMark[0].Rect.top!=0 && CGrindMark[0].Rect.right!=0 && CGrindMark[0].Rect.bottom!=0) {  //GrindMark �� ã�������
											// OutData���� �ּҰ� ������ Count���� 
											GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
											for(int k=0; k<C1CirOutEdgeCount; k++){
												if(C1CirOutPoint[k].x==0.0f || C1CirOutPoint[k].y==0.0f) continue;

												Cir1GrindRectPoint.x = CGrindMark[0].Rect.left + (CGrindMark[0].Rect.right  - CGrindMark[0].Rect.left)/2; 
												Cir1GrindRectPoint.y = CGrindMark[0].Rect.top  + (CGrindMark[0].Rect.bottom - CGrindMark[0].Rect.top )/2; 

												GrindMeaDis = Math.GetLength(Cir1GrindRectPoint, C1CirOutPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													GrindMarkOutCnt = k;
												}
											}

											// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
											if(GrindMarkOutCnt!=0){
												GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
												for(int k=0; k<C1CirInEdgeCount; k++){
													if(C1CirInPoint[k].x   ==0.0f             || C1CirInPoint[k].y                ==0.0f) continue;
													if(C1CirOutPoint[GrindMarkOutCnt].x==0.0f || C1CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

													GrindMeaDis = Math.GetLength(C1CirOutPoint[GrindMarkOutCnt], C1CirInPoint[k]);
													if(GrindMeaDis<MinDis){
														MinDis = GrindMeaDis;
														GrindMarkInCnt = k;
													}
												}
											}

											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
											if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
												CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
												for(int k=-5; k<5; k++){
													if(C1CirOutPoint[GrindMarkOutCnt+k].x==0.0f && C1CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
													if(C1CirInPoint [GrindMarkInCnt +k].x==0.0f && C1CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
													if((GrindMarkOutCnt+k)>=C1CirOutEdgeCount                                              ) continue;
													if((GrindMarkInCnt +k)>=C1CirInEdgeCount                                               ) continue;

													GrindWidthDis = Math.GetLength(C1CirInPoint[GrindMarkInCnt+k], C1CirOutPoint[GrindMarkOutCnt+k]);
													TotalCirDis+=   GrindWidthDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[0].GrindWidth = TotalCirDis/CircleCount     ;
													CGrindMark[0].GrindWidth = CGrindMark[0].GrindWidth+0.5; //SubPixel
												}
											}
										}
									}
								}
								else         { //Polygon EnCap ���� OutPoint �߰��� ���� ���� �Ÿ� ��հ��� ����Ѵ�(LeftTop) 
									if(C1CirInEdgeCount!=0 && C1CirOutEdgeCount!=0){
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
										GrindMarkOutCnt = (int)(C1CirOutEdgeCount/2);
										// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
										if(GrindMarkOutCnt!=0){
											GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
											for(int k=0; k<C1CirInEdgeCount; k++){
												if(C1CirInPoint[k].x   ==0.0f             || C1CirInPoint[k].y                ==0.0f) continue;
												if(C1CirOutPoint[GrindMarkOutCnt].x==0.0f || C1CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

												GrindMeaDis = Math.GetLength(C1CirOutPoint[GrindMarkOutCnt], C1CirInPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													GrindMarkInCnt = k;
												}
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
										if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
											CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
											for(int k=-5; k<5; k++){
												if(C1CirOutPoint[GrindMarkOutCnt+k].x==0.0f && C1CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
												if(C1CirInPoint [GrindMarkInCnt +k].x==0.0f && C1CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
												if((GrindMarkOutCnt+k)>=C1CirOutEdgeCount                                              ) continue;
												if((GrindMarkInCnt +k)>=C1CirInEdgeCount                                               ) continue;

												GrindWidthDis = Math.GetLength(C1CirInPoint[GrindMarkInCnt+k], C1CirOutPoint[GrindMarkOutCnt+k]);
												TotalCirDis+=   GrindWidthDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												CGrindMark[0].GrindWidth = TotalCirDis/CircleCount;
												CGrindMark[0].GrindWidth = CGrindMark[0].GrindWidth+0.5; //SubPixel
											}
										}
									}
								}
								///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								// FAVION Circle Fitting �����(Circle_1)
								SampleSize   = C1CirInEdgeCount*0.75; //0.5~0.8���Ϸ� �Է��Ұ�
								DistDevRatio = 0.15                 ; //0.1~0.25 ���Ϸ� �Է��Ұ�
								CircleFit->OnRansac(C1CirInPoint,C1CirInEdgeCount,SampleSize,DistDevRatio,&CircleFit1Center,&CirFitDiameter1);

								// Polygon �ڸ� �˻� ������ 
								if(!IsMarkBtm && BrokenThresHold!=0){
									RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
									EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBrokenImg);
									EasyImage::Threshold(&RotateImg,&RotateBrokenImg, BrokenThresHold);
									Edge->GImage = &RotateBrokenImg;
									Edge->SetEdgeType(0);

									int DvCnt =0, DataCount=0;
									double deg =0.0f; 
									TFPoint Target(0.0f, 0.0f);

									if(GrindMarkInCnt>GrindMarkOutCnt){
										DvCnt = GrindMarkInCnt - GrindMarkOutCnt;
										for(int k=0; k<C1CirOutEdgeCount; k++){
											deg =0.0f, Target.x =0.0f, Target.y =0.0f;
											if(((k+DvCnt)<C1CirInEdgeCount) && C1CirInPoint[k+DvCnt].x>0  && C1CirInPoint[k+DvCnt].y>0){
												deg = Math.GetDegree(C1CirOutPoint[k],C1CirInPoint[k+DvCnt]);

												Target  = Math.OnFindPerpendicularPointXY(C1CirOutPoint[k],dEdgeToTrapLineDis,-dEdgeToTrapLineDis,deg);
												Edge->OnExecute(C1CirOutPoint[k],Target); // �����ܰ����ο��� Target���� White Point Search 
												if(Edge->GetResultEdgePointCount()>0){
													C1CircleData[DataCount].Dis =0.0f;
													C1CircleData[DataCount].Dv  =0.0f;
													C1CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
													C1CircleData[DataCount].P.y = C1CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
													DataCount++;
												}
											}
										}
										C1CirInCrackCount = DataCount;
									}
									else if(GrindMarkInCnt<GrindMarkOutCnt)   {
										DvCnt = GrindMarkOutCnt - GrindMarkInCnt;
										for(int k=DvCnt; k<C1CirOutEdgeCount; k++){
											deg =0.0f, Target.x =0.0f, Target.y =0.0f;
											if(((k-DvCnt)<C1CirInEdgeCount) && C1CirInPoint[k-DvCnt].x>0  && C1CirInPoint[k-DvCnt].y>0 && k-DvCnt>=0){
												deg = Math.GetDegree(C1CirOutPoint[k],C1CirInPoint[k-DvCnt]);

												Target  = Math.OnFindPerpendicularPointXY(C1CirOutPoint[k],dEdgeToTrapLineDis,-dEdgeToTrapLineDis,deg);
												Edge->OnExecute(C1CirOutPoint[k],Target); // �����ܰ����ο��� Target���� White Point Search 
												if(Edge->GetResultEdgePointCount()>0){
													C1CircleData[DataCount].Dis =0.0f;
													C1CircleData[DataCount].Dv  =0.0f;
													C1CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
													C1CircleData[DataCount].P.y = C1CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
													DataCount++;
												}
											}
										}
										C1CirInCrackCount = DataCount;
									}
								}
								LogUnit.SetLog(L"CellID:%d, CamID:%d ==>Polygon1_Insp_End",CellId,CamId);     
							}
						}
						else if (FInspectMode==Insp_Mark) { // Circle LeftBtm
							if(CSideCircleWidth!=0.0f && CSideCircleHeight!=0.0f){
								C1CirCenter.x = FLBPoint.x + CSideCircleWidth + dCirOffsetX;  C1CirCenter.y = FLBPoint.y -  CSideCircleHeight - dCirOffsetY;  
							}

							Circle->SetSamplingStep(1)                            ; // Circle Edge ���� 

							// ù��° Circle EdgePoint Data ���� 
							if(C1CirCenter.x!=0 && C1CirCenter.y!=0){
								LogUnit.SetLog(L"Circle_1_Insp_Start");    

								if(CirGrindInThresHold==0) Circle->GImage = &RotateImg             ; //Org Image �Ѱ��� 
								else                       Circle->GImage = &RotateGrindInBinaryImg; //Binary Image �Ѱ��� 

								Circle->SetCenterXY(C1CirCenter.x      , C1CirCenter.y     ); // �ܰ����� ������ ���� ���� 
								C1Diameter = (CSideCircleWidth*2);
								// 									 C1Diameter = (CSideCircleWidth*2);
								// �˻� �������� ���ؼ� �ҷ� �̰��� �߻��Ҽ� ����....LTPS�� EnCap�з���
								if(IsMarkBtm) Circle->SetTolerance(60.0f);
								else          Circle->SetTolerance(80.0f);
								// 									 Circle->SetTolerance(40.0f )                              ; // Circle Tolerance 
								Circle->SetDiameter (C1Diameter)                          ; // Circle Diameter 
								Circle->SetCircleChoice(1)                                ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
								Circle->SetCircleType  (2)                                ; // 0:Black to White, 1:White to Black 2: Both Black, White 
								Circle->SetCircleIndex (1)                                ; // Circle EdgePoint Index 

								// Angle �� Amplitude ���� 
								if(SideCirDeg>=0){
									Circle->SetAngle    (90+CirAngleMargin          );  // Circle Fitting ������ ����
									Circle->SetAmplitude(SideCirDeg-2*CirAngleMargin);  // Circle Fitting ������ ���� �˻� ���� 
								}
								// Circle1 InData EdgePoint 
								//////////////////////////////////////////////////////////////////////////////////////////////////////
								Circle->OnExecute()                               ;

								// Circle In Data ���� ���ο� Circle Center ��ǥ �޾ƿ� 
								RealC1CirCenter.x = Circle->FCP.x;
								RealC1CirCenter.y = Circle->FCP.y;
								CirDiameter1      = (Circle->Diameter)/2.0;

								// Circle In Data���� 
								OrgC1CirInEdgeCount = Circle->FCirclePointCount;
								if(OrgC1CirInEdgeCount>0){
									if(OrgC1CirInPoint!=NULL) delete[] OrgC1CirInPoint;
									OrgC1CirInPoint = new TFPoint[OrgC1CirInEdgeCount];

									for(int k=0; k<OrgC1CirInEdgeCount; k++){
										if(k>MAX_CIRCLE_EDGEPOINT) break;
										OrgC1CirInPoint[k].x = Circle->FCirclePoint[k].x;
										OrgC1CirInPoint[k].y = Circle->FCirclePoint[k].y;
										if(OrgC1CirInPoint[k].x==0.0f && OrgC1CirInPoint[k].y==0.0f) C1CirInEdgeCount++;
									}
								}
								//////////////////////////////////////////////////////////////////////////////////////////////////////

								// Circle Out Data �ٽ� �˻�  
								//////////////////////////////////////////////////////////////////////////////////////////////////////
								if(BtmEdgeThresHold==0) Circle->GImage = &RotateImg      ; //Org Image �Ѱ��� 
								else                    Circle->GImage = &RotateBinaryImg; //Binary Image �Ѱ��� 

								// �˻� �������� ���ؼ� �ҷ� �̰��� �߻��Ҽ� ����....LTPS�� EnCap�з���
								if(IsMarkBtm) Circle->SetTolerance(60.0f);
								else          Circle->SetTolerance(80.0f);
								// 									Circle->SetTolerance(40.0f )      ; // Circle Tolerance 
								Circle->SetDiameter (C1Diameter)  ; // Circle Diameter 
								Circle->SetCircleChoice(1)        ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
								Circle->SetCircleType(2)          ; // 0:Black to White, 1:White to Black 2: Both Black, White 
								Circle->SetCircleIndex (0)        ; // Circle EdgePoint Index 
								// Circle1 OutData EdgePoint 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////
								Circle->OnExecute()               ;

								// Circle Out Data���� 
								OrgC1CirOutEdgeCount = Circle->FCirclePointCount;
								if(OrgC1CirOutEdgeCount>0){
									if(OrgC1CirOutPoint!=NULL) delete[] OrgC1CirOutPoint;
									OrgC1CirOutPoint = new TFPoint[OrgC1CirOutEdgeCount];

									for(int k=0; k<OrgC1CirOutEdgeCount; k++){
										if(k>MAX_CIRCLE_EDGEPOINT) break;
										OrgC1CirOutPoint[k].x = Circle->FCirclePoint[k].x;
										OrgC1CirOutPoint[k].y = Circle->FCirclePoint[k].y;
										// BTM���� Circle�� ������ ���� Defect�˻� ������ ��ǥ��� 
										if(k==0) {BtmEdgePoint[0].x = OrgC1CirOutPoint[k].x; BtmEdgePoint[0].y = OrgC1CirOutPoint[k].y;}
										if(OrgC1CirOutPoint[k].x==0.0f && OrgC1CirOutPoint[k].y==0.0f) C1CirOutEdgeCount++;
									}
								}
								// C1Circle Grind_Width Calculate 
								// TrackMode CirInEdgePoint ���ذ� CirOutEdgePoint �����Ÿ� ����

								if(IsMarkBtm){ // LTPS��� ������ ��� ==> ������ ��� 
									if(OrgC1CirOutEdgeCount!=0 && OrgC1CirInEdgeCount!=0) { // MarkMode��� ������ �˻縦 ���� ���� �� ������ ����  
										GrindMeaDis =0.0f, MinDis =10000.0f;C1GrindMarkCount=0;
										if(CGrindMark[0].Point.x!=0.0f && CGrindMark[0].Point.y!=0.0f){ // Grind Mark ã������� 
											for(int k=0; k<OrgC1CirOutEdgeCount; k++){
												if(OrgC1CirOutPoint[k].x==0.0f || OrgC1CirOutPoint[k].y==0.0f) continue;
												if(CGrindMark[0].Point.x==0.0f || CGrindMark[0].Point.y==0.0f) continue;

												GrindMeaDis = Math.GetLength(CGrindMark[0].Point, OrgC1CirOutPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													C1GrindMarkCount = k;
												}
											}
											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ���� 
											if(C1GrindMarkCount!=0 && C1GrindMarkCount>5 &&(C1GrindMarkCount+5<OrgC1CirOutEdgeCount)){
												CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
												for(int k=C1GrindMarkCount-5; k<C1GrindMarkCount+5; k++){
													if(OrgC1CirOutPoint[k].x==0.0f || OrgC1CirOutPoint[k].y==0.0f) continue;
													GrindMeaDis = Math.GetLength(CGrindMark[0].Point, OrgC1CirOutPoint[k]);
													TotalCirDis+= GrindMeaDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[0].GrindMea = TotalCirDis/CircleCount;
												}
											}

											// GrindMark���� ���� ��հ� ������ ���� 
											if(C1GrindMarkCount!=0 && C1GrindMarkCount>5){
												CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
												for(int k=C1GrindMarkCount-5; k<C1GrindMarkCount+5; k++){
													if(OrgC1CirOutPoint[k].x==0.0f && OrgC1CirOutPoint[k].y==0.0f) continue;
													if(OrgC1CirInPoint [k].x==0.0f && OrgC1CirInPoint [k].y==0.0f) continue;

													GrindWidthDis = Math.GetLength(OrgC1CirInPoint[k], OrgC1CirOutPoint[k]);
													TotalCirDis+=   GrindWidthDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[0].GrindWidth = TotalCirDis/CircleCount;
												}
											}
										}
										else { // Grind Mark �� ã������� (������ 0, ������ �����)
											for(int k=0; k<OrgC1CirOutEdgeCount; k++){
												if(OrgC1CirOutPoint[k].x==0.0f || OrgC1CirOutPoint[k].y==0.0f) continue;

												Cir1GrindRectPoint.x = CGrindMark[0].Rect.left + (CGrindMark[0].Rect.right  - CGrindMark[0].Rect.left)/2; 
												Cir1GrindRectPoint.y = CGrindMark[0].Rect.top  + (CGrindMark[0].Rect.bottom - CGrindMark[0].Rect.top )/2; 

												GrindMeaDis = Math.GetLength(Cir1GrindRectPoint, OrgC1CirOutPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													C1GrindMarkCount = k;
												}
											}
											// GrindMark���� ���� ��հ� ������ ���� 
											if(C1GrindMarkCount!=0 && C1GrindMarkCount>5){
												CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
												for(int k=C1GrindMarkCount-5; k<C1GrindMarkCount+5; k++){
													if(OrgC1CirOutPoint[k].x==0.0f && OrgC1CirOutPoint[k].y==0.0f) continue;
													if(OrgC1CirInPoint [k].x==0.0f && OrgC1CirInPoint [k].y==0.0f) continue;
													GrindWidthDis = Math.GetLength(OrgC1CirInPoint[k], OrgC1CirOutPoint[k]);
													TotalCirDis+=   GrindWidthDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[0].GrindWidth = TotalCirDis/CircleCount;
												}
											}
										}
									}
								}
								else     { // MarkMode ==> EnCap��� �������� ����Ѵ� 
									if(OrgC1CirInEdgeCount==OrgC1CirOutEdgeCount){
										double OutDis=0.0f, InDis=0.0f, TotalGrindWidth=0.0f, AvgGrindWidth=0.0f;C1GrindWidthCount=0;
										for(int k=0; k<OrgC1CirInEdgeCount; k++){
											if(OrgC1CirOutPoint[k].x==0.0f && OrgC1CirOutPoint[k].y==0.0f){
												OutDis =0.0f;
											}
											else {
												OutDis = Math.GetLengthFrPointToPoint(C1CirCenter,OrgC1CirOutPoint[k]);
											}

											if(OrgC1CirInPoint[k].x==0.0f && OrgC1CirInPoint[k].y==0.0f){
												InDis =0.0f;
											}
											else {
												InDis = Math.GetLengthFrPointToPoint(C1CirCenter,OrgC1CirInPoint[k]);
											}

											// CircleFitting ����� ���� EdgePoint �����Ͱ� ���� �� ��츸 �����Ͽ� ����Ѵ�
											if(OutDis!=0.0f && InDis!=0.0f){
												TotalGrindWidth+=(OutDis - InDis);
												C1GrindWidthCount++;
											}
										}
										if(C1GrindWidthCount!=0){
											AvgGrindWidth = (TotalGrindWidth/C1GrindWidthCount);
											CGrindMark[0].GrindWidth = AvgGrindWidth;
										}
									}
								}
								// C1 Grind Mark ���� ������ ���� ������ ����  
								//////////////////////////////////////////////////////////////////////////////////////////////////////

								// Org Circle�������� ��ȣ���� ���� ���� ������ ���� �����ϴ� ������ �������� ���� �޸� ������
								// Circle Defect�˻翡 �ʿ��� Edge������ ��
								int Count=0;
								if(OrgC1CirInEdgeCount>0){
									C1CirInEdgeCount = OrgC1CirInEdgeCount - C1CirInEdgeCount;
									// 										 C1CirInEdgeCount = OrgC1CirInEdgeCount;
									if(C1CirInEdgeCount>0){
										if(C1CirInPoint!=NULL) delete[] C1CirInPoint;
										C1CirInPoint = new TFPoint[C1CirInEdgeCount];

										for(int k=0; k<OrgC1CirInEdgeCount; k++){
											if(k>MAX_CIRCLE_EDGEPOINT) break;
											if(OrgC1CirInPoint[k].x==0.0f && OrgC1CirInPoint[k].y==0.0f) continue;
											C1CirInPoint[Count].x = OrgC1CirInPoint[k].x;
											C1CirInPoint[Count].y = OrgC1CirInPoint[k].y;
											Count++;
										}
									}
									// Circle ���� ������ �޸� ����
									if(OrgC1CirInPoint!=NULL) delete[] OrgC1CirInPoint;
								}
								// FAVION Circle Fitting ����� 
								SampleSize = C1CirInEdgeCount*0.75; //0.5~0.8���Ϸ� �Է��Ұ�
								DistDevRatio = 0.15;                //0.1~0.25 ���Ϸ� �Է��Ұ�
								CircleFit->OnRansac(C1CirInPoint,C1CirInEdgeCount,SampleSize,DistDevRatio,&CircleFit1Center,&CirFitDiameter1);

								if(OrgC1CirOutEdgeCount>0){
									C1CirOutEdgeCount = OrgC1CirOutEdgeCount - C1CirOutEdgeCount;
									C1CirOutEdgeCount = OrgC1CirOutEdgeCount;
									Count =0;
									if(C1CirOutEdgeCount>0){
										if(C1CirOutPoint!=NULL) delete[] C1CirOutPoint;
										C1CirOutPoint = new TFPoint[C1CirOutEdgeCount];

										for(int k=0; k<OrgC1CirOutEdgeCount; k++){
											if(k>MAX_CIRCLE_EDGEPOINT) break;
											if(OrgC1CirOutPoint[k].x==0.0f && OrgC1CirOutPoint[k].y==0.0f) continue;
											C1CirOutPoint[Count].x = OrgC1CirOutPoint[k].x;
											C1CirOutPoint[Count].y = OrgC1CirOutPoint[k].y;
											Count++;
										}
									}
									// Circle ���� ������ �޸� ����
									if(OrgC1CirOutPoint!=NULL) delete[] OrgC1CirOutPoint;
								}
								// Circle ���� �ɹڸ�, �����ڸ� ���� ���� ������(EnCap)
								// ���� ��Ī�� ����.....�ϴ� Crack���� ���� 
								if(!IsMarkBtm && BrokenThresHold!=0){
									RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
									EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBrokenImg);

									EasyImage::Threshold(&RotateImg,&RotateBrokenImg,BrokenThresHold);                 //�ڸ� �˻� ���� �̹��� ��ȯ

									if(CirFitDiameter1!=0.0f && CircleFit1Center.x!=0.0f && CircleFit1Center.y!=0.0f){ // FAVION Circle Fitting ������ �ڸ��˻� ���� 
										Circle->GImage = &RotateBrokenImg;                                             //Binary Image �Ѱ��� 

										Circle->SetCenterXY(CircleFit1Center.x,CircleFit1Center.y)      ;
										Circle->SetTolerance((dEdgeToTrapLineDis/2)-CirOffsetMargin)    ; // Circle Tolerance 
										Circle->SetDiameter ((CirFitDiameter1-(dEdgeToTrapLineDis/2))*2); // Circle Diameter 
										Circle->SetCircleChoice(0)                                      ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
										Circle->SetCircleType  (2)                                      ; // 0:Black to White, 1:White to Black 2: Both Black, White 
										Circle->SetCircleIndex (0)                                      ; // Circle EdgePoint Index 
										Circle->OnExecute(true)                                         ;
										CrackCirDiameter1 = (CirFitDiameter1-(dEdgeToTrapLineDis/2))+((dEdgeToTrapLineDis/2)-CirOffsetMargin);

										C1CirInCrackCount = Circle->FCirclePointCount;

										if(C1CirInCrackCount>0){
											if(C1CirInCrackPoint!=NULL) delete[] C1CirInCrackPoint;
											C1CirInCrackPoint = new TFPoint[C1CirInCrackCount];

											for(int k=0; k<C1CirInCrackCount; k++){
												if(k>MAX_CIRCLE_EDGEPOINT) break;
												C1CirInCrackPoint[k].x = Circle->FCirclePoint[k].x;
												C1CirInCrackPoint[k].y = Circle->FCirclePoint[k].y;
											}
										}
									}
								}
							}
						}
					}
				}
				if(FRightBottom){
					if(IsCornerBtm){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, ECThresHold);               // Corner Out GinrdThresHold 
						EasyImage::Threshold(&RotateImg,&RotateGrindBinaryImg, CorGrindMeaThresHold); // Corner Measure GrindThresHold(3Point)

						if(ECThresHold==0)    {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
						else                  {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

						SingleLine->IRoi->SetOrgX  ((int)FRBPoint.x-RBCornerWidth );
						SingleLine->IRoi->SetOrgY  ((int)FRBPoint.y-RBCornerHeight);
						SingleLine->IRoi->SetWidth ((int)RBCornerWidth+10         );
						SingleLine->IRoi->SetHeight((int)RBCornerHeight           );

						SingleLine->SetOrientation(orEast);
						SingleLine->FIncrement =1;
						SingleLine->FThreshold =15;
						SingleLine->SetFindLineId(0);
						SingleLine->SetEdgeType(1);  //White To Black 
						SingleLine->OnExecute();

						Line[6].FP0 = SingleLine->GetResultP0();
						Line[6].FP1 = SingleLine->GetResultP1();

						// ORG 
						FRBRPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall ].FP0, Line[waRightWall] .FP1);
						FRBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);
						BtmEdgePoint[1].x = FRBBPoint.x;
						BtmEdgePoint[1].y = FRBBPoint.y;

						if(CMarkToGrindMarkDvX==0.0f && CMarkToGrindMarkDvY==0.0f){ // Corner Chamber Cut 3Point 
							if((FRBRPoint.x==0 || FRBRPoint.y==0 || FRBBPoint.x==0 || FRBBPoint.y==0) || (SingleLine->FEdgePointCount <30)){
								RBDx = 0.0f;
								RBDy = 0.0f;
								//ȭ�鿡 ���÷��� ���ؼ� 
								FRBRPoint.x = FRBPoint.x;
								FRBRPoint.y = FRBPoint.y;
								FRBBPoint.x = FRBPoint.x;
								FRBBPoint.y = FRBPoint.y;
							}
							else { 
								RBDx = Math.GetLengthFrPointToPoint(FRBPoint, FRBBPoint);
								RBDy = Math.GetLengthFrPointToPoint(FRBPoint, FRBRPoint);

								// OutLine C4Corner Data 
								if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
									if(C4Point!=NULL) delete[] C4Point;
									C4Point  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
									C4EdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

									CornerCount =0;  
									for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
										if(CornerCount>=MAX_CORNER_POINT) break;
										C4Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
										C4Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
										CornerCount++;
									}
								}

								// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
								// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
								if(CorGrindInThresHold!=0){
									EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
									SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

									if(RBCornerWidth >(RBCornerHeight*1.5)){
										SingleLine->IRoi->SetOrgX  ((int)FRBBPoint.x                 ) ;
										SingleLine->IRoi->SetOrgY  ((int)FRBBPoint.y - RBCornerHeight) ;
										SingleLine->IRoi->SetWidth ((int)(RBCornerWidth+10           ));
										SingleLine->IRoi->SetHeight((int)(RBCornerHeight             ));

										if(FRBBPoint.x<=0    || FRBBPoint.y - RBCornerHeight<=0    || RBCornerWidth+10<=0    || RBCornerHeight+10 <=0    ) return false;
										if(FRBBPoint.x>=ImgW || FRBBPoint.y - RBCornerHeight>=ImgH || RBCornerWidth+10>=ImgW || RBCornerHeight+10 >=ImgH ) return false;

										SingleLine->SetOrientation(orSouth);
										SingleLine->FIncrement =1;
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //White To Black 
										SingleLine->OnExecute();
									}
									else {
										// Corner ���� ���ʶ��� ã�´� 
										SingleLine->SetOrientation(orEast);
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //White To Black 
										SingleLine->OnExecute();
									}

									// InLine C4Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C4InPoint!=NULL) delete[] C4InPoint;
										C4InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C4InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C4InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C4InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
									FRBRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall ].FP0, Line[waRightWall ].FP1);
									FRBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);
									BtmEdgePoint[1].x = FRBBInPoint.x;
									BtmEdgePoint[1].y = FRBBInPoint.y;

									//�ڳ� ������ 
									if(IsMarkBtm){
										Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
										for(int k=0; k<C4InEdgeCount; k++){
											Dis = Math.GetLengthFrLineToPoint(FLBLPoint, FLBBPoint, C4InPoint[k]);
											TotalGrindWidth+=Dis;
										}
										if(C4InEdgeCount!=0){
											AvgGrindWidth = (TotalGrindWidth/C4InEdgeCount);
											C4CorGrindWd  = AvgGrindWidth;
										}
									}
								}

								// �ڳ� ������ ���� ������ ã�´� 
								// ROI ���� ����(3���� ��� ���� ���� ������ ã�´�)
								if(CorGrindMeaThresHold!=0){
									if(CorGrindMeaThresHold==0){SingleLine->GImage = &RotateImg           ;  SingleLine->FThreshold =5 ;}
									else                       {SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold =10;}

									if(RBCornerWidth >(RBCornerHeight*1.5)){
										SingleLine->IRoi->SetOrgX  ((int)FRBPoint.x -RBCornerWidth     );
										SingleLine->IRoi->SetOrgY  ((int)FRBPoint.y -RBCornerHeight +30);
										SingleLine->IRoi->SetWidth ((int)RBCornerWidth-50              );
										SingleLine->IRoi->SetHeight((int)RBCornerHeight-40             );

										SingleLine->SetOrientation(orSouth);
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();
									}
									else                                   {
										SingleLine->IRoi->SetOrgX  ((int)FRBPoint.x -RBCornerWidth     );
										SingleLine->IRoi->SetOrgY  ((int)FRBPoint.y -RBCornerHeight +30);
										SingleLine->IRoi->SetWidth ((int)RBCornerWidth-50              );
										SingleLine->IRoi->SetHeight((int)RBCornerHeight-40             );

										SingleLine->SetOrientation(orEast);
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();
									}

									//�ڳ� ������ ���� ���ΰ� ���ο� ������ ���� 
									FRBRMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall ].FP0, Line[waRightWall ].FP1);
									FRBBMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);

									// C4Corner Grind Measure Stand Data 
									if((SingleLine->FEdgePointCount)>0){
										if(C4MeaPoint!=NULL) delete[] C4MeaPoint;
										C4MeaPoint     = new TFPoint [(SingleLine->FEdgePointCount)];
										C4MeaEdgeCount = (SingleLine->FEdgePointCount);

										CornerCount =0;  
										for(int k=0; k<(SingleLine->FEdgePointCount); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C4MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C4MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}
									//�ڳ� ������ ���  
									Dis =0.0f, AvgGrindMea=0.0f, TotalGrindMeasure=0.0f;
									for(int k=0; k<C4MeaEdgeCount; k++){
										Dis = Math.GetLengthFrLineToPoint(FRBRPoint, FRBBPoint, C4MeaPoint[k]);
										TotalGrindMeasure+=Dis;
									}
									if(C4MeaEdgeCount!=0){
										AvgGrindMea    = (TotalGrindMeasure/C4MeaEdgeCount);
										C4CorGrindMea  = AvgGrindMea;
									}
								}
							}	
						}
						else                                                      { // Corner Chamber Cut GrindMark 
							TFPoint C4GrindRectPoint(0.0f, 0.0f);
							if((FRBRPoint.x==0 || FRBRPoint.y==0 || FRBBPoint.x==0 || FRBBPoint.y==0) || (SingleLine->FEdgePointCount <30)){
								RBDx = 0.0f;
								RBDy = 0.0f;
								//ȭ�鿡 ���÷��� ���ؼ� 
								FRBRPoint.x = FRBPoint.x;
								FRBRPoint.y = FRBPoint.y;
								FRBBPoint.x = FRBPoint.x;
								FRBBPoint.y = FRBPoint.y;
							}
							else { 
								RBDx = Math.GetLengthFrPointToPoint(FRBPoint, FRBBPoint);
								RBDy = Math.GetLengthFrPointToPoint(FRBPoint, FRBRPoint);

								// OutLine C4Corner Data 
								if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
									if(C4Point!=NULL) delete[] C4Point;
									C4Point  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
									C4EdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

									CornerCount =0;  
									for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
										if(CornerCount>=MAX_CORNER_POINT) break;
										C4Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
										C4Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
										CornerCount++;
									}
								}

								// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
								// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
								if(CorGrindInThresHold!=0){
									EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
									SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

									if(RBCornerWidth >(RBCornerHeight*1.5)){
										SingleLine->IRoi->SetOrgX  ((int)FRBBPoint.x                 ) ;
										SingleLine->IRoi->SetOrgY  ((int)FRBBPoint.y - RBCornerHeight) ;
										SingleLine->IRoi->SetWidth ((int)(RBCornerWidth+10           ));
										SingleLine->IRoi->SetHeight((int)(RBCornerHeight             ));

										if(FRBBPoint.x<=0    || FRBBPoint.y - RBCornerHeight<=0    || RBCornerWidth+10<=0    || RBCornerHeight+10 <=0    ) return false;
										if(FRBBPoint.x>=ImgW || FRBBPoint.y - RBCornerHeight>=ImgH || RBCornerWidth+10>=ImgW || RBCornerHeight+10 >=ImgH ) return false;

										SingleLine->SetOrientation(orSouth);
										SingleLine->FIncrement =1;
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();
									}
									else {
										// Corner ���� ���ʶ��� ã�´� 
										SingleLine->SetOrientation(orEast);
										SingleLine->FIncrement =1;
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();
									}
									// InLine C4Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(C4InPoint!=NULL) delete[] C4InPoint;
										C4InPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										C4InEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											C4InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C4InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
									FRBRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall ].FP0, Line[waRightWall ].FP1);
									FRBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);
									BtmEdgePoint[1].x = FRBBInPoint.x;
									BtmEdgePoint[1].y = FRBBInPoint.y;
								}

								// GrindMark���� ������,������ ������ 
								// RightBtm 
								if(IsMarkBtm){
									CGrindMark[1].Rect.left   = (FRBPoint.x - CMarkToGrindMarkDvX) -GrindMarkRange;
									CGrindMark[1].Rect.top    = (FRBPoint.y - CMarkToGrindMarkDvY) -GrindMarkRange;
									CGrindMark[1].Rect.right  = CGrindMark[1].Rect.left+GrindMarkLength;
									CGrindMark[1].Rect.bottom = CGrindMark[1].Rect.top +GrindMarkLength;

									// �����̹������� Mark ã�´� 
									Find->OnLoad(true,FindDir)                     ;
									Find->GImage = &RotateImg                      ;
									Find->SetContrastMode(EFindContrastMode_Normal);
									Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind �˻� �ּ� Score
									Find->SetZeroScore(GrindPadMarkMinScore)       ; // �˻� �� �ּ� Score 
									Find->SetMaxInstances(1)                       ;
									Find->SetAngleTolerance(20.0f)                 ;
									Find->SetAngleBias(315.0f)                     ;
									Find->SetScaleBias(1.00f)                      ;
									Find->SetScaleTolerance(0.0f)                  ;
									Find->Property.Ox = (float)ImgW/2              ;
									Find->Property.Oy = (float)ImgH/2              ;

									GrindRectWidth  = (CGrindMark[1].Rect.right  - CGrindMark[1].Rect.left);
									GrindRectHeight = (CGrindMark[1].Rect.bottom - CGrindMark[1].Rect.top );
									Find->IRoi->SetPlacement(CGrindMark[1].Rect.left,CGrindMark[1].Rect.top,GrindRectWidth,GrindRectHeight);
									if(CGrindMark[1].Rect.left<=0    || CGrindMark[1].Rect.top<=0    || GrindRectWidth<=0    || GrindRectHeight<=0   ) return false;
									if(CGrindMark[1].Rect.left>=ImgW || CGrindMark[1].Rect.top>=ImgH || GrindRectWidth>=ImgW || GrindRectHeight>=ImgH) return false;

									// �˻� �� Match Pos ��ǥ �޾�´� 
									// Find �˻� ������ ������ �޾ƿ� 
									if(Find->OnExecute()){
										CGrindMark[1].Point.x = Find->Result[0].Px;
										CGrindMark[1].Point.y = Find->Result[0].Py;
										CGrindMark[1].Score   = Find->Result[0].Score;
									}
									// GrindMakrã����� ������,������ ������
									if(CGrindMark[1].Point.x!=0.0f && CGrindMark[1].Point.y!=0.0f){
										// GrindMark���� OutData �ִܰŸ� Count���� Calc 
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
										for(int k=0; k<C4EdgeCount; k++){
											if(C4Point[k].x         ==0.0f || C4Point[k].y         ==0.0f) continue;
											if(CGrindMark[1].Point.x==0.0f || CGrindMark[1].Point.y==0.0f) continue;

											GrindMeaDis = Math.GetLength(CGrindMark[1].Point, C4Point[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkOutCnt = k;
											}
										}
										// OutData���� InData �ִܰŸ� Count���� Calc 
										if(GrindMarkOutCnt!=0){
											GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
											for(int k=0; k<C4InEdgeCount; k++){
												if(C4InPoint[k].x   ==0.0f          || C4InPoint[k].y             ==0.0f) continue;
												if(C4Point[GrindMarkOutCnt].x==0.0f || C4Point[GrindMarkOutCnt].y ==0.0f) continue;

												GrindMeaDis = Math.GetLength(C4Point[GrindMarkOutCnt], C4InPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													GrindMarkInCnt = k;
												}
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
										if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5 && (GrindMarkOutCnt+5<C4EdgeCount)){
											CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
											for(int k=GrindMarkOutCnt-5; k<GrindMarkOutCnt+5; k++){
												if(C4Point[k].x==0.0f || C4Point[k].y==0.0f) continue;
												if(k>=C4EdgeCount                          ) continue;

												GrindMeaDis = Math.GetLength(CGrindMark[1].Point, C4Point[k]);
												TotalCirDis+= GrindMeaDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												CGrindMark[1].GrindMea = TotalCirDis/CircleCount;
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
										if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
											CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
											for(int k=-5; k<5; k++){
												if(C4Point  [GrindMarkOutCnt+k].x==0.0f && C4Point  [GrindMarkOutCnt+k].y==0.0f) continue;
												if(C4InPoint[GrindMarkInCnt +k].x==0.0f && C4InPoint[GrindMarkInCnt +k].y==0.0f) continue;
												if((GrindMarkOutCnt+k)>=C4EdgeCount                                            ) continue;
												if((GrindMarkInCnt +k)>=C4InEdgeCount                                          ) continue;

												GrindWidthDis = Math.GetLength(C4InPoint[GrindMarkInCnt+k], C4Point[GrindMarkOutCnt+k]);
												TotalCirDis+=   GrindWidthDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												CGrindMark[1].GrindWidth  = TotalCirDis/CircleCount;
											}
										}
									}
									else { // GrindMrk ��ã����� �������� ������ 
										// OutData���� �ּҰ� ������ Count���� 
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
										for(int k=0; k<C4EdgeCount; k++){
											if(C4Point[k].x==0.0f || C4Point[k].y==0.0f) continue;

											C4GrindRectPoint.x = CGrindMark[1].Rect.left + (CGrindMark[1].Rect.right  - CGrindMark[1].Rect.left)/2; 
											C4GrindRectPoint.y = CGrindMark[1].Rect.top  + (CGrindMark[1].Rect.bottom - CGrindMark[1].Rect.top )/2; 

											GrindMeaDis = Math.GetLength(C4GrindRectPoint, C4Point[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkOutCnt = k;
											}
										}

										// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
										if(GrindMarkOutCnt!=0){
											GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
											for(int k=0; k<C4InEdgeCount; k++){
												if(C4InPoint[k].x            ==0.0f || C4InPoint[k].y             ==0.0f) continue;
												if(C4Point[GrindMarkOutCnt].x==0.0f || C4Point[GrindMarkOutCnt].y ==0.0f) continue;

												GrindMeaDis = Math.GetLength(C4Point[GrindMarkOutCnt], C4InPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													GrindMarkInCnt = k;
												}
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
										if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
											CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
											for(int k=-5; k<5; k++){
												if(C4Point  [GrindMarkOutCnt+k].x==0.0f && C4Point  [GrindMarkOutCnt+k].y==0.0f) continue;
												if(C4InPoint[GrindMarkInCnt +k].x==0.0f && C4InPoint[GrindMarkInCnt +k].y==0.0f) continue;
												if((GrindMarkOutCnt+k)>=C4EdgeCount                                            ) continue;
												if((GrindMarkInCnt +k)>=C4InEdgeCount                                          ) continue;

												GrindWidthDis = Math.GetLength(C4InPoint[GrindMarkInCnt+k], C4Point[GrindMarkOutCnt+k]);
												TotalCirDis+=   GrindWidthDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												CGrindMark[1].GrindWidth = TotalCirDis/CircleCount;
											}
										}
									}
								}
							}	
						}
					}
					else if(IsRectBtm){ // Rect Cut 
						RBDx = 0.0f;
						RBDy = 0.0f;
						//ȭ�鿡 ���÷��� ���ؼ� 
						FRBRPoint.x = FRBPoint.x;
						FRBRPoint.y = FRBPoint.y;
						FRBBPoint.x = FRBPoint.x;
						FRBBPoint.y = FRBPoint.y;
						// Btm�������� �������� ����,���� ���� �� Defect�˻翡 �ʿ��� ���� ��ǥ 
						BtmEdgePoint[1].x =FRBPoint.x;
						BtmEdgePoint[1].y =FRBPoint.y;
					}
					else if(IsCircleBtm){ // Circle Cut(RightBtm )
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg       , BtmEdgeThresHold    ); // Circle Grind Edge(Polygon_Mode, Mark_Mode���)
						EasyImage::Threshold(&RotateImg,&RotateGrindInBinaryImg, CirGrindInThresHold ); // Circle Grind In_EdgePoint ���� (Mark_Modr���)
						TFPoint Cir2GrindRectPoint(0.0f, 0.0f);

						if(IsMarkBtm){ // GrindMark Search 
							if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f){
								// �����̹������� Mark ã�´� 
								Find->OnLoad(true,FindDir)                     ;
								Find->GImage = &RotateImg                      ;
								Find->SetContrastMode(EFindContrastMode_Normal);
								Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind �˻� �ּ� Score
								Find->SetZeroScore(GrindPadMarkMinScore)       ; // �˻� �� �ּ� Score 
								Find->SetMaxInstances(1)                       ;
								Find->SetAngleTolerance(20.0f)                 ;
								Find->SetScaleBias(1.00f)                      ;
								Find->SetScaleTolerance(0.0f)                  ;
								Find->Property.Ox = (float)ImgW/2              ;
								Find->Property.Oy = (float)ImgH/2              ;

								// ù��° GrindMark���� ���� C1 Search 
								if(CMarkToGrindMarkDvX!=0.0f && CMarkToGrindMarkDvY!=0.0f){
									CGrindMark[1].Rect.left   = (FRBPoint.x-CMarkToGrindMarkDvX)-40;
									CGrindMark[1].Rect.top    = (FRBPoint.y-CMarkToGrindMarkDvY)-40;
									CGrindMark[1].Rect.right  = (CGrindMark[1].Rect.left + 80);
									CGrindMark[1].Rect.bottom = (CGrindMark[1].Rect.top  + 80);

									GrindRectWidth  = (CGrindMark[1].Rect.right  - CGrindMark[1].Rect.left);
									GrindRectHeight = (CGrindMark[1].Rect.bottom - CGrindMark[1].Rect.top );

									Find->IRoi->SetPlacement(CGrindMark[1].Rect.left,CGrindMark[1].Rect.top,GrindRectWidth,GrindRectHeight);
									if(CGrindMark[1].Rect.left<=0 || CGrindMark[1].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

									// �˻� �� Match Pos ��ǥ �޾�´� 
									// Find �˻� ������ ������ �޾ƿ� 
									if(RqMode == true)
										Find->SetAngleBias(35.0f);
									else
										Find->SetAngleBias(45.0f);
									//Find->SetAngleBias(45.0f);
									if(Find->OnExecute()){
										CGrindMark[1].Point.x = Find->Result[0].Px;
										CGrindMark[1].Point.y = Find->Result[0].Py;
										CGrindMark[1].Score   = Find->Result[0].Score;
									}
								}
							}
						}

						if(FInspectMode==Insp_Polygon){ //RightBottom
							if(CSideCircleWidth!=0.0f && CSideCircleHeight!=0.0f  ){ // Notch�� �¿� �������� 
								NotchPolyRect[1].left  = (long)(FRBPoint.x-CSideCircleWidth);  NotchPolyRect[1].top    = (long)(FRBPoint.y - CSideCircleHeight);
								NotchPolyRect[1].right = (long)(FRBPoint.x                 );  NotchPolyRect[1].bottom = (long)(FLBPoint.y                    );
							}
							TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
							// Polygon Algorithm �⺻ ����  
							Polygon->IWidth          = ImgW;
							Polygon->IHeight         = ImgH;
							Polygon->PoPa.MinPxCnt   =300  ;
							Polygon->PoPa.MaxPxCnt   =20000;
							Polygon->PoPa.MaxBbCnt   =10   ;

							if(NotchPolyRect[1].left!=0 && NotchPolyRect[1].top!=0 && NotchPolyRect[1].right!=0 && NotchPolyRect[1].bottom!=0){
								P1.x=0, P1.y=0, P2.x=0, P2.y=0, P3.x=0, P3.y=0, P4.x=0, P4.y=0;
								LogUnit.SetLog(L"CellID:%d, CamID:%d ==>Polygon6_Insp_Start",CellId,CamId);     

								P1.x = (NotchPolyRect[1].left); P1.y =(NotchPolyRect[1].top             ); P2.x = (P1.x+NotchPolyRect[1].Width()); P2.y = (P1.y);
								P3.x = (NotchPolyRect[1].left); P3.y =(P1.y+NotchPolyRect[1].Height()+50); P4.x = (P2.x                         ); P4.y = (P3.y);

								// Median Filter ���� 
								if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
								if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
								if( (P1.x + int(P2.x - P1.x)) >= ImgW || (P1.y + int(P3.y - P1.y)) >= ImgH ) return false;
								FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
								FilterROI.Attach(&RotateBinaryImg)                                    ;
								FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
								LogUnit.SetLog(L"Polygon6 ==>Px :%d, Py :%d, Width:%d, Height:%d ==>Median Filter",P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
								LogUnit.SetLog(L"Polygon6 ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
								EasyImage::Median(&FilterROI, &FilterROI)                             ;

								Polygon->GImage          = &RotateBinaryImg           ;
								Polygon->PoPa.FMaxWidth  =(int)(CSideCircleWidth+100 );
								Polygon->PoPa.FMaxHeight =(int)(CSideCircleHeight+100);
								Polygon->PoPa.IsSeparate =true                        ;
								Polygon->PoPa.IsInToOut  =true                        ;
								Polygon->PoPa.HiThHold   = BtmEdgeThresHold           ;
								Polygon->PoPa.LoThHold   = 0                          ;
								Polygon->PoPa.FMinWidth  =(int)(CSideCircleWidth /2)  ;
								Polygon->PoPa.FMinHeight =(int)(CSideCircleHeight/2)  ;

								if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
									Polygon->OnExecute(P1,P2,P3,P4);
								}

								// Polygon Algorithm InEdgeData/OutEdgeData Separate �� ������ ���� 
								if(Polygon->BlobCount==1){
									PolygonMarginCnt =10;
									if(Polygon->Blob[0].InCnt>(PolygonMarginCnt*2)){
										C2CirInEdgeCount=0;
										C2CirInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt*2));
										if(C2CirInEdgeCount>0){
											if(C2CirInPoint!=NULL) {
												delete[] C2CirInPoint;
											}
											C2CirInPoint = new TFPoint[C2CirInEdgeCount];

											PixelCnt=0;
											for(int k=0; k<Polygon->Blob[0].InCnt; k++){
												if(k<PolygonMarginCnt || k>=(Polygon->Blob[0].InCnt-PolygonMarginCnt)) continue;
												C2CirInPoint[PixelCnt].x   = Polygon->Blob[0].InPoint[k].x ;
												C2CirInPoint[PixelCnt].y   = Polygon->Blob[0].InPoint[k].y ;
												PixelCnt++;
											}
										}
									}

									if(Polygon->Blob[0].OutCnt>(PolygonMarginCnt*2)){
										C2CirOutEdgeCount=0;
										C2CirOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt*2));
										if(C2CirOutEdgeCount>0){
											if(C2CirOutPoint!=NULL) delete[] C2CirOutPoint;
											C2CirOutPoint = new TFPoint[C2CirOutEdgeCount];

											PixelCnt=0;
											for(int k=0; k<Polygon->Blob[0].OutCnt; k++){
												if(k<PolygonMarginCnt || k>=(Polygon->Blob[0].OutCnt-PolygonMarginCnt)) continue;
												C2CirOutPoint[PixelCnt].x  = Polygon->Blob[0].OutPoint[k].x ;
												C2CirOutPoint[PixelCnt].y  = Polygon->Blob[0].OutPoint[k].y ;

												if(k==PolygonMarginCnt){ //���������� �ʿ��� ������  
													BtmEdgePoint[1].x = C2CirOutPoint[PixelCnt].x;
													BtmEdgePoint[1].y = C2CirOutPoint[PixelCnt].y;
												}
												PixelCnt++;
											}
										}
									}
								}
								/////////////////////////////////////////////////////////////////////////////////////////////

								// InEdgeData/OutEdgeData ���� ������ �� ������ ���
								// Polygon �˰��򿡼��� GrindMark���� OutData,InData �ִܰŸ� 2�� Tag�� �����ؾ��� 
								if(IsMarkBtm){ // LTPS ������,������ ��� 
									if(C2CirOutEdgeCount!=0 && C2CirInEdgeCount!=0){ // MarkMode��� ������ �˻縦 ���� ���� �� ������ ���� 
										if(CGrindMark[1].Point.x!=0.0f && CGrindMark[1].Point.y!=0.0f){  //GrindMark ã������� 
											// GrindMark���� OutData �ִܰŸ� Count���� Calc 
											GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
											for(int k=0; k<C2CirOutEdgeCount; k++){
												if(C2CirOutPoint[k].x   ==0.0f || C2CirOutPoint[k].y  ==0.0f ) continue;
												if(CGrindMark[1].Point.x==0.0f || CGrindMark[1].Point.y==0.0f) continue;

												GrindMeaDis = Math.GetLength(CGrindMark[1].Point, C2CirOutPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													GrindMarkOutCnt = k;
												}
											}

											// GrindMark���� InData �ִܰŸ� Count���� Calc 
											if(GrindMarkOutCnt!=0){
												GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
												for(int k=0; k<C2CirInEdgeCount; k++){
													if(C2CirInPoint[k].x   ==0.0f             || C2CirInPoint[k].y                ==0.0f) continue;
													if(C2CirOutPoint[GrindMarkOutCnt].x==0.0f || C2CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

													GrindMeaDis = Math.GetLength(C2CirOutPoint[GrindMarkOutCnt], C2CirInPoint[k]);
													if(GrindMeaDis<MinDis){
														MinDis = GrindMeaDis;
														GrindMarkInCnt = k;
													}
												}
											}

											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
											if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5){
												CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
												for(int k=GrindMarkOutCnt-5; k<GrindMarkOutCnt+5; k++){
													if(C2CirOutPoint[k].x==0.0f || C2CirOutPoint[k].y==0.0f) continue;
													if(k>=C2CirOutEdgeCount                                ) continue;

													GrindMeaDis = Math.GetLength(CGrindMark[1].Point,C2CirOutPoint[k]);
													TotalCirDis+= GrindMeaDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[1].GrindMea = TotalCirDis/CircleCount;
													CGrindMark[1].GrindMea = CGrindMark[1].GrindMea+0.5; //SubPixel 
												}
											}

											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
											if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
												CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
												for(int k=-5; k<5; k++){
													if(C2CirOutPoint[GrindMarkOutCnt+k].x==0.0f && C2CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
													if(C2CirInPoint [GrindMarkInCnt +k].x==0.0f && C2CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
													if((GrindMarkOutCnt+k)>=C2CirOutEdgeCount                                              ) continue;
													if((GrindMarkInCnt +k)>=C2CirInEdgeCount                                               ) continue;

													GrindWidthDis = Math.GetLength(C2CirInPoint[GrindMarkInCnt+k], C2CirOutPoint[GrindMarkOutCnt+k]);
													TotalCirDis+=   GrindWidthDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[1].GrindWidth = TotalCirDis/CircleCount;
													CGrindMark[1].GrindWidth = CGrindMark[1].GrindWidth+0.5; //SubPixel
												}
											}
										}
										else if(CGrindMark[1].Rect.left!=0 && CGrindMark[1].Rect.top!=0 && CGrindMark[1].Rect.right!=0 && CGrindMark[1].Rect.bottom!=0){  //GrindMark �� ã�������
											// OutData���� �ּҰ� ������ Count���� 
											GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
											for(int k=0; k<C2CirOutEdgeCount; k++){
												if(C2CirOutPoint[k].x==0.0f || C2CirOutPoint[k].y==0.0f) continue;

												Cir2GrindRectPoint.x = CGrindMark[1].Rect.left + (CGrindMark[1].Rect.right  - CGrindMark[1].Rect.left)/2; 
												Cir2GrindRectPoint.y = CGrindMark[1].Rect.top  + (CGrindMark[1].Rect.bottom - CGrindMark[1].Rect.top )/2; 

												GrindMeaDis = Math.GetLength(Cir2GrindRectPoint, C2CirOutPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													GrindMarkOutCnt = k;
												}
											}

											// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
											if(GrindMarkOutCnt!=0){
												GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
												for(int k=0; k<C2CirInEdgeCount; k++){
													if(C2CirInPoint[k].x   ==0.0f             || C2CirInPoint[k].y                ==0.0f) continue;
													if(C2CirOutPoint[GrindMarkOutCnt].x==0.0f || C2CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

													GrindMeaDis = Math.GetLength(C2CirOutPoint[GrindMarkOutCnt], C2CirInPoint[k]);
													if(GrindMeaDis<MinDis){
														MinDis = GrindMeaDis;
														GrindMarkInCnt = k;
													}
												}
											}

											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
											if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
												CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
												for(int k=-5; k<5; k++){
													if(C2CirOutPoint[GrindMarkOutCnt+k].x==0.0f && C2CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
													if(C2CirInPoint [GrindMarkInCnt +k].x==0.0f && C2CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
													if((GrindMarkOutCnt+k)>=C2CirOutEdgeCount                                              ) continue;
													if((GrindMarkInCnt +k)>=C2CirInEdgeCount                                               ) continue;

													GrindWidthDis = Math.GetLength(C2CirInPoint[GrindMarkInCnt+k], C2CirOutPoint[GrindMarkOutCnt+k]);
													TotalCirDis+=   GrindWidthDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[1].GrindWidth = TotalCirDis/CircleCount;
													CGrindMark[1].GrindWidth = CGrindMark[1].GrindWidth+0.5; //SubPixel
												}
											}
										}
									}
								}
								else         { //Polygon EnCap ���� OutPoint �߰��� ���� ���� �Ÿ� ��հ��� ����Ѵ�(LeftTop) 
									if(C2CirInEdgeCount!=0 && C2CirOutEdgeCount!=0){
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
										GrindMarkOutCnt = (int)(C2CirOutEdgeCount/2);
										// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
										if(GrindMarkOutCnt!=0){
											GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
											for(int k=0; k<C2CirInEdgeCount; k++){
												if(C2CirInPoint[k].x   ==0.0f             || C2CirInPoint[k].y                ==0.0f) continue;
												if(C2CirOutPoint[GrindMarkOutCnt].x==0.0f || C2CirOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

												GrindMeaDis = Math.GetLength(C2CirOutPoint[GrindMarkOutCnt], C2CirInPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													GrindMarkInCnt = k;
												}
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
										if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
											CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
											for(int k=-5; k<5; k++){
												if(C2CirOutPoint[GrindMarkOutCnt+k].x==0.0f && C2CirOutPoint[GrindMarkOutCnt+k].y==0.0f) continue;
												if(C2CirInPoint [GrindMarkInCnt +k].x==0.0f && C2CirInPoint [GrindMarkInCnt +k].y==0.0f) continue;
												if((GrindMarkOutCnt+k)>=C2CirOutEdgeCount                                              ) continue;
												if((GrindMarkInCnt +k)>=C2CirInEdgeCount                                               ) continue;

												GrindWidthDis = Math.GetLength(C2CirInPoint[GrindMarkInCnt+k], C2CirOutPoint[GrindMarkOutCnt+k]);
												TotalCirDis+=   GrindWidthDis;
												CircleCount++;
											}
											if(TotalCirDis!=0.0f && CircleCount!=0){
												CGrindMark[1].GrindWidth = TotalCirDis/CircleCount;
												CGrindMark[1].GrindWidth = CGrindMark[1].GrindWidth+0.5; //SubPixel
											}
										}
									}
								}
								///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								// FAVION Circle Fitting �����(Circle_1)
								SampleSize   = C2CirInEdgeCount*0.75; //0.5~0.8���Ϸ� �Է��Ұ�
								DistDevRatio = 0.15                 ; //0.1~0.25 ���Ϸ� �Է��Ұ�
								CircleFit->OnRansac(C2CirInPoint,C2CirInEdgeCount,SampleSize,DistDevRatio,&CircleFit2Center,&CirFitDiameter2);

								// Polygon �ڸ� �˻� ������ 
								if(!IsMarkBtm && BrokenThresHold!=0){
									RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
									EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBrokenImg);
									EasyImage::Threshold(&RotateImg,&RotateBrokenImg, BrokenThresHold);
									// 											 RotateBrokenImg.Save("D:\\ROTATE_IMG\\Polygon_�ϴܹڸ�.bmp");
									Edge->GImage = &RotateBrokenImg;
									Edge->SetEdgeType(0);

									int DvCnt =0, DataCount=0;
									double deg =0.0f; 
									TFPoint Target(0.0f, 0.0f);

									if(GrindMarkInCnt>GrindMarkOutCnt){
										DvCnt = GrindMarkInCnt - GrindMarkOutCnt;
										for(int k=0; k<C2CirOutEdgeCount; k++){
											deg =0.0f, Target.x =0.0f, Target.y =0.0f;
											if(((k+DvCnt)<C2CirInEdgeCount) && C2CirInPoint[k+DvCnt].x>0  && C2CirInPoint[k+DvCnt].y>0){
												deg = Math.GetDegree(C2CirOutPoint[k],C2CirInPoint[k+DvCnt]);

												Target  = Math.OnFindPerpendicularPointXY(C2CirOutPoint[k],-dEdgeToTrapLineDis,-dEdgeToTrapLineDis,deg);
												Edge->OnExecute(C2CirOutPoint[k],Target); // �����ܰ����ο��� Target���� White Point Search 
												if(Edge->GetResultEdgePointCount()>0){
													C2CircleData[DataCount].Dis =0.0f;
													C2CircleData[DataCount].Dv  =0.0f;
													C2CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
													C2CircleData[DataCount].P.y = C2CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
													DataCount++;
												}
											}
										}
										C2CirInCrackCount = DataCount;
									}
									else if(GrindMarkInCnt<GrindMarkOutCnt)   {
										DvCnt = GrindMarkOutCnt - GrindMarkInCnt;
										for(int k=DvCnt; k<C2CirOutEdgeCount; k++){
											deg =0.0f, Target.x =0.0f, Target.y =0.0f;
											if(((k-DvCnt)<C2CirInEdgeCount) && C2CirInPoint[k-DvCnt].x>0  && C2CirInPoint[k-DvCnt].y>0 && k-DvCnt>=0){
												deg = Math.GetDegree(C2CirOutPoint[k],C2CirInPoint[k-DvCnt]);

												Target  = Math.OnFindPerpendicularPointXY(C2CirOutPoint[k],-dEdgeToTrapLineDis,-dEdgeToTrapLineDis,deg);
												Edge->OnExecute(C2CirOutPoint[k],Target); // �����ܰ����ο��� Target���� White Point Search 
												if(Edge->GetResultEdgePointCount()>0){
													C2CircleData[DataCount].Dis =0.0f;
													C2CircleData[DataCount].Dv  =0.0f;
													C2CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
													C2CircleData[DataCount].P.y = C2CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
													DataCount++;
												}
											}
										}
										C2CirInCrackCount = DataCount;
									}
								}
								LogUnit.SetLog(L"CellID:%d, CamID:%d ==>Polygon6_Insp_End",CellId,CamId);     
							}
						}
						else if (FInspectMode==Insp_Mark) { //Circle(RightBtm)
							if(CSideCircleWidth!=0.0f && CSideCircleHeight!=0.0f){
								C2CirCenter.x = FRBPoint.x - CSideCircleWidth - dCirOffsetX;  C2CirCenter.y = FRBPoint.y -  CSideCircleHeight;  
							}

							if(C2CirCenter.x!=0 && C2CirCenter.y!=0){
								LogUnit.SetLog(L"Circle_6_Insp_Start");    

								if(CirGrindInThresHold==0) Circle->GImage = &RotateImg             ; //Org Image �Ѱ��� 
								else                       Circle->GImage = &RotateGrindInBinaryImg; //Binary Image �Ѱ��� 

								Circle->SetCenterXY(C2CirCenter.x      , C2CirCenter.y     ); // �ܰ����� ������ ���� ���� 
								C2Diameter = (CSideCircleWidth*2);
								// 									 C6Diameter = (CSideCircleWidth*2);
								if(IsMarkBtm) Circle->SetTolerance(60.0f);
								else          Circle->SetTolerance(80.0f);
								Circle->SetDiameter (C2Diameter)     ; // Circle Diameter 
								Circle->SetCircleChoice(1)           ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
								Circle->SetCircleType  (2)           ; // 0:Black to White, 1:White to Black 2: Both Black, White 
								Circle->SetCircleIndex (1)           ; // Circle EdgePoint Index 

								if(SideCirDeg>=0){
									if(90-SideCirDeg>=0) Circle->SetAngle(90-SideCirDeg+CirAngleMargin   );  // Circle Fitting ������ ����
									Circle->SetAmplitude(SideCirDeg-2*CirAngleMargin);  // Circle Fitting ������ ���� �˻� ���� 
								}

								Circle->OnExecute()                  ;

								// Circle In Data ���� ���ο� Circle Center ��ǥ �޾ƿ� 
								RealC2CirCenter.x = Circle->FCP.x;
								RealC2CirCenter.y = Circle->FCP.y;
								CirDiameter2      = (Circle->Diameter)/2.0;

								// Circle In Data���� 
								OrgC2CirInEdgeCount = Circle->FCirclePointCount;
								if(OrgC2CirInEdgeCount>0){
									if(OrgC2CirInPoint!=NULL) delete[] OrgC2CirInPoint;
									OrgC2CirInPoint = new TFPoint[OrgC2CirInEdgeCount];

									for(int k=0; k<OrgC2CirInEdgeCount; k++){
										if(k>MAX_CIRCLE_EDGEPOINT) break;
										OrgC2CirInPoint[k].x = Circle->FCirclePoint[k].x;
										OrgC2CirInPoint[k].y = Circle->FCirclePoint[k].y;
										if(OrgC2CirInPoint[k].x==0.0f && OrgC2CirInPoint[k].y==0.0f) C2CirInEdgeCount++;
									}
								}

								// Circle Out Data���� �ٽ� �˻�  
								if(BtmEdgeThresHold==0) Circle->GImage = &RotateImg      ; //Org Image �Ѱ��� 
								else                    Circle->GImage = &RotateBinaryImg; //Binary Image �Ѱ��� 

								if(IsMarkBtm) Circle->SetTolerance(60.0f);
								else          Circle->SetTolerance(80.0f);
								// 									 Circle->SetTolerance(40.0f )                      ; // Circle Tolerance 
								Circle->SetDiameter (C2Diameter)                  ; // Circle Diameter 
								// 									 Circle->SetCircleType(0)                          ; // 0:Black to White, 1:White to Black 2: Both Black, White 
								Circle->SetCircleChoice(1)                        ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
								Circle->SetCircleType  (2)                        ; // 0:Black to White, 1:White to Black 2: Both Black, White 
								Circle->SetCircleIndex (0)                        ; // Circle EdgePoint Index 

								Circle->OnExecute()                               ;

								// Circle Out Data���� 
								OrgC2CirOutEdgeCount = Circle->FCirclePointCount;
								if(OrgC2CirOutEdgeCount>0){
									if(OrgC2CirOutPoint!=NULL) delete[] OrgC2CirOutPoint;
									OrgC2CirOutPoint = new TFPoint[OrgC2CirOutEdgeCount];

									for(int k=0; k<OrgC2CirOutEdgeCount; k++){
										if(k>MAX_CIRCLE_EDGEPOINT) break;
										OrgC2CirOutPoint[k].x = Circle->FCirclePoint[k].x;
										OrgC2CirOutPoint[k].y = Circle->FCirclePoint[k].y;
										// BTM���� Circle�� ������ ���� Defect�˻� ������ ��ǥ��� 
										if(k==(OrgC2CirOutEdgeCount-1)) {BtmEdgePoint[1].x = OrgC2CirOutPoint[k].x; BtmEdgePoint[1].y = OrgC2CirOutPoint[k].y;}
										if(OrgC2CirOutPoint[k].x==0.0f && OrgC2CirOutPoint[k].y==0.0f) C2CirOutEdgeCount++;
									}
								}
								// C2Circle Grind_Width, Grind_Measure 
								// C2Circle ������ ������ �˻� ������ ����(TrackMode VS MarkMode)
								if(IsMarkBtm){ // LTPS ������ ��� ==> ������ ��� 
									if(OrgC2CirOutEdgeCount!=0 && OrgC2CirInEdgeCount!=0){// �������� �����Ұ�� ������ �˻縦 ������ 
										GrindMeaDis =0.0f, MinDis =10000.0f; C2GrindMarkCount=0;
										if(CGrindMark[1].Point.x!=0.0f && CGrindMark[1].Point.y!=0.0f){
											for(int k=0; k<OrgC2CirOutEdgeCount; k++){
												if(OrgC2CirOutPoint[k].x  ==0.0f || OrgC2CirOutPoint[k].y==0.0f) continue;
												if(CGrindMark[1].Point.x  ==0.0f || CGrindMark[1].Point.y==0.0f) continue;

												GrindMeaDis = Math.GetLength(CGrindMark[1].Point, OrgC2CirOutPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													C2GrindMarkCount = k;
												}
											}
											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� 
											if(C2GrindMarkCount!=0 && C2GrindMarkCount>5 && (C2GrindMarkCount+5<OrgC2CirOutEdgeCount)){
												CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
												for(int k=C2GrindMarkCount-5; k<C2GrindMarkCount+5; k++){
													if(OrgC2CirOutPoint[k].x==0.0f || OrgC2CirOutPoint[k].y==0.0f) continue;

													GrindMeaDis = Math.GetLength(CGrindMark[1].Point, OrgC2CirOutPoint[k]);
													TotalCirDis+= GrindMeaDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[1].GrindMea = TotalCirDis/CircleCount;
												}
											}
											// GrindMark���� ���� ��հ� ������ ���� 
											if(C2GrindMarkCount!=0 && C2GrindMarkCount>5){
												CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
												for(int k=C2GrindMarkCount-5; k<C2GrindMarkCount+5; k++){
													if(OrgC2CirOutPoint[k].x==0.0f && OrgC2CirOutPoint[k].y==0.0f) continue;
													if(OrgC2CirInPoint [k].x==0.0f && OrgC2CirInPoint [k].y==0.0f) continue;
													GrindWidthDis = Math.GetLength(OrgC2CirInPoint[k], OrgC2CirOutPoint[k]);
													TotalCirDis+=   GrindWidthDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[1].GrindWidth = TotalCirDis/CircleCount;
												}
											}
										}
										else { // Grind Mark �� ã������� (������ 0, ������ �����)
											for(int k=0; k<OrgC2CirOutEdgeCount; k++){
												if(OrgC2CirOutPoint[k].x==0.0f || OrgC2CirOutPoint[k].y==0.0f) continue;

												Cir2GrindRectPoint.x = CGrindMark[1].Rect.left + (CGrindMark[1].Rect.right  - CGrindMark[1].Rect.left)/2; 
												Cir2GrindRectPoint.y = CGrindMark[1].Rect.top  + (CGrindMark[1].Rect.bottom - CGrindMark[1].Rect.top )/2; 

												GrindMeaDis = Math.GetLength(Cir2GrindRectPoint, OrgC2CirOutPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													C2GrindMarkCount = k;
												}
											}
											// GrindMark���� ���� ��հ� ������ ���� 
											if(C2GrindMarkCount!=0 && C2GrindMarkCount>5 && (C2GrindMarkCount+5<OrgC2CirOutEdgeCount)){
												CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
												for(int k=C2GrindMarkCount-5; k<C2GrindMarkCount+5; k++){
													if(OrgC2CirOutPoint[k].x==0.0f && OrgC2CirOutPoint[k].y==0.0f) continue;
													if(OrgC2CirInPoint [k].x==0.0f && OrgC2CirInPoint [k].y==0.0f) continue;
													GrindWidthDis = Math.GetLength(OrgC2CirInPoint[k], OrgC2CirOutPoint[k]);
													TotalCirDis+=   GrindWidthDis;
													CircleCount++;
												}
												if(TotalCirDis!=0.0f && CircleCount!=0){
													CGrindMark[1].GrindWidth  = TotalCirDis/CircleCount;
												}
											}
										}
									}
								}
								else          { // EnCap ������ ��� 
									if(OrgC2CirInEdgeCount==OrgC2CirOutEdgeCount){//������ ��� 
										double OutDis=0.0f, InDis=0.0f, TotalGrindWidth=0.0f, AvgGrindWidth=0.0f; C2GrindWidthCount=0;
										for(int k=0; k<OrgC2CirInEdgeCount; k++){
											if(OrgC2CirOutPoint[k].x==0.0f && OrgC2CirOutPoint[k].y==0.0f){
												OutDis =0.0f;
											}
											else {
												OutDis = Math.GetLengthFrPointToPoint(C2CirCenter,OrgC2CirOutPoint[k]);
											}

											if(OrgC2CirInPoint[k].x==0.0f && OrgC2CirInPoint[k].y==0.0f){
												InDis =0.0f;
											}
											else {
												InDis = Math.GetLengthFrPointToPoint(C2CirCenter,OrgC2CirInPoint[k]);
											}

											if(OutDis!=0.0f && InDis!=0.0f){
												TotalGrindWidth+=(OutDis - InDis);
												C2GrindWidthCount++;
											}
										}
										if(C2GrindWidthCount!=0){
											AvgGrindWidth = (TotalGrindWidth/C2GrindWidthCount);
											CGrindMark[1].GrindWidth= AvgGrindWidth;
										}
									}
								}
								//////////////////////////////////////////////////////////////////////////////////////////////////////

								// Org Circle�������� ��ȣ���� ���� ���� ������ ���� �����ϴ� ������ �������� ���� �޸� ������
								// Circle Defect�˻翡 �ʿ��� Edge������ ��
								int Count=0;
								if(OrgC2CirInEdgeCount>0){
									C2CirInEdgeCount = OrgC2CirInEdgeCount - C2CirInEdgeCount;
									// 										 C2CirInEdgeCount = OrgC2CirInEdgeCount;
									if(C2CirInEdgeCount>0){
										if(C2CirInPoint!=NULL) delete[] C2CirInPoint;
										C2CirInPoint = new TFPoint[C2CirInEdgeCount];

										for(int k=0; k<OrgC2CirInEdgeCount; k++){
											if(k>MAX_CIRCLE_EDGEPOINT) break;
											if(OrgC2CirInPoint[k].x==0.0f && OrgC2CirInPoint[k].y==0.0f) continue;
											C2CirInPoint[Count].x = OrgC2CirInPoint[k].x;
											C2CirInPoint[Count].y = OrgC2CirInPoint[k].y;
											Count++;
										}
									}
									// Circle ���� ������ �޸� ����
									if(OrgC2CirInPoint!=NULL) delete[] OrgC2CirInPoint;
								}
								// Circle Fitting ����� 
								SampleSize = C2CirInEdgeCount*0.75; //0.5~0.8���Ϸ� �Է��Ұ�
								DistDevRatio = 0.15; //0.1~0.25 ���Ϸ� �Է��Ұ�
								// 									Radius = 0;
								CircleFit->OnRansac(C2CirInPoint,C2CirInEdgeCount,SampleSize,DistDevRatio,&CircleFit2Center,&CirFitDiameter2);

								if(OrgC2CirOutEdgeCount>0){
									C2CirOutEdgeCount = OrgC2CirOutEdgeCount - C2CirOutEdgeCount;
									Count =0;
									if(C2CirOutEdgeCount>0){
										if(C2CirOutPoint!=NULL) delete[] C2CirOutPoint;
										C2CirOutPoint = new TFPoint[C2CirOutEdgeCount];

										for(int k=0; k<OrgC2CirOutEdgeCount; k++){
											if(k>MAX_CIRCLE_EDGEPOINT) break;
											if(OrgC2CirOutPoint[k].x==0.0f && OrgC2CirOutPoint[k].y==0.0f) continue;
											C2CirOutPoint[Count].x = OrgC2CirOutPoint[k].x;
											C2CirOutPoint[Count].y = OrgC2CirOutPoint[k].y;
											Count++;
										}
									}
									// Circle ���� ������ �޸� ����
									if(OrgC2CirOutPoint!=NULL) delete[] OrgC2CirOutPoint;
								}
								LogUnit.SetLog(L"Circle_6_Insp_End");   
								// Circle ���� �ɹڸ�, �����ڸ� ���� ���� ������(EnCap)
								// ���� ��Ī�� ����.....�ϴ� Crack���� ���� 
								if(!IsMarkBtm && BrokenThresHold!=0){
									if(CirFitDiameter2!=0.0f && CircleFit2Center.x!=0.0f && CircleFit2Center.y!=0.0f){ // FAVION Circle Fitting ������ �ڸ��˻� ���� 
										Circle->GImage = &RotateBrokenImg;                                             //Binary Image �Ѱ��� 

										Circle->SetCenterXY(CircleFit2Center.x,CircleFit2Center.y)      ;
										Circle->SetTolerance((dEdgeToTrapLineDis/2) -CirOffsetMargin)   ; // Circle Tolerance 
										Circle->SetDiameter ((CirFitDiameter2-(dEdgeToTrapLineDis/2))*2); // Circle Diameter 
										Circle->SetCircleChoice(0)                                      ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
										Circle->SetCircleType  (2)                                      ; // 0:Black to White, 1:White to Black 2: Both Black, White 
										Circle->SetCircleIndex (0)                                      ; // Circle EdgePoint Index 
										Circle->OnExecute(true)                                         ;
										CrackCirDiameter2 = (CirFitDiameter2-(dEdgeToTrapLineDis/2))+((dEdgeToTrapLineDis/2) -CirOffsetMargin);

										C2CirInCrackCount = Circle->FCirclePointCount;

										if(C2CirInCrackCount>0){
											if(C2CirInCrackPoint!=NULL) delete[] C2CirInCrackPoint;
											C2CirInCrackPoint = new TFPoint[C2CirInCrackCount];

											for(int k=0; k<C2CirInCrackCount; k++){
												if(k>MAX_CIRCLE_EDGEPOINT) break;
												C2CirInCrackPoint[k].x = Circle->FCirclePoint[k].x;
												C2CirInCrackPoint[k].y = Circle->FCirclePoint[k].y;
											}
										}
									}
								}
							}
						}
					}
				}
				if(FBottomWall){ // ��������Ʈ ���� ����,���� ������ ����
					if(BtmEdgePoint[0].x!=0.0f && BtmEdgePoint[0].y!=0.0f && BtmEdgePoint[1].x!=0.0f && BtmEdgePoint[1].y!=0.0f){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, BtmEdgeThresHold);
						int BtmRange =0;
						TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
						P1.x = (BtmEdgePoint[0].x+10); P1.y =(BtmEdgePoint[0].y-50); P2.x = (P1.x+(BtmEdgePoint[1].x - BtmEdgePoint[0].x)-20); P2.y = (P1.y);
						P3.x = (BtmEdgePoint[0].x+10); P3.y =(BtmEdgePoint[0].y+50); P4.x = (P2.x                                           ); P4.y = (P3.y);

						// Median Filter ���� 
						if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
						if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
						FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
						FilterROI.Attach(&RotateBinaryImg)                                    ;
						FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
						LogUnit.SetLog(L"ù��° ���� ==>Px :%d, Py :%d, Width:%d, Height:%d ==>Median Filter",P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
						LogUnit.SetLog(L"ù��° ���� ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
						EasyImage::Median(&FilterROI, &FilterROI)        ;

						Polygon->PoPa.FMinHeight = 5                     ;
						Polygon->IWidth          = ImgW                  ;
						Polygon->IHeight         = ImgH                  ;
						Polygon->GImage          = &RotateBinaryImg      ;
						Polygon->PoPa.FMaxWidth  =(int)(P2.x - P1.x)+100 ;
						Polygon->PoPa.FMaxHeight =(int)(P3.y - P1.y)     ;
						Polygon->PoPa.FMinWidth  =(int)(P2.x - P1.x)-100 ; 
						Polygon->PoPa.IsSeparate =true                   ;
						Polygon->PoPa.IsInToOut  =true                   ;
						Polygon->PoPa.HiThHold   = BtmEdgeThresHold      ;
						Polygon->PoPa.LoThHold   = 0                     ;

						if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
							Polygon->OnExecute(P1,P2,P3,P4);
						}

						// Polygon Algorithm InEdgeData/OutEdgeData Separate �� ������ ���� 
						if(Polygon->BlobCount==1){
							PolygonMarginCnt =10;
							if(Polygon->Blob[0].InCnt>(PolygonMarginCnt*2)){
								BtmInEdgeCount=0;
								BtmInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt*2));
								if(BtmInEdgeCount>0){
									if(BtmInPoint!=NULL) delete[] BtmInPoint;
									BtmInPoint = new TFPoint[BtmInEdgeCount];

									PixelCnt=0;
									for(int k=0; k<Polygon->Blob[0].InCnt; k++){
										if(k<PolygonMarginCnt || k>=(Polygon->Blob[0].InCnt-PolygonMarginCnt)) continue;
										BtmInPoint[PixelCnt].x   = Polygon->Blob[0].InPoint[k].x ;
										BtmInPoint[PixelCnt].y   = Polygon->Blob[0].InPoint[k].y ;
										PixelCnt++;
									}
								}
								// Org Data 
								BtmInLine[0].FP0 = BtmInPoint[0];
								BtmInLine[0].FP1 = BtmInPoint[PixelCnt-1];
							}

							if(Polygon->Blob[0].OutCnt>(PolygonMarginCnt*2)){
								BtmOutEdgeCount=0;
								BtmOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt*2));
								if(BtmOutEdgeCount>0){
									if(BtmOutPoint!=NULL) delete[] BtmOutPoint;
									BtmOutPoint = new TFPoint[BtmOutEdgeCount];

									PixelCnt=0;
									for(int k=0; k<Polygon->Blob[0].OutCnt; k++){
										if(k<PolygonMarginCnt || k>=(Polygon->Blob[0].OutCnt-PolygonMarginCnt)) continue;
										BtmOutPoint[PixelCnt].x  = Polygon->Blob[0].OutPoint[k].x ;
										BtmOutPoint[PixelCnt].y  = Polygon->Blob[0].OutPoint[k].y ;
										PixelCnt++;
									}
								}
								// Org Data 
								BtmOutLine[0].FP0 = BtmOutPoint[0];
								BtmOutLine[0].FP1 = BtmOutPoint[PixelCnt-1];
							}
						}

						// BtmInLine���� BtmStdLine EdgePoint ���� �� LineFitting ������ 
						if(BtmStdThresHold!=0){
// 							EasyImage::Threshold(GImage,&BinaryImg, BtmStdThresHold);
							EasyImage::Threshold(&RotateImg,&RotateBinaryImg, BtmStdThresHold);
// 							RotateBinaryImg.Save("D:\\ROTATE_IMG\\SoulBrain_BTM_��������.bmp");

							if(BtmEdgeThresHold==0)    {SingleLine->GImage = &RotateImg      ; SingleLine->FThreshold =5 ;}
							else                       {SingleLine->GImage = &RotateBinaryImg; SingleLine->FThreshold =10;}

							SingleLine->IRoi->SetOrgX  (BtmInLine[0].FP0.x                    );
							SingleLine->IRoi->SetOrgY  (BtmInLine[0].FP0.y -200               );
							SingleLine->IRoi->SetWidth (BtmInLine[0].FP1.x -BtmInLine[0].FP0.x);
							SingleLine->IRoi->SetHeight(180                                   ); // TEST����

							SingleLine->SetOrientation(orSouth);
							SingleLine->FIncrement =50;
							SingleLine->SetFindLineId(0);
							SingleLine->FThreshold =5;
							SingleLine->SetEdgeType(1);  // White To Black 

							// �⺻ BtmLine Search 
							SingleLine->OnExecute();
							BtmStdLine[0].FP0 = SingleLine->GetResultP0();
							BtmStdLine[0].FP1 = SingleLine->GetResultP1();
						}

						// BmtStdLine���� GrindMark��ǥ���� �� ����,������ ���� ������ 
						if(IsMarkBtm){ // LTPS ������,������ ���� ������ 
							if(BtmStdLine[0].FP0.x!=0.0f && BtmStdLine[0].FP0.y!=0.0f && BtmStdLine[0].FP1.x!=0.0f && BtmStdLine[0].FP1.y!=0.0f){
								BtmRange = (int)((BtmStdLine[0].FP1.x - BtmStdLine[0].FP0.x)/8);
								for(int k=0; k<7; k++){
									BtmGrindMark[k].Point.x     =  BtmStdLine[0].FP0.x + (BtmRange*(k+1))        ;
									BtmGrindMark[k].Point.y     = (BtmStdLine[0].FP0.y + BtmStdLine[0].FP1.y)/2.0;
									BtmGrindMark[k].Rect.left   = BtmGrindMark[k].Point.x - GrindMarkRange       ;
									BtmGrindMark[k].Rect.top    = BtmGrindMark[k].Point.y - GrindMarkRange       ;
									BtmGrindMark[k].Rect.right  = BtmGrindMark[k].Rect.left+GrindMarkLength      ;
									BtmGrindMark[k].Rect.bottom = BtmGrindMark[k].Rect.top +GrindMarkLength      ;

									// BtmStdLine���� ����GrindMark���� �������� ������ �ּҰŸ� ��ǥ ī��Ʈ ���  
									GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
									for(int i=0; i<BtmOutEdgeCount; i++){
										if(BtmGrindMark[k].Point.x==0.0f || BtmGrindMark[k].Point.y==0.0f) continue;
										if(BtmOutPoint[i].x       ==0.0f || BtmOutPoint[i].y       ==0.0f) continue;

										GrindMeaDis = Math.GetLength(BtmGrindMark[k].Point, BtmOutPoint[i]);
										if(GrindMeaDis<MinDis){
											MinDis = GrindMeaDis;
											GrindMarkOutCnt = i;
										}
									}

									// BtmOutLine���� BtmInLine�������� ������ �ּҰŸ� ��ǥ ī��Ʈ ���  
									if(GrindMarkOutCnt!=0){
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
										for(int i=0; i<BtmInEdgeCount; i++){
											if(BtmOutPoint[GrindMarkOutCnt].x ==0.0f  || BtmOutPoint[GrindMarkOutCnt].y ==0.0f) continue;
											if(BtmInPoint[i].x                ==0.0f  || BtmInPoint[i].y                ==0.0f) continue;

											GrindMeaDis = Math.GetLength(BtmOutPoint[GrindMarkOutCnt], BtmInPoint[i]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												GrindMarkInCnt = i;
											}
										}
									}
									// BtmStdLine���� ������ ���� 
									if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5 && (GrindMarkOutCnt+5<BtmOutEdgeCount)){
										CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
										for(int i=GrindMarkOutCnt-5; i<GrindMarkOutCnt+5; i++){
											if(BtmOutPoint[i].x==0.0f || BtmOutPoint[i].y==0.0f) continue;
											if(i>=BtmOutEdgeCount                              ) continue;

											GrindMeaDis = Math.GetLength(BtmGrindMark[k].Point, BtmOutPoint[i]);
											TotalCirDis+= GrindMeaDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											BtmGrindMark[k].GrindMea = TotalCirDis/CircleCount;
											BtmGrindMark[k].GrindMea = BtmGrindMark[k].GrindMea+0.5;
										}
									}

									// ���� GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
									if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
										CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
										for(int i=-5; i<5; i++){
											if(BtmOutPoint[GrindMarkOutCnt+i].x==0.0f && BtmOutPoint[GrindMarkOutCnt+i].y==0.0f) continue;
											if(BtmInPoint [GrindMarkInCnt +i].x==0.0f && BtmInPoint [GrindMarkInCnt +i].y==0.0f) continue;
											if((GrindMarkOutCnt+i)>=BtmOutEdgeCount                                            ) continue;
											if((GrindMarkInCnt +i)>=BtmInEdgeCount                                             ) continue;

											GrindWidthDis = Math.GetLength(BtmInPoint[GrindMarkInCnt+i], BtmOutPoint[GrindMarkOutCnt+i]);
											TotalCirDis+=   GrindWidthDis;
											CircleCount++;
										}
										if(TotalCirDis!=0.0f && CircleCount!=0){
											BtmGrindMark[k].GrindWidth  = TotalCirDis/CircleCount       ;
											BtmGrindMark[k].GrindWidth  = BtmGrindMark[k].GrindWidth+0.5;
										}
									}
								}
							}
						}
						else         { // ENCAP 
							if(BtmOutLine[0].FP0.x!=0.0f && BtmOutLine[0].FP0.y!=0.0f && BtmOutLine[0].FP1.x!=0.0f && BtmOutLine[0].FP1.y!=0.0f &&
								BtmInLine [0].FP0.x!=0.0f && BtmInLine [0].FP0.y!=0.0f && BtmInLine [0].FP1.x!=0.0f && BtmInLine [0].FP1.y!=0.0f ){
									// ��ü ��� ������ ��� 
								GrindWidthDis = 0.0f;
							    if((fabs(BtmOutLine[0].FP0.y - BtmOutLine[0].FP1.y))>1 ||(fabs(BtmInLine[0].FP0.y - BtmInLine[0].FP1.y))>1){
								    GrindWidthDis = BtmOutLine[0].FP0.y - BtmInLine[0].FP0.y; 
								}
								else{
									GrindWidthDis = Math.GetLengthFrLineToPoint(BtmOutLine[0].FP0, BtmOutLine[0].FP1, BtmInLine[0].FP0);
								}

								// BtmGrindMark�����Ϳ� ��ü���� Aag������ ������ �Ѱ��� 
								for(int k=0; k<7; k++){
									BtmGrindMark[k].GrindWidth = GrindWidthDis;
								}
							}
						}
					}
				}
			}
		}
		Index++;

		// Find Edge Point - Bevel In Side Only
		//----------------------------------------------------------------------
	case 11 :
		DefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&DefectImg);
		//EasyImage::Threshold(GImage,&DefectImg, DThresHold);

		if (FProcessMode==pmAutoBevelIn) {
			//BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
			//EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
			//EasyImage::Threshold(GImage,&BinaryImg, ThredHold);
			Edge->GImage = &DefectImg;
			//Edge->GImage = GImage;

			if(!OnAutoBevelIn()) return false;
			Index=0;
			return true;
		}
		Index++;

	case 12 :
		Index=0;
		break;
	}
	return true;
}
void CFM2xLineDefect::OnLoad(bool IsLoad, CString Folder)
{
	CIFile Ini;

	if (Folder=="") return;
	CString Path = Folder + "\\PLine.ini";
	if (Path=="") return;

	//Ini.Load(IsLoad, Path, "Parameter", "FMaxLines"  , &FMaxLines);
}
//------------------------------------------------------------------------------------
// ����� Get�Լ� 
//------------------------------------------------------------------------------------------
CFind* CFM2xLineDefect::GetFindPointer(int n)
{
	if     (n==0) return Find1;
	else if(n==1) return Find2;
	return NULL;
}

//------------------------------------------------------------------------------------
// ����� Set�Լ� 
//------------------------------------------------------------------------------------------
void CFM2xLineDefect::SetProcessMode(TProcessMode ProcessMode)
{
	FProcessMode  = ProcessMode;
}
//------------------------------------------------------------------------------------------
void CFM2xLineDefect::SetInspectMode(TInspectMode InspectMode)
{
	FInspectMode  = InspectMode;
}
//------------------------------------------------------------------------------------------
void CFM2xLineDefect::SetShape(TShape Shape)
{
	FShape = Shape;
}
//-------------------------------------------------------------------------------------------
void CFM2xLineDefect::SetCrossLeftPoint(int x, int y)
{
	FP0.x = (float)x;
	FP0.y = (float)y;
	AlignMark1.x = FP0.x;
	AlignMark1.y = FP0.y;
}
//------------------------------------------------------------------------------------------
void CFM2xLineDefect::SetCrossRightPoint(int x, int y)
{
	FP1.x =(float)x;
	FP1.y =(float)y;
	AlignMark2.x = FP1.x;
	AlignMark2.y = FP1.y;
}
//------------------------------------------------------------------------------------------
void CFM2xLineDefect::SetSpecWidth(float MinWidth , float MaxWidth)
{
	FMinWidth = MinWidth;
	FMaxWidth = MaxWidth;
}
//------------------------------------------------------------------------------------------
void CFM2xLineDefect::SetSpecHeight(float MinHeight, float MaxHeight)
{
	FMinHeight = MinHeight;
	FMaxHeight = MaxHeight;
}
//------------------------------------------------------------------------------------------
