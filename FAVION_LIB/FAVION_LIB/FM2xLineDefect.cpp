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

// 외관검사기 개조(LineScan23k X 2)
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
	DefectPointCount = 0; // 전체 불량 개수 초기화
	ACount           = 0; // 모자모양 불량 개수 초기화
	BCount           = 0; // 젓가락모양 불량 개수 초기화
	CCount           = 0; // 컵모양 불량 개수 초기화
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

	// Cell 정보 
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

	// 박리 Circle 검사 InPoint Init 
	A1CirInCrackPoint = NULL;
	A2CirInCrackPoint = NULL;

	C1CirInCrackPoint = NULL;
	C2CirInCrackPoint = NULL;

	// Btm 직선영역 EdgePoint Data Init 
	BtmInPoint        = NULL;
	BtmOutPoint       = NULL;

	// MultiThresHold 
	ECThresHold       =0;
	EnCapTopThresHold =0;
	EnCapMidThresHold =0;
	EnCapBtmThresHold =0;

	//기본 EdgeThresHold Init 
	LeftEdgeThresHold    =0;
	TopEdgeThresHold     =0;
	RightEdgeThresHold   =0;
	BtmEdgeThresHold     =0;

	// 연마라인 찾는 ThresHold 
	LeftGrindThresHold   =0;
	RightGrindThresHold  =0;
	BtmGrindThresHold    =0;

	// Standard Line 찾는 ThresHold 
	LeftStdThresHold     =0;
	RightStdThresHold    =0;
	BtmStdThresHold      =0;

	CirGrindMeaThresHold =0;
	CorGrindMeaThresHold =0;
	CirGrindInThresHold  =0;
	CorGrindInThresHold  =0;

	//프로그램 실행시 한번 초기화  
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

	// 외곽라인에서 개별 GrindMark까지 Width,Height 거리
	OutToGrindMarkDis1=0.0f;
	OutToGrindMarkDis2=0.0f;

	// 박리 검출 목적으로 EdgeLine에서 Trap까지 거리 
	dEdgeToTrapLineDis=0.0f;

	// Notch Cell Mark Init 
	dCirOffsetX        =0.0f;
	dCirOffsetY        =0.0f;

	LogUnit.Create(_T("FAVION"));

	FInspectMode =0; //InspectMode Classify Parameter

	//LHJ 추가 20180808
	RqMode = false;
	WkMode = false;

	// Corner 연마검사 여부 확인 
	IsCornerTop = false;
	IsCornerBtm = false;

	// Rect 검사 여부 확인 
	IsRectTop = false;
	IsRectBtm = false;

	// Cell Circle Grind 검사 유무 결정
	IsCircleTop = false;
	IsCircleBtm = false;

	// Find Dir 
	FindDir  ="";
	MatchDir =""; 

	// Grind Mark Find Score 
	GrindMarkMinScore    =0.0f;
	GrindPadMarkMinScore =0.0f;

	// A_Side AlignMark에서 GrindMark까지 거리 편차값 Init
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
	// 직선과 한점의 수직한 교차점에 의한 치수 측정
	// 공식 : ax + by + c = 0, (m, n) -> Distance = (ma + nb +c) / sqrt(a*a + b*b)
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
	// 개별 LineEdge 검출전 고객사Cell형태 기준으로 영역별 검사유무 결정해야함   
	// NOTCH CELL 검사 시퀀스 
	if (FShape==shRectH) Index = 0;
	if (FShape==shRectS) Index = 1;
	if (FShape==shRectC) Index = 2;
	// 기존시퀀스랑 Cell형태가 상이할 경우 Company 추가해야함 
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
		// RectH 모자모양 
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
		// RectS : 젓가락 모양
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
		// RectC : 컵모양
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

	// Defect Data 존재유무 판단함 
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

					// 내부 Pixel Data 축출 
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

	// 꽃 박리와 반원형 박리 구별 목적 필요한 변수  
	TFPoint LTPoint(0.0f, 0.0f);
	TFPoint RTPoint(0.0f, 0.0f);
	TFPoint LBPoint(0.0f, 0.0f);
	TFPoint RBPoint(0.0f, 0.0f);
	double CenDisLTPos =0.0f;
	double CenDisRTPos =0.0f;
	double CenDisLBPos =0.0f;
	double CenDisRBPos =0.0f;

	// Defect Data 존재유무 판단g 
	if(Count>5) IsStart = true;

	if(IsStart){	
		for (register int k=0; k<Count; k++) {
			if ( lpCircle[k].P.x!=0.0f && lpCircle[k].P.y!=0.0f && IsFlagCh) {   // 1이상 큰 정수부터 가능함 즉....2부터.....
				IsRoof = true;
				IsFlagCh = false ;
				StaPos.x = lpCircle[k].P.x;
				StaPos.y = lpCircle[k].P.y;
				PointDis =0.0f;
			} 
			if(k>1       ) PointDis = Math.GetLength(lpCircle[k-1].P,lpCircle[k].P);
			if(k==Count-1) PointDis = MinDis+1;

			if (IsRoof && PointDis >MinDis) { 
				// 내부 Pixel Data 축출 
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
				// Circle Defect 내부의 PixelCount 수량을 체크한다 => 오버킬 감소 목적 
				if(Score>PixelScore){
					// 						MaxPos.x = lpCircle[MaxCount].P.x;
					// 						MaxPos.y = lpCircle[MaxCount].P.y;
					// 꽃 박리와 반원형 박리 구별목적
					LTPoint.x = (float)Rect.left ;  LTPoint.y = (float)Rect.top   ;
					RTPoint.x = (float)Rect.right;  RTPoint.y = (float)Rect.top   ;
					LBPoint.x = (float)Rect.left ;  LBPoint.y = (float)Rect.bottom;
					RBPoint.x = (float)Rect.right;  RBPoint.y = (float)Rect.bottom;

					CenDisLTPos =0.0f;
					CenDisRTPos =0.0f;
					CenDisLBPos =0.0f;
					CenDisRBPos =0.0f;

					// 꽃박리(Peeling) vs 반원형박리(Broken)구별 시퀀스 
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

			if (IsRoof) { // Bright Point 기준 Rect생성 
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

	// Defect Data 존재유무 판단함 
	for(int i=0; i<Count; i++){
		if(lpCircle[i].Dv>Dv){
			IsStart = true;
			break;
		}
	}

	if(IsStart){	
		for (register int k=0; k<Count; k++) {
			if ( lpCircle[k].Dv >1 && IsFlagCh) {   // 1이상 큰 정수부터 가능함 즉....2부터.....
				IsRoof = true;
				IsFlagCh = false ;
				StaPos.x = lpCircle[k].P.x;
				StaPos.y = lpCircle[k].P.y;
			} 

			if ( IsRoof && lpCircle[k].Dv <=1 && DefectPoint>3) { 
				if(DefectPoint>3){
					// 내부 Pixel Data 축출 
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
					// Circle Defect 내부의 PixelCount 수량을 체크한다 => 오버킬 감소 목적 
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

	// Defect 검출시 내부 Quantity Score Calc 
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

	// 3개의 기준 Point 좌표 
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

	// Range범위만큼 Polygon Data Point 분활하여 Defect 유무 Calc
	for(int k=0; k<TotalCount; k+=Range){
		TFPoint *Q1 = &PolygonData[k];
		int v = (TotalCount-k)/Range;  // Range범위만큼 분활시 나머지 값
		int r = TotalCount%Range    ;  // 전체 픽셀을 Range범위보다 분활시 Range범위보다 작은범위가 남을경우 
		int n = (v>=1)? Range: r    ;  // 전체 픽셀을 Range범위보다 분활시 Range범위보다 작은범위가 남을경우 

		if(v<0) break;

		// Range 범위만큼 분활하여 동적 생성한다 
		if(RangePoint!=NULL) delete [] RangePoint;
		RangePoint = new TFPoint[n];

		// Range범위에서 실제 Polygon Pos 좌표 및 BadPixel Pos좌표 Init 
		for(int z=0; z<n; z++){
			RangePoint[z].x    = Q1[z].x;
			RangePoint[z].y    = Q1[z].y;
		}

		// Find Defct Pixel 
		f1 = RangePoint[0         ]; //Range범위에서 첫번째 Point 좌표
		f2 = RangePoint[(int)(n/2)]; //Range범위에서 가운데 Point 좌표
		f3 = RangePoint[n-1       ]; //Range범위에서 마지막 Point 좌표

		// Rect Data Init 
		Rect.left   =0;
		Rect.top    =0;
		Rect.right  =0;
		Rect.bottom =0;

		IsBadRect  = false;
		PixelCount =0     ;
		for(int i=0; i<n; i++){ // Range영역에서 3개 Point기준 BadRect,BadPixel 정보를 취합한다 
			Dv1 = GetLengthFrLineToPoint(f1, f3, RangePoint[i]);
			Dv2 = GetLengthFrLineToPoint(f1, f2, RangePoint[i]);
			Dv3 = GetLengthFrLineToPoint(f2, f3, RangePoint[i]);

			if(Dv1>DefectDv || Dv2>DefectDv || Dv3>DefectDv){ // Defect Point 존재할경우 
				if(!IsBadRect){ // Defect Point StaPos Data 취합 목적
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
					BadRect[BadRectCount].EPoint.x                                   = RangePoint[i].x; // End Pos 정보 업데이트 
					BadRect[BadRectCount].EPoint.y                                   = RangePoint[i].y; // End Pos 정보 업데이트 
					BadRect[BadRectCount].BadPixelCount++                                             ;
				}
			}
		}

		if(IsBadRect){
			BadRect[BadRectCount].Rect = Rect;

			float Distance=0.0f;

			if(BadRectCount>0){ // 두번째 BadRect부터 이전 BadRect외 비교하여 거리값 측정 
				Distance = Math.GetLength(BadRect[BadRectCount].SPoint, BadRect[BadRectCount-1].EPoint);
			}

			// 현재 BadRect기준 일정거리 이하이면 이전BadRect데이터와 Merge한다 
			if(((Rect.right - Rect.left)>2) && ((Rect.bottom- Rect.top)>2) && Distance && (Distance<20)){
				BadRect[BadRectCount-1].Rect.left   = BadRect[BadRectCount-1].Rect.left  <Rect.left   ? BadRect[BadRectCount-1].Rect.left   : Rect.left  ;
				BadRect[BadRectCount-1].Rect.top    = BadRect[BadRectCount-1].Rect.top   <Rect.top    ? BadRect[BadRectCount-1].Rect.top    : Rect.top   ;
				BadRect[BadRectCount-1].Rect.right  = BadRect[BadRectCount-1].Rect.right >Rect.right  ? BadRect[BadRectCount-1].Rect.right  : Rect.right ;
				BadRect[BadRectCount-1].Rect.bottom = BadRect[BadRectCount-1].Rect.bottom>Rect.bottom ? BadRect[BadRectCount-1].Rect.bottom : Rect.bottom;

				BadRect[BadRectCount-1].EPoint = BadRect[BadRectCount].EPoint;

				for(int b=0; b<BadRect[BadRectCount].BadPixelCount; b++){ // 현재 BadPixelData값을 이전 BadRect에 넘겨준다 
					BadRect[BadRectCount-1].Pixel[BadRect[BadRectCount-1].BadPixelCount] = BadRect[BadRectCount].Pixel[b];
					BadRect[BadRectCount-1].BadPixelCount++;

					// 현재 Data Init 
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

	// Range 범위만큼 분활하여 동적 생성한다 
	// Range 범위기준 마지막 데이터 삭제 
	if(RangePoint!=NULL) delete [] RangePoint;

	// BadRect Data 기준 1차 필터링 및 실제 lpDefect에 데이터 넘겨줌
	if(BadRectCount>0){
		TFPoint Target(0.0f,0.0f);

		for(int k=0; k<BadRectCount; k++){
			IsRoof = false;
			for(int i=0; i<BadRect[k].BadPixelCount; i++){
				f1     = BadRect[k].SPoint  ; // BadRect 시작점 좌표
				f2     = BadRect[k].EPoint  ; // BadRect 종료점 좌표 
				Target = BadRect[k].Pixel[i]; // BddRect 리얼   좌표 
				Dv1    = GetLengthFrLineToPoint(f1, f2, Target);

				if(Dv1>3){
					if(!IsRoof){ // Defect Point StaPos Data 취합 목적
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
				else                                            { // Defect Point 없을경우 
					if(IsRoof){
						if(DefectPoint>10){
							// 내부 Pixel Data 축출 
							PixelCount =0, PixelPoint=0, PixelScore=5, Score =0.0f;
							for(int n=Rect.top ; n<Rect.bottom; n++){
								for(int m=Rect.left; m<Rect.right; m++){
									PixelValue = IElement(m,n);
									if(PixelValue < EnCapBtmThresHold) PixelCount++;
									PixelPoint++;
								}
							}
							Score  = (((float)PixelCount/(float)PixelPoint)*100);
							// Circle Defect 내부의 PixelCount 수량을 체크한다 => 오버킬 감소 목적 
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
								lpDefect[DefectCount].P.y          =  (float)(Rect.top +ImageHeight); // 전체 Frame기준 Real좌표 기준으로 변환 

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
						else { // Defect Count 작을경우 초기화 
							DefectPoint = 0;
							Rect.left   = 0;
							Rect.top    = 0;
							Rect.right  = 0;
							Rect.bottom = 0;
							IsRoof      = false; 
						}
					}
					else      { // 최소 Defect Distance 작을경우 
						Rect.left   = 0;
						Rect.top    = 0;
						Rect.right  = 0;
						Rect.bottom = 0;
					}
				}
			}
		}
	}

	// Defect Data기준 Merge 진행함 
	if(DefectCount>1){ // Defect정보가 최소 2개이상 존재할경우 Merge 시퀀스 진행함
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

	// 박리불량 검출 Point Count
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

	//Left,Right 연마,가공 Init
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
	// Btm Line Search 시간 단축 목적 
	FEndLine =0;

	// Notch Cell Mark Degree 
	MarkDegree =0.0f;

	// 연마 각도 (Circle)
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

	// Center Cell Size  측정시 필요한 데이터 
	TotalCellSizeWidth  =0.0f;
	CenterCellSize      =0.0f;
	TotalCellSizeCount  =0   ;

	// Cell Size 측정 기준
	LeftStaPos.x   =0.0f;
	LeftStaPos.y   =0.0f;
	LeftEndPos.x   =0.0f;
	LeftEndPos.y   =0.0f;
	CenterStaPos.x =0.0f;
	CenterStaPos.y =0.0f;
	CenterEndPos.x =0.0f;
	CenterEndPos.y =0.0f;

	// Align 심하게 틀어질경우 발생하는 Defect오버킬 
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
int CFM2xLineDefect::OnAutoBevelIn()// Line기준 실제 검사 함수 
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
	// NOTCH Defect 검사 시퀀스 
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

	// Pixel 내부Data 추출 Buffer넘겨줌 
	//IAddress = DefectImg.GetImagePtr();
	int PixelValue;
	int PixelCount;

	// 광원 Btm OverKill방지 목적 
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
	//CNC_6HEAD 이미지 변환 후 기본설정 진행 
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

	// Notch Cell Size 측정 
	TFPoint NotchCenterPos(0.0f, 0.0f);

	// ROI 영역 설정
	int LeftHeight ;
	int RightHeight;
	int TopWidth   ;
	int BtmWidth   ;
	int BlobCount  ;
	const int Margin =15;

	// ImageRotate 불량 발생함 OverKill 감소 목적
	const int TopMargin =10;
	const int BtmMargin =30;

	// 컵영역 직사각형 형태일경우 단차로 인해서 불량이 패턴과 붙어서
	// 검사시 검출 불가함...이로인해서 Offset추가함 
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

	// GrindMark 각도 적용하여 회전목적(검사화면 디스플레이)
	TFPoint LtPos, RbPos;

	// 검사 ROI 설정 
	// Notch Cell 모자 검사 
	if(FShape==shRectH ) {
		SP = Math.OnIntersect(LeftOutLine [0].FP0, LeftOutLine [0].FP1, LB, RB);
		EP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LB, RB);

		FLeftWidth  =200;
		FRightWidth =200;
		FTopHeight  =200;

		if(IsCornerTop)     { //Corner Cutting일경우 Defect검사 영역 설정 
			TopWidth    =(int)(FRTTInPoint.x - FLTTInPoint.x); // TOP검사 진행시 양쪽 끝에 오버킬 감소 목적 
			LeftHeight  =(int)(SP.y - FLTLInPoint.y);
			RightHeight =(int)(EP.y - FRTRInPoint.y);
		}
		else if(IsRectTop)  {//Corner Cutting 없을경우 Defect검사 영역 설정 
			TopWidth    =(int)(FRTPoint.x - FLTPoint.x); // TOP검사 진행시 양쪽 끝에 오버킬 감소 목적 
			LeftHeight  =(int)(SP.y - FLTPoint.y);
			RightHeight =(int)(EP.y - FRTPoint.y);
		}
		else if(IsCircleTop){//Circle Cutting일경우 Defect 검사 영역 설정 
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
			TopWidth    =(int)(TopEdgePoint[2].x - TopEdgePoint[1].x); // TOP검사 진행시 양쪽 끝에 오버킬 감소 목적   
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
		DefectPointCount = 0; // 전체 불량 개수 초기화
		BCount           = 0; // B_SIDE  불량 포인트 개수
		ACount           = 0; // A_SIDE  불량 포인트 개수
		CCount           = 0; // C_SIDE  불량 포인트 개수
		DCount           = 0; // D_SIDE  불량 포인트 개수
		// 		C1CirDeftectCnt  = 0;
		C2CirDeftectCnt  = 0;
		ABurrCount       = 0;
		BBurrCount       = 0;
		CBurrCount       = 0;
		DBurrCount       = 0;

		// BSide, DSide Defect 검사시 Margin값 설정 
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
	// Notch Cell 스틱 검사 
	if(FShape==shRectS) {
		LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
		LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);

		RSP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LT, RT);
		REP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LB, RB);

		// 오버킬 방지목적으로 무조건 안쪽 라인 기준 검사 
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
	// Notch Cell 컵 검사 
	if(FShape==shRectC) {
		SP = Math.OnIntersect(LeftOutLine [0].FP0, LeftOutLine [0].FP1, LT, RT);
		EP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LT, RT);

		// BtmWidth 설정값은 Circle계산 이 후에 개별 적용함
		FLeftWidth  = 200;
		FRightWidth = 200;
		FBtmHeight  = 50;

		if(IsCornerBtm)     {//Corner Cutting일경우 Defect검사 영역 설정 
			LeftHeight  = (int)((FLBLInPoint.y - SP.y));
			RightHeight = (int)((FRBRInPoint.y - EP.y));
			BtmWidth    = (int)((FRBBInPoint.x - FLBBInPoint.x));
		}				    
		else if(IsRectBtm)  {//Corner Cutting 없을경우 Defect검사 영역 설정 
			LeftHeight  = (int)((FLBPoint.y - SP.y      ));
			RightHeight = (int)((FRBPoint.y - EP.y      ));
			BtmWidth    = (int)((FRBPoint.x - FLBPoint.x));
		}
		else if(IsCircleBtm){//Circle Cutting일경우 Defect 검사 영역 설정 
			LeftHeight  = (int)((FLBPoint.y - SP.y) - CSideCircleHeight - dCirOffsetY);
			RightHeight = (int)((FRBPoint.y - EP.y) - CSideCircleHeight - dCirOffsetY);
		}

		// BSide, DSide Defect 검사시 Margin값 설정 
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
		// Corner Defect 검사 
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
			// Blob 검사 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
					for(int n=ADefectPoint[ACount].Rect.top+1 ; n<ADefectPoint[ACount].Rect.bottom-1; n++){
						for(int m=ADefectPoint[ACount].Rect.left+1; m<ADefectPoint[ACount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}

					ADefectPoint[ACount].Round  = PixelCount;
					ADefectPoint[ACount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width/2 )+FLTTInPoint.x+SideMargin       );
					ADefectPoint[ACount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2)+((FLTTInPoint.y+FRTTInPoint.y)/2));

					// Image Rotate 기준 좌표를 다시 Rotate한다 _ Display 목적 
					if(ImgDegree!=0.0f){
						ADefectPoint[ACount].P = Math.OnRotate(CenterPoint, ADefectPoint[ACount].P, (ImgDegree));
					}
					// Image Rotate 기준 좌표를 회전 이후에 ImageHeight값을 더하여 실제 좌표를 산출한다 
					ADefectPoint[ACount].P.y    =(float)(ADefectPoint[ACount].P.y+ImageHeight);

					//Merget위해서 필요함 
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
			//  Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) + FRTRInPoint.y+Margin+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount =0;

			// 외곽라인에서 Mark까지의 거리 산출 
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

			// Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);

			// Blob 필터링 
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
					// 내부 Pixel Data 축출 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}
					// Display 데이터 필요함 
					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
					DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(FLTLInPoint.y+Margin)+ImageHeight );
					DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

					// Merge 위해서 데이터 필요함 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(FLTLInPoint.y+Margin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount =0;

			// 외곽라인에서 Mark까지의 거리 산출 
			if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = (FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2));
			else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = (FP0.x -  Line[waLeftWall].FP0.x)                                                     ;
			else                                                    LDistance = (FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2)); 
		}
		else if(IsRectTop) {  // Corner Cutting이 없을경우 
			//Side A
			////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(FLTPoint.x+SideMargin), (int)((FLTPoint.y+FRTPoint.y)/2), TopWidth-(2*SideMargin), FTopHeight);
			if((FLTPoint.x+SideMargin) <=0          || ((FLTPoint.y+FRTPoint.y)/2)<=0           || TopWidth-(2*SideMargin)<=0          || FTopHeight<=0          )return false;
			if((FLTPoint.x+SideMargin) >=ImageWidth || ((FLTPoint.y+FRTPoint.y)/2)>TopLimitLine || TopWidth-(2*SideMargin)>=ImageWidth || FTopHeight>TopLimitLine) return false;
			if((FLTPoint.x+SideMargin)+(TopWidth-(2*SideMargin)) >=ImageWidth || ((FLTPoint.y+FRTPoint.y)/2)+ FTopHeight>TopLimitLine) return false;
			// Blob 검사 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
					for(int n=ADefectPoint[ACount].Rect.top+1 ; n<ADefectPoint[ACount].Rect.bottom-1; n++){
						for(int m=ADefectPoint[ACount].Rect.left+1; m<ADefectPoint[ACount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}

					ADefectPoint[ACount].Round  = PixelCount;
					ADefectPoint[ACount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width/2 )+FLTPoint.x+SideMargin      );
					ADefectPoint[ACount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2)+((FLTPoint.y+FRTPoint.y)/2));

					// Image Rotate 기준 좌표를 다시 Rotate한다 _ Display 목적 
					if(ImgDegree!=0.0f){
						ADefectPoint[ACount].P = Math.OnRotate(CenterPoint, ADefectPoint[ACount].P, (ImgDegree));
					}
					// Image Rotate 기준 좌표를 회전 이후에 ImageHeight값을 더하여 실제 좌표를 산출한다 
					ADefectPoint[ACount].P.y    =(float)(ADefectPoint[ACount].P.y+ImageHeight);

					//Merget위해서 필요함 
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

			//  Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) + FRTPoint.y+Margin+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount =0;

			// 외곽라인에서 Mark까지의 거리 산출 
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

			// Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);

			// Blob 필터링 
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
					// 내부 Pixel Data 축출 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}
					// Display 데이터 필요함 
					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
					DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(FLTPoint.y+Margin)+ImageHeight );
					DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

					// Merge 위해서 데이터 필요함 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(FLTPoint.y+Margin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount =0;

			// 외곽라인에서 Mark까지의 거리 산출 
			if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = (FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2));
			else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = (FP0.x -  Line[waLeftWall].FP0.x)                                                     ;
			else                                                    LDistance = (FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2)); 
		}
		else if(IsCircleTop){ // Circle Cutting일경우 Defect검사 진행 
			if(FInspectMode==Insp_Polygon){ // A_Side 영역 Arc Polygon Mode 검사 시퀀스 
				// 				LogUnit.SetLog("Polygon_Defect_START");     
				if(A1CirInEdgeCount!=0){    // Polygon Mode 검사에서는 Dx,Dy,R,박리불량 미검사 
					IAddress = DefectImg.GetImagePtr(); // Pixel 정보 추출 목적 

					A1CirDeftectCnt  = GetPolygonSize(A1CirInPoint, A1CirDefectPoint, A1CirInEdgeCount, 50, 5); // Dark Defect 
					// 박리 불량 검사 
					if(!IsMarkBtm && A1CirInCrackCount>0 && BrokenThresHold!=0){
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel 정보 추출 목적 
						A1CirCrackCnt  = GetCircleCrackSize(A1CircleData, A1CirCrackPoint, A1CirInCrackCount, 30,ACirFitDiameter1,ACircleFit1Center); //Bright Defect 
					}
				}

				if(A2CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel 정보 추출 목적 
					A2CirDeftectCnt  = GetPolygonSize(A2CirInPoint, A2CirDefectPoint, A2CirInEdgeCount, 50, 5); // Dark Defect 

					// 박리 불량 검사 
					if(!IsMarkBtm && A2CirInCrackCount>0 && BrokenThresHold!=0){
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel 정보 추출 목적 
						A2CirCrackCnt  = GetCircleCrackSize(A2CircleData, A2CirCrackPoint, A2CirInCrackCount, 30,ACirFitDiameter2,ACircleFit2Center); // Bright Defect 
					}
				}
				// 				LogUnit.SetLog("Polygon_Defect_END");     
			}
			else                          { // A_Side 영역 Arc Mark Mode 검사 시퀀스 (A_Side영역은 Only MarkMode만 존재함)
				// 				LogUnit.SetLog("Mark_Defect_START");     
				if(A1CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel 정보 추출 목적 
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

						if(k==0){ // BTM Defect 검사를 위한 좌표값 및 Dx,Dy 산출 
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

					// 박리불량 검사 시퀀스 
					if(!IsMarkBtm && A1CirInCrackCount>0 && BrokenThresHold!=0){
						for(int i=0; i<A1CirInCrackCount; i++){
							A1CircleData[i].Dv  =0.0f;
							A1CircleData[i].Dis =0.0f;
							A1CircleData[i].P.x  = A1CirInCrackPoint[i].x;
							A1CircleData[i].P.y  = A1CirInCrackPoint[i].y;
						}
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel 정보 추출 목적 
						A1CirCrackCnt  = GetCircleCrackSize(A1CircleData, A1CirCrackPoint, A1CirInCrackCount, 30,CrackACirDiameter1,ACircleFit1Center);
					}
				}

				if(A2CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel 정보 추출 목적 
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

						if(k==0){ // BTM Defect 검사를 위한 좌표값 및 Dx,Dy 산출 
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

					// 박리불량 검사 시퀀스 
					if(!IsMarkBtm && A2CirInCrackCount>0 && BrokenThresHold!=0){
						for(int i=0; i<A2CirInCrackCount; i++){
							A2CircleData[i].Dv  =0.0f;
							A2CircleData[i].Dis =0.0f;
							A2CircleData[i].P.x  = A2CirInCrackPoint[i].x;
							A2CircleData[i].P.y  = A2CirInCrackPoint[i].y;
						}
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel 정보 추출 목적 
						A2CirCrackCnt  = GetCircleCrackSize(A2CircleData, A2CirCrackPoint, A2CirInCrackCount, 20,CrackACirDiameter2,ACircleFit2Center);
					}
				}
				// 				LogUnit.SetLog("Mark_Defect_END");     
			}

			// SIDE A 검사 
			////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(TopEdgePoint[1].x+SideMargin), (int)((TopEdgePoint[1].y+TopEdgePoint[2].y)/2), TopWidth-(2*SideMargin), FTopHeight);
			if((TopEdgePoint[1].x+SideMargin) <=0          || ((TopEdgePoint[1].y+TopEdgePoint[2].y)/2)<=0           || TopWidth-(2*SideMargin)<=0          || FTopHeight<=0          ) return false;
			if((TopEdgePoint[1].x+SideMargin) >=ImageWidth || ((TopEdgePoint[1].y+TopEdgePoint[2].y)/2)>TopLimitLine || TopWidth-(2*SideMargin)>=ImageWidth || FTopHeight>TopLimitLine) return false;
			if((TopEdgePoint[1].x+SideMargin)+(TopWidth-(2*SideMargin)) >=ImageWidth || ((TopEdgePoint[1].y+TopEdgePoint[2].y)/2)+ FTopHeight>TopLimitLine                            ) return false;
			// Blob 검사 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
					for(int n=ADefectPoint[ACount].Rect.top+1 ; n<ADefectPoint[ACount].Rect.bottom-1; n++){
						for(int m=ADefectPoint[ACount].Rect.left+1; m<ADefectPoint[ACount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}

					ADefectPoint[ACount].Round  = PixelCount;
					ADefectPoint[ACount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width/2 )+(TopEdgePoint[1].x+SideMargin));
					ADefectPoint[ACount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2)+((TopEdgePoint[1].y+TopEdgePoint[2].y)/2));


					// Image Rotate 기준 좌표를 다시 Rotate한다 _ Display 목적 
					if(ImgDegree!=0.0f){
						ADefectPoint[ACount].P = Math.OnRotate(CenterPoint, ADefectPoint[ACount].P, (ImgDegree));
					}
					// Image Rotate 기준 좌표를 회전 이후에 ImageHeight값을 더하여 실제 좌표를 산출한다 
					ADefectPoint[ACount].P.y    =(float)(ADefectPoint[ACount].P.y+ImageHeight);

					//Merget위해서 필요함 
					ADefectPoint[ACount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2) +((TopEdgePoint[1].y+TopEdgePoint[2].y)/2)+ImageHeight);
					ADefectPoint[ACount].Rect.bottom =(long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);
					ACount++;
				}
			}
			BlobCount =0;

			// SIDE B 검사 
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

			//  Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) + (TopEdgePoint[3].y+Margin)+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount =0;

			// 외곽라인에서 Mark까지의 거리 산출 
			if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = ((Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x);
			else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP0.x - FP1.x);
			else                                                      RDistance = ((Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x);

			// SIDE D 검사 
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

			// Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);

			// Blob 필터링 
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
					// 내부 Pixel Data 축출 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapTopThresHold) PixelCount++;
						}
					}
					// Display 데이터 필요함 
					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
					DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(TopEdgePoint[0].y+Margin)+ImageHeight );
					DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

					// Merge 위해서 데이터 필요함 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(TopEdgePoint[0].y+Margin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount =0;

			// 외곽라인에서 Mark까지의 거리 산출 
			if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = (FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2));
			else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = (FP0.x -  Line[waLeftWall].FP0.x)                                                     ;
			else                                                    LDistance = (FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2)); 
		}


		// Cell Size 측정 데이터 넘겨줌(Image 회전 없을경우)
		LeftStaPos.x   =  FLTPoint.x            ;
		LeftStaPos.y   =  FLTPoint.y+ImageHeight;
		CenterStaPos.x =  FLTPoint.x + (FRTPoint.x - FLTPoint.x)/2            ; 
		CenterStaPos.y = (FLTPoint.y + FRTPoint.y)              /2+ImageHeight; 

		// FLTPoint,FRTPoint 기준 Image Rotate 적용하여 Display 목적으로 원본이미지 기준 좌표값 복원시킨
		if(ImgDegree!=0.0f){
			// 검사영역 분리과 상관없이 무조건 각도 적용하여 다시 계산해야 하는 좌표값들 
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

			// Cell Size 측정 데이터 넘겨줌(Image  회전)
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

				// Corner Defect 좌표도 각도 적용하여 변경해야함 
				if(C1Count!=0){
					for(int k=0; k<C1Count; k++){
						// Fram기준 Y좌표값으로 원복한다 
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
						// Fram기준 Y좌표값으로 원복한다 
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
			else if(IsCircleTop) { // Display 목적으로 월래 각도 기준으로 좌표값을 다시 회전 시켜야 한다   
				// A영역 Circle_Defect 좌표 이미지 기준 회전함
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
					// A영역 Circle_Defect 좌표 이미지 기준 회전함
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

				if(A1CirCenter      .x!=0.0f && A1CirCenter      .y!=0.0f) A1CirCenter        = Math.OnRotate(CenterPoint, A1CirCenter       , (ImgDegree)); // 외곽 교차점 기준  
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

				if(A2CirCenter      .x!=0.0f && A2CirCenter      .y!=0.0f) A2CirCenter        = Math.OnRotate(CenterPoint, A2CirCenter       , (ImgDegree)); // 외곽 교차점 기준  
				if(RealA2CirCenter  .x!=0.0f && RealA2CirCenter  .y!=0.0f) RealA2CirCenter    = Math.OnRotate(CenterPoint, RealA2CirCenter   , (ImgDegree)); // Euresys 
				if(ACircleFit2Center.x!=0.0f && ACircleFit2Center.y!=0.0f) ACircleFit2Center  = Math.OnRotate(CenterPoint, ACircleFit2Center , (ImgDegree)); // FAVION
			}

			// GrindMarkPoint Rotate (Display 목적) 
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

				// Pad ROI Rect Rotate (Display 목적) 
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

		// NOTCH RectC : 젓가락 모양
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
		// Blob 검사 진행함 
		CodeImageSelection.Clear();
		CodeImageEncoder.Encode(DefectROI,CodeImage);
		CodeImageSelection.Clear();
		CodeImageSelection.AddObjects(CodeImage);
		// Blob 필터링 
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

				// 내부 Pixel Data 축출 
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

				// Merge 위해서 데이터 필요함 
				BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +ImageHeight);
				BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
				BCount++;
			}
		}
		BlobCount =0;
		//  		LogUnit.SetLog("STATIC_Side_B_END_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
		// 외곽라인에서 Mark까지의 거리 산출 
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
		// Blob 검사 진행 
		CodeImageSelection.Clear();
		CodeImageEncoder.Encode(DefectROI,CodeImage);
		CodeImageSelection.Clear();
		CodeImageSelection.AddObjects(CodeImage);
		// Blob 필터링 
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

				// 내부 Pixel Data 축출 
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

				// Merge 위해서 데이터 필요함 
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
		// 외곽라인에서 Mark까지의 거리 산출 
		// CellSize 
		if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2);
		else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = FP0.x -  Line[waLeftWall].FP0.x;
		else                                                    LDistance = FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2); 
		////////////////////////////////////////////////////////////////////////////////////////////////

		// Broken 및 박리 불량 검사 시퀀스(EnCap만 진행함)
		if(!IsMarkBtm && BrokenThresHold!=0){
			RotateBrokenImg.SetSize(GImage->GetWidth(), GImage->GetHeight())       ;
			EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&RotateBrokenImg);
			EasyImage::Threshold(GImage    ,&RotateBrokenImg, BrokenThresHold)     ;

			DefectROI.Attach(&RotateBrokenImg);
			CodeImageSelection.SetFeretAngle(0.00f);
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
			IAddress = RotateBrokenImg.GetImagePtr();

			FRightWidth = 60; // 테스트 목적
			FLeftWidth  = 60; // 테스트 목적

			// Side_B Bright Defect 검출 및 분류(Broken,반원형 박리___꽃박리)
			/////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(RSP.x-FRightWidth-SideMargin), (int)RSP.y, FRightWidth, RightHeight);
			if((RSP.x-FRightWidth-SideMargin) <=0          ||(RSP.y)<0           || FRightWidth<=0          || RightHeight<0          )return false;
			if((RSP.x-FRightWidth-SideMargin) >=ImageWidth ||(RSP.y)>ImageHeight || FRightWidth>=ImageWidth || RightHeight>ImageHeight)return false;
			if((RSP.x-FRightWidth-SideMargin)+FRightWidth >=ImageWidth ||(RSP.y)+RightHeight>ImageHeight                              )return false;
			//Line Position
			LinePosition = RSP.x-FRightWidth-SideMargin;
			// Blob 검사 진행함 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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
					BCrackPoint[BCrackCount].LineAttach  =((CodeImageSelection.GetElement(k).GetRightLimit() > (FRightWidth-5))>0)? 1:0; //LineAttach:1 반원형 박리, 0: 꽃박리 

					PixelCount = 0;
					PixelValue = 0;

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					BCrackPoint[BCrackCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BCrackPoint[BCrackCount].Height/2) +ImageHeight);
					BCrackPoint[BCrackCount].Rect.bottom =(long)(BCrackPoint[BCrackCount].Rect.top + BCrackPoint[BCrackCount].Height);
					BCrackCount++;
				}
			}
			BlobCount =0;

			// Side_D Bright Defect 검출 및 분류(Broken,반원형 박리___꽃박리)
			/////////////////////////////////////////////////////////////////////////////////////////////////
			DefectROI.SetPlacement((int)(LSP.x+SideMargin),(int)LSP.y, FLeftWidth, LeftHeight);
			if((LSP.x+SideMargin) <=0          || (LSP.y)<0           || FLeftWidth<=0          || LeftHeight<0          ) return false;
			if((LSP.x+SideMargin) >=ImageWidth || (LSP.y)>ImageHeight || FLeftWidth>=ImageWidth || LeftHeight>ImageHeight) return false;
			if((LSP.x+SideMargin)+FLeftWidth >=ImageWidth || (LSP.y)+LeftHeight>ImageHeight                              ) return false;
			// Line Position
			LinePosition = LSP.x+SideMargin;
			// Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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
					DCrackPoint[DCrackCount].LineAttach  =(CodeImageSelection.GetElement(k).GetLeftLimit() < 5)? 1:0; //LineAttach:1 반원형 박리, 0: 꽃박리 

					PixelCount = 0;
					PixelValue = 0;

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					DCrackPoint[DCrackCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/2) +ImageHeight);
					DCrackPoint[DCrackCount].Rect.bottom =(long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
					DCrackCount++;
				}
			}
			BlobCount =0;
		}
		Index = 0;
		return true;

		// NOTCH RectC : 컵모양
		//------------------------------------------------------------------
	case 2 :

		if(IsCornerBtm)     { //Corner Cutting 일경우 Defect 검사 시퀀스 
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
			// Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(SP.y+BtmMargin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount=0;
			// 외곽라인에서 Mark까지의 거리 산출 
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
			// Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(EP.y+BtmMargin)+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount =0;
			// 외곽라인에서 Mark까지의 거리 산출 
			if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x;
			else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance =  Line[waRightWall].FP0.x - FP1.x;
			else                                                      RDistance = (Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x;
			////////////////////////////////////////////////////////////////////////////////////////////////


			// SIDE C 
			// Btm검사시 오버킬을 줄위기 위해서 
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
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					// 						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +((FLBBPoint.y+FRBBPoint.y)/2-FBtmHeight-1)+ImageHeight);
					CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BTMLine-FBtmHeight)+ImageHeight);
					CDefectPoint[CCount].Rect.bottom =(long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
					CCount++;
				}
			}
			BlobCount =0;
		}
		else if(IsRectBtm)  { //직사각형 경우 Defect 검사 시퀀스 
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
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
						}
					}

					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +LinePosition   );
					DDefectPoint[DCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+(SP.y+BtmMargin)+ImageHeight     );

					// Merge 위해서 데이터 필요함 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(SP.y+BtmMargin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount=0;
			// 외곽라인에서 Mark까지의 거리 산출 
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
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
					for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
						for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < EnCapBtmThresHold) PixelCount++;
						}
					}
					BDefectPoint[BCount].Round  = PixelCount;
					BDefectPoint[BCount].P.x    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 )+(LinePosition));
					BDefectPoint[BCount].P.y    =(float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)+(EP.y+BtmMargin)+ImageHeight   );

					// Merge 위해서 데이터 필요함 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(EP.y+BtmMargin)+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}

			BlobCount =0;
			// 외곽라인에서 Mark까지의 거리 산출 
			//if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x;
			//else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance =  Line[waRightWall].FP0.x - FP1.x;
			//else                                                      RDistance = (Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x;

			if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x;
			else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance =  Line[waRightWall].FP0.x - FP1.x;
			else                                                      RDistance = (Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x;
			////////////////////////////////////////////////////////////////////////////////////////////////

			//SIDE C
			////////////////////////////////////////////////////////////////////////////////////////////////
			// Btm검사시 오버킬을 줄위기 위해서 
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
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					// 						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +((FLBBPoint.y+FRBBPoint.y)/2-FBtmHeight-1)+ImageHeight);
					CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BTMLine-FBtmHeight)+ImageHeight);
					CDefectPoint[CCount].Rect.bottom =(long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
					CCount++;
				}
			}
			BlobCount =0;
			////////////////////////////////////////////////////////////////////////////////////////////////
		}
		else if(IsCircleBtm){ //Circle Cutting일경우 Defect 검사 시퀀스 
			if(FInspectMode==Insp_Polygon){ // 비선형 검사에서는 Dx,Dy,R 미검사 
				if(C1CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel 정보 추출 목적 
					C1CirDeftectCnt  = GetPolygonSize(C1CirInPoint, C1CirDefectPoint, C1CirInEdgeCount, 50, 5); // Dark Defect 

					if(!IsMarkBtm && C1CirInCrackCount>0 && BrokenThresHold!=0){
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel 정보 추출 목적 
						C1CirCrackCnt  = GetCircleCrackSize(C1CircleData, C1CirCrackPoint, C1CirInCrackCount, 30,CirFitDiameter1,CircleFit1Center); // Bright Defect 
					}
				}

				if(C2CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel 정보 추출 목적 
					C2CirDeftectCnt  = GetPolygonSize(C2CirInPoint, C2CirDefectPoint, C2CirInEdgeCount, 50, 5); // Dark Defect 

					if(!IsMarkBtm && C2CirInCrackCount>0 && BrokenThresHold!=0){
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel 정보 추출 목적 
						C2CirCrackCnt  = GetCircleCrackSize(C2CircleData, C2CirCrackPoint, C2CirInCrackCount, 30,CirFitDiameter2,CircleFit2Center); // Bright Defect 
					}
				}
			}
			else { // MarkMode, TrackMode 
				if(C1CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel 정보 추출 목적 
					MaxDis =0.0f        ;
					MinDis =10000.0f    ;
					MaxDv  =0.0f        ;
					MinDv  =10000.0f    ;
					StdMaxDis =0.0f     ;
					StdMinDis =10000.0f ;
					MinCirCount =0      ;
					// 삼항 연산자 최소값 구하기  
					MinCirCount = C1CirInEdgeCount;

					CircleCount=0;TotalCirDis=0.0f; AvgCirDis=0.0f;
					for(int k=0; k<MinCirCount; k++){
						if(FInspectMode==Insp_Mark){ // MarkMode 
							if(C1CirInPoint[k].x==0    || C1CirInPoint[k].y==0    || k>=MAX_CIRCLE_EDGEPOINT) continue;

							if(IsMarkBtm){ // RealC1CirCneter값은 연마안쪽 EdgePoint기준 Circle_Fitting 결과물 
								// 							Dis    = Math.GetLengthFrPointToPoint(RealC1CirCenter,C1CirInPoint[k] ); // Euresys Circle Fitting
								Dis = Math.GetLengthFrPointToPoint(CircleFit1Center,C1CirInPoint[k]   ); // FAVION  Circle Fitting 
							}
							else {
								Dis    = Math.GetLengthFrPointToPoint(CircleFit1Center,C1CirInPoint[k]   );
							}

							if(Dis>MaxDis) MaxDis = Dis;
							if(Dis<MinDis) MinDis = Dis;

							if(k==0){ // BTM Defect 검사를 위한 좌표값 및 Dx,Dy 산출 
								// 						    C1CirDy = Math.GetLengthFrPointToPoint(RealC1CirCenter,C1CirInPoint[k]);
								C1CirDy = Math.GetLengthFrPointToPoint(CircleFit1Center,C1CirInPoint[k]);
							} 
							if(k==(C1CirInEdgeCount-1)){
								// 						    C1CirDx = Math.GetLengthFrPointToPoint(RealC1CirCenter,C1CirInPoint[k]);
								C1CirDx = Math.GetLengthFrPointToPoint(CircleFit1Center,C1CirInPoint[k]);
							}
						}
						//Data 취합 
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
						// 					C1CircleData[i].Dv = fabs(AvgCirDis - C1CircleData[i].Dis); // Avg EdgePoint 기준 편차값 
						C1CircleData[i].Dv = (CirFitDiameter1 - C1CircleData[i].Dis) >0 ?(CirFitDiameter1 - C1CircleData[i].Dis):0;
						if(C1CircleData[i].Dv>MaxDv) MaxDv = C1CircleData[i].Dv;
						if(C1CircleData[i].Dv<MinDv) MinDv = C1CircleData[i].Dv;
					}
					C1CirDeftectCnt  = GetCircleSize(C1CircleData, C1CirDefectPoint, CircleCount, 5); // Dark Defect 

					// 박리불량 검사 시퀀스 
					if(!IsMarkBtm && C1CirInCrackCount>0 && BrokenThresHold!=0){
						for(int i=0; i<C1CirInCrackCount; i++){
							C1CircleData[i].Dv  =0.0f;
							C1CircleData[i].Dis =0.0f;
							C1CircleData[i].P.x  = C1CirInCrackPoint[i].x;
							C1CircleData[i].P.y  = C1CirInCrackPoint[i].y;
						}
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel 정보 추출 목적 
						C1CirCrackCnt  = GetCircleCrackSize(C1CircleData, C1CirCrackPoint, C1CirInCrackCount, 30, CrackCirDiameter1,CircleFit1Center); // Bright Defect 
					}
				}

				// C2Circle Calculate 
				if(C2CirInEdgeCount!=0){
					IAddress = DefectImg.GetImagePtr(); // Pixel 정보 추출 목적 
					MaxDis =0.0f     ;
					MinDis =10000.0f ;
					MaxDv  =0.0f     ;
					MinDv  =10000.0f ;
					StdMaxDis =0.0f     ;
					StdMinDis =10000.0f ;
					MinCirCount =0      ;
					// 삼항 연산자 최소값 구하기  
					MinCirCount =  C2CirInEdgeCount;

					CircleCount=0;TotalCirDis=0.0f; AvgCirDis=0.0f;
					for(int k=0; k<MinCirCount; k++){
						if(FInspectMode==Insp_Mark){ // MarkMode 
							if(C2CirInPoint[k].x==0    || C2CirInPoint[k].y==0    || k>=MAX_CIRCLE_EDGEPOINT) continue;

							if(IsMarkBtm){ // RealC2CirCneter값은 연마안쪽 EdgePoint기준 Circle_Fitting 결과물 
								// 							Dis    = Math.GetLengthFrPointToPoint(RealC2CirCenter,C2CirInPoint[k]   );
								Dis = Math.GetLengthFrPointToPoint(CircleFit2Center,C2CirInPoint[k]     );
								// 							Dis    = Math.GetLengthFrPointToPoint(C2CirCenter,C2CirInPoint[k]   );
							}
							else {         // RealC2CirCneter값은 연마안쪽 EdgePoint기준 Circle_Fitting 결과물 
								Dis    = Math.GetLengthFrPointToPoint(CircleFit2Center,C2CirInPoint[k]   );
							}

							if(Dis>MaxDis) MaxDis = Dis;
							if(Dis<MinDis) MinDis = Dis;

							if(k==0){ // BTM Defect 검사를 위한 좌표값 및 Dx,Dy 산출 
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
						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel 정보 추출 목적 
						C2CirCrackCnt  = GetCircleCrackSize(C2CircleData, C2CirCrackPoint, C2CirInCrackCount, 30, CrackCirDiameter2,CircleFit2Center);
					}
				}
			}


			// Side D 
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			IAddress = DefectImg.GetImagePtr(); // Pixel 정보 추출 목적 
			DefectROI.SetPlacement((int)(SP.x+SideMargin),(int)(SP.y+LeftDegMargin), FLeftWidth, LeftHeight-(2*LeftDegMargin));
			if((SP.x+SideMargin) <=0          || SP.y+LeftDegMargin<0            || FLeftWidth<=0          || LeftHeight-(2*LeftDegMargin)< 0          ) return false;
			if((SP.x+SideMargin) >=ImageWidth || SP.y+LeftDegMargin>BtmLimitLine || FLeftWidth>=ImageWidth || LeftHeight-(2*LeftDegMargin)>BtmLimitLine) return false;
			if((SP.x+SideMargin)+FLeftWidth >=ImageWidth || (SP.y+LeftDegMargin)+(LeftHeight-(2*LeftDegMargin))>BtmLimitLine                           ) return false;

			// Line Position 
			LinePosition = SP.x+SideMargin;
			// Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +(SP.y+LeftDegMargin)+ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount=0;
			// 외곽라인에서 Mark까지의 거리 산출 
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
			// Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob 필터링 
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

					// 내부 Pixel Data 축출 
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

					// Merge 위해서 데이터 필요함 
					BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +(EP.y+RightDegMargin)+ImageHeight);
					BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount =0;
			// 외곽라인에서 Mark까지의 거리 산출 
			if     (Line[waRightWall].FP0.x >Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x+(Line[waRightWall].FP0.x - Line[waRightWall].FP1.x)/2) - FP1.x;
			else if(Line[waRightWall].FP0.x==Line[waRightWall].FP1.x) RDistance =  Line[waRightWall].FP0.x - FP1.x;
			else                                                      RDistance = (Line[waRightWall].FP0.x+(Line[waRightWall].FP1.x - Line[waRightWall].FP0.x)/2) - FP1.x;
			////////////////////////////////////////////////////////////////////////////////////////////////

			// Side C 
			// 개조 프로젝트에서는 기본적인 BTM영역 1개만 검사 한다 
			///////////////////////////////////////////////////////////////////////////////////////////////
			// 첫번째 Btm영역의 직선라인 기준 검사 
			// Dark 불량 검사 진행함 
			if(BtmInLine[0].FP0.x!=0 && BtmInLine[0].FP0.y!=0 && BtmInLine[0].FP1.x!=0 && BtmInLine[0].FP1.y!=0){
				DefectROI.SetPlacement((int)(BtmInLine[0].FP0.x+SideMargin), (int)(BtmInLine[0].FP0.y-FBtmHeight), (BtmInLine[0].FP1.x - BtmInLine[0].FP0.x)-(2*SideMargin), FBtmHeight);
				if((BtmInLine[0].FP0.x+SideMargin) <=0          ||(BtmInLine[0].FP0.y-FBtmHeight)< 0           || (BtmInLine[0].FP1.x - BtmInLine[0].FP0.x)-(2*SideMargin)<=0          || FBtmHeight< 0          ) return false;
				if((BtmInLine[0].FP0.x+SideMargin) >=ImageWidth ||(BtmInLine[0].FP0.y-FBtmHeight)>BtmLimitLine || (BtmInLine[0].FP1.x - BtmInLine[0].FP0.x)-(2*SideMargin)>=ImageWidth || FBtmHeight>BtmLimitLine) return false;
				if((BtmInLine[0].FP0.x+SideMargin)+((BtmInLine[0].FP1.x - BtmInLine[0].FP0.x)-(2*SideMargin)) >=ImageWidth ||(BtmInLine[0].FP0.y-FBtmHeight)+FBtmHeight>BtmLimitLine                             ) return false;
				//Blob 검사 진행함 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);
				// Blob 필터링 
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

						// 내부 Pixel Data 축출 
						for(int n=CDefectPoint[CCount].Rect.top+1 ; n<CDefectPoint[CCount].Rect.bottom-1; n++){
							for(int m=CDefectPoint[CCount].Rect.left+1; m<CDefectPoint[CCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < EnCapBtmThresHold) PixelCount++;
							}
						}
						CDefectPoint[CCount].Round  =  PixelCount;
						CDefectPoint[CCount].P.x    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CDefectPoint[CCount].Width/2 ) +(BtmInLine[0].FP0.x+SideMargin));
						CDefectPoint[CCount].P.y    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BtmInLine[0].FP0.y-FBtmHeight));

						// Image Rotate 기준 좌표를 다시 Rotate한다 _ Display 목적 
						if(ImgDegree!=0.0f){
							CDefectPoint[CCount].P = Math.OnRotate(CenterPoint, CDefectPoint[CCount].P, (ImgDegree));
						}
						// Image Rotate 기준 좌표를 회전 이후에 ImageHeight값을 더하여 실제 좌표를 산출한다 
						CDefectPoint[CCount].P.y    =(float)(CDefectPoint[CCount].P.y+ImageHeight);

						// Merge 위해서 데이터 필요함 
						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +(BtmInLine[0].FP0.y-FBtmHeight)+ImageHeight);
						CDefectPoint[CCount].Rect.bottom =(long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
						CCount++;
					}
				}
			}
			BlobCount =0;

			// 연마 후 세정 작업시 물이 들어갈경우 발생하는 불량 검출 목적 
			// 원형 박리불량, 꽃 박리불량 검출 시퀀스.....EnCap영역에서만 검사를 진행함 
			// Bright Defect 검사 진행 
			if(BrokenThresHold!=0 && !IsMarkBtm){
				// 이미지 변환 
				// 				EasyImage::Threshold(&RotateImg,&DefectImg, BrokenThresHold); 
				DefectROI.Attach(&RotateBrokenImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
				IAddress = RotateBrokenImg.GetImagePtr();

				// TEST 목적 
				FBtmHeight =70;

				if(BtmEdgePoint[0].x!=0 && BtmInLine[0].FP0.y!=0 && BtmEdgePoint[1].x!=0 && BtmInLine[0].FP1.y!=0){
					DefectROI.SetPlacement((int)(BtmEdgePoint[0].x)-50, (int)(BtmInLine[0].FP0.y-FBtmHeight), (BtmEdgePoint[1].x - BtmEdgePoint[0].x)+100, FBtmHeight);
					if((BtmEdgePoint[0].x-50) <=0          ||(BtmInLine[0].FP0.y-FBtmHeight)< 0           || ((BtmEdgePoint[1].x - BtmEdgePoint[0].x)+100)<=0          || FBtmHeight< 0          ) return false;
					if((BtmEdgePoint[0].x-50) >=ImageWidth ||(BtmInLine[0].FP0.y-FBtmHeight)>BtmLimitLine || ((BtmEdgePoint[1].x - BtmEdgePoint[0].x)+100)>=ImageWidth || FBtmHeight>BtmLimitLine) return false;
					if((BtmEdgePoint[0].x-50)+((BtmEdgePoint[1].x - BtmEdgePoint[0].x)) >=ImageWidth ||(BtmInLine[0].FP0.y-FBtmHeight)+FBtmHeight>BtmLimitLine                             ) return false;

					//Blob 검사 진행함 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);
					// Blob 필터링 
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

							// 내부 Pixel Data 축출 
							for(int n=CCrackPoint[CCrackCount].Rect.top+1 ; n<CCrackPoint[CCrackCount].Rect.bottom-1; n++){
								for(int m=CCrackPoint[CCrackCount].Rect.left+1; m<CCrackPoint[CCrackCount].Rect.right-1 ; m++){
									PixelValue = IElement(m,n);
									if(PixelValue > BrokenThresHold) PixelCount++;
								}
							}
							CCrackPoint[CCrackCount].Round  =  PixelCount;
							CCrackPoint[CCrackCount].P.x    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CCrackPoint[CCrackCount].Width/2 ) +(BtmEdgePoint[0].x-50));
							CCrackPoint[CCrackCount].P.y    = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CCrackPoint[CCrackCount].Height/2) +(BtmInLine[0].FP0.y-FBtmHeight));

							// Image Rotate 기준 좌표를 다시 Rotate한다 _ Display 목적 
							if(ImgDegree!=0.0f){
								CCrackPoint[CCrackCount].P = Math.OnRotate(CenterPoint, CCrackPoint[CCrackCount].P, (ImgDegree));
							}
							// Image Rotate 기준 좌표를 회전 이후에 ImageHeight값을 더하여 실제 좌표를 산출한다 
							CCrackPoint[CCrackCount].P.y    =(float)(CCrackPoint[CCrackCount].P.y+ImageHeight);

							// Merge 위해서 데이터 필요함 
							CCrackPoint[CCrackCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CCrackPoint[CCrackCount].Height/2) +(BtmInLine[0].FP0.y-FBtmHeight)+ImageHeight);
							CCrackPoint[CCrackCount].Rect.bottom =(long)(CCrackPoint[CCrackCount].Rect.top + CCrackPoint[CCrackCount].Height);
							CCrackCount++;
						}
					}
				}
				BlobCount =0;
				// Notch부 직선영역 Bright Defect 검사 시퀀스 종료  
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		// 원본 이미지 회전하여 검사 이후 화면에 디스플레이 목적으로 좌표값을 다시 계사한다 
		////////////////////////////////////////////////////////////////////////////////////////////////
		// Cell Size 측정 데이터 넘겨줌(Image 회전 없을경우)
// 		if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f && FRBPoint.x!=0.0f && FRBPoint.y!=0.0f){
// 			LeftEndPos.x     =  FLBPoint.x            ;
// 			LeftEndPos.y     =  FLBPoint.y+ImageHeight;
// 
// 			CenterEndPos.x =  BtmOutLine[2].FP0.x + (BtmOutLine[2].FP0.x - BtmOutLine[2].FP1.x)/2; 
// 			CenterEndPos.y =  (BtmOutLine[2].FP0.y + BtmOutLine[2].FP1.y)/2+ImageHeight; 
// 		}
		// Notch Cell Height Calc (Image  회전없음)
// 		if(BtmOutLine[2].FP0.x!=0.0f && BtmOutLine[2].FP0.y!=0.0f && BtmOutLine[2].FP1.x!=0.0f && BtmOutLine[2].FP1.y!=0.0f){
// 			NotchCenterPos.x =  BtmOutLine[2].FP0.x + (BtmOutLine[2].FP1.x - BtmOutLine[2].FP0.x)/2.0;
// 			NotchCenterPos.y = (BtmOutLine[2].FP0.y + BtmOutLine[2].FP1.y)/2.0;
// 			NotchCellSizeHeight = Math.GetLengthFrLineToPoint(FLBPoint, FRBPoint, NotchCenterPos);
// 		}
		// Notch Cell Width Calc(Image 회전없음)
		////////////////////////////////////////////////////////////////////////////////////////////////
// 		if(BtmEdgePoint[2].x!=0.0f && BtmEdgePoint[2].y!=0.0f && BtmEdgePoint[7].x!=0.0f && BtmEdgePoint[7].y!=0.0f){
// 			NotchCellSizeWidth = Math.GetLengthFrPointToPoint((float)BtmEdgePoint[2].x,(float)BtmEdgePoint[2].y, (float)BtmEdgePoint[7].x,(float)BtmEdgePoint[7].y);
// 		}
		// Center Cell Size Width Calc(Image 회전과 상관없음)
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

			// Cell Size 측정 데이터 넘겨줌(Image 회전 있음)
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

				// Corner Defect 좌표도 각도 적용하여 변경해야함 
				if(C3Count!=0){
					for(int k=0; k<C3Count; k++){
						// Fram기준 Y좌표값으로 원복한다 
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
						// Fram기준 Y좌표값으로 원복한다 
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
				if(C1CirCenter     .x!=0.0f && C1CirCenter     .y!=0.0f) C1CirCenter        = Math.OnRotate(CenterPoint, C1CirCenter         , (ImgDegree)); // 외곽 교차점 기준  
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
				if(C2CirCenter     .x!=0.0f && C2CirCenter     .y!=0.0f) C2CirCenter        = Math.OnRotate(CenterPoint, C2CirCenter         , (ImgDegree)); // 외곽 교차점 기준  
				if(RealC2CirCenter .x!=0.0f && RealC2CirCenter .y!=0.0f) RealC2CirCenter    = Math.OnRotate(CenterPoint, RealC2CirCenter     , (ImgDegree));
				if(CircleFit2Center.x!=0.0f && CircleFit2Center.y!=0.0f) CircleFit2Center   = Math.OnRotate(CenterPoint, CircleFit2Center    , (ImgDegree));
			}

			// GrindMarkPoint Rotate (Display 목적) 
			if(IsMarkBtm){
				// C1,C2 Grind Mark 영역 회전
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

				// Btm 영역 연마,가공 측정 기준 Display목적 
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

				// Notch ROI Rect Rotate (Display 목적) 
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
			               +DBurrCount+ACrackCount+BCrackCount+CCrackCount+DCrackCount+A1CirCrackCnt+A2CirCrackCnt+C1CirCrackCnt+C2CirCrackCnt; // 전체 불량개수 계산

		// BTM 영역 검사 종료 후 Cell Size 측정 
		// 검사 종료 후 Cell Size 측정함 
		if(LeftStaPos.x!=0.0f && LeftStaPos.y!=0.0f && LeftEndPos.x!=0.0f && LeftEndPos.y!=0.0f){
			LeftCellSizeHeight = Math.GetLengthFrPointToPoint(LeftStaPos, LeftEndPos);
		}
		if(CenterStaPos.x!=0.0f && CenterStaPos.y!=0.0f && CenterEndPos.x!=0.0f && CenterEndPos.y!=0.0f){
			CenCellSizeHeight  = Math.GetLengthFrPointToPoint(CenterStaPos, CenterEndPos);
		}

		// Dark Defect Blob Merge
		///////////////////////////////////////////////////////////////////////////////////////////////
		// Circle_1 불량 Merge 기능 
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

		// Circle_2 불량 Merge 기능 
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

		// Side C Merge (박리불량)
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

	// 시작부분 Broken발생시 
	int LineMargin =30;
	int LineRange  =80;

	// Euresys ROI 
	// Median Filtter 적용 목적 ROI   
	EROIBW8   FilterROI                ;

	// Circle 생성시 필요한 데이터 
	const int   TrackLineMargin = 5    ;
	const int   CirAngleMargin  = 10   ;
	const int   CirCenterMargin = 0    ;
	const float CirCrackMargin  = 30.0f;  
	const float CirOffsetMargin = 7.0f ;

	//Polygon Value Test 
	int   PolygonMarginCnt= 0 ;

	// Circle Notch부 Trench_Width 길이 마진값 
	const int TrenchWidthMargin =2000;

	// ㅏMark Find 검사시 필요한 ROI영역 데이터 
	int GrindRectWidth=0, GrindRectHeight=0;

	// Value Init 
	double AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;

	// FAVION Circle Fitting 필요한 Data 
	float SampleSize   = 0.0f; //0.5~0.8이하로 입력할것
	float DistDevRatio = 0.15; //0.1~0.25 이하로 입력할것

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

	//Polygon Algorithm 적용시 OutData,InData 개별 최단거리 계산시 필요한 Count
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

	// TOP, BTM Cell_Size, Grind_Size 검사시 필요 변수

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
	FLTPoint.x = 0.0f; //좌측라인과 상단라인의 교차점 좌표
	FLTPoint.y = 0.0f; //좌측라인과 상단라인의 교차점 좌표
	FRTPoint.x = 0.0f; //우측라인과 상단라인의 교차점 좌표
	FRTPoint.y = 0.0f; //우측라인과 상단라인의 교차점 좌표
	FRBPoint.x = 0.0f; //우측라인과 하단라인의 교차점 좌표
	FRBPoint.y = 0.0f; //우측라인과 하단라인의 교차점 좌표
	FLBPoint.x = 0.0f; //좌측라인과 하단라인의 교차점 좌표
	FLBPoint.y = 0.0f; //좌측라인과 하단라인의 교차점 좌표

	FLTLPoint.x = 0.0f; //좌측라인과   첫번째 코너 교차점 좌표 
	FLTLPoint.y = 0.0f; //좌측라인과   첫번째 코너 교차점 좌표 
	FLTTPoint.x = 0.0f; //위쪽라인과   첫번째 코너 교차점 좌표 
	FLTTPoint.y = 0.0f; //위쪽라인과   첫번째 코너 교차점 좌표 
	FRTRPoint.x = 0.0f; //우측라인과   두번째 코너 교차점 좌표 
	FRTRPoint.y = 0.0f; //우측라인과   두번째 코너 교차점 좌표 
	FRTTPoint.x = 0.0f; //위쪽라인과   두번째 코너 교차점 좌표 
	FRTTPoint.y = 0.0f; //위쪽라인과   두번째 코너 교차점 좌표 
	FRBRPoint.x = 0.0f; //우측라인과   세번째 코너 교차점 좌표 
	FRBRPoint.y = 0.0f; //우측라인과   세번째 코너 교차점 좌표 
	FRBBPoint.x = 0.0f; //아래쪽라인과 세번째 코너 교차점 좌표 
	FRBBPoint.y = 0.0f; //아래쪽라인과 세번째 코너 교차점 좌표 
	FLBLPoint.x = 0.0f; //좌측라인과   네번째 코너 교차점 좌표 
	FLBLPoint.y = 0.0f; //좌측라인과   네번째 코너 교차점 좌표 
	FLBBPoint.x = 0.0f; //아래쪽라인과 네번째 코너 교차점 좌표 
	FLBBPoint.y = 0.0f; //아래쪽라인과 네번째 코너 교차점 좌표 

	//NOTCH Inspection
	FLTLInPoint.x = 0.0f; //왼쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FLTLInPoint.y = 0.0f; //왼쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FLTTInPoint.x = 0.0f; //위쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FLTTInPoint.y = 0.0f; //위쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FRTRInPoint.x = 0.0f; //오른쪽라인과 코너 연마안쪽 라인과 교차점 좌표
	FRTRInPoint.y = 0.0f; //오른쪽라인과 코너 연마안쪽 라인과 교차점 좌표
	FRTTInPoint.x = 0.0f; //위쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FRTTInPoint.y = 0.0f; //위쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FLBLInPoint.x = 0.0f; //왼쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FLBLInPoint.y = 0.0f; //왼쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FLBBInPoint.x = 0.0f; //아래쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FLBBInPoint.y = 0.0f; //아래쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FRBRInPoint.x = 0.0f; //오른쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FRBRInPoint.y = 0.0f; //오른쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FRBBInPoint.x = 0.0f; //아래쪽라인과   코너 연마안쪽 라인과 교차점 좌표
	FRBBInPoint.y = 0.0f; //아래쪽라인과   코너 연마안쪽 라인과 교차점 좌표

	FLTLMeaPoint.x = 0.0f; //왼쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표
	FLTLMeaPoint.y = 0.0f; //왼쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표
	FLTTMeaPoint.x = 0.0f; //위쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표
	FLTTMeaPoint.y = 0.0f; //위쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표
	FRTRMeaPoint.x = 0.0f; //오른쪽라인과 코너 연마량 측정 기준 라인과 교차점 좌표
	FRTRMeaPoint.y = 0.0f; //오른쪽라인과 코너 연마량 측정 기준 라인과 교차점 좌표
	FRTTMeaPoint.x = 0.0f; //위쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표
	FRTTMeaPoint.y = 0.0f; //위쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표

	FLBLMeaPoint.x = 0.0f; //왼쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표
	FLBLMeaPoint.y = 0.0f; //왼쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표
	FLBBMeaPoint.x = 0.0f; //아래쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표
	FLBBMeaPoint.y = 0.0f; //아래쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표
	FRBRMeaPoint.x = 0.0f; //오른쪽라인과 코너 연마량 측정 기준 라인과 교차점 좌표
	FRBRMeaPoint.y = 0.0f; //오른쪽라인과 코너 연마량 측정 기준 라인과 교차점 좌표
	FRBBMeaPoint.x = 0.0f; //아래쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표
	FRBBMeaPoint.y = 0.0f; //아래쪽라인과   코너 연마량 측정 기준 라인과 교차점 좌표

	// Notch Cell Track 교차점 좌표 
	FTRLBPoint.x = 0.0f; //좌측트랙라인과 하단트랙라인의 교차점 좌표
	FTRLBPoint.y = 0.0f; //좌측트랙라인과 하단트랙라인의 교차점 좌표
	FTRRBPoint.x = 0.0f; //우측트랙라인과 하단트랙라인의 교차점 좌표
	FTRRBPoint.y = 0.0f; //우측트랙라인과 하단트랙라인의 교차점 좌표

	// Mark에서 TrackLine까지 거리 값 
	float MarkToTrackLineDis1 =0.0f;
	float MarkToTrackLineDis2 =0.0f;

	// 외곽라인에서 TrackLine까지 거리 값 
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

	// BtmTrackLine 좌표
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
		if (FLeftWall) { //SoulBrain 개조 ==> 기본 외곽라인 검출 
			EasyImage::Threshold(GImage,&BinaryImg, LeftEdgeThresHold);
			if(LeftEdgeThresHold==0) {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
			else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

			if(FShape==shRectC){
				SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()                                );
				SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()                                );
				SingleLine->IRoi->SetWidth (IRoi->GetWidth()                               );
				// ChamberCut VS Circle VS Rect경우에 따라서 Left영역 Fitting구간 높이가 달라짐 
				if     (IsCornerBtm) SingleLine->IRoi->SetHeight(FEndLine-LBCornerHeight   );
				else if(IsCircleBtm) SingleLine->IRoi->SetHeight(FEndLine-CSideCircleHeight);
				else                 SingleLine->IRoi->SetHeight(IRoi->GetHeight()         );
				SingleLine->SetIncrement(1);
			}
			else if(FShape==shRectH) {
				if(IsCircleTop){ // Circle 검사시 
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

			// Btm,Top 상관없이 첫번째
			LeftOutLine[0].FP0.x = Line[0].FP0.x;
			LeftOutLine[0].FP0.y = Line[0].FP0.y;
			LeftOutLine[0].FP1.x = Line[0].FP1.x;
			LeftOutLine[0].FP1.y = Line[0].FP1.y;

			// LeftOutLine....make OutLine Data 
			// 현재의 데이터를 넘겨준다 
			// 검사중 기준라인을 찾지 못할경우 이전데이터 넘겨줘서 기준라인 생성함 
			if(LeftOutLine[0].FP0.x!=0.0f && LeftOutLine[0].FP0.y!=0.0f && LeftOutLine[0].FP1.x!=0.0f && LeftOutLine[0].FP1.y!=0.0f){
				PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
				PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
			} // 셀의 상단 시작부터 Broken 발생으로 데이터가 없는경우 
			else if(LeftOutLine[0].FP0.x==0.0f || LeftOutLine[0].FP0.y==0.0f || LeftOutLine[0].FP1.x==0.0f || LeftOutLine[0].FP1.y==0.0f){
				if(PreLeftOutLine[0].FP0.x==0.0f && PreLeftOutLine[0].FP0.y==0.0f && PreLeftOutLine[0].FP1.x==0.0f && PreLeftOutLine[0].FP1.y==0.0f){// 시작부분 Broken발생으로 데이터가 없는경우 
					// Mark좌표를 기준으로 라인좌표값을 다시 설정함 
					ROI_H = IRoi->GetHeight();
					ROI_W = IRoi->GetOrgX()  ;
					LeftOutLine[0].FP0.x = ROI_W;    LeftOutLine[0].FP0.y = 0.0f        ;
					LeftOutLine[0].FP1.x = ROI_W;    LeftOutLine[0].FP1.y = (float)ROI_H;

					//Line 데이터를 PreLine에 넘겨준다 
					PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
					PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
				}
				else { // 중간부분 Broken발생으로 데이터가 없는경우 
					// PreLine 데이터를 넘겨줌 
					LeftOutLine[0].FP0.x = PreLeftOutLine[0].FP0.x;  LeftOutLine[0].FP1.x = PreLeftOutLine[0].FP1.x;  
					LeftOutLine[0].FP0.y = PreLeftOutLine[0].FP0.y;  LeftOutLine[0].FP1.y = PreLeftOutLine[0].FP1.y;  
				}
			}

			// LeftGrindIn 라인 검출 
			if(LeftGrindThresHold!=0){
				EasyImage::Threshold(GImage,&BinaryImg, LeftGrindThresHold);
				SingleLine->GImage = &BinaryImg;
				LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;
				// ROI영역 기준점  계산  
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
				// 영역 설정 
				if(FShape==shRectS){// 스티영역 설정 
					SingleLine->IRoi->SetOrgX  ((int)LSP.x+3);
					SingleLine->IRoi->SetOrgY  (0           );
					SingleLine->IRoi->SetWidth (100         );
					SingleLine->IRoi->SetHeight(ImgH        );
					if((LSP.x+3)<=0 ||(LSP.x+3)>=ImgW) break;
				}
				else               {// 모자,컵 영역설정 
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

				// 연마량 계산 
				GrindWd1=0.0f,GrindWd2=0.0f;
				GrindWd1 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP0);
				GrindWd2 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP1);
				LeftGrindWidth = (GrindWd1+GrindWd2)/2.0;
			}

			// 가공량 측정 기준라인 검출 시퀀스 
			if(LeftStdThresHold!=0 && LeftInLine[0].FP0.x!=0 && LeftInLine[0].FP0.y!=0 && LeftInLine[0].FP1.x!=0 && LeftInLine[0].FP1.y!=0){
				EasyImage::Threshold(GImage,&BinaryImg, LeftStdThresHold);
				SingleLine->GImage = &BinaryImg;
// 				BinaryImg.Save("D:\\ROTATE_IMG\\SoulBrain_MID.bmp");
				LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;

				// 영역기준 계산  
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
				// 영역 설정 
				if(FShape==shRectS){// 스티영역 설정 
					SingleLine->IRoi->SetOrgX  ((int)LSP.x+10);
					SingleLine->IRoi->SetOrgY  (0            );
					SingleLine->IRoi->SetWidth (100          );
					SingleLine->IRoi->SetHeight(ImgH         );
					if((LSP.x+3)<=0 ||(LSP.x+3)>=ImgW) break;
				}
				else               {// 모자,컵 영역설정 
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

				//가공량 계산 
				GrindMea1=0.0f,GrindMea2=0.0f;
				GrindMea1 = Math.GetLengthFrLineToPoint(LeftStdLine[0].FP0,LeftStdLine[0].FP1,LeftOutLine[0].FP0);
				GrindMea2 = Math.GetLengthFrLineToPoint(LeftStdLine[0].FP0,LeftStdLine[0].FP1,LeftOutLine[0].FP1);
				LeftGrindMea = (GrindMea1+GrindMea2)/2.0;
			}
		}
		if (FTopWall) { // SoulBrain 개조  
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
		if (FRightWall) { //SoulBrain 개조 ==> 기본 외곽라인 검출 
			EasyImage::Threshold(GImage,&BinaryImg, RightEdgeThresHold);
			if(LeftEdgeThresHold==0) {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
			else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

			if(FShape==shRectC){
				SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()                                );
				SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()                                );
				SingleLine->IRoi->SetWidth (IRoi->GetWidth()                               );
				// ChamberCut VS Circle VS Rect경우에 따라서 Left영역 Fitting구간 높이가 달라짐 
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

			// Btm,Top 상관없이 첫번째
			RightOutLine[0].FP0.x = Line[2].FP0.x;
			RightOutLine[0].FP0.y = Line[2].FP0.y;
			RightOutLine[0].FP1.x = Line[2].FP1.x;
			RightOutLine[0].FP1.y = Line[2].FP1.y;

			// LeftOutLine....make OutLine Data 
			// 현재의 데이터를 넘겨준다 
			// 검사중 기준라인을 찾지 못할경우 이전데이터 넘겨줘서 기준라인 생성함 
			if(RightOutLine[0].FP0.x!=0.0f && RightOutLine[0].FP0.y!=0.0f && RightOutLine[0].FP1.x!=0.0f && RightOutLine[0].FP1.y!=0.0f){
				PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
				PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
			} // 셀의 상단 시작부터 Broken 발생으로 데이터가 없는경우 
			else if(RightOutLine[0].FP0.x==0.0f || RightOutLine[0].FP0.y==0.0f || RightOutLine[0].FP1.x==0.0f || RightOutLine[0].FP1.y==0.0f){
				if(PreRightOutLine[0].FP0.x==0.0f && PreRightOutLine[0].FP0.y==0.0f && PreRightOutLine[0].FP1.x==0.0f && PreRightOutLine[0].FP1.y==0.0f){// 시작부분 Broken발생으로 데이터가 없는경우 
					// Mark좌표를 기준으로 라인좌표값을 다시 설정함 
					ROI_H = IRoi->GetHeight();
					RightOutLine[0].FP0.x = Line[2].FP0.x;    RightOutLine[0].FP0.y = 0.0f        ;
					RightOutLine[0].FP1.x = Line[2].FP1.x;    RightOutLine[0].FP1.y = (float)ROI_H;

					//Line 데이터를 PreLine에 넘겨준다 
					PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
					PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
				}
				else { // 중간부분 Broken발생으로 데이터가 없는경우 
					// PreLine 데이터를 넘겨줌 
					RightOutLine[0].FP0.x = PreRightOutLine[0].FP0.x;  RightOutLine[0].FP1.x = PreRightOutLine[0].FP1.x;  
					RightOutLine[0].FP0.y = PreRightOutLine[0].FP0.y;  RightOutLine[0].FP1.y = PreRightOutLine[0].FP1.y;  
				}
			}

			// RightGrindIn라인 검출 
			if(RightGrindThresHold!=0){
				EasyImage::Threshold(GImage,&BinaryImg, RightGrindThresHold);
				SingleLine->GImage = &BinaryImg;

				RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;
				// ROI영역 기준점  계산  
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
				// 영역 설정 
				if(FShape==shRectS){// 스티영역 설정 
					SingleLine->IRoi->SetOrgX  ((int)RSP.x-100 );
					SingleLine->IRoi->SetOrgY  (0              );
					SingleLine->IRoi->SetWidth (100            );
					SingleLine->IRoi->SetHeight(ImgH           );
					if((RSP.x-100)<=0 ||(RSP.x-100)>=ImgW) break;
				}
				else               {// 모자,컵 영역설정 
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

				// 연마량 계산 
				GrindWd1=0.0f,GrindWd2=0.0f;
				GrindWd1 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP0);
				GrindWd2 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP1);
				RightGrindWidth = (GrindWd1+GrindWd2)/2.0;
			}

			// 가공량 측정 기준라인 검출 시퀀스 
			if(RightStdThresHold!=0 && RightInLine[0].FP0.x!=0 && RightInLine[0].FP0.y!=0 && RightInLine[0].FP1.x!=0 && RightInLine[0].FP1.y!=0){
				EasyImage::Threshold(GImage,&BinaryImg, RightStdThresHold);
				SingleLine->GImage = &BinaryImg;
				RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;

				// 영역기준 계산  
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
				// 영역 설정 
				if(FShape==shRectS){// 스티영역 설정 
					SingleLine->IRoi->SetOrgX  ((int)RSP.x-100 );
					SingleLine->IRoi->SetOrgY  (0              );
					SingleLine->IRoi->SetWidth (80             );
					SingleLine->IRoi->SetHeight(ImgH           );
					if((RSP.x-100)<=0 ||(RSP.x-100)>=ImgW) break;
				}
				else               {// 모자,컵 영역설정 
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

				//가공량 계산 
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

			// 기본 BtmLine Search 
			SingleLine->OnExecute();
			Line[3].FP0 = SingleLine->GetResultP0();
			Line[3].FP1 = SingleLine->GetResultP1();

			BtmOutLine[0].FP0 =  Line[3].FP0;
			BtmOutLine[0].FP1 =  Line[3].FP1;
		}
		if (FLeftTop    ){ //SoulBrain ==>이미지 회전후 반복검사 진행하므로 필요없음 

		}
		if (FRightTop   ){ //SoulBrain ==>이미지 회전후 반복검사 진행하므로 필요없음 

		}
		if (FRightBottom){ //SoulBrain ==>이미지 회전후 반복검사 진행하므로 필요없음 

		}
		if (FLeftBottom ){ //SoulBrain ==>이미지 회전후 반복검사 진행하므로 필요없음 

		}

		// SoulBrain 외관검사시 셀 얼라인 불안정하여 셀이 틀어졌을경우 
		// 검사 ROI영역을 틀어진 각도 기준으로 회전하여도 외곽라인 근처에 존재하는 패턴으로 인해서 오버킬 발생함 
		// 이로 인해서 부득이하게 원본 이미지를 복사하여 틀어진 각도만큼 회전하여 기본 Edge데이터를 다시 산출 및 Defect검사
		// 회전된 이미지기준으로 검사 진행함 
		if(FShape==shRectH ){ //SoulBrain ROTATE IMAGE
			// Image Roate 전 원본이미지 기준 데이터 산출 
			FLTPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
			FRTPoint = Math.OnIntersect(Line[waRightWall].FP0, Line[waRightWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);

			// Align Mark 기준으로 이미지 회전
			if(FP0.x!=0 && FP0.y!=0 && FP1.x!=0 && FP1.y!=0) ImgDegree = -(Math.GetDegree(FP0.x , FP0.y, FP1.x, FP1.y));

			// 원본 RotateImg Size 설정 및 복사 
			LogUnit.SetLog(L"shRectH_RotateImg_Start");     
			RotateImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
			EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&RotateImg);

			// 원본 복사하여 이미지 회전 
			EasyImage::ScaleRotate(GImage, (float)ImgW/2, (float)ImgH/2, (float)ImgW/2, (float)ImgH/2, 1.0f, 1.0f, ImgDegree, &RotateImg, 4);
			CenterPoint.x = ImgW/2;
			CenterPoint.y = ImgH/2;
// 			RotateImg.Save("D:\\ROTATE_IMG\\SoulBrain_TOP.bmp");

			// Binary RotateImg Size 설정 및 복사 
			RotateBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
			EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBinaryImg);
			// Grind Measure Corner, Circle 검사 목적 
			RotateGrindBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
			EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateGrindBinaryImg);
			LogUnit.SetLog(L"shRectH_RotateImg_End");     

			if(FLeftWall){
				EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftEdgeThresHold); // 기본 LeftEdgeLine
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

				// Btm,Top 상관없이 첫번째
				LeftOutLine[0].FP0.x = Line[0].FP0.x;
				LeftOutLine[0].FP0.y = Line[0].FP0.y;
				LeftOutLine[0].FP1.x = Line[0].FP1.x;
				LeftOutLine[0].FP1.y = Line[0].FP1.y;

				// LeftOutLine....make OutLine Data 
				// 현재의 데이터를 넘겨준다 
				// 검사중 기준라인을 찾지 못할경우 이전데이터 넘겨줘서 기준라인 생성함 
				if(LeftOutLine[0].FP0.x!=0.0f && LeftOutLine[0].FP0.y!=0.0f && LeftOutLine[0].FP1.x!=0.0f && LeftOutLine[0].FP1.y!=0.0f){
					PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
					PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
				} // 셀의 상단 시작부터 Broken 발생으로 데이터가 없는경우 
				else if(LeftOutLine[0].FP0.x==0.0f || LeftOutLine[0].FP0.y==0.0f || LeftOutLine[0].FP1.x==0.0f || LeftOutLine[0].FP1.y==0.0f){
					if(PreLeftOutLine[0].FP0.x==0.0f && PreLeftOutLine[0].FP0.y==0.0f && PreLeftOutLine[0].FP1.x==0.0f && PreLeftOutLine[0].FP1.y==0.0f){// 시작부분 Broken발생으로 데이터가 없는경우 
						// Mark좌표를 기준으로 라인좌표값을 다시 설정함 
						ROI_H = IRoi->GetHeight();
						LeftOutLine[0].FP0.x = Line[0].FP0.x;    LeftOutLine[0].FP0.y = 0.0f        ;
						LeftOutLine[0].FP1.x = Line[0].FP1.x;    LeftOutLine[0].FP1.y = (float)ROI_H;

						//Line 데이터를 PreLine에 넘겨준다 
						PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
						PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
					}
					else { // 중간부분 Broken발생으로 데이터가 없는경우 
						// PreLine 데이터를 넘겨줌 
						LeftOutLine[0].FP0.x = PreLeftOutLine[0].FP0.x;  LeftOutLine[0].FP1.x = PreLeftOutLine[0].FP1.x;  
						LeftOutLine[0].FP0.y = PreLeftOutLine[0].FP0.y;  LeftOutLine[0].FP1.y = PreLeftOutLine[0].FP1.y;  
					}
				}

				// LeftGrindIn 라인 검출 
				if(LeftGrindThresHold!=0){
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftGrindThresHold);
					SingleLine->GImage = &RotateBinaryImg;
					LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;
					// ROI영역 기준점  계산  
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
					// 영역 설정 
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

					// 연마량 계산 
					GrindWd1=0.0f,GrindWd2=0.0f;
					GrindWd1 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP0);
					GrindWd2 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP1);
					LeftGrindWidth = (GrindWd1+GrindWd2)/2.0;
				}

				// 가공량 측정 기준라인 검출 시퀀스 
				if(LeftStdThresHold!=0 && LeftInLine[0].FP0.x!=0 && LeftInLine[0].FP0.y!=0 && LeftInLine[0].FP1.x!=0 && LeftInLine[0].FP1.y!=0){
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftStdThresHold);
					SingleLine->GImage = &RotateBinaryImg;
					LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;

					// 영역기준 계산  
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
					// 영역 설정 
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

					//가공량 계산 
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
				EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightEdgeThresHold); //기본 RightEdgeLine 
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

				// Btm,Top 상관없이 첫번째
				RightOutLine[0].FP0.x = Line[2].FP0.x;
				RightOutLine[0].FP0.y = Line[2].FP0.y;
				RightOutLine[0].FP1.x = Line[2].FP1.x;
				RightOutLine[0].FP1.y = Line[2].FP1.y;

				// RightOutLine....make OutLine Data 
				// 현재의 데이터를 넘겨준다 
				// 검사중 기준라인을 찾지 못할경우 이전데이터 넘겨줘서 기준라인 생성함 
				if(RightOutLine[0].FP0.x!=0.0f && RightOutLine[0].FP0.y!=0.0f && RightOutLine[0].FP1.x!=0.0f && RightOutLine[0].FP1.y!=0.0f){
					PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
					PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
				} // 셀의 상단 시작부터 Broken 발생으로 데이터가 없는경우 
				else if(RightOutLine[0].FP0.x==0.0f || RightOutLine[0].FP0.y==0.0f || RightOutLine[0].FP1.x==0.0f || RightOutLine[0].FP1.y==0.0f){
					if(PreRightOutLine[0].FP0.x==0.0f && PreRightOutLine[0].FP0.y==0.0f && PreRightOutLine[0].FP1.x==0.0f && PreRightOutLine[0].FP1.y==0.0f){// 시작부분 Broken발생으로 데이터가 없는경우 
						// Mark좌표를 기준으로 라인좌표값을 다시 설정함 
						ROI_H = IRoi->GetHeight();
						RightOutLine[0].FP0.x = Line[2].FP0.x;    RightOutLine[0].FP0.y = 0.0f        ;
						RightOutLine[0].FP1.x = Line[2].FP1.x;    RightOutLine[0].FP1.y = (float)ROI_H;

						//Line 데이터를 PreLine에 넘겨준다 
						PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
						PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
					}
					else { // 중간부분 Broken발생으로 데이터가 없는경우 
						// PreLine 데이터를 넘겨줌 
						RightOutLine[0].FP0.x = PreRightOutLine[0].FP0.x;  RightOutLine[0].FP1.x = PreRightOutLine[0].FP1.x;  
						RightOutLine[0].FP0.y = PreRightOutLine[0].FP0.y;  RightOutLine[0].FP1.y = PreRightOutLine[0].FP1.y;  
					}
				}

				// RightGrindIn라인 검출 
				if(RightGrindThresHold!=0){ // ImageRotate 
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightGrindThresHold);
					SingleLine->GImage = &RotateBinaryImg;
					RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;
					// ROI영역 기준점  계산  
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
					// 영역 설정 
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

					// 연마량 계산 
					GrindWd1=0.0f,GrindWd2=0.0f;
					GrindWd1 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP0);
					GrindWd2 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP1);
					RightGrindWidth = (GrindWd1+GrindWd2)/2.0;
				}

				// 가공량 측정 기준라인 검출 시퀀스 
				if(RightStdThresHold!=0 && RightInLine[0].FP0.x!=0 && RightInLine[0].FP0.y!=0 && RightInLine[0].FP1.x!=0 && RightInLine[0].FP1.y!=0){
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightStdThresHold);
					SingleLine->GImage = &RotateBinaryImg;
					RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;

					// 영역기준 계산  
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
					// 영역 설정 
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

					//가공량 계산 
					GrindMea1=0.0f,GrindMea2=0.0f;
					GrindMea1 = Math.GetLengthFrLineToPoint(RightStdLine[0].FP0,RightStdLine[0].FP1,RightOutLine[0].FP0);
					GrindMea2 = Math.GetLengthFrLineToPoint(RightStdLine[0].FP0,RightStdLine[0].FP1,RightOutLine[0].FP1);
					RightGrindMea = (GrindMea1+GrindMea2)/2.0;
				}
			}

			//Image Rotate 후 교차점 다시 계산 
			FLTPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
			FRTPoint = Math.OnIntersect(Line[waRightWall].FP0, Line[waRightWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);

			if(FLeftTop){ // SoulBrain 교차점 찾는다 
				EasyImage::Threshold(&RotateImg,&RotateBinaryImg     , ECThresHold         ); //Corner 외곽라인 검출     
				EasyImage::Threshold(&RotateImg,&RotateGrindBinaryImg, CorGrindMeaThresHold); //Corner 3Point측정시 가공량 기준 라인 검출 

				if(ECThresHold==0)  {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
				else                {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

				OrgPoint = Math.OnIntersect(Line[waLeftWall].FP0, Line[waLeftWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
				FLTPoint = OrgPoint;

				// Mark에서 Top라인까지 거리측정 
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

				if(IsCornerTop){ //코너 연마량 설정되어 있을경우 코너 검사진행함(Dx,Dy,R,GW,GM 측정함)
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

					// ORG 교차점 
					FLTLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
					FLTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

					// 3Point 기준으로 가공량 측정 방법 적용 시퀀스(GrindMark 없을경우)
					if(AMarkToGrindMarkDvX==0.0f && AMarkToGrindMarkDvY==0.0f){ 
						if((FLTLPoint.x==0 || FLTLPoint.y==0 || FLTTPoint.x==0 || FLTTPoint.y==0) || (SingleLine->FEdgePointCount <30)){
							LTDx = 0.0f;
							LTDy = 0.0f;
							//화면에 디스플레이 위해서 
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
							// Corner Grind In Gray Level 편차로 인해서 Corner Grind Out/In ThresHold값을 분류하다 
							// Corner Grind In Start 
							if(CorGrindInThresHold!=0){ // Corner InLine Search Sequence 
								EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
								SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

								// Corner 기울기 완만하여 Edge찾을 경우 Defect 오버킬 발생함 
								// 방향을 Top방향에서 다시 엣지를 찾음 (Corner Defect OverKill 감소 목적)
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
								else { // Corner 연마 안쪽라인 찾는다 
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

								//코너 연마안쪽라인 기준 새로운 교차점 생성 
								FLTLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
								FLTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

								//C1 코너 연마폭(LTPS경우에만 연마폭 측정)
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

							if(CorGrindMeaThresHold!=0){ // Corner Grind ThresHold값이 설정되어 있을경우만 연마량 검사를 진행한다 
								// 코너 연마량 기준 라인을 찾는다 
								// ROI 영역 설정(3개의 흰색 점선 기준 라인을 찾는다)
								if(CorGrindMeaThresHold==0){SingleLine->GImage = &RotateImg           ;  SingleLine->FThreshold =5 ;}
								else                       {SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold =10;}

								// 존나 찝찝함....일단 스팩을 받기전까지 임시로 영역을 분류하여 CornerEdgePoint 찾자 
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


									//코너 연마량 계산  
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

									//코너 연마량 기준 라인과 새로운 교차점 생성 
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
									//코너 연마량 계산  
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
					else { // 십자가 마크 기준 연마량,가공량 측정 시퀀스(LeftTop)
						TFPoint C1GrindRectPoint(0.0f, 0.0f);
						if((FLTLPoint.x==0 || FLTLPoint.y==0 || FLTTPoint.x==0 || FLTTPoint.y==0) || (SingleLine->FEdgePointCount <30)){
							LTDx = 0.0f;
							LTDy = 0.0f;
							//화면에 디스플레이 위해서 
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

							// Corner Grind In Gray Level 편차로 인해서 Corner Grind Out/In ThresHold값을 분류하다 
							// Corner Grind In Start 
							if(CorGrindInThresHold!=0){ // Corner InLine Search Sequence 
								EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
								SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

								// Corner 기울기 완만하여 Edge찾을 경우 Defect 오버킬 발생함 
								// 방향을 Top방향에서 다시 엣지를 찾음 (Corner Defect OverKill 감소 목적)
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
									// Corner 연마 안쪽라인 찾는다 
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
								//코너 연마안쪽라인 기준 새로운 교차점 생성 
								FLTLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
								FLTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);
							}

							// Corner Chamber Cut GrindMark기준 연마량,가공량 측정함 
							// LeftTop 
							if(IsMarkBtm && FP0.x!=0.0f && FP0.y!=0.0f){
								AGrindMark[0].Rect.left   = (FP0.x-AMarkToGrindMarkDvX)-40;
								AGrindMark[0].Rect.top    = (FP0.y+AMarkToGrindMarkDvY)-40;
								AGrindMark[0].Rect.right  = (AGrindMark[0].Rect.left + 80);
								AGrindMark[0].Rect.bottom = (AGrindMark[0].Rect.top  + 80);

								GrindRectWidth  = (AGrindMark[0].Rect.right  - AGrindMark[0].Rect.left);
								GrindRectHeight = (AGrindMark[0].Rect.bottom - AGrindMark[0].Rect.top );

								// 원본이미지에서 Mark 찾는다 
								Find->OnLoad(true,FindDir,1)                   ;
								Find->GImage  = &RotateImg                      ;
								//Find->SetContrastMode(EFindContrastMode_Any)   ;
								Find->SetContrastMode(EFindContrastMode_Normal);
								//Find->SetMinScore(0.8f)                        ;
								Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind 검사 최소 Score
								Find->SetZeroScore(GrindPadMarkMinScore)       ; // 검사 후 최소 Score 
								//ind->SetMinScore (0.75)                       ; // PatternrFind 검사 최소 Score
								//ind->SetZeroScore(0.75)                       ; // 검사 후 최소 Score 
								Find->SetMaxInstances(1)                       ;
								Find->SetAngleTolerance(20.0f)                 ;
								Find->SetAngleBias(45.0f)                      ;
								Find->SetScaleBias(1.00f)                      ;
								Find->SetScaleTolerance(0.0f)                  ;
								Find->Property.Ox = (float)ImgW/2              ;
								Find->Property.Oy = (float)ImgH/2              ;

								Find->IRoi->SetPlacement(AGrindMark[0].Rect.left,AGrindMark[0].Rect.top,GrindRectWidth,GrindRectHeight);
								if(AGrindMark[0].Rect.left<=0 || AGrindMark[0].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

								// 검사 및 Match Pos 좌표 받어온다 
								// Find 검사 성공시 데이터 받아옴 
								if(Find->OnExecute()){
									AGrindMark[0].Point.x = Find->Result[0].Px;
									AGrindMark[0].Point.y = Find->Result[0].Py;
									AGrindMark[0].Score   = Find->Result[0].Score;
								}

								// Corner GrindMakr찾을경우 GrinaMark기준으로 연마,가공량 측정
								if(AGrindMark[0].Point.x!=0.0f && AGrindMark[0].Point.y!=0.0f){
									// GrindMark기준 OutData 최단거리 Count정보 Calc 
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
									// OutData기준 InData 최단거리 Count정보 Calc 
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

									// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정(OutData기준)
									// 가공량 산출 
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

									// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
								else { // GrindMrk 못찾을경우 연마량만 측정함 
									// OutData기준 최소값 데이터 Count산출 
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

									// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

									// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
				else if(IsRectTop){ // 코너 설정값이 없을경우 교차점 기준 검사 Rect검사  
					LTDx = 0.0f;
					LTDy = 0.0f;
					//화면에 디스플레이 위해서 
					FLTLPoint.x = FLTPoint.x;
					FLTLPoint.y = FLTPoint.y;
					FLTTPoint.x = FLTPoint.x;
					FLTTPoint.y = FLTPoint.y;
				}
				else if(IsCircleTop){ // TOP영역 LeftTop Circle 검사 (A1Circle 영영검사),
					// A_Side Pad영역에는 TrackLine존재하지 않음, 즉 GrindMark기준 가공량,연마량 측정만 가능함 
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
							// 원본이미지에서 Mark 찾는다 
							Find->OnLoad(true,FindDir,1)                   ;
							Find->GImage = &RotateImg                      ;
							// 										 Find->SetContrastMode(EFindContrastMode_Any)   ;
							Find->SetContrastMode(EFindContrastMode_Normal);
							// 										 Find->SetMinScore(0.8f)                        ;
							Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind 검사 최소 Score
							Find->SetZeroScore(GrindPadMarkMinScore)       ; // 검사 후 최소 Score 
							// 										 Find->SetMinScore (0.75)                       ; // PatternrFind 검사 최소 Score
							// 										 Find->SetZeroScore(0.75)                       ; // 검사 후 최소 Score 
							Find->SetMaxInstances(1)                       ;
							Find->SetAngleTolerance(20.0f)                 ;
							// 										 Find->SetAngleBias(45.0f)                      ;
							Find->SetScaleBias(1.00f)                      ;
							Find->SetScaleTolerance(0.0f)                  ;
							Find->Property.Ox = (float)ImgW/2              ;
							Find->Property.Oy = (float)ImgH/2              ;

							// 첫번째 GrindMark기준 센터 A1 Search 
							if(AMarkToGrindMarkDvX!=0.0f && AMarkToGrindMarkDvY!=0.0f){
								AGrindMark[0].Rect.left   = (FP0.x-AMarkToGrindMarkDvX)-40;
								AGrindMark[0].Rect.top    = (FP0.y+AMarkToGrindMarkDvY)-40;
								AGrindMark[0].Rect.right  = (AGrindMark[0].Rect.left + 80);
								AGrindMark[0].Rect.bottom = (AGrindMark[0].Rect.top  + 80);

								GrindRectWidth  = (AGrindMark[0].Rect.right  - AGrindMark[0].Rect.left);
								GrindRectHeight = (AGrindMark[0].Rect.bottom - AGrindMark[0].Rect.top );

								Find->IRoi->SetPlacement(AGrindMark[0].Rect.left,AGrindMark[0].Rect.top,GrindRectWidth,GrindRectHeight);
								if(AGrindMark[0].Rect.left<=0 || AGrindMark[0].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

								// 검사 및 Match Pos 좌표 받어온다 
								// Find 검사 성공시 데이터 받아옴 
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
					if(FInspectMode==Insp_Polygon){ // 비선형 곡선 알고리즘 
						TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);

						//P1.x = (int)(FLTPoint.x-20);    P1.y =(int)(FLTPoint.y-20            ); P2.x = (int)(P1.x+ASideCircleWidth+50);    P2.y = (int)(P1.y);
						//P3.x = (int)(FLTPoint.x-20);    P3.y =(int)(P1.y+ASideCircleHeight+50); P4.x = (int)(P2.x                    );    P4.y = (int)(P3.y);

						//P1.x = (int)(FLTPoint.x-20);    P1.y =(int)(FLTPoint.y-20            ); P2.x = (int)(P1.x+ASideCircleWidth+40);    P2.y = (int)(P1.y);
						//P3.x = (int)(FLTPoint.x-20);    P3.y =(int)(P1.y+ASideCircleHeight+40); P4.x = (int)(P2.x                    );    P4.y = (int)(P3.y);

						P1.x = (int)(FLTPoint.x-20);    P1.y =(int)(FLTPoint.y-20            ); P2.x = (int)(P1.x+ASideCircleWidth+20);    P2.y = (int)(P1.y);
						P3.x = (int)(FLTPoint.x-20);    P3.y =(int)(P1.y+ASideCircleHeight+20); P4.x = (int)(P2.x                    );    P4.y = (int)(P3.y);

						// 검사화면 Display 목적 
						PadPolyRect[0].left  = P1.x;   PadPolyRect[0].top    = P1.y;
						PadPolyRect[0].right = P2.x;   PadPolyRect[0].bottom = P3.y;

						int PixelCnt =0;

						// Median Filter 적용 
						if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
						if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
						FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
						FilterROI.Attach(&RotateBinaryImg)                                         ;
						FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y))     ;
						LogUnit.SetLog(L"LeftTop ==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
						LogUnit.SetLog(L"LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
						EasyImage::Median(&FilterROI, &FilterROI)                                  ;
						//  									 RotateBinaryImg.Save("D:\\ROTATE_IMG\\Polygon_LeftTop_필터.bmp");

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

						// InEdgeData/OutEdgeData 기준 가공량 및 연마량 계산
						// Polygon 알고리즘에서는 GrindMark기준 OutData,InData 최단거리 2개 Tag값 산출해야함 
						if(IsMarkBtm){ // LTPS 가공량,연마량 계산 
							if(A1CirOutEdgeCount!=0 && A1CirInEdgeCount!=0){ // MarkMode경우 가공량 검사를 먼저 진행 후 연마량 산출 
								// 25번째 GrindMark Search 및 연마,가공 데이터 계산
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								if(AGrindMark[0].Point.x!=0.0f && AGrindMark[0].Point.y!=0.0f){  //GrindMark 찾았을경우 
									// GrindMark기준 OutData 최단거리 Count정보 Calc 
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
// 									// GrindMark기준 InData 최단거리 Count정보 Calc 
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

									// CircleOutData 기준 InData 최단거리 Count정보 Calc 
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

									// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정(OutData기준)
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

									// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
								else if(AGrindMark[0].Rect.left!=0 && AGrindMark[0].Rect.top!=0 && AGrindMark[0].Rect.right!=0 && AGrindMark[0].Rect.bottom!=0){  //GrindMark 못 찾았을경우
									// OutData기준 최소값 데이터 Count산출 
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

									// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

									// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
						else         { //Polygon EnCap 계산시 OutPoint 중간값 기준 실측 거리 평균값을 계산한다(LeftTop) 
							if(A1CirInEdgeCount!=0 && A1CirOutEdgeCount!=0){
								GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
								GrindMarkOutCnt = (int)(A1CirOutEdgeCount/2);
								// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

								// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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

						// FAVION Circle Fitting 결과물(LeftTop)
						SampleSize = A1CirInEdgeCount*0.75; //0.5~0.8이하로 입력할것
						DistDevRatio = 0.15; //0.1~0.25 이하로 입력할것
						CircleFit->OnRansac(A1CirInPoint,A1CirInEdgeCount,SampleSize,DistDevRatio,&ACircleFit1Center,&ACirFitDiameter1);

						// Polygon 박리 검사 시퀀스(LeftTop) 
						if(!IsMarkBtm && BrokenThresHold!=0){
							RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
							EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBrokenImg);
							EasyImage::Threshold(&RotateImg,&RotateBrokenImg, BrokenThresHold);
							Edge->GImage = &RotateBrokenImg;
							Edge->SetEdgeType(0);
							// 										 RotateBrokenImg.Save("D:\\ROTATE_IMG\\Polygon_LeftTop_박리.bmp");

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
										Edge->OnExecute(A1CirOutPoint[k],Target); // 연마외곽라인에서 Target까지 White Point Search 
										if(Edge->GetResultEdgePointCount()>0){
											A1CircleData[DataCount].Dis =0.0f;
											A1CircleData[DataCount].Dv  =0.0f;
											A1CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
											A1CircleData[DataCount].P.y = A1CircleData[DataCount].P.y - ImageHeight; // Frame기준 단위좌표로 변경함  
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
										Edge->OnExecute(A1CirOutPoint[k],Target); // 연마외곽라인에서 Target까지 White Point Search 
										if(Edge->GetResultEdgePointCount()>0){
											A1CircleData[DataCount].Dis =0.0f;
											A1CircleData[DataCount].Dv  =0.0f;
											A1CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
											A1CircleData[DataCount].P.y = A1CircleData[DataCount].P.y - ImageHeight; // Frame기준 단위좌표로 변경함  
											DataCount++;
										}
									}
								}
								A1CirInCrackCount = DataCount;
							}
						}
					}
					else if(FInspectMode==Insp_Mark){ // LeftTop
						// 자재의 외곽라인 교차점 기준 CirCenter 설정함 
						if(ASideCircleWidth!=0.0f && ASideCircleHeight!=0.0f) A1CirCenter.x = FLTPoint.x + ASideCircleWidth;  A1CirCenter.y = FLTPoint.y + ASideCircleHeight;  
						// Circle Fitting 진행시 공통으로 사용하는 설정값
						Circle->SetSamplingStep(1)                              ; // A1Circle Edge 간격 
						//A1 In Circle EdgePoint Data 추출 
						if(A1CirCenter.x!=0 && A1CirCenter.y!=0){
							// A_Side Circle 검사전 Median 필터 적용 
							TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
							P1.x = (int)(FLTPoint.x-20);    P1.y =(int)(FLTPoint.y-20            ); P2.x = (int)(P1.x+ASideCircleWidth+40);    P2.y = (int)(P1.y);
							P3.x = (int)(FLTPoint.x-20);    P3.y =(int)(P1.y+ASideCircleHeight+40); P4.x = (int)(P2.x                    );    P4.y = (int)(P3.y);

							// Median Filter 적용 
							if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
							if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
							FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
							FilterROI.Attach(&RotateBinaryImg)                                    ;
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"Px : %d, Py : %d, Width : %d, Height : %d => MedianFilter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"P1.x : %d, P1.y : %d, P2.x : %d, P2.y : %d, P3.x : %d, P3.y : %d, P4.x : %d, P4.y : %d => MedianFilter", P1.x, P1.y, P2.x, P2.y, P3.x, P3.y, P4.x, P4.y);
							EasyImage::Median(&FilterROI, &FilterROI)                             ;

							if(ECThresHold==0) Circle->GImage = &RotateImg      ; //Org Image 넘겨줌 
							else               Circle->GImage = &RotateBinaryImg; //Binary Image 넘겨줌 

							Circle->SetCenterXY(A1CirCenter.x , A1CirCenter.y)  ; // Circle Center 좌표설정 
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

							// Circle In Data 기준 새로운 Circle Center 좌표 받아옴 
							// Euresys Circle Fitting Center & Diameter  
							RealA1CirCenter.x = Circle->FCP.x;
							RealA1CirCenter.y = Circle->FCP.y;
							ACirDiameter1     = (Circle->Diameter)/2.0;

							// Circle In Data취합 
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

							// Circle Out Data 다시 검사  
							//////////////////////////////////////////////////////////////////////////////////////////////////////
							Circle->SetTolerance(60.0f )      ; // Circle Tolerance 
							Circle->SetDiameter (A1Diameter)  ; // Circle Diameter 
							Circle->SetCircleChoice(1)        ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
							Circle->SetCircleType(2)          ; // 0:Black to White, 1:White to Black 2: Both Black, White 
							Circle->SetCircleIndex (0)        ; // Circle EdgePoint Index 

							Circle->OnExecute()               ;

							// Circle Out Data취합 
							OrgA1CirOutEdgeCount = Circle->FCirclePointCount;
							if(OrgA1CirOutEdgeCount>0){
								if(OrgA1CirOutPoint!=NULL) delete[] OrgA1CirOutPoint;
								OrgA1CirOutPoint = new TFPoint[OrgA1CirOutEdgeCount];

								for(int k=0; k<OrgA1CirOutEdgeCount; k++){
									if(k>MAX_CIRCLE_EDGEPOINT) break;
									OrgA1CirOutPoint[k].x = Circle->FCirclePoint[k].x;
									OrgA1CirOutPoint[k].y = Circle->FCirclePoint[k].y;
									// BTM영역 Circle을 제외한 영역 Defect검사 목적에 좌표계산 
									if(k==0                     ) {TopEdgePoint[0].x = OrgA1CirOutPoint[k].x; TopEdgePoint[0].y = OrgA1CirOutPoint[k].y;}
									if(k==OrgA1CirOutEdgeCount-1) {TopEdgePoint[1].x = OrgA1CirOutPoint[k].x; TopEdgePoint[1].y = OrgA1CirOutPoint[k].y;}
									if(OrgA1CirOutPoint[k].x==0.0f && OrgA1CirOutPoint[k].y==0.0f) A1CirOutEdgeCount++;
								}
							}

							// A1Circle Grind_Width Calculate (연마량 계산)
							if(IsMarkBtm){ // LTPS경우 가공량 계산 ==> 연마량 계산 
								if(OrgA1CirOutEdgeCount!=0 && OrgA1CirInEdgeCount!=0){ // MarkMode경우 가공량 검사를 먼저 진행 후 연마량 산출  
									GrindMeaDis =0.0f, MinDis =10000.0f; A1GrindMarkCount=0;
									if(AGrindMark[0].Point.x!=0.0f && AGrindMark[0].Point.y!=0.0f){  //GrindMark 찾았을경우 
										for(int k=0; k<OrgA1CirOutEdgeCount; k++){
											if(OrgA1CirOutPoint[k].x==0.0f || OrgA1CirOutPoint[k].y==0.0f) continue;
											if(AGrindMark[0].Point.x==0.0f || AGrindMark[0].Point.y==0.0f) continue;

											GrindMeaDis = Math.GetLength(AGrindMark[0].Point, OrgA1CirOutPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												A1GrindMarkCount = k;
											}
										}
										// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정 
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

										// GrindMark기준 수직 평균값 연마량 측정 
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
									else {  //GrindMark 못 찾았을경우
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
										// GrindMark기준 수직 평균값 연마량 측정 
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
							else     { // MarkMode ==> EnCap경우 연마량만 계산한다 
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

										// CircleFitting 진행시 실제 EdgePoint 데이터가 존재 할 경우만 포함하여 계산한다
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
							// Org Circle데이터중 유호하지 않은 값을 제외한 실제 존재하는 데이터 기준으로 동적 메모리 생성함
							// Circle Defect검사에 필요한 Edge데이터 값
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
								// Circle 원본 데이터 메모리 삭제
								if(OrgA1CirInPoint!=NULL) delete[] OrgA1CirInPoint;
							}
							// FAVION Circle Fitting 결과물 
							SampleSize = A1CirInEdgeCount*0.75; //0.5~0.8이하로 입력할것
							DistDevRatio = 0.15; //0.1~0.25 이하로 입력할것
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
								// Circle 원본 데이터 메모리 삭제
								if(OrgA1CirOutPoint!=NULL) delete[] OrgA1CirOutPoint;
							}
							// Circle 영역 꽃박리, 원형박리 검출 목적 시퀀스(EnCap)
							// 정식 명칭은 몰라서.....일단 Crack으로 정함 
							// SoulBrain LeftTop 박리 검사 
							if(!IsMarkBtm && BrokenThresHold!=0){
								RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
								EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBrokenImg);
								EasyImage::Threshold(&RotateImg,&RotateBrokenImg, BrokenThresHold);
								// 										 RotateBrokenImg.Save("D:\\ROTATE_IMG\\RotateBrokenImg_HAP.bmp");

								if(ACirFitDiameter1!=0.0f && ACircleFit1Center.x!=0.0f && ACircleFit1Center.y!=0.0f){ // FAVION Circle Fitting 성공시 박리검사 진행 
									Circle->GImage = &RotateBrokenImg;                                             //Binary Image 넘겨줌 

									Circle->SetCenterXY(ACircleFit1Center.x,ACircleFit1Center.y);
									// 											 Circle->SetTolerance(CirCrackMargin)                      ; // Circle Tolerance 
									// 											 Circle->SetDiameter ((ACirFitDiameter1-CirCrackMargin)*2)  ; // Circle Diameter 
									Circle->SetTolerance((dEdgeToTrapLineDis/2)-CirOffsetMargin)      ; // Circle Tolerance 
									Circle->SetDiameter ((ACirFitDiameter1-(dEdgeToTrapLineDis/2))*2) ; // Circle Diameter 
									Circle->SetCircleChoice(0)                                ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
									Circle->SetCircleType  (0)                                ; // 0:Black to White, 1:White to Black 2: Both Black, White 
									Circle->SetCircleIndex (0)                                ; // Circle EdgePoint Index 
									Circle->SetAmplitude(25)                                  ; // 패턴으로 인해서 오버킬 발생...범위를 좁혀준다 
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
				EasyImage::Threshold(&RotateImg,&RotateBinaryImg     , ECThresHold         ); // Corner 외곽라인 검출 
				EasyImage::Threshold(&RotateImg,&RotateGrindBinaryImg, CorGrindMeaThresHold); // Corner 3Point측정시 가공량 기준 라인 검출 
				//							 RotateBinaryImg.Save("D:\\ROTATE_IMG\\RotateBinaryImg회전_HAP.bmp");

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

					if(AMarkToGrindMarkDvX==0.0f && AMarkToGrindMarkDvY==0.0f){ // 3Point 기준으로 가공량 측정 방법 적용 시퀀스 
						if((FRTRPoint.x==0 || FRTRPoint.y==0 || FRTTPoint.x==0 || FRTTPoint.y==0) || (SingleLine->FEdgePointCount <30)){
							RTDx = 0.0f;
							RTDy = 0.0f;
							//화면에 디스플레이 위해서 
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

								// Corner 기울기 완만하여 Edge찾을 경우 Defect 오버킬 발생함 
								// 방향을 Top방향에서 다시 엣지를 찾음 (Corner Defect OverKill 감소 목적)
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
									// Corner 연마 안쪽라인 찾는다 
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

								//코너 연마안쪽라인 기준 새로운 교차점 생성 
								FRTRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
								FRTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall  ].FP0, Line[waTopWall].FP1  );

								//코너 연마폭, 연마량 계산 
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

							if(CorGrindMeaThresHold!=0){ // Corner Grind ThresHold값이 설정되어 있을경우만 연마량 검사를 진행한다 
								// 코너 연마량 기준 라인을 찾는다 
								// ROI 영역 설정 
								if(CorGrindMeaThresHold==0){SingleLine->GImage = &RotateImg           ;  SingleLine->FThreshold =5 ;}
								else                       {SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold =10;}

								// 존나 찝찝함....일단 스팩을 받기전까지 임시로 영역을 분류하여 CornerEdgePoint 찾자 
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
									//코너 연마안쪽라인 기준 새로운 교차점 생성 
									FRTRMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
									FRTTMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall  ].FP0, Line[waTopWall].FP1  );

									//코너 연마량 계산  
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

									//코너 연마안쪽라인 기준 새로운 교차점 생성 
									FRTRMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
									FRTTMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall  ].FP0, Line[waTopWall].FP1  );

									//코너 연마량 계산  
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
					else { //십자가 마크 기준 연마량,가공량 측정 시퀀스(RightTop)
						TFPoint C2GrindRectPoint(0.0f, 0.0f);
						if((FRTRPoint.x==0 || FRTRPoint.y==0 || FRTTPoint.x==0 || FRTTPoint.y==0) || (SingleLine->FEdgePointCount <30)){
							RTDx = 0.0f;
							RTDy = 0.0f;
							//화면에 디스플레이 위해서 
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

								// Corner 기울기 완만하여 Edge찾을 경우 Defect 오버킬 발생함 
								// 방향을 Top방향에서 다시 엣지를 찾음 (Corner Defect OverKill 감소 목적)
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
									// Corner 연마 안쪽라인 찾는다 
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

								//코너 연마안쪽라인 기준 새로운 교차점 생성 
								FRTRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
								FRTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall  ].FP0, Line[waTopWall].FP1  );
							}

							// GrindMark기준 연마량,가공량 측정함 
							// RightTop 
							if(IsMarkBtm && FP1.x!=0.0f && FP1.y!=0.0f){
								AGrindMark[1].Rect.left   = (FP1.x+AMarkToGrindMarkDvX)-40;
								AGrindMark[1].Rect.top    = (FP1.y+AMarkToGrindMarkDvY)-40;
								AGrindMark[1].Rect.right  = (AGrindMark[1].Rect.left + 80);
								AGrindMark[1].Rect.bottom = (AGrindMark[1].Rect.top  + 80);

								GrindRectWidth  = (AGrindMark[1].Rect.right  - AGrindMark[1].Rect.left);
								GrindRectHeight = (AGrindMark[1].Rect.bottom - AGrindMark[1].Rect.top );

								// 원본이미지에서 Mark 찾는다 
								Find->OnLoad(true,FindDir,1)                   ;
								Find->GImage = &RotateImg                      ;
								//Find->SetContrastMode(EFindContrastMode_Any)   ;
								Find->SetContrastMode(EFindContrastMode_Normal);
								//Find->SetMinScore(0.8f)                        ;
								Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind 검사 최소 Score
								Find->SetZeroScore(GrindPadMarkMinScore)       ; // 검사 후 최소 Score 
								//ind->SetMinScore (0.75)                       ; // PatternrFind 검사 최소 Score
								//ind->SetZeroScore(0.75)                       ; // 검사 후 최소 Score 
								Find->SetMaxInstances(1)                       ;
								Find->SetAngleTolerance(20.0f)                 ;
								Find->SetAngleBias(135.0f)                     ;
								Find->SetScaleBias(1.00f)                      ;
								Find->SetScaleTolerance(0.0f)                  ;
								Find->Property.Ox = (float)ImgW/2              ;
								Find->Property.Oy = (float)ImgH/2              ;

								Find->IRoi->SetPlacement(AGrindMark[1].Rect.left,AGrindMark[1].Rect.top,GrindRectWidth,GrindRectHeight);
								if(AGrindMark[1].Rect.left<=0 || AGrindMark[1].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

								// 검사 및 Match Pos 좌표 받어온다 
								// Find 검사 성공시 데이터 받아옴 
								if(Find->OnExecute()){
									AGrindMark[1].Point.x = Find->Result[0].Px;
									AGrindMark[1].Point.y = Find->Result[0].Py;
									AGrindMark[1].Score   = Find->Result[0].Score;
								}
								// Contrast Gradient Pixel Value
// 								GrindMarkGradient[20] = EasyImage::Focusing(Find->IRoi);
							}

							// GrindMakr찾을경우 연마량,가공량 측정함
							if(AGrindMark[1].Point.x!=0.0f && AGrindMark[1].Point.y!=0.0f){
								// GrindMark기준 OutData 최단거리 Count정보 Calc 
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
								// OutData기준 InData 최단거리 Count정보 Calc 
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

								// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정(OutData기준)
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

								// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
							else { // GrindMrk 못찾을경우 연마량만 측정함 
								// OutData기준 최소값 데이터 Count산출 
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

								// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

								// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
					//화면에 디스플레이 위해서 
					FRTRPoint.x = FRTPoint.x;
					FRTRPoint.y = FRTPoint.y;
					FRTTPoint.x = FRTPoint.x;
					FRTTPoint.y = FRTPoint.y;
				}
				else if(IsCircleTop){ // TOP영역 RightTop Circle 검사  A2Circle 
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
							Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind 검사 최소 Score
							Find->SetZeroScore(GrindPadMarkMinScore)       ; // 검사 후 최소 Score 
							// 										 Find->SetMinScore (0.75)                       ; // PatternrFind 검사 최소 Score
							// 										 Find->SetZeroScore(0.75)                       ; // 검사 후 최소 Score 
							Find->SetMaxInstances(1)                       ;
							Find->SetAngleTolerance(20.0f)                 ;
							// 										 Find->SetAngleBias(135.0f)                     ;
							Find->SetScaleBias(1.00f)                      ;
							Find->SetScaleTolerance(0.0f)                  ;
							Find->Property.Ox = (float)ImgW/2              ;
							Find->Property.Oy = (float)ImgH/2              ;

							// 첫번째 GrindMark기준 센터 A2 Search
							if(AMarkToGrindMarkDvX!=0.0f && AMarkToGrindMarkDvX!=0.0f){
								AGrindMark[1].Rect.left   = (FP1.x+AMarkToGrindMarkDvX)-40;
								AGrindMark[1].Rect.top    = (FP1.y+AMarkToGrindMarkDvY)-40;
								AGrindMark[1].Rect.right  = (AGrindMark[1].Rect.left + 80);
								AGrindMark[1].Rect.bottom = (AGrindMark[1].Rect.top  + 80);

								GrindRectWidth  = (AGrindMark[1].Rect.right  - AGrindMark[1].Rect.left);
								GrindRectHeight = (AGrindMark[1].Rect.bottom - AGrindMark[1].Rect.top );

								Find->IRoi->SetPlacement(AGrindMark[1].Rect.left,AGrindMark[1].Rect.top,GrindRectWidth,GrindRectHeight);
								if(AGrindMark[1].Rect.left<=0 ||AGrindMark[1].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

								// 검사 및 Match Pos 좌표 받어온다 
								// Find 검사 성공시 데이터 받아옴 

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

						// 검사화면 Display 목적 
						PadPolyRect[1].left  = P1.x;   PadPolyRect[1].top    = P1.y;
						PadPolyRect[1].right = P2.x;   PadPolyRect[1].bottom = P3.y;

						int PixelCnt =0;

						// Median Filter 적용 
						if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
						if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
						FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
						FilterROI.Attach(&RotateBinaryImg)                                    ;
						FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
						LogUnit.SetLog(L"RightTop ==>Px :%d, Py :%d, Width:%d, Height:%d ==>Median Filter",P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
						LogUnit.SetLog(L"RightTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
						EasyImage::Median(&FilterROI, &FilterROI)                             ;
						// 									 RotateBinaryImg.Save("D:\\ROTATE_IMG\\Polygon_RightTop_필터.bmp");

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


						// InEdgeData/OutEdgeData 기준 가공량 및 연마량 계산
						if(IsMarkBtm){ // LTPS 가공량,연마량 계산 
							if(A2CirOutEdgeCount!=0 && A2CirInEdgeCount!=0){ // MarkMode경우 가공량 검사를 먼저 진행 후 연마량 산출
								////////////////////////////////////////////////////////////////////////////////////////////////////
								if(AGrindMark[1].Point.x!=0.0f && AGrindMark[1].Point.y!=0.0f){  //GrindMark 찾았을경우 
									// GrindMark기준 OutData 최단거리 Count정보 Calc 
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
									// GrindMark기준 InData 최단거리 Count정보 Calc 
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
										// GrindMark기준 InData 최단거리 Count정보 Calc 
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

									// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정(OutData기준)
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

									// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
								else if(AGrindMark[1].Rect.left!=0 && AGrindMark[1].Rect.top!=0 && AGrindMark[1].Rect.right!=0 && AGrindMark[1].Rect.bottom!=0){  //GrindMark 못 찾았을경우
									// OutData기준 최소값 데이터 Count산출 
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

									// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

									// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
						else         { //Polygon EnCap 계산시 OutPoint 중간값 기준 실측 거리 평균값을 계산한다(RightTop)
							if(A2CirInEdgeCount!=0 &&A2CirOutEdgeCount!=0){
								GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
								GrindMarkOutCnt = (int)(A2CirOutEdgeCount/2);
								// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

								// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
						// FAVION Circle Fitting 결과물(RightTop)
						SampleSize = A2CirInEdgeCount*0.75; //0.5~0.8이하로 입력할것
						DistDevRatio = 0.15; //0.1~0.25 이하로 입력할것
						CircleFit->OnRansac(A2CirInPoint,A2CirInEdgeCount,SampleSize,DistDevRatio,&ACircleFit2Center,&ACirFitDiameter2);

						// Polygon 박리 검사 시퀀스(RightTop) 
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
										Edge->OnExecute(A2CirOutPoint[k],Target); // 연마외곽라인에서 Target까지 White Point Search 
										if(Edge->GetResultEdgePointCount()>0){
											A2CircleData[DataCount].Dis =0.0f;
											A2CircleData[DataCount].Dv  =0.0f;
											A2CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
											A2CircleData[DataCount].P.y = A2CircleData[DataCount].P.y - ImageHeight; // Frame기준 단위좌표로 변경함  
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
										Edge->OnExecute(A2CirOutPoint[k],Target); // 연마외곽라인에서 Target까지 White Point Search 
										if(Edge->GetResultEdgePointCount()>0){
											A2CircleData[DataCount].Dis =0.0f;
											A2CircleData[DataCount].Dv  =0.0f;
											A2CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
											A2CircleData[DataCount].P.y = A2CircleData[DataCount].P.y - ImageHeight; // Frame기준 단위좌표로 변경함  
											DataCount++;
										}
									}
								}
								A2CirInCrackCount = DataCount;
							}
						}
					}
					else if(FInspectMode==Insp_Mark){ // RightTop
						// 자재의 외곽라인 교차점 기준 CirCenter 설정함 
						if(ASideCircleWidth!=0.0f && ASideCircleHeight!=0.0f)  A2CirCenter.x = FRTPoint.x - ASideCircleWidth;  A2CirCenter.y = FRTPoint.y + ASideCircleHeight;  

						// Circle Fitting 진행시 공통으로 사용하는 설정값
						Circle->SetSamplingStep(1)                              ; // A2Circle Edge 간격 

						//A1 In Circle EdgePoint Data 추출 
						if(A2CirCenter.x!=0 && A2CirCenter.y!=0){
							TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
							P1.x = (int)(FRTPoint.x-ASideCircleWidth   );    P1.y =(int)(FRTPoint.y-20            );    P2.x = (int)(FRTPoint.x+20);    P2.y = (int)(P1.y);
							P3.x = (int)(P1.x)                          ;    P3.y =(int)(P1.y+ASideCircleHeight+20);    P4.x = (int)(P2.x         );    P4.y = (int)(P3.y);
							// Median Filter 적용 
							if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
							if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
							FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
							FilterROI.Attach(&RotateBinaryImg)                                    ;
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"Px : %d, Py : %d, Width : %d, Height : %d => MedianFilter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"P1.x : %d, P1.y : %d, P2.x : %d, P2.y : %d, P3.x : %d, P3.y : %d, P4.x : %d, P4.y : %d => MedianFilter", P1.x, P1.y, P2.x, P2.y, P3.x, P3.y, P4.x, P4.y);
							EasyImage::Median(&FilterROI, &FilterROI)                             ;
							// 										 RotateBinaryImg.Save("D:\\ROTATE_IMG\\RightTop_필터.bmp");


							if(ECThresHold==0) Circle->GImage = &RotateImg      ; //Org Image 넘겨줌 
							else               Circle->GImage = &RotateBinaryImg; //Binary Image 넘겨줌 

							Circle->SetCenterXY(A2CirCenter.x , A2CirCenter.y)  ; // Circle Center 좌표설정 
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

							// Circle In Data 기준 새로운 Circle Center 좌표 받아옴 
							// Euresys Circle Fitting Center 
							RealA2CirCenter.x = Circle->FCP.x;
							RealA2CirCenter.y = Circle->FCP.y;
							ACirDiameter2     = (Circle->Diameter)/2.0;

							// Circle In Data취합 
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

							// Circle Out Data 다시 검사  
							//////////////////////////////////////////////////////////////////////////////////////////////////////
							Circle->SetTolerance(60.0f )      ; // Circle Tolerance 
							Circle->SetDiameter (A2Diameter)  ; // Circle Diameter 
							Circle->SetCircleChoice(1)        ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
							Circle->SetCircleType(2)          ; // 0:Black to White, 1:White to Black 2: Both Black, White 
							Circle->SetCircleIndex (0)        ; // Circle EdgePoint Index 

							Circle->OnExecute()               ;

							// Circle Out Data취합 
							OrgA2CirOutEdgeCount = Circle->FCirclePointCount;
							if(OrgA2CirOutEdgeCount>0){
								if(OrgA2CirOutPoint!=NULL) delete[] OrgA2CirOutPoint;
								OrgA2CirOutPoint = new TFPoint[OrgA2CirOutEdgeCount];

								for(int k=0; k<OrgA2CirOutEdgeCount; k++){
									if(k>MAX_CIRCLE_EDGEPOINT) break;
									OrgA2CirOutPoint[k].x = Circle->FCirclePoint[k].x;
									OrgA2CirOutPoint[k].y = Circle->FCirclePoint[k].y;
									// BTM영역 Circle을 제외한 영역 Defect검사 목적에 좌표계산 
									if(k==0                     ) {TopEdgePoint[2].x = OrgA2CirOutPoint[k].x; TopEdgePoint[2].y = OrgA2CirOutPoint[k].y;}
									if(k==OrgA2CirOutEdgeCount-1) {TopEdgePoint[3].x = OrgA2CirOutPoint[k].x; TopEdgePoint[3].y = OrgA2CirOutPoint[k].y;}
									if(OrgA2CirOutPoint[k].x==0.0f && OrgA2CirOutPoint[k].y==0.0f) A2CirOutEdgeCount++;
								}
							}
							// A2Circle Grind_Width Calculate (연마량 계산)
							if(IsMarkBtm){ // LTPS경우 가공량 계산 ==> 연마량 계산 
								if(OrgA2CirOutEdgeCount!=0 && OrgA2CirInEdgeCount!=0){// MarkMode경우 가공량 검사를 먼저 진행 후 연마량 산출  
									GrindMeaDis =0.0f, MinDis =10000.0f; A2GrindMarkCount=0;
									if(AGrindMark[1].Point.x!=0.0f && AGrindMark[1].Point.y!=0.0f){  // GrindMark 찾았을경우 
										for(int k=0; k<OrgA2CirOutEdgeCount; k++){
											if(OrgA2CirOutPoint[k].x ==0.0f || OrgA2CirOutPoint[k].y ==0.0f) continue;
											if(AGrindMark[1].Point.x ==0.0f || AGrindMark[1].Point.y ==0.0f) continue;

											GrindMeaDis = Math.GetLength(AGrindMark[1].Point, OrgA2CirOutPoint[k]);
											if(GrindMeaDis<MinDis){
												MinDis = GrindMeaDis;
												A2GrindMarkCount = k;
											}
										}

										// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정 
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

										// GrindMark기준 수직 평균값 연마량 측정 
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
									else { // GrindMark 못 찾았을 경우 
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
										// GrindMark기준 수직 평균값 연마량 측정 
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
							else     { // MarkMode ==> EnCap경우 연마량만 계산한다 
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

										// CircleFitting 진행시 실제 EdgePoint 데이터가 존재 할 경우만 포함하여 계산한다
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
							// Org Circle데이터중 유호하지 않은 값을 제외한 실제 존재하는 데이터 기준으로 동적 메모리 생성함
							// Circle Defect검사에 필요한 Edge데이터 값
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
								// Circle 원본 데이터 메모리 삭제
								if(OrgA2CirInPoint!=NULL) delete[] OrgA2CirInPoint;
							}
							// FAVION Circle Fitting 결과물 
							SampleSize = A2CirInEdgeCount*0.75; //0.5~0.8이하로 입력할것
							DistDevRatio = 0.15; //0.1~0.25 이하로 입력할것
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
								// Circle 원본 데이터 메모리 삭제
								if(OrgA2CirOutPoint!=NULL) delete[] OrgA2CirOutPoint;
							}
							// Circle 영역 꽃박리, 원형박리 검출 목적 시퀀스(EnCap)
							// 정식 명칭은 몰라서.....일단 Crack으로 정함 
							if(!IsMarkBtm && BrokenThresHold!=0){
								if(ACirFitDiameter2!=0.0f && ACircleFit2Center.x!=0.0f && ACircleFit2Center.y!=0.0f){ // FAVION Circle Fitting 성공시 박리검사 진행 
									Circle->GImage = &RotateBrokenImg;                                             //Binary Image 넘겨줌 

									Circle->SetCenterXY(ACircleFit2Center.x,ACircleFit2Center.y);
									// 											 Circle->SetTolerance(CirCrackMargin)                      ; // Circle Tolerance 
									// 											 Circle->SetDiameter ((ACirFitDiameter2-CirCrackMargin)*2) ; // Circle Diameter 
									Circle->SetTolerance((dEdgeToTrapLineDis/2)-CirOffsetMargin); // Circle Tolerance 
									Circle->SetDiameter ((ACirFitDiameter2-(dEdgeToTrapLineDis/2))*2) ; // Circle Diameter 
									Circle->SetCircleChoice(0)                                ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
									Circle->SetCircleType  (0)                                ; // 0:Black to White, 1:White to Black 2: Both Black, White 
									Circle->SetCircleIndex (0)                                ; // Circle EdgePoint Index 
									Circle->SetAngle(325)                                     ; // 패턴으로 인해서 오버킬 발생...범위를 좁혀준다 
									Circle->SetAmplitude(25)                                  ; // 패턴으로 인해서 오버킬 발생...범위를 좁혀준다 
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
			// Circle 검사에 필요한 로컬변수 초기화 
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

			// Notch 직선영영 데이터 취합에 필요함 
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

			// Image Roate 전 원본이미지 기준 교차점 산출 
			FLBPoint = Math.OnIntersect(Line[0].FP0 , Line[0].FP1 , Line[3].FP0, Line[3].FP1);
			FRBPoint = Math.OnIntersect(Line[2].FP0 , Line[2].FP1 , Line[3].FP0, Line[3].FP1);

			if(FLBPoint.x!=0 && FLBPoint.y!=0 && FRBPoint.x!=0 && FRBPoint.y!=0) ImgDegree = -(Math.GetDegree(FLBPoint.x , FLBPoint.y, FRBPoint.x, FRBPoint.y));

			// Left_Mark와 Right_Mark각도가 정확히 0도인경우 발생하여 조건을 바꿈
			// 						 if(ImgDegree!=0.0f){ // 원본이미지 교차점 기준 Align틀어졌을경우 이미지 회전 
			if(FLBPoint.x!=0 && FLBPoint.y!=0 && FRBPoint.x!=0 && FRBPoint.y!=0){ // 원본이미지 교차점 기준 Align틀어졌을경우 이미지 회전 
				// 원본 RotateImg Size 설정 및 복사 
				LogUnit.SetLog(L"shRectC_RotateImg_Start");     
				RotateImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&RotateImg);

				// 원본 복사하여 이미지 회전 
				EasyImage::ScaleRotate(GImage, (float)ImgW/2, (float)ImgH/2, (float)ImgW/2, (float)ImgH/2, 1.0f, 1.0f, ImgDegree, &RotateImg, 4);
				CenterPoint.x = ImgW/2;
				CenterPoint.y = ImgH/2;
// 				RotateImg.Save("D:\\ROTATE_IMG\\SoulBrain_BTM.bmp");

				// Binary RotateImg Size 설정 및 복사 
				RotateBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBinaryImg);
				// 							 RotateBinaryImg.Save("D:\\ROTATE_IMG\\RotateImg_CUP_원본.bmp");

				// Circle Grind In EdgePoint 검출목적 이미지 
				RotateGrindInBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateGrindInBinaryImg);

				// Grind Measure Corner, Circle 검사 목적 
				RotateGrindBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateGrindBinaryImg);
				//							 RotateImg.Save("D:\\ROTATE_IMG\\RotateImg회전_CUP.bmp");
				LogUnit.SetLog(L"shRectC_RotateImg_End");    

				if(FLeftWall){ // shRectC Image Rotate Sequence  
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftEdgeThresHold);
					if(LeftEdgeThresHold==0) {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
					else                     {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()          );
					SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()          );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth()         );

					// ChamberCut VS Circle VS Rect경우에 따라서 Left영역 Fitting구간 높이가 달라짐 
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

					// Btm,Top 상관없이 첫번째
					LeftOutLine[0].FP0.x = Line[0].FP0.x;
					LeftOutLine[0].FP0.y = Line[0].FP0.y;
					LeftOutLine[0].FP1.x = Line[0].FP1.x;
					LeftOutLine[0].FP1.y = Line[0].FP1.y;

					if(fabs(LeftOutLine[0].FP0.x - LeftOutLine[0].FP1.x)>=0.5){
						LeftOutLine[0].FP0.x = (LeftOutLine[0].FP0.x+LeftOutLine[0].FP1.x)/2.0;
						LeftOutLine[0].FP1.x = (LeftOutLine[0].FP0.x+LeftOutLine[0].FP1.x)/2.0;
					}
					// LeftOutLine....make OutLine Data 
					// 현재의 데이터를 넘겨준다 
					// 검사중 기준라인을 찾지 못할경우 이전데이터 넘겨줘서 기준라인 생성함 
					if(LeftOutLine[0].FP0.x!=0.0f && LeftOutLine[0].FP0.y!=0.0f && LeftOutLine[0].FP1.x!=0.0f && LeftOutLine[0].FP1.y!=0.0f){
						PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
						PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
					} // 셀의 상단 시작부터 Broken 발생으로 데이터가 없는경우 
					else if(LeftOutLine[0].FP0.x==0.0f || LeftOutLine[0].FP0.y==0.0f || LeftOutLine[0].FP1.x==0.0f || LeftOutLine[0].FP1.y==0.0f){
						if(PreLeftOutLine[0].FP0.x==0.0f && PreLeftOutLine[0].FP0.y==0.0f && PreLeftOutLine[0].FP1.x==0.0f && PreLeftOutLine[0].FP1.y==0.0f){// 시작부분 Broken발생으로 데이터가 없는경우 
							// Mark좌표를 기준으로 라인좌표값을 다시 설정함 
							ROI_H = IRoi->GetHeight();
							LeftOutLine[0].FP0.x = Line[0].FP0.x;    LeftOutLine[0].FP0.y = 0.0f        ;
							LeftOutLine[0].FP1.x = Line[0].FP1.x;    LeftOutLine[0].FP1.y = (float)ROI_H;

							//Line 데이터를 PreLine에 넘겨준다 
							PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;       
							PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;       
						}
						else { // 중간부분 Broken발생으로 데이터가 없는경우 
							// PreLine 데이터를 넘겨줌 
							LeftOutLine[0].FP0.x = PreLeftOutLine[0].FP0.x;  LeftOutLine[0].FP1.x = PreLeftOutLine[0].FP1.x;  
							LeftOutLine[0].FP0.y = PreLeftOutLine[0].FP0.y;  LeftOutLine[0].FP1.y = PreLeftOutLine[0].FP1.y;  
						}
					}

					// LeftInLine 검출
					// LeftGrindIn 라인 검출 
					if(LeftGrindThresHold!=0){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftGrindThresHold);
						SingleLine->GImage = &RotateBinaryImg;
						LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;
						// ROI영역 기준점  계산  
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
						// 영역 설정 
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

						// 연마량 계산 
						GrindWd1=0.0f,GrindWd2=0.0f;
						GrindWd1 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP0);
						GrindWd2 = Math.GetLengthFrLineToPoint(LeftInLine[0].FP0,LeftInLine[0].FP1,LeftOutLine[0].FP1);
						LeftGrindWidth = (GrindWd1+GrindWd2)/2.0;
					}

					// 가공량 측정 기준라인 검출 시퀀스 
					if(LeftStdThresHold!=0 && LeftInLine[0].FP0.x!=0 && LeftInLine[0].FP0.y!=0 && LeftInLine[0].FP1.x!=0 && LeftInLine[0].FP1.y!=0){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, LeftStdThresHold);
						SingleLine->GImage = &RotateBinaryImg;
						LSP.x =0.0f, LSP.y =0.0f, LEP.x =0.0f, LEP.y =0.0f;

						// 영역기준 계산  
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
						// 영역 설정 
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

						//가공량 계산 
						GrindMea1=0.0f,GrindMea2=0.0f;
						GrindMea1 = Math.GetLengthFrLineToPoint(LeftStdLine[0].FP0,LeftStdLine[0].FP1,LeftOutLine[0].FP0);
						GrindMea2 = Math.GetLengthFrLineToPoint(LeftStdLine[0].FP0,LeftStdLine[0].FP1,LeftOutLine[0].FP1);
						LeftGrindMea = (GrindMea1+GrindMea2)/2.0;
					}
				}
				if(FRightWall){ // SoulBrain Image Rotate 적용 
					EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightEdgeThresHold);
// 					RotateBinaryImg.Save("D:\\ROTATE_IMG\\SoulBrain_BTM.bmp");
					if(RightEdgeThresHold==0) {SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold =5 ;}
					else                      {SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;}

					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()           );
					SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()           );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth()          );
					// ChamberCut VS Circle VS Rect경우에 따라서 Left영역 Fitting구간 높이가 달라짐 
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

					// Btm,Top 상관없이 첫번째
					RightOutLine[0].FP0.x = Line[2].FP0.x;
					RightOutLine[0].FP0.y = Line[2].FP0.y;
					RightOutLine[0].FP1.x = Line[2].FP1.x;
					RightOutLine[0].FP1.y = Line[2].FP1.y;

					if(fabs(RightOutLine[0].FP0.x - RightOutLine[0].FP1.x)>=0.5){
						RightOutLine[0].FP0.x = (RightOutLine[0].FP0.x+RightOutLine[0].FP1.x)/2.0;
						RightOutLine[0].FP1.x = (RightOutLine[0].FP0.x+RightOutLine[0].FP1.x)/2.0;
					}

					// LeftOutLine....make OutLine Data 
					// 현재의 데이터를 넘겨준다 
					// 검사중 기준라인을 찾지 못할경우 이전데이터 넘겨줘서 기준라인 생성함 
					if(RightOutLine[0].FP0.x!=0.0f && RightOutLine[0].FP0.y!=0.0f && RightOutLine[0].FP1.x!=0.0f && RightOutLine[0].FP1.y!=0.0f){
						PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
						PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
					} // 셀의 상단 시작부터 Broken 발생으로 데이터가 없는경우 
					else if(RightOutLine[0].FP0.x==0.0f || RightOutLine[0].FP0.y==0.0f || RightOutLine[0].FP1.x==0.0f || RightOutLine[0].FP1.y==0.0f){
						if(PreRightOutLine[0].FP0.x==0.0f && PreRightOutLine[0].FP0.y==0.0f && PreRightOutLine[0].FP1.x==0.0f && PreRightOutLine[0].FP1.y==0.0f){// 시작부분 Broken발생으로 데이터가 없는경우 
							// Mark좌표를 기준으로 라인좌표값을 다시 설정함 
							ROI_H = IRoi->GetHeight();
							RightOutLine[0].FP0.x = Line[2].FP0.x;    RightOutLine[0].FP0.y = 0.0f        ;
							RightOutLine[0].FP1.x = Line[2].FP1.x;    RightOutLine[0].FP1.y = (float)ROI_H;

							//Line 데이터를 PreLine에 넘겨준다 
							PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;       
							PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;       
						}
						else { // 중간부분 Broken발생으로 데이터가 없는경우 
							// PreLine 데이터를 넘겨줌 
							RightOutLine[0].FP0.x = PreRightOutLine[0].FP0.x;  RightOutLine[0].FP1.x = PreRightOutLine[0].FP1.x;  
							RightOutLine[0].FP0.y = PreRightOutLine[0].FP0.y;  RightOutLine[0].FP1.y = PreRightOutLine[0].FP1.y;  
						}
					}

					// RightGrindIn라인 검출 
					if(RightGrindThresHold!=0){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightGrindThresHold);
						SingleLine->GImage = &RotateBinaryImg;
						RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;
						// ROI영역 기준점  계산  
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
						// 영역 설정 
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

						// 연마량 계산 
						GrindWd1=0.0f,GrindWd2=0.0f;
						GrindWd1 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP0);
						GrindWd2 = Math.GetLengthFrLineToPoint(RightInLine[0].FP0,RightInLine[0].FP1,RightOutLine[0].FP1);
						RightGrindWidth = (GrindWd1+GrindWd2)/2.0;
					}

					// 가공량 측정 기준라인 검출 시퀀스 
					if(RightStdThresHold!=0 && RightInLine[0].FP0.x!=0 && RightInLine[0].FP0.y!=0 && RightInLine[0].FP1.x!=0 && RightInLine[0].FP1.y!=0){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, RightStdThresHold);
						SingleLine->GImage = &RotateBinaryImg;
						RSP.x =0.0f, RSP.y =0.0f, REP.x =0.0f, REP.y =0.0f;

						// 영역기준 계산  
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
						// 영역 설정 
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

						//가공량 계산 
						GrindMea1=0.0f,GrindMea2=0.0f;
						GrindMea1 = Math.GetLengthFrLineToPoint(RightStdLine[0].FP0,RightStdLine[0].FP1,RightOutLine[0].FP0);
						GrindMea2 = Math.GetLengthFrLineToPoint(RightStdLine[0].FP0,RightStdLine[0].FP1,RightOutLine[0].FP1);
						RightGrindMea = (GrindMea1+GrindMea2)/2.0;
					}
				}
				if(FBottomWall){ //SoulBrain 이미지 회전 후 교차점만 정확이 찾는다 
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

					// Image Roate 후 기준 데이터 다시 산출 
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

						// 이미지 회전 후 데이터기준 교차점 산출 
						FLBLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
						FLBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);
						BtmEdgePoint[0].x = FLBBPoint.x;
						BtmEdgePoint[0].y = FLBBPoint.y;

						if(CMarkToGrindMarkDvX==0.0f && CMarkToGrindMarkDvY==0.0f){ // Corner Chamber Cut 3Point 
							if((FLBLPoint.x==0 || FLBLPoint.y==0 || FLBBPoint.x==0 || FLBBPoint.y==0) || (SingleLine->FEdgePointCount <30)){
								LBDx = 0.0f;
								LBDy = 0.0f;
								//화면에 디스플레이 위해서 
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

								// Corner 연마 안쪽라인 
								if(CorGrindInThresHold!=0){
									EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
									SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

									// Corner 기울기 완만하여 Edge찾을 경우 Defect 오버킬 발생함 
									// 방향을 Top방향에서 다시 엣지를 찾음 (Corner Defect OverKill 감소 목적)
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
										// Corner 연마 안쪽라인 찾는다 
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

									//코너 연마안쪽라인 기준 새로운 교차점 생성 
									FLBLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
									FLBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);
									BtmEdgePoint[0].x = FLBBInPoint.x;
									BtmEdgePoint[0].y = FLBBInPoint.y;

									//C3 코너 연마폭 
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
								// 코너 연마량 기준 라인을 찾는다 
								// ROI 영역 설정(3개의 흰색 점선 기준 라인을 찾는다)
								if(CorGrindMeaThresHold!=0){
									if(CorGrindMeaThresHold==0){SingleLine->GImage = &RotateImg           ;  SingleLine->FThreshold =5 ;}
									else                       {SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold =10;}

									if(LBCornerWidth >(LBCornerHeight*1.5)){ // 비대칭 형식 Chamber Cut 
										SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x                   );
										SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y-LBCornerHeight +20);
										SingleLine->IRoi->SetWidth ((int)LBCornerWidth -50            );
										SingleLine->IRoi->SetHeight((int)LBCornerHeight-30            );

										SingleLine->SetOrientation(orSouth);
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();
									}
									else                                   { //일반적인 Chamber CUt
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
									//코너 연마량 기준 라인과 새로운 교차점 생성 
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
									//코너 연마량 계산  
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
								//화면에 디스플레이 위해서 
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

								// Corner 연마 안쪽라인 
								if(CorGrindInThresHold!=0){
									EasyImage::Threshold(&RotateImg,&RotateBinaryImg, CorGrindInThresHold);
									SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold =10;

									// Corner 기울기 완만하여 Edge찾을 경우 Defect 오버킬 발생함 
									// 방향을 Top방향에서 다시 엣지를 찾음 (Corner Defect OverKill 감소 목적)
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
										// Corner 연마 안쪽라인 찾는다 
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

									//코너 연마안쪽라인 기준 새로운 교차점 생성 
									FLBLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
									FLBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);
									BtmEdgePoint[0].x = FLBBInPoint.x;
									BtmEdgePoint[0].y = FLBBInPoint.y;
								}

								// GrindMark기준 연마량,가공량 측정함 
								// LeftBtm 
								if(IsMarkBtm){
									CGrindMark[0].Rect.left = (FLBPoint.x + CMarkToGrindMarkDvX) -GrindMarkRange;
									CGrindMark[0].Rect.top  = (FLBPoint.y - CMarkToGrindMarkDvY) -GrindMarkRange;
									CGrindMark[0].Rect.right  = CGrindMark[0].Rect.left+GrindMarkLength;
									CGrindMark[0].Rect.bottom = CGrindMark[0].Rect.top +GrindMarkLength;

									// 원본이미지에서 Mark 찾는다 
									Find->OnLoad(true,FindDir)                     ;
									Find->GImage = &RotateImg                      ;
									Find->SetContrastMode(EFindContrastMode_Normal);
									Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind 검사 최소 Score
									Find->SetZeroScore(GrindPadMarkMinScore)       ; // 검사 후 최소 Score 
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

									// 검사 및 Match Pos 좌표 받어온다 
									// Find 검사 성공시 데이터 받아옴 
									if(Find->OnExecute()){
										CGrindMark[0].Point.x = Find->Result[0].Px;
										CGrindMark[0].Point.y = Find->Result[0].Py;
										CGrindMark[0].Score   = Find->Result[0].Score;
									}

									// GrindMakr찾을경우 연마량,가공량 측정함
									if(CGrindMark[0].Point.x !=0.0f && CGrindMark[0].Point.y!=0.0f){
										// GrindMark기준 OutData 최단거리 Count정보 Calc 
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
										// OutData기준 InData 최단거리 Count정보 Calc 
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

										// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정(OutData기준)
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

										// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
									else { // GrindMrk 못찾을경우 연마량만 측정함 
										// OutData기준 최소값 데이터 Count산출 
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

										// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

										// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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

						//화면에 디스플레이 위해서 
						FLBLPoint.x = FLBPoint.x;
						FLBLPoint.y = FLBPoint.y;
						FLBBPoint.x = FLBPoint.x;
						FLBBPoint.y = FLBPoint.y;
						// Btm영역에서 직선구간 연마,가공 측정 및 Defect검사에 필요한 기준 좌표 
						BtmEdgePoint[0].x =FLBPoint.x;
						BtmEdgePoint[0].y =FLBPoint.y;
					}
					else if(IsCircleBtm){ // Circle Cut(LeftBtm)
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg       , BtmEdgeThresHold    ); // Circle Grind Edge(Polygon_Mode, Mark_Mode사용)
						EasyImage::Threshold(&RotateImg,&RotateGrindInBinaryImg, CirGrindInThresHold ); // Circle Grind In_EdgePoint 측정 (Mark_Modr사용)
						TFPoint Cir1GrindRectPoint(0.0f, 0.0f);
						if(IsMarkBtm){
							if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f){
								// 원본이미지에서 Mark 찾는다 
								Find->OnLoad(true,FindDir)                     ;
								Find->GImage = &RotateImg                      ;
								Find->SetContrastMode(EFindContrastMode_Normal);
								Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind 검사 최소 Score
								Find->SetZeroScore(GrindPadMarkMinScore)       ; // 검사 후 최소 Score 
								Find->SetMaxInstances(1)                       ;
								Find->SetAngleTolerance(20.0f)                 ;
								Find->SetScaleBias(1.00f)                      ;
								Find->SetScaleTolerance(0.0f)                  ;
								Find->Property.Ox = (float)ImgW/2              ;
								Find->Property.Oy = (float)ImgH/2              ;

								// 첫번째 GrindMark기준 센터 C1 Search 
								if(CMarkToGrindMarkDvX!=0.0f && CMarkToGrindMarkDvY!=0.0f){
									CGrindMark[0].Rect.left   = (FLBPoint.x+CMarkToGrindMarkDvX)-40;
									CGrindMark[0].Rect.top    = (FLBPoint.y-CMarkToGrindMarkDvY)-40;
									CGrindMark[0].Rect.right  = (CGrindMark[0].Rect.left + 80);
									CGrindMark[0].Rect.bottom = (CGrindMark[0].Rect.top  + 80);

									GrindRectWidth  = (CGrindMark[0].Rect.right  - CGrindMark[0].Rect.left);
									GrindRectHeight = (CGrindMark[0].Rect.bottom - CGrindMark[0].Rect.top );

									Find->IRoi->SetPlacement(CGrindMark[0].Rect.left,CGrindMark[0].Rect.top,GrindRectWidth,GrindRectHeight);
									if(CGrindMark[0].Rect.left<=0 || CGrindMark[0].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

									// 검사 및 Match Pos 좌표 받어온다 
									// Find 검사 성공시 데이터 받아옴 
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
							if(CSideCircleWidth!=0.0f && CSideCircleHeight!=0.0f  ){ // Notch부 좌우 영역설정 
								NotchPolyRect[0].left  = (long)(FLBPoint.x                 );  NotchPolyRect[0].top    = (long)(FLBPoint.y - CSideCircleHeight);
								NotchPolyRect[0].right = (long)(FLBPoint.x+CSideCircleWidth);  NotchPolyRect[0].bottom = (long)(FLBPoint.y                    );
							}
							TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
							// Polygon Algorithm 기본 설정  
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

								// Median Filter 적용 
								if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
								if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
								if( (P1.x + int(P2.x - P1.x)) >= ImgW || (P1.y + int(P3.y - P1.y)) >= ImgH ) return false;
								FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
								FilterROI.Attach(&RotateBinaryImg)                                    ;
								FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
								LogUnit.SetLog(L"Polygon1 ==>Px :%d, Py :%d, Width:%d, Height:%d ==>Median Filter",P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
								LogUnit.SetLog(L"Polygon1 ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
								EasyImage::Median(&FilterROI, &FilterROI)                             ;
								// 										 RotateBinaryImg.Save("D:\\ROTATE_IMG\\Polygon_BinaryImg_필터1.bmp");

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

								// Polygon Algorithm InEdgeData/OutEdgeData Separate 및 데이터 추출 
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
												if(k==(Polygon->Blob[0].OutCnt-PolygonMarginCnt-1)){ // 직선구간에 필요한 시작점 데이터 좌료 
													BtmEdgePoint[0].x = C1CirOutPoint[PixelCnt].x;
													BtmEdgePoint[0].y = C1CirOutPoint[PixelCnt].y;
												}
												PixelCnt++;
											}
										}
									}
								}
								/////////////////////////////////////////////////////////////////////////////////////////////

								// InEdgeData/OutEdgeData 기준 가공량 및 연마량 계산
								// Polygon 알고리즘에서는 GrindMark기준 OutData,InData 최단거리 2개 Tag값 산출해야함 
								if(IsMarkBtm){ // LTPS 가공량,연마량 계산 
									if(C1CirOutEdgeCount!=0 && C1CirInEdgeCount!=0){ // MarkMode경우 가공량 검사를 먼저 진행 후 연마량 산출
										// GrindMark[0] 기준으로 연마량,가공량 측정 시퀀스 
										if(CGrindMark[0].Point.x!=0.0f && CGrindMark[0].Point.y!=0.0f){  //GrindMark 찾았을경우 
											// GrindMark기준 OutData 최단거리 Count정보 Calc 
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

											// GrindMark기준 InData 최단거리 Count정보 Calc 
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

											// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정(OutData기준)
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

											// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
										else if(CGrindMark[0].Rect.left!=0 && CGrindMark[0].Rect.top!=0 && CGrindMark[0].Rect.right!=0 && CGrindMark[0].Rect.bottom!=0) {  //GrindMark 못 찾았을경우
											// OutData기준 최소값 데이터 Count산출 
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

											// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

											// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
								else         { //Polygon EnCap 계산시 OutPoint 중간값 기준 실측 거리 평균값을 계산한다(LeftTop) 
									if(C1CirInEdgeCount!=0 && C1CirOutEdgeCount!=0){
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
										GrindMarkOutCnt = (int)(C1CirOutEdgeCount/2);
										// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

										// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
								// FAVION Circle Fitting 결과물(Circle_1)
								SampleSize   = C1CirInEdgeCount*0.75; //0.5~0.8이하로 입력할것
								DistDevRatio = 0.15                 ; //0.1~0.25 이하로 입력할것
								CircleFit->OnRansac(C1CirInPoint,C1CirInEdgeCount,SampleSize,DistDevRatio,&CircleFit1Center,&CirFitDiameter1);

								// Polygon 박리 검사 시퀀스 
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
												Edge->OnExecute(C1CirOutPoint[k],Target); // 연마외곽라인에서 Target까지 White Point Search 
												if(Edge->GetResultEdgePointCount()>0){
													C1CircleData[DataCount].Dis =0.0f;
													C1CircleData[DataCount].Dv  =0.0f;
													C1CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
													C1CircleData[DataCount].P.y = C1CircleData[DataCount].P.y - ImageHeight; // Frame기준 단위좌표로 변경함  
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
												Edge->OnExecute(C1CirOutPoint[k],Target); // 연마외곽라인에서 Target까지 White Point Search 
												if(Edge->GetResultEdgePointCount()>0){
													C1CircleData[DataCount].Dis =0.0f;
													C1CircleData[DataCount].Dv  =0.0f;
													C1CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
													C1CircleData[DataCount].P.y = C1CircleData[DataCount].P.y - ImageHeight; // Frame기준 단위좌표로 변경함  
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

							Circle->SetSamplingStep(1)                            ; // Circle Edge 간격 

							// 첫번째 Circle EdgePoint Data 추출 
							if(C1CirCenter.x!=0 && C1CirCenter.y!=0){
								LogUnit.SetLog(L"Circle_1_Insp_Start");    

								if(CirGrindInThresHold==0) Circle->GImage = &RotateImg             ; //Org Image 넘겨줌 
								else                       Circle->GImage = &RotateGrindInBinaryImg; //Binary Image 넘겨줌 

								Circle->SetCenterXY(C1CirCenter.x      , C1CirCenter.y     ); // 외곽라인 교차점 기준 센터 
								C1Diameter = (CSideCircleWidth*2);
								// 									 C1Diameter = (CSideCircleWidth*2);
								// 검사 영역으로 인해서 불량 미검출 발생할수 있음....LTPS와 EnCap분류함
								if(IsMarkBtm) Circle->SetTolerance(60.0f);
								else          Circle->SetTolerance(80.0f);
								// 									 Circle->SetTolerance(40.0f )                              ; // Circle Tolerance 
								Circle->SetDiameter (C1Diameter)                          ; // Circle Diameter 
								Circle->SetCircleChoice(1)                                ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
								Circle->SetCircleType  (2)                                ; // 0:Black to White, 1:White to Black 2: Both Black, White 
								Circle->SetCircleIndex (1)                                ; // Circle EdgePoint Index 

								// Angle 및 Amplitude 설정 
								if(SideCirDeg>=0){
									Circle->SetAngle    (90+CirAngleMargin          );  // Circle Fitting 시작점 각도
									Circle->SetAmplitude(SideCirDeg-2*CirAngleMargin);  // Circle Fitting 시작점 부터 검사 범위 
								}
								// Circle1 InData EdgePoint 
								//////////////////////////////////////////////////////////////////////////////////////////////////////
								Circle->OnExecute()                               ;

								// Circle In Data 기준 새로운 Circle Center 좌표 받아옴 
								RealC1CirCenter.x = Circle->FCP.x;
								RealC1CirCenter.y = Circle->FCP.y;
								CirDiameter1      = (Circle->Diameter)/2.0;

								// Circle In Data취합 
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

								// Circle Out Data 다시 검사  
								//////////////////////////////////////////////////////////////////////////////////////////////////////
								if(BtmEdgeThresHold==0) Circle->GImage = &RotateImg      ; //Org Image 넘겨줌 
								else                    Circle->GImage = &RotateBinaryImg; //Binary Image 넘겨줌 

								// 검사 영역으로 인해서 불량 미검출 발생할수 있음....LTPS와 EnCap분류함
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

								// Circle Out Data취합 
								OrgC1CirOutEdgeCount = Circle->FCirclePointCount;
								if(OrgC1CirOutEdgeCount>0){
									if(OrgC1CirOutPoint!=NULL) delete[] OrgC1CirOutPoint;
									OrgC1CirOutPoint = new TFPoint[OrgC1CirOutEdgeCount];

									for(int k=0; k<OrgC1CirOutEdgeCount; k++){
										if(k>MAX_CIRCLE_EDGEPOINT) break;
										OrgC1CirOutPoint[k].x = Circle->FCirclePoint[k].x;
										OrgC1CirOutPoint[k].y = Circle->FCirclePoint[k].y;
										// BTM영역 Circle을 제외한 영역 Defect검사 목적에 좌표계산 
										if(k==0) {BtmEdgePoint[0].x = OrgC1CirOutPoint[k].x; BtmEdgePoint[0].y = OrgC1CirOutPoint[k].y;}
										if(OrgC1CirOutPoint[k].x==0.0f && OrgC1CirOutPoint[k].y==0.0f) C1CirOutEdgeCount++;
									}
								}
								// C1Circle Grind_Width Calculate 
								// TrackMode CirInEdgePoint 기준과 CirOutEdgePoint 수직거리 산출

								if(IsMarkBtm){ // LTPS경우 가공량 계산 ==> 연마량 계산 
									if(OrgC1CirOutEdgeCount!=0 && OrgC1CirInEdgeCount!=0) { // MarkMode경우 가공량 검사를 먼저 진행 후 연마량 산출  
										GrindMeaDis =0.0f, MinDis =10000.0f;C1GrindMarkCount=0;
										if(CGrindMark[0].Point.x!=0.0f && CGrindMark[0].Point.y!=0.0f){ // Grind Mark 찾았을경우 
											for(int k=0; k<OrgC1CirOutEdgeCount; k++){
												if(OrgC1CirOutPoint[k].x==0.0f || OrgC1CirOutPoint[k].y==0.0f) continue;
												if(CGrindMark[0].Point.x==0.0f || CGrindMark[0].Point.y==0.0f) continue;

												GrindMeaDis = Math.GetLength(CGrindMark[0].Point, OrgC1CirOutPoint[k]);
												if(GrindMeaDis<MinDis){
													MinDis = GrindMeaDis;
													C1GrindMarkCount = k;
												}
											}
											// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정 
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

											// GrindMark기준 수직 평균값 연마량 측정 
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
										else { // Grind Mark 못 찾았을경우 (가공량 0, 연마량 계산함)
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
											// GrindMark기준 수직 평균값 연마량 측정 
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
								else     { // MarkMode ==> EnCap경우 연마량만 계산한다 
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

											// CircleFitting 진행시 실제 EdgePoint 데이터가 존재 할 경우만 포함하여 계산한다
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
								// C1 Grind Mark 기준 가공량 측정 시퀀스 종료  
								//////////////////////////////////////////////////////////////////////////////////////////////////////

								// Org Circle데이터중 유호하지 않은 값을 제외한 실제 존재하는 데이터 기준으로 동적 메모리 생성함
								// Circle Defect검사에 필요한 Edge데이터 값
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
									// Circle 원본 데이터 메모리 삭제
									if(OrgC1CirInPoint!=NULL) delete[] OrgC1CirInPoint;
								}
								// FAVION Circle Fitting 결과물 
								SampleSize = C1CirInEdgeCount*0.75; //0.5~0.8이하로 입력할것
								DistDevRatio = 0.15;                //0.1~0.25 이하로 입력할것
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
									// Circle 원본 데이터 메모리 삭제
									if(OrgC1CirOutPoint!=NULL) delete[] OrgC1CirOutPoint;
								}
								// Circle 영역 꽃박리, 원형박리 검출 목적 시퀀스(EnCap)
								// 정식 명칭은 몰라서.....일단 Crack으로 정함 
								if(!IsMarkBtm && BrokenThresHold!=0){
									RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
									EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBrokenImg);

									EasyImage::Threshold(&RotateImg,&RotateBrokenImg,BrokenThresHold);                 //박리 검사 목적 이미지 변환

									if(CirFitDiameter1!=0.0f && CircleFit1Center.x!=0.0f && CircleFit1Center.y!=0.0f){ // FAVION Circle Fitting 성공시 박리검사 진행 
										Circle->GImage = &RotateBrokenImg;                                             //Binary Image 넘겨줌 

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
								//화면에 디스플레이 위해서 
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

								// Corner 기울기 완만하여 Edge찾을 경우 Defect 오버킬 발생함 
								// 방향을 Top방향에서 다시 엣지를 찾음 (Corner Defect OverKill 감소 목적)
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
										// Corner 연마 안쪽라인 찾는다 
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

									//코너 연마안쪽라인 기준 새로운 교차점 생성 
									FRBRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall ].FP0, Line[waRightWall ].FP1);
									FRBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);
									BtmEdgePoint[1].x = FRBBInPoint.x;
									BtmEdgePoint[1].y = FRBBInPoint.y;

									//코너 연마폭 
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

								// 코너 연마량 기준 라인을 찾는다 
								// ROI 영역 설정(3개의 흰색 점선 기준 라인을 찾는다)
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

									//코너 연마량 기준 라인과 새로운 교차점 생성 
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
									//코너 연마량 계산  
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
								//화면에 디스플레이 위해서 
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

								// Corner 기울기 완만하여 Edge찾을 경우 Defect 오버킬 발생함 
								// 방향을 Top방향에서 다시 엣지를 찾음 (Corner Defect OverKill 감소 목적)
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
										// Corner 연마 안쪽라인 찾는다 
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

									//코너 연마안쪽라인 기준 새로운 교차점 생성 
									FRBRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall ].FP0, Line[waRightWall ].FP1);
									FRBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);
									BtmEdgePoint[1].x = FRBBInPoint.x;
									BtmEdgePoint[1].y = FRBBInPoint.y;
								}

								// GrindMark기준 연마량,가공량 측정함 
								// RightBtm 
								if(IsMarkBtm){
									CGrindMark[1].Rect.left   = (FRBPoint.x - CMarkToGrindMarkDvX) -GrindMarkRange;
									CGrindMark[1].Rect.top    = (FRBPoint.y - CMarkToGrindMarkDvY) -GrindMarkRange;
									CGrindMark[1].Rect.right  = CGrindMark[1].Rect.left+GrindMarkLength;
									CGrindMark[1].Rect.bottom = CGrindMark[1].Rect.top +GrindMarkLength;

									// 원본이미지에서 Mark 찾는다 
									Find->OnLoad(true,FindDir)                     ;
									Find->GImage = &RotateImg                      ;
									Find->SetContrastMode(EFindContrastMode_Normal);
									Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind 검사 최소 Score
									Find->SetZeroScore(GrindPadMarkMinScore)       ; // 검사 후 최소 Score 
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

									// 검사 및 Match Pos 좌표 받어온다 
									// Find 검사 성공시 데이터 받아옴 
									if(Find->OnExecute()){
										CGrindMark[1].Point.x = Find->Result[0].Px;
										CGrindMark[1].Point.y = Find->Result[0].Py;
										CGrindMark[1].Score   = Find->Result[0].Score;
									}
									// GrindMakr찾을경우 연마량,가공량 측정함
									if(CGrindMark[1].Point.x!=0.0f && CGrindMark[1].Point.y!=0.0f){
										// GrindMark기준 OutData 최단거리 Count정보 Calc 
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
										// OutData기준 InData 최단거리 Count정보 Calc 
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

										// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정(OutData기준)
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

										// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
									else { // GrindMrk 못찾을경우 연마량만 측정함 
										// OutData기준 최소값 데이터 Count산출 
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

										// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

										// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
						//화면에 디스플레이 위해서 
						FRBRPoint.x = FRBPoint.x;
						FRBRPoint.y = FRBPoint.y;
						FRBBPoint.x = FRBPoint.x;
						FRBBPoint.y = FRBPoint.y;
						// Btm영역에서 직선구간 연마,가공 측정 및 Defect검사에 필요한 기준 좌표 
						BtmEdgePoint[1].x =FRBPoint.x;
						BtmEdgePoint[1].y =FRBPoint.y;
					}
					else if(IsCircleBtm){ // Circle Cut(RightBtm )
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg       , BtmEdgeThresHold    ); // Circle Grind Edge(Polygon_Mode, Mark_Mode사용)
						EasyImage::Threshold(&RotateImg,&RotateGrindInBinaryImg, CirGrindInThresHold ); // Circle Grind In_EdgePoint 측정 (Mark_Modr사용)
						TFPoint Cir2GrindRectPoint(0.0f, 0.0f);

						if(IsMarkBtm){ // GrindMark Search 
							if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f){
								// 원본이미지에서 Mark 찾는다 
								Find->OnLoad(true,FindDir)                     ;
								Find->GImage = &RotateImg                      ;
								Find->SetContrastMode(EFindContrastMode_Normal);
								Find->SetMinScore (GrindPadMarkMinScore)       ; // PatternrFind 검사 최소 Score
								Find->SetZeroScore(GrindPadMarkMinScore)       ; // 검사 후 최소 Score 
								Find->SetMaxInstances(1)                       ;
								Find->SetAngleTolerance(20.0f)                 ;
								Find->SetScaleBias(1.00f)                      ;
								Find->SetScaleTolerance(0.0f)                  ;
								Find->Property.Ox = (float)ImgW/2              ;
								Find->Property.Oy = (float)ImgH/2              ;

								// 첫번째 GrindMark기준 센터 C1 Search 
								if(CMarkToGrindMarkDvX!=0.0f && CMarkToGrindMarkDvY!=0.0f){
									CGrindMark[1].Rect.left   = (FRBPoint.x-CMarkToGrindMarkDvX)-40;
									CGrindMark[1].Rect.top    = (FRBPoint.y-CMarkToGrindMarkDvY)-40;
									CGrindMark[1].Rect.right  = (CGrindMark[1].Rect.left + 80);
									CGrindMark[1].Rect.bottom = (CGrindMark[1].Rect.top  + 80);

									GrindRectWidth  = (CGrindMark[1].Rect.right  - CGrindMark[1].Rect.left);
									GrindRectHeight = (CGrindMark[1].Rect.bottom - CGrindMark[1].Rect.top );

									Find->IRoi->SetPlacement(CGrindMark[1].Rect.left,CGrindMark[1].Rect.top,GrindRectWidth,GrindRectHeight);
									if(CGrindMark[1].Rect.left<=0 || CGrindMark[1].Rect.top<=0 || GrindRectWidth<=0 || GrindRectHeight<=0) return false;

									// 검사 및 Match Pos 좌표 받어온다 
									// Find 검사 성공시 데이터 받아옴 
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
							if(CSideCircleWidth!=0.0f && CSideCircleHeight!=0.0f  ){ // Notch부 좌우 영역설정 
								NotchPolyRect[1].left  = (long)(FRBPoint.x-CSideCircleWidth);  NotchPolyRect[1].top    = (long)(FRBPoint.y - CSideCircleHeight);
								NotchPolyRect[1].right = (long)(FRBPoint.x                 );  NotchPolyRect[1].bottom = (long)(FLBPoint.y                    );
							}
							TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
							// Polygon Algorithm 기본 설정  
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

								// Median Filter 적용 
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

								// Polygon Algorithm InEdgeData/OutEdgeData Separate 및 데이터 추출 
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

												if(k==PolygonMarginCnt){ //직선구간에 필요한 시작점  
													BtmEdgePoint[1].x = C2CirOutPoint[PixelCnt].x;
													BtmEdgePoint[1].y = C2CirOutPoint[PixelCnt].y;
												}
												PixelCnt++;
											}
										}
									}
								}
								/////////////////////////////////////////////////////////////////////////////////////////////

								// InEdgeData/OutEdgeData 기준 가공량 및 연마량 계산
								// Polygon 알고리즘에서는 GrindMark기준 OutData,InData 최단거리 2개 Tag값 산출해야함 
								if(IsMarkBtm){ // LTPS 가공량,연마량 계산 
									if(C2CirOutEdgeCount!=0 && C2CirInEdgeCount!=0){ // MarkMode경우 가공량 검사를 먼저 진행 후 연마량 산출 
										if(CGrindMark[1].Point.x!=0.0f && CGrindMark[1].Point.y!=0.0f){  //GrindMark 찾았을경우 
											// GrindMark기준 OutData 최단거리 Count정보 Calc 
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

											// GrindMark기준 InData 최단거리 Count정보 Calc 
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

											// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정(OutData기준)
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

											// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
										else if(CGrindMark[1].Rect.left!=0 && CGrindMark[1].Rect.top!=0 && CGrindMark[1].Rect.right!=0 && CGrindMark[1].Rect.bottom!=0){  //GrindMark 못 찾았을경우
											// OutData기준 최소값 데이터 Count산출 
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

											// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

											// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
								else         { //Polygon EnCap 계산시 OutPoint 중간값 기준 실측 거리 평균값을 계산한다(LeftTop) 
									if(C2CirInEdgeCount!=0 && C2CirOutEdgeCount!=0){
										GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
										GrindMarkOutCnt = (int)(C2CirOutEdgeCount/2);
										// 최단거리OUtCount기준 InData 최단거리 Count정보 Calc 
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

										// GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
								// FAVION Circle Fitting 결과물(Circle_1)
								SampleSize   = C2CirInEdgeCount*0.75; //0.5~0.8이하로 입력할것
								DistDevRatio = 0.15                 ; //0.1~0.25 이하로 입력할것
								CircleFit->OnRansac(C2CirInPoint,C2CirInEdgeCount,SampleSize,DistDevRatio,&CircleFit2Center,&CirFitDiameter2);

								// Polygon 박리 검사 시퀀스 
								if(!IsMarkBtm && BrokenThresHold!=0){
									RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
									EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBrokenImg);
									EasyImage::Threshold(&RotateImg,&RotateBrokenImg, BrokenThresHold);
									// 											 RotateBrokenImg.Save("D:\\ROTATE_IMG\\Polygon_하단박리.bmp");
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
												Edge->OnExecute(C2CirOutPoint[k],Target); // 연마외곽라인에서 Target까지 White Point Search 
												if(Edge->GetResultEdgePointCount()>0){
													C2CircleData[DataCount].Dis =0.0f;
													C2CircleData[DataCount].Dv  =0.0f;
													C2CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
													C2CircleData[DataCount].P.y = C2CircleData[DataCount].P.y - ImageHeight; // Frame기준 단위좌표로 변경함  
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
												Edge->OnExecute(C2CirOutPoint[k],Target); // 연마외곽라인에서 Target까지 White Point Search 
												if(Edge->GetResultEdgePointCount()>0){
													C2CircleData[DataCount].Dis =0.0f;
													C2CircleData[DataCount].Dv  =0.0f;
													C2CircleData[DataCount].P   = Edge->GetResultEdgePoint(0);
													C2CircleData[DataCount].P.y = C2CircleData[DataCount].P.y - ImageHeight; // Frame기준 단위좌표로 변경함  
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

								if(CirGrindInThresHold==0) Circle->GImage = &RotateImg             ; //Org Image 넘겨줌 
								else                       Circle->GImage = &RotateGrindInBinaryImg; //Binary Image 넘겨줌 

								Circle->SetCenterXY(C2CirCenter.x      , C2CirCenter.y     ); // 외곽라인 교차점 기준 센터 
								C2Diameter = (CSideCircleWidth*2);
								// 									 C6Diameter = (CSideCircleWidth*2);
								if(IsMarkBtm) Circle->SetTolerance(60.0f);
								else          Circle->SetTolerance(80.0f);
								Circle->SetDiameter (C2Diameter)     ; // Circle Diameter 
								Circle->SetCircleChoice(1)           ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
								Circle->SetCircleType  (2)           ; // 0:Black to White, 1:White to Black 2: Both Black, White 
								Circle->SetCircleIndex (1)           ; // Circle EdgePoint Index 

								if(SideCirDeg>=0){
									if(90-SideCirDeg>=0) Circle->SetAngle(90-SideCirDeg+CirAngleMargin   );  // Circle Fitting 시작점 각도
									Circle->SetAmplitude(SideCirDeg-2*CirAngleMargin);  // Circle Fitting 시작점 부터 검사 범위 
								}

								Circle->OnExecute()                  ;

								// Circle In Data 기준 새로운 Circle Center 좌표 받아옴 
								RealC2CirCenter.x = Circle->FCP.x;
								RealC2CirCenter.y = Circle->FCP.y;
								CirDiameter2      = (Circle->Diameter)/2.0;

								// Circle In Data취합 
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

								// Circle Out Data목적 다시 검사  
								if(BtmEdgeThresHold==0) Circle->GImage = &RotateImg      ; //Org Image 넘겨줌 
								else                    Circle->GImage = &RotateBinaryImg; //Binary Image 넘겨줌 

								if(IsMarkBtm) Circle->SetTolerance(60.0f);
								else          Circle->SetTolerance(80.0f);
								// 									 Circle->SetTolerance(40.0f )                      ; // Circle Tolerance 
								Circle->SetDiameter (C2Diameter)                  ; // Circle Diameter 
								// 									 Circle->SetCircleType(0)                          ; // 0:Black to White, 1:White to Black 2: Both Black, White 
								Circle->SetCircleChoice(1)                        ; // 0:From Begin, 1:From End, 2:Largest Amplitude 
								Circle->SetCircleType  (2)                        ; // 0:Black to White, 1:White to Black 2: Both Black, White 
								Circle->SetCircleIndex (0)                        ; // Circle EdgePoint Index 

								Circle->OnExecute()                               ;

								// Circle Out Data취합 
								OrgC2CirOutEdgeCount = Circle->FCirclePointCount;
								if(OrgC2CirOutEdgeCount>0){
									if(OrgC2CirOutPoint!=NULL) delete[] OrgC2CirOutPoint;
									OrgC2CirOutPoint = new TFPoint[OrgC2CirOutEdgeCount];

									for(int k=0; k<OrgC2CirOutEdgeCount; k++){
										if(k>MAX_CIRCLE_EDGEPOINT) break;
										OrgC2CirOutPoint[k].x = Circle->FCirclePoint[k].x;
										OrgC2CirOutPoint[k].y = Circle->FCirclePoint[k].y;
										// BTM영역 Circle을 제외한 영역 Defect검사 목적에 좌표계산 
										if(k==(OrgC2CirOutEdgeCount-1)) {BtmEdgePoint[1].x = OrgC2CirOutPoint[k].x; BtmEdgePoint[1].y = OrgC2CirOutPoint[k].y;}
										if(OrgC2CirOutPoint[k].x==0.0f && OrgC2CirOutPoint[k].y==0.0f) C2CirOutEdgeCount++;
									}
								}
								// C2Circle Grind_Width, Grind_Measure 
								// C2Circle 연마량 가공량 검사 시퀀스 시작(TrackMode VS MarkMode)
								if(IsMarkBtm){ // LTPS 가공량 계산 ==> 연마량 계산 
									if(OrgC2CirOutEdgeCount!=0 && OrgC2CirInEdgeCount!=0){// 연마량이 존재할경우 가공량 검사를 진행함 
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
											// GrindMark기준 수직 포인트 주변값 Avg산출한다 
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
											// GrindMark기준 수직 평균값 연마량 측정 
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
										else { // Grind Mark 못 찾았을경우 (가공량 0, 연마량 계산함)
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
											// GrindMark기준 수직 평균값 연마량 측정 
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
								else          { // EnCap 연마량 계산 
									if(OrgC2CirInEdgeCount==OrgC2CirOutEdgeCount){//연마량 계산 
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

								// Org Circle데이터중 유호하지 않은 값을 제외한 실제 존재하는 데이터 기준으로 동적 메모리 생성함
								// Circle Defect검사에 필요한 Edge데이터 값
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
									// Circle 원본 데이터 메모리 삭제
									if(OrgC2CirInPoint!=NULL) delete[] OrgC2CirInPoint;
								}
								// Circle Fitting 결과물 
								SampleSize = C2CirInEdgeCount*0.75; //0.5~0.8이하로 입력할것
								DistDevRatio = 0.15; //0.1~0.25 이하로 입력할것
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
									// Circle 원본 데이터 메모리 삭제
									if(OrgC2CirOutPoint!=NULL) delete[] OrgC2CirOutPoint;
								}
								LogUnit.SetLog(L"Circle_6_Insp_End");   
								// Circle 영역 꽃박리, 원형박리 검출 목적 시퀀스(EnCap)
								// 정식 명칭은 몰라서.....일단 Crack으로 정함 
								if(!IsMarkBtm && BrokenThresHold!=0){
									if(CirFitDiameter2!=0.0f && CircleFit2Center.x!=0.0f && CircleFit2Center.y!=0.0f){ // FAVION Circle Fitting 성공시 박리검사 진행 
										Circle->GImage = &RotateBrokenImg;                                             //Binary Image 넘겨줌 

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
				if(FBottomWall){ // 가상포인트 기준 연마,가공 재측정 목적
					if(BtmEdgePoint[0].x!=0.0f && BtmEdgePoint[0].y!=0.0f && BtmEdgePoint[1].x!=0.0f && BtmEdgePoint[1].y!=0.0f){
						EasyImage::Threshold(&RotateImg,&RotateBinaryImg, BtmEdgeThresHold);
						int BtmRange =0;
						TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
						P1.x = (BtmEdgePoint[0].x+10); P1.y =(BtmEdgePoint[0].y-50); P2.x = (P1.x+(BtmEdgePoint[1].x - BtmEdgePoint[0].x)-20); P2.y = (P1.y);
						P3.x = (BtmEdgePoint[0].x+10); P3.y =(BtmEdgePoint[0].y+50); P4.x = (P2.x                                           ); P4.y = (P3.y);

						// Median Filter 적용 
						if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
						if(P1.x>=ImgW || P1.y>=ImgH || (P2.x - P1.x)>=ImgW || (P3.y - P1.y)>=ImgH) return false;
						FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
						FilterROI.Attach(&RotateBinaryImg)                                    ;
						FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
						LogUnit.SetLog(L"첫번째 직선 ==>Px :%d, Py :%d, Width:%d, Height:%d ==>Median Filter",P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
						LogUnit.SetLog(L"첫번째 직선 ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
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

						// Polygon Algorithm InEdgeData/OutEdgeData Separate 및 데이터 추출 
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

						// BtmInLine기준 BtmStdLine EdgePoint 검출 및 LineFitting 진행함 
						if(BtmStdThresHold!=0){
// 							EasyImage::Threshold(GImage,&BinaryImg, BtmStdThresHold);
							EasyImage::Threshold(&RotateImg,&RotateBinaryImg, BtmStdThresHold);
// 							RotateBinaryImg.Save("D:\\ROTATE_IMG\\SoulBrain_BTM_직선영역.bmp");

							if(BtmEdgeThresHold==0)    {SingleLine->GImage = &RotateImg      ; SingleLine->FThreshold =5 ;}
							else                       {SingleLine->GImage = &RotateBinaryImg; SingleLine->FThreshold =10;}

							SingleLine->IRoi->SetOrgX  (BtmInLine[0].FP0.x                    );
							SingleLine->IRoi->SetOrgY  (BtmInLine[0].FP0.y -200               );
							SingleLine->IRoi->SetWidth (BtmInLine[0].FP1.x -BtmInLine[0].FP0.x);
							SingleLine->IRoi->SetHeight(180                                   ); // TEST목적

							SingleLine->SetOrientation(orSouth);
							SingleLine->FIncrement =50;
							SingleLine->SetFindLineId(0);
							SingleLine->FThreshold =5;
							SingleLine->SetEdgeType(1);  // White To Black 

							// 기본 BtmLine Search 
							SingleLine->OnExecute();
							BtmStdLine[0].FP0 = SingleLine->GetResultP0();
							BtmStdLine[0].FP1 = SingleLine->GetResultP1();
						}

						// BmtStdLine기준 GrindMark좌표설정 및 연마,가공량 측정 시퀀스 
						if(IsMarkBtm){ // LTPS 연마량,가공량 측정 시퀀스 
							if(BtmStdLine[0].FP0.x!=0.0f && BtmStdLine[0].FP0.y!=0.0f && BtmStdLine[0].FP1.x!=0.0f && BtmStdLine[0].FP1.y!=0.0f){
								BtmRange = (int)((BtmStdLine[0].FP1.x - BtmStdLine[0].FP0.x)/8);
								for(int k=0; k<7; k++){
									BtmGrindMark[k].Point.x     =  BtmStdLine[0].FP0.x + (BtmRange*(k+1))        ;
									BtmGrindMark[k].Point.y     = (BtmStdLine[0].FP0.y + BtmStdLine[0].FP1.y)/2.0;
									BtmGrindMark[k].Rect.left   = BtmGrindMark[k].Point.x - GrindMarkRange       ;
									BtmGrindMark[k].Rect.top    = BtmGrindMark[k].Point.y - GrindMarkRange       ;
									BtmGrindMark[k].Rect.right  = BtmGrindMark[k].Rect.left+GrindMarkLength      ;
									BtmGrindMark[k].Rect.bottom = BtmGrindMark[k].Rect.top +GrindMarkLength      ;

									// BtmStdLine기준 가상GrindMark에서 수직으로 만나는 최소거리 좌표 카운트 계산  
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

									// BtmOutLine기준 BtmInLine수직으로 만나는 최소거리 좌표 카운트 계산  
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
									// BtmStdLine기준 가공량 측정 
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

									// 가상 GrindMark기준 수직 포인트 주변값 Avg산출한다 ==>연마량 측정(OutData,InData기준)
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
									// 전체 평균 연마량 계산 
								GrindWidthDis = 0.0f;
							    if((fabs(BtmOutLine[0].FP0.y - BtmOutLine[0].FP1.y))>1 ||(fabs(BtmInLine[0].FP0.y - BtmInLine[0].FP1.y))>1){
								    GrindWidthDis = BtmOutLine[0].FP0.y - BtmInLine[0].FP0.y; 
								}
								else{
									GrindWidthDis = Math.GetLengthFrLineToPoint(BtmOutLine[0].FP0, BtmOutLine[0].FP1, BtmInLine[0].FP0);
								}

								// BtmGrindMark데이터에 전체기준 Aag연마량 데이터 넘겨줌 
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
// 사용자 Get함수 
//------------------------------------------------------------------------------------------
CFind* CFM2xLineDefect::GetFindPointer(int n)
{
	if     (n==0) return Find1;
	else if(n==1) return Find2;
	return NULL;
}

//------------------------------------------------------------------------------------
// 사용자 Set함수 
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
