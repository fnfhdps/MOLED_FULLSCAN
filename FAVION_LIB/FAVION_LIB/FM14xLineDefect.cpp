#include "stdafx.h"
#include "FM14xLineDefect.h"
#include "clipper.h"

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

//신규 프로젝트(LineScan4k X 8)
//////////////////////////////////////////////////////////////
CFM14xLineDefect::CFM14xLineDefect()
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
	IsShortCell  = false;

	MarkPos.x    = 0.0f;
	MarkPos.y    = 0.0f;

	MarkEndPos.x = 0.0f;
	MarkEndPos.y = 0.0f;

	MarkStrPos.x = 0.0f;
	MarkStrPos.y = 0.0f;

	MarkAng     = 0.0f;

	FBevelIn = false;

	IRoi = new EROIBW8();

	FProcessMode = pmAutoOutLine;

// 	IRoi = new EROIBW8();

	// Dark Defect 
	DefectPointCount = 0; // 전체 불량 개수 초기화
	// Dark Defect 
	ACount           = 0; // 모자모양 불량 개수 초기화
	BCount           = 0; // 젓가락모양 불량 개수 초기화
	CCount           = 0; // 컵모양 불량 개수 초기화
	DCount           = 0;
	// Bridht Defect 
	ACrackCount      = 0;
	BCrackCount      = 0;
	CCrackCount      = 0;
	DCrackCount      = 0;

	// Corner Dark Defect 
	LTCorDefectCnt   = 0;
	LBCorDefectCnt   = 0;

	// Corner Bright Defect 
	LTCorCrackCnt    = 0;
	LBCorCrackCnt    = 0;

	// Dark Defect 
	LTCirDeftectCnt  = 0;
	LBCirDeftectCnt  = 0;

	// Bright Defect 
	LTCirCrackCnt    =0;
	LBCirCrackCnt    =0;

	LTDx=0.0f, LBDx=0.0f;
	LTDy=0.0f, LBDy=0.0f;

	// Corner Data OutPoint Init
	LTCorOutPoint = NULL;
	LBCorOutPoint = NULL;

	// Circle Data OutPoint Init
	LTCirOutPoint   = NULL;
	LBCirOutPoint   = NULL;

	//Left EdgePoint Data Init 
	LeftInPoint       = NULL;
	LeftOutPoint      = NULL;

	//Print EdgePoint Data Init
	PrintScPoint      = NULL;

	//Masking Point Init 
	MaskPoint = NULL;

	CornerThresHold    =0;
	CircleThresHold    =0;

	DarkThresHold      =0;
	BrokenThresHold    =0; 
	MaskDefectThresHold=0;
	MaskBrokenThresHold=0;

	// Mask Image 생성을 위한 Offset 
	MaskDefectOffset   =0;

	//기본 EdgeThresHold Init 
	LeftEdgeThresHold    =0;
	TopEdgeThresHold     =0;
	RightEdgeThresHold   =0;
	BtmEdgeThresHold     =0;

	LeftEdgeBTWhite      =false; //Black To White Edge 검출 
	TopEdgeBTWhite       =false; //Black To White Edge 검출 
	RightEdgeBTWhite     =false; //Black To White Edge 검출 
	BtmEdgeBTWhite       =false; //Black To White Edge 검출 

	// Cut Line Find ThresHold 
	LeftCutThresHold   =0;

	// Print Screen Find ThresHold 
	LeftPrintThresHold =0;

	//  Print Screen 검출 목적 Offset 
	PrintScOffset      =0;
	PrintScRange       =0;

	// Real Out Line Count
	RealOutCnt         =0;

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

	//Notch Circle Dx,Dy 
	LTCircleWidth   =0.0f;
	LTCircleHeight  =0.0f;

	LBCircleWidth   =0.0f;
	LBCircleHeight  =0.0f;

	// 외곽라인에서 개별 PrintMark까지 Width,Height 거리
	OutToGrindMarkDis1=0.0f;
	OutToGrindMarkDis2=0.0f;

	// 박리 검출 목적으로 EdgeLine에서 Trap까지 거리 
	dEdgeToTrapLineDis=0.0f;

	// Notch Cell Mark Init 
	dCirOffsetX        =0.0f;
	dCirOffsetY        =0.0f;

	LogUnit.Create(_T("FAVION"));

	FInspectMode =0; //InspectMode Classify Parameter
	FGlassType   =0;

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

	// Frame기준 GrindMar 유무 결정 
	IsFrameGrindMark = false;

	// Find Dir 
	FindDir  =L"";

	// Grind Mark Find Score 
	GrindMarkMinScore    =0.0f;
	GrindPadMarkMinScore =0.0f;

	// A_Side AlignMark에서 PrintMark까지 거리 편차값 Init
	AMarkToGrindMarkDvX =0.0f;
	AMarkToGrindMarkDvY =0.0f;

	CMarkToGrindMarkDvX =0.0f;
	CMarkToGrindMarkDvY =0.0f;

	OnClearData();
}
CFM14xLineDefect::~CFM14xLineDefect()
{
	delete SingleLine    ;
	delete RectEdge      ;
	delete Edge          ;
	delete Find1         ;
	delete Find2         ;
	delete Circle        ;
	delete Find          ;
	delete CircleFit     ;
	delete Polygon       ;
}

float CFM14xLineDefect::GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3)
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
int CFM14xLineDefect::OnExecute(TShape Shape)
{
	FShape = Shape;
	return OnExecute();
}
int CFM14xLineDefect::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(L"Check CFM14xLineDefect OnExecute GImage Memory Allocate"); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(L"Check CFM14xLineDefect OnExecute GImage Width Size"     ); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(L"Check CFM14xLineDefect OnExecute GImage Height Size"    ); return false; }

	IAddress    = GImage->GetImagePtr();
	BufferWidth = GImage->GetWidth   (); //Buffer 크기설정....특적 좌표에서 Pixel 추출시 필요함 
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
		r  = FRightWall   = false;
		b  = FBottomWall  = false;
		lt = FLeftTop     = true ;
		rt = FRightTop    = false;
		rb = FRightBottom = false;
		lb = FLeftBottom  = false;
		break;

	case 1 :
		// RectS : 젓가락 모양
		//------------------------------------------------------------------
		l  = FLeftWall    = true ;
		t  = FTopWall     = false;
		r  = FRightWall   = false;
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
		r  = FRightWall   = false;
		b  = FBottomWall  = true ;
		lt = FLeftTop     = false;
		rt = FRightTop    = false;
		rb = FRightBottom = false;
		lb = FLeftBottom  = true ;
		break;
	}
	if(!OnExecute(l, t, r, b, lt, rt, rb, lb)) return false;
	return true;
}
CRect CFM14xLineDefect::OnMerge( CRect ar, CRect br)
{
	CRect r;
	r.left   = (ar.left   < br.left  ) ? ar.left   : br.left  ;
	r.top    = (ar.top    < br.top   ) ? ar.top    : br.top   ;
	r.right  = (ar.right  > br.right ) ? ar.right  : br.right ;
	r.bottom = (ar.bottom > br.bottom) ? ar.bottom : br.bottom;

	return r;
}
/////////////////////////////////////////////////////////////////////////////////////
int CFM14xLineDefect::GetSize(TCorner *lpCorner,TBlob *lpDefect, int Count, int Dv)
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
							if(m<=0 || n<=0 || m>FrameImgW || n>FrameImgH) continue;
							PixelValue = IElement(m,n);
							if(PixelValue < CornerThresHold) PixelCount++;
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
int CFM14xLineDefect::GetCircleCrackSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dis, double CirDiameter, TFPoint Center,int AllDefectCnt, bool IsInCircle)
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


	// TEST 
	int StaX =0, StaY=0, StaWidth=0, StaHeight=0;
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

			if (IsRoof && PointDis >MinDis && DefectPoint>5) { 
				// 내부 Pixel Data 축출 
				PixelCount =0, PixelPoint=0, PixelScore=5, Score =0.0f;
				for(int n=Rect.top ; n<Rect.bottom; n++){
					for(int m=Rect.left; m<Rect.right; m++){
						if(m<=0 || n<=0 || m>FrameImgW || n>FrameImgH) continue;
						PixelValue = IElement(m,n);
						if(PixelValue > BrokenThresHold) PixelCount++;
						PixelPoint++;
					}
				}
				EndPos.x = lpCircle[k].P.x;
				EndPos.y = lpCircle[k].P.y;

				StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);

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

					lpDefect[AllDefectCnt].Rect.left    = Rect.left  ;
					lpDefect[AllDefectCnt].Rect.top     = Rect.top   ;
					lpDefect[AllDefectCnt].Rect.right   = Rect.right ; 
					lpDefect[AllDefectCnt].Rect.bottom  = Rect.bottom;

					lpDefect[AllDefectCnt].Height       = (Rect.bottom - Rect.top );
					lpDefect[AllDefectCnt].Width        = (Rect.right  - Rect.left);

					// 					lpDefect[DefectCount].Ver_Dis      = lpCircle[MaxCount].Dv        ;
					// 					lpDefect[DefectCount].Hor_Dis      = Math.GetLength(StaPos,EndPos);

					lpDefect[AllDefectCnt].Ver_Dis      = lpDefect[AllDefectCnt].Height;
					lpDefect[AllDefectCnt].Hor_Dis      = lpDefect[AllDefectCnt].Width ;

					lpDefect[AllDefectCnt].P.x          =  (float) Rect.left;
					lpDefect[AllDefectCnt].P.y          =  (float)(Rect.top +ImageHeight);

					lpDefect[AllDefectCnt].Rect.top     = lpDefect[AllDefectCnt].P.y;
					lpDefect[AllDefectCnt].Rect.bottom  = lpDefect[AllDefectCnt].Rect.top+lpDefect[AllDefectCnt].Height;
					lpDefect[AllDefectCnt].Round        =  PixelCount;
					AllDefectCnt++;
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
int CFM14xLineDefect::GetCircleSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dv) //Mark_Mode Dark Defect 
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
							if(m<=0 || n<=0 || m>FrameImgW || n>FrameImgH) continue;
							PixelValue = IElement(m,n);
							if(PixelValue < DarkThresHold) PixelCount++;
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
int CFM14xLineDefect::GetPolygonSize(TFPoint *PolygonData, TBlob *lpDefect,int TotalCount,int Range, int DefectDv, int AllDefectCnt, int ThresHold)// Polygon Mode Dark Defect 
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
					if(BadRect[BadRectCount-1].BadPixelCount>=MAX_POLYGON_DEFECT_POINT){
						for(int k=0; k<BadRect[BadRectCount].BadPixelCount; k++) {
							BadRect[BadRectCount].Pixel[k].x =0;
							BadRect[BadRectCount].Pixel[k].y =0;
						}
						break;
					}
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
			if(BadRectCount>=MAX_POLYGON_DEFECT_COUNT) break;
			BadRectCount++;
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
									if(m<=0 || n<=0 || m>FrameImgW || n>FrameImgH) continue;
									PixelValue = IElement(m,n);
									if(PixelValue < ThresHold) PixelCount++;
									PixelPoint++;
								}
							}
							Score  = (((float)PixelCount/(float)PixelPoint)*100);
							// Circle Defect 내부의 PixelCount 수량을 체크한다 => 오버킬 감소 목적 
							if(Score>PixelScore){
								lpDefect[AllDefectCnt].Rect.left    = Rect.left  ;
								lpDefect[AllDefectCnt].Rect.top     = Rect.top   ;
								lpDefect[AllDefectCnt].Rect.right   = Rect.right ; 
								lpDefect[AllDefectCnt].Rect.bottom  = Rect.bottom;

								lpDefect[AllDefectCnt].Height       = (Rect.bottom - Rect.top );
								lpDefect[AllDefectCnt].Width        = (Rect.right  - Rect.left);

								lpDefect[AllDefectCnt].Ver_Dis      = lpDefect[AllDefectCnt].Height;
								lpDefect[AllDefectCnt].Hor_Dis      = lpDefect[AllDefectCnt].Width ;

								lpDefect[AllDefectCnt].P.x          =  (float)(Rect.left            );
								lpDefect[AllDefectCnt].P.y          =  (float)(Rect.top +ImageHeight); // 전체 Frame기준 Real좌표 기준으로 변환 

								lpDefect[AllDefectCnt].Rect.top     = lpDefect[AllDefectCnt].P.y;
								lpDefect[AllDefectCnt].Rect.bottom  = lpDefect[AllDefectCnt].Rect.top+lpDefect[AllDefectCnt].Height;
								lpDefect[AllDefectCnt].Round        =  PixelCount;
								AllDefectCnt++;
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
int CFM14xLineDefect::GetAvgCellSize(int TopCount, int MidCount, int BtmCount, int AvgMode)
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
int CFM14xLineDefect::GetCellSize(int Count)
{
	if(MarkStrPos.x==0 || MarkStrPos.y==0 || MarkEndPos.x==0 || MarkEndPos.y==0) return false;
	if( FCellPoint[Count].x==0 || FCellPoint[Count].y==0)                        return false;
	if(Count>=MAX_DEFECT_POINT)                                                  return false;

	int nCellSize      =0;

	nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkStrPos, MarkEndPos, FCellPoint[Count]));     
	return  nCellSize;
}
void CFM14xLineDefect::OnClearData()
{
	// Defect Data Init 
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

		// Dark Defect 
		BDefectPoint[k].Height    =0;
		BDefectPoint[k].Width     =0;
		BDefectPoint[k].Round     =0;
		BDefectPoint[k].P.x       =0;
		BDefectPoint[k].P.y       =0;
		BDefectPoint[k].Ver_Dis   =0;
		BDefectPoint[k].Hor_Dis   =0;
		BDefectPoint[k].LineAttach=0;

		// Dark Defect 
		CDefectPoint[k].Height    =0;
		CDefectPoint[k].Width     =0;
		CDefectPoint[k].Round     =0;
		CDefectPoint[k].P.x       =0;
		CDefectPoint[k].P.y       =0;
		CDefectPoint[k].Ver_Dis   =0;
		CDefectPoint[k].Hor_Dis   =0;
		CDefectPoint[k].LineAttach=0;

		// Dark Defect 
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

		// Bright Defect 
		BCrackPoint[k].Height    =0;
		BCrackPoint[k].Width     =0;
		BCrackPoint[k].Round     =0;
		BCrackPoint[k].P.x       =0;
		BCrackPoint[k].P.y       =0;
		BCrackPoint[k].Ver_Dis   =0;
		BCrackPoint[k].Hor_Dis   =0;
		BCrackPoint[k].LineAttach=0;

		// Bright Defect 
		CCrackPoint[k].Height    =0;
		CCrackPoint[k].Width     =0;
		CCrackPoint[k].Round     =0;
		CCrackPoint[k].P.x       =0;
		CCrackPoint[k].P.y       =0;
		CCrackPoint[k].Ver_Dis   =0;
		CCrackPoint[k].Hor_Dis   =0;
		CCrackPoint[k].LineAttach=0;

		// Bright Defect 
		DCrackPoint[k].Height    =0;
		DCrackPoint[k].Width     =0;
		DCrackPoint[k].Round     =0;
		DCrackPoint[k].P.x       =0;
		DCrackPoint[k].P.y       =0;
		DCrackPoint[k].Ver_Dis   =0;
		DCrackPoint[k].Hor_Dis   =0;
		DCrackPoint[k].LineAttach=0;

		LTCorDefectPoint[k].Height    =0;
		LTCorDefectPoint[k].Width     =0;
		LTCorDefectPoint[k].Round     =0;
		LTCorDefectPoint[k].P.x       =0;
		LTCorDefectPoint[k].P.y       =0;
		LTCorDefectPoint[k].Ver_Dis   =0;
		LTCorDefectPoint[k].Hor_Dis   =0;
		LTCorDefectPoint[k].LineAttach=0;

		LBCorDefectPoint[k].Height    =0;
		LBCorDefectPoint[k].Width     =0;
		LBCorDefectPoint[k].Round     =0;
		LBCorDefectPoint[k].P.x       =0;
		LBCorDefectPoint[k].P.y       =0;
		LBCorDefectPoint[k].Ver_Dis   =0;
		LBCorDefectPoint[k].Hor_Dis   =0;
		LBCorDefectPoint[k].LineAttach=0;

		LTCorCrackPoint[k].Height    =0;
		LTCorCrackPoint[k].Width     =0;
		LTCorCrackPoint[k].Round     =0;
		LTCorCrackPoint[k].P.x       =0;
		LTCorCrackPoint[k].P.y       =0;
		LTCorCrackPoint[k].Ver_Dis   =0;
		LTCorCrackPoint[k].Hor_Dis   =0;
		LTCorCrackPoint[k].LineAttach=0;

		LBCorCrackPoint[k].Height    =0;
		LBCorCrackPoint[k].Width     =0;
		LBCorCrackPoint[k].Round     =0;
		LBCorCrackPoint[k].P.x       =0;
		LBCorCrackPoint[k].P.y       =0;
		LBCorCrackPoint[k].Ver_Dis   =0;
		LBCorCrackPoint[k].Hor_Dis   =0;
		LBCorCrackPoint[k].LineAttach=0;

		// Circle Defect Data Init
		// Dark Defect 
		LTCirDefectPoint[k].Height      =0;
		LTCirDefectPoint[k].Width       =0;
		LTCirDefectPoint[k].Round       =0;
		LTCirDefectPoint[k].P.x         =0;
		LTCirDefectPoint[k].P.y         =0;
		LTCirDefectPoint[k].Rect.left   =0;
		LTCirDefectPoint[k].Rect.top    =0;
		LTCirDefectPoint[k].Rect.right  =0;
		LTCirDefectPoint[k].Rect.bottom =0;
		LTCirDefectPoint[k].Ver_Dis     =0;
		LTCirDefectPoint[k].Hor_Dis     =0;
		LTCirDefectPoint[k].LineAttach  =0;

		LBCirDefectPoint[k].Height      =0;
		LBCirDefectPoint[k].Width       =0;
		LBCirDefectPoint[k].Round       =0;
		LBCirDefectPoint[k].P.x         =0;
		LBCirDefectPoint[k].P.y         =0;
		LBCirDefectPoint[k].Rect.left   =0;
		LBCirDefectPoint[k].Rect.top    =0;
		LBCirDefectPoint[k].Rect.right  =0;
		LBCirDefectPoint[k].Rect.bottom =0;
		LBCirDefectPoint[k].Ver_Dis     =0;
		LBCirDefectPoint[k].Hor_Dis     =0;
		LBCirDefectPoint[k].LineAttach  =0;


		// Bright Defect 
		LTCirCrackPoint[k].Height     =0;
		LTCirCrackPoint[k].Width      =0;
		LTCirCrackPoint[k].Round      =0;
		LTCirCrackPoint[k].P.x        =0;
		LTCirCrackPoint[k].P.y        =0;
		LTCirCrackPoint[k].Rect.left  =0;
		LTCirCrackPoint[k].Rect.top   =0;
		LTCirCrackPoint[k].Rect.right =0;
		LTCirCrackPoint[k].Rect.bottom=0;
		LTCirCrackPoint[k].Ver_Dis    =0;
		LTCirCrackPoint[k].Hor_Dis    =0;
		LTCirCrackPoint[k].LineAttach =0;

		LBCirCrackPoint[k].Height     =0;
		LBCirCrackPoint[k].Width      =0;
		LBCirCrackPoint[k].Round      =0;
		LBCirCrackPoint[k].P.x        =0;
		LBCirCrackPoint[k].P.y        =0;
		LBCirCrackPoint[k].Rect.left  =0;
		LBCirCrackPoint[k].Rect.top   =0;
		LBCirCrackPoint[k].Rect.right =0;
		LBCirCrackPoint[k].Rect.bottom=0;
		LBCirCrackPoint[k].Ver_Dis    =0;
		LBCirCrackPoint[k].Hor_Dis    =0;
		LBCirCrackPoint[k].LineAttach =0;

		LeftDefectPoint[k].Height     =0;
		LeftDefectPoint[k].Width      =0;
		LeftDefectPoint[k].Round      =0;
		LeftDefectPoint[k].P.x        =0;
		LeftDefectPoint[k].P.y        =0;
		LeftDefectPoint[k].Rect.left  =0;
		LeftDefectPoint[k].Rect.top   =0;
		LeftDefectPoint[k].Rect.right =0;
		LeftDefectPoint[k].Rect.bottom=0;
		LeftDefectPoint[k].Ver_Dis    =0;
		LeftDefectPoint[k].Hor_Dis    =0;
		LeftDefectPoint[k].LineAttach =0;

		LeftCrackPoint[k].Height     =0;
		LeftCrackPoint[k].Width      =0;
		LeftCrackPoint[k].Round      =0;
		LeftCrackPoint[k].P.x        =0;
		LeftCrackPoint[k].P.y        =0;
		LeftCrackPoint[k].Rect.left  =0;
		LeftCrackPoint[k].Rect.top   =0;
		LeftCrackPoint[k].Rect.right =0;
		LeftCrackPoint[k].Rect.bottom=0;
		LeftCrackPoint[k].Ver_Dis    =0;
		LeftCrackPoint[k].Hor_Dis    =0;
		LeftCrackPoint[k].LineAttach =0;

		FCellPoint[k].x         =0.0f;
		FCellPoint[k].y         =0.0f;
	}

	// LeftOutLine,TopOutLine,BtmOutLine Init 
	// 기본 라인 
	LeftOutLine [0].FP0.x = 0.0f;
	LeftOutLine [0].FP0.y = 0.0f;
	LeftOutLine [0].FP1.x = 0.0f;
	LeftOutLine [0].FP1.y = 0.0f;

	TopOutLine  [0].FP0.x = 0.0f;
	TopOutLine  [0].FP0.y = 0.0f;
	TopOutLine  [0].FP1.x = 0.0f;
	TopOutLine  [0].FP1.y = 0.0f;

	BtmOutLine  [0].FP0.x = 0.0f;
	BtmOutLine  [0].FP0.y = 0.0f;
	BtmOutLine  [0].FP1.x = 0.0f;
	BtmOutLine  [0].FP1.y = 0.0f;

	PreLeftOutLine[0].FP0.x = 0.0f;
	PreLeftOutLine[0].FP0.y = 0.0f;
	PreLeftOutLine[0].FP1.x = 0.0f;
	PreLeftOutLine[0].FP1.y = 0.0f;
	PreLeftInLine [0].FP0.x = 0.0f;
	PreLeftInLine [0].FP0.y = 0.0f;
	PreLeftInLine [0].FP1.x = 0.0f;
	PreLeftInLine [0].FP1.y = 0.0f;

	// Line Position
	LinePosition = 0.0f;

	// Line Data 
	LeftLineData  =0.0f;
	RightLineData =0.0f;

	//CELL INSPECTION LineData Init
	LeftOutLine [0].FP0.x = 0.0f;
	LeftOutLine [0].FP0.y = 0.0f;
	LeftOutLine [0].FP1.x = 0.0f;
	LeftOutLine [0].FP1.y = 0.0f;

	TopOutLine [0].FP0.x  = 0.0f;
	TopOutLine [0].FP0.y  = 0.0f;
	TopOutLine [0].FP1.x  = 0.0f;
	TopOutLine [0].FP1.y  = 0.0f;

	BtmOutLine  [0].FP0.x = 0.0f;
	BtmOutLine  [0].FP0.y = 0.0f;
	BtmOutLine  [0].FP1.x = 0.0f;
	BtmOutLine  [0].FP1.y = 0.0f;

	//Line Data Init
	for(int i=0; i<MAX_LINE_POINT; i++){
		Line[i].FP0.x =0.0f;
		Line[i].FP0.y =0.0f;
		Line[i].FP1.x =0.0f;
		Line[i].FP1.y =0.0f;
	}
	// Roi Rect Init
	LTROI.left   =0;
	LTROI.top    =0;
	LTROI.right  =0;
	LTROI.bottom =0;

	LBROI.left   =0;
	LBROI.top    =0;
	LBROI.right  =0;
	LBROI.bottom =0;

	PrintROI.left   =0;
	PrintROI.top    =0;
	PrintROI.right  =0;
	PrintROI.bottom =0;

	// CornerData Init
	LTCorOutEdgeCount =0;
	LBCorOutEdgeCount =0;

	// Inspection End
	TopLimitLine = 0 ;
	BtmLimitLine = 0 ;

	// Circle OutEdge Point Count
	LTCirOutEdgeCount = 0;
	LBCirOutEdgeCount = 0;

	LeftInEdgeCount    = 0;
	LeftOutEdgeCount   = 0;

	PrintScEdgeCount = 0;

	// Dark Defect 
	DefectPointCount = 0; // 전체 불량 개수 초기화
	ACount           = 0; // 모자모양 불량 개수 초기화
	BCount           = 0; // 젓가락모양 불량 개수 초기화
	CCount           = 0; // 컵모양 불량 개수 초기화
	DCount           = 0;
	LTCorDefectCnt       = 0;
	LBCorDefectCnt       = 0;

	// Bridht Defect 
	ACrackCount      = 0;
	BCrackCount      = 0;
	CCrackCount      = 0;
	DCrackCount      = 0;

	// Dark Defect 
	LTCirDeftectCnt  = 0;
	LBCirDeftectCnt  = 0;

	// Bright Defect 
	LTCirCrackCnt    =0;
	LBCirCrackCnt    =0;

	// 전체 Defect Count(Polygon 검사)
	LeftDefectCnt =0;
	LeftCrackCnt  =0;

	// 개별 Frame기준 Defect Count(Polygon 검사)
	FrameLeftDefectCnt=0;
	FrameLeftCrackCnt =0;

	// Circle Calculate Data Init 
	LTCirDx      = 0.0f;
	LTCirDy      = 0.0f;
	LTCirRound   = 0.0f;
	LBCirDx      = 0.0f;
	LBCirDy      = 0.0f;
	LBCirRound   = 0.0f;

	// Circle Center Data Init 
	LTCirCenter.x = 0.0f;
	LTCirCenter.y = 0.0f;
	LBCirCenter.x = 0.0f;
	LBCirCenter.y = 0.0f;

	// Real Circle Center Data Init 
	LTRealCirCenter.x = 0.0f;
	LTRealCirCenter.y = 0.0f;
	LBRealCirCenter.x = 0.0f;
	LBRealCirCenter.y = 0.0f;

	LTCircleFitCenter.x    =0.0f;
	LTCircleFitCenter.y    =0.0f;
	LBCircleFitCenter.x    =0.0f;
	LBCircleFitCenter.y    =0.0f;
	AllGrindCriFitCenter.x =0.0f;
	AllGrindCriFitCenter.y =0.0f;
	// 반원형 박리 및 꽃박리 구별목적 Diameter
	LTCrackCirDiameter = 0.0f;
	LBCrackCirDiameter = 0.0f;

	// Euresys Circle Fitting Diameter 
	LTCirDiameter = 0.0f;
	LBCirDiameter = 0.0f;

	// FAVION Circle Fitting Diameter
	LTCirFitDiameter = 0.0f;
	LBCirFitDiameter = 0.0f;

	//Left Init
	LeftGrindWidth =0.0f;
	LeftGrindMea   =0.0f;

	// Corner Chamber Cut 3Point 측정 데이터 초기화 
	LTCorGrindWd  = 0.0f;
	LTCorGrindMea = 0.0f;
	LBCorGrindWd  = 0.0f;
	LBCorGrindMea = 0.0f;

	// Btm Line Search 시간 단축 목적 
	FEndLine =0;

	// Notch Cell Mark Degree 
	MarkDegree =0.0f;

	// 연마 각도 (Circle)
// 	SideCirDeg   =0;
// 	CenterCirDeg =0;

	// 	Find->OnLoad(true,FindDir);

	// Notch Cell Size Data Init 
	LeftCellSizeHeight  =0.0f;

	// Cell Size 관련 Value Init 
	PrintEdge1 =0.0f;
	PrintEdge2 =0.0f;
	PrintEdge3 =0.0f;

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

	// Total Left Grind InData/OutData Init 
	for(int k=0; k<MAX_ALL_EDGE_COUNT; k++){
		AllLeftInData   [k].x = 0.0f; //Etching 후 Cutting 단차 발생시 전체 데이터 
		AllLeftInData   [k].y = 0.0f; //Etching 후 Cutting 단차 발생시 전체 데이터 
		AllLeftOutData  [k].x = 0.0f; //Etching 후 Cutting 단차 발생시 전체 데이터 
		AllLeftOutData  [k].y = 0.0f; //Etching 후 Cutting 단차 발생시 전체 데이터 
		AllCircleOutData[k].x = 0.0f; //Etching 후 Cutting 단차 없을경우 시작,끝 Circle 가공 개별 데이터
		AllCircleOutData[k].y = 0.0f; //Etching 후 Cutting 단차 없을경우 시작,끝 Circle 가공 개별 데이터
		AllCornerOutData[k].x = 0.0f; //Etching 후 Cutting 단차 없을경우 시작,끝 Corner 가공 개별 데이터
		AllCornerOutData[k].y = 0.0f; //Etching 후 Cutting 단차 없을경우 시작,끝 Corner 가공 개별 데이터
		AllPrintScData  [k].x = 0.0f; //Print Screen Data 
		AllPrintScData  [k].y = 0.0f; //Print Screen Data 
	}
	// All Left Data Count Init 
	AllLeftInEdgeCount =0;
	AllLeftOutEdgeCount=0;

	// All Circle Data Count Init 
	AllCirOutEdgeCount=0;

	// All Corner Data Count Init 
	AllCorOutEdgeCount=0;

	// All Print Data Count Init 
	AllPrintScEdgeCount=0;

	// Grind Mark Init(Total:23개)
	for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
		// 넘겨받을 데이터 
		PrintMark[k].degree      =0.0f;
		PrintMark[k].DvX         =0   ;
		PrintMark[k].DvY         =0   ;
		PrintMark[k].MarkID      =0   ;
		PrintMark[k].Range       =0.0f;
		// 검사 후 결과물 데이터 
		PrintMark[k].Gradient    =0.0f;
		PrintMark[k].PrintShift  =0.0f;
		PrintMark[k].PrintEdge   =0.0f;
		PrintMark[k].Point.x     =0.0f;
		PrintMark[k].Point.y     =0.0f;
		PrintMark[k].Rect.left   =0   ;
		PrintMark[k].Rect.top    =0   ;
		PrintMark[k].Rect.right  =0   ;
		PrintMark[k].Rect.bottom =0   ;
		PrintMark[k].RectPoint.x =0   ;
		PrintMark[k].RectPoint.y =0   ;
		PrintMark[k].Score       =0.0f;
	}


	// PrintMark 영역설정시 필요한 각도 데이터 
	dSideDeg =0.0f; 

	//전체 Buffer기준 FLTPoint 좌표 
	BuffLTPoint.x =0.0f;
	BuffLTPoint.y =0.0f;
	
	//Real Out Line Data Init 
	for(int k=0; k<MAX_REAL_FRAME_COUNT; k++){
		RealOutLine[k].FP0.x = 0.0f;
		RealOutLine[k].FP0.y = 0.0f;
		RealOutLine[k].FP1.x = 0.0f;
		RealOutLine[k].FP1.y = 0.0f;
	}
}
//---------------------------------------------------------------
int CFM14xLineDefect::OnAutoBevelIn()// Line기준 실제 검사 함수 
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
	double MaxDy  =0.0f     ;
	double MaxDx  =0.0f     ;
	double StdMaxDis =0.0f     ;
	double StdMinDis =10000.0f ;
	int    MinCirCount =0;

	//Corner Masking 목적 두번째 Masking에서만 사용함
	double CorMaskingDy = 0.0f;

	double GrindWidthDis  =0.0f;
	int GrindMarkOutCnt=0, GrindMarkInCnt =0; 


	int Index = 0;
	// LineScan4K X 8EA 검사 시퀀스(장축/단축) 
	if( FShape==shRectH ){ // (LineScan4K X 8EA)  ImageCopy -> ImageConversion -> ROI Attatch 
//		EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);
		Index =0;   
	}                                                                                                     
	if( FShape==shRectS ){ // (LineScan4K X 8EA)  ImageCopy -> ImageConversion -> ROI Attatch 
//		EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);
		Index =1;                                                                              
	}
	if( FShape==shRectC ){ // (LineScan4K X 8EA)  ImageCopy -> ImageConversion -> ROI Attatch 
//		EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);
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
	const int MaskingMargin   =20  ;

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

	FrameImgW  = GImage->GetWidth() ;
	FrameImgH  = GImage->GetHeight();

// 	ImageWidth = GImage->GetWidth();;

	//Euresys Blob Parameter Setting 
	//////////////////////////////////////////////////
	//CNC_6HEAD 이미지 변환 후 기본설정 진행 
	if((FShape==shRectH || FShape==shRectS || FShape==shRectC)){
		DefectROI.Attach(&DefectImg);
		CodeImageSelection.SetFeretAngle(0.00f);
		CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
		CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );

//		IAddress = DefectImg.GetImagePtr();
	}
	//////////////////////////////////////////////////

	TFPoint LT(0               ,                0);
	TFPoint RT((float)FrameImgW,                0);
	TFPoint RB((float)FrameImgW, (float)FrameImgH);
	TFPoint LB(0               , (float)FrameImgH);
	TFPoint LSP, LEP                              ;

	int BlobCount   =0;
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
	const int yMerge  =35;
	const int MgCount =5 ;

	//Remain Offset 
	const int RemainMargin = 30;

	// BM Line Check
	float RightBMGab=0;
	float LeftBMGab =0;

	// Side Offset Margin
	int SideMargin =2;

	// Margin Filter
	const int BroMarginWidth  = 200;
	const int BroMarginHeight = 200;

	// All Grind(비선형) Circle Fitting Center 
	AllGrindCriFitCenter.x =0.0f;
	AllGrindCriFitCenter.y =0.0f;

	// PrintMark 각도 적용하여 회전목적(검사화면 디스플레이)
	TFPoint LtPos, RbPos;

	float Dis    =0.0f;
	float StdDis =0.0f;

	// Side Offset TEST 
	double  Width   =0.0f;
	long    Left    =0;
	float   CenterX =0.0F;

	// 박리 및 스크래치 검사 목적 변수들 
	CRect Rect;
	Rect.left   = 0;
	Rect.right  = 0;
	Rect.top    = 0;
	Rect.bottom = 0;

	TFPoint DefectLT(0.0f, 0.0f);
	TFPoint DefectRT(0.0f, 0.0f);
	TFPoint DefectLB(0.0f, 0.0f);
	TFPoint DefectRB(0.0f, 0.0f);
	const int MaskDefectMagin =5; // MaskImage Defect좌표에서 연마안쪽라인 까지 최소거리 이하일경우 Defect인정 
	bool IsDefect = false;
	double LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
	TPoint RoiLt(0,0), RoiRt(0,0), RoiLb(0,0), RoiRb(0,0);
	int PreCompareCnt = 0;

	//실제 불량이 Masking Line Attach 여부 체크 필요함 
	TFPoint StaMaskPos(0.0f, 0.0f);
	TFPoint EndMaskPos(0.0f, 0.0f);

	//Clipper Offset Value
	int PolyOutCnt  =0;
	//Maskig Point Count 
	int MaskCnt = 0;

	switch (Index) {

		// shRectH  
		//----------------------------------------------------------------------
	case 0 :
		//CutLine 발생 or CutLine 미발생 분류 Defect 시퀀스 구현 
		if(bCutLine){ //CutLine 발생 
			//Masing 미사용 Dark Defect 
			if(LeftOutEdgeCount!=0){
				LTCirDeftectCnt = GetPolygonSize(LeftInPoint, LTCirDefectPoint, LeftInEdgeCount, 100, 10, 0, LeftCutThresHold);// Dark Defect 
			}

			//White_Masking 사용 Dark Defect 검출
			if(LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
				MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskDarkDefectImg); //Mask Image Copy 

				HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
				HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

				//MaskPoint Init 
				if(MaskPoint!=NULL){delete []MaskPoint;}

				// Clipper Use Parameter 
				The::Path  Poly_In;
				The::Paths Poly_Out;
				The::ClipperOffset ClipOffset;
				Poly_In.reserve (LeftInEdgeCount + 3);
				Poly_Out.reserve(LeftInEdgeCount + 3);

				Poly_In.push_back(The::IntPoint(LeftInPoint[0].x, FLTPoint.y));                //폐곡선 시작점 
				for (int k = 0; k < LeftInEdgeCount; k++) {Poly_In.push_back(The::IntPoint(LeftInPoint[k].x, LeftInPoint[k].y));}
				Poly_In.push_back(The::IntPoint(FLTPoint.x, LeftInPoint[LeftInEdgeCount-1].y));
				Poly_In.push_back(The::IntPoint(FLTPoint.x, FLTPoint.y                      ));//폐곡선 끝점

				ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
				ClipOffset.Execute(Poly_Out, MaskDefectOffset);
				PolyOutCnt = Poly_Out[0].size();

				MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
				MaskPoint = new POINT[MaskCnt];

				for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
					if(k>=Poly_Out[0].size()) break;
					MaskPoint[k].x = 0;
					MaskPoint[k].y = 0;
					MaskPoint[k].x = Poly_Out[0][k].x;
					MaskPoint[k].y = Poly_Out[0][k].y;

					if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
					if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
					if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
					if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
				}

				HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
				::FillRgn(hdc, Rgn, fillBrh);
				::DeleteObject(Rgn);
				::DeleteObject(fillBrh);

				Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);

				if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
					EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, MaskDefectThresHold);

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&BinaryImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = BinaryImg.GetImagePtr();

					// 검사 ROI설정 
					ROI_X =(int)(FLTPoint.x+SideMargin);
					ROI_Y =(int)(FLTPoint.y           );
					ROI_W =(int)(MaxDx - FLTPoint.x   );
					ROI_H =(int)(MaxDy - FLTPoint.y   );

					DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
					if((ROI_X) <=0                ||(ROI_Y)<0            || ROI_W <=0         || (ROI_H)<=0          ) return false;
					if((ROI_X) >=FrameImgW        ||(ROI_Y)>TopLimitLine || ROI_W >=FrameImgW || (ROI_H)>TopLimitLine) return false;
					if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)>FrameImgH)                                         return false;

							// Blob 검사 진행 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

					if (BlobCount != 0) {
						PreCompareCnt = 0;
						for (int k = 0; k < BlobCount; k++) {
							if (LTCirDeftectCnt >= MAX_DEFECT_POINT) break;
							DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
							DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
							DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
							DefectRT.y = (float)(DefectLT.y);
							DefectLB.x = (float)(DefectLT.x);
							DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
							DefectRB.x = (float)(DefectRT.x);
							DefectRB.y = (float)(DefectLB.y);

							for (int i =PreCompareCnt; i <LeftInEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
								LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
								IsDefect = false;
								LTDis = Math.GetLength(LeftInPoint[i], DefectLT);
								RTDis = Math.GetLength(LeftInPoint[i], DefectRT);
								LBDis = Math.GetLength(LeftInPoint[i], DefectLB);
								RBDis = Math.GetLength(LeftInPoint[i], DefectRB);
								if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
									IsDefect = true;
									PreCompareCnt = i;
									break;
								}
							}

							if (IsDefect) {
								LTCirDefectPoint[LTCirDeftectCnt].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
								LTCirDefectPoint[LTCirDeftectCnt].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
								LTCirDefectPoint[LTCirDeftectCnt].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirDefectPoint[LTCirDeftectCnt].Width / 2) + ROI_X);
								LTCirDefectPoint[LTCirDeftectCnt].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/ 2) + ROI_Y);
								LTCirDefectPoint[LTCirDeftectCnt].Rect.right = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left + LTCirDefectPoint[LTCirDeftectCnt].Width );
								LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom= (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top  + LTCirDefectPoint[LTCirDeftectCnt].Height);

								PixelCount = 0;
								PixelValue = 0;
								// 내부 Pixel Data 축출 
								for (int n = LTCirDefectPoint[LTCirDeftectCnt].Rect.top + 1; n < LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom - 1; n++) {
									for (int m = LTCirDefectPoint[LTCirDeftectCnt].Rect.left + 1; m < LTCirDefectPoint[LTCirDeftectCnt].Rect.right - 1; m++) {
										PixelValue = IElement(m, n);
										if (PixelValue < MaskDefectThresHold) PixelCount++;
									}
								}
								// Display 데이터 필요함 
								LTCirDefectPoint[LTCirDeftectCnt].Round = PixelCount;
								LTCirDefectPoint[LTCirDeftectCnt].P.x   = (float)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left);
								LTCirDefectPoint[LTCirDeftectCnt].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height / 2) + ROI_Y + ImageHeight);
								LTCirDefectPoint[LTCirDeftectCnt].Width = LTCirDefectPoint[LTCirDeftectCnt].Width + SideMargin;

								// Merge 위해서 데이터 필요함 
								LTCirDefectPoint[LTCirDeftectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height / 2) + ROI_Y + ImageHeight);
								LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);
								LTCirDeftectCnt++;
							}
						}
					}
					BlobCount = 0;
				}
			}


			//Black_Masking 사용 Bright Defect 검출
			if(LeftInEdgeCount!=0 && MaskBrokenThresHold!=0){
				MaskBrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskBrightDefectImg); //Mask Image Copy 

				HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black Brush 
				HDC hdc = Easy::OpenImageGraphicContext(&MaskBrightDefectImg);

				//MaskPoint Init 
				if(MaskPoint!=NULL){delete []MaskPoint;}

				// Clipper Use Parameter 
				The::Path  Poly_In;
				The::Paths Poly_Out;
				The::ClipperOffset ClipOffset;
				Poly_In.reserve (LeftInEdgeCount + 3);
				Poly_Out.reserve(LeftInEdgeCount + 3);

				Poly_In.push_back(The::IntPoint(LeftInPoint[0].x, FLTPoint.y));               //폐곡선 시작점 
				for (int k = 0; k < LeftInEdgeCount; k++) {Poly_In.push_back(The::IntPoint(LeftInPoint[k].x, LeftInPoint[k].y));}
				Poly_In.push_back(The::IntPoint(FLTPoint.x, LeftInPoint[LeftInEdgeCount-1].y));
				Poly_In.push_back(The::IntPoint(FLTPoint.x, FLTPoint.y                      ));//폐곡선 끝점

				ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
				ClipOffset.Execute(Poly_Out, MaskDefectOffset);
				PolyOutCnt = Poly_Out[0].size();

				MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
				MaskPoint = new POINT[MaskCnt];

				for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
					if(k>=Poly_Out[0].size()) break;
					MaskPoint[k].x = 0;
					MaskPoint[k].y = 0;
					MaskPoint[k].x = Poly_Out[0][k].x;
					MaskPoint[k].y = Poly_Out[0][k].y;

					if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
					if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
					if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
					if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
				}

				HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
				::FillRgn(hdc, Rgn, fillBrh);
				::DeleteObject(Rgn);
				::DeleteObject(fillBrh);

				Easy::CloseImageGraphicContext(&MaskBrightDefectImg, hdc);

				if(MaskBrightDefectImg.GetImagePtr()){ //Dark Defect 검출 
					EasyImage::Threshold(&MaskBrightDefectImg, &BinaryImg, MaskBrokenThresHold);

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&BinaryImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
					IAddress = BinaryImg.GetImagePtr();

					// 검사 ROI설정 
					ROI_X =(int)(FLTPoint.x+SideMargin);
					ROI_Y =(int)(FLTPoint.y           );
					ROI_W =(int)(MaxDx - FLTPoint.x   );
					ROI_H =(int)(MaxDy - FLTPoint.y   );

					DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
					if((ROI_X) <=0                ||(ROI_Y)< 0           || ROI_W <=0         || (ROI_H)<=0          ) return false;
					if((ROI_X) >=FrameImgW        ||(ROI_Y)>TopLimitLine || ROI_W >=FrameImgW || (ROI_H)>TopLimitLine) return false;
					if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)>FrameImgH)                                         return false;

							// Blob 검사 진행 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

					if (BlobCount != 0) {
						PreCompareCnt = 0;
						for (int k = 0; k < BlobCount; k++) {
							if (LTCirDeftectCnt >= MAX_DEFECT_POINT) break;
							DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
							DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
							DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
							DefectRT.y = (float)(DefectLT.y);
							DefectLB.x = (float)(DefectLT.x);
							DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
							DefectRB.x = (float)(DefectRT.x);
							DefectRB.y = (float)(DefectLB.y);

							for (int i =PreCompareCnt; i <LeftInEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
								LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
								IsDefect = false;
								LTDis = Math.GetLength(LeftInPoint[i], DefectLT);
								RTDis = Math.GetLength(LeftInPoint[i], DefectRT);
								LBDis = Math.GetLength(LeftInPoint[i], DefectLB);
								RBDis = Math.GetLength(LeftInPoint[i], DefectRB);
								if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
									IsDefect = true;
									PreCompareCnt = i;
									break;
								}
							}

							if (IsDefect) {
								LTCirCrackPoint[LTCirCrackCnt].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
								LTCirCrackPoint[LTCirCrackCnt].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
								LTCirCrackPoint[LTCirCrackCnt].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirCrackPoint[LTCirCrackCnt].Width / 2) + ROI_X);
								LTCirCrackPoint[LTCirCrackCnt].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/ 2) + ROI_Y);
								LTCirCrackPoint[LTCirCrackCnt].Rect.right = (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.left + LTCirCrackPoint[LTCirCrackCnt].Width );
								LTCirCrackPoint[LTCirCrackCnt].Rect.bottom= (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.top  + LTCirCrackPoint[LTCirCrackCnt].Height);

								PixelCount = 0;
								PixelValue = 0;
								// 내부 Pixel Data 축출 
								for (int n = LTCirCrackPoint[LTCirCrackCnt].Rect.top + 1; n < LTCirCrackPoint[LTCirCrackCnt].Rect.bottom - 1; n++) {
									for (int m = LTCirCrackPoint[LTCirCrackCnt].Rect.left + 1; m < LTCirCrackPoint[LTCirCrackCnt].Rect.right - 1; m++) {
										PixelValue = IElement(m, n);
										if (PixelValue > MaskBrokenThresHold) PixelCount++;
									}
								}
								// Display 데이터 필요함 
								LTCirCrackPoint[LTCirCrackCnt].Round = PixelCount;
								LTCirCrackPoint[LTCirCrackCnt].P.x   = (float)(LTCirCrackPoint[LTCirCrackCnt].Rect.left);
								LTCirCrackPoint[LTCirCrackCnt].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height / 2) + ROI_Y + ImageHeight);
								LTCirCrackPoint[LTCirCrackCnt].Width = LTCirDefectPoint[LTCirDeftectCnt].Width + SideMargin;

								// Merge 위해서 데이터 필요함 
								LTCirCrackPoint[LTCirCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height / 2) + ROI_Y + ImageHeight);
								LTCirCrackPoint[LTCirCrackCnt].Rect.bottom = (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.top + LTCirCrackPoint[LTCirCrackCnt].Height);
								LTCirCrackCnt++;
							}
						}
					}
					BlobCount = 0;
				}
			}
		}
		else        { //CutLine 미발생
		    //Shape 형태에 따라서 기본 검사 + Masking DarkDefect + Masking BrightDefect 
			if     (IsRectTop  ){
				// Image Conversion Binary_Image
				EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);
//				GImage->Save("D:\\ROTATE_IMG\\신규_HAP_ORG.bmp");

				// Image Euresys Algorithm Attach 
				DefectROI.Attach(&DefectImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
				IAddress = DefectImg.GetImagePtr();

				// 검사 ROI설정 
				ROI_X = (int)(FLTPoint.x + SideMargin   );
				ROI_Y = (int)(FLTPoint.y + SideMargin   );
				ROI_W = (int)(SideMargin + PrintScOffset);
				ROI_H = (int)(FrameImgH - ROI_Y         );

				// 검사 시퀀스 
				DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
				if((ROI_X) <=0               ||(ROI_Y)< 0                  || ROI_W<=0         || (ROI_H)<=0          ) return false;
				if((ROI_X) >=FrameImgW       ||(ROI_Y)>TopLimitLine        || ROI_W>=FrameImgW || (ROI_H)>TopLimitLine) return false;
				if((ROI_X+ROI_W) >=FrameImgW ||(ROI_Y+ ROI_H)>FrameImgH)                                                return false;

				// Blob 검사 진행 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);

				// Blob 필터링 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(DCount>=MAX_DEFECT_POINT) break;
						DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +ROI_X);
						DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +ROI_Y);
						DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
						DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

						PixelCount = 0;
						PixelValue = 0;
						// 내부 Pixel Data 축출 
						for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
							for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < DarkThresHold) PixelCount++;
							}
						}
						// Display 데이터 필요함 
						DDefectPoint[DCount].Round = PixelCount;
						DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
						DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight );
						DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

						// Merge 위해서 데이터 필요함 
						DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight);
						DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
						DCount++;
					}
				}
				BlobCount =0;

				//White Masking Dark Defect 
				if(FLTPoint.x!=0.0f && FLTPoint.y!=0.0f && MaskDefectThresHold!=0){
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskDarkDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
//					Poly_In.reserve (LeftInEdgeCount + 3);
//					Poly_Out.reserve(LeftInEdgeCount + 3);

					Poly_In.push_back(The::IntPoint(FLTPoint.x -Margin, FLTPoint.y)); 
					Poly_In.push_back(The::IntPoint(FLTPoint.x        , FLTPoint.y));
					Poly_In.push_back(The::IntPoint(FLTPoint.x        , FrameImgH ));
					Poly_In.push_back(The::IntPoint(FLTPoint.x -Margin, FrameImgH ));

					//Masking Line Attch 체크 
					StaMaskPos.x = FLTPoint.x   ; StaMaskPos.y = FLTPoint.y;
					EndMaskPos.x = Line[0].FP1.x; EndMaskPos.y = FrameImgH ;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, MaskDefectThresHold);
//						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\신규_HAP_MASKING.bmp");

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X =(int)(FLTPoint.x+SideMargin);
						ROI_Y =(int)(FLTPoint.y+SideMargin);
						ROI_W =(int)(PrintScOffset        );
						ROI_H =(int)(FrameImgH - ROI_Y    );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0           || ROI_W <=0         || (ROI_H)<=0          ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)>TopLimitLine || ROI_W >=FrameImgW || (ROI_H)>TopLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)>TopLimitLine)                                      return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							for (int k = 0; k < BlobCount; k++) {
								if (DCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								// 실제 비선형 연마라인 붙어있는 불량인지 확인 
								LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
								IsDefect = false;
								LTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLT);
								RTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRT);
								LBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLB);
								RBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRB);
								if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
									IsDefect = true;
								}

								if (IsDefect) {
									DDefectPoint[DCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DDefectPoint[DCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DDefectPoint[DCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + ROI_X);
									DDefectPoint[DCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/ 2) + ROI_Y);
									DDefectPoint[DCount].Rect.right = (long)(DDefectPoint[DCount].Rect.left + DDefectPoint[DCount].Width );
									DDefectPoint[DCount].Rect.bottom= (long)(DDefectPoint[DCount].Rect.top  + DDefectPoint[DCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DDefectPoint[DCount].Rect.top + 1; n < DDefectPoint[DCount].Rect.bottom - 1; n++) {
										for (int m = DDefectPoint[DCount].Rect.left + 1; m < DDefectPoint[DCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue < MaskDefectThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DDefectPoint[DCount].Round = PixelCount;
									DDefectPoint[DCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
									DDefectPoint[DCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Width = DDefectPoint[DCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
									DCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}

				//Black Masking Bright Defect 
				if(FLTPoint.x!=0.0f && FLTPoint.y!=0.0f && MaskBrokenThresHold!=0){
					MaskBrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskBrightDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskBrightDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
//					Poly_In.reserve (LeftInEdgeCount + 3);
//					Poly_Out.reserve(LeftInEdgeCount + 3);

					Poly_In.push_back(The::IntPoint(FLTPoint.x -Margin, FLTPoint.y)); 
					Poly_In.push_back(The::IntPoint(FLTPoint.x        , FLTPoint.y));
					Poly_In.push_back(The::IntPoint(FLTPoint.x        , FrameImgH ));
					Poly_In.push_back(The::IntPoint(FLTPoint.x -Margin, FrameImgH ));

					//Masking Line Attch 체크 
					StaMaskPos.x = FLTPoint.x   ; StaMaskPos.y = FLTPoint.y;
					EndMaskPos.x = Line[0].FP1.x; EndMaskPos.y = FrameImgH ;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskBrightDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskBrightDefectImg, &BinaryImg, MaskBrokenThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X =(int)(FLTPoint.x+SideMargin);
						ROI_Y =(int)(FLTPoint.y+SideMargin);
						ROI_W =(int)(PrintScOffset        );
						ROI_H =(int)(FrameImgH - ROI_Y    );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0           || ROI_W <=0         || (ROI_H)<=0          ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)>TopLimitLine || ROI_W >=FrameImgW || (ROI_H)>TopLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)>TopLimitLine)                                      return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							for (int k = 0; k < BlobCount; k++) {
								if (DCrackCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								// 실제 비선형 연마라인 붙어있는 불량인지 확인 
								LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
								IsDefect = false;
								LTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLT);
								RTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRT);
								LBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLB);
								RBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRB);
								if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
									IsDefect = true;
								}

								if (IsDefect) {
									DCrackPoint[DCrackCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DCrackPoint[DCrackCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DCrackPoint[DCrackCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width / 2) + ROI_X);
									DCrackPoint[DCrackCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/ 2) + ROI_Y);
									DCrackPoint[DCrackCount].Rect.right = (long)(DCrackPoint[DCrackCount].Rect.left + DCrackPoint[DCrackCount].Width );
									DCrackPoint[DCrackCount].Rect.bottom= (long)(DCrackPoint[DCrackCount].Rect.top  + DCrackPoint[DCrackCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DCrackPoint[DCrackCount].Rect.top + 1; n < DCrackPoint[DCrackCount].Rect.bottom - 1; n++) {
										for (int m = DCrackPoint[DCrackCount].Rect.left + 1; m < DCrackPoint[DCrackCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue > MaskBrokenThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DCrackPoint[DCrackCount].Round = PixelCount;
									DCrackPoint[DCrackCount].P.x   = (float)(DCrackPoint[DCrackCount].Rect.left);
									DCrackPoint[DCrackCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Width = DCrackPoint[DCrackCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DCrackPoint[DCrackCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Rect.bottom = (long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
									DCrackCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}
			}
			else if(IsCornerTop){
			    if(LTCorOutEdgeCount!=0){ //No Masking Corner영역 Dark Defect 
					CornerCount = 0;
					for(int k=0; k<LTCorOutEdgeCount; k++){ //Chamber Cut 영역 기본 Dark Defect 검사
						if(LTCorOutPoint[k].y<FLTTPoint.y) continue;
						if(LTCorOutPoint[k].y>FLTLPoint.y) continue;
						Dis = Math.GetLengthFrLineToPoint(FLTLPoint, FLTTPoint, LTCorOutPoint[k]);
						LTCorner[CornerCount].Dv  = Dis;
						LTCorner[CornerCount].P.x = LTCorOutPoint[k].x;
						LTCorner[CornerCount].P.y = LTCorOutPoint[k].y;
						CornerCount++;
					}
					if (LTCornerWidth > (LTCornerHeight * 2)) LTCorDefectCnt = GetSize(LTCorner, LTCorDefectPoint, CornerCount, 10);
					else                                      LTCorDefectCnt = GetSize(LTCorner, LTCorDefectPoint, CornerCount, 5 );
				}

				//No Masking 직선라인 기본 Dark Defect 검사 
				// Image Conversion Binary_Image
				EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

				// Image Euresys Algorithm Attach 
				DefectROI.Attach(&DefectImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
				IAddress = DefectImg.GetImagePtr();

				// 검사 ROI설정 
				ROI_X = (int)(FLTLPoint.x + SideMargin   );
				ROI_Y = (int)(FLTLPoint.y + SideMargin   );
				ROI_W = (int)(SideMargin + PrintScOffset );
				ROI_H = (int)(FrameImgH - ROI_Y          );

				// 검사 시퀀스 
				DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
				if((ROI_X) <=0               ||(ROI_Y)< 0                  || ROI_W<=0         || (ROI_H)<=0          ) return false;
				if((ROI_X) >=FrameImgW       ||(ROI_Y)>TopLimitLine        || ROI_W>=FrameImgW || (ROI_H)>TopLimitLine) return false;
				if((ROI_X+ROI_W) >=FrameImgW ||(ROI_Y+ ROI_H)>TopLimitLine)                                             return false;

				// Blob 검사 진행 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);

				// Blob 필터링 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(DCount>=MAX_DEFECT_POINT) break;
						DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +ROI_X);
						DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +ROI_Y);
						DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
						DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

						PixelCount = 0;
						PixelValue = 0;
						// 내부 Pixel Data 축출 
						for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
							for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < DarkThresHold) PixelCount++;
							}
						}
						// Display 데이터 필요함 
						DDefectPoint[DCount].Round = PixelCount;
						DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
						DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ROI_Y+ImageHeight );
						DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

						// Merge 위해서 데이터 필요함 
						DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight);
						DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
						DCount++;
					}
				}
				BlobCount =0;
				

				//White_Masking Dark Defect 
				if(FLTPoint.x!=0.0f && FLTPoint.y!=0.0f && LTCorOutEdgeCount!=0 && MaskDefectThresHold!=0){
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskDarkDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
//					Poly_In.reserve (LeftInEdgeCount + 3);
//					Poly_Out.reserve(LeftInEdgeCount + 3);

					Poly_In.push_back(The::IntPoint(FLTPoint.x -Margin  , FLTPoint.y )); 
					Poly_In.push_back(The::IntPoint(FLTTPoint.x         , FLTTPoint.y));
					for(int k=0; k<LTCorOutEdgeCount; k++){Poly_In.push_back(The::IntPoint(LTCorOutPoint[k].x, LTCorOutPoint[k].y));}
					Poly_In.push_back(The::IntPoint(FLTLPoint.x         , FLTLPoint.y));
					Poly_In.push_back(The::IntPoint(Line[0].FP1.x       , FrameImgH  ));
					Poly_In.push_back(The::IntPoint(Line[0].FP1.x-Margin, FrameImgH  ));

					//Masking Line Attch 체크 
					StaMaskPos.x = FLTPoint.x   ; StaMaskPos.y = FLTPoint.y;
					EndMaskPos.x = Line[0].FP1.x; EndMaskPos.y = FrameImgH ;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, MaskDefectThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X =(int)(FLTPoint.x+SideMargin);
						ROI_Y =(int)(FLTPoint.y           );
						ROI_W =(int)(LTCornerWidth        );
						ROI_H =(int)(FrameImgH - ROI_Y    );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0           || ROI_W <=0         || (ROI_H)<=0          ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)>TopLimitLine || ROI_W >=FrameImgW || (ROI_H)>TopLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                        return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							PreCompareCnt = 0;
							for (int k = 0; k < BlobCount; k++) {
								if (DCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								//Chamber Cut Line 붙어있는 불량인지 확인 
								IsDefect = false;
								for (int i = PreCompareCnt; i < LTCorOutEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLength(LTCorOutPoint[i], DefectLT);
									RTDis = Math.GetLength(LTCorOutPoint[i], DefectRT);
									LBDis = Math.GetLength(LTCorOutPoint[i], DefectLB);
									RBDis = Math.GetLength(LTCorOutPoint[i], DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
										PreCompareCnt = i;
										break;
									}
								}

								//직선 Line 붙어있는 불량인지 확인 
								if(!IsDefect){
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLT);
									RTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRT);
									LBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLB);
									RBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
									}
								}

								if (IsDefect) {
									DDefectPoint[DCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DDefectPoint[DCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DDefectPoint[DCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + ROI_X);
									DDefectPoint[DCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/ 2) + ROI_Y);
									DDefectPoint[DCount].Rect.right = (long)(DDefectPoint[DCount].Rect.left + DDefectPoint[DCount].Width );
									DDefectPoint[DCount].Rect.bottom= (long)(DDefectPoint[DCount].Rect.top  + DDefectPoint[DCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DDefectPoint[DCount].Rect.top + 1; n < DDefectPoint[DCount].Rect.bottom - 1; n++) {
										for (int m = DDefectPoint[DCount].Rect.left + 1; m < DDefectPoint[DCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue < MaskDefectThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DDefectPoint[DCount].Round = PixelCount;
									DDefectPoint[DCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
									DDefectPoint[DCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Width = DDefectPoint[DCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
									DCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}



				//Black_Masking Bright Defect 
				if(FLTPoint.x!=0.0f && FLTPoint.y!=0.0f && LTCorOutEdgeCount!=0 && MaskBrokenThresHold!=0){
					MaskBrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskBrightDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskBrightDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
//					Poly_In.reserve (LeftInEdgeCount + 3);
//					Poly_Out.reserve(LeftInEdgeCount + 3);

					Poly_In.push_back(The::IntPoint(FLTPoint.x -Margin  , FLTPoint.y )); 
					Poly_In.push_back(The::IntPoint(FLTTPoint.x         , FLTTPoint.y));
					for(int k=0; k<LTCorOutEdgeCount; k++){Poly_In.push_back(The::IntPoint(LTCorOutPoint[k].x, LTCorOutPoint[k].y));}
					Poly_In.push_back(The::IntPoint(FLTLPoint.x         , FLTLPoint.y));
					Poly_In.push_back(The::IntPoint(Line[0].FP1.x       , FrameImgH  ));
					Poly_In.push_back(The::IntPoint(Line[0].FP1.x-Margin, FrameImgH  ));

					//Masking Line Attch 체크 
					StaMaskPos.x = FLTPoint.x   ; StaMaskPos.y = FLTPoint.y;
					EndMaskPos.x = Line[0].FP1.x; EndMaskPos.y = FrameImgH ;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskBrightDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskBrightDefectImg, &BinaryImg, MaskBrokenThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X =(int)(FLTPoint.x+SideMargin);
						ROI_Y =(int)(FLTPoint.y           );
						ROI_W =(int)(LTCornerWidth        );
						ROI_H =(int)(FrameImgH - ROI_Y    );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0           || ROI_W <=0         || (ROI_H)<=0          ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)>TopLimitLine || ROI_W >=FrameImgW || (ROI_H)>TopLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                        return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							PreCompareCnt = 0;
							for (int k = 0; k < BlobCount; k++) {
								if (DCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								//Chamber Cut Line 붙어있는 불량인지 확인 
								IsDefect = false;
								for (int i = PreCompareCnt; i < LTCorOutEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLength(LTCorOutPoint[i], DefectLT);
									RTDis = Math.GetLength(LTCorOutPoint[i], DefectRT);
									LBDis = Math.GetLength(LTCorOutPoint[i], DefectLB);
									RBDis = Math.GetLength(LTCorOutPoint[i], DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
										PreCompareCnt = i;
										break;
									}
								}

								//직선 Line 붙어있는 불량인지 확인 
								if (!IsDefect) {
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLT);
									RTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRT);
									LBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLB);
									RBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
									}
								}

								if (IsDefect) {
									DCrackPoint[DCrackCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DCrackPoint[DCrackCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DCrackPoint[DCrackCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width / 2) + ROI_X);
									DCrackPoint[DCrackCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/ 2) + ROI_Y);
									DCrackPoint[DCrackCount].Rect.right = (long)(DCrackPoint[DCrackCount].Rect.left + DCrackPoint[DCrackCount].Width );
									DCrackPoint[DCrackCount].Rect.bottom= (long)(DCrackPoint[DCrackCount].Rect.top  + DCrackPoint[DCrackCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DCrackPoint[DCrackCount].Rect.top + 1; n < DCrackPoint[DCrackCount].Rect.bottom - 1; n++) {
										for (int m = DCrackPoint[DCrackCount].Rect.left + 1; m < DCrackPoint[DCrackCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue > MaskBrokenThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DCrackPoint[DCrackCount].Round = PixelCount;
									DCrackPoint[DCrackCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
									DCrackPoint[DCrackCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Width = DCrackPoint[DCrackCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DCrackPoint[DCrackCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Rect.bottom = (long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
									DCrackCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}
			}
			else if(IsCircleTop){
			    if(LTCirOutEdgeCount!=0){ //No Masking Circle영역 Dark Defect 
					LTCirDeftectCnt = GetPolygonSize(LTCirOutPoint, LTCirDefectPoint, LTCirOutEdgeCount, 100, 10, 0, CircleThresHold); //DarkDefect
				}

				//No Masking 직선라인 기본 Dark Defect 검사 
				// Image Conversion Binary_Image
				EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

				// Image Euresys Algorithm Attach 
				DefectROI.Attach(&DefectImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
				IAddress = DefectImg.GetImagePtr();

				// 검사 ROI설정 
				ROI_X = (int)(FLTPoint.x + SideMargin                          );
				ROI_Y = (int)(LTCirOutPoint[LTCirOutEdgeCount-1].y + SideMargin);
				ROI_W = (int)(SideMargin + PrintScOffset                       );
				ROI_H = (int)(FrameImgH - ROI_Y                                );

				// 검사 시퀀스 
				DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
				if((ROI_X) <=0               ||(ROI_Y)< 0                  || ROI_W<=0         || (ROI_H)<=0          ) return false;
				if((ROI_X) >=FrameImgW       ||(ROI_Y)>TopLimitLine        || ROI_W>=FrameImgW || (ROI_H)>TopLimitLine) return false;
				if((ROI_X+ROI_W) >=FrameImgW ||(ROI_Y+ ROI_H)> FrameImgH)                                               return false;
				  
				// Blob 검사 진행 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);

				// Blob 필터링 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(DCount>=MAX_DEFECT_POINT) break;
						DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +ROI_Y);
						DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +ROI_Y);
						DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
						DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

						PixelCount = 0;
						PixelValue = 0;
						// 내부 Pixel Data 축출 
						for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
							for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < DarkThresHold) PixelCount++;
							}
						}
						// Display 데이터 필요함 
						DDefectPoint[DCount].Round = PixelCount;
						DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
						DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight );
						DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

						// Merge 위해서 데이터 필요함 
						DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight);
						DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
						DCount++;
					}
				}
				BlobCount =0;


				//White_Masking Dark Defect 
				if(FLTPoint.x!=0.0f && FLTPoint.y!=0.0f && LTCirOutEdgeCount!=0 && MaskDefectThresHold!=0){
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskDarkDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LTCirOutEdgeCount + 4);
					Poly_Out.reserve(LTCirOutEdgeCount + 4);

					Poly_In.push_back(The::IntPoint(FLTPoint.x -Margin  , FLTPoint.y)); 
					Poly_In.push_back(The::IntPoint(LTCirOutPoint[0].x  , FLTPoint.y));
					for(int k=0; k<LTCirOutEdgeCount; k++){Poly_In.push_back(The::IntPoint(LTCirOutPoint[k].x, LTCirOutPoint[k].y));}
					Poly_In.push_back(The::IntPoint(Line[0].FP1.x       , FrameImgH  ));
					Poly_In.push_back(The::IntPoint(Line[0].FP1.x-Margin, FrameImgH  ));

					//Masking Line Attch 체크 
					StaMaskPos.x = FLTPoint.x   ; StaMaskPos.y = FLTPoint.y;
					EndMaskPos.x = Line[0].FP1.x; EndMaskPos.y = FrameImgH ;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, MaskDefectThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X =(int)(FLTPoint.x+SideMargin);
						ROI_Y =(int)(FLTPoint.y           );
						ROI_W =(int)(LTCircleWidth        );
						ROI_H =(int)(FrameImgH - ROI_Y    );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0           || ROI_W <=0         || (ROI_H)<=0          ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)>TopLimitLine || ROI_W >=FrameImgW || (ROI_H)>TopLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                        return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							PreCompareCnt = 0;
							for (int k = 0; k < BlobCount; k++) {
								if (DCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								//Chamber Cut Line 붙어있는 불량인지 확인 
								IsDefect = false;
								for (int i = PreCompareCnt; i < LTCirOutEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLength(LTCirOutPoint[i], DefectLT);
									RTDis = Math.GetLength(LTCirOutPoint[i], DefectRT);
									LBDis = Math.GetLength(LTCirOutPoint[i], DefectLB);
									RBDis = Math.GetLength(LTCirOutPoint[i], DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
										PreCompareCnt = i;
										break;
									}
								}

								//직선 Line 붙어있는 불량인지 확인 
								if (!IsDefect) {
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLT);
									RTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRT);
									LBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLB);
									RBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
									}
								}

								if (IsDefect) {
									DDefectPoint[DCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DDefectPoint[DCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DDefectPoint[DCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + ROI_X);
									DDefectPoint[DCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/ 2) + ROI_Y);
									DDefectPoint[DCount].Rect.right = (long)(DDefectPoint[DCount].Rect.left + DDefectPoint[DCount].Width );
									DDefectPoint[DCount].Rect.bottom= (long)(DDefectPoint[DCount].Rect.top  + DDefectPoint[DCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DDefectPoint[DCount].Rect.top + 1; n < DDefectPoint[DCount].Rect.bottom - 1; n++) {
										for (int m = DDefectPoint[DCount].Rect.left + 1; m < DDefectPoint[DCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue < MaskDefectThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DDefectPoint[DCount].Round = PixelCount;
									DDefectPoint[DCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
									DDefectPoint[DCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Width = DDefectPoint[DCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
									DCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}

				//Black_Masking Bright Defect 
				if(FLTPoint.x!=0.0f && FLTPoint.y!=0.0f && LTCirOutEdgeCount!=0 && MaskBrokenThresHold!=0){
					MaskBrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskBrightDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskBrightDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LTCirOutEdgeCount + 4);
					Poly_Out.reserve(LTCirOutEdgeCount + 4);

					Poly_In.push_back(The::IntPoint(FLTPoint.x -Margin  , FLTPoint.y)); 
					Poly_In.push_back(The::IntPoint(LTCirOutPoint[0].x  , FLTPoint.y));
					for(int k=0; k<LTCirOutEdgeCount; k++){Poly_In.push_back(The::IntPoint(LTCirOutPoint[k].x, LTCirOutPoint[k].y));}
					Poly_In.push_back(The::IntPoint(Line[0].FP1.x       , FrameImgH  ));
					Poly_In.push_back(The::IntPoint(Line[0].FP1.x-Margin, FrameImgH  ));

					//Masking Line Attch 체크 
					StaMaskPos.x = FLTPoint.x   ; StaMaskPos.y = FLTPoint.y;
					EndMaskPos.x = Line[0].FP1.x; EndMaskPos.y = FrameImgH ;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskBrightDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskBrightDefectImg, &BinaryImg, MaskBrokenThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X =(int)(FLTPoint.x+SideMargin);
						ROI_Y =(int)(FLTPoint.y           );
						ROI_W =(int)(LTCircleWidth        );
						ROI_H =(int)(FrameImgH - ROI_Y    );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0           || ROI_W <=0         || (ROI_H)<=0          ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)>TopLimitLine || ROI_W >=FrameImgW || (ROI_H)>TopLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                        return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							PreCompareCnt = 0;
							for (int k = 0; k < BlobCount; k++) {
								if (DCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								//Chamber Cut Line 붙어있는 불량인지 확인 
								IsDefect = false;
								for (int i = PreCompareCnt; i < LTCirOutEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLength(LTCirOutPoint[i], DefectLT);
									RTDis = Math.GetLength(LTCirOutPoint[i], DefectRT);
									LBDis = Math.GetLength(LTCirOutPoint[i], DefectLB);
									RBDis = Math.GetLength(LTCirOutPoint[i], DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
										PreCompareCnt = i;
										break;
									}
								}

								//직선 Line 붙어있는 불량인지 확인 
								if (!IsDefect) {
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLT);
									RTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRT);
									LBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLB);
									RBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
									}
								}

								if (IsDefect) {
									DCrackPoint[DCrackCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DCrackPoint[DCrackCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DCrackPoint[DCrackCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width / 2) + ROI_X);
									DCrackPoint[DCrackCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/ 2) + ROI_Y);
									DCrackPoint[DCrackCount].Rect.right = (long)(DCrackPoint[DCrackCount].Rect.left + DCrackPoint[DCrackCount].Width );
									DCrackPoint[DCrackCount].Rect.bottom= (long)(DCrackPoint[DCrackCount].Rect.top  + DCrackPoint[DCrackCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DCrackPoint[DCrackCount].Rect.top + 1; n < DCrackPoint[DCrackCount].Rect.bottom - 1; n++) {
										for (int m = DCrackPoint[DCrackCount].Rect.left + 1; m < DCrackPoint[DCrackCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue > MaskBrokenThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DCrackPoint[DCrackCount].Round = PixelCount;
									DCrackPoint[DCrackCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
									DCrackPoint[DCrackCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Width = DCrackPoint[DCrackCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DCrackPoint[DCrackCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Rect.bottom = (long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
									DCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
		Index = 0;
		return true;

		// NOTCH RectC : 젓가락 모양
		//------------------------------------------------------------------
	case 1 :
		//CutLine 발생 or CutLine 미발생 분류 Defect 시퀀스 구현 
		if(bCutLine){ //CutLine 발생 
			//Masing 미사용 Dark Defect 
			if(LeftOutEdgeCount!=0){
				LTCirDeftectCnt = GetPolygonSize(LeftInPoint, LTCirDefectPoint, LeftInEdgeCount, 100, 10, 0, LeftCutThresHold);// Dark Defect 
			}

			LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
			LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);

			//White_Masking 사용 Dark Defect 검출
			if(LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
				MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskDarkDefectImg); //Mask Image Copy 

				HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
				HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

				//MaskPoint Init 
				if(MaskPoint!=NULL){delete []MaskPoint;}

				// Clipper Use Parameter 
				The::Path  Poly_In;
				The::Paths Poly_Out;
				The::ClipperOffset ClipOffset;
				Poly_In.reserve (LeftInEdgeCount + 2);
				Poly_Out.reserve(LeftInEdgeCount + 2);

				Poly_In.push_back(The::IntPoint(LSP.x- SideMargin, 0        ));//폐곡선 시작점 
				for (int k = 0; k < LeftInEdgeCount; k++) {Poly_In.push_back(The::IntPoint(LeftInPoint[k].x, LeftInPoint[k].y));}
				Poly_In.push_back(The::IntPoint(LEP.x- SideMargin, FrameImgH));//폐곡선 끝점 

				ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
				ClipOffset.Execute(Poly_Out, MaskDefectOffset);
				PolyOutCnt = Poly_Out[0].size();

				MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
				MaskPoint = new POINT[MaskCnt];

				for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
					if(k>=Poly_Out[0].size()) break;
					MaskPoint[k].x = 0;
					MaskPoint[k].y = 0;
					MaskPoint[k].x = Poly_Out[0][k].x;
					MaskPoint[k].y = Poly_Out[0][k].y;

					if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
					if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
					if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
					if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
				}

				HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
				::FillRgn(hdc, Rgn, fillBrh);
				::DeleteObject(Rgn);
				::DeleteObject(fillBrh);

				Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);

				if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
					EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, MaskDefectThresHold);

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&BinaryImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = BinaryImg.GetImagePtr();

					// 검사 ROI설정 
					ROI_X = (int)(LSP.x+SideMargin          );
					ROI_Y = (int)(LSP.y                     );
					ROI_W = (int)(SideMargin + PrintScOffset);
					ROI_H = (int)(FrameImgH - ROI_Y         );

					DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
					if((ROI_X) <=0                ||(ROI_Y)< 0         || ROI_W <=0         || (ROI_H)<=0       ) return false;
					if((ROI_X) >=FrameImgW        ||(ROI_Y)> FrameImgH || ROI_W >=FrameImgW || (ROI_H)>FrameImgH) return false;
					if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                   return false;

							// Blob 검사 진행 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

					if (BlobCount != 0) {
						PreCompareCnt = 0;
						for (int k = 0; k < BlobCount; k++) {
							if (LTCirDeftectCnt >= MAX_DEFECT_POINT) break;
							DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
							DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
							DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
							DefectRT.y = (float)(DefectLT.y);
							DefectLB.x = (float)(DefectLT.x);
							DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
							DefectRB.x = (float)(DefectRT.x);
							DefectRB.y = (float)(DefectLB.y);

							for (int i =PreCompareCnt; i <LeftInEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
								LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
								IsDefect = false;
								LTDis = Math.GetLength(LeftInPoint[i], DefectLT);
								RTDis = Math.GetLength(LeftInPoint[i], DefectRT);
								LBDis = Math.GetLength(LeftInPoint[i], DefectLB);
								RBDis = Math.GetLength(LeftInPoint[i], DefectRB);
								if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
									IsDefect = true;
									PreCompareCnt = i;
									break;
								}
							}

							if (IsDefect) {
								LeftDefectPoint[LeftDefectCnt].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
								LeftDefectPoint[LeftDefectCnt].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
								LeftDefectPoint[LeftDefectCnt].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LeftDefectPoint[LeftDefectCnt].Width / 2) + ROI_X);
								LeftDefectPoint[LeftDefectCnt].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height/ 2) + ROI_Y);
								LeftDefectPoint[LeftDefectCnt].Rect.right = (long)(LeftDefectPoint[LeftDefectCnt].Rect.left + LeftDefectPoint[LeftDefectCnt].Width );
								LeftDefectPoint[LeftDefectCnt].Rect.bottom= (long)(LeftDefectPoint[LeftDefectCnt].Rect.top  + LeftDefectPoint[LeftDefectCnt].Height);

								PixelCount = 0;
								PixelValue = 0;
								// 내부 Pixel Data 축출 
								for (int n = LeftDefectPoint[LeftDefectCnt].Rect.top + 1; n < LeftDefectPoint[LeftDefectCnt].Rect.bottom - 1; n++) {
									for (int m = LeftDefectPoint[LeftDefectCnt].Rect.left + 1; m < LeftDefectPoint[LeftDefectCnt].Rect.right - 1; m++) {
										PixelValue = IElement(m, n);
										if (PixelValue < MaskDefectThresHold) PixelCount++;
									}
								}
								// Display 데이터 필요함 
								LeftDefectPoint[LeftDefectCnt].Round = PixelCount;
								LeftDefectPoint[LeftDefectCnt].P.x   = (float)(LeftDefectPoint[LeftDefectCnt].Rect.left);
								LeftDefectPoint[LeftDefectCnt].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height / 2) + ROI_Y + ImageHeight);
								LeftDefectPoint[LeftDefectCnt].Width = LeftDefectPoint[LeftDefectCnt].Width + SideMargin;

								// Merge 위해서 데이터 필요함 
								LeftDefectPoint[LeftDefectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height / 2) + ROI_Y + ImageHeight);
								LeftDefectPoint[LeftDefectCnt].Rect.bottom = (long)(LeftDefectPoint[LeftDefectCnt].Rect.top + LeftDefectPoint[LeftDefectCnt].Height);
								LeftDefectCnt++;
							}
						}
					}
					BlobCount = 0;
				}
			}


			//Black_Masking 사용 Bright Defect 검출
			if(LeftInEdgeCount!=0 && MaskBrokenThresHold!=0){
				MaskBrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskBrightDefectImg); //Mask Image Copy 

				HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black Brush 
				HDC hdc = Easy::OpenImageGraphicContext(&MaskBrightDefectImg);

				//MaskPoint Init 
				if(MaskPoint!=NULL){delete []MaskPoint;}

				// Clipper Use Parameter 
				The::Path  Poly_In;
				The::Paths Poly_Out;
				The::ClipperOffset ClipOffset;
				Poly_In.reserve (LeftInEdgeCount + 2);
				Poly_Out.reserve(LeftInEdgeCount + 2);

				Poly_In.push_back(The::IntPoint(LSP.x- SideMargin, 0        ));//폐곡선 시작점 
				for (int k = 0; k < LeftInEdgeCount; k++) {Poly_In.push_back(The::IntPoint(LeftInPoint[k].x, LeftInPoint[k].y));}
				Poly_In.push_back(The::IntPoint(LEP.x- SideMargin, FrameImgH));//폐곡선 끝점 

				ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
				ClipOffset.Execute(Poly_Out, MaskDefectOffset);
				PolyOutCnt = Poly_Out[0].size();

				MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
				MaskPoint = new POINT[MaskCnt];

				for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
					if(k>=Poly_Out[0].size()) break;
					MaskPoint[k].x = 0;
					MaskPoint[k].y = 0;
					MaskPoint[k].x = Poly_Out[0][k].x;
					MaskPoint[k].y = Poly_Out[0][k].y;

					if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
					if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
					if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
					if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
				}

				HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
				::FillRgn(hdc, Rgn, fillBrh);
				::DeleteObject(Rgn);
				::DeleteObject(fillBrh);

				Easy::CloseImageGraphicContext(&MaskBrightDefectImg, hdc);

				if(MaskBrightDefectImg.GetImagePtr()){ //Dark Defect 검출 
					EasyImage::Threshold(&MaskBrightDefectImg, &BinaryImg, MaskBrokenThresHold);

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&BinaryImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
					IAddress = BinaryImg.GetImagePtr();

					// 검사 ROI설정 
					ROI_X = (int)(LSP.x+SideMargin          );
					ROI_Y = (int)(LSP.y                     );
					ROI_W = (int)(SideMargin + PrintScOffset);
					ROI_H = (int)(FrameImgH - ROI_Y         );

					DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
					if((ROI_X) <=0                ||(ROI_Y)< 0         || ROI_W <=0         || (ROI_H)<=0        ) return false;
					if((ROI_X) >=FrameImgW        ||(ROI_Y)> FrameImgH || ROI_W >=FrameImgW || (ROI_H)> FrameImgH) return false;
					if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                    return false;

							// Blob 검사 진행 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

					if (BlobCount != 0) {
						PreCompareCnt = 0;
						for (int k = 0; k < BlobCount; k++) {
							if (LTCirDeftectCnt >= MAX_DEFECT_POINT) break;
							DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
							DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
							DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
							DefectRT.y = (float)(DefectLT.y);
							DefectLB.x = (float)(DefectLT.x);
							DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
							DefectRB.x = (float)(DefectRT.x);
							DefectRB.y = (float)(DefectLB.y);

							for (int i =PreCompareCnt; i <LeftInEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
								LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
								IsDefect = false;
								LTDis = Math.GetLength(LeftInPoint[i], DefectLT);
								RTDis = Math.GetLength(LeftInPoint[i], DefectRT);
								LBDis = Math.GetLength(LeftInPoint[i], DefectLB);
								RBDis = Math.GetLength(LeftInPoint[i], DefectRB);
								if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
									IsDefect = true;
									PreCompareCnt = i;
									break;
								}
							}

							if (IsDefect) {
								LeftCrackPoint[LeftCrackCnt].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
								LeftCrackPoint[LeftCrackCnt].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
								LeftCrackPoint[LeftCrackCnt].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LeftCrackPoint[LeftCrackCnt].Width / 2) + ROI_X);
								LeftCrackPoint[LeftCrackCnt].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height/ 2) + ROI_Y);
								LeftCrackPoint[LeftCrackCnt].Rect.right = (long)(LeftCrackPoint[LeftCrackCnt].Rect.left + LeftCrackPoint[LeftCrackCnt].Width );
								LeftCrackPoint[LeftCrackCnt].Rect.bottom= (long)(LeftCrackPoint[LeftCrackCnt].Rect.top  + LeftCrackPoint[LeftCrackCnt].Height);

								PixelCount = 0;
								PixelValue = 0;
								// 내부 Pixel Data 축출 
								for (int n = LeftCrackPoint[LeftCrackCnt].Rect.top + 1; n < LeftCrackPoint[LeftCrackCnt].Rect.bottom - 1; n++) {
									for (int m = LeftCrackPoint[LeftCrackCnt].Rect.left + 1; m < LeftCrackPoint[LeftCrackCnt].Rect.right - 1; m++) {
										PixelValue = IElement(m, n);
										if (PixelValue > MaskBrokenThresHold) PixelCount++;
									}
								}
								// Display 데이터 필요함 
								LeftCrackPoint[LeftCrackCnt].Round = PixelCount;
								LeftCrackPoint[LeftCrackCnt].P.x   = (float)(LeftCrackPoint[LeftCrackCnt].Rect.left);
								LeftCrackPoint[LeftCrackCnt].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height / 2) + ROI_Y + ImageHeight);
								LeftCrackPoint[LeftCrackCnt].Width = LeftCrackPoint[LeftCrackCnt].Width + SideMargin;

								// Merge 위해서 데이터 필요함 
								LeftCrackPoint[LeftCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height / 2) + ROI_Y + ImageHeight);
								LeftCrackPoint[LeftCrackCnt].Rect.bottom = (long)(LeftCrackPoint[LeftCrackCnt].Rect.top + LeftCrackPoint[LeftCrackCnt].Height);
								LeftCrackCnt++;
							}
						}
					}
					BlobCount = 0;
				}
			}
		}
		else        { //CutLine 미발생 
			// Image Conversion Binary_Image
			EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);
//			GImage->Save("D:\\ROTATE_IMG\\신규_STATIC_ORG.bmp");

			// Image Euresys Algorithm Attach 
			DefectROI.Attach(&DefectImg);
			CodeImageSelection.SetFeretAngle(0.00f);
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
			IAddress = DefectImg.GetImagePtr();

			LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
			LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);

			// 검사 ROI설정 
			ROI_X = (int)(LSP.x+SideMargin          );
			ROI_Y = (int)(LSP.y                     );
			ROI_W = (int)(SideMargin + PrintScOffset);
			ROI_H = (int)(FrameImgH - ROI_Y         );

			// 검사 시퀀스 
			DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
			if((ROI_X) <=0               ||(ROI_Y)< 0         || ROI_W<=0         || (ROI_H)<=0        ) return false;
			if((ROI_X) >=FrameImgW       ||(ROI_Y)> FrameImgH || ROI_W>=FrameImgW || (ROI_H)> FrameImgH) return false;
			if((ROI_X+ROI_W) >=FrameImgW ||(ROI_Y+ ROI_H)> FrameImgH)                                    return false;

			// Blob 검사 진행 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI,CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);

			// Blob 필터링 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
			if(BlobCount!=0){
				for(int k=0; k<BlobCount; k++){
					if(DCount>=MAX_DEFECT_POINT) break;
					DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
					DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +ROI_X);
					DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +ROI_Y);
					DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
					DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

					PixelCount = 0;
					PixelValue = 0;
					// 내부 Pixel Data 축출 
					for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
						for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
							PixelValue = IElement(m,n);
							if(PixelValue < DarkThresHold) PixelCount++;
						}
					}
					// Display 데이터 필요함 
					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
					DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight );
					DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

					// Merge 위해서 데이터 필요함 
					DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight);
					DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount =0;

			//White Masking Dark Defect 
			if(LSP.x!=0.0f && LEP.x!=0.0f && LEP.y!=0.0f && MaskDefectThresHold!=0){
				MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskDarkDefectImg); //Mask Image Copy 

				HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
				HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

				//MaskPoint Init 
				if(MaskPoint!=NULL){delete []MaskPoint;}

				// Clipper Use Parameter 
				The::Path  Poly_In;
				The::Paths Poly_Out;
				The::ClipperOffset ClipOffset;
				Poly_In.reserve (4);
				Poly_Out.reserve(4);

				Poly_In.push_back(The::IntPoint(LSP.x -Margin , LSP.y     )); 
				Poly_In.push_back(The::IntPoint(LSP.x         , LSP.y     ));
				Poly_In.push_back(The::IntPoint(LEP.x         , LEP.y     ));
				Poly_In.push_back(The::IntPoint(LEP.x - Margin, FrameImgH ));

				//Masking Line Attch 체크 
				StaMaskPos.x = LSP.x; StaMaskPos.y = LSP.y;
				EndMaskPos.x = LEP.x; EndMaskPos.y = LEP.y;

				ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
				ClipOffset.Execute(Poly_Out, MaskDefectOffset);
				PolyOutCnt = Poly_Out[0].size();

				MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
				MaskPoint = new POINT[MaskCnt];

				for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
					if(k>=Poly_Out[0].size()) break;
					MaskPoint[k].x = 0;
					MaskPoint[k].y = 0;
					MaskPoint[k].x = Poly_Out[0][k].x;
					MaskPoint[k].y = Poly_Out[0][k].y;

					if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
					if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
					if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
					if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
				}

				HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
				::FillRgn(hdc, Rgn, fillBrh);
				::DeleteObject(Rgn);
				::DeleteObject(fillBrh);

				Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);

				if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
					EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, MaskDefectThresHold);
//					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\신규_STATIC_MASKING.bmp");

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&BinaryImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = BinaryImg.GetImagePtr();

					// 검사 ROI설정 
					ROI_X = (int)(LSP.x + SideMargin        );
					ROI_Y = (int)(LSP.y                     );
					ROI_W = (int)(SideMargin + PrintScOffset);
					ROI_H = (int)(FrameImgH - ROI_Y         );

					DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
					if((ROI_X) <=0                ||(ROI_Y)< 0         || ROI_W <=0         || (ROI_H)<=0        ) return false;
					if((ROI_X) >=FrameImgW        ||(ROI_Y)> FrameImgH || ROI_W >=FrameImgW || (ROI_H)> FrameImgH) return false;
					if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)>FrameImgH)                                     return false;

					// Blob 검사 진행 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

					if (BlobCount != 0) {
						for (int k = 0; k < BlobCount; k++) {
							if (DCount >= MAX_DEFECT_POINT) break;
							DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
							DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
							DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
							DefectRT.y = (float)(DefectLT.y);
							DefectLB.x = (float)(DefectLT.x);
							DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
							DefectRB.x = (float)(DefectRT.x);
							DefectRB.y = (float)(DefectLB.y);

							// 실제 비선형 연마라인 붙어있는 불량인지 확인 
							LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
							IsDefect = false;
							LTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLT);
							RTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRT);
							LBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLB);
							RBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRB);
							if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
								IsDefect = true;
							}

							if (IsDefect) {
								DDefectPoint[DCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
								DDefectPoint[DCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
								DDefectPoint[DCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + ROI_X);
								DDefectPoint[DCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/ 2) + ROI_Y);
								DDefectPoint[DCount].Rect.right = (long)(DDefectPoint[DCount].Rect.left + DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom= (long)(DDefectPoint[DCount].Rect.top  + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// 내부 Pixel Data 축출 
								for (int n = DDefectPoint[DCount].Rect.top + 1; n < DDefectPoint[DCount].Rect.bottom - 1; n++) {
									for (int m = DDefectPoint[DCount].Rect.left + 1; m < DDefectPoint[DCount].Rect.right - 1; m++) {
										PixelValue = IElement(m, n);
										if (PixelValue < MaskDefectThresHold) PixelCount++;
									}
								}
								// Display 데이터 필요함 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
								DDefectPoint[DCount].Width = DDefectPoint[DCount].Width + SideMargin;

								// Merge 위해서 데이터 필요함 
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
								DCount++;
							}
						}
					}
					BlobCount = 0;
				}
			}

			//Black Masking Bright Defect 
			if(LSP.x!=0.0f && LEP.x!=0.0f && LEP.y!=0.0f &&  MaskBrokenThresHold!=0){
				MaskBrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskBrightDefectImg); //Mask Image Copy 

				HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black Brush 
				HDC hdc = Easy::OpenImageGraphicContext(&MaskBrightDefectImg);

				//MaskPoint Init 
				if(MaskPoint!=NULL){delete []MaskPoint;}

				// Clipper Use Parameter 
				The::Path  Poly_In;
				The::Paths Poly_Out;
				The::ClipperOffset ClipOffset;
				Poly_In.reserve (4);
				Poly_Out.reserve(4);

				Poly_In.push_back(The::IntPoint(LSP.x -Margin , LSP.y     )); 
				Poly_In.push_back(The::IntPoint(LSP.x         , LSP.y     ));
				Poly_In.push_back(The::IntPoint(LEP.x         , LEP.y     ));
				Poly_In.push_back(The::IntPoint(LEP.x - Margin, FrameImgH ));

				//Masking Line Attch 체크 
				StaMaskPos.x = LSP.x; StaMaskPos.y = LSP.y;
				EndMaskPos.x = LEP.x; EndMaskPos.y = LEP.y;

				ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
				ClipOffset.Execute(Poly_Out, MaskDefectOffset);
				PolyOutCnt = Poly_Out[0].size();

				MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
				MaskPoint = new POINT[MaskCnt];

				for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
					if(k>=Poly_Out[0].size()) break;
					MaskPoint[k].x = 0;
					MaskPoint[k].y = 0;
					MaskPoint[k].x = Poly_Out[0][k].x;
					MaskPoint[k].y = Poly_Out[0][k].y;

					if (MaskPoint[k].x > MaxDx      ) MaxDx        = MaskPoint[k].x;
					if (MaskPoint[k].y > MaxDy      ) MaxDy        = MaskPoint[k].y;
					if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
					if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
				}

				HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
				::FillRgn(hdc, Rgn, fillBrh);
				::DeleteObject(Rgn);
				::DeleteObject(fillBrh);

				Easy::CloseImageGraphicContext(&MaskBrightDefectImg, hdc);

				if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
					EasyImage::Threshold(&MaskBrightDefectImg, &BinaryImg, MaskBrokenThresHold);

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&BinaryImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
					IAddress = BinaryImg.GetImagePtr();

					// 검사 ROI설정 
					ROI_X = (int)(LSP.x+SideMargin          );
					ROI_Y = (int)(LSP.y                     );
					ROI_W = (int)(SideMargin + PrintScOffset);
					ROI_H = (int)(FrameImgH - ROI_Y         );

					DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
					if((ROI_X) <=0                ||(ROI_Y)< 0         || ROI_W <=0         || (ROI_H)<=0        ) return false;
					if((ROI_X) >=FrameImgW        ||(ROI_Y)> FrameImgH || ROI_W >=FrameImgW || (ROI_H)> FrameImgH) return false;
					if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                    return false;

					// Blob 검사 진행 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

					if (BlobCount != 0) {
						for (int k = 0; k < BlobCount; k++) {
							if (DCrackCount >= MAX_DEFECT_POINT) break;
							DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
							DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
							DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
							DefectRT.y = (float)(DefectLT.y);
							DefectLB.x = (float)(DefectLT.x);
							DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
							DefectRB.x = (float)(DefectRT.x);
							DefectRB.y = (float)(DefectLB.y);

							// 실제 비선형 연마라인 붙어있는 불량인지 확인 
							LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
							IsDefect = false;
							LTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLT);
							RTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRT);
							LBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLB);
							RBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRB);
							if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
								IsDefect = true;
							}

							if (IsDefect) {
								DCrackPoint[DCrackCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
								DCrackPoint[DCrackCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
								DCrackPoint[DCrackCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width / 2) + ROI_X);
								DCrackPoint[DCrackCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/ 2) + ROI_Y);
								DCrackPoint[DCrackCount].Rect.right = (long)(DCrackPoint[DCrackCount].Rect.left + DCrackPoint[DCrackCount].Width );
								DCrackPoint[DCrackCount].Rect.bottom= (long)(DCrackPoint[DCrackCount].Rect.top  + DCrackPoint[DCrackCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// 내부 Pixel Data 축출 
								for (int n = DCrackPoint[DCrackCount].Rect.top + 1; n < DCrackPoint[DCrackCount].Rect.bottom - 1; n++) {
									for (int m = DCrackPoint[DCrackCount].Rect.left + 1; m < DCrackPoint[DCrackCount].Rect.right - 1; m++) {
										PixelValue = IElement(m, n);
										if (PixelValue > MaskBrokenThresHold) PixelCount++;
									}
								}
								// Display 데이터 필요함 
								DCrackPoint[DCrackCount].Round = PixelCount;
								DCrackPoint[DCrackCount].P.x   = (float)(DCrackPoint[DCrackCount].Rect.left);
								DCrackPoint[DCrackCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
								DCrackPoint[DCrackCount].Width = DCrackPoint[DCrackCount].Width + SideMargin;

								// Merge 위해서 데이터 필요함 
								DCrackPoint[DCrackCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
								DCrackPoint[DCrackCount].Rect.bottom = (long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
								DCrackCount++;
							}
						}
					}
					BlobCount = 0;
				}
			}
		}
		Index = 0;
		return true;

		// NOTCH RectC : 컵모양
		//------------------------------------------------------------------
	case 2 :
		//CutLine 발생 or CutLine 미발생 분류 Defect 시퀀스 구현 
		if(bCutLine){ //CutLine 발생 
			//Masing 미사용 Dark Defect 
			if(LeftOutEdgeCount!=0){
				LBCirDeftectCnt = GetPolygonSize(LeftInPoint, LBCirDefectPoint, LeftInEdgeCount, 100, 10, 0, LeftCutThresHold);// Dark Defect 
			}
			LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);


			//White_Masking 사용 Dark Defect 검출
			if(LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
				MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskDarkDefectImg); //Mask Image Copy 

				HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
				HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

				//MaskPoint Init 
				if(MaskPoint!=NULL){delete []MaskPoint;}

				// Clipper Use Parameter 
				The::Path  Poly_In;
				The::Paths Poly_Out;
				The::ClipperOffset ClipOffset;
				Poly_In.reserve (LeftInEdgeCount + 3);
				Poly_Out.reserve(LeftInEdgeCount + 3);

				Poly_In.push_back(The::IntPoint(FLBPoint.x, 0));                              //폐곡선 시작점 
				for (int k = 0; k < LeftInEdgeCount; k++) {Poly_In.push_back(The::IntPoint(LeftInPoint[k].x, LeftInPoint[k].y));}
				Poly_In.push_back(The::IntPoint(LeftInPoint[LeftInEdgeCount-1].x,FLBPoint.y));
				Poly_In.push_back(The::IntPoint(FLBPoint.x                      ,FLBPoint.y));//폐곡선 끝점

				ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
				ClipOffset.Execute(Poly_Out, MaskDefectOffset);
				PolyOutCnt = Poly_Out[0].size();

				MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
				MaskPoint = new POINT[MaskCnt];

				for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
					if(k>=Poly_Out[0].size()) break;
					MaskPoint[k].x = 0;
					MaskPoint[k].y = 0;
					MaskPoint[k].x = Poly_Out[0][k].x;
					MaskPoint[k].y = Poly_Out[0][k].y;

					if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
					if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
					if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
					if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
				}

				HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
				::FillRgn(hdc, Rgn, fillBrh);
				::DeleteObject(Rgn);
				::DeleteObject(fillBrh);

				Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);

				if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
					EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, MaskDefectThresHold);

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&BinaryImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = BinaryImg.GetImagePtr();

					// 검사 ROI설정 
					ROI_X =(int)(LSP.x+SideMargin);
					ROI_Y =(int)(LSP.y           );
					ROI_W =(int)(MaxDx - LSP.x   );
					ROI_H =(int)(MaxDy - LSP.y   );

					DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
					if((ROI_X) <=0                ||(ROI_Y)< 0           || ROI_W <=0         || (ROI_H)<=0           ) return false;
					if((ROI_X) >=FrameImgW        ||(ROI_Y)> BtmLimitLine|| ROI_W >=FrameImgW || (ROI_H)> BtmLimitLine) return false;
					if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                         return false;

							// Blob 검사 진행 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

					if (BlobCount != 0) {
						PreCompareCnt = 0;
						for (int k = 0; k < BlobCount; k++) {
							if (LTCirDeftectCnt >= MAX_DEFECT_POINT) break;
							DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
							DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
							DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
							DefectRT.y = (float)(DefectLT.y);
							DefectLB.x = (float)(DefectLT.x);
							DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
							DefectRB.x = (float)(DefectRT.x);
							DefectRB.y = (float)(DefectLB.y);

							for (int i =PreCompareCnt; i <LeftInEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
								LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
								IsDefect = false;
								LTDis = Math.GetLength(LeftInPoint[i], DefectLT);
								RTDis = Math.GetLength(LeftInPoint[i], DefectRT);
								LBDis = Math.GetLength(LeftInPoint[i], DefectLB);
								RBDis = Math.GetLength(LeftInPoint[i], DefectRB);
								if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
									IsDefect = true;
									PreCompareCnt = i;
									break;
								}
							}

							if (IsDefect) {
								LBCirDefectPoint[LBCirDeftectCnt].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
								LBCirDefectPoint[LBCirDeftectCnt].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
								LBCirDefectPoint[LBCirDeftectCnt].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirDefectPoint[LBCirDeftectCnt].Width / 2) + ROI_X);
								LBCirDefectPoint[LBCirDeftectCnt].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/ 2) + ROI_Y);
								LBCirDefectPoint[LBCirDeftectCnt].Rect.right = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left + LBCirDefectPoint[LBCirDeftectCnt].Width );
								LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom= (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top  + LBCirDefectPoint[LBCirDeftectCnt].Height);

								PixelCount = 0;
								PixelValue = 0;
								// 내부 Pixel Data 축출 
								for (int n = LBCirDefectPoint[LBCirDeftectCnt].Rect.top + 1; n < LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom - 1; n++) {
									for (int m = LBCirDefectPoint[LBCirDeftectCnt].Rect.left + 1; m < LBCirDefectPoint[LBCirDeftectCnt].Rect.right - 1; m++) {
										PixelValue = IElement(m, n);
										if (PixelValue < MaskDefectThresHold) PixelCount++;
									}
								}
								// Display 데이터 필요함 
								LBCirDefectPoint[LBCirDeftectCnt].Round = PixelCount;
								LBCirDefectPoint[LBCirDeftectCnt].P.x   = (float)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left);
								LBCirDefectPoint[LBCirDeftectCnt].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height / 2) + ROI_Y + ImageHeight);
								LBCirDefectPoint[LBCirDeftectCnt].Width = LBCirDefectPoint[LBCirDeftectCnt].Width + SideMargin;

								// Merge 위해서 데이터 필요함 
								LBCirDefectPoint[LBCirDeftectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height / 2) + ROI_Y + ImageHeight);
								LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);
								LBCirDeftectCnt++;
							}
						}
					}
					BlobCount = 0;
				}
			}


			//Black_Masking 사용 Bright Defect 검출
			if(LeftInEdgeCount!=0 && MaskBrokenThresHold!=0){
				MaskBrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskBrightDefectImg); //Mask Image Copy 

				HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black Brush 
				HDC hdc = Easy::OpenImageGraphicContext(&MaskBrightDefectImg);

				//MaskPoint Init 
				if(MaskPoint!=NULL){delete []MaskPoint;}

				// Clipper Use Parameter 
				The::Path  Poly_In;
				The::Paths Poly_Out;
				The::ClipperOffset ClipOffset;
				Poly_In.reserve (LeftInEdgeCount + 3);
				Poly_Out.reserve(LeftInEdgeCount + 3);

				Poly_In.push_back(The::IntPoint(FLBPoint.x, 0));                              //폐곡선 시작점 
				for (int k = 0; k < LeftInEdgeCount; k++) {Poly_In.push_back(The::IntPoint(LeftInPoint[k].x, LeftInPoint[k].y));}
				Poly_In.push_back(The::IntPoint(LeftInPoint[LeftInEdgeCount-1].x,FLBPoint.y));
				Poly_In.push_back(The::IntPoint(FLBPoint.x                      ,FLBPoint.y));//폐곡선 끝점

				ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
				ClipOffset.Execute(Poly_Out, MaskDefectOffset);
				PolyOutCnt = Poly_Out[0].size();

				MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
				MaskPoint = new POINT[MaskCnt];

				for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
					if(k>=Poly_Out[0].size()) break;
					MaskPoint[k].x = 0;
					MaskPoint[k].y = 0;
					MaskPoint[k].x = Poly_Out[0][k].x;
					MaskPoint[k].y = Poly_Out[0][k].y;

					if (MaskPoint[k].x > MaxDx    ) MaxDx = MaskPoint[k].x    ;
					if (MaskPoint[k].y > MaxDy    ) MaxDy = MaskPoint[k].y    ;
					if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0        ;
					if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH;
				}

				HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
				::FillRgn(hdc, Rgn, fillBrh);
				::DeleteObject(Rgn);
				::DeleteObject(fillBrh);

				Easy::CloseImageGraphicContext(&MaskBrightDefectImg, hdc);

				if(MaskBrightDefectImg.GetImagePtr()){ //Dark Defect 검출 
					EasyImage::Threshold(&MaskBrightDefectImg, &BinaryImg, MaskBrokenThresHold);

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&BinaryImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
					IAddress = BinaryImg.GetImagePtr();

					// 검사 ROI설정 
					ROI_X =(int)(LSP.x+SideMargin);
					ROI_Y =(int)(LSP.y           );
					ROI_W =(int)(MaxDx - LSP.x   );
					ROI_H =(int)(MaxDy - LSP.y   );

					DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
					if((ROI_X) <=0                ||(ROI_Y)< 0            || ROI_W <=0         || (ROI_H)<=0           ) return false;
					if((ROI_X) >=FrameImgW        ||(ROI_Y)> BtmLimitLine || ROI_W >=FrameImgW || (ROI_H)> BtmLimitLine) return false;
					if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                          return false;

				    // Blob 검사 진행 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

					if (BlobCount != 0) {
						PreCompareCnt = 0;
						for (int k = 0; k < BlobCount; k++) {
							if (LTCirDeftectCnt >= MAX_DEFECT_POINT) break;
							DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
							DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
							DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
							DefectRT.y = (float)(DefectLT.y);
							DefectLB.x = (float)(DefectLT.x);
							DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
							DefectRB.x = (float)(DefectRT.x);
							DefectRB.y = (float)(DefectLB.y);

							for (int i =PreCompareCnt; i <LeftInEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
								LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
								IsDefect = false;
								LTDis = Math.GetLength(LeftInPoint[i], DefectLT);
								RTDis = Math.GetLength(LeftInPoint[i], DefectRT);
								LBDis = Math.GetLength(LeftInPoint[i], DefectLB);
								RBDis = Math.GetLength(LeftInPoint[i], DefectRB);
								if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
									IsDefect = true;
									PreCompareCnt = i;
									break;
								}
							}

							if (IsDefect) {
								LBCirCrackPoint[LBCirCrackCnt].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
								LBCirCrackPoint[LBCirCrackCnt].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
								LBCirCrackPoint[LBCirCrackCnt].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirCrackPoint[LBCirCrackCnt].Width / 2) + ROI_X);
								LBCirCrackPoint[LBCirCrackCnt].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height / 2) + ROI_Y);
								LBCirCrackPoint[LBCirCrackCnt].Rect.right = (long)(LBCirCrackPoint[LBCirCrackCnt].Rect.left + LBCirCrackPoint[LBCirCrackCnt].Width);
								LBCirCrackPoint[LBCirCrackCnt].Rect.bottom= (long)(LBCirCrackPoint[LBCirCrackCnt].Rect.top  + LBCirCrackPoint[LBCirCrackCnt].Height);

								PixelCount = 0;
								PixelValue = 0;
								// 내부 Pixel Data 축출 
								for (int n = LBCirCrackPoint[LBCirCrackCnt].Rect.top + 1; n < LBCirCrackPoint[LBCirCrackCnt].Rect.bottom - 1; n++) {
									for (int m = LBCirCrackPoint[LBCirCrackCnt].Rect.left + 1; m < LBCirCrackPoint[LBCirCrackCnt].Rect.right - 1; m++) {
										PixelValue = IElement(m, n);
										if (PixelValue > MaskBrokenThresHold) PixelCount++;
									}
								}
								// Display 데이터 필요함 
								LBCirCrackPoint[LBCirCrackCnt].Round = PixelCount;
								LBCirCrackPoint[LBCirCrackCnt].P.x   = (float)(LBCirCrackPoint[LBCirCrackCnt].Rect.left);
								LBCirCrackPoint[LBCirCrackCnt].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height / 2) + ROI_Y + ImageHeight);
								LBCirCrackPoint[LBCirCrackCnt].Width = LBCirCrackPoint[LBCirCrackCnt].Width + SideMargin;

								// Merge 위해서 데이터 필요함 
								LBCirCrackPoint[LBCirCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height / 2) + ROI_Y + ImageHeight);
								LBCirCrackPoint[LBCirCrackCnt].Rect.bottom = (long)(LBCirCrackPoint[LBCirCrackCnt].Rect.top + LBCirCrackPoint[LBCirCrackCnt].Height);
								LBCirCrackCnt++;
							}
						}
					}
					BlobCount = 0;
				}
			}
		}
		else        { //CutLine 미발생
		    //Shape 형태에 따라서 기본 검사 + Masking DarkDefect + Masking BrightDefect 
			if     (IsRectBtm  ){
				// Image Conversion Binary_Image
				EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);
//				GImage->Save("D:\\ROTATE_IMG\\신규_CUP_ORG.bmp");

				// Image Euresys Algorithm Attach 
				DefectROI.Attach(&DefectImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
				IAddress = DefectImg.GetImagePtr();

				LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);

				// 검사 ROI설정 
				ROI_X = (int)(LSP.x + SideMargin        );
				ROI_Y = (int)(LSP.y                     );
				ROI_W = (int)(SideMargin + PrintScOffset);
				ROI_H = (int)(FLBPoint.y                );

				// 검사 시퀀스 
				DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
				if((ROI_X) <=0               ||(ROI_Y)< 0            || ROI_W<=0         || (ROI_H)<=0           ) return false;
				if((ROI_X) >=FrameImgW       ||(ROI_Y)> BtmLimitLine || ROI_W>=FrameImgW || (ROI_H)> BtmLimitLine) return false;
				if((ROI_X+ROI_W) >=FrameImgW ||(ROI_Y+ ROI_H)> FrameImgH)                                          return false;

				// Blob 검사 진행 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);

				// Blob 필터링 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(DCount>=MAX_DEFECT_POINT) break;
						DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +ROI_X);
						DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +ROI_Y);
						DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
						DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

						PixelCount = 0;
						PixelValue = 0;
						// 내부 Pixel Data 축출 
						for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
							for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < DarkThresHold) PixelCount++;
							}
						}
						// Display 데이터 필요함 
						DDefectPoint[DCount].Round = PixelCount;
						DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
						DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight );
						DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

						// Merge 위해서 데이터 필요함 
						DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight);
						DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
						DCount++;
					}
				}
				BlobCount =0;

				//White Masking Dark Defect 
				if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f && MaskDefectThresHold!=0){
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskDarkDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (4);
					Poly_Out.reserve(4);

					Poly_In.push_back(The::IntPoint(LSP.x -Margin     , LSP.y     ));
					Poly_In.push_back(The::IntPoint(LSP.x             , LSP.y     ));
					Poly_In.push_back(The::IntPoint(FLBPoint.x        , FLBPoint.y));
					Poly_In.push_back(The::IntPoint(FLBPoint.x -Margin, FLBPoint.y));

					//Masking Line Attch 체크 
					StaMaskPos.x = FLTPoint.x   ; StaMaskPos.y = FLTPoint.y;
					EndMaskPos.x = Line[0].FP1.x; EndMaskPos.y = FrameImgH ;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, MaskDefectThresHold);
//						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\신규_CUP_MASKING.bmp");

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X = (int)(LSP.x + SideMargin        );
						ROI_Y = (int)(LSP.y                     );
						ROI_W = (int)(SideMargin + PrintScOffset);
						ROI_H = (int)(FLBPoint.y                );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0            || ROI_W <=0         || (ROI_H)<=0           ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)> BtmLimitLine || ROI_W >=FrameImgW || (ROI_H)> BtmLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                          return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							for (int k = 0; k < BlobCount; k++) {
								if (DCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								// 실제 비선형 연마라인 붙어있는 불량인지 확인 
								LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
								IsDefect = false;
								LTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLT);
								RTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRT);
								LBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLB);
								RBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRB);
								if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
									IsDefect = true;
								}

								if (IsDefect) {
									DDefectPoint[DCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DDefectPoint[DCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DDefectPoint[DCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + ROI_X);
									DDefectPoint[DCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/ 2) + ROI_Y);
									DDefectPoint[DCount].Rect.right = (long)(DDefectPoint[DCount].Rect.left + DDefectPoint[DCount].Width );
									DDefectPoint[DCount].Rect.bottom= (long)(DDefectPoint[DCount].Rect.top  + DDefectPoint[DCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DDefectPoint[DCount].Rect.top + 1; n < DDefectPoint[DCount].Rect.bottom - 1; n++) {
										for (int m = DDefectPoint[DCount].Rect.left + 1; m < DDefectPoint[DCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue < MaskDefectThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DDefectPoint[DCount].Round = PixelCount;
									DDefectPoint[DCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
									DDefectPoint[DCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Width = DDefectPoint[DCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
									DCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}

				//Black Masking Bright Defect 
				if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f && MaskBrokenThresHold!=0){
					MaskBrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskBrightDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskBrightDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (4);
					Poly_Out.reserve(4);

					Poly_In.push_back(The::IntPoint(LSP.x -Margin     , LSP.y     ));
					Poly_In.push_back(The::IntPoint(LSP.x             , LSP.y     ));
					Poly_In.push_back(The::IntPoint(FLBPoint.x        , FLBPoint.y));
					Poly_In.push_back(The::IntPoint(FLBPoint.x -Margin, FLBPoint.y));

					//Masking Line Attch 체크 
					StaMaskPos.x = FLTPoint.x   ; StaMaskPos.y = FLTPoint.y;
					EndMaskPos.x = Line[0].FP1.x; EndMaskPos.y = FrameImgH ;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskBrightDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskBrightDefectImg, &BinaryImg, MaskBrokenThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X = (int)(LSP.x + SideMargin        );
						ROI_Y = (int)(LSP.y                     );
						ROI_W = (int)(SideMargin + PrintScOffset);
						ROI_H = (int)(FLBPoint.y                );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0            || ROI_W <=0         || (ROI_H)<=0           ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)> BtmLimitLine || ROI_W >=FrameImgW || (ROI_H)> BtmLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                          return false;
						 
						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							for (int k = 0; k < BlobCount; k++) {
								if (DCrackCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								// 실제 비선형 연마라인 붙어있는 불량인지 확인 
								LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
								IsDefect = false;
								LTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLT);
								RTDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRT);
								LBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectLB);
								RBDis = Math.GetLengthFrLineToPoint(StaMaskPos,EndMaskPos,DefectRB);
								if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
									IsDefect = true;
								}

								if (IsDefect) {
									DCrackPoint[DCrackCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DCrackPoint[DCrackCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DCrackPoint[DCrackCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width / 2) + ROI_X);
									DCrackPoint[DCrackCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/ 2) + ROI_Y);
									DCrackPoint[DCrackCount].Rect.right = (long)(DCrackPoint[DCrackCount].Rect.left + DCrackPoint[DCrackCount].Width );
									DCrackPoint[DCrackCount].Rect.bottom= (long)(DCrackPoint[DCrackCount].Rect.top  + DCrackPoint[DCrackCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DCrackPoint[DCrackCount].Rect.top + 1; n < DCrackPoint[DCrackCount].Rect.bottom - 1; n++) {
										for (int m = DCrackPoint[DCrackCount].Rect.left + 1; m < DCrackPoint[DCrackCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue > MaskBrokenThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DCrackPoint[DCrackCount].Round = PixelCount;
									DCrackPoint[DCrackCount].P.x   = (float)(DCrackPoint[DCrackCount].Rect.left);
									DCrackPoint[DCrackCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Width = DCrackPoint[DCrackCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DCrackPoint[DCrackCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Rect.bottom = (long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
									DCrackCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}
			}
			else if(IsCornerBtm){
			    if(LBCorOutEdgeCount!=0){ //No Masking Corner영역 Dark Defect 
					CornerCount = 0;
					for(int k=0; k< LBCorOutEdgeCount; k++){ //Chamber Cut 영역 기본 Dark Defect 검사
						if(LTCorOutPoint[k].y<FLBLPoint.y) continue;
						if(LTCorOutPoint[k].y>FLBBPoint.y) continue;
						Dis = Math.GetLengthFrLineToPoint(FLBLPoint, FLBBPoint, LBCorOutPoint[k]);
						LBCorner[CornerCount].Dv = Dis;
						LBCorner[CornerCount].P.x = LBCorOutPoint[k].x;
						LBCorner[CornerCount].P.y = LBCorOutPoint[k].y;
						CornerCount++;
					}
					if (LBCornerWidth > (LBCornerHeight * 2)) LBCorDefectCnt = GetSize(LBCorner, LBCorDefectPoint, CornerCount, 10);
					else                                      LBCorDefectCnt = GetSize(LBCorner, LBCorDefectPoint, CornerCount, 5 );
				}

				//No Masking 직선라인 기본 Dark Defect 검사 
				// Image Conversion Binary_Image
				EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

				// Image Euresys Algorithm Attach 
				DefectROI.Attach(&DefectImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
				IAddress = DefectImg.GetImagePtr();

				LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);

				// 검사 ROI설정 
				ROI_X = (int)(LSP.x + SideMargin        );
				ROI_Y = (int)(LSP.y                     );
				ROI_W = (int)(SideMargin + PrintScOffset);
				ROI_H = (int)(FLBLPoint.y               );

				// 검사 시퀀스 
				DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
				if((ROI_X) <=0               ||(ROI_Y)< 0              || ROI_W<=0         || (ROI_H)<=0           ) return false;
				if((ROI_X) >=FrameImgW       ||(ROI_Y)> BtmLimitLine   || ROI_W>=FrameImgW || (ROI_H)> BtmLimitLine) return false;
				if((ROI_X+ROI_W) >=FrameImgW ||(ROI_Y+ ROI_H)> FrameImgH)                                            return false;

				// Blob 검사 진행 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);

				// Blob 필터링 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(DCount>=MAX_DEFECT_POINT) break;
						DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +ROI_X);
						DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +ROI_Y);
						DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
						DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

						PixelCount = 0;
						PixelValue = 0;
						// 내부 Pixel Data 축출 
						for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
							for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < DarkThresHold) PixelCount++;
							}
						}
						// Display 데이터 필요함 
						DDefectPoint[DCount].Round = PixelCount;
						DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
						DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ROI_Y+ImageHeight );
						DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

						// Merge 위해서 데이터 필요함 
						DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight);
						DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
						DCount++;
					}
				}
				BlobCount =0;
				

				//White_Masking Dark Defect 
				if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f && LBCorOutEdgeCount!=0 && MaskDefectThresHold!=0){
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskDarkDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LBCorOutEdgeCount + 4);
					Poly_Out.reserve(LBCorOutEdgeCount + 4);

					Poly_In.push_back(The::IntPoint(LSP.x -Margin  , LSP.y      )); 
					Poly_In.push_back(The::IntPoint(LSP.x          , LSP.y      ));
					Poly_In.push_back(The::IntPoint(FLBLPoint.x    , FLBLPoint.y));
					for(int k=0; k<LBCorOutEdgeCount; k++){Poly_In.push_back(The::IntPoint(LBCorOutPoint[k].x, LBCorOutPoint[k].y));}
					Poly_In.push_back(The::IntPoint(FLBBPoint.x    , FLBBPoint.y));
					Poly_In.push_back(The::IntPoint(LSP.x - Margin , FLBPoint.y ));

					//Masking Line Attch 체크 
					StaMaskPos.x = LSP.x      ; StaMaskPos.y = LSP.y      ;
					EndMaskPos.x = FLBLPoint.x; EndMaskPos.y = FLBLPoint.y;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, MaskDefectThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X = (int)(LSP.x + SideMargin );
						ROI_Y = (int)(LSP.y              );
						ROI_W = (int)(LTCornerWidth      );
						ROI_H = (int)(FLBPoint.y         );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0            || ROI_W <=0         || (ROI_H)<=0           ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)> BtmLimitLine || ROI_W >=FrameImgW || (ROI_H)> BtmLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                          return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							PreCompareCnt = 0;
							for (int k = 0; k < BlobCount; k++) {
								if (DCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								//Chamber Cut Line 붙어있는 불량인지 확인 
								IsDefect = false;
								for (int i = PreCompareCnt; i < LTCorOutEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLength(LTCorOutPoint[i], DefectLT);
									RTDis = Math.GetLength(LTCorOutPoint[i], DefectRT);
									LBDis = Math.GetLength(LTCorOutPoint[i], DefectLB);
									RBDis = Math.GetLength(LTCorOutPoint[i], DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
										PreCompareCnt = i;
										break;
									}
								}

								//직선 Line 붙어있는 불량인지 확인 
								if(!IsDefect){
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLT);
									RTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRT);
									LBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLB);
									RBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
									}
								}

								if (IsDefect) {
									DDefectPoint[DCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DDefectPoint[DCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DDefectPoint[DCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + ROI_X);
									DDefectPoint[DCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/ 2) + ROI_Y);
									DDefectPoint[DCount].Rect.right = (long)(DDefectPoint[DCount].Rect.left + DDefectPoint[DCount].Width );
									DDefectPoint[DCount].Rect.bottom= (long)(DDefectPoint[DCount].Rect.top  + DDefectPoint[DCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DDefectPoint[DCount].Rect.top + 1; n < DDefectPoint[DCount].Rect.bottom - 1; n++) {
										for (int m = DDefectPoint[DCount].Rect.left + 1; m < DDefectPoint[DCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue < MaskDefectThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DDefectPoint[DCount].Round = PixelCount;
									DDefectPoint[DCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
									DDefectPoint[DCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Width = DDefectPoint[DCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
									DCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}



				//Black_Masking Bright Defect 
				if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f && LBCorOutEdgeCount!=0 && MaskBrokenThresHold!=0){
					MaskBrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskBrightDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskBrightDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LBCorOutEdgeCount + 4);
					Poly_Out.reserve(LBCorOutEdgeCount + 4);

					Poly_In.push_back(The::IntPoint(LSP.x -Margin  , LSP.y      )); 
					Poly_In.push_back(The::IntPoint(LSP.x          , LSP.y      ));
					Poly_In.push_back(The::IntPoint(FLBLPoint.x    , FLBLPoint.y));
					for(int k=0; k<LBCorOutEdgeCount; k++){Poly_In.push_back(The::IntPoint(LBCorOutPoint[k].x, LBCorOutPoint[k].y));}
					Poly_In.push_back(The::IntPoint(FLBBPoint.x    , FLBBPoint.y));
					Poly_In.push_back(The::IntPoint(LSP.x - Margin , FLBPoint.y ));

					//Masking Line Attch 체크 
					StaMaskPos.x = LSP.x      ; StaMaskPos.y = LSP.y      ;
					EndMaskPos.x = FLBLPoint.x; EndMaskPos.y = FLBLPoint.y;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskBrightDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskBrightDefectImg, &BinaryImg, MaskBrokenThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X = (int)(LSP.x + SideMargin );
						ROI_Y = (int)(LSP.y              );
						ROI_W = (int)(LTCornerWidth      );
						ROI_H = (int)(FLBPoint.y         );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0            || ROI_W <=0         || (ROI_H)<=0           ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)> BtmLimitLine || ROI_W >=FrameImgW || (ROI_H)> BtmLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                          return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							PreCompareCnt = 0;
							for (int k = 0; k < BlobCount; k++) {
								if (DCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								//Chamber Cut Line 붙어있는 불량인지 확인 
								IsDefect = false;
								for (int i = PreCompareCnt; i < LTCorOutEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLength(LTCorOutPoint[i], DefectLT);
									RTDis = Math.GetLength(LTCorOutPoint[i], DefectRT);
									LBDis = Math.GetLength(LTCorOutPoint[i], DefectLB);
									RBDis = Math.GetLength(LTCorOutPoint[i], DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
										PreCompareCnt = i;
										break;
									}
								}

								//직선 Line 붙어있는 불량인지 확인 
								if (!IsDefect) {
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLT);
									RTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRT);
									LBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLB);
									RBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
									}
								}

								if (IsDefect) {
									DCrackPoint[DCrackCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DCrackPoint[DCrackCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DCrackPoint[DCrackCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width / 2) + ROI_X);
									DCrackPoint[DCrackCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/ 2) + ROI_Y);
									DCrackPoint[DCrackCount].Rect.right = (long)(DCrackPoint[DCrackCount].Rect.left + DCrackPoint[DCrackCount].Width );
									DCrackPoint[DCrackCount].Rect.bottom= (long)(DCrackPoint[DCrackCount].Rect.top  + DCrackPoint[DCrackCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DCrackPoint[DCrackCount].Rect.top + 1; n < DCrackPoint[DCrackCount].Rect.bottom - 1; n++) {
										for (int m = DCrackPoint[DCrackCount].Rect.left + 1; m < DCrackPoint[DCrackCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue > MaskBrokenThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DCrackPoint[DCrackCount].Round = PixelCount;
									DCrackPoint[DCrackCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
									DCrackPoint[DCrackCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Width = DCrackPoint[DCrackCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DCrackPoint[DCrackCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Rect.bottom = (long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
									DCrackCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}
			}
			else if(IsCircleBtm){
			    if(LBCirOutEdgeCount!=0){ //No Masking Circle영역 Dark Defect 
					LBCirDeftectCnt = GetPolygonSize(LBCirOutPoint, LBCirDefectPoint, LBCirOutEdgeCount, 100, 10, 0, CircleThresHold); //DarkDefect
				}

				//No Masking 직선라인 기본 Dark Defect 검사 
				// Image Conversion Binary_Image
				EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

				// Image Euresys Algorithm Attach 
				DefectROI.Attach(&DefectImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
				IAddress = DefectImg.GetImagePtr();

				LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);

				// 검사 ROI설정 
				ROI_X = (int)(LSP.x + SideMargin            );
				ROI_Y = (int)(LSP.y                         );
				ROI_W = (int)(SideMargin + PrintScOffset    );
				ROI_H = (int)(FrameImgH - LBCirOutPoint[0].y);

				// 검사 시퀀스 
				DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
				if((ROI_X) <=0               ||(ROI_Y)< 0             || ROI_W<=0         || (ROI_H)<=0           ) return false;
				if((ROI_X) >=FrameImgW       ||(ROI_Y)> BtmLimitLine  || ROI_W>=FrameImgW || (ROI_H)> BtmLimitLine) return false;
				if((ROI_X+ROI_W) >=FrameImgW ||(ROI_Y+ ROI_H)>FrameImgH)                                            return false;

				// Blob 검사 진행 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);

				// Blob 필터링 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(DCount>=MAX_DEFECT_POINT) break;
						DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +ROI_X);
						DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +ROI_Y);
						DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
						DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

						PixelCount = 0;
						PixelValue = 0;
						// 내부 Pixel Data 축출 
						for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
							for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < DarkThresHold) PixelCount++;
							}
						}
						// Display 데이터 필요함 
						DDefectPoint[DCount].Round = PixelCount;
						DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
						DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight );
						DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

						// Merge 위해서 데이터 필요함 
						DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+ ROI_Y +ImageHeight);
						DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
						DCount++;
					}
				}
				BlobCount =0;


				//White_Masking Dark Defect 
				if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f && LBCirOutEdgeCount!=0 && MaskDefectThresHold!=0){
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskDarkDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255)); //White Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LBCirOutEdgeCount + 4);
					Poly_Out.reserve(LBCirOutEdgeCount + 4);

					Poly_In.push_back(The::IntPoint(LSP.x -Margin  , LSP.y                         )); 
					Poly_In.push_back(The::IntPoint(LSP.x          , LSP.y                         ));
					for(int k=0; k<LBCirOutEdgeCount; k++){Poly_In.push_back(The::IntPoint(LBCirOutPoint[k].x, LBCirOutPoint[k].y));}
					Poly_In.push_back(The::IntPoint(LBCirOutPoint[LBCirOutEdgeCount-1].x, FLBPoint.y));
					Poly_In.push_back(The::IntPoint(LSP.x - Margin, FLBPoint.y                      ));

					//Masking Line Attch 체크 
					StaMaskPos.x = LSP.x     ; StaMaskPos.y = LSP.y     ;
					EndMaskPos.x = FLBPoint.x; EndMaskPos.y = FLBPoint.y;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, MaskDefectThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X = (int)(LSP.x + SideMargin );
						ROI_Y = (int)(LSP.y              );
						ROI_W = (int)(LTCircleWidth      );
						ROI_H = (int)(FLBPoint.y         );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0            || ROI_W <=0         || (ROI_H)<=0           ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)> BtmLimitLine || ROI_W >=FrameImgW || (ROI_H)> BtmLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                          return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							PreCompareCnt = 0;
							for (int k = 0; k < BlobCount; k++) {
								if (DCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								//Chamber Cut Line 붙어있는 불량인지 확인 
								IsDefect = false;
								for (int i = PreCompareCnt; i < LTCirOutEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLength(LTCirOutPoint[i], DefectLT);
									RTDis = Math.GetLength(LTCirOutPoint[i], DefectRT);
									LBDis = Math.GetLength(LTCirOutPoint[i], DefectLB);
									RBDis = Math.GetLength(LTCirOutPoint[i], DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
										PreCompareCnt = i;
										break;
									}
								}

								//직선 Line 붙어있는 불량인지 확인 
								if (!IsDefect) {
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLT);
									RTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRT);
									LBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLB);
									RBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
									}
								}

								if (IsDefect) {
									DDefectPoint[DCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DDefectPoint[DCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DDefectPoint[DCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + ROI_X);
									DDefectPoint[DCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/ 2) + ROI_Y);
									DDefectPoint[DCount].Rect.right = (long)(DDefectPoint[DCount].Rect.left + DDefectPoint[DCount].Width );
									DDefectPoint[DCount].Rect.bottom= (long)(DDefectPoint[DCount].Rect.top  + DDefectPoint[DCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DDefectPoint[DCount].Rect.top + 1; n < DDefectPoint[DCount].Rect.bottom - 1; n++) {
										for (int m = DDefectPoint[DCount].Rect.left + 1; m < DDefectPoint[DCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue < MaskDefectThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DDefectPoint[DCount].Round = PixelCount;
									DDefectPoint[DCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
									DDefectPoint[DCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Width = DDefectPoint[DCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ROI_Y + ImageHeight);
									DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
									DCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}

				//Black_Masking Bright Defect 
				if(FLBPoint.x!=0.0f && FLBPoint.y!=0.0f && LBCirOutEdgeCount!=0 && MaskBrokenThresHold!=0){
					MaskBrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());          //Mask Image Size 설정
					EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskBrightDefectImg); //Mask Image Copy 

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black Brush 
					HDC hdc = Easy::OpenImageGraphicContext(&MaskBrightDefectImg);

					//MaskPoint Init 
					if(MaskPoint!=NULL){delete []MaskPoint;}

					LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LTCirOutEdgeCount + 4);
					Poly_Out.reserve(LTCirOutEdgeCount + 4);

					Poly_In.push_back(The::IntPoint(LSP.x -Margin  , LSP.y                         )); 
					Poly_In.push_back(The::IntPoint(LSP.x          , LSP.y                         ));
					for(int k=0; k<LBCirOutEdgeCount; k++){Poly_In.push_back(The::IntPoint(LBCirOutPoint[k].x, LBCirOutPoint[k].y));}
					Poly_In.push_back(The::IntPoint(LBCirOutPoint[LBCirOutEdgeCount-1].x, FLBPoint.y));
					Poly_In.push_back(The::IntPoint(LSP.x - Margin, FLBPoint.y                      ));

					//Masking Line Attch 체크 
					StaMaskPos.x = LSP.x     ; StaMaskPos.y = LSP.y     ;
					EndMaskPos.x = FLBPoint.x; EndMaskPos.y = FLBPoint.y;

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size();

					MaskCnt = PolyOutCnt; //ClipperOffset 활용하여 최종 산출된 폐곡선 Count 
					MaskPoint = new POINT[MaskCnt];

					for(int k=0; k<MaskCnt; k++){ //동적생성 데이터 초기화 및 데이터 넘겨줌 
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;

						if (MaskPoint[k].x > MaxDx    ) MaxDx          = MaskPoint[k].x;
						if (MaskPoint[k].y > MaxDy    ) MaxDy          = MaskPoint[k].y;
						if (MaskPoint[k].x < 0        ) MaskPoint[k].x = 0             ;
						if (MaskPoint[k].y > FrameImgH) MaskPoint[k].y = FrameImgH     ;
					}

					HRGN Rgn = ::CreatePolygonRgn(MaskPoint, MaskCnt, WINDING);
					::FillRgn(hdc, Rgn, fillBrh);
					::DeleteObject(Rgn);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskBrightDefectImg, hdc);

					if(MaskDarkDefectImg.GetImagePtr()){ //Dark Defect 검출 
						EasyImage::Threshold(&MaskBrightDefectImg, &BinaryImg, MaskBrokenThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BinaryImg.GetImagePtr();

						// 검사 ROI설정 
						ROI_X = (int)(LSP.x + SideMargin );
						ROI_Y = (int)(LSP.y              );
						ROI_W = (int)(LTCircleWidth      );
						ROI_H = (int)(FLBPoint.y         );

						DefectROI.SetPlacement(ROI_X, ROI_Y, ROI_W, ROI_H);
						if((ROI_X) <=0                ||(ROI_Y)< 0            || ROI_W <=0         || (ROI_H)<=0           ) return false;
						if((ROI_X) >=FrameImgW        ||(ROI_Y)> BtmLimitLine || ROI_W >=FrameImgW || (ROI_H)> BtmLimitLine) return false;
						if((ROI_X+ ROI_W) >=FrameImgW ||(ROI_Y + ROI_H)> FrameImgH)                                          return false;

						// Blob 검사 진행 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						if (BlobCount != 0) {
							PreCompareCnt = 0;
							for (int k = 0; k < BlobCount; k++) {
								if (DCount >= MAX_DEFECT_POINT) break;
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + ROI_X);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/ 2) + ROI_Y);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								//Chamber Cut Line 붙어있는 불량인지 확인 
								IsDefect = false;
								for (int i = PreCompareCnt; i < LTCirOutEdgeCount; i++) { // 실제 비선형 연마라인 붙어있는 불량인지 확인 
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLength(LTCirOutPoint[i], DefectLT);
									RTDis = Math.GetLength(LTCirOutPoint[i], DefectRT);
									LBDis = Math.GetLength(LTCirOutPoint[i], DefectLB);
									RBDis = Math.GetLength(LTCirOutPoint[i], DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
										PreCompareCnt = i;
										break;
									}
								}

								//직선 Line 붙어있는 불량인지 확인 
								if (!IsDefect) {
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									LTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLT);
									RTDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRT);
									LBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectLB);
									RBDis = Math.GetLengthFrLineToPoint(StaMaskPos, EndMaskPos, DefectRB);
									if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
										IsDefect = true;
									}
								}

								if (IsDefect) {
									DCrackPoint[DCrackCount].Width      = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									DCrackPoint[DCrackCount].Height     = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();
									DCrackPoint[DCrackCount].Rect.left  = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width / 2) + ROI_X);
									DCrackPoint[DCrackCount].Rect.top   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/ 2) + ROI_Y);
									DCrackPoint[DCrackCount].Rect.right = (long)(DCrackPoint[DCrackCount].Rect.left + DCrackPoint[DCrackCount].Width );
									DCrackPoint[DCrackCount].Rect.bottom= (long)(DCrackPoint[DCrackCount].Rect.top  + DCrackPoint[DCrackCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// 내부 Pixel Data 축출 
									for (int n = DCrackPoint[DCrackCount].Rect.top + 1; n < DCrackPoint[DCrackCount].Rect.bottom - 1; n++) {
										for (int m = DCrackPoint[DCrackCount].Rect.left + 1; m < DCrackPoint[DCrackCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue > MaskBrokenThresHold) PixelCount++;
										}
									}
									// Display 데이터 필요함 
									DCrackPoint[DCrackCount].Round = PixelCount;
									DCrackPoint[DCrackCount].P.x   = (float)(DDefectPoint[DCount].Rect.left);
									DCrackPoint[DCrackCount].P.y   = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Width = DCrackPoint[DCrackCount].Width + SideMargin;

									// Merge 위해서 데이터 필요함 
									DCrackPoint[DCrackCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ROI_Y + ImageHeight);
									DCrackPoint[DCrackCount].Rect.bottom = (long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
									DCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////

		DefectPointCount = LTCirDeftectCnt+LTCirCrackCnt +LTCorDefectCnt+LTCorCrackCnt+ACount+BCount+CCount+DCount+
			               ACrackCount+BCrackCount+CCrackCount+DCrackCount+LBCirDeftectCnt+LBCirCrackCnt+LBCorDefectCnt+LBCorCrackCnt+ LeftDefectCnt+LeftCrackCnt; // 전체 불량개수 계산

		// BTM 영역 검사 종료 후 Cell Size 측정 
		// 검사 종료 후 Cell Size 측정함 
		if(LeftStaPos.x!=0.0f && LeftStaPos.y!=0.0f && LeftEndPos.x!=0.0f && LeftEndPos.y!=0.0f){
			LeftCellSizeHeight = Math.GetLengthFrPointToPoint(LeftStaPos, LeftEndPos);
		}
		// Dark Defect Blob Merge
		///////////////////////////////////////////////////////////////////////////////////////////////
		// LTCircle Dark Defect Merge 기능 
		if(LTCirDeftectCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = LTCirDeftectCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(LTCirDefectPoint[k].Rect.left==0 && LTCirDefectPoint[k].Rect.top==0 && LTCirDefectPoint[k].Rect.right==0 && LTCirDefectPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(LTCirDefectPoint[l].Rect.left==0 && LTCirDefectPoint[l].Rect.top==0 && LTCirDefectPoint[l].Rect.right==0 && LTCirDefectPoint[l].Rect.bottom==0) continue;

					r1 = LTCirDefectPoint[k].Rect;
					r2 = LTCirDefectPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						LTCirDefectPoint[k].Round    += LTCirDefectPoint[l].Round;
						LTCirDefectPoint[k].Rect      = OnMerge(LTCirDefectPoint[k].Rect, LTCirDefectPoint[l].Rect);
						LTCirDefectPoint[k].Width     = LTCirDefectPoint[k].Rect.right  - LTCirDefectPoint[k].Rect.left;
						LTCirDefectPoint[k].Height    = LTCirDefectPoint[k].Rect.bottom - LTCirDefectPoint[k].Rect.top ;
						LTCirDefectPoint[k].Hor_Dis   = LTCirDefectPoint[k].Width ; 
						LTCirDefectPoint[k].Ver_Dis   = LTCirDefectPoint[k].Height;
						LTCirDefectPoint[k].P.x       = (float)(LTCirDefectPoint[k].Rect.left);
						LTCirDefectPoint[k].P.y       = (float)(LTCirDefectPoint[k].Rect.top ); 
						LTCirDefectPoint[l].Rect.left = LTCirDefectPoint[l].Rect.top = LTCirDefectPoint[l].Rect.right = LTCirDefectPoint[l].Rect.bottom =0;
						LTCirDeftectCnt--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(LTCirDefectPoint[j].Rect.left==0 && LTCirDefectPoint[j].Rect.top==0 && LTCirDefectPoint[j].Rect.right==0 && LTCirDefectPoint[j].Rect.bottom==0) continue;
				LTCirDefectPoint[sort] = LTCirDefectPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (LTCirCrackCnt != 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpDDefectCount = LTCirCrackCnt;

			for (int k = 0; k < tmpDDefectCount; k++) {
				if (LTCirCrackPoint[k].Rect.left == 0 && LTCirCrackPoint[k].Rect.top == 0 && LTCirCrackPoint[k].Rect.right == 0 && LTCirCrackPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpDDefectCount - 1)) break;
					if (LTCirCrackPoint[l].Rect.left == 0 && LTCirCrackPoint[l].Rect.top == 0 && LTCirCrackPoint[l].Rect.right == 0 && LTCirCrackPoint[l].Rect.bottom == 0) continue;

					r1 = LTCirCrackPoint[k].Rect;
					r2 = LTCirCrackPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left <= r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right > r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						LTCirCrackPoint[k].Round  += LTCirCrackPoint[l].Round;
						LTCirCrackPoint[k].Rect    = OnMerge(LTCirCrackPoint[k].Rect, LTCirCrackPoint[l].Rect);
						LTCirCrackPoint[k].Width   = LTCirCrackPoint[k].Rect.right  - LTCirCrackPoint[k].Rect.left;
						LTCirCrackPoint[k].Height  = LTCirCrackPoint[k].Rect.bottom - LTCirCrackPoint[k].Rect.top;
						LTCirCrackPoint[k].Hor_Dis = LTCirCrackPoint[k].Width;
						LTCirCrackPoint[k].Ver_Dis = LTCirCrackPoint[k].Height;
						LTCirCrackPoint[k].P.x = (float)(LTCirCrackPoint[k].Rect.left);
						LTCirCrackPoint[k].P.y = (float)(LTCirCrackPoint[k].Rect.top);
						LTCirCrackPoint[l].Rect.left = LTCirCrackPoint[l].Rect.top = LTCirCrackPoint[l].Rect.right = LTCirCrackPoint[l].Rect.bottom = 0;
						LTCirCrackCnt--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpDDefectCount; j++) {
				if (LTCirCrackPoint[j].Rect.left == 0 && LTCirCrackPoint[j].Rect.top == 0 && LTCirCrackPoint[j].Rect.right == 0 && LTCirCrackPoint[j].Rect.bottom == 0) continue;
				LTCirCrackPoint[sort] = LTCirCrackPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(LBCirDeftectCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = LBCirDeftectCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(LBCirDefectPoint[k].Rect.left==0 && LBCirDefectPoint[k].Rect.top==0 && LBCirDefectPoint[k].Rect.right==0 && LBCirDefectPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(LBCirDefectPoint[l].Rect.left==0 && LBCirDefectPoint[l].Rect.top==0 && LBCirDefectPoint[l].Rect.right==0 && LBCirDefectPoint[l].Rect.bottom==0) continue;

					r1 = LBCirDefectPoint[k].Rect;
					r2 = LBCirDefectPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						LBCirDefectPoint[k].Round    += LBCirDefectPoint[l].Round;
						LBCirDefectPoint[k].Rect      = OnMerge(LBCirDefectPoint[k].Rect, LBCirDefectPoint[l].Rect);
						LBCirDefectPoint[k].Width     = LBCirDefectPoint[k].Rect.right  - LBCirDefectPoint[k].Rect.left;
						LBCirDefectPoint[k].Height    = LBCirDefectPoint[k].Rect.bottom - LBCirDefectPoint[k].Rect.top ;
						LBCirDefectPoint[k].Hor_Dis   = LBCirDefectPoint[k].Width ; 
						LBCirDefectPoint[k].Ver_Dis   = LBCirDefectPoint[k].Height;
						LBCirDefectPoint[k].P.x       = (float)(LBCirDefectPoint[k].Rect.left);
						LBCirDefectPoint[k].P.y       = (float)(LBCirDefectPoint[k].Rect.top ); 
						LBCirDefectPoint[l].Rect.left = LBCirDefectPoint[l].Rect.top = LBCirDefectPoint[l].Rect.right = LBCirDefectPoint[l].Rect.bottom =0;
						LBCirDeftectCnt--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(LBCirDefectPoint[j].Rect.left==0 && LBCirDefectPoint[j].Rect.top==0 && LBCirDefectPoint[j].Rect.right==0 && LBCirDefectPoint[j].Rect.bottom==0) continue;
				LBCirDefectPoint[sort] = LBCirDefectPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(LBCirCrackCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = LBCirCrackCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(LBCirCrackPoint[k].Rect.left==0 && LBCirCrackPoint[k].Rect.top==0 && LBCirCrackPoint[k].Rect.right==0 && LBCirCrackPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(LBCirCrackPoint[l].Rect.left==0 && LBCirCrackPoint[l].Rect.top==0 && LBCirCrackPoint[l].Rect.right==0 && LBCirCrackPoint[l].Rect.bottom==0) continue;

					r1 = LBCirCrackPoint[k].Rect;
					r2 = LBCirCrackPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						LBCirCrackPoint[k].Round    += LBCirCrackPoint[l].Round;
						LBCirCrackPoint[k].Rect      = OnMerge(LBCirCrackPoint[k].Rect, LBCirCrackPoint[l].Rect);
						LBCirCrackPoint[k].Width     = LBCirCrackPoint[k].Rect.right  - LBCirCrackPoint[k].Rect.left;
						LBCirCrackPoint[k].Height    = LBCirCrackPoint[k].Rect.bottom - LBCirCrackPoint[k].Rect.top ;
						LBCirCrackPoint[k].Hor_Dis   = LBCirCrackPoint[k].Width ; 
						LBCirCrackPoint[k].Ver_Dis   = LBCirCrackPoint[k].Height;
						LBCirCrackPoint[k].P.x       = (float)(LBCirCrackPoint[k].Rect.left);
						LBCirCrackPoint[k].P.y       = (float)(LBCirCrackPoint[k].Rect.top ); 
						LBCirCrackPoint[l].Rect.left = LBCirCrackPoint[l].Rect.top = LBCirCrackPoint[l].Rect.right = LBCirCrackPoint[l].Rect.bottom =0;
						LBCirCrackCnt--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(LBCirCrackPoint[j].Rect.left==0 && LBCirCrackPoint[j].Rect.top==0 && LBCirCrackPoint[j].Rect.right==0 && LBCirCrackPoint[j].Rect.bottom==0) continue;
				LBCirCrackPoint[sort] = LBCirCrackPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(LTCorDefectCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = LTCorDefectCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(LTCorDefectPoint[k].Rect.left==0 && LTCorDefectPoint[k].Rect.top==0 && LTCorDefectPoint[k].Rect.right==0 && LTCorDefectPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(LTCorDefectPoint[l].Rect.left==0 && LTCorDefectPoint[l].Rect.top==0 && LTCorDefectPoint[l].Rect.right==0 && LTCorDefectPoint[l].Rect.bottom==0) continue;

					r1 = LTCorDefectPoint[k].Rect;
					r2 = LTCorDefectPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						LTCorDefectPoint[k].Round    += LTCorDefectPoint[l].Round;
						LTCorDefectPoint[k].Rect      = OnMerge(LTCorDefectPoint[k].Rect, LTCorDefectPoint[l].Rect);
						LTCorDefectPoint[k].Width     = LTCorDefectPoint[k].Rect.right  - LTCorDefectPoint[k].Rect.left;
						LTCorDefectPoint[k].Height    = LTCorDefectPoint[k].Rect.bottom - LTCorDefectPoint[k].Rect.top ;
						LTCorDefectPoint[k].Hor_Dis   = LTCorDefectPoint[k].Width ; 
						LTCorDefectPoint[k].Ver_Dis   = LTCorDefectPoint[k].Height;
						LTCorDefectPoint[k].P.x       = (float)(LTCorDefectPoint[k].Rect.left);
						LTCorDefectPoint[k].P.y       = (float)(LTCorDefectPoint[k].Rect.top ); 
						LTCorDefectPoint[l].Rect.left = LTCorDefectPoint[l].Rect.top = LTCorDefectPoint[l].Rect.right = LTCorDefectPoint[l].Rect.bottom =0;
						LTCorDefectCnt--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(LTCorDefectPoint[j].Rect.left==0 && LTCorDefectPoint[j].Rect.top==0 && LTCorDefectPoint[j].Rect.right==0 && LTCorDefectPoint[j].Rect.bottom==0) continue;
				LTCorDefectPoint[sort] = LTCorDefectPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(LTCorCrackCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = LTCorCrackCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(LTCorCrackPoint[k].Rect.left==0 && LTCorCrackPoint[k].Rect.top==0 && LTCorCrackPoint[k].Rect.right==0 && LTCorCrackPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(LTCorCrackPoint[l].Rect.left==0 && LTCorCrackPoint[l].Rect.top==0 && LTCorCrackPoint[l].Rect.right==0 && LTCorCrackPoint[l].Rect.bottom==0) continue;

					r1 = LTCorCrackPoint[k].Rect;
					r2 = LTCorCrackPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						LTCorCrackPoint[k].Round    += LTCorCrackPoint[l].Round;
						LTCorCrackPoint[k].Rect      = OnMerge(LTCorCrackPoint[k].Rect, LTCorCrackPoint[l].Rect);
						LTCorCrackPoint[k].Width     = LTCorCrackPoint[k].Rect.right  - LTCorCrackPoint[k].Rect.left;
						LTCorCrackPoint[k].Height    = LTCorCrackPoint[k].Rect.bottom - LTCorCrackPoint[k].Rect.top ;
						LTCorCrackPoint[k].Hor_Dis   = LTCorCrackPoint[k].Width ; 
						LTCorCrackPoint[k].Ver_Dis   = LTCorCrackPoint[k].Height;
						LTCorCrackPoint[k].P.x       = (float)(LTCorCrackPoint[k].Rect.left);
						LTCorCrackPoint[k].P.y       = (float)(LTCorCrackPoint[k].Rect.top ); 
						LTCorCrackPoint[l].Rect.left = LTCorCrackPoint[l].Rect.top = LTCorCrackPoint[l].Rect.right = LTCorCrackPoint[l].Rect.bottom =0;
						LTCorCrackCnt--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(LTCorCrackPoint[j].Rect.left==0 && LTCorCrackPoint[j].Rect.top==0 && LTCorCrackPoint[j].Rect.right==0 && LTCorCrackPoint[j].Rect.bottom==0) continue;
				LTCorCrackPoint[sort] = LTCorCrackPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(LBCorDefectCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = LBCorDefectCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(LBCorDefectPoint[k].Rect.left==0 && LBCorDefectPoint[k].Rect.top==0 && LBCorDefectPoint[k].Rect.right==0 && LBCorDefectPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(LBCorDefectPoint[l].Rect.left==0 && LBCorDefectPoint[l].Rect.top==0 && LBCorDefectPoint[l].Rect.right==0 && LBCorDefectPoint[l].Rect.bottom==0) continue;

					r1 = LBCorDefectPoint[k].Rect;
					r2 = LBCorDefectPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						LBCorDefectPoint[k].Round    += LBCorDefectPoint[l].Round;
						LBCorDefectPoint[k].Rect      = OnMerge(LBCorDefectPoint[k].Rect, LBCorDefectPoint[l].Rect);
						LBCorDefectPoint[k].Width     = LBCorDefectPoint[k].Rect.right  - LBCorDefectPoint[k].Rect.left;
						LBCorDefectPoint[k].Height    = LBCorDefectPoint[k].Rect.bottom - LBCorDefectPoint[k].Rect.top ;
						LBCorDefectPoint[k].Hor_Dis   = LBCorDefectPoint[k].Width ; 
						LBCorDefectPoint[k].Ver_Dis   = LBCorDefectPoint[k].Height;
						LBCorDefectPoint[k].P.x       = (float)(LBCorDefectPoint[k].Rect.left);
						LBCorDefectPoint[k].P.y       = (float)(LBCorDefectPoint[k].Rect.top ); 
						LBCorDefectPoint[l].Rect.left = LBCorDefectPoint[l].Rect.top = LBCorDefectPoint[l].Rect.right = LBCorDefectPoint[l].Rect.bottom =0;
						LBCorDefectCnt--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(LBCorDefectPoint[j].Rect.left==0 && LBCorDefectPoint[j].Rect.top==0 && LBCorDefectPoint[j].Rect.right==0 && LBCorDefectPoint[j].Rect.bottom==0) continue;
				LBCorDefectPoint[sort] = LBCorDefectPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(LBCorCrackCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = LBCorCrackCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(LBCorCrackPoint[k].Rect.left==0 && LBCorCrackPoint[k].Rect.top==0 && LBCorCrackPoint[k].Rect.right==0 && LBCorCrackPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(LBCorCrackPoint[l].Rect.left==0 && LBCorCrackPoint[l].Rect.top==0 && LBCorCrackPoint[l].Rect.right==0 && LBCorCrackPoint[l].Rect.bottom==0) continue;

					r1 = LBCorCrackPoint[k].Rect;
					r2 = LBCorCrackPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						LBCorCrackPoint[k].Round    += LBCorCrackPoint[l].Round;
						LBCorCrackPoint[k].Rect      = OnMerge(LBCorCrackPoint[k].Rect, LBCorCrackPoint[l].Rect);
						LBCorCrackPoint[k].Width     = LBCorCrackPoint[k].Rect.right  - LBCorCrackPoint[k].Rect.left;
						LBCorCrackPoint[k].Height    = LBCorCrackPoint[k].Rect.bottom - LBCorCrackPoint[k].Rect.top ;
						LBCorCrackPoint[k].Hor_Dis   = LBCorCrackPoint[k].Width ; 
						LBCorCrackPoint[k].Ver_Dis   = LBCorCrackPoint[k].Height;
						LBCorCrackPoint[k].P.x       = (float)(LBCorCrackPoint[k].Rect.left);
						LBCorCrackPoint[k].P.y       = (float)(LBCorCrackPoint[k].Rect.top );
						LBCorCrackPoint[l].Rect.left = LBCorCrackPoint[l].Rect.top = LBCorCrackPoint[l].Rect.right = LBCorCrackPoint[l].Rect.bottom =0;
						LBCorCrackCnt--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(LBCorCrackPoint[j].Rect.left==0 && LBCorCrackPoint[j].Rect.top==0 && LBCorCrackPoint[j].Rect.right==0 && LBCorCrackPoint[j].Rect.bottom==0) continue;
				LBCorCrackPoint[sort] = LBCorCrackPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		if(LeftDefectCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = LeftDefectCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(LeftDefectPoint[k].Rect.left==0 && LeftDefectPoint[k].Rect.top==0 && LeftDefectPoint[k].Rect.right==0 && LeftDefectPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(LeftDefectPoint[l].Rect.left==0 && LeftDefectPoint[l].Rect.top==0 && LeftDefectPoint[l].Rect.right==0 && LeftDefectPoint[l].Rect.bottom==0) continue;

					r1 = LeftDefectPoint[k].Rect;
					r2 = LeftDefectPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						LeftDefectPoint[k].Round    += LeftDefectPoint[l].Round;
						LeftDefectPoint[k].Rect      = OnMerge(LeftDefectPoint[k].Rect, LeftDefectPoint[l].Rect);
						LeftDefectPoint[k].Width     = LeftDefectPoint[k].Rect.right  - LeftDefectPoint[k].Rect.left;
						LeftDefectPoint[k].Height    = LeftDefectPoint[k].Rect.bottom - LeftDefectPoint[k].Rect.top ;
						LeftDefectPoint[k].Hor_Dis   = LeftDefectPoint[k].Width ; 
						LeftDefectPoint[k].Ver_Dis   = LeftDefectPoint[k].Height;
						LeftDefectPoint[k].P.x       = (float)(LeftDefectPoint[k].Rect.left);
						LeftDefectPoint[k].P.y       = (float)(LeftDefectPoint[k].Rect.top ); 
						LeftDefectPoint[l].Rect.left = LeftDefectPoint[l].Rect.top = LeftDefectPoint[l].Rect.right = LeftDefectPoint[l].Rect.bottom =0;
						LeftDefectCnt--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(LeftDefectPoint[j].Rect.left==0 && LeftDefectPoint[j].Rect.top==0 && LeftDefectPoint[j].Rect.right==0 && LeftDefectPoint[j].Rect.bottom==0) continue;
				LeftDefectPoint[sort] = LeftDefectPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(LeftCrackCnt!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = LeftCrackCnt;

			for(int k=0; k<tmpDDefectCount; k++){
				if(LeftCrackPoint[k].Rect.left==0 && LeftCrackPoint[k].Rect.top==0 && LeftCrackPoint[k].Rect.right==0 && LeftCrackPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(LeftCrackPoint[l].Rect.left==0 && LeftCrackPoint[l].Rect.top==0 && LeftCrackPoint[l].Rect.right==0 && LeftCrackPoint[l].Rect.bottom==0) continue;

					r1 = LeftCrackPoint[k].Rect;
					r2 = LeftCrackPoint[l].Rect; 

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						LeftCrackPoint[k].Round    += LeftCrackPoint[l].Round;
						LeftCrackPoint[k].Rect      = OnMerge(LeftCrackPoint[k].Rect, LeftCrackPoint[l].Rect);
						LeftCrackPoint[k].Width     = LeftCrackPoint[k].Rect.right  - LeftCrackPoint[k].Rect.left;
						LeftCrackPoint[k].Height    = LeftCrackPoint[k].Rect.bottom - LeftCrackPoint[k].Rect.top ;
						LeftCrackPoint[k].Hor_Dis   = LeftCrackPoint[k].Width ; 
						LeftCrackPoint[k].Ver_Dis   = LeftCrackPoint[k].Height;
						LeftCrackPoint[k].P.x       = (float)(LeftCrackPoint[k].Rect.left);
						LeftCrackPoint[k].P.y       = (float)(LeftCrackPoint[k].Rect.top ); 
						LeftCrackPoint[l].Rect.left = LeftCrackPoint[l].Rect.top = LeftCrackPoint[l].Rect.right = LeftCrackPoint[l].Rect.bottom =0;
						LeftCrackCnt--;
						k=-1;
						break;
					}

				}
			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(LeftCrackPoint[j].Rect.left==0 && LeftCrackPoint[j].Rect.top==0 && LeftCrackPoint[j].Rect.right==0 && LeftCrackPoint[j].Rect.bottom==0) continue;
				LeftCrackPoint[sort] = LeftCrackPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Side C Merge (박리불량)
		if(DCrackCount!=0){
			bool IsxMg=false, IsyMg=false, IsInMg=false;
			CRect r1, r2;
			int sort =0;
			int tmpDDefectCount = DCrackCount;

			for(int k=0; k<tmpDDefectCount; k++){
				if(DCrackPoint[k].Rect.left==0 && DCrackPoint[k].Rect.top==0 && DCrackPoint[k].Rect.right==0 && DCrackPoint[k].Rect.bottom==0) continue;

				for(int l=k+1; l<k+MgCount; l++ ){
					IsxMg = false; IsyMg = false;
					if(l>(tmpDDefectCount-1)) break;
					if(DCrackPoint[l].Rect.left==0 && DCrackPoint[l].Rect.top==0 && DCrackPoint[l].Rect.right==0 && DCrackPoint[l].Rect.bottom==0) continue;

					r1 = DCrackPoint[k].Rect;
					r2 = DCrackPoint[l].Rect;

					register int HorDist   = abs((r1.left < r2.left) ?  r2.left - r1.right  : r1.left - r2.right );
					register int VerDist   = abs((r1.top  < r2.top ) ?  r2.top  - r1.bottom : r1.top  - r2.bottom);
					IsInMg    = ((r1.left<=r2.left) && (r1.top<r2.top) && (r1.bottom>r2.bottom) && (r1.right >r2.right))? true: false;

					if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

					IsxMg = (HorDist<xMerge) ? true: false;
					IsyMg = (VerDist<yMerge) ? true: false;

					if((IsxMg && IsyMg) || IsInMg){
						DCrackPoint[k].Round    += DCrackPoint[l].Round;
						DCrackPoint[k].Rect      = OnMerge(DCrackPoint[k].Rect, DCrackPoint[l].Rect);
						DCrackPoint[k].Width     = DCrackPoint[k].Rect.right  - DCrackPoint[k].Rect.left;
						DCrackPoint[k].Height    = DCrackPoint[k].Rect.bottom - DCrackPoint[k].Rect.top ;
						DCrackPoint[k].P.x       = (float)(DCrackPoint[k].Rect.left);
						DCrackPoint[k].P.y       = (float)(DCrackPoint[k].Rect.top );
						DCrackPoint[l].Rect.left = DCrackPoint[l].Rect.top = DCrackPoint[l].Rect.right = DCrackPoint[l].Rect.bottom =0;
						DCrackCount--;
						k=-1;
						break;
					}

				}

			}
			for(int j=0; j<tmpDDefectCount; j++){
				if(DCrackPoint[j].Rect.left==0 && DCrackPoint[j].Rect.top==0 && DCrackPoint[j].Rect.right==0 && DCrackPoint[j].Rect.bottom==0) continue;
				DCrackPoint[sort] = DCrackPoint[j];
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
					if(DDefectPoint[l].Rect.left==0 && DDefectPoint[l].Rect.top==0 && DDefectPoint[l].Rect.right==0 && DDefectPoint[l].Rect.bottom==0) continue;

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
int CFM14xLineDefect::OnExecute(bool l, bool t, bool r, bool b, bool lt, bool rt, bool rb, bool lb)
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
	const int CornerMargin = 2  ;
	int BMWidth            = 0  ;

	// StatLine검출 후 자동ROI생성시 사용하는 RoiRange변수 
	// 단축 검사시 Frame기준 비선형 곡선으로 인해서 Line_Fitting불가함 
	// 이로 인해서 HAP데이터를 기준으로 Margin적요한 ROI설정 및 LineFitting없이 바로 Polygon검사 진행함 
	const int RoiMargin       = 200 ;
	const int LeftRoiPxMargin = 150 ;
	const int LeftRoiWdMargin = 1500;

	// PrintMark 영역 Range
	const int GrindMarkRange =40;    
	const int GrindMarkLength=80;

	// 	if(FCompanyMode==COM_TIANMA) BMWidth =500;
	// 	if(FCompanyMode==COM_KWON  ) BMWidth =300;

	// 시작부분 Broken발생시 
	int LineMargin =30;
	int LineRange  =80;

	// 전체Buff기준 개별Frame Real좌표 
	// Cutting 공차 검사시 필요한 데이터 
	///////////////////////////////////////
	int RealFrameStaY=0;
	int RealFrameEndY=0;

	int CutMinPos =1000000;
	int CutMaxPos =      0;
	int RoiCount  =      0;

	int GrindThresHold =0;
	///////////////////////////////////////

	// Euresys ROI 
	// Median Filtter 적용 목적 ROI   
	EROIBW8   FilterROI                ;

	//Polygon Value Test 
	int         PolygonMarginCnt= 0;
	const int   PolygonMarin    = 5;

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

	FrameImgW = GImage->GetWidth();
	FrameImgH = GImage->GetHeight();

	TFPoint LT(0          ,           0)  ;
	TFPoint RT((float)FrameImgW,           0)  ;
	TFPoint RB((float)FrameImgW, (float)FrameImgH)  ;
	TFPoint LB(0          , (float)FrameImgH)  ;

	TFPoint LSP(0.0f,0.0f), LEP(0.0f,0.0f);
	TFPoint RSP(0.0f,0.0f), REP(0.0f,0.0f);

	//박리 검사 
	TFPoint StaPos(0.0f, 0.0f);
	TFPoint EndPos(0.0f, 0.0f);

	double GrindWd1 =0.0f, GrindWd2 =0.0f;
	double GrindMea1=0.0f, GrindMea2=0.0f;

	ImgDegree        =0.0f;

	// Data Init
	FLTPoint.x = 0.0f; //좌측라인과 상단라인의 교차점 좌표
	FLTPoint.y = 0.0f; //좌측라인과 상단라인의 교차점 좌표
	FLBPoint.x = 0.0f; //좌측라인과 하단라인의 교차점 좌표
	FLBPoint.y = 0.0f; //좌측라인과 하단라인의 교차점 좌표

	FLTLPoint.x = 0.0f; //좌측라인과   첫번째 코너 교차점 좌표 
	FLTLPoint.y = 0.0f; //좌측라인과   첫번째 코너 교차점 좌표 
	FLTTPoint.x = 0.0f; //위쪽라인과   첫번째 코너 교차점 좌표 
	FLTTPoint.y = 0.0f; //위쪽라인과   첫번째 코너 교차점 좌표 
	FLBLPoint.x = 0.0f; //좌측라인과   네번째 코너 교차점 좌표 
	FLBLPoint.y = 0.0f; //좌측라인과   네번째 코너 교차점 좌표 
	FLBBPoint.x = 0.0f; //아래쪽라인과 네번째 코너 교차점 좌표 
	FLBBPoint.y = 0.0f; //아래쪽라인과 네번째 코너 교차점 좌표 

	for(int k=0; k<MAX_ROI_ID; k++){
		PolyP1[k].x =0; PolyP1[k].y =0;
		PolyP2[k].x =0; PolyP2[k].y =0;
		PolyP3[k].x =0; PolyP3[k].y =0;
		PolyP4[k].x =0; PolyP4[k].y =0;
	}

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

	TPoint P1(0, 0), P2(0, 0), P3(0, 0), P4(0, 0);

	int PixelCnt = 0;

	int Index = 10;

	bool IsMiddle       = false;
	bool IsGrindMarkUse = false;

	// ThresHold Image Copy 
	if(GImage->GetImagePtr()){
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
	}
	else {
		return false;
	}

	// Polygon 알고리즘 적용시 자재성 문제로 인해서 2개의 비선형 곡선을 찾을경우 구별해야함 
	int nPolyPosCnt=0; int HalfCnt =0; double MaxPolyDis =0.0f; double RealPolyDis=0.0f; double MinPolyDis =100000.0f;
	TFPoint PolyRectCenter; TFPoint PolygonCenter;

	switch (Index) {
		// Find Line
		//----------------------------------------------------------------------
	case 10 :
		//Mobile =>기존 시퀀스 유지 
		//VR  =>전체연마:Polygon 적용, 부분연마:LineFitting 적용 
			if (FTopWall) { 
				EasyImage::Threshold(GImage,&BinaryImg, TopEdgeThresHold);
				if(TopEdgeThresHold==0)  {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
				else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

				ROI_X =0, ROI_Y=0, ROI_W=0, ROI_H=0;
				//Top Shape 기준 ROI 설정 
				if     (IsRectTop  ){
					ROI_X = IRoi->GetOrgX();
					ROI_Y = IRoi->GetOrgY();
					ROI_W = IRoi->GetWidth();
					ROI_H = IRoi->GetHeight();
				}
				else if(IsCornerTop){
					ROI_X = IRoi->GetOrgX()+LTCornerWidth+RoiMargin;
					ROI_Y = IRoi->GetOrgY()                        ;
					ROI_W = (FrameImgW - ROI_X)                    ;
					ROI_H = IRoi->GetHeight()                      ;
				}
				else if(IsCircleTop){
					ROI_X = IRoi->GetOrgX()+LTCircleWidth+RoiMargin;
					ROI_Y = IRoi->GetOrgY()                            ;
					ROI_W = (FrameImgW - ROI_X)                        ;
					ROI_H = IRoi->GetHeight()                          ;
				}

				SingleLine->IRoi->SetOrgX  (ROI_X);
				SingleLine->IRoi->SetOrgY  (ROI_Y);
				SingleLine->IRoi->SetWidth (ROI_W);
				SingleLine->IRoi->SetHeight(ROI_H);

				if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
				if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;

				SingleLine->SetOrientation(orNorth);
				SingleLine->FIncrement =50;
				SingleLine->SetFindLineId(0);
				SingleLine->FThreshold =5;

				if(TopEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
				else               SingleLine->SetEdgeType(1);  // White To Black 

				SingleLine->OnExecute();

				Line[1].FP0 = SingleLine->GetResultP0();
				Line[1].FP1 = SingleLine->GetResultP1();

				TopOutLine[0].FP0 = Line[1].FP0;
				TopOutLine[0].FP1 = Line[1].FP1;
				// 			GImage->Save("D:\\ROTATE_IMG\\신규_HAP.bmp");
			}
			if (FBottomWall) { 
				EasyImage::Threshold(GImage,&BinaryImg, BtmEdgeThresHold);
				if(BtmEdgeThresHold==0)    {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
				else                       {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

				ROI_X =0, ROI_Y=0, ROI_W=0, ROI_H=0;
				//Btm Shape 기준 ROI 설정 
				if     (IsRectBtm  ){
					ROI_X = IRoi->GetOrgX() ;
					ROI_Y = FEndLine-100    ;
					ROI_W = IRoi->GetWidth();
					ROI_H = 200             ;
				}
				else if(IsCornerBtm){
					ROI_X = IRoi->GetOrgX()+LBCornerWidth+RoiMargin    ;
					ROI_Y = FEndLine-100                               ;
					ROI_W = (FrameImgW - ROI_X)                        ;
					ROI_H = 200                                        ;
				}												 
				else if(IsCircleBtm){
					ROI_X = IRoi->GetOrgX()+LBCircleWidth+RoiMargin;
					ROI_Y = FEndLine-100                               ;
					ROI_W = (FrameImgW - ROI_X)                        ;
					ROI_H = 200                                        ;
				}

				SingleLine->IRoi->SetOrgX  (ROI_X);
				SingleLine->IRoi->SetOrgY  (ROI_Y);
				SingleLine->IRoi->SetWidth (ROI_W);
				SingleLine->IRoi->SetHeight(ROI_H);

				if(ROI_X<=0    || ROI_Y< 0    || ROI_W<=0    || ROI_H<=0   ) return false;
				if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;

				SingleLine->SetOrientation(orSouth);
				SingleLine->FIncrement =50;
				SingleLine->SetFindLineId(0);
				SingleLine->FThreshold =5;

				if(BtmEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
				else               SingleLine->SetEdgeType(1);  // White To Black 

				// 기본 BtmLine Search 
				SingleLine->OnExecute();
				Line[3].FP0 = SingleLine->GetResultP0();
				Line[3].FP1 = SingleLine->GetResultP1();

				BtmOutLine[0].FP0 =  Line[3].FP0;
				BtmOutLine[0].FP1 =  Line[3].FP1;
				// 			GImage->Save("D:\\ROTATE_IMG\\신규_CUP.bmp");
			}
			if (FLeftWall) {  
				EasyImage::Threshold(GImage,&BinaryImg, LeftEdgeThresHold);
				if(LeftEdgeThresHold==0) {SingleLine->GImage = GImage;     SingleLine->FThreshold =10;}
				else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

				ROI_X = 0, ROI_Y = 0, ROI_W = 0, ROI_H = 0;
				///////////////////////////////////////////////////
				//(1)모자,컵 기준으로 ROI영역 설정 및 LineFitting 진행 
				///////////////////////////////////////////////////
				if(FShape==shRectH){ // 모자 ROI 설정  
					SingleLine->FIncrement = 10;
					if     (IsRectTop  ){
						ROI_X = IRoi->GetOrgX()  ;
						ROI_W = IRoi->GetWidth() ;
						ROI_Y = IRoi->GetOrgY()  ;
						ROI_H = IRoi->GetHeight();
					}
					else if(IsCornerTop){
						ROI_X = IRoi->GetOrgX()                  ;
						ROI_W = IRoi->GetWidth()                 ;
						ROI_Y = LTCornerHeight + 50              ;
						ROI_H = FrameImgH - (LTCornerHeight + 50);
					}
					else if(IsCircleTop){
						ROI_X = IRoi->GetOrgX()                  ;
						ROI_W = IRoi->GetWidth()                 ;
						ROI_Y = LTCircleHeight + 50              ;
						ROI_H = FrameImgH - (LTCircleHeight + 50);
					}
					// 				GImage->Save("D:\\ROTATE_IMG\\신규_HAP.bmp");
				}
				else if(FShape==shRectC) { // 컵 ROI 설정  
					SingleLine->FIncrement = 1;
					if     (IsRectBtm  ){
						ROI_X = IRoi->GetOrgX()  ;
						ROI_Y = IRoi->GetOrgY()  ;
						ROI_W = IRoi->GetWidth() ;
						ROI_H = IRoi->GetHeight();
					}
					else if(IsCornerBtm){
						ROI_X = IRoi->GetOrgX()          ;
						ROI_Y = IRoi->GetOrgY()          ;
						ROI_W = IRoi->GetWidth()         ;
						ROI_H = FEndLine - LBCornerHeight;
					}
					else if(IsCircleBtm){
						ROI_X = IRoi->GetOrgX()              ;
						ROI_Y = IRoi->GetOrgY()              ;
						ROI_W = IRoi->GetWidth()             ;
						ROI_H = FEndLine - LBCircleHeight;
					}
				}
				else if(FShape==shRectS){ // 스틱 ROI 설정  
					SingleLine->FIncrement = 50;
					ROI_X = IRoi->GetOrgX()  ;
					ROI_Y = IRoi->GetOrgY()  ;
					ROI_W = IRoi->GetWidth() ;
					ROI_H = IRoi->GetHeight();
				}

				SingleLine->IRoi->SetOrgX(ROI_X  );
				SingleLine->IRoi->SetOrgY(ROI_Y  );
				SingleLine->IRoi->SetWidth(ROI_W );
				SingleLine->IRoi->SetHeight(ROI_H);

				if (ROI_X <= 0         || ROI_Y < 0         || ROI_W <= 0         || ROI_H <= 0       ) return false;
				if (ROI_X >= FrameImgW || ROI_Y > FrameImgW || ROI_W >= FrameImgW || ROI_H > FrameImgH) return false;

				SingleLine->SetOrientation(orWest);
				SingleLine->SetFindLineId(0);

				if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
				else                SingleLine->SetEdgeType(1);// White To Black 

				SingleLine->OnExecute();
				Line[0].FP0 = SingleLine->GetResultP0();
				Line[0].FP1 = SingleLine->GetResultP1();

				LeftOutLine[0].FP0 = Line[0].FP0;
				LeftOutLine[0].FP1 = Line[0].FP1;

				///////////////////////////////////////////////////
				// (2)교차점 분류 
				///////////////////////////////////////////////////
				if     (FShape==shRectH){ 
					FLTPoint = Math.OnIntersect(Line[waLeftWall].FP0, Line[waLeftWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
					BuffLTPoint.x = FLTPoint.x;
					BuffLTPoint.y = FLTPoint.y + ImageHeight;
				}
				else if(FShape==shRectC){   
					FLBPoint = Math.OnIntersect(Line[waLeftWall].FP0, Line[waLeftWall].FP1, Line[waBottomWall].FP0, Line[waBottomWall].FP1);
				}

				///////////////////////////////////////////////////
				// (3)Btm Real외곽라인 가상으로 만들어주기 위해서 Top Cam데이터 넘겨줌  
				///////////////////////////////////////////////////
				if(CamId==Left_Top_Cam || CamId==Right_Top_Cam){
					if     (FShape==shRectH){
						RealOutLine[RealOutCnt].FP0.x = LeftOutLine[0].FP0.x;
						RealOutLine[RealOutCnt].FP0.y = FLTPoint.y          ;
						RealOutLine[RealOutCnt].FP1.x = LeftOutLine[0].FP1.x;
						RealOutLine[RealOutCnt].FP1.y = FrameImgH           ;
					}
					else if(FShape==shRectS){
						RealOutLine[RealOutCnt].FP0.x = LeftOutLine[0].FP0.x;
						RealOutLine[RealOutCnt].FP0.y = 0.0f                ;
						RealOutLine[RealOutCnt].FP1.x = LeftOutLine[0].FP1.x;
						RealOutLine[RealOutCnt].FP1.y = FrameImgH           ;
					}
					else if(FShape==shRectC){
						RealOutLine[RealOutCnt].FP0.x = LeftOutLine[0].FP0.x;
						RealOutLine[RealOutCnt].FP0.y = 0.0f                ;
						RealOutLine[RealOutCnt].FP1.x = LeftOutLine[0].FP1.x;
						RealOutLine[RealOutCnt].FP1.y = FLBPoint.y          ;
					}
					RealOutCnt++;
				}

				///////////////////////////////////////////////////
				// (4)CutLine 발생 vs CutLine 미발생  
				///////////////////////////////////////////////////

				if(bCutLine){ //Etching 후 CutLine 발생 =>LeftInPoint,LeftOutPoint 동적 생성 및 데이터 취합  
					//기본라인 기준 최종 ROI 설정 =>CutLine 데이터 추출
					if     (FShape==shRectH){
						if     (IsCornerTop) AllCutWidth = LTCornerWidth;
						else if(IsCircleTop) AllCutWidth = LTCircleWidth;
						else                 AllCutWidth = 100          ;

						P1.x = (int)(FLTPoint.x-50); P1.y =0                     ; P2.x = (int)(P1.x+ AllCutWidth); P2.y = (int)(P1.y);
						P3.x = (int)(FLTPoint.x-50); P3.y =(int)(FrameImgH- P1.y); P4.x = (int)(P2.x             ); P4.y = (int)(P3.y);
					}
					else if(FShape==shRectS){
//						P1.x = (int)(PreLeftOutLine[0].FP0.x-LeftRoiPxMargin); P1.y =0         ; P2.x = (int)(P1.x+LeftRoiWdMargin); P2.y = (int)(P1.y);
//						P3.x = (int)(PreLeftOutLine[0].FP0.x-LeftRoiPxMargin); P3.y =FrameImgH ; P4.x = (int)(P2.x                ); P4.y = (int)(P3.y);

						P1.x = (int)(LeftOutLine[0].FP0.x-LeftRoiPxMargin); P1.y =0         ; P2.x = (int)(P1.x+LeftRoiWdMargin); P2.y = (int)(P1.y);
						P3.x = (int)(LeftOutLine[0].FP0.x-LeftRoiPxMargin); P3.y =FrameImgH ; P4.x = (int)(P2.x                ); P4.y = (int)(P3.y);
					}
					else if(FShape==shRectC){
						if     (IsCornerBtm) AllCutWidth = LBCornerWidth;
						else if(IsCircleBtm) AllCutWidth = LBCircleWidth;
						else                 AllCutWidth = 100          ;

						P1.x = (int)(FLBPoint.x-50); P1.y =0        ; P2.x = (int)(P1.x+ AllCutWidth); P2.y = (int)(P1.y);
						P3.x = (int)(FLBPoint.x-50); P3.y =FrameImgH; P4.x = (int)(P2.x             ); P4.y = (int)(P3.y);
					}

					//CutLine Data 추출 
					if(P1.x!=0 &&  P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
						EasyImage::Threshold(GImage, &BinaryImg, LeftCutThresHold);
						// Median Filter 적용 
						if (P1.x <= 0                           || P1.y < 0          || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0       ) return false;
						if (P1.x >= FrameImgW                   || P1.y >= FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) > FrameImgH) return false;
						if ((P1.x + (P2.x - P1.x)) >= FrameImgW || (P1.y + (P3.y - P1.y)) > FrameImgH) return false;

						FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
						FilterROI.Attach(&BinaryImg);
						FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
						LogUnit.SetLog(L"CutLine==>ScanID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", ScanID, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
						//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
//						EasyImage::Median(&FilterROI, &FilterROI);
						OnFilter(ftMedian, &FilterROI, &FilterROI);

						//							if(FShape==shRectH) GImage->Save("D:\\ROTATE_IMG\\Polygon_Left_HAP_원본.bmp");
						//							if(FShape==shRectS) BinaryImg.Save("D:\\ROTATE_IMG\\Polygon_Left_STICK.bmp");

						Polygon->GImage             = &BinaryImg               ;
						Polygon->IWidth             = FrameImgW                ;
						Polygon->IHeight            = FrameImgH                ;
						Polygon->PoPa.FMinWidth     = 2                        ;
						Polygon->PoPa.FMaxWidth     = (int)(P2.x - P1.x)       ;
						Polygon->PoPa.FMinHeight    = (int)((P3.y - P1.y) / 2) ;
						Polygon->PoPa.FMaxHeight    = (int)((P3.y - P1.y) + 10);
						Polygon->PoPa.MinPxCnt      = 100                      ;
						Polygon->PoPa.MaxPxCnt      = 20000                    ;
						Polygon->PoPa.MaxBbCnt      = 10                       ;
						Polygon->PoPa.IsSeparate    = true                     ;
						Polygon->PoPa.IsInToOut     = false                    ;
						Polygon->PoPa.HiThHold      = LeftCutThresHold         ;
						Polygon->PoPa.LoThHold      = 0                        ;
						Polygon->PoPa.IgInSide      = true                     ;
						Polygon->PoPa.IsHorSort     = false                    ;
						Polygon->PoPa.IsVerSort     = true                     ;
						Polygon->PoPa.IsInToLeft    = true                     ;
						Polygon->PoPa.TopFirstPoint = true                     ; 
						Polygon->PoPa.BtmFirstPoint = false                    ;

						// 화면 디스플레이 목적 로컬 데이터 전역변수 넘겨줌 
						PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
						PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
						PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
						PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

						if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
							Polygon->OnExecute(P1, P2, P3, P4);
						}

						// 2개이상 비선형 곡선 데이터 검출시 분류 목적 
						if (Polygon->BlobCount > 1) {
							// Polygon 데이터 2개이상 존재시 최소값 기준 분류 시퀀스 
							PolyRectCenter.x = 0.0f, PolyRectCenter.y = 0.0f, nPolyPosCnt = 0, RealPolyDis = 0.0f, MinPolyDis = 1000000.0f;
							PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y) / 2);
							for (int k = 0; k < Polygon->BlobCount; k++) {
								PolygonCenter.x = Polygon->Blob[k].Rect.l;
								PolygonCenter.y = Polygon->Blob[k].Rect.t + (Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t) / 2;
								// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
								RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
								if (RealPolyDis < MinPolyDis) {
									MinPolyDis = RealPolyDis;
									nPolyPosCnt = k;
								}
							}
						}
						else nPolyPosCnt = 0;
						// 새로운(Buffer기준 Y축 동일위치에 좌표존재시 제거함) 방법으로 Out/In Data 분류 및 데이터 생성 
						////////////////////////////////////////////////////////////////////
						if (Polygon->Blob[nPolyPosCnt].InCnt > 0) {
							LeftInEdgeCount = 0;
							for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
								if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
								if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

								if (FShape == shRectH) { // Hap 검사시 교차점 기준 꺽이는 좌표로 인해서 추가적인 조건 추가 
									//										if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
									if (!IsRectTop && (fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x) <= PolygonMarin)) continue;

								}
								else if (FShape == shRectC) {
									//										if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
									if (!IsRectBtm && (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x) <= PolygonMarin)) continue;
								}
								LeftInEdgeCount++;
							}

							if (LeftInEdgeCount > 0) {
								if (LeftInPoint != NULL) delete[] LeftInPoint;
								LeftInPoint = new TFPoint[LeftInEdgeCount];

								PixelCnt = 0;
								for (int k = 0; k < LeftInEdgeCount; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

									if (FShape == shRectH) { // Hap 검사시 교차점 기준 꺽이는 좌표로 인해서 추가적인 조건 추가 
										//											if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
										if (!IsRectTop && (fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x) <= PolygonMarin)) continue;
									}
									else if (FShape == shRectC) {
										//											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
										if (!IsRectBtm && (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x) <= PolygonMarin)) continue;
									}
									LeftInPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].InPoint[k].x;
									LeftInPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].InPoint[k].y;
									PixelCnt++;
								}
							}
						}

						if (Polygon->Blob[nPolyPosCnt].OutCnt > 0) {
							LeftOutEdgeCount = 0;
							for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
								if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
								if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

								if (FShape == shRectC) { // Cup검사시 교차점 기준 꺽이는 좌표로 인해서 추가적인 조건 추가 
									if (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x) <= PolygonMarin) continue;
								}
								LeftOutEdgeCount++;
							}

							if (LeftOutEdgeCount > 0) {
								if (LeftOutPoint != NULL) delete[] LeftOutPoint;
								LeftOutPoint = new TFPoint[LeftOutEdgeCount];

								PixelCnt = 0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

									if (FShape == shRectC) { // Cup검사시 교차점 기준 꺽이는 좌표로 인해서 추가적인 조건 추가 
										if (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x) <= PolygonMarin) continue;
									}
									LeftOutPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].OutPoint[k].x;
									LeftOutPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].OutPoint[k].y;
									PixelCnt++;
								}
							}
						}
						////////////////////////////////////////////////////////////////////

						// Frmae 기준 개별 데이터 전체 데이터에 넘겨줌 
						if (LeftInEdgeCount > 0 && LeftOutEdgeCount > 0) {
							// Total Left In Data 
							for (int k = 0; k < LeftInEdgeCount; k++) {
								if (AllLeftInEdgeCount > MAX_ALL_EDGE_COUNT) return false;
								AllLeftInData[AllLeftInEdgeCount].x = LeftInPoint[k].x;
								AllLeftInData[AllLeftInEdgeCount].y = LeftInPoint[k].y + ImageHeight;
								AllLeftInEdgeCount++;
							}
							// Total Left Out Data 
							for (int k = 0; k < LeftOutEdgeCount; k++) {
								if (AllLeftOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
								AllLeftOutData[AllLeftOutEdgeCount].x = LeftOutPoint[k].x;
								AllLeftOutData[AllLeftOutEdgeCount].y = LeftOutPoint[k].y + ImageHeight;
								AllLeftOutEdgeCount++;
							}
						}
					}
				}
				else        { //Etching 후 CutLine 미발생 =>H,C Shape 기준 LTCorOutPoint,LTCirOutPoint,LBCorOutPoint,LBCirOutPoint 데이터 취합
					if     (FShape==shRectH){
						if     (IsRectTop  ){
							LTDx = 0.0f;
							LTDy = 0.0f;
							//화면에 디스플레이 위해서 
							FLTLPoint.x = FLTPoint.x;
							FLTLPoint.y = FLTPoint.y;
							FLTTPoint.x = FLTPoint.x;
							FLTTPoint.y = FLTPoint.y;
						}
						else if(IsCornerTop){
							EasyImage::Threshold(GImage, &BinaryImg, CornerThresHold);

							if(CornerThresHold==0) {SingleLine->GImage = GImage    ; SingleLine->FThreshold =5 ;}
							else        		   {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

							if(LTCornerWidth!=0 && LTCornerHeight!=0){
								//ROI 영역 설정 
								SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x-10   );
								SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y      );
								SingleLine->IRoi->SetWidth ((int)LTCornerWidth+20);
								SingleLine->IRoi->SetHeight((int)LTCornerHeight  );

								//Parameter Setting 
								if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
								else                SingleLine->SetEdgeType(1);// White To Black 

								SingleLine->SetOrientation(orWest);
								SingleLine->FIncrement =1         ;
								SingleLine->FThreshold =15        ;
								SingleLine->SetFindLineId(0)      ;
								SingleLine->OnExecute()           ;

								Line[4].FP0 = SingleLine->GetResultP0();
								Line[4].FP1 = SingleLine->GetResultP1();

								// Corner Chamber_Cut 교차점 
								FLTLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
								FLTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

								if((FLTLPoint.x==0 || FLTLPoint.y==0 || FLTTPoint.x==0 || FLTTPoint.y==0) || (SingleLine->FEdgePointCount <30)){
									LTDx = 0.0f;
									LTDy = 0.0f;
									//화면에 디스플레이 위해서 
									FLTLPoint.x = FLTPoint.x;
									FLTLPoint.y = FLTPoint.y;
									FLTTPoint.x = FLTPoint.x;
									FLTTPoint.y = FLTPoint.y;
								}
								else { //Corner Chamber Cut 교차점 산출 성공
									LTDx = Math.GetLengthFrPointToPoint(FLTPoint, FLTTPoint);
									LTDy = Math.GetLengthFrPointToPoint(FLTPoint, FLTLPoint);

									// OutLine C1Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(LTCorOutPoint!=NULL) delete[] LTCorOutPoint;
										LTCorOutPoint     = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										LTCorOutEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											LTCorOutPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											LTCorOutPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									//교차점 근처 Mark에서 PrintScreen or ChamberCut Line까지 거리 산출시 필요할수도 있음 
									if(LTCorOutEdgeCount>0){
										// Total Corner Out Data 
										for(int k=0; k<LTCorOutEdgeCount; k++){
											if(AllCorOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                 ;
											AllCornerOutData[AllCorOutEdgeCount].x = LTCorOutPoint[k].x            ;
											AllCornerOutData[AllCorOutEdgeCount].y = LTCorOutPoint[k].y+ImageHeight;
											AllCorOutEdgeCount++;
										}
									}
								}
							}
						}
						else if(IsCircleTop){
							EasyImage::Threshold(GImage, &BinaryImg, CircleThresHold);

							if(LTCircleWidth!=0 && LTCircleHeight!=0){
								P1.x=0,P1.y=0,P2.x=0,P2.y=0,P3.x=0,P3.y=0,P4.x=0,P4.y=0;
								P1.x = (int)(FLTPoint.x - 20); P1.y = (int)(FLTPoint.y -  (FLTPoint.y / 2)); P2.x = (int)(P1.x + LTCircleWidth + 20); P2.y = (int)(P1.y);
								P3.x = (int)(FLTPoint.x - 20); P3.y = (int)(P1.y + LTCircleHeight + 20); P4.x = (int)(P2.x);                          P4.y = (int)(P3.y);

								//검사 ROI Display 
								LTROI.left  = P1.x;   LTROI.top    = P1.y;
								LTROI.right = P2.x;   LTROI.bottom = P3.y;

								//Median Filter 적용 
								if(P1.x<=0                         || P1.y<=0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
							    if(P1.x>=FrameImgW                 || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)>=FrameImgH) return false;
								if((P1.x+(P2.x - P1.x))>=FrameImgW || (P1.y+(P3.y - P1.y))>FrameImgH                                         ) return false;

								FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
								FilterROI.Attach(&BinaryImg)                                          ;
								FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
								LogUnit.SetLog(L"LTCircle==>ScanID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", ScanID, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
								OnFilter(ftMedian, &FilterROI,&FilterROI)                             ;


								Polygon->GImage             = &BinaryImg               ;
								Polygon->IWidth             = FrameImgW                ;
								Polygon->IHeight            = FrameImgH                ;
								Polygon->PoPa.FMinWidth     = 2                        ;
								Polygon->PoPa.FMaxWidth     = (int)(P2.x - P1.x)       ;
								Polygon->PoPa.FMinHeight    = (int)((P3.y - P1.y) / 2) ;
								Polygon->PoPa.FMaxHeight    = (int)((P3.y - P1.y) + 10);
								Polygon->PoPa.MinPxCnt      = 100                      ;
								Polygon->PoPa.MaxPxCnt      = 20000                    ;
								Polygon->PoPa.MaxBbCnt      = 10                       ;
								Polygon->PoPa.IsSeparate    = true                     ;
								Polygon->PoPa.IsInToOut     = false                    ;
								Polygon->PoPa.HiThHold      = CircleThresHold          ;
								Polygon->PoPa.LoThHold      = 0                        ;
								Polygon->PoPa.IgInSide      = true                     ;
								Polygon->PoPa.IsHorSort     = false                    ;
								Polygon->PoPa.IsVerSort     = true                     ;
								Polygon->PoPa.IsInToLeft    = true                     ;
								Polygon->PoPa.TopFirstPoint = true                     ; 
								Polygon->PoPa.BtmFirstPoint = false                    ;

								if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
									Polygon->OnExecute(P1, P2, P3, P4);
								}

								// 2개이상 비선형 곡선 데이터 검출시 분류 목적 
								if(Polygon->BlobCount>1){
									// Polygon 데이터 2개이상 존재시 최소값 기준 분류 시퀀스 
									PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f,MinPolyDis=1000000.0f;
									PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
									for(int k=0; k<Polygon->BlobCount; k++){
										PolygonCenter.x = Polygon->Blob[k].Rect.l;
										PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
										RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
										if(RealPolyDis<MinPolyDis) {
											MinPolyDis  = RealPolyDis;
											nPolyPosCnt = k;
										}
									}
								}
								else nPolyPosCnt=0;

								if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
									LTCirOutEdgeCount=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										LTCirOutEdgeCount++;
									}

									if(LTCirOutEdgeCount>0){
										if(LTCirOutPoint!=NULL) delete[] LTCirOutPoint;
										LTCirOutPoint = new TFPoint[LTCirOutEdgeCount];

										PixelCnt=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
											LTCirOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
											LTCirOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
											PixelCnt++;
										}
									}
								}
								////////////////////////////////////////////////////////////////////

								// Frmae 기준 개별 데이터 전체 데이터에 넘겨줌 
								if(LTCirOutEdgeCount>0){
									// Total Corner Out Data 
									for(int k=0; k<LTCirOutEdgeCount; k++){
										if(AllCirOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                 ;
										AllCircleOutData[AllCirOutEdgeCount].x = LTCirOutPoint[k].x            ;
										AllCircleOutData[AllCirOutEdgeCount].y = LTCirOutPoint[k].y+ImageHeight;
										AllCirOutEdgeCount++;
									}
								}
							}
						}
					}
					else if(FShape==shRectC){
						if     (IsRectBtm  ){
							LBDx = 0.0f;
							LBDy = 0.0f;
							//화면에 디스플레이 위해서 
							FLBLPoint.x = FLBPoint.x;
							FLBLPoint.y = FLBPoint.y;
							FLBBPoint.x = FLBPoint.x;
							FLBBPoint.y = FLBPoint.y;
						}
						else if(IsCornerBtm){
							EasyImage::Threshold(GImage, &BinaryImg, CornerThresHold);

							if(CornerThresHold==0) {SingleLine->GImage = GImage    ; SingleLine->FThreshold =5 ;}
							else        		   {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

							if(LBCornerWidth!=0 && LTCornerHeight!=0){
								SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x-10                    );
								SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y-LBCornerHeight        );
								SingleLine->IRoi->SetHeight((int)LBCornerHeight                   );
								SingleLine->IRoi->SetWidth ((int)LBCornerWidth+LBCornerWidthOffset);

								//Parameter Setting 
								if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
								else                SingleLine->SetEdgeType(1);// White To Black 

								SingleLine->SetOrientation(orWest);
								SingleLine->FIncrement =1;
								SingleLine->FThreshold =15;
								SingleLine->SetFindLineId(0);
								SingleLine->OnExecute();

								Line[7].FP0 = SingleLine->GetResultP0();
								Line[7].FP1 = SingleLine->GetResultP1();

								// Corner Chamber_Cut 교차점 
								FLBLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall   ].FP0, Line[waLeftWall   ].FP1);
								FLBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);

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

									// OutLine C1Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(LBCorOutPoint !=NULL) delete[] LBCorOutPoint;
										LBCorOutPoint     = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										LBCorOutEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											LBCorOutPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											LBCorOutPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									//교차점 근처 Mark에서 PrintScreen or ChamberCut Line까지 거리 산출시 필요할수도 있음 
									if(LBCorOutEdgeCount>0){
										// Total Corner Out Data 
										for(int k=0; k< LBCorOutEdgeCount; k++){
											if(AllCorOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                 ;
											AllCornerOutData[AllCorOutEdgeCount].x = LBCorOutPoint[k].x            ;
											AllCornerOutData[AllCorOutEdgeCount].y = LBCorOutPoint[k].y+ImageHeight;
											AllCorOutEdgeCount++;
										}
									}
								}
							}
						}
						else if(IsCircleBtm){
							EasyImage::Threshold(GImage, &BinaryImg, CircleThresHold);

							if (LBCircleWidth!=0 && LBCircleHeight!=0) {
								P1.x=0,P1.y=0,P2.x=0,P2.y=0,P3.x=0,P3.y=0,P4.x=0,P4.y=0;

								P1.x = (int)(FLBPoint.x-20);    P1.y =(int)(FLBPoint.y-LBCircleHeight-20); P2.x = (int)(P1.x+LTCircleWidth+20);    P2.y = (int)(P1.y);
								P3.x = (int)(FLBPoint.x-20);    P3.y =(int)(P1.y+LTCircleHeight+40      ); P4.x = (int)(P2.x                     );    P4.y = (int)(P3.y);

								// 검사화면 Display 목적 
								LBROI.left  = P1.x;   LBROI.top    = P1.y;
								LBROI.right = P2.x;   LBROI.bottom = P3.y;

								//Median Filter 적용 
								if(P1.x<=0                         || P1.y<=0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
							    if(P1.x>=FrameImgW                 || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)>=FrameImgH) return false;
								if((P1.x+(P2.x - P1.x))>=FrameImgW || (P1.y+(P3.y - P1.y))>FrameImgH                                         ) return false;

								FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
								FilterROI.Attach(&BinaryImg)                                          ;
								FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
								LogUnit.SetLog(L"LBCircle==>ScanID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", ScanID, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
								OnFilter(ftMedian, &FilterROI,&FilterROI)                             ;

								Polygon->GImage             = &BinaryImg               ;
								Polygon->IWidth             = FrameImgW                ;
								Polygon->IHeight            = FrameImgH                ;
								Polygon->PoPa.FMinWidth     = 2                        ;
								Polygon->PoPa.FMaxWidth     = (int)(P2.x - P1.x)       ;
								Polygon->PoPa.FMinHeight    = (int)((P3.y - P1.y) / 2) ;
								Polygon->PoPa.FMaxHeight    = (int)((P3.y - P1.y) + 10);
								Polygon->PoPa.MinPxCnt      = 100                      ;
								Polygon->PoPa.MaxPxCnt      = 20000                    ;
								Polygon->PoPa.MaxBbCnt      = 10                       ;
								Polygon->PoPa.IsSeparate    = true                     ;
								Polygon->PoPa.IsInToOut     = false                    ;
								Polygon->PoPa.HiThHold      = CircleThresHold          ;
								Polygon->PoPa.LoThHold      = 0                        ;
								Polygon->PoPa.IgInSide      = true                     ;
								Polygon->PoPa.IsHorSort     = false                    ;
								Polygon->PoPa.IsVerSort     = true                     ;
								Polygon->PoPa.IsInToLeft    = true                     ;
								Polygon->PoPa.TopFirstPoint = true                     ; 
								Polygon->PoPa.BtmFirstPoint = false                    ;

								if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
									Polygon->OnExecute(P1, P2, P3, P4);
								}

								// 2개이상 비선형 곡선 데이터 검출시 분류 목적 
								if(Polygon->BlobCount>1){
									// Polygon 데이터 2개이상 존재시 최소값 기준 분류 시퀀스 
									PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f,MinPolyDis=1000000.0f;
									PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
									for(int k=0; k<Polygon->BlobCount; k++){
										PolygonCenter.x = Polygon->Blob[k].Rect.l;
										PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
										RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
										if(RealPolyDis<MinPolyDis) {
											MinPolyDis  = RealPolyDis;
											nPolyPosCnt = k;
										}
									}
								}
								else nPolyPosCnt=0;
								    
								//LBCircle OutData Calculate 
								if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
									LBCirOutEdgeCount=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										// Cup검사시 교차점 기준 꺽이는 좌표로 인해서 추가적인 조건 추가 
										if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;

										LBCirOutEdgeCount++;
									}

									if(LBCirOutEdgeCount>0){
										if(LBCirOutPoint!=NULL) delete[] LBCirOutPoint;
										LBCirOutPoint = new TFPoint[LBCirOutEdgeCount];

										PixelCnt=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
											// Cup검사시 교차점 기준 꺽이는 좌표로 인해서 추가적인 조건 추가 
											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;

											LBCirOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
											LBCirOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
											PixelCnt++;
										}
									}
								}
								////////////////////////////////////////////////////////////////////


								// Frmae 기준 개별 데이터 전체 데이터에 넘겨줌 
								if(LBCirOutEdgeCount>0){
									// Total Corner Out Data 
									for(int k=0; k<LBCirOutEdgeCount; k++){
										if(AllCirOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                 ;
										AllCircleOutData[AllCirOutEdgeCount].x = LBCirOutPoint[k].x            ;
										AllCircleOutData[AllCirOutEdgeCount].y = LBCirOutPoint[k].y+ImageHeight;
										AllCirOutEdgeCount++;
									}
								}
							}
						}
					}
				}

				///////////////////////////////////////////////////
				//(5)Print Screen Data 추출
				///////////////////////////////////////////////////
				if(CamId==Left_Btm_Cam || CamId==Right_Btm_Cam){ //Btm영역 Print Screen 도포함 ==>PrintScPoint 데이터 취합
					if(LeftPrintThresHold!=0){
						if     (FShape==shRectH){
							P1.x = (int)(FLTPoint.x+PrintScOffset); P1.y =FLTPoint.y            ; P2.x = (int)(P1.x+ PrintScRange); P2.y = (int)(P1.y);
							P3.x = (int)(FLTPoint.x+PrintScOffset); P3.y =(int)(FrameImgH- P1.y); P4.x = (int)(P2.x              ); P4.y = (int)(P3.y);
						}
						else if(FShape==shRectS){
							if(LeftOutLine[0].FP0.x==0.0f || LeftOutLine[0].FP0.x==0.0f || LeftOutLine[0].FP0.x==0.0f || LeftOutLine[0].FP0.x==0.0f){
								P1.x = (int)(PreLeftOutLine[0].FP0.x+PrintScOffset); P1.y =0         ; P2.x = (int)(P1.x+PrintScRange); P2.y = (int)(P1.y);
								P3.x = (int)(PreLeftOutLine[0].FP0.x+PrintScOffset); P3.y =FrameImgH ; P4.x = (int)(P2.x             ); P4.y = (int)(P3.y);
							}
							else                                                                                                                     {
								P1.x = (int)(LeftOutLine[0].FP0.x + PrintScOffset); P1.y = 0       ; P2.x = (int)(P1.x + PrintScRange); P2.y = (int)(P1.y);
								P3.x = (int)(LeftOutLine[0].FP0.x + PrintScOffset); P3.y =FrameImgH; P4.x = (int)(P2.x               ); P4.y = (int)(P3.y);
							}
						}
						else if(FShape==shRectC){
							P1.x = (int)(FLBPoint.x + PrintScOffset); P1.y = 0        ; P2.x = (int)(P1.x + PrintScRange); P2.y = (int)(P1.y);
							P3.x = (int)(FLBPoint.x + PrintScOffset); P3.y =FLBPoint.y; P4.x = (int)(P2.x               ); P4.y = (int)(P3.y);
						}

						//Print Screen 검출 ROI 설정 후 검사 진행함 
						if(P1.x!=0 &&  P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
							EasyImage::Threshold(GImage, &BinaryImg, LeftPrintThresHold);
							// Median Filter 적용 
							if (P1.x <= 0                           || P1.y < 0          || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0       ) return false;
							if (P1.x >= FrameImgW                   || P1.y >= FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) > FrameImgH) return false;
							if ((P1.x + (P2.x - P1.x)) >= FrameImgW || (P1.y + (P3.y - P1.y)) > FrameImgH) return false;

							FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
							FilterROI.Attach(&BinaryImg);
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"PrintScreen==>ScanID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", ScanID, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
//							EasyImage::Median(&FilterROI, &FilterROI);
							OnFilter(ftMedian, &FilterROI, &FilterROI);

							Polygon->GImage             = &BinaryImg               ;
							Polygon->IWidth             = FrameImgW                ;
							Polygon->IHeight            = FrameImgH                ;
							Polygon->PoPa.FMinWidth     = 2                        ;
							Polygon->PoPa.FMaxWidth     = (int)(P2.x - P1.x)       ;
							Polygon->PoPa.FMinHeight    = (int)((P3.y - P1.y) / 2) ;
							Polygon->PoPa.FMaxHeight    = (int)((P3.y - P1.y) + 10);
							Polygon->PoPa.MinPxCnt      = 100                      ;
							Polygon->PoPa.MaxPxCnt      = 20000                    ;
							Polygon->PoPa.MaxBbCnt      = 10                       ;
							Polygon->PoPa.IsSeparate    = true                     ;
							Polygon->PoPa.IsInToOut     = false                    ;
							Polygon->PoPa.HiThHold      = LeftPrintThresHold       ;
							Polygon->PoPa.LoThHold      = 0                        ;
							Polygon->PoPa.IgInSide      = true                     ;
							Polygon->PoPa.IsHorSort     = false                    ;
							Polygon->PoPa.IsVerSort     = true                     ;
							Polygon->PoPa.IsInToLeft    = true                     ;
							Polygon->PoPa.TopFirstPoint = true                     ; 
							Polygon->PoPa.BtmFirstPoint = true                     ;
//							Polygon->PoPa.BtmFirstPoint = false                    ;
//							if     (FShape==shRectH) Polygon->GImage->Save("D:\\ROTATE_IMG\\HAT.bmp");
//							else if(FShape==shRectC) Polygon->GImage->Save("D:\\ROTATE_IMG\\CUP.bmp");

							// 화면 디스플레이 목적 로컬 데이터 전역변수 넘겨줌 
							PrintROI.left  = P1.x;   PrintROI.top    = P1.y;
							PrintROI.right = P2.x;   PrintROI.bottom = P3.y;

							if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
								Polygon->OnExecute(P1, P2, P3, P4);
							}

							// 2개이상 비선형 곡선 데이터 검출시 분류 목적 
							if (Polygon->BlobCount > 1) {
								// Polygon 데이터 2개이상 존재시 최소값 기준 분류 시퀀스 
								PolyRectCenter.x = 0.0f, PolyRectCenter.y = 0.0f, nPolyPosCnt = 0, RealPolyDis = 0.0f, MinPolyDis = 1000000.0f;
								PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y) / 2);
								for (int k = 0; k < Polygon->BlobCount; k++) {
									PolygonCenter.x = Polygon->Blob[k].Rect.l;
									PolygonCenter.y = Polygon->Blob[k].Rect.t + (Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t) / 2;
									// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
									RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
									if (RealPolyDis < MinPolyDis) {
										MinPolyDis = RealPolyDis;
										nPolyPosCnt = k;
									}
								}
							}
							else nPolyPosCnt = 0;

							// 새로운(Buffer기준 Y축 동일위치에 좌표존재시 제거함)방법 적용하여 Out 데이터 생성 
							if (Polygon->Blob[nPolyPosCnt].OutCnt > 0) {
								PrintScEdgeCount = 0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

									if (FShape == shRectC) { // Cup검사시 교차점 기준 꺽이는 좌표로 인해서 추가적인 조건 추가 
										if (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x) <= PolygonMarin) continue;
									}
									PrintScEdgeCount++;
								}

								if (PrintScEdgeCount > 0) {
									if (PrintScPoint != NULL) delete[] PrintScPoint;
									PrintScPoint = new TFPoint[PrintScEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
										if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

										if (FShape == shRectC) { // Cup검사시 교차점 기준 꺽이는 좌표로 인해서 추가적인 조건 추가 
											if (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x) <= PolygonMarin) continue;
										}
										PrintScPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].OutPoint[k].x;
										PrintScPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].OutPoint[k].y;
										PixelCnt++;
									}
								}
							}
							////////////////////////////////////////////////////////////////////

							// Frmae 기준 개별 데이터 전체 데이터에 넘겨줌 
							if (PrintScEdgeCount > 0) {
								// Total Print Out Data 
								for (int k = 0; k < PrintScEdgeCount; k++) {
									if (AllLeftOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllPrintScData[AllPrintScEdgeCount].x = PrintScPoint[k].x;
									AllPrintScData[AllPrintScEdgeCount].y = PrintScPoint[k].y + ImageHeight;
									AllPrintScEdgeCount++;
								}
							}
						}
				    }
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
			}
			if (FRightWall) {  //개발 프로젝트(PC2대)_(LineScan4K X 4) ==>Glass 가장자리(A,B,C,D)검사 시 필요없음 

			}
			if (FLeftTop    ){ //개발 프로젝트(PC2대)_(LineScan4K X 4) ==>Glass 가장자리(A,B,C,D)검사 시 필요없음 

			}
			if (FRightTop   ){ //개발 프로젝트(PC2대)_(LineScan4K X 4) ==>Glass 가장자리(A,B,C,D)검사 시 필요없음 

			}
			if (FRightBottom){ //개발 프로젝트(PC2대)_(LineScan4K X 4) ==>Glass 가장자리(A,B,C,D)검사 시 필요없음 

			}
			if (FLeftBottom ){ //개발 프로젝트(PC2대)_(LineScan4K X 4) ==>Glass 가장자리(A,B,C,D)검사 시 필요없음 
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
			//Edge->GImage = &DefectImg;
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
void CFM14xLineDefect::OnExecutePrint(TFPoint StaPos, TFPoint EndPos, int InspMode, bool bCutLine)
{
	if(AllPrintScEdgeCount==0) {AfxMessageBox(L"Print Edge Count Error"); return;}
	int FrameImgW   = GImage->GetWidth ();
	int FrameImgH   = GImage->GetHeight();

	double GlassDeg= 0.0f;
	double Orgdeg  = 0.0f;
	double Newdeg  = 0.0f;
	double PrintDeg= 0.0f;
	double MaxScore= 0.0f;
	double MarkDeg = 0.0f;

	int MaxGrindID = 0;
	int MarkID     = 0;

	// ASide EnCap Pattern Distance 측정 변수 
	TFPoint StaPatern(0.0f,0.0f);
	TFPoint EndPatern(0.0f,0.0f);
	const int Magin =50;
	int PatternLength;

	// ASide Align Mart To Edge 측정 변수
	TFPoint StaMarkToEdge(0.0f,0.0f);
	TFPoint EndMarkToEdge(0.0f,0.0f);
	TFPoint CellHalfPos(0.0f,0.0f);

	const int MarkToEdgeMargin =35;
	int MarkToEdgeLength;
	double MarkToEdgeDeg=0.0f;

	// Frame기준 비선형 곡선데이터 취합시 Overlap및 검사 종료시점 Buffer오버랩 으로 인해서 
	// Polygon 데이터 튀는 현상 간훨적 발생,데이터 튀는 현상 방지목적 변수 
	const int XPosMargin = 20; const int YPosMargin =20;

	TFPoint Target(0.0f,0.0f);
	// DvX,DvY 설정값을 이미지 회전 기준으로 변경하기 위해서 필요한 변수 
	TFPoint OrgMarkDv; TFPoint NewMarkDv; 

	// EdgePoint Data 포인터 
	TFPoint   *AllOutPoint=NULL;
	TFPoint   *AllPrint   =NULL;

	//TEST
	TFPoint BufferPos   (0.0f, 0.0f);
	TFPoint MinBufferPos(0.0f, 0.0f);

	double PrintEdgeDis =0.0f, MinDis =10000.0f, TotalPrintDis=0.0f,PrintShiftDis=0.0f, DvDeg=0.0f,MinDeg=360.0f;
	int PrintEdgeCnt =0, PrePrintEdgeCnt =0, PrintShiftCnt =0, PrePrintShiftCnt=0;
    int AllOutCnt    =0, AllPrintCnt     =0, PrintCnt      =0;
	bool bVerDis = false;

	//기준 각도 및 
	Orgdeg = dSideDeg; //PrintMark Dx,Dy 설정시 기준 각도 
	if(StaPos.x!=0.0f && StaPos.y!=0.0f && EndPos.x!=0.0f && EndPos.y!=0.0f) GlassDeg = (Math.GetDegree(StaPos.x , StaPos.y, EndPos.x, EndPos.y));
	Newdeg = GlassDeg - Orgdeg;

	//Print Screen Data 
	AllPrint    = AllPrintScData     ;
	AllPrintCnt = AllPrintScEdgeCount;

	if(bCutLine){ //Cut Line 발생 
		AllOutPoint = AllLeftOutData     ;
		AllOutCnt   = AllLeftOutEdgeCount;
	}
	else        { //Cut Line 미발생 
		if     (InspMode==Rect_Shape  ){AllOutPoint = NULL            ; AllOutCnt=0;                 }
		else if(InspMode==Corner_Shape){AllOutPoint = AllCornerOutData; AllOutCnt=AllCorOutEdgeCount;}
		else if(InspMode==Circle_Shape){AllOutPoint = AllCircleOutData; AllOutCnt=AllCirOutEdgeCount;}
	}

	// PrintMark DvX,DvY기준으로 영역 설정
	// PrintMark Range 적용하여 PrintRect영역 설정 
	for(int k=0; k< MAX_PRINT_MARK_COUNT; k++){
		if(PrintMark[k].DvX==0 || PrintMark[k].DvY==0) continue;

		//기준 각도 대비 현재 Glass 기준으로 각도 변환 및
		OrgMarkDv.x = (float)PrintMark[k].DvX; OrgMarkDv.y = (float)PrintMark[k].DvY;
		NewMarkDv = Math.OnRotate(StaPos, OrgMarkDv, Newdeg);
		PrintMark[k].DvX = (int)NewMarkDv.x; PrintMark[k].DvY = (int)NewMarkDv.y;

		Target.x = StaPos.x ; Target.y = 0  ;
		Target.x = Target.x+PrintMark[k].DvX;
		Target.y = Target.y+PrintMark[k].DvY;

		PrintMark[k].Rect.left   = Target.x - PrintMark[k].Range;
		PrintMark[k].Rect.top    = Target.y - PrintMark[k].Range;
		PrintMark[k].Rect.right  = PrintMark[k].Rect.left+(PrintMark[k].Range*2); 
		PrintMark[k].Rect.bottom = PrintMark[k].Rect.top +(PrintMark[k].Range*2); 

		if(PrintMark[k].MarkID>MaxGrindID) MaxGrindID = PrintMark[k].MarkID; 
	}

	// PrintMark Find 및 연마,가공량 측정 
// 	Find->OnLoad(true,FindDir)        ;
	// 설정된 Multi_Teaching 수량만큼 데이터 로딩 및 기본값 설정함 
	for(int k=0; k<(MaxGrindID+1); k++){
		if(k>=MAX_GRINDMARK_ID) continue;
		MarkFind[k].OnLoad(true,FindDir,k);

		MarkFind[k].GImage = GImage                          ;
		MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
		MarkFind[k].SetMinScore (GrindMarkMinScore)          ; // PatternrFind 검사 최소 Score
		MarkFind[k].SetZeroScore(GrindMarkMinScore)          ; // 검사 후 최소 Score 
		MarkFind[k].SetMaxInstances(1)                       ;
		MarkFind[k].SetAngleTolerance(20.0f)                 ;
		MarkFind[k].SetScaleBias(1.00f)                      ;
		MarkFind[k].SetScaleTolerance(0.0f)                  ;
		MarkFind[k].Property.Ox = (float)FrameImgW/2         ;
		MarkFind[k].Property.Oy = (float)FrameImgH/2         ;
	}

	int PrintRectWidth=0, PrintRectHeight=0;
//    GImage->Save("D:\\ROTATE_IMG\\AllBuffer.bmp")        ;
	//(1)Multi Mark Search =>
	for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
		if(PrintMark[k].Rect.left==0 || PrintMark[k].Rect.top==0 || PrintMark[k].Rect.right==0 || PrintMark[k].Rect.bottom==0) continue;
		PrintRectWidth  = (PrintMark[k].Rect.right  - PrintMark[k].Rect.left);
		PrintRectHeight = (PrintMark[k].Rect.bottom - PrintMark[k].Rect.top );

		if(PrintMark[k].Rect.left<=0         || PrintMark[k].Rect.top<=0         || PrintRectWidth<=0         || PrintRectHeight<=0        ) continue;
		if(PrintMark[k].Rect.left>=FrameImgW || PrintMark[k].Rect.top>=FrameImgH || PrintRectWidth>=FrameImgW || PrintRectHeight>=FrameImgH) continue;
		if(PrintMark[k].Point.x!=0.0f && PrintMark[k].Point.y!=0.0f) continue;

		MarkID  = PrintMark[k].MarkID; // Multi Teaching MarkID 
		MarkDeg = PrintMark[k].degree;
		MarkFind[MarkID].IRoi->SetPlacement(PrintMark[k].Rect.left, PrintMark[k].Rect.top, PrintRectWidth, PrintRectHeight);

		MarkFind[MarkID].SetAngleBias(MarkDeg);
		PrintMark[k].Score = 0.0f;

		//사용자 설정 ID 기준 Mark Search  
		if (MarkFind[MarkID].OnExecute()) { //Print Mark Search OK 
			PrintMark[k].Point.x = MarkFind[MarkID].Result[0].Px;
			PrintMark[k].Point.y = MarkFind[MarkID].Result[0].Py;
			PrintMark[k].Score   = MarkFind[MarkID].Result[0].Score;
		}
		else                              { //Print Mark Search NG (등록된 여러개 Mark 다시 Search 진행 
			MaxScore = 0.0f;
			for(int j=0; j<MaxGrindID+1; j++){
				MarkFind[j].IRoi->SetPlacement(PrintMark[k].Rect.left, PrintMark[k].Rect.top, PrintRectWidth, PrintRectHeight);
				MarkFind[j].SetAngleBias(MarkDeg);

				// Multi Mark Search 진행할때 최고 Score값 기준으로 데이터 추출 
				if(MarkFind[j].OnExecute() && MarkFind[j].Result[0].Score>MaxScore){ //PrintMark Search OK 
					PrintMark[k].Point.x = MarkFind[j].Result[0].Px;
					PrintMark[k].Point.y = MarkFind[j].Result[0].Py;
					PrintMark[k].Score   = MarkFind[j].Result[0].Score;
					MaxScore             = MarkFind[j].Result[0].Score;
				}
 			}
		}

		//PrintShift 산출기준 무조건 최단거리 기준점 기준 주위 평균값 
		//PrintEdge  산출기준 분류 (수직교차점 or 최단거리)
		if(bCutLine) {bVerDis = false;}
		else         {
			if     (InspMode==Rect_Shape  ) bVerDis=true;
			else if(InspMode==Corner_Shape) bVerDis=((PrintMark[k].Point.y<LTCornerHeight) || (PrintMark[k].Point.y>(EndPos.y-LBCornerHeight)))?false:true;
			else if(InspMode==Circle_Shape) bVerDis=((PrintMark[k].Point.y<LTCircleHeight) || (PrintMark[k].Point.y>(EndPos.y-LBCircleHeight)))?false:true;
		}

		// PrintEdge, PrintShift Calculate Sequence 
		if(PrintMark[k].Point.x!=0.0f && PrintMark[k].Point.y!=0.0f){ // PrintMark 찾았을경우
			// 전체 Buffer기준 좌표계산 다시함 
			PrintMark[k].Point.y = PrintMark[k].Point.y + StaPos.y;

			// PrintEdge 산출 (CutLine 존재 유무 기준으로 분류)
			if(bVerDis){ //Print Edge 산출시 수직 교차점 기준  
//				PrintEdgeDis = Math.GetLengthFrLineToPoint(StaPos, EndPos, PrintMark[k].Point);
//				PrintMark[k].PrintEdge = PrintEdgeDis;
				//TopCamera 기준 데이터 활용하여 가상으로 만든 BtmOutData 기준으로 PrintEdge 산출한다 
				for(int i=0; i<RealOutCnt; i++){ //전체 Buffer기준 PrintMark 위치 일치하는 가상 외곽라인 찾는다 
					if(PrintMark[k].Point.y >RealOutLine[i].FP0.y && PrintMark[k].Point.y <RealOutLine[i].FP1.y){
						PrintEdgeDis = Math.GetLengthFrLineToPoint(RealOutLine[i].FP0, RealOutLine[i].FP1, PrintMark[k].Point);
						PrintMark[k].PrintEdge = PrintEdgeDis;
						break;
					}
				}
			}
			else       { //PrintEdge 산출시 최단 거리 기준(주변값 10개 평균)=>CutLine,Corner,Circle 
			// PrintMark기준 OutData 최단거리 Count정보 Calc 
				PrintEdgeDis = 0.0f, MinDis = 10000.0f; PrintEdgeCnt = 0;
				for (int j = PrePrintEdgeCnt; j < AllOutCnt; j++) {
					if (AllOutPoint[j].x == 0.0f || AllOutPoint[j].y == 0.0f) continue;

					PrintEdgeDis = Math.GetLength(PrintMark[k].Point, AllOutPoint[j]);
					if (PrintEdgeDis < MinDis) {
						MinDis = PrintEdgeDis;
						PrintEdgeCnt = j;
						PrePrintEdgeCnt = PrintEdgeCnt; // 비교EdgePoint를 최소화 하기 위해서 이전 데이터 필요함 
					}
				}

				// PrintMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정(OutData기준)
				if (PrintEdgeCnt != 0 && PrintEdgeCnt > 5) {
					PrintCnt = 0, TotalPrintDis = 0.0f, PrintEdgeDis = 0.0f;
					for (int i = PrintEdgeCnt - 5; i < PrintEdgeCnt + 5; i++) {
						if (AllOutPoint[i].x == 0.0f || AllOutPoint[i].y == 0.0f) continue;
						// 데이터 튀는 현상 방지 목적(주변 데이터는 비슷함)
						if (fabs(AllOutPoint[PrintEdgeCnt].x - AllOutPoint[i].x) >= XPosMargin) continue;
						if (fabs(AllOutPoint[PrintEdgeCnt].y - AllOutPoint[i].y) >= YPosMargin) continue;

						PrintEdgeDis = Math.GetLength(PrintMark[k].Point, AllOutPoint[i]);
						TotalPrintDis += PrintEdgeDis;
						PrintCnt++;
					}
					if (TotalPrintDis != 0.0f && PrintCnt != 0) {
						PrintMark[k].PrintEdge = TotalPrintDis / PrintCnt;
					}
				}
			}

			// PrintShift 산출(PrintMark기준 PrintScreen 거리 측정)=>현재 Glass 각도기준 최소값에 해당하는 포인트 산출(10개평균)
			MinDeg = 360.0f;
			for (int i = PrePrintShiftCnt; i < AllPrintCnt; i++) {
				if (AllPrint[i].x == 0.0f || AllPrint[i].y == 0.0f) continue; //PrintScreen Data 
				if (AllPrint[i].y < (PrintMark[k].Point.y - Magin)) continue; //Print Mark 주위값 기준 데이터 추출 목적   
				if (AllPrint[i].y > (PrintMark[k].Point.y + Magin)) continue; //Print Mark 주위값 기준 데이터 추출 목적   

				PrintDeg = Math.GetDegree(PrintMark[k].Point, AllPrint[i]);
				DvDeg = fabs(GlassDeg + PrintDeg); //(Glass 각도 + PrintMark기준 PrintScreen각도)
				
				//Debug Test 목적
				BufferPos.x = AllPrint[i].x           ;
				BufferPos.y = AllPrint[i].y - StaPos.y;
				
				if (DvDeg < MinDeg) {
					MinDeg = DvDeg;
					MinBufferPos.x = BufferPos.x;
					MinBufferPos.y = BufferPos.y;
					PrintShiftCnt = i;
					PrePrintShiftCnt = PrintShiftCnt; // 비교EdgePoint를 최소화 하기 위해서 이전 데이터 필요함 
				}
			}

			// PrintMark기준 수직 포인트 주변값 Avg산출한다 ==>가공량 측정(OutData기준)
			if (PrintShiftCnt!= 0 && PrintShiftCnt> 5) {
				PrintCnt = 0, TotalPrintDis = 0.0f, PrintShiftDis = 0.0f;
				for (int i = PrintShiftCnt - 5; i < PrintShiftCnt + 5; i++) {
					if (AllPrint[i].x == 0.0f || AllPrint[i].y == 0.0f) continue;
					// 데이터 튀는 현상 방지 목적(주변 데이터는 비슷함)
					if (fabs(AllPrint[PrintShiftCnt].x - AllPrint[i].x) >= XPosMargin) continue;
					if (fabs(AllPrint[PrintShiftCnt].y - AllPrint[i].y) >= YPosMargin) continue;

					PrintShiftDis = Math.GetLength(PrintMark[k].Point, AllPrint[i]);
					TotalPrintDis += PrintShiftDis;
					PrintCnt++;
				}
				if (TotalPrintDis != 0.0f && PrintCnt != 0) {
					PrintMark[k].PrintShift = TotalPrintDis / PrintCnt;
				}
			}

			// 화면 디스플레이 목적으로 다시 계산함 
			PrintMark[k].Point.y = PrintMark[k].Point.y - StaPos.y; 
		}
		else                                                        { // PrintMark 못찾을경우
			PrintMark[k].PrintEdge  =0.0f;
			PrintMark[k].PrintShift =0.0f;
		}
	}

	// X1,X2,X3,Y1,Y2,Y3 측정 목적으로 데이터 취합 
	// 정해진 포인트가 없으니...분활해서 고정값으로 정하자 
	if(CamId==Left_Btm_Cam || CamId==Right_Btm_Cam){
		PrintEdge1 = (PrintMark[0].PrintEdge!= 0.0f) ? PrintMark[0].PrintEdge : 0.0f;
		PrintEdge2 = (PrintMark[4].PrintEdge!= 0.0f) ? PrintMark[4].PrintEdge : 0.0f;
		PrintEdge3 = (PrintMark[9].PrintEdge!= 0.0f) ? PrintMark[9].PrintEdge : 0.0f;
	}
}
void CFM14xLineDefect::OnLoad(bool IsLoad, CString Folder)
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
CFind* CFM14xLineDefect::GetFindPointer(int n)
{
	if     (n==0) return Find1;
	else if(n==1) return Find2;
	return NULL;
}

//------------------------------------------------------------------------------------
// 사용자 Set함수 
//------------------------------------------------------------------------------------------
void CFM14xLineDefect::SetProcessMode(TProcessMode ProcessMode)
{
	FProcessMode  = ProcessMode;
}
//------------------------------------------------------------------------------------------
void CFM14xLineDefect::SetInspectMode(TInspectMode InspectMode)
{
	FInspectMode  = InspectMode;
}
//------------------------------------------------------------------------------------------
void CFM14xLineDefect::SetShape(TShape Shape)
{
	FShape = Shape;
}
//------------------------------------------------------------------------------------------
void CFM14xLineDefect::SetLineScanType(int LineScanType)
{
	FLineScanType        = LineScanType;
	BufferWidth          = FLineScanType;
	Polygon->BufferWidth = FLineScanType; //Polygon 검사기 IELEMENT 사용함 
}
//-------------------------------------------------------------------------------------------
void CFM14xLineDefect::SetCrossLeftPoint(int x, int y)
{
	FP0.x = (float)x;
	FP0.y = (float)y;
	AlignMark1.x = FP0.x;
	AlignMark1.y = FP0.y;
}
//------------------------------------------------------------------------------------------
void CFM14xLineDefect::SetCrossRightPoint(int x, int y)
{
	FP1.x =(float)x;
	FP1.y =(float)y;
	AlignMark2.x = FP1.x;
	AlignMark2.y = FP1.y;
}
//------------------------------------------------------------------------------------------
void CFM14xLineDefect::SetSpecWidth(float MinWidth , float MaxWidth)
{
	FMinWidth = MinWidth;
	FMaxWidth = MaxWidth;
}
//------------------------------------------------------------------------------------------
void CFM14xLineDefect::SetSpecHeight(float MinHeight, float MaxHeight)
{
	FMinHeight = MinHeight;
	FMaxHeight = MaxHeight;
}
//------------------------------------------------------------------------------------------
void CFM14xLineDefect::SetPath(CString Path)
{
	FindDir  = Path;
}
//------------------------------------------------------------------------------------------
