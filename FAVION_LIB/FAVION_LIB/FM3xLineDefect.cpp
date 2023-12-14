#include "stdafx.h"
#include "FM3xLineDefect.h"
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

//�ű� ������Ʈ(LineScan4k X 8)
//////////////////////////////////////////////////////////////
CFM3xLineDefect::CFM3xLineDefect()
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
	DefectPointCount = 0; // ��ü �ҷ� ���� �ʱ�ȭ
	ACount           = 0; // ���ڸ�� �ҷ� ���� �ʱ�ȭ
	BCount           = 0; // ��������� �ҷ� ���� �ʱ�ȭ
	CCount           = 0; // �Ÿ�� �ҷ� ���� �ʱ�ȭ
	DCount           = 0;
	LTCorCount       = 0;
	LBCorCount       = 0;
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
	LTCirDeftectCnt  = 0;
	LBCirDeftectCnt  = 0;

	// Bright Defect 
	LTCirCrackCnt    =0;
	LBCirCrackCnt    =0;

	LTDx=0.0f, LBDx=0.0f;
	LTDy=0.0f, LBDy=0.0f;

	// Corner Data OutPoint Init
	LTCorPoint = NULL;
	LBCorPoint = NULL;

	// Corner Data InPoint Init
	LTCorInPoint = NULL;
	LBCorInPoint = NULL;

	// Corner Grind Meaasure StandPoint
	LTCorMeaPoint = NULL;
	LBCorMeaPoint = NULL;

	// Circle Data InPoint Init
	LTCirInPoint   = NULL;
	LBCirInPoint   = NULL;

	// Circle Data OutPoint Init
	LTCirOutPoint   = NULL;
	LBCirOutPoint   = NULL;

	// �ڸ� Circle_Fitting����� InPoint Init 
// 	LTCirInCrackPoint = NULL;
// 	LBCirInCrackPoint = NULL;

	// Btm �������� EdgePoint Data Init 
	LeftInPoint       = NULL;
	LeftOutPoint      = NULL;
	FrameLeftInPoint  = NULL;
	FrameLeftOutPoint = NULL;

	// MultiThresHold 
	ECThresHold_Sta    =0;
	ECThresHold_End    =0;
// 	EnCapTopThresHold  =0;
// 	EnCapMidThresHold  =0;
// 	EnCapBtmThresHold  =0;
	DarkThresHold      =0;
	BrokenThresHold    =0; 
	MaskDefectThresHold=0;

	// Mask Image ������ ���� Offset 
	MaskDefectOffset   =0;
	MaskOffsetDistance =0;

	//�⺻ EdgeThresHold Init 
	LeftEdgeThresHold    =0;
	TopEdgeThresHold     =0;
	RightEdgeThresHold   =0;
	BtmEdgeThresHold     =0;
	PatternThresHold     =0;
	PatternOffSet        =0;

	LeftEdgeBTWhite      =false; //Black To White Edge ���� 
	TopEdgeBTWhite       =false; //Black To White Edge ���� 
	RightEdgeBTWhite     =false; //Black To White Edge ���� 
	BtmEdgeBTWhite       =false; //Black To White Edge ���� 

	// Only �������� ã�� ThresHold 
	LeftGrindThresHold   =0;
	RightGrindThresHold  =0;
	BtmGrindThresHold    =0;

	LeftCutThresHold     =0;

	CorGrindMeaThresHold =0;
	CirGrindInThresHold  =0;
	CorGrindInThresHold_Sta  =0;
	CorGrindInThresHold_End  =0;

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

	//Notch Circle Dx,Dy 
	LTSideCircleWidth   =0.0f;
	LTSideCircleHeight  =0.0f;

	LBSideCircleWidth   =0.0f;
	LBSideCircleHeight  =0.0f;

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
	FGlassType   =0;

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

	// Frame���� GrindMar ���� ���� 
	IsFrameGrindMark = false;

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

	OnClearData();
}
CFM3xLineDefect::~CFM3xLineDefect()
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

float CFM3xLineDefect::GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3)
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
int CFM3xLineDefect::OnExecute(TShape Shape)
{
	FShape = Shape;
	return OnExecute();
}
int CFM3xLineDefect::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(L"Check CFM3xLineDefect OnExecute GImage Memory Allocate"); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(L"Check CFM3xLineDefect OnExecute GImage Width Size"     ); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(L"Check CFM3xLineDefect OnExecute GImage Height Size"    ); return false; }

	IAddress    = GImage->GetImagePtr();
	BufferWidth = GImage->GetWidth   (); //Buffer ũ�⼳��....Ư�� ��ǥ���� Pixel ����� �ʿ��� 
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
		r  = FRightWall   = false;
		b  = FBottomWall  = false;
		lt = FLeftTop     = true ;
		rt = FRightTop    = false;
		rb = FRightBottom = false;
		lb = FLeftBottom  = false;
		break;

	case 1 :
		// RectS : ������ ���
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
		// RectC : �Ÿ��
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
CRect CFM3xLineDefect::OnMerge( CRect ar, CRect br)
{
	CRect r;
	r.left   = (ar.left   < br.left  ) ? ar.left   : br.left  ;
	r.top    = (ar.top    < br.top   ) ? ar.top    : br.top   ;
	r.right  = (ar.right  > br.right ) ? ar.right  : br.right ;
	r.bottom = (ar.bottom > br.bottom) ? ar.bottom : br.bottom;

	return r;
}
/////////////////////////////////////////////////////////////////////////////////////
int CFM3xLineDefect::GetSize(TCorner *lpCorner,TBlob *lpDefect, int Count, int Dv)
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
							if(m<=0 || n<=0 || m>FrameImgW || n>FrameImgH) continue;
							PixelValue = IElement(m,n);
							if(PixelValue < ECThresHold_Sta) PixelCount++;
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
int CFM3xLineDefect::GetCircleCrackSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dis, double CirDiameter, TFPoint Center,int AllDefectCnt, bool IsInCircle)
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


	// TEST 
	int StaX =0, StaY=0, StaWidth=0, StaHeight=0;
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

			if (IsRoof && PointDis >MinDis && DefectPoint>5) { 
				// ���� Pixel Data ���� 
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
int CFM3xLineDefect::GetCircleSize(TCircle *lpCircle,TBlob *lpDefect, int Count, int Dv) //Mark_Mode Dark Defect 
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
							if(m<=0 || n<=0 || m>FrameImgW || n>FrameImgH) continue;
							PixelValue = IElement(m,n);
							if(PixelValue < DarkThresHold) PixelCount++;
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
int CFM3xLineDefect::GetPolygonSize(TFPoint *PolygonData, TBlob *lpDefect,int TotalCount,int Range, int DefectDv, int AllDefectCnt, int ThresHold)// Polygon Mode Dark Defect 
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
					if(BadRect[BadRectCount-1].BadPixelCount>=MAX_POLYGON_DEFECT_POINT){
						for(int k=0; k<BadRect[BadRectCount].BadPixelCount; k++) {
							BadRect[BadRectCount].Pixel[k].x =0;
							BadRect[BadRectCount].Pixel[k].y =0;
						}
						break;
					}
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
			if(BadRectCount>=MAX_POLYGON_DEFECT_COUNT) break;
			BadRectCount++;
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
									if(m<=0 || n<=0 || m>FrameImgW || n>FrameImgH) continue;
									PixelValue = IElement(m,n);
									if(PixelValue < ThresHold) PixelCount++;
									PixelPoint++;
								}
							}
							Score  = (((float)PixelCount/(float)PixelPoint)*100);
							// Circle Defect ������ PixelCount ������ üũ�Ѵ� => ����ų ���� ���� 
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
								lpDefect[AllDefectCnt].P.y          =  (float)(Rect.top +ImageHeight); // ��ü Frame���� Real��ǥ �������� ��ȯ 

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
int CFM3xLineDefect::GetAvgCellSize(int TopCount, int MidCount, int BtmCount, int AvgMode)
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
int CFM3xLineDefect::GetCellSize(int Count)
{
	if(MarkStrPos.x==0 || MarkStrPos.y==0 || MarkEndPos.x==0 || MarkEndPos.y==0) return false;
	if( FCellPoint[Count].x==0 || FCellPoint[Count].y==0)                        return false;
	if(Count>=MAX_DEFECT_POINT)                                                  return false;

	int nCellSize      =0;

	nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkStrPos, MarkEndPos, FCellPoint[Count]));     
	return  nCellSize;
}
void CFM3xLineDefect::OnClearData()
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
	LeftOutLine [0].FP0.x = 0.0f;
	LeftOutLine [0].FP0.y = 0.0f;
	LeftOutLine [0].FP1.x = 0.0f;
	LeftOutLine [0].FP1.y = 0.0f;
	LeftInLine  [0].FP0.x = 0.0f;
	LeftInLine  [0].FP0.y = 0.0f;
	LeftInLine  [0].FP1.x = 0.0f;
	LeftInLine  [0].FP1.y = 0.0f;

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
	ASideRect[0].left   =0;
	ASideRect[0].top    =0;
	ASideRect[0].right  =0;
	ASideRect[0].bottom =0;

	CSideRect[0].left   =0;
	CSideRect[0].top    =0;
	CSideRect[0].right  =0;
	CSideRect[0].bottom =0;

	// CornerData Init
	LTCorEdgeCount =0;
	LBCorEdgeCount =0;

	// Corner InEdge Count
	LTCorInEdgeCount =0;
	LBCorInEdgeCount =0;

	// Corner Grind Measure Stand Edge Count 
	LTCorMeaEdgeCount =0;
	LBCorMeaEdgeCount =0;

	// Inspection End
	TopLimitLine = 0 ;
	BtmLimitLine = 0 ;

	// Circle InEdge Point Count
	LTCirInEdgeCount  = 0;
	LBCirInEdgeCount  = 0;

	// Circle OutEdge Point Count
	LTCirOutEdgeCount = 0;
	LBCirOutEdgeCount = 0;

	LeftInEdgeCount    = 0;
	LeftOutEdgeCount   = 0;

	// Compare Data(Defect �������) 
	// �˻縦 ���ؼ� �ʿ��� �߰� ������ 
	for(int k=0; k<MAX_CIRCLE_EDGEPOINT; k++){
		LTCircleData[k].Dis = 0.0f;
		LTCircleData[k].Dv  = 0.0f;
		LTCircleData[k].P.x = 0.0f;
		LTCircleData[k].P.y = 0.0f;

		LBCircleData[k].Dis = 0.0f;
		LBCircleData[k].Dv  = 0.0f;
		LBCircleData[k].P.x = 0.0f;
		LBCircleData[k].P.y = 0.0f;
	}

	// �ڸ��ҷ� ���� Point Count
	LTCirInCrackCount = 0;
	LBCirInCrackCount = 0;
	LeftCrackCount    = 0;

	// Dark Defect 
	DefectPointCount = 0; // ��ü �ҷ� ���� �ʱ�ȭ
	ACount           = 0; // ���ڸ�� �ҷ� ���� �ʱ�ȭ
	BCount           = 0; // ��������� �ҷ� ���� �ʱ�ȭ
	CCount           = 0; // �Ÿ�� �ҷ� ���� �ʱ�ȭ
	DCount           = 0;
	LTCorCount       = 0;
	LBCorCount       = 0;
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
	LTCirDeftectCnt  = 0;
	LBCirDeftectCnt  = 0;

	// Bright Defect 
	LTCirCrackCnt    =0;
	LBCirCrackCnt    =0;

	// ��ü Defect Count(Polygon �˻�)
	LeftDefectCnt =0;
	LeftCrackCnt  =0;

	// ���� Frame���� Defect Count(Polygon �˻�)
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
	// �ݿ��� �ڸ� �� �ɹڸ� �������� Diameter
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

	// Corner Chamber Cut 3Point ���� ������ �ʱ�ȭ 
	LTCorGrindWd  = 0.0f;
	LTCorGrindMea = 0.0f;
	LBCorGrindWd  = 0.0f;
	LBCorGrindMea = 0.0f;

	// Btm Line Search �ð� ���� ���� 
	FEndLine =0;

	// Notch Cell Mark Degree 
	MarkDegree =0.0f;

	// ���� ���� (Circle)
// 	SideCirDeg   =0;
// 	CenterCirDeg =0;

	// 	Find->OnLoad(true,FindDir);

	// Notch Cell Size Data Init 
	LeftCellSizeHeight  =0.0f;
	CenCellSizeWidth    =0.0f;
	NotchCellSizeWidth  =0.0f;
	NotchCellSizeHeight =0.0f;

	// MarkToTopSide Height
	LeftMarkToTopHeight  =0.0f;
	CenMarkToTopHeight   =0.0f;
	RightMarkToTopHeight =0.0f;

	// GrindMarkToBmtHeight 
	LeftGrindToBtmHeight =0.0f;
	CenGrindToBtmHeight  =0.0f;
	RightGrindBtmHeight  =0.0f;

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

	// Total Left Grind InData/OutData Init 
	for(int k=0; k<MAX_ALL_EDGE_COUNT; k++){
		AllLeftInData   [k].x = 0.0f;
		AllLeftInData   [k].y = 0.0f;
		AllLeftOutData  [k].x = 0.0f;
		AllLeftOutData  [k].y = 0.0f;
		AllCircleInData [k].x = 0.0f;
		AllCircleInData [k].y = 0.0f;
		AllCircleOutData[k].x = 0.0f;
		AllCircleOutData[k].y = 0.0f;
		AllCornerInData [k].x = 0.0f;
		AllCornerInData [k].y = 0.0f;
		AllCornerOutData[k].x = 0.0f;
		AllCornerOutData[k].y = 0.0f;
	}
	// All Left Data Count Init 
	AllLeftInEdgeCount =0;
	AllLeftOutEdgeCount=0;

	StaLeftInEdgeCount =0;
	StaLeftOutEdgeCount=0;

	// All Circle Data Count Init 
	AllCirInEdgeCount =0;
	AllCirOutEdgeCount=0;

	// All Corner Data Count Init 
	AllCorInEdgeCount =0;
	AllCorOutEdgeCount=0;

	// Grind Mark Init(Total:23��)
	for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
		// �Ѱܹ��� ������ 
		GrindMark[k].degree      =0.0f;
		GrindMark[k].DvX         =0   ;
		GrindMark[k].DvY         =0   ;
		GrindMark[k].MarkID      =0   ;
		GrindMark[k].Range       =0.0f;
		// �˻� �� ����� ������ 
		GrindMark[k].Gradient    =0.0f;
		GrindMark[k].GrindMea    =0.0f;
		GrindMark[k].GrindWidth  =0.0f;
		GrindMark[k].Point.x     =0.0f;
		GrindMark[k].Point.y     =0.0f;
		GrindMark[k].Rect.left   =0   ;
		GrindMark[k].Rect.top    =0   ;
		GrindMark[k].Rect.right  =0   ;
		GrindMark[k].Rect.bottom =0   ;
		GrindMark[k].RectPoint.x =0   ;
		GrindMark[k].RectPoint.y =0   ;
		GrindMark[k].Score       =0.0f;

		//Pattern Distacne Init 
		PatternDis[k].Dis = 0.0f;
	}

	//Hole Grind Mark Init(Total:8��)
	for(int k=0; k<MAX_HOLE_GRIND_MARK_COUNT; k++){
		// �Ѱܹ��� ������ 
        HoleGrindMark[k].degree      =0.0f;
		HoleGrindMark[k].DvX         =0   ;
		HoleGrindMark[k].DvY         =0   ;
		HoleGrindMark[k].MarkID      =0   ;
		HoleGrindMark[k].Range       =0.0f;

		// �˻� �� ����� ������ 
		HoleGrindMark[k].Gradient    =0.0f;
		HoleGrindMark[k].GrindMea    =0.0f;
		HoleGrindMark[k].GrindWidth  =0.0f;
		HoleGrindMark[k].Point.x     =0.0f;
		HoleGrindMark[k].Point.y     =0.0f;
		HoleGrindMark[k].Rect.left   =0   ;
		HoleGrindMark[k].Rect.top    =0   ;
		HoleGrindMark[k].Rect.right  =0   ;
		HoleGrindMark[k].Rect.bottom =0   ;
		HoleGrindMark[k].RectPoint.x =0   ;
		HoleGrindMark[k].RectPoint.y =0   ;
		HoleGrindMark[k].Score       =0.0f;
	}



	// GrindMark ���������� �ʿ��� ���� ������ 
	dSideDeg =0.0f; 

	//��ü Buffer���� FLTPoint ��ǥ 
	BuffLTPoint.x =0.0f;
	BuffLTPoint.y =0.0f;

	// Frame���� Start EdgeCount
    StaLeftInEdgeCount  =0;
	StaLeftOutEdgeCount =0;

	// ������ ��ȭ�� ���ؼ� �߻��� ������ ����Ʈ 
	PatternOffSet =0;
}
//---------------------------------------------------------------
int CFM3xLineDefect::OnAutoBevelIn()// Line���� ���� �˻� �Լ� 
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

	//Corner Masking ���� �ι�° Masking������ �����
	double CorMaskingDy = 0.0f;

	double GrindWidthDis  =0.0f;
	int GrindMarkOutCnt=0, GrindMarkInCnt =0; 


	int Index = 0;
	// LineScan4K X 8EA �˻� ������(����/����) 
	if( FShape==shRectH ){ // (LineScan4K X 8EA)  ImageCopy -> ImageConversion -> ROI Attatch 
		EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);
		Index =0;   
	}                                                                                                     
	if( FShape==shRectS ){ // (LineScan4K X 8EA)  ImageCopy -> ImageConversion -> ROI Attatch 
		EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);
		Index =1;                                                                              
	}
	if( FShape==shRectC ){ // (LineScan4K X 8EA)  ImageCopy -> ImageConversion -> ROI Attatch 
		EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);
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

	TFPoint LT(0               ,                0);
	TFPoint RT((float)FrameImgW,                0);
	TFPoint RB((float)FrameImgW, (float)FrameImgH);
	TFPoint LB(0               , (float)FrameImgH);
	TFPoint LSP, LEP                              ;

	// Notch Cell Size ���� 
	TFPoint NotchCenterPos(0.0f, 0.0f);

	// ROI ���� ����
	int RoiStaX     =0;
	int RoiStaY     =0;
	int RoiWidth    =0;
	int RoiHeight   =0;

	int BlobCount   =0;
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

	// All Grind(����) Circle Fitting Center 
	AllGrindCriFitCenter.x =0.0f;
	AllGrindCriFitCenter.y =0.0f;

	// GrindMark ���� �����Ͽ� ȸ������(�˻�ȭ�� ���÷���)
	TFPoint LtPos, RbPos;

	float Dis    =0.0f;
	float StdDis =0.0f;

	// Side Offset TEST 
	double  Width   =0.0f;
	long    Left    =0;
	float   CenterX =0.0F;

	// DarkDefect Image Masking 
	POINT *MaskPoint ;
	POINT *MaskPoint2;
	int TotalInCount=0, TotalInCount2=0;
	int MaskStaPos  =0;
	TPoint TargetPoint(0,0);
	TPoint StaPos     (0,0);
	TPoint EndPos     (0,0);
	double deg =0.0f;

	// �ڸ� �� ��ũ��ġ �˻� ���� ������ 
	CRect Rect;
	Rect.left   = 0;
	Rect.right  = 0;
	Rect.top    = 0;
	Rect.bottom = 0;

	TFPoint DefectLT(0.0f, 0.0f);
	TFPoint DefectRT(0.0f, 0.0f);
	TFPoint DefectLB(0.0f, 0.0f);
	TFPoint DefectRB(0.0f, 0.0f);
	const int MaskDefectMagin =5; // MaskImage Defect��ǥ���� �������ʶ��� ���� �ּҰŸ� �����ϰ�� Defect���� 
	bool IsDefect = false;
	double LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
	TPoint RoiLt(0,0), RoiRt(0,0), RoiLb(0,0), RoiRb(0,0);

	//Clipper Offset Value
	int PolyOutCnt  =0;

	switch (Index) {

		// shRectH  
		//----------------------------------------------------------------------
	case 0 :
		//Mobile or IT =>���� ������ ����(���� or ���� =>��ü���� or �κп��� =>�Ϲݰ˻� or Image Masking �˻� 
		//VR =>��ü����:Polygon ����, �κп���:LineFitting ���� 
		if(FGlassType==Mobile_Glass){ //Mobile_IT
			if(IsShortCell){ //���� �˻� 
				if(IsAllGrind){ //��ü����(Notch,Narrow,CAI)
					// Image Conversion Binary_Image, Pixel Value ���� 
	 				EasyImage::Threshold(GImage,&DefectImg, LeftEdgeThresHold);
					// 				IAddress = DefectImg.GetImagePtr();
					IAddress = DefectImg.GetImagePtr();
					// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					if(LeftInEdgeCount!=0){    
						LTCirDeftectCnt  = GetPolygonSize(LeftInPoint, LTCirDefectPoint, LeftInEdgeCount, 100, 10,0, LeftEdgeThresHold); // Dark Defect 
					}
					// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
					if(LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);	

//						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black 
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						// ù��° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
							deg = 0.0f;

							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k-1].x;  
								EndPos.y =(int)LeftInPoint[k-1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k+1].x;  
								EndPos.y =(int)LeftInPoint[k+1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,MaskDefectOffset,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint[k+1].x = TargetPoint.x;
							MaskPoint[k+1].y = TargetPoint.y;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv   ;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);


						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k  ].x;  
								EndPos.y =(int)MaskPoint[k  ].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k+2].x;  
								EndPos.y =(int)MaskPoint[k+2].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,MaskOffsetDistance,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint2[k+1].x = TargetPoint.x;
							MaskPoint2[k+1].y = TargetPoint.y;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);

						if(MaskPoint !=NULL) {
							delete []MaskPoint ;
						}
						if(MaskPoint2!=NULL) {
							delete []MaskPoint2;
						}
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
//							MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
//							BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
							IAddress = BinaryImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                ;
							RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
							// 						RoiWidth     =(int)(MaxDv- MinDv)      ;
							RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
							if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

							// TEST 
							int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
							int TotalPixelCnt =0, PixelScore  =0;
							float Score     =0.0f;

							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									IsDefect = false;
									Rect.left   = 0;
									Rect.right  = 0;
									Rect.top    = 0;
									Rect.bottom = 0;

									if(LTCirCrackCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
									Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

									PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
									for(int n=Rect.top ; n<Rect.bottom; n++){
										for(int m=Rect.left; m<Rect.right; m++){
											PixelValue = IElement(m,n);
											if(PixelValue > BrokenThresHold) PixelCount++;
											TotalPixelCnt++;
										}
									}
									StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
									if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

									if(Score>PixelScore){
										IsDefect = true;
										for(int i=0; i<LeftInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
											RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
											LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
											RBDis = Math.GetLength(LeftInPoint[i],DefectRB);

											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
												LTCirCrackPoint[LTCirCrackCnt].LineAttach =1;
												break;
											}
										}

										if(IsDefect){
											LTCirCrackPoint[LTCirCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LTCirCrackPoint[LTCirCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LTCirCrackPoint[LTCirCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirCrackPoint[LTCirCrackCnt].Width/2 ) +RoiStaX);
											LTCirCrackPoint[LTCirCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2) +RoiStaY);
											LTCirCrackPoint[LTCirCrackCnt].Rect.right  = (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.left+ LTCirCrackPoint[LTCirCrackCnt].Width );
											LTCirCrackPoint[LTCirCrackCnt].Rect.bottom = (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.top + LTCirCrackPoint[LTCirCrackCnt].Height);

											// Display ������ �ʿ��� 
											LTCirCrackPoint[LTCirCrackCnt].Round = PixelCount;
											LTCirCrackPoint[LTCirCrackCnt].P.x       = (float)(LTCirCrackPoint[LTCirCrackCnt].Rect.left);
											LTCirCrackPoint[LTCirCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2)+RoiStaY+ImageHeight );
											LTCirCrackPoint[LTCirCrackCnt].Width     = LTCirCrackPoint[LTCirCrackCnt].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LTCirCrackPoint[LTCirCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2)+RoiStaY+ImageHeight);
											LTCirCrackPoint[LTCirCrackCnt].Rect.bottom =(long)(LTCirCrackPoint[LTCirCrackCnt].Rect.top + LTCirCrackPoint[LTCirCrackCnt].Height);
											LTCirCrackPoint[LTCirCrackCnt].Ver_Dis      =LTCirCrackPoint[LTCirCrackCnt].Height;
											LTCirCrackPoint[LTCirCrackCnt].Hor_Dis      =LTCirCrackPoint[LTCirCrackCnt].Width ;
											LTCirCrackCnt++;
										}
									}
									else                {
										continue; // Score���Ͽ� �ҷ��� ����ó��
									}
								}
							}
							BlobCount =0;
						}
					}

					// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
					// ���� ��ü���� Dark Masking
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && LTCirDeftectCnt==0 && MaskDefectThresHold!=0){
					if(LeftInEdgeCount!=0 && LTCirDeftectCnt==0 && MaskDefectThresHold!=0){
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

//						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_HAP_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						// ù��° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
							deg = 0.0f;

							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k-1].x;  
								EndPos.y =(int)LeftInPoint[k-1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k+1].x;  
								EndPos.y =(int)LeftInPoint[k+1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,MaskDefectOffset,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint[k+1].x = TargetPoint.x;
							MaskPoint[k+1].y = TargetPoint.y;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv-20;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);


						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k  ].x;  
								EndPos.y =(int)MaskPoint[k  ].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k+2].x;  
								EndPos.y =(int)MaskPoint[k+2].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,MaskOffsetDistance,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint2[k+1].x = TargetPoint.x;
							MaskPoint2[k+1].y = TargetPoint.y;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
						if(MaskPoint !=NULL)  {
							delete []MaskPoint ;
						}
						if(MaskPoint2!=NULL)  {
							delete []MaskPoint2;
						}
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
//							MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_HAP.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
//							BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_HAP_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
							IAddress = BinaryImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                ;
							RoiStaY      =(int)( LeftInPoint[0].y+MaskingMargin)           ;
							// 						RoiWidth     =(int)(MaxDv- MinDv)                              ;
							RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
							if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);


							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									if(LTCirDeftectCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									for(int i=0; i<LeftInEdgeCount; i++){
										LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
										IsDefect = false;
										LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
										RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
										LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
										RBDis = Math.GetLength(LeftInPoint[i],DefectRB);
										// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
										// 									if(LTDis<=MaskDefectOffset || RTDis<=MaskDefectOffset || LBDis<=MaskDefectOffset || RBDis<=MaskDefectOffset){ 
										if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
											IsDefect = true; 
											break;
										}
									}

									if(IsDefect){
										LTCirDefectPoint[LTCirDeftectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
										LTCirDefectPoint[LTCirDeftectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

										LTCirDefectPoint[LTCirDeftectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirDefectPoint[LTCirDeftectCnt].Width/2 ) +RoiStaX);
										LTCirDefectPoint[LTCirDeftectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2) +RoiStaY);
										LTCirDefectPoint[LTCirDeftectCnt].Rect.right  = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left+ LTCirDefectPoint[LTCirDeftectCnt].Width );
										LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);

										PixelCount = 0;
										PixelValue = 0;
										// ���� Pixel Data ���� 
										for(int n=LTCirDefectPoint[LTCirDeftectCnt].Rect.top+1 ; n<LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom-1; n++){
											for(int m=LTCirDefectPoint[LTCirDeftectCnt].Rect.left+1; m<LTCirDefectPoint[LTCirDeftectCnt].Rect.right-1 ; m++){
												PixelValue = IElement(m,n);
												if(PixelValue < MaskDefectThresHold) PixelCount++;
											}
										}
										// Display ������ �ʿ��� 
										LTCirDefectPoint[LTCirDeftectCnt].Round = PixelCount;
										LTCirDefectPoint[LTCirDeftectCnt].P.x       = (float)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left);
										LTCirDefectPoint[LTCirDeftectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2)+RoiStaY+ImageHeight );
										LTCirDefectPoint[LTCirDeftectCnt].Width     = LTCirDefectPoint[LTCirDeftectCnt].Width+SideMargin; 

										// Merge ���ؼ� ������ �ʿ��� 
										LTCirDefectPoint[LTCirDeftectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2)+RoiStaY+ImageHeight);
										LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom =(long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);
										LTCirDeftectCnt++;
									}
								}
							}
							BlobCount =0;
						}
					}
				}
				else          { //�κп���

					if(IsRectTop)       {
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						RoiStaX      =(int)(FLTPoint.x+SideMargin);
						RoiStaY      =(int)(FLTPoint.y+Margin)    ;
						RoiWidth     =200                         ;
						RoiHeight    =(int)(FrameImgH - RoiStaY)  ;

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						// Blob ���͸� 
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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight );
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
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
					else if(IsCornerTop){
						// Image Conversion Binary_Image, Pixel Value ���� 
						EasyImage::Threshold(GImage,&DefectImg, CorGrindInThresHold_Sta);
						IAddress = DefectImg.GetImagePtr();

						if(LTCorInEdgeCount!=0){
							CornerCount=0;
							for(int k=0; k<LTCorInEdgeCount; k++){
								if(LTCorInPoint[k].y>FLTLPoint.y-1) continue;
								Dis = Math.GetLengthFrLineToPoint(FLTLInPoint, FLTTInPoint, LTCorInPoint[k]);
								LTCorner[CornerCount].Dv   = Dis         ;
								LTCorner[CornerCount].P.x  = LTCorInPoint[k].x;
								LTCorner[CornerCount].P.y  = LTCorInPoint[k].y;
								CornerCount++;
								if(Dis>MaxDv) MaxDv = Dis;
								if(Dis<MinDv) MinDv = Dis;
							}
							if(LTCornerWidth>(LTCornerHeight*2)) LTCorCount = GetSize(LTCorner, LTCorDefectPoint, CornerCount, 10);
							else                                 LTCorCount = GetSize(LTCorner, LTCorDefectPoint, CornerCount, 5 );
						}

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						RoiStaX      =(int)(FLTLInPoint.x+SideMargin);
						RoiStaY      =(int)(FLTLInPoint.y+Margin)    ;
						RoiWidth     =200                            ;
						RoiHeight    =(int)(FrameImgH - RoiStaY)     ;

						//�˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth,RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						// Blob ���͸� 
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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
								DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
								DCount++;
							}
						}
						BlobCount =0;

						//HSKIM Corner ���� ���� Masking �߰�
//					if(Polygon->Blob[0].InCnt!=0 && LTCorInEdgeCount!=0 && MaskDefectThresHold!=0){
						if(LTCorInEdgeCount!=0 && MaskDefectThresHold!=0){
							// MaskImage Create Start \
							//////////////////////////////////////////////////////////////////////////////////////////////////
							BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

							HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
							HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

							TotalInCount = LTCorInEdgeCount+2; 
							MaskPoint = new POINT[TotalInCount];

							// Data Init 
							for(int k=0; k<TotalInCount; k++){
								MaskPoint[k].x = 0;
								MaskPoint[k].y = 0;
							}

							// Data Convert 
							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LTCorInEdgeCount; k++){
								MaskPoint[k+1].x = (int)LTCorInPoint[k].x+MaskDefectOffset;
								MaskPoint[k+1].y = (int)LTCorInPoint[k].y                 ;

								if(k==(LTCorInEdgeCount-1)){
									StaPos.x =(int)LTCorInPoint[k  ].x; 
									StaPos.y =(int)LTCorInPoint[k  ].y; 
									EndPos.x =(int)LTCorInPoint[k-1].x;  
									EndPos.y =(int)LTCorInPoint[k-1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)LTCorInPoint[k  ].x; 
									StaPos.y =(int)LTCorInPoint[k  ].y; 
									EndPos.x =(int)LTCorInPoint[k+1].x;  
									EndPos.y =(int)LTCorInPoint[k+1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

//								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,MaskDefectOffset,deg);
								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,0,deg); //HSKIM Coner

								MaskPoint[k+1].x = TargetPoint.x;
								MaskPoint[k+1].y = TargetPoint.y;

								if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
								if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
							}
							// �а ������,���� 
							MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LTCorInPoint[0                 ].y;
							MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LTCorInPoint[LTCorInEdgeCount-1].y; 
							MaskStaPos                  =MinDv-20;

							HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
							::FillRgn(hdc, Rgn1, fillBrh);
							::DeleteObject(Rgn1);

							//TEST
							//						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
							//						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\MaskDarkDefectImg_HAP_Part1.bmp");

							// �ι�° �̹��� Masking���� �� ��ǥ ��� 
							TotalInCount2 = LTCorInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint2 = new POINT[TotalInCount2];

							// Data Init 
							for(int k=0; k<TotalInCount2; k++){
								MaskPoint2[k].x = 0;
								MaskPoint2[k].y = 0;
							}

							// Data Convert 
							MinDv        =100000.0f;
							MaxDv        = 0.0f;
							CorMaskingDy = 0.0f;
							for(int k=0; k<LTCorInEdgeCount; k++){
								MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
								MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LTCorInEdgeCount-1)){
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k  ].x;  
									EndPos.y =(int)MaskPoint[k  ].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k+2].x;  
									EndPos.y =(int)MaskPoint[k+2].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,MaskOffsetDistance,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint2[k+1].x = TargetPoint.x;
								MaskPoint2[k+1].y = TargetPoint.y;

								if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
								if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;

								//�а�� �ƴ� Corner ����, �� Y ��ǥ ���� ����
								if(MaskPoint2[k+1].y>CorMaskingDy) CorMaskingDy   = MaskPoint2[k+1].y;
							}

							// �а ������,���� 
							// 						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LTCorInPoint[0                 ].y;
							// 						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LTCorInPoint[LTCorInEdgeCount-1].y; 
							//Corner Masking ���� ����, �ι�° Masking������ ������
							MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = CorMaskingDy;
							MaskPoint2[TotalInCount2-1].x =MinDv-20; MaskPoint2[TotalInCount2-1].y = CorMaskingDy; 

							HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
							::FillRgn(hdc, Rgn2, fillBrh);
							::DeleteObject(Rgn2);
							::DeleteObject(fillBrh);

							Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
							if(MaskPoint !=NULL)  delete []MaskPoint ;
							if(MaskPoint2!=NULL)  delete []MaskPoint2;

							if(MaskDarkDefectImg.GetImagePtr()){
								// Image Conversion Binary_Image
//								MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\MaskDarkDefectImg_����.bmp");
								EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
//								BinaryImg.Save("D:\\ROTATE_IMG\\MaskDarkDefectImg_Binary.bmp");

								// Image Euresys Algorithm Attach 
								DefectROI.Attach(&BinaryImg);
								CodeImageSelection.SetFeretAngle(0.00f);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
								IAddress = BinaryImg.GetImagePtr();
								// 						IAddress = MaskDarkDefectImg.GetImagePtr();

								RoiStaX      =(int)(MaskStaPos)                                  ;
								RoiStaY      =(int)( LTCorInPoint[0].y)                          ;
//								RoiStaY      =(int)( LTCorInPoint[0].y+MaskingMargin)            ; Original
								RoiWidth     =(int)(MaxDv- MinDv)                                ;
								RoiHeight    =(int)(LTCorInPoint[LTCorInEdgeCount-1].y)-( LTCorInPoint[0].y)          ;
//								RoiHeight    =(int)(LTCorInPoint[LTCorInEdgeCount-1].y - RoiStaY); Original

								// �˻� ������ 
								DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
								if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
								if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
								if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

								// Blob �˻� ���� 
								CodeImageSelection.Clear();
								CodeImageEncoder.Encode(DefectROI,CodeImage);
								CodeImageSelection.Clear();
								CodeImageSelection.AddObjects(CodeImage);

								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

								BlobCount = CodeImageSelection.GetElementCount();
								CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

								if(BlobCount!=0){
									for(int k=0; k<BlobCount; k++){
										if(LTCorCount>=MAX_DEFECT_POINT) break;
										// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
										DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
										DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
										DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
										DefectRT.y  = (float)( DefectLT.y                                                         );
										DefectLB.x  = (float)( DefectLT.x                                                         );
										DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
										DefectRB.x  = (float)( DefectRT.x                                                         );
										DefectRB.y  = (float)( DefectLB.y                                                         );

										for(int i=0; i<LTCorInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											IsDefect = false;
											LTDis = Math.GetLength(LTCorInPoint[i],DefectLT);
											RTDis = Math.GetLength(LTCorInPoint[i],DefectRT);
											LBDis = Math.GetLength(LTCorInPoint[i],DefectLB);
											RBDis = Math.GetLength(LTCorInPoint[i],DefectRB);
											// 										if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
											// 										if(LTDis<=MaskDefectOffset || RTDis<=MaskDefectOffset || LBDis<=MaskDefectOffset || RBDis<=MaskDefectOffset){ 
											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
//											if(LTDis<=MaskDefectOffset+10 || RTDis<=MaskDefectOffset+10 || LBDis<=MaskDefectOffset+10 || RBDis<=MaskDefectOffset+10){ 
												IsDefect = true; 
												break;
											}
										}

										if(IsDefect){
											LTCorDefectPoint[LTCorCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LTCorDefectPoint[LTCorCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LTCorDefectPoint[LTCorCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCorDefectPoint[LTCorCount].Width/2 ) +RoiStaX);
											LTCorDefectPoint[LTCorCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCorDefectPoint[LTCorCount].Height/2) +RoiStaY);
											LTCorDefectPoint[LTCorCount].Rect.right  = (long)(LTCorDefectPoint[LTCorCount].Rect.left+ LTCorDefectPoint[LTCorCount].Width );
											LTCorDefectPoint[LTCorCount].Rect.bottom = (long)(LTCorDefectPoint[LTCorCount].Rect.top + LTCorDefectPoint[LTCorCount].Height);

											PixelCount = 0;
											PixelValue = 0;
											// ���� Pixel Data ���� 
											for(int n=LTCorDefectPoint[LTCorCount].Rect.top+1 ; n<LTCorDefectPoint[LTCorCount].Rect.bottom-1; n++){
												for(int m=LTCorDefectPoint[LTCorCount].Rect.left+1; m<LTCorDefectPoint[LTCorCount].Rect.right-1 ; m++){
													PixelValue = IElement(m,n);
													if(PixelValue < MaskDefectThresHold) PixelCount++;
												}
											}
											// Display ������ �ʿ��� 
											LTCorDefectPoint[LTCorCount].Round = PixelCount;
											LTCorDefectPoint[LTCorCount].P.x       = (float)(LTCorDefectPoint[LTCorCount].Rect.left);
											LTCorDefectPoint[LTCorCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCorDefectPoint[LTCorCount].Height/2)+RoiStaY+ImageHeight );
											LTCorDefectPoint[LTCorCount].Width     = LTCorDefectPoint[LTCorCount].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LTCorDefectPoint[LTCorCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCorDefectPoint[LTCorCount].Height/2)+RoiStaY+ImageHeight);
											LTCorDefectPoint[LTCorCount].Rect.bottom =(long)(LTCorDefectPoint[LTCorCount].Rect.top + LTCorDefectPoint[LTCorCount].Height);
											LTCorCount++;
										}
									}
								}
								BlobCount =0;
							}
						}

						// �ܰ����ο��� Mark������ �Ÿ� ���� 
						if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = (FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2));
						else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = (FP0.x -  Line[waLeftWall].FP0.x)                                                     ;
						else                                                    LDistance = (FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2)); 
					}
					else if(IsCircleTop){ //�κп����� �˻� ������ 
						// Image Conversion Binary_Image, Pixel Value ���� 
						EasyImage::Threshold(GImage,&DefectImg, ECThresHold_Sta);
						// 					EasyImage::Threshold(GImage,&DefectImg, EnCapTopThresHold);
						IAddress = DefectImg.GetImagePtr();

						// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
						if(LTCirInEdgeCount!=0){  
							// 						LTCirDeftectCnt  = GetPolygonSize(LTCirInPoint, LTCirDefectPoint, LTCirInEdgeCount, 50, 5,0, ECThresHold); // Dark Defect 
							LTCirDeftectCnt  = GetPolygonSize(LTCirInPoint, LTCirDefectPoint, LTCirInEdgeCount, 100, 10,0, ECThresHold_Sta); // Dark Defect 
						}

						// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
						if(Polygon->Blob[0].InCnt!=0 && LTCirInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
							// MaskImage Create Start 
							//////////////////////////////////////////////////////////////////////////////////////////////////
							BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_WHITE_MaskDarkDefectImg_HAP_����.bmp");

							HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0));
							HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

							// ù��° �̹��� Masking���� �� ��ǥ ��� 
							TotalInCount = LTCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint    = new POINT[TotalInCount];

							// Data Init 
							for(int k=0; k<TotalInCount; k++){
								MaskPoint[k].x = 0;
								MaskPoint[k].y = 0;
							}

							// Data Convert 
							MinDv =100000.0f;
							MaxDv =     0.0f;
							for(int k=0; k<LTCirInEdgeCount; k++){
								MaskPoint[k+1].x = (int)LTCirInPoint[k].x+MaskDefectOffset;
								MaskPoint[k+1].y = (int)LTCirInPoint[k].y                 ;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LTCirInEdgeCount-1)){
									StaPos.x =(int)LTCirInPoint[k  ].x; 
									StaPos.y =(int)LTCirInPoint[k  ].y; 
									EndPos.x =(int)LTCirInPoint[k-1].x;  
									EndPos.y =(int)LTCirInPoint[k-1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)LTCirInPoint[k  ].x; 
									StaPos.y =(int)LTCirInPoint[k  ].y; 
									EndPos.x =(int)LTCirInPoint[k+1].x;  
									EndPos.y =(int)LTCirInPoint[k+1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,MaskDefectOffset,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint[k+1].x = TargetPoint.x;
								MaskPoint[k+1].y = TargetPoint.y;

								if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
								if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
							}

							// �а ������,���� 
							MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LTCirInPoint[0                 ].y;
							MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LTCirInPoint[LTCirInEdgeCount-1].y; 
							MaskStaPos                  =MinDv   ;

							HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
							::FillRgn(hdc, Rgn1, fillBrh);
							::DeleteObject(Rgn1);


							// �ι�° �̹��� Masking���� �� ��ǥ ��� 
							TotalInCount2 = LTCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint2 = new POINT[TotalInCount2];

							// Data Init 
							for(int k=0; k<TotalInCount2; k++){
								MaskPoint2[k].x = 0;
								MaskPoint2[k].y = 0;
							}

							// Data Convert 
							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LTCirInEdgeCount; k++){
								MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
								MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LTCirInEdgeCount-1)){
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k  ].x;  
									EndPos.y =(int)MaskPoint[k  ].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k+2].x;  
									EndPos.y =(int)MaskPoint[k+2].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,MaskOffsetDistance,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint2[k+1].x = TargetPoint.x;
								MaskPoint2[k+1].y = TargetPoint.y;

								if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
								if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
							}

							// �а ������,���� 
							MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LTCirInPoint[0                 ].y;
							MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LTCirInPoint[LTCirInEdgeCount-1].y; 

							HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
							::FillRgn(hdc, Rgn2, fillBrh);
							::DeleteObject(Rgn2);
							::DeleteObject(fillBrh);

							Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
							if(MaskPoint !=NULL)  delete []MaskPoint ;
							if(MaskPoint2!=NULL)  delete []MaskPoint2;
							// MaskImage Create End
							//////////////////////////////////////////////////////////////////////////////////////////////////

							if(MaskDarkDefectImg.GetImagePtr()){
								// Image Conversion Binary_Image
								// 							MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_WHITE_MaskDarkDefectImg_HAP.bmp");
								EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
								// 							BinaryImg.Save("D:\\ROTATE_IMG\\����_�κ�_WHITE_MaskDarkDefectImg_HAP_Binary.bmp");

								// Image Euresys Algorithm Attach 
								DefectROI.Attach(&BinaryImg);
								CodeImageSelection.SetFeretAngle(0.00f);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
								IAddress = BinaryImg.GetImagePtr();

								RoiStaX      =(int)(MaskStaPos)                                  ;
								RoiStaY      =(int)( LTCirInPoint[0].y+MaskingMargin)            ;
								// 						RoiWidth     =(int)(MaxDv- MinDv)                              ;
								RoiWidth     =(int)(MaxDv- RoiStaX)                              ;
								RoiHeight    =(int)(LTCirInPoint[LTCirInEdgeCount-1].y - RoiStaY);

								// �˻� ������ 
								DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
								if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
								if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
								if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

								// Blob �˻� ���� 
								CodeImageSelection.Clear();
								CodeImageEncoder.Encode(DefectROI,CodeImage);
								CodeImageSelection.Clear();
								CodeImageSelection.AddObjects(CodeImage);

								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

								BlobCount = CodeImageSelection.GetElementCount();
								CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

								// TEST 
								int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
								int TotalPixelCnt =0, PixelScore  =0;
								float Score     =0.0f;

								if(BlobCount!=0){
									for(int k=0; k<BlobCount; k++){
										IsDefect = false;
										Rect.left   = 0;
										Rect.right  = 0;
										Rect.top    = 0;
										Rect.bottom = 0;

										if(LTCirCrackCnt>=MAX_DEFECT_POINT) break;
										// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
										DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
										DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
										DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
										DefectRT.y  = (float)( DefectLT.y                                                         );
										DefectLB.x  = (float)( DefectLT.x                                                         );
										DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
										DefectRB.x  = (float)( DefectRT.x                                                         );
										DefectRB.y  = (float)( DefectLB.y                                                         );

										Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
										Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

										PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
										for(int n=Rect.top ; n<Rect.bottom; n++){
											for(int m=Rect.left; m<Rect.right; m++){
												PixelValue = IElement(m,n);
												if(PixelValue > BrokenThresHold) PixelCount++;
												TotalPixelCnt++;
											}
										}
										StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
										if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

										if(Score>PixelScore){
											IsDefect = true;
											for(int i=0; i<LTCirInEdgeCount; i++){
												LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
												LTDis = Math.GetLength(LTCirInPoint[i],DefectLT);
												RTDis = Math.GetLength(LTCirInPoint[i],DefectRT);
												LBDis = Math.GetLength(LTCirInPoint[i],DefectLB);
												RBDis = Math.GetLength(LTCirInPoint[i],DefectRB);

												if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
													LTCirCrackPoint[LTCirCrackCnt].LineAttach =1;
													break;
												}
											}

											if(IsDefect){
												LTCirCrackPoint[LTCirCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
												LTCirCrackPoint[LTCirCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

												LTCirCrackPoint[LTCirCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirCrackPoint[LTCirCrackCnt].Width/2 ) +RoiStaX);
												LTCirCrackPoint[LTCirCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2) +RoiStaY);
												LTCirCrackPoint[LTCirCrackCnt].Rect.right  = (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.left+ LTCirCrackPoint[LTCirCrackCnt].Width );
												LTCirCrackPoint[LTCirCrackCnt].Rect.bottom = (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.top + LTCirCrackPoint[LTCirCrackCnt].Height);

												// Display ������ �ʿ��� 
												LTCirCrackPoint[LTCirCrackCnt].Round = PixelCount;
												LTCirCrackPoint[LTCirCrackCnt].P.x       = (float)(LTCirCrackPoint[LTCirCrackCnt].Rect.left);
												LTCirCrackPoint[LTCirCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2)+RoiStaY+ImageHeight );
												LTCirCrackPoint[LTCirCrackCnt].Width     = LTCirCrackPoint[LTCirCrackCnt].Width+SideMargin; 

												// Merge ���ؼ� ������ �ʿ��� 
												LTCirCrackPoint[LTCirCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2)+RoiStaY+ImageHeight);
												LTCirCrackPoint[LTCirCrackCnt].Rect.bottom =(long)(LTCirCrackPoint[LTCirCrackCnt].Rect.top + LTCirCrackPoint[LTCirCrackCnt].Height);
												LTCirCrackPoint[LTCirCrackCnt].Ver_Dis      =LTCirCrackPoint[LTCirCrackCnt].Height;
												LTCirCrackPoint[LTCirCrackCnt].Hor_Dis      =LTCirCrackPoint[LTCirCrackCnt].Width ;
												LTCirCrackCnt++;
											}
										}
										else                {
											continue; // Score���Ͽ� �ҷ��� ����ó��
										}
									}
								}
								BlobCount =0;
							}
						}


						// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
						// 					if(Polygon->Blob[0].InCnt!=0 && LTCirInEdgeCount!=0 && LTCirDeftectCnt==0 && MaskDefectThresHold!=0){
						if(Polygon->Blob[0].InCnt!=0 && LTCirInEdgeCount!=0 && MaskDefectThresHold!=0){
							// MaskImage Create Start \
							//////////////////////////////////////////////////////////////////////////////////////////////////
							BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_DARK_MaskDarkDefectImg_HAP_����.bmp");

							HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
							HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

							TotalInCount = LTCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint = new POINT[TotalInCount];

							// Data Init 
							for(int k=0; k<TotalInCount; k++){
								MaskPoint[k].x = 0;
								MaskPoint[k].y = 0;
							}

							// Data Convert 
							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LTCirInEdgeCount; k++){
								MaskPoint[k+1].x = (int)LTCirInPoint[k].x+MaskDefectOffset;
								MaskPoint[k+1].y = (int)LTCirInPoint[k].y                 ;

								if(k==(LTCirInEdgeCount-1)){
									StaPos.x =(int)LTCirInPoint[k  ].x; 
									StaPos.y =(int)LTCirInPoint[k  ].y; 
									EndPos.x =(int)LTCirInPoint[k-1].x;  
									EndPos.y =(int)LTCirInPoint[k-1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)LTCirInPoint[k  ].x; 
									StaPos.y =(int)LTCirInPoint[k  ].y; 
									EndPos.x =(int)LTCirInPoint[k+1].x;  
									EndPos.y =(int)LTCirInPoint[k+1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,MaskDefectOffset,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint[k+1].x = TargetPoint.x;
								MaskPoint[k+1].y = TargetPoint.y;

								if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
								if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
							}

							// �а ������,���� 
							MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LTCirInPoint[0                 ].y;
							MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LTCirInPoint[LTCirInEdgeCount-1].y; 
							MaskStaPos                  =MinDv-20;

							HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
							::FillRgn(hdc, Rgn1, fillBrh);
							::DeleteObject(Rgn1);


							// �ι�° �̹��� Masking���� �� ��ǥ ��� 
							TotalInCount2 = LTCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint2 = new POINT[TotalInCount2];

							// Data Init 
							for(int k=0; k<TotalInCount2; k++){
								MaskPoint2[k].x = 0;
								MaskPoint2[k].y = 0;
							}

							// Data Convert 
							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LTCirInEdgeCount; k++){
								MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
								MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LTCirInEdgeCount-1)){
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k  ].x;  
									EndPos.y =(int)MaskPoint[k  ].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k+2].x;  
									EndPos.y =(int)MaskPoint[k+2].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,MaskOffsetDistance,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint2[k+1].x = TargetPoint.x;
								MaskPoint2[k+1].y = TargetPoint.y;

								if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
								if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
							}

							// �а ������,���� 
							MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LTCirInPoint[0                 ].y;
							MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LTCirInPoint[LTCirInEdgeCount-1].y; 

							HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
							::FillRgn(hdc, Rgn2, fillBrh);
							::DeleteObject(Rgn2);
							::DeleteObject(fillBrh);

							Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
							if(MaskPoint !=NULL)  delete []MaskPoint ;
							if(MaskPoint2!=NULL)  delete []MaskPoint2;
							// MaskImage Create End
							//////////////////////////////////////////////////////////////////////////////////////////////////

							if(MaskDarkDefectImg.GetImagePtr()){
								// Image Conversion Binary_Image
								// 							MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_MaskDarkDefectImg_HAP.bmp");
								EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
								// 							MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\MaskDarkDefectImg_HAP_Part.bmp");

								// Image Euresys Algorithm Attach 
								DefectROI.Attach(&BinaryImg);
								CodeImageSelection.SetFeretAngle(0.00f);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
								IAddress = BinaryImg.GetImagePtr();

								RoiStaX      =(int)(MaskStaPos)                                  ;
								RoiStaY      =(int)( LTCirInPoint[0].y+MaskingMargin)            ;
								RoiWidth     =(int)(MaxDv- MinDv)                                ;
								RoiHeight    =(int)(LTCirInPoint[LTCirInEdgeCount-1].y - RoiStaY);

								// �˻� ������ 
								DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
								if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
								if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
								if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

								// Blob �˻� ���� 
								CodeImageSelection.Clear();
								CodeImageEncoder.Encode(DefectROI,CodeImage);
								CodeImageSelection.Clear();
								CodeImageSelection.AddObjects(CodeImage);

								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

								BlobCount = CodeImageSelection.GetElementCount();
								CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

								if(BlobCount!=0){
									for(int k=0; k<BlobCount; k++){
										if(LTCirDeftectCnt>=MAX_DEFECT_POINT) break;
										// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
										DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
										DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
										DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
										DefectRT.y  = (float)( DefectLT.y                                                         );
										DefectLB.x  = (float)( DefectLT.x                                                         );
										DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
										DefectRB.x  = (float)( DefectRT.x                                                         );
										DefectRB.y  = (float)( DefectLB.y                                                         );

										for(int i=0; i<LTCirInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											IsDefect = false;
											LTDis = Math.GetLength(LTCirInPoint[i],DefectLT);
											RTDis = Math.GetLength(LTCirInPoint[i],DefectRT);
											LBDis = Math.GetLength(LTCirInPoint[i],DefectLB);
											RBDis = Math.GetLength(LTCirInPoint[i],DefectRB);
											// 										if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
											// 										if(LTDis<=MaskDefectOffset || RTDis<=MaskDefectOffset || LBDis<=MaskDefectOffset || RBDis<=MaskDefectOffset){ 
											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
												IsDefect = true; 
												break;
											}
										}

										if(IsDefect){
											LTCirDefectPoint[LTCirDeftectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LTCirDefectPoint[LTCirDeftectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LTCirDefectPoint[LTCirDeftectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirDefectPoint[LTCirDeftectCnt].Width/2 ) +RoiStaX);
											LTCirDefectPoint[LTCirDeftectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2) +RoiStaY);
											LTCirDefectPoint[LTCirDeftectCnt].Rect.right  = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left+ LTCirDefectPoint[LTCirDeftectCnt].Width );
											LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);

											PixelCount = 0;
											PixelValue = 0;
											// ���� Pixel Data ���� 
											for(int n=LTCirDefectPoint[LTCirDeftectCnt].Rect.top+1 ; n<LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom-1; n++){
												for(int m=LTCirDefectPoint[LTCirDeftectCnt].Rect.left+1; m<LTCirDefectPoint[LTCirDeftectCnt].Rect.right-1 ; m++){
													PixelValue = IElement(m,n);
													if(PixelValue < MaskDefectThresHold) PixelCount++;
												}
											}
											// Display ������ �ʿ��� 
											LTCirDefectPoint[LTCirDeftectCnt].Round = PixelCount;
											LTCirDefectPoint[LTCirDeftectCnt].P.x       = (float)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left);
											LTCirDefectPoint[LTCirDeftectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2)+RoiStaY+ImageHeight );
											LTCirDefectPoint[LTCirDeftectCnt].Width     = LTCirDefectPoint[LTCirDeftectCnt].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LTCirDefectPoint[LTCirDeftectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2)+RoiStaY+ImageHeight);
											LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom =(long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);
											LTCirDeftectCnt++;
										}
									}
								}
								BlobCount =0;
							}
						}

						// Left���� �˻� 
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						RoiStaX      =(int)(FLTPoint.x+SideMargin)                  ;
						RoiStaY      =(int)(FLTPoint.y+LTSideCircleHeight+10+Margin);
						RoiWidth     =200                                           ;
						RoiHeight    =(int)(FrameImgH - RoiStaY)                    ;


						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth,RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
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
				}
			}
			else           { //����˻� 
				if(IsAllGrind){ // ���� New Sequence �˻� 
					// Image Conversion Binary_Image, Pixel Value ���� 
					// 				EasyImage::Threshold(GImage,&DefectImg, LeftEdgeThresHold);
					// 				IAddress = DefectImg.GetImagePtr();
					IAddress = DefectImg.GetImagePtr();

					// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					if(LeftInEdgeCount!=0){    
						LTCirDeftectCnt  = GetPolygonSize(LeftInPoint, LTCirDefectPoint, LeftInEdgeCount, 100, 10,0, LeftEdgeThresHold); // Dark Defect 
					}

					// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
					if(LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

						// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						// ù��° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k-1].x;  
								EndPos.y =(int)LeftInPoint[k-1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k+1].x;  
								EndPos.y =(int)LeftInPoint[k+1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,MaskDefectOffset,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint[k+1].x = TargetPoint.x;
							MaskPoint[k+1].y = TargetPoint.y;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv   ;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);


						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k  ].x;  
								EndPos.y =(int)MaskPoint[k  ].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k+2].x;  
								EndPos.y =(int)MaskPoint[k+2].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,MaskOffsetDistance,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint2[k+1].x = TargetPoint.x;
							MaskPoint2[k+1].y = TargetPoint.y;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);

						if(MaskPoint !=NULL) {
							delete []MaskPoint ;
						}
						if(MaskPoint2!=NULL) {
							delete []MaskPoint2;
						}
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
							// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
							IAddress = BinaryImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                ;
							RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
							// 						RoiWidth     =(int)(MaxDv- MinDv)      ;
							RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
							if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

							// TEST 
							int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
							int TotalPixelCnt =0, PixelScore  =0;
							float Score     =0.0f;

							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									IsDefect = false;
									Rect.left   = 0;
									Rect.right  = 0;
									Rect.top    = 0;
									Rect.bottom = 0;

									if(LTCirCrackCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
									Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

									PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
									for(int n=Rect.top ; n<Rect.bottom; n++){
										for(int m=Rect.left; m<Rect.right; m++){
											PixelValue = IElement(m,n);
											if(PixelValue > BrokenThresHold) PixelCount++;
											TotalPixelCnt++;
										}
									}
									StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
									if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

									if(Score>PixelScore){
										IsDefect = true;
										for(int i=0; i<LeftInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
											RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
											LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
											RBDis = Math.GetLength(LeftInPoint[i],DefectRB);

											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
												LTCirCrackPoint[LTCirCrackCnt].LineAttach =1;
												break;
											}
										}

										if(IsDefect){
											LTCirCrackPoint[LTCirCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LTCirCrackPoint[LTCirCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LTCirCrackPoint[LTCirCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirCrackPoint[LTCirCrackCnt].Width/2 ) +RoiStaX);
											LTCirCrackPoint[LTCirCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2) +RoiStaY);
											LTCirCrackPoint[LTCirCrackCnt].Rect.right  = (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.left+ LTCirCrackPoint[LTCirCrackCnt].Width );
											LTCirCrackPoint[LTCirCrackCnt].Rect.bottom = (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.top + LTCirCrackPoint[LTCirCrackCnt].Height);

											// Display ������ �ʿ��� 
											LTCirCrackPoint[LTCirCrackCnt].Round = PixelCount;
											LTCirCrackPoint[LTCirCrackCnt].P.x       = (float)(LTCirCrackPoint[LTCirCrackCnt].Rect.left);
											LTCirCrackPoint[LTCirCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2)+RoiStaY+ImageHeight );
											LTCirCrackPoint[LTCirCrackCnt].Width     = LTCirCrackPoint[LTCirCrackCnt].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LTCirCrackPoint[LTCirCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2)+RoiStaY+ImageHeight);
											LTCirCrackPoint[LTCirCrackCnt].Rect.bottom =(long)(LTCirCrackPoint[LTCirCrackCnt].Rect.top + LTCirCrackPoint[LTCirCrackCnt].Height);
											LTCirCrackPoint[LTCirCrackCnt].Ver_Dis      =LTCirCrackPoint[LTCirCrackCnt].Height;
											LTCirCrackPoint[LTCirCrackCnt].Hor_Dis      =LTCirCrackPoint[LTCirCrackCnt].Width ;
											LTCirCrackCnt++;
										}
									}
									else                {
										continue; // Score���Ͽ� �ҷ��� ����ó��
									}
								}
							}
							BlobCount =0;
						}
					}

					// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
					// ���� ��ü���� Dark Masking
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && LTCirDeftectCnt==0 && MaskDefectThresHold!=0){
					if(LeftInEdgeCount!=0 && LTCirDeftectCnt==0 && MaskDefectThresHold!=0){
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

						// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_HAP_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						// ù��° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
							deg = 0.0f;

							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k-1].x;  
								EndPos.y =(int)LeftInPoint[k-1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k+1].x;  
								EndPos.y =(int)LeftInPoint[k+1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,MaskDefectOffset,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint[k+1].x = TargetPoint.x;
							MaskPoint[k+1].y = TargetPoint.y;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv-20;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);

						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k  ].x;  
								EndPos.y =(int)MaskPoint[k  ].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k+2].x;  
								EndPos.y =(int)MaskPoint[k+2].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,MaskOffsetDistance,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint2[k+1].x = TargetPoint.x;
							MaskPoint2[k+1].y = TargetPoint.y;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
						if(MaskPoint !=NULL)  {
							delete []MaskPoint ;
						}
						if(MaskPoint2!=NULL)  {
							delete []MaskPoint2;
						}
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_HAP.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
							// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_HAP_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
							IAddress = BinaryImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                ;
							RoiStaY      =(int)( LeftInPoint[0].y+MaskingMargin)           ;
							// 						RoiWidth     =(int)(MaxDv- MinDv)                              ;
							RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
							if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);


							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									if(LTCirDeftectCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									for(int i=0; i<LeftInEdgeCount; i++){
										LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
										IsDefect = false;
										LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
										RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
										LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
										RBDis = Math.GetLength(LeftInPoint[i],DefectRB);
										// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
										// 									if(LTDis<=MaskDefectOffset || RTDis<=MaskDefectOffset || LBDis<=MaskDefectOffset || RBDis<=MaskDefectOffset){ 
										if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
											IsDefect = true; 
											break;
										}
									}

									if(IsDefect){
										LTCirDefectPoint[LTCirDeftectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
										LTCirDefectPoint[LTCirDeftectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

										LTCirDefectPoint[LTCirDeftectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirDefectPoint[LTCirDeftectCnt].Width/2 ) +RoiStaX);
										LTCirDefectPoint[LTCirDeftectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2) +RoiStaY);
										LTCirDefectPoint[LTCirDeftectCnt].Rect.right  = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left+ LTCirDefectPoint[LTCirDeftectCnt].Width );
										LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);

										PixelCount = 0;
										PixelValue = 0;
										// ���� Pixel Data ���� 
										for(int n=LTCirDefectPoint[LTCirDeftectCnt].Rect.top+1 ; n<LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom-1; n++){
											for(int m=LTCirDefectPoint[LTCirDeftectCnt].Rect.left+1; m<LTCirDefectPoint[LTCirDeftectCnt].Rect.right-1 ; m++){
												PixelValue = IElement(m,n);
												if(PixelValue < MaskDefectThresHold) PixelCount++;
											}
										}
										// Display ������ �ʿ��� 
										LTCirDefectPoint[LTCirDeftectCnt].Round = PixelCount;
										LTCirDefectPoint[LTCirDeftectCnt].P.x       = (float)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left);
										LTCirDefectPoint[LTCirDeftectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2)+RoiStaY+ImageHeight );
										LTCirDefectPoint[LTCirDeftectCnt].Width     = LTCirDefectPoint[LTCirDeftectCnt].Width+SideMargin; 

										// Merge ���ؼ� ������ �ʿ��� 
										LTCirDefectPoint[LTCirDeftectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2)+RoiStaY+ImageHeight);
										LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom =(long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);
										LTCirDeftectCnt++;
									}
								}
							}
							BlobCount =0;
						}
					}
				}
				else          { // ���� �κп��� Sequence �˻� 
					if(IsRectTop)       {
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						RoiStaX      =(int)(FLTPoint.x+SideMargin);
						RoiStaY      =(int)(FLTPoint.y+Margin)    ;
						RoiWidth     =200                         ;
						RoiHeight    =(int)(FrameImgH - RoiStaY)  ;

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						// Blob ���͸� 
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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight );
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
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
					else if(IsCornerTop){
						// Image Conversion Binary_Image, Pixel Value ���� 
						EasyImage::Threshold(GImage,&DefectImg, CorGrindInThresHold_Sta);
						IAddress = DefectImg.GetImagePtr();

						if(LTCorInEdgeCount!=0){
							CornerCount=0;
							for(int k=0; k<LTCorInEdgeCount; k++){
								if(LTCorInPoint[k].y>FLTLPoint.y-1) continue;
								Dis = Math.GetLengthFrLineToPoint(FLTLInPoint, FLTTInPoint, LTCorInPoint[k]);
								LTCorner[CornerCount].Dv   = Dis         ;
								LTCorner[CornerCount].P.x  = LTCorInPoint[k].x;
								LTCorner[CornerCount].P.y  = LTCorInPoint[k].y;
								CornerCount++;
								if(Dis>MaxDv) MaxDv = Dis;
								if(Dis<MinDv) MinDv = Dis;
							}
							if(LTCornerWidth>(LTCornerHeight*2)) LTCorCount = GetSize(LTCorner, LTCorDefectPoint, CornerCount, 10);
							else                                 LTCorCount = GetSize(LTCorner, LTCorDefectPoint, CornerCount, 5 );
						}

						// Left�����˻� 
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						RoiStaX      =(int)(FLTLInPoint.x+SideMargin);
						RoiStaY      =(int)(FLTLInPoint.y+Margin)    ;
						RoiWidth     =200                            ;
						RoiHeight    =(int)(FrameImgH - RoiStaY)     ;

						//�˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth,RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						// Blob ���͸� 
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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
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
					else if(IsCircleTop){ //���� Circle
						// Image Conversion Binary_Image, Pixel Value ���� 
						EasyImage::Threshold(GImage,&DefectImg, ECThresHold_Sta);
						// 				EasyImage::Threshold(GImage,&DefectImg, EnCapTopThresHold);
						IAddress = DefectImg.GetImagePtr();

						// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
						if(LTCirInEdgeCount!=0){    
							LTCirDeftectCnt  = GetPolygonSize(LTCirInPoint, LTCirDefectPoint, LTCirInEdgeCount, 100, 10,0,ECThresHold_Sta); // Dark Defect 
						}

						// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
						if(Polygon->Blob[0].InCnt!=0 && LTCirInEdgeCount!=0 && LTCirDeftectCnt==0 && MaskDefectThresHold!=0){
							// MaskImage Create Start 
							//////////////////////////////////////////////////////////////////////////////////////////////////
							BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

							// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_DARK_MaskDarkDefectImg_HAP_����.bmp");

							HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
							HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

							TotalInCount = LTCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint = new POINT[TotalInCount];

							// Data Init 
							// ù��° �̹��� Masking���� �� ��ǥ ��� 
							for(int k=0; k<TotalInCount; k++){
								MaskPoint[k].x = 0;
								MaskPoint[k].y = 0;
							}

							// Data Convert 
							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LTCirInEdgeCount; k++){
								MaskPoint[k+1].x = (int)LTCirInPoint[k].x+MaskDefectOffset;
								MaskPoint[k+1].y = (int)LTCirInPoint[k].y                 ;

								if(k==(LTCirInEdgeCount-1)){
									StaPos.x =(int)LTCirInPoint[k  ].x; 
									StaPos.y =(int)LTCirInPoint[k  ].y; 
									EndPos.x =(int)LTCirInPoint[k-1].x;  
									EndPos.y =(int)LTCirInPoint[k-1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)LTCirInPoint[k  ].x; 
									StaPos.y =(int)LTCirInPoint[k  ].y; 
									EndPos.x =(int)LTCirInPoint[k+1].x;  
									EndPos.y =(int)LTCirInPoint[k+1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,MaskDefectOffset,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint[k+1].x = TargetPoint.x;
								MaskPoint[k+1].y = TargetPoint.y;

								if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
								if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
							}

							// �а ������,���� 
							MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LTCirInPoint[0                 ].y;
							MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LTCirInPoint[LTCirInEdgeCount-1].y; 
							MaskStaPos                  =MinDv-20;

							HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
							::FillRgn(hdc, Rgn1, fillBrh);
							::DeleteObject(Rgn1);


							// �ι�° �̹��� Masking���� �� ��ǥ ��� 
							TotalInCount2 = LTCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint2 = new POINT[TotalInCount2];

							// Data Init 
							for(int k=0; k<TotalInCount2; k++){
								MaskPoint2[k].x = 0;
								MaskPoint2[k].y = 0;
							}

							// Data Convert 
							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LTCirInEdgeCount; k++){
								MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
								MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LTCirInEdgeCount-1)){
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k  ].x;  
									EndPos.y =(int)MaskPoint[k  ].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k+2].x;  
									EndPos.y =(int)MaskPoint[k+2].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,MaskOffsetDistance,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint2[k+1].x = TargetPoint.x;
								MaskPoint2[k+1].y = TargetPoint.y;

								if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
								if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
							}

							// �а ������,���� 
							MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LTCirInPoint[0                 ].y;
							MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LTCirInPoint[LTCirInEdgeCount-1].y; 

							HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
							::FillRgn(hdc, Rgn2, fillBrh);
							::DeleteObject(Rgn2);
							::DeleteObject(fillBrh);

							Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
							if(MaskPoint !=NULL)  delete []MaskPoint ;
							if(MaskPoint2!=NULL)  delete []MaskPoint2;
							// MaskImage Create End
							//////////////////////////////////////////////////////////////////////////////////////////////////

							if(MaskDarkDefectImg.GetImagePtr()){
								// Image Conversion Binary_Image
								// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_MaskDarkDefectImg_HAP.bmp");
								EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
								// 						BinaryImg.Save("D:\\ROTATE_IMG\\MaskDarkDefectImg_HAP_LONG_Binary.bmp");

								// Image Euresys Algorithm Attach 
								DefectROI.Attach(&BinaryImg);
								CodeImageSelection.SetFeretAngle(0.00f);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
								IAddress = BinaryImg.GetImagePtr();

								RoiStaX      =(int)(MaskStaPos)                                  ;
								RoiStaY      =(int)( LTCirInPoint[0].y+MaskingMargin)            ;
								RoiWidth     =(int)(MaxDv- MinDv)                                ;
								RoiHeight    =(int)(LTCirInPoint[LTCirInEdgeCount-1].y - RoiStaY);

								// �˻� ������ 
								DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
								if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
								if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
								if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

								// Blob �˻� ���� 
								CodeImageSelection.Clear();
								CodeImageEncoder.Encode(DefectROI,CodeImage);
								CodeImageSelection.Clear();
								CodeImageSelection.AddObjects(CodeImage);

								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

								BlobCount = CodeImageSelection.GetElementCount();
								CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

								if(BlobCount!=0){
									for(int k=0; k<BlobCount; k++){
										if(LTCirDeftectCnt>=MAX_DEFECT_POINT) break;
										// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
										DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
										DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
										DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
										DefectRT.y  = (float)( DefectLT.y                                                         );
										DefectLB.x  = (float)( DefectLT.x                                                         );
										DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
										DefectRB.x  = (float)( DefectRT.x                                                         );
										DefectRB.y  = (float)( DefectLB.y                                                         );

										for(int i=0; i<LTCirInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											IsDefect = false;
											LTDis = Math.GetLength(LTCirInPoint[i],DefectLT);
											RTDis = Math.GetLength(LTCirInPoint[i],DefectRT);
											LBDis = Math.GetLength(LTCirInPoint[i],DefectLB);
											RBDis = Math.GetLength(LTCirInPoint[i],DefectRB);
											// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
											// 									if(LTDis<=MaskDefectOffset || RTDis<=MaskDefectOffset || LBDis<=MaskDefectOffset || RBDis<=MaskDefectOffset){ 
											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
												IsDefect = true; 
												break;
											}
										}

										if(IsDefect){
											LTCirDefectPoint[LTCirDeftectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LTCirDefectPoint[LTCirDeftectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LTCirDefectPoint[LTCirDeftectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirDefectPoint[LTCirDeftectCnt].Width/2 ) +RoiStaX);
											LTCirDefectPoint[LTCirDeftectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2) +RoiStaY);
											LTCirDefectPoint[LTCirDeftectCnt].Rect.right  = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left+ LTCirDefectPoint[LTCirDeftectCnt].Width );
											LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);

											PixelCount = 0;
											PixelValue = 0;
											// ���� Pixel Data ���� 
											for(int n=LTCirDefectPoint[LTCirDeftectCnt].Rect.top+1 ; n<LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom-1; n++){
												for(int m=LTCirDefectPoint[LTCirDeftectCnt].Rect.left+1; m<LTCirDefectPoint[LTCirDeftectCnt].Rect.right-1 ; m++){
													PixelValue = IElement(m,n);
													if(PixelValue < MaskDefectThresHold) PixelCount++;
												}
											}
											// Display ������ �ʿ��� 
											LTCirDefectPoint[LTCirDeftectCnt].Round = PixelCount;
											LTCirDefectPoint[LTCirDeftectCnt].P.x       = (float)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left);
											LTCirDefectPoint[LTCirDeftectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2)+RoiStaY+ImageHeight );
											LTCirDefectPoint[LTCirDeftectCnt].Width     = LTCirDefectPoint[LTCirDeftectCnt].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LTCirDefectPoint[LTCirDeftectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2)+RoiStaY+ImageHeight);
											LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom =(long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);
											LTCirDeftectCnt++;
										}
									}
								}
								BlobCount =0;
							}
						}


						// Left���� �˻� 
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						RoiStaX      =(int)(FLTPoint.x+SideMargin)                  ;
						RoiStaY      =(int)(FLTPoint.y+LTSideCircleHeight+10+Margin);
						RoiWidth     =200                                           ;
						RoiHeight    =(int)(FrameImgH - RoiStaY)                    ;

						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth,RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
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
				}
			}
			// Cell Size ���� ������ �Ѱ���(Image ȸ�� �������)
			LeftStaPos.x   =  FLTPoint.x            ;
			LeftStaPos.y   =  FLTPoint.y+ImageHeight;
		}
		else                        { //VR (��ü���� or �κп���)=>����
			if(IsAllGrind){ //��ü���� 
				IAddress = DefectImg.GetImagePtr();

				// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
				if(LeftInEdgeCount!=0){    
					// FOV�������� �������� ������� 
					LTCirDeftectCnt  = GetPolygonSize(LeftInPoint, LTCirDefectPoint, LeftInEdgeCount, 100, 10,0, DarkThresHold); // Dark Defect 
				}

				// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
				if(LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
					// MaskImage Create Start 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					BinaryImg.SetSize        (GImage->GetWidth(), GImage->GetHeight()); // Image Masking Conversion Binary
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());	// Image Masking Org
					EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);
					//////////////////////////////////////////////////////////////////////////////////////////////////
					//MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP_����.bmp");

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black 
					HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//Clipper Offset USE 
					/*
					// Masking Data Init 
					if(MaskPoint !=NULL)  {
						delete []MaskPoint ;
					}
					if(MaskPoint2!=NULL)  {
						delete []MaskPoint2;
					}


					// ROI���� ��ǥ ������ �Ѱ��� 
					RoiLt.x = IRoi->GetOrgX()         ; RoiLt.y = IRoi->GetOrgY()          ;
					RoiRt.x = RoiLt.x+IRoi->GetWidth(); RoiRt.y = IRoi->GetOrgY()          ;
					RoiLb.x = RoiLt.x                 ; RoiLb.y = RoiLt.y+IRoi->GetHeight();
					RoiRb.x = RoiRt.x                 ; RoiRb.y = RoiRt.y+IRoi->GetHeight();

					// InEdgeData���� Out���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					The::Path  Poly_In ;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

					// Polygon InEdgePoint Data Send ClipperOffset 
					Poly_In.push_back(The::IntPoint(RoiLt.x,LeftInPoint[0].y));
					for(int k=0; k<LeftInEdgeCount; k++){
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
					Poly_In.push_back(The::IntPoint(RoiLb.x,RoiLb.y));

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);	

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;

					// InEdgeData���� In���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					Poly_In.clear();
					Poly_Out.clear();
					ClipOffset.Clear();
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

// 					Poly_In.push_back(The::IntPoint(FrameImgW,0)); //Image���� ������ ��� �� ��ġ  
					Poly_In.push_back(The::IntPoint(RoiRt.x,LeftInPoint[0].y)); //Image���� ������ ��� �� ��ġ  
					for(int k=0; k<LeftInEdgeCount; k++){
						deg = 0.0f;
						//////////////////////////////////////////////////////////
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
// 					Poly_In.push_back(The::IntPoint(FrameImgW,FrameImgH)); //Image���� ������ �ϴ� �� ��ġ  
					Poly_In.push_back(The::IntPoint(RoiRb.x,RoiRb.y));

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,-MaskOffsetDistance);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint2 = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint2[k].x = Poly_Out[0][k].x;
						MaskPoint2[k].y = Poly_Out[0][k].y;
					}


					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);
					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////
					*/

                    //OnFindPerpendicularPointXY Coordinage USE 
					// ù��° �̹��� Masking���� �� ��ǥ ��� 
					TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint = new POINT[TotalInCount];

					// Data Init 
					for(int k=0; k<TotalInCount; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
						MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
						deg = 0.0f;

						if(k==(LeftInEdgeCount-1)){
							StaPos.x =(int)LeftInPoint[k  ].x; 
							StaPos.y =(int)LeftInPoint[k  ].y; 
							EndPos.x =(int)LeftInPoint[k-1].x;  
							EndPos.y =(int)LeftInPoint[k-1].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}
						else                      {
							StaPos.x =(int)LeftInPoint[k  ].x; 
							StaPos.y =(int)LeftInPoint[k  ].y; 
							EndPos.x =(int)LeftInPoint[k+1].x;  
							EndPos.y =(int)LeftInPoint[k+1].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}

						TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,MaskDefectOffset,deg);
						// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
						MaskPoint[k+1].x = TargetPoint.x;
						MaskPoint[k+1].y = TargetPoint.y;

						if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
						if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
					}

					// �а ������,���� 
					MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
					MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
					MaskStaPos                  =MinDv   ;

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);


					// �ι�° �̹��� Masking���� �� ��ǥ ��� 
					TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint2 = new POINT[TotalInCount2];

					// Data Init 
					for(int k=0; k<TotalInCount2; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
						MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
						deg = 0.0f;

						// Rotate Test 
						if(k==(LeftInEdgeCount-1)){
							StaPos.x =(int)MaskPoint[k+1].x; 
							StaPos.y =(int)MaskPoint[k+1].y; 
							EndPos.x =(int)MaskPoint[k  ].x;  
							EndPos.y =(int)MaskPoint[k  ].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}
						else                      {
							StaPos.x =(int)MaskPoint[k+1].x; 
							StaPos.y =(int)MaskPoint[k+1].y; 
							EndPos.x =(int)MaskPoint[k+2].x;  
							EndPos.y =(int)MaskPoint[k+2].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}

						TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,MaskOffsetDistance,deg);
						// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
						MaskPoint2[k+1].x = TargetPoint.x;
						MaskPoint2[k+1].y = TargetPoint.y;

						if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
						if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
					}

					// �а ������,���� 
					MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
					MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);

					if(MaskPoint !=NULL) {
						delete []MaskPoint ;
					}
					if(MaskPoint2!=NULL) {
						delete []MaskPoint2;
					}
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////

					if(MaskDarkDefectImg.GetImagePtr()){
						// Image Conversion Binary_Image
// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP.bmp");
						EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
						// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP_Binary.bmp");

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BinaryImg.GetImagePtr();

// 						RoiStaX      =(int)(RoiLt.x)                                   ;
// 						RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
// 						RoiWidth     =(int)(RoiRt.x - RoiLt.x)                         ;
// 						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

						RoiStaX      =(int)(MaskStaPos)                                ;
						RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
						RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						// TEST 
						int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
						int TotalPixelCnt =0, PixelScore  =0;
						float Score     =0.0f;

						if(BlobCount!=0){
							for(int k=0; k<BlobCount; k++){
								IsDefect = false;
								Rect.left   = 0;
								Rect.right  = 0;
								Rect.top    = 0;
								Rect.bottom = 0;

								if(LTCirCrackCnt>=MAX_DEFECT_POINT) break;
								// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
								DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
								DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
								DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
								DefectRT.y  = (float)( DefectLT.y                                                         );
								DefectLB.x  = (float)( DefectLT.x                                                         );
								DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
								DefectRB.x  = (float)( DefectRT.x                                                         );
								DefectRB.y  = (float)( DefectLB.y                                                         );

								Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
								Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

								PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
								for(int n=Rect.top ; n<Rect.bottom; n++){
									for(int m=Rect.left; m<Rect.right; m++){
										PixelValue = IElement(m,n);
										if(PixelValue > BrokenThresHold) PixelCount++;
										TotalPixelCnt++;
									}
								}
								StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
								if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

								if(Score>PixelScore){
									IsDefect = true;
									for(int i=0; i<LeftInEdgeCount; i++){
										LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
										LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
										RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
										LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
										RBDis = Math.GetLength(LeftInPoint[i],DefectRB);

										if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
											LTCirCrackPoint[LTCirCrackCnt].LineAttach =1;
											break;
										}
									}

									if(IsDefect){
										LTCirCrackPoint[LTCirCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
										LTCirCrackPoint[LTCirCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

										LTCirCrackPoint[LTCirCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirCrackPoint[LTCirCrackCnt].Width/2 ) +RoiStaX);
										LTCirCrackPoint[LTCirCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2) +RoiStaY);
										LTCirCrackPoint[LTCirCrackCnt].Rect.right  = (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.left+ LTCirCrackPoint[LTCirCrackCnt].Width );
										LTCirCrackPoint[LTCirCrackCnt].Rect.bottom = (long)(LTCirCrackPoint[LTCirCrackCnt].Rect.top + LTCirCrackPoint[LTCirCrackCnt].Height);

										// Display ������ �ʿ��� 
										LTCirCrackPoint[LTCirCrackCnt].Round = PixelCount;
										LTCirCrackPoint[LTCirCrackCnt].P.x       = (float)(LTCirCrackPoint[LTCirCrackCnt].Rect.left);
										LTCirCrackPoint[LTCirCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2)+RoiStaY+ImageHeight );
										LTCirCrackPoint[LTCirCrackCnt].Width     = LTCirCrackPoint[LTCirCrackCnt].Width+SideMargin; 

										// Merge ���ؼ� ������ �ʿ��� 
										LTCirCrackPoint[LTCirCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirCrackPoint[LTCirCrackCnt].Height/2)+RoiStaY+ImageHeight);
										LTCirCrackPoint[LTCirCrackCnt].Rect.bottom =(long)(LTCirCrackPoint[LTCirCrackCnt].Rect.top + LTCirCrackPoint[LTCirCrackCnt].Height);
										LTCirCrackPoint[LTCirCrackCnt].Ver_Dis     =LTCirCrackPoint[LTCirCrackCnt].Height;
										LTCirCrackPoint[LTCirCrackCnt].Hor_Dis     =LTCirCrackPoint[LTCirCrackCnt].Width ;
										LTCirCrackCnt++;
									}
								}
								else                {
									continue; // Score���Ͽ� �ҷ��� ����ó��
								}
							}
						}
						BlobCount =0;
					}
				}

				// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
				// ���� ��ü���� Dark Masking
				// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && LTCirDeftectCnt==0 && MaskDefectThresHold!=0){
// 				if(LeftInEdgeCount!=0 && LTCirDeftectCnt==0 && MaskDefectThresHold!=0){
				if(LeftInEdgeCount!=0 &&  MaskDefectThresHold!=0){
					// MaskImage Create Start 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
					EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\WATCH_����_HAP_����.bmp");
					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
					HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//ClipperOffset USE 
					/*
					// Masking Data Init 
					if(MaskPoint !=NULL)  {
						delete []MaskPoint ;
					}
					if(MaskPoint2!=NULL)  {
						delete []MaskPoint2;
					}

					// ROI���� ��ǥ ������ �Ѱ��� 
					RoiLt.x = IRoi->GetOrgX()         ; RoiLt.y = IRoi->GetOrgY()          ;
					RoiRt.x = RoiLt.x+IRoi->GetWidth(); RoiRt.y = IRoi->GetOrgY()          ;
					RoiLb.x = RoiLt.x                 ; RoiLb.y = RoiLt.y+IRoi->GetHeight();
					RoiRb.x = RoiRt.x                 ; RoiRb.y = RoiRt.y+IRoi->GetHeight();

					// InEdgeData���� Out���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					The::Path  Poly_In ;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

					// Polygon InEdgePoint Data Send ClipperOffset 
// 					Poly_In.push_back(The::IntPoint(RoiLt.x,RoiLt.y));
					Poly_In.push_back(The::IntPoint(RoiLt.x,LeftInPoint[0].y));
					for(int k=0; k<LeftInEdgeCount; k++){
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
					Poly_In.push_back(The::IntPoint(RoiLb.x,RoiLb.y));

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);	
					//////////////////////////////////////////////////////////////////////////////////////////////////


					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;

					// InEdgeData���� In���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					Poly_In.clear();
					Poly_Out.clear();
					ClipOffset.Clear();
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

// 					Poly_In.push_back(The::IntPoint(FrameImgW,0)); //Image���� ������ ��� �� ��ġ  
// 					Poly_In.push_back(The::IntPoint(RoiRt.x,RoiRt.y)); //Image���� ������ ��� �� ��ġ  
					Poly_In.push_back(The::IntPoint(RoiRt.x,LeftInPoint[0].y));
					for(int k=0; k<LeftInEdgeCount; k++){
						deg = 0.0f;
						//////////////////////////////////////////////////////////
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
					Poly_In.push_back(The::IntPoint(RoiRb.x,RoiRb.y));
// 					Poly_In.push_back(The::IntPoint(FrameImgW,FrameImgH)); //Image���� ������ �ϴ� �� ��ġ  

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,-MaskOffsetDistance);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint2 = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint2[k].x = Poly_Out[0][k].x;
						MaskPoint2[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);
					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////
					*/

                     //OnFindPerpendicularPointXY Coordinage USE 
					// ù��° �̹��� Masking���� �� ��ǥ ��� 
					TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint = new POINT[TotalInCount];

					// Data Init 
					for(int k=0; k<TotalInCount; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
						MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
						deg = 0.0f;

						if(k==(LeftInEdgeCount-1)){
							StaPos.x =(int)LeftInPoint[k  ].x; 
							StaPos.y =(int)LeftInPoint[k  ].y; 
							EndPos.x =(int)LeftInPoint[k-1].x;  
							EndPos.y =(int)LeftInPoint[k-1].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}
						else                      {
							StaPos.x =(int)LeftInPoint[k  ].x; 
							StaPos.y =(int)LeftInPoint[k  ].y; 
							EndPos.x =(int)LeftInPoint[k+1].x;  
							EndPos.y =(int)LeftInPoint[k+1].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}

						TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,MaskDefectOffset,deg);
						// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
						MaskPoint[k+1].x = TargetPoint.x;
						MaskPoint[k+1].y = TargetPoint.y;

						if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
						if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
					}

					// �а ������,���� 
					MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
					MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
					MaskStaPos                  =MinDv-20;

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);


					// �ι�° �̹��� Masking���� �� ��ǥ ��� 
					TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint2 = new POINT[TotalInCount2];

					// Data Init 
					for(int k=0; k<TotalInCount2; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
						MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
						deg = 0.0f;

						// Rotate Test 
						if(k==(LeftInEdgeCount-1)){
							StaPos.x =(int)MaskPoint[k+1].x; 
							StaPos.y =(int)MaskPoint[k+1].y; 
							EndPos.x =(int)MaskPoint[k  ].x;  
							EndPos.y =(int)MaskPoint[k  ].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}
						else                      {
							StaPos.x =(int)MaskPoint[k+1].x; 
							StaPos.y =(int)MaskPoint[k+1].y; 
							EndPos.x =(int)MaskPoint[k+2].x;  
							EndPos.y =(int)MaskPoint[k+2].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}

						TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,MaskOffsetDistance,deg);
						// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
						MaskPoint2[k+1].x = TargetPoint.x;
						MaskPoint2[k+1].y = TargetPoint.y;

						if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
						if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
					}

					// �а ������,���� 
					MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
					MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					if(MaskPoint !=NULL)  {
						delete []MaskPoint ;
					}
					if(MaskPoint2!=NULL)  {
						delete []MaskPoint2;
					}
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////

					if(MaskDarkDefectImg.GetImagePtr()){
						// Image Conversion Binary_Image
// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\WATCH_����_HAP.bmp");
						EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
// 						BinaryImg.Save("D:\\ROTATE_IMG\\WATCH_����_HAP_Binary.bmp");

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = BinaryImg.GetImagePtr();
						// 						IAddress = MaskDarkDefectImg.GetImagePtr();

// 						RoiStaX      =(int)(RoiLt.x)                                   ;
// 						RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
// 						RoiWidth     =(int)(RoiRt.x - RoiLt.x)                         ;
// 						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

						RoiStaX      =(int)(MaskStaPos)                                ;
						RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
						RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);


						if(BlobCount!=0){
							for(int k=0; k<BlobCount; k++){
								if(LTCirDeftectCnt>=MAX_DEFECT_POINT) break;
								// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
								DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
								DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
								DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
								DefectRT.y  = (float)( DefectLT.y                                                         );
								DefectLB.x  = (float)( DefectLT.x                                                         );
								DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
								DefectRB.x  = (float)( DefectRT.x                                                         );
								DefectRB.y  = (float)( DefectLB.y                                                         );

								for(int i=0; i<LeftInEdgeCount; i++){
									LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
									IsDefect = false;
									LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
									RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
									LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
									RBDis = Math.GetLength(LeftInPoint[i],DefectRB);
									if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
										IsDefect = true; 
										break;
									}
								}

								if(IsDefect){
									LTCirDefectPoint[LTCirDeftectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
									LTCirDefectPoint[LTCirDeftectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

									LTCirDefectPoint[LTCirDeftectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LTCirDefectPoint[LTCirDeftectCnt].Width/2 ) +RoiStaX);
									LTCirDefectPoint[LTCirDeftectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2) +RoiStaY);
									LTCirDefectPoint[LTCirDeftectCnt].Rect.right  = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left+ LTCirDefectPoint[LTCirDeftectCnt].Width );
									LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom = (long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);

									PixelCount = 0;
									PixelValue = 0;
									// ���� Pixel Data ���� 
									for(int n=LTCirDefectPoint[LTCirDeftectCnt].Rect.top+1 ; n<LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom-1; n++){
										for(int m=LTCirDefectPoint[LTCirDeftectCnt].Rect.left+1; m<LTCirDefectPoint[LTCirDeftectCnt].Rect.right-1 ; m++){
											PixelValue = IElement(m,n);
											if(PixelValue < MaskDefectThresHold) PixelCount++;
										}
									}
									// Display ������ �ʿ��� 
									LTCirDefectPoint[LTCirDeftectCnt].Round = PixelCount;
									LTCirDefectPoint[LTCirDeftectCnt].P.x       = (float)(LTCirDefectPoint[LTCirDeftectCnt].Rect.left);
									LTCirDefectPoint[LTCirDeftectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2)+RoiStaY+ImageHeight );
									LTCirDefectPoint[LTCirDeftectCnt].Width     = LTCirDefectPoint[LTCirDeftectCnt].Width+SideMargin; 

									// Merge ���ؼ� ������ �ʿ��� 
									LTCirDefectPoint[LTCirDeftectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LTCirDefectPoint[LTCirDeftectCnt].Height/2)+RoiStaY+ImageHeight);
									LTCirDefectPoint[LTCirDeftectCnt].Rect.bottom =(long)(LTCirDefectPoint[LTCirDeftectCnt].Rect.top + LTCirDefectPoint[LTCirDeftectCnt].Height);
									LTCirDeftectCnt++;
								}
							}
						}
						BlobCount =0;
					}
				}
			}
			else          { //ASide 
				// �˻� ROI���� 
				LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
				LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);

				//Align Ʋ���� �����Ͽ� �˻�ROI���� ������ 
				if(fabs(LSP.x - LEP.x)>SideMargin){
					if     (LSP.x>LEP.x){LSP.x = LEP.x+((LSP.x - LEP.x)/2);}
					else if(LSP.x<LEP.x){LSP.x = LSP.x+((LEP.x - LSP.x)/2);}
				}

				RoiStaX      =(int)(LSP.x+SideMargin   );
				RoiStaY      =(int)(LSP.y              );
				RoiWidth     =200                       ;
				RoiHeight    =(int)(FrameImgH - RoiStaY);

				// �˻� ������ 
				DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
				if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
				if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
				if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

				// Blob �˻� ���� 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);

				// Blob ���͸� 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 2          , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(DCount>=MAX_DEFECT_POINT) break;
						DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
						DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
						DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
						DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

						PixelCount = 0;
						PixelValue = 0;
						// ���� Pixel Data ���� 
						for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
							for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < DarkThresHold) PixelCount++;
							}
						}
						// Display ������ �ʿ��� 
						DDefectPoint[DCount].Round = PixelCount;
						DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
						DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight );
						DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

						// Merge ���ؼ� ������ �ʿ��� 
						DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
						DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
						DCount++;
					}
				}
				BlobCount =0;

				// Broken �� �ڸ� �ҷ� �˻� ������(EnCap�� ������)
				if(!IsMarkBtm && BrokenThresHold!=0){
					BrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight())       ;
					EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BrightDefectImg);
					EasyImage::Threshold(GImage    ,&BrightDefectImg, BrokenThresHold)     ;

					DefectROI.Attach(&BrightDefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
					IAddress = BrightDefectImg.GetImagePtr();

					// Side_D Bright Defect ���� �� �з�(Broken,�ݿ��� �ڸ�___�ɹڸ�)
					/////////////////////////////////////////////////////////////////////////////////////////////////
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

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
							if(DCrackCount>=MAX_DEFECT_POINT) break; 
							DCrackPoint[DCrackCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
							DCrackPoint[DCrackCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

							DCrackPoint[DCrackCount].Rect.left   =(long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width/2) +RoiStaX));
							DCrackPoint[DCrackCount].Rect.top    =(long)(( CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/2)+RoiStaY );
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
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		Index = 0;
		return true;

		// NOTCH RectC : ������ ���
		//------------------------------------------------------------------
	case 1 :
		if(FGlassType==Mobile_Glass){ //���� or ���� =>��ü���� or �κп��� 
			if(IsShortCell){ //��ƽ ���� �˻� 
				if(IsAllGrind){ //��ü����(Notch,Narrow,CAI)
					// Image Conversion Binary_Image, Pixel Value ���� 
					EasyImage::Threshold(GImage,&DefectImg, LeftEdgeThresHold);
					IAddress = DefectImg.GetImagePtr();

					// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					if(LeftInEdgeCount!=0){    
						FrameLeftDefectCnt =0;
						// 					FrameLeftDefectCnt  = GetPolygonSize(LeftInPoint, LeftDefectPoint, LeftInEdgeCount, 50, 5,LeftDefectCnt,LeftEdgeThresHold); // Dark Defect 
						FrameLeftDefectCnt  = GetPolygonSize(LeftInPoint, LeftDefectPoint, LeftInEdgeCount, 100, 10,LeftDefectCnt,LeftEdgeThresHold); // Dark Defect 
						LeftDefectCnt +=FrameLeftDefectCnt;
					}

					// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
					if(LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

						// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_STATIC_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						// ù��° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv   ;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);

						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x+MaskOffsetDistance;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y                   ;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
						if(MaskPoint !=NULL)  delete []MaskPoint ;
						if(MaskPoint2!=NULL)  delete []MaskPoint2;
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_STATIC.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
							// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_STATIC_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
							IAddress = BinaryImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                ;
							RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
							// 						RoiWidth     =(int)(MaxDv- MinDv)                              ;
							RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0          ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
							if((RoiStaX) >=FrameImgW  ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                                return false;

							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

							// TEST 
							int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
							int TotalPixelCnt =0, PixelScore  =0;
							float Score     =0.0f;

							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									IsDefect = false;
									Rect.left   = 0;
									Rect.right  = 0;
									Rect.top    = 0;
									Rect.bottom = 0;

									if(LeftCrackCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
									Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

									PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
									for(int n=Rect.top ; n<Rect.bottom; n++){
										for(int m=Rect.left; m<Rect.right; m++){
											PixelValue = IElement(m,n);
											if(PixelValue > BrokenThresHold) PixelCount++;
											TotalPixelCnt++;
										}
									}
									StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
									if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

									if(Score>PixelScore){
										IsDefect = true;
										for(int i=0; i<LeftInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
											RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
											LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
											RBDis = Math.GetLength(LeftInPoint[i],DefectRB);

											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
												LeftCrackPoint[LeftCrackCnt].LineAttach =1;
												break;
											}
										}

										if(IsDefect){
											LeftCrackPoint[LeftCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LeftCrackPoint[LeftCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LeftCrackPoint[LeftCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LeftCrackPoint[LeftCrackCnt].Width/2 ) +RoiStaX);
											LeftCrackPoint[LeftCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height/2) +RoiStaY);
											LeftCrackPoint[LeftCrackCnt].Rect.right  = (long)(LeftCrackPoint[LeftCrackCnt].Rect.left+ LeftCrackPoint[LeftCrackCnt].Width );
											LeftCrackPoint[LeftCrackCnt].Rect.bottom = (long)(LeftCrackPoint[LeftCrackCnt].Rect.top + LeftCrackPoint[LeftCrackCnt].Height);

											// Display ������ �ʿ��� 
											LeftCrackPoint[LeftCrackCnt].Round = PixelCount;
											LeftCrackPoint[LeftCrackCnt].P.x       = (float)(LeftCrackPoint[LeftCrackCnt].Rect.left);
											LeftCrackPoint[LeftCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height/2)+RoiStaY+ImageHeight );
											LeftCrackPoint[LeftCrackCnt].Width     = LeftCrackPoint[LeftCrackCnt].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LeftCrackPoint[LeftCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height/2)+RoiStaY+ImageHeight);
											LeftCrackPoint[LeftCrackCnt].Rect.bottom =(long)(LeftCrackPoint[LeftCrackCnt].Rect.top + LeftCrackPoint[LeftCrackCnt].Height);
											LeftCrackPoint[LeftCrackCnt].Ver_Dis      =LeftCrackPoint[LeftCrackCnt].Height;
											LeftCrackPoint[LeftCrackCnt].Hor_Dis      =LeftCrackPoint[LeftCrackCnt].Width ;
											LeftCrackCnt++;
										}
									}
									else                {
										continue; // Score���Ͽ� �ҷ��� ����ó��
									}
								}
							}
							BlobCount =0;
						}
					}


					if(LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);
						// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_STATIC_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						MinDv =100000.0f;

						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv   ;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);

						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x+MaskOffsetDistance;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y                   ;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
						if(MaskPoint !=NULL) delete []MaskPoint ;
						if(MaskPoint2!=NULL) delete []MaskPoint2;
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\MaskDarkDefectImg_HAP.bmp");
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_STATIC.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
//	 						BinaryImg.Save("D:\\ROTATE_IMG\\����__DARK_MaskDarkDefectImg_STATIC_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
							IAddress = BinaryImg.GetImagePtr();
							
							RoiStaX      =(int)(MaskStaPos)                                        ;
							RoiStaY      =(int)( LeftInPoint[0].y+MaskingMargin)                   ;
//							RoiWidth     =(int)(MaxDv-MinDv)<=10 ? MaskOffsetDistance : MaxDv-MinDv;
							RoiWidth     =(int)(MaxDv-RoiStaX)                                     ;
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY)        ;

							// ���������� �ش��ϴ� ��ü�����ϰ�� ������������ 
							if(RoiWidth<10) RoiWidth = 30;

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0          ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
							if((RoiStaX) >=FrameImgW  ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                                return false;


							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									if(LeftDefectCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									for(int i=0; i<LeftInEdgeCount; i++){
										LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
										IsDefect = false;
										LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
										RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
										LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
										RBDis = Math.GetLength(LeftInPoint[i],DefectRB);
										// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
										if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
											IsDefect = true; 
											break;
										}
									}

									if(IsDefect){
										LeftDefectPoint[LeftDefectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
										LeftDefectPoint[LeftDefectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

										LeftDefectPoint[LeftDefectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LeftDefectPoint[LeftDefectCnt].Width/2 ) +RoiStaX);
										LeftDefectPoint[LeftDefectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height/2) +RoiStaY);
										LeftDefectPoint[LeftDefectCnt].Rect.right  = (long)(LeftDefectPoint[LeftDefectCnt].Rect.left+ LeftDefectPoint[LeftDefectCnt].Width );
										LeftDefectPoint[LeftDefectCnt].Rect.bottom = (long)(LeftDefectPoint[LeftDefectCnt].Rect.top + LeftDefectPoint[LeftDefectCnt].Height);

										PixelCount = 0;
										PixelValue = 0;
										// ���� Pixel Data ���� 
										for(int n=LeftDefectPoint[LeftDefectCnt].Rect.top+1 ; n<LeftDefectPoint[LeftDefectCnt].Rect.bottom-1; n++){
											for(int m=LeftDefectPoint[LeftDefectCnt].Rect.left+1; m<LeftDefectPoint[LeftDefectCnt].Rect.right-1 ; m++){
												PixelValue = IElement(m,n);
												if(PixelValue < MaskDefectThresHold) PixelCount++;
											}
										}
										// Display ������ �ʿ��� 
										LeftDefectPoint[LeftDefectCnt].Round = PixelCount;
										LeftDefectPoint[LeftDefectCnt].P.x       = (float)(LeftDefectPoint[LeftDefectCnt].Rect.left);
										LeftDefectPoint[LeftDefectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height/2)+RoiStaY+ImageHeight );
										LeftDefectPoint[LeftDefectCnt].Width     = LeftDefectPoint[LeftDefectCnt].Width+SideMargin; 

										// Merge ���ؼ� ������ �ʿ��� 
										LeftDefectPoint[LeftDefectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height/2)+RoiStaY+ImageHeight);
										LeftDefectPoint[LeftDefectCnt].Rect.bottom =(long)(LeftDefectPoint[LeftDefectCnt].Rect.top + LeftDefectPoint[LeftDefectCnt].Height);
										LeftDefectCnt++;
									}
								}
							}
							BlobCount =0;
						}
					}
				}
				else          { //�κп���
					// Image Conversion Binary_Image
					EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&DefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = DefectImg.GetImagePtr();

					// �˻� ROI���� 
					LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
					LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);

					RoiStaX      =(int)(LSP.x+SideMargin);
					RoiStaY      =(int)(LSP.y           );
					RoiWidth     =200                    ;
					RoiHeight    =(int)(FrameImgH - RoiStaY  );

					// �˻� ������ 
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

					// Blob �˻� ���� 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					// Blob ���͸� 
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

							DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
							DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
							DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
							DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

							PixelCount = 0;
							PixelValue = 0;
							// ���� Pixel Data ���� 
							for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
								for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
									PixelValue = IElement(m,n);
									if(PixelValue < DarkThresHold) PixelCount++;
								}
							}
							// Display ������ �ʿ��� 
							DDefectPoint[DCount].Round = PixelCount;
							DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
							DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight );
							DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

							// Merge ���ؼ� ������ �ʿ��� 
							DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
							DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
							DCount++;
						}
					}
					BlobCount =0;

					// �ܰ����ο��� Mark������ �Ÿ� ���� 
					if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = (FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2));
					else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = (FP0.x -  Line[waLeftWall].FP0.x)                                                     ;
					else                                                    LDistance = (FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2)); 

					// Broken �� �ڸ� �ҷ� �˻� ������(EnCap�� ������)
					if(!IsMarkBtm && BrokenThresHold!=0){
						BrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight())       ;
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BrightDefectImg);
						EasyImage::Threshold(GImage    ,&BrightDefectImg, BrokenThresHold)     ;

						DefectROI.Attach(&BrightDefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BrightDefectImg.GetImagePtr();

						// Side_D Bright Defect ���� �� �з�(Broken,�ݿ��� �ڸ�___�ɹڸ�)
						/////////////////////////////////////////////////////////////////////////////////////////////////
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
						if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

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
								if(DCrackCount>=MAX_DEFECT_POINT) break; 
								DCrackPoint[DCrackCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
								DCrackPoint[DCrackCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

								DCrackPoint[DCrackCount].Rect.left   =(long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width/2) +RoiStaX));
								DCrackPoint[DCrackCount].Rect.top    =(long)(( CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/2)+RoiStaY );
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
				}
			}
			else           { //����˻� 
				if(IsAllGrind){ //��ƽ ���� �˻� 
					// Image Conversion Binary_Image, Pixel Value ���� 
					EasyImage::Threshold(GImage,&DefectImg, LeftEdgeThresHold);
					IAddress = DefectImg.GetImagePtr();

					// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					if(LeftInEdgeCount!=0){    
						FrameLeftDefectCnt =0;
						// 					FrameLeftDefectCnt  = GetPolygonSize(LeftInPoint, LeftDefectPoint, LeftInEdgeCount, 50, 5,LeftDefectCnt,LeftEdgeThresHold); // Dark Defect 
						FrameLeftDefectCnt  = GetPolygonSize(LeftInPoint, LeftDefectPoint, LeftInEdgeCount, 100, 10,LeftDefectCnt,LeftEdgeThresHold); // Dark Defect 
						LeftDefectCnt +=FrameLeftDefectCnt;
					}

					// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
					if(LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

						// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\����_��ü_WHITE_MaskDarkDefectImg_STATIC_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						// ù��° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv   ;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);

						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x+MaskOffsetDistance;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y                   ;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
						if(MaskPoint !=NULL)  delete []MaskPoint ;
						if(MaskPoint2!=NULL)  delete []MaskPoint2;
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_STATIC.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
							// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_STATIC_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
							IAddress = BinaryImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                ;
							RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
							// 						RoiWidth     =(int)(MaxDv- MinDv)                              ;
							RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
							if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

							// TEST 
							int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
							int TotalPixelCnt =0, PixelScore  =0;
							float Score     =0.0f;

							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									IsDefect = false;
									Rect.left   = 0;
									Rect.right  = 0;
									Rect.top    = 0;
									Rect.bottom = 0;

									if(LeftCrackCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
									Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

									PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
									for(int n=Rect.top ; n<Rect.bottom; n++){
										for(int m=Rect.left; m<Rect.right; m++){
											PixelValue = IElement(m,n);
											if(PixelValue > BrokenThresHold) PixelCount++;
											TotalPixelCnt++;
										}
									}
									StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
									if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

									if(Score>PixelScore){
										IsDefect = true;
										for(int i=0; i<LeftInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
											RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
											LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
											RBDis = Math.GetLength(LeftInPoint[i],DefectRB);

											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
												LeftCrackPoint[LeftCrackCnt].LineAttach =1;
												break;
											}
										}

										if(IsDefect){
											LeftCrackPoint[LeftCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LeftCrackPoint[LeftCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LeftCrackPoint[LeftCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LeftCrackPoint[LeftCrackCnt].Width/2 ) +RoiStaX);
											LeftCrackPoint[LeftCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height/2) +RoiStaY);
											LeftCrackPoint[LeftCrackCnt].Rect.right  = (long)(LeftCrackPoint[LeftCrackCnt].Rect.left+ LeftCrackPoint[LeftCrackCnt].Width );
											LeftCrackPoint[LeftCrackCnt].Rect.bottom = (long)(LeftCrackPoint[LeftCrackCnt].Rect.top + LeftCrackPoint[LeftCrackCnt].Height);

											// Display ������ �ʿ��� 
											LeftCrackPoint[LeftCrackCnt].Round = PixelCount;
											LeftCrackPoint[LeftCrackCnt].P.x       = (float)(LeftCrackPoint[LeftCrackCnt].Rect.left);
											LeftCrackPoint[LeftCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height/2)+RoiStaY+ImageHeight );
											LeftCrackPoint[LeftCrackCnt].Width     = LeftCrackPoint[LeftCrackCnt].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LeftCrackPoint[LeftCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height/2)+RoiStaY+ImageHeight);
											LeftCrackPoint[LeftCrackCnt].Rect.bottom =(long)(LeftCrackPoint[LeftCrackCnt].Rect.top + LeftCrackPoint[LeftCrackCnt].Height);
											LeftCrackPoint[LeftCrackCnt].Ver_Dis      =LeftCrackPoint[LeftCrackCnt].Height;
											LeftCrackPoint[LeftCrackCnt].Hor_Dis      =LeftCrackPoint[LeftCrackCnt].Width ;
											LeftCrackCnt++;
										}
									}
									else                {
										continue; // Score���Ͽ� �ҷ��� ����ó��
									}
								}
							}
							BlobCount =0;
						}
					}

					// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && FrameLeftDefectCnt==0 && MaskDefectThresHold!=0){
					if(LeftInEdgeCount!=0 && FrameLeftDefectCnt==0 && MaskDefectThresHold!=0){
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);
						// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_STATIC_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv   ;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);

						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x+MaskOffsetDistance;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y                   ;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
						if(MaskPoint !=NULL) delete []MaskPoint ;
						if(MaskPoint2!=NULL) delete []MaskPoint2;
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\MaskDarkDefectImg_HAP.bmp");
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_STATIC.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
							// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_STATIC_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
							IAddress = BinaryImg.GetImagePtr();
							// 						IAddress = MaskDarkDefectImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                ;
							RoiStaY      =(int)( LeftInPoint[0].y+MaskingMargin)           ;
							RoiWidth     =(int)(MaxDv- MinDv)                              ;
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

							// ���������� �ش��ϴ� ��ü�����ϰ�� ������������ 
							if(RoiWidth<10) RoiWidth = 30;

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
							if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;


							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									if(LeftDefectCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									for(int i=0; i<LeftInEdgeCount; i++){
										LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
										IsDefect = false;
										LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
										RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
										LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
										RBDis = Math.GetLength(LeftInPoint[i],DefectRB);
										// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
										if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
											IsDefect = true; 
											break;
										}
									}

									if(IsDefect){
										LeftDefectPoint[LeftDefectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
										LeftDefectPoint[LeftDefectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

										LeftDefectPoint[LeftDefectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LeftDefectPoint[LeftDefectCnt].Width/2 ) +RoiStaX);
										LeftDefectPoint[LeftDefectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height/2) +RoiStaY);
										LeftDefectPoint[LeftDefectCnt].Rect.right  = (long)(LeftDefectPoint[LeftDefectCnt].Rect.left+ LeftDefectPoint[LeftDefectCnt].Width );
										LeftDefectPoint[LeftDefectCnt].Rect.bottom = (long)(LeftDefectPoint[LeftDefectCnt].Rect.top + LeftDefectPoint[LeftDefectCnt].Height);

										PixelCount = 0;
										PixelValue = 0;
										// ���� Pixel Data ���� 
										for(int n=LeftDefectPoint[LeftDefectCnt].Rect.top+1 ; n<LeftDefectPoint[LeftDefectCnt].Rect.bottom-1; n++){
											for(int m=LeftDefectPoint[LeftDefectCnt].Rect.left+1; m<LeftDefectPoint[LeftDefectCnt].Rect.right-1 ; m++){
												PixelValue = IElement(m,n);
												if(PixelValue < MaskDefectThresHold) PixelCount++;
											}
										}
										// Display ������ �ʿ��� 
										LeftDefectPoint[LeftDefectCnt].Round = PixelCount;
										LeftDefectPoint[LeftDefectCnt].P.x       = (float)(LeftDefectPoint[LeftDefectCnt].Rect.left);
										LeftDefectPoint[LeftDefectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height/2)+RoiStaY+ImageHeight );
										LeftDefectPoint[LeftDefectCnt].Width     = LeftDefectPoint[LeftDefectCnt].Width+SideMargin; 

										// Merge ���ؼ� ������ �ʿ��� 
										LeftDefectPoint[LeftDefectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height/2)+RoiStaY+ImageHeight);
										LeftDefectPoint[LeftDefectCnt].Rect.bottom =(long)(LeftDefectPoint[LeftDefectCnt].Rect.top + LeftDefectPoint[LeftDefectCnt].Height);
										LeftDefectCnt++;
									}
								}
							}
							BlobCount =0;
						}
					}
				}
				else          { //���� �κп���
					// 				LogUnit.SetLog("NORMAL_START_STATIC");     

					// Image Conversion Binary_Image
					EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);
					// 				DefectImg.Save("D:\\ROTATE_IMG\\STATIC_NORAML.bmp");

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&DefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = DefectImg.GetImagePtr();

					// �˻� ROI���� 
					LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
					LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);

					RoiStaX      =(int)(LSP.x+SideMargin);
					RoiStaY      =(int)(LSP.y           );
					RoiWidth     =200                    ;
					RoiHeight    =(int)(FrameImgH - RoiStaY  );

					// 				LogUnit.SetLog("�˻�_START_STATIC");     

					// �˻� ������ 
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

					// Blob �˻� ���� 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					BlobCount = CodeImageSelection.GetElementCount();
					// 				LogUnit.SetLog("�˻�_END_STATIC");     


					// Blob ���͸� 
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

							DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
							DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
							DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
							DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

							PixelCount = 0;
							PixelValue = 0;
							// ���� Pixel Data ���� 
							for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
								for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
									PixelValue = IElement(m,n);
									if(PixelValue < DarkThresHold) PixelCount++;
								}
							}
							// Display ������ �ʿ��� 
							DDefectPoint[DCount].Round = PixelCount;
							DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
							DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight );
							DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

							// Merge ���ؼ� ������ �ʿ��� 
							DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
							DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
							DCount++;
						}
					}
					BlobCount =0;

					// �ܰ����ο��� Mark������ �Ÿ� ���� 
					if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = (FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2));
					else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = (FP0.x -  Line[waLeftWall].FP0.x)                                                     ;
					else                                                    LDistance = (FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2)); 

					// Broken �� �ڸ� �ҷ� �˻� ������(EnCap�� ������)
					if(!IsMarkBtm && BrokenThresHold!=0){
						BrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight())       ;
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BrightDefectImg);
						EasyImage::Threshold(GImage    ,&BrightDefectImg, BrokenThresHold)     ;

						DefectROI.Attach(&BrightDefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BrightDefectImg.GetImagePtr();

						// Side_D Bright Defect ���� �� �з�(Broken,�ݿ��� �ڸ�___�ɹڸ�)
						/////////////////////////////////////////////////////////////////////////////////////////////////
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
						if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

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
								if(DCrackCount>=MAX_DEFECT_POINT) break; 
								DCrackPoint[DCrackCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
								DCrackPoint[DCrackCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

								DCrackPoint[DCrackCount].Rect.left   =(long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width/2) +RoiStaX));
								DCrackPoint[DCrackCount].Rect.top    =(long)(( CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/2)+RoiStaY );
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
					// 				LogUnit.SetLog("NORMAL_END_STATIC");     

				}
			}
		}
		else                        {
			if(IsAllGrind){ //��ü����
				IAddress = DefectImg.GetImagePtr();

				// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
				if(LeftInEdgeCount!=0){    
					// FOV�������� �������� ������� 
					FrameLeftDefectCnt  =0;
					FrameLeftDefectCnt  = GetPolygonSize(LeftInPoint, LeftDefectPoint, LeftInEdgeCount, 100, 10,LeftDefectCnt, DarkThresHold); // Dark Defect 
					LeftDefectCnt +=FrameLeftDefectCnt;
				}

				// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
				if(LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
					// MaskImage Create Start 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					BinaryImg.SetSize        (GImage->GetWidth(), GImage->GetHeight()); // Image Masking Conversion Binary
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());	// Image Masking Org
					EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);
					//////////////////////////////////////////////////////////////////////////////////////////////////
					//MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP_����.bmp");

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black 
					HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//ClipperOffset USE 
					/*
					// Masking Data Init 
					if(MaskPoint !=NULL)  {
						delete []MaskPoint ;
					}
					if(MaskPoint2!=NULL)  {
						delete []MaskPoint2;
					}


					// ROI���� ��ǥ ������ �Ѱ��� 
					RoiLt.x = IRoi->GetOrgX()         ; RoiLt.y = IRoi->GetOrgY()          ;
					RoiRt.x = RoiLt.x+IRoi->GetWidth(); RoiRt.y = IRoi->GetOrgY()          ;
					RoiLb.x = RoiLt.x                 ; RoiLb.y = RoiLt.y+IRoi->GetHeight();
					RoiRb.x = RoiRt.x                 ; RoiRb.y = RoiRt.y+IRoi->GetHeight();

					// InEdgeData���� Out���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					The::Path  Poly_In ;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

					// Polygon InEdgePoint Data Send ClipperOffset 
					Poly_In.push_back(The::IntPoint(RoiLt.x,RoiLt.y));
					for(int k=0; k<LeftInEdgeCount; k++){
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
					Poly_In.push_back(The::IntPoint(RoiLb.x,RoiLb.y));

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);	

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;

					// InEdgeData���� In���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					Poly_In.clear();
					Poly_Out.clear();
					ClipOffset.Clear();
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

					// 					Poly_In.push_back(The::IntPoint(FrameImgW,0)); //Image���� ������ ��� �� ��ġ  
					Poly_In.push_back(The::IntPoint(RoiRt.x,RoiRt.y)); //Image���� ������ ��� �� ��ġ  
					for(int k=0; k<LeftInEdgeCount; k++){
						deg = 0.0f;
						//////////////////////////////////////////////////////////
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
					// 					Poly_In.push_back(The::IntPoint(FrameImgW,FrameImgH)); //Image���� ������ �ϴ� �� ��ġ  
					Poly_In.push_back(The::IntPoint(RoiRb.x,RoiRb.y));

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,-MaskOffsetDistance);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint2 = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint2[k].x = Poly_Out[0][k].x;
						MaskPoint2[k].y = Poly_Out[0][k].y;
					}


					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);
					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////
					*/

                    //OnFindPerpendicularPointXY Coordinage USE 
						// ù��° �̹��� Masking���� �� ��ǥ ��� 
					TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint = new POINT[TotalInCount];

					// Data Init 
					for(int k=0; k<TotalInCount; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
						MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;

						if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
						if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
					}

					// �а ������,���� 
					MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
					MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
					MaskStaPos                  =MinDv   ;

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);

					// �ι�° �̹��� Masking���� �� ��ǥ ��� 
					TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint2 = new POINT[TotalInCount2];

					// Data Init 
					for(int k=0; k<TotalInCount2; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint2[k+1].x = (int)MaskPoint[k+1].x+MaskOffsetDistance;
						MaskPoint2[k+1].y = (int)MaskPoint[k+1].y                   ;

						if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
						if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
					}

					// �а ������,���� 
					MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
					MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					if(MaskPoint !=NULL)  delete []MaskPoint ;
					if(MaskPoint2!=NULL)  delete []MaskPoint2;
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////

					if(MaskDarkDefectImg.GetImagePtr()){
						// Image Conversion Binary_Image
						// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP.bmp");
						EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
						// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP_Binary.bmp");

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BinaryImg.GetImagePtr();

// 						RoiStaX      =(int)(RoiLt.x)          ;
// 						RoiStaY      =(int)(RoiLt.y)          ;
// 						RoiWidth     =(int)(RoiRt.x - RoiLt.x);
// 						RoiHeight    =(int)(RoiLb.y - RoiLt.y);

						RoiStaX      =(int)(MaskStaPos)                                ;
						RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
						RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						// TEST 
						int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
						int TotalPixelCnt =0, PixelScore  =0;
						float Score     =0.0f;

						if(BlobCount!=0){
							for(int k=0; k<BlobCount; k++){
								IsDefect = false;
								Rect.left   = 0;
								Rect.right  = 0;
								Rect.top    = 0;
								Rect.bottom = 0;

								if(LTCirCrackCnt>=MAX_DEFECT_POINT) break;
								// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
								DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
								DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
								DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
								DefectRT.y  = (float)( DefectLT.y                                                         );
								DefectLB.x  = (float)( DefectLT.x                                                         );
								DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
								DefectRB.x  = (float)( DefectRT.x                                                         );
								DefectRB.y  = (float)( DefectLB.y                                                         );

								Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
								Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

								PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
								for(int n=Rect.top ; n<Rect.bottom; n++){
									for(int m=Rect.left; m<Rect.right; m++){
										PixelValue = IElement(m,n);
										if(PixelValue > BrokenThresHold) PixelCount++;
										TotalPixelCnt++;
									}
								}
								StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
								if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

								if(Score>PixelScore){
									IsDefect = true;
									for(int i=0; i<LeftInEdgeCount; i++){
										LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
										LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
										RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
										LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
										RBDis = Math.GetLength(LeftInPoint[i],DefectRB);

										if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
											LeftCrackPoint[LeftCrackCnt].LineAttach =1;
											break;
										}
									}

									if(IsDefect){
										LeftCrackPoint[LeftCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
										LeftCrackPoint[LeftCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

										LeftCrackPoint[LeftCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LeftCrackPoint[LeftCrackCnt].Width/2 ) +RoiStaX);
										LeftCrackPoint[LeftCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height/2) +RoiStaY);
										LeftCrackPoint[LeftCrackCnt].Rect.right  = (long)(LeftCrackPoint[LeftCrackCnt].Rect.left+ LeftCrackPoint[LeftCrackCnt].Width );
										LeftCrackPoint[LeftCrackCnt].Rect.bottom = (long)(LeftCrackPoint[LeftCrackCnt].Rect.top + LeftCrackPoint[LeftCrackCnt].Height);

										// Display ������ �ʿ��� 
										LeftCrackPoint[LeftCrackCnt].Round = PixelCount;
										LeftCrackPoint[LeftCrackCnt].P.x       = (float)(LTCirCrackPoint[LTCirCrackCnt].Rect.left);
										LeftCrackPoint[LeftCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height/2)+RoiStaY+ImageHeight );
										LeftCrackPoint[LeftCrackCnt].Width     = LeftCrackPoint[LeftCrackCnt].Width+SideMargin; 

										// Merge ���ؼ� ������ �ʿ��� 
										LeftCrackPoint[LeftCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftCrackPoint[LeftCrackCnt].Height/2)+RoiStaY+ImageHeight);
										LeftCrackPoint[LeftCrackCnt].Rect.bottom =(long)(LeftCrackPoint[LeftCrackCnt].Rect.top + LeftCrackPoint[LeftCrackCnt].Height);
										LeftCrackPoint[LeftCrackCnt].Ver_Dis      =LeftCrackPoint[LeftCrackCnt].Height;
										LeftCrackPoint[LeftCrackCnt].Hor_Dis      =LeftCrackPoint[LeftCrackCnt].Width ;
										LeftCrackCnt++;
									}
								}
								else                {
									continue; // Score���Ͽ� �ҷ��� ����ó��
								}
							}
						}
						BlobCount =0;
					}
				}

				// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
				// ���� ��ü���� Dark Masking
				// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && LTCirDeftectCnt==0 && MaskDefectThresHold!=0){
				if(LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
					// MaskImage Create Start 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
					EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

					// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_HAP_����.bmp");
					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
					HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//ClipperOffset USE 
					/*
					// Masking Data Init 
					if(MaskPoint !=NULL)  {
						delete []MaskPoint ;
					}
					if(MaskPoint2!=NULL)  {
						delete []MaskPoint2;
					}

					// ROI���� ��ǥ ������ �Ѱ��� 
					RoiLt.x = IRoi->GetOrgX()         ; RoiLt.y = IRoi->GetOrgY()          ;
					RoiRt.x = RoiLt.x+IRoi->GetWidth(); RoiRt.y = IRoi->GetOrgY()          ;
					RoiLb.x = RoiLt.x                 ; RoiLb.y = RoiLt.y+IRoi->GetHeight();
					RoiRb.x = RoiRt.x                 ; RoiRb.y = RoiRt.y+IRoi->GetHeight();

					// InEdgeData���� Out���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					The::Path  Poly_In ;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

					// Polygon InEdgePoint Data Send ClipperOffset 
					Poly_In.push_back(The::IntPoint(RoiLt.x,RoiLt.y));
					for(int k=0; k<LeftInEdgeCount; k++){
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
					Poly_In.push_back(The::IntPoint(RoiLb.x,RoiLb.y));

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);	
					//////////////////////////////////////////////////////////////////////////////////////////////////


					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;

					// InEdgeData���� In���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					Poly_In.clear();
					Poly_Out.clear();
					ClipOffset.Clear();
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

					// 					Poly_In.push_back(The::IntPoint(FrameImgW,0)); //Image���� ������ ��� �� ��ġ  
					Poly_In.push_back(The::IntPoint(RoiRt.x,RoiRt.y)); //Image���� ������ ��� �� ��ġ  
					for(int k=0; k<LeftInEdgeCount; k++){
						deg = 0.0f;
						//////////////////////////////////////////////////////////
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
					Poly_In.push_back(The::IntPoint(RoiRb.x,RoiRb.y));
					// 					Poly_In.push_back(The::IntPoint(FrameImgW,FrameImgH)); //Image���� ������ �ϴ� �� ��ġ  

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,-MaskOffsetDistance);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint2 = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint2[k].x = Poly_Out[0][k].x;
						MaskPoint2[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);
					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////
					*/

                     //OnFindPerpendicularPointXY Coordinage USE 

					TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint = new POINT[TotalInCount];

					// Data Init 
					for(int k=0; k<TotalInCount; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					MinDv =100000.0f;

					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
						MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;

						if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
						if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
					}

					// �а ������,���� 
					MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
					MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
					MaskStaPos                  =MinDv   ;

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);

					// �ι�° �̹��� Masking���� �� ��ǥ ��� 
					TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint2 = new POINT[TotalInCount2];

					// Data Init 
					for(int k=0; k<TotalInCount2; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint2[k+1].x = (int)MaskPoint[k+1].x+MaskOffsetDistance;
						MaskPoint2[k+1].y = (int)MaskPoint[k+1].y                   ;

						if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
						if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
					}

					// �а ������,���� 
					MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
					MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					if(MaskPoint !=NULL) delete []MaskPoint ;
					if(MaskPoint2!=NULL) delete []MaskPoint2;
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////

					if(MaskDarkDefectImg.GetImagePtr()){
						// Image Conversion Binary_Image
// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\WATCH_����_STATIC.bmp");
						EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
// 						BinaryImg.Save("D:\\ROTATE_IMG\\WATCH_����_STATIC_Binary.bmp");

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = BinaryImg.GetImagePtr();
						// 						IAddress = MaskDarkDefectImg.GetImagePtr();

// 						RoiStaX      =(int)(RoiLt.x)          ;
// 						RoiStaY      =(int)(RoiLt.y)          ;
// 						RoiWidth     =(int)(RoiRt.x - RoiLt.x);
// 						RoiHeight    =(int)(RoiLb.y - RoiLt.y);

						RoiStaX      =(int)(MaskStaPos)                                ;
						RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
						RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);


						if(BlobCount!=0){
							for(int k=0; k<BlobCount; k++){
								if(LTCirDeftectCnt>=MAX_DEFECT_POINT) break;
								// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
								DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
								DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
								DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
								DefectRT.y  = (float)( DefectLT.y                                                         );
								DefectLB.x  = (float)( DefectLT.x                                                         );
								DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
								DefectRB.x  = (float)( DefectRT.x                                                         );
								DefectRB.y  = (float)( DefectLB.y                                                         );

								for(int i=0; i<LeftInEdgeCount; i++){
									LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
									IsDefect = false;
									LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
									RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
									LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
									RBDis = Math.GetLength(LeftInPoint[i],DefectRB);
									if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
										IsDefect = true; 
										break;
									}
								}

								if(IsDefect){
									LeftDefectPoint[LeftDefectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
									LeftDefectPoint[LeftDefectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

									LeftDefectPoint[LeftDefectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LeftDefectPoint[LeftDefectCnt].Width/2 ) +RoiStaX);
									LeftDefectPoint[LeftDefectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height/2) +RoiStaY);
									LeftDefectPoint[LeftDefectCnt].Rect.right  = (long)(LeftDefectPoint[LeftDefectCnt].Rect.left+ LeftDefectPoint[LeftDefectCnt].Width );
									LeftDefectPoint[LeftDefectCnt].Rect.bottom = (long)(LeftDefectPoint[LeftDefectCnt].Rect.top + LeftDefectPoint[LeftDefectCnt].Height);

									PixelCount = 0;
									PixelValue = 0;
									// ���� Pixel Data ���� 
									for(int n=LeftDefectPoint[LeftDefectCnt].Rect.top+1 ; n<LeftDefectPoint[LeftDefectCnt].Rect.bottom-1; n++){
										for(int m=LeftDefectPoint[LeftDefectCnt].Rect.left+1; m<LeftDefectPoint[LeftDefectCnt].Rect.right-1 ; m++){
											PixelValue = IElement(m,n);
											if(PixelValue < MaskDefectThresHold) PixelCount++;
										}
									}
									// Display ������ �ʿ��� 
									LeftDefectPoint[LeftDefectCnt].Round = PixelCount;
									LeftDefectPoint[LeftDefectCnt].P.x       = (float)(LeftDefectPoint[LeftDefectCnt].Rect.left);
									LeftDefectPoint[LeftDefectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height/2)+RoiStaY+ImageHeight );
									LeftDefectPoint[LeftDefectCnt].Width     = LeftDefectPoint[LeftDefectCnt].Width+SideMargin; 

									// Merge ���ؼ� ������ �ʿ��� 
									LeftDefectPoint[LeftDefectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LeftDefectPoint[LeftDefectCnt].Height/2)+RoiStaY+ImageHeight);
									LeftDefectPoint[LeftDefectCnt].Rect.bottom =(long)(LeftDefectPoint[LeftDefectCnt].Rect.top + LeftDefectPoint[LeftDefectCnt].Height);
									LeftDefectCnt++;
								}
							}
						}
						BlobCount =0;
					}
				}
			}
			else          { //�κп��� 
				// �˻� ROI���� 
				LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
				LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);

				//Align Ʋ���� �����Ͽ� �˻�ROI���� ������ 
				if(fabs(LSP.x - LEP.x)>SideMargin){
					if     (LSP.x>LEP.x){LSP.x = LEP.x+((LSP.x - LEP.x)/2);}
					else if(LSP.x<LEP.x){LSP.x = LSP.x+((LEP.x - LSP.x)/2);}
				}

				RoiStaX      =(int)(LSP.x+SideMargin   );
				RoiStaY      =(int)(LSP.y              );
				RoiWidth     =200                       ;
				RoiHeight    =(int)(FrameImgH - RoiStaY);

				// �˻� ������ 
				DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
				if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
				if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
				if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

				// Blob �˻� ���� 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);

				// Blob ���͸� 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 2          , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(DCount>=MAX_DEFECT_POINT) break;
						DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
						DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
						DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
						DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

						PixelCount = 0;
						PixelValue = 0;
						// ���� Pixel Data ���� 
						for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
							for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < DarkThresHold) PixelCount++;
							}
						}
						// Display ������ �ʿ��� 
						DDefectPoint[DCount].Round = PixelCount;
						DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
						DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight );
						DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

						// Merge ���ؼ� ������ �ʿ��� 
						DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
						DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
						DCount++;
					}
				}
				BlobCount =0;

				// Broken �� �ڸ� �ҷ� �˻� ������(EnCap�� ������)
				if(!IsMarkBtm && BrokenThresHold!=0){
					BrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight())       ;
					EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BrightDefectImg);
					EasyImage::Threshold(GImage    ,&BrightDefectImg, BrokenThresHold)     ;

					DefectROI.Attach(&BrightDefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
					IAddress = BrightDefectImg.GetImagePtr();

					// Side_D Bright Defect ���� �� �з�(Broken,�ݿ��� �ڸ�___�ɹڸ�)
					/////////////////////////////////////////////////////////////////////////////////////////////////
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

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
							if(DCrackCount>=MAX_DEFECT_POINT) break; 
							DCrackPoint[DCrackCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
							DCrackPoint[DCrackCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

							DCrackPoint[DCrackCount].Rect.left   =(long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width/2) +RoiStaX));
							DCrackPoint[DCrackCount].Rect.top    =(long)(( CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/2)+RoiStaY );
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
			}
		}
		Index = 0;
		return true;

		// NOTCH RectC : �Ÿ��
		//------------------------------------------------------------------
	case 2 :
		if(FGlassType==Mobile_Glass){ //���� or ���� =>��ü���� or �κп��� 
			if(IsShortCell){ //���� �˻� 
				if(IsAllGrind){ //��ü����(Notch,Narrow,CAI)
					// Image Conversion Binary_Image, Pixel Value ���� 
					EasyImage::Threshold(GImage,&DefectImg, LeftEdgeThresHold);
					IAddress = DefectImg.GetImagePtr();

					// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					if(LeftInEdgeCount!=0){    
						LBCirDeftectCnt  = GetPolygonSize(LeftInPoint, LBCirDefectPoint, LeftInEdgeCount, 100, 10,0,LeftEdgeThresHold); // Dark Defect 
					}

					// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
					if(LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

						// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_CUP_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						// ù��° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k-1].x;  
								EndPos.y =(int)LeftInPoint[k-1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k+1].x;  
								EndPos.y =(int)LeftInPoint[k+1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,-MaskDefectOffset,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint[k+1].x = TargetPoint.x;
							MaskPoint[k+1].y = TargetPoint.y;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv   ;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);


						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k  ].x;  
								EndPos.y =(int)MaskPoint[k  ].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k+2].x;  
								EndPos.y =(int)MaskPoint[k+2].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,-MaskOffsetDistance,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint2[k+1].x = TargetPoint.x;
							MaskPoint2[k+1].y = TargetPoint.y;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
						if(MaskPoint !=NULL)  delete []MaskPoint ;
						if(MaskPoint2!=NULL)  delete []MaskPoint2;
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_CUP.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
							// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_CUP_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
							IAddress = BinaryImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                ;
							RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
							// 						RoiWidth     =(int)(MaxDv- MinDv)                              ;
							RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0       ) return false;
							if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;

							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

							// TEST 
							int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
							int TotalPixelCnt =0, PixelScore  =0;
							float Score     =0.0f;

							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									IsDefect = false;
									Rect.left   = 0;
									Rect.right  = 0;
									Rect.top    = 0;
									Rect.bottom = 0;

									if(LBCirCrackCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
									Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

									PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
									for(int n=Rect.top ; n<Rect.bottom; n++){
										for(int m=Rect.left; m<Rect.right; m++){
											PixelValue = IElement(m,n);
											if(PixelValue > BrokenThresHold) PixelCount++;
											TotalPixelCnt++;
										}
									}
									StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
									if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

									if(Score>PixelScore){
										IsDefect = true;
										for(int i=0; i<LeftInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
											RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
											LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
											RBDis = Math.GetLength(LeftInPoint[i],DefectRB);

											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
												LBCirCrackPoint[LBCirCrackCnt].LineAttach =1;
												break;
											}
										}

										if(IsDefect){
											LBCirCrackPoint[LBCirCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LBCirCrackPoint[LBCirCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LBCirCrackPoint[LBCirCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirCrackPoint[LBCirCrackCnt].Width/2 ) +RoiStaX);
											LBCirCrackPoint[LBCirCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height/2) +RoiStaY);
											LBCirCrackPoint[LBCirCrackCnt].Rect.right  = (long)(LBCirCrackPoint[LBCirCrackCnt].Rect.left+ LBCirCrackPoint[LBCirCrackCnt].Width );
											LBCirCrackPoint[LBCirCrackCnt].Rect.bottom = (long)(LBCirCrackPoint[LBCirCrackCnt].Rect.top + LBCirCrackPoint[LBCirCrackCnt].Height);

											// Display ������ �ʿ��� 
											LBCirCrackPoint[LBCirCrackCnt].Round = PixelCount;
											LBCirCrackPoint[LBCirCrackCnt].P.x       = (float)(LBCirCrackPoint[LBCirCrackCnt].Rect.left);
											LBCirCrackPoint[LBCirCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height/2)+RoiStaY+ImageHeight );
											LBCirCrackPoint[LBCirCrackCnt].Width     = LBCirCrackPoint[LBCirCrackCnt].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LBCirCrackPoint[LBCirCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height/2)+RoiStaY+ImageHeight);
											LBCirCrackPoint[LBCirCrackCnt].Rect.bottom =(long)(LBCirCrackPoint[LBCirCrackCnt].Rect.top + LBCirCrackPoint[LBCirCrackCnt].Height);
											LBCirCrackPoint[LBCirCrackCnt].Ver_Dis      =LBCirCrackPoint[LBCirCrackCnt].Height;
											LBCirCrackPoint[LBCirCrackCnt].Hor_Dis      =LBCirCrackPoint[LBCirCrackCnt].Width ;
											LeftCrackCnt++;
										}
									}
									else                {
										continue; // Score���Ͽ� �ҷ��� ����ó��
									}
								}
							}
							BlobCount =0;
						}
					}

					// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
					// 				if(FrameLeftDefectCnt==0 && Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
					if(LeftInEdgeCount!=0 && MaskDefectThresHold!=0){

						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);
						// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_CUP_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						TotalInCount =LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k-1].x;  
								EndPos.y =(int)LeftInPoint[k-1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k+1].x;  
								EndPos.y =(int)LeftInPoint[k+1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,-MaskDefectOffset,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint[k+1].x = TargetPoint.x;
							MaskPoint[k+1].y = TargetPoint.y;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv-20;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);

						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						double deg =0.0f;

						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k  ].x;  
								EndPos.y =(int)MaskPoint[k  ].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k+2].x;  
								EndPos.y =(int)MaskPoint[k+2].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,-MaskOffsetDistance,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint2[k+1].x = TargetPoint.x;
							MaskPoint2[k+1].y = TargetPoint.y;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
						if(MaskPoint !=NULL) delete []MaskPoint ;
						if(MaskPoint2!=NULL) delete []MaskPoint2;
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_CUP.bmp");
							// 						EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,EnCapTopThresHold);
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
							// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_CUP_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
							IAddress = BinaryImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                              ;
							RoiStaY      =(int)( LeftInPoint[0].y+MaskingMargin)                         ;
							RoiWidth     =(int)(MaxDv- MinDv)                                            ;
							// 						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY-MaskingMargin);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)<=0       ) return false;
							if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;


							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									if(LBCirDeftectCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									for(int i=0; i<LeftInEdgeCount; i++){
										LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
										IsDefect = false;
										LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
										RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
										LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
										RBDis = Math.GetLength(LeftInPoint[i],DefectRB);
										// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
										// 									if(LTDis<=MaskDefectOffset || RTDis<=MaskDefectOffset || LBDis<=MaskDefectOffset || RBDis<=MaskDefectOffset){ 
										if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
											IsDefect = true; 
											break;
										}
									}

									if(IsDefect){
										LBCirDefectPoint[LBCirDeftectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
										LBCirDefectPoint[LBCirDeftectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

										LBCirDefectPoint[LBCirDeftectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirDefectPoint[LBCirDeftectCnt].Width/2 ) +RoiStaX);
										LBCirDefectPoint[LBCirDeftectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2) +RoiStaY);
										LBCirDefectPoint[LBCirDeftectCnt].Rect.right  = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left+ LBCirDefectPoint[LBCirDeftectCnt].Width );
										LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);

										PixelCount = 0;
										PixelValue = 0;
										// ���� Pixel Data ���� 
										for(int n=LBCirDefectPoint[LBCirDeftectCnt].Rect.top+1 ; n<LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom-1; n++){
											for(int m=LBCirDefectPoint[LBCirDeftectCnt].Rect.left+1; m<LBCirDefectPoint[LBCirDeftectCnt].Rect.right-1 ; m++){
												PixelValue = IElement(m,n);
												if(PixelValue < MaskDefectThresHold) PixelCount++;
											}
										}
										// Display ������ �ʿ��� 
										LBCirDefectPoint[LBCirDeftectCnt].Round = PixelCount;
										LBCirDefectPoint[LBCirDeftectCnt].P.x       = (float)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left);
										LBCirDefectPoint[LBCirDeftectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2)+RoiStaY+ImageHeight );
										LBCirDefectPoint[LBCirDeftectCnt].Width     = LBCirDefectPoint[LBCirDeftectCnt].Width+SideMargin; 

										// Merge ���ؼ� ������ �ʿ��� 
										LBCirDefectPoint[LBCirDeftectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2)+RoiStaY+ImageHeight);
										LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom =(long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);
										LeftDefectCnt++;
									}
								}
							}
							BlobCount =0;
						}
					}
				}
				else          { //�κп���
					if(IsRectBtm)       {
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						LSP = Math.OnIntersect(LeftOutLine [0].FP0, LeftOutLine [0].FP1, LT, RT);

						RoiStaX      =(int)(LSP.x+SideMargin);
						RoiStaY      =(int)(LSP.y           );
						RoiWidth     =200                    ;
						RoiHeight    =(int)(FLBPoint.y)      ;

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)<=0       ) return false;
						if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						// Blob ���͸� 
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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight );
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
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
					else if(IsCornerBtm) {
						// Image Conversion Binary_Image, Pixel Value ���� 
						EasyImage::Threshold(GImage,&DefectImg, CorGrindInThresHold_End);
						IAddress = DefectImg.GetImagePtr();

						if(LBCorInEdgeCount!=0){
							CornerCount=0;
							for(int k=0; k<LBCorInEdgeCount; k++){
								if(LBCorInPoint[k].y>FLBLPoint.y-1) continue;
								Dis = Math.GetLengthFrLineToPoint(FLBLInPoint, FLBBInPoint, LBCorInPoint[k]);
								LBCorner[CornerCount].Dv   = Dis         ;
								LBCorner[CornerCount].P.x  = LBCorInPoint[k].x;
								LBCorner[CornerCount].P.y  = LBCorInPoint[k].y;
								CornerCount++;
								if(Dis>MaxDv) MaxDv = Dis;
								if(Dis<MinDv) MinDv = Dis;
							}
							if(LBCornerWidth>(LBCornerHeight*2)) LBCorCount = GetSize(LBCorner, LBCorDefectPoint, CornerCount, 10);
							else                                 LBCorCount = GetSize(LBCorner, LBCorDefectPoint, CornerCount, 5 );
						}

						// SIDE_D�˻� 
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						LSP = Math.OnIntersect(LeftOutLine [0].FP0, LeftOutLine [0].FP1, LT, RT);

						RoiStaX      =(int)(LSP.x+SideMargin);
						RoiStaY      =(int)(LSP.y           );
						RoiWidth     =200                    ;
						RoiHeight    =(int)(FLBLInPoint.y)   ;

						//�˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth,RoiHeight);
						if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)<=0       ) return false;
						if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						// Blob ���͸� 
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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
								DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
								DCount++;
							}
						}
						BlobCount =0;

						//HSKIM Corner ���� ���� Masking �߰�
						//					if(Polygon->Blob[0].InCnt!=0 && LTCorInEdgeCount!=0 && MaskDefectThresHold!=0){
						if(LBCorInEdgeCount!=0 && MaskDefectThresHold!=0){
							// MaskImage Create Start \
							//////////////////////////////////////////////////////////////////////////////////////////////////
							BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

							HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
							HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

							TotalInCount = LBCorInEdgeCount+2; 
							MaskPoint = new POINT[TotalInCount];

							// Data Init 
							for(int k=0; k<TotalInCount; k++){
								MaskPoint[k].x = 0;
								MaskPoint[k].y = 0;
							}

							// Data Convert 
							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LBCorInEdgeCount; k++){
								MaskPoint[k+1].x = (int)LBCorInPoint[k].x+MaskDefectOffset;
								MaskPoint[k+1].y = (int)LBCorInPoint[k].y                 ;

								if(k==(LBCorInEdgeCount-1)){
									StaPos.x =(int)LBCorInPoint[k  ].x; 
									StaPos.y =(int)LBCorInPoint[k  ].y; 
									EndPos.x =(int)LBCorInPoint[k-1].x;  
									EndPos.y =(int)LBCorInPoint[k-1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)LBCorInPoint[k  ].x; 
									StaPos.y =(int)LBCorInPoint[k  ].y; 
									EndPos.x =(int)LBCorInPoint[k+1].x;  
									EndPos.y =(int)LBCorInPoint[k+1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,0,deg);

								MaskPoint[k+1].x = TargetPoint.x;
								MaskPoint[k+1].y = TargetPoint.y;

								if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
								if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
							}
							// �а ������,���� 
							MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LBCorInPoint[0                 ].y;
							MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LBCorInPoint[LBCorInEdgeCount-1].y; 
							MaskStaPos                  =MinDv-20;

							HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
							::FillRgn(hdc, Rgn1, fillBrh);
							::DeleteObject(Rgn1);

							// �ι�° �̹��� Masking���� �� ��ǥ ��� 
							TotalInCount2 = LBCorInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint2 = new POINT[TotalInCount2];

							// Data Init 
							for(int k=0; k<TotalInCount2; k++){
								MaskPoint2[k].x = 0;
								MaskPoint2[k].y = 0;
							}

							// Data Convert 
							MinDv        =100000.0f;
							MaxDv        = 0.0f;
							CorMaskingDy = 100000.0f;
							for(int k=0; k<LBCorInEdgeCount; k++){
								MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
								MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LBCorInEdgeCount-1)){
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k  ].x;  
									EndPos.y =(int)MaskPoint[k  ].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k+2].x;  
									EndPos.y =(int)MaskPoint[k+2].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,-MaskOffsetDistance,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint2[k+1].x = TargetPoint.x;
								MaskPoint2[k+1].y = TargetPoint.y;

								if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
								if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;

								//�а�� �ƴ� Corner ����, �� Y ��ǥ ���� ����
								if(MaskPoint2[k+1].y<CorMaskingDy) CorMaskingDy   = MaskPoint2[k+1].y;
							}

							// �а ������,���� 
							// 						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LBCorInPoint[0                 ].y;
							// 						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LBCorInPoint[LBCorInEdgeCount-1].y; 
							//Corner Masking ���� ����, �ι�° Masking������ ������
							MaskPoint2[0].x               =MinDv+20; MaskPoint2[0].y               = CorMaskingDy;
							MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = CorMaskingDy; 

							HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
							::FillRgn(hdc, Rgn2, fillBrh);
							::DeleteObject(Rgn2);
							::DeleteObject(fillBrh);

							Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
							if(MaskPoint !=NULL)  delete []MaskPoint ;
							if(MaskPoint2!=NULL)  delete []MaskPoint2;

							if(MaskDarkDefectImg.GetImagePtr()){
								// Image Conversion Binary_Image
//								MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\MaskDarkDefectImg_CUP_����.bmp");
								EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
//								BinaryImg.Save("D:\\ROTATE_IMG\\MaskDarkDefectImg_CUP_Binary.bmp");

								// Image Euresys Algorithm Attach 
								DefectROI.Attach(&BinaryImg);
								CodeImageSelection.SetFeretAngle(0.00f);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
								IAddress = BinaryImg.GetImagePtr();
								// 						IAddress = MaskDarkDefectImg.GetImagePtr();

								RoiStaX      =(int)(MaskStaPos)                                  ;
								RoiStaY      =(int)(LBCorInPoint[0].y)             ;
//								RoiStaY      =(int)(LBCorInPoint[0].y+MaskingMargin)             ; Original 
								RoiWidth     =(int)(MaxDv- MinDv)                                ;
								RoiHeight    =(int)(LBCorInPoint[LBCorInEdgeCount-1].y - RoiStaY);

								// �˻� ������ 
								DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
								if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
								if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
								if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

								// Blob �˻� ���� 
								CodeImageSelection.Clear();
								CodeImageEncoder.Encode(DefectROI,CodeImage);
								CodeImageSelection.Clear();
								CodeImageSelection.AddObjects(CodeImage);

								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

								BlobCount = CodeImageSelection.GetElementCount();
								CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

								if(BlobCount!=0){
									for(int k=0; k<BlobCount; k++){
										if(LBCorCount>=MAX_DEFECT_POINT) break;
										// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
										DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
										DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
										DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
										DefectRT.y  = (float)( DefectLT.y                                                         );
										DefectLB.x  = (float)( DefectLT.x                                                         );
										DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
										DefectRB.x  = (float)( DefectRT.x                                                         );
										DefectRB.y  = (float)( DefectLB.y                                                         );

										for(int i=0; i<LBCorInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											IsDefect = false;
											LTDis = Math.GetLength(LBCorInPoint[i],DefectLT);
											RTDis = Math.GetLength(LBCorInPoint[i],DefectRT);
											LBDis = Math.GetLength(LBCorInPoint[i],DefectLB);
											RBDis = Math.GetLength(LBCorInPoint[i],DefectRB);
											// 										if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
											// 										if(LTDis<=MaskDefectOffset || RTDis<=MaskDefectOffset || LBDis<=MaskDefectOffset || RBDis<=MaskDefectOffset){ 
											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
//											if(LTDis<=MaskDefectOffset+10 || RTDis<=MaskDefectOffset+10 || LBDis<=MaskDefectOffset+10 || RBDis<=MaskDefectOffset+10){ 
												IsDefect = true; 
												break;
											}
										}

										if(IsDefect){
											LBCorDefectPoint[LBCorCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LBCorDefectPoint[LBCorCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LBCorDefectPoint[LBCorCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCorDefectPoint[LBCorCount].Width/2 ) +RoiStaX);
											LBCorDefectPoint[LBCorCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCorDefectPoint[LBCorCount].Height/2) +RoiStaY);
											LBCorDefectPoint[LBCorCount].Rect.right  = (long)(LBCorDefectPoint[LBCorCount].Rect.left+ LBCorDefectPoint[LBCorCount].Width );
											LBCorDefectPoint[LBCorCount].Rect.bottom = (long)(LBCorDefectPoint[LBCorCount].Rect.top + LBCorDefectPoint[LBCorCount].Height);

											PixelCount = 0;
											PixelValue = 0;
											// ���� Pixel Data ���� 
											for(int n=LBCorDefectPoint[LBCorCount].Rect.top+1 ; n<LBCorDefectPoint[LBCorCount].Rect.bottom-1; n++){
												for(int m=LBCorDefectPoint[LBCorCount].Rect.left+1; m<LBCorDefectPoint[LBCorCount].Rect.right-1 ; m++){
													PixelValue = IElement(m,n);
													if(PixelValue < MaskDefectThresHold) PixelCount++;
												}
											}
											// Display ������ �ʿ��� 
											LBCorDefectPoint[LBCorCount].Round = PixelCount;
											LBCorDefectPoint[LBCorCount].P.x       = (float)(LBCorDefectPoint[LBCorCount].Rect.left);
											LBCorDefectPoint[LBCorCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCorDefectPoint[LBCorCount].Height/2)+RoiStaY+ImageHeight );
											LBCorDefectPoint[LBCorCount].Width     = LBCorDefectPoint[LBCorCount].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LBCorDefectPoint[LBCorCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCorDefectPoint[LBCorCount].Height/2)+RoiStaY+ImageHeight);
											LBCorDefectPoint[LBCorCount].Rect.bottom =(long)(LBCorDefectPoint[LBCorCount].Rect.top + LBCorDefectPoint[LBCorCount].Height);
											LBCorCount++;
										}
									}
								}
								BlobCount =0;
							}
						}

						// �ܰ����ο��� Mark������ �Ÿ� ���� 
						if     (Line[waLeftWall].FP0.x >Line[waLeftWall].FP1.x) LDistance = (FP0.x - (Line[waLeftWall].FP1.x+(Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x)/2));
						else if(Line[waLeftWall].FP0.x==Line[waLeftWall].FP1.x) LDistance = (FP0.x -  Line[waLeftWall].FP0.x)                                                     ;
						else                                                    LDistance = (FP0.x - (Line[waLeftWall].FP0.x+(Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x)/2)); 
					}
					else if(IsCircleBtm){ // ���� �Ű˻� 
						// Image Conversion Binary_Image, Pixel Value ���� 
						EasyImage::Threshold(GImage,&DefectImg, ECThresHold_End);
						// 					EasyImage::Threshold(GImage,&DefectImg, BtmEdgeThresHold);
						IAddress = DefectImg.GetImagePtr();

						// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
						if(LBCirInEdgeCount!=0){    
							// 						LBCirDeftectCnt  = GetPolygonSize(LBCirInPoint, LBCirDefectPoint, LBCirInEdgeCount, 50, 5,0,ECThresHold); // Dark Defect 
							LBCirDeftectCnt  = GetPolygonSize(LBCirInPoint, LBCirDefectPoint, LBCirInEdgeCount, 100, 10,0,ECThresHold_End); // Dark Defect 
						}

						// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
						// ���� �κа˻� Bright Masking 
						if(Polygon->Blob[0].InCnt!=0 && LBCirInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
							// MaskImage Create Start 
							//////////////////////////////////////////////////////////////////////////////////////////////////
							BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_WHITE_MaskDarkDefectImg_CUP_����.bmp");

							HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0));
							HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

							// ù��° �̹��� Masking���� �� ��ǥ ��� 
							TotalInCount = LBCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint = new POINT[TotalInCount];

							// Data Init 
							for(int k=0; k<TotalInCount; k++){
								MaskPoint[k].x = 0;
								MaskPoint[k].y = 0;
							}

							// Data Convert 
							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LBCirInEdgeCount; k++){
								MaskPoint[k+1].x = (int)LBCirInPoint[k].x+MaskDefectOffset;
								MaskPoint[k+1].y = (int)LBCirInPoint[k].y                 ;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LBCirInEdgeCount-1)){
									StaPos.x =(int)LBCirInPoint[k  ].x; 
									StaPos.y =(int)LBCirInPoint[k  ].y; 
									EndPos.x =(int)LBCirInPoint[k-1].x;  
									EndPos.y =(int)LBCirInPoint[k-1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)LBCirInPoint[k  ].x; 
									StaPos.y =(int)LBCirInPoint[k  ].y; 
									EndPos.x =(int)LBCirInPoint[k+1].x;  
									EndPos.y =(int)LBCirInPoint[k+1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,-MaskDefectOffset,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint[k+1].x = TargetPoint.x;
								MaskPoint[k+1].y = TargetPoint.y;

								if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
								if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
							}

							// �а ������,���� 
							MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LBCirInPoint[0                ].y;
							MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LBCirInPoint[LBCirInEdgeCount-1].y; 
							MaskStaPos                  =MinDv   ;

							HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
							::FillRgn(hdc, Rgn1, fillBrh);
							::DeleteObject(Rgn1);

							// �ι�° �̹��� Masking���� �� ��ǥ ��� 
							TotalInCount2 = LBCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint2 = new POINT[TotalInCount2];

							// Data Init 
							for(int k=0; k<TotalInCount2; k++){
								MaskPoint2[k].x = 0;
								MaskPoint2[k].y = 0;
							}

							// Data Convert 
							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LBCirInEdgeCount; k++){
								MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
								MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LBCirInEdgeCount-1)){
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k  ].x;  
									EndPos.y =(int)MaskPoint[k  ].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k+2].x;  
									EndPos.y =(int)MaskPoint[k+2].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,-MaskOffsetDistance,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint2[k+1].x = TargetPoint.x;
								MaskPoint2[k+1].y = TargetPoint.y;

								if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
								if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
							}

							// �а ������,���� 
							MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LBCirInPoint[0                ].y;
							MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LBCirInPoint[LBCirInEdgeCount-1].y; 

							HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
							::FillRgn(hdc, Rgn2, fillBrh);
							::DeleteObject(Rgn2);
							::DeleteObject(fillBrh);

							Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
							if(MaskPoint !=NULL)  delete []MaskPoint ;
							if(MaskPoint2!=NULL)  delete []MaskPoint2;
							// MaskImage Create End
							//////////////////////////////////////////////////////////////////////////////////////////////////

							if(MaskDarkDefectImg.GetImagePtr()){
								// Image Conversion Binary_Image
								// 							MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_WHITE_MaskDarkDefectImg_CUP.bmp");
								EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
								// 							BinaryImg.Save("D:\\ROTATE_IMG\\����_�κ�_WHITE_MaskDarkDefectImg_CUP_Binary.bmp");

								// Image Euresys Algorithm Attach 
								DefectROI.Attach(&BinaryImg);
								CodeImageSelection.SetFeretAngle(0.00f);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
								IAddress = BinaryImg.GetImagePtr();

								RoiStaX      =(int)(MaskStaPos)                                ;
								RoiStaY      =(int)(LBCirInPoint[0].y+MaskingMargin)           ;
								// 						RoiWidth     =(int)(MaxDv- MinDv)                              ;
								RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
								RoiHeight    =(int)(LBCirInPoint[LBCirInEdgeCount-1].y - RoiStaY);

								// �˻� ������ 
								DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
								if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0       ) return false;
								if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
								if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;

								// Blob �˻� ���� 
								CodeImageSelection.Clear();
								CodeImageEncoder.Encode(DefectROI,CodeImage);
								CodeImageSelection.Clear();
								CodeImageSelection.AddObjects(CodeImage);

								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

								BlobCount = CodeImageSelection.GetElementCount();
								CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

								// TEST 
								int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
								int TotalPixelCnt =0, PixelScore  =0;
								float Score     =0.0f;

								if(BlobCount!=0){
									for(int k=0; k<BlobCount; k++){
										IsDefect = false;
										Rect.left   = 0;
										Rect.right  = 0;
										Rect.top    = 0;
										Rect.bottom = 0;

										if(LBCirCrackCnt>=MAX_DEFECT_POINT) break;
										// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
										DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
										DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
										DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
										DefectRT.y  = (float)( DefectLT.y                                                         );
										DefectLB.x  = (float)( DefectLT.x                                                         );
										DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
										DefectRB.x  = (float)( DefectRT.x                                                         );
										DefectRB.y  = (float)( DefectLB.y                                                         );

										Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
										Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

										PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
										for(int n=Rect.top ; n<Rect.bottom; n++){
											for(int m=Rect.left; m<Rect.right; m++){
												PixelValue = IElement(m,n);
												if(PixelValue > BrokenThresHold) PixelCount++;
												TotalPixelCnt++;
											}
										}
										StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
										if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

										if(Score>PixelScore){
											IsDefect = true;
											for(int i=0; i<LBCirInEdgeCount; i++){
												LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
												LTDis = Math.GetLength(LBCirInPoint[i],DefectLT);
												RTDis = Math.GetLength(LBCirInPoint[i],DefectRT);
												LBDis = Math.GetLength(LBCirInPoint[i],DefectLB);
												RBDis = Math.GetLength(LBCirInPoint[i],DefectRB);

												if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
													LBCirCrackPoint  [LBCirCrackCnt].LineAttach =1;
													break;
												}
											}

											if(IsDefect){
												LBCirCrackPoint  [LBCirCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
												LBCirCrackPoint  [LBCirCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

												LBCirCrackPoint  [LBCirCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirCrackPoint  [LBCirCrackCnt].Width/2 ) +RoiStaX);
												LBCirCrackPoint  [LBCirCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint  [LBCirCrackCnt].Height/2) +RoiStaY);
												LBCirCrackPoint  [LBCirCrackCnt].Rect.right  = (long)(LBCirCrackPoint  [LBCirCrackCnt].Rect.left+ LBCirCrackPoint  [LBCirCrackCnt].Width );
												LBCirCrackPoint  [LBCirCrackCnt].Rect.bottom = (long)(LBCirCrackPoint  [LBCirCrackCnt].Rect.top + LBCirCrackPoint  [LBCirCrackCnt].Height);

												// Display ������ �ʿ��� 
												LBCirCrackPoint  [LBCirCrackCnt].Round = PixelCount;
												LBCirCrackPoint  [LBCirCrackCnt].P.x       = (float)(LBCirCrackPoint  [LBCirCrackCnt].Rect.left);
												LBCirCrackPoint  [LBCirCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint  [LBCirCrackCnt].Height/2)+RoiStaY+ImageHeight );
												LBCirCrackPoint  [LBCirCrackCnt].Width     = LBCirCrackPoint  [LBCirCrackCnt].Width+SideMargin; 

												// Merge ���ؼ� ������ �ʿ��� 
												LBCirCrackPoint  [LBCirCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint  [LBCirCrackCnt].Height/2)+RoiStaY+ImageHeight);
												LBCirCrackPoint  [LBCirCrackCnt].Rect.bottom =(long)(LBCirCrackPoint  [LBCirCrackCnt].Rect.top + LBCirCrackPoint  [LBCirCrackCnt].Height);
												LBCirCrackPoint  [LBCirCrackCnt].Ver_Dis      =LBCirCrackPoint  [LBCirCrackCnt].Height;
												LBCirCrackPoint  [LBCirCrackCnt].Hor_Dis      =LBCirCrackPoint  [LBCirCrackCnt].Width ;
												LBCirCrackCnt++;
											}
										}
										else                {
											continue; // Score���Ͽ� �ҷ��� ����ó��
										}
									}
								}
								BlobCount =0;
							}
						}

						// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
						// 				if(FrameLeftDefectCnt==0 && Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
						// ���� �κа˻� Dark Masking 
						if(Polygon->Blob[0].InCnt!=0 && LBCirInEdgeCount!=0 && MaskDefectThresHold!=0){

							// MaskImage Create Start 
							//////////////////////////////////////////////////////////////////////////////////////////////////
							BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_DARK_MaskDarkDefectImg_CUP_����.bmp");

							HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
							HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

							TotalInCount =LBCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint = new POINT[TotalInCount];

							// Data Init 
							for(int k=0; k<TotalInCount; k++){
								MaskPoint[k].x = 0;
								MaskPoint[k].y = 0;
							}

							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LBCirInEdgeCount; k++){
								MaskPoint[k+1].x = (int)LBCirInPoint[k].x+MaskDefectOffset;
								MaskPoint[k+1].y = (int)LBCirInPoint[k].y                 ;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LBCirInEdgeCount-1)){
									StaPos.x =(int)LBCirInPoint[k  ].x; 
									StaPos.y =(int)LBCirInPoint[k  ].y; 
									EndPos.x =(int)LBCirInPoint[k-1].x;  
									EndPos.y =(int)LBCirInPoint[k-1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)LBCirInPoint[k  ].x; 
									StaPos.y =(int)LBCirInPoint[k  ].y; 
									EndPos.x =(int)LBCirInPoint[k+1].x;  
									EndPos.y =(int)LBCirInPoint[k+1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,-MaskDefectOffset,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint[k+1].x = TargetPoint.x;
								MaskPoint[k+1].y = TargetPoint.y;

								if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
								if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
							}

							// �а ������,���� 
							MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LBCirInPoint[0                ].y;
							MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LBCirInPoint[LBCirInEdgeCount-1].y; 
							MaskStaPos                  =MinDv-20;

							HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
							::FillRgn(hdc, Rgn1, fillBrh);
							::DeleteObject(Rgn1);

							// �ι�° �̹��� Masking���� �� ��ǥ ��� 
							TotalInCount2 = LBCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint2 = new POINT[TotalInCount2];

							// Data Init 
							for(int k=0; k<TotalInCount2; k++){
								MaskPoint2[k].x = 0;
								MaskPoint2[k].y = 0;
							}

							double deg =0.0f;

							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LBCirInEdgeCount; k++){
								MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
								MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LBCirInEdgeCount-1)){
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k  ].x;  
									EndPos.y =(int)MaskPoint[k  ].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k+2].x;  
									EndPos.y =(int)MaskPoint[k+2].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,-MaskOffsetDistance,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint2[k+1].x = TargetPoint.x;
								MaskPoint2[k+1].y = TargetPoint.y;

								if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
								if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
							}

							// �а ������,���� 
							MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LBCirInPoint[0                 ].y;
							MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LBCirInPoint[LBCirInEdgeCount-1].y; 

							HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
							::FillRgn(hdc, Rgn2, fillBrh);
							::DeleteObject(Rgn2);
							::DeleteObject(fillBrh);

							Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
							if(MaskPoint !=NULL) delete []MaskPoint ;
							if(MaskPoint2!=NULL) delete []MaskPoint2;
							// MaskImage Create End
							//////////////////////////////////////////////////////////////////////////////////////////////////

							if(MaskDarkDefectImg.GetImagePtr()){
								// Image Conversion Binary_Image
								// 							MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_DARK_MaskDarkDefectImg_CUP.bmp");
								// 						EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,EnCapTopThresHold);
								EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
								// 							BinaryImg.Save("D:\\ROTATE_IMG\\����_�κ�_DARK_MaskDarkDefectImg_CUP_Binary.bmp");

								// Image Euresys Algorithm Attach 
								DefectROI.Attach(&BinaryImg);
								CodeImageSelection.SetFeretAngle(0.00f);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
								IAddress = BinaryImg.GetImagePtr();

								RoiStaX      =(int)(MaskStaPos)                                  ;
								RoiStaY      =(int)(LBCirInPoint[0].y+MaskingMargin)             ;
								RoiWidth     =(int)(MaxDv- MinDv)                                ;
								RoiHeight    =(int)(LBCirInPoint[LBCirInEdgeCount-1].y - RoiStaY);

								// �˻� ������ 
								DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
								if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)<=0       ) return false;
								if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
								if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;


								// Blob �˻� ���� 
								CodeImageSelection.Clear();
								CodeImageEncoder.Encode(DefectROI,CodeImage);
								CodeImageSelection.Clear();
								CodeImageSelection.AddObjects(CodeImage);

								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

								BlobCount = CodeImageSelection.GetElementCount();
								CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

								if(BlobCount!=0){
									for(int k=0; k<BlobCount; k++){
										if(LBCirDeftectCnt>=MAX_DEFECT_POINT) break;
										// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
										DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
										DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
										DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
										DefectRT.y  = (float)( DefectLT.y                                                         );
										DefectLB.x  = (float)( DefectLT.x                                                         );
										DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
										DefectRB.x  = (float)( DefectRT.x                                                         );
										DefectRB.y  = (float)( DefectLB.y                                                         );

										for(int i=0; i<LBCirInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											IsDefect = false;
											LTDis = Math.GetLength(LBCirInPoint[i],DefectLT);
											RTDis = Math.GetLength(LBCirInPoint[i],DefectRT);
											LBDis = Math.GetLength(LBCirInPoint[i],DefectLB);
											RBDis = Math.GetLength(LBCirInPoint[i],DefectRB);
											// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
											// 									if(LTDis<=MaskDefectOffset || RTDis<=MaskDefectOffset || LBDis<=MaskDefectOffset || RBDis<=MaskDefectOffset){ 
											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
												IsDefect = true; 
												break;
											}
										}

										if(IsDefect){
											LBCirDefectPoint[LBCirDeftectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LBCirDefectPoint[LBCirDeftectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LBCirDefectPoint[LBCirDeftectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirDefectPoint[LBCirDeftectCnt].Width/2 ) +RoiStaX);
											LBCirDefectPoint[LBCirDeftectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2) +RoiStaY);
											LBCirDefectPoint[LBCirDeftectCnt].Rect.right  = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left+ LBCirDefectPoint[LBCirDeftectCnt].Width );
											LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);

											PixelCount = 0;
											PixelValue = 0;
											// ���� Pixel Data ���� 
											for(int n=LBCirDefectPoint[LBCirDeftectCnt].Rect.top+1 ; n<LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom-1; n++){
												for(int m=LBCirDefectPoint[LBCirDeftectCnt].Rect.left+1; m<LBCirDefectPoint[LBCirDeftectCnt].Rect.right-1 ; m++){
													PixelValue = IElement(m,n);
													if(PixelValue < MaskDefectThresHold) PixelCount++;
												}
											}
											// Display ������ �ʿ��� 
											LBCirDefectPoint[LBCirDeftectCnt].Round = PixelCount;
											LBCirDefectPoint[LBCirDeftectCnt].P.x       = (float)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left);
											LBCirDefectPoint[LBCirDeftectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2)+RoiStaY+ImageHeight );
											LBCirDefectPoint[LBCirDeftectCnt].Width     = LBCirDefectPoint[LBCirDeftectCnt].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LBCirDefectPoint[LBCirDeftectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2)+RoiStaY+ImageHeight);
											LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom =(long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);
											LBCirDeftectCnt++;
										}
									}
								}
								BlobCount =0;
							}
						}

						// Left���� �˻� 
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						LSP = Math.OnIntersect(LeftOutLine [0].FP0, LeftOutLine [0].FP1, LT, RT);

						RoiStaX      =(int)(LSP.x+SideMargin)                  ;
						RoiStaY      =(int)(LSP.y           )                  ;
						RoiWidth     =200                                      ;
						RoiHeight    =(int)(FLBPoint.y - LBSideCircleHeight-10);

						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth,RoiHeight);
						if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)<=0       ) return false;
						if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;

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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
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
				}
			}
			else           { //����˻� 
				if(IsAllGrind){ // ���� New Sequence(��ü����) 
					// Image Conversion Binary_Image, Pixel Value ���� 
					EasyImage::Threshold(GImage,&DefectImg, LeftEdgeThresHold);
					IAddress = DefectImg.GetImagePtr();

					// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					if(LeftInEdgeCount!=0){    
						LBCirDeftectCnt  = GetPolygonSize(LeftInPoint, LBCirDefectPoint, LeftInEdgeCount, 100, 10,0,LeftEdgeThresHold); // Dark Defect 
					}

					// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
					if(LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

						// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_CUP_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						// ù��° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k-1].x;  
								EndPos.y =(int)LeftInPoint[k-1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k+1].x;  
								EndPos.y =(int)LeftInPoint[k+1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,-MaskDefectOffset,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint[k+1].x = TargetPoint.x;
							MaskPoint[k+1].y = TargetPoint.y;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv   ;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);


						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						// Data Convert 
						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k  ].x;  
								EndPos.y =(int)MaskPoint[k  ].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k+2].x;  
								EndPos.y =(int)MaskPoint[k+2].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,-MaskOffsetDistance,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint2[k+1].x = TargetPoint.x;
							MaskPoint2[k+1].y = TargetPoint.y;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
						if(MaskPoint !=NULL)  delete []MaskPoint ;
						if(MaskPoint2!=NULL)  delete []MaskPoint2;
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_CUP.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
							// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_CUP_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
							IAddress = BinaryImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                ;
							RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
							// 						RoiWidth     =(int)(MaxDv- MinDv)                              ;
							RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0       ) return false;
							if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;

							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

							// TEST 
							int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
							int TotalPixelCnt =0, PixelScore  =0;
							float Score     =0.0f;

							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									IsDefect = false;
									Rect.left   = 0;
									Rect.right  = 0;
									Rect.top    = 0;
									Rect.bottom = 0;

									if(LBCirCrackCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
									Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

									PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
									for(int n=Rect.top ; n<Rect.bottom; n++){
										for(int m=Rect.left; m<Rect.right; m++){
											PixelValue = IElement(m,n);
											if(PixelValue > BrokenThresHold) PixelCount++;
											TotalPixelCnt++;
										}
									}
									StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
									if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

									if(Score>PixelScore){
										IsDefect = true;
										for(int i=0; i<LeftInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
											RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
											LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
											RBDis = Math.GetLength(LeftInPoint[i],DefectRB);

											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
												LBCirCrackPoint[LBCirCrackCnt].LineAttach =1;
												break;
											}
										}

										if(IsDefect){
											LBCirCrackPoint[LBCirCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LBCirCrackPoint[LBCirCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LBCirCrackPoint[LBCirCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirCrackPoint[LBCirCrackCnt].Width/2 ) +RoiStaX);
											LBCirCrackPoint[LBCirCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height/2) +RoiStaY);
											LBCirCrackPoint[LBCirCrackCnt].Rect.right  = (long)(LBCirCrackPoint[LBCirCrackCnt].Rect.left+ LBCirCrackPoint[LBCirCrackCnt].Width );
											LBCirCrackPoint[LBCirCrackCnt].Rect.bottom = (long)(LBCirCrackPoint[LBCirCrackCnt].Rect.top + LBCirCrackPoint[LBCirCrackCnt].Height);

											// Display ������ �ʿ��� 
											LBCirCrackPoint[LBCirCrackCnt].Round = PixelCount;
											LBCirCrackPoint[LBCirCrackCnt].P.x       = (float)(LBCirCrackPoint[LBCirCrackCnt].Rect.left);
											LBCirCrackPoint[LBCirCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height/2)+RoiStaY+ImageHeight );
											LBCirCrackPoint[LBCirCrackCnt].Width     = LBCirCrackPoint[LBCirCrackCnt].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LBCirCrackPoint[LBCirCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height/2)+RoiStaY+ImageHeight);
											LBCirCrackPoint[LBCirCrackCnt].Rect.bottom =(long)(LBCirCrackPoint[LBCirCrackCnt].Rect.top + LBCirCrackPoint[LBCirCrackCnt].Height);
											LBCirCrackPoint[LBCirCrackCnt].Ver_Dis      =LBCirCrackPoint[LBCirCrackCnt].Height;
											LBCirCrackPoint[LBCirCrackCnt].Hor_Dis      =LBCirCrackPoint[LBCirCrackCnt].Width ;
											LeftCrackCnt++;
										}
									}
									else                {
										continue; // Score���Ͽ� �ҷ��� ����ó��
									}
								}
							}
							BlobCount =0;
						}
					}

					// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
					// 				if(FrameLeftDefectCnt==0 && Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
					// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
					if(LeftInEdgeCount!=0 && MaskDefectThresHold!=0){

						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);
						// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_CUP_����.bmp");

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
						HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						TotalInCount =LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint = new POINT[TotalInCount];

						// Data Init 
						for(int k=0; k<TotalInCount; k++){
							MaskPoint[k].x = 0;
							MaskPoint[k].y = 0;
						}

						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
							MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k-1].x;  
								EndPos.y =(int)LeftInPoint[k-1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)LeftInPoint[k  ].x; 
								StaPos.y =(int)LeftInPoint[k  ].y; 
								EndPos.x =(int)LeftInPoint[k+1].x;  
								EndPos.y =(int)LeftInPoint[k+1].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,-MaskDefectOffset,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint[k+1].x = TargetPoint.x;
							MaskPoint[k+1].y = TargetPoint.y;

							if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
							if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
						}

						// �а ������,���� 
						MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
						MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
						MaskStaPos                  =MinDv-20;

						HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);

						// �ι�° �̹��� Masking���� �� ��ǥ ��� 
						TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
						MaskPoint2 = new POINT[TotalInCount2];

						// Data Init 
						for(int k=0; k<TotalInCount2; k++){
							MaskPoint2[k].x = 0;
							MaskPoint2[k].y = 0;
						}

						double deg =0.0f;

						MinDv =100000.0f;
						MaxDv = 0.0f;
						for(int k=0; k<LeftInEdgeCount; k++){
							MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
							MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
							deg = 0.0f;

							// Rotate Test 
							if(k==(LeftInEdgeCount-1)){
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k  ].x;  
								EndPos.y =(int)MaskPoint[k  ].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}
							else                      {
								StaPos.x =(int)MaskPoint[k+1].x; 
								StaPos.y =(int)MaskPoint[k+1].y; 
								EndPos.x =(int)MaskPoint[k+2].x;  
								EndPos.y =(int)MaskPoint[k+2].y;  
								deg = Math.GetDegree(StaPos,EndPos);
							}

							TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,-MaskOffsetDistance,deg);
							// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
							MaskPoint2[k+1].x = TargetPoint.x;
							MaskPoint2[k+1].y = TargetPoint.y;

							if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
							if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
						}

						// �а ������,���� 
						MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
						MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

						HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
						if(MaskPoint !=NULL) delete []MaskPoint ;
						if(MaskPoint2!=NULL) delete []MaskPoint2;
						// MaskImage Create End
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if(MaskDarkDefectImg.GetImagePtr()){
							// Image Conversion Binary_Image
							// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_CUP.bmp");
							// 						EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,EnCapTopThresHold);
							EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
							// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_CUP_Binary.bmp");

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
							IAddress = BinaryImg.GetImagePtr();

							RoiStaX      =(int)(MaskStaPos)                                              ;
							RoiStaY      =(int)( LeftInPoint[0].y+MaskingMargin)                         ;
							RoiWidth     =(int)(MaxDv- MinDv)                                            ;
							// 						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);
							RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY-MaskingMargin);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)<=0       ) return false;
							if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
							if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;


							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI,CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
							CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

							if(BlobCount!=0){
								for(int k=0; k<BlobCount; k++){
									if(LBCirDeftectCnt>=MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
									DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
									DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
									DefectRT.y  = (float)( DefectLT.y                                                         );
									DefectLB.x  = (float)( DefectLT.x                                                         );
									DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
									DefectRB.x  = (float)( DefectRT.x                                                         );
									DefectRB.y  = (float)( DefectLB.y                                                         );

									for(int i=0; i<LeftInEdgeCount; i++){
										LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
										IsDefect = false;
										LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
										RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
										LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
										RBDis = Math.GetLength(LeftInPoint[i],DefectRB);
										// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
										// 									if(LTDis<=MaskDefectOffset || RTDis<=MaskDefectOffset || LBDis<=MaskDefectOffset || RBDis<=MaskDefectOffset){ 
										if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
											IsDefect = true; 
											break;
										}
									}

									if(IsDefect){
										LBCirDefectPoint[LBCirDeftectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
										LBCirDefectPoint[LBCirDeftectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

										LBCirDefectPoint[LBCirDeftectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirDefectPoint[LBCirDeftectCnt].Width/2 ) +RoiStaX);
										LBCirDefectPoint[LBCirDeftectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2) +RoiStaY);
										LBCirDefectPoint[LBCirDeftectCnt].Rect.right  = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left+ LBCirDefectPoint[LBCirDeftectCnt].Width );
										LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);

										PixelCount = 0;
										PixelValue = 0;
										// ���� Pixel Data ���� 
										for(int n=LBCirDefectPoint[LBCirDeftectCnt].Rect.top+1 ; n<LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom-1; n++){
											for(int m=LBCirDefectPoint[LBCirDeftectCnt].Rect.left+1; m<LBCirDefectPoint[LBCirDeftectCnt].Rect.right-1 ; m++){
												PixelValue = IElement(m,n);
												if(PixelValue < MaskDefectThresHold) PixelCount++;
											}
										}
										// Display ������ �ʿ��� 
										LBCirDefectPoint[LBCirDeftectCnt].Round = PixelCount;
										LBCirDefectPoint[LBCirDeftectCnt].P.x       = (float)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left);
										LBCirDefectPoint[LBCirDeftectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2)+RoiStaY+ImageHeight );
										LBCirDefectPoint[LBCirDeftectCnt].Width     = LBCirDefectPoint[LBCirDeftectCnt].Width+SideMargin; 

										// Merge ���ؼ� ������ �ʿ��� 
										LBCirDefectPoint[LBCirDeftectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2)+RoiStaY+ImageHeight);
										LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom =(long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);
										LeftDefectCnt++;
									}
								}
							}
							BlobCount =0;
						}
					}
				}
				else          { // ���� �κп��� Sequence 
					if(IsRectBtm)       {
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						LSP = Math.OnIntersect(LeftOutLine [0].FP0, LeftOutLine [0].FP1, LT, RT);

						RoiStaX      =(int)(LSP.x+SideMargin);
						RoiStaY      =(int)(LSP.y           );
						RoiWidth     =200                    ;
						RoiHeight    =(int)(FLBPoint.y)      ;

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)<=0       ) return false;
						if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						// Blob ���͸� 
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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight );
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
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
					else if(IsCornerBtm) {
						// Image Conversion Binary_Image, Pixel Value ���� 
						EasyImage::Threshold(GImage,&DefectImg, CorGrindInThresHold_End);
						IAddress = DefectImg.GetImagePtr();

						if(LBCorInEdgeCount!=0){
							CornerCount=0;
							for(int k=0; k<LBCorInEdgeCount; k++){
								if(LBCorInPoint[k].y>FLBLPoint.y-1) continue;
								Dis = Math.GetLengthFrLineToPoint(FLBLInPoint, FLBBInPoint, LBCorInPoint[k]);
								LBCorner[CornerCount].Dv   = Dis         ;
								LBCorner[CornerCount].P.x  = LBCorInPoint[k].x;
								LBCorner[CornerCount].P.y  = LBCorInPoint[k].y;
								CornerCount++;
								if(Dis>MaxDv) MaxDv = Dis;
								if(Dis<MinDv) MinDv = Dis;
							}
							if(LBCornerWidth>(LBCornerHeight*2)) LBCorCount = GetSize(LBCorner, LBCorDefectPoint, CornerCount, 10);
							else                                 LBCorCount = GetSize(LBCorner, LBCorDefectPoint, CornerCount, 5 );
						}

						// SIDE_D�˻� 
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						LSP = Math.OnIntersect(LeftOutLine [0].FP0, LeftOutLine [0].FP1, LT, RT);

						RoiStaX      =(int)(LSP.x+SideMargin);
						RoiStaY      =(int)(LSP.y           );
						RoiWidth     =200                    ;
						RoiHeight    =(int)(FLBLInPoint.y)   ;

						//�˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth,RoiHeight);
						if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)<=0       ) return false;
						if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						// Blob ���͸� 
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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
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
					else if(IsCircleBtm){ //���� �Ű˻� 
						// Image Conversion Binary_Image, Pixel Value ���� 
						EasyImage::Threshold(GImage,&DefectImg, ECThresHold_End);
						// 				EasyImage::Threshold(GImage,&DefectImg, BtmEdgeThresHold);
						IAddress = DefectImg.GetImagePtr();

						// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
						if(LBCirInEdgeCount!=0){    
							// 					LBCirDeftectCnt  = GetPolygonSize(LBCirInPoint, LBCirDefectPoint, LBCirInEdgeCount, 50, 5,0,ECThresHold); // Dark Defect 
							LBCirDeftectCnt  = GetPolygonSize(LBCirInPoint, LBCirDefectPoint, LBCirInEdgeCount, 100, 10,0,ECThresHold_End); // Dark Defect 
						}


						// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
						// 				if(FrameLeftDefectCnt==0 && Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
						// ���� �κа˻� Dark Masking 
						if(Polygon->Blob[0].InCnt!=0 && LBCirInEdgeCount!=0 && MaskDefectThresHold!=0){

							// MaskImage Create Start 
							//////////////////////////////////////////////////////////////////////////////////////////////////
							BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
							EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);
							// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_DARK_MaskDarkDefectImg_CUP_����.bmp");

							HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
							HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

							TotalInCount =LBCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint = new POINT[TotalInCount];

							// Data Init 
							for(int k=0; k<TotalInCount; k++){
								MaskPoint[k].x = 0;
								MaskPoint[k].y = 0;
							}

							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LBCirInEdgeCount; k++){
								MaskPoint[k+1].x = (int)LBCirInPoint[k].x+MaskDefectOffset;
								MaskPoint[k+1].y = (int)LBCirInPoint[k].y                 ;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LBCirInEdgeCount-1)){
									StaPos.x =(int)LBCirInPoint[k  ].x; 
									StaPos.y =(int)LBCirInPoint[k  ].y; 
									EndPos.x =(int)LBCirInPoint[k-1].x;  
									EndPos.y =(int)LBCirInPoint[k-1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)LBCirInPoint[k  ].x; 
									StaPos.y =(int)LBCirInPoint[k  ].y; 
									EndPos.x =(int)LBCirInPoint[k+1].x;  
									EndPos.y =(int)LBCirInPoint[k+1].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,-MaskDefectOffset,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint[k+1].x = TargetPoint.x;
								MaskPoint[k+1].y = TargetPoint.y;

								if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
								if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
							}

							// �а ������,���� 
							MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LBCirInPoint[0                ].y;
							MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LBCirInPoint[LBCirInEdgeCount-1].y; 
							MaskStaPos                  =MinDv-20;

							HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
							::FillRgn(hdc, Rgn1, fillBrh);
							::DeleteObject(Rgn1);

							// �ι�° �̹��� Masking���� �� ��ǥ ��� 
							TotalInCount2 = LBCirInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
							MaskPoint2 = new POINT[TotalInCount2];

							// Data Init 
							for(int k=0; k<TotalInCount2; k++){
								MaskPoint2[k].x = 0;
								MaskPoint2[k].y = 0;
							}

							double deg =0.0f;

							MinDv =100000.0f;
							MaxDv = 0.0f;
							for(int k=0; k<LBCirInEdgeCount; k++){
								MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
								MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
								deg = 0.0f;

								// Rotate Test 
								if(k==(LBCirInEdgeCount-1)){
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k  ].x;  
									EndPos.y =(int)MaskPoint[k  ].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}
								else                      {
									StaPos.x =(int)MaskPoint[k+1].x; 
									StaPos.y =(int)MaskPoint[k+1].y; 
									EndPos.x =(int)MaskPoint[k+2].x;  
									EndPos.y =(int)MaskPoint[k+2].y;  
									deg = Math.GetDegree(StaPos,EndPos);
								}

								TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,-MaskOffsetDistance,deg);
								// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
								MaskPoint2[k+1].x = TargetPoint.x;
								MaskPoint2[k+1].y = TargetPoint.y;

								if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
								if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
							}

							// �а ������,���� 
							MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LBCirInPoint[0                 ].y;
							MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LBCirInPoint[LBCirInEdgeCount-1].y; 

							HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
							::FillRgn(hdc, Rgn2, fillBrh);
							::DeleteObject(Rgn2);
							::DeleteObject(fillBrh);

							Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
							if(MaskPoint !=NULL) delete []MaskPoint ;
							if(MaskPoint2!=NULL) delete []MaskPoint2;
							// MaskImage Create End
							//////////////////////////////////////////////////////////////////////////////////////////////////

							if(MaskDarkDefectImg.GetImagePtr()){
								// Image Conversion Binary_Image
//		 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_�κ�_DARK_MaskDarkDefectImg_CUP.bmp");
								// 						EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,EnCapTopThresHold);
								EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
//								BinaryImg.Save("D:\\ROTATE_IMG\\����_�κ�_DARK_MaskDarkDefectImg_CUP_Binary.bmp");

								// Image Euresys Algorithm Attach 
								DefectROI.Attach(&BinaryImg);
								CodeImageSelection.SetFeretAngle(0.00f);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
								CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
								IAddress = BinaryImg.GetImagePtr();

								RoiStaX      =(int)(MaskStaPos)                                  ;
								RoiStaY      =(int)( LBCirInPoint[0].y+MaskingMargin)            ;
								RoiWidth     =(int)(MaxDv- MinDv)                                ;
								RoiHeight    =(int)(LBCirInPoint[LBCirInEdgeCount-1].y - RoiStaY);

								// �˻� ������ 
								DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
								if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)<=0       ) return false;
								if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
								if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;


								// Blob �˻� ���� 
								CodeImageSelection.Clear();
								CodeImageEncoder.Encode(DefectROI,CodeImage);
								CodeImageSelection.Clear();
								CodeImageSelection.AddObjects(CodeImage);

								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
								CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

								BlobCount = CodeImageSelection.GetElementCount();
								CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

								if(BlobCount!=0){
									for(int k=0; k<BlobCount; k++){
										if(LBCirDeftectCnt>=MAX_DEFECT_POINT) break;
										// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
										DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
										DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
										DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
										DefectRT.y  = (float)( DefectLT.y                                                         );
										DefectLB.x  = (float)( DefectLT.x                                                         );
										DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
										DefectRB.x  = (float)( DefectRT.x                                                         );
										DefectRB.y  = (float)( DefectLB.y                                                         );

										for(int i=0; i<LBCirInEdgeCount; i++){
											LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
											IsDefect = false;
											LTDis = Math.GetLength(LBCirInPoint[i],DefectLT);
											RTDis = Math.GetLength(LBCirInPoint[i],DefectRT);
											LBDis = Math.GetLength(LBCirInPoint[i],DefectLB);
											RBDis = Math.GetLength(LBCirInPoint[i],DefectRB);
											// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
											// 									if(LTDis<=MaskDefectOffset || RTDis<=MaskDefectOffset || LBDis<=MaskDefectOffset || RBDis<=MaskDefectOffset){ 
											if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
												IsDefect = true; 
												break;
											}
										}

										if(IsDefect){
											LBCirDefectPoint[LBCirDeftectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
											LBCirDefectPoint[LBCirDeftectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

											LBCirDefectPoint[LBCirDeftectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirDefectPoint[LBCirDeftectCnt].Width/2 ) +RoiStaX);
											LBCirDefectPoint[LBCirDeftectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2) +RoiStaY);
											LBCirDefectPoint[LBCirDeftectCnt].Rect.right  = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left+ LBCirDefectPoint[LBCirDeftectCnt].Width );
											LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);

											PixelCount = 0;
											PixelValue = 0;
											// ���� Pixel Data ���� 
											for(int n=LBCirDefectPoint[LBCirDeftectCnt].Rect.top+1 ; n<LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom-1; n++){
												for(int m=LBCirDefectPoint[LBCirDeftectCnt].Rect.left+1; m<LBCirDefectPoint[LBCirDeftectCnt].Rect.right-1 ; m++){
													PixelValue = IElement(m,n);
													if(PixelValue < MaskDefectThresHold) PixelCount++;
												}
											}
											// Display ������ �ʿ��� 
											LBCirDefectPoint[LBCirDeftectCnt].Round = PixelCount;
											LBCirDefectPoint[LBCirDeftectCnt].P.x       = (float)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left);
											LBCirDefectPoint[LBCirDeftectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2)+RoiStaY+ImageHeight );
											LBCirDefectPoint[LBCirDeftectCnt].Width     = LBCirDefectPoint[LBCirDeftectCnt].Width+SideMargin; 

											// Merge ���ؼ� ������ �ʿ��� 
											LBCirDefectPoint[LBCirDeftectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2)+RoiStaY+ImageHeight);
											LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom =(long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);
											LBCirDeftectCnt++;
										}
									}
								}
								BlobCount =0;
							}
						}
						// Left���� �˻� 
						// Image Conversion Binary_Image
						EasyImage::Threshold(GImage,&DefectImg, DarkThresHold);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&DefectImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = DefectImg.GetImagePtr();

						// �˻� ROI���� 
						LSP = Math.OnIntersect(LeftOutLine [0].FP0, LeftOutLine [0].FP1, LT, RT);

						RoiStaX      =(int)(LSP.x+SideMargin)                  ;
						RoiStaY      =(int)(LSP.y           )                  ;
						RoiWidth     =200                                      ;
						RoiHeight    =(int)(FLBPoint.y - LBSideCircleHeight-10);

						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth,RoiHeight);
						if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)<=0       ) return false;
						if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>BtmLimitLine)                           return false;

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

								DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
								DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
								DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
								DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

								PixelCount = 0;
								PixelValue = 0;
								// ���� Pixel Data ���� 
								for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
									for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
										PixelValue = IElement(m,n);
										if(PixelValue < DarkThresHold) PixelCount++;
									}
								}
								// Display ������ �ʿ��� 
								DDefectPoint[DCount].Round = PixelCount;
								DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
								DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
								DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

								// Merge ���ؼ� ������ �ʿ��� 
								DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
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
				}
			}
		}
		else                        { //��ü���� or �κп��� 
			if(IsAllGrind){
				IAddress = DefectImg.GetImagePtr();

				// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
				if(LeftInEdgeCount!=0){    
					// FOV�������� �������� ������� 
					LBCirDeftectCnt  = GetPolygonSize(LeftInPoint, LBCirDefectPoint, LeftInEdgeCount, 100, 10,0, DarkThresHold); // Dark Defect 
				}

				// EnCap���� �ڸ��� ��ũ��ġ �˻� ���� �̹��� ����ŷ ���� �� �˻� ������ 
				if(LeftInEdgeCount!=0 && BrokenThresHold!=0 && !IsMarkBtm){
					// MaskImage Create Start 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					BinaryImg.SetSize        (GImage->GetWidth(), GImage->GetHeight()); // Image Masking Conversion Binary
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());	// Image Masking Org
					EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);
					//////////////////////////////////////////////////////////////////////////////////////////////////
					//MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP_����.bmp");

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(0, 0, 0)); //Black 
					HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//ClipperOffset USE 
					/*
					// Masking Data Init 
					if(MaskPoint !=NULL)  {
						delete []MaskPoint ;
					}
					if(MaskPoint2!=NULL)  {
						delete []MaskPoint2;
					}


					// ROI���� ��ǥ ������ �Ѱ��� 
					RoiLt.x = IRoi->GetOrgX()         ; RoiLt.y = IRoi->GetOrgY()          ;
					RoiRt.x = RoiLt.x+IRoi->GetWidth(); RoiRt.y = IRoi->GetOrgY()          ;
					RoiLb.x = RoiLt.x                 ; RoiLb.y = RoiLt.y+IRoi->GetHeight();
					RoiRb.x = RoiRt.x                 ; RoiRb.y = RoiRt.y+IRoi->GetHeight();

					// InEdgeData���� Out���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					The::Path  Poly_In ;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

					// Polygon InEdgePoint Data Send ClipperOffset 
					Poly_In.push_back(The::IntPoint(RoiLt.x,RoiLt.y));
					for(int k=0; k<LeftInEdgeCount; k++){
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
// 					Poly_In.push_back(The::IntPoint(RoiLb.x,RoiLb.y));
					Poly_In.push_back(The::IntPoint(RoiLb.x,LeftInPoint[LeftInEdgeCount-1].y));

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);	

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;

					// InEdgeData���� In���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					Poly_In.clear();
					Poly_Out.clear();
					ClipOffset.Clear();
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

					// 					Poly_In.push_back(The::IntPoint(FrameImgW,0)); //Image���� ������ ��� �� ��ġ  
					Poly_In.push_back(The::IntPoint(RoiRt.x,RoiRt.y)); //Image���� ������ ��� �� ��ġ  
					for(int k=0; k<LeftInEdgeCount; k++){
						deg = 0.0f;
						//////////////////////////////////////////////////////////
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
					// 					Poly_In.push_back(The::IntPoint(FrameImgW,FrameImgH)); //Image���� ������ �ϴ� �� ��ġ  
// 					Poly_In.push_back(The::IntPoint(RoiRb.x,RoiRb.y));
					Poly_In.push_back(The::IntPoint(RoiRb.x,LeftInPoint[LeftInEdgeCount-1].y));

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,-MaskOffsetDistance);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint2 = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint2[k].x = Poly_Out[0][k].x;
						MaskPoint2[k].y = Poly_Out[0][k].y;
					}


					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);
					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////
					*/

                    //OnFindPerpendicularPointXY Coordinage USE
					// ù��° �̹��� Masking���� �� ��ǥ ��� 
					TotalInCount = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint = new POINT[TotalInCount];

					// Data Init 
					for(int k=0; k<TotalInCount; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
						MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
						deg = 0.0f;

						// Rotate Test 
						if(k==(LeftInEdgeCount-1)){
							StaPos.x =(int)LeftInPoint[k  ].x; 
							StaPos.y =(int)LeftInPoint[k  ].y; 
							EndPos.x =(int)LeftInPoint[k-1].x;  
							EndPos.y =(int)LeftInPoint[k-1].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}
						else                      {
							StaPos.x =(int)LeftInPoint[k  ].x; 
							StaPos.y =(int)LeftInPoint[k  ].y; 
							EndPos.x =(int)LeftInPoint[k+1].x;  
							EndPos.y =(int)LeftInPoint[k+1].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}

						TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,-MaskDefectOffset,deg);
						// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
						MaskPoint[k+1].x = TargetPoint.x;
						MaskPoint[k+1].y = TargetPoint.y;

						if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
						if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
					}

					// �а ������,���� 
					MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
					MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
					MaskStaPos                  =MinDv   ;

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);


					// �ι�° �̹��� Masking���� �� ��ǥ ��� 
					TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint2 = new POINT[TotalInCount2];

					// Data Init 
					for(int k=0; k<TotalInCount2; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
						MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
						deg = 0.0f;

						// Rotate Test 
						if(k==(LeftInEdgeCount-1)){
							StaPos.x =(int)MaskPoint[k+1].x; 
							StaPos.y =(int)MaskPoint[k+1].y; 
							EndPos.x =(int)MaskPoint[k  ].x;  
							EndPos.y =(int)MaskPoint[k  ].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}
						else                      {
							StaPos.x =(int)MaskPoint[k+1].x; 
							StaPos.y =(int)MaskPoint[k+1].y; 
							EndPos.x =(int)MaskPoint[k+2].x;  
							EndPos.y =(int)MaskPoint[k+2].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}

						TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,-MaskOffsetDistance,deg);
						// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
						MaskPoint2[k+1].x = TargetPoint.x;
						MaskPoint2[k+1].y = TargetPoint.y;

						if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
						if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
					}

					// �а ������,���� 
					MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
					MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					if(MaskPoint !=NULL)  delete []MaskPoint ;
					if(MaskPoint2!=NULL)  delete []MaskPoint2;
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////

					if(MaskDarkDefectImg.GetImagePtr()){
						// Image Conversion Binary_Image
// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP.bmp");
						EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,BrokenThresHold);
						// 						BinaryImg.Save("D:\\ROTATE_IMG\\����_��ü_WHITE_MaskDarkDefectImg_HAP_Binary.bmp");

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
						IAddress = BinaryImg.GetImagePtr();

// 						RoiStaX      =(int)(RoiLt.x)          ;
// 						RoiStaY      =(int)(RoiLt.y)          ;
// 						RoiWidth     =(int)(RoiRt.x - RoiLt.x);
// // 						RoiHeight    =(int)(RoiLb.y - RoiLt.y);
// 						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

						RoiStaX      =(int)(MaskStaPos)                                ;
						RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)            ;
						RoiWidth     =(int)(MaxDv- RoiStaX)                            ;
						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

						// TEST 
						int StaX =0, StaY=0, StaWidth=0, StaHeight=0; 
						int TotalPixelCnt =0, PixelScore  =0;
						float Score     =0.0f;

						if(BlobCount!=0){
							for(int k=0; k<BlobCount; k++){
								IsDefect = false;
								Rect.left   = 0;
								Rect.right  = 0;
								Rect.top    = 0;
								Rect.bottom = 0;

								if(LTCirCrackCnt>=MAX_DEFECT_POINT) break;
								// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
								DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
								DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
								DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
								DefectRT.y  = (float)( DefectLT.y                                                         );
								DefectLB.x  = (float)( DefectLT.x                                                         );
								DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
								DefectRB.x  = (float)( DefectRT.x                                                         );
								DefectRB.y  = (float)( DefectLB.y                                                         );

								Rect.left  = DefectLT.x; Rect.top    = DefectLT.y;
								Rect.right = DefectRT.x; Rect.bottom = DefectLB.y;

								PixelCount  =0, TotalPixelCnt =0, PixelScore  =20, Score =0.0f;
								for(int n=Rect.top ; n<Rect.bottom; n++){
									for(int m=Rect.left; m<Rect.right; m++){
										PixelValue = IElement(m,n);
										if(PixelValue > BrokenThresHold) PixelCount++;
										TotalPixelCnt++;
									}
								}
								StaX = Rect.left; StaY = Rect.top; StaWidth = (Rect.right - Rect.left); StaHeight = (Rect.bottom - Rect.top);
								if(PixelCount!=0 && TotalPixelCnt!=0) Score  = (((float)PixelCount/(float)TotalPixelCnt)*100);

								if(Score>PixelScore){
									IsDefect = true;
									for(int i=0; i<LeftInEdgeCount; i++){
										LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
										LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
										RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
										LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
										RBDis = Math.GetLength(LeftInPoint[i],DefectRB);

										if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
											LBCirCrackPoint[LBCirCrackCnt].LineAttach =1;
											break;
										}
									}

									if(IsDefect){
										LBCirCrackPoint[LBCirCrackCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
										LBCirCrackPoint[LBCirCrackCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

										LBCirCrackPoint[LBCirCrackCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirCrackPoint[LBCirCrackCnt].Width/2 ) +RoiStaX);
										LBCirCrackPoint[LBCirCrackCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height/2) +RoiStaY);
										LBCirCrackPoint[LBCirCrackCnt].Rect.right  = (long)(LBCirCrackPoint[LBCirCrackCnt].Rect.left+ LBCirCrackPoint[LBCirCrackCnt].Width );
										LBCirCrackPoint[LBCirCrackCnt].Rect.bottom = (long)(LBCirCrackPoint[LBCirCrackCnt].Rect.top + LBCirCrackPoint[LBCirCrackCnt].Height);

										// Display ������ �ʿ��� 
										LBCirCrackPoint[LBCirCrackCnt].Round = PixelCount;
										LBCirCrackPoint[LBCirCrackCnt].P.x       = (float)(LTCirCrackPoint[LTCirCrackCnt].Rect.left);
										LBCirCrackPoint[LBCirCrackCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height/2)+RoiStaY+ImageHeight );
										LBCirCrackPoint[LBCirCrackCnt].Width     = LBCirCrackPoint[LBCirCrackCnt].Width+SideMargin; 

										// Merge ���ؼ� ������ �ʿ��� 
										LBCirCrackPoint[LBCirCrackCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirCrackPoint[LBCirCrackCnt].Height/2)+RoiStaY+ImageHeight);
										LBCirCrackPoint[LBCirCrackCnt].Rect.bottom =(long)(LBCirCrackPoint[LBCirCrackCnt].Rect.top + LBCirCrackPoint[LBCirCrackCnt].Height);
										LBCirCrackPoint[LBCirCrackCnt].Ver_Dis      =LBCirCrackPoint[LBCirCrackCnt].Height;
										LBCirCrackPoint[LBCirCrackCnt].Hor_Dis      =LBCirCrackPoint[LBCirCrackCnt].Width ;
										LBCirCrackCnt++;
									}
								}
								else                {
									continue; // Score���Ͽ� �ҷ��� ����ó��
								}
							}
						}
						BlobCount =0;
					}
				}

				// Polygon Alogorithm���� �ҷ� �̰����....MaskImage���� �ٽ� �ѹ� �˻� 
				// ���� ��ü���� Dark Masking
				// 				if(Polygon->Blob[0].InCnt!=0 && LeftInEdgeCount!=0 && LTCirDeftectCnt==0 && MaskDefectThresHold!=0){
				if(LeftInEdgeCount!=0 && MaskDefectThresHold!=0){
					// MaskImage Create Start 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
					MaskDarkDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
					EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskDarkDefectImg);

					// 					MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\����_��ü_DARK_MaskDarkDefectImg_HAP_����.bmp");
					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
					HDC hdc         = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					//ClipperOffset USE 
					/*
					// Masking Data Init 
					if(MaskPoint !=NULL)  {
						delete []MaskPoint ;
					}
					if(MaskPoint2!=NULL)  {
						delete []MaskPoint2;
					}

					// ROI���� ��ǥ ������ �Ѱ��� 
					RoiLt.x = IRoi->GetOrgX()         ; RoiLt.y = IRoi->GetOrgY()          ;
					RoiRt.x = RoiLt.x+IRoi->GetWidth(); RoiRt.y = IRoi->GetOrgY()          ;
					RoiLb.x = RoiLt.x                 ; RoiLb.y = RoiLt.y+IRoi->GetHeight();
					RoiRb.x = RoiRt.x                 ; RoiRb.y = RoiRt.y+IRoi->GetHeight();

					// InEdgeData���� Out���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					The::Path  Poly_In ;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

					// Polygon InEdgePoint Data Send ClipperOffset 
					Poly_In.push_back(The::IntPoint(RoiLt.x,RoiLt.y));
					for(int k=0; k<LeftInEdgeCount; k++){
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
// 					Poly_In.push_back(The::IntPoint(RoiLb.x,RoiLb.y));
					Poly_In.push_back(The::IntPoint(RoiLb.x,LeftInPoint[LeftInEdgeCount-1].y));

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,MaskDefectOffset);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);	
					//////////////////////////////////////////////////////////////////////////////////////////////////


					// Data Convert 
					MinDv =100000.0f;
					MaxDv = 0.0f;

					// InEdgeData���� In���� Masking ������ 
					// Clipper Use Parameter 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					Poly_In.clear();
					Poly_Out.clear();
					ClipOffset.Clear();
					Poly_In.reserve (LeftInEdgeCount+2);
					Poly_Out.reserve(2000);

					// 					Poly_In.push_back(The::IntPoint(FrameImgW,0)); //Image���� ������ ��� �� ��ġ  
					Poly_In.push_back(The::IntPoint(RoiRt.x,RoiRt.y)); //Image���� ������ ��� �� ��ġ  
					for(int k=0; k<LeftInEdgeCount; k++){
						deg = 0.0f;
						//////////////////////////////////////////////////////////
						// Clipper use 
						Poly_In.push_back(The::IntPoint(LeftInPoint[k].x,LeftInPoint[k].y));
					}
// 					Poly_In.push_back(The::IntPoint(RoiRb.x,RoiRb.y));
					Poly_In.push_back(The::IntPoint(RoiRb.x,LeftInPoint[LeftInEdgeCount-1].y));

					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out,-MaskOffsetDistance);
					PolyOutCnt = Poly_Out[0].size(); //�а ���� ClipperOut ���ؼ� ����� ���� ���� 

					MaskPoint2 = new POINT[PolyOutCnt];

					// �������� ������ �ʱ�ȭ 
					for(int k=0; k<PolyOutCnt; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					for(int k=0; k<PolyOutCnt; k++){
						if(k>=Poly_Out[0].size()) break;
						MaskPoint2[k].x = Poly_Out[0][k].x;
						MaskPoint2[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, PolyOutCnt, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);
					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////
					*/

                    //OnFindPerpendicularPointXY Coordinage USE 
					TotalInCount =LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint = new POINT[TotalInCount];

					// Data Init 
					for(int k=0; k<TotalInCount; k++){
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint[k+1].x = (int)LeftInPoint[k].x+MaskDefectOffset;
						MaskPoint[k+1].y = (int)LeftInPoint[k].y                 ;
						deg = 0.0f;

						// Rotate Test 
						if(k==(LeftInEdgeCount-1)){
							StaPos.x =(int)LeftInPoint[k  ].x; 
							StaPos.y =(int)LeftInPoint[k  ].y; 
							EndPos.x =(int)LeftInPoint[k-1].x;  
							EndPos.y =(int)LeftInPoint[k-1].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}
						else                      {
							StaPos.x =(int)LeftInPoint[k  ].x; 
							StaPos.y =(int)LeftInPoint[k  ].y; 
							EndPos.x =(int)LeftInPoint[k+1].x;  
							EndPos.y =(int)LeftInPoint[k+1].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}

						TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskDefectOffset,-MaskDefectOffset,deg);
						// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
						MaskPoint[k+1].x = TargetPoint.x;
						MaskPoint[k+1].y = TargetPoint.y;

						if(MaskPoint[k+1].x<MinDv) MinDv   = MaskPoint[k+1].x;
						if(MaskPoint[k+1].x>MaxDv) MaxDv   = MaskPoint[k+1].x;
					}

					// �а ������,���� 
					MaskPoint[0].x              =MinDv-20; MaskPoint[0].y              = LeftInPoint[0                ].y;
					MaskPoint[TotalInCount-1].x =MinDv-20; MaskPoint[TotalInCount-1].y = LeftInPoint[LeftInEdgeCount-1].y; 
					MaskStaPos                  =MinDv-20;

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, TotalInCount, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);

					// �ι�° �̹��� Masking���� �� ��ǥ ��� 
					TotalInCount2 = LeftInEdgeCount+2; //�а ���� Masking�ϱ� ������ Start,End����Ʈ �߰� 
					MaskPoint2 = new POINT[TotalInCount2];

					// Data Init 
					for(int k=0; k<TotalInCount2; k++){
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					double deg =0.0f;

					MinDv =100000.0f;
					MaxDv = 0.0f;
					for(int k=0; k<LeftInEdgeCount; k++){
						MaskPoint2[k+1].x = (int)MaskPoint[k+1].x;
						MaskPoint2[k+1].y = (int)MaskPoint[k+1].y;
						deg = 0.0f;

						// Rotate Test 
						if(k==(LeftInEdgeCount-1)){
							StaPos.x =(int)MaskPoint[k+1].x; 
							StaPos.y =(int)MaskPoint[k+1].y; 
							EndPos.x =(int)MaskPoint[k  ].x;  
							EndPos.y =(int)MaskPoint[k  ].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}
						else                      {
							StaPos.x =(int)MaskPoint[k+1].x; 
							StaPos.y =(int)MaskPoint[k+1].y; 
							EndPos.x =(int)MaskPoint[k+2].x;  
							EndPos.y =(int)MaskPoint[k+2].y;  
							deg = Math.GetDegree(StaPos,EndPos);
						}

						TargetPoint = Math.OnFindPerpendicularPointXY(StaPos,MaskOffsetDistance,-MaskOffsetDistance,deg);
						// 						if(TargetPoint.x<0 || TargetPoint.x>FrameImgW || TargetPoint.y<0 || TargetPoint.y>FrameImgH) continue;
						MaskPoint2[k+1].x = TargetPoint.x;
						MaskPoint2[k+1].y = TargetPoint.y;

						if(MaskPoint2[k+1].x<MinDv) MinDv   = MaskPoint2[k+1].x;
						if(MaskPoint2[k+1].x>MaxDv) MaxDv   = MaskPoint2[k+1].x;
					}

					// �а ������,���� 
					MaskPoint2[0].x               =MaxDv+20; MaskPoint2[0].y               = LeftInPoint[0                ].y;
					MaskPoint2[TotalInCount2-1].x =MaxDv+20; MaskPoint2[TotalInCount2-1].y = LeftInPoint[LeftInEdgeCount-1].y; 

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, TotalInCount2, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg   , hdc);
					if(MaskPoint !=NULL) delete []MaskPoint ;
					if(MaskPoint2!=NULL) delete []MaskPoint2;
					// MaskImage Create End
					//////////////////////////////////////////////////////////////////////////////////////////////////

					if(MaskDarkDefectImg.GetImagePtr()){
						// Image Conversion Binary_Image
// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\WATCH_����_CUP.bmp");
						EasyImage::Threshold(&MaskDarkDefectImg,&BinaryImg,MaskDefectThresHold);
// 						BinaryImg.Save("D:\\ROTATE_IMG\\WATCH_����_SUP_Binary.bmp");

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
						IAddress = BinaryImg.GetImagePtr();
						// 						IAddress = MaskDarkDefectImg.GetImagePtr();

// 						RoiStaX      =(int)(RoiLt.x)          ;
// 						RoiStaY      =(int)(RoiLt.y)          ;
// 						RoiWidth     =(int)(RoiRt.x - RoiLt.x);
// 						// 						RoiHeight    =(int)(RoiLb.y - RoiLt.y);
// 						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY);

						RoiStaX      =(int)(MaskStaPos)                                  ;
						RoiStaY      =(int)(LeftInPoint[0].y+MaskingMargin)              ;
						RoiWidth     =(int)(MaxDv- RoiStaX)                              ;
						RoiHeight    =(int)(LeftInPoint[LeftInEdgeCount-1].y - RoiStaY)-5;

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if((RoiStaX) <=0          ||(RoiStaY)<=0           || RoiWidth<=0         || (RoiHeight)<=0          ) return false;
						if((RoiStaX) >=FrameImgW  ||(RoiStaY)>TopLimitLine || RoiWidth>=FrameImgW || (RoiHeight)>TopLimitLine) return false;
						if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>TopLimitLine)                                  return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI,CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
						CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);


						if(BlobCount!=0){
							for(int k=0; k<BlobCount; k++){
								if(LTCirDeftectCnt>=MAX_DEFECT_POINT) break;
								// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
								DefectLT.x  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ())/2 ) +RoiStaX);
								DefectLT.y  = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ( CodeImageSelection.GetElement(k).GetBoundingBoxHeight())/2 ) +RoiStaY);
								DefectRT.x  = (float)( DefectLT.x+CodeImageSelection.GetElement(k).GetBoundingBoxWidth  ());
								DefectRT.y  = (float)( DefectLT.y                                                         );
								DefectLB.x  = (float)( DefectLT.x                                                         );
								DefectLB.y  = (float)( DefectLT.y+CodeImageSelection.GetElement(k).GetBoundingBoxHeight ());
								DefectRB.x  = (float)( DefectRT.x                                                         );
								DefectRB.y  = (float)( DefectLB.y                                                         );

								for(int i=0; i<LeftInEdgeCount; i++){
									LTDis =0.0f, RTDis=0.0f, LBDis=0.0f, RBDis=0.0f;
									IsDefect = false;
									LTDis = Math.GetLength(LeftInPoint[i],DefectLT);
									RTDis = Math.GetLength(LeftInPoint[i],DefectRT);
									LBDis = Math.GetLength(LeftInPoint[i],DefectLB);
									RBDis = Math.GetLength(LeftInPoint[i],DefectRB);
									if(LTDis<=MaskDefectOffset+MaskDefectMagin || RTDis<=MaskDefectOffset+MaskDefectMagin || LBDis<=MaskDefectOffset+MaskDefectMagin || RBDis<=MaskDefectOffset+MaskDefectMagin){ 
										IsDefect = true; 
										break;
									}
								}

								if(IsDefect){
									LBCirDefectPoint[LBCirDeftectCnt].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() +1;
									LBCirDefectPoint[LBCirDeftectCnt].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight()+1;

									LBCirDefectPoint[LBCirDeftectCnt].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - LBCirDefectPoint[LBCirDeftectCnt].Width/2 ) +RoiStaX);
									LBCirDefectPoint[LBCirDeftectCnt].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2) +RoiStaY);
									LBCirDefectPoint[LBCirDeftectCnt].Rect.right  = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left+ LBCirDefectPoint[LBCirDeftectCnt].Width );
									LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom = (long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);

									PixelCount = 0;
									PixelValue = 0;
									// ���� Pixel Data ���� 
									for(int n=LBCirDefectPoint[LBCirDeftectCnt].Rect.top+1 ; n<LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom-1; n++){
										for(int m=LBCirDefectPoint[LBCirDeftectCnt].Rect.left+1; m<LBCirDefectPoint[LBCirDeftectCnt].Rect.right-1 ; m++){
											PixelValue = IElement(m,n);
											if(PixelValue < MaskDefectThresHold) PixelCount++;
										}
									}
									// Display ������ �ʿ��� 
									LBCirDefectPoint[LBCirDeftectCnt].Round = PixelCount;
									LBCirDefectPoint[LBCirDeftectCnt].P.x       = (float)(LBCirDefectPoint[LBCirDeftectCnt].Rect.left);
									LBCirDefectPoint[LBCirDeftectCnt].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2)+RoiStaY+ImageHeight );
									LBCirDefectPoint[LBCirDeftectCnt].Width     = LBCirDefectPoint[LBCirDeftectCnt].Width+SideMargin; 

									// Merge ���ؼ� ������ �ʿ��� 
									LBCirDefectPoint[LBCirDeftectCnt].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - LBCirDefectPoint[LBCirDeftectCnt].Height/2)+RoiStaY+ImageHeight);
									LBCirDefectPoint[LBCirDeftectCnt].Rect.bottom =(long)(LBCirDefectPoint[LBCirDeftectCnt].Rect.top + LBCirDefectPoint[LBCirDeftectCnt].Height);
									LBCirDeftectCnt++;
								}
							}
						}
						BlobCount =0;
					}
				}
			}
			else          {
				// �˻� ROI���� 
				LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
				LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);

				//Align Ʋ���� �����Ͽ� �˻�ROI���� ������ 
				if(fabs(LSP.x - LEP.x)>SideMargin){
					if     (LSP.x>LEP.x){LSP.x = LEP.x+((LSP.x - LEP.x)/2);}
					else if(LSP.x<LEP.x){LSP.x = LSP.x+((LEP.x - LSP.x)/2);}
				}

				RoiStaX      =(int)(LSP.x+SideMargin   );
				RoiStaY      =(int)(LSP.y              );
				RoiWidth     =200                       ;
				RoiHeight    =(int)(FrameImgH - RoiStaY);

				// �˻� ������ 
				DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
				if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
				if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
				if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

				// Blob �˻� ���� 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);

				// Blob ���͸� 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 1          , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 2          , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(DCount>=MAX_DEFECT_POINT) break;
						DDefectPoint[DCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						DDefectPoint[DCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						DDefectPoint[DCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width/2 ) +RoiStaX);
						DDefectPoint[DCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2) +RoiStaY);
						DDefectPoint[DCount].Rect.right  = (long)(DDefectPoint[DCount].Rect.left+ DDefectPoint[DCount].Width );
						DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

						PixelCount = 0;
						PixelValue = 0;
						// ���� Pixel Data ���� 
						for(int n=DDefectPoint[DCount].Rect.top+1 ; n<DDefectPoint[DCount].Rect.bottom-1; n++){
							for(int m=DDefectPoint[DCount].Rect.left+1; m<DDefectPoint[DCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < DarkThresHold) PixelCount++;
							}
						}
						// Display ������ �ʿ��� 
						DDefectPoint[DCount].Round = PixelCount;
						DDefectPoint[DCount].P.x       = (float)(DDefectPoint[DCount].Rect.left);
						DDefectPoint[DCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight );
						DDefectPoint[DCount].Width     = DDefectPoint[DCount].Width+SideMargin; 

						// Merge ���ؼ� ������ �ʿ��� 
						DDefectPoint[DCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height/2)+RoiStaY+ImageHeight);
						DDefectPoint[DCount].Rect.bottom =(long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
						DCount++;
					}
				}
				BlobCount =0;

				// Broken �� �ڸ� �ҷ� �˻� ������(EnCap�� ������)
				if(!IsMarkBtm && BrokenThresHold!=0){
					BrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight())       ;
					EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BrightDefectImg);
					EasyImage::Threshold(GImage    ,&BrightDefectImg, BrokenThresHold)     ;

					DefectROI.Attach(&BrightDefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
					IAddress = BrightDefectImg.GetImagePtr();

					// Side_D Bright Defect ���� �� �з�(Broken,�ݿ��� �ڸ�___�ɹڸ�)
					/////////////////////////////////////////////////////////////////////////////////////////////////
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

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
							if(DCrackCount>=MAX_DEFECT_POINT) break; 
							DCrackPoint[DCrackCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
							DCrackPoint[DCrackCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

							DCrackPoint[DCrackCount].Rect.left   =(long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width/2) +RoiStaX));
							DCrackPoint[DCrackCount].Rect.top    =(long)(( CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height/2)+RoiStaY );
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
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////
		DefectPointCount = LTCirDeftectCnt+LTCirCrackCnt +LTCorCount+ ACount+BCount+CCount+DCount+ABurrCount+BBurrCount+CBurrCount
			+DBurrCount+ACrackCount+BCrackCount+CCrackCount+DCrackCount+LBCirDeftectCnt+LBCirCrackCnt+LBCorCount+LeftDefectCnt+LeftCrackCnt; // ��ü �ҷ����� ���

		// BTM ���� �˻� ���� �� Cell Size ���� 
		// �˻� ���� �� Cell Size ������ 
		if(LeftStaPos.x!=0.0f && LeftStaPos.y!=0.0f && LeftEndPos.x!=0.0f && LeftEndPos.y!=0.0f){
			LeftCellSizeHeight = Math.GetLengthFrPointToPoint(LeftStaPos, LeftEndPos);
		}
		// Dark Defect Blob Merge
		///////////////////////////////////////////////////////////////////////////////////////////////
		// Circle_1 �ҷ� Merge ��� 
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
					if(CCrackPoint[l].Rect.left==0 && CCrackPoint[l].Rect.top==0 && CCrackPoint[l].Rect.right==0 && CCrackPoint[l].Rect.bottom==0) continue;

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
					if(DBurrPoint[l].Rect.left==0 && DBurrPoint[l].Rect.top==0 && DBurrPoint[l].Rect.right==0 && DBurrPoint[l].Rect.bottom==0) continue;

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
					if(BDefectPoint[l].Rect.left==0 && BDefectPoint[l].Rect.top==0 && BDefectPoint[l].Rect.right==0 && BDefectPoint[l].Rect.bottom==0) continue;

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
					if(BBurrPoint[l].Rect.left==0 && BBurrPoint[l].Rect.top==0 && BBurrPoint[l].Rect.right==0 && BBurrPoint[l].Rect.bottom==0) continue;

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
int CFM3xLineDefect::OnExecute(bool l, bool t, bool r, bool b, bool lt, bool rt, bool rb, bool lb)
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

	// StatLine���� �� �ڵ�ROI������ ����ϴ� RoiRange���� 
	// ���� �˻�� Frame���� ���� ����� ���ؼ� Line_Fitting�Ұ��� 
	// �̷� ���ؼ� HAP�����͸� �������� Margin������ ROI���� �� LineFitting���� �ٷ� Polygon�˻� ������ 
	const int RoiMargin       = 200 ;
	const int LeftRoiPxMargin = 150 ;
	const int LeftRoiWdMargin = 1500;

	// GrindMark ���� Range
	const int GrindMarkRange =40;    
	const int GrindMarkLength=80;

	// HAP or CUP �˻�� ROI���� ����ũ�� ���� ���� 
	int AllGrindWidth =0;

	// 	if(FCompanyMode==COM_TIANMA) BMWidth =500;
	// 	if(FCompanyMode==COM_KWON  ) BMWidth =300;

	// ���ۺκ� Broken�߻��� 
	int LineMargin =30;
	int LineRange  =80;

	// ��üBuff���� ����Frame Real��ǥ 
	// Cutting ���� �˻�� �ʿ��� ������ 
	///////////////////////////////////////
	int RealFrameStaY=0;
	int RealFrameEndY=0;

	int CutMinPos =1000000;
	int CutMaxPos =      0;
	int RoiCount  =      0;

	int GrindThresHold =0;
	///////////////////////////////////////

	// Euresys ROI 
	// Median Filtter ���� ���� ROI   
	EROIBW8   FilterROI                ;

	//Polygon Value Test 
	int         PolygonMarginCnt= 0;
	const int   PolygonMarin    = 5;

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

	FrameImgW = GImage->GetWidth();
	FrameImgH = GImage->GetHeight();
	int PixelCnt =0;

	TFPoint LT(0          ,           0)  ;
	TFPoint RT((float)FrameImgW,           0)  ;
	TFPoint RB((float)FrameImgW, (float)FrameImgH)  ;
	TFPoint LB(0          , (float)FrameImgH)  ;

	TFPoint LSP(0.0f,0.0f), LEP(0.0f,0.0f);
	TFPoint RSP(0.0f,0.0f), REP(0.0f,0.0f);

	//�ڸ� �˻� 
	TFPoint StaPos(0.0f, 0.0f);
	TFPoint EndPos(0.0f, 0.0f);

	double GrindWd1 =0.0f, GrindWd2 =0.0f;
	double GrindMea1=0.0f, GrindMea2=0.0f;

	ImgDegree        =0.0f;

	// Data Init
	FLTPoint.x = 0.0f; //�������ΰ� ��ܶ����� ������ ��ǥ
	FLTPoint.y = 0.0f; //�������ΰ� ��ܶ����� ������ ��ǥ
	FLBPoint.x = 0.0f; //�������ΰ� �ϴܶ����� ������ ��ǥ
	FLBPoint.y = 0.0f; //�������ΰ� �ϴܶ����� ������ ��ǥ

	FLTLPoint.x = 0.0f; //�������ΰ�   ù��° �ڳ� ������ ��ǥ 
	FLTLPoint.y = 0.0f; //�������ΰ�   ù��° �ڳ� ������ ��ǥ 
	FLTTPoint.x = 0.0f; //���ʶ��ΰ�   ù��° �ڳ� ������ ��ǥ 
	FLTTPoint.y = 0.0f; //���ʶ��ΰ�   ù��° �ڳ� ������ ��ǥ 
	FLBLPoint.x = 0.0f; //�������ΰ�   �׹�° �ڳ� ������ ��ǥ 
	FLBLPoint.y = 0.0f; //�������ΰ�   �׹�° �ڳ� ������ ��ǥ 
	FLBBPoint.x = 0.0f; //�Ʒ��ʶ��ΰ� �׹�° �ڳ� ������ ��ǥ 
	FLBBPoint.y = 0.0f; //�Ʒ��ʶ��ΰ� �׹�° �ڳ� ������ ��ǥ 

	//NOTCH Inspection
	FLTLInPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLTLInPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLTTInPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLTTInPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLBLInPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLBLInPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLBBInPoint.x = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ
	FLBBInPoint.y = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� �������� ���ΰ� ������ ��ǥ

	FLTLMeaPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLTLMeaPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLTTMeaPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLTTMeaPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ

	FLBLMeaPoint.x = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLBLMeaPoint.y = 0.0f; //���ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLBBMeaPoint.x = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ
	FLBBMeaPoint.y = 0.0f; //�Ʒ��ʶ��ΰ�   �ڳ� ������ ���� ���� ���ΰ� ������ ��ǥ

	for(int k=0; k<MAX_ROI_ID; k++){
		PolyP1[k].x =0; PolyP1[k].y =0;
		PolyP2[k].x =0; PolyP2[k].y =0;
		PolyP3[k].x =0; PolyP3[k].y =0;
		PolyP4[k].x =0; PolyP4[k].y =0;
	}

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

	// Polygon �˰��� ����� ���缺 ������ ���ؼ� 2���� ���� ��� ã����� �����ؾ��� 
	int nPolyPosCnt=0; int HalfCnt =0; double MaxPolyDis =0.0f; double RealPolyDis=0.0f; double MinPolyDis =100000.0f;
	TFPoint PolyRectCenter; TFPoint PolygonCenter;

	switch (Index) {
		// Find Line
		//----------------------------------------------------------------------
	case 10 :
		//Mobile =>���� ������ ���� 
		//VR  =>��ü����:Polygon ����, �κп���:LineFitting ���� 
		if(FGlassType==Mobile_Glass){ //Mobile Glass 
			if (FTopWall) { //NEW_8CAMERA 
				EasyImage::Threshold(GImage,&BinaryImg, TopEdgeThresHold);
				if(TopEdgeThresHold==0)  {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
				else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

				ROI_X =0, ROI_Y=0, ROI_W=0, ROI_H=0;
				if     (IsRectTop  ){
					ROI_X = IRoi->GetOrgX();
					ROI_Y = IRoi->GetOrgY();
					ROI_W = IRoi->GetWidth();
					ROI_H = IRoi->GetHeight();
				}
				else if(IsCornerTop){
					ROI_X = IRoi->GetOrgX()+LTCornerWidth+RoiMargin   ;
					ROI_Y = IRoi->GetOrgY()                           ;
					ROI_W = (FrameImgW - ROI_X)                       ;
					// 				ROI_W = IRoi->GetWidth()-(LTCornerWidth+RoiMargin);
					ROI_H = IRoi->GetHeight()                         ;
				}
				else if(IsCircleTop){
					ROI_X = IRoi->GetOrgX()+LTSideCircleWidth+RoiMargin   ;
					ROI_Y = IRoi->GetOrgY()                               ;
					ROI_W = (FrameImgW - ROI_X)                           ;
					// 				ROI_W = IRoi->GetWidth()-(LTSideCircleWidth+RoiMargin);
					ROI_H = IRoi->GetHeight()                             ;
				}

				SingleLine->IRoi->SetOrgX  (ROI_X);
				SingleLine->IRoi->SetOrgY  (ROI_Y);
				SingleLine->IRoi->SetWidth (ROI_W);
				SingleLine->IRoi->SetHeight(ROI_H);

				if(ROI_X<=0    || ROI_Y< 0    || ROI_W<=0    || ROI_H<=0   ) return false;
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
				// 			GImage->Save("D:\\ROTATE_IMG\\�ű�_HAP.bmp");
			}
			if (FBottomWall) { //NEW_8CAMERA 
				EasyImage::Threshold(GImage,&BinaryImg, BtmEdgeThresHold);
				if(BtmEdgeThresHold==0)    {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
				else                       {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

				ROI_X =0, ROI_Y=0, ROI_W=0, ROI_H=0;
				if     (IsRectBtm  ){
					ROI_X = IRoi->GetOrgX() ;
					ROI_Y = FEndLine-100    ;
					ROI_W = IRoi->GetWidth();
					ROI_H = 200             ;
				}
				else if(IsCornerBtm){
					ROI_X = IRoi->GetOrgX()+LBCornerWidth+RoiMargin   ;
					ROI_Y = FEndLine-100                              ;
					ROI_W = (FrameImgW - ROI_X)                            ;
					// 				ROI_W = IRoi->GetWidth()-(LBCornerWidth+RoiMargin);
					ROI_H = 200                                       ;
				}
				else if(IsCircleBtm){
					ROI_X = IRoi->GetOrgX()+LBSideCircleWidth+RoiMargin   ;
					ROI_Y = FEndLine-100                                  ;
					ROI_W = (FrameImgW - ROI_X)                                ;
					// 				ROI_W = IRoi->GetWidth()-(LBSideCircleWidth+RoiMargin);
					ROI_H = 200                                           ;
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

				// 			if(IsAllGrind) SingleLine->SetEdgeType(1);  // White To Black 
				// 			else           SingleLine->SetEdgeType(0);  // Black To White
				if(BtmEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
				else               SingleLine->SetEdgeType(1);  // White To Black 
				// �⺻ BtmLine Search 
				SingleLine->OnExecute();
				Line[3].FP0 = SingleLine->GetResultP0();
				Line[3].FP1 = SingleLine->GetResultP1();

				BtmOutLine[0].FP0 =  Line[3].FP0;
				BtmOutLine[0].FP1 =  Line[3].FP1;
				// 			GImage->Save("D:\\ROTATE_IMG\\�ű�_CUP.bmp");
			}
			if (FLeftWall) { //SoulBrain ���� ==> �⺻ �ܰ����� ���� 
				EasyImage::Threshold(GImage,&BinaryImg, LeftEdgeThresHold);
				if(LeftEdgeThresHold==0) {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
				else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

				if(FShape==shRectH){ //���� LeftLineã�� 
					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()                                  );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth()                                 );

					// ChamberCut VS Circle VS Rect��쿡 ���� Left���� Fitting���� ���̰� �޶��� 
					if     (IsCornerTop) SingleLine->IRoi->SetOrgY(LTCornerHeight   +50               );
					else if(IsCircleTop) SingleLine->IRoi->SetOrgY(LTSideCircleHeight+50              );
					else                 SingleLine->IRoi->SetOrgY(IRoi->GetOrgY()                    );

					if     (IsCornerTop) SingleLine->IRoi->SetHeight(FrameImgH-(LTCornerHeight+50    ));
					else if(IsCircleTop) SingleLine->IRoi->SetHeight(FrameImgH-(LTSideCircleHeight+50));
					else                 SingleLine->IRoi->SetHeight(IRoi->GetHeight()                );
					SingleLine->SetIncrement(1);

					ROI_X = SingleLine->IRoi->GetOrgX();
					ROI_Y = SingleLine->IRoi->GetOrgY();
					ROI_W = SingleLine->IRoi->GetWidth();
					ROI_H = SingleLine->IRoi->GetHeight();
					// 				GImage->Save("D:\\ROTATE_IMG\\�ű�_HAP.bmp");
				}
				else if(FShape==shRectC) { //�� LeftLineã�� 
					SingleLine->IRoi->SetOrgX  (IRoi->GetOrgX()                                  );
					SingleLine->IRoi->SetOrgY  (IRoi->GetOrgY()                                  );
					SingleLine->IRoi->SetWidth (IRoi->GetWidth()                                 );

					// ChamberCut VS Circle VS Rect��쿡 ���� Left���� Fitting���� ���̰� �޶��� 
					if     (IsCornerBtm) SingleLine->IRoi->SetHeight(FEndLine-LBCornerHeight    );
					else if(IsCircleBtm) SingleLine->IRoi->SetHeight(FEndLine-LBSideCircleHeight);
					else                 SingleLine->IRoi->SetHeight(IRoi->GetHeight()          );
					SingleLine->SetIncrement(1);

					ROI_X = SingleLine->IRoi->GetOrgX();
					ROI_Y = SingleLine->IRoi->GetOrgY();
					ROI_W = SingleLine->IRoi->GetWidth();
					ROI_H = SingleLine->IRoi->GetHeight();
				}
				else {                      // ��ƽ LeftLine ã�� 
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

				if(IsShortCell){ // �����ϰ�� 
					if(IsAllGrind){ // ��ü���� ������ 
						TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
						// �˻翵�� ����ȭ �� Polygon ROI���� ���� 
						if(FShape==shRectH)     { // ����
							// Pre Left Data Count Init 
							// 						GImage->Save("D:\\ROTATE_IMG\\�ű�_HAP.bmp");

							SingleLine->SetOrientation(orWest);
							SingleLine->SetFindLineId(0);
							// 						SingleLine->SetEdgeType(1); // White to Black 
							if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
							else                SingleLine->SetEdgeType(1);  // White To Black 

							SingleLine->OnExecute();
							Line[0].FP0 = SingleLine->GetResultP0();
							Line[0].FP1 = SingleLine->GetResultP1();

							LeftOutLine[0].FP0 = Line[0].FP0;
							LeftOutLine[0].FP1 = Line[0].FP1;

							FLTPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
							BuffLTPoint.x = FLTPoint.x             ;
							BuffLTPoint.y = FLTPoint.y +ImageHeight;

							if     (IsCornerTop) AllGrindWidth = LTCornerWidth    ;
							else if(IsCircleTop) AllGrindWidth = LTSideCircleWidth;
							else                 AllGrindWidth = 100              ;

							// 						P1.x = (int)(FLTPoint.x-50); P1.y =(int)(FLTPoint.y-(FLTPoint.y/2)); P2.x = (int)(P1.x+AllGrindWidth); P2.y = (int)(P1.y);
							// 						P3.x = (int)(FLTPoint.x-50); P3.y =(int)(FrameImgH- P1.y          ); P4.x = (int)(P2.x              ); P4.y = (int)(P3.y);
							P1.x = (int)(FLTPoint.x-50); P1.y =0                     ; P2.x = (int)(P1.x+AllGrindWidth); P2.y = (int)(P1.y);
							P3.x = (int)(FLTPoint.x-50); P3.y =(int)(FrameImgH- P1.y); P4.x = (int)(P2.x              ); P4.y = (int)(P3.y);
						}
						else if(FShape==shRectC){ //�� 
							// 						GImage->Save("D:\\ROTATE_IMG\\�ű�_CUP.bmp");
							SingleLine->SetOrientation(orWest);
							SingleLine->SetFindLineId(0);
							// 						SingleLine->SetEdgeType(1); // White to Black 
							if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
							else                SingleLine->SetEdgeType(1);  // White To Black 

							SingleLine->OnExecute();
							Line[0].FP0 = SingleLine->GetResultP0();
							Line[0].FP1 = SingleLine->GetResultP1();

							LeftOutLine[0].FP0 = Line[0].FP0;
							LeftOutLine[0].FP1 = Line[0].FP1;

							FLBPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waBottomWall].FP0, Line[waBottomWall].FP1);

							if     (IsCornerBtm) AllGrindWidth = LBCornerWidth    ;
							else if(IsCircleBtm) AllGrindWidth = LBSideCircleWidth;
							else                 AllGrindWidth = 100              ;

							// 						P1.x = (int)(FLBPoint.x-50); P1.y =0                    ; P2.x = (int)(P1.x+AllGrindWidth); P2.y = (int)(P1.y);
							// 						P3.x = (int)(FLBPoint.x-50); P3.y =(int)(FLBPoint.y +10); P4.x = (int)(P2.x              ); P4.y = (int)(P3.y);
							P1.x = (int)(FLBPoint.x-50); P1.y =0        ; P2.x = (int)(P1.x+AllGrindWidth); P2.y = (int)(P1.y);
							P3.x = (int)(FLBPoint.x-50); P3.y =FrameImgH; P4.x = (int)(P2.x              ); P4.y = (int)(P3.y);
						}
						else                    { //��ƽ 
							P1.x = (int)(PreLeftOutLine[0].FP0.x-LeftRoiPxMargin); P1.y =0         ; P2.x = (int)(P1.x+LeftRoiWdMargin); P2.y = (int)(P1.y);
							P3.x = (int)(PreLeftOutLine[0].FP0.x-LeftRoiPxMargin); P3.y =FrameImgH ; P4.x = (int)(P2.x                ); P4.y = (int)(P3.y);
							// 						GImage->Save("D:\\ROTATE_IMG\\�ű�_STATIC.bmp");
						}

						//����,������ ���� ���� ������ ���� 
						// 					if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
						if(P1.x!=0 &&  P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
							// 						LeftCutThresHold = 65;
							if(LeftCutThresHold==0){ //Cut���� ���� �Ƕ����� ���������� ���ϰ�� or ���� ��ü ���� �ϰ�� ������ 
								EasyImage::Threshold(GImage,&BinaryImg, LeftGrindThresHold);
								// Median Filter ���� 
								if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
								if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) return false;
								if((P1.x+(P2.x - P1.x))>=FrameImgW    || (P1.y+(P3.y - P1.y))>FrameImgH                      ) return false;

								FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
								FilterROI.Attach(&BinaryImg)                                          ;
								FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
								LogUnit.SetLog(L"AllGrind_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
								//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
								EasyImage::Median(&FilterROI, &FilterROI);
//	 							if(FShape==shRectH) GImage->Save("D:\\ROTATE_IMG\\Polygon_Left_HAP_����.bmp");
//								if(FShape==shRectS) BinaryImg.Save("D:\\ROTATE_IMG\\Polygon_Left_STICK.bmp");

								Polygon->GImage = &BinaryImg                     ;
								Polygon->IWidth = FrameImgW                      ;
								Polygon->IHeight= FrameImgH                      ;
								Polygon->PoPa.FMinWidth  =2                      ;                       
								Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
								Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
								Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
								Polygon->PoPa.MinPxCnt   =100                    ;
								Polygon->PoPa.MaxPxCnt   =20000                  ;
								Polygon->PoPa.MaxBbCnt   =10                     ;
								Polygon->PoPa.IsSeparate =true                   ;
								Polygon->PoPa.IsInToOut  =false                  ;
								Polygon->PoPa.HiThHold   = LeftGrindThresHold    ;
								Polygon->PoPa.LoThHold   = 0                     ;
								Polygon->PoPa.IgInSide   = true                  ;
								Polygon->PoPa.IsHorSort  = false                 ;
								Polygon->PoPa.IsVerSort  = true                  ;
								Polygon->PoPa.IsInToLeft = true                  ;

								// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
								PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
								PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
								PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
								PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

								if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
									Polygon->OnExecute(P1,P2,P3,P4);
								}

								// 2���̻� ���� � ������ ����� �з� ���� 
								if(Polygon->BlobCount>1){
									// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
									PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
									PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
									for(int k=0; k<Polygon->BlobCount; k++){
										PolygonCenter.x = Polygon->Blob[k].Rect.l;
										PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
										// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
										RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
										if(RealPolyDis<MinPolyDis) {
											MinPolyDis  = RealPolyDis;
											nPolyPosCnt = k;
										}
									}
								}
								else nPolyPosCnt=0;
								// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
								////////////////////////////////////////////////////////////////////
								if(Polygon->Blob[nPolyPosCnt].InCnt>0){
									LeftInEdgeCount=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

										if(FShape==shRectH ){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
											//										if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
											if(!IsRectTop && (fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;

										}
										else if(FShape==shRectC){
											//										if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
											if(!IsRectBtm && (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
										}
										LeftInEdgeCount++;
									}

									if(LeftInEdgeCount>0){
										if(LeftInPoint!=NULL) delete[] LeftInPoint;
										LeftInPoint = new TFPoint[LeftInEdgeCount];

										PixelCnt=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

											if(FShape==shRectH ){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												//											if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
												if(!IsRectTop && (fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
											}
											else if(FShape==shRectC ){
												//											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
												if(!IsRectBtm && (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
											}
											LeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
											LeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
											PixelCnt++;
										}
									}
								}

								if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
									LeftOutEdgeCount=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

										if(FShape==shRectC ){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
										}
										LeftOutEdgeCount++;
									}

									if(LeftOutEdgeCount>0){
										if(LeftOutPoint!=NULL) delete[] LeftOutPoint;
										LeftOutPoint = new TFPoint[LeftOutEdgeCount];

										PixelCnt=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

											if(FShape==shRectC ){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
											}
											LeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
											LeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
											PixelCnt++;
										}
									}
								}
								////////////////////////////////////////////////////////////////////

								// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
								if(LeftInEdgeCount>0 && LeftOutEdgeCount>0){
									// Total Left In Data 
									for(int k=0; k<LeftInEdgeCount; k++){
										if(AllLeftInEdgeCount>MAX_ALL_EDGE_COUNT) return false            ;
										AllLeftInData[AllLeftInEdgeCount].x = LeftInPoint[k].x            ;
										AllLeftInData[AllLeftInEdgeCount].y = LeftInPoint[k].y+ImageHeight;
										AllLeftInEdgeCount++;
									}
									// Total Left Out Data 
									for(int k=0; k<LeftOutEdgeCount; k++){
										if(AllLeftOutEdgeCount>MAX_ALL_EDGE_COUNT) return false              ;
										AllLeftOutData[AllLeftOutEdgeCount].x = LeftOutPoint[k].x            ;
										AllLeftOutData[AllLeftOutEdgeCount].y = LeftOutPoint[k].y+ImageHeight;
										AllLeftOutEdgeCount++;
									}
								}
							}
							else                   { //Cut���� ���� �Ƕ����� ������������ ���ϰ�� 
								IsFrameGrindMark = false                ;
								RealFrameStaY    = ImageHeight          ; //��üBuff���� Frame_Real��ǥ 
								RealFrameEndY    = ImageHeight+FrameImgH; //��üBuff���� Frame_Real��ǥ 

								// 							for(int k=0; k<2; k++){GrindStepy[k].x=0; GrindStepy[k].y=0;}
								for(int k=0; k<3; k++){
									GrindLine[k].LinePoint.x =0;
									GrindLine[k].LinePoint.y =0;
									GrindLine[k].IsGrindLine =false;
								}

								// ��üBuffer���� �˻� Frame�� GrindMark�����ϴ��� Ȯ�� �� üũ 
								for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
									if(GrindMark[k].DvX==0 || GrindMark[k].DvY==0) continue;
									if(((BuffLTPoint.y + GrindMark[k].DvY)>= RealFrameStaY) && ((BuffLTPoint.y + GrindMark[k].DvY)<= RealFrameEndY)){
										IsFrameGrindMark = true;

										//��ü Buffer���� ��ǥ 
										if((BuffLTPoint.y + GrindMark[k].DvY)<CutMinPos) CutMinPos =(BuffLTPoint.y + GrindMark[k].DvY);
										if((BuffLTPoint.y + GrindMark[k].DvY)>CutMaxPos) CutMaxPos =(BuffLTPoint.y + GrindMark[k].DvY); 
									}
								}

								// Frame���� ��ǥ�� ��ȯ 
								if(CutMinPos>0) CutMinPos = CutMinPos - ImageHeight;
								if(CutMaxPos>0) CutMaxPos = CutMaxPos - ImageHeight;

								// ��üBuffer���� ���� Frame�� GrindMark�� �����ϸ� ������ �и��Ͽ� �˻� ������ 
								if(IsFrameGrindMark){
									// TEST ���� 
									// 								LeftGrindThresHold =30;
									// 								LeftCutThresHold   =65;
									if     (FShape==shRectH){
										RoiCount =2;
										GrindLine[0].LinePoint.y = 0           ;
										GrindLine[0].IsGrindLine = true        ;
										GrindLine[1].LinePoint.y = CutMaxPos+30;
										GrindLine[1].IsGrindLine = false       ;
									}
									else if(FShape==shRectS){
										RoiCount =3;
										GrindLine[0].LinePoint.y = 0           ;
										GrindLine[0].IsGrindLine = false       ;
										GrindLine[1].LinePoint.y = ((CutMinPos-30)<0)?0:CutMinPos-30;
										GrindLine[1].IsGrindLine = true        ;
										GrindLine[2].LinePoint.y = ((CutMaxPos+30)>FrameImgH)?FrameImgH:(CutMaxPos+30);
										GrindLine[2].IsGrindLine = false       ;
									}
									else if(FShape==shRectC){
										RoiCount =2;
										GrindLine[0].LinePoint.y = 0           ;
										GrindLine[0].IsGrindLine = false       ;
										GrindLine[1].LinePoint.y = CutMinPos-30;
										GrindLine[1].IsGrindLine = true        ;
									}

									for(int k=0; k<RoiCount; k++){
										GrindThresHold =0;
										if(k==(RoiCount-1)){
											P1.y =GrindLine[k].LinePoint.y; P2.y = (int)(P1.y);
											// 										P3.y =(int)(FrameImgH- P1.y)  ; P4.y = (int)(P3.y);
											P3.y =(int)(FrameImgH      )  ; P4.y = (int)(P3.y);
										}
										else               {
											P1.y =GrindLine[k  ].LinePoint.y; P2.y = (int)(P1.y);
											// 										P3.y =GrindLine[k+1].LinePoint.y -GrindLine[k].LinePoint.y; P4.y = (int)(P3.y);
											P3.y =GrindLine[k+1].LinePoint.y; P4.y = (int)(P3.y);
										}
										GrindThresHold = (GrindLine[k].IsGrindLine)?LeftGrindThresHold:LeftCutThresHold;

										// 1�� Frame_Buff�� GrindMark Position���� ��Ȱ�Ͽ� ROI���������� �ʿ��� 
										if(k==0){StaLeftInEdgeCount=AllLeftInEdgeCount; StaLeftOutEdgeCount=AllLeftOutEdgeCount; }
										// GrindMark���� ���� ��Ȱ�� ������ �ʹ� ������� Polygon_Data �������� ���� 
										if((P3.y - P1.y)<15) continue;

										EasyImage::Threshold(GImage,&BinaryImg, GrindThresHold);
										// Median Filter ���� 
										if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) continue;
										if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) continue;
										if((P1.x+(P2.x - P1.x))>=FrameImgW    || (P1.y+(P3.y - P1.y))>FrameImgH                      ) continue;


										FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
										FilterROI.Attach(&BinaryImg)                                          ;
										FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
										LogUnit.SetLog(L"AllGrind_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
										//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
										EasyImage::Median(&FilterROI, &FilterROI)                             ;

										Polygon->GImage = &BinaryImg                     ;
										Polygon->IWidth = FrameImgW                      ;
										Polygon->IHeight= FrameImgH                      ;
										Polygon->PoPa.FMinWidth  =2                      ;                       
										Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
										Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
										Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
										// 									Polygon->PoPa.MinPxCnt   =100                    ;
										Polygon->PoPa.MinPxCnt   =50                     ;
										Polygon->PoPa.MaxPxCnt   =20000                  ;
										Polygon->PoPa.MaxBbCnt   =10                     ;
										Polygon->PoPa.IsSeparate =true                   ;
										Polygon->PoPa.IsInToOut  =false                  ;
										Polygon->PoPa.HiThHold   = GrindThresHold        ;
										Polygon->PoPa.LoThHold   = 0                     ;
										Polygon->PoPa.IgInSide   = true                  ;
										Polygon->PoPa.IsHorSort  = false                 ;
										Polygon->PoPa.IsVerSort  = true                  ;
										Polygon->PoPa.IsInToLeft = true                  ;

										// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
										PolyP1[k].x = P1.x;  PolyP1[k].y = P1.y;
										PolyP2[k].x = P2.x;  PolyP2[k].y = P2.y;
										PolyP3[k].x = P3.x;  PolyP3[k].y = P3.y;
										PolyP4[k].x = P4.x;  PolyP4[k].y = P4.y;

										if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
											Polygon->OnExecute(P1,P2,P3,P4);
										}

										// 2���̻� ���� � ������ ����� �з� ���� 
										if(Polygon->BlobCount>1){
											// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
											PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
											PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
											for(int k=0; k<Polygon->BlobCount; k++){
												PolygonCenter.x = Polygon->Blob[k].Rect.l;
												PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
												// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
												RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
												if(RealPolyDis<MinPolyDis) {
													MinPolyDis  = RealPolyDis;
													nPolyPosCnt = k;
												}
											}
										}
										else nPolyPosCnt=0;

										// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
										////////////////////////////////////////////////////////////////////
										if(Polygon->Blob[nPolyPosCnt].InCnt>0){
											FrameLeftInEdgeCount=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

												if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
													//												if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
													if(!IsRectTop && (fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
												}
												else if(FShape==shRectC){
													//												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
													if(!IsRectBtm && (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
												}
												FrameLeftInEdgeCount++;
											}

											if(FrameLeftInEdgeCount>0){
												if(FrameLeftInPoint!=NULL) delete[] FrameLeftInPoint;
												FrameLeftInPoint = new TFPoint[FrameLeftInEdgeCount];

												PixelCnt=0;
												for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
													if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
													if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

													if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
														//													if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
														if(!IsRectTop && (fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
													}
													else if(FShape==shRectC){
														//													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
														if(!IsRectBtm && (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
													}

													FrameLeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
													FrameLeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
													PixelCnt++;
												}
											}
										}

										if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
											FrameLeftOutEdgeCount=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

												if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
												}
												FrameLeftOutEdgeCount++;
											}

											if(FrameLeftOutEdgeCount>0){
												if(FrameLeftOutPoint!=NULL) delete[] FrameLeftOutPoint;
												FrameLeftOutPoint = new TFPoint[FrameLeftOutEdgeCount];

												PixelCnt=0;
												for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
													if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
													if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

													if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
														if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
													}
													FrameLeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
													FrameLeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
													PixelCnt++;
												}
											}
										}
										////////////////////////////////////////////////////////////////////

										// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
										if(FrameLeftInEdgeCount>0 && FrameLeftOutEdgeCount>0){
											// Total Left In Data 
											for(int k=0; k<FrameLeftInEdgeCount; k++){
												if(AllLeftInEdgeCount>MAX_ALL_EDGE_COUNT) return false                 ;
												AllLeftInData[AllLeftInEdgeCount].x = FrameLeftInPoint[k].x            ;
												AllLeftInData[AllLeftInEdgeCount].y = FrameLeftInPoint[k].y+ImageHeight;
												AllLeftInEdgeCount++;
											}
											// Total Left Out Data 
											for(int k=0; k<FrameLeftOutEdgeCount; k++){
												if(AllLeftOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                   ;
												AllLeftOutData[AllLeftOutEdgeCount].x = FrameLeftOutPoint[k].x            ;
												AllLeftOutData[AllLeftOutEdgeCount].y = FrameLeftOutPoint[k].y+ImageHeight;
												AllLeftOutEdgeCount++;
											}
										}
									}

									// Frame�� ROI�������� ��Ȱ�˻� ���� �� Frame�������� �ٽ� ������ �Ѱ���
									// Dark_Defect/Bright_Defect�˻� ���� 
									if((AllLeftInEdgeCount-StaLeftInEdgeCount)>0 && (AllLeftOutEdgeCount-StaLeftOutEdgeCount)>0){
										// LeftInEdgeData ���� ���� 
										LeftInEdgeCount = (AllLeftInEdgeCount-StaLeftInEdgeCount);
										if(LeftInPoint!=NULL) delete[] LeftInPoint;
										LeftInPoint = new TFPoint[LeftInEdgeCount];

										// LeftOutEdgeData ���� ���� 
										LeftOutEdgeCount = (AllLeftOutEdgeCount-StaLeftOutEdgeCount);
										if(LeftOutPoint!=NULL) delete[] LeftOutPoint;
										LeftOutPoint = new TFPoint[LeftOutEdgeCount];

										// ��ü �����Ϳ��� Ư������ �����͸� �Ѱ��� 
										for(int k=0; k<LeftInEdgeCount; k++){
											LeftInPoint[k].x   = AllLeftInData[StaLeftInEdgeCount+k].x             ;
											LeftInPoint[k].y   = AllLeftInData[StaLeftInEdgeCount+k].y -ImageHeight;
										}

										// ��ü �����Ϳ��� Ư������ �����͸� �Ѱ��� 
										for(int k=0; k<LeftOutEdgeCount; k++){
											LeftOutPoint[k].x   = AllLeftOutData[StaLeftOutEdgeCount+k].x             ;
											LeftOutPoint[k].y   = AllLeftOutData[StaLeftOutEdgeCount+k].y -ImageHeight;
										}
									}
								}
								else                { // GrindMark�������� ���� 
									EasyImage::Threshold(GImage,&BinaryImg, LeftCutThresHold);
									// Median Filter ���� 
									if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
									if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) return false;
									if((P1.x+(P2.x - P1.x))>=FrameImgW    || (P1.y+(P3.y - P1.y))>FrameImgH                      ) return false;

									FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
									FilterROI.Attach(&BinaryImg)                                          ;
									FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
									LogUnit.SetLog(L"AllGrind_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
									//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
									EasyImage::Median(&FilterROI, &FilterROI)                                 ;
									// 								if(FShape==shRectH) BinaryImg.Save("D:\\ROTATE_IMG\\Polygon_Left_CUTLine.bmp");

									Polygon->GImage = &BinaryImg                     ;
									Polygon->IWidth = FrameImgW                      ;
									Polygon->IHeight= FrameImgH                      ;
									Polygon->PoPa.FMinWidth  =2                      ;                       
									Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
									Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
									Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
									Polygon->PoPa.MinPxCnt   =100                    ;
									Polygon->PoPa.MaxPxCnt   =20000                  ;
									Polygon->PoPa.MaxBbCnt   =10                     ;
									Polygon->PoPa.IsSeparate =true                   ;
									Polygon->PoPa.IsInToOut  =false                  ;
									Polygon->PoPa.HiThHold   = LeftCutThresHold      ;
									Polygon->PoPa.LoThHold   = 0                     ;
									Polygon->PoPa.IgInSide   = true                  ;
									Polygon->PoPa.IsHorSort  = false                 ;
									Polygon->PoPa.IsVerSort  = true                  ;
									Polygon->PoPa.IsInToLeft = true                  ;

									// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
									PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
									PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
									PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
									PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

									if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
										Polygon->OnExecute(P1,P2,P3,P4);
									}

									// 2���̻� ���� � ������ ����� �з� ���� 
									if(Polygon->BlobCount>1){
										// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
										PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
										PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
										for(int k=0; k<Polygon->BlobCount; k++){
											PolygonCenter.x = Polygon->Blob[k].Rect.l;
											PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
											// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
											RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
											if(RealPolyDis<MinPolyDis) {
												MinPolyDis  = RealPolyDis;
												nPolyPosCnt = k;
											}
										}
									}
									else nPolyPosCnt=0;

									// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
									////////////////////////////////////////////////////////////////////
									if(Polygon->Blob[nPolyPosCnt].InCnt>0){
										LeftInEdgeCount=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

											if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												//											if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
												if(!IsRectTop && (fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
											}
											else if(FShape==shRectC){
												//											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
												if(!IsRectBtm && (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
											}
											LeftInEdgeCount++;
										}

										if(LeftInEdgeCount>0){
											if(LeftInPoint!=NULL) delete[] LeftInPoint;
											LeftInPoint = new TFPoint[LeftInEdgeCount];

											PixelCnt=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

												if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
													//												if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
													if(!IsRectTop && (fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
												}
												else if(FShape==shRectC){
													//												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
													if(!IsRectBtm && (fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin)) continue;
												}
												LeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
												LeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
												PixelCnt++;
											}
										}
									}

									if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
										LeftOutEdgeCount=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

											if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
											}
											LeftOutEdgeCount++;
										}

										if(LeftOutEdgeCount>0){
											if(LeftOutPoint!=NULL) delete[] LeftOutPoint;
											LeftOutPoint = new TFPoint[LeftOutEdgeCount];

											PixelCnt=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

												if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
												}
												LeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
												LeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
												PixelCnt++;
											}
										}
									}
									////////////////////////////////////////////////////////////////////

									// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
									if(LeftInEdgeCount>0 && LeftOutEdgeCount>0){
										// Total Left In Data 
										for(int k=0; k<LeftInEdgeCount; k++){
											if(AllLeftInEdgeCount>MAX_ALL_EDGE_COUNT) return false            ;
											AllLeftInData[AllLeftInEdgeCount].x = LeftInPoint[k].x            ;
											AllLeftInData[AllLeftInEdgeCount].y = LeftInPoint[k].y+ImageHeight;
											AllLeftInEdgeCount++;
										}
										// Total Left Out Data 
										for(int k=0; k<LeftOutEdgeCount; k++){
											if(AllLeftOutEdgeCount>MAX_ALL_EDGE_COUNT) return false              ;
											AllLeftOutData[AllLeftOutEdgeCount].x = LeftOutPoint[k].x            ;
											AllLeftOutData[AllLeftOutEdgeCount].y = LeftOutPoint[k].y+ImageHeight;
											AllLeftOutEdgeCount++;
										}
									}
								}
							}
						}
					}
					else          { // �κп��� ������ 
						SingleLine->SetOrientation(orWest);
						SingleLine->SetFindLineId(0);
						// 					SingleLine->SetEdgeType(0); // White to Black 
						if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
						else                SingleLine->SetEdgeType(1);  // White To Black 

						SingleLine->OnExecute();
						Line[0].FP0 = SingleLine->GetResultP0();
						Line[0].FP1 = SingleLine->GetResultP1();

						//�������� ���� Line �����Ұ�� Line_Fitting ��ǥ ������ 
						if(ROI_H<400){
							if(fabs(Line[0].FP0.x- Line[0].FP1.x)>3){
								if(Line[0].FP0.x>Line[0].FP1.x) Line[0].FP0.x = Line[0].FP1.x;
								if(Line[0].FP0.x<Line[0].FP1.x) Line[0].FP1.x = Line[0].FP0.x;
							}
						}

						// Btm,Top ������� ù��°
						LeftOutLine[0].FP0.x = Line[0].FP0.x;
						LeftOutLine[0].FP0.y = Line[0].FP0.y;
						LeftOutLine[0].FP1.x = Line[0].FP1.x;
						LeftOutLine[0].FP1.y = Line[0].FP1.y;
					}
				}
				else           { // �����ϰ�� 
					if(IsAllGrind){ // ���� NEW Sequence(��ü�������)
						TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
						// �˻翵�� ����ȭ �� Polygon ROI���� ���� 
						if(FShape==shRectH)     { // ����
							// Pre Left Data Count Init 
							// 						GImage->Save("D:\\ROTATE_IMG\\�ű�_HAP.bmp");
							SingleLine->SetOrientation(orWest);
							SingleLine->SetFindLineId(0);
							// 						SingleLine->SetEdgeType(1); // White to Black 
							if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
							else                SingleLine->SetEdgeType(1);  // White To Black 

							SingleLine->OnExecute();
							Line[0].FP0 = SingleLine->GetResultP0();
							Line[0].FP1 = SingleLine->GetResultP1();

							LeftOutLine[0].FP0 = Line[0].FP0;
							LeftOutLine[0].FP1 = Line[0].FP1;

							FLTPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
							BuffLTPoint.x = FLTPoint.x             ;
							BuffLTPoint.y = FLTPoint.y +ImageHeight;

							if     (IsCornerTop) AllGrindWidth = LTCornerWidth    ;
							else if(IsCircleTop) AllGrindWidth = LTSideCircleWidth;
							else                 AllGrindWidth = 100              ;

							// 						P1.x = (int)(FLTPoint.x-50); P1.y =(int)(FLTPoint.y-(FLTPoint.y/2)); P2.x = (int)(P1.x+AllGrindWidth); P2.y = (int)(P1.y);
							// 						P3.x = (int)(FLTPoint.x-50); P3.y =(int)(FrameImgH- P1.y          ); P4.x = (int)(P2.x              ); P4.y = (int)(P3.y);
							P1.x = (int)(FLTPoint.x-50); P1.y =0                     ; P2.x = (int)(P1.x+AllGrindWidth); P2.y = (int)(P1.y);
							P3.x = (int)(FLTPoint.x-50); P3.y =(int)(FrameImgH- P1.y); P4.x = (int)(P2.x              ); P4.y = (int)(P3.y);
						}
						else if(FShape==shRectC){ //�� 
							// 						GImage->Save("D:\\ROTATE_IMG\\�ű�_CUP.bmp");
							SingleLine->SetOrientation(orWest);
							SingleLine->SetFindLineId(0);
							// 						SingleLine->SetEdgeType(1); // White to Black 
							if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
							else                SingleLine->SetEdgeType(1);  // White To Black 

							SingleLine->OnExecute();
							Line[0].FP0 = SingleLine->GetResultP0();
							Line[0].FP1 = SingleLine->GetResultP1();

							LeftOutLine[0].FP0 = Line[0].FP0;
							LeftOutLine[0].FP1 = Line[0].FP1;

							FLBPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waBottomWall].FP0, Line[waBottomWall].FP1);

							if     (IsCornerBtm) AllGrindWidth = LBCornerWidth    ;
							else if(IsCircleBtm) AllGrindWidth = LBSideCircleWidth;
							else                 AllGrindWidth = 100              ;

							// 						P1.x = (int)(FLBPoint.x-50); P1.y =0                    ; P2.x = (int)(P1.x+AllGrindWidth); P2.y = (int)(P1.y);
							// 						P3.x = (int)(FLBPoint.x-50); P3.y =(int)(FLBPoint.y +10); P4.x = (int)(P2.x              ); P4.y = (int)(P3.y);
							P1.x = (int)(FLBPoint.x-50); P1.y =0        ; P2.x = (int)(P1.x+AllGrindWidth); P2.y = (int)(P1.y);
							P3.x = (int)(FLBPoint.x-50); P3.y =FrameImgH; P4.x = (int)(P2.x              ); P4.y = (int)(P3.y);
						}
						else                    { //��ƽ 
							P1.x = (int)(PreLeftOutLine[0].FP0.x-LeftRoiPxMargin); P1.y =0         ; P2.x = (int)(P1.x+LeftRoiWdMargin); P2.y = (int)(P1.y);
							P3.x = (int)(PreLeftOutLine[0].FP0.x-LeftRoiPxMargin); P3.y =FrameImgH ; P4.x = (int)(P2.x                ); P4.y = (int)(P3.y);
							// 						GImage->Save("D:\\ROTATE_IMG\\�ű�_STATIC.bmp");
						}

						// ���� New Sequence Start 
						if(P1.x!=0 &&  P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
							// 						LeftCutThresHold = 65;
							if(LeftCutThresHold==0){ //Cut���� ���� �Ƕ����� ���������� ���ϰ�� or ���� ��ü ���� �ϰ�� ������ 
								EasyImage::Threshold(GImage,&BinaryImg, LeftGrindThresHold);
								// Median Filter ���� 
								if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
								if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) return false;
								if((P1.x+(P2.x - P1.x))>=FrameImgW    || (P1.y+(P3.y - P1.y))>FrameImgH                      ) return false;

								FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
								FilterROI.Attach(&BinaryImg)                                          ;
								FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
								LogUnit.SetLog(L"AllGrind_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
								//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
								EasyImage::Median(&FilterROI, &FilterROI)                                 ;
								// 							if(FShape==shRectH) BinaryImg.Save("D:\\ROTATE_IMG\\Polygon_Left_HAP.bmp");

								Polygon->GImage = &BinaryImg                     ;
								Polygon->IWidth = FrameImgW                      ;
								Polygon->IHeight= FrameImgH                      ;
								Polygon->PoPa.FMinWidth  =2                      ;                       
								Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
								Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
								Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
								Polygon->PoPa.MinPxCnt   =100                    ;
								Polygon->PoPa.MaxPxCnt   =20000                  ;
								Polygon->PoPa.MaxBbCnt   =10                     ;
								Polygon->PoPa.IsSeparate =true                   ;
								Polygon->PoPa.IsInToOut  =false                  ;
								Polygon->PoPa.HiThHold   = LeftGrindThresHold    ;
								Polygon->PoPa.LoThHold   = 0                     ;
								Polygon->PoPa.IgInSide   = true                  ;
								Polygon->PoPa.IsHorSort  = false                 ;
								Polygon->PoPa.IsVerSort  = true                  ;
								Polygon->PoPa.IsInToLeft = true                  ;

								// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
								PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
								PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
								PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
								PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

								if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
									Polygon->OnExecute(P1,P2,P3,P4);
								}

								// 2���̻� ���� � ������ ����� �з� ���� 
								if(Polygon->BlobCount>1){
									// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
									PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
									PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
									for(int k=0; k<Polygon->BlobCount; k++){
										PolygonCenter.x = Polygon->Blob[k].Rect.l;
										PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
										// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
										RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
										if(RealPolyDis<MinPolyDis) {
											MinPolyDis  = RealPolyDis;
											nPolyPosCnt = k;
										}
									}
								}
								else nPolyPosCnt=0;

								// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
								////////////////////////////////////////////////////////////////////
								if(Polygon->Blob[nPolyPosCnt].InCnt>0){
									LeftInEdgeCount=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

										if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
											if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
										}
										else if(FShape==shRectC){
											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
										}
										LeftInEdgeCount++;
									}

									if(LeftInEdgeCount>0){
										if(LeftInPoint!=NULL) delete[] LeftInPoint;
										LeftInPoint = new TFPoint[LeftInEdgeCount];

										PixelCnt=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

											if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
											}
											else if(FShape==shRectC){
												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
											}
											LeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
											LeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
											PixelCnt++;
										}
									}
								}

								if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
									LeftOutEdgeCount=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

										if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
										}
										LeftOutEdgeCount++;
									}

									if(LeftOutEdgeCount>0){
										if(LeftOutPoint!=NULL) delete[] LeftOutPoint;
										LeftOutPoint = new TFPoint[LeftOutEdgeCount];

										PixelCnt=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

											if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
											}
											LeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
											LeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
											PixelCnt++;
										}
									}
								}
								////////////////////////////////////////////////////////////////////

								// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
								if(LeftInEdgeCount>0 && LeftOutEdgeCount>0){
									// Total Left In Data 
									for(int k=0; k<LeftInEdgeCount; k++){
										if(AllLeftInEdgeCount>MAX_ALL_EDGE_COUNT) return false            ;
										AllLeftInData[AllLeftInEdgeCount].x = LeftInPoint[k].x            ;
										AllLeftInData[AllLeftInEdgeCount].y = LeftInPoint[k].y+ImageHeight;
										AllLeftInEdgeCount++;
									}
									// Total Left Out Data 
									for(int k=0; k<LeftOutEdgeCount; k++){
										if(AllLeftOutEdgeCount>MAX_ALL_EDGE_COUNT) return false              ;
										AllLeftOutData[AllLeftOutEdgeCount].x = LeftOutPoint[k].x            ;
										AllLeftOutData[AllLeftOutEdgeCount].y = LeftOutPoint[k].y+ImageHeight;
										AllLeftOutEdgeCount++;
									}
								}
							}
							else                   { //Cut���� ���� �Ƕ����� ������������ ���ϰ�� 
								IsFrameGrindMark = false                ;
								RealFrameStaY    = ImageHeight          ; //��üBuff���� Frame_Real��ǥ 
								RealFrameEndY    = ImageHeight+FrameImgH; //��üBuff���� Frame_Real��ǥ 

								// 							for(int k=0; k<2; k++){GrindStepy[k].x=0; GrindStepy[k].y=0;}
								for(int k=0; k<3; k++){
									GrindLine[k].LinePoint.x =0;
									GrindLine[k].LinePoint.y =0;
									GrindLine[k].IsGrindLine =false;
								}

								// ��üBuffer���� �˻� Frame�� GrindMark�����ϴ��� Ȯ�� �� üũ 
								for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
									if(GrindMark[k].DvX==0 || GrindMark[k].DvY==0) continue;
									if(((BuffLTPoint.y + GrindMark[k].DvY)>= RealFrameStaY) && ((BuffLTPoint.y + GrindMark[k].DvY)<= RealFrameEndY)){
										IsFrameGrindMark = true;

										//��ü Buffer���� ��ǥ 
										if((BuffLTPoint.y + GrindMark[k].DvY)<CutMinPos) CutMinPos =(BuffLTPoint.y + GrindMark[k].DvY);
										if((BuffLTPoint.y + GrindMark[k].DvY)>CutMaxPos) CutMaxPos =(BuffLTPoint.y + GrindMark[k].DvY); 
									}
								}

								// Frame���� ��ǥ�� ��ȯ 
								if(CutMinPos>0) CutMinPos = CutMinPos - ImageHeight;
								if(CutMaxPos>0) CutMaxPos = CutMaxPos - ImageHeight;

								// ��üBuffer���� ���� Frame�� GrindMark�� �����ϸ� ������ �и��Ͽ� �˻� ������ 
								if(IsFrameGrindMark){
									if     (FShape==shRectH){
										RoiCount =2;
										GrindLine[0].LinePoint.y = 0           ;
										GrindLine[0].IsGrindLine = true        ;
										GrindLine[1].LinePoint.y = CutMaxPos+30;
										GrindLine[1].IsGrindLine = false       ;
									}
									else if(FShape==shRectS){
										RoiCount =3;
										GrindLine[0].LinePoint.y = 0           ;
										GrindLine[0].IsGrindLine = false       ;
										GrindLine[1].LinePoint.y = ((CutMinPos-30)<0)?0:CutMinPos-30;
										GrindLine[1].IsGrindLine = true        ;
										GrindLine[2].LinePoint.y = ((CutMaxPos+30)>FrameImgH)?FrameImgH:(CutMaxPos+30);
										GrindLine[2].IsGrindLine = false       ;
									}
									else if(FShape==shRectC){
										RoiCount =2;
										GrindLine[0].LinePoint.y = 0           ;
										GrindLine[0].IsGrindLine = false       ;
										GrindLine[1].LinePoint.y = CutMinPos-30;
										GrindLine[1].IsGrindLine = true        ;
									}

									for(int k=0; k<RoiCount; k++){
										GrindThresHold =0;
										if(k==(RoiCount-1)){
											P1.y =GrindLine[k].LinePoint.y; P2.y = (int)(P1.y);
											// 										P3.y =(int)(FrameImgH- P1.y)  ; P4.y = (int)(P3.y);
											P3.y =(int)(FrameImgH      )  ; P4.y = (int)(P3.y);
										}
										else               {
											P1.y =GrindLine[k  ].LinePoint.y; P2.y = (int)(P1.y);
											// 										P3.y =GrindLine[k+1].LinePoint.y -GrindLine[k].LinePoint.y; P4.y = (int)(P3.y);
											P3.y =GrindLine[k+1].LinePoint.y; P4.y = (int)(P3.y);
										}
										GrindThresHold = (GrindLine[k].IsGrindLine)?LeftGrindThresHold:LeftCutThresHold;

										// 1�� Frame_Buff�� GrindMark Position���� ��Ȱ�Ͽ� ROI���������� �ʿ��� 
										if(k==0){StaLeftInEdgeCount=AllLeftInEdgeCount; StaLeftOutEdgeCount=AllLeftOutEdgeCount; }
										// GrindMark���� ���� ��Ȱ�� ������ �ʹ� ������� Polygon_Data �������� ���� 
										if((P3.y - P1.y)<15) continue;

										EasyImage::Threshold(GImage,&BinaryImg, GrindThresHold);

										if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) continue;
										if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) continue;
										if((P1.x+(P2.x - P1.x))>=FrameImgW    || (P1.y+(P3.y - P1.y))>FrameImgH                      ) continue;


										FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
										FilterROI.Attach(&BinaryImg)                                          ;
										FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
										LogUnit.SetLog(L"AllGrind_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
										//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
										EasyImage::Median(&FilterROI, &FilterROI)                             ;

										Polygon->GImage = &BinaryImg                     ;
										Polygon->IWidth = FrameImgW                      ;
										Polygon->IHeight= FrameImgH                      ;
										Polygon->PoPa.FMinWidth  =2                      ;                       
										Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
										Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
										Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
										// 									Polygon->PoPa.MinPxCnt   =100                    ;
										Polygon->PoPa.MinPxCnt   =50                     ;
										Polygon->PoPa.MaxPxCnt   =20000                  ;
										Polygon->PoPa.MaxBbCnt   =10                     ;
										Polygon->PoPa.IsSeparate =true                   ;
										Polygon->PoPa.IsInToOut  =false                  ;
										Polygon->PoPa.HiThHold   = GrindThresHold        ;
										Polygon->PoPa.LoThHold   = 0                     ;
										Polygon->PoPa.IgInSide   = true                  ;
										Polygon->PoPa.IsHorSort  = false                 ;
										Polygon->PoPa.IsVerSort  = true                  ;
										Polygon->PoPa.IsInToLeft = true                  ;

										// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
										PolyP1[k].x = P1.x;  PolyP1[k].y = P1.y;
										PolyP2[k].x = P2.x;  PolyP2[k].y = P2.y;
										PolyP3[k].x = P3.x;  PolyP3[k].y = P3.y;
										PolyP4[k].x = P4.x;  PolyP4[k].y = P4.y;

										if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
											Polygon->OnExecute(P1,P2,P3,P4);
										}

										// 2���̻� ���� � ������ ����� �з� ���� 
										if(Polygon->BlobCount>1){
											// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
											PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
											PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
											for(int k=0; k<Polygon->BlobCount; k++){
												PolygonCenter.x = Polygon->Blob[k].Rect.l;
												PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
												// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
												RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
												if(RealPolyDis<MinPolyDis) {
													MinPolyDis  = RealPolyDis;
													nPolyPosCnt = k;
												}
											}
										}
										else nPolyPosCnt=0;

										// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
										////////////////////////////////////////////////////////////////////
										if(Polygon->Blob[nPolyPosCnt].InCnt>0){
											FrameLeftInEdgeCount=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

												if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
													if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
												}
												else if(FShape==shRectC){
													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
												}
												FrameLeftInEdgeCount++;
											}

											if(FrameLeftInEdgeCount>0){
												if(FrameLeftInPoint!=NULL) delete[] FrameLeftInPoint;
												FrameLeftInPoint = new TFPoint[FrameLeftInEdgeCount];

												PixelCnt=0;
												for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
													if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
													if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

													if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
														if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
													}
													else if(FShape==shRectC){
														if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
													}

													FrameLeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
													FrameLeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
													PixelCnt++;
												}
											}
										}

										if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
											FrameLeftOutEdgeCount=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

												if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
												}
												FrameLeftOutEdgeCount++;
											}

											if(FrameLeftOutEdgeCount>0){
												if(FrameLeftOutPoint!=NULL) delete[] FrameLeftOutPoint;
												FrameLeftOutPoint = new TFPoint[FrameLeftOutEdgeCount];

												PixelCnt=0;
												for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
													if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
													if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

													if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
														if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
													}
													FrameLeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
													FrameLeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
													PixelCnt++;
												}
											}
										}
										////////////////////////////////////////////////////////////////////

										// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
										if(FrameLeftInEdgeCount>0 && FrameLeftOutEdgeCount>0){
											// Total Left In Data 
											for(int k=0; k<FrameLeftInEdgeCount; k++){
												if(AllLeftInEdgeCount>MAX_ALL_EDGE_COUNT) return false                 ;
												AllLeftInData[AllLeftInEdgeCount].x = FrameLeftInPoint[k].x            ;
												AllLeftInData[AllLeftInEdgeCount].y = FrameLeftInPoint[k].y+ImageHeight;
												AllLeftInEdgeCount++;
											}
											// Total Left Out Data 
											for(int k=0; k<FrameLeftOutEdgeCount; k++){
												if(AllLeftOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                   ;
												AllLeftOutData[AllLeftOutEdgeCount].x = FrameLeftOutPoint[k].x            ;
												AllLeftOutData[AllLeftOutEdgeCount].y = FrameLeftOutPoint[k].y+ImageHeight;
												AllLeftOutEdgeCount++;
											}
										}
									}

									// Frame�� ROI�������� ��Ȱ�˻� ���� �� Frame�������� �ٽ� ������ �Ѱ���
									// Dark_Defect/Bright_Defect�˻� ���� 
									if((AllLeftInEdgeCount-StaLeftInEdgeCount)>0 && (AllLeftOutEdgeCount-StaLeftOutEdgeCount)>0){
										// LeftInEdgeData ���� ���� 
										LeftInEdgeCount = (AllLeftInEdgeCount-StaLeftInEdgeCount);
										if(LeftInPoint!=NULL) delete[] LeftInPoint;
										LeftInPoint = new TFPoint[LeftInEdgeCount];

										// LeftOutEdgeData ���� ���� 
										LeftOutEdgeCount = (AllLeftOutEdgeCount-StaLeftOutEdgeCount);
										if(LeftOutPoint!=NULL) delete[] LeftOutPoint;
										LeftOutPoint = new TFPoint[LeftOutEdgeCount];

										// ��ü �����Ϳ��� Ư������ �����͸� �Ѱ��� 
										for(int k=0; k<LeftInEdgeCount; k++){
											LeftInPoint[k].x   = AllLeftInData[StaLeftInEdgeCount+k].x             ;
											LeftInPoint[k].y   = AllLeftInData[StaLeftInEdgeCount+k].y -ImageHeight;
										}

										// ��ü �����Ϳ��� Ư������ �����͸� �Ѱ��� 
										for(int k=0; k<LeftOutEdgeCount; k++){
											LeftOutPoint[k].x   = AllLeftOutData[StaLeftOutEdgeCount+k].x             ;
											LeftOutPoint[k].y   = AllLeftOutData[StaLeftOutEdgeCount+k].y -ImageHeight;
										}
									}
								}
								else                { // GrindMark�������� ���� 
									EasyImage::Threshold(GImage,&BinaryImg, LeftCutThresHold);
									// Median Filter ���� 
									if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
									if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) return false;
									if((P1.x+(P2.x - P1.x))>=FrameImgW    || (P1.y+(P3.y - P1.y))>FrameImgH                      ) return false;

									FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
									FilterROI.Attach(&BinaryImg)                                          ;
									FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
									LogUnit.SetLog(L"AllGrind_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
									//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
									EasyImage::Median(&FilterROI, &FilterROI)                                 ;
									// 								if(FShape==shRectH) BinaryImg.Save("D:\\ROTATE_IMG\\Polygon_Left_CUTLine.bmp");

									Polygon->GImage = &BinaryImg                     ;
									Polygon->IWidth = FrameImgW                      ;
									Polygon->IHeight= FrameImgH                      ;
									Polygon->PoPa.FMinWidth  =2                      ;                       
									Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
									Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
									Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
									Polygon->PoPa.MinPxCnt   =100                    ;
									Polygon->PoPa.MaxPxCnt   =20000                  ;
									Polygon->PoPa.MaxBbCnt   =10                     ;
									Polygon->PoPa.IsSeparate =true                   ;
									Polygon->PoPa.IsInToOut  =false                  ;
									Polygon->PoPa.HiThHold   = LeftCutThresHold      ;
									Polygon->PoPa.LoThHold   = 0                     ;
									Polygon->PoPa.IgInSide   = true                  ;
									Polygon->PoPa.IsHorSort  = false                 ;
									Polygon->PoPa.IsVerSort  = true                  ;
									Polygon->PoPa.IsInToLeft = true                  ;

									// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
									PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
									PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
									PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
									PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

									if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
										Polygon->OnExecute(P1,P2,P3,P4);
									}

									// 2���̻� ���� � ������ ����� �з� ���� 
									if(Polygon->BlobCount>1){
										// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
										PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
										PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
										for(int k=0; k<Polygon->BlobCount; k++){
											PolygonCenter.x = Polygon->Blob[k].Rect.l;
											PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
											// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
											RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
											if(RealPolyDis<MinPolyDis) {
												MinPolyDis  = RealPolyDis;
												nPolyPosCnt = k;
											}
										}
									}
									else nPolyPosCnt=0;

									// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
									////////////////////////////////////////////////////////////////////
									if(Polygon->Blob[nPolyPosCnt].InCnt>0){
										LeftInEdgeCount=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

											if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
											}
											else if(FShape==shRectC){
												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
											}
											LeftInEdgeCount++;
										}

										if(LeftInEdgeCount>0){
											if(LeftInPoint!=NULL) delete[] LeftInPoint;
											LeftInPoint = new TFPoint[LeftInEdgeCount];

											PixelCnt=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

												if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
													if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
												}
												else if(FShape==shRectC){
													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
												}
												LeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
												LeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
												PixelCnt++;
											}
										}
									}

									if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
										LeftOutEdgeCount=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

											if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
											}
											LeftOutEdgeCount++;
										}

										if(LeftOutEdgeCount>0){
											if(LeftOutPoint!=NULL) delete[] LeftOutPoint;
											LeftOutPoint = new TFPoint[LeftOutEdgeCount];

											PixelCnt=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

												if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
												}
												LeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
												LeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
												PixelCnt++;
											}
										}
									}
									////////////////////////////////////////////////////////////////////

									// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
									if(LeftInEdgeCount>0 && LeftOutEdgeCount>0){
										// Total Left In Data 
										for(int k=0; k<LeftInEdgeCount; k++){
											if(AllLeftInEdgeCount>MAX_ALL_EDGE_COUNT) return false            ;
											AllLeftInData[AllLeftInEdgeCount].x = LeftInPoint[k].x            ;
											AllLeftInData[AllLeftInEdgeCount].y = LeftInPoint[k].y+ImageHeight;
											AllLeftInEdgeCount++;
										}
										// Total Left Out Data 
										for(int k=0; k<LeftOutEdgeCount; k++){
											if(AllLeftOutEdgeCount>MAX_ALL_EDGE_COUNT) return false              ;
											AllLeftOutData[AllLeftOutEdgeCount].x = LeftOutPoint[k].x            ;
											AllLeftOutData[AllLeftOutEdgeCount].y = LeftOutPoint[k].y+ImageHeight;
											AllLeftOutEdgeCount++;
										}
									}
								}
							}
						}
					}
					else          { //�κп��� ������ 
						SingleLine->SetOrientation(orWest);
						SingleLine->SetFindLineId(0);
						// 					SingleLine->SetEdgeType(0); // White to Black 
						if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
						else                SingleLine->SetEdgeType(1);  // White To Black 

						SingleLine->OnExecute();
						Line[0].FP0 = SingleLine->GetResultP0();
						Line[0].FP1 = SingleLine->GetResultP1();

						//�������� ���� Line �����Ұ�� Line_Fitting ��ǥ ������ 
						if(ROI_H<400){
							if(fabs(Line[0].FP0.x- Line[0].FP1.x)>3){
								if(Line[0].FP0.x>Line[0].FP1.x) Line[0].FP0.x = Line[0].FP1.x;
								if(Line[0].FP0.x<Line[0].FP1.x) Line[0].FP1.x = Line[0].FP0.x;
							}
						}

						// Btm,Top ������� ù��°
						LeftOutLine[0].FP0.x = Line[0].FP0.x;
						LeftOutLine[0].FP0.y = Line[0].FP0.y;
						LeftOutLine[0].FP1.x = Line[0].FP1.x;
						LeftOutLine[0].FP1.y = Line[0].FP1.y;
					}
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
			}
			if (FRightWall) {  //�ű� ������Ʈ(LineScan4K X 8) ==> Cell �����ڸ��� �˻��ϹǷ� ����� �ʿ����  

			}
			if (FLeftTop    ){ //�ű� ������Ʈ(LineScan4K X 8) ==> ����/���� �з� �˻� ������   
				FLTPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
				EasyImage::Threshold(GImage,&BinaryImg, ECThresHold_Sta);
				BuffLTPoint.x = FLTPoint.x             ;
				BuffLTPoint.y = FLTPoint.y +ImageHeight;

				if(IsShortCell){ //�����ϰ��(A1-->A2)_S4 or (C1-->C2)_S2_Dark_Bright�ҷ� ���� 
					if(!IsAllGrind){
						// Chamber_Cut���� ������ GrindMark��뿩�� üũ
						for(int k=0; k<4; k++) {
							if(GrindMark[k].DvX!=0 && GrindMark[k].DvY!=0){IsGrindMarkUse = true; break;}
						}

						if(IsRectTop){
							LTDx = 0.0f;
							LTDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FLTLPoint.x = FLTPoint.x;
							FLTLPoint.y = FLTPoint.y;
							FLTTPoint.x = FLTPoint.x;
							FLTTPoint.y = FLTPoint.y;
						}
						else if(IsCornerTop){
							if(ECThresHold_Sta==0) {SingleLine->GImage = GImage    ; SingleLine->FThreshold =5 ;}
							else                   {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

							if(LTCornerWidth!=0 && LTCornerHeight!=0){ //Corner Dx,Dy ������ �˻� ����
								if(!IsGrindMarkUse){ //3Point ���� ����,������ ���� ������ 
									// Corner ���� �ܰ����� 
									SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x-10                    );
									SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y                       );
									SingleLine->IRoi->SetWidth ((int)LTCornerWidth+20                 );
									//								SingleLine->IRoi->SetWidth ((int)LTCornerWidth+LTCornerWidthOffset);
									SingleLine->IRoi->SetHeight((int)LTCornerHeight                   );

									SingleLine->SetOrientation(orWest);
									SingleLine->FIncrement =1         ;
									SingleLine->FThreshold =15        ;
									SingleLine->SetFindLineId(0)      ;
									SingleLine->SetEdgeType(1)        ; //White To Black 
									SingleLine->OnExecute()           ;

									Line[4].FP0 = SingleLine->GetResultP0();
									Line[4].FP1 = SingleLine->GetResultP1();

									// Corner Chamber_Cut ������ 
									FLTLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
									FLTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

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
											if(LTCorPoint!=NULL) delete[] LTCorPoint;
											LTCorPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
											LTCorEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

											CornerCount =0;  
											for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
												if(CornerCount>=MAX_CORNER_POINT) break;
												LTCorPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
												LTCorPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
												CornerCount++;
											}
										}

										double Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
										// Corner Grind In Gray Level ������ ���ؼ� Corner Grind Out/In ThresHold���� �з��ϴ� 
										if(CorGrindInThresHold_Sta!=0){ // Corner InLine Search Sequence 
											EasyImage::Threshold(GImage,&BinaryImg, CorGrindInThresHold_Sta);
											SingleLine->GImage = &BinaryImg;  SingleLine->FThreshold =10;

											// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
											// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
											if(LTCornerWidth >(LTCornerHeight*2)){
												SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x );
												SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y );
												SingleLine->IRoi->SetWidth ((int)((FLTTPoint.x - FLTLPoint.x+5 )));
												SingleLine->IRoi->SetHeight((int)((FLTLPoint.y - FLTTPoint.y+10)));

												if(FLTPoint.x<=0    || FLTPoint.y<=0    || (FLTTPoint.x - FLTLPoint.x)<=0    || (FLTLPoint.y - FLTTPoint.y)<=0   ) return false;
												if(FLTPoint.x>=FrameImgW || FLTPoint.y>=FrameImgH || (FLTTPoint.x - FLTLPoint.x)>=FrameImgW || (FLTLPoint.y - FLTTPoint.y)>=FrameImgH) return false;

												SingleLine->SetOrientation(orSouth);
												SingleLine->FIncrement =2;
												SingleLine->SetFindLineId(0);
												SingleLine->SetEdgeType(1);   //White To Black 
												SingleLine->OnExecute();

												// InLine C1Corner Data 
												if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
													if(LTCorInPoint!=NULL) delete[] LTCorInPoint;
													LTCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
													LTCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

													CornerCount =0;  
													for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
														if(CornerCount>=MAX_CORNER_POINT) break;
														LTCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														LTCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
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
													if(LTCorInPoint!=NULL) delete[] LTCorInPoint;
													LTCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
													LTCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

													CornerCount =0;  
													for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
														if(CornerCount>=MAX_CORNER_POINT) break;
														LTCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														LTCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
														CornerCount++;
													}
												}
											}
											//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
											FLTLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
											FLTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

											//�ڳ� ������ 
											Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
											for(int k=0; k<LTCorInEdgeCount; k++){
												Dis = Math.GetLengthFrLineToPoint(FLTLPoint, FLTTPoint, LTCorInPoint[k]);
												TotalGrindWidth+=Dis;
											}
											if(LTCorInEdgeCount!=0){
												AvgGrindWidth = (TotalGrindWidth/LTCorInEdgeCount);
												LTCorGrindWd  = AvgGrindWidth;
											}
										}

										// ������ ���� 
										if(CorGrindMeaThresHold!=0){ // Corner Grind ThresHold���� �����Ǿ� ������츸 ������ �˻縦 �����Ѵ� 
											// �ڳ� ������ ���� ������ ã�´� 
											// ROI ���� ����(3���� ��� ���� ���� ������ ã�´�)
											EasyImage::Threshold(GImage,&BinaryImg, CorGrindMeaThresHold);
											SingleLine->GImage = &BinaryImg;  SingleLine->FThreshold =10;

//											GImage->Save("D:\\ROTATE_IMG\\����_CornerGrindMeaTest_����.bmp");

											// ���� ������....�ϴ� ������ �ޱ������� �ӽ÷� ������ �з��Ͽ� CornerEdgePoint ã�� 
											if(LTCornerWidth >(LTCornerHeight*1.5)){
												SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x+LTCornerWidthOffset);
												SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y+LTCornerHeightOffset );
												SingleLine->IRoi->SetWidth ((int)LTCornerWidth );
												SingleLine->IRoi->SetHeight((int)LTCornerHeight);
												// 											SingleLine->IRoi->SetWidth (LTCornerWidth -50     ); HSKIM
												// 											SingleLine->IRoi->SetHeight((int)LTCornerHeight-40);

												SingleLine->SetOrientation(orNorth);
												SingleLine->SetFindLineId(0);
												SingleLine->FIncrement =1;
												SingleLine->SetEdgeType(0);  //Black To White 
												SingleLine->OnExecute();

												// C1Corner Grind Measure Stand Data 
												if((SingleLine->FEdgePointCount)>0){
													if(LTCorMeaPoint!=NULL) delete[] LTCorMeaPoint;
													LTCorMeaPoint     = new TFPoint [(SingleLine->FEdgePointCount)];
													LTCorMeaEdgeCount = (SingleLine->FEdgePointCount);

													CornerCount =0;  
													for(int k=0; k<(SingleLine->FEdgePointCount); k++){
														if(CornerCount>=MAX_CORNER_POINT) break;
														if(k==0                  ) P1 = SingleLine->FEdgePoint[k];
														if(k==LTCorMeaEdgeCount-1) P2 = SingleLine->FEdgePoint[k];
														LTCorMeaPoint[CornerCount].x  = SingleLine->FEdgePoint[k].x;
														LTCorMeaPoint[CornerCount].y  = SingleLine->FEdgePoint[k].y;
														CornerCount++;
													}
												}

												FLTLMeaPoint = Math.OnIntersect(P1, P2, Line[waLeftWall].FP0, Line[waLeftWall].FP1);
												FLTTMeaPoint = Math.OnIntersect(P1, P2, Line[waTopWall ].FP0, Line[waTopWall ].FP1);


												//�ڳ� ������ ���  
												Dis =0.0f, AvgGrindMea=0.0f, TotalGrindMeasure=0.0f;
												for(int k=0; k<LTCorEdgeCount; k++){
													Dis = Math.GetLengthFrLineToPoint(FLTLMeaPoint, FLTTMeaPoint, LTCorPoint[k]);
													TotalGrindMeasure+=Dis;
												}
												if(LTCorEdgeCount!=0){
													AvgGrindMea    = (TotalGrindMeasure/LTCorEdgeCount);
													LTCorGrindMea  = AvgGrindMea;
												}
											}
											else {
												SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x+LTCornerWidthOffset);
												SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y+LTCornerHeightOffset );
												SingleLine->IRoi->SetWidth ((int)LTCornerWidth );
												SingleLine->IRoi->SetHeight((int)LTCornerHeight);
//												SingleLine->IRoi->SetWidth (LTCornerWidth -LTCornerWidthOffset      );
//												SingleLine->IRoi->SetHeight((int)LTCornerHeight-LTCornerHeightOffset);

												SingleLine->SetOrientation(orWest);
												SingleLine->SetFindLineId(0);
												SingleLine->FIncrement =1;
												SingleLine->SetEdgeType(0);  //Black To White 
												SingleLine->OnExecute();

												P1 = SingleLine->GetResultP0();
												P2 = SingleLine->GetResultP1();

												//�ڳ� ������ ���� ���ΰ� ���ο� ������ ���� 
												FLTLMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
												FLTTMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

												// C1Corner Grind Measure Stand Data 
												if((SingleLine->FEdgePointCount)>0){
													if(LTCorMeaPoint!=NULL) delete[] LTCorMeaPoint;
													LTCorMeaPoint     = new TFPoint [(SingleLine->FEdgePointCount)];
													LTCorMeaEdgeCount = (SingleLine->FEdgePointCount);

													CornerCount =0;  
													for(int k=0; k<(SingleLine->FEdgePointCount); k++){
														if(CornerCount>=MAX_CORNER_POINT) break;
														LTCorMeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														LTCorMeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
														CornerCount++;
													}
												}
												//�ڳ� ������ ���  
												Dis =0.0f, AvgGrindMea=0.0f, TotalGrindMeasure=0.0f;
												for(int k=0; k<LTCorEdgeCount; k++){
													Dis = Math.GetLengthFrLineToPoint(FLTLMeaPoint, FLTTMeaPoint, LTCorPoint[k]);
													TotalGrindMeasure+=Dis;
												}
												if(LTCorEdgeCount!=0){
													AvgGrindMea    = (TotalGrindMeasure/LTCorEdgeCount);
													LTCorGrindMea  = AvgGrindMea;
												}
											}
										}
									}
								}
								else                                                { // GrindMark ���� ����,������ ���� ������ 
									// Corner ���� �ܰ����� 
									SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x-10                    );
									SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y                       );
									SingleLine->IRoi->SetHeight((int)LTCornerHeight                   );
									//								SingleLine->IRoi->SetWidth ((int)LTCornerWidth+LTCornerWidthOffset);
									SingleLine->IRoi->SetWidth ((int)LTCornerWidth+20);

									SingleLine->SetOrientation(orWest);
									SingleLine->FIncrement =1;
									SingleLine->FThreshold =15;
									SingleLine->SetFindLineId(0);
									SingleLine->SetEdgeType(1);  //White To Black 
									SingleLine->OnExecute();

									Line[4].FP0 = SingleLine->GetResultP0();
									Line[4].FP1 = SingleLine->GetResultP1();

									// Corner Chamber_Cut ������ 
									FLTLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
									FLTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

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
											if(LTCorPoint!=NULL) delete[] LTCorPoint;
											LTCorPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
											LTCorEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

											CornerCount =0;  
											for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
												if(CornerCount>=MAX_CORNER_POINT) break;
												LTCorPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
												LTCorPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
												CornerCount++;
											}
										}

										double Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
										// Corner Grind In Gray Level ������ ���ؼ� Corner Grind Out/In ThresHold���� �з��ϴ� 
										if(CorGrindInThresHold_Sta!=0){ // Corner InLine Search Sequence 
											EasyImage::Threshold(GImage,&BinaryImg, CorGrindInThresHold_Sta);
											SingleLine->GImage = &BinaryImg;  SingleLine->FThreshold =10;

											// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
											// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
											if(LTCornerWidth >(LTCornerHeight*2)){
												SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x );
												SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y );
												SingleLine->IRoi->SetWidth ((int)((FLTTPoint.x - FLTLPoint.x+5 )));
												SingleLine->IRoi->SetHeight((int)((FLTLPoint.y - FLTTPoint.y+10)));

												if(FLTPoint.x<=0    || FLTPoint.y<=0    || (FLTTPoint.x - FLTLPoint.x)<=0    || (FLTLPoint.y - FLTTPoint.y)<=0   ) return false;
												if(FLTPoint.x>=FrameImgW || FLTPoint.y>=FrameImgH || (FLTTPoint.x - FLTLPoint.x)>=FrameImgW || (FLTLPoint.y - FLTTPoint.y)>=FrameImgH) return false;

												SingleLine->SetOrientation(orSouth);
												SingleLine->FIncrement =1;
												SingleLine->SetFindLineId(0);
												SingleLine->SetEdgeType(1);   //White To Black 
												SingleLine->OnExecute();

												// InLine C1Corner Data 
												if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
													if(LTCorInPoint!=NULL) delete[] LTCorInPoint;
													LTCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
													LTCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

													CornerCount =0;  
													// 												for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
													for(int k=((SingleLine->FEdgePointCount)-CornerMargin-1); k>(CornerMargin-1); k--){
														if(CornerCount>=MAX_CORNER_POINT) break;
														LTCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														LTCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
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
													if(LTCorInPoint!=NULL) delete[] LTCorInPoint;
													LTCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
													LTCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

													CornerCount =0;  
													for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
														if(CornerCount>=MAX_CORNER_POINT) break;
														LTCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														LTCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
														CornerCount++;
													}
												}
											}
											//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
											FLTLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
											FLTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

											// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
											if(LTCorInEdgeCount>0 && LTCorEdgeCount>0){
												// Total Corner In Data 
												for(int k=0; k<LTCorInEdgeCount; k++){
													if(AllCorInEdgeCount>MAX_ALL_EDGE_COUNT) return false               ;
													AllCornerInData[AllCorInEdgeCount].x = LTCorInPoint[k].x            ;
													AllCornerInData[AllCorInEdgeCount].y = LTCorInPoint[k].y+ImageHeight;
													AllCorInEdgeCount++;
												}
												// Total Corner Out Data 
												for(int k=0; k<LTCorEdgeCount; k++){
													if(AllCorOutEdgeCount>MAX_ALL_EDGE_COUNT) return false              ;
													AllCornerOutData[AllCorOutEdgeCount].x = LTCorPoint[k].x            ;
													AllCornerOutData[AllCorOutEdgeCount].y = LTCorPoint[k].y+ImageHeight;
													AllCorOutEdgeCount++;
												}
											}
										}
									}
								}
							}
						}
						else if(IsCircleTop){
							if(LTSideCircleWidth!=0.0f && LTSideCircleHeight!=0.0f){
								TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
								if(LeftCutThresHold==0){ //ASide Pad�ƴҰ��, �� ���Ǻκ��� ��� 1��ThresHold�� �������� ���� �����Ұ��  
									P1.x = (int)(FLTPoint.x-20);   P1.y =(int)(FLTPoint.y-(FLTPoint.y/2)  );P2.x = (int)(P1.x+LTSideCircleWidth+20);    P2.y = (int)(P1.y);
									P3.x = (int)(FLTPoint.x-20);   P3.y =(int)(P1.y+LTSideCircleHeight+20 );P4.x = (int)(P2.x                     );    P4.y = (int)(P3.y);

									// �˻�ȭ�� Display ���� 
									ASideRect[0].left  = P1.x;   ASideRect[0].top    = P1.y;
									ASideRect[0].right = P2.x;   ASideRect[0].bottom = P3.y;

									int PixelCnt =0;

									// Median Filter ���� 
									if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
									if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)>=FrameImgH) return false;
									if((P1.x+(P2.x - P1.x))>=FrameImgW || (P1.y+(P3.y - P1.y))>FrameImgH                ) return false;

									FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
									FilterROI.Attach(&BinaryImg)                                               ;
									FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y))     ;
									LogUnit.SetLog(L"LeftTop_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
									// 							LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
									EasyImage::Median(&FilterROI, &FilterROI)                                  ;
//									RotateBinaryImg.Save("D:\\ROTATE_IMG\\Polygon_LeftTop_����.bmp");

									Polygon->GImage = &BinaryImg                     ;
									Polygon->IWidth = FrameImgW                           ;
									Polygon->IHeight= FrameImgH                           ;
									Polygon->PoPa.FMinWidth  =5                      ;                       
									Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
									Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
									Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
									Polygon->PoPa.MinPxCnt   =100                    ;
									Polygon->PoPa.MaxPxCnt   =20000                  ;
									Polygon->PoPa.MaxBbCnt   =10                     ;
									Polygon->PoPa.IsSeparate =true                   ;
									Polygon->PoPa.IsInToOut  =false                  ;
									Polygon->PoPa.HiThHold   = ECThresHold_Sta       ;
									Polygon->PoPa.LoThHold   = 0                     ;
									// 							Polygon->PoPa.IgInSide   = true                  ;
									Polygon->PoPa.IgInSide   = false                 ;
									Polygon->PoPa.IsHorSort  = false                 ;
									Polygon->PoPa.IsVerSort  = true                  ;
									Polygon->PoPa.IsInToLeft = true                  ;

									// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
									PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
									PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
									PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
									PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

									if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
										Polygon->OnExecute(P1,P2,P3,P4);
									}

									// 2���̻� ���� � ������ ����� �з� ���� 
									if(Polygon->BlobCount>1){
										// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
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

									// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
									////////////////////////////////////////////////////////////////////
									if(Polygon->Blob[nPolyPosCnt].InCnt>0){
										LTCirInEdgeCount=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											// Hap�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
											if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
											LTCirInEdgeCount++;
										}

										if(LTCirInEdgeCount>0){
											if(LTCirInPoint!=NULL) delete[] LTCirInPoint;
											LTCirInPoint = new TFPoint[LTCirInEdgeCount];

											PixelCnt=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												// Hap�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

												LTCirInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
												LTCirInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
												PixelCnt++;
											}
										}
									}

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

									// 							// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
									if(LTCirInEdgeCount>0 && LTCirOutEdgeCount>0){
										// Total Corner In Data 
										for(int k=0; k<LTCirInEdgeCount; k++){
											if(AllCirInEdgeCount>MAX_ALL_EDGE_COUNT) return false               ;
											AllCircleInData[AllCirInEdgeCount].x = LTCirInPoint[k].x            ;
											AllCircleInData[AllCirInEdgeCount].y = LTCirInPoint[k].y+ImageHeight;
											AllCirInEdgeCount++;
										}
										// Total Corner Out Data 
										for(int k=0; k<LTCirOutEdgeCount; k++){
											if(AllCirOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                 ;
											AllCircleOutData[AllCirOutEdgeCount].x = LTCirOutPoint[k].x            ;
											AllCircleOutData[AllCirOutEdgeCount].y = LTCirOutPoint[k].y+ImageHeight;
											AllCirOutEdgeCount++;
										}
									}
								}
								else                   { //ASide Pad, ���ǰ� ������ �����ؾ��� ���(FLTPoint)
									IsFrameGrindMark = false                ;
									RealFrameStaY    = ImageHeight          ; //��üBuff���� Frame_Real��ǥ 
									RealFrameEndY    = ImageHeight+FrameImgH; //��üBuff���� Frame_Real��ǥ 

									P1.x = (int)(FLTPoint.x-20);   P1.y =(int)(FLTPoint.y-(FLTPoint.y/2)  );P2.x = (int)(P1.x+LTSideCircleWidth+20);    P2.y = (int)(P1.y);
									P3.x = (int)(FLTPoint.x-20);   P3.y =(int)(P1.y+LTSideCircleHeight+20); P4.x = (int)(P2.x                     );    P4.y = (int)(P3.y);

									// 							for(int k=0; k<2; k++){GrindStepy[k].x=0; GrindStepy[k].y=0;}
									for(int k=0; k<3; k++){
										GrindLine[k].LinePoint.x =0;
										GrindLine[k].LinePoint.y =0;
										GrindLine[k].IsGrindLine =false;
									}

									// ��üBuffer���� �˻� Frame�� GrindMark�����ϴ��� Ȯ�� �� üũ 
									for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
										if(GrindMark[k].DvX==0 || GrindMark[k].DvY==0) continue;
										if(((BuffLTPoint.y + GrindMark[k].DvY)>= RealFrameStaY) && ((BuffLTPoint.y + GrindMark[k].DvY)<= RealFrameEndY)){
											IsFrameGrindMark = true;

											//��ü Buffer���� ��ǥ 
											if((BuffLTPoint.y + GrindMark[k].DvY)<CutMinPos) CutMinPos =(BuffLTPoint.y + GrindMark[k].DvY);
											if((BuffLTPoint.y + GrindMark[k].DvY)>CutMaxPos) CutMaxPos =(BuffLTPoint.y + GrindMark[k].DvY); 
										}
									}

									// Frame���� ��ǥ�� ��ȯ 
									if(CutMinPos>0) CutMinPos = CutMinPos - ImageHeight;
									if(CutMaxPos>0) CutMaxPos = CutMaxPos - ImageHeight;

									// ��üBuffer���� ���� Frame�� GrindMark�� �����ϸ� ������ �и��Ͽ� �˻� ������ 
									if(IsFrameGrindMark){
										RoiCount =2;
										GrindLine[0].LinePoint.y = 0           ;
										GrindLine[0].IsGrindLine = true        ;
										GrindLine[1].LinePoint.y = CutMaxPos+30;
										GrindLine[1].IsGrindLine = false       ;

										for(int k=0; k<RoiCount; k++){
											GrindThresHold =0;
											if(k==(RoiCount-1)){
												P1.y =GrindLine[k].LinePoint.y; P2.y = (int)(P1.y);
												P3.y =(int)(FrameImgH      )  ; P4.y = (int)(P3.y);
											}
											else               {
												P1.y =GrindLine[k  ].LinePoint.y; P2.y = (int)(P1.y);
												P3.y =GrindLine[k+1].LinePoint.y; P4.y = (int)(P3.y);
											}
											GrindThresHold = (GrindLine[k].IsGrindLine) ? ECThresHold_Sta:LeftCutThresHold;

											// 1�� Frame_Buff�� GrindMark Position���� ��Ȱ�Ͽ� ROI���������� �ʿ��� 
											if(k==0){StaLeftInEdgeCount=AllCirInEdgeCount; StaLeftOutEdgeCount=AllCirOutEdgeCount; }
											// GrindMark���� ���� ��Ȱ�� ������ �ʹ� ������� Polygon_Data �������� ���� 
											if((P3.y - P1.y)<15) continue;

											EasyImage::Threshold(GImage,&BinaryImg, GrindThresHold);
											// Median Filter ���� 
											if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
											if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) return false;
											if((P1.x+(P2.x - P1.x))>=FrameImgW    || (P1.y+(P3.y - P1.y))>FrameImgH                      ) return false;

											FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
											FilterROI.Attach(&BinaryImg)                                          ;
											FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
											LogUnit.SetLog(L"AllGrind_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
											//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
											EasyImage::Median(&FilterROI, &FilterROI)                             ;

											Polygon->GImage = &BinaryImg                     ;
											Polygon->IWidth = FrameImgW                      ;
											Polygon->IHeight= FrameImgH                      ;
											Polygon->PoPa.FMinWidth  =2                      ;                       
											Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
											Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
											Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
											// 									Polygon->PoPa.MinPxCnt   =100                    ;
											Polygon->PoPa.MinPxCnt   =50                     ;
											Polygon->PoPa.MaxPxCnt   =20000                  ;
											Polygon->PoPa.MaxBbCnt   =10                     ;
											Polygon->PoPa.IsSeparate =true                   ;
											Polygon->PoPa.IsInToOut  =false                  ;
											Polygon->PoPa.HiThHold   = GrindThresHold        ;
											Polygon->PoPa.LoThHold   = 0                     ;
											Polygon->PoPa.IgInSide   = true                  ;
											Polygon->PoPa.IsHorSort  = false                 ;
											Polygon->PoPa.IsVerSort  = true                  ;
											Polygon->PoPa.IsInToLeft = true                  ;

											// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
											PolyP1[k].x = P1.x;  PolyP1[k].y = P1.y;
											PolyP2[k].x = P2.x;  PolyP2[k].y = P2.y;
											PolyP3[k].x = P3.x;  PolyP3[k].y = P3.y;
											PolyP4[k].x = P4.x;  PolyP4[k].y = P4.y;

											if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
												Polygon->OnExecute(P1,P2,P3,P4);
											}

											// 2���̻� ���� � ������ ����� �з� ���� 
											if(Polygon->BlobCount>1){
												// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
												PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
												PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
												for(int k=0; k<Polygon->BlobCount; k++){
													PolygonCenter.x = Polygon->Blob[k].Rect.l;
													PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
													// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
													RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
													if(RealPolyDis<MinPolyDis) {
														MinPolyDis  = RealPolyDis;
														nPolyPosCnt = k;
													}
												}
											}
											else nPolyPosCnt=0;

											// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
											////////////////////////////////////////////////////////////////////
											if(Polygon->Blob[nPolyPosCnt].InCnt>0){
												FrameLeftInEdgeCount=0;
												for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
													if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
													if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

													if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
													FrameLeftInEdgeCount++;
												}

												if(FrameLeftInEdgeCount>0){
													if(FrameLeftInPoint!=NULL) delete[] FrameLeftInPoint;
													FrameLeftInPoint = new TFPoint[FrameLeftInEdgeCount];

													PixelCnt=0;
													for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
														if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
														if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

														if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

														FrameLeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
														FrameLeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
														PixelCnt++;
													}
												}
											}

											if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
												FrameLeftOutEdgeCount=0;
												for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
													if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
													if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

													FrameLeftOutEdgeCount++;
												}

												if(FrameLeftOutEdgeCount>0){
													if(FrameLeftOutPoint!=NULL) delete[] FrameLeftOutPoint;
													FrameLeftOutPoint = new TFPoint[FrameLeftOutEdgeCount];

													PixelCnt=0;
													for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
														if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
														if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

														FrameLeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
														FrameLeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
														PixelCnt++;
													}
												}
											}
											////////////////////////////////////////////////////////////////////

											// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
											if(FrameLeftInEdgeCount>0 && FrameLeftOutEdgeCount>0){
												// Total Left In Data 
												for(int k=0; k<FrameLeftInEdgeCount; k++){
													if(AllCirInEdgeCount>MAX_ALL_EDGE_COUNT) return false                   ;
													AllCircleInData[AllCirInEdgeCount].x = FrameLeftInPoint[k].x            ;
													AllCircleInData[AllCirInEdgeCount].y = FrameLeftInPoint[k].y+ImageHeight;
													AllCirInEdgeCount++;
												}
												// Total Left Out Data 
												for(int k=0; k<FrameLeftOutEdgeCount; k++){
													if(AllCirOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                     ;
													AllCircleOutData[AllCirOutEdgeCount].x = FrameLeftOutPoint[k].x            ;
													AllCircleOutData[AllCirOutEdgeCount].y = FrameLeftOutPoint[k].y+ImageHeight;
													AllCirOutEdgeCount++;
												}
											}
										}

										// Frame�� ROI�������� ��Ȱ�˻� ���� �� Frame�������� �ٽ� ������ �Ѱ���
										// Dark_Defect/Bright_Defect�˻� ���� 
										if((AllCirInEdgeCount-StaLeftInEdgeCount)>0 && (AllCirOutEdgeCount-StaLeftOutEdgeCount)>0){
											// LeftInEdgeData ���� ���� 
											LTCirInEdgeCount = (AllCirInEdgeCount-StaLeftInEdgeCount);
											if(LTCirInPoint!=NULL) delete[] LTCirInPoint;
											LTCirInPoint = new TFPoint[LTCirInEdgeCount];

											// LeftOutEdgeData ���� ���� 
											LTCirOutEdgeCount = (AllCirOutEdgeCount-StaLeftOutEdgeCount);
											if(LTCirOutPoint!=NULL) delete[] LTCirOutPoint;
											LTCirOutPoint = new TFPoint[LTCirOutEdgeCount];

											// ��ü �����Ϳ��� Ư������ �����͸� �Ѱ��� 
											for(int k=0; k<LTCirInEdgeCount; k++){
												LTCirInPoint[k].x   = AllCircleInData[StaLeftInEdgeCount+k].x             ;
												LTCirInPoint[k].y   = AllCircleInData[StaLeftInEdgeCount+k].y -ImageHeight;
											}

											// ��ü �����Ϳ��� Ư������ �����͸� �Ѱ��� 
											for(int k=0; k<LTCirOutEdgeCount; k++){
												LTCirOutPoint[k].x   = AllCircleOutData[StaLeftOutEdgeCount+k].x             ;
												LTCirOutPoint[k].y   = AllCircleOutData[StaLeftOutEdgeCount+k].y -ImageHeight;
											}
										}
									}
									else            { //���� Frame�� GrindMark�������� ���� 
										EasyImage::Threshold(GImage,&BinaryImg, LeftCutThresHold);
										P1.x = (int)(FLTPoint.x-20);   P1.y =(int)(FLTPoint.y-(FLTPoint.y/2)  );P2.x = (int)(P1.x+LTSideCircleWidth+20);    P2.y = (int)(P1.y);
										P3.x = (int)(FLTPoint.x-20);   P3.y =(int)(P1.y+LTSideCircleHeight+20); P4.x = (int)(P2.x                     );    P4.y = (int)(P3.y);

										// �˻�ȭ�� Display ���� 
										ASideRect[0].left  = P1.x;   ASideRect[0].top    = P1.y;
										ASideRect[0].right = P2.x;   ASideRect[0].bottom = P3.y;

										int PixelCnt =0;

										// Median Filter ���� 
										if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
										if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)>=FrameImgH) return false;
										if((P1.x+(P2.x - P1.x))>=FrameImgW || (P1.y+(P3.y - P1.y))>FrameImgH                ) return false;

										FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
										FilterROI.Attach(&BinaryImg)                                               ;
										FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y))     ;
										LogUnit.SetLog(L"LeftTop_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
										// 							LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
										EasyImage::Median(&FilterROI, &FilterROI)                                  ;
										//  									 RotateBinaryImg.Save("D:\\ROTATE_IMG\\Polygon_LeftTop_����.bmp");
										Polygon->GImage = &BinaryImg                     ;
										Polygon->IWidth = FrameImgW                           ;
										Polygon->IHeight= FrameImgH                           ;
										Polygon->PoPa.FMinWidth  =5                      ;                       
										Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
										Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
										Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
										Polygon->PoPa.MinPxCnt   =100                    ;
										Polygon->PoPa.MaxPxCnt   =20000                  ;
										Polygon->PoPa.MaxBbCnt   =10                     ;
										Polygon->PoPa.IsSeparate =true                   ;
										Polygon->PoPa.IsInToOut  =false                  ;
										Polygon->PoPa.HiThHold   = ECThresHold_Sta       ;
										Polygon->PoPa.LoThHold   = 0                     ;
										// 							Polygon->PoPa.IgInSide   = true                  ;
										Polygon->PoPa.IgInSide   = false                 ;
										Polygon->PoPa.IsHorSort  = false                 ;
										Polygon->PoPa.IsVerSort  = true                  ;
										Polygon->PoPa.IsInToLeft = true                  ;

										// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
										PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
										PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
										PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
										PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

										if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
											Polygon->OnExecute(P1,P2,P3,P4);
										}

										// 2���̻� ���� � ������ ����� �з� ���� 
										if(Polygon->BlobCount>1){
											// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
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

										// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
										////////////////////////////////////////////////////////////////////
										if(Polygon->Blob[nPolyPosCnt].InCnt>0){
											LTCirInEdgeCount=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												// Hap�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
												LTCirInEdgeCount++;
											}

											if(LTCirInEdgeCount>0){
												if(LTCirInPoint!=NULL) delete[] LTCirInPoint;
												LTCirInPoint = new TFPoint[LTCirInEdgeCount];

												PixelCnt=0;
												for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
													if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
													if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
													// Hap�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
													if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

													LTCirInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
													LTCirInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
													PixelCnt++;
												}
											}
										}

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

										// 							// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
										if(LTCirInEdgeCount>0 && LTCirOutEdgeCount>0){
											// Total Corner In Data 
											for(int k=0; k<LTCirInEdgeCount; k++){
												if(AllCirInEdgeCount>MAX_ALL_EDGE_COUNT) return false               ;
												AllCircleInData[AllCirInEdgeCount].x = LTCirInPoint[k].x            ;
												AllCircleInData[AllCirInEdgeCount].y = LTCirInPoint[k].y+ImageHeight;
												AllCirInEdgeCount++;
											}
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
								// FAVION Circle Fitting �����(LeftTop)
								SampleSize = LTCirInEdgeCount*0.75; //0.5~0.8���Ϸ� �Է��Ұ�
								DistDevRatio = 0.15; //0.1~0.25 ���Ϸ� �Է��Ұ�
								CircleFit->OnRansac(LTCirInPoint,LTCirInEdgeCount,SampleSize,DistDevRatio,&LTCircleFitCenter,&LTCirFitDiameter);
							}
						}
					}
				}
				else           { //�����ϰ��(A1-->C1)_S3 or (A2-->C2)_S1_Dark�ҷ��� ���� 
					if(!IsAllGrind){
						if(IsRectTop){
							LTDx = 0.0f;
							LTDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FLTLPoint.x = FLTPoint.x;
							FLTLPoint.y = FLTPoint.y;
							FLTTPoint.x = FLTPoint.x;
							FLTTPoint.y = FLTPoint.y;
						}
						else if(IsCornerTop){
							if(ECThresHold_Sta==0) {SingleLine->GImage = GImage    ; SingleLine->FThreshold =5 ;}
							else                   {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

							if(LTCornerWidth!=0 && LTCornerHeight!=0){ //Corner Dx,Dy ������ �˻� ����
								// Corner ���� �ܰ����� 
								SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x-10    );
								SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y       );
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

								// Corner Chamber_Cut ������ 
								FLTLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
								FLTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

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
										if(LTCorPoint!=NULL) delete[] LTCorPoint;
										LTCorPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										LTCorEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											LTCorPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											LTCorPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									double Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
									// Corner Grind In Gray Level ������ ���ؼ� Corner Grind Out/In ThresHold���� �з��ϴ� 
									if(CorGrindInThresHold_Sta!=0){ // Corner InLine Search Sequence 
										EasyImage::Threshold(GImage,&BinaryImg, CorGrindInThresHold_Sta);
										SingleLine->GImage = &BinaryImg;  SingleLine->FThreshold =10;

										// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
										// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
										if(LTCornerWidth >(LTCornerHeight*1.5)){
											SingleLine->IRoi->SetOrgX  ((int)FLTPoint.x );
											SingleLine->IRoi->SetOrgY  ((int)FLTPoint.y );
											SingleLine->IRoi->SetWidth ((int)((FLTTPoint.x - FLTLPoint.x+5 )));
											SingleLine->IRoi->SetHeight((int)((FLTLPoint.y - FLTTPoint.y+10)));

											if(FLTPoint.x<=0    || FLTPoint.y<=0    || (FLTTPoint.x - FLTLPoint.x)<=0    || (FLTLPoint.y - FLTTPoint.y)<=0   ) return false;
											if(FLTPoint.x>=FrameImgW || FLTPoint.y>=FrameImgH || (FLTTPoint.x - FLTLPoint.x)>=FrameImgW || (FLTLPoint.y - FLTTPoint.y)>=FrameImgH) return false;

											SingleLine->SetOrientation(orSouth);
											SingleLine->FIncrement =2;
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(1);   //White To Black 
											SingleLine->OnExecute();

											// InLine C1Corner Data 
											if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
												if(LTCorInPoint!=NULL) delete[] LTCorInPoint;
												LTCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
												LTCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

												CornerCount =0;  
												for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
													if(CornerCount>=MAX_CORNER_POINT) break;
													LTCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													LTCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
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
												if(LTCorInPoint!=NULL) delete[] LTCorInPoint;
												LTCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
												LTCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

												CornerCount =0;  
												for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
													if(CornerCount>=MAX_CORNER_POINT) break;
													LTCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													LTCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}
										//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
										FLTLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
										FLTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);
									}
								}
							}
						}
						else if(IsCircleTop){ //���� 
							if(LTSideCircleWidth!=0.0f && LTSideCircleHeight!=0.0f){
								TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);

								P1.x = (int)(FLTPoint.x-20);    P1.y =(int)(FLTPoint.y-(FLTPoint.y/2) ); P2.x = (int)(P1.x+LTSideCircleWidth+20);    P2.y = (int)(P1.y);
								P3.x = (int)(FLTPoint.x-20);    P3.y =(int)(P1.y+LTSideCircleHeight+20); P4.x = (int)(P2.x                    );    P4.y = (int)(P3.y);

								// �˻�ȭ�� Display ���� 
								ASideRect[0].left  = P1.x;   ASideRect[0].top    = P1.y;
								ASideRect[0].right = P2.x;   ASideRect[0].bottom = P3.y;

								int PixelCnt =0;

								// Median Filter ���� 
								if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
								if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)>=FrameImgH) return false;
								if((P1.x+(P2.x - P1.x))>=FrameImgW || (P1.y+(P3.y - P1.y))>FrameImgH                ) return false;

								FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
								FilterROI.Attach(&BinaryImg)                                               ;
								FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y))     ;
								LogUnit.SetLog(L"LeftTop_Long==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
								// 						LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
								EasyImage::Median(&FilterROI, &FilterROI)                                  ;
//		 						GImage->Save("D:\\ROTATE_IMG\\Polygon_LeftTop_����.bmp");

								Polygon->GImage = &BinaryImg                     ;
								Polygon->IWidth = FrameImgW                           ;
								Polygon->IHeight= FrameImgH                           ;
								Polygon->PoPa.FMinWidth  =5                      ;                       
								Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
								Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
								Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
								Polygon->PoPa.MinPxCnt   =100                    ;
								Polygon->PoPa.MaxPxCnt   =20000                  ;
								Polygon->PoPa.MaxBbCnt   =10                     ;
								Polygon->PoPa.IsSeparate =true                   ;
								Polygon->PoPa.IsInToOut  =false                  ;
								Polygon->PoPa.HiThHold   = ECThresHold_Sta       ;
								Polygon->PoPa.LoThHold   = 0                     ;
								// 						Polygon->PoPa.IgInSide   = true                  ;
								Polygon->PoPa.IgInSide   = false                 ;
								Polygon->PoPa.IsHorSort  = false                 ;
								Polygon->PoPa.IsVerSort  = true                  ;
								Polygon->PoPa.IsInToLeft = true                  ;

								// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
								PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
								PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
								PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
								PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

								if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
									Polygon->OnExecute(P1,P2,P3,P4);
								}

								// 2���̻� ���� � ������ ����� �з� ���� 
								if(Polygon->BlobCount>1){
									// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
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

								// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
								////////////////////////////////////////////////////////////////////
								if(Polygon->Blob[nPolyPosCnt].InCnt>0){
									LTCirInEdgeCount=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y                  ) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y                  ) continue;
										// Hap�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
										if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
										LTCirInEdgeCount++;
									}

									if(LTCirInEdgeCount>0){
										if(LTCirInPoint!=NULL) delete[] LTCirInPoint;
										LTCirInPoint = new TFPoint[LTCirInEdgeCount];

										PixelCnt=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											// Hap�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
											if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

											LTCirInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
											LTCirInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
											PixelCnt++;
										}
									}
								}

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
							}
						}
					}
				}
			}
			if (FRightTop   ){ //�ű� ������Ʈ(LineScan4K X 8) ==> Cell �����ڸ��� �˻��ϹǷ� ����� �ʿ����  

			}
			if (FRightBottom){ //�ű� ������Ʈ(LineScan4K X 8) ==> Cell �����ڸ��� �˻��ϹǷ� ����� �ʿ����  

			}
			if (FLeftBottom ){ //�ű� ������Ʈ(LineScan4K X 8) ==> ����/���� �з� �˻� ������  
				FLTPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waTopWall].FP0   , Line[waTopWall].FP1   );
				FLBPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waBottomWall].FP0, Line[waBottomWall].FP1);
				EasyImage::Threshold(GImage,&BinaryImg, ECThresHold_End);

				if(IsShortCell){ //�����ϰ��(A1-->A2)_S4 or (C1-->C2)_S2_Dark_Bright�ҷ� ���� 
					if(!IsAllGrind){
						// Chamber_Cut���� ������ GrindMark��뿩�� üũ
						for(int k=19; k<23; k++) {
							if(GrindMark[k].DvX!=0 && GrindMark[k].DvY!=0){IsGrindMarkUse = true; break;}
						}

						if(IsRectBtm){
							LBDx = 0.0f;
							LBDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FLBLPoint.x = FLBPoint.x;
							FLBLPoint.y = FLBPoint.y;
							FLBBPoint.x = FLBPoint.x;
							FLBBPoint.y = FLBPoint.y;
						}
						else if(IsCornerBtm){	
							if(ECThresHold_End==0) {SingleLine->GImage = GImage    ; SingleLine->FThreshold =5 ;}
							else                   {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

							if(LBCornerWidth!=0 && LBCornerHeight!=0){ //Corner Dx,Dy ������ �˻� ����
								if(!IsGrindMarkUse){ //3Point ���� ����,������ ���� ������ 
									// Corner ���� �ܰ����� 
									SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x-10                     );
									SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y-LBCornerHeight         );
									SingleLine->IRoi->SetHeight((int)LBCornerHeight                    );
									SingleLine->IRoi->SetWidth ((int)LBCornerWidth+LBCornerWidthOffset);
									//								SingleLine->IRoi->SetWidth ((int)LBCornerWidth+20                  ); HSKIM

									SingleLine->SetOrientation(orWest);
									SingleLine->FIncrement =1;
									SingleLine->FThreshold =15;
									SingleLine->SetFindLineId(0);
									SingleLine->SetEdgeType(1);  //White To Black 
									SingleLine->OnExecute();

									Line[7].FP0 = SingleLine->GetResultP0();
									Line[7].FP1 = SingleLine->GetResultP1();

									// Corner Chamber_Cut ������ 
									FLBLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall   ].FP0, Line[waLeftWall   ].FP1);
									FLBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);

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

										// OutLine C1Corner Data 
										if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
											if(LBCorPoint!=NULL) delete[] LBCorPoint;
											LBCorPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
											LBCorEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

											CornerCount =0;  
											for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
												if(CornerCount>=MAX_CORNER_POINT) break;
												LBCorPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
												LBCorPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
												CornerCount++;
											}
										}

										double Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
										// Corner Grind In Gray Level ������ ���ؼ� Corner Grind Out/In ThresHold���� �з��ϴ� 
										if(CorGrindInThresHold_End!=0){ // Corner InLine Search Sequence 
											EasyImage::Threshold(GImage,&BinaryImg, CorGrindInThresHold_End);
											SingleLine->GImage = &BinaryImg;  SingleLine->FThreshold =10;

											// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
											// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
											if(LBCornerWidth >(LBCornerHeight*1.5)){
												SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x                     );
												SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y - LBCornerHeight    );
												SingleLine->IRoi->SetWidth ((int)((FLBBPoint.x - FLBPoint.x)+20));
												SingleLine->IRoi->SetHeight((int)((FLBPoint.y  - FLBLPoint.y)  ));

												if(FLBPoint.x<=0    || FLBPoint.y - LBCornerHeight<=0    || (FLBBPoint.x - FLBPoint.x )<=0    || ((FLBPoint.y  - FLBLPoint.y)+20)<=0   ) return false;
												if(FLBPoint.x>=FrameImgW || FLBPoint.y - LBCornerHeight>=FrameImgH || (FLBBPoint.x - FLBPoint.x )>=FrameImgW || ((FLBPoint.y  - FLBLPoint.y)+20)>=FrameImgH) return false;

												SingleLine->SetOrientation(orNorth);
												SingleLine->FIncrement =1;
												SingleLine->SetFindLineId(0);
												SingleLine->SetEdgeType(1);   //White To Black 
												SingleLine->OnExecute();

												// InLine C1Corner Data 
												if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
													if(LBCorInPoint!=NULL) delete[] LBCorInPoint;
													LBCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
													LBCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

													CornerCount =0;  
													for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
														if(CornerCount>=MAX_CORNER_POINT) break;
														LBCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														LBCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
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
													if(LBCorInPoint!=NULL) delete[] LBCorInPoint;
													LBCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
													LBCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

													CornerCount =0;  
													for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
														if(CornerCount>=MAX_CORNER_POINT) break;
														LBCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														LBCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
														CornerCount++;
													}
												}
											}
											//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
											FLBLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
											FLBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);

											//�ڳ� ������ 
											Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
											for(int k=0; k<LBCorInEdgeCount; k++){
												Dis = Math.GetLengthFrLineToPoint(FLBLPoint, FLBBPoint, LBCorInPoint[k]);
												TotalGrindWidth+=Dis;
											}
											if(LBCorInEdgeCount!=0){
												AvgGrindWidth = (TotalGrindWidth/LTCorInEdgeCount);
												LBCorGrindWd  = AvgGrindWidth;
											}
										}

										// ������ ���� 
										if(CorGrindMeaThresHold!=0){ // Corner Grind ThresHold���� �����Ǿ� ������츸 ������ �˻縦 �����Ѵ� 
											// �ڳ� ������ ���� ������ ã�´� 
											// ROI ���� ����(3���� ��� ���� ���� ������ ã�´�)
											EasyImage::Threshold(GImage,&BinaryImg, CorGrindMeaThresHold);
											//										BinaryImg.Save("D:\\ROTATE_IMG\\CornerMeaThreshold_Bottom.bmp");
											SingleLine->GImage = &BinaryImg;  SingleLine->FThreshold =10;

											// ���� ������....�ϴ� ������ �ޱ������� �ӽ÷� ������ �з��Ͽ� CornerEdgePoint ã�� 
											if(LBCornerWidth >(LBCornerHeight*1.5)){
												SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x+LBCornerWidthOffset                     );
												SingleLine->IRoi->SetOrgY  ((int)(FLBPoint.y - LBCornerHeight)-LBCornerHeightOffset );
												SingleLine->IRoi->SetHeight((int)LBCornerHeight                                     ); 
												SingleLine->IRoi->SetWidth ((int)LBCornerWidth                                      );
												// 											SingleLine->IRoi->SetWidth (LBCornerWidth -50                       ); HSKIM
												// 											SingleLine->IRoi->SetHeight((int)LBCornerHeight-40                  );

												SingleLine->SetOrientation(orNorth);
												SingleLine->SetFindLineId(0);
												SingleLine->FIncrement =1;
												SingleLine->SetEdgeType(0);  //Black To White 
												SingleLine->OnExecute();

												// C1Corner Grind Measure Stand Data 
												if((SingleLine->FEdgePointCount)>0){
													if(LBCorMeaPoint!=NULL) delete[] LBCorMeaPoint;
													LBCorMeaPoint     = new TFPoint [(SingleLine->FEdgePointCount)];
													LBCorMeaEdgeCount = (SingleLine->FEdgePointCount);

													CornerCount =0;  
													for(int k=0; k<(SingleLine->FEdgePointCount); k++){
														if(CornerCount>=MAX_CORNER_POINT) break;
														if(k==0                  ) P1 = SingleLine->FEdgePoint[k];
														if(k==LBCorMeaEdgeCount-1) P2 = SingleLine->FEdgePoint[k];
														LBCorMeaPoint[CornerCount].x  = SingleLine->FEdgePoint[k].x;
														LBCorMeaPoint[CornerCount].y  = SingleLine->FEdgePoint[k].y;
														CornerCount++;
													}
												}
												FLBLMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
												FLBBMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);

												//�ڳ� ������ ���  
												Dis =0.0f, AvgGrindMea=0.0f, TotalGrindMeasure=0.0f;
												for(int k=0; k<LBCorEdgeCount; k++){
													Dis = Math.GetLengthFrLineToPoint(FLBLMeaPoint, FLBBMeaPoint, LTCorPoint[k]);
													TotalGrindMeasure+=Dis;
												}
												if(LBCorEdgeCount!=0){
													AvgGrindMea    = (TotalGrindMeasure/LBCorEdgeCount);
													LBCorGrindMea  = AvgGrindMea;
												}
											}
											else {
//												SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x+15                      );
												SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x+LBCornerWidthOffset                     );
												SingleLine->IRoi->SetOrgY  ((int)(FLBPoint.y - LBCornerHeight)-LBCornerHeightOffset );
												SingleLine->IRoi->SetHeight((int)LBCornerHeight                                     ); 
												SingleLine->IRoi->SetWidth ((int)LBCornerWidth                                      );
												//											SingleLine->IRoi->SetHeight((int)LBCornerHeight-15                  ); HSKIM

//												int ROI_X, ROI_Y, ROI_W, ROI_H;
//												ROI_X = SingleLine->IRoi->GetOrgX  ();
//												ROI_Y = SingleLine->IRoi->GetOrgY  ();
//												ROI_W = SingleLine->IRoi->GetHeight();
//												ROI_H = SingleLine->IRoi->GetWidth ();

												SingleLine->SetOrientation(orWest);
												SingleLine->SetFindLineId(0);
												SingleLine->FIncrement =1;
												SingleLine->SetEdgeType(0);  //Black To White 
												SingleLine->OnExecute();

												P1 = SingleLine->GetResultP0();
												P2 = SingleLine->GetResultP1();

												//�ڳ� ������ ���� ���ΰ� ���ο� ������ ���� 
												FLBLMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall  ].FP0, Line[waLeftWall  ].FP1);
												FLBBMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);
												//											FLBBMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall ].FP0, Line[waTopWall ].FP1);

												// C1Corner Grind Measure Stand Data 
												if((SingleLine->FEdgePointCount)>0){
													if(LBCorMeaPoint!=NULL) delete[] LBCorMeaPoint;
													LBCorMeaPoint     = new TFPoint [(SingleLine->FEdgePointCount)];
													LBCorMeaEdgeCount = (SingleLine->FEdgePointCount);

													CornerCount =0;  
													for(int k=0; k<(SingleLine->FEdgePointCount); k++){
														if(CornerCount>=MAX_CORNER_POINT) break;
														LBCorMeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														LBCorMeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
														CornerCount++;
													}
												}
												//�ڳ� ������ ���  
												Dis =0.0f, AvgGrindMea=0.0f, TotalGrindMeasure=0.0f;
												for(int k=0; k<LBCorEdgeCount; k++){
													Dis = Math.GetLengthFrLineToPoint(FLBLMeaPoint, FLBBMeaPoint, LBCorPoint[k]);
													TotalGrindMeasure+=Dis;
												}
												if(LBCorEdgeCount!=0){
													AvgGrindMea    = (TotalGrindMeasure/LBCorEdgeCount);
													LBCorGrindMea  = AvgGrindMea;
												}
											}
										}
									}
								}
								else                                                { // GrindMark ���� ����,������ ���� ������ 
									// Corner ���� �ܰ����� 
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

									// Corner Chamber_Cut ������ 
									FLBLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall   ].FP0, Line[waLeftWall   ].FP1);
									FLBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);

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

										// OutLine C1Corner Data 
										if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
											if(LBCorPoint!=NULL) delete[] LBCorPoint;
											LBCorPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
											LBCorEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

											CornerCount =0;  
											for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
												if(CornerCount>=MAX_CORNER_POINT) break;
												LBCorPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
												LBCorPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
												CornerCount++;
											}
										}

										double Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
										// Corner Grind In Gray Level ������ ���ؼ� Corner Grind Out/In ThresHold���� �з��ϴ� 
										if(CorGrindInThresHold_End!=0){ // Corner InLine Search Sequence 
											EasyImage::Threshold(GImage,&BinaryImg, CorGrindInThresHold_End);
											SingleLine->GImage = &BinaryImg;  SingleLine->FThreshold =10;

											// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
											// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
											if(LBCornerWidth >(LBCornerHeight*2)){
												SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x                     );
												SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y - LBCornerHeight    );
												SingleLine->IRoi->SetWidth ((int)((FLBBPoint.x - FLBPoint.x)+20));
												SingleLine->IRoi->SetHeight((int)((FLBPoint.y  - FLBLPoint.y)  ));

												if(FLBPoint.x<=0    || FLBPoint.y - LBCornerHeight<=0    || (FLBBPoint.x - FLBPoint.x )<=0    || ((FLBPoint.y  - FLBLPoint.y)+20)<=0   ) return false;
												if(FLBPoint.x>=FrameImgW || FLBPoint.y - LBCornerHeight>=FrameImgH || (FLBBPoint.x - FLBPoint.x )>=FrameImgW || ((FLBPoint.y  - FLBLPoint.y)+20)>=FrameImgH) return false;

												SingleLine->SetOrientation(orNorth);
												SingleLine->FIncrement =1;
												SingleLine->SetFindLineId(0);
												SingleLine->SetEdgeType(1);   //White To Black 
												SingleLine->OnExecute();

												// InLine C1Corner Data 
												if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
													if(LBCorInPoint!=NULL) delete[] LBCorInPoint;
													LBCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
													LBCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

													CornerCount =0;  
													// 												for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
													for(int k=((SingleLine->FEdgePointCount)-CornerMargin-1); k>(CornerMargin-1); k--){
														if(CornerCount>=MAX_CORNER_POINT) break;
														LBCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														LBCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
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
													if(LBCorInPoint!=NULL) delete[] LBCorInPoint;
													LBCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
													LBCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

													CornerCount =0;  
													for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
														if(CornerCount>=MAX_CORNER_POINT) break;
														LBCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														LBCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
														CornerCount++;
													}
												}
											}
											//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
											FLBLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
											FLBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);

											// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
											if(LBCorInEdgeCount>0 && LBCorEdgeCount>0){
												// Total Corner In Data 
												for(int k=0; k<LBCorInEdgeCount; k++){
													if(AllCorInEdgeCount>MAX_ALL_EDGE_COUNT) return false               ;
													AllCornerInData[AllCorInEdgeCount].x = LBCorInPoint[k].x            ;
													AllCornerInData[AllCorInEdgeCount].y = LBCorInPoint[k].y+ImageHeight;
													AllCorInEdgeCount++;
												}
												// Total Corner Out Data 
												for(int k=0; k<LBCorEdgeCount; k++){
													if(AllCorOutEdgeCount>MAX_ALL_EDGE_COUNT) return false              ;
													AllCornerOutData[AllCorOutEdgeCount].x = LBCorPoint[k].x            ;
													AllCornerOutData[AllCorOutEdgeCount].y = LBCorPoint[k].y+ImageHeight;
													AllCorOutEdgeCount++;
												}
											}
										}
									}
								}
							}
						}
						else if(IsCircleBtm){ // ���� �κп��� ���� 
							if(LBSideCircleWidth!=0.0f && LBSideCircleHeight!=0.0f){
								TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
								if(LeftCutThresHold==0){ //ASide Pad�ƴҰ��, �� ���Ǻκ��� ��� 1��ThresHold�� �������� ���� �����Ұ��  
									P1.x = (int)(FLBPoint.x-20);    P1.y =(int)(FLBPoint.y-LBSideCircleHeight-20); P2.x = (int)(P1.x+LTSideCircleWidth+20);    P2.y = (int)(P1.y);
									P3.x = (int)(FLBPoint.x-20);    P3.y =(int)(P1.y+LTSideCircleHeight+40      ); P4.x = (int)(P2.x                     );    P4.y = (int)(P3.y);

									// �˻�ȭ�� Display ���� 
									CSideRect[0].left  = P1.x;   CSideRect[0].top    = P1.y;
									CSideRect[0].right = P2.x;   CSideRect[0].bottom = P3.y;

									int PixelCnt =0;

									// Median Filter ���� 
									if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0                       ) return false;
									if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)>=FrameImgH) return false;
									if((P1.x+(P2.x - P1.x))>=FrameImgW || (P1.y+(P3.y - P1.y))>FrameImgH                         ) return false;

									FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
									FilterROI.Attach(&BinaryImg)                                               ;
									FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y))     ;
									LogUnit.SetLog(L"LeftBtm_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
									// 							LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
									EasyImage::Median(&FilterROI, &FilterROI)                                  ;
									// 							GImage->Save("D:\\ROTATE_IMG\\Polygon_LeftBtm.bmp");
									// 							BinaryImg.Save("D:\\ROTATE_IMG\\Polygon_LeftBtm_����.bmp");

									Polygon->GImage = &BinaryImg                     ;
									Polygon->IWidth = FrameImgW                           ;
									Polygon->IHeight= FrameImgH                           ;
									Polygon->PoPa.FMinWidth  =5                      ;                       
									Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
									Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
									Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
									Polygon->PoPa.MinPxCnt   =100                    ;
									Polygon->PoPa.MaxPxCnt   =20000                  ;
									Polygon->PoPa.MaxBbCnt   =10                     ;
									Polygon->PoPa.IsSeparate =true                   ;
									Polygon->PoPa.IsInToOut  =false                  ;
									Polygon->PoPa.HiThHold   = ECThresHold_End       ;
									Polygon->PoPa.LoThHold   = 0                     ;
									Polygon->PoPa.IgInSide   = false                 ;
									Polygon->PoPa.IsHorSort  = false                 ;
									Polygon->PoPa.IsVerSort  = true                  ;
									Polygon->PoPa.IsInToLeft = true                  ;

									// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
									PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
									PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
									PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
									PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

									if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
										Polygon->OnExecute(P1,P2,P3,P4);
									}

									// 2���̻� ���� � ������ ����� �з� ���� 
									if(Polygon->BlobCount>1){
										// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
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

									// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
									////////////////////////////////////////////////////////////////////
									if(Polygon->Blob[nPolyPosCnt].InCnt>0){
										LBCirInEdgeCount=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											// Cup�˻�� �������� ������ ���� 
											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

											LBCirInEdgeCount++;
										}

										if(LBCirInEdgeCount>0){
											if(LBCirInPoint!=NULL) delete[] LBCirInPoint;
											LBCirInPoint = new TFPoint[LBCirInEdgeCount];

											PixelCnt=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												// Cup�˻�� �������� ������ ���� 
												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

												LBCirInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
												LBCirInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
												PixelCnt++;
											}
										}
									}

									if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
										LBCirOutEdgeCount=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
											// Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
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
												// Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;

												LBCirOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
												LBCirOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
												PixelCnt++;
											}
										}
									}
									////////////////////////////////////////////////////////////////////


									// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
									if(LBCirInEdgeCount>0 && LBCirOutEdgeCount>0){
										// Total Corner In Data 
										for(int k=0; k<LBCirInEdgeCount; k++){
											if(AllCirInEdgeCount>MAX_ALL_EDGE_COUNT) return false               ;
											AllCircleInData[AllCirInEdgeCount].x = LBCirInPoint[k].x            ;
											AllCircleInData[AllCirInEdgeCount].y = LBCirInPoint[k].y+ImageHeight;
											AllCirInEdgeCount++;
										}
										// Total Corner Out Data 
										for(int k=0; k<LBCirOutEdgeCount; k++){
											if(AllCirOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                 ;
											AllCircleOutData[AllCirOutEdgeCount].x = LBCirOutPoint[k].x            ;
											AllCircleOutData[AllCirOutEdgeCount].y = LBCirOutPoint[k].y+ImageHeight;
											AllCirOutEdgeCount++;
										}
									}
								}
								else                   {//ASide Pad, ���ǰ� ������ �����ؾ��� ���(FLBPoint)
									IsFrameGrindMark = false                ;
									RealFrameStaY    = ImageHeight          ; //��üBuff���� Frame_Real��ǥ 
									RealFrameEndY    = ImageHeight+FrameImgH; //��üBuff���� Frame_Real��ǥ 

									P1.x = (int)(FLBPoint.x-20);    P1.y =(int)(FLBPoint.y-LBSideCircleHeight-20); P2.x = (int)(P1.x+LTSideCircleWidth+20);    P2.y = (int)(P1.y);
									P3.x = (int)(FLBPoint.x-20);    P3.y =(int)(P1.y+LTSideCircleHeight+40      ); P4.x = (int)(P2.x                     );    P4.y = (int)(P3.y);

									// 							for(int k=0; k<2; k++){GrindStepy[k].x=0; GrindStepy[k].y=0;}
									for(int k=0; k<3; k++){
										GrindLine[k].LinePoint.x =0;
										GrindLine[k].LinePoint.y =0;
										GrindLine[k].IsGrindLine =false;
									}

									// ��üBuffer���� �˻� Frame�� GrindMark�����ϴ��� Ȯ�� �� üũ 
									for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
										if(GrindMark[k].DvX==0 || GrindMark[k].DvY==0) continue;
										if(((BuffLTPoint.y + GrindMark[k].DvY)>= RealFrameStaY) && ((BuffLTPoint.y + GrindMark[k].DvY)<= RealFrameEndY)){
											IsFrameGrindMark = true;

											//��ü Buffer���� ��ǥ 
											if((BuffLTPoint.y + GrindMark[k].DvY)<CutMinPos) CutMinPos =(BuffLTPoint.y + GrindMark[k].DvY);
											if((BuffLTPoint.y + GrindMark[k].DvY)>CutMaxPos) CutMaxPos =(BuffLTPoint.y + GrindMark[k].DvY); 
										}
									}

									// Frame���� ��ǥ�� ��ȯ 
									if(CutMinPos>0) CutMinPos = CutMinPos - ImageHeight;
									if(CutMaxPos>0) CutMaxPos = CutMaxPos - ImageHeight;

									if(IsFrameGrindMark){ //���� Frame�� GrindMark�� ������� 
										RoiCount =2;
										GrindLine[0].LinePoint.y = 0           ;
										GrindLine[0].IsGrindLine = false       ;
										GrindLine[1].LinePoint.y = CutMinPos-30;
										GrindLine[1].IsGrindLine = true        ;

										for(int k=0; k<RoiCount; k++){
											GrindThresHold =0;
											if(k==(RoiCount-1)){
												P1.y =GrindLine[k].LinePoint.y; P2.y = (int)(P1.y);
												P3.y =(int)(FrameImgH      )  ; P4.y = (int)(P3.y);
											}
											else               {
												P1.y =GrindLine[k  ].LinePoint.y; P2.y = (int)(P1.y);
												P3.y =GrindLine[k+1].LinePoint.y; P4.y = (int)(P3.y);
											}
											GrindThresHold = (GrindLine[k].IsGrindLine)?ECThresHold_End:LeftCutThresHold;

											// 1�� Frame_Buff�� GrindMark Position���� ��Ȱ�Ͽ� ROI���������� �ʿ��� 
											if(k==0){StaLeftInEdgeCount=AllCirInEdgeCount; StaLeftOutEdgeCount=AllCirOutEdgeCount; }
											// GrindMark���� ���� ��Ȱ�� ������ �ʹ� ������� Polygon_Data �������� ���� 
											if((P3.y - P1.y)<15) continue;

											EasyImage::Threshold(GImage,&BinaryImg, GrindThresHold);
											// Median Filter ���� 
											if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
											if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) return false;
											if((P1.x+(P2.x - P1.x))>=FrameImgW    || (P1.y+(P3.y - P1.y))>FrameImgH                      ) return false;

											FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
											FilterROI.Attach(&BinaryImg)                                          ;
											FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
											LogUnit.SetLog(L"AllGrind_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
											//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
											EasyImage::Median(&FilterROI, &FilterROI)                             ;

											Polygon->GImage = &BinaryImg                     ;
											Polygon->IWidth = FrameImgW                      ;
											Polygon->IHeight= FrameImgH                      ;
											Polygon->PoPa.FMinWidth  =2                      ;                       
											Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
											Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
											Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
											Polygon->PoPa.MinPxCnt   =50                     ;
											Polygon->PoPa.MaxPxCnt   =20000                  ;
											Polygon->PoPa.MaxBbCnt   =10                     ;
											Polygon->PoPa.IsSeparate =true                   ;
											Polygon->PoPa.IsInToOut  =false                  ;
											Polygon->PoPa.HiThHold   = GrindThresHold        ;
											Polygon->PoPa.LoThHold   = 0                     ;
											Polygon->PoPa.IgInSide   = true                  ;
											Polygon->PoPa.IsHorSort  = false                 ;
											Polygon->PoPa.IsVerSort  = true                  ;
											Polygon->PoPa.IsInToLeft = true                  ;

											// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
											PolyP1[k].x = P1.x;  PolyP1[k].y = P1.y;
											PolyP2[k].x = P2.x;  PolyP2[k].y = P2.y;
											PolyP3[k].x = P3.x;  PolyP3[k].y = P3.y;
											PolyP4[k].x = P4.x;  PolyP4[k].y = P4.y;

											if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
												Polygon->OnExecute(P1,P2,P3,P4);
											}

											// 2���̻� ���� � ������ ����� �з� ���� 
											if(Polygon->BlobCount>1){
												// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
												PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
												PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
												for(int k=0; k<Polygon->BlobCount; k++){
													PolygonCenter.x = Polygon->Blob[k].Rect.l;
													PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
													// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
													RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
													if(RealPolyDis<MinPolyDis) {
														MinPolyDis  = RealPolyDis;
														nPolyPosCnt = k;
													}
												}
											}
											else nPolyPosCnt=0;

											// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
											////////////////////////////////////////////////////////////////////
											if(Polygon->Blob[nPolyPosCnt].InCnt>0){
												FrameLeftInEdgeCount=0;
												for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
													if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
													if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
													// Cup�˻�� �������� ������ ���� 
													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

													FrameLeftInEdgeCount++;
												}

												if(FrameLeftInEdgeCount>0){
													if(FrameLeftInPoint!=NULL) delete[] FrameLeftInPoint;
													FrameLeftInPoint = new TFPoint[FrameLeftInEdgeCount];

													PixelCnt=0;
													for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
														if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
														if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
														// Cup�˻�� �������� ������ ���� 
														if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

														FrameLeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
														FrameLeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
														PixelCnt++;
													}
												}
											}

											if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
												FrameLeftOutEdgeCount=0;
												for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
													if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
													if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
													FrameLeftOutEdgeCount++;
												}

												if(FrameLeftOutEdgeCount>0){
													if(FrameLeftOutPoint!=NULL) delete[] FrameLeftOutPoint;
													FrameLeftOutPoint = new TFPoint[FrameLeftOutEdgeCount];

													PixelCnt=0;
													for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
														if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
														if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

														if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
														FrameLeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
														FrameLeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
														PixelCnt++;
													}
												}
											}
											////////////////////////////////////////////////////////////////////

											// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
											if(FrameLeftInEdgeCount>0 && FrameLeftOutEdgeCount>0){
												// Total Left In Data 
												for(int k=0; k<FrameLeftInEdgeCount; k++){
													if(AllCirInEdgeCount>MAX_ALL_EDGE_COUNT) return false                 ;
													AllCircleInData[AllCirInEdgeCount].x = FrameLeftInPoint[k].x            ;
													AllCircleInData[AllCirInEdgeCount].y = FrameLeftInPoint[k].y+ImageHeight;
													AllCirInEdgeCount++;
												}
												// Total Left Out Data 
												for(int k=0; k<FrameLeftOutEdgeCount; k++){
													if(AllCirOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                   ;
													AllCircleOutData[AllCirOutEdgeCount].x = FrameLeftOutPoint[k].x            ;
													AllCircleOutData[AllCirOutEdgeCount].y = FrameLeftOutPoint[k].y+ImageHeight;
													AllCirOutEdgeCount++;
												}
											}
										}

										// Frame�� ROI�������� ��Ȱ�˻� ���� �� Frame�������� �ٽ� ������ �Ѱ���
										// Dark_Defect/Bright_Defect�˻� ���� 
										if((AllCirInEdgeCount-StaLeftInEdgeCount)>0 && (AllCirOutEdgeCount-StaLeftOutEdgeCount)>0){
											// LeftInEdgeData ���� ���� 
											LBCirInEdgeCount = (AllCirInEdgeCount-StaLeftInEdgeCount);
											if(LBCirInPoint!=NULL) delete[] LBCirInPoint;
											LBCirInPoint = new TFPoint[LBCirInEdgeCount];

											// LeftOutEdgeData ���� ���� 
											LBCirOutEdgeCount = (AllCirOutEdgeCount-StaLeftOutEdgeCount);
											if(LBCirOutPoint!=NULL) delete[] LBCirOutPoint;
											LBCirOutPoint = new TFPoint[LBCirOutEdgeCount];

											// ��ü �����Ϳ��� Ư������ �����͸� �Ѱ��� 
											for(int k=0; k<LBCirInEdgeCount; k++){
												LBCirInPoint[k].x   = AllCircleInData[StaLeftInEdgeCount+k].x             ;
												LBCirInPoint[k].y   = AllCircleInData[StaLeftInEdgeCount+k].y -ImageHeight;
											}

											// ��ü �����Ϳ��� Ư������ �����͸� �Ѱ��� 
											for(int k=0; k<LBCirOutEdgeCount; k++){
												LBCirOutPoint[k].x   = AllCircleOutData[StaLeftOutEdgeCount+k].x             ;
												LBCirOutPoint[k].y   = AllCircleOutData[StaLeftOutEdgeCount+k].y -ImageHeight;
											}
										}
									}
									else                { //���� Frame�� GrindMark�� ������� 
										EasyImage::Threshold(GImage,&BinaryImg, LeftCutThresHold);
										P1.x = (int)(FLBPoint.x-20);    P1.y =(int)(FLBPoint.y-LBSideCircleHeight-20); P2.x = (int)(P1.x+LTSideCircleWidth+20);    P2.y = (int)(P1.y);
										P3.x = (int)(FLBPoint.x-20);    P3.y =(int)(P1.y+LTSideCircleHeight+40      ); P4.x = (int)(P2.x                     );    P4.y = (int)(P3.y);

										// �˻�ȭ�� Display ���� 
										CSideRect[0].left  = P1.x;   CSideRect[0].top    = P1.y;
										CSideRect[0].right = P2.x;   CSideRect[0].bottom = P3.y;

										int PixelCnt =0;

										// Median Filter ���� 
										if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0                       ) return false;
										if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)>=FrameImgH) return false;
										if((P1.x+(P2.x - P1.x))>=FrameImgW || (P1.y+(P3.y - P1.y))>FrameImgH                         ) return false;

										FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
										FilterROI.Attach(&BinaryImg)                                               ;
										FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y))     ;
										LogUnit.SetLog(L"LeftBtm_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
										// 							LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
										EasyImage::Median(&FilterROI, &FilterROI)                                  ;
										// 							GImage->Save("D:\\ROTATE_IMG\\Polygon_LeftBtm.bmp");
										// 							BinaryImg.Save("D:\\ROTATE_IMG\\Polygon_LeftBtm_����.bmp");

										Polygon->GImage = &BinaryImg                     ;
										Polygon->IWidth = FrameImgW                           ;
										Polygon->IHeight= FrameImgH                           ;
										Polygon->PoPa.FMinWidth  =5                      ;                       
										Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
										Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
										Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
										Polygon->PoPa.MinPxCnt   =100                    ;
										Polygon->PoPa.MaxPxCnt   =20000                  ;
										Polygon->PoPa.MaxBbCnt   =10                     ;
										Polygon->PoPa.IsSeparate =true                   ;
										Polygon->PoPa.IsInToOut  =false                  ;
										Polygon->PoPa.HiThHold   = ECThresHold_End       ;
										Polygon->PoPa.LoThHold   = 0                     ;
										Polygon->PoPa.IgInSide   = false                 ;
										Polygon->PoPa.IsHorSort  = false                 ;
										Polygon->PoPa.IsVerSort  = true                  ;
										Polygon->PoPa.IsInToLeft = true                  ;

										// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
										PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
										PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
										PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
										PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

										if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
											Polygon->OnExecute(P1,P2,P3,P4);
										}

										// 2���̻� ���� � ������ ����� �з� ���� 
										if(Polygon->BlobCount>1){
											// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
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

										// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
										////////////////////////////////////////////////////////////////////
										if(Polygon->Blob[nPolyPosCnt].InCnt>0){
											LBCirInEdgeCount=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
												// Cup�˻�� �������� ������ ���� 
												if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

												LBCirInEdgeCount++;
											}

											if(LBCirInEdgeCount>0){
												if(LBCirInPoint!=NULL) delete[] LBCirInPoint;
												LBCirInPoint = new TFPoint[LBCirInEdgeCount];

												PixelCnt=0;
												for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
													if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
													if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
													// Cup�˻�� �������� ������ ���� 
													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

													LBCirInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
													LBCirInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
													PixelCnt++;
												}
											}
										}

										if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
											LBCirOutEdgeCount=0;
											for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
												if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
												if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
												// Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
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
													// Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
													if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;

													LBCirOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
													LBCirOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
													PixelCnt++;
												}
											}
										}
										////////////////////////////////////////////////////////////////////


										// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
										if(LBCirInEdgeCount>0 && LBCirOutEdgeCount>0){
											// Total Corner In Data 
											for(int k=0; k<LBCirInEdgeCount; k++){
												if(AllCirInEdgeCount>MAX_ALL_EDGE_COUNT) return false               ;
												AllCircleInData[AllCirInEdgeCount].x = LBCirInPoint[k].x            ;
												AllCircleInData[AllCirInEdgeCount].y = LBCirInPoint[k].y+ImageHeight;
												AllCirInEdgeCount++;
											}
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

								// FAVION Circle Fitting �����(LeftBtm)
								SampleSize = LBCirInEdgeCount*0.75; //0.5~0.8���Ϸ� �Է��Ұ�
								DistDevRatio = 0.15; //0.1~0.25 ���Ϸ� �Է��Ұ�
								CircleFit->OnRansac(LBCirInPoint,LBCirInEdgeCount,SampleSize,DistDevRatio,&LBCircleFitCenter,&LBCirFitDiameter);
							}
						}
					}
				}
				else           { //�����ϰ��(A1-->C1)_S3 or (A2-->C2)_S1_Dark�ҷ��� ���� 
					if(!IsAllGrind){
						if(IsRectBtm){
							LBDx = 0.0f;
							LBDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FLBLPoint.x = FLBPoint.x;
							FLBLPoint.y = FLBPoint.y;
							FLBBPoint.x = FLBPoint.x;
							FLBBPoint.y = FLBPoint.y;
						}
						else if(IsCornerBtm){
							if(ECThresHold_End==0) {SingleLine->GImage = GImage    ; SingleLine->FThreshold =5 ;}
							else                   {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

							if(LBCornerWidth!=0 && LBCornerHeight!=0){ //Corner Dx,Dy ������ �˻� ����
								// Corner ���� �ܰ����� 
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

								// Corner Chamber_Cut ������ 
								FLBLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall   ].FP0, Line[waLeftWall   ].FP1);
								FLBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);

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

									// OutLine C1Corner Data 
									if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
										if(LBCorPoint!=NULL) delete[] LBCorPoint;
										LBCorPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
										LBCorEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

										CornerCount =0;  
										for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
											if(CornerCount>=MAX_CORNER_POINT) break;
											LBCorPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
											LBCorPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									double Dis =0.0f, AvgGrindWidth=0.0f, AvgGrindMea=0.0f, TotalGrindWidth=0.0f, TotalGrindMeasure=0.0f;
									// Corner Grind In Gray Level ������ ���ؼ� Corner Grind Out/In ThresHold���� �з��ϴ� 
									if(CorGrindInThresHold_End!=0){ // Corner InLine Search Sequence 
										EasyImage::Threshold(GImage,&BinaryImg, CorGrindInThresHold_End);
										SingleLine->GImage = &BinaryImg;  SingleLine->FThreshold =10;

										// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
										// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
										if(LBCornerWidth >(LBCornerHeight*1.5)){
											SingleLine->IRoi->SetOrgX  ((int)FLBPoint.x                     );
											SingleLine->IRoi->SetOrgY  ((int)FLBPoint.y - LBCornerHeight    );
											SingleLine->IRoi->SetWidth ((int)((FLBBPoint.x - FLBPoint.x)+20));
											SingleLine->IRoi->SetHeight((int)((FLBPoint.y  - FLBLPoint.y)  ));

											if(FLBPoint.x<=0    || FLBPoint.y - LBCornerHeight<=0    || (FLBBPoint.x - FLBPoint.x )<=0    || ((FLBPoint.y  - FLBLPoint.y)+20)<=0   ) return false;
											if(FLBPoint.x>=FrameImgW || FLBPoint.y - LBCornerHeight>=FrameImgH || (FLBBPoint.x - FLBPoint.x )>=FrameImgW || ((FLBPoint.y  - FLBLPoint.y)+20)>=FrameImgH) return false;

											SingleLine->SetOrientation(orSouth);
											SingleLine->FIncrement =1;
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(1);   //White To Black 
											SingleLine->OnExecute();

											// InLine C1Corner Data 
											if((SingleLine->FEdgePointCount) - (CornerMargin*2)>0){
												if(LBCorInPoint!=NULL) delete[] LBCorInPoint;
												LBCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
												LBCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

												CornerCount =0;  
												for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
													if(CornerCount>=MAX_CORNER_POINT) break;
													LBCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													LBCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
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
												if(LBCorInPoint!=NULL) delete[] LBCorInPoint;
												LBCorInPoint  = new TFPoint [(SingleLine->FEdgePointCount) - (CornerMargin*2)];
												LBCorInEdgeCount = (SingleLine->FEdgePointCount) -(CornerMargin*2);

												CornerCount =0;  
												for(int k=CornerMargin; k<((SingleLine->FEdgePointCount)-CornerMargin); k++){
													if(CornerCount>=MAX_CORNER_POINT) break;
													LBCorInPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													LBCorInPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}
										//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
										FLBLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0   , Line[waLeftWall].FP1);
										FLBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall ].FP0, Line[waBottomWall ].FP1);
									}
								}
							}
						}
						else if(IsCircleBtm){
							if(LBSideCircleWidth!=0.0f && LBSideCircleHeight!=0.0f){
								TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);

								P1.x = (int)(FLBPoint.x-20);    P1.y =(int)(FLBPoint.y-LBSideCircleHeight-20); P2.x = (int)(P1.x+LBSideCircleWidth+20);    P2.y = (int)(P1.y);
								P3.x = (int)(FLBPoint.x-20);    P3.y =(int)(P1.y+LBSideCircleHeight+40      ); P4.x = (int)(P2.x                     );    P4.y = (int)(P3.y);

								// �˻�ȭ�� Display ���� 
								CSideRect[0].left  = P1.x;   CSideRect[0].top    = P1.y;
								CSideRect[0].right = P2.x;   CSideRect[0].bottom = P3.y;

								int PixelCnt =0;

								// Median Filter ���� 
								if(P1.x<=0    || P1.y<=0    || (P2.x - P1.x)<=0    || (P3.y - P1.y)<=0   ) return false;
								if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)>=FrameImgH) return false;						
								if((P1.x+(P2.x - P1.x))>=FrameImgW || (P1.y+(P3.y - P1.y))>FrameImgH                ) return false;

								FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                      ;
								FilterROI.Attach(&BinaryImg)                                               ;
								FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y))     ;
								LogUnit.SetLog(L"LeftBtm_Short==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
								// 						LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
								EasyImage::Median(&FilterROI, &FilterROI)                                  ;
								//  									 RotateBinaryImg.Save("D:\\ROTATE_IMG\\Polygon_LeftTop_����.bmp");
								Polygon->GImage = &BinaryImg                     ;
								Polygon->IWidth = FrameImgW                           ;
								Polygon->IHeight= FrameImgH                           ;
								Polygon->PoPa.FMinWidth  =5                      ;                       
								Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
								Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
								Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
								Polygon->PoPa.MinPxCnt   =100                    ;
								Polygon->PoPa.MaxPxCnt   =20000                  ;
								Polygon->PoPa.MaxBbCnt   =10                     ;
								Polygon->PoPa.IsSeparate =true                   ;
								Polygon->PoPa.IsInToOut  =false                  ;
								Polygon->PoPa.HiThHold   = ECThresHold_End       ;
								Polygon->PoPa.LoThHold   = 0                     ;
								// 						Polygon->PoPa.IgInSide   = true                  ;
								Polygon->PoPa.IgInSide   = false                 ;
								Polygon->PoPa.IsHorSort  = false                 ;
								Polygon->PoPa.IsVerSort  = true                  ;
								Polygon->PoPa.IsInToLeft = true                  ;

								// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
								PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
								PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
								PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
								PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

								if(P1.x!=0 && P1.y!=0 && P2.x!=0 && P2.y!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
									Polygon->OnExecute(P1,P2,P3,P4);
								}

								// 2���̻� ���� � ������ ����� �з� ���� 
								if(Polygon->BlobCount>1){
									// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
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
								// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
								////////////////////////////////////////////////////////////////////
								if(Polygon->Blob[nPolyPosCnt].InCnt>0){
									LBCirInEdgeCount=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
										// Cup�˻�� �������� ������ ���� 
										if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

										LBCirInEdgeCount++;
									}

									if(LBCirInEdgeCount>0){
										if(LBCirInPoint!=NULL) delete[] LBCirInPoint;
										LBCirInPoint = new TFPoint[LBCirInEdgeCount];

										PixelCnt=0;
										for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
											if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
											// Cup�˻�� �������� ������ ���� 
											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;

											LBCirInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
											LBCirInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
											PixelCnt++;
										}
									}
								}

								if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
									LBCirOutEdgeCount=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										// Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
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
											// Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;

											LBCirOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
											LBCirOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
											PixelCnt++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else                        { //VR Glass 
			TPoint P1(0,0),P2(0,0),P3(0,0),P4(0,0);
			//�⺻ ROI���� ���� Polygon ���� ���� 
			//����� ���� ROI ������ 
			P1.x = IRoi->GetOrgX()      ; P1.y = IRoi->GetOrgY()       ;
			P2.x = P1.x+IRoi->GetWidth(); P2.y = IRoi->GetOrgY()       ;
			P3.x = IRoi->GetOrgX()      ; P3.y = P1.y+IRoi->GetHeight();
			P4.x = P2.x                 ; P4.y = P2.y+IRoi->GetHeight();

			if(IsAllGrind){ //��ü����(B,C,D)
				if(LeftCutThresHold==0){ //Cut���� ���� �Ƕ����� ���������� ���ϰ�� or ���� ��ü ���� �ϰ�� ������ 
					EasyImage::Threshold(GImage,&BinaryImg, LeftGrindThresHold);
// 					GImage->Save("D:\\ROTATE_IMG\\WATCH_ORG_����.bmp");

					// Median Filter ���� 
					if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
					if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) return false;
					if((P1.x+(P2.x - P1.x))>FrameImgW     || (P1.y+(P3.y - P1.y))>FrameImgH                      ) return false;

					FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
					FilterROI.Attach(&BinaryImg)                                          ;
					FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
					LogUnit.SetLog(L"POLYGON==>CellID:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
					EasyImage::Median(&FilterROI, &FilterROI)                                 ;
					// 						if(FShape==shRectH) BinaryImg.Save("D:\\ROTATE_IMG\\Polygon_Left_HAP_Median.bmp");

					Polygon->GImage = &BinaryImg                     ;
					Polygon->IWidth = FrameImgW                      ;
					Polygon->IHeight= FrameImgH                      ;
					Polygon->PoPa.FMinWidth  =2                      ;                       
					Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
					Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/3) ;
					Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
					Polygon->PoPa.MinPxCnt   =100                    ;
					Polygon->PoPa.MaxPxCnt   =20000                  ;
					Polygon->PoPa.MaxBbCnt   =10                     ;
					Polygon->PoPa.IsSeparate =true                   ;
					Polygon->PoPa.IsInToOut  =false                  ;
					Polygon->PoPa.HiThHold   = LeftGrindThresHold    ;
					Polygon->PoPa.LoThHold   = 0                     ;
					// 					Polygon->PoPa.IgInSide   = true                  ;
					Polygon->PoPa.IgInSide   = false                 ;
					Polygon->PoPa.IsHorSort  = false                 ;
					Polygon->PoPa.IsVerSort  = true                  ;
					Polygon->PoPa.IsInToLeft = true                  ;

					// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
					PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
					PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
					PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
					PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

					if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
						Polygon->OnExecute(P1,P2,P3,P4);
					}

					// 2���̻� ���� � ������ ����� �з� ���� 
					if(Polygon->BlobCount>1){
						// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
						PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
						PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
						for(int k=0; k<Polygon->BlobCount; k++){
							PolygonCenter.x = Polygon->Blob[k].Rect.l;
							PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
							// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
							RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
							if(RealPolyDis<MinPolyDis) {
								MinPolyDis  = RealPolyDis;
								nPolyPosCnt = k;
							}
						}
					}
					else if(Polygon->BlobCount==1){
						nPolyPosCnt=0;
					}
					else if(Polygon->BlobCount==0){
						nPolyPosCnt=0;
						//���� ������ �ʱ�ȭ 
						if(LeftInEdgeCount >0){ if(LeftInPoint !=NULL) delete[] LeftInPoint ; LeftInEdgeCount =0;LeftInPoint =NULL;}
						if(LeftOutEdgeCount>0){ if(LeftOutPoint!=NULL) delete[] LeftOutPoint; LeftOutEdgeCount=0;LeftOutPoint=NULL;}
					}

					// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
					////////////////////////////////////////////////////////////////////
					if(Polygon->Blob[nPolyPosCnt].InCnt>0){
						LeftInEdgeCount=0;
						for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
							if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
							if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

							if(FShape==shRectH ){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
								if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
							}
							else if(FShape==shRectC){
								if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
							}
							LeftInEdgeCount++;
						}

						if(LeftInEdgeCount>0){
							if(LeftInPoint!=NULL) delete[] LeftInPoint;
							LeftInPoint = new TFPoint[LeftInEdgeCount];

							PixelCnt=0;
							for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
								if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
								if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

								if(FShape==shRectH ){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
									if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
								}
								else if(FShape==shRectC ){
									if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
								}
								LeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
								LeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
								PixelCnt++;
							}
						}
					}

					if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
						LeftOutEdgeCount=0;
						for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
							if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
							if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

							if(FShape==shRectC ){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
								if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
							}
							LeftOutEdgeCount++;
						}

						if(LeftOutEdgeCount>0){
							if(LeftOutPoint!=NULL) delete[] LeftOutPoint;
							LeftOutPoint = new TFPoint[LeftOutEdgeCount];

							PixelCnt=0;
							for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
								if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
								if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

								if(FShape==shRectC ){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
									if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
								}
								LeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
								LeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
								PixelCnt++;
							}
						}
					}
					////////////////////////////////////////////////////////////////////

					// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
					if(LeftInEdgeCount>0 && LeftOutEdgeCount>0){
						// Total Left In Data 
						for(int k=0; k<LeftInEdgeCount; k++){
							if(AllLeftInEdgeCount>MAX_ALL_EDGE_COUNT) return false            ;
							AllLeftInData[AllLeftInEdgeCount].x = LeftInPoint[k].x            ;
							AllLeftInData[AllLeftInEdgeCount].y = LeftInPoint[k].y+ImageHeight;
							AllLeftInEdgeCount++;
						}
						// Total Left Out Data 
						for(int k=0; k<LeftOutEdgeCount; k++){
							if(AllLeftOutEdgeCount>MAX_ALL_EDGE_COUNT) return false              ;
							AllLeftOutData[AllLeftOutEdgeCount].x = LeftOutPoint[k].x            ;
							AllLeftOutData[AllLeftOutEdgeCount].y = LeftOutPoint[k].y+ImageHeight;
							AllLeftOutEdgeCount++;
						}
					}
				}
				else                   { //Cut���� ���� �Ƕ����� ������������ ���ϰ�� 
					IsFrameGrindMark = false                ;
					RealFrameStaY    = ImageHeight          ; //��üBuff���� Frame_Real��ǥ 
					RealFrameEndY    = ImageHeight+FrameImgH; //��üBuff���� Frame_Real��ǥ 

					// 							for(int k=0; k<2; k++){GrindStepy[k].x=0; GrindStepy[k].y=0;}
					for(int k=0; k<3; k++){
						GrindLine[k].LinePoint.x =0;
						GrindLine[k].LinePoint.y =0;
						GrindLine[k].IsGrindLine =false;
					}

					// ��üBuffer���� �˻� Frame�� GrindMark�����ϴ��� Ȯ�� �� üũ 
					for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
						if(GrindMark[k].DvX==0 || GrindMark[k].DvY==0) continue;
						if(((BuffLTPoint.y + GrindMark[k].DvY)>= RealFrameStaY) && ((BuffLTPoint.y + GrindMark[k].DvY)<= RealFrameEndY)){
							IsFrameGrindMark = true;

							//��ü Buffer���� ��ǥ 
							if((BuffLTPoint.y + GrindMark[k].DvY)<CutMinPos) CutMinPos =(BuffLTPoint.y + GrindMark[k].DvY);
							if((BuffLTPoint.y + GrindMark[k].DvY)>CutMaxPos) CutMaxPos =(BuffLTPoint.y + GrindMark[k].DvY); 
						}
					}

					// Frame���� ��ǥ�� ��ȯ 
					if(CutMinPos>0) CutMinPos = CutMinPos - ImageHeight;
					if(CutMaxPos>0) CutMaxPos = CutMaxPos - ImageHeight;

					// ��üBuffer���� ���� Frame�� GrindMark�� �����ϸ� ������ �и��Ͽ� �˻� ������ 
					if(IsFrameGrindMark){
						RoiCount =3;
						GrindLine[0].LinePoint.y = 0           ;
						GrindLine[0].IsGrindLine = false       ;
						GrindLine[1].LinePoint.y = ((CutMinPos-30)<0)?0:CutMinPos-30;
						GrindLine[1].IsGrindLine = true        ;
						GrindLine[2].LinePoint.y = ((CutMaxPos+30)>FrameImgH)?FrameImgH:(CutMaxPos+30);
						GrindLine[2].IsGrindLine = false       ;

						for(int k=0; k<RoiCount; k++){
							GrindThresHold =0;
							if(k==(RoiCount-1)){
								P1.y =GrindLine[k].LinePoint.y; P2.y = (int)(P1.y);
								// 										P3.y =(int)(FrameImgH- P1.y)  ; P4.y = (int)(P3.y);
								P3.y =(int)(FrameImgH      )  ; P4.y = (int)(P3.y);
							}
							else               {
								P1.y =GrindLine[k  ].LinePoint.y; P2.y = (int)(P1.y);
								// 										P3.y =GrindLine[k+1].LinePoint.y -GrindLine[k].LinePoint.y; P4.y = (int)(P3.y);
								P3.y =GrindLine[k+1].LinePoint.y; P4.y = (int)(P3.y);
							}
							GrindThresHold = (GrindLine[k].IsGrindLine)?LeftGrindThresHold:LeftCutThresHold;

							// 1�� Frame_Buff�� GrindMark Position���� ��Ȱ�Ͽ� ROI���������� �ʿ��� 
							if(k==0){StaLeftInEdgeCount=AllLeftInEdgeCount; StaLeftOutEdgeCount=AllLeftOutEdgeCount; }
							// GrindMark���� ���� ��Ȱ�� ������ �ʹ� ������� Polygon_Data �������� ���� 
							if((P3.y - P1.y)<15) continue;

							EasyImage::Threshold(GImage,&BinaryImg, GrindThresHold);
							// Median Filter ���� 
							// 									if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
							// 									if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) return false;
							// 									if((P1.x+(P2.x - P1.x))>=FrameImgW    || (P1.y+(P3.y - P1.y))>FrameImgH                      ) return false;

							if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) continue;
							if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) continue;
							if((P1.x+(P2.x - P1.x))> FrameImgW    || (P1.y+(P3.y - P1.y))>FrameImgH                      ) continue;


							FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
							FilterROI.Attach(&BinaryImg)                                          ;
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"AllGrind_Short==>CellId:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
							//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
							EasyImage::Median(&FilterROI, &FilterROI)                             ;

							Polygon->GImage = &BinaryImg                     ;
							Polygon->IWidth = FrameImgW                      ;
							Polygon->IHeight= FrameImgH                      ;
							Polygon->PoPa.FMinWidth  =2                      ;                       
							Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
							Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
							Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
							// 									Polygon->PoPa.MinPxCnt   =100                    ;
							Polygon->PoPa.MinPxCnt   =50                     ;
							Polygon->PoPa.MaxPxCnt   =20000                  ;
							Polygon->PoPa.MaxBbCnt   =10                     ;
							Polygon->PoPa.IsSeparate =true                   ;
							Polygon->PoPa.IsInToOut  =false                  ;
							Polygon->PoPa.HiThHold   = GrindThresHold        ;
							Polygon->PoPa.LoThHold   = 0                     ;
							Polygon->PoPa.IgInSide   = true                  ;
							Polygon->PoPa.IsHorSort  = false                 ;
							Polygon->PoPa.IsVerSort  = true                  ;
							Polygon->PoPa.IsInToLeft = true                  ;

							// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
							PolyP1[k].x = P1.x;  PolyP1[k].y = P1.y;
							PolyP2[k].x = P2.x;  PolyP2[k].y = P2.y;
							PolyP3[k].x = P3.x;  PolyP3[k].y = P3.y;
							PolyP4[k].x = P4.x;  PolyP4[k].y = P4.y;

							if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
								Polygon->OnExecute(P1,P2,P3,P4);
							}

							// 2���̻� ���� � ������ ����� �з� ���� 
							if(Polygon->BlobCount>1){
								// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
								PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
								PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
								for(int k=0; k<Polygon->BlobCount; k++){
									PolygonCenter.x = Polygon->Blob[k].Rect.l;
									PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
									// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
									RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
									if(RealPolyDis<MinPolyDis) {
										MinPolyDis  = RealPolyDis;
										nPolyPosCnt = k;
									}
								}
							}
							else                  {
								nPolyPosCnt=0;
// 								//���� ������ �ʱ�ȭ 
// 								if(FrameLeftInEdgeCount >0){ if(FrameLeftInPoint !=NULL) delete[] FrameLeftInPoint ; FrameLeftInEdgeCount =0;}
// 								if(FrameLeftOutEdgeCount>0){ if(FrameLeftOutPoint!=NULL) delete[] FrameLeftOutPoint; FrameLeftOutEdgeCount=0;}
							}

							// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
							////////////////////////////////////////////////////////////////////
							if(Polygon->Blob[nPolyPosCnt].InCnt>0){
								FrameLeftInEdgeCount=0;
								for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
									if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

									if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
										if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
									}
									else if(FShape==shRectC){
										if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
									}
									FrameLeftInEdgeCount++;
								}

								if(FrameLeftInEdgeCount>0){
									if(FrameLeftInPoint!=NULL) delete[] FrameLeftInPoint;
									FrameLeftInPoint = new TFPoint[FrameLeftInEdgeCount];

									PixelCnt=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

										if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
											if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
										}
										else if(FShape==shRectC){
											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
										}

										FrameLeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
										FrameLeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
										PixelCnt++;
									}
								}
							}

							if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
								FrameLeftOutEdgeCount=0;
								for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
									if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
									if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

									if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
										if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
									}
									FrameLeftOutEdgeCount++;
								}

								if(FrameLeftOutEdgeCount>0){
									if(FrameLeftOutPoint!=NULL) delete[] FrameLeftOutPoint;
									FrameLeftOutPoint = new TFPoint[FrameLeftOutEdgeCount];

									PixelCnt=0;
									for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
										if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

										if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
											if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
										}
										FrameLeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
										FrameLeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
										PixelCnt++;
									}
								}
							}
							////////////////////////////////////////////////////////////////////

							// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
							if(FrameLeftInEdgeCount>0 && FrameLeftOutEdgeCount>0){
								// Total Left In Data 
								for(int k=0; k<FrameLeftInEdgeCount; k++){
									if(AllLeftInEdgeCount>MAX_ALL_EDGE_COUNT) return false                 ;
									AllLeftInData[AllLeftInEdgeCount].x = FrameLeftInPoint[k].x            ;
									AllLeftInData[AllLeftInEdgeCount].y = FrameLeftInPoint[k].y+ImageHeight;
									AllLeftInEdgeCount++;
								}
								// Total Left Out Data 
								for(int k=0; k<FrameLeftOutEdgeCount; k++){
									if(AllLeftOutEdgeCount>MAX_ALL_EDGE_COUNT) return false                   ;
									AllLeftOutData[AllLeftOutEdgeCount].x = FrameLeftOutPoint[k].x            ;
									AllLeftOutData[AllLeftOutEdgeCount].y = FrameLeftOutPoint[k].y+ImageHeight;
									AllLeftOutEdgeCount++;
								}
							}
						}

						// Frame�� ROI�������� ��Ȱ�˻� ���� �� Frame�������� �ٽ� ������ �Ѱ���
						// Dark_Defect/Bright_Defect�˻� ���� 
						if((AllLeftInEdgeCount-StaLeftInEdgeCount)>0 && (AllLeftOutEdgeCount-StaLeftOutEdgeCount)>0){
							// LeftInEdgeData ���� ���� 
							LeftInEdgeCount = (AllLeftInEdgeCount-StaLeftInEdgeCount);
							if(LeftInPoint!=NULL) delete[] LeftInPoint;
							LeftInPoint = new TFPoint[LeftInEdgeCount];

							// LeftOutEdgeData ���� ���� 
							LeftOutEdgeCount = (AllLeftOutEdgeCount-StaLeftOutEdgeCount);
							if(LeftOutPoint!=NULL) delete[] LeftOutPoint;
							LeftOutPoint = new TFPoint[LeftOutEdgeCount];

							// ��ü �����Ϳ��� Ư������ �����͸� �Ѱ��� 
							for(int k=0; k<LeftInEdgeCount; k++){
								LeftInPoint[k].x   = AllLeftInData[StaLeftInEdgeCount+k].x             ;
								LeftInPoint[k].y   = AllLeftInData[StaLeftInEdgeCount+k].y -ImageHeight;
							}

							// ��ü �����Ϳ��� Ư������ �����͸� �Ѱ��� 
							for(int k=0; k<LeftOutEdgeCount; k++){
								LeftOutPoint[k].x   = AllLeftOutData[StaLeftOutEdgeCount+k].x             ;
								LeftOutPoint[k].y   = AllLeftOutData[StaLeftOutEdgeCount+k].y -ImageHeight;
							}
						}
					}
					else                { // GrindMark�������� ���� 
						EasyImage::Threshold(GImage,&BinaryImg, LeftCutThresHold);
						// Median Filter ���� 
						if(P1.x<=0         || P1.y< 0         || (P2.x - P1.x)<=0         || (P3.y - P1.y)<=0        ) return false;
						if(P1.x>=FrameImgW || P1.y>=FrameImgH || (P2.x - P1.x)>=FrameImgW || (P3.y - P1.y)> FrameImgH) return false;
						if((P1.x+(P2.x - P1.x))>FrameImgW     || (P1.y+(P3.y - P1.y))>FrameImgH                      ) return false;

						FilterROI.SetSize(int(P2.x - P1.x),int(P3.y - P1.y) )                 ;
						FilterROI.Attach(&BinaryImg)                                          ;
						FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
						LogUnit.SetLog(L"AllGrind_Short==>CellId:%d,CamID:%d,Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter",CellId, CamId, P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));     
						//LogUnit.SetLog("LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
						EasyImage::Median(&FilterROI, &FilterROI)                                 ;
						// 								if(FShape==shRectH) BinaryImg.Save("D:\\ROTATE_IMG\\Polygon_Left_CUTLine.bmp");

						Polygon->GImage = &BinaryImg                     ;
						Polygon->IWidth = FrameImgW                      ;
						Polygon->IHeight= FrameImgH                      ;
						Polygon->PoPa.FMinWidth  =2                      ;                       
						Polygon->PoPa.FMaxWidth  =(int) (P2.x - P1.x)    ;
						Polygon->PoPa.FMinHeight =(int)((P3.y - P1.y)/2) ;
						Polygon->PoPa.FMaxHeight =(int)((P3.y - P1.y)+10);
						Polygon->PoPa.MinPxCnt   =100                    ;
						Polygon->PoPa.MaxPxCnt   =20000                  ;
						Polygon->PoPa.MaxBbCnt   =10                     ;
						Polygon->PoPa.IsSeparate =true                   ;
						Polygon->PoPa.IsInToOut  =false                  ;
						Polygon->PoPa.HiThHold   = LeftCutThresHold      ;
						Polygon->PoPa.LoThHold   = 0                     ;
						Polygon->PoPa.IgInSide   = true                  ;
						Polygon->PoPa.IsHorSort  = false                 ;
						Polygon->PoPa.IsVerSort  = true                  ;
						Polygon->PoPa.IsInToLeft = true                  ;

						// ȭ�� ���÷��� ���� ���� ������ �������� �Ѱ��� 
						PolyP1[0].x = P1.x;  PolyP1[0].y = P1.y;
						PolyP2[0].x = P2.x;  PolyP2[0].y = P2.y;
						PolyP3[0].x = P3.x;  PolyP3[0].y = P3.y;
						PolyP4[0].x = P4.x;  PolyP4[0].y = P4.y;

						if(P1.x!=0 && P2.x!=0 && P3.x!=0 && P3.y!=0 && P4.x!=0 && P4.y!=0){
							Polygon->OnExecute(P1,P2,P3,P4);
						}

						// 2���̻� ���� � ������ ����� �з� ���� 
						if(Polygon->BlobCount>1){
							// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
							PolyRectCenter.x = 0.0f, PolyRectCenter.y =0.0f, nPolyPosCnt=0, RealPolyDis=0.0f, MinPolyDis=1000000.0f;
							PolyRectCenter.x = P1.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y)/2); 
							for(int k=0; k<Polygon->BlobCount; k++){
								PolygonCenter.x = Polygon->Blob[k].Rect.l;
								PolygonCenter.y = Polygon->Blob[k].Rect.t+(Polygon->Blob[k].Rect.b - Polygon->Blob[k].Rect.t)/2;
								// 								RealPolyDis = Math.GetLength(PolyRectCenter,PolygonCenter);
								RealPolyDis = fabs(PolyRectCenter.x - PolygonCenter.x);
								if(RealPolyDis<MinPolyDis) {
									MinPolyDis  = RealPolyDis;
									nPolyPosCnt = k;
								}
							}
						}
						else                  {
							nPolyPosCnt=0;
// 							//���� ������ �ʱ�ȭ 
// 							if(LeftInEdgeCount >0){ if(LeftInPoint !=NULL) delete[] LeftInPoint ; LeftInEdgeCount =0;}
// 							if(LeftOutEdgeCount>0){ if(LeftOutPoint!=NULL) delete[] LeftOutPoint; LeftOutEdgeCount=0;}
						}

						// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
						////////////////////////////////////////////////////////////////////
						if(Polygon->Blob[nPolyPosCnt].InCnt>0){
							LeftInEdgeCount=0;
							for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
								if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
								if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

								if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
									if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
								}
								else if(FShape==shRectC){
									if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
								}
								LeftInEdgeCount++;
							}

							if(LeftInEdgeCount>0){
								if(LeftInPoint!=NULL) delete[] LeftInPoint;
								LeftInPoint = new TFPoint[LeftInEdgeCount];

								PixelCnt=0;
								for(int k=0; k<Polygon->Blob[nPolyPosCnt].InCnt; k++){
									if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;

									if(FShape==shRectH){ // Hap �˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
										if(fabs(Polygon->Blob[nPolyPosCnt].TopPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
									}
									else if(FShape==shRectC){
										if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].InPoint[k].x)<=PolygonMarin) continue;
									}
									LeftInPoint[PixelCnt].x   = Polygon->Blob[nPolyPosCnt].InPoint[k].x ;
									LeftInPoint[PixelCnt].y   = Polygon->Blob[nPolyPosCnt].InPoint[k].y ;
									PixelCnt++;
								}
							}
						}

						if(Polygon->Blob[nPolyPosCnt].OutCnt>0){
							LeftOutEdgeCount=0;
							for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
								if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
								if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

								if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
									if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
								}
								LeftOutEdgeCount++;
							}

							if(LeftOutEdgeCount>0){
								if(LeftOutPoint!=NULL) delete[] LeftOutPoint;
								LeftOutPoint = new TFPoint[LeftOutEdgeCount];

								PixelCnt=0;
								for(int k=0; k<Polygon->Blob[nPolyPosCnt].OutCnt; k++){
									if(Polygon->Blob[nPolyPosCnt].TopPoint.y   ==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
									if(Polygon->Blob[nPolyPosCnt].BottomPoint.y==Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

									if(FShape==shRectC){ // Cup�˻�� ������ ���� ���̴� ��ǥ�� ���ؼ� �߰����� ���� �߰� 
										if(fabs(Polygon->Blob[nPolyPosCnt].BottomPoint.x - Polygon->Blob[nPolyPosCnt].OutPoint[k].x)<=PolygonMarin) continue;
									}
									LeftOutPoint[PixelCnt].x  = Polygon->Blob[nPolyPosCnt].OutPoint[k].x ;
									LeftOutPoint[PixelCnt].y  = Polygon->Blob[nPolyPosCnt].OutPoint[k].y ;
									PixelCnt++;
								}
							}
						}
						////////////////////////////////////////////////////////////////////

						// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
						if(LeftInEdgeCount>0 && LeftOutEdgeCount>0){
							// Total Left In Data 
							for(int k=0; k<LeftInEdgeCount; k++){
								if(AllLeftInEdgeCount>MAX_ALL_EDGE_COUNT) return false            ;
								AllLeftInData[AllLeftInEdgeCount].x = LeftInPoint[k].x            ;
								AllLeftInData[AllLeftInEdgeCount].y = LeftInPoint[k].y+ImageHeight;
								AllLeftInEdgeCount++;
							}
							// Total Left Out Data 
							for(int k=0; k<LeftOutEdgeCount; k++){
								if(AllLeftOutEdgeCount>MAX_ALL_EDGE_COUNT) return false              ;
								AllLeftOutData[AllLeftOutEdgeCount].x = LeftOutPoint[k].x            ;
								AllLeftOutData[AllLeftOutEdgeCount].y = LeftOutPoint[k].y+ImageHeight;
								AllLeftOutEdgeCount++;
							}
						}
					}
				}
			}
			else          {
				EasyImage::Threshold(GImage,&BinaryImg, LeftEdgeThresHold);
				if(LeftEdgeThresHold==0) {SingleLine->GImage = GImage;     SingleLine->FThreshold =5 ;}
				else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}
// 				GImage->Save("D:\\ROTATE_IMG\\WATCH_ORG_����.bmp");

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

				if(ROI_X<0 || ROI_W<0 || ROI_X>FrameImgW || ROI_W>FrameImgW || ROI_X+ROI_W>FrameImgW) return false;
				if(ROI_Y<0 || ROI_H<0 || ROI_Y>FrameImgH || ROI_H>FrameImgH || ROI_Y+ROI_H>FrameImgH) return false;

				SingleLine->SetOrientation(orWest);
				SingleLine->SetFindLineId(0);

				if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
				else                SingleLine->SetEdgeType(1);  // White To Black 

				SingleLine->OnExecute();
				Line[0].FP0 = SingleLine->GetResultP0();
				Line[0].FP1 = SingleLine->GetResultP1();

				// Btm,Top ������� ù��°
				LeftOutLine[0].FP0.x = Line[0].FP0.x;
				LeftOutLine[0].FP0.y = Line[0].FP0.y;
				LeftOutLine[0].FP1.x = Line[0].FP1.x;
				LeftOutLine[0].FP1.y = Line[0].FP1.y;

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
/*
void CFM3xLineDefect::OnExecuteGrind(TFPoint StaPos, TFPoint EndPos)
{
	int GrindMarkRange = 40;
	int FrameImgW   = GImage->GetWidth();
	int FrameImgH   = GImage->GetHeight();

	double deg = 0.0f;
	double Orgdeg = 0.0f;
	double Newdeg = 0.0f;

	TFPoint Target(0.0f,0.0f);
	// DvX,DvY �������� �̹��� ȸ�� �������� �����ϱ� ���ؼ� �ʿ��� ���� 
	TFPoint OrgMarkDv; TFPoint NewMarkDv; 

	double GrindMeaDis =0.0f, MinDis =10000.0f, TotalCirDis=0.0f,GrindWidthDis=0.0f;
	int GrindMarkOutCnt=0, GrindMarkInCnt=0, PreGrindMarkOutCnt=0, PreGrindMarkInCnt=0,AllOutCnt=0,AllInCnt=0, CircleCount=0;

	Orgdeg = dSideDeg;
	if(StaPos.x!=0.0f && StaPos.y!=0.0f && EndPos.x!=0.0f && EndPos.y!=0.0f) deg = (Math.GetDegree(StaPos.x , StaPos.y, EndPos.x, EndPos.y));
	Newdeg = deg - Orgdeg;

	// GrindMark DvX,DvY�������� ���� ���� 
	for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
		if(GrindMark[k].DvX==0 || GrindMark[k].DvY==0) continue;

// 		Target  = Math.OnFindPerpendicularPointXY(StaPos,(float)GrindMark[k].DvX ,(float)GrindMark[k].DvY,deg);
		// Ư�� Buffer���� GrindMark Find������ 
		OrgMarkDv.x = (float)GrindMark[k].DvX; OrgMarkDv.y = (float)GrindMark[k].DvY;
		NewMarkDv   = Math.OnRotate(FLTPoint , OrgMarkDv, Newdeg);  

		GrindMark[k].DvX = (int)NewMarkDv.x; GrindMark[k].DvY = (int)NewMarkDv.y; 

		Target.x = StaPos.x ; Target.y = 0  ;
		Target.x = Target.x+GrindMark[k].DvX;
		Target.y = Target.y+GrindMark[k].DvY;

		GrindMark[k].Rect.left   = Target.x - GrindMarkRange;
		GrindMark[k].Rect.top    = Target.y - GrindMarkRange;
		GrindMark[k].Rect.right  = GrindMark[k].Rect.left+(GrindMarkRange*2); 
		GrindMark[k].Rect.bottom = GrindMark[k].Rect.top +(GrindMarkRange*2); 
	}

	// GrindMark Find �� ����,������ ���� 
	Find->OnLoad(true,FindDir)                     ;
	Find->Image  = GImage                          ;
	Find->SetContrastMode(EFindContrastMode_Normal);
	Find->SetMinScore (GrindMarkMinScore)          ; // PatternrFind �˻� �ּ� Score
	Find->SetZeroScore(GrindMarkMinScore)          ; // �˻� �� �ּ� Score 
	Find->SetMaxInstances(1)                       ;
	Find->SetAngleTolerance(20.0f)                 ;
	Find->SetScaleBias(1.00f)                      ;
	Find->SetScaleTolerance(0.0f)                  ;
	Find->Property.Ox = (float)FrameImgW/2              ;
	Find->Property.Oy = (float)FrameImgH/2              ;

	int GrindRectWidth=0, GrindRectHeight=0;
	AllOutCnt = AllLeftOutEdgeCount; // ������� ��ü ������ ���� � Total Out EdgePoint Count
	AllInCnt  = AllLeftInEdgeCount ; // ������� ��ü ������ ���� � Total In  EdgePoint Count
	
	if(AllOutCnt!=0 && AllInCnt!=0){
		for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
			if(GrindMark[k].Rect.left==0 || GrindMark[k].Rect.top==0 || GrindMark[k].Rect.right==0 || GrindMark[k].Rect.bottom==0) continue;
			GrindRectWidth  = (GrindMark[k].Rect.right  - GrindMark[k].Rect.left);
			GrindRectHeight = (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top );

			Find->IRoi->SetPlacement(GrindMark[k].Rect.left, GrindMark[k].Rect.top, GrindRectWidth, GrindRectHeight);
			if(GrindMark[k].Rect.left<=0    || GrindMark[k].Rect.top<=0    || GrindRectWidth<=0    || GrindRectHeight<=0   ) continue;
			if(GrindMark[k].Rect.left>=FrameImgW || GrindMark[k].Rect.top>=FrameImgH || GrindRectWidth>=FrameImgW || GrindRectHeight>=FrameImgH) continue;

			deg = GrindMark[k].degree;
			Find->SetAngleBias(deg);

			// GrindMark Search 
			if(Find->OnExecute()){
				GrindMark[k].Point.x = Find->Result[0].Px;
				GrindMark[k].Point.y = Find->Result[0].Py;
				GrindMark[k].Score   = Find->Result[0].Score;
			}

			// ������,������ ��� 
			if(GrindMark[k].Point.x!=0.0f && GrindMark[k].Point.y!=0.0f){ // GrindMark ã�������
				// ��ü Buffer���� ��ǥ��� �ٽ��� 
				GrindMark[k].Point.y = GrindMark[k].Point.y+ StaPos.y; 

				// GrindMark���� OutData �ִܰŸ� Count���� Calc 
				GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
				for(int i=PreGrindMarkOutCnt; i<AllOutCnt; i++){
					if(AllLeftOutData[i].x   ==0.0f || AllLeftOutData[i].y  ==0.0f) continue;

					GrindMeaDis = Math.GetLength(GrindMark[k].Point, AllLeftOutData[i]);
					if(GrindMeaDis<MinDis){
						MinDis = GrindMeaDis;
						GrindMarkOutCnt = i;
						PreGrindMarkOutCnt = GrindMarkOutCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
					}
				}
				// OutData���� InData �ִܰŸ� Count���� Calc 
				if(GrindMarkOutCnt!=0){
					GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
					for(int i=PreGrindMarkInCnt; i<AllInCnt; i++){
						if(AllLeftInData [i].x              ==0.0f || AllLeftInData [i].y              ==0.0f) continue;
						if(AllLeftOutData[GrindMarkOutCnt].x==0.0f || AllLeftOutData[GrindMarkOutCnt].y==0.0f) continue;

						GrindMeaDis = Math.GetLength(AllLeftOutData[GrindMarkOutCnt], AllLeftInData[i]);
						if(GrindMeaDis<MinDis){
							MinDis = GrindMeaDis;
							GrindMarkInCnt = i;
							PreGrindMarkInCnt = GrindMarkInCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
						}
					}
				}

				// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
				if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5){
					CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
					for(int i=GrindMarkOutCnt-5; i<GrindMarkOutCnt+5; i++){
						if(AllLeftOutData[i].x==0.0f || AllLeftOutData[i].y==0.0f) continue;

						GrindMeaDis = Math.GetLength(GrindMark[k].Point, AllLeftOutData[i]);
						TotalCirDis+= GrindMeaDis;
						CircleCount++;
					}
					if(TotalCirDis!=0.0f && CircleCount!=0){
						GrindMark[k].GrindMea = TotalCirDis/CircleCount;
					}
				}

				// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
				if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
					CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
					for(int i=-5; i<5; i++){
						if(AllLeftOutData[GrindMarkOutCnt+i].x==0.0f && AllLeftOutData[GrindMarkOutCnt+i].y==0.0f) continue;
						if(AllLeftInData [GrindMarkInCnt +i].x==0.0f && AllLeftInData [GrindMarkInCnt +i].y==0.0f) continue;
						GrindWidthDis = Math.GetLength(AllLeftInData[GrindMarkInCnt +i], AllLeftOutData  [GrindMarkOutCnt+i]);
						TotalCirDis+=   GrindWidthDis;
						CircleCount++;
					}
					if(TotalCirDis!=0.0f && CircleCount!=0){
						GrindMark[k].GrindWidth  = TotalCirDis/CircleCount;
					}
				}
				// ȭ�� ���÷��� �������� �ٽ� ����� 
				GrindMark[k].Point.y = GrindMark[k].Point.y - StaPos.y; 
			}
			else                                                        { // GrindMark ��ã�����
				// OutData���� �ּҰ� ������ Count���� 
				GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
				for(int i=PreGrindMarkOutCnt; i<AllOutCnt; i++){
					if(AllLeftOutData[i].x==0.0f || AllLeftOutData[i].y==0.0f) continue;

					GrindMark[k].RectPoint.x = GrindMark[k].Rect.left + (GrindMark[k].Rect.right  - GrindMark[k].Rect.left)/2 ; 
					GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2+StaPos.y; 
// 					GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2 ; 

					GrindMeaDis = Math.GetLength(GrindMark[k].RectPoint, AllLeftOutData[i]);
					if(GrindMeaDis<MinDis){
						MinDis = GrindMeaDis;
						GrindMarkOutCnt = i;
						PreGrindMarkOutCnt = GrindMarkOutCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
					}
				}

				// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
				if(GrindMarkOutCnt!=0){
					GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
					for(int i=PreGrindMarkInCnt; i<AllInCnt; i++){
						if(AllLeftInData [i].x              ==0.0f || AllLeftInData [i].y               ==0.0f) continue;
						if(AllLeftOutData[GrindMarkOutCnt].x==0.0f || AllLeftOutData[GrindMarkOutCnt].y ==0.0f) continue;

						GrindMeaDis = Math.GetLength(AllLeftOutData[GrindMarkOutCnt], AllLeftInData[i]);
						if(GrindMeaDis<MinDis){
							MinDis = GrindMeaDis;
							GrindMarkInCnt = i;
						}
					}
				}

				// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
				if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
					CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
					for(int i=-5; i<5; i++){
						if(AllLeftOutData[GrindMarkOutCnt+i].x==0.0f && AllLeftOutData[GrindMarkOutCnt+i].y==0.0f) continue;
						if(AllLeftInData [GrindMarkInCnt +i].x==0.0f && AllLeftInData [GrindMarkInCnt +i].y==0.0f) continue;
						GrindWidthDis = Math.GetLength(AllLeftOutData[GrindMarkInCnt+i], AllLeftInData[GrindMarkOutCnt+i]);
						TotalCirDis+=   GrindWidthDis;
						CircleCount++;
					}
					if(TotalCirDis!=0.0f && CircleCount!=0){
						GrindMark[k].GrindWidth = TotalCirDis/CircleCount;
					}
				}
			}
		}
	}
}
*/
void CFM3xLineDefect::OnExecuteGrind(TFPoint StaPos, TFPoint EndPos, int InspMode)
{
	int FrameImgW   = GImage->GetWidth();
	int FrameImgH   = GImage->GetHeight();

	double deg     = 0.0f;
	double Orgdeg  = 0.0f;
	double Newdeg  = 0.0f;
	int MaxGrindID = 0;
	int MarkID     = 0;

	// ASide EnCap Pattern Distance ���� ���� 
	TFPoint StaPatern(0.0f,0.0f);
	TFPoint EndPatern(0.0f,0.0f);
// 	const int PatternMagin = 35;
// 	const int PatternLength=100;
	const int PatternMagin =35;
	int PatternLength;

	// ASide Align Mart To Edge ���� ����
	TFPoint StaMarkToEdge(0.0f,0.0f);
	TFPoint EndMarkToEdge(0.0f,0.0f);
	TFPoint CellHalfPos(0.0f,0.0f);

	const int MarkToEdgeMargin =35;
	int MarkToEdgeLength;
	double MarkToEdgeDeg=0.0f;

	// Frame���� ���� ������� ���ս� Overlap�� �˻� ������� Buffer������ ���� ���ؼ� 
	// Polygon ������ Ƣ�� ���� ������ �߻�,������ Ƣ�� ���� �������� ���� 
	const int XPosMargin = 20; const int YPosMargin =20;

	TFPoint Target(0.0f,0.0f);
	// DvX,DvY �������� �̹��� ȸ�� �������� �����ϱ� ���ؼ� �ʿ��� ���� 
	TFPoint OrgMarkDv; TFPoint NewMarkDv; 

	// EdgePoint Data ������ 
	TFPoint   *AllOutPoint;
	TFPoint   *AllInPoint ;

	double GrindMeaDis =0.0f, MinDis =10000.0f, TotalCirDis=0.0f,GrindWidthDis=0.0f;
	int GrindMarkOutCnt=0, GrindMarkInCnt=0, PreGrindMarkOutCnt=0, PreGrindMarkInCnt=0,AllOutCnt=0,AllInCnt=0, CircleCount=0;

	Orgdeg = dSideDeg;
	if(StaPos.x!=0.0f && StaPos.y!=0.0f && EndPos.x!=0.0f && EndPos.y!=0.0f) deg = (Math.GetDegree(StaPos.x , StaPos.y, EndPos.x, EndPos.y));
	Newdeg = deg - Orgdeg;

	// InspMode �������� �з� InspMode: 0 ==>��ü����(����      ,����) 
	// InspMode:1                         ==>�κп���(Circle����,����) 
	// InspMode:2                         ==>�κп���(Corner����,����)
	if     (InspMode==0){ AllOutPoint = AllLeftOutData  ; AllInPoint = AllLeftInData  ; AllOutCnt=AllLeftOutEdgeCount; AllInCnt = AllLeftInEdgeCount;}
	else if(InspMode==1){ AllOutPoint = AllCircleOutData; AllInPoint = AllCircleInData; AllOutCnt=AllCirOutEdgeCount ; AllInCnt = AllCirInEdgeCount ;}
	else if(InspMode==2){ AllOutPoint = AllCornerOutData; AllInPoint = AllCornerInData; AllOutCnt=AllCorOutEdgeCount ; AllInCnt = AllCorInEdgeCount ;}

	// GrindMark DvX,DvY�������� ���� ����
	// GrindMark Range �����Ͽ� GrindRect���� ���� 
	for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
		if(GrindMark[k].DvX==0 || GrindMark[k].DvY==0) continue;

		// Ư�� Buffer���� GrindMark Find������ 
		if(FGlassType==Mobile_Glass){ //Mobile =>������ �������� ���� �����Ͽ� ������ 
			OrgMarkDv.x      = (float)GrindMark[k].DvX; OrgMarkDv.y = (float)GrindMark[k].DvY;
			NewMarkDv        = Math.OnRotate(FLTPoint , OrgMarkDv, Newdeg);  
			GrindMark[k].DvX = (int)NewMarkDv.x; GrindMark[k].DvY = (int)NewMarkDv.y; 
		}

		Target.x = StaPos.x ; Target.y = 0  ;
		Target.x = Target.x+GrindMark[k].DvX;
		Target.y = Target.y+GrindMark[k].DvY;

		GrindMark[k].Rect.left   = Target.x - GrindMark[k].Range;
		GrindMark[k].Rect.top    = Target.y - GrindMark[k].Range;
		GrindMark[k].Rect.right  = GrindMark[k].Rect.left+(GrindMark[k].Range*2); 
		GrindMark[k].Rect.bottom = GrindMark[k].Rect.top +(GrindMark[k].Range*2); 

		if(GrindMark[k].MarkID>MaxGrindID) MaxGrindID = GrindMark[k].MarkID; 
	}

	// GrindMark Find �� ����,������ ���� 
// 	Find->OnLoad(true,FindDir)        ;
	// ������ Multi_Teaching ������ŭ ������ �ε� �� �⺻�� ������ 
	for(int k=0; k<(MaxGrindID+1); k++){
		if(k>=MAX_GRINDMARK_ID) continue;
		MarkFind[k].OnLoad(true,FindDir,k);

		MarkFind[k].GImage = GImage                           ;
		MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
		MarkFind[k].SetMinScore (GrindMarkMinScore)          ; // PatternrFind �˻� �ּ� Score
		MarkFind[k].SetZeroScore(GrindMarkMinScore)          ; // �˻� �� �ּ� Score 
		MarkFind[k].SetMaxInstances(1)                       ;
		MarkFind[k].SetAngleTolerance(20.0f)                 ;
		MarkFind[k].SetScaleBias(1.00f)                      ;
		MarkFind[k].SetScaleTolerance(0.0f)                  ;
		MarkFind[k].Property.Ox = (float)FrameImgW/2         ;
		MarkFind[k].Property.Oy = (float)FrameImgH/2         ;
	}

	int GrindRectWidth=0, GrindRectHeight=0;
	// 	AllOutCnt = AllLeftOutEdgeCount; // ������� ��ü ������ ���� � Total Out EdgePoint Count
	// 	AllInCnt  = AllLeftInEdgeCount ; // ������� ��ü ������ ���� � Total In  EdgePoint Count
	double Patterndeg=0.0f; TFPoint PatternPos(0.0f,0.0f); 
//    GImage->Save("D:\\ROTATE_IMG\\AllBuffer.bmp")        ;
	if(AllOutCnt!=0 && AllInCnt!=0){
		for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
			if(GrindMark[k].Rect.left==0 || GrindMark[k].Rect.top==0 || GrindMark[k].Rect.right==0 || GrindMark[k].Rect.bottom==0) continue;
			GrindRectWidth  = (GrindMark[k].Rect.right  - GrindMark[k].Rect.left);
			GrindRectHeight = (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top );
			MarkID          =  GrindMark[k].MarkID; // Multi Teaching MarkID 

// 			Find->IRoi->SetPlacement(GrindMark[k].Rect.left, GrindMark[k].Rect.top, GrindRectWidth, GrindRectHeight);
//			if(FGlassType==Mobile_Glass){ //HSKIM VR CELL�� �з�
			// GrindMark Search 
			for(int j=0; j<MaxGrindID+1; j++){
				MarkFind[j].IRoi->SetPlacement(GrindMark[k].Rect.left, GrindMark[k].Rect.top, GrindRectWidth, GrindRectHeight);
				if(GrindMark[k].Rect.left<=0         || GrindMark[k].Rect.top<=0         || GrindRectWidth<=0         || GrindRectHeight<=0        ) continue;
				if(GrindMark[k].Rect.left>=FrameImgW || GrindMark[k].Rect.top>=FrameImgH || GrindRectWidth>=FrameImgW || GrindRectHeight>=FrameImgH) continue;
				if(GrindMark[k].Point.x!=0.0f && GrindMark[k].Point.y!=0.0f) continue;

				deg = GrindMark[k].degree;
				// 			Find->SetAngleBias(deg);
				MarkFind[j].SetAngleBias(deg);
				GrindMark[k].Score =0.0f;
				
				if(MarkFind[j].OnExecute()){ //GrindMark Search OK 
					GrindMark[k].Score = MarkFind[j].Result[0].Score;
					if(PatternOffSet==0){
						GrindMark[k].Point.x = MarkFind[j].Result[0].Px;
						GrindMark[k].Point.y = MarkFind[j].Result[0].Py;
						GrindMark[k].Score   = MarkFind[j].Result[0].Score;
					}
					else {
						if(k==4)      { //ù��° AlignMark 
							GrindMark[k].Point.x = MarkFind[j].Result[0].Px   ;
							GrindMark[k].Point.y = MarkFind[j].Result[0].Py   ;
							GrindMark[k].Score   = MarkFind[j].Result[0].Score;

							PatternPos.x         = MarkFind[j].Result[0].Px              ;
							PatternPos.y         = MarkFind[j].Result[0].Py+PatternOffSet;

							Patterndeg = Math.GetDegree(StaPos,EndPos);
							PatternPos = Math.OnRotate (GrindMark[k].Point,PatternPos,Patterndeg); // �ι�° AlignMark ������ ������ ȸ�� �� ��ǥ���� 

							GrindMark[k].Point.x = PatternPos.x;
							GrindMark[k].Point.y = PatternPos.y;

							GrindMark[k].Rect.top    = GrindMark[k].Rect.top   +PatternOffSet; 
							GrindMark[k].Rect.bottom = GrindMark[k].Rect.bottom+PatternOffSet; 
						}
						else if(k==18){ //�ι�° AlignMark 
							GrindMark[k].Point.x = MarkFind[j].Result[0].Px   ;
							GrindMark[k].Point.y = MarkFind[j].Result[0].Py   ;
							GrindMark[k].Score   = MarkFind[j].Result[0].Score;

							PatternPos.x         = MarkFind[j].Result[0].Px              ;
							PatternPos.y         = MarkFind[j].Result[0].Py-PatternOffSet;

							Patterndeg = Math.GetDegree(EndPos,StaPos);
							PatternPos = Math.OnRotate (GrindMark[k].Point,PatternPos,-Patterndeg); // �ι�° AlignMark ������ ������ ȸ�� �� ��ǥ���� 

							GrindMark[k].Point.x = PatternPos.x;
							GrindMark[k].Point.y = PatternPos.y;

							GrindMark[k].Rect.top    = GrindMark[k].Rect.top   -PatternOffSet; 
							GrindMark[k].Rect.bottom = GrindMark[k].Rect.bottom-PatternOffSet; 
						}
						else {
							GrindMark[k].Point.x = MarkFind[j].Result[0].Px;
							GrindMark[k].Point.y = MarkFind[j].Result[0].Py;
							GrindMark[k].Score   = MarkFind[j].Result[0].Score;
						}
					}
				}
				else                            { //GrindMark Search NG
					if(PatternOffSet==0){
						GrindMark[k].RectPoint.x = GrindMark[k].Rect.left + (GrindMark[k].Rect.right  - GrindMark[k].Rect.left)/2 ; 
						GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2+StaPos.y; 
					}
					else {
						if(j==MaxGrindID){
							if(k==4)      { //ù��° AlignMark 
								GrindMark[k].Rect.top    = GrindMark[k].Rect.top   +PatternOffSet; 
								GrindMark[k].Rect.bottom = GrindMark[k].Rect.bottom+PatternOffSet; 
								GrindMark[k].RectPoint.x = GrindMark[k].Rect.left + (GrindMark[k].Rect.right  - GrindMark[k].Rect.left)/2 ; 
								GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2+StaPos.y; 
							}
							else if(k==18){ //�ι�° AlignMark 
								GrindMark[k].Rect.top    = GrindMark[k].Rect.top   -PatternOffSet; 
								GrindMark[k].Rect.bottom = GrindMark[k].Rect.bottom-PatternOffSet; 
								GrindMark[k].RectPoint.x = GrindMark[k].Rect.left + (GrindMark[k].Rect.right  - GrindMark[k].Rect.left)/2 ; 
								GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2+StaPos.y; 
							}
							else {
								GrindMark[k].RectPoint.x = GrindMark[k].Rect.left + (GrindMark[k].Rect.right  - GrindMark[k].Rect.left)/2 ; 
								GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2+StaPos.y; 
							}
						}
					}
				}
 			}
//			}
			/*
			else                        {//HSKIM VR CELL Mark Find
				for(int j=0; j<MaxGrindID+1; j++){
					if(GrindMark[k].Rect.left==0 || GrindMark[k].Rect.top==0 || GrindMark[k].Rect.right==0 || GrindMark[k].Rect.bottom==0) continue;
					GrindRectWidth  = (GrindMark[k].Rect.right  - GrindMark[k].Rect.left);
					GrindRectHeight = (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top );
					MarkID          =  j                                                 ;

					MarkFind[MarkID].IRoi->SetPlacement(GrindMark[k].Rect.left, GrindMark[k].Rect.top, GrindRectWidth, GrindRectHeight);
					if(GrindMark[k].Rect.left<=0         || GrindMark[k].Rect.top<=0         || GrindRectWidth<=0         || GrindRectHeight<=0        ) continue;
					if(GrindMark[k].Rect.left>=FrameImgW || GrindMark[k].Rect.top>=FrameImgH || GrindRectWidth>=FrameImgW || GrindRectHeight>=FrameImgH) continue;

					deg = GrindMark[k].degree;
					MarkFind[MarkID].SetAngleBias(deg);
					GrindMark[k].Score =0.0f;
			
					if(MarkFind[MarkID].OnExecute()){
						GrindMark[k].Point.x = MarkFind[MarkID].Result[0].Px;
						GrindMark[k].Point.y = MarkFind[MarkID].Result[0].Py;
						GrindMark[k].Score   = MarkFind[MarkID].Result[0].Score;
						break;	
					}
					else                            {
						//GrindMark Find ���� �� �̹��� ���� ����
						GrindMark[k].RectPoint.x = GrindMark[k].Rect.left + (GrindMark[k].Rect.right  - GrindMark[k].Rect.left)/2 ; 
						GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2+StaPos.y; 
					}
				}
			}
			*/

			// ������,������ ��� 
			if(GrindMark[k].Point.x!=0.0f && GrindMark[k].Point.y!=0.0f){ // GrindMark ã�������
				// ��ü Buffer���� ��ǥ��� �ٽ��� 
				GrindMark[k].Point.y = GrindMark[k].Point.y+ StaPos.y; 

				// GrindMark���� OutData �ִܰŸ� Count���� Calc 
				GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
				for(int i=PreGrindMarkOutCnt; i<AllOutCnt; i++){
					if(AllOutPoint[i].x   ==0.0f || AllOutPoint[i].y  ==0.0f) continue;

					GrindMeaDis = Math.GetLength(GrindMark[k].Point, AllOutPoint[i]);
					if(GrindMeaDis<MinDis){
						MinDis = GrindMeaDis;
						GrindMarkOutCnt = i;
						PreGrindMarkOutCnt = GrindMarkOutCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
					}
				}
				// OutData���� InData �ִܰŸ� Count���� Calc 
				if(GrindMarkOutCnt!=0){
					GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
					for(int i=PreGrindMarkInCnt; i<AllInCnt; i++){
						if(AllInPoint [i].x              ==0.0f || AllInPoint [i].y              ==0.0f) continue;
						if(AllOutPoint[GrindMarkOutCnt].x==0.0f || AllOutPoint[GrindMarkOutCnt].y==0.0f) continue;

						GrindMeaDis = Math.GetLength(AllOutPoint[GrindMarkOutCnt], AllInPoint[i]);
						if(GrindMeaDis<MinDis){
							MinDis = GrindMeaDis;
							GrindMarkInCnt = i;
							PreGrindMarkInCnt = GrindMarkInCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
						}
					}
				}

				// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
				if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5){
					CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
					for(int i=GrindMarkOutCnt-5; i<GrindMarkOutCnt+5; i++){
						if(AllOutPoint[i].x==0.0f || AllOutPoint[i].y==0.0f) continue;
						// ������ Ƣ�� ���� ���� ���� 
						if(fabs(AllOutPoint[GrindMarkOutCnt].x - AllOutPoint[i].x)>= XPosMargin) continue;
						if(fabs(AllOutPoint[GrindMarkOutCnt].y - AllOutPoint[i].y)>= YPosMargin) continue;

						GrindMeaDis = Math.GetLength(GrindMark[k].Point, AllOutPoint[i]);
						TotalCirDis+= GrindMeaDis;
						CircleCount++;
					}
					if(TotalCirDis!=0.0f && CircleCount!=0){
						GrindMark[k].GrindMea = TotalCirDis/CircleCount;
					}
				}

				// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
				if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
					CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
					for(int i=-5; i<5; i++){
						if(AllOutPoint[GrindMarkOutCnt+i].x==0.0f && AllOutPoint[GrindMarkOutCnt+i].y==0.0f) continue;
						if(AllInPoint [GrindMarkInCnt +i].x==0.0f && AllInPoint [GrindMarkInCnt +i].y==0.0f) continue;

						// ������ Ƣ�� ���� ���� ���� 
						if(fabs(AllOutPoint[GrindMarkOutCnt].x - AllOutPoint[GrindMarkOutCnt+i].x)>= XPosMargin) continue;
						if(fabs(AllOutPoint[GrindMarkOutCnt].y - AllOutPoint[GrindMarkOutCnt+i].y)>= YPosMargin) continue;
						if(fabs(AllInPoint [GrindMarkInCnt ].x - AllInPoint [GrindMarkInCnt+i ].x)>= XPosMargin) continue;
						if(fabs(AllInPoint [GrindMarkInCnt ].y - AllInPoint [GrindMarkInCnt+i ].y)>= YPosMargin) continue;

						GrindWidthDis = Math.GetLength(AllInPoint[GrindMarkInCnt +i], AllOutPoint  [GrindMarkOutCnt+i]);
						TotalCirDis+=   GrindWidthDis;
						CircleCount++;
					}
					if(TotalCirDis!=0.0f && CircleCount!=0){
						GrindMark[k].GrindWidth  = TotalCirDis/CircleCount;
					}
				}
				// ȭ�� ���÷��� �������� �ٽ� ����� 
				GrindMark[k].Point.y = GrindMark[k].Point.y - StaPos.y; 
			}
			else                                                        { // GrindMark ��ã�����
				// OutData���� �ּҰ� ������ Count���� 
				GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;	
				for(int i=PreGrindMarkOutCnt; i<AllOutCnt; i++){
					if(AllOutPoint[i].x==0.0f || AllOutPoint[i].y==0.0f) continue;

// 					GrindMark[k].RectPoint.x = GrindMark[k].Rect.left + (GrindMark[k].Rect.right  - GrindMark[k].Rect.left)/2 ; 
// 					GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2+StaPos.y; 
// 					GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2 ; 

					GrindMeaDis = Math.GetLength(GrindMark[k].RectPoint, AllOutPoint[i]);
					if(GrindMeaDis<MinDis){
						MinDis = GrindMeaDis;
						GrindMarkOutCnt = i;
						PreGrindMarkOutCnt = GrindMarkOutCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
					}
				}

				// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
				if(GrindMarkOutCnt!=0){
					GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
					for(int i=PreGrindMarkInCnt; i<AllInCnt; i++){
						if(AllInPoint [i].x              ==0.0f || AllInPoint [i].y               ==0.0f) continue;
						if(AllOutPoint[GrindMarkOutCnt].x==0.0f || AllOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

						GrindMeaDis = Math.GetLength(AllOutPoint[GrindMarkOutCnt], AllInPoint[i]);
						if(GrindMeaDis<MinDis){
							MinDis = GrindMeaDis;
							GrindMarkInCnt = i;
							PreGrindMarkInCnt = GrindMarkInCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
						}
					}
				}

				// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
				if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
					CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
					for(int i=-5; i<5; i++){
						if(AllOutPoint[GrindMarkOutCnt+i].x==0.0f && AllOutPoint[GrindMarkOutCnt+i].y==0.0f) continue;
						if(AllInPoint [GrindMarkInCnt +i].x==0.0f && AllInPoint [GrindMarkInCnt +i].y==0.0f) continue;

						// ������ Ƣ�� ���� ���� ���� 
						if(fabs(AllOutPoint[GrindMarkOutCnt].x - AllOutPoint[GrindMarkOutCnt+i].x)>= XPosMargin) continue;
						if(fabs(AllOutPoint[GrindMarkOutCnt].y - AllOutPoint[GrindMarkOutCnt+i].y)>= YPosMargin) continue;
						if(fabs(AllInPoint [GrindMarkInCnt ].x - AllInPoint [GrindMarkInCnt+i ].x)>= XPosMargin) continue;
						if(fabs(AllInPoint [GrindMarkInCnt ].y - AllInPoint [GrindMarkInCnt+i ].y)>= YPosMargin) continue;

						GrindWidthDis = Math.GetLength(AllInPoint[GrindMarkInCnt+i], AllOutPoint[GrindMarkOutCnt+i]);
						TotalCirDis+=   GrindWidthDis;
						CircleCount++;
					}
					if(TotalCirDis!=0.0f && CircleCount!=0){
						GrindMark[k].GrindWidth = TotalCirDis/CircleCount;
					}
				}
			}
		}

		// Y1,Y2,Y3 Size���� ���� ������ ���� 
		if(IsShortCell && CamId==0){
			LeftGrindToBtmHeight = (GrindMark[4 ].GrindMea!=0.0f)?GrindMark[4 ].GrindMea:0.0f;
			CenGrindToBtmHeight  = (GrindMark[11].GrindMea!=0.0f)?GrindMark[11].GrindMea:0.0f;
			RightGrindBtmHeight  = (GrindMark[18].GrindMea!=0.0f)?GrindMark[18].GrindMea:0.0f;
		}
	}

	//20200624 �κ� ���� ���� ��� Y ������ ���� �߰� HSKIM
	// Mobile or VR ���� ����� 
	if(IsShortCell && CamId==2 && !IsAllGrind){
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
		EasyImage::Threshold(GImage,&BinaryImg, LeftEdgeThresHold);
			
//		BinaryImg.Save("D:\\ROTATE_IMG\\ASideMarkToEdge.bmp");
 		for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
			if(k!=4 && k!=11 && k!=18) continue;
 			if(k==11 &&
			   GrindMark[4].Point.x  != 0.0  && GrindMark[4].Point.y  != 0.0f &&
			   GrindMark[18].Point.x != 0.0f && GrindMark[18].Point.y != 0.0f    ){ // �� ���ʹ� + ��ũ�� ���� ������ �־���
				GrindMark[11].Point.x=(GrindMark[11].Rect.left+GrindMark[11].Rect.right )/2;
				GrindMark[11].Point.y=(GrindMark[11].Rect.top +GrindMark[11].Rect.bottom)/2;
 			}
			if(GrindMark[k].Point.x==0.0f || GrindMark[k].Point.y==0.0f) continue;

			MarkToEdgeLength = ((GrindMark[k].Point.x - ((StaPos.x+EndPos.x)/2))+100);

			SingleLine->IRoi->SetOrgX  (GrindMark[k].Point.x - MarkToEdgeLength);
			SingleLine->IRoi->SetOrgY  (GrindMark[k].Point.y - MarkToEdgeMargin);
			SingleLine->IRoi->SetWidth (MarkToEdgeLength                       );
			SingleLine->IRoi->SetHeight(MarkToEdgeMargin*2                     );
		
			if(((GrindMark[k].Point.x - MarkToEdgeLength)<=0)    || ((GrindMark[k].Point.y - MarkToEdgeMargin)<=0)) return;

			SingleLine->SetOrientation(orEast);
			SingleLine->FIncrement =1;
			SingleLine->SetFindLineId(0); 
			SingleLine->SetEdgeType(1);  // White To Black 
			SingleLine->OnExecute();

			StaMarkToEdge.x =0.0f, StaMarkToEdge.y=0.0f,EndMarkToEdge.x=0.0f, EndMarkToEdge.y=0.0f;
			StaMarkToEdge = SingleLine->GetResultP0();
			EndMarkToEdge = SingleLine->GetResultP1();

			if(k==4){
				LeftMarkToTopHeight=(Math.GetLengthFrLineToPoint(StaMarkToEdge, EndMarkToEdge, GrindMark[k].Point));     
			}
			else if(k==11){
				CenMarkToTopHeight=(Math.GetLengthFrLineToPoint(StaMarkToEdge, EndMarkToEdge, GrindMark[k].Point));
			}
			else if(k==18){
				RightMarkToTopHeight=(Math.GetLengthFrLineToPoint(StaMarkToEdge, EndMarkToEdge, GrindMark[k].Point));
			}
		}
	}

	// ASide AllGrind�� AlignMark���� �߰���(���� ��ũ�� ���� ����ũ ��ġ) ������ ��� �������� �߰� ������ ���� 
	// �Ϲ����� ����ũ�� �ƴϰ�...�ص����� ���� AlignMark�߰����̶��...����..�� �������� ��¿������ ������ �߰���..��.��;;;;
	// ASide LTPS,EnCap��� ����,���� ���� 
	if(IsShortCell && IsAllGrind && (CamId==2 || CamId==3)){
		double degree=0.0f; TFPoint Target(0.0f,0.0f); 
		if(GrindMark[4].Point.x!=0.0f && GrindMark[4].Point.y!=0.0f && GrindMark[18].Point.x!=0.0f && GrindMark[18].Point.y!=0.0f ){
// 			if(PatternOffSet!=0){
// 				//Pattern Distance �����ϱ� ���� ������ ��� ���� 
// 				// ù��° ��ǥ ��� 
// 				Patterndeg    = Math.GetDegree(GrindMark[4].Point,GrindMark[18].Point);
// 				PatternPos.x  = GrindMark[4].Point.x              ;
// 				PatternPos.y  = GrindMark[4].Point.y+PatternOffSet;
// 
// 				PatternPos    = Math.OnRotate (GrindMark[4].Point,PatternPos,Patterndeg); // �ι�° AlignMark ������ ������ ȸ�� �� ��ǥ���� 
// 
// 				GrindMark[4].Point.x = PatternPos.x;
// 				GrindMark[4].Point.y = PatternPos.y;
// 
// 				// ù��° ��ǥ ��� 
// 				Patterndeg    = Math.GetDegree(GrindMark[18].Point,GrindMark[4].Point);
// 				PatternPos.x  = GrindMark[18].Point.x              ;
// 				PatternPos.y  = GrindMark[18].Point.y-PatternOffSet;
// 
// 				PatternPos    = Math.OnRotate (GrindMark[18].Point,PatternPos,Patterndeg); // �ι�° AlignMark ������ ������ ȸ�� �� ��ǥ���� 
// 
// 				GrindMark[18].Point.x = PatternPos.x;
// 				GrindMark[18].Point.y = PatternPos.y;
// 			}

			degree = Math.GetDegree(GrindMark[4].Point,GrindMark[18].Point);
			Target = Math.OnRotate (GrindMark[4].Point,GrindMark[18].Point, -degree); // �ι�° AlignMark ������ ������ ȸ�� �� ��ǥ���� 

			GrindMark[11].Point.x = GrindMark[4].Point.x;
			GrindMark[11].Point.y = ((Target.y - GrindMark[4].Point.y)/2) + GrindMark[4].Point.y;
			GrindMark[11].Point   = Math.OnRotate(GrindMark[4].Point, GrindMark[11].Point, degree);

			// ��ü Buffer���� ��ǥ��� �ٽ��� 
			GrindMark[11].Point.y = GrindMark[11].Point.y+StaPos.y; 

			// GrindMark���� OutData �ִܰŸ� Count���� Calc 
			GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
			for(int i=0; i<AllOutCnt; i++){
				if(AllOutPoint[i].x   ==0.0f || AllOutPoint[i].y  ==0.0f) continue;

				GrindMeaDis = Math.GetLength(GrindMark[11].Point, AllOutPoint[i]);
				if(GrindMeaDis<MinDis){
					MinDis = GrindMeaDis;
					GrindMarkOutCnt = i;
				}
			}
			// OutData���� InData �ִܰŸ� Count���� Calc 
			if(GrindMarkOutCnt!=0){
				GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
				for(int i=0; i<AllInCnt; i++){
					if(AllInPoint [i].x              ==0.0f || AllInPoint [i].y              ==0.0f) continue;
					if(AllOutPoint[GrindMarkOutCnt].x==0.0f || AllOutPoint[GrindMarkOutCnt].y==0.0f) continue;

					GrindMeaDis = Math.GetLength(AllOutPoint[GrindMarkOutCnt], AllInPoint[i]);
					if(GrindMeaDis<MinDis){
						MinDis = GrindMeaDis;
						GrindMarkInCnt = i;
					}
				}
			}

			// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
			if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5){
				CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
				for(int i=GrindMarkOutCnt-5; i<GrindMarkOutCnt+5; i++){
					if(AllOutPoint[i].x==0.0f || AllOutPoint[i].y==0.0f) continue;
					// ������ Ƣ�� ���� ���� ���� 
					if(fabs(AllOutPoint[GrindMarkOutCnt].x - AllOutPoint[i].x)>= XPosMargin) continue;
					if(fabs(AllOutPoint[GrindMarkOutCnt].y - AllOutPoint[i].y)>= YPosMargin) continue;

					GrindMeaDis = Math.GetLength(GrindMark[11].Point, AllOutPoint[i]);
					TotalCirDis+= GrindMeaDis;
					CircleCount++;
				}
				if(TotalCirDis!=0.0f && CircleCount!=0){
					GrindMark[11].GrindMea = TotalCirDis/CircleCount;
				}
			}

			// Y1,Y2,Y3 Size���� ���� ������ ���� 
			if(CamId==2){
				LeftMarkToTopHeight  = (GrindMark[4 ].GrindMea!=0.0f)?GrindMark[4 ].GrindMea:0.0f;
				CenMarkToTopHeight   = (GrindMark[11].GrindMea!=0.0f)?GrindMark[11].GrindMea:0.0f;
				RightMarkToTopHeight = (GrindMark[18].GrindMea!=0.0f)?GrindMark[18].GrindMea:0.0f;
			}

			// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
			if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
				CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
				for(int i=-5; i<5; i++){
					if(AllOutPoint[GrindMarkOutCnt+i].x==0.0f && AllOutPoint[GrindMarkOutCnt+i].y==0.0f) continue;
					if(AllInPoint [GrindMarkInCnt +i].x==0.0f && AllInPoint [GrindMarkInCnt +i].y==0.0f) continue;

					// ������ Ƣ�� ���� ���� ���� 
					if(fabs(AllOutPoint[GrindMarkOutCnt].x - AllOutPoint[GrindMarkOutCnt+i].x)>= XPosMargin) continue;
					if(fabs(AllOutPoint[GrindMarkOutCnt].y - AllOutPoint[GrindMarkOutCnt+i].y)>= YPosMargin) continue;
					if(fabs(AllInPoint [GrindMarkInCnt ].x - AllInPoint [GrindMarkInCnt+i ].x)>= XPosMargin) continue;
					if(fabs(AllInPoint [GrindMarkInCnt ].y - AllInPoint [GrindMarkInCnt+i ].y)>= YPosMargin) continue;

					GrindWidthDis = Math.GetLength(AllInPoint[GrindMarkInCnt +i], AllOutPoint  [GrindMarkOutCnt+i]);
					TotalCirDis+=   GrindWidthDis;
					CircleCount++;
				}
				if(TotalCirDis!=0.0f && CircleCount!=0){
					GrindMark[11].GrindWidth  = TotalCirDis/CircleCount;
				}
			}
			// ȭ�� ���÷��� �������� �ٽ� ����� 
			GrindMark[11].Point.y   = GrindMark[11].Point.y - StaPos.y; 
			// �˻� ROI���� ���÷��� ����
			GrindMark[11].Rect.left   = GrindMark[11].Point.x - GrindMark[11].Range;
			GrindMark[11].Rect.top    = GrindMark[11].Point.y - GrindMark[11].Range;
			GrindMark[11].Rect.right  = GrindMark[11].Rect.left+(GrindMark[11].Range*2); 
			GrindMark[11].Rect.bottom = GrindMark[11].Rect.top +(GrindMark[11].Range*2); 
		} 

		// Pattern Dis ����(CamID=3)_ASide EnCap��� 
		if(PatternThresHold!=0 && CamId==3){
// 			GImage->Save("D:\\ROTATE_IMG\\��üBuffer.bmp");
			BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
			EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
			EasyImage::Threshold(GImage,&BinaryImg, PatternThresHold);

			if(PatternThresHold==0)  {SingleLine->GImage = GImage    ; SingleLine->FThreshold =5 ;}
			else                     {SingleLine->GImage = &BinaryImg; SingleLine->FThreshold =10;}

			for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
				if(GrindMark[k].Point.x==0.0f || GrindMark[k].Point.y==0.0f) continue;
// 				GrindMark[k].Point.y  = GrindMark[k].Point.y+StaPos.y; 
				// TEST 
				PatternLength = ((GrindMark[k].Point.x - ((StaPos.x+EndPos.x)/2))+100);
				
				SingleLine->IRoi->SetOrgX  (GrindMark[k].Point.x - PatternLength);
				SingleLine->IRoi->SetOrgY  (GrindMark[k].Point.y - PatternMagin );
				SingleLine->IRoi->SetWidth (PatternLength                       );
				SingleLine->IRoi->SetHeight(PatternMagin*2                      );

				if(((GrindMark[k].Point.x - PatternLength)<=0)    || ((GrindMark[k].Point.y - PatternMagin)<=0)) return;

				SingleLine->SetOrientation(orEast);
				SingleLine->FIncrement =1;
				SingleLine->SetFindLineId(0); 
				SingleLine->SetEdgeType(1);  // White To Black 
				SingleLine->OnExecute();

				StaPatern.x =0.0f, StaPatern.y=0.0f,EndPatern.x=0.0f, EndPatern.y=0.0f;
				StaPatern = SingleLine->GetResultP0();
				EndPatern = SingleLine->GetResultP1();

				PatternDis[k].Dis = (Math.GetLengthFrLineToPoint(StaPatern, EndPatern, GrindMark[k].Point));     
				PatternDis[k].Dis = PatternDis[k].Dis+0.5  ; //SubPixel 
			}
		}
	}
}
/* Org Sequence 
void CFM3xLineDefect::OnExecuteGrind(TFPoint StaPos, TFPoint EndPos, int InspMode)
{
	int GrindMarkRange = 40;
	int FrameImgW   = GImage->GetWidth();
	int FrameImgH   = GImage->GetHeight();

	double deg = 0.0f;
	double Orgdeg = 0.0f;
	double Newdeg = 0.0f;

	// Frame���� ���� ������� ���ս� Overlap�� �˻� ������� Buffer������ ���� ���ؼ� 
	// Polygon ������ Ƣ�� ���� ������ �߻�,������ Ƣ�� ���� �������� ���� 
	const int XPosMargin = 20; const int YPosMargin =20;

	TFPoint Target(0.0f,0.0f);
	// DvX,DvY �������� �̹��� ȸ�� �������� �����ϱ� ���ؼ� �ʿ��� ���� 
	TFPoint OrgMarkDv; TFPoint NewMarkDv; 

	// EdgePoint Data ������ 
	TFPoint   *AllOutPoint;
	TFPoint   *AllInPoint ;

	double GrindMeaDis =0.0f, MinDis =10000.0f, TotalCirDis=0.0f,GrindWidthDis=0.0f;
	int GrindMarkOutCnt=0, GrindMarkInCnt=0, PreGrindMarkOutCnt=0, PreGrindMarkInCnt=0,AllOutCnt=0,AllInCnt=0, CircleCount=0;

	Orgdeg = dSideDeg;
	if(StaPos.x!=0.0f && StaPos.y!=0.0f && EndPos.x!=0.0f && EndPos.y!=0.0f) deg = (Math.GetDegree(StaPos.x , StaPos.y, EndPos.x, EndPos.y));
	Newdeg = deg - Orgdeg;

	// InspMode �������� �з� InspMode: 0 ==>��ü����(����      ,����) 
	// InspMode:1                         ==>�κп���(Circle����,����) 
	// InspMode:2                         ==>�κп���(Corner����,����)
	if     (InspMode==0){ AllOutPoint = AllLeftOutData  ; AllInPoint = AllLeftInData  ; AllOutCnt=AllLeftOutEdgeCount; AllInCnt = AllLeftInEdgeCount;}
	else if(InspMode==1){ AllOutPoint = AllCircleOutData; AllInPoint = AllCircleInData; AllOutCnt=AllCirOutEdgeCount ; AllInCnt = AllCirInEdgeCount ;}
	else if(InspMode==2){ AllOutPoint = AllCornerOutData; AllInPoint = AllCornerInData; AllOutCnt=AllCorOutEdgeCount ; AllInCnt = AllCorInEdgeCount ;}

	// GrindMark DvX,DvY�������� ���� ���� 
	for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
		if(GrindMark[k].DvX==0 || GrindMark[k].DvY==0) continue;

		// 		Target  = Math.OnFindPerpendicularPointXY(StaPos,(float)GrindMark[k].DvX ,(float)GrindMark[k].DvY,deg);
		// Ư�� Buffer���� GrindMark Find������ 
		OrgMarkDv.x = (float)GrindMark[k].DvX; OrgMarkDv.y = (float)GrindMark[k].DvY;
		NewMarkDv   = Math.OnRotate(FLTPoint , OrgMarkDv, Newdeg);  

		GrindMark[k].DvX = (int)NewMarkDv.x; GrindMark[k].DvY = (int)NewMarkDv.y; 

		Target.x = StaPos.x ; Target.y = 0  ;
		Target.x = Target.x+GrindMark[k].DvX;
		Target.y = Target.y+GrindMark[k].DvY;

		GrindMark[k].Rect.left   = Target.x - GrindMarkRange;
		GrindMark[k].Rect.top    = Target.y - GrindMarkRange;
		GrindMark[k].Rect.right  = GrindMark[k].Rect.left+(GrindMarkRange*2); 
		GrindMark[k].Rect.bottom = GrindMark[k].Rect.top +(GrindMarkRange*2); 
	}

	// GrindMark Find �� ����,������ ���� 
	Find->OnLoad(true,FindDir)                     ;
	Find->Image  = GImage                          ;
	Find->SetContrastMode(EFindContrastMode_Normal);
	Find->SetMinScore (GrindMarkMinScore)          ; // PatternrFind �˻� �ּ� Score
	Find->SetZeroScore(GrindMarkMinScore)          ; // �˻� �� �ּ� Score 
	Find->SetMaxInstances(1)                       ;
	Find->SetAngleTolerance(20.0f)                 ;
	Find->SetScaleBias(1.00f)                      ;
	Find->SetScaleTolerance(0.0f)                  ;
	Find->Property.Ox = (float)FrameImgW/2         ;
	Find->Property.Oy = (float)FrameImgH/2         ;

	int GrindRectWidth=0, GrindRectHeight=0;
	// 	AllOutCnt = AllLeftOutEdgeCount; // ������� ��ü ������ ���� � Total Out EdgePoint Count
	// 	AllInCnt  = AllLeftInEdgeCount ; // ������� ��ü ������ ���� � Total In  EdgePoint Count

	if(AllOutCnt!=0 && AllInCnt!=0){
		for(int k=0; k<MAX_GRIND_MARK_COUNT; k++){
			if(GrindMark[k].Rect.left==0 || GrindMark[k].Rect.top==0 || GrindMark[k].Rect.right==0 || GrindMark[k].Rect.bottom==0) continue;
			GrindRectWidth  = (GrindMark[k].Rect.right  - GrindMark[k].Rect.left);
			GrindRectHeight = (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top );

			Find->IRoi->SetPlacement(GrindMark[k].Rect.left, GrindMark[k].Rect.top, GrindRectWidth, GrindRectHeight);
			if(GrindMark[k].Rect.left<=0    || GrindMark[k].Rect.top<=0    || GrindRectWidth<=0    || GrindRectHeight<=0   ) continue;
			if(GrindMark[k].Rect.left>=FrameImgW || GrindMark[k].Rect.top>=FrameImgH || GrindRectWidth>=FrameImgW || GrindRectHeight>=FrameImgH) continue;

			deg = GrindMark[k].degree;
			Find->SetAngleBias(deg);

			// GrindMark Search 
			if(Find->OnExecute()){
				GrindMark[k].Point.x = Find->Result[0].Px;
				GrindMark[k].Point.y = Find->Result[0].Py;
				GrindMark[k].Score   = Find->Result[0].Score;
			}

			// ������,������ ��� 
			if(GrindMark[k].Point.x!=0.0f && GrindMark[k].Point.y!=0.0f){ // GrindMark ã�������
				// ��ü Buffer���� ��ǥ��� �ٽ��� 
				GrindMark[k].Point.y = GrindMark[k].Point.y+ StaPos.y; 

				// GrindMark���� OutData �ִܰŸ� Count���� Calc 
				GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
				for(int i=PreGrindMarkOutCnt; i<AllOutCnt; i++){
					if(AllOutPoint[i].x   ==0.0f || AllOutPoint[i].y  ==0.0f) continue;

					GrindMeaDis = Math.GetLength(GrindMark[k].Point, AllOutPoint[i]);
					if(GrindMeaDis<MinDis){
						MinDis = GrindMeaDis;
						GrindMarkOutCnt = i;
						PreGrindMarkOutCnt = GrindMarkOutCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
					}
				}
				// OutData���� InData �ִܰŸ� Count���� Calc 
				if(GrindMarkOutCnt!=0){
					GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
					for(int i=PreGrindMarkInCnt; i<AllInCnt; i++){
						if(AllInPoint [i].x              ==0.0f || AllInPoint [i].y              ==0.0f) continue;
						if(AllOutPoint[GrindMarkOutCnt].x==0.0f || AllOutPoint[GrindMarkOutCnt].y==0.0f) continue;

						GrindMeaDis = Math.GetLength(AllOutPoint[GrindMarkOutCnt], AllInPoint[i]);
						if(GrindMeaDis<MinDis){
							MinDis = GrindMeaDis;
							GrindMarkInCnt = i;
							PreGrindMarkInCnt = GrindMarkInCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
						}
					}
				}

				// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
				if(GrindMarkOutCnt!=0 && GrindMarkOutCnt>5){
					CircleCount=0,TotalCirDis=0.0f,GrindMeaDis=0.0f;
					for(int i=GrindMarkOutCnt-5; i<GrindMarkOutCnt+5; i++){
						if(AllOutPoint[i].x==0.0f || AllOutPoint[i].y==0.0f) continue;
						// ������ Ƣ�� ���� ���� ���� 
						if(fabs(AllOutPoint[GrindMarkOutCnt].x - AllOutPoint[i].x)>= XPosMargin) continue;
						if(fabs(AllOutPoint[GrindMarkOutCnt].y - AllOutPoint[i].y)>= YPosMargin) continue;

						GrindMeaDis = Math.GetLength(GrindMark[k].Point, AllOutPoint[i]);
						TotalCirDis+= GrindMeaDis;
						CircleCount++;
					}
					if(TotalCirDis!=0.0f && CircleCount!=0){
						GrindMark[k].GrindMea = TotalCirDis/CircleCount;
					}
				}

				// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
				if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
					CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
					for(int i=-5; i<5; i++){
						if(AllOutPoint[GrindMarkOutCnt+i].x==0.0f && AllOutPoint[GrindMarkOutCnt+i].y==0.0f) continue;
						if(AllInPoint [GrindMarkInCnt +i].x==0.0f && AllInPoint [GrindMarkInCnt +i].y==0.0f) continue;

						// ������ Ƣ�� ���� ���� ���� 
						if(fabs(AllOutPoint[GrindMarkOutCnt].x - AllOutPoint[GrindMarkOutCnt+i].x)>= XPosMargin) continue;
						if(fabs(AllOutPoint[GrindMarkOutCnt].y - AllOutPoint[GrindMarkOutCnt+i].y)>= YPosMargin) continue;
						if(fabs(AllInPoint [GrindMarkInCnt ].x - AllInPoint [GrindMarkInCnt+i ].x)>= XPosMargin) continue;
						if(fabs(AllInPoint [GrindMarkInCnt ].y - AllInPoint [GrindMarkInCnt+i ].y)>= YPosMargin) continue;

						GrindWidthDis = Math.GetLength(AllInPoint[GrindMarkInCnt +i], AllOutPoint  [GrindMarkOutCnt+i]);
						TotalCirDis+=   GrindWidthDis;
						CircleCount++;
					}
					if(TotalCirDis!=0.0f && CircleCount!=0){
						GrindMark[k].GrindWidth  = TotalCirDis/CircleCount;
					}
				}
				// ȭ�� ���÷��� �������� �ٽ� ����� 
				GrindMark[k].Point.y = GrindMark[k].Point.y - StaPos.y; 
			}
			else                                                        { // GrindMark ��ã�����
				// OutData���� �ּҰ� ������ Count���� 
				GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkOutCnt=0;
				for(int i=PreGrindMarkOutCnt; i<AllOutCnt; i++){
					if(AllOutPoint[i].x==0.0f || AllOutPoint[i].y==0.0f) continue;

					GrindMark[k].RectPoint.x = GrindMark[k].Rect.left + (GrindMark[k].Rect.right  - GrindMark[k].Rect.left)/2 ; 
					GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2+StaPos.y; 
					// 					GrindMark[k].RectPoint.y = GrindMark[k].Rect.top  + (GrindMark[k].Rect.bottom - GrindMark[k].Rect.top )/2 ; 

					GrindMeaDis = Math.GetLength(GrindMark[k].RectPoint, AllOutPoint[i]);
					if(GrindMeaDis<MinDis){
						MinDis = GrindMeaDis;
						GrindMarkOutCnt = i;
						PreGrindMarkOutCnt = GrindMarkOutCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
					}
				}

				// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
				if(GrindMarkOutCnt!=0){
					GrindMeaDis =0.0f, MinDis =10000.0f; GrindMarkInCnt=0;
					for(int i=PreGrindMarkInCnt; i<AllInCnt; i++){
						if(AllInPoint [i].x              ==0.0f || AllInPoint [i].y               ==0.0f) continue;
						if(AllOutPoint[GrindMarkOutCnt].x==0.0f || AllOutPoint[GrindMarkOutCnt].y ==0.0f) continue;

						GrindMeaDis = Math.GetLength(AllOutPoint[GrindMarkOutCnt], AllInPoint[i]);
						if(GrindMeaDis<MinDis){
							MinDis = GrindMeaDis;
							GrindMarkInCnt = i;
						}
					}
				}

				// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
				if((GrindMarkOutCnt!=0 && GrindMarkOutCnt>5) && (GrindMarkInCnt!=0 && GrindMarkInCnt>5)){
					CircleCount=0,TotalCirDis=0.0f,GrindWidthDis=0.0f;
					for(int i=-5; i<5; i++){
						if(AllOutPoint[GrindMarkOutCnt+i].x==0.0f && AllOutPoint[GrindMarkOutCnt+i].y==0.0f) continue;
						if(AllInPoint [GrindMarkInCnt +i].x==0.0f && AllInPoint [GrindMarkInCnt +i].y==0.0f) continue;

						// ������ Ƣ�� ���� ���� ���� 
						if(fabs(AllOutPoint[GrindMarkOutCnt].x - AllOutPoint[GrindMarkOutCnt+i].x)>= XPosMargin) continue;
						if(fabs(AllOutPoint[GrindMarkOutCnt].y - AllOutPoint[GrindMarkOutCnt+i].y)>= YPosMargin) continue;
						if(fabs(AllInPoint [GrindMarkInCnt ].x - AllInPoint [GrindMarkInCnt+i ].x)>= XPosMargin) continue;
						if(fabs(AllInPoint [GrindMarkInCnt ].y - AllInPoint [GrindMarkInCnt+i ].y)>= YPosMargin) continue;

						GrindWidthDis = Math.GetLength(AllInPoint[GrindMarkInCnt+i], AllOutPoint[GrindMarkOutCnt+i]);
						TotalCirDis+=   GrindWidthDis;
						CircleCount++;
					}
					if(TotalCirDis!=0.0f && CircleCount!=0){
						GrindMark[k].GrindWidth = TotalCirDis/CircleCount;
					}
				}
			}
		}
	}
}
*/
void CFM3xLineDefect::OnLoad(bool IsLoad, CString Folder)
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
CFind* CFM3xLineDefect::GetFindPointer(int n)
{
	if     (n==0) return Find1;
	else if(n==1) return Find2;
	return NULL;
}

//------------------------------------------------------------------------------------
// ����� Set�Լ� 
//------------------------------------------------------------------------------------------
void CFM3xLineDefect::SetProcessMode(TProcessMode ProcessMode)
{
	FProcessMode  = ProcessMode;
}
//------------------------------------------------------------------------------------------
void CFM3xLineDefect::SetInspectMode(TInspectMode InspectMode)
{
	FInspectMode  = InspectMode;
}
//------------------------------------------------------------------------------------------
void CFM3xLineDefect::SetShape(TShape Shape)
{
	FShape = Shape;
}
//------------------------------------------------------------------------------------------
void CFM3xLineDefect::SetLineScanType(int LineScanType)
{
	FLineScanType        = LineScanType;
	BufferWidth          = FLineScanType;
	Polygon->BufferWidth = FLineScanType; //Polygon �˻�� IELEMENT ����� 
}
//-------------------------------------------------------------------------------------------
void CFM3xLineDefect::SetGlassType(TGlassType GlassType)
{
	FGlassType  = GlassType;
}
//-------------------------------------------------------------------------------------------
void CFM3xLineDefect::SetCrossLeftPoint(int x, int y)
{
	FP0.x = (float)x;
	FP0.y = (float)y;
	AlignMark1.x = FP0.x;
	AlignMark1.y = FP0.y;
}
//------------------------------------------------------------------------------------------
void CFM3xLineDefect::SetCrossRightPoint(int x, int y)
{
	FP1.x =(float)x;
	FP1.y =(float)y;
	AlignMark2.x = FP1.x;
	AlignMark2.y = FP1.y;
}
//------------------------------------------------------------------------------------------
void CFM3xLineDefect::SetSpecWidth(float MinWidth , float MaxWidth)
{
	FMinWidth = MinWidth;
	FMaxWidth = MaxWidth;
}
//------------------------------------------------------------------------------------------
void CFM3xLineDefect::SetSpecHeight(float MinHeight, float MaxHeight)
{
	FMinHeight = MinHeight;
	FMaxHeight = MaxHeight;
}
//------------------------------------------------------------------------------------------
