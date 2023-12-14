#include "stdafx.h"
#include "FM15xLineDefect.h"
#include "clipper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CFM15xLineDefect::CFM15xLineDefect()
{
	SingleLine = new CSingleLine();
	RectEdge   = new CRectEdge();
	Edge       = new CEdge();
	CircleFit  = new CCircleFit();
	Polygon    = new CPolygon();
	Find1      = new CFind();

	FLeftWall       = true;
	FTopWall        = true;
	FRightWall      = true;
	FBottomWall     = true;
	FLeftTop        = true;
	FRightTop       = true;
	FRightBottom    = true;
	FLeftBottom     = true;
	IsLeftAllGrind  = false;
	IsTopAllGrind   = false;
	IsRightAllGrind = false;
	IsBtmAllGrind   = false;

	MarkPos.x    = 0.0f;
	MarkPos.y    = 0.0f;

	MarkEndPos.x = 0.0f;
	MarkEndPos.y = 0.0f;

	MarkStrPos.x = 0.0f;
	MarkStrPos.y = 0.0f;

	MarkAng = 0.0f;

	FBevelIn = false;

	FProcessMode = pmAutoOutLine;

	IRoi = new EROIBW8();

	// Dark Defect 
	DefectPointCount = 0; // ��ü �ҷ� ���� �ʱ�ȭ
	ACount = 0; // ���ڸ�� �ҷ� ���� �ʱ�ȭ
	BCount = 0; // ��������� �ҷ� ���� �ʱ�ȭ
	CCount = 0; // �Ÿ�� �ҷ� ���� �ʱ�ȭ
	DCount = 0;
	C1Count = 0;
	C2Count = 0;
	C3Count = 0;
	C4Count = 0;

	// Bridht Defect 
	ACrackCount = 0;
	BCrackCount = 0;
	CCrackCount = 0;
	DCrackCount = 0;

	// Dark Defect 
	A1CirDeftectCnt = 0;
	A2CirDeftectCnt = 0;
	C1CirDeftectCnt = 0;
	C2CirDeftectCnt = 0;

	// Bright Defect 
	A1CirCrackCnt = 0;
	A2CirCrackCnt = 0;
	C1CirCrackCnt = 0;
	C2CirCrackCnt = 0;

	// ��ü���� Dark_Defect Count
	LeftDefectCount  = 0;
	RIghtDefectCount = 0;
	TopDefectCount   = 0;
	BtmDefectCount   = 0;

	// ��ü���� Bright_Defect Count
	LeftCrackCount  = 0;
	RIghtCrackCount = 0;
	TopCrackCount   = 0;
	BtmCrackCount   = 0;

	// Cell ���� 
	CELL_A = 0;
	CELL_B = 0;
	CELL_C = 0;
	CELL_D = 0;
	CELL_E = 0;
	CELL_F = 0;
	CELL_G = 0;
	CELL_H = 0;

	LTDx = 0.0f, RTDx = 0.0f, RBDx = 0.0f, LBDx = 0.0f;
	LTDy = 0.0f, RTDy = 0.0f, RBDy = 0.0f, LBDy = 0.0f;

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
	A1CirInPoint = NULL;
	A2CirInPoint = NULL;
	// 	A1PolyInPoint  = NULL;

	C1CirInPoint = NULL;
	C2CirInPoint = NULL;

	// Circle Data OutPoint Init
	A1CirOutPoint = NULL;
	A2CirOutPoint = NULL;

	C1CirOutPoint = NULL;
	C2CirOutPoint = NULL;

	// Circle Data StandPoint Init 
	C1CirStandPoint = NULL;
	C2CirStandPoint = NULL;

	// ��ü������ ������ �ʱ�ȭ 
	// AllGrind��� ���� Frame���� ������ ����
	LeftInPoint   = NULL;
	LeftOutPoint  = NULL;
	TopInPoint    = NULL;
	TopOutPoint   = NULL;
	RightInPoint  = NULL;
	RightOutPoint = NULL;
	BtmInPoint    = NULL;
	BtmOutPoint   = NULL;

	// NarrowTrench �������� ���������� 
	GrindLine = NULL;

	// Masking test 
	MaskPoint = NULL;
	MaskPoint2 = NULL;

	// Masking test 
	LeftMaskPoint = NULL;
	LeftMaskPoint2 = NULL;

	// Masking test 
	RightMaskPoint = NULL;
	RightMaskPoint2 = NULL;

	// MultiThresHold 
	PadSideThresHold   = 0;
	TopDarkThresHold   = 0;
	LeftDarkThresHold  = 0;
	RightDarkThresHold = 0;
	BtmDarkThresHold   = 0;

	// ���� �������� ã�� ThresHold 
	PadCenThresHold     = 0;
	NotchSideThresHold  = 0;
	NotchCenThresHold   = 0;
	BSideGrindThresHold = 0;
	DSideGrindThresHold = 0;
	//Cell Inspection 
	LeftEdgeThresHold    = 0;
	TopEdgeThresHold     = 0;
	RightEdgeThresHold   = 0;
	BtmEdgeThresHold     = 0;
	CorGrindOutThresHold = 0;
	CorGrindMeaThresHold = 0;
	CorGrindInThresHold  = 0;
	PatternThresHold     = 0;
	PatternOffSet        = 0;

	LeftEdgeBTWhite      =false; //Black To White Edge ���� 
	TopEdgeBTWhite       =false; //Black To White Edge ���� 
	RightEdgeBTWhite     =false; //Black To White Edge ���� 
	BtmEdgeBTWhite       =false; //Black To White Edge ���� 

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

	// �ܰ����ο��� ���� GrindMark���� Width,Height �Ÿ�
	OutToGrindMarkDis1 = 0.0f;
	OutToGrindMarkDis2 = 0.0f;

	// �ڸ� ���� �������� EdgeLine���� Trap���� �Ÿ� 
	dEdgeToTrapLineDis = 0.0f;

	// Notch Cell Mark Init 
	dCirOffsetX = 0.0f;
	dCirOffsetY = 0.0f;

	LogUnit.Create(_T("FAVION"));

	FInspectMode = 0; //InspectMode Classify Parameter

	//LHJ �߰� 20180808
	RqMode = false;
	WkMode = false;

	// Find Dir 
	FindDir = "";
	MatchDir = "";
		
	// Grind Mark Find Score 
	GrindMarkMinScore = 0.0f;
	GrindPadMarkMinScore = 0.0f;

	LeftMaskDarkDefectTh  = 0;
	TopMaskDarkDefectTh   = 0;
	RightMaskDarkDefectTh = 0;
	BtmMaskDarkDefectTh   = 0;

	LeftMaskBrightDefectTh  =0;
	TopMaskBrightDefectTh   =0;
	RightMaskBrightDefectTh =0;
	BtmMaskBrightDefectTh   =0;

	// ������ Mask Offset 
	LeftMaskOffset = 0;
	TopMaskOffset = 0;
	RightMaskOffset = 0;
	BtmMaskOffset = 0;

	// ������ Mask Distance 
	LeftMaskDis  = 0;
	TopMaskDis   = 0;
	RightMaskDis = 0;
	BtmMaskDis   = 0;

	IsFrameGrindMark = false;
	OnClearData();
}
CFM15xLineDefect::~CFM15xLineDefect()
{
	delete SingleLine;
	delete RectEdge  ;
	delete Edge      ;
	delete CircleFit ;
	delete Find1     ;
}

float CFM15xLineDefect::GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3)
{
	//--------------------------------------------------------------------------
	// ������ ������ ������ �������� ���� ġ�� ����
	// ���� : ax + by + c = 0, (m, n) -> Distance = (ma + nb +c) / sqrt(a*a + b*b)
	//--------------------------------------------------------------------------
	double xDiff = P2.x - P1.x;
	double yDiff = P2.y - P1.y;

	if (xDiff == 0) return (float)(fabs(P3.x - P1.x));
	else if (yDiff == 0) return (float)(fabs(P3.y - P1.y));

	float m = (float)(yDiff / xDiff);
	float dist = (float)(fabs((-m * P3.x + P3.y + m * P1.x - P1.y) / sqrt(m * m + 1)));

	return dist;
}
int CFM15xLineDefect::OnExecute(TShape Shape)
{
	FShape = Shape;
	return OnExecute();
}
int CFM15xLineDefect::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(L"Check CFM14xLineDefect OnExecute GImage Memory Allocate"); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(L"Check CFM14xLineDefect OnExecute GImage Width Size"     ); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(L"Check CFM14xLineDefect OnExecute GImage Height Size"    ); return false; }

	IAddress    = GImage->GetImagePtr();
	BufferWidth = GImage->GetWidth   (); //Buffer ũ�⼳��....Ư�� ��ǥ���� Pixel ����� �ʿ��� 

	int Index = 0;
	// ���� LineEdge ������ ����Cell���� �������� ������ �˻����� �����ؾ���   
	// NOTCH CELL �˻� ������ 
	if (FShape == shRectH) Index = 0;
	if (FShape == shRectS) Index = 1;
	if (FShape == shRectC) Index = 2;
	// ������������ Cell���°� ������ ��� Company �߰��ؾ��� 
	bool l = false;
	bool t = false;
	bool r = false;
	bool b = false;
	bool lt = false;
	bool rt = false;
	bool rb = false;
	bool lb = false;

	switch (Index) {

	case 0:
		// RectH ���ڸ�� 
		//------------------------------------------------------------------
		l = FLeftWall = true;
		t = FTopWall = true;
		r = FRightWall = true;
		b = FBottomWall = false;
		lt = FLeftTop = true;
		rt = FRightTop = true;
		rb = FRightBottom = false;
		lb = FLeftBottom = false;
		break;

	case 1:
		// RectS : ������ ���
		//------------------------------------------------------------------
		l = FLeftWall = true;
		t = FTopWall = false;
		r = FRightWall = true;
		b = FBottomWall = false;
		lt = FLeftTop = false;
		rt = FRightTop = false;
		rb = FRightBottom = false;
		lb = FLeftBottom = false;
		break;

	case 2:
		// RectC : �Ÿ��
		//------------------------------------------------------------------
		l = FLeftWall = true;
		t = FTopWall = false;
		r = FRightWall = true;
		b = FBottomWall = true;
		lt = FLeftTop = false;
		rt = FRightTop = false;
		rb = FRightBottom = true;
		lb = FLeftBottom = true;
		break;
	}
	if (!OnExecute(l, t, r, b, lt, rt, rb, lb)) return false;
	return true;
}
CRect CFM15xLineDefect::OnMerge(CRect ar, CRect br)
{
	CRect r;
	r.left = (ar.left < br.left) ? ar.left : br.left;
	r.top = (ar.top < br.top) ? ar.top : br.top;
	r.right = (ar.right > br.right) ? ar.right : br.right;
	r.bottom = (ar.bottom > br.bottom) ? ar.bottom : br.bottom;

	return r;
}
/////////////////////////////////////////////////////////////////////////////////////
int CFM15xLineDefect::GetSize(TCorner* lpCorner, TBlob* lpDefect, int Count, int Dv, int ThresHold)
{
	bool IsRoof = false;
	bool IsStart = false;
	CRect Rect;
	Rect.left = 0;
	Rect.right = 0;
	Rect.top = 0;
	Rect.bottom = 0;
	int DefectCount = 0;
	int DefectPoint = 0;
	const double MinDv = 2;
	int PixelValue = 0;
	int PixelCount = 0;
	int PixelPoint = 0;
	int PixelScore = 0;
	float Score = 0.0f;

	// Defect Data �������� �Ǵ��� 
	for (int i = 0; i < Count; i++) {
		if (lpCorner[i].Dv > Dv) {
			IsStart = true;
			break;
		}
	}

	if (IsStart) {
		for (register int k = 0; k < Count; k++) {
			if (lpCorner[k].Dv > 1) { IsRoof = true; }
			if (IsRoof && lpCorner[k].Dv <= MinDv) {
				if (DefectPoint > 3) {
					lpDefect[DefectCount].Height = (Rect.bottom - Rect.top);
					lpDefect[DefectCount].Width = (Rect.right - Rect.left);

					// ���� Pixel Data ���� 
					PixelCount = 0; PixelPoint = 0;
					for (int n = Rect.top + 1; n < Rect.bottom - 1; n++) {
						for (int m = Rect.left + 1; m < Rect.right - 1; m++) {
							if (m <= 0 || n <= 0 || m > FrameImgW || n > FrameImgH) continue;
							PixelValue = IElement(m, n);
							if (PixelValue < ThresHold) PixelCount++;
							PixelPoint++;
						}
					}
					Score = (((float)PixelCount / (float)PixelPoint) * 100);
					if (Score > 10) {
						lpDefect[DefectCount].P.x = (float)Rect.left;
						lpDefect[DefectCount].P.y = (float)(Rect.top + ImageHeight);
						lpDefect[DefectCount].Round = PixelCount;
						DefectCount++;
					}
				}
				DefectPoint = 0;
				IsRoof = false;
			}

			if (IsRoof) {
				Rect.left = Rect.left < lpCorner[k].P.x && Rect.left != 0 ? Rect.left : (long)lpCorner[k].P.x;
				Rect.top = Rect.top < lpCorner[k].P.y && Rect.top != 0 ? Rect.top : (long)lpCorner[k].P.y;
				Rect.right = Rect.right > lpCorner[k].P.x ? Rect.right : (long)lpCorner[k].P.x;
				Rect.bottom = Rect.bottom > lpCorner[k].P.y ? Rect.bottom : (long)lpCorner[k].P.y;
				DefectPoint++;
			}
			else {
				Rect.left = 0;
				Rect.top = 0;
				Rect.right = 0;
				Rect.bottom = 0;
			}

		}
	}
	return DefectCount;
}
int CFM15xLineDefect::GetCircleCrackSize(TCircle* lpCircle, TBlob* lpDefect, int Count, int Dis, double CirDiameter, TFPoint Center, int PreDefectCnt, bool IsInCircle)
{
	bool IsRoof = false;
	bool IsStart = false;
	bool IsFlagCh = true;
	CRect Rect;
	Rect.left = 0;
	Rect.right = 0;
	Rect.top = 0;
	Rect.bottom = 0;
	int DefectCount = 0;
	int DefectPoint = 0;
	// 	const double MinDv =0.5;
	int MinDis = Dis;
	int PixelValue = 0;
	int PixelCount = 0;
	int PixelPoint = 0;
	int PixelScore = 0;
	float Score = 0.0f;

	int DefectWidth = 0;
	int DefectHeight = 0;
	double MaxDis = 0.0f;

	TFPoint StaPos(0.0f, 0.0f);
	TFPoint EndPos(0.0f, 0.0f);
	TFPoint MaxPos(0.0f, 0.0f);
	int     MaxCount = 0;
	float   PointDis = 0.0f;

	// �� �ڸ��� �ݿ��� �ڸ� ���� ���� �ʿ��� ����  
	TFPoint LTPoint(0.0f, 0.0f);
	TFPoint RTPoint(0.0f, 0.0f);
	TFPoint LBPoint(0.0f, 0.0f);
	TFPoint RBPoint(0.0f, 0.0f);
	double CenDisLTPos = 0.0f;
	double CenDisRTPos = 0.0f;
	double CenDisLBPos = 0.0f;
	double CenDisRBPos = 0.0f;

	// Defect Data �������� �Ǵ�g 
	if (Count > 5) IsStart = true;

	if (IsStart) {
		for (register int k = 0; k < Count; k++) {
			if (lpCircle[k].P.x != 0.0f && lpCircle[k].P.y != 0.0f && IsFlagCh) {   // 1�̻� ū �������� ������ ��....2����.....
				IsRoof = true;
				IsFlagCh = false;
				StaPos.x = lpCircle[k].P.x;
				StaPos.y = lpCircle[k].P.y;
				PointDis = 0.0f;
			}
			if (k > 1) PointDis = Math.GetLength(lpCircle[k - 1].P, lpCircle[k].P);
			if (k == Count - 1) PointDis = MinDis + 1;

			if (IsRoof && PointDis > MinDis) {
				// ���� Pixel Data ���� 
				PixelCount = 0, PixelPoint = 0, PixelScore = 5, Score = 0.0f;
				for (int n = Rect.top; n < Rect.bottom; n++) {
					for (int m = Rect.left; m < Rect.right; m++) {
						if (m <= 0 || n <= 0 || m > FrameImgW || n > FrameImgH) continue;
						PixelValue = IElement(m, n);
						if (PixelValue > BrokenThresHold) PixelCount++;
						PixelPoint++;
					}
				}
				EndPos.x = lpCircle[k].P.x;
				EndPos.y = lpCircle[k].P.y;

				Score = (((float)PixelCount / (float)PixelPoint) * 100);
				// Circle Defect ������ PixelCount ������ üũ�Ѵ� => ����ų ���� ���� 
				if (Score > PixelScore) {
					// 						MaxPos.x = lpCircle[MaxCount].P.x;
					// 						MaxPos.y = lpCircle[MaxCount].P.y;
										// �� �ڸ��� �ݿ��� �ڸ� ��������
					LTPoint.x = (float)Rect.left;  LTPoint.y = (float)Rect.top;
					RTPoint.x = (float)Rect.right;  RTPoint.y = (float)Rect.top;
					LBPoint.x = (float)Rect.left;  LBPoint.y = (float)Rect.bottom;
					RBPoint.x = (float)Rect.right;  RBPoint.y = (float)Rect.bottom;

					CenDisLTPos = 0.0f;
					CenDisRTPos = 0.0f;
					CenDisLBPos = 0.0f;
					CenDisRBPos = 0.0f;

					// �ɹڸ�(Peeling) vs �ݿ����ڸ�(Broken)���� ������ 
					if (CirDiameter != 0.0f) {
						if (IsInCircle) { // Cir1, Cir2, Cir5, Cir6
							CenDisLTPos = Math.GetLength(Center, LTPoint);
							CenDisRTPos = Math.GetLength(Center, RTPoint);
							CenDisLBPos = Math.GetLength(Center, LBPoint);
							CenDisRBPos = Math.GetLength(Center, RBPoint);
							if (CenDisLTPos >= CirDiameter || CenDisRTPos >= CirDiameter || CenDisLBPos >= CirDiameter || CenDisRBPos >= CirDiameter) lpDefect[DefectCount].LineAttach = 1;
						}
						else {// Cir3, Cir4 
							CenDisLTPos = Math.GetLength(Center, LTPoint);
							CenDisRTPos = Math.GetLength(Center, RTPoint);
							CenDisLBPos = Math.GetLength(Center, LBPoint);
							CenDisRBPos = Math.GetLength(Center, RBPoint);
							if (CenDisLTPos <= CirDiameter || CenDisRTPos <= CirDiameter || CenDisLBPos <= CirDiameter || CenDisRBPos <= CirDiameter) lpDefect[DefectCount].LineAttach = 1;
						}
					}

					lpDefect[PreDefectCnt].Rect.left = Rect.left;
					lpDefect[PreDefectCnt].Rect.top = Rect.top;
					lpDefect[PreDefectCnt].Rect.right = Rect.right;
					lpDefect[PreDefectCnt].Rect.bottom = Rect.bottom;

					lpDefect[PreDefectCnt].Height = (Rect.bottom - Rect.top);
					lpDefect[PreDefectCnt].Width = (Rect.right - Rect.left);

					// 					lpDefect[DefectCount].Ver_Dis      = lpCircle[MaxCount].Dv        ;
					// 					lpDefect[DefectCount].Hor_Dis      = Math.GetLength(StaPos,EndPos);

					lpDefect[PreDefectCnt].Ver_Dis = lpDefect[PreDefectCnt].Height;
					lpDefect[PreDefectCnt].Hor_Dis = lpDefect[PreDefectCnt].Width;

					lpDefect[PreDefectCnt].P.x = (float)Rect.left;
					lpDefect[PreDefectCnt].P.y = (float)(Rect.top + ImageHeight);

					lpDefect[PreDefectCnt].Rect.top = lpDefect[PreDefectCnt].P.y;
					lpDefect[PreDefectCnt].Rect.bottom = lpDefect[PreDefectCnt].Rect.top + lpDefect[PreDefectCnt].Height;
					lpDefect[PreDefectCnt].Round = PixelCount;
					PreDefectCnt++;
					DefectCount++;
				}
				DefectPoint = 0;
				IsRoof = false;
				IsFlagCh = true;
				MaxDis = 0.0f;
				MaxCount = 0;
			}

			if (IsRoof) { // Bright Point ���� Rect���� 
// 				if(lpCircle[k].Dv>MaxDis){
// 					MaxDis   = lpCircle[k].Dv;
// 					MaxCount = k             ;
// 				}
				Rect.left = Rect.left < lpCircle[k].P.x && Rect.left != 0 ? Rect.left : (long)lpCircle[k].P.x;
				Rect.top = Rect.top < lpCircle[k].P.y && Rect.top != 0 ? Rect.top : (long)lpCircle[k].P.y;
				Rect.right = Rect.right > lpCircle[k].P.x ? Rect.right : (long)lpCircle[k].P.x;
				Rect.bottom = Rect.bottom > lpCircle[k].P.y ? Rect.bottom : (long)lpCircle[k].P.y;
				DefectPoint++;
			}
			else {
				Rect.left = 0;
				Rect.top = 0;
				Rect.right = 0;
				Rect.bottom = 0;
			}

		}
	}
	return DefectCount;
}
int CFM15xLineDefect::GetCircleSize(TCircle* lpCircle, TBlob* lpDefect, int Count, int Dv) //Mark_Mode Dark Defect 
{
	bool IsRoof = false;
	bool IsStart = false;
	bool IsFlagCh = true;
	CRect Rect;
	Rect.left = 0;
	Rect.right = 0;
	Rect.top = 0;
	Rect.bottom = 0;
	int DefectCount = 0;
	int DefectPoint = 0;
	// 	const double MinDv =0.5;
	const double MinDv = Dv / 2;
	int PixelValue = 0;
	int PixelCount = 0;
	int PixelPoint = 0;
	int PixelScore = 0;
	float Score = 0.0f;

	int DefectWidth = 0;
	int DefectHeight = 0;
	double MaxDis = 0.0f;

	TFPoint StaPos(0.0f, 0.0f);
	TFPoint EndPos(0.0f, 0.0f);
	TFPoint MaxPos(0.0f, 0.0f);
	int     MaxCount = 0;

	// Defect Data �������� �Ǵ��� 
	for (int i = 0; i < Count; i++) {
		if (lpCircle[i].Dv > Dv) {
			IsStart = true;
			break;
		}
	}

	if (IsStart) {
		for (register int k = 0; k < Count; k++) {
			if (lpCircle[k].Dv > 1 && IsFlagCh) {   // 1�̻� ū �������� ������ ��....2����.....
				IsRoof = true;
				IsFlagCh = false;
				StaPos.x = lpCircle[k].P.x;
				StaPos.y = lpCircle[k].P.y;
			}

			if (IsRoof && lpCircle[k].Dv <= 1 && DefectPoint > 3) {
				if (DefectPoint > 3) {
					// ���� Pixel Data ���� 
					PixelCount = 0, PixelPoint = 0, PixelScore = 45, Score = 0.0f;
					for (int n = Rect.top + 1; n < Rect.bottom - 1; n++) {
						for (int m = Rect.left + 1; m < Rect.right - 1; m++) {
							if (m <= 0 || n <= 0 || m > FrameImgW || n > FrameImgH) continue;
							PixelValue = IElement(m, n);
							if (PixelValue < BtmDarkThresHold) PixelCount++;
							PixelPoint++;
						}
					}
					EndPos.x = lpCircle[k].P.x;
					EndPos.y = lpCircle[k].P.y;

					Score = (((float)PixelCount / (float)PixelPoint) * 100);
					// Circle Defect ������ PixelCount ������ üũ�Ѵ� => ����ų ���� ���� 
					if (Score > PixelScore) {
						MaxPos.x = lpCircle[MaxCount].P.x;
						MaxPos.y = lpCircle[MaxCount].P.y;

						lpDefect[DefectCount].Rect.left = Rect.left;
						lpDefect[DefectCount].Rect.top = Rect.top;
						lpDefect[DefectCount].Rect.right = Rect.right;
						lpDefect[DefectCount].Rect.bottom = Rect.bottom;

						lpDefect[DefectCount].Height = (Rect.bottom - Rect.top);
						lpDefect[DefectCount].Width = (Rect.right - Rect.left);

						lpDefect[DefectCount].Ver_Dis = lpCircle[MaxCount].Dv;
						lpDefect[DefectCount].Hor_Dis = Math.GetLength(StaPos, EndPos);

						lpDefect[DefectCount].P.x = (float)Rect.left;
						lpDefect[DefectCount].P.y = (float)(Rect.top + ImageHeight);

						lpDefect[DefectCount].Rect.top = lpDefect[DefectCount].P.y;
						lpDefect[DefectCount].Rect.bottom = lpDefect[DefectCount].Rect.top + lpDefect[DefectCount].Height;

						lpDefect[DefectCount].Round = PixelCount;
						DefectCount++;
					}
				}
				DefectPoint = 0;
				IsRoof = false;
				IsFlagCh = true;
				MaxDis = 0.0f;
				MaxCount = 0;
			}

			if (IsRoof) {
				if (lpCircle[k].Dv > MaxDis) {
					MaxDis = lpCircle[k].Dv;
					MaxCount = k;
				}
				Rect.left = Rect.left < lpCircle[k].P.x && Rect.left != 0 ? Rect.left : (long)lpCircle[k].P.x;
				Rect.top = Rect.top < lpCircle[k].P.y && Rect.top != 0 ? Rect.top : (long)lpCircle[k].P.y;
				Rect.right = Rect.right > lpCircle[k].P.x ? Rect.right : (long)lpCircle[k].P.x;
				Rect.bottom = Rect.bottom > lpCircle[k].P.y ? Rect.bottom : (long)lpCircle[k].P.y;
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
				Rect.left = 0;
				Rect.top = 0;
				Rect.right = 0;
				Rect.bottom = 0;
			}

		}
	}
	return DefectCount;
}
int CFM15xLineDefect::GetPolygonSize(TFPoint* PolygonData, TBlob* lpDefect, int TotalCount, int Range, int DefectDv, int ThresHold, int PreDefectCnt)// Polygon Mode Dark Defect 
{
	bool IsRoof = false;
	bool IsBadRect = false;
	CRect Rect;
	Rect.left = 0;
	Rect.right = 0;
	Rect.top = 0;
	Rect.bottom = 0;

	int DefectCount = 0;
	int DefectPoint = 0;

	// Defect ����� ���� Quantity Score Calc 
	int PixelValue = 0;
	int PixelCount = 0;
	int PixelPoint = 0;
	int PixelScore = 0;
	float Score = 0.0f;

	int BadRectCount = 0;

	// Defect Merge Value
	const int MgCount = 3;
	const int xMerge = 10;
	const int yMerge = 10;

	// 3���� ���� Point ��ǥ 
	TFPoint f1(0.0f, 0.0f), f2(0.0f, 0.0f), f3(0.0f, 0.0f);

	// Polygon Vlaue 
	TFPoint* RangePoint = NULL;
	float Dv1 = 0.0f, Dv2 = 0.0F, Dv3 = 0.0f;

	//BadRect Init 
	for (int k = 0; k < MAX_POLYGON_DEFECT_COUNT; k++) {
		// Polygon Defect Data 
		BadRect[k].Rect.left = 0;
		BadRect[k].Rect.top = 0;
		BadRect[k].Rect.right = 0;
		BadRect[k].Rect.bottom = 0;
		BadRect[k].SPoint.x = 0.0f;
		BadRect[k].SPoint.y = 0.0f;
		BadRect[k].EPoint.x = 0.0f;
		BadRect[k].EPoint.y = 0.0f;
		BadRect[k].BadPixelCount = 0;

		for (int i = 0; i < MAX_POLYGON_DEFECT_POINT; i++) {
			BadRect[k].Pixel[i].x = 0.0f;
			BadRect[k].Pixel[i].y = 0.0f;
		}
	}

	// Range������ŭ Polygon Data Point ��Ȱ�Ͽ� Defect ���� Calc
	for (int k = 0; k < TotalCount; k += Range) {
		TFPoint* Q1 = &PolygonData[k];
		int v = (TotalCount - k) / Range;  // Range������ŭ ��Ȱ�� ������ ��
		int r = TotalCount % Range;  // ��ü �ȼ��� Range�������� ��Ȱ�� Range�������� ���������� ������� 
		int n = (v >= 1) ? Range : r;  // ��ü �ȼ��� Range�������� ��Ȱ�� Range�������� ���������� ������� 

		if (v < 0) break;

		// Range ������ŭ ��Ȱ�Ͽ� ���� �����Ѵ� 
		if (RangePoint != NULL) delete[] RangePoint;
		RangePoint = new TFPoint[n];

		// Range�������� ���� Polygon Pos ��ǥ �� BadPixel Pos��ǥ Init 
		for (int z = 0; z < n; z++) {
			RangePoint[z].x = Q1[z].x;
			RangePoint[z].y = Q1[z].y;
		}

		// Find Defct Pixel 
		f1 = RangePoint[0]; //Range�������� ù��° Point ��ǥ
		f2 = RangePoint[(int)(n / 2)]; //Range�������� ��� Point ��ǥ
		f3 = RangePoint[n - 1]; //Range�������� ������ Point ��ǥ

		// Rect Data Init 
		Rect.left = 0;
		Rect.top = 0;
		Rect.right = 0;
		Rect.bottom = 0;

		IsBadRect = false;
		PixelCount = 0;
		for (int i = 0; i < n; i++) { // Range�������� 3�� Point���� BadRect,BadPixel ������ �����Ѵ� 
			Dv1 = GetLengthFrLineToPoint(f1, f3, RangePoint[i]);
			Dv2 = GetLengthFrLineToPoint(f1, f2, RangePoint[i]);
			Dv3 = GetLengthFrLineToPoint(f2, f3, RangePoint[i]);

			if (Dv1 > DefectDv || Dv2 > DefectDv || Dv3 > DefectDv) { // Defect Point �����Ұ�� 
				if (!IsBadRect) { // Defect Point StaPos Data ���� ����
					IsBadRect = true;
					BadRect[BadRectCount].SPoint.x = RangePoint[i].x;
					BadRect[BadRectCount].SPoint.y = RangePoint[i].y;
				}

				if (IsBadRect) {
					Rect.left   = Rect.left   < RangePoint[i].x && Rect.left != 0 ? Rect.left : (long)RangePoint[i].x;
					Rect.top    = Rect.top    < RangePoint[i].y && Rect.top != 0 ? Rect.top : (long)RangePoint[i].y;
					Rect.right  = Rect.right  > RangePoint[i].x ? Rect.right : (long)RangePoint[i].x;
					Rect.bottom = Rect.bottom > RangePoint[i].y ? Rect.bottom : (long)RangePoint[i].y;

					BadRect[BadRectCount].Pixel[BadRect[BadRectCount].BadPixelCount] = RangePoint[i];
					BadRect[BadRectCount].EPoint.x = RangePoint[i].x; // End Pos ���� ������Ʈ 
					BadRect[BadRectCount].EPoint.y = RangePoint[i].y; // End Pos ���� ������Ʈ 
					BadRect[BadRectCount].BadPixelCount++;
				}
			}
		}

		if (IsBadRect) {
			BadRect[BadRectCount].Rect = Rect;

			float Distance = 0.0f;

			if (BadRectCount > 0) { // �ι�° BadRect���� ���� BadRect�� ���Ͽ� �Ÿ��� ���� 
				Distance = Math.GetLength(BadRect[BadRectCount].SPoint, BadRect[BadRectCount - 1].EPoint);
			}

			// ���� BadRect���� �����Ÿ� �����̸� ����BadRect�����Ϳ� Merge�Ѵ� 
			if (((Rect.right - Rect.left) > 2) && ((Rect.bottom - Rect.top) > 2) && Distance && (Distance < 20)) {
				BadRect[BadRectCount - 1].Rect.left = BadRect[BadRectCount - 1].Rect.left < Rect.left ? BadRect[BadRectCount - 1].Rect.left : Rect.left;
				BadRect[BadRectCount - 1].Rect.top = BadRect[BadRectCount - 1].Rect.top < Rect.top ? BadRect[BadRectCount - 1].Rect.top : Rect.top;
				BadRect[BadRectCount - 1].Rect.right = BadRect[BadRectCount - 1].Rect.right > Rect.right ? BadRect[BadRectCount - 1].Rect.right : Rect.right;
				BadRect[BadRectCount - 1].Rect.bottom = BadRect[BadRectCount - 1].Rect.bottom > Rect.bottom ? BadRect[BadRectCount - 1].Rect.bottom : Rect.bottom;

				BadRect[BadRectCount - 1].EPoint = BadRect[BadRectCount].EPoint;

				for (int b = 0; b < BadRect[BadRectCount].BadPixelCount; b++) { // ���� BadPixelData���� ���� BadRect�� �Ѱ��ش� 
					if (BadRect[BadRectCount - 1].BadPixelCount >= MAX_POLYGON_DEFECT_POINT) {
						for (int k = 0; k < BadRect[BadRectCount].BadPixelCount; k++) {
							BadRect[BadRectCount].Pixel[k].x = 0;
							BadRect[BadRectCount].Pixel[k].y = 0;
						}
						break;
					}
					BadRect[BadRectCount - 1].Pixel[BadRect[BadRectCount - 1].BadPixelCount] = BadRect[BadRectCount].Pixel[b];
					BadRect[BadRectCount - 1].BadPixelCount++;

					// ���� Data Init 
					BadRect[BadRectCount].Pixel[b].x = 0;
					BadRect[BadRectCount].Pixel[b].y = 0;
				}
				BadRect[BadRectCount].BadPixelCount = 0;
				BadRect[BadRectCount].EPoint.x = 0;
				BadRect[BadRectCount].EPoint.y = 0;
				BadRect[BadRectCount].SPoint.x = 0;
				BadRect[BadRectCount].SPoint.y = 0;
			}
			else {
				if (BadRectCount >= MAX_POLYGON_DEFECT_COUNT) break;
				BadRectCount++;
			}
		}
	}

	// Range ������ŭ ��Ȱ�Ͽ� ���� �����Ѵ� 
	// Range �������� ������ ������ ���� 
	if (RangePoint != NULL) delete[] RangePoint;

	// BadRect Data ���� 1�� ���͸� �� ���� lpDefect�� ������ �Ѱ���
	if (BadRectCount > 0) {
		TFPoint Target(0.0f, 0.0f);

		for (int k = 0; k < BadRectCount; k++) {
			IsRoof = false;
			for (int i = 0; i < BadRect[k].BadPixelCount; i++) {
				f1 = BadRect[k].SPoint; // BadRect ������ ��ǥ
				f2 = BadRect[k].EPoint; // BadRect ������ ��ǥ 
				Target = BadRect[k].Pixel[i]; // BddRect ����   ��ǥ 
				Dv1 = GetLengthFrLineToPoint(f1, f2, Target);

				if (Dv1 > 3) {
					if (!IsRoof) { // Defect Point StaPos Data ���� ����
						IsRoof = true;
					}

					if (IsRoof) {
						Rect.left = Rect.left < Target.x && Rect.left != 0 ? Rect.left : (long)Target.x;
						Rect.top = Rect.top < Target.y && Rect.top != 0 ? Rect.top : (long)Target.y;
						Rect.right = Rect.right > Target.x ? Rect.right : (long)Target.x;
						Rect.bottom = Rect.bottom > Target.y ? Rect.bottom : (long)Target.y;
						DefectPoint++;
					}
				}
				else { // Defect Point ������� 
					if (IsRoof) {
						if (DefectPoint > 10) {
							// ���� Pixel Data ���� 
							PixelCount = 0, PixelPoint = 0, PixelScore = 5, Score = 0.0f;
							for (int n = Rect.top; n < Rect.bottom; n++) {
								for (int m = Rect.left; m < Rect.right; m++) {
									if (m <= 0 || n <= 0 || m > FrameImgW || n >= FrameImgH) continue;
									PixelValue = IElement(m, n);
									if (PixelValue < ThresHold) PixelCount++;
									PixelPoint++;
								}
							}
							Score = (((float)PixelCount / (float)PixelPoint) * 100);
							// Circle Defect ������ PixelCount ������ üũ�Ѵ� => ����ų ���� ���� 
							if (Score > PixelScore) {
								lpDefect[PreDefectCnt].Rect.left = Rect.left;
								lpDefect[PreDefectCnt].Rect.top = Rect.top;
								lpDefect[PreDefectCnt].Rect.right = Rect.right;
								lpDefect[PreDefectCnt].Rect.bottom = Rect.bottom;

								lpDefect[PreDefectCnt].Height = (Rect.bottom - Rect.top);
								lpDefect[PreDefectCnt].Width = (Rect.right - Rect.left);

								lpDefect[PreDefectCnt].Ver_Dis = lpDefect[PreDefectCnt].Height;
								lpDefect[PreDefectCnt].Hor_Dis = lpDefect[PreDefectCnt].Width;

								lpDefect[PreDefectCnt].P.x = (float)(Rect.left);
								lpDefect[PreDefectCnt].P.y = (float)(Rect.top + ImageHeight); // ��ü Frame���� Real��ǥ �������� ��ȯ 

								lpDefect[PreDefectCnt].Rect.top = lpDefect[PreDefectCnt].P.y;
								lpDefect[PreDefectCnt].Rect.bottom = lpDefect[PreDefectCnt].Rect.top + lpDefect[PreDefectCnt].Height;
								lpDefect[PreDefectCnt].Round = PixelCount;
								PreDefectCnt++;
								DefectCount++;

								// Data Init 
								DefectPoint = 0;
								Rect.left = 0;
								Rect.top = 0;
								Rect.right = 0;
								Rect.bottom = 0;
								IsRoof = false;
							}
							else {
								DefectPoint = 0;
								Rect.left = 0;
								Rect.top = 0;
								Rect.right = 0;
								Rect.bottom = 0;
								IsRoof = false;
							}
						}
						else { // Defect Count ������� �ʱ�ȭ 
							DefectPoint = 0;
							Rect.left = 0;
							Rect.top = 0;
							Rect.right = 0;
							Rect.bottom = 0;
							IsRoof = false;
						}
					}
					else { // �ּ� Defect Distance ������� 
						Rect.left = 0;
						Rect.top = 0;
						Rect.right = 0;
						Rect.bottom = 0;
					}
				}
			}
		}
	}

	// Defect Data���� Merge ������ 
	if (DefectCount > 1) { // Defect������ �ּ� 2���̻� �����Ұ�� Merge ������ ������
		bool IsxMg = false, IsyMg = false, IsInMg = false;
		CRect r1, r2;
		int sort = 0;
		int tmpDDefectCount = DefectCount;

		for (int k = PreDefectCnt - DefectCount; k < tmpDDefectCount + DefectCount; k++) {
			if (lpDefect[k].Rect.left == 0 && lpDefect[k].Rect.top == 0 && lpDefect[k].Rect.right == 0 && lpDefect[k].Rect.bottom == 0) continue;

			for (int l = k + 1; l < k + MgCount; l++) {
				IsxMg = false; IsyMg = false;
				if (l > (tmpDDefectCount - 1)) break;
				if (lpDefect[l].Rect.left == 0 && lpDefect[l].Rect.top == 0 && lpDefect[k].Rect.right == 0 && lpDefect[k].Rect.bottom == 0) continue;

				r1 = lpDefect[k].Rect;
				r2 = lpDefect[l].Rect;

				register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
				register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
				IsInMg = ((r1.left <= r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right > r2.right)) ? true : false;

				if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

				IsxMg = (HorDist < xMerge) ? true : false;
				IsyMg = (VerDist < yMerge) ? true : false;

				if ((IsxMg && IsyMg) || IsInMg) {
					lpDefect[k].Round += lpDefect[l].Round;
					lpDefect[k].Rect = OnMerge(lpDefect[k].Rect, lpDefect[l].Rect);
					lpDefect[k].Width = lpDefect[k].Rect.right - lpDefect[k].Rect.left;
					lpDefect[k].Height = lpDefect[k].Rect.bottom - lpDefect[k].Rect.top;
					lpDefect[k].Hor_Dis = lpDefect[k].Width;
					lpDefect[k].Ver_Dis = lpDefect[k].Height;
					lpDefect[k].P.x = (float)(lpDefect[k].Rect.left);
					lpDefect[k].P.y = (float)(lpDefect[k].Rect.top);
					lpDefect[l].Rect.left = lpDefect[l].Rect.top = lpDefect[l].Rect.right = lpDefect[l].Rect.bottom = 0;
					DefectCount--;
					k = -1;
					break;
				}
			}
		}
		for (int j = PreDefectCnt - DefectCount; j < tmpDDefectCount + DefectCount; j++) {
			if (lpDefect[j].Rect.left == 0 && lpDefect[j].Rect.top == 0 && lpDefect[j].Rect.right == 0 && lpDefect[j].Rect.bottom == 0) continue;
			lpDefect[sort] = lpDefect[j];
			sort++;
		}
	}
	return DefectCount;
}
int CFM15xLineDefect::GetAvgCellSize(int TopCount, int MidCount, int BtmCount, int AvgMode)
{
	if (MarkStrPos.x == 0 || MarkStrPos.y == 0 || MarkEndPos.x == 0 || MarkEndPos.y == 0) return false;

	int nTotalCellSize = 0;
	int nCellSize = 0;
	int nAvgCellSize = 0;


	int nCellSizeCount = 0;

	if (AvgMode == 0) {  // TopCellSizeAvg
		for (int k1 = 0; k1 < TopCount; k1++) {
			if (FCellPoint[k1].x == 0 || FCellPoint[k1].y == 0) continue;
			nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkStrPos, MarkEndPos, FCellPoint[k1]));
			nTotalCellSize += nCellSize;
			nCellSizeCount++;
		}
		if (nTotalCellSize > 0 && nCellSizeCount > 0) nAvgCellSize = nTotalCellSize / nCellSizeCount;
	}
	else if (AvgMode == 1) {  // MidCellSizeAvg
		for (int k2 = TopCount; k2 < TopCount + MidCount; k2++) {
			if (FCellPoint[k2].x == 0 || FCellPoint[k2].y == 0) continue;
			nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkStrPos, MarkEndPos, FCellPoint[k2]));
			nTotalCellSize += nCellSize;
			nCellSizeCount++;
		}
		if (nTotalCellSize > 0 && nCellSizeCount > 0) nAvgCellSize = nTotalCellSize / nCellSizeCount;

	}
	else if (AvgMode == 2) {  // BtmCellSizeAvg
		for (int k3 = TopCount + MidCount; k3 < TopCount + MidCount + BtmCount; k3++) {
			if (FCellPoint[k3].x == 0 || FCellPoint[k3].y == 0) continue;
			nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkStrPos, MarkEndPos, FCellPoint[k3]));
			nTotalCellSize += nCellSize;
			nCellSizeCount++;
		}
		if (nTotalCellSize > 0 && nCellSizeCount > 0) nAvgCellSize = nTotalCellSize / nCellSizeCount;

	}
	else if (AvgMode == 3) {  // AllCellSizeAvg
		for (int k4 = 0; k4 < TopCount + MidCount + BtmCount; k4++) {
			if (FCellPoint[k4].x == 0 || FCellPoint[k4].y == 0) continue;
			nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkPos, MarkEndPos, FCellPoint[k4]));
			nTotalCellSize += nCellSize;
			nCellSizeCount++;
		}
		if (nTotalCellSize > 0 && nCellSizeCount > 0) nAvgCellSize = nTotalCellSize / nCellSizeCount;
	}
	return  nAvgCellSize;
}
int CFM15xLineDefect::GetCellSize(int Count)
{
	if (MarkStrPos.x == 0 || MarkStrPos.y == 0 || MarkEndPos.x == 0 || MarkEndPos.y == 0) return false;
	if (FCellPoint[Count].x == 0 || FCellPoint[Count].y == 0)                        return false;
	if (Count >= MAX_DEFECT_POINT)                                                  return false;

	int nCellSize = 0;

	nCellSize = (int)(Math.GetLengthFrLineToPoint(MarkStrPos, MarkEndPos, FCellPoint[Count]));
	return  nCellSize;
}
void CFM15xLineDefect::OnClearData()
{
	for (int k = 0; k < MAX_DEFECT_POINT; k++) {
		// Cell Inspection Init
		// Dark Defect 
		ADefectPoint[k].Height = 0;
		ADefectPoint[k].Width = 0;
		ADefectPoint[k].Round = 0;
		ADefectPoint[k].P.x = 0;
		ADefectPoint[k].P.y = 0;
		ADefectPoint[k].Ver_Dis = 0;
		ADefectPoint[k].Hor_Dis = 0;
		ADefectPoint[k].LineAttach = 0;

		BDefectPoint[k].Height = 0;
		BDefectPoint[k].Width = 0;
		BDefectPoint[k].Round = 0;
		BDefectPoint[k].P.x = 0;
		BDefectPoint[k].P.y = 0;
		BDefectPoint[k].Ver_Dis = 0;
		BDefectPoint[k].Hor_Dis = 0;
		BDefectPoint[k].LineAttach = 0;

		CDefectPoint[k].Height = 0;
		CDefectPoint[k].Width = 0;
		CDefectPoint[k].Round = 0;
		CDefectPoint[k].P.x = 0;
		CDefectPoint[k].P.y = 0;
		CDefectPoint[k].Ver_Dis = 0;
		CDefectPoint[k].Hor_Dis = 0;
		CDefectPoint[k].LineAttach = 0;


		DDefectPoint[k].Height = 0;
		DDefectPoint[k].Width = 0;
		DDefectPoint[k].Round = 0;
		DDefectPoint[k].P.x = 0;
		DDefectPoint[k].P.y = 0;
		DDefectPoint[k].Ver_Dis = 0;
		DDefectPoint[k].Hor_Dis = 0;
		DDefectPoint[k].LineAttach = 0;


		// Bright Defect 
		ACrackPoint[k].Height = 0;
		ACrackPoint[k].Width = 0;
		ACrackPoint[k].Round = 0;
		ACrackPoint[k].P.x = 0;
		ACrackPoint[k].P.y = 0;
		ACrackPoint[k].Ver_Dis = 0;
		ACrackPoint[k].Hor_Dis = 0;
		ACrackPoint[k].LineAttach = 0;

		BCrackPoint[k].Height = 0;
		BCrackPoint[k].Width = 0;
		BCrackPoint[k].Round = 0;
		BCrackPoint[k].P.x = 0;
		BCrackPoint[k].P.y = 0;
		BCrackPoint[k].Ver_Dis = 0;
		BCrackPoint[k].Hor_Dis = 0;
		BCrackPoint[k].LineAttach = 0;

		CCrackPoint[k].Height = 0;
		CCrackPoint[k].Width = 0;
		CCrackPoint[k].Round = 0;
		CCrackPoint[k].P.x = 0;
		CCrackPoint[k].P.y = 0;
		CCrackPoint[k].Ver_Dis = 0;
		CCrackPoint[k].Hor_Dis = 0;
		CCrackPoint[k].LineAttach = 0;

		DCrackPoint[k].Height = 0;
		DCrackPoint[k].Width = 0;
		DCrackPoint[k].Round = 0;
		DCrackPoint[k].P.x = 0;
		DCrackPoint[k].P.y = 0;
		DCrackPoint[k].Ver_Dis = 0;
		DCrackPoint[k].Hor_Dis = 0;
		DCrackPoint[k].LineAttach = 0;

		C1DefectPoint[k].Height = 0;
		C1DefectPoint[k].Width = 0;
		C1DefectPoint[k].Round = 0;
		C1DefectPoint[k].P.x = 0;
		C1DefectPoint[k].P.y = 0;
		C1DefectPoint[k].Ver_Dis = 0;
		C1DefectPoint[k].Hor_Dis = 0;
		C1DefectPoint[k].LineAttach = 0;

		C2DefectPoint[k].Height = 0;
		C2DefectPoint[k].Width = 0;
		C2DefectPoint[k].Round = 0;
		C2DefectPoint[k].P.x = 0;
		C2DefectPoint[k].P.y = 0;
		C2DefectPoint[k].Ver_Dis = 0;
		C2DefectPoint[k].Hor_Dis = 0;
		C2DefectPoint[k].LineAttach = 0;

		C3DefectPoint[k].Height = 0;
		C3DefectPoint[k].Width = 0;
		C3DefectPoint[k].Round = 0;
		C3DefectPoint[k].P.x = 0;
		C3DefectPoint[k].P.y = 0;
		C3DefectPoint[k].Ver_Dis = 0;
		C3DefectPoint[k].Hor_Dis = 0;
		C3DefectPoint[k].LineAttach = 0;


		C4DefectPoint[k].Height = 0;
		C4DefectPoint[k].Width = 0;
		C4DefectPoint[k].Round = 0;
		C4DefectPoint[k].P.x = 0;
		C4DefectPoint[k].P.y = 0;
		C4DefectPoint[k].Ver_Dis = 0;
		C4DefectPoint[k].Hor_Dis = 0;
		C4DefectPoint[k].LineAttach = 0;

		// Circle Defect Data Init
		// Dark Defect 
		A1CirDefectPoint[k].Height = 0;
		A1CirDefectPoint[k].Width = 0;
		A1CirDefectPoint[k].Round = 0;
		A1CirDefectPoint[k].P.x = 0;
		A1CirDefectPoint[k].P.y = 0;
		A1CirDefectPoint[k].Rect.left = 0;
		A1CirDefectPoint[k].Rect.top = 0;
		A1CirDefectPoint[k].Rect.right = 0;
		A1CirDefectPoint[k].Rect.bottom = 0;
		A1CirDefectPoint[k].Ver_Dis = 0;
		A1CirDefectPoint[k].Hor_Dis = 0;
		A1CirDefectPoint[k].LineAttach = 0;

		A2CirDefectPoint[k].Height = 0;
		A2CirDefectPoint[k].Width = 0;
		A2CirDefectPoint[k].Round = 0;
		A2CirDefectPoint[k].P.x = 0;
		A2CirDefectPoint[k].P.y = 0;
		A2CirDefectPoint[k].Rect.left = 0;
		A2CirDefectPoint[k].Rect.top = 0;
		A2CirDefectPoint[k].Rect.right = 0;
		A2CirDefectPoint[k].Rect.bottom = 0;
		A2CirDefectPoint[k].Ver_Dis = 0;
		A2CirDefectPoint[k].Hor_Dis = 0;
		A2CirDefectPoint[k].LineAttach = 0;

		C1CirDefectPoint[k].Height = 0;
		C1CirDefectPoint[k].Width = 0;
		C1CirDefectPoint[k].Round = 0;
		C1CirDefectPoint[k].P.x = 0;
		C1CirDefectPoint[k].P.y = 0;
		C1CirDefectPoint[k].Rect.left = 0;
		C1CirDefectPoint[k].Rect.top = 0;
		C1CirDefectPoint[k].Rect.right = 0;
		C1CirDefectPoint[k].Rect.bottom = 0;
		C1CirDefectPoint[k].Ver_Dis = 0;
		C1CirDefectPoint[k].Hor_Dis = 0;
		C1CirDefectPoint[k].LineAttach = 0;

		C2CirDefectPoint[k].Height = 0;
		C2CirDefectPoint[k].Width = 0;
		C2CirDefectPoint[k].Round = 0;
		C2CirDefectPoint[k].P.x = 0;
		C2CirDefectPoint[k].P.y = 0;
		C2CirDefectPoint[k].Rect.left = 0;
		C2CirDefectPoint[k].Rect.top = 0;
		C2CirDefectPoint[k].Rect.right = 0;
		C2CirDefectPoint[k].Rect.bottom = 0;
		C2CirDefectPoint[k].Ver_Dis = 0;
		C2CirDefectPoint[k].Hor_Dis = 0;
		C2CirDefectPoint[k].LineAttach = 0;

		LeftDefectPoint[k].Height = 0;
		LeftDefectPoint[k].Width = 0;
		LeftDefectPoint[k].Round = 0;
		LeftDefectPoint[k].P.x = 0;
		LeftDefectPoint[k].P.y = 0;
		LeftDefectPoint[k].Rect.left = 0;
		LeftDefectPoint[k].Rect.top = 0;
		LeftDefectPoint[k].Rect.right = 0;
		LeftDefectPoint[k].Rect.bottom = 0;
		LeftDefectPoint[k].Ver_Dis = 0;
		LeftDefectPoint[k].Hor_Dis = 0;
		LeftDefectPoint[k].LineAttach = 0;

		TopDefectPoint[k].Height = 0;
		TopDefectPoint[k].Width = 0;
		TopDefectPoint[k].Round = 0;
		TopDefectPoint[k].P.x = 0;
		TopDefectPoint[k].P.y = 0;
		TopDefectPoint[k].Rect.left = 0;
		TopDefectPoint[k].Rect.top = 0;
		TopDefectPoint[k].Rect.right = 0;
		TopDefectPoint[k].Rect.bottom = 0;
		TopDefectPoint[k].Ver_Dis = 0;
		TopDefectPoint[k].Hor_Dis = 0;
		TopDefectPoint[k].LineAttach = 0;

		RightDefectPoint[k].Height = 0;
		RightDefectPoint[k].Width = 0;
		RightDefectPoint[k].Round = 0;
		RightDefectPoint[k].P.x = 0;
		RightDefectPoint[k].P.y = 0;
		RightDefectPoint[k].Rect.left = 0;
		RightDefectPoint[k].Rect.top = 0;
		RightDefectPoint[k].Rect.right = 0;
		RightDefectPoint[k].Rect.bottom = 0;
		RightDefectPoint[k].Ver_Dis = 0;
		RightDefectPoint[k].Hor_Dis = 0;
		RightDefectPoint[k].LineAttach = 0;

		BtmDefectPoint[k].Height = 0;
		BtmDefectPoint[k].Width = 0;
		BtmDefectPoint[k].Round = 0;
		BtmDefectPoint[k].P.x = 0;
		BtmDefectPoint[k].P.y = 0;
		BtmDefectPoint[k].Rect.left = 0;
		BtmDefectPoint[k].Rect.top = 0;
		BtmDefectPoint[k].Rect.right = 0;
		BtmDefectPoint[k].Rect.bottom = 0;
		BtmDefectPoint[k].Ver_Dis = 0;
		BtmDefectPoint[k].Hor_Dis = 0;
		BtmDefectPoint[k].LineAttach = 0;

		// Bright Defect 
		A1CirCrackPoint[k].Height = 0;
		A1CirCrackPoint[k].Width = 0;
		A1CirCrackPoint[k].Round = 0;
		A1CirCrackPoint[k].P.x = 0;
		A1CirCrackPoint[k].P.y = 0;
		A1CirCrackPoint[k].Rect.left = 0;
		A1CirCrackPoint[k].Rect.top = 0;
		A1CirCrackPoint[k].Rect.right = 0;
		A1CirCrackPoint[k].Rect.bottom = 0;
		A1CirCrackPoint[k].Ver_Dis = 0;
		A1CirCrackPoint[k].Hor_Dis = 0;
		A1CirCrackPoint[k].LineAttach = 0;

		A2CirCrackPoint[k].Height = 0;
		A2CirCrackPoint[k].Width = 0;
		A2CirCrackPoint[k].Round = 0;
		A2CirCrackPoint[k].P.x = 0;
		A2CirCrackPoint[k].P.y = 0;
		A2CirCrackPoint[k].Rect.left = 0;
		A2CirCrackPoint[k].Rect.top = 0;
		A2CirCrackPoint[k].Rect.right = 0;
		A2CirCrackPoint[k].Rect.bottom = 0;
		A2CirCrackPoint[k].Ver_Dis = 0;
		A2CirCrackPoint[k].Hor_Dis = 0;
		A2CirCrackPoint[k].LineAttach = 0;

		C1CirCrackPoint[k].Height = 0;
		C1CirCrackPoint[k].Width = 0;
		C1CirCrackPoint[k].Round = 0;
		C1CirCrackPoint[k].P.x = 0;
		C1CirCrackPoint[k].P.y = 0;
		C1CirCrackPoint[k].Rect.left = 0;
		C1CirCrackPoint[k].Rect.top = 0;
		C1CirCrackPoint[k].Rect.right = 0;
		C1CirCrackPoint[k].Rect.bottom = 0;
		C1CirCrackPoint[k].Ver_Dis = 0;
		C1CirCrackPoint[k].Hor_Dis = 0;
		C1CirCrackPoint[k].LineAttach = 0;

		C2CirCrackPoint[k].Height = 0;
		C2CirCrackPoint[k].Width = 0;
		C2CirCrackPoint[k].Round = 0;
		C2CirCrackPoint[k].P.x = 0;
		C2CirCrackPoint[k].P.y = 0;
		C2CirCrackPoint[k].Rect.left = 0;
		C2CirCrackPoint[k].Rect.top = 0;
		C2CirCrackPoint[k].Rect.right = 0;
		C2CirCrackPoint[k].Rect.bottom = 0;
		C2CirCrackPoint[k].Ver_Dis = 0;
		C2CirCrackPoint[k].Hor_Dis = 0;
		C2CirCrackPoint[k].LineAttach = 0;

		LeftCrackPoint[k].Height = 0;
		LeftCrackPoint[k].Width = 0;
		LeftCrackPoint[k].Round = 0;
		LeftCrackPoint[k].P.x = 0;
		LeftCrackPoint[k].P.y = 0;
		LeftCrackPoint[k].Rect.left = 0;
		LeftCrackPoint[k].Rect.top = 0;
		LeftCrackPoint[k].Rect.right = 0;
		LeftCrackPoint[k].Rect.bottom = 0;
		LeftCrackPoint[k].Ver_Dis = 0;
		LeftCrackPoint[k].Hor_Dis = 0;
		LeftCrackPoint[k].LineAttach = 0;

		TopCrackPoint[k].Height = 0;
		TopCrackPoint[k].Width = 0;
		TopCrackPoint[k].Round = 0;
		TopCrackPoint[k].P.x = 0;
		TopCrackPoint[k].P.y = 0;
		TopCrackPoint[k].Rect.left = 0;
		TopCrackPoint[k].Rect.top = 0;
		TopCrackPoint[k].Rect.right = 0;
		TopCrackPoint[k].Rect.bottom = 0;
		TopCrackPoint[k].Ver_Dis = 0;
		TopCrackPoint[k].Hor_Dis = 0;
		TopCrackPoint[k].LineAttach = 0;

		RightCrackPoint[k].Height = 0;
		RightCrackPoint[k].Width = 0;
		RightCrackPoint[k].Round = 0;
		RightCrackPoint[k].P.x = 0;
		RightCrackPoint[k].P.y = 0;
		RightCrackPoint[k].Rect.left = 0;
		RightCrackPoint[k].Rect.top = 0;
		RightCrackPoint[k].Rect.right = 0;
		RightCrackPoint[k].Rect.bottom = 0;
		RightCrackPoint[k].Ver_Dis = 0;
		RightCrackPoint[k].Hor_Dis = 0;
		RightCrackPoint[k].LineAttach = 0;

		BtmCrackPoint[k].Height = 0;
		BtmCrackPoint[k].Width = 0;
		BtmCrackPoint[k].Round = 0;
		BtmCrackPoint[k].P.x = 0;
		BtmCrackPoint[k].P.y = 0;
		BtmCrackPoint[k].Rect.left = 0;
		BtmCrackPoint[k].Rect.top = 0;
		BtmCrackPoint[k].Rect.right = 0;
		BtmCrackPoint[k].Rect.bottom = 0;
		BtmCrackPoint[k].Ver_Dis = 0;
		BtmCrackPoint[k].Hor_Dis = 0;
		BtmCrackPoint[k].LineAttach = 0;

		FCellPoint[k].x = 0.0f;
		FCellPoint[k].y = 0.0f;
	}
	// TOP/BTM Broken Inspection StartLine

	// Line Data 
	LeftLineData = 0.0f;
	RightLineData = 0.0f;

	// NOTCH CELL INSPECTION LineData Init
	LeftOutLine[0].FP0.x = 0.0f;
	LeftOutLine[0].FP0.y = 0.0f;
	LeftOutLine[0].FP1.x = 0.0f;
	LeftOutLine[0].FP1.y = 0.0f;
	LeftInLine[0].FP0.x = 0.0f;
	LeftInLine[0].FP0.y = 0.0f;
	LeftInLine[0].FP1.x = 0.0f;
	LeftInLine[0].FP1.y = 0.0f;

	TopOutLine[0].FP0.x = 0.0f;
	TopOutLine[0].FP0.y = 0.0f;
	TopOutLine[0].FP1.x = 0.0f;
	TopOutLine[0].FP1.y = 0.0f;
	TopInLine[0].FP0.x = 0.0f;
	TopInLine[0].FP0.y = 0.0f;
	TopInLine[0].FP1.x = 0.0f;
	TopInLine[0].FP1.y = 0.0f;

	RightOutLine[0].FP0.x = 0.0f;
	RightOutLine[0].FP0.y = 0.0f;
	RightOutLine[0].FP1.x = 0.0f;
	RightOutLine[0].FP1.y = 0.0f;
	RightInLine [0].FP0.x = 0.0f;
	RightInLine [0].FP0.y = 0.0f;
	RightInLine [0].FP1.x = 0.0f;
	RightInLine [0].FP1.y = 0.0f;

	for (int k = 0; k < MAX_BTM_COUNT; k++) {
		BtmOutLine[k].FP0.x = 0.0f;
		BtmOutLine[k].FP0.y = 0.0f;
		BtmOutLine[k].FP1.x = 0.0f;
		BtmOutLine[k].FP1.y = 0.0f;
		BtmInLine[k].FP0.x = 0.0f;
		BtmInLine[k].FP0.y = 0.0f;
		BtmInLine[k].FP1.x = 0.0f;
		BtmInLine[k].FP1.y = 0.0f;
	}

	//Line Data Init
	for (int i = 0; i < MAX_LINE_POINT; i++) {
		Line[i].FP0.x = 0.0f;
		Line[i].FP0.y = 0.0f;
		Line[i].FP1.x = 0.0f;
		Line[i].FP1.y = 0.0f;
	}

	// ASide Grind Mark Data Init 
	for (int k = 0; k < MAX_GRINDMARK_COUNT; k++) {
		AGrindMark[k].Rect.left  = 0   ;
		AGrindMark[k].Rect.top   = 0   ;
		AGrindMark[k].Rect.right = 0   ;
		AGrindMark[k].Rect.bottom= 0   ;
		AGrindMark[k].RectPoint.x= 0.0f;
		AGrindMark[k].RectPoint.y= 0.0f;
		AGrindMark[k].Point.x    = 0.0f;
		AGrindMark[k].Point.y    = 0.0f;
		AGrindMark[k].DvX        = 0   ;
		AGrindMark[k].DvY        = 0   ;
		AGrindMark[k].Range      = 0   ;
		AGrindMark[k].degree     = 0.0f;
		AGrindMark[k].Score      = 0.0f;
		AGrindMark[k].Gradient   = 0.0f;
		AGrindMark[k].GrindWidth = 0.0f;
		AGrindMark[k].GrindMea   = 0.0f;

		BGrindMark[k].Rect.left  = 0   ;
		BGrindMark[k].Rect.top   = 0   ;
		BGrindMark[k].Rect.right = 0   ;
		BGrindMark[k].Rect.bottom= 0   ;
		BGrindMark[k].RectPoint.x= 0.0f;
		BGrindMark[k].RectPoint.y= 0.0f;
		BGrindMark[k].Point.x    = 0.0f;
		BGrindMark[k].Point.y    = 0.0f;
		BGrindMark[k].DvX        = 0   ;
		BGrindMark[k].DvY        = 0   ;
		BGrindMark[k].Range      = 0   ;
		BGrindMark[k].degree     = 0.0f;
		BGrindMark[k].Score      = 0.0f;
		BGrindMark[k].Gradient   = 0.0f;
		BGrindMark[k].GrindWidth = 0.0f;
		BGrindMark[k].GrindMea   = 0.0f;

		CGrindMark[k].Rect.left  = 0   ;
		CGrindMark[k].Rect.top   = 0   ;
		CGrindMark[k].Rect.right = 0   ;
		CGrindMark[k].Rect.bottom= 0   ;
		CGrindMark[k].RectPoint.x= 0.0f;
		CGrindMark[k].RectPoint.y= 0.0f;
		CGrindMark[k].Point.x    = 0.0f;
		CGrindMark[k].Point.y    = 0.0f;
		CGrindMark[k].DvX        = 0   ;
		CGrindMark[k].DvY        = 0   ;
		CGrindMark[k].Range      = 0   ;
		CGrindMark[k].degree     = 0.0f;
		CGrindMark[k].Score      = 0.0f;
		CGrindMark[k].Gradient   = 0.0f;
		CGrindMark[k].GrindWidth = 0.0f;
		CGrindMark[k].GrindMea   = 0.0f;

		DGrindMark[k].Rect.left  = 0   ;
		DGrindMark[k].Rect.top   = 0   ;
		DGrindMark[k].Rect.right = 0   ;
		DGrindMark[k].Rect.bottom= 0   ;
		DGrindMark[k].RectPoint.x= 0.0f;
		DGrindMark[k].RectPoint.y= 0.0f;
		DGrindMark[k].Point.x    = 0.0f;
		DGrindMark[k].Point.y    = 0.0f;
		DGrindMark[k].DvX        = 0   ;
		DGrindMark[k].DvY        = 0   ;
		DGrindMark[k].Range      = 0   ;
		DGrindMark[k].degree     = 0.0f;
		DGrindMark[k].Score      = 0.0f;
		DGrindMark[k].Gradient   = 0.0f;
		DGrindMark[k].GrindWidth = 0.0f;
		DGrindMark[k].GrindMea   = 0.0f;

		H1GrindMark[k].Rect.left  = 0   ;
		H1GrindMark[k].Rect.top   = 0   ;
		H1GrindMark[k].Rect.right = 0   ;
		H1GrindMark[k].Rect.bottom= 0   ;
		H1GrindMark[k].RectPoint.x= 0.0f;
		H1GrindMark[k].RectPoint.y= 0.0f;
		H1GrindMark[k].Point.x    = 0.0f;
		H1GrindMark[k].Point.y    = 0.0f;
		H1GrindMark[k].DvX        = 0   ;
		H1GrindMark[k].DvY        = 0   ;
		H1GrindMark[k].Range      = 0   ;
		H1GrindMark[k].degree     = 0.0f;
		H1GrindMark[k].Score      = 0.0f;
		H1GrindMark[k].Gradient   = 0.0f;
		H1GrindMark[k].GrindWidth = 0.0f;
		H1GrindMark[k].GrindMea   = 0.0f;

		H2GrindMark[k].Rect.left  = 0   ;
		H2GrindMark[k].Rect.top   = 0   ;
		H2GrindMark[k].Rect.right = 0   ;
		H2GrindMark[k].Rect.bottom= 0   ;
		H2GrindMark[k].RectPoint.x= 0.0f;
		H2GrindMark[k].RectPoint.y= 0.0f;
		H2GrindMark[k].Point.x    = 0.0f;
		H2GrindMark[k].Point.y    = 0.0f;
		H2GrindMark[k].DvX        = 0   ;
		H2GrindMark[k].DvY        = 0   ;
		H2GrindMark[k].Range      = 0   ;
		H2GrindMark[k].degree     = 0.0f;
		H2GrindMark[k].Score      = 0.0f;
		H2GrindMark[k].Gradient   = 0.0f;
		H2GrindMark[k].GrindWidth = 0.0f;
		H2GrindMark[k].GrindMea   = 0.0f;
	}

	// ASide Pattern Distance 
	for (int k = 0; k < MAX_GRINDMARK_COUNT; k++) {
		PatternDis[k].Dis = 0.0f;
	}

	// Pad Roi Rect 
	for (int k = 0; k < 5; k++) {
		PadPolyRect[k].left = 0;
		PadPolyRect[k].top = 0;
		PadPolyRect[k].right = 0;
		PadPolyRect[k].bottom = 0;
	}

	//Defect Roi Rect 
	for (int k=0; k<MAX_DEFECT_ROI_COUNT; k++){
		DefectRoiRect[k].left   = 0;
		DefectRoiRect[k].top    = 0;
		DefectRoiRect[k].right  = 0;
		DefectRoiRect[k].bottom = 0;
	}

	// Notch Roi Rect 
	for (int k = 0; k < MAX_CUT_GRIND_ROI; k++) {
		NotchPolyRect[k].left = 0;
		NotchPolyRect[k].top = 0;
		NotchPolyRect[k].right = 0;
		NotchPolyRect[k].bottom = 0;
	}

	// 	for(int k=0; k<5; k++){
	// 		NotchPolyRect[k].left   =0;
	// 		NotchPolyRect[k].top    =0;
	// 		NotchPolyRect[k].right  =0;
	// 		NotchPolyRect[k].bottom =0;
	// 	}

		// CornerData Init
	C1EdgeCount = 0;
	C2EdgeCount = 0;
	C3EdgeCount = 0;
	C4EdgeCount = 0;

	// Corner InEdge Count
	C1InEdgeCount = 0;
	C2InEdgeCount = 0;
	C3InEdgeCount = 0;
	C4InEdgeCount = 0;

	// Corner Grind Measure Stand Edge Count 
	C1MeaEdgeCount = 0;
	C2MeaEdgeCount = 0;
	C3MeaEdgeCount = 0;
	C4MeaEdgeCount = 0;

	// Inspection End
	TopLimitLine = 0;
	BtmLimitLine = 0;

	// Circle Data Init
	//----------------------------------------------------------------
	// Circle Data 
	for (int k = 0; k < MAX_CIRCLE_EDGEPOINT; k++) {
		A1CircleData[k].Dis = 0.0f;
		A1CircleData[k].Dv = 0.0f;
		A1CircleData[k].P.x = 0.0f;
		A1CircleData[k].P.y = 0.0f;

		A2CircleData[k].Dis = 0.0f;
		A2CircleData[k].Dv = 0.0f;
		A2CircleData[k].P.x = 0.0f;
		A2CircleData[k].P.y = 0.0f;

		C1CircleData[k].Dis = 0.0f;
		C1CircleData[k].Dv = 0.0f;
		C1CircleData[k].P.x = 0.0f;
		C1CircleData[k].P.y = 0.0f;

		C2CircleData[k].Dis = 0.0f;
		C2CircleData[k].Dv = 0.0f;
		C2CircleData[k].P.x = 0.0f;
		C2CircleData[k].P.y = 0.0f;

		TopCircleData[k].Dis = 0.0f;
		TopCircleData[k].Dv  = 0.0f;
		TopCircleData[k].P.x = 0.0f;
		TopCircleData[k].P.y = 0.0f;

		BtmCircleData[k].Dis = 0.0f;
		BtmCircleData[k].Dv = 0.0f;
		BtmCircleData[k].P.x = 0.0f;
		BtmCircleData[k].P.y = 0.0f;
	}

	for (int k = 0; k < MAX_ALL_EDGE_COUNT; k++) {
		AllLeftInData  [k].x = 0.0f;
		AllLeftInData  [k].y = 0.0f;
		AllLeftOutData [k].x = 0.0f;
		AllLeftOutData [k].y = 0.0f;
		AllTopInData   [k].x = 0.0f;
		AllTopInData   [k].y = 0.0f;
		AllTopOutData  [k].x = 0.0f;
		AllTopOutData  [k].y = 0.0f;
		AllRightInData [k].x = 0.0f;
		AllRightInData [k].y = 0.0f;
		AllRightOutData[k].x = 0.0f;
		AllRightOutData[k].y = 0.0f;
		AllBtmInData   [k].x = 0.0f;
		AllBtmInData   [k].y = 0.0f;
		AllBtmOutData  [k].x = 0.0f;
		AllBtmOutData  [k].y = 0.0f;
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

	// �ڸ��ҷ� ���� Point Count
	A1CirInCrackCount = 0;
	A2CirInCrackCount = 0;

	C1CirInCrackCount = 0;
	C2CirInCrackCount = 0;

	// Bridht Defect 
	ACrackCount = 0;
	BCrackCount = 0;
	CCrackCount = 0;
	DCrackCount = 0;

	//�ڸ��ҷ� ������ Edge Point Count 
	LeftInCrackCount = 0;
	RIghtInCrackCount = 0;
	TopInCrackCount = 0;
	BtmInCrackCount = 0;

	// ��ü ������ �ʿ��� ��ü Count���� 
	AllLeftInEdgeCount = 0;
	AllLeftOutEdgeCount = 0;
	AllRightInEdgeCount = 0;
	AllRightOutEdgeCount = 0;
	AllTopInEdgeCount = 0;
	AllTopOutEdgeCount = 0;
	AllBtmInEdgeCount = 0;
	AllBtmOutEdgeCount = 0;

	// ��ü ������ �ʿ��� ���� Count���� 
	LeftInEdgeCount = 0;
	LeftOutEdgeCount = 0;
	RightInEdgeCount = 0;
	RightOutEdgeCount = 0;
	TopInEdgeCount = 0;
	TopOutEdgeCount = 0;
	BtmInEdgeCount = 0;
	BtmOutEdgeCount = 0;

	// NarrowTrench �������� ���������� 
	GrindLineCount = 0;

	ACircleFit1Center.x = 0.0f;
	ACircleFit1Center.y = 0.0f;
	ACircleFit2Center.x = 0.0f;
	ACircleFit2Center.y = 0.0f;

	CircleFit1Center.x = 0.0f;
	CircleFit1Center.y = 0.0f;
	CircleFit2Center.x = 0.0f;
	CircleFit2Center.y = 0.0f;

	ACirFitDiameter1 = 0.0f;
	ACirFitDiameter2 = 0.0f;

	CirFitDiameter1 = 0.0f;
	CirFitDiameter2 = 0.0f;

	// NOTCH CELL Btm EdgePoint Data Init 
	for (int k = 0; k < MAX_BTM_LINE_POINT; k++) {
		BtmEdgePoint[k].x = 0.0f;
		BtmEdgePoint[k].y = 0.0f;
	}

	// NOTCH CELL Top EdgePoint Data Init 
	for (int k = 0; k < MAX_TOP_LINE_POINT; k++) {
		TopEdgePoint[k].x = 0.0f;
		TopEdgePoint[k].y = 0.0f;
	}

	// NOTCH Corner Data Init 
	C1CorGrindWd = 0.0f;
	C1CorGrindMea = 0.0f;
	C2CorGrindWd = 0.0f;
	C2CorGrindMea = 0.0f;
	C3CorGrindWd = 0.0f;
	C3CorGrindMea = 0.0f;
	C4CorGrindWd = 0.0f;
	C4CorGrindMea = 0.0f;

	// NOTCH 
	// Btm Line Search �ð� ���� ���� 
	FEndLine = 0;

	// Notch Cell Mark Degree 
	MarkDegree = 0.0f;

	// ���� ���� (Circle)
	SideCirDeg = 0;
	CenterCirDeg = 0;

	// ROI Center Setting Length 
	nASideRoiCenterLength = 0.0f;
	nCSideRoiCenterLength = 0.0f;
	// 	Find->OnLoad(true,FindDir);

	// Notch Cell Size Data Init 
	LeftCellSizeHeight  = 0.0f;
	CenCellSizeHeight   = 0.0f;
	CenCellSizeWidth    = 0.0f;
	NotchCellSizeWidth  = 0.0f;
	NotchCellSizeHeight = 0.0f;

	// MarkToTopSide Height
	LeftMarkToTopHeight  = 0.0f;
	CenMarkToTopHeight   = 0.0f;
	RightMarkToTopHeight = 0.0f;

	// Center Cell Size  ������ �ʿ��� ������ 
	TotalCellSizeWidth = 0.0f;
	CenterCellSize     = 0.0f;
	TotalCellSizeCount = 0;

	// Cell Size ���� ����
	LeftStaPos.x = 0.0f;
	LeftStaPos.y = 0.0f;
	LeftEndPos.x = 0.0f;
	LeftEndPos.y = 0.0f;
	CenterStaPos.x = 0.0f;
	CenterStaPos.y = 0.0f;
	CenterEndPos.x = 0.0f;
	CenterEndPos.y = 0.0f;

	// Align ���ϰ� Ʋ������� �߻��ϴ� Defect����ų 
	int LeftDegMargin  = 0;
	int RightDegMargin = 0;

	AlignMark1.x = 0.0f;
	AlignMark1.y = 0.0f;
	AlignMark2.x = 0.0f;
	AlignMark2.y = 0.0f;

	//Mark Data Init
	FP0.x = 0.0f;
	FP0.y = 0.0f;
	FP1.x = 0.0f;
	FP1.y = 0.0f;

	//4�� ������ Rect ROI Data Init 
	LTRectWidth  = 0;
	LTRectHeight = 0;
	RTRectWidth  = 0;
	RTRectHeight = 0;
	LBRectWidth  = 0;
	LBRectHeight = 0;
	RBRectWidth  = 0;
	RBRectHeight = 0;

	//4�� ������ Corner ROI Data Init 
	LTCornerWidth  = 0;
	LTCornerHeight = 0;
	RTCornerWidth  = 0;
	RTCornerHeight = 0;
	LBCornerWidth  = 0;
	LBCornerHeight = 0;
	RBCornerWidth  = 0;
	RBCornerHeight = 0;

	//4�� ������ Circle ROI 
	LTCircleWidth = 0;
	LTCircleHeight = 0;
	RTCircleWidth = 0;
	RTCircleHeight = 0;
	LBCircleWidth = 0;
	LBCircleHeight = 0;
	RBCircleWidth = 0;
	RBCircleHeight = 0;

	// �˻��Ҷ� ���� ���ڿ� ���� Shape���´� �ʱ�ȭ �Ǿ�� �Ѵ� 
	// Corner �����˻� ���� Ȯ�� 
	IsCornerTop = false;
	IsCornerBtm = false;

	// Rect �˻� ���� Ȯ�� 
	IsRectTop = false;
	IsRectBtm = false;

	// Cell Circle Grind �˻� ���� ����
	IsCircleTop = false;
	IsCircleBtm = false;

	// Masking Image Test
	// 20190720
	AllMaskCount1      = 0;
	AllMaskCount2      = 0;
	AllLeftMaskCount1  = 0;
	AllLeftMaskCount2  = 0;
	AllRightMaskCount1 = 0;
	AllRightMaskCount2 = 0;

	//GrindMark ROI ������ �ʿ��� ���� Deg 
	dASideDeg =0.0f; //���� ���� - Glass����
	dBSideDeg =0.0f; //Glass���� - ���� ���� 
	dCSideDeg =0.0f; //���� ���� - Glass����
	dDSideDeg =0.0f; //Glass���� - ���� ���� 

	//Corner Data 
	CornerP1.x = 0.0f;
	CornerP1.y = 0.0f;
	CornerP2.x = 0.0f;
	CornerP2.y = 0.0f;

	LeftMaskDarkDefectTh  = 0;
	TopMaskDarkDefectTh   = 0;
	RightMaskDarkDefectTh = 0;
	BtmMaskDarkDefectTh   = 0;

	LeftMaskBrightDefectTh  =0;
	TopMaskBrightDefectTh   =0;
	RightMaskBrightDefectTh =0;
	BtmMaskBrightDefectTh   =0;

	// Corner �����˻� ���� Ȯ�� 
	IsCornerTop = false;
	IsCornerBtm = false;

	// Rect �˻� ���� Ȯ�� 
	IsRectTop = false;
	IsRectBtm = false;

	// Cell Circle Grind �˻� ���� ����
	IsCircleTop = false;
	IsCircleBtm = false;

	//Mark Pos Init 
	FP0.x = 0.0f;
	FP0.y = 0.0f;
	FP1.x = 0.0f;
	FP1.y = 0.0f;

	//���� ���ʶ��� Pos Count Init
	LeftGrindInCnt  =0;
	TopGrindInCnt   =0;
	RightGrindInCnt =0;
	BtmGrindInCnt   =0;

	//���� ���ʶ��� Pos Data Init
	LeftGrindInPos  =0;
	TopGrindInPos   =0;
	RightGrindInPos =0;
	BtmGrindInPos   =0;

	//���� �������� Defect �˻���� Range Init
	LeftDefectRange =0;
	TopDefectRange  =0;
	RightDefectRange=0;
	BtmDefectRange  =0;
	//----------------------------------------------------------------
}
int CFM15xLineDefect::OnAutoBevelIn()// Line���� ���� �˻� �Լ� 
{
	EImageEncoder CodeImageEncoder;
	ECodedImage2  CodeImage;
	EObjectSelection CodeImageSelection;
	EROIBW8   DefectROI;

	int ROI_X = 0;
	int ROI_Y = 0;
	int ROI_W = 0;
	int ROI_H = 0;

	// Circle Dis, Dv TEST 
	double MaxDis = 0.0f;
	double MinDis = 10000.0f;
	double MaxDv = 0.0f;
	double MinDv = 10000.0f;
	double StdMaxDis = 0.0f;
	double StdMinDis = 10000.0f;
	int    MinCirCount = 0;

	if (!GImage->GetImagePtr()) return false;
	if (!RotateImg.GetImagePtr()) return false;
	if (!DefectImg.GetImagePtr()) return false;

	int Index = 0;
	// NOTCH Defect �˻� ������ 
	if (FShape == shRectH ) { // NOTCH Edge Inspection(All Inspection     )  ImageCopy -> ImageConversion -> ROI Attatch 
		if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, TopDarkThresHold);
		else                   EasyImage::Threshold(&RotateImg, &DefectImg, TopDarkThresHold); //DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_HAP.bmp");
		Index = 0;
	}
	if (FShape == shRectS ) {
		//        EasyImage::Threshold(GImage    ,&DefectImg, EnCapMidThresHold);
		Index = 1;                                                                              // NOTCH Edge Inspection(Only Top Inspection)  Short Cell Cam0
	}
	if (FShape == shRectC ) { // NOTCH Edge Inspection(All Inspection     )  ImageCopy -> ImageConversion -> ROI Attatch 
		if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, BtmDarkThresHold);
		else                   EasyImage::Threshold(&RotateImg, &DefectImg, BtmDarkThresHold); //DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_HAP.bmp");
		Index = 2;
	}

	// Pixel ����Data ���� Buffer�Ѱ��� 
	//IAddress = DefectImg.GetImagePtr();
	int PixelValue;
	int PixelCount;

	// ���� Btm OverKill���� ���� 
	int BTMLine = 0;
	int TotalPixel = 0;

	// Frit OverKill Remove Value
	int    MaxPixel    = 0;
	int    MaxData     = 0;
	int    CurData     = 0;
	int    BrokenPixel = 0;
	int    CrackPixel  = 0;
	const int LongBrokenMargin = 7500;
	const int LongCrackMargin = 10;
	// 	const int ShortBrokenMargin  =7500;
	const int ShortCrackMargin = 25;
	const int MaskingMargin = 20;

	// Burr & Remain
	int BurrWidth = 100;

	// Left Line Dv 
	double LeftDv = 0.0f;

	bool l = false; bool lt = false;
	bool t = false; bool rt = false;
	bool r = false; bool rb = false;
	bool b = false; bool lb = false;

	// LeftLimit Remove Blob
	bool IsRemoveBlob = true;

	//int ImgW   = GImage->GetWidth();
	//int ImgH   = GImage->GetHeight();
	FrameImgW = GImage->GetWidth();
	FrameImgH = GImage->GetHeight();
	//ImageWidth = GImage->GetWidth();;

	int Count1 = 0, Count2 = 0, Count3 = 0, Count4 = 0, Count5 = 0;

	//Euresys Blob Parameter Setting 
	//////////////////////////////////////////////////
	//CNC_6HEAD �̹��� ��ȯ �� �⺻���� ���� 
	if ((FShape == shRectH || FShape == shRectS || FShape == shRectC)) {
		DefectROI.Attach(&DefectImg);
		CodeImageSelection.SetFeretAngle(0.00f);
		CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
		CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true);

		IAddress = DefectImg.GetImagePtr();
	}
	//////////////////////////////////////////////////

	//Frame ���� �˻翵�� ���� 
	//////////////////////////////////////////////////
	//Defect Roi Rect Init 
	for (int k = 0; k < MAX_DEFECT_ROI_COUNT; k++) {
		DefectRoiRect[k].left   = 0;
		DefectRoiRect[k].top    = 0;
		DefectRoiRect[k].right  = 0;
		DefectRoiRect[k].bottom = 0;
	}
	//////////////////////////////////////////////////

	TFPoint LT(0, 0);
	TFPoint RT((float)FrameImgW, 0);
	TFPoint RB((float)FrameImgW, (float)FrameImgH);
	TFPoint LB(0, (float)FrameImgH);
	TFPoint SP, EP, LSP, LEP, RSP, REP;

	// Notch Cell Size ���� 
	TFPoint NotchCenterPos(0.0f, 0.0f);

	// ROI ���� ����
	int RoiStaX   = 0;
	int RoiStaY   = 0;
	int RoiWidth  = 0;
	int RoiHeight = 0;

	// ROI ���� ����
	int LeftHeight ;
	int RightHeight;
	int TopWidth   ;
	int BtmWidth   ;
	int BlobCount  ;
	const int Margin = 15;

	// ImageRotate �ҷ� �߻��� OverKill ���� ����
	const int TopMargin    = 10;
	const int BtmMargin    = 30;
	const int DefectMargin = 5 ;

	// �ſ��� ���簢�� �����ϰ�� ������ ���ؼ� �ҷ��� ���ϰ� �پ
	// �˻�� ���� �Ұ���...�̷����ؼ� Offset�߰��� 
	const int BtmInspOffset = 6;

	// Corner Data 
	int CornerCount = 0;
	int MarktoBmLine = 0;

	//Circle Count,Avg Round 
	int CircleCount = 0;
	float AvgCirDis = 0.0f;
	float TotalCirDis = 0.0f;

	// Merge Margin
	const int xMerge = 30;
	const int yMerge = 30;
	const int MgCount = 3;

	//Remain Offset 
	const int RemainMargin = 30;

	// BM Line Check
	float RightBMGab = 0;
	float LeftBMGab = 0;

	// Side Offset Margin
	int SideMargin = 5;

	// Margin Filter
	const int BroMarginWidth = 200;
	const int BroMarginHeight = 200;

	// GrindMark ���� �����Ͽ� ȸ������(�˻�ȭ�� ���÷���)
	TFPoint LtPos, RbPos;

	// �˻� ROI ���� 
	// Notch Cell ���� �˻� 
	if (FShape == shRectH) {
		SP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);
		EP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LB, RB);

		FLeftWidth = 200;
		FRightWidth = 200;
		FTopHeight = 200;

		if (IsTopAllGrind) { //��ü���� 
			if (TopEdgePoint[0].x == 0 || TopEdgePoint[0].y == 0) {
				if     (IsRectTop  ){TopEdgePoint[0].x =0; TopEdgePoint[0].y=0;                                                     }
				else if(IsCornerTop){TopEdgePoint[0].x = (int)FLTPoint.x; TopEdgePoint[0].y = (int)FLTPoint.y + LTCornerHeight + 10;}
				else if(IsCircleTop){TopEdgePoint[0].x = (int)FLTPoint.x; TopEdgePoint[0].y = (int)FLTPoint.y + LTCircleHeight + 10;}
			}
			if (TopEdgePoint[3].x == 0 || TopEdgePoint[3].y == 0) {
				if     (IsRectTop  ){TopEdgePoint[3].x =0; TopEdgePoint[0].y=0;                                                    }
				else if(IsCornerTop){TopEdgePoint[3].x =(int)FRTPoint.x; TopEdgePoint[3].y = (int)FRTPoint.y + RTCornerHeight + 10;}
				else if(IsCircleTop){TopEdgePoint[3].x =(int)FRTPoint.x; TopEdgePoint[3].y = (int)FRTPoint.y + RTCircleHeight + 10;}
			}
			TopEdgePoint[1].x = 0, TopEdgePoint[1].y = 0;
			TopEdgePoint[2].x = 0, TopEdgePoint[2].y = 0;
		}
		else { //�κп��� 
			if (IsCornerTop) { //Corner Cutting�ϰ�� Defect�˻� ���� ���� 
				if (TopEdgePoint[0].x == 0 || TopEdgePoint[0].y == 0) {
					TopEdgePoint[0].x = (int)FLTPoint.x;
					TopEdgePoint[0].y = (int)FLTPoint.y + LTCornerHeight + 10;
				}
				if (TopEdgePoint[1].x == 0 || TopEdgePoint[1].y == 0) {
					TopEdgePoint[1].x = (int)FLTPoint.x + LTCornerWidth + 10;
					TopEdgePoint[1].y = (int)FLTPoint.y;
				}
				if (TopEdgePoint[2].x == 0 || TopEdgePoint[2].y == 0) {
					TopEdgePoint[2].x = (int)FRTPoint.x - RTCornerWidth - 10;
					TopEdgePoint[2].y = (int)FRTPoint.y;
				}
				if (TopEdgePoint[3].x == 0 || TopEdgePoint[3].y == 0) {
					TopEdgePoint[3].x = (int)FRTPoint.x;
					TopEdgePoint[3].y = (int)FRTPoint.y + RTCornerHeight + 10;
				}
			}
			else if (IsRectTop) {//Corner Cutting ������� Defect�˻� ���� ���� 
				if (TopEdgePoint[0].x == 0 || TopEdgePoint[0].y == 0) {
					TopEdgePoint[0].x = (int)FLTPoint.x;
					TopEdgePoint[0].y = (int)FLTPoint.y;
				}
				if (TopEdgePoint[1].x == 0 || TopEdgePoint[1].y == 0) {
					TopEdgePoint[1].x = (int)FLTPoint.x;
					TopEdgePoint[1].y = (int)FLTPoint.y;
				}
				if (TopEdgePoint[2].x == 0 || TopEdgePoint[2].y == 0) {
					TopEdgePoint[2].x = (int)FRTPoint.x;
					TopEdgePoint[2].y = (int)FRTPoint.y;
				}
				if (TopEdgePoint[3].x == 0 || TopEdgePoint[3].y == 0) {
					TopEdgePoint[3].x = (int)FRTPoint.x;
					TopEdgePoint[3].y = (int)FRTPoint.y;
				}
			}
			else if (IsCircleTop) { //Circle Cutting�ϰ�� Defect �˻� ���� ���� 
				if (TopEdgePoint[0].x == 0 || TopEdgePoint[0].y == 0) {
					TopEdgePoint[0].x = (int)FLTPoint.x;
					TopEdgePoint[0].y = (int)FLTPoint.y + LTCircleHeight + 10;
				}
				if (TopEdgePoint[1].x == 0 || TopEdgePoint[1].y == 0) {
					TopEdgePoint[1].x = (int)FLTPoint.x + LTCircleWidth + 10;
					TopEdgePoint[1].y = (int)FLTPoint.y;
				}
				if (TopEdgePoint[2].x == 0 || TopEdgePoint[2].y == 0) {
					TopEdgePoint[2].x = (int)FRTPoint.x - RTCircleWidth - 10;
					TopEdgePoint[2].y = (int)FRTPoint.y;
				}
				if (TopEdgePoint[3].x == 0 || TopEdgePoint[3].y == 0) {
					TopEdgePoint[3].x = (int)FRTPoint.x;
					TopEdgePoint[3].y = (int)FRTPoint.y + RTCircleHeight + 10;
				}

				if (CamId==Cam_Top) {
					LeftHeight  = (int)(SP.y - TopEdgePoint[0].y);
					RightHeight = (int)(EP.y - TopEdgePoint[3].y);
				}
				else {
					TopEdgePoint[0].y = (int)FLTPoint.y + LTCircleHeight + 10;
					TopEdgePoint[3].y = (int)FRTPoint.y + RTCircleHeight + 10;
					LeftHeight  = (int)(SP.y - TopEdgePoint[0].y);
					RightHeight = (int)(EP.y - TopEdgePoint[3].y);
				}
			}
		}


		// Defect Data Init 
		DefectPointCount = 0; // ��ü �ҷ� ���� �ʱ�ȭ
		BCount = 0; // B_SIDE  �ҷ� ����Ʈ ����
		ACount = 0; // A_SIDE  �ҷ� ����Ʈ ����
		CCount = 0; // C_SIDE  �ҷ� ����Ʈ ����
		DCount = 0; // D_SIDE  �ҷ� ����Ʈ ����
		C1Count = 0;
		C2Count = 0;
		C3Count = 0;
		C4Count = 0;
		A1CirDeftectCnt = 0;
		A2CirDeftectCnt = 0;
		C1CirDeftectCnt = 0;
		C2CirDeftectCnt = 0;
		TopDefectCount = 0;
		RIghtDefectCount = 0;
		BtmDefectCount = 0;
		LeftDefectCount = 0;
		ACrackCount = 0;
		BCrackCount = 0;
		CCrackCount = 0;
		DCrackCount = 0;
		A1CirCrackCnt = 0;
		A2CirCrackCnt = 0;
		C1CirCrackCnt = 0;
		C2CirCrackCnt = 0;
		TopCrackCount = 0;
		RIghtCrackCount = 0;
		BtmCrackCount = 0;
		LeftCrackCount = 0;

		// BSide, DSide Defect �˻�� Margin�� ���� 
		if (AlignMark1.y != 0.0f && AlignMark2.y != 0.0f) {
			if (AlignMark1.y > AlignMark2.y) {
				LeftDegMargin = (int)((AlignMark1.y - AlignMark2.y) + 5);
				RightDegMargin = 5;
			}
			else if (AlignMark1.y < AlignMark2.y) {
				LeftDegMargin = 5;
				RightDegMargin = (int)((AlignMark2.y - AlignMark1.y) + 5);
			}
			else if (AlignMark1.y == AlignMark2.y) {
				LeftDegMargin = 5;
				RightDegMargin = 5;
			}
		}
	}
	// Notch Cell ��ƽ �˻� 
	if (FShape == shRectS) {
		if (!IsLeftAllGrind) {
			LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
			LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);
			// ����ų ������������ ������ ���� ���� ���� �˻� 
			if ((fabs(LSP.x - LEP.x) > Margin)) { LSP.x = (LSP.x > LEP.x) ? LSP.x : LEP.x; }
		}
		if (!IsRightAllGrind) {
			RSP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LT, RT);
			REP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LB, RB);
			// ����ų ������������ ������ ���� ���� ���� �˻� 
			if ((fabs(RSP.x - REP.x) > Margin)) { RSP.x = (RSP.x > REP.x) ? REP.x : RSP.x; }
		}
		// Blob
		FLeftWidth = 200;
		FRightWidth = 200;
	}
	// Notch Cell �� �˻� 
	if (FShape == shRectC) {
		SP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
		EP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LT, RT);

		// BtmWidth �������� Circle��� �� �Ŀ� ���� ������
		FLeftWidth = 200;
		FRightWidth = 200;
		FBtmHeight = 50;

		if (IsBtmAllGrind) { //��ü����
			if (BtmEdgePoint[0].x == 0 || BtmEdgePoint[0].y == 0) {
				if     (IsRectBtm  ){BtmEdgePoint[0].x =0; BtmEdgePoint[0].y=0;                                                     }
				else if(IsCornerBtm){BtmEdgePoint[0].x = (int)FLBPoint.x; BtmEdgePoint[0].y = (int)FLBPoint.y - LBCornerHeight - 10;}
				else if(IsCircleBtm){BtmEdgePoint[0].x = (int)FLBPoint.x; BtmEdgePoint[0].y = (int)FLBPoint.y - LBCircleHeight - 10;}
			}
			if (BtmEdgePoint[3].x == 0 || BtmEdgePoint[3].y == 0) {
				if     (IsRectBtm  ){BtmEdgePoint[3].x =0; BtmEdgePoint[3].y=0;                                                     }
				else if(IsCornerBtm){BtmEdgePoint[3].x = (int)FLBPoint.x; BtmEdgePoint[3].y = (int)FRBPoint.y - RBCornerHeight - 10;}
				else if(IsCircleBtm){BtmEdgePoint[3].x = (int)FLBPoint.x; BtmEdgePoint[3].y = (int)FRBPoint.y - RBCircleHeight - 10;}
			}
			BtmEdgePoint[1].x = 0, BtmEdgePoint[1].y = 0;
			BtmEdgePoint[2].x = 0, BtmEdgePoint[2].y = 0;
		}
		else { //�κп��� 
			if (IsCornerTop) { //Corner Cutting�ϰ�� Defect�˻� ���� ���� 
				if (BtmEdgePoint[0].x == 0 || BtmEdgePoint[0].y == 0) {
					BtmEdgePoint[0].x = (int)FLBPoint.x;
					BtmEdgePoint[0].y = (int)FLBPoint.y - LBCornerHeight - 10;
				}
				if (BtmEdgePoint[1].x == 0 || BtmEdgePoint[1].y == 0) {
					BtmEdgePoint[1].x = (int)FLBPoint.x + LBCornerWidth + 10;
					BtmEdgePoint[1].y = (int)FLBPoint.y;
				}
				if (BtmEdgePoint[2].x == 0 || BtmEdgePoint[2].y == 0) {
					BtmEdgePoint[2].x = (int)FRBPoint.x - RBCornerWidth - 10;
					BtmEdgePoint[2].y = (int)FRBPoint.y;
				}
				if (BtmEdgePoint[3].x == 0 || BtmEdgePoint[3].y == 0) {
					BtmEdgePoint[3].x = (int)FRBPoint.x;
					BtmEdgePoint[3].y = (int)FRBPoint.y - RBCornerHeight - 10;
				}
			}
			else if (IsRectTop) { //Corner Cutting ������� Defect�˻� ���� ���� 
				if (BtmEdgePoint[0].x == 0 || BtmEdgePoint[0].y == 0) {
					BtmEdgePoint[0].x = (int)FLBPoint.x;
					BtmEdgePoint[0].y = (int)FLBPoint.y;
				}
				if (BtmEdgePoint[1].x == 0 || BtmEdgePoint[1].y == 0) {
					BtmEdgePoint[1].x = (int)FLBPoint.x;
					BtmEdgePoint[1].y = (int)FLBPoint.y;
				}
				if (BtmEdgePoint[2].x == 0 || BtmEdgePoint[2].y == 0) {
					BtmEdgePoint[2].x = (int)FRBPoint.x;
					BtmEdgePoint[2].y = (int)FRBPoint.y;
				}
				if (BtmEdgePoint[3].x == 0 || BtmEdgePoint[3].y == 0) {
					BtmEdgePoint[3].x = (int)FRBPoint.x;
					BtmEdgePoint[3].y = (int)FRBPoint.y;
				}
			}
			else if (IsCircleTop) { //Circle Cutting�ϰ�� Defect �˻� ���� ���� 
				if (BtmEdgePoint[0].x == 0 || BtmEdgePoint[0].y == 0) {
					BtmEdgePoint[0].x = (int)FLBPoint.x;
					BtmEdgePoint[0].y = (int)FLBPoint.y - LBCircleHeight - 10;
				}
				if (BtmEdgePoint[1].x == 0 || BtmEdgePoint[1].y == 0) {
					BtmEdgePoint[1].x = (int)FLBPoint.x + LBCircleWidth + 10;
					BtmEdgePoint[1].y = (int)FLBPoint.y;
				}
				if (BtmEdgePoint[2].x == 0 || BtmEdgePoint[2].y == 0) {
					BtmEdgePoint[2].x = (int)FRBPoint.x - RBCircleWidth - 10;
					BtmEdgePoint[2].y = (int)FRBPoint.y;
				}
				if (BtmEdgePoint[3].x == 0 || BtmEdgePoint[3].y == 0) {
					BtmEdgePoint[3].x = (int)FRBPoint.x;
					BtmEdgePoint[3].y = (int)FRBPoint.y - RBCircleHeight - 10;
				}
			}
		}

		// BSide, DSide Defect �˻�� Margin�� ���� 
		if (AlignMark1.y != 0.0f && AlignMark2.y != 0.0f) {
			if (AlignMark1.y > AlignMark2.y) {
				LeftDegMargin = 5;
				RightDegMargin = (int)((AlignMark1.y - AlignMark2.y) + 5);
			}
			else if (AlignMark1.y < AlignMark2.y) {
				LeftDegMargin = (int)((AlignMark2.y - AlignMark1.y) + 5);
				RightDegMargin = 5;
			}
			else if (AlignMark1.y == AlignMark2.y) {
				LeftDegMargin = 5;
				RightDegMargin = 5;
			}
		}
	}

	float Dis = 0.0f;
	float StdDis = 0.0f;

	// Side Offset TEST 
	double  Width = 0.0f;
	long    Left = 0;
	float   CenterX = 0.0F;

	// ���� Frmae���� DefectCount 
	int FrameLeftDefectCnt = 0;
	int FrameLeftCrackCnt = 0;
	int FrameRightDefectCnt = 0;
	int FrameRightCrackCnt = 0;

	// Clipper Test 
	int PolyOutCnt = 0;

	// ���� ����� ���ؼ� �ʿ��� ������ ������ 
// 	POINT *MaskPoint;
// 	POINT *MaskPoint2;
	int TotalInCount = 0, TotalInCount2 = 0;
	int MaskStaPos = 0;
	int MaskEndPos = 0;
	int MinCnt = 0;
	int MaxCnt = 0;

	TPoint TargetPoint(0, 0);
	TPoint StaPos(0, 0);
	TPoint EndPos(0, 0);
	double deg = 0.0f;

	TFPoint DefectLT(0.0f, 0.0f);
	TFPoint DefectRT(0.0f, 0.0f);
	TFPoint DefectLB(0.0f, 0.0f);
	TFPoint DefectRB(0.0f, 0.0f);
	const int MaskDefectMagin = 5; // MaskImage Defect��ǥ���� �������ʶ��� ���� �ּҰŸ� �����ϰ�� Defect���� 
	bool IsDefect = false;
	double LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
	int PreTopCompareCnt = 0;
	int PreBtmCompareCnt = 0;

	TPoint RotatePoint;

	//All Grind Defect Value 
	int LeftOffset  = 0;
	int TopOffset   = 0;
	int RightOffset = 0;
	int BtmOffset   = 0;

	switch (Index) {

		// shRectH NOTCH 
		//----------------------------------------------------------------------
	case 0:
		// Corner Defect �˻� 
		if (IsTopAllGrind) { //��ü���� 
			if (AllTopInEdgeCount != 0) { //��ü������ TopInEdgeCount     

				if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, PadSideThresHold);
				else                   EasyImage::Threshold(&RotateImg, &DefectImg, PadSideThresHold); //DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_HAP.bmp");
				IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 

				TopDefectCount = GetPolygonSize(AllTopInData, TopDefectPoint, AllTopInEdgeCount, 50, 2, PadCenThresHold); // Dark Defect 
				// �ڸ� �ҷ� �˻� (MOLED �ʿ�����Ͱ���..�ϴ� �ּ�ó��)
//				if (!IsMarkBtm && TopInCrackCount > 0 && BrokenThresHold != 0) {
//					IAddress = BrightDefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
//					CircleFit1Center.x = 0.0f;
//					CircleFit1Center.y = 0.0f;
//					TopCrackCount = GetCircleCrackSize(TopCircleData, TopCrackPoint, TopInCrackCount, 30, 0.0f, CircleFit1Center); //Bright Defect 
//				}

				// �⺻ Dark_Defect������� MarkImage�������� �ѹ��� DarkDefect�˻��Ѵ�
				// White Masking =>DarkDefect 
				if (TopDefectCount == 0 && AllTopInEdgeCount != 0 && TopMaskDarkDefectTh != 0) {
					// MaskImage Create Start 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					BinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Conversion Binary
					MaskDarkDefectImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Org
					EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &MaskDarkDefectImg);
					//////////////////////////////////////////////////////////////////////////////////////////////////

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
					HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					// Masking Data Init 
					if (MaskPoint != NULL) {
						delete[]MaskPoint;
					}
					if (MaskPoint2 != NULL) {
						delete[]MaskPoint2;
					}

					// ROI���� ���� �������� �ʿ��� Data 
					MaskStaPos = 0; MaskEndPos = 0;
					MinCnt = 0; MaxCnt = 0;

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve(AllTopInEdgeCount + 2);
					Poly_Out.reserve(2000);

					Poly_In.push_back(The::IntPoint(FLTPoint.x, FLTPoint.y));
					for (int k = 0; k < AllTopInEdgeCount; k++) {
						// Clipper use 
						Poly_In.push_back(The::IntPoint(AllTopInData[k].x, AllTopInData[k].y));
					}
					Poly_In.push_back(The::IntPoint(FRTPoint.x, FRTPoint.y));

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, TopMaskOffset);
					PolyOutCnt = Poly_Out[0].size();

					AllMaskCount1 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
					MaskPoint = new POINT[AllMaskCount1];

					// �������� ������ �ʱ�ȭ 
					for (int k = 0; k < AllMaskCount1; k++) {
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					for (int k = 0; k < PolyOutCnt; k++) {
						if (k >= Poly_Out[0].size()) break;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, AllMaskCount1, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);

					// Data Convert 
					MinDv = 100000.0f;
					MaxDv = 0.0f;

					Poly_In.clear();
					Poly_Out.clear();
					ClipOffset.Clear();
					Poly_In.reserve(AllTopInEdgeCount + 2);
					Poly_Out.reserve(2000);

					Poly_In.push_back(The::IntPoint(FLTPoint.x, (AllTopInData[0].y + TopMaskDis)));
					// 					Poly_In.push_back(The::IntPoint(FLTPoint.x,(MaxDv+TopMaskDis)));
					for (int k = 0; k < AllTopInEdgeCount; k++) {
						deg = 0.0f;
						// Clipper use 
						Poly_In.push_back(The::IntPoint(AllTopInData[k].x, AllTopInData[k].y));
					}
					Poly_In.push_back(The::IntPoint(FRTPoint.x, (AllTopInData[AllTopInEdgeCount - 1].y + TopMaskDis)));
					// 					Poly_In.push_back(The::IntPoint(FRTPoint.x,(MaxDv+TopMaskDis)));

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, -TopMaskDis);
					PolyOutCnt = Poly_Out[0].size();

					AllMaskCount2 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
					MaskPoint2 = new POINT[AllMaskCount2];

					// �������� ������ �ʱ�ȭ 
					for (int k = 0; k < AllMaskCount2; k++) {
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					for (int k = 0; k < PolyOutCnt; k++) {
						if (k >= Poly_Out[0].size()) break;
						MaskPoint2[k].x = Poly_Out[0][k].x;
						MaskPoint2[k].y = Poly_Out[0][k].y;
						if (MaskPoint2[k].x < MinDv) { MinDv = MaskPoint2[k].x; MinCnt = k; }
						if (MaskPoint2[k].x > MaxDv) { MaxDv = MaskPoint2[k].x; MaxCnt = k; }
					}

					// ���� �� �������� ���� ���� ���� ã�´� 
					MaskEndPos = (MaskPoint2[MinCnt].y > MaskPoint2[MaxCnt].y) ? MaskPoint2[MaxCnt].y : MaskPoint2[MinCnt].y;

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, AllMaskCount2, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);
					//////////////////////////////////////////////////////////////////////////////////////////////////

					if (MaskDarkDefectImg.GetImagePtr()) {
						// Image Conversion Binary_Image
//                         MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\Mask_Org_HAP.bmp");
						EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, TopMaskDarkDefectTh);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true);
						IAddress = MaskDarkDefectImg.GetImagePtr();

						// �˻� ROI���� 
						RoiStaX = (int)(FLTPoint.x + SideMargin);
						RoiStaY = (int)(FLTPoint.y);
						RoiWidth = (int)(FRTPoint.x - FLTPoint.x) - 2 * SideMargin;
						// 						RoiHeight    =(int)(LTCircleHeight)                      ;
						RoiHeight = (int)(MaskEndPos - FLTPoint.y);

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if ((RoiStaX) <= 0         || (RoiStaY) <= 0           || RoiWidth <= 0         || (RoiHeight) <= 0          ) return false;
						if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || (RoiHeight) > TopLimitLine) return false;
						if ((RoiStaX + RoiWidth) >= FrameImgW        || (RoiStaY + RoiHeight) > TopLimitLine)                          return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI, CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

						if (BlobCount != 0) {
							for (int k = 0; k < BlobCount; k++) {
								if (TopDefectCount >= MAX_DEFECT_POINT) break;
								// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + RoiStaX);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight()) / 2) + RoiStaY);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								// 								for(int i=PreTopCompareCnt; i<AllTopInEdgeCount; i++){
								for (int i = 0; i < AllTopInEdgeCount; i++) {
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									IsDefect = false;
									LTDis = Math.GetLength(AllTopInData[i], DefectLT);
									RTDis = Math.GetLength(AllTopInData[i], DefectRT);
									LBDis = Math.GetLength(AllTopInData[i], DefectLB);
									RBDis = Math.GetLength(AllTopInData[i], DefectRB);
									// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
									if (LTDis <= (MaskDefectMagin + TopMaskOffset) || RTDis <= (MaskDefectMagin + TopMaskOffset) || LBDis <= (MaskDefectMagin + TopMaskOffset) || RBDis <= (MaskDefectMagin + TopMaskOffset)) {
										IsDefect = true;
										PreTopCompareCnt = i;
										break;
									}
								}

								if (IsDefect) {
									TopDefectPoint[TopDefectCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									TopDefectPoint[TopDefectCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

									TopDefectPoint[TopDefectCount].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - TopDefectPoint[TopDefectCount].Width / 2) + RoiStaX);
									TopDefectPoint[TopDefectCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - TopDefectPoint[TopDefectCount].Height / 2) + RoiStaY);
									TopDefectPoint[TopDefectCount].Rect.right = (long)(TopDefectPoint[TopDefectCount].Rect.left + TopDefectPoint[TopDefectCount].Width);
									TopDefectPoint[TopDefectCount].Rect.bottom = (long)(TopDefectPoint[TopDefectCount].Rect.top + TopDefectPoint[TopDefectCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// ���� Pixel Data ���� 
									for (int n = TopDefectPoint[TopDefectCount].Rect.top + 1; n < TopDefectPoint[TopDefectCount].Rect.bottom - 1; n++) {
										for (int m = TopDefectPoint[TopDefectCount].Rect.left + 1; m < TopDefectPoint[TopDefectCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue < TopMaskDarkDefectTh) PixelCount++;
										}
									}
									// Display ������ �ʿ��� 
									TopDefectPoint[TopDefectCount].Round = PixelCount;
									TopDefectPoint[TopDefectCount].P.x = (float)(TopDefectPoint[TopDefectCount].Rect.left);
									TopDefectPoint[TopDefectCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - TopDefectPoint[TopDefectCount].Height / 2) + RoiStaY + ImageHeight);
									TopDefectPoint[TopDefectCount].Width = TopDefectPoint[TopDefectCount].Width + SideMargin;

									// Merge ���ؼ� ������ �ʿ��� 
									TopDefectPoint[TopDefectCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - TopDefectPoint[TopDefectCount].Height / 2) + RoiStaY + ImageHeight);
									TopDefectPoint[TopDefectCount].Rect.bottom = (long)(TopDefectPoint[TopDefectCount].Rect.top + TopDefectPoint[TopDefectCount].Height);
									TopDefectCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}
			}

			//Rect���� ��ü ������� LeftLine, RightLine �߰� �˻� �ʿ��� 
			if(IsRectTop){
				////////////////////////////////////////////////////////////////
				if (IsLeftAllGrind) { //���� ��ü���� 
					EasyImage::Threshold(GImage, &DefectImg, DSideGrindThresHold);
					IAddress = DefectImg.GetImagePtr();

					// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					if (LeftInEdgeCount != 0) { //�������� DarkDefect 
						FrameLeftDefectCnt = 0;
						FrameLeftDefectCnt = GetPolygonSize(LeftInPoint, LeftDefectPoint, LeftInEdgeCount, 50, 5,DSideGrindThresHold, LeftDefectCount); // Dark Defect 
						LeftDefectCount += FrameLeftDefectCnt;
					}
				}
				////////////////////////////////////////////////////////////////
				if (IsRightAllGrind) { //������ ��ü���� 
					EasyImage::Threshold(GImage, &DefectImg, BSideGrindThresHold);
					IAddress = DefectImg.GetImagePtr();

					// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					if (RightInEdgeCount != 0) { //�������� DarkDefect 
						FrameRightDefectCnt = 0;
						FrameRightDefectCnt = GetPolygonSize(RightInPoint, RightDefectPoint, RightInEdgeCount, 50, 5, BSideGrindThresHold,RIghtDefectCount); // Dark Defect 
						RIghtDefectCount += FrameRightDefectCnt;
					}
				}
				////////////////////////////////////////////////////////////////

				//��ü���� �ܰ����� ���� �˻� ������ 
				//DSide Dark Defect 
				if(LeftDefectRange!=0 && LeftDarkThresHold!=0){
					// Image Conversion Binary_Image
					if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, LeftDarkThresHold);
					else                   EasyImage::Threshold(&RotateImg, &DefectImg, LeftDarkThresHold); 

//					RotateImg.Save("D:\\ROTATE_IMG\\AllGrind_HAT_Defect.bmp");

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&DefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = DefectImg.GetImagePtr();

					//�˻� ROI ���� ���� 
					if (LeftGrindInCnt >0 && LeftGrindInPos >0)  LeftOffset = (int)(LeftGrindInPos / LeftGrindInCnt);
					if (TopGrindInCnt  >0 && TopGrindInPos  >0)  TopOffset  = (int)(TopGrindInPos  / TopGrindInCnt );

					RoiStaX   = (int)((LeftOffset +DefectMargin       ));
					RoiStaY   = (int)((TopOffset +DefectMargin        ));
					RoiWidth  = (int)(LeftDefectRange                  );
					RoiHeight = (int)(FrameImgH - RoiStaY -DefectMargin);

					// �˻� ������ 
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

					//�˻� ȭ�� ���÷��� ���� 
					DefectRoiRect[Left_Roi].left   = RoiStaX            ;
					DefectRoiRect[Left_Roi].top    = RoiStaY            ;
					DefectRoiRect[Left_Roi].right  = RoiStaX + RoiWidth ;
					DefectRoiRect[Left_Roi].bottom = RoiStaY + RoiHeight;

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
									if(PixelValue < LeftDarkThresHold) PixelCount++;
								}
							}
							// Display ������ �ʿ��� 
							DDefectPoint[DCount].Round     = PixelCount;
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
				}


				//ASide Dark Defect 
				if(TopDefectRange!=0 && TopDarkThresHold!=0){
					// Image Conversion Binary_Image
					if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, TopDarkThresHold);
					else                   EasyImage::Threshold(&RotateImg, &DefectImg, TopDarkThresHold); 

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&DefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = DefectImg.GetImagePtr();

					//�˻� ROI ���� ���� 
					if (LeftGrindInCnt  >0 && LeftGrindInPos  >0)  LeftOffset  = (int)(LeftGrindInPos  / LeftGrindInCnt );
					if (TopGrindInCnt   >0 && TopGrindInPos   >0)  TopOffset   = (int)(TopGrindInPos   / TopGrindInCnt  );
					if (RightGrindInCnt >0 && RightGrindInPos >0)  RightOffset = (int)(RightGrindInPos / RightGrindInCnt);

					RoiStaX   = (int)(LeftOffset   + DefectMargin               );
					RoiStaY   = (int)(TopOffset    + DefectMargin               );
					RoiWidth  = (int)((RightOffset - LeftOffset) -2*DefectMargin);
					RoiHeight = (int)(TopDefectRange                            );

					// �˻� ������ 
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

					//�˻� ȭ�� ���÷��� ���� 
					DefectRoiRect[TopBtm_Roi].left   = RoiStaX            ;
					DefectRoiRect[TopBtm_Roi].top    = RoiStaY            ;
					DefectRoiRect[TopBtm_Roi].right  = RoiStaX + RoiWidth ;
					DefectRoiRect[TopBtm_Roi].bottom = RoiStaY + RoiHeight;

					// Blob �˻� ���� 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					// Blob ���͸� 
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight, FMaxHeight , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingIntegerFeature(EFeature_TopLimit         ,0         , 1          , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
					if(BlobCount!=0){
						for(int k=0; k<BlobCount; k++){
							if(DCount>=MAX_DEFECT_POINT) break;
							ADefectPoint[ACount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
							ADefectPoint[ACount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

							ADefectPoint[ACount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width/2 ) +RoiStaX);
							ADefectPoint[ACount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2) +RoiStaY);
							ADefectPoint[ACount].Rect.right  = (long)(ADefectPoint[ACount].Rect.left+ ADefectPoint[ACount].Width );
							ADefectPoint[ACount].Rect.bottom = (long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);

							PixelCount = 0;
							PixelValue = 0;
							// ���� Pixel Data ���� 
							for(int n=ADefectPoint[ACount].Rect.top+1 ; n<ADefectPoint[ACount].Rect.bottom-1; n++){
								for(int m=ADefectPoint[ACount].Rect.left+1; m<ADefectPoint[ACount].Rect.right-1 ; m++){
									PixelValue = IElement(m,n);
									if(PixelValue < TopDarkThresHold) PixelCount++;
								}
							}
							// Display ������ �ʿ��� 
							ADefectPoint[ACount].Round     = PixelCount;
							ADefectPoint[ACount].P.x       = (float)(ADefectPoint[ACount].Rect.left);
							ADefectPoint[ACount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2)+RoiStaY+ImageHeight );
							ADefectPoint[ACount].Width     = ADefectPoint[ACount].Width+SideMargin; 

							// Merge ���ؼ� ������ �ʿ��� 
							ADefectPoint[ACount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height/2)+RoiStaY+ImageHeight);
							ADefectPoint[ACount].Rect.bottom =(long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);
							ACount++;
						}
					}
					BlobCount =0;
				}

				//BSide Dark Defect 
				if(RightDefectRange!=0 && RightDarkThresHold!=0){
					// Image Conversion Binary_Image
					if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, RightDarkThresHold);
					else                   EasyImage::Threshold(&RotateImg, &DefectImg, RightDarkThresHold); 

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&DefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = DefectImg.GetImagePtr();

					//�˻� ROI ���� ���� 
					if (RightGrindInCnt >0 && RightGrindInPos >0)  RightOffset = (int)(RightGrindInPos / RightGrindInCnt);
					if (TopGrindInCnt   >0 && TopGrindInPos   >0)  TopOffset   = (int)(TopGrindInPos   / TopGrindInCnt  );


					RoiStaX   = (int)((RightOffset -RightDefectRange   ));
					RoiStaY   = (int)((TopOffset   +DefectMargin       ));
					RoiWidth  = (int)(RightDefectRange -DefectMargin    );
					RoiHeight = (int)(FrameImgH - RoiStaY - DefectMargin);

					// �˻� ������ 
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

					//�˻� ȭ�� ���÷��� ���� 
					DefectRoiRect[Right_Roi].left   = RoiStaX            ;
					DefectRoiRect[Right_Roi].top    = RoiStaY            ;
					DefectRoiRect[Right_Roi].right  = RoiStaX + RoiWidth ;
					DefectRoiRect[Right_Roi].bottom = RoiStaY + RoiHeight;

					// Blob �˻� ���� 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					// Blob ���͸� 
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth  , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight , FMaxHeight , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit       , RoiWidth-1, RoiWidth+1 , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
					if(BlobCount!=0){
						for(int k=0; k<BlobCount; k++){
							if(BCount>=MAX_DEFECT_POINT) break;
							BDefectPoint[BCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
							BDefectPoint[BCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

							BDefectPoint[BCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +RoiStaX);
							BDefectPoint[BCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +RoiStaY);
							BDefectPoint[BCount].Rect.right  = (long)(BDefectPoint[BCount].Rect.left+ BDefectPoint[BCount].Width );
							BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);

							PixelCount = 0;
							PixelValue = 0;
							// ���� Pixel Data ���� 
							for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
								for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
									PixelValue = IElement(m,n);
									if(PixelValue < RightDarkThresHold) PixelCount++;
								}
							}
							// Display ������ �ʿ��� 
							BDefectPoint[BCount].Round     = PixelCount;
							BDefectPoint[BCount].P.x       = (float)(BDefectPoint[BCount].Rect.left);
							BDefectPoint[BCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)+RoiStaY+ImageHeight );
							BDefectPoint[BCount].Width     = BDefectPoint[BCount].Width+SideMargin; 

							// Merge ���ؼ� ������ �ʿ��� 
							BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)+RoiStaY+ImageHeight);
							BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
							BCount++;
						}
					}
					BlobCount =0;
				}
			}
		}
		else { //�κп��� 
			if (IsCornerTop) {
				MaxDv = 0.0f;
				MinDv = 10000.0f;
				// Corner 1 
				if (C1InEdgeCount != 0) {
					CornerCount = 0;
					for (int k = 0; k < C1InEdgeCount; k++) {
						if (C1InPoint[k].y > FLTLPoint.y - 1) continue;
						Dis = Math.GetLengthFrLineToPoint(FLTLInPoint, FLTTInPoint, C1InPoint[k]);
						C1Corner[CornerCount].Dv = Dis;
						C1Corner[CornerCount].P.x = C1InPoint[k].x;
						C1Corner[CornerCount].P.y = C1InPoint[k].y;
						CornerCount++;
						if (Dis > MaxDv) MaxDv = Dis;
						if (Dis < MinDv) MinDv = Dis;
					}
					if (LTCornerWidth > (LTCornerHeight * 2)) C1Count = GetSize(C1Corner, C1DefectPoint, CornerCount, 10,CorGrindInThresHold);
					else                                      C1Count = GetSize(C1Corner, C1DefectPoint, CornerCount, 5 ,CorGrindInThresHold);
				}
				// Corner 2 
				if (C2InEdgeCount != 0) {
					CornerCount = 0;
					for (int k = 0; k < C2InEdgeCount; k++) {
						if (C2InPoint[k].y > FRTRPoint.y - 1) continue;
						Dis = Math.GetLengthFrLineToPoint(FRTTInPoint, FRTRInPoint, C2InPoint[k]);
						C2Corner[CornerCount].Dv = Dis;
						C2Corner[CornerCount].P.x = C2InPoint[k].x;
						C2Corner[CornerCount].P.y = C2InPoint[k].y;
						CornerCount++;
						if (Dis > MaxDv) MaxDv = Dis;
						if (Dis < MinDv) MinDv = Dis;
					}
					if (RTCornerWidth > (RTCornerHeight * 1.5)) C2Count = GetSize(C2Corner, C2DefectPoint, CornerCount, 10,CorGrindInThresHold);
					else                                        C2Count = GetSize(C2Corner, C2DefectPoint, CornerCount, 5 ,CorGrindInThresHold);
				}
			}
			else if (IsCircleTop) { // Circle Cutting�ϰ�� Defect�˻� ���� 
				if (FInspectMode == Insp_Polygon) { // A_Side ���� Arc Polygon Mode �˻� ������ 
					if (A1CirInEdgeCount != 0) {    // Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 

					if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, PadSideThresHold);
					else                   EasyImage::Threshold(&RotateImg, &DefectImg, PadSideThresHold); //DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_HAP.bmp");
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 

						IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 

						A1CirDeftectCnt = GetPolygonSize(A1CirInPoint, A1CirDefectPoint, A1CirInEdgeCount, 50, 5,PadSideThresHold); // Dark Defect 
         				// �ڸ� �ҷ� �˻� (MOLED �ʿ�����Ͱ���..�ϴ� �ּ�ó��)
//						if (!IsMarkBtm && A1CirInCrackCount > 0 && BrokenThresHold != 0) {
//							IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
//							A1CirCrackCnt = GetCircleCrackSize(A1CircleData, A1CirCrackPoint, A1CirInCrackCount, 30, ACirFitDiameter1, ACircleFit1Center); //Bright Defect 
//						}
					}

					// A1 Circle Masking Image ���� DarkDefect�˻� ������ 
					if (A1CirDeftectCnt == 0 && A1CirInEdgeCount != 0 && TopMaskDarkDefectTh != 0) {
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Conversion Binary
						MaskDarkDefectImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Org
						EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &MaskDarkDefectImg);
						//////////////////////////////////////////////////////////////////////////////////////////////////

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
						HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						// Masking Data Init 
						if (LeftMaskPoint != NULL)  delete[]LeftMaskPoint;
						if (LeftMaskPoint2 != NULL)  delete[]LeftMaskPoint2;

						// ROI���� ���� �������� �ʿ��� Data 
						MaskStaPos = 0; MaskEndPos = 0;
						MinCnt = 0; MaxCnt = 0;

						// Clipper Use Parameter 
						The::Path  Poly_In;
						The::Paths Poly_Out;
						The::ClipperOffset ClipOffset;
						Poly_In.reserve(A1CirInEdgeCount + 2);
						Poly_Out.reserve(2000);
						//////////////////////////////////////////////////////////

						Poly_In.push_back(The::IntPoint(FLTPoint.x, FLTPoint.y));
						for (int k = 0; k < A1CirInEdgeCount; k++) {
							Poly_In.push_back(The::IntPoint(A1CirInPoint[k].x, A1CirInPoint[k].y));
						}
						Poly_In.push_back(The::IntPoint(A1CirInPoint[A1CirInEdgeCount - 1].x, FLTPoint.y));

						ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
						// 						ClipOffset.AddPath(Poly_In ,The::jtSquare,The::etClosedPolygon);
						ClipOffset.Execute(Poly_Out, TopMaskOffset);
						PolyOutCnt = Poly_Out[0].size();

						AllLeftMaskCount1 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
						LeftMaskPoint = new POINT[AllLeftMaskCount1];

						// �������� ������ �ʱ�ȭ 
						for (int k = 0; k < AllLeftMaskCount1; k++) {
							LeftMaskPoint[k].x = 0;
							LeftMaskPoint[k].y = 0;
						}

						for (int k = 0; k < AllLeftMaskCount1; k++) {
							if (k >= Poly_Out[0].size()) break;
							LeftMaskPoint[k].x = Poly_Out[0][k].x;
							LeftMaskPoint[k].y = Poly_Out[0][k].y;
						}

						HRGN Rgn1 = ::CreatePolygonRgn(LeftMaskPoint, AllLeftMaskCount1, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);


						// Data Convert 
						MinDv = 100000.0f;
						MaxDv = 0.0f;
						Poly_In.clear();
						Poly_Out.clear();
						ClipOffset.Clear();
						Poly_In.reserve(A1CirInEdgeCount + 2);
						Poly_Out.reserve(2000);

						Poly_In.push_back(The::IntPoint(FLTPoint.x, (A1CirInPoint[0].y + TopMaskDis)));
						for (int k = 0; k < A1CirInEdgeCount; k++) {
							Poly_In.push_back(The::IntPoint(A1CirInPoint[k].x, A1CirInPoint[k].y));
						}
						// 						Poly_In.push_back(The::IntPoint(A1CirInPoint[A1CirInEdgeCount-1].x+TopMaskDis,(A1CirInPoint[0].y+TopMaskDis)));
						Poly_In.push_back(The::IntPoint(A1CirInPoint[A1CirInEdgeCount - 1].x, (A1CirInPoint[0].y + TopMaskDis)));

						ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
						ClipOffset.Execute(Poly_Out, -TopMaskDis);
						PolyOutCnt = Poly_Out[0].size();

						AllLeftMaskCount2 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
						LeftMaskPoint2 = new POINT[AllLeftMaskCount2];

						// �������� ������ �ʱ�ȭ 
						for (int k = 0; k < AllLeftMaskCount2; k++) {
							LeftMaskPoint2[k].x = 0;
							LeftMaskPoint2[k].y = 0;
						}

						for (int k = 0; k < AllLeftMaskCount2; k++) {
							if (k >= Poly_Out[0].size()) break;
							LeftMaskPoint2[k].x = Poly_Out[0][k].x;
							LeftMaskPoint2[k].y = Poly_Out[0][k].y;
							// 							if(LeftMaskPoint2[k].x<MinDv){MinDv = LeftMaskPoint2[k].x; MinCnt =k;}
							// 							if(LeftMaskPoint2[k].x>MaxDv){MaxDv = LeftMaskPoint2[k].x; MaxCnt =k;}
							if (LeftMaskPoint2[k].x > MaxDv)MaxDv = LeftMaskPoint2[k].x;
						}

						// ���� �� �������� ���� ���� ���� ã�´� 
// 						MaskEndPos = (LeftMaskPoint2[MinCnt].y>LeftMaskPoint2[MaxCnt].y)?LeftMaskPoint2[MaxCnt].y:LeftMaskPoint2[MinCnt].y;

						HRGN Rgn2 = ::CreatePolygonRgn(LeftMaskPoint2, AllLeftMaskCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if (MaskDarkDefectImg.GetImagePtr()) {
							// Image Conversion Binary_Image
// 							MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\Mask_Org_HAP_LEFT.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, TopMaskDarkDefectTh);

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true);
							IAddress = MaskDarkDefectImg.GetImagePtr();

							// �˻� ROI���� 
							RoiStaX   = (int)(FLTPoint.x + SideMargin);
							RoiStaY   = (int)(FLTPoint.y);
							RoiWidth  = (int)(MaxDv - RoiStaX);
							RoiHeight = (int)((A1CirInPoint[0].y - SideMargin) - RoiStaY);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if ((RoiStaX) <= 0 || (RoiStaY) <= 0 || RoiWidth <= 0 || (RoiHeight) <= 0) return false;
							if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || (RoiHeight) > TopLimitLine) return false;
							if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > TopLimitLine)                                          return false;

							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI, CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

							if (BlobCount != 0) {
								for (int k = 0; k < BlobCount; k++) {
									if (A1CirDeftectCnt >= MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + RoiStaX);
									DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight()) / 2) + RoiStaY);
									DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
									DefectRT.y = (float)(DefectLT.y);
									DefectLB.x = (float)(DefectLT.x);
									DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
									DefectRB.x = (float)(DefectRT.x);
									DefectRB.y = (float)(DefectLB.y);

									// 									for(int i=PreTopCompareCnt; i<A1CirInEdgeCount; i++){
									for (int i = 0; i < A1CirInEdgeCount; i++) {
										LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
										IsDefect = false;
										LTDis = Math.GetLength(A1CirInPoint[i], DefectLT);
										RTDis = Math.GetLength(A1CirInPoint[i], DefectRT);
										LBDis = Math.GetLength(A1CirInPoint[i], DefectLB);
										RBDis = Math.GetLength(A1CirInPoint[i], DefectRB);
										// 										if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
										if (LTDis <= (MaskDefectMagin + TopMaskOffset) || RTDis <= (MaskDefectMagin + TopMaskOffset) || LBDis <= (MaskDefectMagin + TopMaskOffset) || RBDis <= (MaskDefectMagin + TopMaskOffset)) {
											IsDefect = true;
											PreTopCompareCnt = i;
											break;
										}
									}

									if (IsDefect) {
										A1CirDefectPoint[A1CirDeftectCnt].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
										A1CirDefectPoint[A1CirDeftectCnt].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

										A1CirDefectPoint[A1CirDeftectCnt].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - A1CirDefectPoint[A1CirDeftectCnt].Width / 2) + RoiStaX);
										A1CirDefectPoint[A1CirDeftectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - A1CirDefectPoint[A1CirDeftectCnt].Height / 2) + RoiStaY);
										A1CirDefectPoint[A1CirDeftectCnt].Rect.right = (long)(A1CirDefectPoint[A1CirDeftectCnt].Rect.left + A1CirDefectPoint[A1CirDeftectCnt].Width);
										A1CirDefectPoint[A1CirDeftectCnt].Rect.bottom = (long)(A1CirDefectPoint[A1CirDeftectCnt].Rect.top + A1CirDefectPoint[A1CirDeftectCnt].Height);

										PixelCount = 0;
										PixelValue = 0;
										// ���� Pixel Data ���� 
										for (int n = A1CirDefectPoint[A1CirDeftectCnt].Rect.top + 1; n < A1CirDefectPoint[A1CirDeftectCnt].Rect.bottom - 1; n++) {
											for (int m = A1CirDefectPoint[A1CirDeftectCnt].Rect.left + 1; m < A1CirDefectPoint[A1CirDeftectCnt].Rect.right - 1; m++) {
												PixelValue = IElement(m, n);
												if (PixelValue < TopMaskDarkDefectTh) PixelCount++;
											}
										}
										// Display ������ �ʿ��� 
										A1CirDefectPoint[A1CirDeftectCnt].Round = PixelCount;
										A1CirDefectPoint[A1CirDeftectCnt].P.x = (float)(A1CirDefectPoint[A1CirDeftectCnt].Rect.left);
										A1CirDefectPoint[A1CirDeftectCnt].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - A1CirDefectPoint[A1CirDeftectCnt].Height / 2) + RoiStaY + ImageHeight);
										A1CirDefectPoint[A1CirDeftectCnt].Width = A1CirDefectPoint[A1CirDeftectCnt].Width + SideMargin;

										// Merge ���ؼ� ������ �ʿ��� 
										A1CirDefectPoint[A1CirDeftectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - A1CirDefectPoint[A1CirDeftectCnt].Height / 2) + RoiStaY + ImageHeight);
										A1CirDefectPoint[A1CirDeftectCnt].Rect.bottom = (long)(A1CirDefectPoint[A1CirDeftectCnt].Rect.top + A1CirDefectPoint[A1CirDeftectCnt].Height);
										A1CirDeftectCnt++;
									}
								}
							}
							BlobCount = 0;
						}
					}


					if (A2CirInEdgeCount != 0) {

						if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, PadSideThresHold);
						else                   EasyImage::Threshold(&RotateImg, &DefectImg, PadSideThresHold); //DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_HAP.bmp");
						IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
						A2CirDeftectCnt = GetPolygonSize(A2CirInPoint, A2CirDefectPoint, A2CirInEdgeCount, 50, 5, PadSideThresHold); // Dark Defect 

         				// �ڸ� �ҷ� �˻� (MOLED �ʿ�����Ͱ���..�ϴ� �ּ�ó��)
//						if (!IsMarkBtm && A2CirInCrackCount > 0 && BrokenThresHold != 0) {
//							IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
//							A2CirCrackCnt = GetCircleCrackSize(A2CircleData, A2CirCrackPoint, A2CirInCrackCount, 30, ACirFitDiameter2, ACircleFit2Center); // Bright Defect 
//						}
					}

					// A1 Circle Masking Image ���� DarkDefect�˻� ������ 
					if (A2CirDeftectCnt == 0 && A2CirInEdgeCount != 0 && TopMaskDarkDefectTh != 0) {
						// MaskImage Create Start 
						//////////////////////////////////////////////////////////////////////////////////////////////////
						BinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Conversion Binary
						MaskDarkDefectImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Org
						EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &MaskDarkDefectImg);
						//////////////////////////////////////////////////////////////////////////////////////////////////

						HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
						HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

						if (RightMaskPoint != NULL)  delete[]RightMaskPoint;
						if (RightMaskPoint2 != NULL)  delete[]RightMaskPoint2;

						// ROI���� ���� �������� �ʿ��� Data 
						MaskStaPos = 0; MaskEndPos = 0;
						MinCnt = 0; MaxCnt = 0;

						// Clipper Use Parameter 
						The::Path  Poly_In;
						The::Paths Poly_Out;
						The::ClipperOffset ClipOffset;
						Poly_In.reserve(A2CirInEdgeCount + 2);
						Poly_Out.reserve(2000);

						// ��ü���� ������ MaskPoint �Ѱ��� 
						Poly_In.push_back(The::IntPoint(A2CirInPoint[0].x, FRTPoint.y));
						for (int k = 0; k < A2CirInEdgeCount; k++) {
							Poly_In.push_back(The::IntPoint(A2CirInPoint[k].x, A2CirInPoint[k].y));
						}
						Poly_In.push_back(The::IntPoint(FRTPoint.x, FRTPoint.y));

						// 						ClipOffset.AddPath(Poly_In ,The::jtSquare,The::etClosedPolygon);
						ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
						ClipOffset.Execute(Poly_Out, TopMaskOffset);
						PolyOutCnt = Poly_Out[0].size();

						AllRightMaskCount1 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
						RightMaskPoint = new POINT[AllRightMaskCount1];

						// �������� ������ �ʱ�ȭ 
						for (int k = 0; k < AllRightMaskCount1; k++) {
							RightMaskPoint[k].x = 0;
							RightMaskPoint[k].y = 0;
						}

						for (int k = 0; k < AllRightMaskCount1; k++) {
							if (k >= Poly_Out[0].size()) break;
							RightMaskPoint[k].x = Poly_Out[0][k].x;
							RightMaskPoint[k].y = Poly_Out[0][k].y;
						}

						HRGN Rgn1 = ::CreatePolygonRgn(RightMaskPoint, AllRightMaskCount1, WINDING);
						::FillRgn(hdc, Rgn1, fillBrh);
						::DeleteObject(Rgn1);

						// Data Convert 
						MinDv = 100000.0f;
						MaxDv = 0.0f;
						Poly_In.clear();
						Poly_Out.clear();
						ClipOffset.Clear();
						Poly_In.reserve(A2CirInEdgeCount + 2);
						Poly_Out.reserve(2000);

						Poly_In.push_back(The::IntPoint(A2CirInPoint[0].x, (A2CirInPoint[A2CirInEdgeCount - 1].y + TopMaskDis)));
						// 						Poly_In.push_back(The::IntPoint(A2CirInPoint[0].x-TopMaskDis,(A2CirInPoint[0].y+TopMaskDis)));
						// 						Poly_In.push_back(The::IntPoint(A2CirInPoint[0].x,(A2CirInPoint[0].y+TopMaskDis)));
						for (int k = 0; k < A2CirInEdgeCount; k++) {
							Poly_In.push_back(The::IntPoint(A2CirInPoint[k].x, A2CirInPoint[k].y));
						}
						// 						Poly_In.push_back(The::IntPoint(A2CirInPoint[0].x,(A2CirInPoint[A2CirInEdgeCount-1].y+TopMaskDis)));
						// 						Poly_In.push_back(The::IntPoint(A2CirInPoint[A2CirInEdgeCount-1].x-TopMaskDis,(A2CirInPoint[A2CirInEdgeCount-1].y+TopMaskDis)));
						Poly_In.push_back(The::IntPoint(A2CirInPoint[A2CirInEdgeCount - 1].x, (A2CirInPoint[A2CirInEdgeCount - 1].y + TopMaskDis)));

						ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
						ClipOffset.Execute(Poly_Out, -TopMaskDis);
						PolyOutCnt = Poly_Out[0].size();

						AllRightMaskCount2 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
						RightMaskPoint2 = new POINT[AllRightMaskCount2];

						// �������� ������ �ʱ�ȭ 
						for (int k = 0; k < AllRightMaskCount2; k++) {
							RightMaskPoint2[k].x = 0;
							RightMaskPoint2[k].y = 0;
						}

						for (int k = 0; k < AllRightMaskCount2; k++) {
							if (k >= Poly_Out[0].size()) break;
							RightMaskPoint2[k].x = Poly_Out[0][k].x;
							RightMaskPoint2[k].y = Poly_Out[0][k].y;
							// 							if(RightMaskPoint2[k].x<MinDv){MinDv = RightMaskPoint2[k].x; MinCnt =k;}
							// 							if(RightMaskPoint2[k].x>MaxDv){MaxDv = RightMaskPoint2[k].x; MaxCnt =k;}
							if (RightMaskPoint2[k].x < MinDv)MinDv = RightMaskPoint2[k].x;
						}

						HRGN Rgn2 = ::CreatePolygonRgn(RightMaskPoint2, AllRightMaskCount2, WINDING);
						::FillRgn(hdc, Rgn2, fillBrh);
						::DeleteObject(Rgn2);
						::DeleteObject(fillBrh);

						Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);
						//////////////////////////////////////////////////////////////////////////////////////////////////

						if (MaskDarkDefectImg.GetImagePtr()) {
							// Image Conversion Binary_Image
// 							MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\Mask_Org_HAP_RIGHT.bmp");
							EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, TopMaskDarkDefectTh);

							// Image Euresys Algorithm Attach 
							DefectROI.Attach(&BinaryImg);
							CodeImageSelection.SetFeretAngle(0.00f);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
							CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true);
							IAddress = MaskDarkDefectImg.GetImagePtr();

							// �˻� ROI���� 
							RoiStaX = (int)(MinDv);
							// 							RoiStaY      =(int)(A2CirInPoint[0].y+MaskingMargin)           ;
							// 							RoiWidth     =(int)(MaxDv - MinDv)                             ;
							// 							RoiHeight    =(int)(A2CirInPoint[A2CirInEdgeCount-1].y-RoiStaY);
							RoiStaY = (int)(FRTPoint.y);
							RoiWidth = (int)(FRTPoint.x - MinDv);
							// 							RoiHeight    =(int)(MaskEndPos-FRTPoint.y)                     ;
							RoiHeight = (int)((A2CirInPoint[A2CirInEdgeCount - 1].y - SideMargin) - RoiStaY);

							// �˻� ������ 
							DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
							if ((RoiStaX) <= 0 || (RoiStaY) <= 0 || RoiWidth <= 0 || (RoiHeight) <= 0) return false;
							if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || (RoiHeight) > TopLimitLine) return false;
							if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > TopLimitLine)                                          return false;

							// Blob �˻� ���� 
							CodeImageSelection.Clear();
							CodeImageEncoder.Encode(DefectROI, CodeImage);
							CodeImageSelection.Clear();
							CodeImageSelection.AddObjects(CodeImage);

							BlobCount = CodeImageSelection.GetElementCount();
							CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

							if (BlobCount != 0) {
								for (int k = 0; k < BlobCount; k++) {
									if (A2CirDeftectCnt >= MAX_DEFECT_POINT) break;
									// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
									DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + RoiStaX);
									DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight()) / 2) + RoiStaY);
									DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
									DefectRT.y = (float)(DefectLT.y);
									DefectLB.x = (float)(DefectLT.x);
									DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
									DefectRB.x = (float)(DefectRT.x);
									DefectRB.y = (float)(DefectLB.y);

									// 									for(int i=PreTopCompareCnt; i<A2CirInEdgeCount; i++){
									for (int i = 0; i < A2CirInEdgeCount; i++) {
										LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
										IsDefect = false;
										LTDis = Math.GetLength(A2CirInPoint[i], DefectLT);
										RTDis = Math.GetLength(A2CirInPoint[i], DefectRT);
										LBDis = Math.GetLength(A2CirInPoint[i], DefectLB);
										RBDis = Math.GetLength(A2CirInPoint[i], DefectRB);
										if (LTDis <= MaskDefectMagin || RTDis <= MaskDefectMagin || LBDis <= MaskDefectMagin || RBDis <= MaskDefectMagin) {
											IsDefect = true;
											PreTopCompareCnt = i;
											break;
										}
									}

									if (IsDefect) {
										A2CirDefectPoint[A2CirDeftectCnt].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
										A2CirDefectPoint[A2CirDeftectCnt].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

										A2CirDefectPoint[A2CirDeftectCnt].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - A2CirDefectPoint[A2CirDeftectCnt].Width / 2) + RoiStaX);
										A2CirDefectPoint[A2CirDeftectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - A2CirDefectPoint[A2CirDeftectCnt].Height / 2) + RoiStaY);
										A2CirDefectPoint[A2CirDeftectCnt].Rect.right = (long)(A2CirDefectPoint[A2CirDeftectCnt].Rect.left + A2CirDefectPoint[A2CirDeftectCnt].Width);
										A2CirDefectPoint[A2CirDeftectCnt].Rect.bottom = (long)(A2CirDefectPoint[A2CirDeftectCnt].Rect.top + A2CirDefectPoint[A2CirDeftectCnt].Height);

										PixelCount = 0;
										PixelValue = 0;
										// ���� Pixel Data ���� 
										for (int n = A2CirDefectPoint[A2CirDeftectCnt].Rect.top + 1; n < A2CirDefectPoint[A2CirDeftectCnt].Rect.bottom - 1; n++) {
											for (int m = A2CirDefectPoint[A2CirDeftectCnt].Rect.left + 1; m < A2CirDefectPoint[A2CirDeftectCnt].Rect.right - 1; m++) {
												PixelValue = IElement(m, n);
												if (PixelValue < TopMaskDarkDefectTh) PixelCount++;
											}
										}
										// Display ������ �ʿ��� 
										A2CirDefectPoint[A2CirDeftectCnt].Round = PixelCount;
										A2CirDefectPoint[A2CirDeftectCnt].P.x = (float)(A2CirDefectPoint[A2CirDeftectCnt].Rect.left);
										A2CirDefectPoint[A2CirDeftectCnt].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - A2CirDefectPoint[A2CirDeftectCnt].Height / 2) + RoiStaY + ImageHeight);
										A2CirDefectPoint[A2CirDeftectCnt].Width = A2CirDefectPoint[A2CirDeftectCnt].Width + SideMargin;

										// Merge ���ؼ� ������ �ʿ��� 
										A2CirDefectPoint[A2CirDeftectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - A2CirDefectPoint[A2CirDeftectCnt].Height / 2) + RoiStaY + ImageHeight);
										A2CirDefectPoint[A2CirDeftectCnt].Rect.bottom = (long)(A2CirDefectPoint[A2CirDeftectCnt].Rect.top + A2CirDefectPoint[A2CirDeftectCnt].Height);
										A2CirDeftectCnt++;
									}
								}
							}
							BlobCount = 0;
						}
					}
				}
			}

			// Side A
			// �κп��������� �˻��Ѵ� 
			////////////////////////////////////////////////////////////////////////////////////////////////
			//�˻� ROI���� 
//			DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_ASide.bmp");
			RoiStaX   = TopEdgePoint[1].x + SideMargin;
			RoiStaY   = TopEdgePoint[1].y + Margin;
			RoiWidth  = (TopEdgePoint[2].x - TopEdgePoint[1].x) - (2 * SideMargin);
			RoiHeight = 200;

			DefectROI.SetPlacement((int)RoiStaX, (int)RoiStaY, RoiWidth, RoiHeight);
			if ((RoiStaX) <= 0         || (RoiStaY) <= 0           || RoiWidth <= 0         || RoiHeight <= 0          ) return false;
			if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || RoiHeight > TopLimitLine) return false;
			if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > TopLimitLine) return false;

			// Blob �˻� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI, CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth , FMinWidth , FMaxWidth , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight, FMinHeight, FMaxHeight, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_TopLimit, 0, 3, EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_LeftLimit, ESortDirection_Ascending);

			if (BlobCount != 0) {
				for (int k = 0; k < BlobCount; k++) {
					if (ACount >= MAX_DEFECT_POINT) break;
					ADefectPoint[ACount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
					ADefectPoint[ACount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					ADefectPoint[ACount].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width / 2) + RoiStaX);
					ADefectPoint[ACount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height / 2) + RoiStaY);
					ADefectPoint[ACount].Rect.right = (long)(ADefectPoint[ACount].Rect.left + ADefectPoint[ACount].Width);
					ADefectPoint[ACount].Rect.bottom = (long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for (int n = ADefectPoint[ACount].Rect.top + 1; n < ADefectPoint[ACount].Rect.bottom - 1; n++) {
						for (int m = ADefectPoint[ACount].Rect.left + 1; m < ADefectPoint[ACount].Rect.right - 1; m++) {
							PixelValue = IElement(m, n);
							if (PixelValue < TopDarkThresHold) PixelCount++;
						}
					}

					ADefectPoint[ACount].Round = PixelCount;
					ADefectPoint[ACount].P.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - ADefectPoint[ACount].Width / 2) + RoiStaX);
					ADefectPoint[ACount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height / 2) + RoiStaY);

					// Image Rotate ���� ��ǥ�� �ٽ� Rotate�Ѵ� _ Display ���� 
					if (ImgDegree != 0.0f) {
						ADefectPoint[ACount].P = Math.OnRotate(CenterPoint, ADefectPoint[ACount].P, (ImgDegree));
					}
					// Image Rotate ���� ��ǥ�� ȸ�� ���Ŀ� ImageHeight���� ���Ͽ� ���� ��ǥ�� �����Ѵ� 
					ADefectPoint[ACount].P.y = (float)(ADefectPoint[ACount].P.y + ImageHeight);

					//Merget���ؼ� �ʿ��� 
					ADefectPoint[ACount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - ADefectPoint[ACount].Height / 2) + RoiStaY + ImageHeight);
					ADefectPoint[ACount].Rect.bottom = (long)(ADefectPoint[ACount].Rect.top + ADefectPoint[ACount].Height);
					ACount++;
				}
			}
		}

		// ��ü���� vs �κп��� ������� �������� BSide,DSide �˻縦 �����Ѵ� 
		//Side B
		////////////////////////////////////////////////////////////////////////////////////////////////
		// �˻� ROI����
		if(TopEdgePoint[3].x!=0 && TopEdgePoint[3].y!=0){
			EasyImage::Threshold(GImage, &DefectImg, RightDarkThresHold);
			IAddress = DefectImg.GetImagePtr();

			//		DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_BSide.bmp");

			RoiWidth = 200;
			RoiStaX = TopEdgePoint[3].x - (RoiWidth + 1);
			RoiStaY = TopEdgePoint[3].y + Margin;
			RoiHeight = (FrameImgH - RoiStaY) - RightDegMargin;

			DefectROI.SetPlacement((int)RoiStaX, (int)RoiStaY, RoiWidth, RoiHeight);
			if ((RoiStaX) <= 0         || (RoiStaY) <= 0           || RoiWidth <= 0         || RoiHeight <= 0          ) return false;
			if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || RoiHeight > TopLimitLine) return false;
			if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > TopLimitLine) return false;
			//  Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI, CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth, FMinWidth, FMaxWidth, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight, FMinHeight, FMaxHeight, EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit     ,FRightWidth-2 , FRightWidth+1, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit, FRightWidth - 1, FRightWidth + 1, EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

			if (BlobCount != 0) {
				for (int k = 0; k < BlobCount; k++) {
					if (BCount >= MAX_DEFECT_POINT) break;
					BDefectPoint[BCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
					BDefectPoint[BCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					BDefectPoint[BCount].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width / 2) + RoiStaX);
					BDefectPoint[BCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height / 2) + RoiStaY);
					BDefectPoint[BCount].Rect.right = (long)(BDefectPoint[BCount].Rect.left + BDefectPoint[BCount].Width);
					BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for (int n = BDefectPoint[BCount].Rect.top + 1; n < BDefectPoint[BCount].Rect.bottom - 1; n++) {
						for (int m = BDefectPoint[BCount].Rect.left + 1; m < BDefectPoint[BCount].Rect.right - 1; m++) {
							PixelValue = IElement(m, n);
							if (PixelValue < RightDarkThresHold) PixelCount++;
						}
					}
					BDefectPoint[BCount].Round = PixelCount;
					BDefectPoint[BCount].P.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width / 2) + RoiStaX);
					BDefectPoint[BCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height / 2) + RoiStaY + ImageHeight);
					BDefectPoint[BCount].Width = BDefectPoint[BCount].Width + SideMargin;

					// Merge ���ؼ� ������ �ʿ��� 
					BDefectPoint[BCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height / 2) + RoiStaY + ImageHeight);
					BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount = 0;

			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if      (Line[waRightWall].FP0.x > Line[waRightWall].FP1.x) RDistance = ((Line[waRightWall].FP1.x + (Line[waRightWall].FP0.x - Line[waRightWall].FP1.x) / 2) - FP1.x);
			else if (Line[waRightWall].FP0.x== Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP0.x - FP1.x);
			else                                                        RDistance = ((Line[waRightWall].FP0.x + (Line[waRightWall].FP1.x - Line[waRightWall].FP0.x) / 2) - FP1.x);

			MarktoBmLine = (CamId==Cam_Top) ? 5 : 50;
			if (CamId == Cam_Top) RDistance = RDistance - MarktoBmLine;
			else                  RDistance = RDistance + MarktoBmLine;
		}

		// Side D	
		////////////////////////////////////////////////////////////////////////////////////////////////
		// �˻� ROI���� 
		if(TopEdgePoint[0].x!=0 && TopEdgePoint[0].y!=0){
			EasyImage::Threshold(GImage, &DefectImg, LeftDarkThresHold);
			IAddress = DefectImg.GetImagePtr();

			RoiStaX = TopEdgePoint[0].x + SideMargin;
			RoiStaY = TopEdgePoint[0].y + Margin;
			RoiWidth = 200;
			RoiHeight = (FrameImgH - RoiStaY) - LeftDegMargin;

			DefectROI.SetPlacement((int)RoiStaX, (int)RoiStaY, RoiWidth, RoiHeight);
			if ((RoiStaX) <= 0 || (RoiStaY) <= 0 || RoiWidth <= 0 || RoiHeight <= 0) return false;
			if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || RoiHeight > TopLimitLine) return false;
			if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > TopLimitLine) return false;

			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI, CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);

			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth, FMinWidth, FMaxWidth, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight, FMinHeight, FMaxHeight, EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0         , 3          , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit, 0, 1, EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

			if (BlobCount != 0) {
				for (int k = 0; k < BlobCount; k++) {
					if (DCount >= MAX_DEFECT_POINT) break;
					DDefectPoint[DCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
					DDefectPoint[DCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DDefectPoint[DCount].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + RoiStaX);
					DDefectPoint[DCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + RoiStaY);
					DDefectPoint[DCount].Rect.right = (long)(DDefectPoint[DCount].Rect.left + DDefectPoint[DCount].Width);
					DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

					PixelCount = 0;
					PixelValue = 0;
					// ���� Pixel Data ���� 
					for (int n = DDefectPoint[DCount].Rect.top + 1; n < DDefectPoint[DCount].Rect.bottom - 1; n++) {
						for (int m = DDefectPoint[DCount].Rect.left + 1; m < DDefectPoint[DCount].Rect.right - 1; m++) {
							PixelValue = IElement(m, n);
							if (PixelValue < LeftDarkThresHold) PixelCount++;
						}
					}
					// Display ������ �ʿ��� 
					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x = (float)(DDefectPoint[DCount].Rect.left);
					DDefectPoint[DCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + RoiStaY + ImageHeight);
					DDefectPoint[DCount].Width = DDefectPoint[DCount].Width + SideMargin;

					// Merge ���ؼ� ������ �ʿ��� 
					DDefectPoint[DCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + RoiStaY + ImageHeight);
					DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount = 0;

			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if      (Line[waLeftWall].FP0.x > Line[waLeftWall].FP1.x ) LDistance = (FP0.x - (Line[waLeftWall].FP1.x + (Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x) / 2));
			else if (Line[waLeftWall].FP0.x == Line[waLeftWall].FP1.x) LDistance = (FP0.x - Line[waLeftWall].FP0.x);
			else                                                       LDistance = (FP0.x - (Line[waLeftWall].FP0.x + (Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x) / 2));
		}
		////////////////////////////////////////////////////////////////////////////////////////////////


		// Cell Size ���� ������ �Ѱ���(Image ȸ�� �������)
		LeftStaPos.x = FLTPoint.x;
		LeftStaPos.y = FLTPoint.y + ImageHeight;
		CenterStaPos.x = FLTPoint.x + (FRTPoint.x - FLTPoint.x) / 2;
		CenterStaPos.y = (FLTPoint.y + FRTPoint.y) / 2 + ImageHeight;

		// FLTPoint,FRTPoint ���� Image Rotate �����Ͽ� Display �������� �����̹��� ���� ��ǥ�� ������Ų
		if (ImgDegree != 0.0f) {
			if (IsTopAllGrind) {
				// EdgeInPoint 
				if (AllTopInEdgeCount != 0) {
					for (int k = 0; k < AllTopInEdgeCount; k++) {
						if (AllTopInData[k].x == 0.0f || AllTopInData[k].y == 0.0f) continue;
						AllTopInData[k] = Math.OnRotate(CenterPoint, AllTopInData[k], ImgDegree);
					}
				}

//				// 20190720
//				if (AllMaskCount1 != 0) {
//					for (int k = 0; k < AllMaskCount1; k++) {
//						MaskPoint[k].x = Math.OnRotate((int)CenterPoint.x, (int)CenterPoint.y, (int)MaskPoint[k].x, (int)MaskPoint[k].y, ImgDegree).x;
//						MaskPoint[k].y = Math.OnRotate((int)CenterPoint.x, (int)CenterPoint.y, (int)MaskPoint[k].x, (int)MaskPoint[k].y, ImgDegree).y;
//					}
//				}
//
//				if (AllMaskCount2 != 0) {
//					for (int k = 0; k < AllMaskCount2; k++) {
//						MaskPoint2[k].x = Math.OnRotate((int)CenterPoint.x, (int)CenterPoint.y, (int)MaskPoint2[k].x, (int)MaskPoint2[k].y, ImgDegree).x;
//						MaskPoint2[k].y = Math.OnRotate((int)CenterPoint.x, (int)CenterPoint.y, (int)MaskPoint2[k].x, (int)MaskPoint2[k].y, ImgDegree).y;
//					}
//				}

				// EdgeOutPoint 
				if (AllTopOutEdgeCount != 0) {
					for (int k = 0; k < AllTopOutEdgeCount; k++) {
						if (AllTopOutData[k].x == 0.0f || AllTopOutData[k].y == 0.0f) continue;
						AllTopOutData[k] = Math.OnRotate(CenterPoint, AllTopOutData[k], ImgDegree);
					}
				}
				//Dark_Defect 
				if (TopDefectCount != 0) {
					for (int k = 0; k < TopDefectCount; k++) {
						TopDefectPoint[k].P.y = TopDefectPoint[k].P.y - ImageHeight;
						if (TopDefectPoint[k].P.y > 0) {
							TopDefectPoint[k].P = Math.OnRotate(CenterPoint, TopDefectPoint[k].P, (ImgDegree));
							TopDefectPoint[k].P.y = TopDefectPoint[k].P.y + ImageHeight;
						}
						else {
							TopDefectPoint[k].P.y = TopDefectPoint[k].P.y + ImageHeight;
						}
					}
				}
				//Bright_Defect 
				if (TopCrackCount != 0) {
					for (int k = 0; k < TopCrackCount; k++) {
						TopCrackPoint[k].P.y = TopCrackPoint[k].P.y - ImageHeight;
						if (TopCrackPoint[k].P.y > 0) {
							TopCrackPoint[k].P = Math.OnRotate(CenterPoint, TopCrackPoint[k].P, (ImgDegree));
							TopCrackPoint[k].P.y = TopCrackPoint[k].P.y + ImageHeight;
						}
						else {
							TopCrackPoint[k].P.y = TopCrackPoint[k].P.y + ImageHeight;
						}
					}
				}

				//ASide Dark Defect 
				if(ACount!=0){
					for (int k = 0; k < ACount; k++) {
						ADefectPoint[k].P.y = ADefectPoint[k].P.y - ImageHeight;
						if (ADefectPoint[k].P.y > 0) {
							ADefectPoint[k].P = Math.OnRotate(CenterPoint, ADefectPoint[k].P, (ImgDegree));
							ADefectPoint[k].P.y = ADefectPoint[k].P.y + ImageHeight;
						}
						else {
							ADefectPoint[k].P.y = ADefectPoint[k].P.y + ImageHeight;
						}
					}
				}

				//BSide Dark Defect 
				if(BCount!=0){
					for (int k = 0; k < BCount; k++) {
						BDefectPoint[k].P.y = BDefectPoint[k].P.y - ImageHeight;
						if (BDefectPoint[k].P.y > 0) {
							BDefectPoint[k].P = Math.OnRotate(CenterPoint, BDefectPoint[k].P, (ImgDegree));
							BDefectPoint[k].P.y = BDefectPoint[k].P.y + ImageHeight;
						}
						else {
							BDefectPoint[k].P.y = BDefectPoint[k].P.y + ImageHeight;
						}
					}
				}

				//BSide Dark Defect 
				if(DCount!=0){
					for (int k = 0; k < DCount; k++) {
						DDefectPoint[k].P.y = DDefectPoint[k].P.y - ImageHeight;
						if (DDefectPoint[k].P.y > 0) {
							DDefectPoint[k].P = Math.OnRotate(CenterPoint, DDefectPoint[k].P, (ImgDegree));
							DDefectPoint[k].P.y = DDefectPoint[k].P.y + ImageHeight;
						}
						else {
							DDefectPoint[k].P.y = DDefectPoint[k].P.y + ImageHeight;
						}
					}
				}
			}
			else {
				if (IsCornerTop) {
					FLTTPoint    = Math.OnRotate(CenterPoint, FLTTPoint, (ImgDegree));
					FLTLPoint    = Math.OnRotate(CenterPoint, FLTLPoint, (ImgDegree));
					FLTTInPoint  = Math.OnRotate(CenterPoint, FLTTInPoint, (ImgDegree));
					FLTLInPoint  = Math.OnRotate(CenterPoint, FLTLInPoint, (ImgDegree));
					FLTTMeaPoint = Math.OnRotate(CenterPoint, FLTTMeaPoint, (ImgDegree));
					FLTLMeaPoint = Math.OnRotate(CenterPoint, FLTLMeaPoint, (ImgDegree));

					FRTTPoint    = Math.OnRotate(CenterPoint, FRTTPoint, (ImgDegree));
					FRTRPoint    = Math.OnRotate(CenterPoint, FRTRPoint, (ImgDegree));
					FRTTInPoint  = Math.OnRotate(CenterPoint, FRTTInPoint, (ImgDegree));
					FRTRInPoint  = Math.OnRotate(CenterPoint, FRTRInPoint, (ImgDegree));
					FRTTMeaPoint = Math.OnRotate(CenterPoint, FRTTMeaPoint, (ImgDegree));
					FRTRMeaPoint = Math.OnRotate(CenterPoint, FRTRMeaPoint, (ImgDegree));

					// Corner Defect ��ǥ�� ���� �����Ͽ� �����ؾ��� 
					if (C1Count != 0) {
						for (int k = 0; k < C1Count; k++) {
							// Fram���� Y��ǥ������ �����Ѵ� 
							C1DefectPoint[k].P.y = C1DefectPoint[k].P.y - ImageHeight;
							if (C1DefectPoint[k].P.y > 0) {
								C1DefectPoint[k].P = Math.OnRotate(CenterPoint, C1DefectPoint[k].P, (ImgDegree));
								C1DefectPoint[k].P.y = C1DefectPoint[k].P.y + ImageHeight;
							}
							else {
								C1DefectPoint[k].P.y = C1DefectPoint[k].P.y + ImageHeight;
							}
						}
					}

					if (C2Count != 0) {
						for (int k = 0; k < C2Count; k++) {
							// Fram���� Y��ǥ������ �����Ѵ� 
							C2DefectPoint[k].P.y = C2DefectPoint[k].P.y - ImageHeight;
							if (C2DefectPoint[k].P.y > 0) {
								C2DefectPoint[k].P = Math.OnRotate(CenterPoint, C2DefectPoint[k].P, (ImgDegree));
								C2DefectPoint[k].P.y = C2DefectPoint[k].P.y + ImageHeight;
							}
							else {
								C2DefectPoint[k].P.y = C2DefectPoint[k].P.y + ImageHeight;
							}
						}
					}
				}
				else if (IsCircleTop) {
					if (A1CirDeftectCnt != 0) {
						for (int k = 0; k < A1CirDeftectCnt; k++) {
							A1CirDefectPoint[k].P.y = A1CirDefectPoint[k].P.y - ImageHeight;
							if (A1CirDefectPoint[k].P.y > 0) {
								A1CirDefectPoint[k].P = Math.OnRotate(CenterPoint, A1CirDefectPoint[k].P, (ImgDegree));
								A1CirDefectPoint[k].P.y = A1CirDefectPoint[k].P.y + ImageHeight;
							}
							else {
								A1CirDefectPoint[k].P.y = A1CirDefectPoint[k].P.y + ImageHeight;
							}
						}
					}
					if (A2CirDeftectCnt != 0) {
						// A���� Circle_Defect ��ǥ �̹��� ���� ȸ����
						for (int k = 0; k < A2CirDeftectCnt; k++) {
							A2CirDefectPoint[k].P.y = A2CirDefectPoint[k].P.y - ImageHeight;
							if (A2CirDefectPoint[k].P.y > 0) {
								A2CirDefectPoint[k].P = Math.OnRotate(CenterPoint, A2CirDefectPoint[k].P, (ImgDegree));
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
					if (A1CirCrackCnt != 0) {
						for (int k = 0; k < A1CirCrackCnt; k++) {
							A1CirCrackPoint[k].P.y = A1CirCrackPoint[k].P.y - ImageHeight;
							if (A1CirCrackPoint[k].P.y > 0) {
								A1CirCrackPoint[k].P = Math.OnRotate(CenterPoint, A1CirCrackPoint[k].P, (ImgDegree));
								A1CirCrackPoint[k].P.y = A1CirCrackPoint[k].P.y + ImageHeight;
							}
							else {
								A1CirCrackPoint[k].P.y = A1CirCrackPoint[k].P.y + ImageHeight;
							}
						}
					}
					if (A2CirCrackCnt != 0) {
						for (int k = 0; k < A2CirCrackCnt; k++) {
							A2CirCrackPoint[k].P.y = A2CirCrackPoint[k].P.y - ImageHeight;
							if (A2CirCrackPoint[k].P.y > 0) {
								A2CirCrackPoint[k].P = Math.OnRotate(CenterPoint, A2CirCrackPoint[k].P, (ImgDegree));
								A2CirCrackPoint[k].P.y = A2CirCrackPoint[k].P.y + ImageHeight;
							}
							else {
								A2CirCrackPoint[k].P.y = A2CirCrackPoint[k].P.y + ImageHeight;
							}
						}
					}
					//////////////////////////////////////////////////////////////////////////////////////

					if (A1CirInEdgeCount != 0) {
						for (int k = 0; k < A1CirInEdgeCount; k++) {
							if (A1CirInPoint[k].x == 0.0f || A1CirInPoint[k].y == 0.0f) continue;
							A1CirInPoint[k] = Math.OnRotate(CenterPoint, A1CirInPoint[k], ImgDegree);
						}
					}
					if (A1CirOutEdgeCount != 0) {
						for (int k = 0; k < A1CirOutEdgeCount; k++) {
							if (A1CirOutPoint[k].x == 0.0f || A1CirOutPoint[k].y == 0.0f) continue;
							A1CirOutPoint[k] = Math.OnRotate(CenterPoint, A1CirOutPoint[k], ImgDegree);
						}
					}


					if (A2CirInEdgeCount != 0) {
						for (int k = 0; k < A2CirInEdgeCount; k++) {
							if (A2CirInPoint[k].x == 0.0f || A2CirInPoint[k].y == 0.0f) continue;
							A2CirInPoint[k] = Math.OnRotate(CenterPoint, A2CirInPoint[k], ImgDegree);
						}
					}
					if (A2CirOutEdgeCount != 0) {
						for (int k = 0; k < A2CirOutEdgeCount; k++) {
							if (A2CirOutPoint[k].x == 0.0f || A2CirOutPoint[k].y == 0.0f) continue;
							A2CirOutPoint[k] = Math.OnRotate(CenterPoint, A2CirOutPoint[k], ImgDegree);
						}
					}
				}
			}
			// Shape������� ���� ������ 
			// TopLine 
			Line[1].FP0        = Math.OnRotate(CenterPoint, Line[1].FP0, (ImgDegree));
			Line[1].FP1        = Math.OnRotate(CenterPoint, Line[1].FP1, (ImgDegree));

			// LeftOutLIne 
			LeftOutLine[0].FP0 = Math.OnRotate(CenterPoint, LeftOutLine[0].FP0, (ImgDegree));
			LeftOutLine[0].FP1 = Math.OnRotate(CenterPoint, LeftOutLine[0].FP1, (ImgDegree));

			// RightOutLIne 
			RightOutLine[0].FP0 = Math.OnRotate(CenterPoint, RightOutLine[0].FP0, (ImgDegree));
			RightOutLine[0].FP1 = Math.OnRotate(CenterPoint, RightOutLine[0].FP1, (ImgDegree));

			// ������ ��ǥ 
			FRTPoint = Math.OnRotate(CenterPoint, FRTPoint, (ImgDegree));
			FLTPoint = Math.OnRotate(CenterPoint, FLTPoint, (ImgDegree));

			PatternLine[0].FP0 = Math.OnRotate(CenterPoint, PatternLine[0].FP0, (ImgDegree));
			PatternLine[0].FP1 = Math.OnRotate(CenterPoint, PatternLine[0].FP1, (ImgDegree));

			// Cell Size ���� ������ �Ѱ���(Image  ȸ��)
			LeftStaPos.x = FLTPoint.x;
			LeftStaPos.y = FLTPoint.y + ImageHeight;
			CenterStaPos.x = FLTPoint.x + (FRTPoint.x - FLTPoint.x) / 2;
			CenterStaPos.y = (FLTPoint.y + FRTPoint.y) / 2 + ImageHeight;

			// GrindMarkPoint Rotate 
			for (int k = 0; k < MAX_GRINDMARK_COUNT; k++) {
				LtPos.x = 0.0f, LtPos.y = 0.0f, RbPos.x = 0.0f, RbPos.y = 0.0f;
				if (AGrindMark[k].Rect.left == 0 || AGrindMark[k].Rect.top == 0 || AGrindMark[k].Rect.right == 0 || AGrindMark[k].Rect.bottom == 0) continue;

				LtPos.x = (float)AGrindMark[k].Rect.left;
				LtPos.y = (float)AGrindMark[k].Rect.top;
				RbPos.x = (float)AGrindMark[k].Rect.right;
				RbPos.y = (float)AGrindMark[k].Rect.bottom;
				LtPos = Math.OnRotate(CenterPoint, LtPos, ImgDegree);
				RbPos = Math.OnRotate(CenterPoint, RbPos, ImgDegree);

				AGrindMark[k].Rect.left = (long)LtPos.x;
				AGrindMark[k].Rect.top = (long)LtPos.y;
				AGrindMark[k].Rect.right = (long)RbPos.x;
				AGrindMark[k].Rect.bottom = (long)RbPos.y;
				if (AGrindMark[k].Point.x != 0.0f && AGrindMark[k].Point.y != 0.0f) {
					AGrindMark[k].Point = Math.OnRotate(CenterPoint, AGrindMark[k].Point, ImgDegree);
				}
			}

			// Pad ROI Rect Rotate 
			for (int k = 0; k <MAX_GRIND_ROI_COUNT; k++) {
				LtPos.x = 0.0f, LtPos.y = 0.0f, RbPos.x = 0.0f, RbPos.y = 0.0f;
				if (PadPolyRect[k].left == 0 || PadPolyRect[k].top == 0 || PadPolyRect[k].right == 0 || PadPolyRect[k].bottom == 0) continue;

				LtPos.x = (float)PadPolyRect[k].left;
				LtPos.y = (float)PadPolyRect[k].top;
				RbPos.x = (float)PadPolyRect[k].right;
				RbPos.y = (float)PadPolyRect[k].bottom;
				LtPos = Math.OnRotate(CenterPoint, LtPos, ImgDegree);
				RbPos = Math.OnRotate(CenterPoint, RbPos, ImgDegree);

				PadPolyRect[k].left = (long)LtPos.x;
				PadPolyRect[k].top = (long)LtPos.y;
				PadPolyRect[k].right = (long)RbPos.x;
				PadPolyRect[k].bottom = (long)RbPos.y;
			}

			// Defect  ROI Rect Rotate 
			for (int k = 0; k < MAX_DEFECT_ROI_COUNT; k++) {
				LtPos.x = 0.0f, LtPos.y = 0.0f, RbPos.x = 0.0f, RbPos.y = 0.0f;
				if (DefectRoiRect[k].left == 0 || DefectRoiRect[k].top == 0 || DefectRoiRect[k].right == 0 || DefectRoiRect[k].bottom == 0) continue;

				LtPos.x = (float)DefectRoiRect[k].left;
				LtPos.y = (float)DefectRoiRect[k].top;
				RbPos.x = (float)DefectRoiRect[k].right;
				RbPos.y = (float)DefectRoiRect[k].bottom;
				LtPos = Math.OnRotate(CenterPoint, LtPos, ImgDegree);
				RbPos = Math.OnRotate(CenterPoint, RbPos, ImgDegree);

				DefectRoiRect[k].left   = (long)LtPos.x;
				DefectRoiRect[k].top    = (long)LtPos.y;
				DefectRoiRect[k].right  = (long)RbPos.x;
				DefectRoiRect[k].bottom = (long)RbPos.y;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
		Index = 0;
		return true;

		// NOTCH RectC : ������ ���
		//------------------------------------------------------------------
	case 1:
		if (IsLeftAllGrind) { //���� ��ü���� 
			EasyImage::Threshold(GImage, &DefectImg, DSideGrindThresHold);
			IAddress = DefectImg.GetImagePtr();

			// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
			if (LeftInEdgeCount != 0) { //�������� DarkDefect 
				FrameLeftDefectCnt = 0;
				FrameLeftDefectCnt = GetPolygonSize(LeftInPoint, LeftDefectPoint, LeftInEdgeCount, 50, 2,DSideGrindThresHold, LeftDefectCount); // Dark Defect 
				LeftDefectCount += FrameLeftDefectCnt;
			}

			//��ü���� �ܰ����� ���� �˻� ������ 
			//DSide Dark Defect 
			if(LeftDefectRange!=0 && LeftDarkThresHold!=0){
				// Image Conversion Binary_Image
				EasyImage::Threshold(GImage    , &DefectImg, LeftDarkThresHold);
//				GImage->Save("D:\\ROTATE_IMG\\AllGrind_STATIC_Defect.bmp");

				// Image Euresys Algorithm Attach 
				DefectROI.Attach(&DefectImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
				IAddress = DefectImg.GetImagePtr();

				//�˻� ROI ���� ���� 
				if (LeftGrindInCnt >0 && LeftGrindInPos >0)  LeftOffset = (int)(LeftGrindInPos / LeftGrindInCnt);

				LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);
				LEP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LB, RB);

				RoiStaX   = (int)((LeftOffset + DefectMargin));
				RoiStaY   = (int)(LSP.y       + DefectMargin );
				RoiWidth  = (int)(LeftDefectRange            );
				RoiHeight = (int)(FrameImgH - RoiStaY        );

				// �˻� ������ 
				DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
				if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
				if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
				if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

				//�˻� ȭ�� ���÷��� ���� 
				DefectRoiRect[Left_Roi].left   = RoiStaX            ;
				DefectRoiRect[Left_Roi].top    = RoiStaY            ;
				DefectRoiRect[Left_Roi].right  = RoiStaX + RoiWidth ;
				DefectRoiRect[Left_Roi].bottom = RoiStaY + RoiHeight;

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
								if(PixelValue < LeftDarkThresHold) PixelCount++;
							}
						}
						// Display ������ �ʿ��� 
						DDefectPoint[DCount].Round     = PixelCount;
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
			}

		}
		else { //���� �κп��� 
			EasyImage::Threshold(GImage, &DefectImg, LeftDarkThresHold);
			IAddress = DefectImg.GetImagePtr();
			//			DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_Left.bmp");

						// SIDE D 
			RoiStaX   = (int)(LSP.x + SideMargin);
			RoiStaY   = (int)(LSP.y);
			RoiWidth  = 200;
			RoiHeight = (int)(FrameImgH - RoiStaY);

			// �˻� ������ 
			DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
			if ((RoiStaX) < 0 || (RoiStaY) < 0 || RoiWidth <= 0 || (RoiHeight) <= 0) return false;
			if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || (RoiHeight) > TopLimitLine) return false;
			if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > ImageHeight)                                   return false;

			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI, CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth, FMinWidth, FMaxWidth, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight, FMinHeight, FMaxHeight, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit, 0, 1, EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

			if (BlobCount != 0) {
				for (int k = 0; k < BlobCount; k++) {
					if (DCount >= MAX_DEFECT_POINT) break;
					DDefectPoint[DCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
					DDefectPoint[DCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DDefectPoint[DCount].Rect.left = (long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + RoiStaX));
					DDefectPoint[DCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2));
					DDefectPoint[DCount].Rect.right = (long)(DDefectPoint[DCount].Rect.left + DDefectPoint[DCount].Width);
					DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);

					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for (int n = DDefectPoint[DCount].Rect.top + 1; n < DDefectPoint[DCount].Rect.bottom - 1; n++) {
						for (int m = DDefectPoint[DCount].Rect.left + 1; m < DDefectPoint[DCount].Rect.right - 1; m++) {
							PixelValue = IElement(m, n);
							if (PixelValue < LeftDarkThresHold) PixelCount++;
						}
					}

					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x = (float)(DDefectPoint[DCount].Rect.left);
					DDefectPoint[DCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ImageHeight);
					DDefectPoint[DCount].Width = (InspectMode) ? DDefectPoint[DCount].Width : DDefectPoint[DCount].Width + SideMargin;

					// Merge ���ؼ� ������ �ʿ��� 
					DDefectPoint[DCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + ImageHeight);
					DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount = 0;
			//  		LogUnit.SetLog(LL"STATIC_Side_D_END_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			// Notch Cell Size Calc
			if (LSP.x != 0.0f && RSP.x != 0.0f) {
				CenterCellSize = Math.GetLengthFrPointToPoint(LSP, RSP);
				TotalCellSizeWidth += CenterCellSize;
				TotalCellSizeCount++;
			}
			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			// CellSize 
			if      (Line[waLeftWall].FP0.x >  Line[waLeftWall].FP1.x) LDistance = FP0.x - (Line[waLeftWall].FP1.x + (Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x) / 2);
			else if (Line[waLeftWall].FP0.x == Line[waLeftWall].FP1.x) LDistance = FP0.x - Line[waLeftWall].FP0.x;
			else                                                       LDistance = FP0.x - (Line[waLeftWall].FP0.x + (Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x) / 2);

			if (CamId==Cam_Top && BrokenThresHold!= 0) {
				RotateBrokenImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &RotateBrokenImg);
				EasyImage::Threshold(GImage, &RotateBrokenImg, BrokenThresHold);

				DefectROI.Attach(&RotateBrokenImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
				IAddress = RotateBrokenImg.GetImagePtr();

				// Side_D Bright Defect ���� �� �з�(Broken,�ݿ��� �ڸ�___�ɹڸ�)
				/////////////////////////////////////////////////////////////////////////////////////////////////
				DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
				if ((RoiStaX) < 0 || (RoiStaY) < 0 || RoiWidth <= 0 || (RoiHeight) <= 0) return false;
				if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || (RoiHeight) > TopLimitLine) return false;
				if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > TopLimitLine)                                  return false;

				// Blob �˻� ���� 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI, CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);
				// Blob ���͸� 
				CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth, FMinWidth, FMaxWidth, EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight, FMinHeight, FMaxHeight, EDoubleThresholdMode_Outside);
				// 			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 1          , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

				if (BlobCount != 0) {
					for (int k = 0; k < BlobCount; k++) {
						if (BCount >= MAX_DEFECT_POINT) break;
						DCrackPoint[DCrackCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
						DCrackPoint[DCrackCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						DCrackPoint[DCrackCount].Rect.left = (long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DCrackPoint[DCrackCount].Width / 2) + RoiStaX));
						DCrackPoint[DCrackCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2));
						DCrackPoint[DCrackCount].Rect.right = (long)(DCrackPoint[DCrackCount].Rect.left + DCrackPoint[DCrackCount].Width);
						DCrackPoint[DCrackCount].Rect.bottom = (long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
						DCrackPoint[DCrackCount].LineAttach = (CodeImageSelection.GetElement(k).GetLeftLimit() < 5) ? 1 : 0; //LineAttach:1 �ݿ��� �ڸ�, 0: �ɹڸ� 

						PixelCount = 0;
						PixelValue = 0;

						// ���� Pixel Data ���� 
						for (int n = DCrackPoint[DCrackCount].Rect.top + 1; n < DCrackPoint[DCrackCount].Rect.bottom - 1; n++) {
							for (int m = DCrackPoint[DCrackCount].Rect.left + 1; m < DCrackPoint[DCrackCount].Rect.right - 1; m++) {
								PixelValue = IElement(m, n);
								if (PixelValue > BrokenThresHold) PixelCount++;
							}
						}

						DCrackPoint[DCrackCount].Round = PixelCount;
						DCrackPoint[DCrackCount].P.x = (float)(DCrackPoint[DCrackCount].Rect.left);
						DCrackPoint[DCrackCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ImageHeight);
						DCrackPoint[DCrackCount].Width = (InspectMode) ? DCrackPoint[DCrackCount].Width : DCrackPoint[DCrackCount].Width + SideMargin;

						// Merge ���ؼ� ������ �ʿ��� 
						DCrackPoint[DCrackCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DCrackPoint[DCrackCount].Height / 2) + ImageHeight);
						DCrackPoint[DCrackCount].Rect.bottom = (long)(DCrackPoint[DCrackCount].Rect.top + DCrackPoint[DCrackCount].Height);
						DCrackCount++;
					}
				}
				BlobCount = 0;
			}
		}


		if (IsRightAllGrind) { //������ ��ü���� 
			EasyImage::Threshold(GImage, &DefectImg, BSideGrindThresHold);
			IAddress = DefectImg.GetImagePtr();

			// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
			if (RightInEdgeCount != 0) { //�������� DarkDefect 
				FrameRightDefectCnt = 0;
				FrameRightDefectCnt = GetPolygonSize(RightInPoint, RightDefectPoint, RightInEdgeCount, 50, 2, BSideGrindThresHold,RIghtDefectCount); // Dark Defect 
				RIghtDefectCount += FrameRightDefectCnt;
			}

			//��ü���� �ܰ����� ���� �˻� ������
			//BSide Dark Defect 
			if(RightDefectRange!=0 && RightDarkThresHold!=0){
				// Image Conversion Binary_Image
				EasyImage::Threshold(GImage    , &DefectImg, RightDarkThresHold);

				// Image Euresys Algorithm Attach 
				DefectROI.Attach(&DefectImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
				IAddress = DefectImg.GetImagePtr();

				//�˻� ROI ���� ���� 
				if (RightGrindInCnt >0 && RightGrindInPos >0)  RightOffset = (int)(RightGrindInPos / RightGrindInCnt);

				RSP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LT, RT);
				REP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LB, RB);

				RoiStaX   = (int)((RightOffset -RightDefectRange));
				RoiStaY   = (int)((RSP.y       +DefectMargin    ));
				RoiWidth  = (int)(RightDefectRange -DefectMargin );
				RoiHeight = (int)(FrameImgH - RoiStaY            );

				// �˻� ������ 
				DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
				if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
				if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
				if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

				//�˻� ȭ�� ���÷��� ���� 
				DefectRoiRect[Right_Roi].left   = RoiStaX            ;
				DefectRoiRect[Right_Roi].top    = RoiStaY            ;
				DefectRoiRect[Right_Roi].right  = RoiStaX + RoiWidth ;
				DefectRoiRect[Right_Roi].bottom = RoiStaY + RoiHeight;

				// Blob �˻� ���� 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI,CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);

				// Blob ���͸� 
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth  , FMaxWidth  , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight , FMaxHeight , EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit       , RoiWidth-1, RoiWidth+1 , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
				if(BlobCount!=0){
					for(int k=0; k<BlobCount; k++){
						if(BCount>=MAX_DEFECT_POINT) break;
						BDefectPoint[BCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
						BDefectPoint[BCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						BDefectPoint[BCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +RoiStaX);
						BDefectPoint[BCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +RoiStaY);
						BDefectPoint[BCount].Rect.right  = (long)(BDefectPoint[BCount].Rect.left+ BDefectPoint[BCount].Width );
						BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);

						PixelCount = 0;
						PixelValue = 0;
						// ���� Pixel Data ���� 
						for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
							for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
								PixelValue = IElement(m,n);
								if(PixelValue < RightDarkThresHold) PixelCount++;
							}
						}
						// Display ������ �ʿ��� 
						BDefectPoint[BCount].Round     = PixelCount;
						BDefectPoint[BCount].P.x       = (float)(BDefectPoint[BCount].Rect.left);
						BDefectPoint[BCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)+RoiStaY+ImageHeight );
						BDefectPoint[BCount].Width     = BDefectPoint[BCount].Width+SideMargin; 

						// Merge ���ؼ� ������ �ʿ��� 
						BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)+RoiStaY+ImageHeight);
						BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
						BCount++;
					}
				}
				BlobCount =0;
			}
		}
		else { //������ �κп���
			EasyImage::Threshold(GImage, &DefectImg, RightDarkThresHold);
			IAddress = DefectImg.GetImagePtr();
			//			DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_Right.bmp");

			RoiStaX = (int)(RSP.x - RoiWidth - SideMargin);
			RoiStaY = (int)(RSP.y);
			RoiWidth = 200;
			RoiHeight = (int)(FrameImgH - RoiStaY);

			DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
			if ((RoiStaX) < 0 || (RoiStaY) < 0 || RoiWidth <= 0 || (RoiHeight) <= 0) return false;
			if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || (RoiHeight) > TopLimitLine) return false;
			if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > ImageHeight)                                   return false;

			// Blob �˻� ������ 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI, CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth, FMinWidth, FMaxWidth, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight, FMinHeight, FMaxHeight, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit, FRightWidth - 1, FRightWidth + 1, EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

			if (BlobCount != 0) {
				for (int k = 0; k < BlobCount; k++) {
					if (BCount >= MAX_DEFECT_POINT) break;
					BDefectPoint[BCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
					BDefectPoint[BCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					BDefectPoint[BCount].Rect.left = (long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width / 2) + RoiStaX));
					BDefectPoint[BCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height / 2));
					BDefectPoint[BCount].Rect.right = (long)(BDefectPoint[BCount].Rect.left + BDefectPoint[BCount].Width);
					BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);

					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for (int n = BDefectPoint[BCount].Rect.top + 1; n < BDefectPoint[BCount].Rect.bottom - 1; n++) {
						for (int m = BDefectPoint[BCount].Rect.left + 1; m < BDefectPoint[BCount].Rect.right - 1; m++) {
							PixelValue = IElement(m, n);
							if (PixelValue < RightDarkThresHold) PixelCount++;
						}
					}

					BDefectPoint[BCount].Round = PixelCount;
					BDefectPoint[BCount].P.x = (float)(BDefectPoint[BCount].Rect.left);
					BDefectPoint[BCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height / 2) + ImageHeight);
					BDefectPoint[BCount].Width = (InspectMode) ? BDefectPoint[BCount].Width : BDefectPoint[BCount].Width + SideMargin;

					// Merge ���ؼ� ������ �ʿ��� 
					BDefectPoint[BCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height / 2) + ImageHeight);
					BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount = 0;
			//  		LogUnit.SetLog(LL"STATIC_Side_B_END_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			// CellSize 
			if (Line[waRightWall].FP0.x > Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x + (Line[waRightWall].FP0.x - Line[waRightWall].FP1.x) / 2) - FP1.x;
			else if (Line[waRightWall].FP0.x == Line[waRightWall].FP1.x) RDistance = Line[waRightWall].FP0.x - FP1.x;
			else                                                      RDistance = (Line[waRightWall].FP0.x + (Line[waRightWall].FP1.x - Line[waRightWall].FP0.x) / 2) - FP1.x;

			if (CamId==Cam_Top&& BrokenThresHold != 0) {
				RotateBrokenImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &RotateBrokenImg);
				EasyImage::Threshold(GImage, &RotateBrokenImg, BrokenThresHold);

				DefectROI.Attach(&RotateBrokenImg);
				CodeImageSelection.SetFeretAngle(0.00f);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true);
				CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
				IAddress = RotateBrokenImg.GetImagePtr();

				// Side_B Bright Defect ���� �� �з�(Broken,�ݿ��� �ڸ�___�ɹڸ�)
				/////////////////////////////////////////////////////////////////////////////////////////////////
				DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
				if ((RoiStaX) < 0 || (RoiStaY) < 0 || RoiWidth <= 0 || (RoiHeight) <= 0) return false;
				if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || (RoiHeight) > TopLimitLine) return false;
				if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > TopLimitLine)                                  return false;

				// Blob �˻� ������ 
				CodeImageSelection.Clear();
				CodeImageEncoder.Encode(DefectROI, CodeImage);
				CodeImageSelection.Clear();
				CodeImageSelection.AddObjects(CodeImage);
				// Blob ���͸� 
				CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth, FMinWidth, FMaxWidth, EDoubleThresholdMode_Outside);
				CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight, FMinHeight, FMaxHeight, EDoubleThresholdMode_Outside);
				// 			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit       ,FRightWidth-1, FRightWidth+1 , EDoubleThresholdMode_Outside);

				BlobCount = CodeImageSelection.GetElementCount();
				CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

				if (BlobCount != 0) {
					for (int k = 0; k < BlobCount; k++) {
						if (BCount >= MAX_DEFECT_POINT) break;
						BCrackPoint[BCrackCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
						BCrackPoint[BCrackCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

						BCrackPoint[BCrackCount].Rect.left = (long)(((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BCrackPoint[BCrackCount].Width / 2) + RoiStaX));
						BCrackPoint[BCrackCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BCrackPoint[BCrackCount].Height / 2));
						BCrackPoint[BCrackCount].Rect.right = (long)(BCrackPoint[BCrackCount].Rect.left + BCrackPoint[BCrackCount].Width);
						BCrackPoint[BCrackCount].Rect.bottom = (long)(BCrackPoint[BCrackCount].Rect.top + BCrackPoint[BCrackCount].Height);
						BCrackPoint[BCrackCount].LineAttach = ((CodeImageSelection.GetElement(k).GetRightLimit() > (FRightWidth - 5)) > 0) ? 1 : 0; //LineAttach:1 �ݿ��� �ڸ�, 0: �ɹڸ� 

						PixelCount = 0;
						PixelValue = 0;

						// ���� Pixel Data ���� 
						for (int n = BCrackPoint[BCrackCount].Rect.top + 1; n < BCrackPoint[BCrackCount].Rect.bottom - 1; n++) {
							for (int m = BCrackPoint[BCrackCount].Rect.left + 1; m < BCrackPoint[BCrackCount].Rect.right - 1; m++) {
								PixelValue = IElement(m, n);
								if (PixelValue > BrokenThresHold) PixelCount++;
							}
						}

						BCrackPoint[BCrackCount].Round = PixelCount;
						BCrackPoint[BCrackCount].P.x = (float)(BCrackPoint[BCrackCount].Rect.left);
						BCrackPoint[BCrackCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BCrackPoint[BCrackCount].Height / 2) + ImageHeight);
						BCrackPoint[BCrackCount].Width = (InspectMode) ? BCrackPoint[BCrackCount].Width : BCrackPoint[BCrackCount].Width + SideMargin;

						// Merge ���ؼ� ������ �ʿ��� 
						BCrackPoint[BCrackCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BCrackPoint[BCrackCount].Height / 2) + ImageHeight);
						BCrackPoint[BCrackCount].Rect.bottom = (long)(BCrackPoint[BCrackCount].Rect.top + BCrackPoint[BCrackCount].Height);
						BCrackCount++;
					}
				}
				BlobCount = 0;
			}
		}
		Index = 0;
		return true;

		// NOTCH RectC : �Ÿ��
		//------------------------------------------------------------------
	case 2:

		if (IsBtmAllGrind) { //��ü���� 
			if (AllBtmInEdgeCount != 0) { //��ü������ TopInEdgeCount     

				if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, NotchSideThresHold);
				else                   EasyImage::Threshold(&RotateImg, &DefectImg, NotchSideThresHold); //DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_HAP.bmp");
				IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 

				BtmDefectCount = GetPolygonSize(AllBtmInData, BtmDefectPoint, AllBtmInEdgeCount, 50, 2, NotchCenThresHold); // Dark Defect 
				// �ڸ� �ҷ� �˻� 
//				if (!IsMarkBtm && BtmInCrackCount > 0 && BrokenThresHold != 0) {
//					IAddress = BrightDefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
//					ACircleFit1Center.x = 0.0f;
//					ACircleFit1Center.y = 0.0f;
//					BtmCrackCount = GetCircleCrackSize(BtmCircleData, BtmCrackPoint, BtmInCrackCount, 30, 0.0f, ACircleFit1Center); //Bright Defect 
//				}

				// �⺻ Dark_Defect������� MarkImage�������� �ѹ��� DarkDefect�˻��Ѵ�
				// DarkDefect���� �˻� �������� �߰��غ���(20190712)
				if (BtmDefectCount == 0 && AllBtmInEdgeCount != 0 && BtmMaskDarkDefectTh != 0) {
					// MaskImage Create Start 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					BinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Conversion Binary
					MaskDarkDefectImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Org
					EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &MaskDarkDefectImg);
					//////////////////////////////////////////////////////////////////////////////////////////////////

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
					HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					// Masking Data Init 
					if (MaskPoint != NULL) {
						delete[]MaskPoint;
					}
					if (MaskPoint2 != NULL) {
						delete[]MaskPoint2;
					}

					// ROI���� ���� �������� �ʿ��� Data 
					MaskStaPos = 0; MaskEndPos = 0;
					MinCnt = 0; MaxCnt = 0;

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve(AllBtmInEdgeCount + 2);
					Poly_Out.reserve(2000);

					Poly_In.push_back(The::IntPoint(FLBPoint.x, FLBPoint.y)); // Clipper
					for (int k = 0; k < AllBtmInEdgeCount; k++) {
						// Clipper use 
						Poly_In.push_back(The::IntPoint(AllBtmInData[k].x, AllBtmInData[k].y));
					}
					Poly_In.push_back(The::IntPoint(FRBPoint.x, FRBPoint.y));

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					// 					ClipOffset.AddPath(Poly_In ,The::jtRound,The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, BtmMaskOffset);
					PolyOutCnt = Poly_Out[0].size();

					AllMaskCount1 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
					MaskPoint = new POINT[AllMaskCount1];

					// �������� ������ �ʱ�ȭ 
					for (int k = 0; k < AllMaskCount1; k++) {
						MaskPoint[k].x = 0;
						MaskPoint[k].y = 0;
					}

					for (int k = 0; k < AllMaskCount1; k++) {
						if (k >= Poly_Out[0].size()) break;
						MaskPoint[k].x = Poly_Out[0][k].x;
						MaskPoint[k].y = Poly_Out[0][k].y;
					}
					HRGN Rgn1 = ::CreatePolygonRgn(MaskPoint, AllMaskCount1, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);


					MinDv = 100000.0f;
					MaxDv = 0.0f;
					// Clipper Data Init 
					Poly_In.clear();
					Poly_Out.clear();
					ClipOffset.Clear();
					Poly_In.reserve(AllBtmInEdgeCount + 2);
					Poly_Out.reserve(2000);

					Poly_In.push_back(The::IntPoint(FLBPoint.x, (AllBtmInData[0].y - BtmMaskDis)));
					for (int k = 0; k < AllBtmInEdgeCount; k++) {
						Poly_In.push_back(The::IntPoint(AllBtmInData[k].x, AllBtmInData[k].y));
					}
					Poly_In.push_back(The::IntPoint(FRBPoint.x, (AllBtmInData[AllBtmInEdgeCount - 1].y - BtmMaskDis)));

					ClipOffset.AddPath(Poly_In, The::jtSquare, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, -BtmMaskDis);
					PolyOutCnt = Poly_Out[0].size();

					AllMaskCount2 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
					MaskPoint2 = new POINT[AllMaskCount2];

					// �������� ������ �ʱ�ȭ 
					for (int k = 0; k < AllMaskCount2; k++) {
						MaskPoint2[k].x = 0;
						MaskPoint2[k].y = 0;
					}

					for (int k = 0; k < PolyOutCnt; k++) {
						if (k >= Poly_Out[0].size()) break;
						MaskPoint2[k].x = Poly_Out[0][k].x;
						MaskPoint2[k].y = Poly_Out[0][k].y;
						if (MaskPoint2[k].x < MinDv) { MinDv = MaskPoint2[k].x; MinCnt = k; }
						if (MaskPoint2[k].x > MaxDv) { MaxDv = MaskPoint2[k].x; MaxCnt = k; }
					}

					// ���� �� �������� ���� ���� ���� ã�´� 
					MaskStaPos = (MaskPoint2[MinCnt].y > MaskPoint2[MaxCnt].y) ? MaskPoint2[MinCnt].y : MaskPoint2[MaxCnt].y;

					HRGN Rgn2 = ::CreatePolygonRgn(MaskPoint2, AllMaskCount2, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);
					//////////////////////////////////////////////////////////////////////////////////////////////////

					if (MaskDarkDefectImg.GetImagePtr()) {
						// Image Conversion Binary_Image
// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\Mask_Org_CUP.bmp");
						EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, BtmMaskDarkDefectTh);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true);
						IAddress = MaskDarkDefectImg.GetImagePtr();

						// �˻� ROI���� 
						RoiStaX   = (int)(FLBPoint.x + SideMargin);
						RoiStaY   = (int)(FLBPoint.y - LBCircleHeight);
						RoiStaY   = (int)(MaskStaPos);
						RoiWidth  = (int)(FRBPoint.x - FLBPoint.x) - 2 * SideMargin;
						RoiHeight = (int)(FLBPoint.y - RoiStaY);

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if ((RoiStaX) <= 0 || (RoiStaY) <= 0 || RoiWidth <= 0 || (RoiHeight) <= 0) return false;
						if ((RoiStaX) >= FrameImgW || (RoiStaY) > BtmLimitLine || RoiWidth >= FrameImgW || (RoiHeight) > BtmLimitLine) return false;
						if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > BtmLimitLine)                                          return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI, CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

						if (BlobCount != 0) {
							for (int k = 0; k < BlobCount; k++) {
								if (BtmDefectCount >= MAX_DEFECT_POINT) break;
								// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + RoiStaX);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight()) / 2) + RoiStaY);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								// 								for(int i=PreBtmCompareCnt; i<AllBtmInEdgeCount; i++){
								for (int i = 0; i < AllBtmInEdgeCount; i++) {
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									IsDefect = false;
									LTDis = Math.GetLength(AllBtmInData[i], DefectLT);
									RTDis = Math.GetLength(AllBtmInData[i], DefectRT);
									LBDis = Math.GetLength(AllBtmInData[i], DefectLB);
									RBDis = Math.GetLength(AllBtmInData[i], DefectRB);
									// 									if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
									if (LTDis <= (MaskDefectMagin + BtmMaskOffset) || RTDis <= (MaskDefectMagin + BtmMaskOffset) || LBDis <= (MaskDefectMagin + BtmMaskOffset) || RBDis <= (MaskDefectMagin + BtmMaskOffset)) {
										IsDefect = true;
										PreTopCompareCnt = i;
										break;
									}
								}

								if (IsDefect) {
									BtmDefectPoint[BtmDefectCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									BtmDefectPoint[BtmDefectCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

									BtmDefectPoint[BtmDefectCount].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BtmDefectPoint[BtmDefectCount].Width / 2) + RoiStaX);
									BtmDefectPoint[BtmDefectCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BtmDefectPoint[BtmDefectCount].Height / 2) + RoiStaY);
									BtmDefectPoint[BtmDefectCount].Rect.right = (long)(BtmDefectPoint[BtmDefectCount].Rect.left + BtmDefectPoint[BtmDefectCount].Width);
									BtmDefectPoint[BtmDefectCount].Rect.bottom = (long)(BtmDefectPoint[BtmDefectCount].Rect.top + BtmDefectPoint[BtmDefectCount].Height);

									PixelCount = 0;
									PixelValue = 0;
									// ���� Pixel Data ���� 
									for (int n = BtmDefectPoint[BtmDefectCount].Rect.top + 1; n < BtmDefectPoint[BtmDefectCount].Rect.bottom - 1; n++) {
										for (int m = BtmDefectPoint[BtmDefectCount].Rect.left + 1; m < BtmDefectPoint[BtmDefectCount].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue < BtmMaskDarkDefectTh) PixelCount++;
										}
									}
									// Display ������ �ʿ��� 
									BtmDefectPoint[BtmDefectCount].Round = PixelCount;
									BtmDefectPoint[BtmDefectCount].P.x = (float)(BtmDefectPoint[BtmDefectCount].Rect.left);
									BtmDefectPoint[BtmDefectCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BtmDefectPoint[BtmDefectCount].Height / 2) + RoiStaY + ImageHeight);
									BtmDefectPoint[BtmDefectCount].Width = BtmDefectPoint[BtmDefectCount].Width + SideMargin;

									// Merge ���ؼ� ������ �ʿ��� 
									BtmDefectPoint[BtmDefectCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BtmDefectPoint[BtmDefectCount].Height / 2) + RoiStaY + ImageHeight);
									BtmDefectPoint[BtmDefectCount].Rect.bottom = (long)(BtmDefectPoint[BtmDefectCount].Rect.top + BtmDefectPoint[BtmDefectCount].Height);
									BtmDefectCount++;
								}
							}
						}
						BlobCount = 0;
					}
				}
			}

			//Rect���� ��ü ������� LeftLine, RightLine �߰� �˻� �ʿ��� 
			if(IsRectBtm){
				////////////////////////////////////////////////////////////////
				if (IsLeftAllGrind) { //���� ��ü���� 
					EasyImage::Threshold(GImage, &DefectImg, DSideGrindThresHold);
					IAddress = DefectImg.GetImagePtr();

					// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					if (LeftInEdgeCount != 0) { //�������� DarkDefect 
						FrameLeftDefectCnt = 0;
						FrameLeftDefectCnt = GetPolygonSize(LeftInPoint, LeftDefectPoint, LeftInEdgeCount, 50, 5,DSideGrindThresHold, LeftDefectCount); // Dark Defect 
						LeftDefectCount += FrameLeftDefectCnt;
					}
				}
				////////////////////////////////////////////////////////////////
				if (IsRightAllGrind) { //������ ��ü���� 
					EasyImage::Threshold(GImage, &DefectImg, BSideGrindThresHold);
					IAddress = DefectImg.GetImagePtr();
					//DefectImg.Save("D:\\ROTATE_IMG\\TH.bmp");
					// Polygon Mode �˻翡���� Dx,Dy,R,�ڸ��ҷ� �̰˻� 
					if (RightInEdgeCount != 0) { //�������� DarkDefect 
						FrameRightDefectCnt = 0;
						FrameRightDefectCnt = GetPolygonSize(RightInPoint, RightDefectPoint, RightInEdgeCount, 50, 5, BSideGrindThresHold,RIghtDefectCount); // Dark Defect 
						RIghtDefectCount += FrameRightDefectCnt;
					}
				}
				////////////////////////////////////////////////////////////////

			    //��ü���� �ܰ����� ���� �˻� ������ 
				//DSide Dark Defect 
				if(LeftDefectRange!=0 && LeftDarkThresHold!=0){
					// Image Conversion Binary_Image
					if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, LeftDarkThresHold);
					else                   EasyImage::Threshold(&RotateImg, &DefectImg, LeftDarkThresHold); 

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&DefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = DefectImg.GetImagePtr();

					//�˻� ROI ���� ���� 
					if (LeftGrindInCnt >0 && LeftGrindInPos >0)  LeftOffset = (int)(LeftGrindInPos / LeftGrindInCnt);
					if (BtmGrindInCnt  >0 && BtmGrindInPos  >0)  BtmOffset  = (int)(BtmGrindInPos  / BtmGrindInCnt );
					LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);

					RoiStaX   = (int)((LeftOffset +DefectMargin));
					RoiStaY   = (int)((LSP.y      +DefectMargin));
					RoiWidth  = (int)(LeftDefectRange           );
					RoiHeight = (int)(BtmOffset - 2*DefectMargin);

					// �˻� ������ 
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

					//�˻� ȭ�� ���÷��� ���� 
					DefectRoiRect[Left_Roi].left   = RoiStaX            ;
					DefectRoiRect[Left_Roi].top    = RoiStaY            ;
					DefectRoiRect[Left_Roi].right  = RoiStaX + RoiWidth ;
					DefectRoiRect[Left_Roi].bottom = RoiStaY + RoiHeight;

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
									if(PixelValue < LeftDarkThresHold) PixelCount++;
								}
							}
							// Display ������ �ʿ��� 
							DDefectPoint[DCount].Round     = PixelCount;
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
				}

				//ASide Dark Defect 
				if(BtmDefectRange!=0 && BtmDarkThresHold!=0){
					// Image Conversion Binary_Image
					if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, BtmDarkThresHold);
					else                   EasyImage::Threshold(&RotateImg, &DefectImg, BtmDarkThresHold); 

//					RotateImg.Save("D:\\ROTATE_IMG\\AllGrind_CUP_Defect_����.bmp"  );
//					DefectImg.Save("D:\\ROTATE_IMG\\AllGrind_CUP_Defect_Binary.bmp");

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&DefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = DefectImg.GetImagePtr();

					//�˻� ROI ���� ���� 
					if (LeftGrindInCnt  >0 && LeftGrindInPos  >0)  LeftOffset  = (int)(LeftGrindInPos  / LeftGrindInCnt );
					if (BtmGrindInCnt   >0 && BtmGrindInPos   >0)  BtmOffset   = (int)(BtmGrindInPos   / BtmGrindInCnt  );
					if (RightGrindInCnt >0 && RightGrindInPos >0)  RightOffset = (int)(RightGrindInPos / RightGrindInCnt);

					RoiStaX   = (int)(LeftOffset + DefectMargin                 );
					RoiStaY   = (int)(BtmOffset - BtmDefectRange - DefectMargin );
					RoiWidth  = (int)((RightOffset - LeftOffset) -2*DefectMargin);
					RoiHeight = (int)(BtmDefectRange                            );

					// �˻� ������ 
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

					//�˻� ȭ�� ���÷��� ���� 
					DefectRoiRect[TopBtm_Roi].left   = RoiStaX            ;
					DefectRoiRect[TopBtm_Roi].top    = RoiStaY            ;
					DefectRoiRect[TopBtm_Roi].right  = RoiStaX + RoiWidth ;
					DefectRoiRect[TopBtm_Roi].bottom = RoiStaY + RoiHeight;

					// Blob �˻� ���� 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					BlobCount = CodeImageSelection.GetElementCount();

					// Blob ���͸� 
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth  , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight , FMaxHeight , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingIntegerFeature(EFeature_BottomLimit      ,RoiHeight-1, RoiHeight+1, EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
					if(BlobCount!=0){
						for(int k=0; k<BlobCount; k++){
							if(DCount>=MAX_DEFECT_POINT) break;
							CDefectPoint[CCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
							CDefectPoint[CCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

							CDefectPoint[CCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CDefectPoint[CCount].Width/2 ) +RoiStaX);
							CDefectPoint[CCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +RoiStaY);
							CDefectPoint[CCount].Rect.right  = (long)(CDefectPoint[CCount].Rect.left+ CDefectPoint[CCount].Width );
							CDefectPoint[CCount].Rect.bottom = (long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);

							PixelCount = 0;
							PixelValue = 0;
							// ���� Pixel Data ���� 
							for(int n=CDefectPoint[CCount].Rect.top+1 ; n<CDefectPoint[CCount].Rect.bottom-1; n++){
								for(int m=CDefectPoint[CCount].Rect.left+1; m<CDefectPoint[CCount].Rect.right-1 ; m++){
									PixelValue = IElement(m,n);
									if(PixelValue < BtmDarkThresHold) PixelCount++;
								}
							}
							// Display ������ �ʿ��� 
							CDefectPoint[CCount].Round     = PixelCount;
							CDefectPoint[CCount].P.x       = (float)(CDefectPoint[CCount].Rect.left);
							CDefectPoint[CCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2)+RoiStaY+ImageHeight );
							CDefectPoint[CCount].Width     = CDefectPoint[CCount].Width+SideMargin; 

							// Merge ���ؼ� ������ �ʿ��� 
							CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2)+RoiStaY+ImageHeight);
							CDefectPoint[CCount].Rect.bottom =(long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
							CCount++;
						}
					}
					BlobCount =0;
				}

				//BSide Dark Defect 
				if(RightDefectRange!=0 && RightDarkThresHold!=0){
					// Image Conversion Binary_Image
					if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, RightDarkThresHold);
					else                   EasyImage::Threshold(&RotateImg, &DefectImg, RightDarkThresHold); 

					// Image Euresys Algorithm Attach 
					DefectROI.Attach(&DefectImg);
					CodeImageSelection.SetFeretAngle(0.00f);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
					CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
					IAddress = DefectImg.GetImagePtr();

					//�˻� ROI ���� ���� 
					if (RightGrindInCnt >0 && RightGrindInPos >0)  RightOffset = (int)(RightGrindInPos / RightGrindInCnt);
					if (BtmGrindInCnt   >0 && BtmGrindInPos   >0)  BtmOffset   = (int)(BtmGrindInPos   / BtmGrindInCnt  );
					RSP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LT, RT);

					RoiStaX   = (int)((RightOffset -RightDefectRange));
					RoiStaY   = (int)((RSP.y       +DefectMargin    ));
					RoiWidth  = (int)(RightDefectRange -DefectMargin );
					RoiHeight = (int)(BtmOffset - 2*DefectMargin      );

					// �˻� ������ 
					DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
					if((RoiStaX) <=0         ||(RoiStaY)< 0        || RoiWidth<=0         || (RoiHeight)< 0        ) return false;
					if((RoiStaX) >=FrameImgW ||(RoiStaY)>FrameImgH || RoiWidth>=FrameImgW || (RoiHeight)>FrameImgH ) return false;
					if((RoiStaX+RoiWidth) >=FrameImgW ||(RoiStaY+RoiHeight)>FrameImgH)                               return false;

					//�˻� ȭ�� ���÷��� ���� 
					DefectRoiRect[Right_Roi].left   = RoiStaX            ;
					DefectRoiRect[Right_Roi].top    = RoiStaY            ;
					DefectRoiRect[Right_Roi].right  = RoiStaX + RoiWidth ;
					DefectRoiRect[Right_Roi].bottom = RoiStaY + RoiHeight;

					// Blob �˻� ���� 
					CodeImageSelection.Clear();
					CodeImageEncoder.Encode(DefectROI,CodeImage);
					CodeImageSelection.Clear();
					CodeImageSelection.AddObjects(CodeImage);

					// Blob ���͸� 
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxWidth ,FMinWidth  , FMaxWidth  , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingFloatFeature  (EFeature_BoundingBoxHeight,FMinHeight , FMaxHeight , EDoubleThresholdMode_Outside);
					CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit       , RoiWidth-1, RoiWidth+1 , EDoubleThresholdMode_Outside);

					BlobCount = CodeImageSelection.GetElementCount();
					CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);
					if(BlobCount!=0){
						for(int k=0; k<BlobCount; k++){
							if(BCount>=MAX_DEFECT_POINT) break;
							BDefectPoint[BCount].Width     =  CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
							BDefectPoint[BCount].Height    =  CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

							BDefectPoint[BCount].Rect.left   = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width/2 ) +RoiStaX);
							BDefectPoint[BCount].Rect.top    = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2) +RoiStaY);
							BDefectPoint[BCount].Rect.right  = (long)(BDefectPoint[BCount].Rect.left+ BDefectPoint[BCount].Width );
							BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);

							PixelCount = 0;
							PixelValue = 0;
							// ���� Pixel Data ���� 
							for(int n=BDefectPoint[BCount].Rect.top+1 ; n<BDefectPoint[BCount].Rect.bottom-1; n++){
								for(int m=BDefectPoint[BCount].Rect.left+1; m<BDefectPoint[BCount].Rect.right-1 ; m++){
									PixelValue = IElement(m,n);
									if(PixelValue < RightDarkThresHold) PixelCount++;
								}
							}
							// Display ������ �ʿ��� 
							BDefectPoint[BCount].Round     = PixelCount;
							BDefectPoint[BCount].P.x       = (float)(BDefectPoint[BCount].Rect.left);
							BDefectPoint[BCount].P.y       = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)+RoiStaY+ImageHeight );
							BDefectPoint[BCount].Width     = BDefectPoint[BCount].Width+SideMargin; 

							// Merge ���ؼ� ������ �ʿ��� 
							BDefectPoint[BCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height/2)+RoiStaY+ImageHeight);
							BDefectPoint[BCount].Rect.bottom =(long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
							BCount++;
						}
					}
					BlobCount =0;
				}
			}
		}
		else { //�κп��� 
			if (IsCornerBtm) {
				if (C3InEdgeCount != 0) {
					CornerCount = 0;
					for (int k = 0; k < C3InEdgeCount; k++) {
						if (C3InPoint[k].y < FLBLPoint.y - 1) continue;
						Dis = Math.GetLengthFrLineToPoint(FLBLInPoint, FLBBInPoint, C3InPoint[k]);
						C3Corner[CornerCount].Dv = Dis;
						C3Corner[CornerCount].P.x = C3InPoint[k].x;
						C3Corner[CornerCount].P.y = C3InPoint[k].y;
						CornerCount++;
					}
					if (LBCornerWidth > (LBCornerHeight * 2)) C3Count = GetSize(C3Corner, C3DefectPoint, CornerCount, 10, CorGrindInThresHold);
					else                                      C3Count = GetSize(C3Corner, C3DefectPoint, CornerCount, 5 , CorGrindInThresHold);
				}

				if (C4InEdgeCount != 0) {
					CornerCount = 0;
					for (int k = 0; k < C4InEdgeCount; k++) {
						if (C4InPoint[k].y < FRBRPoint.y - 1) continue;
						Dis = Math.GetLengthFrLineToPoint(FRBRInPoint, FRBBInPoint, C4InPoint[k]);
						C4Corner[CornerCount].Dv = Dis;
						C4Corner[CornerCount].P.x = C4InPoint[k].x;
						C4Corner[CornerCount].P.y = C4InPoint[k].y;
						CornerCount++;
					}
					if (RBCornerWidth > (RBCornerHeight * 2)) C4Count = GetSize(C4Corner, C4DefectPoint, CornerCount, 10, CorGrindInThresHold);
					else                                      C4Count = GetSize(C4Corner, C4DefectPoint, CornerCount, 5 , CorGrindInThresHold);

				}
			}
			else if (IsCircleBtm) {
				if (C1CirInEdgeCount != 0) {

					if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, NotchSideThresHold);
					else                   EasyImage::Threshold(&RotateImg, &DefectImg, NotchSideThresHold); //DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_HAP.bmp");
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
					C1CirDeftectCnt = GetPolygonSize(C1CirInPoint, C1CirDefectPoint, C1CirInEdgeCount, 50, 5,NotchSideThresHold); // Dark Defect 

//					if (!IsMarkBtm && C1CirInCrackCount > 0 && BrokenThresHold != 0) {
//						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
//						C1CirCrackCnt = GetCircleCrackSize(C1CircleData, C1CirCrackPoint, C1CirInCrackCount, 30, CirFitDiameter1, CircleFit1Center); // Bright Defect 
//					}
				}

				// A1 Circle Masking Image ���� DarkDefect�˻� ������ 
				if (C1CirDeftectCnt == 0 && C1CirInEdgeCount != 0 && BtmMaskDarkDefectTh != 0) {
					// MaskImage Create Start 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					BinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Conversion Binary
					MaskDarkDefectImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Org
					EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &MaskDarkDefectImg);
					//////////////////////////////////////////////////////////////////////////////////////////////////

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
					HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					// Masking Data Init 
					if (LeftMaskPoint != NULL)  delete[]LeftMaskPoint;
					if (LeftMaskPoint2 != NULL)  delete[]LeftMaskPoint2;

					// 
										// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve(C1CirInEdgeCount + 2);
					Poly_Out.reserve(2000);

					Poly_In.push_back(The::IntPoint(FLBPoint.x, FLBPoint.y)); // Clipper
					for (int k = 0; k < C1CirInEdgeCount; k++) {
						Poly_In.push_back(The::IntPoint(C1CirInPoint[k].x, C1CirInPoint[k].y));
					}
					Poly_In.push_back(The::IntPoint(C1CirInPoint[C1CirInEdgeCount - 1].x, FLBPoint.y));

					// 					ClipOffset.AddPath(Poly_In ,The::jtSquare,The::etClosedPolygon);
					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, BtmMaskOffset);
					PolyOutCnt = Poly_Out[0].size();

					AllLeftMaskCount1 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
					LeftMaskPoint = new POINT[AllLeftMaskCount1];

					// �������� ������ �ʱ�ȭ 
					for (int k = 0; k < AllLeftMaskCount1; k++) {
						LeftMaskPoint[k].x = 0;
						LeftMaskPoint[k].y = 0;
					}

					for (int k = 0; k < AllLeftMaskCount1; k++) {
						if (k >= Poly_Out[0].size()) break;
						LeftMaskPoint[k].x = Poly_Out[0][k].x;
						LeftMaskPoint[k].y = Poly_Out[0][k].y;
					}
					HRGN Rgn1 = ::CreatePolygonRgn(LeftMaskPoint, AllLeftMaskCount1, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);


					// Data Convert 
					MinDv = 100000.0f;
					MaxDv = 0.0f;

					Poly_In.clear();
					Poly_Out.clear();
					ClipOffset.Clear();
					Poly_In.reserve(C1CirInEdgeCount + 2);
					Poly_Out.reserve(2000);

					Poly_In.push_back(The::IntPoint(FLBPoint.x, (C1CirInPoint[0].y - BtmMaskDis)));
					for (int k = 0; k < C1CirInEdgeCount; k++) {
						Poly_In.push_back(The::IntPoint(C1CirInPoint[k].x, C1CirInPoint[k].y));
					}
					Poly_In.push_back(The::IntPoint((C1CirInPoint[C1CirInEdgeCount - 1].x), (C1CirInPoint[0].y - BtmMaskDis)));

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, -BtmMaskDis);
					PolyOutCnt = Poly_Out[0].size();

					AllLeftMaskCount2 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
					LeftMaskPoint2 = new POINT[AllLeftMaskCount2];

					// �������� ������ �ʱ�ȭ 
					for (int k = 0; k < AllLeftMaskCount2; k++) {
						LeftMaskPoint2[k].x = 0;
						LeftMaskPoint2[k].y = 0;
					}

					for (int k = 0; k < AllLeftMaskCount2; k++) {
						if (k >= Poly_Out[0].size()) break;
						LeftMaskPoint2[k].x = Poly_Out[0][k].x;
						LeftMaskPoint2[k].y = Poly_Out[0][k].y;
						// 							if(LeftMaskPoint2[k].x<MinDv){MinDv = LeftMaskPoint2[k].x; MinCnt =k;}
						// 							if(LeftMaskPoint2[k].x>MaxDv){MaxDv = LeftMaskPoint2[k].x; MaxCnt =k;}
						if (LeftMaskPoint2[k].x > MaxDv)MaxDv = LeftMaskPoint2[k].x;
					}

					// �а ������,���� 
// 					LeftMaskPoint2[0].x               =MaxDv+20; LeftMaskPoint2[0].y               = C1CirInPoint[0].y;
// 					LeftMaskPoint2[TotalInCount2-1].x =MaxDv+20; LeftMaskPoint2[TotalInCount2-1].y = FLBPoint.y       ; 

					HRGN Rgn2 = ::CreatePolygonRgn(LeftMaskPoint2, AllLeftMaskCount2, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);
					//////////////////////////////////////////////////////////////////////////////////////////////////

					if (MaskDarkDefectImg.GetImagePtr()) {
						// Image Conversion Binary_Image
// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\Mask_Org_CUP_LEFT.bmp");
						EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, BtmMaskDarkDefectTh);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true);
						IAddress = MaskDarkDefectImg.GetImagePtr();

						// �˻� ROI���� 
						RoiStaX = (int)(FLBPoint.x + SideMargin);
						RoiStaY = (int)(C1CirInPoint[0].y);
						RoiWidth = (int)(MaxDv - FLBPoint.x);
						RoiHeight = (int)(C1CirInPoint[C1CirInEdgeCount - 1].y - SideMargin) - RoiStaY;

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if ((RoiStaX) <= 0 || (RoiStaY) <= 0 || RoiWidth <= 0 || (RoiHeight) <= 0) return false;
						if ((RoiStaX) >= FrameImgW || (RoiStaY) > BtmLimitLine || RoiWidth >= FrameImgW || (RoiHeight) > BtmLimitLine) return false;
						if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > BtmLimitLine)                                          return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI, CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

						if (BlobCount != 0) {
							for (int k = 0; k < BlobCount; k++) {
								if (A1CirDeftectCnt >= MAX_DEFECT_POINT) break;
								// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + RoiStaX);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight()) / 2) + RoiStaY);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								// 								for(int i=PreTopCompareCnt; i<C1CirInEdgeCount; i++){
								for (int i = 0; i < C1CirInEdgeCount; i++) {
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									IsDefect = false;
									LTDis = Math.GetLength(C1CirInPoint[i], DefectLT);
									RTDis = Math.GetLength(C1CirInPoint[i], DefectRT);
									LBDis = Math.GetLength(C1CirInPoint[i], DefectLB);
									RBDis = Math.GetLength(C1CirInPoint[i], DefectRB);
									//if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
									if (LTDis <= (MaskDefectMagin + BtmMaskOffset) || RTDis <= (MaskDefectMagin + BtmMaskOffset) || LBDis <= (MaskDefectMagin + BtmMaskOffset) || RBDis <= (MaskDefectMagin + BtmMaskOffset)) {
										IsDefect = true;
										PreTopCompareCnt = i;
										break;
									}
								}

								if (IsDefect) {
									C1CirDefectPoint[C1CirDeftectCnt].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									C1CirDefectPoint[C1CirDeftectCnt].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

									C1CirDefectPoint[C1CirDeftectCnt].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - C1CirDefectPoint[C1CirDeftectCnt].Width / 2) + RoiStaX);
									C1CirDefectPoint[C1CirDeftectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - C1CirDefectPoint[C1CirDeftectCnt].Height / 2) + RoiStaY);
									C1CirDefectPoint[C1CirDeftectCnt].Rect.right = (long)(C1CirDefectPoint[C1CirDeftectCnt].Rect.left + C1CirDefectPoint[C1CirDeftectCnt].Width);
									C1CirDefectPoint[C1CirDeftectCnt].Rect.bottom = (long)(C1CirDefectPoint[C1CirDeftectCnt].Rect.top + C1CirDefectPoint[C1CirDeftectCnt].Height);

									PixelCount = 0;
									PixelValue = 0;
									// ���� Pixel Data ���� 
									for (int n = C1CirDefectPoint[C1CirDeftectCnt].Rect.top + 1; n < C1CirDefectPoint[C1CirDeftectCnt].Rect.bottom - 1; n++) {
										for (int m = C1CirDefectPoint[C1CirDeftectCnt].Rect.left + 1; m < C1CirDefectPoint[C1CirDeftectCnt].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue < BtmMaskDarkDefectTh) PixelCount++;
										}
									}
									// Display ������ �ʿ��� 
									C1CirDefectPoint[C1CirDeftectCnt].Round = PixelCount;
									C1CirDefectPoint[C1CirDeftectCnt].P.x = (float)(A1CirDefectPoint[A1CirDeftectCnt].Rect.left);
									C1CirDefectPoint[C1CirDeftectCnt].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - A1CirDefectPoint[A1CirDeftectCnt].Height / 2) + RoiStaY + ImageHeight);
									C1CirDefectPoint[C1CirDeftectCnt].Width = A1CirDefectPoint[A1CirDeftectCnt].Width + SideMargin;

									// Merge ���ؼ� ������ �ʿ��� 
									C1CirDefectPoint[A1CirDeftectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - A1CirDefectPoint[A1CirDeftectCnt].Height / 2) + RoiStaY + ImageHeight);
									C1CirDefectPoint[A1CirDeftectCnt].Rect.bottom = (long)(A1CirDefectPoint[A1CirDeftectCnt].Rect.top + A1CirDefectPoint[A1CirDeftectCnt].Height);
									C1CirDeftectCnt++;
								}
							}
						}
						BlobCount = 0;
					}
				}


				if (C2CirInEdgeCount != 0) {

					if (ImgDegree == 0.0f) EasyImage::Threshold(GImage    , &DefectImg, NotchSideThresHold);
					else                   EasyImage::Threshold(&RotateImg, &DefectImg, NotchSideThresHold); //DefectImg.Save("D:\\ROTATE_IMG\\DefectImg_Binary_HAP.bmp");
					IAddress = DefectImg.GetImagePtr(); // Pixel ���� ���� ���� 
					C2CirDeftectCnt = GetPolygonSize(C2CirInPoint, C2CirDefectPoint, C2CirInEdgeCount, 50, 5, NotchSideThresHold); // Dark Defect 

//					if (!IsMarkBtm && C2CirInCrackCount > 0 && BrokenThresHold != 0) {
//						IAddress = RotateBrokenImg.GetImagePtr(); // Pixel ���� ���� ���� 
//						C2CirCrackCnt = GetCircleCrackSize(C2CircleData, C2CirCrackPoint, C2CirInCrackCount, 30, CirFitDiameter2, CircleFit2Center); // Bright Defect 
//					}
				}

				// A1 Circle Masking Image ���� DarkDefect�˻� ������ 
				if (C2CirDeftectCnt == 0 && C2CirInEdgeCount != 0 && BtmMaskDarkDefectTh != 0) {
					// MaskImage Create Start 
					//////////////////////////////////////////////////////////////////////////////////////////////////
					BinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Conversion Binary
					MaskDarkDefectImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight()); // Image Masking Org
					EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &MaskDarkDefectImg);
					//////////////////////////////////////////////////////////////////////////////////////////////////

					HBRUSH	fillBrh = ::CreateSolidBrush(RGB(255, 255, 255));
					HDC hdc = Easy::OpenImageGraphicContext(&MaskDarkDefectImg);

					if (RightMaskPoint != NULL)  delete[]RightMaskPoint;
					if (RightMaskPoint2 != NULL)  delete[]RightMaskPoint2;

					// Clipper Use Parameter 
					The::Path  Poly_In;
					The::Paths Poly_Out;
					The::ClipperOffset ClipOffset;
					Poly_In.reserve(C2CirInEdgeCount + 2);
					Poly_Out.reserve(2000);

					// ��ü���� ������ MaskPoint �Ѱ��� 
					Poly_In.push_back(The::IntPoint(C2CirInPoint[0].x, FRBPoint.y));
					for (int k = 0; k < C2CirInEdgeCount; k++) {
						Poly_In.push_back(The::IntPoint(C2CirInPoint[k].x, C2CirInPoint[k].y));
					}
					Poly_In.push_back(The::IntPoint(C2CirInPoint[C2CirInEdgeCount - 1].x, FRBPoint.y));

					// 					ClipOffset.AddPath(Poly_In ,The::jtSquare,The::etClosedPolygon);
					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, BtmMaskOffset);
					PolyOutCnt = Poly_Out[0].size();

					AllRightMaskCount1 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
					RightMaskPoint = new POINT[AllRightMaskCount1];

					// �������� ������ �ʱ�ȭ 
					for (int k = 0; k < AllRightMaskCount1; k++) {
						RightMaskPoint[k].x = 0;
						RightMaskPoint[k].y = 0;
					}

					for (int k = 0; k < AllRightMaskCount1; k++) {
						if (k >= Poly_Out[0].size()) break;
						RightMaskPoint[k].x = Poly_Out[0][k].x;
						RightMaskPoint[k].y = Poly_Out[0][k].y;
					}

					HRGN Rgn1 = ::CreatePolygonRgn(RightMaskPoint, AllMaskCount1, WINDING);
					::FillRgn(hdc, Rgn1, fillBrh);
					::DeleteObject(Rgn1);

					// Data Convert 
					MinDv = 100000.0f;
					MaxDv = 0.0f;
					Poly_In.clear();
					Poly_Out.clear();
					ClipOffset.Clear();
					Poly_In.reserve(C2CirInEdgeCount + 2);
					Poly_Out.reserve(2000);

					Poly_In.push_back(The::IntPoint(C2CirInPoint[0].x, (C2CirInPoint[C2CirInEdgeCount - 1].y - BtmMaskDis)));
					for (int k = 0; k < C2CirInEdgeCount; k++) {
						Poly_In.push_back(The::IntPoint(C2CirInPoint[k].x, C2CirInPoint[k].y));
					}
					Poly_In.push_back(The::IntPoint(C2CirInPoint[C2CirInEdgeCount - 1].x, (C2CirInPoint[C2CirInEdgeCount - 1].y - BtmMaskDis)));

					ClipOffset.AddPath(Poly_In, The::jtRound, The::etClosedPolygon);
					ClipOffset.Execute(Poly_Out, -BtmMaskDis);
					PolyOutCnt = Poly_Out[0].size();

					AllRightMaskCount2 = PolyOutCnt; //�а ���� ClipperOut ���ؼ� ����� ���� ���� 
					RightMaskPoint2 = new POINT[AllRightMaskCount2];

					// �������� ������ �ʱ�ȭ 
					for (int k = 0; k < AllRightMaskCount2; k++) {
						RightMaskPoint2[k].x = 0;
						RightMaskPoint2[k].y = 0;
					}

					for (int k = 0; k < PolyOutCnt; k++) {
						if (k >= Poly_Out[0].size()) break;
						RightMaskPoint2[k].x = Poly_Out[0][k].x;
						RightMaskPoint2[k].y = Poly_Out[0][k].y;
						// 							if(RightMaskPoint2[k].x<MinDv){MinDv = RightMaskPoint2[k].x; MinCnt =k;}
						// 							if(RightMaskPoint2[k].x>MaxDv){MaxDv = RightMaskPoint2[k].x; MaxCnt =k;}
						if (RightMaskPoint2[k].x < MinDv)MinDv = RightMaskPoint2[k].x;
					}
					HRGN Rgn2 = ::CreatePolygonRgn(RightMaskPoint2, AllRightMaskCount2, WINDING);
					::FillRgn(hdc, Rgn2, fillBrh);
					::DeleteObject(Rgn2);
					::DeleteObject(fillBrh);

					Easy::CloseImageGraphicContext(&MaskDarkDefectImg, hdc);
					//////////////////////////////////////////////////////////////////////////////////////////////////

					if (MaskDarkDefectImg.GetImagePtr()) {
						// Image Conversion Binary_Image
// 						MaskDarkDefectImg.Save("D:\\ROTATE_IMG\\Mask_Org_CUP_RIGHT.bmp");
						EasyImage::Threshold(&MaskDarkDefectImg, &BinaryImg, BtmMaskDarkDefectTh);

						// Image Euresys Algorithm Attach 
						DefectROI.Attach(&BinaryImg);
						CodeImageSelection.SetFeretAngle(0.00f);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
						CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true);
						IAddress = MaskDarkDefectImg.GetImagePtr();

						// �˻� ROI���� 
						RoiStaX = (int)(MinDv + SideMargin);
						RoiStaY = (int)(C2CirInPoint[C2CirInEdgeCount - 1].y);
						RoiWidth = (int)(FRBPoint.x - RoiStaX);
						// 						RoiHeight    =(int)(RBCircleHeight)                  ;
						RoiHeight = (int)(C2CirInPoint[0].y - SideMargin) - RoiStaY;

						// �˻� ������ 
						DefectROI.SetPlacement(RoiStaX, RoiStaY, RoiWidth, RoiHeight);
						if ((RoiStaX) <= 0 || (RoiStaY) <= 0 || RoiWidth <= 0 || (RoiHeight) <= 0) return false;
						if ((RoiStaX) >= FrameImgW || (RoiStaY) > BtmLimitLine || RoiWidth >= FrameImgW || (RoiHeight) > BtmLimitLine) return false;
						if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > BtmLimitLine)                                          return false;

						// Blob �˻� ���� 
						CodeImageSelection.Clear();
						CodeImageEncoder.Encode(DefectROI, CodeImage);
						CodeImageSelection.Clear();
						CodeImageSelection.AddObjects(CodeImage);

						BlobCount = CodeImageSelection.GetElementCount();
						CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

						if (BlobCount != 0) {
							for (int k = 0; k < BlobCount; k++) {
								if (C2CirDeftectCnt >= MAX_DEFECT_POINT) break;
								// ���� ���� �������� �پ��ִ� �ҷ����� Ȯ�� 
								DefectLT.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - (CodeImageSelection.GetElement(k).GetBoundingBoxWidth()) / 2) + RoiStaX);
								DefectLT.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - (CodeImageSelection.GetElement(k).GetBoundingBoxHeight()) / 2) + RoiStaY);
								DefectRT.x = (float)(DefectLT.x + CodeImageSelection.GetElement(k).GetBoundingBoxWidth());
								DefectRT.y = (float)(DefectLT.y);
								DefectLB.x = (float)(DefectLT.x);
								DefectLB.y = (float)(DefectLT.y + CodeImageSelection.GetElement(k).GetBoundingBoxHeight());
								DefectRB.x = (float)(DefectRT.x);
								DefectRB.y = (float)(DefectLB.y);

								// 								for(int i=PreTopCompareCnt; i<A2CirInEdgeCount; i++){
								for (int i = 0; i < A2CirInEdgeCount; i++) {
									LTDis = 0.0f, RTDis = 0.0f, LBDis = 0.0f, RBDis = 0.0f;
									IsDefect = false;
									LTDis = Math.GetLength(C2CirInPoint[i], DefectLT);
									RTDis = Math.GetLength(C2CirInPoint[i], DefectRT);
									LBDis = Math.GetLength(C2CirInPoint[i], DefectLB);
									RBDis = Math.GetLength(C2CirInPoint[i], DefectRB);
									//if(LTDis<=MaskDefectMagin || RTDis<=MaskDefectMagin || LBDis<=MaskDefectMagin || RBDis<=MaskDefectMagin){ 
									if (LTDis <= (MaskDefectMagin + BtmMaskOffset) || RTDis <= (MaskDefectMagin + BtmMaskOffset) || LBDis <= (MaskDefectMagin + BtmMaskOffset) || RBDis <= (MaskDefectMagin + BtmMaskOffset)) {
										IsDefect = true;
										PreTopCompareCnt = i;
										break;
									}
								}

								if (IsDefect) {
									C2CirDefectPoint[C2CirDeftectCnt].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
									C2CirDefectPoint[C2CirDeftectCnt].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

									C2CirDefectPoint[C2CirDeftectCnt].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - C2CirDefectPoint[C2CirDeftectCnt].Width / 2) + RoiStaX);
									C2CirDefectPoint[C2CirDeftectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - C2CirDefectPoint[C2CirDeftectCnt].Height / 2) + RoiStaY);
									C2CirDefectPoint[C2CirDeftectCnt].Rect.right = (long)(C2CirDefectPoint[C2CirDeftectCnt].Rect.left + C2CirDefectPoint[C2CirDeftectCnt].Width);
									C2CirDefectPoint[C2CirDeftectCnt].Rect.bottom = (long)(C2CirDefectPoint[C2CirDeftectCnt].Rect.top + C2CirDefectPoint[C2CirDeftectCnt].Height);

									PixelCount = 0;
									PixelValue = 0;
									// ���� Pixel Data ���� 
									for (int n = C2CirDefectPoint[C2CirDeftectCnt].Rect.top + 1; n < C2CirDefectPoint[C2CirDeftectCnt].Rect.bottom - 1; n++) {
										for (int m = C2CirDefectPoint[C2CirDeftectCnt].Rect.left + 1; m < C2CirDefectPoint[C2CirDeftectCnt].Rect.right - 1; m++) {
											PixelValue = IElement(m, n);
											if (PixelValue < BtmMaskDarkDefectTh) PixelCount++;
										}
									}
									// Display ������ �ʿ��� 
									C2CirDefectPoint[C2CirDeftectCnt].Round = PixelCount;
									C2CirDefectPoint[C2CirDeftectCnt].P.x = (float)(C2CirDefectPoint[C2CirDeftectCnt].Rect.left);
									C2CirDefectPoint[C2CirDeftectCnt].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - C2CirDefectPoint[C2CirDeftectCnt].Height / 2) + RoiStaY + ImageHeight);
									C2CirDefectPoint[C2CirDeftectCnt].Width = C2CirDefectPoint[C2CirDeftectCnt].Width + SideMargin;

									// Merge ���ؼ� ������ �ʿ��� 
									C2CirDefectPoint[C2CirDeftectCnt].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - C2CirDefectPoint[C2CirDeftectCnt].Height / 2) + RoiStaY + ImageHeight);
									C2CirDefectPoint[C2CirDeftectCnt].Rect.bottom = (long)(C2CirDefectPoint[C2CirDeftectCnt].Rect.top + C2CirDefectPoint[C2CirDeftectCnt].Height);
									C2CirDeftectCnt++;
								}
							}
						}
						BlobCount = 0;
					}
				}
			}
			// Side C
			// �κп��������� �˻��Ѵ� 
			////////////////////////////////////////////////////////////////////////////////////////////////
			if (fabs(FLBPoint.y - FRBPoint.y) > 3) BTMLine = (FLBPoint.y > FRBPoint.y) ? (int)FRBPoint.y : (int)FLBPoint.y;
			else                                   BTMLine = (FLBPoint.y > FRBPoint.y) ? (int)FLBPoint.y : (int)FRBPoint.y;

			RoiHeight = 50;
			RoiStaX = (int)(BtmEdgePoint[1].x + SideMargin);
			RoiStaY = (int)(BTMLine - RoiHeight);
			RoiWidth = (int)(BtmEdgePoint[2].x - BtmEdgePoint[1].x) - 2 * SideMargin;

			DefectROI.SetPlacement((int)RoiStaX, (int)RoiStaY, RoiWidth, RoiHeight);
			if ((RoiStaX) <= 0 || (RoiStaY) <= 0 || RoiWidth <= 0 || RoiHeight <= 0) return false;
			if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || RoiHeight > TopLimitLine) return false;
			if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > BtmLimitLine) return false;

			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI, CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth, FMinWidth, FMaxWidth, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight, FMinHeight, FMaxHeight, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_BottomLimit, FBtmHeight - 1, FBtmHeight + 1, EDoubleThresholdMode_Outside);


			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_LeftLimit, ESortDirection_Ascending);

			if (BlobCount != 0) {
				for (int k = 0; k < BlobCount; k++) {
					if (CCount >= MAX_DEFECT_POINT) break;
					CDefectPoint[CCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
					CDefectPoint[CCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					CDefectPoint[CCount].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CDefectPoint[CCount].Width / 2) + RoiStaX);
					// 						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +((FLBBPoint.y+FRBBPoint.y)/2-FBtmHeight-1));
					CDefectPoint[CCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height / 2) + RoiStaY);
					CDefectPoint[CCount].Rect.right = (long)(CDefectPoint[CCount].Rect.left + CDefectPoint[CCount].Width);
					CDefectPoint[CCount].Rect.bottom = (long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for (int n = CDefectPoint[CCount].Rect.top + 1; n < CDefectPoint[CCount].Rect.bottom - 1; n++) {
						for (int m = CDefectPoint[CCount].Rect.left + 1; m < CDefectPoint[CCount].Rect.right - 1; m++) {
							PixelValue = IElement(m, n);
							if (PixelValue < BtmDarkThresHold) PixelCount++;
						}
					}
					CDefectPoint[CCount].Round = PixelCount;
					CDefectPoint[CCount].P.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - CDefectPoint[CCount].Width / 2) + RoiStaX);
					CDefectPoint[CCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height / 2) + RoiStaY);

					if (ImgDegree != 0.0f) {
						CDefectPoint[CCount].P = Math.OnRotate(CenterPoint, CDefectPoint[CCount].P, ImgDegree);
					}
					CDefectPoint[CCount].P.y = (float)(CDefectPoint[CCount].P.y + ImageHeight);

					// Merge ���ؼ� ������ �ʿ��� 
					// 						CDefectPoint[CCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height/2) +((FLBBPoint.y+FRBBPoint.y)/2-FBtmHeight-1)+ImageHeight);
					CDefectPoint[CCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - CDefectPoint[CCount].Height / 2) + RoiStaY + ImageHeight);
					CDefectPoint[CCount].Rect.bottom = (long)(CDefectPoint[CCount].Rect.top + CDefectPoint[CCount].Height);
					CCount++;
				}
			}
			BlobCount = 0;
		}
		// ��ü���� vs �κп��� ������� �������� BSide,DSide �˻縦 �����Ѵ� 
		// Side B
		////////////////////////////////////////////////////////////////////////////////////////////////
		if(BtmEdgePoint[3].x!=0 && BtmEdgePoint[3].y!=0){
			EasyImage::Threshold(GImage, &DefectImg, RightDarkThresHold);
			IAddress = DefectImg.GetImagePtr();

			RSP = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, LT, RT);

			RoiWidth = 200;
			RoiStaX = (int)(BtmEdgePoint[3].x - RoiWidth - SideMargin);
			RoiStaY = (int)(RSP.y + BtmMargin);
			RoiHeight = (int)(BtmEdgePoint[3].y - RoiStaY);

			DefectROI.SetPlacement((int)RoiStaX, (int)RoiStaY, RoiWidth, RoiHeight);
			if ((RoiStaX) <= 0 || (RoiStaY) <= 0 || RoiWidth <= 0 || RoiHeight <= 0) return false;
			if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || RoiHeight > TopLimitLine) return false;
			if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > BtmLimitLine) return false;

			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI, CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth, FMinWidth, FMaxWidth, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight, FMinHeight, FMaxHeight, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_RightLimit, FRightWidth - 1, FRightWidth + 1, EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

			if (BlobCount != 0) {
				for (int k = 0; k < BlobCount; k++) {
					if (BCount >= MAX_DEFECT_POINT) break;
					BDefectPoint[BCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
					BDefectPoint[BCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					BDefectPoint[BCount].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width / 2) + RoiStaX);
					BDefectPoint[BCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height / 2) + RoiStaY);
					BDefectPoint[BCount].Rect.right = (long)(BDefectPoint[BCount].Rect.left + BDefectPoint[BCount].Width);
					BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for (int n = BDefectPoint[BCount].Rect.top + 1; n < BDefectPoint[BCount].Rect.bottom - 1; n++) {
						for (int m = BDefectPoint[BCount].Rect.left + 1; m < BDefectPoint[BCount].Rect.right - 1; m++) {
							PixelValue = IElement(m, n);
							if (PixelValue < RightDarkThresHold) PixelCount++;
						}
					}
					BDefectPoint[BCount].Round = PixelCount;
					BDefectPoint[BCount].P.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - BDefectPoint[BCount].Width / 2) + (RoiStaX));
					BDefectPoint[BCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height / 2) + (RoiStaY)+ImageHeight);
					BDefectPoint[BCount].Width = (InspectMode) ? BDefectPoint[BCount].Width : BDefectPoint[BCount].Width + SideMargin;

					// Merge ���ؼ� ������ �ʿ��� 
					BDefectPoint[BCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - BDefectPoint[BCount].Height / 2) + (RoiStaY)+ImageHeight);
					BDefectPoint[BCount].Rect.bottom = (long)(BDefectPoint[BCount].Rect.top + BDefectPoint[BCount].Height);
					BCount++;
				}
			}
			BlobCount = 0;
			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if      (Line[waRightWall].FP0.x >  Line[waRightWall].FP1.x) RDistance = (Line[waRightWall].FP1.x + (Line[waRightWall].FP0.x - Line[waRightWall].FP1.x) / 2) - FP1.x;
			else if (Line[waRightWall].FP0.x == Line[waRightWall].FP1.x) RDistance = Line[waRightWall].FP0.x - FP1.x;
			else                                                         RDistance = (Line[waRightWall].FP0.x + (Line[waRightWall].FP1.x - Line[waRightWall].FP0.x) / 2) - FP1.x;
		}

		// Side D 
		// �˻� ROI���� 
		////////////////////////////////////////////////////////////////////////////////////////////////
		if (BtmEdgePoint[0].x != 0 && BtmEdgePoint[0].y != 0) {
			EasyImage::Threshold(GImage, &DefectImg, LeftDarkThresHold);
			IAddress = DefectImg.GetImagePtr();

			LSP = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, LT, RT);

			RoiStaX = (int)(BtmEdgePoint[0].x + SideMargin);
			RoiStaY = (int)(LSP.y + BtmMargin);
			RoiWidth = 200;
			RoiHeight = (int)(BtmEdgePoint[0].y - RoiStaY);

			//LogUnit.SetLog(LL"CUP_Side_D_Start_CamId: %d, ImageHeight: %d ",CamId,ImageHeight);     
			DefectROI.SetPlacement((int)RoiStaX, (int)RoiStaY, RoiWidth, RoiHeight);
			if ((RoiStaX) <= 0 || (RoiStaY) <= 0 || RoiWidth <= 0 || RoiHeight <= 0) return false;
			if ((RoiStaX) >= FrameImgW || (RoiStaY) > TopLimitLine || RoiWidth >= FrameImgW || RoiHeight > TopLimitLine) return false;
			if ((RoiStaX + RoiWidth) >= FrameImgW || (RoiStaY + RoiHeight) > BtmLimitLine) return false;

			// Blob �˻� ���� 
			CodeImageSelection.Clear();
			CodeImageEncoder.Encode(DefectROI, CodeImage);
			CodeImageSelection.Clear();
			CodeImageSelection.AddObjects(CodeImage);
			// Blob ���͸� 
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth, FMinWidth, FMaxWidth, EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight, FMinHeight, FMaxHeight, EDoubleThresholdMode_Outside);
			// 				CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 3          , EDoubleThresholdMode_Outside);
			CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit, 0, 1, EDoubleThresholdMode_Outside);

			BlobCount = CodeImageSelection.GetElementCount();
			CodeImageSelection.Sort(EFeature_TopLimit, ESortDirection_Ascending);

			if (BlobCount != 0) {
				for (int k = 0; k < BlobCount; k++) {
					if (DCount >= MAX_DEFECT_POINT) break;
					DDefectPoint[DCount].Width = CodeImageSelection.GetElement(k).GetBoundingBoxWidth();
					DDefectPoint[DCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

					DDefectPoint[DCount].Rect.left = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + RoiStaX);
					DDefectPoint[DCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + RoiStaY);
					DDefectPoint[DCount].Rect.right = (long)(DDefectPoint[DCount].Rect.left + DDefectPoint[DCount].Width);
					DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					PixelCount = 0;
					PixelValue = 0;

					// ���� Pixel Data ���� 
					for (int n = DDefectPoint[DCount].Rect.top + 1; n < DDefectPoint[DCount].Rect.bottom - 1; n++) {
						for (int m = DDefectPoint[DCount].Rect.left + 1; m < DDefectPoint[DCount].Rect.right - 1; m++) {
							PixelValue = IElement(m, n);
							if (PixelValue < LeftDarkThresHold) PixelCount++;
						}
					}

					DDefectPoint[DCount].Round = PixelCount;
					DDefectPoint[DCount].P.x = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - DDefectPoint[DCount].Width / 2) + RoiStaX);
					DDefectPoint[DCount].P.y = (float)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + RoiStaY + ImageHeight);
					DDefectPoint[DCount].Width = (InspectMode) ? DDefectPoint[DCount].Width : DDefectPoint[DCount].Width + SideMargin;

					// Merge ���ؼ� ������ �ʿ��� 
					DDefectPoint[DCount].Rect.top = (long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - DDefectPoint[DCount].Height / 2) + RoiStaY + ImageHeight);
					DDefectPoint[DCount].Rect.bottom = (long)(DDefectPoint[DCount].Rect.top + DDefectPoint[DCount].Height);
					DCount++;
				}
			}
			BlobCount = 0;
			// �ܰ����ο��� Mark������ �Ÿ� ���� 
			if      (Line[waLeftWall].FP0.x >  Line[waLeftWall].FP1.x) LDistance = FP0.x - (Line[waLeftWall].FP1.x + (Line[waLeftWall].FP0.x - Line[waLeftWall].FP1.x) / 2);
			else if (Line[waLeftWall].FP0.x == Line[waLeftWall].FP1.x) LDistance = FP0.x - Line[waLeftWall].FP0.x;
			else                                                       LDistance = FP0.x - (Line[waLeftWall].FP0.x + (Line[waLeftWall].FP1.x - Line[waLeftWall].FP0.x) / 2);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////
		// ���� �̹��� ȸ���Ͽ� �˻� ���� ȭ�鿡 ���÷��� �������� ��ǥ���� �ٽ� ����Ѵ� 
		////////////////////////////////////////////////////////////////////////////////////////////////
		// Cell Size ���� ������ �Ѱ���(Image ȸ�� �������)
		if (FLBPoint.x != 0.0f && FLBPoint.y != 0.0f && FRBPoint.x != 0.0f && FRBPoint.y != 0.0f) {
			LeftEndPos.x = FLBPoint.x;
			LeftEndPos.y = FLBPoint.y + ImageHeight;

			CenterEndPos.x = BtmOutLine[2].FP0.x + (BtmOutLine[2].FP0.x - BtmOutLine[2].FP1.x) / 2;
			CenterEndPos.y = (BtmOutLine[2].FP0.y + BtmOutLine[2].FP1.y) / 2 + ImageHeight;
		}
		if (TotalCellSizeWidth != 0.0f && TotalCellSizeCount != 0) CenCellSizeWidth = TotalCellSizeWidth / TotalCellSizeCount;

		if (ImgDegree != 0.0f) {
			if (IsBtmAllGrind) {
				// EdgeInPoint 
				if (AllBtmInEdgeCount != 0) {
					for (int k = 0; k < AllBtmInEdgeCount; k++) {
						if (AllBtmInData[k].x == 0.0f || AllBtmInData[k].y == 0.0f) continue;
						AllBtmInData[k] = Math.OnRotate(CenterPoint, AllBtmInData[k], ImgDegree);
					}
				}
				// EdgeOutPoint 
				if (AllBtmOutEdgeCount != 0) {
					for (int k = 0; k < AllBtmOutEdgeCount; k++) {
						if (AllBtmOutData[k].x == 0.0f || AllBtmOutData[k].y == 0.0f) continue;
						AllBtmOutData[k] = Math.OnRotate(CenterPoint, AllBtmOutData[k], ImgDegree);
					}
				}

				// 20190720
//				if (AllMaskCount1 != 0) {
//					for (int k = 0; k < AllMaskCount1; k++) {
//						MaskPoint[k].x = Math.OnRotate((int)CenterPoint.x, (int)CenterPoint.y, (int)MaskPoint[k].x, (int)MaskPoint[k].y, ImgDegree).x;
//						MaskPoint[k].y = Math.OnRotate((int)CenterPoint.x, (int)CenterPoint.y, (int)MaskPoint[k].x, (int)MaskPoint[k].y, ImgDegree).y;
//					}
//				}
//
//				if (AllMaskCount2 != 0) {
//					for (int k = 0; k < AllMaskCount2; k++) {
//						MaskPoint2[k].x = Math.OnRotate((int)CenterPoint.x, (int)CenterPoint.y, (int)MaskPoint2[k].x, (int)MaskPoint2[k].y, ImgDegree).x;
//						MaskPoint2[k].y = Math.OnRotate((int)CenterPoint.x, (int)CenterPoint.y, (int)MaskPoint2[k].x, (int)MaskPoint2[k].y, ImgDegree).y;
//					}
//				}

				//Dark_Defect 
				if (BtmDefectCount != 0) {
					for (int k = 0; k < BtmDefectCount; k++) {
						BtmDefectPoint[k].P.y = BtmDefectPoint[k].P.y - ImageHeight;
						if (BtmDefectPoint[k].P.y > 0) {
							BtmDefectPoint[k].P = Math.OnRotate(CenterPoint, BtmDefectPoint[k].P, (ImgDegree));
							BtmDefectPoint[k].P.y = BtmDefectPoint[k].P.y + ImageHeight;
						}
						else {
							BtmDefectPoint[k].P.y = BtmDefectPoint[k].P.y + ImageHeight;
						}
					}
				}
				//Bright_Defect 
				if (BtmCrackCount != 0) {
					for (int k = 0; k < BtmCrackCount; k++) {
						BtmCrackPoint[k].P.y = BtmCrackPoint[k].P.y - ImageHeight;
						if (BtmCrackPoint[k].P.y > 0) {
							BtmCrackPoint[k].P = Math.OnRotate(CenterPoint, BtmCrackPoint[k].P, (ImgDegree));
							BtmCrackPoint[k].P.y = BtmCrackPoint[k].P.y + ImageHeight;
						}
						else {
							BtmCrackPoint[k].P.y = BtmCrackPoint[k].P.y + ImageHeight;
						}
					}
				}

//				//ASide Dark Defect 
//				if(CCount!=0){
//					for (int k = 0; k < CCount; k++) {
//						CDefectPoint[k].P.y = CDefectPoint[k].P.y - ImageHeight;
//						if (CDefectPoint[k].P.y > 0) {
//							CDefectPoint[k].P = Math.OnRotate(CenterPoint, CDefectPoint[k].P, (ImgDegree));
//							CDefectPoint[k].P.y = CDefectPoint[k].P.y + ImageHeight;
//						}
//						else {
//							CDefectPoint[k].P.y = CDefectPoint[k].P.y + ImageHeight;
//						}
//					}
//				}
//
//				//BSide Dark Defect 
//				if(BCount!=0){
//					for (int k = 0; k < BCount; k++) {
//						BDefectPoint[k].P.y = BDefectPoint[k].P.y - ImageHeight;
//						if (BDefectPoint[k].P.y > 0) {
//							BDefectPoint[k].P = Math.OnRotate(CenterPoint, BDefectPoint[k].P, (ImgDegree));
//							BDefectPoint[k].P.y = BDefectPoint[k].P.y + ImageHeight;
//						}
//						else {
//							BDefectPoint[k].P.y = BDefectPoint[k].P.y + ImageHeight;
//						}
//					}
//				}
//
//				//BSide Dark Defect 
//				if(DCount!=0){
//					for (int k = 0; k < DCount; k++) {
//						DDefectPoint[k].P.y = DDefectPoint[k].P.y - ImageHeight;
//						if (DDefectPoint[k].P.y > 0) {
//							DDefectPoint[k].P = Math.OnRotate(CenterPoint, DDefectPoint[k].P, (ImgDegree));
//							DDefectPoint[k].P.y = DDefectPoint[k].P.y + ImageHeight;
//						}
//						else {
//							DDefectPoint[k].P.y = DDefectPoint[k].P.y + ImageHeight;
//						}
//					}
//				}
			}
			else {
				// BtmLine 
				Line[3].FP0 = Math.OnRotate(CenterPoint, Line[3].FP0, (ImgDegree));
				Line[3].FP1 = Math.OnRotate(CenterPoint, Line[3].FP1, (ImgDegree));
				if (IsCornerBtm) {
					FLBBPoint    = Math.OnRotate(CenterPoint, FLBBPoint, (ImgDegree));
					FLBLPoint    = Math.OnRotate(CenterPoint, FLBLPoint, (ImgDegree));
					FLBBInPoint  = Math.OnRotate(CenterPoint, FLBBInPoint, (ImgDegree));
					FLBLInPoint  = Math.OnRotate(CenterPoint, FLBLInPoint, (ImgDegree));
					FLBBMeaPoint = Math.OnRotate(CenterPoint, FLBBMeaPoint, (ImgDegree));
					FLBLMeaPoint = Math.OnRotate(CenterPoint, FLBLMeaPoint, (ImgDegree));

					FRBBPoint = Math.OnRotate(CenterPoint, FRBBPoint, (ImgDegree));
					FRBRPoint = Math.OnRotate(CenterPoint, FRBRPoint, (ImgDegree));
					FRBBInPoint = Math.OnRotate(CenterPoint, FRBBInPoint, (ImgDegree));
					FRBRInPoint = Math.OnRotate(CenterPoint, FRBRInPoint, (ImgDegree));
					FRBBMeaPoint = Math.OnRotate(CenterPoint, FRBBMeaPoint, (ImgDegree));
					FRBRMeaPoint = Math.OnRotate(CenterPoint, FRBRMeaPoint, (ImgDegree));

					// Corner Defect ��ǥ�� ���� �����Ͽ� �����ؾ��� 
					if (C3Count != 0) {
						for (int k = 0; k < C3Count; k++) {
							// Fram���� Y��ǥ������ �����Ѵ� 
							C3DefectPoint[k].P.y = C3DefectPoint[k].P.y - ImageHeight;
							if (C3DefectPoint[k].P.y > 0) {
								C3DefectPoint[k].P = Math.OnRotate(CenterPoint, C3DefectPoint[k].P, (ImgDegree));
								C3DefectPoint[k].P.y = C3DefectPoint[k].P.y + ImageHeight;
							}
							else {
								C3DefectPoint[k].P.y = C3DefectPoint[k].P.y + ImageHeight;
							}
						}
					}

					if (C4Count != 0) {
						for (int k = 0; k < C4Count; k++) {
							// Fram���� Y��ǥ������ �����Ѵ� 
							C4DefectPoint[k].P.y = C4DefectPoint[k].P.y - ImageHeight;
							if (C4DefectPoint[k].P.y > 0) {
								C4DefectPoint[k].P = Math.OnRotate(CenterPoint, C4DefectPoint[k].P, (ImgDegree));
								C4DefectPoint[k].P.y = C4DefectPoint[k].P.y + ImageHeight;
							}
							else {
								C4DefectPoint[k].P.y = C4DefectPoint[k].P.y + ImageHeight;
							}
						}
					}
				}
				else if (IsCircleBtm) {
					// Dark Defect Data 
					////////////////////////////////////////////////////////////////////////////////////////////////
					if (C1CirDeftectCnt != 0) {
						for (int k = 0; k < C1CirDeftectCnt; k++) {
							C1CirDefectPoint[k].P.y = C1CirDefectPoint[k].P.y - ImageHeight;
							if (C1CirDefectPoint[k].P.y > 0) {
								C1CirDefectPoint[k].P = Math.OnRotate(CenterPoint, C1CirDefectPoint[k].P, (ImgDegree));
								C1CirDefectPoint[k].P.y = C1CirDefectPoint[k].P.y + ImageHeight;
							}
							else {
								C1CirDefectPoint[k].P.y = C1CirDefectPoint[k].P.y + ImageHeight;
							}
						}
					}

					if (C2CirDeftectCnt != 0) {
						for (int k = 0; k < C2CirDeftectCnt; k++) {
							C2CirDefectPoint[k].P.y = C2CirDefectPoint[k].P.y - ImageHeight;
							if (C2CirDefectPoint[k].P.y > 0) {
								C2CirDefectPoint[k].P = Math.OnRotate(CenterPoint, C2CirDefectPoint[k].P, (ImgDegree));
								C2CirDefectPoint[k].P.y = C2CirDefectPoint[k].P.y + ImageHeight;
							}
							else {
								C2CirDefectPoint[k].P.y = C2CirDefectPoint[k].P.y + ImageHeight;
							}
						}
					}
					// Bright Defect Data e
					////////////////////////////////////////////////////////////////////////////////////////////////
					if (C1CirCrackCnt != 0) {
						for (int k = 0; k < C1CirCrackCnt; k++) {
							C1CirCrackPoint[k].P.y = C1CirCrackPoint[k].P.y - ImageHeight;
							if (C1CirCrackPoint[k].P.y > 0) {
								C1CirCrackPoint[k].P = Math.OnRotate(CenterPoint, C1CirCrackPoint[k].P, (ImgDegree));
								C1CirCrackPoint[k].P.y = C1CirCrackPoint[k].P.y + ImageHeight;
							}
							else {
								C1CirCrackPoint[k].P.y = C1CirCrackPoint[k].P.y + ImageHeight;
							}
						}
					}

					if (C2CirCrackCnt != 0) {
						for (int k = 0; k < C2CirCrackCnt; k++) {
							C2CirCrackPoint[k].P.y = C2CirCrackPoint[k].P.y - ImageHeight;
							if (C2CirCrackPoint[k].P.y > 0) {
								C2CirCrackPoint[k].P = Math.OnRotate(CenterPoint, C2CirCrackPoint[k].P, (ImgDegree));
								C2CirCrackPoint[k].P.y = C2CirCrackPoint[k].P.y + ImageHeight;
							}
							else {
								C2CirCrackPoint[k].P.y = C2CirCrackPoint[k].P.y + ImageHeight;
							}
						}
					}
					// C2 Circle Edge Point Rotate 
					if (C1CirInEdgeCount != 0) {
						for (int k = 0; k < C1CirInEdgeCount; k++) {
							if (C1CirInPoint[k].x == 0.0f || C1CirInPoint[k].y == 0.0f) continue;
							C1CirInPoint[k] = Math.OnRotate(CenterPoint, C1CirInPoint[k], ImgDegree);
						}
					}
					if (C1CirOutEdgeCount != 0) {
						for (int k = 0; k < C1CirOutEdgeCount; k++) {
							if (C1CirOutPoint[k].x == 0.0f || C1CirOutPoint[k].y == 0.0f) continue;
							C1CirOutPoint[k] = Math.OnRotate(CenterPoint, C1CirOutPoint[k], ImgDegree);
						}
					}
					if (CircleFit1Center.x != 0.0f && CircleFit1Center.y != 0.0f) CircleFit1Center = Math.OnRotate(CenterPoint, CircleFit1Center, (ImgDegree)); // FAVION

					// C2 Circle Edge Point Rotate 
					if (C2CirInEdgeCount != 0) {
						for (int k = 0; k < C2CirInEdgeCount; k++) {
							if (C2CirInPoint[k].x == 0.0f || C2CirInPoint[k].y == 0.0f) continue;
							C2CirInPoint[k] = Math.OnRotate(CenterPoint, C2CirInPoint[k], ImgDegree);
						}
					}
					if (C2CirOutEdgeCount != 0) {
						for (int k = 0; k < C2CirOutEdgeCount; k++) {
							if (C2CirOutPoint[k].x == 0.0f || C2CirOutPoint[k].y == 0.0f) continue;
							C2CirOutPoint[k] = Math.OnRotate(CenterPoint, C2CirOutPoint[k], ImgDegree);
						}
					}
					if (CircleFit2Center.x != 0.0f && CircleFit2Center.y != 0.0f) CircleFit2Center = Math.OnRotate(CenterPoint, CircleFit2Center, (ImgDegree));
				}
			}
			// Shape ������� ���뺯�� 
			FLBPoint = Math.OnRotate(CenterPoint, FLBPoint, (ImgDegree));
			FRBPoint = Math.OnRotate(CenterPoint, FRBPoint, (ImgDegree));

			// LeftOutLine
			LeftOutLine[0].FP0 = Math.OnRotate(CenterPoint, LeftOutLine[0].FP0, (ImgDegree));
			LeftOutLine[0].FP1 = Math.OnRotate(CenterPoint, LeftOutLine[0].FP1, (ImgDegree));

			// RightOutLine
			RightOutLine[0].FP0 = Math.OnRotate(CenterPoint, RightOutLine[0].FP0, (ImgDegree));
			RightOutLine[0].FP1 = Math.OnRotate(CenterPoint, RightOutLine[0].FP1, (ImgDegree));

			// Cell Size ���� ������ �Ѱ���(Image ȸ�� ����)
			if (FLBPoint.x != 0.0f && FLBPoint.y != 0.0f && FRBPoint.x != 0.0f && FRBPoint.y != 0.0f) {
				LeftEndPos.x = FLBPoint.x;
				LeftEndPos.y = FLBPoint.y + ImageHeight;
				CenterEndPos.x = BtmOutLine[2].FP0.x + (BtmOutLine[2].FP0.x - BtmOutLine[2].FP1.x) / 2;
				CenterEndPos.y = (BtmOutLine[2].FP0.y + BtmOutLine[2].FP1.y) / 2 + ImageHeight;
			}

			if (CamId==Cam_Top) {
				for (int k = 0; k < MAX_GRINDMARK_COUNT; k++) {
					LtPos.x = 0.0f, LtPos.y = 0.0f, RbPos.x = 0.0f, RbPos.y = 0.0f;
					if (CGrindMark[k].Rect.left == 0 || CGrindMark[k].Rect.top == 0 || CGrindMark[k].Rect.right == 0 || CGrindMark[k].Rect.bottom == 0) continue;

					LtPos.x = (float)CGrindMark[k].Rect.left;
					LtPos.y = (float)CGrindMark[k].Rect.top;
					RbPos.x = (float)CGrindMark[k].Rect.right;
					RbPos.y = (float)CGrindMark[k].Rect.bottom;
					LtPos = Math.OnRotate(CenterPoint, LtPos, ImgDegree);
					RbPos = Math.OnRotate(CenterPoint, RbPos, ImgDegree);

					CGrindMark[k].Rect.left = (long)LtPos.x;
					CGrindMark[k].Rect.top = (long)LtPos.y;
					CGrindMark[k].Rect.right = (long)RbPos.x;
					CGrindMark[k].Rect.bottom = (long)RbPos.y;
					if (CGrindMark[k].Point.x != 0.0f && CGrindMark[k].Point.y != 0.0f) {
						CGrindMark[k].Point = Math.OnRotate(CenterPoint, CGrindMark[k].Point, ImgDegree);
					}
				}
			}

			//Notch ROI Rect Rotate
			for (int k = 0; k < MAX_CUT_GRIND_ROI; k++) {
				LtPos.x = 0.0f, LtPos.y = 0.0f, RbPos.x = 0.0f, RbPos.y = 0.0f;
				if (NotchPolyRect[k].left == 0 || NotchPolyRect[k].top == 0 || NotchPolyRect[k].right == 0 || NotchPolyRect[k].bottom == 0) continue;

				LtPos.x = (float)NotchPolyRect[k].left;
				LtPos.y = (float)NotchPolyRect[k].top;
				RbPos.x = (float)NotchPolyRect[k].right;
				RbPos.y = (float)NotchPolyRect[k].bottom;
				LtPos = Math.OnRotate(CenterPoint, LtPos, ImgDegree);
				RbPos = Math.OnRotate(CenterPoint, RbPos, ImgDegree);

				NotchPolyRect[k].left = (long)LtPos.x;
				NotchPolyRect[k].top = (long)LtPos.y;
				NotchPolyRect[k].right = (long)RbPos.x;
				NotchPolyRect[k].bottom = (long)RbPos.y;
			}

			// Defect  ROI Rect Rotate 
			for (int k = 0; k < MAX_DEFECT_ROI_COUNT; k++) {
				LtPos.x = 0.0f, LtPos.y = 0.0f, RbPos.x = 0.0f, RbPos.y = 0.0f;
				if (DefectRoiRect[k].left == 0 || DefectRoiRect[k].top == 0 || DefectRoiRect[k].right == 0 || DefectRoiRect[k].bottom == 0) continue;

				LtPos.x = (float)DefectRoiRect[k].left;
				LtPos.y = (float)DefectRoiRect[k].top;
				RbPos.x = (float)DefectRoiRect[k].right;
				RbPos.y = (float)DefectRoiRect[k].bottom;
				LtPos = Math.OnRotate(CenterPoint, LtPos, ImgDegree);
				RbPos = Math.OnRotate(CenterPoint, RbPos, ImgDegree);

				DefectRoiRect[k].left   = (long)LtPos.x;
				DefectRoiRect[k].top    = (long)LtPos.y;
				DefectRoiRect[k].right  = (long)RbPos.x;
				DefectRoiRect[k].bottom = (long)RbPos.y;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
		// ��ü �ҷ����� ����
		DefectPointCount = 
			               ACount          + BCount          + CCount           + DCount          + //���� ���� DarkDefect
			               ACrackCount     + BCrackCount     + CCrackCount      + DCrackCount     + //���� ���� BrightDefect
			               C1Count         + C2Count         + C3Count          + C4Count         + //Corner ���� DarkDefect
			               A1CirDeftectCnt + A2CirDeftectCnt + C1CirDeftectCnt  + C2CirDeftectCnt + //Circle ���� DarkDefect
			               A1CirCrackCnt   + A2CirCrackCnt   + C1CirCrackCnt    + C2CirCrackCnt   + //Circle ���� BrightDefect
			               LeftDefectCount + TopDefectCount  + RIghtDefectCount + BtmDefectCount  + //���� ��ü���� DarkDefect
			               LeftCrackCount  + TopCrackCount   + RIghtCrackCount  + BtmCrackCount;    //���� ��ü���� BrightDefect
		// BTM ���� �˻� ���� �� Cell Size ���� 
		// �˻� ���� �� Cell Size ������ 
		if (LeftStaPos.x != 0.0f && LeftStaPos.y != 0.0f && LeftEndPos.x != 0.0f && LeftEndPos.y != 0.0f) {
			LeftCellSizeHeight = Math.GetLengthFrPointToPoint(LeftStaPos, LeftEndPos);
		}
		if (CenterStaPos.x != 0.0f && CenterStaPos.y != 0.0f && CenterEndPos.x != 0.0f && CenterEndPos.y != 0.0f) {
			CenCellSizeHeight = Math.GetLengthFrPointToPoint(CenterStaPos, CenterEndPos);
		}

		// Defect Blob Merge
		///////////////////////////////////////////////////////////////////////////////////////////////
		// Circle_1 �ҷ� Merge ��� 
		if (C1CirDeftectCnt != 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpDDefectCount = C1CirDeftectCnt;

			for (int k = 0; k < tmpDDefectCount; k++) {
				if (C1CirDefectPoint[k].Rect.left == 0 && C1CirDefectPoint[k].Rect.top == 0 && C1CirDefectPoint[k].Rect.right == 0 && C1CirDefectPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpDDefectCount - 1)) break;
					if (C1CirDefectPoint[l].Rect.left == 0 && C1CirDefectPoint[l].Rect.top == 0 && C1CirDefectPoint[k].Rect.right == 0 && C1CirDefectPoint[k].Rect.bottom == 0) continue;

					r1 = C1CirDefectPoint[k].Rect;
					r2 = C1CirDefectPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left <= r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right > r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						C1CirDefectPoint[k].Round += C1CirDefectPoint[l].Round;
						C1CirDefectPoint[k].Rect = OnMerge(C1CirDefectPoint[k].Rect, C1CirDefectPoint[l].Rect);
						C1CirDefectPoint[k].Width = C1CirDefectPoint[k].Rect.right - C1CirDefectPoint[k].Rect.left;
						C1CirDefectPoint[k].Height = C1CirDefectPoint[k].Rect.bottom - C1CirDefectPoint[k].Rect.top;
						C1CirDefectPoint[k].Hor_Dis = C1CirDefectPoint[k].Width;
						C1CirDefectPoint[k].Ver_Dis = C1CirDefectPoint[k].Height;
						C1CirDefectPoint[k].P.x = (float)(C1CirDefectPoint[k].Rect.left);
						C1CirDefectPoint[k].P.y = (float)(C1CirDefectPoint[k].Rect.top);
						C1CirDefectPoint[l].Rect.left = C1CirDefectPoint[l].Rect.top = C1CirDefectPoint[l].Rect.right = C1CirDefectPoint[l].Rect.bottom = 0;
						C1CirDeftectCnt--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpDDefectCount; j++) {
				if (C1CirDefectPoint[j].Rect.left == 0 && C1CirDefectPoint[j].Rect.top == 0 && C1CirDefectPoint[j].Rect.right == 0 && C1CirDefectPoint[j].Rect.bottom == 0) continue;
				C1CirDefectPoint[sort] = C1CirDefectPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Circle_2 �ҷ� Merge ��� 
		if (C2CirDeftectCnt != 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpDDefectCount = C2CirDeftectCnt;

			for (int k = 0; k < tmpDDefectCount; k++) {
				if (C2CirDefectPoint[k].Rect.left == 0 && C2CirDefectPoint[k].Rect.top == 0 && C2CirDefectPoint[k].Rect.right == 0 && C2CirDefectPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpDDefectCount - 1)) break;
					if (C2CirDefectPoint[l].Rect.left == 0 && C2CirDefectPoint[l].Rect.top == 0 && C2CirDefectPoint[k].Rect.right == 0 && C2CirDefectPoint[k].Rect.bottom == 0) continue;

					r1 = C2CirDefectPoint[k].Rect;
					r2 = C2CirDefectPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left <= r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right > r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						C2CirDefectPoint[k].Round += C2CirDefectPoint[l].Round;
						C2CirDefectPoint[k].Rect = OnMerge(C2CirDefectPoint[k].Rect, C2CirDefectPoint[l].Rect);
						C2CirDefectPoint[k].Width = C2CirDefectPoint[k].Rect.right - C2CirDefectPoint[k].Rect.left;
						C2CirDefectPoint[k].Height = C2CirDefectPoint[k].Rect.bottom - C2CirDefectPoint[k].Rect.top;
						C2CirDefectPoint[k].Hor_Dis = C2CirDefectPoint[k].Width;
						C2CirDefectPoint[k].Ver_Dis = C2CirDefectPoint[k].Height;
						C2CirDefectPoint[k].P.x = (float)(C2CirDefectPoint[k].Rect.left);
						C2CirDefectPoint[k].P.y = (float)(C2CirDefectPoint[k].Rect.top);
						C2CirDefectPoint[l].Rect.left = C2CirDefectPoint[l].Rect.top = C2CirDefectPoint[l].Rect.right = C2CirDefectPoint[l].Rect.bottom = 0;
						C2CirDeftectCnt--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpDDefectCount; j++) {
				if (C2CirDefectPoint[j].Rect.left == 0 && C2CirDefectPoint[j].Rect.top == 0 && C2CirDefectPoint[j].Rect.right == 0 && C2CirDefectPoint[j].Rect.bottom == 0) continue;
				C2CirDefectPoint[sort] = C2CirDefectPoint[j];
				sort++;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Side A Merge (DarkDefect)
		if (ACount != 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpADefectCount = ACount;

			for (int k = 0; k < tmpADefectCount; k++) {
				if (ADefectPoint[k].Rect.left == 0 && ADefectPoint[k].Rect.top == 0 && ADefectPoint[k].Rect.right == 0 && ADefectPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpADefectCount - 1)) break;
					if (ADefectPoint[l].Rect.left == 0 && ADefectPoint[l].Rect.top == 0 && ADefectPoint[k].Rect.right == 0 && ADefectPoint[k].Rect.bottom == 0) continue;

					r1 = ADefectPoint[k].Rect;
					r2 = ADefectPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left < r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right >= r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						ADefectPoint[k].Round += ADefectPoint[l].Round;
						ADefectPoint[k].Rect   = OnMerge(ADefectPoint[k].Rect, ADefectPoint[l].Rect);
						ADefectPoint[k].Width  = ADefectPoint[k].Rect.right - ADefectPoint[k].Rect.left;
						ADefectPoint[k].Height = ADefectPoint[k].Rect.bottom - ADefectPoint[k].Rect.top;
						ADefectPoint[k].P.x    = (float)(ADefectPoint[k].Rect.left);
						ADefectPoint[k].P.y    = (float)(ADefectPoint[k].Rect.top);
						ADefectPoint[l].Rect.left = ADefectPoint[l].Rect.top = ADefectPoint[l].Rect.right = ADefectPoint[l].Rect.bottom = 0;
						ACount--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpADefectCount; j++) {
				if (ADefectPoint[j].Rect.left == 0 && ADefectPoint[j].Rect.top == 0 && ADefectPoint[j].Rect.right == 0 && ADefectPoint[j].Rect.bottom == 0) continue;
				ADefectPoint[sort] = ADefectPoint[j];
				sort++;
			}
		}


		// Side B Merge (DarkDefect)
		if (BCount != 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpBDefectCount = BCount;

			for (int k = 0; k < tmpBDefectCount; k++) {
				if (BDefectPoint[k].Rect.left == 0 && BDefectPoint[k].Rect.top == 0 && BDefectPoint[k].Rect.right == 0 && BDefectPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpBDefectCount - 1)) break;
					if (BDefectPoint[l].Rect.left == 0 && BDefectPoint[l].Rect.top == 0 && BDefectPoint[k].Rect.right == 0 && BDefectPoint[k].Rect.bottom == 0) continue;

					r1 = BDefectPoint[k].Rect;
					r2 = BDefectPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left < r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right >= r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						BDefectPoint[k].Round += BDefectPoint[l].Round;
						BDefectPoint[k].Rect = OnMerge(BDefectPoint[k].Rect, BDefectPoint[l].Rect);
						BDefectPoint[k].Width = BDefectPoint[k].Rect.right - BDefectPoint[k].Rect.left;
						BDefectPoint[k].Height = BDefectPoint[k].Rect.bottom - BDefectPoint[k].Rect.top;
						BDefectPoint[k].P.x = (float)(BDefectPoint[k].Rect.left);
						BDefectPoint[k].P.y = (float)(BDefectPoint[k].Rect.top);
						BDefectPoint[l].Rect.left = BDefectPoint[l].Rect.top = BDefectPoint[l].Rect.right = BDefectPoint[l].Rect.bottom = 0;
						BCount--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpBDefectCount; j++) {
				if (BDefectPoint[j].Rect.left == 0 && BDefectPoint[j].Rect.top == 0 && BDefectPoint[j].Rect.right == 0 && BDefectPoint[j].Rect.bottom == 0) continue;
				BDefectPoint[sort] = BDefectPoint[j];
				sort++;
			}
		}

		// Side C Merge (DarkDefect)
		if (CCount != 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpCDefectCount = CCount;

			for (int k = 0; k < tmpCDefectCount; k++) {
				if (CDefectPoint[k].Rect.left == 0 && CDefectPoint[k].Rect.top == 0 && CDefectPoint[k].Rect.right == 0 && CDefectPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpCDefectCount - 1)) break;
					if (CDefectPoint[l].Rect.left == 0 && CDefectPoint[l].Rect.top == 0 && CDefectPoint[k].Rect.right == 0 && CDefectPoint[k].Rect.bottom == 0) continue;

					r1 = CDefectPoint[k].Rect;
					r2 = CDefectPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left < r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right >= r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						CDefectPoint[k].Round += CDefectPoint[l].Round;
						CDefectPoint[k].Rect   = OnMerge(CDefectPoint[k].Rect, CDefectPoint[l].Rect);
						CDefectPoint[k].Width  = CDefectPoint[k].Rect.right - CDefectPoint[k].Rect.left;
						CDefectPoint[k].Height = CDefectPoint[k].Rect.bottom - CDefectPoint[k].Rect.top;
						CDefectPoint[k].P.x    = (float)(CDefectPoint[k].Rect.left);
						CDefectPoint[k].P.y    = (float)(CDefectPoint[k].Rect.top);
						CDefectPoint[l].Rect.left = CDefectPoint[l].Rect.top = CDefectPoint[l].Rect.right = CDefectPoint[l].Rect.bottom = 0;
						CCount--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpCDefectCount; j++) {
				if (CDefectPoint[j].Rect.left == 0 && CDefectPoint[j].Rect.top == 0 && CDefectPoint[j].Rect.right == 0 && CDefectPoint[j].Rect.bottom == 0) continue;
				CDefectPoint[sort] = CDefectPoint[j];
				sort++;
			}
		}


		// Side D Merge (Crack & Chipping)
		if (DCount != 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpDDefectCount = DCount;

			for (int k = 0; k < tmpDDefectCount; k++) {
				if (DDefectPoint[k].Rect.left == 0 && DDefectPoint[k].Rect.top == 0 && DDefectPoint[k].Rect.right == 0 && DDefectPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpDDefectCount - 1)) break;
					if (DDefectPoint[l].Rect.left == 0 && DDefectPoint[l].Rect.top == 0 && DDefectPoint[k].Rect.right == 0 && DDefectPoint[k].Rect.bottom == 0) continue;

					r1 = DDefectPoint[k].Rect;
					r2 = DDefectPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left <= r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right > r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						DDefectPoint[k].Round += DDefectPoint[l].Round;
						DDefectPoint[k].Rect = OnMerge(DDefectPoint[k].Rect, DDefectPoint[l].Rect);
						DDefectPoint[k].Width = DDefectPoint[k].Rect.right - DDefectPoint[k].Rect.left;
						DDefectPoint[k].Height = DDefectPoint[k].Rect.bottom - DDefectPoint[k].Rect.top;
						DDefectPoint[k].P.x = (float)(DDefectPoint[k].Rect.left);
						DDefectPoint[k].P.y = (float)(DDefectPoint[k].Rect.top);
						DDefectPoint[l].Rect.left = DDefectPoint[l].Rect.top = DDefectPoint[l].Rect.right = DDefectPoint[l].Rect.bottom = 0;
						DCount--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpDDefectCount; j++) {
				if (DDefectPoint[j].Rect.left == 0 && DDefectPoint[j].Rect.top == 0 && DDefectPoint[j].Rect.right == 0 && DDefectPoint[j].Rect.bottom == 0) continue;
				DDefectPoint[sort] = DDefectPoint[j];
				sort++;
			}
		}

		//��ü���� ���� ASide Polygon Merge 
		if (TopDefectCount!= 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpADefectCount = TopDefectCount;

			for (int k = 0; k < tmpADefectCount; k++) {
				if (TopDefectPoint[k].Rect.left == 0 && TopDefectPoint[k].Rect.top == 0 && TopDefectPoint[k].Rect.right == 0 && TopDefectPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpADefectCount - 1)) break;
					if (TopDefectPoint[l].Rect.left == 0 && TopDefectPoint[l].Rect.top == 0 && TopDefectPoint[k].Rect.right == 0 && TopDefectPoint[k].Rect.bottom == 0) continue;

					r1 = TopDefectPoint[k].Rect;
					r2 = TopDefectPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left < r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right >= r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						TopDefectPoint[k].Round += TopDefectPoint[l].Round;
						TopDefectPoint[k].Rect   = OnMerge(TopDefectPoint[k].Rect, TopDefectPoint[l].Rect);
						TopDefectPoint[k].Width  = TopDefectPoint[k].Rect.right - TopDefectPoint[k].Rect.left;
						TopDefectPoint[k].Height = TopDefectPoint[k].Rect.bottom - TopDefectPoint[k].Rect.top;
						TopDefectPoint[k].P.x    = (float)(TopDefectPoint[k].Rect.left);
						TopDefectPoint[k].P.y    = (float)(TopDefectPoint[k].Rect.top);
						TopDefectPoint[l].Rect.left = TopDefectPoint[l].Rect.top = TopDefectPoint[l].Rect.right = TopDefectPoint[l].Rect.bottom = 0;
						TopDefectCount--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpADefectCount; j++) {
				if (TopDefectPoint[j].Rect.left == 0 && TopDefectPoint[j].Rect.top == 0 && TopDefectPoint[j].Rect.right == 0 && TopDefectPoint[j].Rect.bottom == 0) continue;
				TopDefectPoint[sort] = TopDefectPoint[j];
				sort++;
			}
		}

		//��ü���� ���� BSide Polygon Merge 
		if (RIghtDefectCount!= 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpBDefectCount = RIghtDefectCount;

			for (int k = 0; k < tmpBDefectCount; k++) {
				if (RightDefectPoint[k].Rect.left == 0 && RightDefectPoint[k].Rect.top == 0 && RightDefectPoint[k].Rect.right == 0 && RightDefectPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpBDefectCount - 1)) break;
					if (RightDefectPoint[l].Rect.left == 0 && RightDefectPoint[l].Rect.top == 0 && RightDefectPoint[k].Rect.right == 0 && RightDefectPoint[k].Rect.bottom == 0) continue;

					r1 = RightDefectPoint[k].Rect;
					r2 = RightDefectPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left < r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right >= r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						RightDefectPoint[k].Round += RightDefectPoint[l].Round;
						RightDefectPoint[k].Rect   = OnMerge(RightDefectPoint[k].Rect, RightDefectPoint[l].Rect);
						RightDefectPoint[k].Width  = RightDefectPoint[k].Rect.right  - RightDefectPoint[k].Rect.left;
						RightDefectPoint[k].Height = RightDefectPoint[k].Rect.bottom - RightDefectPoint[k].Rect.top;
						RightDefectPoint[k].P.x    = (float)(RightDefectPoint[k].Rect.left);
						RightDefectPoint[k].P.y    = (float)(RightDefectPoint[k].Rect.top);
						RightDefectPoint[l].Rect.left = RightDefectPoint[l].Rect.top = RightDefectPoint[l].Rect.right = RightDefectPoint[l].Rect.bottom = 0;
						RIghtDefectCount--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpBDefectCount; j++) {
				if (RightDefectPoint[j].Rect.left == 0 && RightDefectPoint[j].Rect.top == 0 && RightDefectPoint[j].Rect.right == 0 && RightDefectPoint[j].Rect.bottom == 0) continue;
				RightDefectPoint[sort] = RightDefectPoint[j];
				sort++;
			}
		}

		//��ü���� ���� CSide Polygon Merge 
		if (BtmDefectCount!= 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpCDefectCount = BtmDefectCount;

			for (int k = 0; k < tmpCDefectCount; k++) {
				if (BtmDefectPoint[k].Rect.left == 0 && BtmDefectPoint[k].Rect.top == 0 && BtmDefectPoint[k].Rect.right == 0 && BtmDefectPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpCDefectCount - 1)) break;
					if (BtmDefectPoint[l].Rect.left == 0 && BtmDefectPoint[l].Rect.top == 0 && BtmDefectPoint[k].Rect.right == 0 && BtmDefectPoint[k].Rect.bottom == 0) continue;

					r1 = BtmDefectPoint[k].Rect;
					r2 = BtmDefectPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left < r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right >= r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						BtmDefectPoint[k].Round += BtmDefectPoint[l].Round;
						BtmDefectPoint[k].Rect   = OnMerge(BtmDefectPoint[k].Rect, BtmDefectPoint[l].Rect);
						BtmDefectPoint[k].Width  = BtmDefectPoint[k].Rect.right  - BtmDefectPoint[k].Rect.left;
						BtmDefectPoint[k].Height = BtmDefectPoint[k].Rect.bottom - BtmDefectPoint[k].Rect.top;
						BtmDefectPoint[k].P.x    = (float)(BtmDefectPoint[k].Rect.left);
						BtmDefectPoint[k].P.y    = (float)(BtmDefectPoint[k].Rect.top);
						BtmDefectPoint[l].Rect.left = BtmDefectPoint[l].Rect.top = BtmDefectPoint[l].Rect.right = BtmDefectPoint[l].Rect.bottom = 0;
						BtmDefectCount--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpCDefectCount; j++) {
				if (BtmDefectPoint[j].Rect.left == 0 && BtmDefectPoint[j].Rect.top == 0 && BtmDefectPoint[j].Rect.right == 0 && BtmDefectPoint[j].Rect.bottom == 0) continue;
				BtmDefectPoint[sort] = BtmDefectPoint[j];
				sort++;
			}
		}

		//��ü���� ���� DSide Polygon Merge 
		if (LeftDefectCount!= 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpDDefectCount = LeftDefectCount;

			for (int k = 0; k < tmpDDefectCount; k++) {
				if (LeftDefectPoint[k].Rect.left == 0 && LeftDefectPoint[k].Rect.top == 0 && LeftDefectPoint[k].Rect.right == 0 && LeftDefectPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpDDefectCount - 1)) break;
					if (LeftDefectPoint[l].Rect.left == 0 && LeftDefectPoint[l].Rect.top == 0 && LeftDefectPoint[k].Rect.right == 0 && LeftDefectPoint[k].Rect.bottom == 0) continue;

					r1 = LeftDefectPoint[k].Rect;
					r2 = LeftDefectPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left < r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right >= r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						LeftDefectPoint[k].Round += LeftDefectPoint[l].Round;
						LeftDefectPoint[k].Rect   = OnMerge(LeftDefectPoint[k].Rect, LeftDefectPoint[l].Rect);
						LeftDefectPoint[k].Width  = LeftDefectPoint[k].Rect.right  - LeftDefectPoint[k].Rect.left;
						LeftDefectPoint[k].Height = LeftDefectPoint[k].Rect.bottom - LeftDefectPoint[k].Rect.top;
						LeftDefectPoint[k].P.x    = (float)(LeftDefectPoint[k].Rect.left);
						LeftDefectPoint[k].P.y    = (float)(LeftDefectPoint[k].Rect.top);
						LeftDefectPoint[l].Rect.left = LeftDefectPoint[l].Rect.top = LeftDefectPoint[l].Rect.right = LeftDefectPoint[l].Rect.bottom = 0;
						LeftDefectCount--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpDDefectCount; j++) {
				if (LeftDefectPoint[j].Rect.left == 0 && LeftDefectPoint[j].Rect.top == 0 && LeftDefectPoint[j].Rect.right == 0 && LeftDefectPoint[j].Rect.bottom == 0) continue;
				LeftDefectPoint[sort] = LeftDefectPoint[j];
				sort++;
			}
		}

		//ASide All Grind Defect + ACount Merge 

		// Side D Merge (BrightDefect)
		if (DCrackCount != 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpDDefectCount = DCrackCount;

			for (int k = 0; k < tmpDDefectCount; k++) {
				if (DCrackPoint[k].Rect.left == 0 && DCrackPoint[k].Rect.top == 0 && DCrackPoint[k].Rect.right == 0 && DCrackPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpDDefectCount - 1)) break;
					if (DCrackPoint[l].Rect.left == 0 && DCrackPoint[l].Rect.top == 0 && DCrackPoint[k].Rect.right == 0 && DCrackPoint[k].Rect.bottom == 0) continue;

					r1 = DCrackPoint[k].Rect;
					r2 = DCrackPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left <= r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right > r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						DCrackPoint[k].Round += DCrackPoint[l].Round;
						DCrackPoint[k].Rect   = OnMerge(DCrackPoint[k].Rect, DCrackPoint[l].Rect);
						DCrackPoint[k].Width  = DCrackPoint[k].Rect.right - DCrackPoint[k].Rect.left;
						DCrackPoint[k].Height = DCrackPoint[k].Rect.bottom - DCrackPoint[k].Rect.top;
						DCrackPoint[k].P.x = (float)(DCrackPoint[k].Rect.left);
						DCrackPoint[k].P.y = (float)(DCrackPoint[k].Rect.top);
						DCrackPoint[l].Rect.left = DCrackPoint[l].Rect.top = DCrackPoint[l].Rect.right = DCrackPoint[l].Rect.bottom = 0;
						DCrackCount--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpDDefectCount; j++) {
				if (DCrackPoint[j].Rect.left == 0 && DCrackPoint[j].Rect.top == 0 && DCrackPoint[j].Rect.right == 0 && DCrackPoint[j].Rect.bottom == 0) continue;
				DCrackPoint[sort] = DCrackPoint[j];
				sort++;
			}
		}


		// Side B Merge (BrightDefect)
		if (BCrackCount != 0) {
			bool IsxMg = false, IsyMg = false, IsInMg = false;
			CRect r1, r2;
			int sort = 0;
			int tmpBDefectCount = BCrackCount;

			for (int k = 0; k < tmpBDefectCount; k++) {
				if (BCrackPoint[k].Rect.left == 0 && BCrackPoint[k].Rect.top == 0 && BCrackPoint[k].Rect.right == 0 && BCrackPoint[k].Rect.bottom == 0) continue;

				for (int l = k + 1; l < k + MgCount; l++) {
					IsxMg = false; IsyMg = false;
					if (l > (tmpBDefectCount - 1)) break;
					if (BCrackPoint[l].Rect.left == 0 && BCrackPoint[l].Rect.top == 0 && BCrackPoint[k].Rect.right == 0 && BCrackPoint[k].Rect.bottom == 0) continue;

					r1 = BCrackPoint[k].Rect;
					r2 = BCrackPoint[l].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);
					IsInMg = ((r1.left < r2.left) && (r1.top < r2.top) && (r1.bottom > r2.bottom) && (r1.right >= r2.right)) ? true : false;

					if (HorDist > xMerge&& VerDist > yMerge && !IsInMg) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if ((IsxMg && IsyMg) || IsInMg) {
						BCrackPoint[k].Round += BCrackPoint[l].Round;
						BCrackPoint[k].Rect   = OnMerge(BCrackPoint[k].Rect, BCrackPoint[l].Rect);
						BCrackPoint[k].Width  = BCrackPoint[k].Rect.right  - BCrackPoint[k].Rect.left;
						BCrackPoint[k].Height = BCrackPoint[k].Rect.bottom - BCrackPoint[k].Rect.top;
						BCrackPoint[k].P.x = (float)(BCrackPoint[k].Rect.left);
						BCrackPoint[k].P.y = (float)(BCrackPoint[k].Rect.top);
						BCrackPoint[l].Rect.left = BCrackPoint[l].Rect.top = BCrackPoint[l].Rect.right = BCrackPoint[l].Rect.bottom = 0;
						BCrackCount--;
						k = -1;
						break;
					}

				}

			}
			for (int j = 0; j < tmpBDefectCount; j++) {
				if (BCrackPoint[j].Rect.left == 0 && BCrackPoint[j].Rect.top == 0 && BCrackPoint[j].Rect.right == 0 && BCrackPoint[j].Rect.bottom == 0) continue;
				BCrackPoint[sort] = BCrackPoint[j];
				sort++;
			}
		}


		if (!IsCornerTop) {
			if (ACount != 0) { // Left 
				bool IsxMg, IsyMg;
				CRect r1, r2;
				int sort = 0;
				int tmpBDefectCount = ACount;

				// SIDE A + SIDE D Merge 
				for (int k = 0; k < tmpBDefectCount; k++) {
					if (ADefectPoint[k].Rect.left == 0 && ADefectPoint[k].Rect.top == 0 && ADefectPoint[k].Rect.right == 0 && ADefectPoint[k].Rect.bottom == 0) continue;
					IsxMg = false; IsyMg = false;
					r1 = ADefectPoint[k].Rect;
					r2 = DDefectPoint[0].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);

					if (HorDist > xMerge&& VerDist > yMerge) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if (IsxMg && IsyMg) {
						DDefectPoint[0].Round = (ADefectPoint[k].Round > DDefectPoint[0].Round) ? ADefectPoint[k].Round : DDefectPoint[0].Round;
						DDefectPoint[0].Rect = OnMerge(ADefectPoint[k].Rect, DDefectPoint[0].Rect);
						DDefectPoint[0].Width = DDefectPoint[0].Rect.right - DDefectPoint[0].Rect.left;
						DDefectPoint[0].Height = DDefectPoint[0].Rect.bottom - DDefectPoint[0].Rect.top;
						ADefectPoint[k].Rect.left = ADefectPoint[k].Rect.top = ADefectPoint[k].Rect.right = ADefectPoint[k].Rect.bottom = 0;
						ACount--;
						k = -1;
						//break;
					}
				}

				for (int j = 0; j < tmpBDefectCount; j++) {
					if (ADefectPoint[j].Rect.left == 0 && ADefectPoint[j].Rect.top == 0 && ADefectPoint[j].Rect.right == 0 && ADefectPoint[j].Rect.bottom == 0) continue;
					ADefectPoint[sort] = ADefectPoint[j];
					sort++;
				}
			}

			if (ACount != 0) { // Right 
				bool IsxMg, IsyMg;
				CRect r1, r2;
				int sort = 0;
				int tmpBDefectCount = ACount;

				// SIDE A + SIDE B Merge 
				for (int k = 0; k < tmpBDefectCount; k++) {
					if (ADefectPoint[k].Rect.left == 0 && ADefectPoint[k].Rect.top == 0 && ADefectPoint[k].Rect.right == 0 && ADefectPoint[k].Rect.bottom == 0) continue;
					IsxMg = false; IsyMg = false;
					r1 = ADefectPoint[k].Rect;
					r2 = BDefectPoint[0].Rect;

					register int HorDist = abs((r1.left < r2.left) ? r2.left - r1.right : r1.left - r2.right);
					register int VerDist = abs((r1.top < r2.top) ? r2.top - r1.bottom : r1.top - r2.bottom);

					if (HorDist > xMerge&& VerDist > yMerge) continue;

					IsxMg = (HorDist < xMerge) ? true : false;
					IsyMg = (VerDist < yMerge) ? true : false;

					if (IsxMg && IsyMg) {
						BDefectPoint[0].Round = (ADefectPoint[k].Round > BDefectPoint[0].Round) ? ADefectPoint[k].Round : BDefectPoint[0].Round;
						BDefectPoint[0].Rect = OnMerge(ADefectPoint[k].Rect, BDefectPoint[0].Rect);
						BDefectPoint[0].Width = BDefectPoint[0].Rect.right - BDefectPoint[0].Rect.left;
						BDefectPoint[0].Height = BDefectPoint[0].Rect.bottom - BDefectPoint[0].Rect.top;
						ADefectPoint[k].Rect.left = ADefectPoint[k].Rect.top = ADefectPoint[k].Rect.right = ADefectPoint[k].Rect.bottom = 0;
						ACount--;
						k = -1;
						//break;
					}
				}

				for (int j = 0; j < tmpBDefectCount; j++) {
					if (ADefectPoint[j].Rect.left == 0 && ADefectPoint[j].Rect.top == 0 && ADefectPoint[j].Rect.right == 0 && ADefectPoint[j].Rect.bottom == 0) continue;
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
int CFM15xLineDefect::OnExecute(bool l, bool t, bool r, bool b, bool lt, bool rt, bool rb, bool lb)
{

	int x = IRoi->GetOrgX();
	int y = IRoi->GetOrgY();
	int w = IRoi->GetWidth();
	int h = IRoi->GetHeight();

	int szX = IRoi->GetWidth();
	int szY = IRoi->GetHeight();
	int szBuffer = szX > szY ? szX : szY;

	int nLRoiOffset = 0;
	int nRRoiOffset = 0;
	int CornerCount = 0;

	int ROI_X = 0;
	int ROI_Y = 0;
	int ROI_W = 0;
	int ROI_H = 0;

	int MaxGrindID = 0;
	int MarkID = 0;
	double deg = 0.0f;

	const int MAX_SIDE = 8;
	//const int BMWidth      = 500;
	const int BMHeight = 200;
	const int GRWidth = 250;
	const int GROffset = 20;
	const int ROIOffset = 10;
	const int CornerMargin = 2;
	int BMWidth = 0;

	// ���ۺκ� Broken�߻��� 
	int LineMargin = 30;
	int LineRange  = 80;

	// Euresys ROI 
	// Median Filtter ���� ���� ROI   
	EROIBW8   FilterROI;

	//Polygon Value Test 
	int   PolygonMarginCnt = 0;

	// Circle Notch�� Trench_Width ���� ������ 
	const int TrenchWidthMargin = 2000;

	// ��Mark Find �˻�� �ʿ��� ROI���� ������ 
	int GrindRectWidth = 0, GrindRectHeight = 0;

	// FAVION Circle Fitting �ʿ��� Data 
	float SampleSize = 0.0f; //0.5~0.8���Ϸ� �Է��Ұ�
	float DistDevRatio = 0.15; //0.1~0.25 ���Ϸ� �Է��Ұ�

	//Circle Count,Avg Round 
	int CircleCount = 0;
	int EdgeCount = 0;
	float AvgCirDis = 0.0f;
	float TotalCirDis = 0.0f;
	double MaxDis = 0.0f;
	double MinDis = 100000.0f;
	double GrindMeaDis = 0.0f;
	double GrindWidthDis = 0.0f;
	double PatternDistance = 0.0f;

	//MultiMark
	double MaxScore = 0.0f;

	//Polygon Algorithm ����� OutData,InData ���� �ִܰŸ� ���� �ʿ��� Count
	int GrindMarkOutCnt = 0, GrindMarkInCnt = 0;

	// GOO 20150819
	//if (FDefectPoint != NULL) delete[] FDefectPoint;
	//FDefectPoint = new TFPoint[szBuffer];

	FLeftWall    = l;
	FTopWall     = t;
	FRightWall   = r;
	FBottomWall  = b;
	FLeftTop     = lt;
	FRightTop    = rt;
	FRightBottom = rb;
	FLeftBottom  = lb;

	// TOP, BTM Cell_Size, Grind_Size �˻�� �ʿ� ����

	FrameImgW = GImage->GetWidth();
	FrameImgH = GImage->GetHeight();

	TFPoint LT(0, 0);
	TFPoint RT((float)FrameImgW, 0);
	TFPoint RB((float)FrameImgW, (float)FrameImgH);
	TFPoint LB(0, (float)FrameImgH);
	TFPoint LSP, LEP;

	ImgDegree = 0.0f;

	// Data Init
	//�⺻ ������ ������ 
	FLTPoint.x = 0.0f; //�������ΰ� ��ܶ����� ������ ��ǥ
	FLTPoint.y = 0.0f; //�������ΰ� ��ܶ����� ������ ��ǥ
	FRTPoint.x = 0.0f; //�������ΰ� ��ܶ����� ������ ��ǥ
	FRTPoint.y = 0.0f; //�������ΰ� ��ܶ����� ������ ��ǥ
	FRBPoint.x = 0.0f; //�������ΰ� �ϴܶ����� ������ ��ǥ
	FRBPoint.y = 0.0f; //�������ΰ� �ϴܶ����� ������ ��ǥ
	FLBPoint.x = 0.0f; //�������ΰ� �ϴܶ����� ������ ��ǥ
	FLBPoint.y = 0.0f; //�������ΰ� �ϴܶ����� ������ ��ǥ

	//ChamberCut ����÷ ������ ,
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

	for (int i = 0; i < MAX_SIDE; i++) {
		Line[i].FP0.x = 0.0f;
		Line[i].FP0.y = 0.0f;
		Line[i].FP1.x = 0.0f;
		Line[i].FP1.y = 0.0f;
	}

	TFPoint OrgPoint;
	// DvX,DvY �������� �̹��� ȸ�� �������� �����ϱ� ���ؼ� �ʿ��� ���� 
	TFPoint OrgMarkDv; TFPoint NewMarkDv;

	TFPoint StaPatern(0.0f, 0.0f);
	TFPoint EndPatern(0.0f, 0.0f);
	const int PatternMagin = 35;

	int Index = 10;

	// Polygon �˰��� ����� ���缺 ������ ���ؼ� 2���� ���� ��� ã����� �����ؾ��� 
	int nPolyPosCnt = 0; int HalfCnt = 0; double MaxPolyDis = 0.0f; double RealPolyDis = 0.0f; int PixelCnt = 0; double MinPolyDis = 100000.0f;
	TFPoint PolyRectCenter; TFPoint PolygonCenter;

	//ROI���� 5���� �ڵ� ��Ȱ �� ���� ���� ���� 
	int LeftRoiWidth  = 0;
	int RightRoiWidth = 0;
	int LeftRoiHeight = 0;
	int RightRoiHeight= 0;
	int CellWidth     = 0;
	int CenRoiRange   = 0;
	int SideRoiRange  = 0;
	int RoiMargin     = 0;

	//ASide���� Buffer �Ѿ��� 
	int PreRoiTop = 0;

	// �������� ã�� ���� ThresHold 
	int GrindThresHold = 0;

	// ��ü ������ ���� ���� ������Count���� Ȯ�� ���� ���� 
	int PreGrindMarkOutCnt = 0, PreGrindMarkInCnt = 0;

	// GrindMark ������� üũ ���� 
	bool IsGrindMarkUse = false;

	// ����,������ ���� ���� 
	double Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;
	TPoint P1(0, 0), P2(0, 0), P3(0, 0), P4(0, 0);

	// ThresHold Image Copy 
	if (GImage->GetImagePtr()) {
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &BinaryImg);
	}
	else {
		return false;
	}

	switch (Index) {
		// Find Line
		//----------------------------------------------------------------------
	case 10:
		if (FTopWall) {
			EasyImage::Threshold(GImage, &BinaryImg, TopEdgeThresHold);
			if (TopEdgeThresHold == 0) { SingleLine->GImage = GImage;     SingleLine->FThreshold = 5; }
			else                       { SingleLine->GImage = &BinaryImg; SingleLine->FThreshold = 10;}

			ROI_X = IRoi->GetOrgX  ();
			ROI_Y = IRoi->GetOrgY  ();
			ROI_W = IRoi->GetWidth ();
			ROI_H = 200              ;

			SingleLine->IRoi->SetOrgX  (ROI_X);
			SingleLine->IRoi->SetOrgY  (ROI_Y);
			SingleLine->IRoi->SetWidth (ROI_W);
			SingleLine->IRoi->SetHeight(ROI_H);

			if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
			if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;
			if((ROI_X + ROI_W)          > FrameImgW || (ROI_Y+ROI_H)            > FrameImgH) return false;

			SingleLine->SetOrientation(orNorth);
			SingleLine->FIncrement = 100;
			SingleLine->SetFindLineId(0);
			SingleLine->FThreshold = 5;

			if(TopEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
			else               SingleLine->SetEdgeType(1);  // White To Black 

			SingleLine->OnExecute();
			Line[1].FP0 = SingleLine->GetResultP0();
			Line[1].FP1 = SingleLine->GetResultP1();
		}
		if (FBottomWall) {
			EasyImage::Threshold(GImage, &BinaryImg, BtmEdgeThresHold);
			if (BtmEdgeThresHold == 0) { SingleLine->GImage = GImage;     SingleLine->FThreshold = 5; }
			else                       { SingleLine->GImage = &BinaryImg; SingleLine->FThreshold = 10;}

			ROI_X = IRoi->GetOrgX  ();
			ROI_Y = FEndLine - 100   ;
			ROI_W = IRoi->GetWidth ();
			ROI_H = 200              ;

			SingleLine->IRoi->SetOrgX  (ROI_X);
			SingleLine->IRoi->SetOrgY  (ROI_Y);
			SingleLine->IRoi->SetWidth (ROI_W);
			SingleLine->IRoi->SetHeight(ROI_H);

			if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
			if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;
			if((ROI_X + ROI_W)          > FrameImgW || (ROI_Y+ROI_H)            > FrameImgH) return false;

			SingleLine->SetOrientation(orSouth);
			SingleLine->FIncrement = 50;
			SingleLine->SetFindLineId(0);
			SingleLine->FThreshold = 5;

			if (BtmEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White
			else                SingleLine->SetEdgeType(1);  // White To Black 

			// �⺻ BtmLine Search 
			SingleLine->OnExecute();
			Line[3].FP0 = SingleLine->GetResultP0();
			Line[3].FP1 = SingleLine->GetResultP1();
		}
		if (FLeftWall) {
			EasyImage::Threshold(GImage, &BinaryImg, LeftEdgeThresHold);
			if (LeftEdgeThresHold == 0) { SingleLine->GImage = GImage;     SingleLine->FThreshold = 5; }
			else                        { SingleLine->GImage = &BinaryImg; SingleLine->FThreshold = 10;}

			// Frame ���� �ܰ����� ã�� �������� 
			if (FShape == shRectH) { //���� LeftLineã�� 
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
				SingleLine->SetIncrement(5);
			}
			else if (FShape == shRectC) { //�� LeftLineã�� 
				if     (IsRectBtm  ){ 
					ROI_X = IRoi->GetOrgX()  ;
					ROI_W = IRoi->GetWidth() ;
					ROI_Y = IRoi->GetOrgY()  ;
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
				SingleLine->SetIncrement(5);
			}
			else {                      // ��ƽ LeftLine ã�� 
				ROI_X = IRoi->GetOrgX()     ;
				ROI_W = IRoi->GetWidth()    ;
				ROI_Y = IRoi->GetOrgY()     ;
				ROI_H = IRoi->GetHeight()   ;
				SingleLine->SetIncrement(50);
			}

			SingleLine->IRoi->SetOrgX(ROI_X  );
			SingleLine->IRoi->SetOrgY(ROI_Y  );
			SingleLine->IRoi->SetWidth(ROI_W );
			SingleLine->IRoi->SetHeight(ROI_H);

			if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
			if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;
			if((ROI_X + ROI_W)          > FrameImgW || (ROI_Y+ROI_H)            > FrameImgH) return false;

			// DSide ��ü���� vs �̿��� �з� 
			if (IsLeftAllGrind) { // DSide ��ü���� Rect or Corner,Circle �з� �ʿ��� 
				if(IsRectTop && IsRectBtm){ //Rect���� ��ü ���� 
					//�⺻ LeftLine Search 
					SingleLine->SetOrientation(orWest);
					SingleLine->SetFindLineId(0);

					if (LeftEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
					else                 SingleLine->SetEdgeType(1);// White To Black 

					SingleLine->OnExecute();
					Line[0].FP0 = SingleLine->GetResultP0();
					Line[0].FP1 = SingleLine->GetResultP1();

					// Btm,Top ������� ù��°
					LeftOutLine[0].FP0.x = Line[0].FP0.x;
					LeftOutLine[0].FP0.y = Line[0].FP0.y;
					LeftOutLine[0].FP1.x = Line[0].FP1.x;
					LeftOutLine[0].FP1.y = Line[0].FP1.y;

					if     (FShape==shRectH){
						//GImage->Save("D:\\ROTATE_IMG\\GIMAGE_H.bmp");
						FLTPoint = Math.OnIntersect(Line[0].FP0, Line[0].FP1, Line[1].FP0, Line[1].FP1);
						if(CamId==Cam_Btm){P1.x = (int)(FLTPoint.x - 50); P1.y = (int)(FLTPoint.y+LTRectHeight    ); P2.x = (int)(P1.x + 100); P2.y = (int)(P1.y);}
						else              {P1.x = (int)(FLTPoint.x - 50); P1.y = (int)(FLTPoint.y+LTRectHeight+500); P2.x = (int)(P1.x + 100); P2.y = (int)(P1.y);}
						P3.x = (int)(FLTPoint.x - 50); P3.y = (int)(FrameImgH                  ); P4.x = (int)(P2.x);       P4.y = (int)(P3.y);
						//P3.x = (int)(FLTPoint.x - 50); P3.y = (int)(FrameImgH - P1.y       ); P4.x = (int)(P2.x);       P4.y = (int)(P3.y);
					}
					else if(FShape==shRectS){
						P1.x = (int)(Line[0].FP0.x - 50); P1.y = (int)(0);         P2.x = (int)(P1.x + 100); P2.y = (int)(P1.y);
						P3.x = (int)(Line[0].FP0.x - 50); P3.y = (int)(FrameImgH); P4.x = (int)(P2.x);       P4.y = (int)(P3.y);
					}
					else if(FShape==shRectC){
//						GImage->Save("D:\\ROTATE_IMG\\GIMAGE_C.bmp");
						FLBPoint = Math.OnIntersect(Line[0].FP0, Line[0].FP1, Line[3].FP0, Line[3].FP1);
						P1.x = (int)(FLBPoint.x - 50); P1.y = (int)(0);                         P2.x = (int)(P1.x + 100); P2.y = (int)(P1.y);
						P3.x = (int)(FLBPoint.x - 50); P3.y = (int)(FLBPoint.y - LBRectHeight); P4.x = (int)(P2.x);       P4.y = (int)(P3.y);
					}

						//����,������ ���� ���� ������ ���� 
					if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
						EasyImage::Threshold(GImage, &BinaryImg, DSideGrindThresHold);

						// Median Filter ���� 
						if (P1.x <= 0         || P1.y < 0         || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0       ) return false;
						if (P1.x >= FrameImgW || P1.y > FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) > FrameImgH) return false;
						if ((P1.x + (P2.x - P1.x)) >= FrameImgW   || (P1.y + (P3.y - P1.y)) > FrameImgH                     ) return false;
						const int   PolygonMarin = 5;

						FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
						FilterROI.Attach(&BinaryImg);
						FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
						LogUnit.SetLog(L"AllGrind_Left==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
//						EasyImage::Median(&FilterROI, &FilterROI);
						OnFilter(ftMedian, &FilterROI, &FilterROI);

						Polygon->GImage             = &BinaryImg;
						Polygon->IWidth             = FrameImgW;
						Polygon->IHeight            = FrameImgH;
						Polygon->PoPa.FMinWidth     = 2;
						Polygon->PoPa.FMaxWidth     = (int)(P2.x - P1.x);
						Polygon->PoPa.FMinHeight    = (int)((P3.y - P1.y) / 2);
						Polygon->PoPa.FMaxHeight    = (int)((P3.y - P1.y) + 10);
						Polygon->PoPa.MinPxCnt      = 100;
						Polygon->PoPa.MaxPxCnt      = 20000;
						Polygon->PoPa.MaxBbCnt      = 10;
						Polygon->PoPa.IsSeparate    = true;
						Polygon->PoPa.IsInToOut     = false;
						Polygon->PoPa.HiThHold      = DSideGrindThresHold;
						Polygon->PoPa.LoThHold      = 0;
						Polygon->PoPa.IgInSide      = true ;
						Polygon->PoPa.IsHorSort     = false;
						Polygon->PoPa.IsVerSort     = true ;
						Polygon->PoPa.IsInToLeft    = true ;
						Polygon->PoPa.TopFirstPoint = true ;
						Polygon->PoPa.BtmFirstPoint = false;


						if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
							Polygon->OnExecute(P1, P2, P3, P4);
						}

						// 2���̻� ���� � ������ ����� �з� ���� 
						if (Polygon->BlobCount > 1) {
							// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
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

						// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
						////////////////////////////////////////////////////////////////////
						LeftInEdgeCount = 0;
						if (Polygon->Blob[nPolyPosCnt].InCnt > 0) { //���� �����ϴ� InEdgeCount üũ 
							//Defect ROI ���� Data Init
							LeftGrindInCnt = 0;
							LeftGrindInPos = 0;
							for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
								if (Polygon->Blob[nPolyPosCnt].TopPoint.y   == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
								if (Polygon->Blob[nPolyPosCnt].BottomPoint.y== Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
								LeftInEdgeCount++;
							}

							if (LeftInEdgeCount > 0) { //InEdgeCount ���� ������ ���� ���� �� ����
								if (LeftInPoint != NULL) delete[] LeftInPoint;
								LeftInPoint = new TFPoint[LeftInEdgeCount];

								PixelCnt = 0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									LeftInPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].InPoint[k].x;
									LeftInPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].InPoint[k].y;
									//Defect ������� ���� ���ʶ��� Pos ������ ���� 
									LeftGrindInPos += LeftInPoint[PixelCnt].x;
									LeftGrindInCnt++;
									PixelCnt++;
								}
							}
						}

						LeftOutEdgeCount = 0;
						if (Polygon->Blob[nPolyPosCnt].OutCnt > 0) { //���� �����ϴ� OutEdgeCount üũ 
							for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
								if (Polygon->Blob[nPolyPosCnt].TopPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
								if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
								LeftOutEdgeCount++;
							}

							if (LeftOutEdgeCount > 0) { //OutEdgeCount ���� ������ ���� ���� �� ����
								if (LeftOutPoint != NULL) delete[] LeftOutPoint;
								LeftOutPoint = new TFPoint[LeftOutEdgeCount];

								PixelCnt = 0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

									LeftOutPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].OutPoint[k].x;
									LeftOutPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].OutPoint[k].y;
									PixelCnt++;
								}
							}
						}
						////////////////////////////////////////////////////////////////////

						// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
						if (LeftInEdgeCount > 0 && LeftOutEdgeCount > 0) {
							// Total Left In Data 
							for (int k = 0; k < LeftInEdgeCount; k++) {
								if (AllLeftInEdgeCount > MAX_ALL_EDGE_COUNT) return false;
								AllLeftInData[AllLeftInEdgeCount].x = LeftInPoint[k].x;
								AllLeftInData[AllLeftInEdgeCount].y = LeftInPoint[k].y + ImageHeight;
								//										 LogUnit.SetLog(L"Left In X : %5.3f Left In Y : %5.3f",AllLeftInData[AllLeftInEdgeCount].x,AllLeftInData[AllLeftInEdgeCount].y);
								AllLeftInEdgeCount++;
							}
							// Total Left Out Data 
							for (int k = 0; k < LeftOutEdgeCount; k++) {
								if (AllLeftOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
								AllLeftOutData[AllLeftOutEdgeCount].x = LeftOutPoint[k].x;
								AllLeftOutData[AllLeftOutEdgeCount].y = LeftOutPoint[k].y + ImageHeight;
								//										 LogUnit.SetLog(L"Left Out X : %5.3f Left Out Y : %5.3f",AllLeftOutData[AllLeftOutEdgeCount].x,AllLeftOutData[AllLeftOutEdgeCount].y);
								AllLeftOutEdgeCount++;
							}
						}
					}
				}
				else                      { //Corner or Circle ���� ��ü ���� 
					if (FShape == shRectS) { //��ü�����ϰ�� �е�� Notch������ �������� ������ ������
						P1.x =0, P1.y=0, P2.x=0, P2.y=0, P3.x=0, P3.y=0, P4.x=0, P4.y=0;
						EasyImage::Threshold(GImage, &BinaryImg, DSideGrindThresHold);

						SingleLine->SetOrientation(orWest);
						SingleLine->SetFindLineId(0);

						if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
						else                SingleLine->SetEdgeType(1);// White To Black 

						SingleLine->OnExecute();
						Line[0].FP0 = SingleLine->GetResultP0();
						Line[0].FP1 = SingleLine->GetResultP1();

						// Btm,Top ������� ù��°
						LeftOutLine[0].FP0.x = Line[0].FP0.x;
						LeftOutLine[0].FP0.y = Line[0].FP0.y;
						LeftOutLine[0].FP1.x = Line[0].FP1.x;
						LeftOutLine[0].FP1.y = Line[0].FP1.y;

						P1.x = (int)(Line[0].FP0.x - 50); P1.y = (int)(0);         P2.x = (int)(P1.x + 100); P2.y = (int)(P1.y);
						P3.x = (int)(Line[0].FP0.x - 50); P3.y = (int)(FrameImgH); P4.x = (int)(P2.x);       P4.y = (int)(P3.y);

						//����,������ ���� ���� ������ ���� 
						if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
							// Median Filter ���� 
							if (P1.x <= 0         || P1.y < 0         || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0       ) return false;
							if (P1.x >= FrameImgW || P1.y > FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) > FrameImgH) return false;
							if ((P1.x + (P2.x - P1.x)) >= FrameImgW   || (P1.y + (P3.y - P1.y)) > FrameImgH                     ) return false;
							const int   PolygonMarin = 5;

							FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
							FilterROI.Attach(&BinaryImg);
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"AllGrind_Left==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
	//						EasyImage::Median(&FilterROI, &FilterROI);
							OnFilter(ftMedian, &FilterROI, &FilterROI);

							Polygon->GImage             = &BinaryImg;
							Polygon->IWidth             = FrameImgW;
							Polygon->IHeight            = FrameImgH;
							Polygon->PoPa.FMinWidth     = 2;
							Polygon->PoPa.FMaxWidth     = (int)(P2.x - P1.x);
							Polygon->PoPa.FMinHeight    = (int)((P3.y - P1.y) / 2);
							Polygon->PoPa.FMaxHeight    = (int)((P3.y - P1.y) + 10);
							Polygon->PoPa.MinPxCnt      = 100;
							Polygon->PoPa.MaxPxCnt      = 20000;
							Polygon->PoPa.MaxBbCnt      = 10;
							Polygon->PoPa.IsSeparate    = true;
							Polygon->PoPa.IsInToOut     = false;
							Polygon->PoPa.HiThHold      = DSideGrindThresHold;
							Polygon->PoPa.LoThHold      = 0;
							Polygon->PoPa.IgInSide      = true ;
							Polygon->PoPa.IsHorSort     = false;
							Polygon->PoPa.IsVerSort     = true ;
							Polygon->PoPa.IsInToLeft    = true ;
							Polygon->PoPa.TopFirstPoint = true ;
							Polygon->PoPa.BtmFirstPoint = false;


							if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
								Polygon->OnExecute(P1, P2, P3, P4);
							}

							// 2���̻� ���� � ������ ����� �з� ���� 
							if (Polygon->BlobCount > 1) {
								// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
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

							// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
							////////////////////////////////////////////////////////////////////
							LeftInEdgeCount = 0;
							if (Polygon->Blob[nPolyPosCnt].InCnt > 0) { //���� �����ϴ� InEdgeCount üũ 
								//Defect Pos Data Init 
								LeftGrindInPos=0;
								LeftGrindInCnt=0;

								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y   == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y== Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									LeftInEdgeCount++;
								}

								if (LeftInEdgeCount > 0) { //InEdgeCount ���� ������ ���� ���� �� ����
									if (LeftInPoint != NULL) delete[] LeftInPoint;
									LeftInPoint = new TFPoint[LeftInEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
										if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
										if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
										LeftInPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].InPoint[k].x;
										LeftInPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].InPoint[k].y;
										//Defect ������� ���� ���ʶ��� Pos ������ ���� 
										LeftGrindInPos += LeftInPoint[PixelCnt].x;
										LeftGrindInCnt++;
										PixelCnt++;
									}
								}
							}

							LeftOutEdgeCount = 0;
							if (Polygon->Blob[nPolyPosCnt].OutCnt > 0) { //���� �����ϴ� OutEdgeCount üũ 
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
									LeftOutEdgeCount++;
								}

								if (LeftOutEdgeCount > 0) { //OutEdgeCount ���� ������ ���� ���� �� ����
									if (LeftOutPoint != NULL) delete[] LeftOutPoint;
									LeftOutPoint = new TFPoint[LeftOutEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
										if (Polygon->Blob[nPolyPosCnt].TopPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

										LeftOutPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].OutPoint[k].x;
										LeftOutPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].OutPoint[k].y;
										PixelCnt++;
									}
								}
							}
							////////////////////////////////////////////////////////////////////

							// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
							if (LeftInEdgeCount > 0 && LeftOutEdgeCount > 0) {
								// Total Left In Data 
								for (int k = 0; k < LeftInEdgeCount; k++) {
									if (AllLeftInEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllLeftInData[AllLeftInEdgeCount].x = LeftInPoint[k].x;
									AllLeftInData[AllLeftInEdgeCount].y = LeftInPoint[k].y + ImageHeight;
									//										 LogUnit.SetLog(L"Left In X : %5.3f Left In Y : %5.3f",AllLeftInData[AllLeftInEdgeCount].x,AllLeftInData[AllLeftInEdgeCount].y);
									AllLeftInEdgeCount++;
								}
								// Total Left Out Data 
								for (int k = 0; k < LeftOutEdgeCount; k++) {
									if (AllLeftOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllLeftOutData[AllLeftOutEdgeCount].x = LeftOutPoint[k].x;
									AllLeftOutData[AllLeftOutEdgeCount].y = LeftOutPoint[k].y + ImageHeight;
									//										 LogUnit.SetLog(L"Left Out X : %5.3f Left Out Y : %5.3f",AllLeftOutData[AllLeftOutEdgeCount].x,AllLeftOutData[AllLeftOutEdgeCount].y);
									AllLeftOutEdgeCount++;
								}
							}
						}
					}
					else { // �Ϲ� �ܰ����� ã�� ������ 
						SingleLine->SetOrientation(orWest);
						SingleLine->SetFindLineId(0);

						if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
						else                SingleLine->SetEdgeType(1);// White To Black 

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
						if (LeftOutLine[0].FP0.x != 0.0f && LeftOutLine[0].FP0.y != 0.0f && LeftOutLine[0].FP1.x != 0.0f && LeftOutLine[0].FP1.y != 0.0f) {
							PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;
							PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;
						} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
						else if (LeftOutLine[0].FP0.x == 0.0f || LeftOutLine[0].FP0.y == 0.0f || LeftOutLine[0].FP1.x == 0.0f || LeftOutLine[0].FP1.y == 0.0f) {
							if (PreLeftOutLine[0].FP0.x == 0.0f && PreLeftOutLine[0].FP0.y == 0.0f && PreLeftOutLine[0].FP1.x == 0.0f && PreLeftOutLine[0].FP1.y == 0.0f) {// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
								// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
								ROI_H = IRoi->GetHeight();
								ROI_X = IRoi->GetOrgX();
								LeftOutLine[0].FP0.x = ROI_X;    LeftOutLine[0].FP0.y = 0.0f;
								LeftOutLine[0].FP1.x = ROI_X;    LeftOutLine[0].FP1.y = (float)ROI_H;

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
			}
			else { // DSide �̿��� 
				SingleLine->SetOrientation(orWest);
				SingleLine->SetFindLineId(0);

				if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
				else                SingleLine->SetEdgeType(1);// White To Black 

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
				if (LeftOutLine[0].FP0.x != 0.0f && LeftOutLine[0].FP0.y != 0.0f && LeftOutLine[0].FP1.x != 0.0f && LeftOutLine[0].FP1.y != 0.0f) {
					PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;
					PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;
				} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
				else if (LeftOutLine[0].FP0.x == 0.0f || LeftOutLine[0].FP0.y == 0.0f || LeftOutLine[0].FP1.x == 0.0f || LeftOutLine[0].FP1.y == 0.0f) {
					if (PreLeftOutLine[0].FP0.x ==0.0f && PreLeftOutLine[0].FP0.y==0.0f && PreLeftOutLine[0].FP1.x==0.0f && PreLeftOutLine[0].FP1.y==0.0f) {// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
						// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
						ROI_H = IRoi->GetHeight();
						ROI_X = IRoi->GetOrgX();
						LeftOutLine[0].FP0.x = ROI_X;    LeftOutLine[0].FP0.y = 0.0f;
						LeftOutLine[0].FP1.x = ROI_X;    LeftOutLine[0].FP1.y = (float)ROI_H;

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
		if (FRightWall) { //NOTCH 
			EasyImage::Threshold(GImage, &BinaryImg, RightEdgeThresHold);
			if (RightEdgeThresHold == 0) { SingleLine->GImage = GImage;     SingleLine->FThreshold = 5; }
			else                         { SingleLine->GImage = &BinaryImg; SingleLine->FThreshold = 10;}

			// Frame ���� �ܰ����� ã�� �������� 
			if (FShape == shRectH) { //���� LeftLineã�� 
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
					ROI_H = FrameImgH - (RTCornerHeight + 50);
				}
				else if(IsCircleTop){
					ROI_X = IRoi->GetOrgX()                  ;
					ROI_W = IRoi->GetWidth()                 ;
					ROI_Y = LTCircleHeight + 50              ;
					ROI_H = FrameImgH - (RTCircleHeight + 50);
				}
				SingleLine->SetIncrement(5);
			}
			else if (FShape == shRectC) { //�� LeftLineã�� 
				if     (IsRectBtm  ){ 
					ROI_X = IRoi->GetOrgX()  ;
					ROI_W = IRoi->GetWidth() ;
					ROI_Y = IRoi->GetOrgY()  ;
					ROI_H = IRoi->GetHeight();
				}
				else if(IsCornerBtm){
					ROI_X = IRoi->GetOrgX()          ;
					ROI_Y = IRoi->GetOrgY()          ;
					ROI_W = IRoi->GetWidth()         ;
					ROI_H = FEndLine - RBCornerHeight;
				}
				else if(IsCircleBtm){
					ROI_X = IRoi->GetOrgX()          ;
					ROI_Y = IRoi->GetOrgY()          ;
					ROI_W = IRoi->GetWidth()         ;
					ROI_H = FEndLine - RBCircleHeight;
				}
				SingleLine->SetIncrement(5);
			}
			else {                      // ��ƽ LeftLine ã�� 
				ROI_X = IRoi->GetOrgX()     ;
				ROI_W = IRoi->GetWidth()    ;
				ROI_Y = IRoi->GetOrgY()     ;
				ROI_H = IRoi->GetHeight()   ;
				SingleLine->SetIncrement(50);
			}

			SingleLine->IRoi->SetOrgX(ROI_X  );
			SingleLine->IRoi->SetOrgY(ROI_Y  );
			SingleLine->IRoi->SetWidth(ROI_W );
			SingleLine->IRoi->SetHeight(ROI_H);

			if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
			if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;
			if((ROI_X + ROI_W)          > FrameImgW || (ROI_Y+ROI_H)            > FrameImgH) return false;

			// BSide ��ü���� vs �̿��� �з� 
			if (IsRightAllGrind) { // BSide ��ü����
				if (IsRectTop && IsRectBtm) { //Rect���� ��ü ���� 
					//�⺻ RightLine Search 
					SingleLine->SetOrientation(orEast);
					SingleLine->SetFindLineId(0);

					if (RightEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
					else                  SingleLine->SetEdgeType(1);// White To Black 

					SingleLine->OnExecute();
					Line[2].FP0 = SingleLine->GetResultP0();
					Line[2].FP1 = SingleLine->GetResultP1();

					RightOutLine[0].FP0.x = Line[2].FP0.x;
					RightOutLine[0].FP0.y = Line[2].FP0.y;
					RightOutLine[0].FP1.x = Line[2].FP1.x;
					RightOutLine[0].FP1.y = Line[2].FP1.y;

					if     (FShape==shRectH){
						FRTPoint = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, Line[1].FP0, Line[1].FP1);
						if(CamId==Cam_Btm){P1.x = (int)(FRTPoint.x - 50); P1.y = (int)(FRTPoint.y + RTRectHeight    ); P2.x = (int)(P1.x + 100); P2.y = (int)(P1.y);}
						else              {P1.x = (int)(FRTPoint.x - 50); P1.y = (int)(FRTPoint.y + RTRectHeight+500); P2.x = (int)(P1.x + 100); P2.y = (int)(P1.y);}
						P3.x = (int)(FRTPoint.x - 50); P3.y = (int)(FrameImgH                    ); P4.x = (int)(P2.x);       P4.y = (int)(P3.y);
						//P3.x = (int)(FRTPoint.x - 50); P3.y = (int)(FrameImgH - P1.y);          P4.x = (int)(P2.x);       P4.y = (int)(P3.y);
					}
					else if(FShape==shRectS){
						P1.x = (int)(Line[2].FP0.x - 50); P1.y = 0;         P2.x = (int)(P1.x + 100); P2.y = (int)(P1.y);
						P3.x = (int)(Line[2].FP0.x - 50); P3.y = FrameImgH; P4.x = (int)(P2.x);       P4.y = (int)(P3.y);
					}
					else if(FShape==shRectC){
						FRBPoint = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, Line[3].FP0, Line[3].FP1);
						P1.x = (int)(FRBPoint.x - 50); P1.y = (int)(0);                         P2.x = (int)(P1.x + 100); P2.y = (int)(P1.y);
						P3.x = (int)(FRBPoint.x - 50); P3.y = (int)(FRBPoint.y - RBRectHeight); P4.x = (int)(P2.x);       P4.y = (int)(P3.y);
					}

					//����,������ ���� ���� ������ ���� 
					if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
						EasyImage::Threshold(GImage, &BinaryImg, BSideGrindThresHold);
						// Median Filter ���� 
						if (P1.x <= 0         || P1.y < 0         || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0       ) return false;
						if (P1.x >= FrameImgW || P1.y > FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) > FrameImgH) return false;
						if ((P1.x + (P2.x - P1.x)) >= FrameImgW   || (P1.y + (P3.y - P1.y)) > FrameImgH                     ) return false;
						const int   PolygonMarin = 5;

						FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
						FilterROI.Attach(&BinaryImg);
						FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
						LogUnit.SetLog(L"AllGrind_Right==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
//						EasyImage::Median(&FilterROI, &FilterROI);
						OnFilter(ftMedian, &FilterROI, &FilterROI);

						Polygon->GImage             = &BinaryImg;
						Polygon->IWidth             = FrameImgW;
						Polygon->IHeight            = FrameImgH;
						Polygon->PoPa.FMinWidth     = 2;
						Polygon->PoPa.FMaxWidth     = (int)(P2.x - P1.x);
						Polygon->PoPa.FMinHeight    = (int)((P3.y - P1.y) / 2);
						Polygon->PoPa.FMaxHeight    = (int)((P3.y - P1.y) + 10);
						Polygon->PoPa.MinPxCnt      = 100;
						Polygon->PoPa.MaxPxCnt      = 20000;
						Polygon->PoPa.MaxBbCnt      = 10;
						Polygon->PoPa.IsSeparate    = true;
						Polygon->PoPa.IsInToOut     = false;
						Polygon->PoPa.HiThHold      = BSideGrindThresHold;
						Polygon->PoPa.LoThHold      = 0;
						Polygon->PoPa.IgInSide      = true ;
						Polygon->PoPa.IsHorSort     = false;
						Polygon->PoPa.IsVerSort     = true ;
						Polygon->PoPa.IsInToLeft    = false;
						Polygon->PoPa.TopFirstPoint = true ;
						Polygon->PoPa.BtmFirstPoint = false;

						if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
							Polygon->OnExecute(P1, P2, P3, P4);
						}

						// 2���̻� ���� � ������ ����� �з� ���� 
						if (Polygon->BlobCount > 1) {
							// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
							PolyRectCenter.x = 0.0f, PolyRectCenter.y = 0.0f, nPolyPosCnt = 0, RealPolyDis = 0.0f, MinPolyDis = 1000000.0f;
							PolyRectCenter.x = P3.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y) / 2);
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

						// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
						////////////////////////////////////////////////////////////////////
						RightInEdgeCount = 0;
						if (Polygon->Blob[nPolyPosCnt].InCnt > 0) { //���� �����ϴ� InEdgeCount üũ 
						//Defect ������� ���� ���ʶ��� Pos ������ Init
							RightGrindInPos =0;
							RightGrindInCnt =0;

							for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
								if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
								if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
								RightInEdgeCount++;
							}

							if (RightInEdgeCount > 0) { //InEdgeCount ���� ������ ���� ���� �� ����
								if (RightInPoint != NULL) delete[] RightInPoint;
								RightInPoint = new TFPoint[RightInEdgeCount];

								PixelCnt = 0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									RightInPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].InPoint[k].x;
									RightInPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].InPoint[k].y;
									//Defect ������� ���� ���ʶ��� Pos ������ ���� 
									RightGrindInPos += RightInPoint[PixelCnt].x;
									RightGrindInCnt++;
									PixelCnt++;
								}
							}
						}

						RightOutEdgeCount = 0;
						if (Polygon->Blob[nPolyPosCnt].OutCnt > 0) { //���� �����ϴ� OutEdgeCount üũ 
							for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
								if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
								if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
								RightOutEdgeCount++;
							}

							if (RightOutEdgeCount > 0) { //OutEdgeCount ���� ������ ���� ���� �� ����
								if (RightOutPoint != NULL) delete[] RightOutPoint;
								RightOutPoint = new TFPoint[RightOutEdgeCount];

								PixelCnt = 0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

									RightOutPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].OutPoint[k].x;
									RightOutPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].OutPoint[k].y;
									PixelCnt++;
								}
							}
						}
						////////////////////////////////////////////////////////////////////

						// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
						if (RightInEdgeCount > 0 && RightOutEdgeCount > 0) {
							// Total Right In Data 
							for (int k = 0; k < RightInEdgeCount; k++) {
								if (AllRightInEdgeCount > MAX_ALL_EDGE_COUNT) return false;
								AllRightInData[AllRightInEdgeCount].x = RightInPoint[k].x;
								AllRightInData[AllRightInEdgeCount].y = RightInPoint[k].y + ImageHeight;
								AllRightInEdgeCount++;
							}
							// Total Right Out Data 
							for (int k = 0; k < RightOutEdgeCount; k++) {
								if (AllRightOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
								AllRightOutData[AllRightOutEdgeCount].x = RightOutPoint[k].x;
								AllRightOutData[AllRightOutEdgeCount].y = RightOutPoint[k].y + ImageHeight;
								AllRightOutEdgeCount++;
							}
						}
					}
				}
				else                        { //Corner or Circle ���� ��ü ���� 
					if (FShape == shRectS) { //��ü�����ϰ�� �е�� Notch������ �������� ������ ������
						P1.x =0, P1.y=0, P2.x=0, P2.y=0, P3.x=0, P3.y=0, P4.x=0, P4.y=0;
						EasyImage::Threshold(GImage, &BinaryImg, BSideGrindThresHold);

						SingleLine->SetOrientation(orEast);
						SingleLine->SetFindLineId(0);

						if (RightEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
						else                  SingleLine->SetEdgeType(1);// White To Black 

						SingleLine->OnExecute();
						Line[2].FP0 = SingleLine->GetResultP0();
						Line[2].FP1 = SingleLine->GetResultP1();

						RightOutLine[0].FP0.x = Line[2].FP0.x;
						RightOutLine[0].FP0.y = Line[2].FP0.y;
						RightOutLine[0].FP1.x = Line[2].FP1.x;
						RightOutLine[0].FP1.y = Line[2].FP1.y;

						P1.x = (int)(Line[2].FP0.x - 50); P1.y = 0;         P2.x = (int)(P1.x + 100); P2.y = (int)(P1.y);
						P3.x = (int)(Line[2].FP0.x - 50); P3.y = FrameImgH; P4.x = (int)(P2.x);       P4.y = (int)(P3.y);

						//����,������ ���� ���� ������ ���� 
						if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
							// Median Filter ���� 
							if (P1.x <= 0         || P1.y < 0         || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0       ) return false;
							if (P1.x >= FrameImgW || P1.y > FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) > FrameImgH) return false;
							if ((P1.x + (P2.x - P1.x)) >= FrameImgW   || (P1.y + (P3.y - P1.y)) > FrameImgH                     ) return false;
							const int   PolygonMarin = 5;

							FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
							FilterROI.Attach(&BinaryImg);
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"AllGrind_Right==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
	//						EasyImage::Median(&FilterROI, &FilterROI);
							OnFilter(ftMedian, &FilterROI, &FilterROI);

							Polygon->GImage             = &BinaryImg;
							Polygon->IWidth             = FrameImgW;
							Polygon->IHeight            = FrameImgH;
							Polygon->PoPa.FMinWidth     = 2;
							Polygon->PoPa.FMaxWidth     = (int)(P2.x - P1.x);
							Polygon->PoPa.FMinHeight    = (int)((P3.y - P1.y) / 2);
							Polygon->PoPa.FMaxHeight    = (int)((P3.y - P1.y) + 10);
							Polygon->PoPa.MinPxCnt      = 100;
							Polygon->PoPa.MaxPxCnt      = 20000;
							Polygon->PoPa.MaxBbCnt      = 10;
							Polygon->PoPa.IsSeparate    = true;
							Polygon->PoPa.IsInToOut     = false;
							Polygon->PoPa.HiThHold      = BSideGrindThresHold;
							Polygon->PoPa.LoThHold      = 0;
							Polygon->PoPa.IgInSide      = true ;
							Polygon->PoPa.IsHorSort     = false;
							Polygon->PoPa.IsVerSort     = true ;
							Polygon->PoPa.IsInToLeft    = false;
							Polygon->PoPa.TopFirstPoint = true ;
							Polygon->PoPa.BtmFirstPoint = false;

							if (P1.x != 0 && P2.x != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
								Polygon->OnExecute(P1, P2, P3, P4);
							}

							// 2���̻� ���� � ������ ����� �з� ���� 
							if (Polygon->BlobCount > 1) {
								// Polygon ������ 2���̻� ����� �ּҰ� ���� �з� ������ 
								PolyRectCenter.x = 0.0f, PolyRectCenter.y = 0.0f, nPolyPosCnt = 0, RealPolyDis = 0.0f, MinPolyDis = 1000000.0f;
								PolyRectCenter.x = P3.x; PolyRectCenter.y = P1.y + ((P3.y - P1.y) / 2);
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

							// ���ο�(Buffer���� Y�� ������ġ�� ��ǥ����� ������) ������� Out/In Data �з� �� ������ ���� 
							////////////////////////////////////////////////////////////////////
							RightInEdgeCount = 0;
							if (Polygon->Blob[nPolyPosCnt].InCnt > 0) { //���� �����ϴ� InEdgeCount üũ 
								//Defect ������� ���� ���ʶ��� Pos ������ ���� 
								RightGrindInPos =0;
								RightGrindInCnt =0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
									LeftInEdgeCount++;
								}

								if (RightInEdgeCount > 0) { //InEdgeCount ���� ������ ���� ���� �� ����
									if (RightInPoint != NULL) delete[] RightInPoint;
									RightInPoint = new TFPoint[RightInEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
										if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
										if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].InPoint[k].y) continue;
										RightInPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].InPoint[k].x;
										RightInPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].InPoint[k].y;
										//Defect ������� ���� ���ʶ��� Pos ������ ���� 
										RightGrindInPos += RightInPoint[PixelCnt].x;
										RightGrindInCnt++;
										PixelCnt++;
									}
								}
							}

							RightOutEdgeCount = 0;
							if (Polygon->Blob[nPolyPosCnt].OutCnt > 0) { //���� �����ϴ� OutEdgeCount üũ 
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
									if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
									RightOutEdgeCount++;
								}

								if (RightOutEdgeCount > 0) { //OutEdgeCount ���� ������ ���� ���� �� ����
									if (RightOutPoint != NULL) delete[] RightOutPoint;
									RightOutPoint = new TFPoint[RightOutEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
										if (Polygon->Blob[nPolyPosCnt].TopPoint.y    == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;
										if (Polygon->Blob[nPolyPosCnt].BottomPoint.y == Polygon->Blob[nPolyPosCnt].OutPoint[k].y) continue;

										RightOutPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].OutPoint[k].x;
										RightOutPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].OutPoint[k].y;
										PixelCnt++;
									}
								}
							}
							////////////////////////////////////////////////////////////////////

							// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
							if (RightInEdgeCount > 0 && RightOutEdgeCount > 0) {
								// Total Right In Data 
								for (int k = 0; k < RightInEdgeCount; k++) {
									if (AllRightInEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllRightInData[AllRightInEdgeCount].x = RightInPoint[k].x;
									AllRightInData[AllRightInEdgeCount].y = RightInPoint[k].y + ImageHeight;
									AllRightInEdgeCount++;
								}
								// Total Right Out Data 
								for (int k = 0; k < RightOutEdgeCount; k++) {
									if (AllRightOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllRightOutData[AllRightOutEdgeCount].x = RightOutPoint[k].x;
									AllRightOutData[AllRightOutEdgeCount].y = RightOutPoint[k].y + ImageHeight;
									AllRightOutEdgeCount++;
								}
							}
						}
					}
					else { // �Ϲ� �ܰ����� ã�� ������ 
						SingleLine->SetOrientation(orEast);
						SingleLine->SetFindLineId(0);

						if(RightEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
						else                 SingleLine->SetEdgeType(1);// White To Black 

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
						if (RightOutLine[0].FP0.x != 0.0f && RightOutLine[0].FP0.y != 0.0f && RightOutLine[0].FP1.x != 0.0f && RightOutLine[0].FP1.y != 0.0f) {
							PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;
							PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;
						} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
						else if (RightOutLine[0].FP0.x == 0.0f || RightOutLine[0].FP0.y == 0.0f || RightOutLine[0].FP1.x == 0.0f || RightOutLine[0].FP1.y == 0.0f) {
							if (PreRightOutLine[0].FP0.x == 0.0f && PreRightOutLine[0].FP0.y == 0.0f && PreRightOutLine[0].FP1.x == 0.0f && PreRightOutLine[0].FP1.y == 0.0f) {// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
								// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
								ROI_H = IRoi->GetHeight();
								ROI_X = IRoi->GetOrgX();
								RightOutLine[0].FP0.x = ROI_X;    RightOutLine[0].FP0.y = 0.0f;
								RightOutLine[0].FP1.x = ROI_X;    RightOutLine[0].FP1.y = (float)ROI_H;

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
					}
				}
			}
			else { // DSide �̿��� 
				SingleLine->SetOrientation(orEast);
				SingleLine->SetFindLineId(0);

				if(RightEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
				else                 SingleLine->SetEdgeType(1);// White To Black 

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
				if (RightOutLine[0].FP0.x != 0.0f && RightOutLine[0].FP0.y != 0.0f && RightOutLine[0].FP1.x != 0.0f && RightOutLine[0].FP1.y != 0.0f) {
					PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;
					PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;
				} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
				else if (RightOutLine[0].FP0.x == 0.0f || RightOutLine[0].FP0.y == 0.0f || RightOutLine[0].FP1.x == 0.0f || RightOutLine[0].FP1.y == 0.0f) {
					if (PreRightOutLine[0].FP0.x == 0.0f && PreRightOutLine[0].FP0.y == 0.0f && PreRightOutLine[0].FP1.x == 0.0f && PreRightOutLine[0].FP1.y == 0.0f) {// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
						// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
						ROI_H = IRoi->GetHeight();
						ROI_X = IRoi->GetOrgX();
						RightOutLine[0].FP0.x = ROI_X;    RightOutLine[0].FP0.y = 0.0f;
						RightOutLine[0].FP1.x = ROI_X;    RightOutLine[0].FP1.y = (float)ROI_H;

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
			}
		}
		if (FLeftTop    ) { //NOTCH ==>�̹��� ȸ���� �ݺ��˻� �����ϹǷ� �ʿ���� 
		}			    
		if (FRightTop   ) { //NOTCH ==>�̹��� ȸ���� �ݺ��˻� �����ϹǷ� �ʿ���� 
		}
		if (FRightBottom) { //NOTCH ==>�̹��� ȸ���� �ݺ��˻� �����ϹǷ� �ʿ���� 
		}
		if (FLeftBottom ) { //NOTCH ==>�̹��� ȸ���� �ݺ��˻� �����ϹǷ� �ʿ����  
		}

		// NOTCH �ܰ��˻�� �� ����� �Ҿ����Ͽ� ���� Ʋ��������� 
		// �˻� ROI������ Ʋ���� ���� �������� ȸ���Ͽ��� �ܰ����� ��ó�� �����ϴ� �������� ���ؼ� ����ų �߻��� 
		// �̷� ���ؼ� �ε����ϰ� ���� �̹����� �����Ͽ� Ʋ���� ������ŭ ȸ���Ͽ� �⺻ Edge�����͸� �ٽ� ���� �� Defect�˻�� 
		// ȸ���� �̹����������� �˻� ������ 
		if (FShape == shRectH) { //NOTCH ROTATE IMAGE
			// Image Roate �� �����̹��� ���� ������ ���� 
			ImgDegree = 0.0f;
			FLTPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
			FRTPoint = Math.OnIntersect(Line[waRightWall].FP0, Line[waRightWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);

			//AlignMark or Cross������ ���� ���� ���� �� Image Rotate ���� 
			if (FP0.x != 0 && FP0.y != 0 && FP1.x != 0 && FP1.y != 0) {
				ImgDegree = -(Math.GetDegree(FP0.x, FP0.y, FP1.x, FP1.y));
				RotateImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &RotateImg);
				EasyImage::ScaleRotate(GImage, (float)FrameImgW / 2, (float)FrameImgH / 2, (float)FrameImgW / 2, (float)FrameImgH / 2, 1.0f, 1.0f, ImgDegree, &RotateImg, 4);
				// 						 RotateImg.Save("D:\\ROTATE_IMG\\NEW_RotateImgȸ��_HAP.bmp");
			}
			else if(FLTPoint.x != 0.0f && FLTPoint.y != 0.0f && FRTPoint.x != 0.0f && FRTPoint.y != 0.0f){
				ImgDegree = -(Math.GetDegree(FLTPoint.x, FLTPoint.y, FRTPoint.x, FRTPoint.y));
				RotateImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &RotateImg);
				EasyImage::ScaleRotate(GImage, (float)FrameImgW / 2, (float)FrameImgH / 2, (float)FrameImgW / 2, (float)FrameImgH / 2, 1.0f, 1.0f, ImgDegree, &RotateImg, 4);
			}

			//�˻� ������ Start 
			if (FLTPoint.x != 0.0f && FLTPoint.y != 0.0f && FRTPoint.x != 0.0f && FRTPoint.y != 0.0f) {
				CenterPoint.x = FrameImgW / 2;
				CenterPoint.y = FrameImgH / 2;

				// Binary RotateImg Size ���� �� ���� 
				RotateBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &RotateBinaryImg);
				// Grind Measure Corner, Circle �˻� ���� 
				RotateGrindBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &RotateGrindBinaryImg);
				// 					 LogUnit.SetLog(L"shRectH_RotateImg_End");  
//                RotateImg.Save("D:\\ROTATE_IMG\\RotateImg_HAP.bmp");
//				GImage->Save  ("D:\\ROTATE_IMG\\Not_RotateImg_HAP.bmp");

				if (FLeftWall) {
					EasyImage::Threshold(&RotateImg, &RotateBinaryImg, LeftEdgeThresHold);
					if (LeftEdgeThresHold == 0) { SingleLine->GImage = &RotateImg;       SingleLine->FThreshold = 5; }
					else                        { SingleLine->GImage = &RotateBinaryImg; SingleLine->FThreshold = 10;}

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
					SingleLine->SetIncrement(5);
					SingleLine->IRoi->SetOrgX(ROI_X  );
					SingleLine->IRoi->SetOrgY(ROI_Y  );
					SingleLine->IRoi->SetWidth(ROI_W );
					SingleLine->IRoi->SetHeight(ROI_H);

					if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
					if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;
					if((ROI_X + ROI_W)          > FrameImgW || (ROI_Y+ROI_H)            > FrameImgH) return false;

					SingleLine->SetOrientation(orWest);
					SingleLine->SetFindLineId(0);

					if(LeftEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
					else                SingleLine->SetEdgeType(1);// White To Black 

					SingleLine->OnExecute();
					Line[0].FP0 = SingleLine->GetResultP0();
					Line[0].FP1 = SingleLine->GetResultP1();

					// Multi Round Test 
					if ((fabs(Line[0].FP0.x - Line[0].FP1.x) > 2) && (Line[0].FP1.x < Line[0].FP0.x)) Line[0].FP0.x = Line[0].FP1.x;

					LeftOutLine[0].FP0.x = Line[0].FP0.x;
					LeftOutLine[0].FP0.y = Line[0].FP0.y;
					LeftOutLine[0].FP1.x = Line[0].FP1.x;
					LeftOutLine[0].FP1.y = Line[0].FP1.y;

					// LeftOutLine....make OutLine Data 
					// ������ �����͸� �Ѱ��ش� 
					// �˻��� ���ض����� ã�� ���Ұ�� ���������� �Ѱ��༭ ���ض��� ������ 
					if (LeftOutLine[0].FP0.x != 0.0f && LeftOutLine[0].FP0.y != 0.0f && LeftOutLine[0].FP1.x != 0.0f && LeftOutLine[0].FP1.y != 0.0f) {
						PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;
						PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;
					} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
					else if (LeftOutLine[0].FP0.x == 0.0f || LeftOutLine[0].FP0.y == 0.0f || LeftOutLine[0].FP1.x == 0.0f || LeftOutLine[0].FP1.y == 0.0f) {
						if (PreLeftOutLine[0].FP0.x == 0.0f && PreLeftOutLine[0].FP0.y == 0.0f && PreLeftOutLine[0].FP1.x == 0.0f && PreLeftOutLine[0].FP1.y == 0.0f) {// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
							// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
							ROI_H = IRoi->GetHeight();
							LeftOutLine[0].FP0.x = Line[0].FP0.x;    LeftOutLine[0].FP0.y = 0.0f;
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
					// 							 LogUnit.SetLog(L"Rotate_HAP_Left_End");     
				}
				if (FTopWall) { // ImageRotate 
					EasyImage::Threshold(&RotateImg, &RotateBinaryImg, TopEdgeThresHold);
					if (TopEdgeThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
					else                       { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

					RoiMargin = 10;

					ROI_X = IRoi->GetOrgX  ()          ;
					ROI_Y = IRoi->GetOrgY  ()+RoiMargin;
					ROI_W = IRoi->GetWidth();
					ROI_H = 200;

					SingleLine->IRoi->SetOrgX  (ROI_X);
					SingleLine->IRoi->SetOrgY  (ROI_Y);
					SingleLine->IRoi->SetWidth (ROI_W);
					SingleLine->IRoi->SetHeight(ROI_H);

					if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
					if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;
					if((ROI_X + ROI_W)          > FrameImgW || (ROI_Y+ROI_H)            > FrameImgH) return false;


					SingleLine->SetOrientation(orNorth);
					SingleLine->FIncrement = 100;
					SingleLine->SetFindLineId(0);

					if (TopEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White 
					else                SingleLine->SetEdgeType(1);  // White To Black  

					SingleLine->OnExecute();
					Line[1].FP0 = SingleLine->GetResultP0();
					Line[1].FP1 = SingleLine->GetResultP1();

					// ���� �ܰ����� 
					TopOutLine[0].FP0 = SingleLine->GetResultP0();
					TopOutLine[0].FP1 = SingleLine->GetResultP1();
				}
				if (FRightWall) {
					EasyImage::Threshold(&RotateImg, &RotateBinaryImg, RightEdgeThresHold);
					if (RightEdgeThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
					else                         { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

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
						ROI_H = FrameImgH - (RTCornerHeight + 50);
					}
					else if(IsCircleTop){
						ROI_X = IRoi->GetOrgX()                  ;
						ROI_W = IRoi->GetWidth()                 ;
						ROI_Y = LTCircleHeight + 50              ;
						ROI_H = FrameImgH - (RTCircleHeight + 50);
					}
					SingleLine->SetIncrement(5);

					SingleLine->IRoi->SetOrgX(ROI_X  );
					SingleLine->IRoi->SetOrgY(ROI_Y  );
					SingleLine->IRoi->SetWidth(ROI_W );
					SingleLine->IRoi->SetHeight(ROI_H);

					if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
					if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;
					if((ROI_X + ROI_W)          > FrameImgW || (ROI_Y+ROI_H)            > FrameImgH) return false;

					SingleLine->SetOrientation(orEast);
					SingleLine->SetFindLineId(0);

					if(RightEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
					else                 SingleLine->SetEdgeType(1);// White To Black 

					SingleLine->OnExecute();
					Line[2].FP0 = SingleLine->GetResultP0();
					Line[2].FP1 = SingleLine->GetResultP1();

					// Multi Round Test 
					if ((fabs(Line[2].FP0.x - Line[2].FP1.x) > 2) && (Line[2].FP1.x > Line[2].FP0.x)) Line[2].FP0.x = Line[2].FP1.x;

					RightOutLine[0].FP0.x = Line[2].FP0.x;
					RightOutLine[0].FP0.y = Line[2].FP0.y;
					RightOutLine[0].FP1.x = Line[2].FP1.x;
					RightOutLine[0].FP1.y = Line[2].FP1.y;

					// LeftOutLine....make OutLine Data 
					// ������ �����͸� �Ѱ��ش� 
					// �˻��� ���ض����� ã�� ���Ұ�� ���������� �Ѱ��༭ ���ض��� ������ 
					if (RightOutLine[0].FP0.x != 0.0f && RightOutLine[0].FP0.y != 0.0f && RightOutLine[0].FP1.x != 0.0f && RightOutLine[0].FP1.y != 0.0f) {
						PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;
						PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;
					} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
					else if (RightOutLine[0].FP0.x == 0.0f || RightOutLine[0].FP0.y == 0.0f || RightOutLine[0].FP1.x == 0.0f || RightOutLine[0].FP1.y == 0.0f) {
						if (PreRightOutLine[0].FP0.x == 0.0f && PreRightOutLine[0].FP0.y == 0.0f && PreRightOutLine[0].FP1.x == 0.0f && PreRightOutLine[0].FP1.y == 0.0f) {// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
							// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
							ROI_H = IRoi->GetHeight();
							RightOutLine[0].FP0.x = Line[2].FP0.x;    RightOutLine[0].FP0.y = 0.0f;
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
				}

				// Org Image Rotate �� ������ �ٽ� ��� 
				FLTPoint = Math.OnIntersect(Line[waLeftWall ].FP0, Line[waLeftWall ].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
				FRTPoint = Math.OnIntersect(Line[waRightWall].FP0, Line[waRightWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);

				if (IsTopAllGrind) { // ��ü����==>������ ENCAP �˻�� ����Ǵ� ������ 
					// Polygon_Algorithm �̿��Ͽ� ROI���� �������� ���� ������ ������ 
					// ��ü ������ �������� GrindWidth,GrindMeasure �����Ѵ� 
					// ASide ��ü������....LTPS vs EnCap��Ⱑ �޶� �ε����ϰ� �з��ؾ��� 
					P1.x = 0, P1.y = 0, P2.x = 0, P2.y = 0, P3.x = 0, P3.y = 0, P4.x = 0, P4.y = 0;
					if (IsRectTop) {
						CellWidth     = (int)(FRTPoint.x - FLTPoint.x) - (LTRectWidth+RTRectWidth);
						CenRoiRange   = (nASideRoiCenterLength == 0) ? (int)(CellWidth /5) : (int)(nASideRoiCenterLength * 2);
						SideRoiRange  = (nASideRoiCenterLength == 0) ? (int)(CellWidth /5) : (int)((CellWidth - (nASideRoiCenterLength * 2)) / 2);
						LeftRoiWidth  = CenRoiRange;
						RightRoiWidth = CenRoiRange;
						LeftRoiHeight = 50         ;
						RightRoiHeight= 50         ;
					}
					else if(IsCornerTop){
						LeftRoiWidth = LTCornerWidth ; RightRoiWidth  = RTCornerWidth ;
						LeftRoiHeight= LTCornerHeight; RightRoiHeight = RTCornerHeight;
						CellWidth    = (int)(FRTPoint.x - FLTPoint.x) - (LeftRoiWidth + RightRoiWidth);
						CenRoiRange  = (nASideRoiCenterLength == 0) ? (int)(CellWidth / 3) : (int)(nASideRoiCenterLength * 2);
						SideRoiRange = (nASideRoiCenterLength == 0) ? (int)(CellWidth / 3) : (int)((CellWidth - (nASideRoiCenterLength * 2)) / 2);
					}
					else if(IsCircleTop){
						LeftRoiWidth = LTCircleWidth ; RightRoiWidth  = RTCircleWidth ;
						LeftRoiHeight= LTCircleHeight; RightRoiHeight = RTCircleHeight;
						CellWidth    = (int)(FRTPoint.x - FLTPoint.x) - (LeftRoiWidth + RightRoiWidth);
						CenRoiRange  = (nASideRoiCenterLength == 0) ? (int)(CellWidth / 3) : (int)(nASideRoiCenterLength * 2);
						SideRoiRange = (nASideRoiCenterLength == 0) ? (int)(CellWidth / 3) : (int)((CellWidth - (nASideRoiCenterLength * 2)) / 2);
					}

					//Align Mark �����Ұ�� �̸� Ȱ���Ͽ� GlassHeight ������ ���⿡ Ȱ���� 
					if(FP0.x!=0.0f && FP0.y!=0.0f) FP0 = Math.OnRotate(CenterPoint, FP0, -ImgDegree);
					if(FP1.x!=0.0f && FP1.y!=0.0f) FP1 = Math.OnRotate(CenterPoint, FP1, -ImgDegree);

					// Left Mark To TopSide Height 
					if (CamId==Cam_Top) {
						if (FP0.x != 0.0f && FP0.y != 0.0f && Line[1].FP0.x != 0.0f && Line[1].FP0.y != 0.0f && Line[1].FP1.x != 0.0f && Line[1].FP1.y != 0.0f) {
							LeftMarkToTopHeight = GetLengthFrLineToPoint(Line[1].FP0, Line[1].FP1, FP0);
						}
						// Right Mark To TopSide Height 
						if (FP1.x != 0.0f && FP1.y != 0.0f && Line[1].FP0.x != 0.0f && Line[1].FP0.y != 0.0f && Line[1].FP1.x != 0.0f && Line[1].FP1.y != 0.0f) {
							RightMarkToTopHeight = GetLengthFrLineToPoint(Line[1].FP0, Line[1].FP1, FP1);
						}
						// Center Mark To TopSide Height
						if (LeftMarkToTopHeight != 0.0f && RightMarkToTopHeight != 0.0f) {
							CenMarkToTopHeight = ((FP0.y + FP1.y) / 2.0) - ((Line[1].FP0.y + Line[1].FP1.y) / 2.0);
						}
					}


					if (CamId==Cam_Top) { // Encap ��츸 ����,����,Defect 
						LogUnit.SetLog(L"HAP_Polygon_Start");
						AllTopInEdgeCount = 0; AllTopOutEdgeCount = 0; PreGrindMarkOutCnt = 0; PreGrindMarkInCnt = 0;

						// ������ ROI���� ��ŭ Polygon_Algorithm�̿��Ͽ� In/Out EdgePoint ������ ���� 
						P1.x = 0; P1.y = 0; P2.x = 0; P2.y = 0; P3.x = 0; P3.y = 0; P4.x = 0; P4.y = 0; RoiMargin = ((FLTPoint.y + FRTPoint.y) / 2) / 2;
						for (int i = 0; i < 5; i++) {
							GrindThresHold = 0;
							//ROI���� �ڵ� �з� 
							if     (i ==PadROI1 ) {
								P1.x = (int)(FLTPoint.x+LTRectWidth );P1.y = (int)((FLTPoint.y + FRTPoint.y) / 2 - (RoiMargin / 2));
								P2.x = (int)(P1.x      +LeftRoiWidth);P2.y = (int)(P1.y                                           );
								P3.x = (int)(P1.x                   );P3.y = (int)(P1.y + LeftRoiHeight+RoiMargin                 ); 
								P4.x = (int)(P2.x                   );P4.y = (int)(P3.y                                           );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadSideThresHold);
								GrindThresHold = PadSideThresHold;
							}
							else if (i ==PadROI2) {
								P1.x = (int)(P2.x               ); P1.y = (int)((FLTPoint.y + FRTPoint.y) / 2 - (RoiMargin / 2));
								P2.x = (int)(P1.x + SideRoiRange); P2.y = (int)(P1.y                                           );
								P3.x = (int)(P1.x               ); P3.y = (int)(P1.y + (RoiMargin * 2)                         );    
								P4.x = (int)(P2.x               ); P4.y = (int)(P3.y                                           );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadCenThresHold);
								GrindThresHold = PadCenThresHold;
							}
							else if (i ==PadROI3) {
								P1.x = (int)(P2.x              ); P1.y = (int)((FLTPoint.y + FRTPoint.y) / 2 - (RoiMargin / 2));
								P2.x = (int)(P1.x + CenRoiRange); P2.y = (int)(P1.y                                           );
								P3.x = (int)(P1.x              ); P3.y = (int)(P1.y + (RoiMargin * 2)                         );                
								P4.x = (int)(P2.x              ); P4.y = (int)(P3.y                                           );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadCenThresHold);
								GrindThresHold = PadCenThresHold;
							}
							else if (i ==PadROI4) {
								P1.x = (int)(P2.x               ); P1.y = (int)((FLTPoint.y + FRTPoint.y) / 2 - (RoiMargin / 2));
								P2.x = (int)(P1.x + SideRoiRange); P2.y = (int)(P1.y                                           );
								P3.x = (int)(P1.x               ); P3.y = (int)(P1.y + (RoiMargin * 2)                         );    
								P4.x = (int)(P2.x               ); P4.y = (int)(P3.y                                           );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadCenThresHold);
								GrindThresHold = PadCenThresHold;
							}
							else if (i ==PadROI5) {
								P1.x = (int)(FRTPoint.x - RightRoiWidth -RTRectWidth); P1.y = (int)((FLTPoint.y + FRTPoint.y) / 2 - (RoiMargin/2));
								P2.x = (int)(P1.x       + RightRoiWidth             ); P2.y = (int)(P1.y                                         );
								P3.x = (int)(P1.x                                   ); P3.y = (int)(P1.y + RightRoiHeight +RoiMargin             );
								P4.x = (int)(P2.x                                   ); P4.y = (int)(P3.y                                         );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadSideThresHold);
								GrindThresHold = PadSideThresHold;
							}

							// �˻�ȭ�� Display ���� 
							PadPolyRect[i].left  = P1.x;   PadPolyRect[i].top    = P1.y;
							PadPolyRect[i].right = P2.x;   PadPolyRect[i].bottom = P3.y;

							// Median Filter ���� 
							if (P1.x <= 0         || P1.y <= 0         || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0        ) return false;
							if (P1.x >= FrameImgW || P1.y >= FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) >= FrameImgH) return false;
							if ((P1.x + (P2.x - P1.x)) >= FrameImgW    || (P1.y + (P3.y - P1.y)) > FrameImgH                      ) return false;

							FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
							FilterROI.Attach(&RotateBinaryImg);
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"AllGrind_Top==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
//							EasyImage::Median(&FilterROI, &FilterROI);
							OnFilter(ftMedian, &FilterROI, &FilterROI);

							Polygon->GImage              = &RotateBinaryImg;
							Polygon->IWidth              = FrameImgW;
							Polygon->IHeight             = FrameImgH;
							Polygon->PoPa.FMinWidth      = (int)((P2.x - P1.x) / 2);
							Polygon->PoPa.FMaxWidth      = (int)((P2.x - P1.x) + 100);
							Polygon->PoPa.FMinHeight     = 2;
							Polygon->PoPa.FMaxHeight     = (int)((P3.y - P1.y) + 100);
							Polygon->PoPa.MinPxCnt       = 100;
							Polygon->PoPa.MaxPxCnt       = 20000;
							Polygon->PoPa.MaxBbCnt       = 10;
							Polygon->PoPa.IsSeparate     = true;
							Polygon->PoPa.IsInToOut      = false;
							Polygon->PoPa.HiThHold       = GrindThresHold;
							Polygon->PoPa.LoThHold       = 0;
							Polygon->PoPa.IgInSide       = true ;
							Polygon->PoPa.IsHorSort      = true ;
							Polygon->PoPa.IsVerSort      = false;
							Polygon->PoPa.LeftFirstPoint = false;
							Polygon->PoPa.RightFirstPoint= true ;

							if (P1.x != 0 && P1.y != 0 && P2.x != 0 && P2.y != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
								Polygon->OnExecute(P1, P2, P3, P4);
							}

							if (Polygon->BlobCount > 1) {
								PolyRectCenter.x = 0.0f, PolyRectCenter.y = 0.0f, nPolyPosCnt = 0, RealPolyDis = 0.0f, MinPolyDis = 1000000.0f;
								PolyRectCenter.x = P1.x + ((P2.x - P1.x) / 2); PolyRectCenter.y = P1.y;
								for (int k = 0; k < Polygon->BlobCount; k++) {
									PolygonCenter.x = Polygon->Blob[k].Rect.l + (Polygon->Blob[k].Rect.r - Polygon->Blob[k].Rect.l) / 2;
									PolygonCenter.y = Polygon->Blob[k].Rect.t; ;
									RealPolyDis = fabs(PolyRectCenter.y - PolygonCenter.y);

									if (RealPolyDis < MinPolyDis) {
										MinPolyDis = RealPolyDis;
										nPolyPosCnt = k;
									}
								}
							}
							else nPolyPosCnt = 0;

							TopInEdgeCount = 0;
							if (Polygon->Blob[nPolyPosCnt].InCnt > 0) { //���� �����ϴ� InEdgeCount üũ 
								TopGrindInCnt  = 0;
								TopGrindInPos  = 0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
									TopInEdgeCount++;
								}

								if (TopInEdgeCount > 0) { //InEdgeCount ���� ������ ���� ���� �� ����
									if (TopInPoint != NULL) delete[] TopInPoint;
									TopInPoint = new TFPoint[TopInEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
										TopInPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].InPoint[k].x;
										TopInPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].InPoint[k].y;
										//Defect ������� ���� ���ʶ��� Pos ������ ���� 
										if (IsRectTop) {
											TopGrindInPos += TopInPoint[PixelCnt].y;
											TopGrindInCnt++;
										}
										PixelCnt++;
									}
								}
							}

							TopOutEdgeCount = 0;
							if (Polygon->Blob[nPolyPosCnt].OutCnt > 0) { //���� �����ϴ� OutEdgeCount üũ 
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
									TopOutEdgeCount++;
								}

								if (TopOutEdgeCount > 0) { //InEdgeCount ���� ������ ���� ���� �� ����
									if (TopOutPoint != NULL) delete[] TopOutPoint;
									TopOutPoint = new TFPoint[TopOutEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
										TopOutPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].OutPoint[k].x;
										TopOutPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].OutPoint[k].y;
										PixelCnt++;
									}
								}
							}

							/*
							PolygonMarginCnt = (i == 0 || i == 4) ? 50 : 20;
							if (Polygon->Blob[0].InCnt > (PolygonMarginCnt * 2)) {
								TopInEdgeCount = 0;
								TopInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt * 2));
								if (TopInEdgeCount > 0) {
									if (TopInPoint != NULL) delete[] TopInPoint;
									TopInPoint = new TFPoint[TopInEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[0].InCnt; k++) {
										if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].InCnt - PolygonMarginCnt)) continue;
										TopInPoint[PixelCnt].x = Polygon->Blob[0].InPoint[k].x;
										TopInPoint[PixelCnt].y = Polygon->Blob[0].InPoint[k].y;
										PixelCnt++;
									}
								}
							}

							PolygonMarginCnt = (CamId==Cam_Top) ? 5 : 20;
							if (Polygon->Blob[0].OutCnt > (PolygonMarginCnt * 2)) {
								TopOutEdgeCount = 0;
								TopOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt * 2));
								if (TopOutEdgeCount > 0) {
									if (TopOutPoint != NULL) delete[] TopOutPoint;
									TopOutPoint = new TFPoint[TopOutEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[0].OutCnt; k++) {
										if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].OutCnt - PolygonMarginCnt)) continue;
										TopOutPoint[PixelCnt].x = Polygon->Blob[0].OutPoint[k].x;
										TopOutPoint[PixelCnt].y = Polygon->Blob[0].OutPoint[k].y;
										PixelCnt++;
									}
								}
							}
							*/

							// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
							if (TopInEdgeCount > 0 && TopOutEdgeCount > 0 ) {
								// Total Top In Data 
								for (int k = 0; k < TopInEdgeCount; k++) {
									if (AllTopInEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllTopInData[AllTopInEdgeCount].x = TopInPoint[k].x;
									AllTopInData[AllTopInEdgeCount].y = TopInPoint[k].y;
									AllTopInEdgeCount++;
								}
								// Total Top Out Data 
								for (int k = 0; k < TopOutEdgeCount; k++) {
									if (AllTopOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllTopOutData[AllTopOutEdgeCount].x = TopOutPoint[k].x;
									AllTopOutData[AllTopOutEdgeCount].y = TopOutPoint[k].y;
									AllTopOutEdgeCount++;
								}
							}
						}
						LogUnit.SetLog(L"HAP_Polygon_End");
					}
					else {  //LTPS TopEdgeThresHold,PadSideThresHold�� �������� �з��Ͽ� ��ü PolygonData ����
						AllTopInEdgeCount = 0; AllTopOutEdgeCount = 0; PreGrindMarkOutCnt = 0; PreGrindMarkInCnt = 0;
						// ������ ROI���� ��ŭ Polygon_Algorithm�̿��Ͽ� In/Out EdgePoint ������ ���� 
						P1.x = 0; P1.y = 0; P2.x = 0; P2.y = 0; P3.x = 0; P3.y = 0; P4.x = 0; P4.y = 0; RoiMargin = ((FLTPoint.y + FRTPoint.y) / 2) / 2;
						for (int i = 0; i < 5; i++) {
							GrindThresHold = 0;
							//ROI���� �ڵ� �з� 
							if     (i ==PadROI1 ) {
								P1.x = (int)(FLTPoint.x+LTRectWidth );P1.y = (int)((FLTPoint.y + FRTPoint.y) / 2 - (RoiMargin/2));
								P2.x = (int)(P1.x      +LeftRoiWidth);P2.y = (int)(P1.y                                         );
								P3.x = (int)(P1.x                   );P3.y = (int)(P1.y + LeftRoiHeight+RoiMargin               ); 
								P4.x = (int)(P2.x                   );P4.y = (int)(P3.y                                         );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadSideThresHold);
								GrindThresHold = PadSideThresHold;
							}
							else if (i ==PadROI2) {
								P1.x = (int)(P2.x               ); P1.y = (int)((FLTPoint.y + FRTPoint.y) / 2 - (RoiMargin/2)); 
								P2.x = (int)(P1.x + SideRoiRange); P2.y = (int)(P1.y                                         );
								P3.x = (int)(P1.x               ); P3.y = (int)(P1.y + (RoiMargin * 2)                       );    
								P4.x = (int)(P2.x               ); P4.y = (int)(P3.y                                         );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadCenThresHold);
								GrindThresHold = PadCenThresHold;
							}
							else if (i ==PadROI3) {
								P1.x = (int)(P2.x              ); P1.y = (int)((FLTPoint.y + FRTPoint.y) / 2 - (RoiMargin/2)); 
								P2.x = (int)(P1.x + CenRoiRange); P2.y = (int)(P1.y                                         );
								P3.x = (int)(P1.x              ); P3.y = (int)(P1.y + (RoiMargin * 2)                       );                
								P4.x = (int)(P2.x              ); P4.y = (int)(P3.y                                         );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadCenThresHold);
								GrindThresHold = PadCenThresHold;
							}
							else if (i ==PadROI4) {
								P1.x = (int)(P2.x               ); P1.y = (int)((FLTPoint.y + FRTPoint.y) / 2 - (RoiMargin/2));
								P2.x = (int)(P1.x + SideRoiRange); P2.y = (int)(P1.y                                         );
								P3.x = (int)(P1.x               ); P3.y = (int)(P1.y + (RoiMargin * 2)                       );    
								P4.x = (int)(P2.x               ); P4.y = (int)(P3.y                                         );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadCenThresHold);
								GrindThresHold = PadCenThresHold;
							}
							else if (i ==PadROI5) {
								P1.x = (int)(FRTPoint.x - RightRoiWidth -RTRectWidth); P1.y = (int)((FLTPoint.y + FRTPoint.y) / 2 - (RoiMargin/2));
								P2.x = (int)(P1.x       + RightRoiWidth             ); P2.y = (int)(P1.y                                         );
								P3.x = (int)(P1.x                                   ); P3.y = (int)(P1.y + RightRoiHeight +RoiMargin             );
								P4.x = (int)(P2.x                                   ); P4.y = (int)(P3.y                                         );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadSideThresHold);
								GrindThresHold = PadSideThresHold;
							}

							// �˻�ȭ�� Display ���� 
							PadPolyRect[i].left  = P1.x;   PadPolyRect[i].top    = P1.y;
							PadPolyRect[i].right = P2.x;   PadPolyRect[i].bottom = P3.y;

							// Median Filter ���� 
							if (P1.x <= 0         || P1.y <= 0         || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0        ) return false;
							if (P1.x >= FrameImgW || P1.y >= FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) >= FrameImgH) return false;
							if ((P1.x + (P2.x - P1.x)) >= FrameImgW    || (P1.y + (P3.y - P1.y)) > FrameImgH                      ) return false;

							FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
							FilterROI.Attach(&RotateBinaryImg);
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"AllGrind_Top==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							// 								 LogUnit.SetLog(L"LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
							EasyImage::Median(&FilterROI, &FilterROI);

							Polygon->GImage               = &RotateBinaryImg;
							Polygon->IWidth               = FrameImgW;
							Polygon->IHeight              = FrameImgH;
							Polygon->PoPa.FMinWidth       = (int)((P2.x - P1.x) / 2);
							Polygon->PoPa.FMaxWidth       = (int)((P2.x - P1.x) + 100);
							Polygon->PoPa.FMinHeight      = 2;
							Polygon->PoPa.FMaxHeight      = (int)((P3.y - P1.y) + 100);
							Polygon->PoPa.MinPxCnt        = 100;
							Polygon->PoPa.MaxPxCnt        = 20000;
							Polygon->PoPa.MaxBbCnt        = 10;
							Polygon->PoPa.IsSeparate      = true;
							Polygon->PoPa.IsInToOut       = false;
							Polygon->PoPa.HiThHold        = GrindThresHold;
							Polygon->PoPa.LoThHold        = 0;
							Polygon->PoPa.IgInSide        = true;
							Polygon->PoPa.IsHorSort       = true;
							Polygon->PoPa.IsVerSort       = false;
							Polygon->PoPa.LeftFirstPoint  = false;
							Polygon->PoPa.RightFirstPoint = true;

							if (P1.x != 0 && P1.y != 0 && P2.x != 0 && P2.y != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
								Polygon->OnExecute(P1, P2, P3, P4);
							}

							if (Polygon->BlobCount > 1) {
								PolyRectCenter.x = 0.0f, PolyRectCenter.y = 0.0f, nPolyPosCnt = 0, RealPolyDis = 0.0f, MinPolyDis = 1000000.0f;
								PolyRectCenter.x = P1.x + ((P2.x - P1.x) / 2); PolyRectCenter.y = P1.y;
								for (int k = 0; k < Polygon->BlobCount; k++) {
									PolygonCenter.x = Polygon->Blob[k].Rect.l + (Polygon->Blob[k].Rect.r - Polygon->Blob[k].Rect.l) / 2;
									PolygonCenter.y = Polygon->Blob[k].Rect.t; ;
									RealPolyDis = fabs(PolyRectCenter.y - PolygonCenter.y);

									if (RealPolyDis < MinPolyDis) {
										MinPolyDis = RealPolyDis;
										nPolyPosCnt = k;
									}
								}
							}
							else nPolyPosCnt = 0;

							if (Polygon->Blob[nPolyPosCnt].InCnt > 0) { //���� �����ϴ� InEdgeCount üũ 
								TopInEdgeCount = 0;
								//Defect ������� ���� ���ʶ��� Pos ������ ����
								TopGrindInPos  = 0;
								TopGrindInCnt  = 0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
									TopInEdgeCount++;
								}

								if (TopInEdgeCount > 0) { //InEdgeCount ���� ������ ���� ���� �� ����
									if (TopInPoint != NULL) delete[] TopInPoint;
									TopInPoint = new TFPoint[TopInEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
										TopInPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].InPoint[k].x;
										TopInPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].InPoint[k].y;
										//Defect ������� ���� ���ʶ��� Pos ������ ���� 
										if (IsRectTop) {
											TopGrindInPos += TopInPoint[PixelCnt].y;
											TopGrindInCnt++;
										}
										PixelCnt++;
									}
								}
							}

							if (Polygon->Blob[nPolyPosCnt].OutCnt > 0) { //���� �����ϴ� OutEdgeCount üũ 
								TopOutEdgeCount = 0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
									TopOutEdgeCount++;
								}

								if (TopOutEdgeCount > 0) { //InEdgeCount ���� ������ ���� ���� �� ����
									if (TopOutPoint != NULL) delete[] TopOutPoint;
									TopOutPoint = new TFPoint[TopOutEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
										TopOutPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].OutPoint[k].x;
										TopOutPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].OutPoint[k].y;
										PixelCnt++;
									}
								}
							}


							/*
							PolygonMarginCnt = (i == 0 || i == 4) ? 50 : 20;
							if (Polygon->Blob[0].InCnt > (PolygonMarginCnt * 2)) {
								TopInEdgeCount = 0;
								TopInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt * 2));
								if (TopInEdgeCount > 0) {
									if (TopInPoint != NULL) delete[] TopInPoint;
									TopInPoint = new TFPoint[TopInEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[0].InCnt; k++) {
										if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].InCnt - PolygonMarginCnt)) continue;
										TopInPoint[PixelCnt].x = Polygon->Blob[0].InPoint[k].x;
										TopInPoint[PixelCnt].y = Polygon->Blob[0].InPoint[k].y;
										PixelCnt++;
									}
								}
							}

							PolygonMarginCnt = (CamId==Cam_Top) ? 5 : 20;
							if (Polygon->Blob[0].OutCnt > (PolygonMarginCnt * 2)) {
								TopOutEdgeCount = 0;
								TopOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt * 2));
								if (TopOutEdgeCount > 0) {
									if (TopOutPoint != NULL) delete[] TopOutPoint;
									TopOutPoint = new TFPoint[TopOutEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[0].OutCnt; k++) {
										if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].OutCnt - PolygonMarginCnt)) continue;
										TopOutPoint[PixelCnt].x = Polygon->Blob[0].OutPoint[k].x;
										TopOutPoint[PixelCnt].y = Polygon->Blob[0].OutPoint[k].y;
										PixelCnt++;
									}
								}
							}
							*/

							// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
							if (TopInEdgeCount > 0 && TopOutEdgeCount > 0) {
								// Total Top In Data 
								for (int k = 0; k < TopInEdgeCount; k++) {
									if (AllTopInEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllTopInData[AllTopInEdgeCount].x = TopInPoint[k].x;
									AllTopInData[AllTopInEdgeCount].y = TopInPoint[k].y;
									AllTopInEdgeCount++;
								}
								// Total Top Out Data 
								for (int k = 0; k < TopOutEdgeCount; k++) {
									if (AllTopOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllTopOutData[AllTopOutEdgeCount].x = TopOutPoint[k].x;
									AllTopOutData[AllTopOutEdgeCount].y = TopOutPoint[k].y;
									AllTopOutEdgeCount++;
								}
							}
						}
					}

					//ENCAP ���� ������ ���� 
					/////////////////////////////////////////////////////////////////////////////////////////////////
					if(CamId==Cam_Top){
						LogUnit.SetLog(L"HAP_GrindMark_Start");
						//Multi GrindMark ������
						for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
							if (k >= MAX_GRINDMARK_ID) continue;
							MarkFind[k].OnLoad(true, FindDir, k);
							MarkFind[k].GImage = &RotateImg;
							MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
							MarkFind[k].SetMinScore(GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
							MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
							MarkFind[k].SetMaxInstances(1);
							MarkFind[k].SetAngleTolerance(20.0f);
							MarkFind[k].SetScaleBias(1.00f);
							MarkFind[k].SetScaleTolerance(0.0f);
							MarkFind[k].Property.Ox = (float)FrameImgW / 2;
							MarkFind[k].Property.Oy = (float)FrameImgH / 2;
						}

						if(FP0.x!=0.0f && FP0.y!=0.0f && FP1.x!=0.0f && FP1.y!=0.0f){ //Align Mark �̿��Ͽ� Align
							for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) { //20230425 GOO 
								if (AGrindMark[i].DvX == 0 || AGrindMark[i].DvY == 0) continue;
								//Train ���� �������� ROI ���� �̵� (��, ASide,CSide Imageȸ�� �� �˻� �����ϹǷ� ���ذ����� �ʿ���)
								//OrgMarkDv.x = (float)AGrindMark[i].DvX; OrgMarkDv.y = (float)AGrindMark[i].DvY;
								//deg = dASideDeg; //Train ����� ���ذ��� 
								//NewMarkDv = Math.OnRotate(FLTPoint, OrgMarkDv, deg);
								//AGrindMark[i].DvX = (int)NewMarkDv.x; AGrindMark[i].DvY = (int)NewMarkDv.y;

								//GrindMark ROI ���� ���� 
								//////////////////////////////////////////////////////////////////////////////////////////////
								if      (i == 4) { // Pad���� ������ ������ 
									AGrindMark[i].Rect.left = (FLTPoint.x + AGrindMark[i].DvX + PatternOffSet) - AGrindMark[i].Range;
									AGrindMark[i].Rect.top  = (FLTPoint.y + AGrindMark[i].DvY) - AGrindMark[i].Range;
								}
								else if (i ==18) { // Pad���� ������ ������ 
									AGrindMark[i].Rect.left = (FLTPoint.x + AGrindMark[i].DvX - PatternOffSet) - AGrindMark[i].Range;
									AGrindMark[i].Rect.top  = (FLTPoint.y + AGrindMark[i].DvY) - AGrindMark[i].Range;
								}
								else { // Pad���� ������ ���� ���� ���� 
									AGrindMark[i].Rect.left = (FLTPoint.x + AGrindMark[i].DvX) - AGrindMark[i].Range;
									AGrindMark[i].Rect.top  = (FLTPoint.y + AGrindMark[i].DvY) - AGrindMark[i].Range;
								}
								AGrindMark[i].Rect.right  = (AGrindMark[i].Rect.left) + (AGrindMark[i].Range*2);
								AGrindMark[i].Rect.bottom = (AGrindMark[i].Rect.top ) + (AGrindMark[i].Range*2);

								//�е念�� Buffer ������ ROI ���� ������
								if (AGrindMark[i].Rect.top <= 0) {PreRoiTop = AGrindMark[i].Rect.top; AGrindMark[i].Rect.top=1;}
								//////////////////////////////////////////////////////////////////////////////////////////////

								if (AGrindMark[i].Rect.left == 0 || AGrindMark[i].Rect.top == 0 || AGrindMark[i].Rect.right == 0 || AGrindMark[i].Rect.bottom == 0) continue;
								GrindRectWidth  = (AGrindMark[i].Rect.right  - AGrindMark[i].Rect.left);
								GrindRectHeight = (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top );

								if (AGrindMark[i].Rect.left <= 0         || AGrindMark[i].Rect.top <= 0         || GrindRectWidth <= 0         || GrindRectHeight <= 0        ) continue;
								if (AGrindMark[i].Rect.left >= FrameImgW || AGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;
								if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) continue;

								MarkID = AGrindMark[i].MarkID; // Multi Teaching MarkID 
								deg    = AGrindMark[i].degree; // Grind Mark Degree 
								MarkFind[MarkID].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
								MarkFind[MarkID].SetAngleBias(deg);
								AGrindMark[i].Score = 0.0f;

								//���� ���� =>AlignMarkȰ�� �� �߰� ������ Ȱ��
								if      (i==4 ) {
									AGrindMark[i].Point.x = FP0.x + PatternOffSet;
									AGrindMark[i].Point.y = FP0.y;
									AGrindMark[i].Score = 100;
								}
								else if (i==18) {
									AGrindMark[i].Point.x = FP1.x - PatternOffSet;
									AGrindMark[i].Point.y = FP1.y;
									AGrindMark[i].Score = 100;
								}
								else { //������ MarkID�������� MarkFind ������ 
									//��� TrainData�߿��� MaxScore�������� ��ǥ ���� 
									MaxScore = 0.0f;
									for(int k=0; k<MAX_GRINDMARK_ID; k++){
										if (AGrindMark[i].Rect.left <= 0         || AGrindMark[i].Rect.top <= 0         || GrindRectWidth <= 0         || GrindRectHeight <= 0        ) continue;
										if (AGrindMark[i].Rect.left >= FrameImgW || AGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;

										MarkFind[k].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
										MarkFind[k].SetAngleBias(AGrindMark[i].degree);
										MarkFind[k].OnExecute();

										if(MarkFind[k].Result[0].Score>MaxScore){ //GrindMark Search OK 
											MaxScore = MarkFind[k].Result[0].Score;
											AGrindMark[i].Point.x = MarkFind[k].Result[0].Px;
											AGrindMark[i].Point.y = MarkFind[k].Result[0].Py;
											AGrindMark[i].Score   = MarkFind[k].Result[0].Score;
										}
									}
								}

								//20230320 HSKIM
								if (AllTopOutEdgeCount != 0 && AllTopInEdgeCount != 0) { // MarkMode��� ������ �˻縦 ���� ���� �� ������ ���� 
									//Buffer ������ �߶� �˻�� ������ ������ ROI���� ������ 
									AGrindMark[i].Rect.top = (PreRoiTop!=0)?PreRoiTop: AGrindMark[i].Rect.top;
									// 25��° GrindMark Search �� ����,���� ������ ���
									/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) {  //GrindMark ã������� 
										// GrindMark���� OutData �ִܰŸ� Count���� Calc 
										GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
										for (int k = PreGrindMarkOutCnt; k < AllTopOutEdgeCount; k++) {
											if (AllTopOutData[k].x == 0.0f || AllTopOutData[k].y == 0.0f) continue;
											if (AGrindMark[i].Point.x == 0.0f || AGrindMark[i].Point.y == 0.0f) continue;

											GrindMeaDis = Math.GetLength(AGrindMark[i].Point, AllTopOutData[k]);
											if (GrindMeaDis < MinDis) {
												MinDis = GrindMeaDis;
												GrindMarkOutCnt = k;
												PreGrindMarkOutCnt = GrindMarkOutCnt; //��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
											}
										}
										// GrindMark���� InData �ִܰŸ� Count���� Calc 
										if (GrindMarkOutCnt != 0) {
											GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
											for (int k = PreGrindMarkInCnt; k < AllTopInEdgeCount; k++) {
												if (AllTopInData[k].x == 0.0f || AllTopInData[k].y == 0.0f) continue;
												if (AllTopOutData[GrindMarkOutCnt].x == 0.0f || AllTopOutData[GrindMarkOutCnt].y == 0.0f) continue;

												GrindMeaDis = Math.GetLength(AllTopOutData[GrindMarkOutCnt], AllTopInData[k]);
												if (GrindMeaDis < MinDis) {
													MinDis = GrindMeaDis;
													GrindMarkInCnt = k;
													PreGrindMarkInCnt = GrindMarkInCnt; //��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
												}
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
										if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
											CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
											for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
												if (AllTopOutData[k].x == 0.0f || AllTopOutData[k].y == 0.0f) continue;

												GrindMeaDis = Math.GetLength(AGrindMark[i].Point, AllTopOutData[k]);
												TotalCirDis += GrindMeaDis;
												CircleCount++;
											}
											if (TotalCirDis != 0.0f && CircleCount != 0) {
												AGrindMark[i].GrindMea = TotalCirDis / CircleCount;
												AGrindMark[i].GrindMea = AGrindMark[i].GrindMea + 0.5; //SubPixel 
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
										if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
											CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
											for (int k = -5; k < 5; k++) {
												if (AllTopOutData[GrindMarkOutCnt + k].x == 0.0f && AllTopOutData[GrindMarkOutCnt + k].y == 0.0f) continue;
												if (AllTopInData[GrindMarkInCnt + k].x == 0.0f && AllTopInData[GrindMarkInCnt + k].y == 0.0f) continue;
												GrindWidthDis = Math.GetLength(AllTopInData[GrindMarkInCnt + k], AllTopOutData[GrindMarkOutCnt + k]);
												TotalCirDis += GrindWidthDis;
												CircleCount++;
											}
											if (TotalCirDis != 0.0f && CircleCount != 0) {
												AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
												AGrindMark[i].GrindWidth = AGrindMark[i].GrindWidth + 0.5; //SubPixel
											}
										}
									}
									else if (AGrindMark[i].Rect.left != 0 && AGrindMark[i].Rect.top != 0 && AGrindMark[i].Rect.right != 0 && AGrindMark[i].Rect.bottom != 0) {  //GrindMark �� ã�������
										// OutData���� �ּҰ� ������ Count���� 
										GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
										for (int k = PreGrindMarkOutCnt; k < AllTopOutEdgeCount; k++) {
											if (AllTopOutData[k].x == 0.0f || AllTopOutData[k].y == 0.0f) continue;

											AGrindMark[i].RectPoint.x = AGrindMark[i].Rect.left + (AGrindMark[i].Rect.right - AGrindMark[i].Rect.left) / 2;
											AGrindMark[i].RectPoint.y = AGrindMark[i].Rect.top + (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top) / 2;

											GrindMeaDis = Math.GetLength(AGrindMark[i].RectPoint, AllTopOutData[k]);
											if (GrindMeaDis < MinDis) {
												MinDis = GrindMeaDis;
												GrindMarkOutCnt = k;
												PreGrindMarkOutCnt = GrindMarkOutCnt; //��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
											}
										}

										// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
										if (GrindMarkOutCnt != 0) {
											GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
											for (int k = PreGrindMarkInCnt; k < AllTopInEdgeCount; k++) {
												if (AllTopInData[k].x == 0.0f || AllTopInData[k].y == 0.0f) continue;
												if (AllTopOutData[GrindMarkOutCnt].x == 0.0f || AllTopOutData[GrindMarkOutCnt].y == 0.0f) continue;

												GrindMeaDis = Math.GetLength(AllTopOutData[GrindMarkOutCnt], AllTopInData[k]);
												if (GrindMeaDis < MinDis) {
													MinDis = GrindMeaDis;
													GrindMarkInCnt = k;
													PreGrindMarkInCnt = GrindMarkInCnt; //��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
												}
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
										if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
											CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
											for (int k = -5; k < 5; k++) {
												if (AllTopOutData[GrindMarkOutCnt + k].x == 0.0f && AllTopOutData[GrindMarkOutCnt + k].y == 0.0f) continue;
												if (AllTopInData[GrindMarkInCnt + k].x == 0.0f && AllTopInData[GrindMarkInCnt + k].y == 0.0f) continue;
												GrindWidthDis = Math.GetLength(AllTopInData[GrindMarkInCnt + k], AllTopOutData[GrindMarkOutCnt + k]);
												TotalCirDis += GrindWidthDis;
												CircleCount++;
											}
											if (TotalCirDis != 0.0f && CircleCount != 0) {
												AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
												AGrindMark[i].GrindWidth = AGrindMark[i].GrindWidth + 0.5; //SubPixel
											}
										}
										// EnCap�������� ASide Pattern Distance Ref ������ ���ϰŸ� ������ ���� 
										if (CamId==Cam_Top && IsTopAllGrind) {
											if (GrindMarkInCnt != 0 && GrindMarkInCnt > 5) {
												CircleCount = 0, TotalCirDis = 0.0f, PatternDistance = 0.0f;
												for (int k = GrindMarkInCnt - 5; k < GrindMarkInCnt + 5; k++) {
													if (AllTopInData[k].x == 0.0f || AllTopInData[k].y == 0.0f) continue;

													PatternDistance = Math.GetLength(AGrindMark[i].Point, AllTopInData[k]);
													TotalCirDis += PatternDistance;
													CircleCount++;
												}
												if (TotalCirDis != 0.0f && CircleCount != 0) {
													PatternDis[i].Dis = TotalCirDis / CircleCount;
													PatternDis[i].Dis = PatternDis[i].Dis + 0.5; //SubPixel 
												}
											}
										}
									}
								}
							}
						}
						else                                                        { //Align RectEdge �̿��Ͽ� Align 
							for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) { //20230425 GOO 
								if (AGrindMark[i].DvX == 0 || AGrindMark[i].DvY == 0) continue;
								//Train ���� �������� ROI ���� �̵� (��, ASide,CSide Imageȸ�� �� �˻� �����ϹǷ� ���ذ����� �ʿ���)
								//OrgMarkDv.x = (float)AGrindMark[i].DvX; OrgMarkDv.y = (float)AGrindMark[i].DvY;
								//deg = dASideDeg; //Train ����� ���ذ��� 
								//NewMarkDv = Math.OnRotate(FLTPoint, OrgMarkDv, deg);
								//AGrindMark[i].DvX = (int)NewMarkDv.x; AGrindMark[i].DvY = (int)NewMarkDv.y;

								//GrindMark ROI ���� ���� 
								//////////////////////////////////////////////////////////////////////////////////////////////
								AGrindMark[i].Rect.left   = (FLTPoint.x + AGrindMark[i].DvX) - AGrindMark[i].Range;
								AGrindMark[i].Rect.top    = (FLTPoint.y + AGrindMark[i].DvY) - AGrindMark[i].Range;
								AGrindMark[i].Rect.right  = (AGrindMark[i].Rect.left) + (AGrindMark[i].Range * 2);
								AGrindMark[i].Rect.bottom = (AGrindMark[i].Rect.top ) + (AGrindMark[i].Range * 2);

								//�е念�� Buffer ������ ROI ���� ������
								if (AGrindMark[i].Rect.top <= 0) {PreRoiTop = AGrindMark[i].Rect.top; AGrindMark[i].Rect.top=1;}
								//////////////////////////////////////////////////////////////////////////////////////////////

								if (AGrindMark[i].Rect.left == 0 || AGrindMark[i].Rect.top == 0 || AGrindMark[i].Rect.right == 0 || AGrindMark[i].Rect.bottom == 0) continue;
								GrindRectWidth  = (AGrindMark[i].Rect.right  - AGrindMark[i].Rect.left);
								GrindRectHeight = (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top );

								//��� TrainData�߿��� MaxScore�������� ��ǥ ���� 
								MaxScore = 0.0f;
								for(int k=0; k<MAX_GRINDMARK_ID; k++){
									if (AGrindMark[i].Rect.left <= 0         || AGrindMark[i].Rect.top <= 0         || GrindRectWidth <= 0         || GrindRectHeight <= 0        ) continue;
									if (AGrindMark[i].Rect.left >= FrameImgW || AGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;

									MarkFind[k].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
									MarkFind[k].SetAngleBias(AGrindMark[i].degree);
									MarkFind[k].OnExecute();

									if(MarkFind[k].Result[0].Score>MaxScore){ //GrindMark Search OK 
										MaxScore = MarkFind[k].Result[0].Score;
										if(PatternOffSet==0){
											AGrindMark[i].Point.x = MarkFind[k].Result[0].Px;
											AGrindMark[i].Point.y = MarkFind[k].Result[0].Py;
											AGrindMark[i].Score   = MarkFind[k].Result[0].Score;
										}
										else {
											if(k==4)      { //ù��° AlignMark 5
												AGrindMark[i].Point.x = MarkFind[k].Result[0].Px + PatternOffSet;
												AGrindMark[i].Point.y = MarkFind[k].Result[0].Py                ;
												AGrindMark[i].Score   = 100                                     ;

												//�˻� ROI 
												AGrindMark[i].Rect.left   = (FLTPoint.x + AGrindMark[i].DvX + PatternOffSet) - AGrindMark[i].Range;
												AGrindMark[i].Rect.top    = (FLTPoint.y + AGrindMark[i].DvY                ) - AGrindMark[i].Range;
												AGrindMark[i].Rect.right  = (AGrindMark[i].Rect.left) + (AGrindMark[i].Range*2);
												AGrindMark[i].Rect.bottom = (AGrindMark[i].Rect.top ) + (AGrindMark[i].Range*2);
											}
											else if(k==18){ //�ι�° AlignMark 19
												AGrindMark[i].Point.x = MarkFind[k].Result[0].Px - PatternOffSet;
												AGrindMark[i].Point.y = MarkFind[k].Result[0].Py                ;
												AGrindMark[i].Score   = 100                                     ;

												//�˻� ROI 
												AGrindMark[i].Rect.left   = (FLTPoint.x + AGrindMark[i].DvX - PatternOffSet) - AGrindMark[i].Range;
												AGrindMark[i].Rect.top    = (FLTPoint.y + AGrindMark[i].DvY                ) - AGrindMark[i].Range;
												AGrindMark[i].Rect.right  = (AGrindMark[i].Rect.left) + (AGrindMark[i].Range*2);
												AGrindMark[i].Rect.bottom = (AGrindMark[i].Rect.top ) + (AGrindMark[i].Range*2);
											}
											else {
												AGrindMark[i].Point.x = MarkFind[k].Result[0].Px   ;
												AGrindMark[i].Point.y = MarkFind[k].Result[0].Py   ;
												AGrindMark[i].Score   = MarkFind[k].Result[0].Score;
											}
										}
									}
								}

								//20230320 HSKIM
								if (AllTopOutEdgeCount != 0 && AllTopInEdgeCount != 0) { // MarkMode��� ������ �˻縦 ���� ���� �� ������ ���� 
									//Buffer ������ �߶� �˻�� ������ ������ ROI���� ������ 
									AGrindMark[i].Rect.top = (PreRoiTop!=0)?PreRoiTop: AGrindMark[i].Rect.top;
									// 25��° GrindMark Search �� ����,���� ������ ���
									/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) {  //GrindMark ã������� 
										// GrindMark���� OutData �ִܰŸ� Count���� Calc 
										GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
										for (int k = PreGrindMarkOutCnt; k < AllTopOutEdgeCount; k++) {
											if (AllTopOutData[k].x == 0.0f || AllTopOutData[k].y == 0.0f) continue;
											if (AGrindMark[i].Point.x == 0.0f || AGrindMark[i].Point.y == 0.0f) continue;

											GrindMeaDis = Math.GetLength(AGrindMark[i].Point, AllTopOutData[k]);
											if (GrindMeaDis < MinDis) {
												MinDis = GrindMeaDis;
												GrindMarkOutCnt = k;
												PreGrindMarkOutCnt = GrindMarkOutCnt; //��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
											}
										}
										// GrindMark���� InData �ִܰŸ� Count���� Calc 
										if (GrindMarkOutCnt != 0) {
											GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
											for (int k = PreGrindMarkInCnt; k < AllTopInEdgeCount; k++) {
												if (AllTopInData[k].x == 0.0f || AllTopInData[k].y == 0.0f) continue;
												if (AllTopOutData[GrindMarkOutCnt].x == 0.0f || AllTopOutData[GrindMarkOutCnt].y == 0.0f) continue;

												GrindMeaDis = Math.GetLength(AllTopOutData[GrindMarkOutCnt], AllTopInData[k]);
												if (GrindMeaDis < MinDis) {
													MinDis = GrindMeaDis;
													GrindMarkInCnt = k;
													PreGrindMarkInCnt = GrindMarkInCnt; //��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
												}
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
										if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
											CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
											for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
												if (AllTopOutData[k].x == 0.0f || AllTopOutData[k].y == 0.0f) continue;

												GrindMeaDis = Math.GetLength(AGrindMark[i].Point, AllTopOutData[k]);
												TotalCirDis += GrindMeaDis;
												CircleCount++;
											}
											if (TotalCirDis != 0.0f && CircleCount != 0) {
												AGrindMark[i].GrindMea = TotalCirDis / CircleCount;
												AGrindMark[i].GrindMea = AGrindMark[i].GrindMea + 0.5; //SubPixel 
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
										if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
											CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
											for (int k = -5; k < 5; k++) {
												if (AllTopOutData[GrindMarkOutCnt + k].x == 0.0f && AllTopOutData[GrindMarkOutCnt + k].y == 0.0f) continue;
												if (AllTopInData[GrindMarkInCnt + k].x == 0.0f && AllTopInData[GrindMarkInCnt + k].y == 0.0f) continue;
												GrindWidthDis = Math.GetLength(AllTopInData[GrindMarkInCnt + k], AllTopOutData[GrindMarkOutCnt + k]);
												TotalCirDis += GrindWidthDis;
												CircleCount++;
											}
											if (TotalCirDis != 0.0f && CircleCount != 0) {
												AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
												AGrindMark[i].GrindWidth = AGrindMark[i].GrindWidth + 0.5; //SubPixel
											}
										}
									}
									else if (AGrindMark[i].Rect.left != 0 && AGrindMark[i].Rect.top != 0 && AGrindMark[i].Rect.right != 0 && AGrindMark[i].Rect.bottom != 0) {  //GrindMark �� ã�������
										// OutData���� �ּҰ� ������ Count���� 
										GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
										for (int k = PreGrindMarkOutCnt; k < AllTopOutEdgeCount; k++) {
											if (AllTopOutData[k].x == 0.0f || AllTopOutData[k].y == 0.0f) continue;

											AGrindMark[i].RectPoint.x = AGrindMark[i].Rect.left + (AGrindMark[i].Rect.right - AGrindMark[i].Rect.left) / 2;
											AGrindMark[i].RectPoint.y = AGrindMark[i].Rect.top + (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top) / 2;

											GrindMeaDis = Math.GetLength(AGrindMark[i].RectPoint, AllTopOutData[k]);
											if (GrindMeaDis < MinDis) {
												MinDis = GrindMeaDis;
												GrindMarkOutCnt = k;
												PreGrindMarkOutCnt = GrindMarkOutCnt; //��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
											}
										}

										// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
										if (GrindMarkOutCnt != 0) {
											GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
											for (int k = PreGrindMarkInCnt; k < AllTopInEdgeCount; k++) {
												if (AllTopInData[k].x == 0.0f || AllTopInData[k].y == 0.0f) continue;
												if (AllTopOutData[GrindMarkOutCnt].x == 0.0f || AllTopOutData[GrindMarkOutCnt].y == 0.0f) continue;

												GrindMeaDis = Math.GetLength(AllTopOutData[GrindMarkOutCnt], AllTopInData[k]);
												if (GrindMeaDis < MinDis) {
													MinDis = GrindMeaDis;
													GrindMarkInCnt = k;
													PreGrindMarkInCnt = GrindMarkInCnt; //��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
												}
											}
										}

										// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
										if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
											CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
											for (int k = -5; k < 5; k++) {
												if (AllTopOutData[GrindMarkOutCnt + k].x == 0.0f && AllTopOutData[GrindMarkOutCnt + k].y == 0.0f) continue;
												if (AllTopInData[GrindMarkInCnt + k].x == 0.0f && AllTopInData[GrindMarkInCnt + k].y == 0.0f) continue;
												GrindWidthDis = Math.GetLength(AllTopInData[GrindMarkInCnt + k], AllTopOutData[GrindMarkOutCnt + k]);
												TotalCirDis += GrindWidthDis;
												CircleCount++;
											}
											if (TotalCirDis != 0.0f && CircleCount != 0) {
												AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
												AGrindMark[i].GrindWidth = AGrindMark[i].GrindWidth + 0.5; //SubPixel
											}
										}
										// EnCap�������� ASide Pattern Distance Ref ������ ���ϰŸ� ������ ���� 
										if (CamId==Cam_Top && IsTopAllGrind) {
											if (GrindMarkInCnt != 0 && GrindMarkInCnt > 5) {
												CircleCount = 0, TotalCirDis = 0.0f, PatternDistance = 0.0f;
												for (int k = GrindMarkInCnt - 5; k < GrindMarkInCnt + 5; k++) {
													if (AllTopInData[k].x == 0.0f || AllTopInData[k].y == 0.0f) continue;

													PatternDistance = Math.GetLength(AGrindMark[i].Point, AllTopInData[k]);
													TotalCirDis += PatternDistance;
													CircleCount++;
												}
												if (TotalCirDis != 0.0f && CircleCount != 0) {
													PatternDis[i].Dis = TotalCirDis / CircleCount;
													PatternDis[i].Dis = PatternDis[i].Dis + 0.5; //SubPixel 
												}
											}
										}
									}
								}
							}

							//YSize ���� ���� ������ ����
							//GrindMark���� Top���α��� �Ÿ� ���� 
							if (AGrindMark[4].Point.x!=0.0f && AGrindMark[4].Point.y!=0.0f && Line[1].FP0.x!=0.0f && Line[1].FP0.y!=0.0f && Line[1].FP1.x!=0.0f && Line[1].FP1.y!=0.0f) {
								LeftMarkToTopHeight = GetLengthFrLineToPoint(Line[1].FP0, Line[1].FP1, AGrindMark[4].Point);
							}
							// Right Mark To TopSide Height 
							if (AGrindMark[18].Point.x!=0.0f && AGrindMark[18].Point.y!=0.0f && Line[1].FP0.x!=0.0f && Line[1].FP0.y!=0.0f && Line[1].FP1.x!=0.0f && Line[1].FP1.y != 0.0f) {
								RightMarkToTopHeight = GetLengthFrLineToPoint(Line[1].FP0, Line[1].FP1, AGrindMark[18].Point);
							}
							// Center Mark To TopSide Height
							if (LeftMarkToTopHeight != 0.0f && RightMarkToTopHeight != 0.0f) {
								CenMarkToTopHeight = ((AGrindMark[4].Point.y + AGrindMark[18].Point.y)/2.0) - ((Line[1].FP0.y + Line[1].FP1.y)/2.0);
							}
						}
						LogUnit.SetLog(L"HAP_GrindMark_End");
					}

					// EnCap�������� ASide Pattern Distance Ref ������ ���ϰŸ� ������ ���� 
					if (CamId == Cam_Top && IsTopAllGrind && PatternThresHold!= 0) {
						EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PatternThresHold);
						// 												 RotateImg.Save("D:\\ROTATE_IMG\\Pattern_Distance.bmp");
						// 												 RotateBinaryImg.Save("D:\\ROTATE_IMG\\Pattern_Distance_Binary.bmp");
						if (PatternThresHold == 0) { SingleLine->GImage = &RotateImg      ; SingleLine->FThreshold = 5; }
						else                       { SingleLine->GImage = &RotateBinaryImg; SingleLine->FThreshold = 10;}

						for(int k=0; k< MAX_GRINDMARK_COUNT; k++){
							if(k<4                         || k>18                       ) continue; 
							if(AGrindMark[k].Point.x==0.0f || AGrindMark[k].Point.y==0.0f) continue;

							SingleLine->IRoi->SetOrgX(AGrindMark[k].Point.x - PatternMagin);
							SingleLine->IRoi->SetOrgY((TopOutLine[0].FP0.y + TopOutLine[0].FP1.y) / 2);
							SingleLine->IRoi->SetWidth(PatternMagin * 2);
							SingleLine->IRoi->SetHeight(AGrindMark[k].Point.y - ((TopOutLine[0].FP0.y + TopOutLine[0].FP1.y) / 2));

							ROI_X = (AGrindMark[k].Point.x - PatternMagin);
							ROI_Y = (TopOutLine[0].FP0.y + TopOutLine[0].FP1.y) / 2;
							ROI_W = PatternMagin * 2;
							ROI_H = AGrindMark[k].Point.y - ((TopOutLine[0].FP0.y + TopOutLine[0].FP1.y) / 2);

							if (ROI_X <= 0         || ROI_Y <= 0         || ROI_W <= 0         || ROI_H <= 0        ) return false;
							if (ROI_X >= FrameImgW || ROI_Y >= FrameImgH || ROI_W >= FrameImgW || ROI_H >= FrameImgH) return false;

							SingleLine->SetOrientation(orSouth);
							SingleLine->FIncrement = 1;
							SingleLine->SetFindLineId(0);
							SingleLine->SetEdgeType(1);  // White To Black 
							SingleLine->OnExecute();

							StaPatern.x = 0.0f, StaPatern.y = 0.0f, EndPatern.x = 0.0f, EndPatern.y = 0.0f;
							StaPatern = SingleLine->GetResultP0();
							EndPatern = SingleLine->GetResultP1();

							PatternDis[k].Dis = (Math.GetLengthFrLineToPoint(StaPatern, EndPatern, AGrindMark[k].Point));
							PatternDis[k].Dis = PatternDis[k].Dis + 0.5; //SubPixel 

							// ȭ�� ���÷��� ���� 
							if (k == 4 ) {PatternLine[0].FP0.x = StaPatern.x; PatternLine[0].FP0.y = StaPatern.y;}
							if (k == 18) {PatternLine[0].FP1.x = EndPatern.x; PatternLine[0].FP1.y = EndPatern.y;}
						}
					}


					// Bright_Defect�˻� ������ ����(�ڸ��˻�)
					// MOLED ���������� �ʿ������..Ȥ�� ���� �ϴ� ����!!
					if (CamId==Cam_Top && BrokenThresHold != 0) {
						if (AllTopInEdgeCount != 0 && AllTopOutEdgeCount != 0) {
							GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
							GrindMarkOutCnt = (int)(AllTopOutEdgeCount / 2);
							// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
							if (GrindMarkOutCnt != 0) {
								GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
								for (int k = 0; k < AllTopInEdgeCount; k++) {
									if (AllTopInData[k].x == 0.0f || AllTopInData[k].y == 0.0f) continue;
									if (AllTopOutData[GrindMarkOutCnt].x == 0.0f || AllTopOutData[GrindMarkOutCnt].y == 0.0f) continue;

									GrindMeaDis = Math.GetLength(AllTopOutData[GrindMarkOutCnt], AllTopInData[k]);
									if (GrindMeaDis < MinDis) {
										MinDis = GrindMeaDis;
										GrindMarkInCnt = k;
									}
								}
							}
						}

						// Bright_Defect�˻� �̹��� ���� 
						BrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &BrightDefectImg);
						EasyImage::Threshold(GImage, &BrightDefectImg, BrokenThresHold);
						Edge->GImage = &BrightDefectImg;
						Edge->FImgHeight = ImageHeight;
						Edge->SetEdgeType(0);

						int DvCnt = 0, DataCount = 0;
						double deg = 0.0f;
						TFPoint Target(0.0f, 0.0f);

						if (GrindMarkInCnt > GrindMarkOutCnt) {
							DvCnt = GrindMarkInCnt - GrindMarkOutCnt;
							for (int k = 0; k < AllTopOutEdgeCount; k++) {
								deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
								if (((k + DvCnt) < AllTopInEdgeCount) && AllTopInData[k + DvCnt].x > 0 && AllTopInData[k + DvCnt].y > 0) {
									deg = Math.GetDegree(AllTopOutData[k], AllTopInData[k + DvCnt]);

									Target = Math.OnFindPerpendicularPointXY(AllTopOutData[k], dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
									//Edge�˰��� ��ü �̹��� ��ǥ �������� EdgePoint������, �̷����ؼ� Frame�������� �ٽ� ����ؾ��� 
									Edge->OnExecute(AllTopOutData[k], Target); // �����ܰ����ο��� Target���� White Point Search 
									if (Edge->GetResultEdgePointCount() > 0) {
										TopCircleData[DataCount].Dis = 0.0f;
										TopCircleData[DataCount].Dv = 0.0f;
										TopCircleData[DataCount].P = Edge->GetResultEdgePoint(0);
										TopCircleData[DataCount].P.y = TopCircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
										DataCount++;
									}
								}
							}
							TopInCrackCount = DataCount;
						}
						else if (GrindMarkInCnt < GrindMarkOutCnt) {
							DvCnt = GrindMarkOutCnt - GrindMarkInCnt;
							for (int k = DvCnt; k < AllTopOutEdgeCount; k++) {
								deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
								if (((k - DvCnt) < AllTopInEdgeCount) && AllTopInData[k - DvCnt].x > 0 && AllTopInData[k - DvCnt].y > 0 && k - DvCnt >= 0) {
									deg = Math.GetDegree(AllTopOutData[k], AllTopInData[k - DvCnt]);

									Target = Math.OnFindPerpendicularPointXY(AllTopOutData[k], dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
									//Edge�˰��� ��ü �̹��� ��ǥ �������� EdgePoint������, �̷����ؼ� Frame�������� �ٽ� ����ؾ��� 
									Edge->OnExecute(AllTopOutData[k], Target); // �����ܰ����ο��� Target���� White Point Search 
									if (Edge->GetResultEdgePointCount() > 0) {
										TopCircleData[DataCount].Dis = 0.0f;
										TopCircleData[DataCount].Dv = 0.0f;
										TopCircleData[DataCount].P = Edge->GetResultEdgePoint(0);
										TopCircleData[DataCount].P.y = TopCircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
										DataCount++;
									}
								}
							}
							TopInCrackCount = DataCount;
						}
					}
				}
				else { // �κп���==>Corner_Cut, Circle_Cut, Rect_Cut 
					if (FLeftTop) { // �κп����ϰ�� FLeftTop �˻� ������ 
						OrgPoint = Math.OnIntersect(Line[waLeftWall].FP0, Line[waLeftWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
						FLTPoint = OrgPoint;

						if (CamId==Cam_Top) {
							if (FP0.x != 0.0f && FP0.y != 0.0f && Line[1].FP0.x != 0.0f && Line[1].FP0.y != 0.0f && Line[1].FP1.x != 0.0f && Line[1].FP1.y != 0.0f) {
								FP0 = Math.OnRotate(CenterPoint, FP0, -ImgDegree);
								LeftMarkToTopHeight = GetLengthFrLineToPoint(Line[1].FP0, Line[1].FP1, FP0);
							}
							// Right Mark To TopSide Height 
							if (FP1.x != 0.0f && FP1.y != 0.0f && Line[1].FP0.x != 0.0f && Line[1].FP0.y != 0.0f && Line[1].FP1.x != 0.0f && Line[1].FP1.y != 0.0f) {
								FP1 = Math.OnRotate(CenterPoint, FP1, -ImgDegree);
								RightMarkToTopHeight = GetLengthFrLineToPoint(Line[1].FP0, Line[1].FP1, FP1);
							}
							// Center Mark To TopSide Height
							if (LeftMarkToTopHeight != 0.0f && RightMarkToTopHeight != 0.0f) {
								CenMarkToTopHeight = ((FP0.y + FP1.y) / 2.0) - ((Line[1].FP0.y + Line[1].FP1.y) / 2.0);
							}
						}

						if (IsCornerTop) { //�ڳ� ������ �����Ǿ� ������� �ڳ� �˻�������(Dx,Dy,R,GW,GM ������)
							EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindOutThresHold);
							if (CorGrindOutThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
							else                           { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

							// Corner_Cut���� 3Point vs GrindMark �����ؾ���
							IsGrindMarkUse = false;
							for (int k = 0; k < 4; k++) {
								if (AGrindMark[k].DvX != 0 && AGrindMark[k].DvY != 0) { IsGrindMarkUse = true; break; }
							}

							if (!IsGrindMarkUse) { // 3Point �������� ������ ���� ��� ���� ������(GrindMark �̻��)==>C1GrindWd, C1GrindMe ����
								// Corner ���� �ܰ����� 
								SingleLine->IRoi->SetOrgX((int)OrgPoint.x - 10);
								SingleLine->IRoi->SetOrgY((int)OrgPoint.y);
								SingleLine->IRoi->SetWidth((int)LTCornerWidth + 20);
								SingleLine->IRoi->SetHeight((int)LTCornerHeight);

								SingleLine->SetOrientation(orWest);
								SingleLine->FIncrement = 1;
								SingleLine->FThreshold = 15;
								SingleLine->SetFindLineId(0);
								SingleLine->SetEdgeType(1);  //White To Black 
								SingleLine->OnExecute();
								Line[4].FP0 = SingleLine->GetResultP0();
								Line[4].FP1 = SingleLine->GetResultP1();

								// ORG ������ 
								FLTLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
								FLTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall].FP0, Line[waTopWall].FP1);

								if ((FLTLPoint.x == 0 || FLTLPoint.y == 0 || FLTTPoint.x == 0 || FLTTPoint.y == 0) || (SingleLine->FEdgePointCount < 30)) {
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
									if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
										if (C1Point != NULL) delete[] C1Point;
										C1Point = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
										C1EdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

										CornerCount = 0;
										for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
											if (CornerCount >= MAX_CORNER_POINT) break;
											C1Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C1Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									double Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;
									if (CorGrindInThresHold != 0) { // Corner InLine Search Sequence 
										EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindInThresHold);
										SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;
										// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
										// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
										// if(LTCornerWidth >(LTCornerHeight*3)){
										if (LTCornerWidth > (LTCornerHeight * 1.5)) {
											SingleLine->IRoi->SetOrgX((int)FLTPoint.x);
											SingleLine->IRoi->SetOrgY((int)FLTPoint.y);
											SingleLine->IRoi->SetWidth((int)((FLTTPoint.x - FLTLPoint.x + 5)));
											SingleLine->IRoi->SetHeight((int)((FLTLPoint.y - FLTTPoint.y + 10)));

											if (FLTPoint.x <= 0 || FLTPoint.y <= 0 || (FLTTPoint.x - FLTLPoint.x) <= 0 || (FLTLPoint.y - FLTTPoint.y) <= 0) return false;
											if (FLTPoint.x >= FrameImgW || FLTPoint.y >= FrameImgH || (FLTTPoint.x - FLTLPoint.x) >= FrameImgW || (FLTLPoint.y - FLTTPoint.y) >= FrameImgH) return false;

											SingleLine->SetOrientation(orNorth);
											SingleLine->FIncrement = 2;
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C1Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C1InPoint != NULL) delete[] C1InPoint;
												C1InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C1InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
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
											SingleLine->FIncrement = 1;
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C1Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C1InPoint != NULL) delete[] C1InPoint;
												C1InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C1InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C1InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C1InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}

										//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
										FLTLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
										FLTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall].FP0, Line[waTopWall].FP1);

										//�ڳ� ������ 
										Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;
										for (int k = 0; k < C1InEdgeCount; k++) {
											Dis = Math.GetLengthFrLineToPoint(FLTLPoint, FLTTPoint, C1InPoint[k]);
											TotalGrindWidth += Dis;
										}
										if (C1InEdgeCount != 0) {
											AvgGrindWidth = (TotalGrindWidth / C1InEdgeCount);
											C1CorGrindWd = AvgGrindWidth;
										}
									}
									// Corner Grind In End 

									if (CorGrindMeaThresHold != 0) { // Corner Grind ThresHold���� �����Ǿ� ������츸 ������ �˻縦 �����Ѵ� 
										EasyImage::Threshold(&RotateImg, &RotateGrindBinaryImg, CorGrindMeaThresHold);
										// ROI ���� ����(3���� ��� ���� ���� ������ ã�´�)
										if (CorGrindMeaThresHold == 0) { SingleLine->GImage = &RotateImg           ;  SingleLine->FThreshold = 5; }
										else                           { SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold = 10;}

										// ���� ������....�ϴ� ������ �ޱ������� �ӽ÷� ������ �з��Ͽ� CornerEdgePoint ã�� 
										if (LTCornerWidth > (LTCornerHeight * 1.5)) {
											SingleLine->IRoi->SetOrgX((int)OrgPoint.x);
											SingleLine->IRoi->SetOrgY((int)OrgPoint.y + 30);
											SingleLine->IRoi->SetWidth(LTCornerWidth - 50);
											SingleLine->IRoi->SetHeight((int)LTCornerHeight - 40);

											SingleLine->SetOrientation(orNorth);
											SingleLine->SetFindLineId(0);
											SingleLine->FIncrement = 1;
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// C1Corner Grind Measure Stand Data 
											if ((SingleLine->FEdgePointCount) > 0) {
												if (C1MeaPoint != NULL) delete[] C1MeaPoint;
												C1MeaPoint = new TFPoint[(SingleLine->FEdgePointCount)];
												C1MeaEdgeCount = (SingleLine->FEdgePointCount);

												CornerCount = 0;
												for (int k = 0; k < (SingleLine->FEdgePointCount); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													if (k == 0                 ) CornerP1 = SingleLine->FEdgePoint[k]; //Chamber StaPos
													if (k == C1MeaEdgeCount - 1) CornerP2 = SingleLine->FEdgePoint[k]; //Chamber EndPos
													C1MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C1MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
											FLTLMeaPoint = Math.OnIntersect(CornerP1, CornerP2, Line[waLeftWall].FP0, Line[waLeftWall].FP1);
											FLTTMeaPoint = Math.OnIntersect(CornerP1, CornerP2, Line[waTopWall].FP0, Line[waTopWall].FP1);


											//�ڳ� ������ ���  
											Dis = 0.0f, AvgGrindMea = 0.0f, TotalGrindMeasure = 0.0f;
											for (int k = 0; k < C1EdgeCount; k++) {
												Dis = Math.GetLengthFrLineToPoint(FLTLMeaPoint, FLTTMeaPoint, C1Point[k]);
												TotalGrindMeasure += Dis;
											}
											if (C1EdgeCount != 0) {
												AvgGrindMea = (TotalGrindMeasure / C1EdgeCount);
												C1CorGrindMea = AvgGrindMea;
											}
										}
										else {
											SingleLine->IRoi->SetOrgX((int)OrgPoint.x + 15);
											SingleLine->IRoi->SetOrgY((int)OrgPoint.y + 15);
											SingleLine->IRoi->SetWidth((int)LTCornerWidth);
											SingleLine->IRoi->SetHeight((int)LTCornerHeight);

											SingleLine->SetOrientation(orWest);
											SingleLine->SetFindLineId(0);
											SingleLine->FIncrement = 1;
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();


											// C1Corner Grind Measure Stand Data 
											if ((SingleLine->FEdgePointCount) > 0) {
												if (C1MeaPoint != NULL) delete[] C1MeaPoint;
												C1MeaPoint = new TFPoint[(SingleLine->FEdgePointCount)];
												C1MeaEdgeCount = (SingleLine->FEdgePointCount);

												CornerCount = 0;
												for (int k = 0; k < (SingleLine->FEdgePointCount); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C1MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C1MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}

											//�ڳ� ������ ���� ���ΰ� ���ο� ������ ���� 
											FLTLMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
											FLTTMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall].FP0, Line[waTopWall].FP1);

											//�ڳ� ������ ���  
											Dis = 0.0f, AvgGrindMea = 0.0f, TotalGrindMeasure = 0.0f;
											for (int k = 0; k < C1EdgeCount; k++) {
												Dis = Math.GetLengthFrLineToPoint(FLTLMeaPoint, FLTTMeaPoint, C1Point[k]);
												TotalGrindMeasure += Dis;
											}
											if (C1EdgeCount != 0) {
												AvgGrindMea = (TotalGrindMeasure / C1EdgeCount);
												C1CorGrindMea = AvgGrindMea;
											}
										}
									}
								}
							}
							else { // ���ڰ� ��ũ ���� ������,������ ���� ������ 
								// Corner ���� �ܰ����� 
								SingleLine->IRoi->SetOrgX((int)OrgPoint.x - 10);
								SingleLine->IRoi->SetOrgY((int)OrgPoint.y);
								SingleLine->IRoi->SetWidth((int)LTCornerWidth + 20);
								SingleLine->IRoi->SetHeight((int)LTCornerHeight);

								SingleLine->SetOrientation(orWest);
								SingleLine->FIncrement = 1;
								SingleLine->FThreshold = 15;
								SingleLine->SetFindLineId(0);
								SingleLine->SetEdgeType(1);  //White To Black 
								SingleLine->OnExecute();

								Line[4].FP0 = SingleLine->GetResultP0();
								Line[4].FP1 = SingleLine->GetResultP1();

								// ORG ������ 
								FLTLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
								FLTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall].FP0, Line[waTopWall].FP1);

								if ((FLTLPoint.x == 0 || FLTLPoint.y == 0 || FLTTPoint.x == 0 || FLTTPoint.y == 0) || (SingleLine->FEdgePointCount < 30)) {
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
									if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
										if (C1Point != NULL) {
											delete[] C1Point;
										}
										C1Point = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
										C1EdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

										CornerCount = 0;
										for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
											if (CornerCount >= MAX_CORNER_POINT) break;
											C1Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C1Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									double Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;
									// Corner Grind In Gray Level ������ ���ؼ� Corner Grind Out/In ThresHold���� �з��ϴ� 
									// Corner Grind In Start 
									if (CorGrindInThresHold != 0) { // Corner InLine Search Sequence 
										EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindInThresHold);
										SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;

										// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
										// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
										// if(LTCornerWidth >(LTCornerHeight*3)){
										if (LTCornerWidth > (LTCornerHeight * 1.5)) {
											SingleLine->IRoi->SetOrgX((int)FLTPoint.x);
											SingleLine->IRoi->SetOrgY((int)FLTPoint.y);
											SingleLine->IRoi->SetWidth((int)((FLTTPoint.x - FLTLPoint.x + 5)));
											SingleLine->IRoi->SetHeight((int)((FLTLPoint.y - FLTTPoint.y + 10)));

											if (FLTPoint.x <= 0 || FLTPoint.y <= 0 || (FLTTPoint.x - FLTLPoint.x) <= 0 || (FLTLPoint.y - FLTTPoint.y) <= 0) return false;
											if (FLTPoint.x >= FrameImgW || FLTPoint.y >= FrameImgH || (FLTTPoint.x - FLTLPoint.x) >= FrameImgW || (FLTLPoint.y - FLTTPoint.y) >= FrameImgH) return false;

											SingleLine->SetOrientation(orNorth);
											SingleLine->FIncrement = 2;
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C1Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C1InPoint != NULL) delete[] C1InPoint;
												C1InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C1InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
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
											SingleLine->FIncrement = 1;
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C1Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C1InPoint != NULL) {
													delete[] C1InPoint;
												}
												C1InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C1InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C1InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C1InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}

										//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
										FLTLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
										FLTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall].FP0, Line[waTopWall].FP1);
									}

									if(CamId==Cam_Top){
										for (int i = 0; i < 4; i++) { //�κп��� 
											if (AGrindMark[i].DvX == 0 || AGrindMark[i].DvY == 0) continue;
											// �̹��� ȸ�� ���� ���� ��ǥ ȸ�� 
											OrgMarkDv.x = (float)AGrindMark[i].DvX; OrgMarkDv.y = (float)AGrindMark[i].DvY;
											deg         = dASideDeg; //Train ����� ���ذ��� 
											NewMarkDv   = Math.OnRotate(FLTPoint, OrgMarkDv, deg);
											AGrindMark[i].DvX = (int)NewMarkDv.x; AGrindMark[i].DvY = (int)NewMarkDv.y;

											//ROI�������� 
											AGrindMark[i].Rect.left   = (FLTPoint.x + AGrindMark[i].DvX) - AGrindMark[i].Range;
											AGrindMark[i].Rect.top    = (FLTPoint.y + AGrindMark[i].DvY) - AGrindMark[i].Range;
											AGrindMark[i].Rect.right = (AGrindMark[i].Rect.left) + (AGrindMark[i].Range*2);
											AGrindMark[i].Rect.bottom = (AGrindMark[i].Rect.top) + (AGrindMark[i].Range*2);
										}
										for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
											if (k >= MAX_GRINDMARK_ID) continue;
											MarkFind[k].OnLoad(true, FindDir, k);
											MarkFind[k].GImage = &RotateImg;
											MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
											MarkFind[k].SetMinScore(GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
											MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
											MarkFind[k].SetMaxInstances(1);
											MarkFind[k].SetAngleTolerance(20.0f);
											MarkFind[k].SetScaleBias(1.00f);
											MarkFind[k].SetScaleTolerance(0.0f);
											MarkFind[k].Property.Ox = (float)FrameImgW / 2;
											MarkFind[k].Property.Oy = (float)FrameImgH / 2;
										}

										for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) {
											if (AGrindMark[i].Rect.left == 0 || AGrindMark[i].Rect.top == 0 || AGrindMark[i].Rect.right == 0 || AGrindMark[i].Rect.bottom == 0) continue;
											GrindRectWidth  = (AGrindMark[i].Rect.right - AGrindMark[i].Rect.left);
											GrindRectHeight = (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top);

											if (AGrindMark[i].Rect.left <= 0         || AGrindMark[i].Rect.top <= 0         || GrindRectWidth <= 0         || GrindRectHeight <= 0        ) continue;
											if (AGrindMark[i].Rect.left >= FrameImgW || AGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;
											if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) continue;

											MarkID = AGrindMark[i].MarkID; // Multi Teaching MarkID 
											deg    = AGrindMark[i].degree;

											MarkFind[MarkID].SetAngleBias(deg);
											MarkFind[MarkID].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
											AGrindMark[i].Score = 0.0f;

											if (MarkFind[MarkID].OnExecute()) { //GrindMark Search OK 
												AGrindMark[i].Point.x = MarkFind[MarkID].Result[0].Px;
												AGrindMark[i].Point.y = MarkFind[MarkID].Result[0].Py;
												AGrindMark[i].Score   = MarkFind[MarkID].Result[0].Score;
											}
											else {
												MaxScore = 0.0f;
												for (int j = 0; j < MAX_GRINDMARK_ID; j++) {
													MarkFind[j].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
													MarkFind[j].SetAngleBias(deg);

													if (MarkFind[j].OnExecute() && MarkFind[j].Result[0].Score > MaxScore) { //GrindMark Search OK 
														AGrindMark[i].Point.x = MarkFind[j].Result[0].Px;
														AGrindMark[i].Point.y = MarkFind[j].Result[0].Py;
														AGrindMark[i].Score = MarkFind[j].Result[0].Score;
														MaxScore = MarkFind[j].Result[0].Score;
													}
												}
											}
											//20230320 HSKIM

										   // GrindMakrã����� ������,������ ������
											if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) {
												// GrindMark���� OutData �ִܰŸ� Count���� Calc 
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
												for (int k = 0; k < C1EdgeCount; k++) {
													if (C1Point[k].x == 0.0f || C1Point[k].y == 0.0f) continue;
													if (AGrindMark[i].Point.x == 0.0f || AGrindMark[i].Point.y == 0.0f) continue;

													GrindMeaDis = Math.GetLength(AGrindMark[i].Point, C1Point[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkOutCnt = k;
													}
												}
												// OutData���� InData �ִܰŸ� Count���� Calc 
												if (GrindMarkOutCnt != 0) {
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
													for (int k = 0; k < C1InEdgeCount; k++) {
														if (C1InPoint[k].x == 0.0f || C1InPoint[k].y == 0.0f) continue;
														if (C1Point[GrindMarkOutCnt].x == 0.0f || C1Point[GrindMarkOutCnt].y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(C1Point[GrindMarkOutCnt], C1InPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkInCnt = k;
														}
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
												if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
													for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
														if (C1Point[k].x == 0.0f || C1Point[k].y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(AGrindMark[i].Point, C1Point[k]);
														TotalCirDis += GrindMeaDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														AGrindMark[i].GrindMea = TotalCirDis / CircleCount;
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
												if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
													for (int k = -5; k < 5; k++) {
														if (C1Point[GrindMarkOutCnt + k].x == 0.0f && C1Point[GrindMarkOutCnt + k].y == 0.0f) continue;
														if (C1InPoint[GrindMarkInCnt + k].x == 0.0f && C1InPoint[GrindMarkInCnt + k].y == 0.0f) continue;
														GrindWidthDis = Math.GetLength(C1InPoint[GrindMarkInCnt + k], C1Point[GrindMarkOutCnt + k]);
														TotalCirDis += GrindWidthDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
													}
												}
											}
											else { // GrindMrk ��ã����� �������� ������ 
												 // OutData���� �ּҰ� ������ Count���� 
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
												for (int k = 0; k < C1EdgeCount; k++) {
													if (C1Point[k].x == 0.0f || C1Point[k].y == 0.0f) continue;

													AGrindMark[i].RectPoint.x = AGrindMark[i].Rect.left + (AGrindMark[i].Rect.right - AGrindMark[i].Rect.left) / 2;
													AGrindMark[i].RectPoint.y = AGrindMark[i].Rect.top + (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top) / 2;

													GrindMeaDis = Math.GetLength(AGrindMark[i].RectPoint, C1Point[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkOutCnt = k;
													}
												}

												// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
												if (GrindMarkOutCnt != 0) {
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
													for (int k = 0; k < C1InEdgeCount; k++) {
														if (C1InPoint[k].x == 0.0f || C1InPoint[k].y == 0.0f) continue;
														if (C1Point[GrindMarkOutCnt].x == 0.0f || C1Point[GrindMarkOutCnt].y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(C1Point[GrindMarkOutCnt], C1InPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkInCnt = k;
														}
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
												if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
													for (int k = -5; k < 5; k++) {
														if (C1Point[GrindMarkOutCnt + k].x == 0.0f && C1Point[GrindMarkOutCnt + k].y == 0.0f) continue;
														if (C1InPoint[GrindMarkInCnt + k].x == 0.0f && C1InPoint[GrindMarkInCnt + k].y == 0.0f) continue;
														GrindWidthDis = Math.GetLength(C1InPoint[GrindMarkInCnt + k], C1Point[GrindMarkOutCnt + k]);
														TotalCirDis += GrindWidthDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
													}
												}
											}
										}
									}
								}
							}
						}
						else if (IsRectTop) { // �ڳ� �������� ������� ������ ���� �˻� Rect�˻�  
							LTDx = 0.0f;
							LTDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FLTLPoint.x = FLTPoint.x;
							FLTLPoint.y = FLTPoint.y;
							FLTTPoint.x = FLTPoint.x;
							FLTTPoint.y = FLTPoint.y;
						}
						else if (IsCircleTop) { // TOP���� LeftTop Circle �˻� 
							EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadSideThresHold);
							if (PadSideThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
							else                       { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

							GrindRectWidth = 0;
							GrindRectHeight = 0;
							// Find Mark Search Start
							////////////////////////////////////////////////////////////////////////////
							// NOTCH LeftTop 
							if (FInspectMode == Insp_Polygon) { // ���� � �˰��� 
								if (LTCircleWidth != 0 && LTCircleHeight != 0) {
									P1.x =0, P1.y=0, P2.x=0, P2.y=0, P3.x=0, P3.y=0, P4.x=0, P4.y=0;

									P1.x = (int)(FLTPoint.x); P1.y = (int)(FLTPoint.y);            P2.x = (int)(P1.x + LTCircleWidth); P2.y = (int)(P1.y);
									P3.x = (int)(P1.x);       P3.y = (int)(P1.y + LTCircleHeight); P4.x = (int)(P2.x);                 P4.y = (int)(P3.y);

									// �˻�ȭ�� Display ���� 
									PadPolyRect[0].left  = P1.x;   PadPolyRect[0].top    = P1.y;
									PadPolyRect[0].right = P2.x;   PadPolyRect[0].bottom = P3.y;
									int PixelCnt = 0;

									// Median Filter ���� 
									if (P1.x <= 0 || P1.y <= 0 || (P2.x - P1.x) <= 0 || (P3.y - P1.y) <= 0) return false;
									if (P1.x >= FrameImgW || P1.y >= FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) >= FrameImgH) return false;
									if ((P1.x + (P2.x - P1.x)) >= FrameImgW || (P1.y + (P3.y - P1.y)) > FrameImgH) return false;

									FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
									FilterROI.Attach(&RotateBinaryImg);
									FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
									LogUnit.SetLog(L"LeftTop ==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
									//LogUnit.SetLog(L"LeftTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
//									EasyImage::Median(&FilterROI, &FilterROI);
									OnFilter(ftMedian, &FilterROI, &FilterROI);

									Polygon->GImage          = &RotateBinaryImg;
									Polygon->IWidth          = FrameImgW;
									Polygon->IHeight         = FrameImgH;
									Polygon->PoPa.FMinWidth  = 5;
									Polygon->PoPa.FMaxWidth  = (int)(P2.x - P1.x);
									Polygon->PoPa.FMinHeight = (int)((P3.y - P1.y) / 2);
									Polygon->PoPa.FMaxHeight = (int)((P3.y - P1.y) + 10);
									Polygon->PoPa.MinPxCnt   = 100;
									Polygon->PoPa.MaxPxCnt   = 20000;
									Polygon->PoPa.MaxBbCnt   = 10;
									Polygon->PoPa.IsSeparate = true;
									Polygon->PoPa.IsInToOut  = false;
									Polygon->PoPa.HiThHold   = PadSideThresHold;
									Polygon->PoPa.LoThHold   = 0;
									Polygon->PoPa.IgInSide   = true;
									Polygon->PoPa.IsHorSort  = true;
									Polygon->PoPa.IsVerSort  = false;
									Polygon->PoPa.IsInToLeft = true;

									if (P1.x != 0 && P1.y != 0 && P2.x != 0 && P2.y != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
										Polygon->OnExecute(P1, P2, P3, P4);
									}

									// Polygon Algorithm InEdgeData/OutEdgeData Separate 
									if (Polygon->BlobCount == 1) {
										PolygonMarginCnt = (CamId==Cam_Top) ? 50 : 20;
										if (Polygon->Blob[0].InCnt > (PolygonMarginCnt * 2)) {
											A1CirInEdgeCount = 0;
											A1CirInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt * 2));
											if (A1CirInEdgeCount > 0) {
												if (A1CirInPoint != NULL) delete[] A1CirInPoint;
												A1CirInPoint = new TFPoint[A1CirInEdgeCount];

												PixelCnt = 0;
												for (int k = 0; k < Polygon->Blob[0].InCnt; k++) {
													if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].InCnt - PolygonMarginCnt)) continue;
													A1CirInPoint[PixelCnt].x = Polygon->Blob[0].InPoint[k].x;
													A1CirInPoint[PixelCnt].y = Polygon->Blob[0].InPoint[k].y;
													PixelCnt++;
												}
											}
										}

										PolygonMarginCnt = (CamId==Cam_Top) ? 5 : 20;
										if (Polygon->Blob[0].OutCnt > (PolygonMarginCnt * 2)) {
											A1CirOutEdgeCount = 0;
											A1CirOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt * 2));
											if (A1CirOutEdgeCount > 0) {
												if (A1CirOutPoint != NULL) delete[] A1CirOutPoint;
												A1CirOutPoint = new TFPoint[A1CirOutEdgeCount];

												PixelCnt = 0;
												for (int k = 0; k < Polygon->Blob[0].OutCnt; k++) {
													if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].OutCnt - PolygonMarginCnt)) continue;
													A1CirOutPoint[PixelCnt].x = Polygon->Blob[0].OutPoint[k].x;
													A1CirOutPoint[PixelCnt].y = Polygon->Blob[0].OutPoint[k].y;
													PixelCnt++;
												}
											}
										}
									}

									// InEdgeData/OutEdgeData ���� ������ �� ������ ���
									// Polygon �˰��򿡼��� GrindMark���� OutData,InData �ִܰŸ� 2�� Tag�� �����ؾ��� 
									// �κп����� ==> LTCPS(Polygon, GrindMark), EnCap(Polygon,Avg����)
									if (CamId==Cam_Top) { //LTPS Circle (�κп���)
										for (int i = 0; i < 4; i++) {
											if (AGrindMark[i].DvX == 0 || AGrindMark[i].DvY == 0) continue;
											// �̹��� ȸ�� ���� ���� ��ǥ ȸ�� 
											OrgMarkDv.x = (float)AGrindMark[i].DvX; OrgMarkDv.y = (float)AGrindMark[i].DvY;
											deg         = dASideDeg; //Train ����� ���ذ��� 
											NewMarkDv   = Math.OnRotate(FLTPoint, OrgMarkDv, deg);
											AGrindMark[i].DvX = (int)NewMarkDv.x; AGrindMark[i].DvY = (int)NewMarkDv.y;

											//ROI�������� 
											AGrindMark[i].Rect.left   = (FLTPoint.x + AGrindMark[i].DvX) - AGrindMark[i].Range;
											AGrindMark[i].Rect.top    = (FLTPoint.y + AGrindMark[i].DvY) - AGrindMark[i].Range;
											AGrindMark[i].Rect.right  = (AGrindMark[i].Rect.left) + (AGrindMark[i].Range*2);
											AGrindMark[i].Rect.bottom = (AGrindMark[i].Rect.top)  + (AGrindMark[i].Range*2);
										}
										for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
											if (k >= MAX_GRINDMARK_ID) continue;
											MarkFind[k].OnLoad(true, FindDir, k);
											MarkFind[k].GImage = &RotateImg;
											MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
											MarkFind[k].SetMinScore(GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
											MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
											MarkFind[k].SetMaxInstances(1);
											MarkFind[k].SetAngleTolerance(20.0f);
											MarkFind[k].SetScaleBias(1.00f);
											MarkFind[k].SetScaleTolerance(0.0f);
											MarkFind[k].Property.Ox = (float)FrameImgW / 2;
											MarkFind[k].Property.Oy = (float)FrameImgH / 2;

										}

										for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) {
											if (AGrindMark[i].Rect.left == 0 || AGrindMark[i].Rect.top == 0 || AGrindMark[i].Rect.right == 0 || AGrindMark[i].Rect.bottom == 0) continue;
											GrindRectWidth  = (AGrindMark[i].Rect.right - AGrindMark[i].Rect.left);
											GrindRectHeight = (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top);

											if (AGrindMark[i].Rect.left <= 0         || AGrindMark[i].Rect.top <= 0         || GrindRectWidth <= 0         || GrindRectHeight <= 0) continue;
											if (AGrindMark[i].Rect.left >= FrameImgW || AGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;
											if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) continue;

											MarkID = AGrindMark[i].MarkID; // Multi Teaching MarkID 
											deg = AGrindMark[i].degree;
											MarkFind[MarkID].SetAngleBias(deg);
											MarkFind[MarkID].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
											AGrindMark[i].Score = 0.0f;

											if (MarkFind[MarkID].OnExecute()) { //GrindMark Search OK 
												AGrindMark[i].Point.x = MarkFind[MarkID].Result[0].Px;
												AGrindMark[i].Point.y = MarkFind[MarkID].Result[0].Py;
												AGrindMark[i].Score = MarkFind[MarkID].Result[0].Score;
											}
											else {
												MaxScore = 0.0f;
												for (int j = 0; j < MAX_GRINDMARK_ID; j++) {
													MarkFind[j].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
													MarkFind[j].SetAngleBias(deg);

													if (MarkFind[j].OnExecute() && MarkFind[j].Result[0].Score > MaxScore) { //GrindMark Search OK 
														AGrindMark[i].Point.x = MarkFind[j].Result[0].Px;
														AGrindMark[i].Point.y = MarkFind[j].Result[0].Py;
														AGrindMark[i].Score = MarkFind[j].Result[0].Score;
														MaxScore = MarkFind[j].Result[0].Score;
													}
												}
											}
											//20230320 HSKIM

											if (A1CirOutEdgeCount != 0 && A1CirInEdgeCount != 0) { // MarkMode��� ������ �˻縦 ���� ���� �� ������ ���� 
												// 25��° GrindMark Search �� ����,���� ������ ���
												/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
												if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) {  //GrindMark ã������� 
													// GrindMark���� OutData �ִܰŸ� Count���� Calc 
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
													for (int k = 0; k < A1CirOutEdgeCount; k++) {
														if (A1CirOutPoint[k].x == 0.0f || A1CirOutPoint[k].y == 0.0f) continue;
														if (AGrindMark[i].Point.x == 0.0f || AGrindMark[i].Point.y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(AGrindMark[i].Point, A1CirOutPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkOutCnt = k;
														}
													}
													// GrindMark���� InData �ִܰŸ� Count���� Calc 
													if (GrindMarkOutCnt != 0) {
														GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
														for (int k = 0; k < A1CirInEdgeCount; k++) {
															if (A1CirInPoint[k].x == 0.0f || A1CirInPoint[k].y == 0.0f) continue;
															if (A1CirOutPoint[GrindMarkOutCnt].x == 0.0f || A1CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(A1CirOutPoint[GrindMarkOutCnt], A1CirInPoint[k]);
															if (GrindMeaDis < MinDis) {
																MinDis = GrindMeaDis;
																GrindMarkInCnt = k;
															}
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
													if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
														for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
															if (A1CirOutPoint[k].x == 0.0f || A1CirOutPoint[k].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(AGrindMark[i].Point, A1CirOutPoint[k]);
															TotalCirDis += GrindMeaDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															AGrindMark[i].GrindMea = TotalCirDis / CircleCount;
															AGrindMark[i].GrindMea = AGrindMark[i].GrindMea + 0.5; //SubPixel 
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
													if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
														for (int k = -5; k < 5; k++) {
															if (A1CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && A1CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
															if (A1CirInPoint[GrindMarkInCnt + k].x == 0.0f && A1CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
															GrindWidthDis = Math.GetLength(A1CirInPoint[GrindMarkInCnt + k], A1CirOutPoint[GrindMarkOutCnt + k]);
															TotalCirDis += GrindWidthDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
															AGrindMark[i].GrindWidth = AGrindMark[i].GrindWidth + 0.5; //SubPixel
														}
													}
												}
												else if (AGrindMark[i].Rect.left != 0 && AGrindMark[i].Rect.top != 0 && AGrindMark[i].Rect.right != 0 && AGrindMark[i].Rect.bottom != 0) {  //GrindMark �� ã�������
													// OutData���� �ּҰ� ������ Count���� 
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
													for (int k = 0; k < A1CirOutEdgeCount; k++) {
														if (A1CirOutPoint[k].x == 0.0f || A1CirOutPoint[k].y == 0.0f) continue;

														AGrindMark[i].RectPoint.x = AGrindMark[i].Rect.left + (AGrindMark[i].Rect.right - AGrindMark[i].Rect.left) / 2;
														AGrindMark[i].RectPoint.y = AGrindMark[i].Rect.top + (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top) / 2;

														GrindMeaDis = Math.GetLength(AGrindMark[i].RectPoint, A1CirOutPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkOutCnt = k;
														}
													}

													// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
													if (GrindMarkOutCnt != 0) {
														GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
														for (int k = 0; k < A1CirInEdgeCount; k++) {
															if (A1CirInPoint[k].x == 0.0f || A1CirInPoint[k].y == 0.0f) continue;
															if (A1CirOutPoint[GrindMarkOutCnt].x == 0.0f || A1CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(A1CirOutPoint[GrindMarkOutCnt], A1CirInPoint[k]);
															if (GrindMeaDis < MinDis) {
																MinDis = GrindMeaDis;
																GrindMarkInCnt = k;
															}
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
													if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
														for (int k = -5; k < 5; k++) {
															if (A1CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && A1CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
															if (A1CirInPoint[GrindMarkInCnt + k].x == 0.0f && A1CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
															GrindWidthDis = Math.GetLength(A1CirInPoint[GrindMarkInCnt + k], A1CirOutPoint[GrindMarkOutCnt + k]);
															TotalCirDis += GrindWidthDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
															AGrindMark[i].GrindWidth = AGrindMark[i].GrindWidth + 0.5; //SubPixel
														}
													}
												}
											}
										}
									}
									else { //LTPS���� �������� �ʿ� ������....Ȥ�� ���� ���ܵ�(GrindMark���� �ƴ�, �ܼ� ���������� ���� ��հ� ����) 
										if (A1CirInEdgeCount != 0 && A1CirOutEdgeCount != 0) {
											GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
											GrindMarkOutCnt = (int)(A1CirOutEdgeCount / 2);
											// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
											if (GrindMarkOutCnt != 0) {
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
												for (int k = 0; k < A1CirInEdgeCount; k++) {
													if (A1CirInPoint[k].x == 0.0f || A1CirInPoint[k].y == 0.0f) continue;
													if (A1CirOutPoint[GrindMarkOutCnt].x == 0.0f || A1CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

													GrindMeaDis = Math.GetLength(A1CirOutPoint[GrindMarkOutCnt], A1CirInPoint[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkInCnt = k;
													}
												}
											}

											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
											if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
												CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
												for (int k = -5; k < 5; k++) {
													if (A1CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && A1CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
													if (A1CirInPoint[GrindMarkInCnt + k].x == 0.0f && A1CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
													GrindWidthDis = Math.GetLength(A1CirInPoint[GrindMarkInCnt + k], A1CirOutPoint[GrindMarkOutCnt + k]);
													TotalCirDis += GrindWidthDis;
													CircleCount++;
												}
											}
										}
									}

									// Polygon �ڸ� �˻� ������(LeftTop) 
									// ���� �ʿ������...���߿� ������� �𸣴� �ϴ� ����
									if (CamId==Cam_Top && BrokenThresHold != 0) {
										RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
										EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &RotateBrokenImg);
										EasyImage::Threshold(&RotateImg, &RotateBrokenImg, BrokenThresHold);
										Edge->GImage = &RotateBrokenImg;
										Edge->FImgHeight = ImageHeight;
										Edge->SetEdgeType(0);

										int DvCnt = 0, DataCount = 0;
										double deg = 0.0f;
										TFPoint Target(0.0f, 0.0f);

										if (GrindMarkInCnt > GrindMarkOutCnt) {
											DvCnt = GrindMarkInCnt - GrindMarkOutCnt;
											for (int k = 0; k < A1CirOutEdgeCount; k++) {
												deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
												if (((k + DvCnt) < A1CirInEdgeCount) && A1CirInPoint[k + DvCnt].x > 0 && A1CirInPoint[k + DvCnt].y > 0) {
													deg = Math.GetDegree(A1CirOutPoint[k], A1CirInPoint[k + DvCnt]);

													Target = Math.OnFindPerpendicularPointXY(A1CirOutPoint[k], dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
													Edge->OnExecute(A1CirOutPoint[k], Target); // �����ܰ����ο��� Target���� White Point Search 
													if (Edge->GetResultEdgePointCount() > 0) {
														A1CircleData[DataCount].Dis = 0.0f;
														A1CircleData[DataCount].Dv = 0.0f;
														A1CircleData[DataCount].P = Edge->GetResultEdgePoint(0);
														A1CircleData[DataCount].P.y = A1CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
														DataCount++;
													}
												}
											}
											A1CirInCrackCount = DataCount;
										}
										else if (GrindMarkInCnt < GrindMarkOutCnt) {
											DvCnt = GrindMarkOutCnt - GrindMarkInCnt;
											for (int k = DvCnt; k < A1CirOutEdgeCount; k++) {
												deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
												if (((k - DvCnt) < A1CirInEdgeCount) && A1CirInPoint[k - DvCnt].x > 0 && A1CirInPoint[k - DvCnt].y > 0 && k - DvCnt >= 0) {
													deg = Math.GetDegree(A1CirOutPoint[k], A1CirInPoint[k - DvCnt]);

													Target = Math.OnFindPerpendicularPointXY(A1CirOutPoint[k], dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
													Edge->OnExecute(A1CirOutPoint[k], Target); // �����ܰ����ο��� Target���� White Point Search 
													if (Edge->GetResultEdgePointCount() > 0) {
														A1CircleData[DataCount].Dis = 0.0f;
														A1CircleData[DataCount].Dv = 0.0f;
														A1CircleData[DataCount].P = Edge->GetResultEdgePoint(0);
														A1CircleData[DataCount].P.y = A1CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
														DataCount++;
													}
												}
											}
											A1CirInCrackCount = DataCount;
										}
									}
								}
							}
						}
					}
					if (FRightTop) { // �κп����ϰ�� FRightTop �˻� ������ 
						OrgPoint = Math.OnIntersect(Line[waRightWall].FP0, Line[waRightWall].FP1, Line[waTopWall].FP0, Line[waTopWall].FP1);
						FRTPoint = OrgPoint;

						if (IsCornerTop) {
							EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindOutThresHold);
							if (CorGrindOutThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
							else                           { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

							IsGrindMarkUse = false;
							for (int k = 19; k < 23; k++) {
								if (AGrindMark[k].DvX != 0 && AGrindMark[k].DvY != 0) { IsGrindMarkUse = true; break; }
							}

							if (!IsGrindMarkUse) { // 3Point �������� ������ ���� ��� ���� ������ 
								SingleLine->IRoi->SetOrgX((int)(OrgPoint.x - RTCornerWidth));
								SingleLine->IRoi->SetOrgY((int)OrgPoint.y);
								SingleLine->IRoi->SetWidth(RTCornerWidth + 10);
								SingleLine->IRoi->SetHeight(RTCornerHeight);

								SingleLine->SetOrientation(orEast);
								SingleLine->FIncrement = 1;
								SingleLine->FThreshold = 15;
								SingleLine->SetFindLineId(0);
								SingleLine->SetEdgeType(1);  //White To Black 
								SingleLine->OnExecute();
								Line[waRightTop].FP0 = SingleLine->GetResultP0();
								Line[waRightTop].FP1 = SingleLine->GetResultP1();

								// ORG 
								FRTRPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
								FRTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall].FP0, Line[waTopWall].FP1);

								if ((FRTRPoint.x == 0 || FRTRPoint.y == 0 || FRTTPoint.x == 0 || FRTTPoint.y == 0) || (SingleLine->FEdgePointCount < 30)) {
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
									if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
										if (C2Point != NULL) delete[] C2Point;
										C2Point = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
										C2EdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

										CornerCount = 0;
										for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
											if (CornerCount >= MAX_CORNER_POINT) break;
											C2Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C2Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									// Value Init 
									double Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;

									// Corner Grind In Start 
									if (CorGrindInThresHold != 0) {
										EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindInThresHold);
										SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;

										// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
										// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
										// if(RTCornerWidth >(RTCornerHeight*3)){
										if (RTCornerWidth > (RTCornerHeight * 1.5)) {
											if (FRTTPoint.x != 0.0f && FRTTPoint.y != 0.0f && FRTRPoint.x != 0.0f && FRTRPoint.y != 0.0f) {

												SingleLine->IRoi->SetOrgX((int)(FRTPoint.x - (FRTRPoint.x - FRTTPoint.x) + 5));
												SingleLine->IRoi->SetOrgY((int)FRTPoint.y);
												SingleLine->IRoi->SetWidth((int)((FRTRPoint.x - FRTTPoint.x) + 5));
												SingleLine->IRoi->SetHeight((int)((FRTRPoint.y - FRTTPoint.y) + 10));

												if (FRTTPoint.x <= 0 || FRTTPoint.y <= 0 || (FRTRPoint.x - FRTTPoint.x) <= 0 || (FRTRPoint.y - FRTTPoint.y) <= 0) return false;
												if (FRTTPoint.x >= FrameImgW || FRTTPoint.y >= FrameImgH || (FRTRPoint.x - FRTTPoint.x) >= FrameImgW || (FRTRPoint.y - FRTTPoint.y) >= FrameImgH) return false;

												//SingleLine->SetOrientation(orNorth);
												SingleLine->SetOrientation(orSouth);
												SingleLine->FIncrement = 2;
												SingleLine->SetFindLineId(0);
												//SingleLine->SetEdgeType(0);  //Black To White 
												SingleLine->SetEdgeType(1);  //Black To White 
												SingleLine->OnExecute();

												// InLine C2Corner Data 
												if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
													if (C2InPoint != NULL) delete[] C2InPoint;
													C2InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
													C2InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

													CornerCount = 0;
													for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
														if (CornerCount >= MAX_CORNER_POINT) break;
														C2InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														C2InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
														CornerCount++;
													}
												}
											}
										}
										else {
											// Corner ���� ���ʶ��� ã�´� 
											SingleLine->SetOrientation(orEast);
											SingleLine->SetFindLineId(0);
											SingleLine->FIncrement = 1;
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C2Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C2InPoint != NULL) delete[] C2InPoint;
												C2InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C2InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C2InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C2InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}

										//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
										FRTRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
										FRTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall].FP0, Line[waTopWall].FP1);

										//�ڳ� ������, ������ ��� 
										Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;
										for (int k = 0; k < C2InEdgeCount; k++) {
											Dis = Math.GetLengthFrLineToPoint(FRTRPoint, FRTTPoint, C2InPoint[k]);
											TotalGrindWidth += Dis;
										}
										if (C2InEdgeCount != 0) {
											AvgGrindWidth = (TotalGrindWidth / C2InEdgeCount);
											C2CorGrindWd = AvgGrindWidth;
										}
									}

									if (CorGrindMeaThresHold != 0) { // Corner Grind ThresHold���� �����Ǿ� ������츸 ������ �˻縦 �����Ѵ� 
										EasyImage::Threshold(&RotateImg, &RotateGrindBinaryImg, CorGrindMeaThresHold);
										// ROI ���� ���� 
										if (CorGrindMeaThresHold == 0) { SingleLine->GImage = &RotateImg           ;  SingleLine->FThreshold = 5; }
										else                           { SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold = 10;}

										// ���� ������....�ϴ� ������ �ޱ������� �ӽ÷� ������ �з��Ͽ� CornerEdgePoint ã�� 
										if (RTCornerWidth > (RTCornerHeight * 1.5)) {
											SingleLine->IRoi->SetOrgX((int)OrgPoint.x - (RTCornerWidth - 50));
											SingleLine->IRoi->SetOrgY((int)OrgPoint.y + 30);
											SingleLine->IRoi->SetWidth(RTCornerWidth - 50);
											SingleLine->IRoi->SetHeight(RTCornerHeight - 40);

											SingleLine->SetOrientation(orNorth);
											SingleLine->SetFindLineId(0);
											SingleLine->FIncrement = 1;
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// C1Corner Grind Measure Stand Data 
											if ((SingleLine->FEdgePointCount) > 0) {
												if (C2MeaPoint != NULL) delete[] C2MeaPoint;
												C2MeaPoint = new TFPoint[(SingleLine->FEdgePointCount)];
												C2MeaEdgeCount = (SingleLine->FEdgePointCount);

												CornerCount = 0;
												for (int k = 0; k < (SingleLine->FEdgePointCount); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													if (k == 0                 ) CornerP1 = SingleLine->FEdgePoint[k]; //Chamber StaPos
													if (k == C2MeaEdgeCount - 1) CornerP2 = SingleLine->FEdgePoint[k]; //Chamber EndPos

													C2MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C2MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
											//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
											FRTRMeaPoint = Math.OnIntersect(CornerP1, CornerP2, Line[waRightWall].FP0, Line[waRightWall].FP1);
											FRTTMeaPoint = Math.OnIntersect(CornerP1, CornerP2, Line[waTopWall  ].FP0, Line[waTopWall  ].FP1);

											//�ڳ� ������ ���  
											Dis = 0.0f, AvgGrindMea = 0.0f, TotalGrindMeasure = 0.0f;
											for (int k = 0; k < C2EdgeCount; k++) {
												Dis = Math.GetLengthFrLineToPoint(FRTRMeaPoint, FRTTMeaPoint, C2Point[k]);
												TotalGrindMeasure += Dis;
											}
											if (C2EdgeCount != 0) {
												AvgGrindMea = (TotalGrindMeasure / C2EdgeCount);
												C2CorGrindMea = AvgGrindMea;
											}
										}
										else {
											// 											 SingleLine->IRoi->SetOrgX  ((int) OrgPoint.x-RTCornerWidth    );
											SingleLine->IRoi->SetOrgX((int)OrgPoint.x - RTCornerWidth + 20);
											SingleLine->IRoi->SetOrgY((int)OrgPoint.y + 15);
											SingleLine->IRoi->SetWidth(RTCornerWidth);
											// 											 SingleLine->IRoi->SetHeight(RTCornerHeight                    );
											SingleLine->IRoi->SetHeight(RTCornerHeight + 10);

											SingleLine->SetOrientation(orEast);
											SingleLine->SetFindLineId(0);
											SingleLine->FIncrement = 1;
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// C1Corner Grind Measure Stand Data 
											if ((SingleLine->FEdgePointCount) > 0) {
												if (C2MeaPoint != NULL) delete[] C2MeaPoint;
												C2MeaPoint = new TFPoint[(SingleLine->FEdgePointCount)];
												C2MeaEdgeCount = (SingleLine->FEdgePointCount);

												CornerCount = 0;
												for (int k = 0; k < (SingleLine->FEdgePointCount); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C2MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C2MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}

											//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
											FRTRMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
											FRTTMeaPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall].FP0, Line[waTopWall].FP1);

											//�ڳ� ������ ���  
											Dis = 0.0f, AvgGrindMea = 0.0f, TotalGrindMeasure = 0.0f;
											for (int k = 0; k < C2EdgeCount; k++) {
												Dis = Math.GetLengthFrLineToPoint(FRTRMeaPoint, FRTTMeaPoint, C2Point[k]);
												TotalGrindMeasure += Dis;
											}
											if (C2EdgeCount != 0) {
												AvgGrindMea = (TotalGrindMeasure / C2EdgeCount);
												C2CorGrindMea = AvgGrindMea;
											}
										}
									}
								}
							}
							else { //���ڰ� ��ũ ���� ������,������ ���� ������ 
								SingleLine->IRoi->SetOrgX((int)(OrgPoint.x - RTCornerWidth));
								SingleLine->IRoi->SetOrgY((int)OrgPoint.y);
								SingleLine->IRoi->SetWidth(RTCornerWidth + 10);
								SingleLine->IRoi->SetHeight(RTCornerHeight);

								SingleLine->SetOrientation(orEast);
								SingleLine->FIncrement = 1;
								SingleLine->FThreshold = 15;
								SingleLine->SetFindLineId(0);
								SingleLine->SetEdgeType(1);  //White To Black 
								SingleLine->OnExecute();
								Line[waRightTop].FP0 = SingleLine->GetResultP0();
								Line[waRightTop].FP1 = SingleLine->GetResultP1();

								// ORG 
								FRTRPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
								FRTTPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall].FP0, Line[waTopWall].FP1);

								if ((FRTRPoint.x == 0 || FRTRPoint.y == 0 || FRTTPoint.x == 0 || FRTTPoint.y == 0) || (SingleLine->FEdgePointCount < 30)) {
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
									if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
										if (C2Point != NULL) delete[] C2Point;
										C2Point = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
										C2EdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

										CornerCount = 0;
										for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
											if (CornerCount >= MAX_CORNER_POINT) break;
											C2Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C2Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									// Value Init 
									double Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;

									// Corner Grind In Start 
									if (CorGrindInThresHold != 0) {
										EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindInThresHold);
										SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;

										// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
										// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
										// if(RTCornerWidth >(RTCornerHeight*3)){
										if (RTCornerWidth > (RTCornerHeight * 1.5)) {
											if (FRTTPoint.x != 0.0f && FRTTPoint.y != 0.0f && FRTRPoint.x != 0.0f && FRTRPoint.y != 0.0f) {

												SingleLine->IRoi->SetOrgX((int)(FRTPoint.x - (FRTRPoint.x - FRTTPoint.x) + 5));
												SingleLine->IRoi->SetOrgY((int)FRTPoint.y);
												SingleLine->IRoi->SetWidth((int)((FRTRPoint.x - FRTTPoint.x) + 5));
												SingleLine->IRoi->SetHeight((int)((FRTRPoint.y - FRTTPoint.y) + 10));

												if (FRTTPoint.x <= 0 || FRTTPoint.y <= 0 || (FRTRPoint.x - FRTTPoint.x) <= 0 || (FRTRPoint.y - FRTTPoint.y) <= 0) return false;
												if (FRTTPoint.x >= FrameImgW || FRTTPoint.y >= FrameImgH || (FRTRPoint.x - FRTTPoint.x) >= FrameImgW || (FRTRPoint.y - FRTTPoint.y) >= FrameImgH) return false;

												SingleLine->SetOrientation(orNorth);
												SingleLine->FIncrement = 2;
												SingleLine->SetFindLineId(0);
												SingleLine->SetEdgeType(0);  //Black To White 
												SingleLine->OnExecute();

												// InLine C2Corner Data 
												if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
													if (C2InPoint != NULL) delete[] C2InPoint;
													C2InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
													C2InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

													CornerCount = 0;
													for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
														if (CornerCount >= MAX_CORNER_POINT) break;
														C2InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
														C2InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
														CornerCount++;
													}
												}
											}
										}
										else {
											// Corner ���� ���ʶ��� ã�´� 
											SingleLine->SetOrientation(orEast);
											SingleLine->SetFindLineId(0);
											SingleLine->FIncrement = 1;
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C2Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C2InPoint != NULL) delete[] C2InPoint;
												C2InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C2InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C2InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C2InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}

										//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
										FRTRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
										FRTTInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waTopWall].FP0, Line[waTopWall].FP1);
									}

									if(CamId==Cam_Top){
										for (int i = 19; i < 23; i++) {
											if (AGrindMark[i].DvX == 0 || AGrindMark[i].DvY == 0) continue;
											// �̹��� ȸ�� ���� ���� ��ǥ ȸ�� 
											OrgMarkDv.x = (float)AGrindMark[i].DvX; OrgMarkDv.y = (float)AGrindMark[i].DvY;
											deg = dASideDeg; //Train ����� ���ذ��� 
											NewMarkDv = Math.OnRotate(FLTPoint, OrgMarkDv, deg);
											AGrindMark[i].DvX = (int)NewMarkDv.x; AGrindMark[i].DvY = (int)NewMarkDv.y;

											//ROI�������� 
											AGrindMark[i].Rect.left   = (FLTPoint.x + AGrindMark[i].DvX) - AGrindMark[i].Range;
											AGrindMark[i].Rect.top    = (FLTPoint.y + AGrindMark[i].DvY) - AGrindMark[i].Range;
											AGrindMark[i].Rect.right  = (AGrindMark[i].Rect.left) + (AGrindMark[i].Range*2);
											AGrindMark[i].Rect.bottom = (AGrindMark[i].Rect.top ) + (AGrindMark[i].Range*2);
										}

										for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
											if (k >= MAX_GRINDMARK_ID) continue;
											MarkFind[k].OnLoad(true, FindDir, k);
											MarkFind[k].GImage = &RotateImg;
											MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
											MarkFind[k].SetMinScore(GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
											MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
											MarkFind[k].SetMaxInstances(1);
											MarkFind[k].SetAngleTolerance(20.0f);
											MarkFind[k].SetScaleBias(1.00f);
											MarkFind[k].SetScaleTolerance(0.0f);
											MarkFind[k].Property.Ox = (float)FrameImgW / 2;
											MarkFind[k].Property.Oy = (float)FrameImgH / 2;
										}

										for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) {
											if (AGrindMark[i].Rect.left == 0 || AGrindMark[i].Rect.top == 0 || AGrindMark[i].Rect.right == 0 || AGrindMark[i].Rect.bottom == 0) continue;
											GrindRectWidth  = (AGrindMark[i].Rect.right  - AGrindMark[i].Rect.left);
											GrindRectHeight = (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top );

											if (AGrindMark[i].Rect.left <= 0         || AGrindMark[i].Rect.top <= 0         || GrindRectWidth <= 0         || GrindRectHeight <= 0        ) continue;
											if (AGrindMark[i].Rect.left >= FrameImgW || AGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;
											if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) continue;

											MarkID = AGrindMark[i].MarkID; // Multi Teaching MarkID 
											deg = AGrindMark[i].degree;
											MarkFind[MarkID].SetAngleBias(deg);
											MarkFind[MarkID].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
											AGrindMark[i].Score = 0.0f;

											if (MarkFind[MarkID].OnExecute()) { //GrindMark Search OK 
												AGrindMark[i].Point.x = MarkFind[MarkID].Result[0].Px;
												AGrindMark[i].Point.y = MarkFind[MarkID].Result[0].Py;
												AGrindMark[i].Score = MarkFind[MarkID].Result[0].Score;
											}
											else {
												MaxScore = 0.0f;
												for (int j = 0; j < MAX_GRINDMARK_ID; j++) {
													MarkFind[j].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
													MarkFind[j].SetAngleBias(deg);

													if (MarkFind[j].OnExecute() && MarkFind[j].Result[0].Score > MaxScore) { //GrindMark Search OK 
														AGrindMark[i].Point.x = MarkFind[j].Result[0].Px;
														AGrindMark[i].Point.y = MarkFind[j].Result[0].Py;
														AGrindMark[i].Score = MarkFind[j].Result[0].Score;
														MaxScore = MarkFind[j].Result[0].Score;
													}
												}
											}
											//20230320 HSKIM

											// GrindMakrã����� ������,������ ������
											if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) {
												// GrindMark���� OutData �ִܰŸ� Count���� Calc 
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
												for (int k = 0; k < C2EdgeCount; k++) {
													if (C2Point[k].x == 0.0f || C2Point[k].y == 0.0f) continue;
													if (AGrindMark[i].Point.x == 0.0f || AGrindMark[i].Point.y == 0.0f) continue;

													GrindMeaDis = Math.GetLength(AGrindMark[i].Point, C2Point[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkOutCnt = k;
													}
												}
												// OutData���� InData �ִܰŸ� Count���� Calc 
												if (GrindMarkOutCnt != 0) {
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
													for (int k = 0; k < C2InEdgeCount; k++) {
														if (C2InPoint[k].x == 0.0f || C2InPoint[k].y == 0.0f) continue;
														if (C2Point[GrindMarkOutCnt].x == 0.0f || C2Point[GrindMarkOutCnt].y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(C2Point[GrindMarkOutCnt], C2InPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkInCnt = k;
														}
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
												if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
													for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
														if (C2Point[k].x == 0.0f || C2Point[k].y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(AGrindMark[i].Point, C2Point[k]);
														TotalCirDis += GrindMeaDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														AGrindMark[i].GrindMea = TotalCirDis / CircleCount;
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
												if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
													for (int k = -5; k < 5; k++) {
														if (C2Point[GrindMarkOutCnt + k].x == 0.0f && C2Point[GrindMarkOutCnt + k].y == 0.0f) continue;
														if (C2InPoint[GrindMarkInCnt + k].x == 0.0f && C2InPoint[GrindMarkInCnt + k].y == 0.0f) continue;
														GrindWidthDis = Math.GetLength(C2InPoint[GrindMarkInCnt + k], C2Point[GrindMarkOutCnt + k]);
														TotalCirDis += GrindWidthDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
													}
												}
											}
											else { // GrindMrk ��ã����� �������� ������ 
												// OutData���� �ּҰ� ������ Count���� 
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
												for (int k = 0; k < C2EdgeCount; k++) {
													if (C2Point[k].x == 0.0f || C2Point[k].y == 0.0f) continue;

													AGrindMark[i].RectPoint.x = AGrindMark[i].Rect.left + (AGrindMark[i].Rect.right - AGrindMark[i].Rect.left) / 2;
													AGrindMark[i].RectPoint.y = AGrindMark[i].Rect.top + (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top) / 2;

													GrindMeaDis = Math.GetLength(AGrindMark[i].RectPoint, C2Point[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkOutCnt = k;
													}
												}

												// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
												if (GrindMarkOutCnt != 0) {
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
													for (int k = 0; k < C2InEdgeCount; k++) {
														if (C2InPoint[k].x == 0.0f || C2InPoint[k].y == 0.0f) continue;
														if (C2Point[GrindMarkOutCnt].x == 0.0f || C2Point[GrindMarkOutCnt].y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(C2Point[GrindMarkOutCnt], C2InPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkInCnt = k;
														}
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
												if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
													for (int k = -5; k < 5; k++) {
														if (C2Point[GrindMarkOutCnt + k].x == 0.0f && C2Point[GrindMarkOutCnt + k].y == 0.0f) continue;
														if (C2InPoint[GrindMarkInCnt + k].x == 0.0f && C2InPoint[GrindMarkInCnt + k].y == 0.0f) continue;
														GrindWidthDis = Math.GetLength(C2InPoint[GrindMarkInCnt + k], C2Point[GrindMarkOutCnt + k]);
														TotalCirDis += GrindWidthDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
													}
												}
											}
										}
									}
								}
							}
						}
						else if (IsRectTop) {
							RTDx = 0.0f;
							RTDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FRTRPoint.x = FRTPoint.x;
							FRTRPoint.y = FRTPoint.y;
							FRTTPoint.x = FRTPoint.x;
							FRTTPoint.y = FRTPoint.y;
						}
						else if (IsCircleTop) { // TOP���� RightTop Circle �˻�  A2Circle 
							GrindRectWidth  = 0;
							GrindRectHeight = 0;
							EasyImage::Threshold(&RotateImg, &RotateBinaryImg, PadSideThresHold);
							if (PadSideThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
							else                       { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

							// NOTCH RightTop
							if (FInspectMode == Insp_Polygon) {
								if (RTCircleWidth != 0 && RTCircleHeight != 0) {
					                P1.x =0, P1.y=0, P2.x=0, P2.y=0, P3.x=0, P3.y=0, P4.x=0, P4.y=0;

									P1.x = (int)(FRTPoint.x - RTCircleWidth); P1.y = (int)(FRTPoint.y);            P2.x = (int)(P1.x + RTCircleWidth); P2.y = (int)(P1.y);
									P3.x = (int)(P1.x);                       P3.y = (int)(P1.y + RTCircleHeight); P4.x = (int)(P2.x);                 P4.y = (int)(P3.y);

									// �˻�ȭ�� Display ���� 
									PadPolyRect[4].left  = P1.x;   PadPolyRect[4].top    = P1.y;
									PadPolyRect[4].right = P2.x;   PadPolyRect[4].bottom = P3.y;

									// Median Filter ���� 
									if (P1.x <= 0                           || P1.y <= 0           || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0        ) return false;
									if (P1.x >= FrameImgW                   || P1.y >= FrameImgH   || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) >= FrameImgH) return false;
									if ((P1.x + (P2.x - P1.x)) >= FrameImgW || (P1.y + (P3.y - P1.y)) > FrameImgH) return false;

									FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
									FilterROI.Attach(&RotateBinaryImg);
									FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
									LogUnit.SetLog(L"RightTop ==>Px :%d, Py :%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
									// 									 LogUnit.SetLog(L"RightTop ==>P1x:%d, P1y:%d, P2x:%d, P2y:%d, P3x:%d, P3y:%d, P4x:%d, P4y:%d,",P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y);     
//									EasyImage::Median(&FilterROI, &FilterROI);
									OnFilter(ftMedian, &FilterROI, &FilterROI);

									Polygon->GImage          = &RotateBinaryImg;
									Polygon->IWidth          = FrameImgW;
									Polygon->IHeight         = FrameImgH;
									Polygon->PoPa.FMinWidth  = 5;
									Polygon->PoPa.FMaxWidth  = (int)(P2.x - P1.x);
									Polygon->PoPa.FMinHeight = (int)((P3.y - P1.y) / 2);
									Polygon->PoPa.FMaxHeight = (int)((P3.y - P1.y) + 10);
									Polygon->PoPa.MinPxCnt   = 100;
									Polygon->PoPa.MaxPxCnt   = 20000;
									Polygon->PoPa.MaxBbCnt   = 10;
									Polygon->PoPa.IsSeparate = true;
									Polygon->PoPa.IsInToOut  = false;
									Polygon->PoPa.HiThHold   = PadSideThresHold;
									Polygon->PoPa.LoThHold   = 0;
									Polygon->PoPa.IgInSide   = true;
									Polygon->PoPa.IsHorSort  = true;
									Polygon->PoPa.IsVerSort  = false;
									Polygon->PoPa.IsInToLeft = true;

									if (P1.x != 0 && P1.y != 0 && P2.x != 0 && P2.y != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
										Polygon->OnExecute(P1, P2, P3, P4);
									}
									// Polygon Algorithm InEdgeData/OutEdgeData Separate 
									if (Polygon->BlobCount == 1) {
										// 												 PolygonMarginCnt = (IsMarkBtm)? 5:20;
										PolygonMarginCnt = (CamId==Cam_Top) ? 50 : 20;
										if (Polygon->Blob[0].InCnt > (PolygonMarginCnt * 2)) {
											A2CirInEdgeCount = 0;
											A2CirInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt * 2));
											if (A2CirInEdgeCount > 0) {
												if (A2CirInPoint != NULL) delete[] A2CirInPoint;
												A2CirInPoint = new TFPoint[A2CirInEdgeCount];

												PixelCnt = 0;
												for (int k = 0; k < Polygon->Blob[0].InCnt; k++) {
													if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].InCnt - PolygonMarginCnt)) continue;
													A2CirInPoint[PixelCnt].x = Polygon->Blob[0].InPoint[k].x;
													A2CirInPoint[PixelCnt].y = Polygon->Blob[0].InPoint[k].y;
													PixelCnt++;
												}
											}
										}

										PolygonMarginCnt = (CamId==Cam_Top) ? 5 : 20;
										if (Polygon->Blob[0].OutCnt > (PolygonMarginCnt * 2)) {
											A2CirOutEdgeCount = 0;
											A2CirOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt * 2));
											if (A2CirOutEdgeCount > 0) {
												if (A2CirOutPoint != NULL) delete[] A2CirOutPoint;
												A2CirOutPoint = new TFPoint[A2CirOutEdgeCount];

												PixelCnt = 0;
												for (int k = 0; k < Polygon->Blob[0].OutCnt; k++) {
													if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].OutCnt - PolygonMarginCnt)) continue;
													A2CirOutPoint[PixelCnt].x = Polygon->Blob[0].OutPoint[k].x;
													A2CirOutPoint[PixelCnt].y = Polygon->Blob[0].OutPoint[k].y;
													PixelCnt++;
												}
											}
										}
									}


									// InEdgeData/OutEdgeData ���� ������ �� ������ ���
									if (CamId==Cam_Top) { // LTPS ������,������ ��� 
										for (int i = 19; i < 23; i++) {
											if (AGrindMark[i].DvX == 0 || AGrindMark[i].DvY == 0) continue;
											// �̹��� ȸ�� ���� ���� ��ǥ ȸ�� 
											OrgMarkDv.x = (float)AGrindMark[i].DvX; OrgMarkDv.y = (float)AGrindMark[i].DvY;
											deg = dASideDeg; //Train ����� ���ذ��� 
											NewMarkDv = Math.OnRotate(FLTPoint, OrgMarkDv, deg);
											AGrindMark[i].DvX = (int)NewMarkDv.x; AGrindMark[i].DvY = (int)NewMarkDv.y;

											//ROI�������� 
											AGrindMark[i].Rect.left   = (FLTPoint.x + AGrindMark[i].DvX) - AGrindMark[i].Range;
											AGrindMark[i].Rect.top    = (FLTPoint.y + AGrindMark[i].DvY) - AGrindMark[i].Range;
											AGrindMark[i].Rect.right  = (AGrindMark[i].Rect.left) + (AGrindMark[i].Range*2);
											AGrindMark[i].Rect.bottom = (AGrindMark[i].Rect.top)  + (AGrindMark[i].Range*2);
										}

										for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
											if (k >= MAX_GRINDMARK_ID) continue;
											MarkFind[k].OnLoad(true, FindDir, k);
											MarkFind[k].GImage = &RotateImg;
											MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
											MarkFind[k].SetMinScore(GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
											MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
											MarkFind[k].SetMaxInstances(1);
											MarkFind[k].SetAngleTolerance(20.0f);
											MarkFind[k].SetScaleBias(1.00f);
											MarkFind[k].SetScaleTolerance(0.0f);
											MarkFind[k].Property.Ox = (float)FrameImgW / 2;
											MarkFind[k].Property.Oy = (float)FrameImgH / 2;
										}

										for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) {
											if (AGrindMark[i].Rect.left == 0 || AGrindMark[i].Rect.top == 0 || AGrindMark[i].Rect.right == 0 || AGrindMark[i].Rect.bottom == 0) continue;
											GrindRectWidth  = (AGrindMark[i].Rect.right  - AGrindMark[i].Rect.left);
											GrindRectHeight = (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top );

											if (AGrindMark[i].Rect.left <= 0 || AGrindMark[i].Rect.top <= 0 || GrindRectWidth <= 0 || GrindRectHeight <= 0) continue;
											if (AGrindMark[i].Rect.left >= FrameImgW || AGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;
											if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) continue;

											MarkID = AGrindMark[i].MarkID; // Multi Teaching MarkID 
											deg    = AGrindMark[i].degree;
											MarkFind[MarkID].SetAngleBias(deg);
											MarkFind[MarkID].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
											AGrindMark[i].Score = 0.0f;

											if (MarkFind[MarkID].OnExecute()) { //GrindMark Search OK 
												AGrindMark[i].Point.x = MarkFind[MarkID].Result[0].Px;
												AGrindMark[i].Point.y = MarkFind[MarkID].Result[0].Py;
												AGrindMark[i].Score = MarkFind[MarkID].Result[0].Score;
											}
											else {
												MaxScore = 0.0f;
												for (int j = 0; j < MAX_GRINDMARK_ID; j++) {
													MarkFind[j].IRoi->SetPlacement(AGrindMark[i].Rect.left, AGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
													MarkFind[j].SetAngleBias(deg);

													if (MarkFind[j].OnExecute() && MarkFind[j].Result[0].Score > MaxScore) { //GrindMark Search OK 
														AGrindMark[i].Point.x = MarkFind[j].Result[0].Px;
														AGrindMark[i].Point.y = MarkFind[j].Result[0].Py;
														AGrindMark[i].Score = MarkFind[j].Result[0].Score;
														MaxScore = MarkFind[j].Result[0].Score;
													}
												}
											}
											//20230320 HSKIM

											if (A2CirOutEdgeCount != 0 && A2CirInEdgeCount != 0) { // MarkMode��� ������ �˻縦 ���� ���� �� ������ ����
												////////////////////////////////////////////////////////////////////////////////////////////////////
												if (AGrindMark[i].Point.x != 0.0f && AGrindMark[i].Point.y != 0.0f) {  //GrindMark ã������� 
													// GrindMark���� OutData �ִܰŸ� Count���� Calc 
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
													for (int k = 0; k < A2CirOutEdgeCount; k++) {
														if (A2CirOutPoint[k].x == 0.0f || A2CirOutPoint[k].y == 0.0f) continue;
														if (AGrindMark[i].Point.x == 0.0f || AGrindMark[i].Point.y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(AGrindMark[i].Point, A2CirOutPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkOutCnt = k;
														}
													}
													if (GrindMarkOutCnt != 0) {
														// GrindMark���� InData �ִܰŸ� Count���� Calc 
														GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
														for (int k = 0; k < A2CirInEdgeCount; k++) {
															if (A2CirInPoint[k].x == 0.0f || A2CirInPoint[k].y == 0.0f) continue;
															if (A2CirOutPoint[GrindMarkOutCnt].x == 0.0f || A2CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(A2CirOutPoint[GrindMarkOutCnt], A2CirInPoint[k]);
															if (GrindMeaDis < MinDis) {
																MinDis = GrindMeaDis;
																GrindMarkInCnt = k;
															}
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
													if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
														for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
															if (A2CirOutPoint[k].x == 0.0f || A2CirOutPoint[k].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(AGrindMark[i].Point, A2CirOutPoint[k]);
															TotalCirDis += GrindMeaDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															AGrindMark[i].GrindMea = TotalCirDis / CircleCount;
															AGrindMark[i].GrindMea = AGrindMark[i].GrindMea + 0.5; //SubPixel 
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
													if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
														for (int k = -5; k < 5; k++) {
															if (A2CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && A2CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
															if (A2CirInPoint[GrindMarkInCnt + k].x == 0.0f && A2CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
															GrindWidthDis = Math.GetLength(A2CirInPoint[GrindMarkInCnt + k], A2CirOutPoint[GrindMarkOutCnt + k]);
															TotalCirDis += GrindWidthDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
															AGrindMark[i].GrindWidth = AGrindMark[i].GrindWidth + 0.5; //SubPixel
														}
													}
												}
												else if (AGrindMark[i].Rect.left != 0 && AGrindMark[i].Rect.top != 0 && AGrindMark[i].Rect.right != 0 && AGrindMark[i].Rect.bottom != 0) {  //GrindMark �� ã�������
													// OutData���� �ּҰ� ������ Count���� 
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
													for (int k = 0; k < A2CirOutEdgeCount; k++) {
														if (A2CirOutPoint[k].x == 0.0f || A2CirOutPoint[k].y == 0.0f) continue;

														AGrindMark[i].RectPoint.x = AGrindMark[i].Rect.left + (AGrindMark[i].Rect.right - AGrindMark[i].Rect.left) / 2;
														AGrindMark[i].RectPoint.y = AGrindMark[i].Rect.top + (AGrindMark[i].Rect.bottom - AGrindMark[i].Rect.top) / 2;

														GrindMeaDis = Math.GetLength(AGrindMark[i].RectPoint, A2CirOutPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkOutCnt = k;
														}
													}

													// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
													if (GrindMarkOutCnt != 0) {
														GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
														for (int k = 0; k < A2CirInEdgeCount; k++) {
															if (A2CirInPoint[k].x == 0.0f || A2CirInPoint[k].y == 0.0f) continue;
															if (A2CirOutPoint[GrindMarkOutCnt].x == 0.0f || A2CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(A2CirOutPoint[GrindMarkOutCnt], A2CirInPoint[k]);
															if (GrindMeaDis < MinDis) {
																MinDis = GrindMeaDis;
																GrindMarkInCnt = k;
															}
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
													if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
														for (int k = -5; k < 5; k++) {
															if (A2CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && A2CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
															if (A2CirInPoint[GrindMarkInCnt + k].x == 0.0f && A2CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
															GrindWidthDis = Math.GetLength(A2CirInPoint[GrindMarkInCnt + k], A2CirOutPoint[GrindMarkOutCnt + k]);
															TotalCirDis += GrindWidthDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															AGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
															AGrindMark[i].GrindWidth = AGrindMark[i].GrindWidth + 0.5; //SubPixel
														}
													}
												}
											}
										}
									}
									else { //Polygon EnCap ���� OutPoint �߰��� ���� ���� �Ÿ� ��հ��� ����Ѵ�(RightTop)
										if (A2CirInEdgeCount != 0 && A2CirOutEdgeCount != 0) {
											GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
											GrindMarkOutCnt = (int)(A2CirOutEdgeCount / 2);
											// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
											if (GrindMarkOutCnt != 0) {
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
												for (int k = 0; k < A2CirInEdgeCount; k++) {
													if (A2CirInPoint[k].x == 0.0f || A2CirInPoint[k].y == 0.0f) continue;
													if (A2CirOutPoint[GrindMarkOutCnt].x == 0.0f || A2CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

													GrindMeaDis = Math.GetLength(A2CirOutPoint[GrindMarkOutCnt], A2CirInPoint[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkInCnt = k;
													}
												}
											}

											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
											if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
												CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
												for (int k = -5; k < 5; k++) {
													if (A2CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && A2CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
													if (A2CirInPoint[GrindMarkInCnt + k].x == 0.0f && A2CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
													GrindWidthDis = Math.GetLength(A2CirInPoint[GrindMarkInCnt + k], A2CirOutPoint[GrindMarkOutCnt + k]);
													TotalCirDis += GrindWidthDis;
													CircleCount++;
												}
												// 												 if(TotalCirDis!=0.0f && CircleCount!=0){
												// 													 A2CirGrindWd[2] = TotalCirDis/CircleCount;
												// 													 A2CirGrindWd[2] = A2CirGrindWd[2]+0.5; //SubPixel
												// 												 }
											}
										}
									}

									// Polygon �ڸ� �˻� ������(RightTop)
									// ���� �ʿ������...���߿� ������� �𸣴� �ϴ� ����
									if (CamId==Cam_Top && BrokenThresHold != 0) {
										RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
										EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &RotateBrokenImg);
										EasyImage::Threshold(&RotateImg, &RotateBrokenImg, BrokenThresHold);
										Edge->GImage = &RotateBrokenImg;
										Edge->FImgHeight = ImageHeight;
										Edge->SetEdgeType(0);

										int DvCnt = 0, DataCount = 0;
										double deg = 0.0f;
										TFPoint Target(0.0f, 0.0f);

										if (GrindMarkInCnt > GrindMarkOutCnt) {
											DvCnt = GrindMarkInCnt - GrindMarkOutCnt;
											for (int k = 0; k < A2CirOutEdgeCount; k++) {
												deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
												if (((k + DvCnt) < A2CirInEdgeCount) && A2CirInPoint[k + DvCnt].x > 0 && A2CirInPoint[k + DvCnt].y > 0) {
													deg = Math.GetDegree(A2CirOutPoint[k], A2CirInPoint[k + DvCnt]);

													Target = Math.OnFindPerpendicularPointXY(A2CirOutPoint[k], -dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
													Edge->OnExecute(A2CirOutPoint[k], Target); // �����ܰ����ο��� Target���� White Point Search 
													if (Edge->GetResultEdgePointCount() > 0) {
														A2CircleData[DataCount].Dis = 0.0f;
														A2CircleData[DataCount].Dv = 0.0f;
														A2CircleData[DataCount].P = Edge->GetResultEdgePoint(0);
														A2CircleData[DataCount].P.y = A2CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
														DataCount++;
													}
												}
											}
											A2CirInCrackCount = DataCount;
										}
										else if (GrindMarkInCnt < GrindMarkOutCnt) {
											DvCnt = GrindMarkOutCnt - GrindMarkInCnt;
											for (int k = DvCnt; k < A2CirOutEdgeCount; k++) {
												deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
												if (((k - DvCnt) < A2CirInEdgeCount) && A2CirInPoint[k - DvCnt].x > 0 && A2CirInPoint[k - DvCnt].y > 0 && k - DvCnt >= 0) {
													deg = Math.GetDegree(A2CirOutPoint[k], A2CirInPoint[k - DvCnt]);

													Target = Math.OnFindPerpendicularPointXY(A2CirOutPoint[k], -dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
													Edge->OnExecute(A2CirOutPoint[k], Target); // �����ܰ����ο��� Target���� White Point Search 
													if (Edge->GetResultEdgePointCount() > 0) {
														A2CircleData[DataCount].Dis = 0.0f;
														A2CircleData[DataCount].Dv = 0.0f;
														A2CircleData[DataCount].P = Edge->GetResultEdgePoint(0);
														A2CircleData[DataCount].P.y = A2CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
														DataCount++;
													}
												}
											}
											A2CirInCrackCount = DataCount;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		if (FShape == shRectC) { // NOTCH ROTATE IMAGE 
			// Image Roate �� �����̹��� ���� ������ ���� 
			ImgDegree = 0.0f;
			FLBPoint = Math.OnIntersect(Line[0].FP0, Line[0].FP1, Line[3].FP0, Line[3].FP1);
			FRBPoint = Math.OnIntersect(Line[2].FP0, Line[2].FP1, Line[3].FP0, Line[3].FP1);

			if (FLBPoint.x != 0 && FLBPoint.y != 0 && FRBPoint.x != 0 && FRBPoint.y != 0) ImgDegree = -(Math.GetDegree(FLBPoint.x, FLBPoint.y, FRBPoint.x, FRBPoint.y));

			if (FLBPoint.x != 0 && FLBPoint.y != 0 && FRBPoint.x != 0 && FRBPoint.y != 0) { // �����̹��� ������ ���� AlignƲ��������� �̹��� ȸ�� 
				// ���� RotateImg Size ���� �� ���� 
// 						 LogUnit.SetLog(L"shRectC_RotateImg_Start");     
				RotateImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &RotateImg);

				// ���� �����Ͽ� �̹��� ȸ�� 
				EasyImage::ScaleRotate(GImage, (float)FrameImgW / 2, (float)FrameImgH / 2, (float)FrameImgW / 2, (float)FrameImgH / 2, 1.0f, 1.0f, ImgDegree, &RotateImg, 4);
				CenterPoint.x = FrameImgW / 2;
				CenterPoint.y = FrameImgH / 2;

				// Binary RotateImg Size ���� �� ���� 
				RotateBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &RotateBinaryImg);
//				RotateImg.Save("D:\\ROTATE_IMG\\RotateImg_CUP_����.bmp");
//				GImage->Save  ("D:\\ROTATE_IMG\\GImage_CUP_����.bmp"   );


				// Circle Grind In EdgePoint ������� �̹��� 
//				RotateGrindInBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
//				EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &RotateGrindInBinaryImg);

				// Grind Measure Corner, Circle �˻� ���� 
				RotateGrindBinaryImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
				EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &RotateGrindBinaryImg);
				// 						 RotateImg.Save("D:\\ROTATE_IMG\\NEW_RotateImgȸ��_CUP.bmp");
				// 						 LogUnit.SetLog(L"shRectC_RotateImg_End");    

				if (FLeftWall) { // shRectC Image Rotate Sequence  
// 							 LogUnit.SetLog(L"CUP_Left_Start");     
					EasyImage::Threshold(&RotateImg, &RotateBinaryImg, LeftEdgeThresHold);
					if (LeftEdgeThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
					else                        { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

					if     (IsRectBtm  ){ 
						ROI_X = IRoi->GetOrgX()  ;
						ROI_W = IRoi->GetWidth() ;
						ROI_Y = IRoi->GetOrgY()  ;
						ROI_H = IRoi->GetHeight();
					}
					else if(IsCornerBtm){
						ROI_X = IRoi->GetOrgX()          ;
						ROI_W = IRoi->GetWidth()         ;
						ROI_Y = IRoi->GetOrgY()          ;
						ROI_H = FEndLine - LBCornerHeight;
					}
					else if(IsCircleBtm){
						ROI_X = IRoi->GetOrgX()          ;
						ROI_W = IRoi->GetWidth()         ;
						ROI_Y = IRoi->GetOrgY()          ;
						ROI_H = FEndLine - LBCircleHeight;
					}
					SingleLine->IRoi->SetOrgX(ROI_X  );
					SingleLine->IRoi->SetOrgY(ROI_Y  );
					SingleLine->IRoi->SetWidth(ROI_W );
					SingleLine->IRoi->SetHeight(ROI_H);

					if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
					if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;
					if((ROI_X + ROI_W)          > FrameImgW || (ROI_Y+ROI_H)            > FrameImgH) return false;

					SingleLine->SetIncrement(5);
					SingleLine->SetOrientation(orWest);
					SingleLine->SetFindLineId(0);

					// TEST 
					if (LeftEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
					else                 SingleLine->SetEdgeType(1);// White To Black 

					SingleLine->OnExecute();
					Line[0].FP0 = SingleLine->GetResultP0();
					Line[0].FP1 = SingleLine->GetResultP1();

					if ((fabs(Line[0].FP0.x - Line[0].FP1.x) > 2) && (Line[0].FP1.x > Line[0].FP0.x)) Line[0].FP1.x = Line[0].FP0.x;

					// Btm,Top ������� ù��°
					LeftOutLine[0].FP0.x = Line[0].FP0.x;
					LeftOutLine[0].FP0.y = Line[0].FP0.y;
					LeftOutLine[0].FP1.x = Line[0].FP1.x;
					LeftOutLine[0].FP1.y = Line[0].FP1.y;

					if (fabs(LeftOutLine[0].FP0.x - LeftOutLine[0].FP1.x) >= 0.5) {
						LeftOutLine[0].FP0.x = (LeftOutLine[0].FP0.x + LeftOutLine[0].FP1.x) / 2.0;
						LeftOutLine[0].FP1.x = (LeftOutLine[0].FP0.x + LeftOutLine[0].FP1.x) / 2.0;
					}

					// LeftOutLine....make OutLine Data 
					// ������ �����͸� �Ѱ��ش� 
					// �˻��� ���ض����� ã�� ���Ұ�� ���������� �Ѱ��༭ ���ض��� ������ 
					if (LeftOutLine[0].FP0.x != 0.0f && LeftOutLine[0].FP0.y != 0.0f && LeftOutLine[0].FP1.x != 0.0f && LeftOutLine[0].FP1.y != 0.0f) {
						PreLeftOutLine[0].FP0.x = LeftOutLine[0].FP0.x;    PreLeftOutLine[0].FP1.x = LeftOutLine[0].FP1.x;
						PreLeftOutLine[0].FP0.y = LeftOutLine[0].FP0.y;    PreLeftOutLine[0].FP1.y = LeftOutLine[0].FP1.y;
					} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
					else if (LeftOutLine[0].FP0.x == 0.0f || LeftOutLine[0].FP0.y == 0.0f || LeftOutLine[0].FP1.x == 0.0f || LeftOutLine[0].FP1.y == 0.0f) {
						if (PreLeftOutLine[0].FP0.x == 0.0f && PreLeftOutLine[0].FP0.y == 0.0f && PreLeftOutLine[0].FP1.x == 0.0f && PreLeftOutLine[0].FP1.y == 0.0f) {// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
							// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
							ROI_H = IRoi->GetHeight();
							LeftOutLine[0].FP0.x = Line[0].FP0.x;    LeftOutLine[0].FP0.y = 0.0f;
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
					// 							 LogUnit.SetLog(L"CUP_Left_End");     
				}
				if (FRightWall) { // Image Rotate ���� 
					EasyImage::Threshold(&RotateImg, &RotateBinaryImg, RightEdgeThresHold);
					if (RightEdgeThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
					else                         { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}
					if     (IsRectTop  ){ 
						ROI_X = IRoi->GetOrgX()  ;
						ROI_W = IRoi->GetWidth() ;
						ROI_Y = IRoi->GetOrgY()  ;
						ROI_H = IRoi->GetHeight();
					}
					else if(IsCornerTop){
						ROI_X = IRoi->GetOrgX()          ;
						ROI_W = IRoi->GetWidth()         ;
						ROI_Y = IRoi->GetOrgY()          ;
						ROI_H = FEndLine - RBCornerHeight;
					}
					else if(IsCircleTop){
						ROI_X = IRoi->GetOrgX()          ;
						ROI_W = IRoi->GetWidth()         ;
						ROI_Y = IRoi->GetOrgY()          ;
						ROI_H = FEndLine - RBCircleHeight;
					}
					SingleLine->SetIncrement(5);

					SingleLine->IRoi->SetOrgX(ROI_X  );
					SingleLine->IRoi->SetOrgY(ROI_Y  );
					SingleLine->IRoi->SetWidth(ROI_W );
					SingleLine->IRoi->SetHeight(ROI_H);

					if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
					if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;
					if((ROI_X + ROI_W)          > FrameImgW || (ROI_Y+ROI_H)            > FrameImgH) return false;

					SingleLine->SetOrientation(orEast);
					SingleLine->SetFindLineId(0);

					if(RightEdgeBTWhite) SingleLine->SetEdgeType(0);// Black To White
					else                 SingleLine->SetEdgeType(1);// White To Black 

					SingleLine->OnExecute();
					Line[2].FP0 = SingleLine->GetResultP0();
					Line[2].FP1 = SingleLine->GetResultP1();

					if ((fabs(Line[2].FP0.x - Line[2].FP1.x) > 2) && (Line[2].FP1.x < Line[2].FP0.x)) Line[2].FP1.x = Line[2].FP0.x;

					// Btm,Top ������� ù��°
					RightOutLine[0].FP0.x = Line[2].FP0.x;
					RightOutLine[0].FP0.y = Line[2].FP0.y;
					RightOutLine[0].FP1.x = Line[2].FP1.x;
					RightOutLine[0].FP1.y = Line[2].FP1.y;

					if (fabs(RightOutLine[0].FP0.x - RightOutLine[0].FP1.x) >= 0.5) {
						RightOutLine[0].FP0.x = (RightOutLine[0].FP0.x + RightOutLine[0].FP1.x) / 2.0;
						RightOutLine[0].FP1.x = (RightOutLine[0].FP0.x + RightOutLine[0].FP1.x) / 2.0;
					}

					// LeftOutLine....make OutLine Data 
					// ������ �����͸� �Ѱ��ش� 
					// �˻��� ���ض����� ã�� ���Ұ�� ���������� �Ѱ��༭ ���ض��� ������ 
					if (RightOutLine[0].FP0.x != 0.0f && RightOutLine[0].FP0.y != 0.0f && RightOutLine[0].FP1.x != 0.0f && RightOutLine[0].FP1.y != 0.0f) {
						PreRightOutLine[0].FP0.x = RightOutLine[0].FP0.x;    PreRightOutLine[0].FP1.x = RightOutLine[0].FP1.x;
						PreRightOutLine[0].FP0.y = RightOutLine[0].FP0.y;    PreRightOutLine[0].FP1.y = RightOutLine[0].FP1.y;
					} // ���� ��� ���ۺ��� Broken �߻����� �����Ͱ� ���°�� 
					else if (RightOutLine[0].FP0.x == 0.0f || RightOutLine[0].FP0.y == 0.0f || RightOutLine[0].FP1.x == 0.0f || RightOutLine[0].FP1.y == 0.0f) {
						if (PreRightOutLine[0].FP0.x == 0.0f && PreRightOutLine[0].FP0.y == 0.0f && PreRightOutLine[0].FP1.x == 0.0f && PreRightOutLine[0].FP1.y == 0.0f) {// ���ۺκ� Broken�߻����� �����Ͱ� ���°�� 
							// Mark��ǥ�� �������� ������ǥ���� �ٽ� ������ 
							ROI_H = IRoi->GetHeight();
							RightOutLine[0].FP0.x = Line[2].FP0.x;    RightOutLine[0].FP0.y = 0.0f;
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
					// 							 LogUnit.SetLog(L"CUP_Right_End");     
				}
				if (FBottomWall) {
					EasyImage::Threshold(&RotateImg, &RotateBinaryImg, BtmEdgeThresHold);
					if (BtmEdgeThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
					else                       { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

					ROI_X = IRoi->GetOrgX();
					ROI_Y = FEndLine - 100 ;
					ROI_W = IRoi->GetWidth();
					ROI_H = 200;

					SingleLine->IRoi->SetOrgX  (ROI_X);
					SingleLine->IRoi->SetOrgY  (ROI_Y);
					SingleLine->IRoi->SetWidth (ROI_W);
					SingleLine->IRoi->SetHeight(ROI_H);

					if(ROI_X<=0         || ROI_Y< 0         || ROI_W<=0         || ROI_H<=0        ) return false;
					if(ROI_X>= FrameImgW|| ROI_Y> FrameImgW || ROI_W>=FrameImgW || ROI_H> FrameImgH) return false;
					if((ROI_X + ROI_W)          > FrameImgW || (ROI_Y+ROI_H)            > FrameImgH) return false;


					SingleLine->SetOrientation(orSouth);
					SingleLine->FIncrement = 100;
					SingleLine->SetFindLineId(0);

					if (BtmEdgeBTWhite) SingleLine->SetEdgeType(0);  // Black To White 
					else                SingleLine->SetEdgeType(1);  // White To Black  

					SingleLine->OnExecute();
					Line[3].FP0 = SingleLine->GetResultP0();
					Line[3].FP1 = SingleLine->GetResultP1();
				}

				// Image Roate �� ���� ������ �ٽ� ���� 
				FLBPoint = Math.OnIntersect(LeftOutLine[0].FP0, LeftOutLine[0].FP1, Line[3].FP0, Line[3].FP1);
				FRBPoint = Math.OnIntersect(RightOutLine[0].FP0, RightOutLine[0].FP1, Line[3].FP0, Line[3].FP1);

				// ��ü���� vs �κп��� �˻� ������ 
				if (IsBtmAllGrind) { //��ü����(Polygon_Algorithm����)
					// Polygon_Algorithm �̿��Ͽ� ROI���� �������� ���� ������ ������ 
					// ��ü ������ �������� GrindWidth,GrindMeasure �����Ѵ� 
					LogUnit.SetLog(L"CUP_Polygon_Start");
					// 							 EasyImage::Threshold(&RotateImg,&RotateBinaryImg, BtmEdgeThresHold);
					//							 RotateImg.Save("D:\\ROTATE_IMG\\NEW_RotateImgȸ��_CUP.bmp");
												 // Narrow Trench ������ ������ ������� 
					if (CutThresHold == 0) { //Cutting Line ������� 
					    P1.x = 0, P1.y = 0, P2.x = 0, P2.y = 0, P3.x = 0, P3.y = 0, P4.x = 0, P4.y = 0;
						AllBtmInEdgeCount = 0; AllBtmOutEdgeCount = 0; PreGrindMarkOutCnt = 0; PreGrindMarkInCnt = 0;
						if (IsRectBtm) {
							CellWidth     = (int)(FRBPoint.x - FLBPoint.x) - (LBRectWidth+RBRectWidth);
							CenRoiRange   = (nCSideRoiCenterLength == 0) ? (int)(CellWidth /5) : (int)(nCSideRoiCenterLength * 2);
							SideRoiRange  = (nCSideRoiCenterLength == 0) ? (int)(CellWidth /5) : (int)((CellWidth - (nCSideRoiCenterLength * 2)) / 2);
							LeftRoiWidth  = CenRoiRange;
							RightRoiWidth = CenRoiRange;
							LeftRoiHeight = 50         ;
							RightRoiHeight= 50         ;
						}
						else if(IsCornerBtm){
							LeftRoiWidth = LBCornerWidth ; RightRoiWidth  = RBCornerWidth ;
							LeftRoiHeight= LBCornerHeight; RightRoiHeight = RBCornerHeight;
							CellWidth    = (int)(FRBPoint.x - FLBPoint.x) - (LeftRoiWidth + RightRoiWidth);
							CenRoiRange  = (nCSideRoiCenterLength == 0) ? (int)(CellWidth / 3) : (int)(nCSideRoiCenterLength * 2);
							SideRoiRange = (nCSideRoiCenterLength == 0) ? (int)(CellWidth / 3) : (int)((CellWidth - (nCSideRoiCenterLength * 2)) / 2);
						}
						else if(IsCircleBtm){
							LeftRoiWidth = LBCircleWidth ; RightRoiWidth  = RBCircleWidth;
							LeftRoiHeight= LBCircleHeight; RightRoiHeight = RBCircleHeight;
							CellWidth    = (int)(FRBPoint.x - FLBPoint.x) - (LeftRoiWidth + RightRoiWidth);
							CenRoiRange  = (nCSideRoiCenterLength == 0) ? (int)(CellWidth / 3) : (int)(nCSideRoiCenterLength * 2);
							SideRoiRange = (nCSideRoiCenterLength == 0) ? (int)(CellWidth / 3) : (int)((CellWidth - (nCSideRoiCenterLength * 2)) / 2);
						}
						RoiMargin    =20;

						for (int i = 0; i < 5; i++) {
							GrindThresHold = 0;
							//ROI���� �ڵ� �з� 
							if      (i==NotROI1) {
								P1.x = (int)(FLBPoint.x + LBRectWidth ); P1.y = (int)(FLBPoint.y - LeftRoiHeight    ); 
								P2.x = (int)(P1.x       + LeftRoiWidth); P2.y = (int)(P1.y                          );
								P3.x = (int)(P1.x                     ); P3.y = (int)(P1.y + LeftRoiHeight+RoiMargin);
								P4.x = (int)(P2.x                     ); P4.y = (int)(P3.y                          );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, NotchSideThresHold);
								GrindThresHold = NotchSideThresHold;
							}
							else if (i==NotROI2) {
								P1.x = (int)(P2.x               ); P1.y = (int)(P2.y + (RoiMargin)); 
								P2.x = (int)(P1.x + SideRoiRange); P2.y = (int)(P1.y              );
								P3.x = (int)(P1.x               ); P3.y = (int)(FLBPoint.y + 20   );
								P4.x = (int)(P2.x               ); P4.y = (int)(P3.y              );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, NotchCenThresHold);
								GrindThresHold = NotchCenThresHold;
							}
							else if (i==NotROI3) {
								P1.x = (int)(P2.x              ); P1.y = (int)(P2.y           ); 
								P2.x = (int)(P1.x + CenRoiRange); P2.y = (int)(P1.y           );
								P3.x = (int)(P1.x              ); P3.y = (int)(FLBPoint.y + 20); 
								P4.x = (int)(P2.x              ); P4.y = (int)(P3.y           );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, NotchCenThresHold);
								GrindThresHold = NotchCenThresHold;
							}
							else if (i==NotROI4) {
								P1.x = (int)(P2.x               ); P1.y = (int)(P2.y           ); 
								P2.x = (int)(P1.x + SideRoiRange); P2.y = (int)(P1.y           );
								P3.x = (int)(P1.x               ); P3.y = (int)(FLBPoint.y + 20); 
								P4.x = (int)(P2.x               ); P4.y = (int)(P3.y           );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, NotchCenThresHold);
								GrindThresHold = NotchCenThresHold;
							}
							else if (i==NotROI5) {
								P1.x = (int)(FRBPoint.x - RightRoiWidth - RBRectWidth); P1.y = (int)(FRBPoint.y - RightRoiHeight    ); 
								P2.x = (int)(P1.x + RightRoiWidth                    ); P2.y = (int)(P1.y                           );
								P3.x = (int)(P1.x                                    ); P3.y = (int)(P1.y + RightRoiHeight+RoiMargin); 
								P4.x = (int)(P2.x                                    ); P4.y = (int)(P3.y                           );
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, NotchSideThresHold);
								GrindThresHold = NotchSideThresHold;
							}

							// �˻�ȭ�� Display ���� 
							NotchPolyRect[i].left  = P1.x;   NotchPolyRect[i].top    = P1.y;
							NotchPolyRect[i].right = P2.x;   NotchPolyRect[i].bottom = P3.y;

							// Median Filter ���� 
							if (P1.x <= 0         || P1.y <= 0         || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0        ) return false;
							if (P1.x >= FrameImgW || P1.y >= FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) >= FrameImgH) return false;
							if ((P1.x + (P2.x - P1.x)) >= FrameImgW    || (P1.y + (P3.y - P1.y)) > FrameImgH) return false;

							FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
							FilterROI.Attach(&RotateBinaryImg);
							FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
							LogUnit.SetLog(L"AllGrind_Btm==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
//							EasyImage::Median(&FilterROI, &FilterROI);
							OnFilter(ftMedian, &FilterROI, &FilterROI);

							Polygon->GImage               = &RotateBinaryImg;
							Polygon->IWidth               = FrameImgW;
							Polygon->IHeight              = FrameImgH;
							Polygon->PoPa.FMinWidth       = (int)((P2.x - P1.x) / 2);
							Polygon->PoPa.FMaxWidth       = (int)((P2.x - P1.x) + 100);
							Polygon->PoPa.FMinHeight      = 2;
							Polygon->PoPa.FMaxHeight      = (int)((P3.y - P1.y) + 100);
							Polygon->PoPa.MinPxCnt        = 100;
							Polygon->PoPa.MaxPxCnt        = 20000;
							Polygon->PoPa.MaxBbCnt        = 10;
							Polygon->PoPa.IsSeparate      = true;
							Polygon->PoPa.IsInToOut       = true;
							Polygon->PoPa.HiThHold        = GrindThresHold;
							Polygon->PoPa.LoThHold        = 0;
							Polygon->PoPa.IgInSide        = true;
							Polygon->PoPa.IsHorSort       = true;
							Polygon->PoPa.IsVerSort       = false;
							Polygon->PoPa.IsInToLeft      = true;
							Polygon->PoPa.LeftFirstPoint  = true;
							Polygon->PoPa.RightFirstPoint = false;

							if (P1.x != 0 && P1.y != 0 && P2.x != 0 && P2.y != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
								Polygon->OnExecute(P1, P2, P3, P4);
							}

							if (Polygon->BlobCount > 1) {
								PolyRectCenter.x = 0.0f, PolyRectCenter.y = 0.0f, nPolyPosCnt = 0, RealPolyDis = 0.0f, MinPolyDis = 1000000.0f;
								PolyRectCenter.x = P1.x + ((P2.x - P1.x) / 2); PolyRectCenter.y = P3.y;
								for (int k = 0; k < Polygon->BlobCount; k++) {
									PolygonCenter.x = Polygon->Blob[k].Rect.l + (Polygon->Blob[k].Rect.r - Polygon->Blob[k].Rect.l) / 2;
									PolygonCenter.y = Polygon->Blob[k].Rect.b;
									RealPolyDis = fabs(PolyRectCenter.y - PolygonCenter.y);

									if (RealPolyDis < MinPolyDis) {
										MinPolyDis = RealPolyDis;
										nPolyPosCnt = k;
									}
								}
							}
							else nPolyPosCnt = 0;


							BtmInEdgeCount = 0;
							if (Polygon->Blob[nPolyPosCnt].InCnt > 0) { //���� �����ϴ� InEdgeCount üũ 
								//Defect ������� ���� ���ʶ��� Pos ������ ���� 
								BtmGrindInPos  = 0;
								BtmGrindInCnt  = 0;
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
									BtmInEdgeCount++;
								}

								if (BtmInEdgeCount > 0) { //InEdgeCount ���� ������ ���� ���� �� ����
									if (BtmInPoint != NULL) delete[] BtmInPoint;
									BtmInPoint = new TFPoint[BtmInEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
										BtmInPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].InPoint[k].x;
										BtmInPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].InPoint[k].y;
										//Defect ������� ���� ���ʶ��� Pos ������ ���� 
										if (IsRectBtm) {
											BtmGrindInPos += BtmInPoint[PixelCnt].y;
											BtmGrindInCnt++;
										}
										PixelCnt++;
									}
								}
							}

							BtmOutEdgeCount = 0;
							if (Polygon->Blob[nPolyPosCnt].OutCnt > 0) { //���� �����ϴ� OutEdgeCount üũ 
								for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
									BtmOutEdgeCount++;
								}

								if (BtmOutEdgeCount > 0) { //InEdgeCount ���� ������ ���� ���� �� ����
									if (BtmOutPoint != NULL) delete[] BtmOutPoint;
									BtmOutPoint = new TFPoint[BtmOutEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
									if (Polygon->Blob[nPolyPosCnt].LeftPoint.x == Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
									if (Polygon->Blob[nPolyPosCnt].RightPoint.x== Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
										BtmOutPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].OutPoint[k].x;
										BtmOutPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].OutPoint[k].y;
										PixelCnt++;
									}
								}
							}


							/*
							PolygonMarginCnt = (CamId==Cam_Top) ? 5 : 20;
							if (Polygon->Blob[0].InCnt > (PolygonMarginCnt * 2)) {
								BtmInEdgeCount = 0;
								BtmInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt * 2));
								if (BtmInEdgeCount > 0) {
									if (BtmInPoint != NULL) delete[] BtmInPoint;
									BtmInPoint = new TFPoint[BtmInEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[0].InCnt; k++) {
										if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].InCnt - PolygonMarginCnt)) continue;
										BtmInPoint[PixelCnt].x = Polygon->Blob[0].InPoint[k].x;
										BtmInPoint[PixelCnt].y = Polygon->Blob[0].InPoint[k].y;
										PixelCnt++;
									}
								}
							}

							if (Polygon->Blob[0].OutCnt > (PolygonMarginCnt * 2)) {
								BtmOutEdgeCount = 0;
								BtmOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt * 2));
								if (BtmOutEdgeCount > 0) {
									if (BtmOutPoint != NULL) delete[] BtmOutPoint;
									BtmOutPoint = new TFPoint[BtmOutEdgeCount];

									PixelCnt = 0;
									for (int k = 0; k < Polygon->Blob[0].OutCnt; k++) {
										if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].OutCnt - PolygonMarginCnt)) continue;
										BtmOutPoint[PixelCnt].x = Polygon->Blob[0].OutPoint[k].x;
										BtmOutPoint[PixelCnt].y = Polygon->Blob[0].OutPoint[k].y;
										PixelCnt++;
									}
								}
							}
							*/

							// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
							if (BtmInEdgeCount > 0 && BtmOutEdgeCount > 0) {
								// Total Top In Data 
								for (int k = 0; k < BtmInEdgeCount; k++) {
									if (AllBtmInEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllBtmInData[AllBtmInEdgeCount].x = BtmInPoint[k].x;
									AllBtmInData[AllBtmInEdgeCount].y = BtmInPoint[k].y;
									AllBtmInEdgeCount++;
								}
								// Total Top Out Data 
								for (int k = 0; k < BtmOutEdgeCount; k++) {
									if (AllBtmOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
									AllBtmOutData[AllBtmOutEdgeCount].x = BtmOutPoint[k].x;
									AllBtmOutData[AllBtmOutEdgeCount].y = BtmOutPoint[k].y;
									AllBtmOutEdgeCount++;
								}
							}
						}
					}
					else { //Cutting Line �����Ұ�� 
						int MarkCount = 0;
						int ROICnt = 0;
						AllBtmInEdgeCount = 0; AllBtmOutEdgeCount = 0; PreGrindMarkOutCnt = 0; PreGrindMarkInCnt = 0;
						// ������ ROI���� ��ŭ Polygon_Algorithm�̿��Ͽ� In/Out EdgePoint ������ ���� 
						P1.x = 0; P1.y = 0; P2.x = 0; P2.y = 0; P3.x = 0; P3.y = 0; P4.x = 0; P4.y = 0;
						CellWidth    = (int)(FRBPoint.x - FLBPoint.x) - (LBCircleWidth + RBCircleWidth);
						CenRoiRange  = (nCSideRoiCenterLength == 0) ? (int)(CellWidth / 3) : (int)(nCSideRoiCenterLength * 2);
						SideRoiRange = (nCSideRoiCenterLength == 0) ? (int)(CellWidth / 3) : (int)((CellWidth - (nCSideRoiCenterLength * 2)) / 2);
						RoiMargin    = LBCircleHeight - (CELL_D + 50);

						for (int i = 0; i < 5; i++) {
							GrindThresHold = 0;
							//ROI���� �ڵ� �з� 
							if      (i==NotROI1) {
								P1.x = (int)(FLBPoint.x); P1.y = (int)(FLBPoint.y - LBCircleHeight); P2.x = (int)(P1.x + LBCircleWidth); P2.y = (int)(P1.y);
								P3.x = (int)(P1.x); P3.y = (int)(P1.y + LBCircleHeight); P4.x = (int)(P2.x); P4.y = (int)(P3.y);
							}
							else if (i==NotROI2) {
								P1.x = (int)(P2.x); P1.y = (int)(P2.y + (RoiMargin)); P2.x = (int)(P1.x + SideRoiRange); P2.y = (int)(P1.y);
								P3.x = (int)(P1.x); P3.y = (int)(FLBPoint.y + 20); P4.x = (int)(P2.x); P4.y = (int)(P3.y);
							}
							else if (i==NotROI3) {
								P1.x = (int)(P2.x); P1.y = (int)(P2.y); P2.x = (int)(P1.x + CenRoiRange); P2.y = (int)(P1.y);
								P3.x = (int)(P1.x); P3.y = (int)(FLBPoint.y + 20); P4.x = (int)(P2.x); P4.y = (int)(P3.y);
							}
							else if (i==NotROI4) {
								P1.x = (int)(P2.x); P1.y = (int)(P2.y); P2.x = (int)(P1.x + SideRoiRange); P2.y = (int)(P1.y);
								P3.x = (int)(P1.x); P3.y = (int)(FLBPoint.y + 20); P4.x = (int)(P2.x); P4.y = (int)(P3.y);
							}
							else if (i==NotROI5) {
								P1.x = (int)(FRBPoint.x - RBCircleWidth); P1.y = (int)(FRBPoint.y - RBCircleHeight); P2.x = (int)(P1.x + RBCircleWidth); P2.y = (int)(P1.y);
								P3.x = (int)(P1.x); P3.y = (int)(P1.y + RBCircleHeight); P4.x = (int)(P2.x); P4.y = (int)(P3.y);
							}
							IsFrameGrindMark = false;
							GrindLineCount = 0;
							// �˻�Frame�� ROI������ GrindMark�������� �� Count���� üũ 
							for (int k = 0; k < MAX_GRINDMARK_COUNT; k++) {
								if (CGrindMark[k].DvX == 0 || CGrindMark[k].DvY == 0) continue;
								if (((FLBPoint.x + CGrindMark[k].DvX) > P1.x) && ((FLBPoint.x + CGrindMark[k].DvX) < P2.x)) {
									IsFrameGrindMark = true;
									GrindLineCount++;
								}
							}

							if (IsFrameGrindMark) { // Frame���� �˻�ROI������ GrindMark������ ��� 
								// GrindLineCount���� ROI��Ȱ�������� ����(GrindLineCount+GrindMark���� ����+���ۿ���+������)
								GrindLineCount = GrindLineCount + (GrindLineCount - 1) + 2;
								// ROI���� ��Ȱ���� ��������
								if (GrindLineCount > 0) {
									if (GrindLine != NULL) delete[] GrindLine;
									GrindLine = new TGrindLine[GrindLineCount];

									// Init Data 
									for (int k = 0; k < GrindLineCount; k++) { GrindLine[k].LinePoint.x = 0; GrindLine[k].LinePoint.x = 0; GrindLine[k].IsGrindLine = false; }

									// GrindMark���� ROI���� ��Ȱ������ ���� 
									MarkCount = 0;
									for (int k = 0; k < MAX_GRINDMARK_COUNT; k++) {
										if (CGrindMark[k].DvX == 0 || CGrindMark[k].DvY == 0) continue;
										if (((FLBPoint.x + CGrindMark[k].DvX) > P1.x) && ((FLBPoint.x + CGrindMark[k].DvX) < P2.x)) {
											// GrindMark������ ���� ���������ǰ� ���� ������ �����Ͽ� ������ ó�� 
											GrindLine[MarkCount].LinePoint.x = FLBPoint.x + CGrindMark[k].DvX - CGrindMark[k].Range - 5;
											GrindLine[MarkCount].IsGrindLine = false;
											MarkCount++;
											GrindLine[MarkCount].LinePoint.x = FLBPoint.x + CGrindMark[k].DvX + CGrindMark[k].Range + 5;
											GrindLine[MarkCount].IsGrindLine = true;
											MarkCount++;
										}
									}
									// ������ ����
									GrindLine[GrindLineCount - 1].LinePoint.x = P2.x;
									GrindLine[GrindLineCount - 1].IsGrindLine = false;

									// ��Ȱ�� �������� �˻� ������ 
									for (int k = 0; k < GrindLineCount; k++) {
										GrindThresHold = 0;
										if (k == 0) { // ù��° ����
											P2.x = GrindLine[k].LinePoint.x;
											P4.x = GrindLine[k].LinePoint.x;
										}
										else { // ������ ����(�߰�,������) 
											P1.x = GrindLine[k - 1].LinePoint.x;  P3.x = (int)P1.x;
											P2.x = GrindLine[k].LinePoint.x    ;  P4.x = (int)P2.x;
										}

										// ThresHold �з� �� ���� 
										if (i == 0 || i == 4) { // Notch Side
											GrindThresHold = (GrindLine[k].IsGrindLine) ? NotchSideThresHold : CutThresHold;
										}
										else { // Notch Center 
											GrindThresHold = (GrindLine[k].IsGrindLine) ? NotchCenThresHold : CutThresHold;
										}

										// �������� �̹��� ��ȯ 
										EasyImage::Threshold(&RotateImg, &RotateBinaryImg, GrindThresHold);

										// ��ȰROI���� ������ �Ѱ���(ȭ�� ��Ȱ ���÷��� ����)
										NotchPolyRect[ROICnt].left = P1.x ;   NotchPolyRect[ROICnt].top    = P1.y;
										NotchPolyRect[ROICnt].right = P2.x;   NotchPolyRect[ROICnt].bottom = P3.y;
										ROICnt++;

										// Median Filter ���� 
										if (P1.x <= 0         || P1.y <= 0         || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0        ) return false;
										if (P1.x >= FrameImgW || P1.y >= FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) >= FrameImgH) return false;
										if ((P1.x + (P2.x - P1.x)) >= FrameImgW    || (P1.y + (P3.y - P1.y)) > FrameImgH) return false;

										FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
										FilterROI.Attach(&RotateBinaryImg);
										FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
										LogUnit.SetLog(L"AllGrind_Btm==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
//										EasyImage::Median(&FilterROI, &FilterROI);
										OnFilter(ftMedian, &FilterROI, &FilterROI);

										Polygon->GImage               = &RotateBinaryImg;
										Polygon->IWidth               = FrameImgW;
										Polygon->IHeight              = FrameImgH;
										Polygon->PoPa.FMinWidth       = (int)((P2.x - P1.x) / 2);
										Polygon->PoPa.FMaxWidth       = (int)((P2.x - P1.x) + 100);
										Polygon->PoPa.FMinHeight      = 2;
										Polygon->PoPa.FMaxHeight      = (int)((P3.y - P1.y) + 100);
										Polygon->PoPa.MinPxCnt        = 100;
										Polygon->PoPa.MaxPxCnt        = 20000;
										Polygon->PoPa.MaxBbCnt        = 10;
										Polygon->PoPa.IsSeparate      = true;
										Polygon->PoPa.IsInToOut       = true;
										Polygon->PoPa.HiThHold        = GrindThresHold;
										Polygon->PoPa.LoThHold        = 0;
										Polygon->PoPa.IgInSide        = true;
										Polygon->PoPa.IsHorSort       = true;
										Polygon->PoPa.IsVerSort       = false;
										Polygon->PoPa.IsInToLeft      = true;
							            Polygon->PoPa.LeftFirstPoint  = true;
							            Polygon->PoPa.RightFirstPoint = false;

										if (P1.x != 0 && P1.y != 0 && P2.x != 0 && P2.y != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
											Polygon->OnExecute(P1, P2, P3, P4);
										}

										// 2���̻� ���� � ������ ����� �з� ����(���� �ٱ��ʿ� �����͸� ���� �����ͷ� ����)
										if (Polygon->BlobCount > 1) {
											PolyRectCenter.x = 0.0f, PolyRectCenter.y = 0.0f, nPolyPosCnt = 0, RealPolyDis = 0.0f, MinPolyDis = 1000000.0f;
											PolyRectCenter.x = P1.x + ((P2.x - P1.x) / 2); PolyRectCenter.y = P3.y;
											for (int k = 0; k < Polygon->BlobCount; k++) {
												PolygonCenter.x = Polygon->Blob[k].Rect.l + (Polygon->Blob[k].Rect.r - Polygon->Blob[k].Rect.l) / 2;
												PolygonCenter.y = Polygon->Blob[k].Rect.b;
												RealPolyDis = fabs(PolyRectCenter.y - PolygonCenter.y);

												if (RealPolyDis < MinPolyDis) {
													MinPolyDis = RealPolyDis;
													nPolyPosCnt = k;
												}
											}
										}
										else nPolyPosCnt = 0;

										if (Polygon->Blob[nPolyPosCnt].InCnt > 0) {
											BtmInEdgeCount = 0;
											for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
												if (Polygon->Blob[nPolyPosCnt].LeftPoint.x  == Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
												if (Polygon->Blob[nPolyPosCnt].RightPoint.x == Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
												BtmInEdgeCount++;
											}

											if (BtmInEdgeCount > 0) {
												if (BtmInPoint != NULL) delete[] BtmInPoint;
												BtmInPoint = new TFPoint[BtmInEdgeCount];

												PixelCnt = 0;
												for (int k = 0; k < Polygon->Blob[nPolyPosCnt].InCnt; k++) {
													if (Polygon->Blob[nPolyPosCnt].LeftPoint.x  == Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
													if (Polygon->Blob[nPolyPosCnt].RightPoint.x == Polygon->Blob[nPolyPosCnt].InPoint[k].x) continue;
													BtmInPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].InPoint[k].x;
													BtmInPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].InPoint[k].y;
													PixelCnt++;
												}
											}
										}

										if (Polygon->Blob[nPolyPosCnt].OutCnt > 0) {
											BtmOutEdgeCount = 0;
											for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
												if (Polygon->Blob[nPolyPosCnt].LeftPoint.x  == Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
												if (Polygon->Blob[nPolyPosCnt].RightPoint.x == Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
												BtmOutEdgeCount++;
											}

											if (BtmOutEdgeCount > 0) {
												if (BtmOutPoint != NULL) delete[] BtmOutPoint;
												BtmOutPoint = new TFPoint[BtmOutEdgeCount];

												PixelCnt = 0;
												for (int k = 0; k < Polygon->Blob[nPolyPosCnt].OutCnt; k++) {
													if (Polygon->Blob[nPolyPosCnt].LeftPoint.x  == Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
													if (Polygon->Blob[nPolyPosCnt].RightPoint.x == Polygon->Blob[nPolyPosCnt].OutPoint[k].x) continue;
													BtmOutPoint[PixelCnt].x = Polygon->Blob[nPolyPosCnt].OutPoint[k].x;
													BtmOutPoint[PixelCnt].y = Polygon->Blob[nPolyPosCnt].OutPoint[k].y;
													PixelCnt++;
												}
											}
										}

										// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
										if (BtmInEdgeCount > 0 && BtmOutEdgeCount > 0) {
											// Total Top In Data 
											for (int k = 0; k < BtmInEdgeCount; k++) {
												if (AllBtmInEdgeCount > MAX_ALL_EDGE_COUNT) return false;
												AllBtmInData[AllBtmInEdgeCount].x = BtmInPoint[k].x;
												AllBtmInData[AllBtmInEdgeCount].y = BtmInPoint[k].y;
												AllBtmInEdgeCount++;
											}
											// Total Top Out Data 
											for (int k = 0; k < BtmOutEdgeCount; k++) {
												if (AllBtmOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
												AllBtmOutData[AllBtmOutEdgeCount].x = BtmOutPoint[k].x;
												AllBtmOutData[AllBtmOutEdgeCount].y = BtmOutPoint[k].y;
												AllBtmOutEdgeCount++;
											}
										}
									}
								}
							}
							else { // Frame���� �˻�ROI������ GrindMark���� ��� 
								EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CutThresHold);

								NotchPolyRect[ROICnt].left = P1.x;   NotchPolyRect[ROICnt].top = P1.y;
								NotchPolyRect[ROICnt].right = P2.x;   NotchPolyRect[ROICnt].bottom = P3.y;
								ROICnt++;
								// Median Filter ���� 
								if (P1.x <= 0         || P1.y <= 0         || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0        ) return false;
								if (P1.x >= FrameImgW || P1.y >= FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) >= FrameImgH) return false;
								if ((P1.x + (P2.x - P1.x)) >= FrameImgW    || (P1.y + (P3.y - P1.y)) > FrameImgH) return false;

								FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
								FilterROI.Attach(&RotateBinaryImg);
								FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
								LogUnit.SetLog(L"AllGrind_Btm==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
//								EasyImage::Median(&FilterROI, &FilterROI);
								OnFilter(ftMedian, &FilterROI, &FilterROI);

								Polygon->GImage               = &RotateBinaryImg;
								Polygon->IWidth               = FrameImgW;
								Polygon->IHeight              = FrameImgH;
								Polygon->PoPa.FMinWidth       = (int)((P2.x - P1.x) / 2);
								Polygon->PoPa.FMaxWidth       = (int)((P2.x - P1.x) + 100);
								Polygon->PoPa.FMinHeight      = 2;
								Polygon->PoPa.FMaxHeight      = (int)((P3.y - P1.y) + 100);
								Polygon->PoPa.MinPxCnt        = 100;
								Polygon->PoPa.MaxPxCnt        = 20000;
								Polygon->PoPa.MaxBbCnt        = 10;
								Polygon->PoPa.IsSeparate      = true;
								Polygon->PoPa.IsInToOut       = true;
								Polygon->PoPa.HiThHold        = CutThresHold;
								Polygon->PoPa.LoThHold        = 0;
								Polygon->PoPa.IgInSide        = true;
								Polygon->PoPa.IsHorSort       = true;
								Polygon->PoPa.IsVerSort       = false;
								Polygon->PoPa.IsInToLeft      = true;
								Polygon->PoPa.LeftFirstPoint  = true;
								Polygon->PoPa.RightFirstPoint = false;

								if (P1.x != 0 && P1.y != 0 && P2.x != 0 && P2.y != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
									Polygon->OnExecute(P1, P2, P3, P4);
								}

								// 2���̻� ���� � ������ ����� �з� ����(���� �ٱ��ʿ� �����͸� ���� �����ͷ� ����)
								if (Polygon->BlobCount > 1) {
									PolyRectCenter.x = 0.0f, PolyRectCenter.y = 0.0f, nPolyPosCnt = 0, RealPolyDis = 0.0f, MinPolyDis = 1000000.0f;
									PolyRectCenter.x = P1.x + ((P2.x - P1.x) / 2); PolyRectCenter.y = P3.y;
									for (int k = 0; k < Polygon->BlobCount; k++) {
										PolygonCenter.x = Polygon->Blob[k].Rect.l + (Polygon->Blob[k].Rect.r - Polygon->Blob[k].Rect.l) / 2;
										PolygonCenter.y = Polygon->Blob[k].Rect.b;
										RealPolyDis = fabs(PolyRectCenter.y - PolygonCenter.y);

										if (RealPolyDis < MinPolyDis) {
											MinPolyDis = RealPolyDis;
											nPolyPosCnt = k;
										}
									}
								}
								else nPolyPosCnt = 0;

								// Polygon Algorithm InEdgeData/OutEdgeData Separate 
								PolygonMarginCnt = (CamId==Cam_Top) ? 5 : 20;
								if (Polygon->Blob[0].InCnt > (PolygonMarginCnt * 2)) {
									BtmInEdgeCount = 0;
									BtmInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt * 2));
									if (BtmInEdgeCount > 0) {
										if (BtmInPoint != NULL) delete[] BtmInPoint;
										BtmInPoint = new TFPoint[BtmInEdgeCount];

										PixelCnt = 0;
										for (int k = 0; k < Polygon->Blob[0].InCnt; k++) {
											if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].InCnt - PolygonMarginCnt)) continue;
											BtmInPoint[PixelCnt].x = Polygon->Blob[0].InPoint[k].x;
											BtmInPoint[PixelCnt].y = Polygon->Blob[0].InPoint[k].y;
											PixelCnt++;
										}
									}
								}

								if (Polygon->Blob[0].OutCnt > (PolygonMarginCnt * 2)) {
									BtmOutEdgeCount = 0;
									BtmOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt * 2));
									if (BtmOutEdgeCount > 0) {
										if (BtmOutPoint != NULL) delete[] BtmOutPoint;
										BtmOutPoint = new TFPoint[BtmOutEdgeCount];

										PixelCnt = 0;
										for (int k = 0; k < Polygon->Blob[0].OutCnt; k++) {
											if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].OutCnt - PolygonMarginCnt)) continue;
											BtmOutPoint[PixelCnt].x = Polygon->Blob[0].OutPoint[k].x;
											BtmOutPoint[PixelCnt].y = Polygon->Blob[0].OutPoint[k].y;
											PixelCnt++;
										}
									}
								}

								// Frmae ���� ���� ������ ��ü �����Ϳ� �Ѱ��� 
								if (BtmInEdgeCount > 0 && BtmOutEdgeCount > 0) {
									// Total Top In Data 
									for (int k = 0; k < BtmInEdgeCount; k++) {
										if (AllBtmInEdgeCount > MAX_ALL_EDGE_COUNT) return false;
										AllBtmInData[AllBtmInEdgeCount].x = BtmInPoint[k].x;
										AllBtmInData[AllBtmInEdgeCount].y = BtmInPoint[k].y;
										AllBtmInEdgeCount++;
									}
									// Total Top Out Data 
									for (int k = 0; k < BtmOutEdgeCount; k++) {
										if (AllBtmOutEdgeCount > MAX_ALL_EDGE_COUNT) return false;
										AllBtmOutData[AllBtmOutEdgeCount].x = BtmOutPoint[k].x;
										AllBtmOutData[AllBtmOutEdgeCount].y = BtmOutPoint[k].y;
										AllBtmOutEdgeCount++;
									}
								}
							}
						}
					}

				    // ��ü ������ ���� ����,������ ����
					if (CamId==Cam_Top) { //LTPS��츸 ����,������ ���� 
						LogUnit.SetLog(L"CUP_GrindMark_Start");
														 //20230320 HSKIM
						for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) {
							if (CGrindMark[i].DvX == 0 || CGrindMark[i].DvY == 0) continue;
							//Train ���� �������� ROI ���� �̵� (��, ASide,CSide Imageȸ�� �� �˻� �����ϹǷ� ���ذ����� �ʿ���)
							//OrgMarkDv.x = (float)CGrindMark[i].DvX; OrgMarkDv.y = (float)CGrindMark[i].DvY;
							//deg = dCSideDeg; //Train ����� ���ذ��� 
							//NewMarkDv = Math.OnRotate(FLBPoint, OrgMarkDv, deg);
							//CGrindMark[i].DvX = (int)NewMarkDv.x; CGrindMark[i].DvY = (int)NewMarkDv.y;

							//ROI�������� 
							CGrindMark[i].Rect.left   = (FLBPoint.x + CGrindMark[i].DvX) - CGrindMark[i].Range;
							CGrindMark[i].Rect.top    = (FLBPoint.y - CGrindMark[i].DvY) - CGrindMark[i].Range;
							CGrindMark[i].Rect.right  = (CGrindMark[i].Rect.left) + (CGrindMark[i].Range*2);
							CGrindMark[i].Rect.bottom = (CGrindMark[i].Rect.top ) + (CGrindMark[i].Range*2);
						}

						for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
							if (k >= MAX_GRINDMARK_ID) continue;
							MarkFind[k].OnLoad(true, FindDir, k);
							MarkFind[k].GImage = &RotateImg;
							MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
							MarkFind[k].SetMinScore(GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
							MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
							MarkFind[k].SetMaxInstances(1);
							MarkFind[k].SetAngleTolerance(20.0f);
							MarkFind[k].SetScaleBias(1.00f);
							MarkFind[k].SetScaleTolerance(0.0f);
							MarkFind[k].Property.Ox = (float)FrameImgW / 2;
							MarkFind[k].Property.Oy = (float)FrameImgH / 2;

						}

						for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) {
							if (CGrindMark[i].Rect.left == 0 || CGrindMark[i].Rect.top == 0 || CGrindMark[i].Rect.right == 0 || CGrindMark[i].Rect.bottom == 0) continue;
							GrindRectWidth  = (CGrindMark[i].Rect.right  - CGrindMark[i].Rect.left);
							GrindRectHeight = (CGrindMark[i].Rect.bottom - CGrindMark[i].Rect.top );

							MaxScore = 0.0f;
							for(int k=0; k<MAX_GRINDMARK_ID; k++){
								if (CGrindMark[i].Rect.left <= 0         || CGrindMark[i].Rect.top <= 0         || GrindRectWidth <= 0         || GrindRectHeight <= 0        ) continue;
								if (CGrindMark[i].Rect.left >= FrameImgW || CGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;

								MarkFind[k].IRoi->SetPlacement(CGrindMark[i].Rect.left, CGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
								MarkFind[k].SetAngleBias(CGrindMark[i].degree);
								MarkFind[k].OnExecute();

								if(MarkFind[k].Result[0].Score>MaxScore){ //GrindMark Search OK 
									MaxScore = MarkFind[k].Result[0].Score;
									CGrindMark[i].Point.x = MarkFind[k].Result[0].Px;
									CGrindMark[i].Point.y = MarkFind[k].Result[0].Py;
									CGrindMark[i].Score   = MarkFind[k].Result[0].Score;
								}
							}

							if (AllBtmOutEdgeCount != 0 && AllBtmInEdgeCount != 0) { // MarkMode��� ������ �˻縦 ���� ���� �� ������ ���� 
								// 25��° GrindMark Search �� ����,���� ������ ���
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								if (CGrindMark[i].Point.x != 0.0f && CGrindMark[i].Point.y != 0.0f) {  //GrindMark ã������� 
									// GrindMark���� OutData �ִܰŸ� Count���� Calc 
									GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
									for (int k = 0; k < AllBtmOutEdgeCount; k++) {
										if (AllBtmOutData[k].x == 0.0f || AllBtmOutData[k].y == 0.0f) continue;
										if (CGrindMark[i].Point.x == 0.0f || CGrindMark[i].Point.y == 0.0f) continue;

										GrindMeaDis = Math.GetLength(CGrindMark[i].Point, AllBtmOutData[k]);
										if (GrindMeaDis < MinDis) {
											MinDis = GrindMeaDis;
											GrindMarkOutCnt = k;
										}
									}
									// GrindMark���� InData �ִܰŸ� Count���� Calc 
									if (GrindMarkOutCnt != 0) {
										GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
										for (int k = 0; k < AllBtmInEdgeCount; k++) {
											if (AllBtmInData[k].x == 0.0f || AllBtmInData[k].y == 0.0f) continue;
											if (AllBtmOutData[GrindMarkOutCnt].x == 0.0f || AllBtmOutData[GrindMarkOutCnt].y == 0.0f) continue;

											GrindMeaDis = Math.GetLength(AllBtmOutData[GrindMarkOutCnt], AllBtmInData[k]);
											if (GrindMeaDis < MinDis) {
												MinDis = GrindMeaDis;
												GrindMarkInCnt = k;
											}
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
									if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
										CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
										for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
											if (AllBtmOutData[k].x == 0.0f || AllBtmOutData[k].y == 0.0f) continue;

											GrindMeaDis = Math.GetLength(CGrindMark[i].Point, AllBtmOutData[k]);
											TotalCirDis += GrindMeaDis;
											CircleCount++;
										}
										if (TotalCirDis != 0.0f && CircleCount != 0) {
											CGrindMark[i].GrindMea = TotalCirDis / CircleCount;
											CGrindMark[i].GrindMea = CGrindMark[i].GrindMea + 0.5; //SubPixel 
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
									if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
										CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
										for (int k = -5; k < 5; k++) {
											if (AllBtmOutData[GrindMarkOutCnt + k].x == 0.0f && AllBtmOutData[GrindMarkOutCnt + k].y == 0.0f) continue;
											if (AllBtmInData[GrindMarkInCnt + k].x == 0.0f && AllBtmInData[GrindMarkInCnt + k].y == 0.0f) continue;
											GrindWidthDis = Math.GetLength(AllBtmInData[GrindMarkInCnt + k], AllBtmOutData[GrindMarkOutCnt + k]);
											TotalCirDis += GrindWidthDis;
											CircleCount++;
										}
										if (TotalCirDis != 0.0f && CircleCount != 0) {
											CGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
											CGrindMark[i].GrindWidth = CGrindMark[i].GrindWidth + 0.5; //SubPixel
										}
									}
								}
								else if (CGrindMark[i].Rect.left != 0 && CGrindMark[i].Rect.top != 0 && CGrindMark[i].Rect.right != 0 && CGrindMark[i].Rect.bottom != 0) {  //GrindMark �� ã�������
									// OutData���� �ּҰ� ������ Count���� 
									GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
									for (int k = 0; k < AllBtmOutEdgeCount; k++) {
										if (AllBtmOutData[k].x == 0.0f || AllBtmOutData[k].y == 0.0f) continue;

										CGrindMark[i].RectPoint.x = CGrindMark[i].Rect.left + (CGrindMark[i].Rect.right - CGrindMark[i].Rect.left) / 2;
										CGrindMark[i].RectPoint.y = CGrindMark[i].Rect.top + (CGrindMark[i].Rect.bottom - CGrindMark[i].Rect.top) / 2;

										GrindMeaDis = Math.GetLength(CGrindMark[i].RectPoint, AllBtmOutData[k]);
										if (GrindMeaDis < MinDis) {
											MinDis = GrindMeaDis;
											GrindMarkOutCnt = k;
										}
									}

									// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
									if (GrindMarkOutCnt != 0) {
										GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
										for (int k = 0; k < AllBtmInEdgeCount; k++) {
											if (AllBtmInData[k].x == 0.0f || AllBtmInData[k].y == 0.0f) continue;
											if (AllBtmOutData[GrindMarkOutCnt].x == 0.0f || AllBtmOutData[GrindMarkOutCnt].y == 0.0f) continue;

											GrindMeaDis = Math.GetLength(AllBtmOutData[GrindMarkOutCnt], AllBtmInData[k]);
											if (GrindMeaDis < MinDis) {
												MinDis = GrindMeaDis;
												GrindMarkInCnt = k;
											}
										}
									}

									// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
									if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
										CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
										for (int k = -5; k < 5; k++) {
											if (AllBtmOutData[GrindMarkOutCnt + k].x == 0.0f && AllBtmOutData[GrindMarkOutCnt + k].y == 0.0f) continue;
											if (AllBtmInData[GrindMarkInCnt + k].x == 0.0f && AllBtmInData[GrindMarkInCnt + k].y == 0.0f) continue;
											GrindWidthDis = Math.GetLength(AllBtmInData[GrindMarkInCnt + k], AllBtmOutData[GrindMarkOutCnt + k]);
											TotalCirDis += GrindWidthDis;
											CircleCount++;
										}
										if (TotalCirDis != 0.0f && CircleCount != 0) {
											CGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
											CGrindMark[i].GrindWidth = CGrindMark[i].GrindWidth + 0.5; //SubPixel
										}
									}
								}
							}
						}
					}

					// Bright_Defect�˻� ������ ����(�ڸ��˻�)
					// ���� ������ �ʿ������...�ϴ� �ּ�ó�� 
					if (CamId==Cam_Top && BrokenThresHold != 0) {
						if (AllBtmInEdgeCount != 0 && AllBtmOutEdgeCount != 0) {
							GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
							GrindMarkOutCnt = (int)(AllBtmOutEdgeCount / 2);
							// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
							if (GrindMarkOutCnt != 0) {
								GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
								for (int k = 0; k < AllBtmInEdgeCount; k++) {
									if (AllBtmInData[k].x == 0.0f || AllBtmInData[k].y == 0.0f) continue;
									if (AllBtmOutData[GrindMarkOutCnt].x == 0.0f || AllBtmOutData[GrindMarkOutCnt].y == 0.0f) continue;

									GrindMeaDis = Math.GetLength(AllBtmOutData[GrindMarkOutCnt], AllBtmInData[k]);
									if (GrindMeaDis < MinDis) {
										MinDis = GrindMeaDis;
										GrindMarkInCnt = k;
									}
								}
							}
						}

						// Bright_Defect�˻� �̹��� ���� 
						BrightDefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
						EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &BrightDefectImg);
						EasyImage::Threshold(GImage, &BrightDefectImg, BrokenThresHold);
						Edge->GImage = &BrightDefectImg;
						Edge->FImgHeight = ImageHeight;
						Edge->SetEdgeType(0);

						int DvCnt = 0, DataCount = 0;
						double deg = 0.0f;
						TFPoint Target(0.0f, 0.0f);

						if (GrindMarkInCnt > GrindMarkOutCnt) {
							DvCnt = GrindMarkInCnt - GrindMarkOutCnt;
							for (int k = 0; k < AllBtmOutEdgeCount; k++) {
								deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
								if (((k + DvCnt) < AllBtmInEdgeCount) && AllBtmInData[k + DvCnt].x > 0 && AllBtmInData[k + DvCnt].y > 0) {
									deg = Math.GetDegree(AllBtmOutData[k], AllBtmInData[k + DvCnt]);

									Target = Math.OnFindPerpendicularPointXY(AllBtmOutData[k], dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
									//Edge�˰��� ��ü �̹��� ��ǥ �������� EdgePoint������, �̷����ؼ� Frame�������� �ٽ� ����ؾ��� 
									Edge->OnExecute(AllBtmOutData[k], Target); // �����ܰ����ο��� Target���� White Point Search 
									if (Edge->GetResultEdgePointCount() > 0) {
										BtmCircleData[DataCount].Dis = 0.0f;
										BtmCircleData[DataCount].Dv = 0.0f;
										BtmCircleData[DataCount].P = Edge->GetResultEdgePoint(0);
										BtmCircleData[DataCount].P.y = BtmCircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
										DataCount++;
									}
								}
							}
							BtmInCrackCount = DataCount;
						}
						else if (GrindMarkInCnt < GrindMarkOutCnt) {
							DvCnt = GrindMarkOutCnt - GrindMarkInCnt;
							for (int k = DvCnt; k < AllBtmOutEdgeCount; k++) {
								deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
								if (((k - DvCnt) < AllBtmInEdgeCount) && AllBtmInData[k - DvCnt].x > 0 && AllBtmInData[k - DvCnt].y > 0 && k - DvCnt >= 0) {
									deg = Math.GetDegree(AllBtmOutData[k], AllBtmInData[k - DvCnt]);

									Target = Math.OnFindPerpendicularPointXY(AllBtmOutData[k], dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
									//Edge�˰��� ��ü �̹��� ��ǥ �������� EdgePoint������, �̷����ؼ� Frame�������� �ٽ� ����ؾ��� 
									Edge->OnExecute(AllBtmOutData[k], Target); // �����ܰ����ο��� Target���� White Point Search 
									if (Edge->GetResultEdgePointCount() > 0) {
										BtmCircleData[DataCount].Dis = 0.0f;
										BtmCircleData[DataCount].Dv = 0.0f;
										BtmCircleData[DataCount].P = Edge->GetResultEdgePoint(0);
										BtmCircleData[DataCount].P.y = BtmCircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
										DataCount++;
									}
								}
							}
							BtmInCrackCount = DataCount;
						}
					}
				}
				else { //�κп��� 
					if (FLeftBottom) { //C1 �κп��� 

						OrgPoint = Math.OnIntersect(Line[waLeftWall].FP0, Line[waLeftWall].FP1, Line[waBottomWall].FP0, Line[waBottomWall].FP1);
						FLBPoint = OrgPoint;

						if (IsCornerBtm) {
							EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindOutThresHold);
							if (CorGrindOutThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
							else                           { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

							// Corner_Cut���� 3Point vs GrindMark �����ؾ���
							IsGrindMarkUse = false;
							for (int k = 0; k < 4; k++) {
								if (CGrindMark[k].DvX != 0 && CGrindMark[k].DvY != 0) { IsGrindMarkUse = true; break; }
							}
							if (!IsGrindMarkUse) { //3Point ���� ����,������ ���� 
								Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;
								// LEFTBTM START 
								///////////////////////////////////////////////////////////////////////////////////////////////////
								SingleLine->IRoi->SetOrgX((int)FLBPoint.x - 10);
								SingleLine->IRoi->SetOrgY((int)FLBPoint.y - LBCornerHeight);
								SingleLine->IRoi->SetWidth((int)LBCornerWidth + 20);
								SingleLine->IRoi->SetHeight((int)LBCornerHeight);

								SingleLine->SetOrientation(orWest);
								SingleLine->FIncrement = 1;
								SingleLine->FThreshold = 15;
								SingleLine->SetFindLineId(0);
								SingleLine->SetEdgeType(1);  //White To Black 
								SingleLine->OnExecute();

								Line[7].FP0 = SingleLine->GetResultP0();
								Line[7].FP1 = SingleLine->GetResultP1();

								// ORG 
								FLBLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
								FLBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);

								if ((FLBLPoint.x == 0 || FLBLPoint.y == 0 || FLBBPoint.x == 0 || FLBBPoint.y == 0) || (SingleLine->FEdgePointCount < 30)) {
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
									if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
										if (C3Point != NULL) delete[] C3Point;
										C3Point = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
										C3EdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

										CornerCount = 0;
										for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
											if (CornerCount >= MAX_CORNER_POINT) break;
											C3Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C3Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									// Corner ���� ���ʶ��� 
									if (CorGrindInThresHold != 0) {
										EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindInThresHold);
										SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;

										// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
										// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
										if (LBCornerWidth > (LBCornerHeight * 1.5)) {
											SingleLine->IRoi->SetOrgX((int)FLBPoint.x);
											SingleLine->IRoi->SetOrgY((int)FLBPoint.y - LBCornerHeight);
											SingleLine->IRoi->SetWidth((int)((FLBBPoint.x - FLBPoint.x) + 20));
											SingleLine->IRoi->SetHeight((int)((FLBPoint.y - FLBLPoint.y)));

											if (FLBPoint.x <= 0 || FLBPoint.y - LBCornerHeight <= 0 || (FLBBPoint.x - FLBPoint.x) <= 0 || ((FLBPoint.y - FLBLPoint.y) + 20) <= 0) return false;
											if (FLBPoint.x >= FrameImgW || FLBPoint.y - LBCornerHeight >= FrameImgH || (FLBBPoint.x - FLBPoint.x) >= FrameImgW || ((FLBPoint.y - FLBLPoint.y) + 20) >= FrameImgH) return false;

											SingleLine->SetOrientation(orSouth);
											SingleLine->FIncrement = 1;
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C3Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C3InPoint != NULL) delete[] C3InPoint;
												C3InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C3InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C3InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C3InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}
										else {
											// Corner ���� ���ʶ��� ã�´� 
											SingleLine->SetOrientation(orWest);
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C1Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C3InPoint != NULL) delete[] C3InPoint;
												C3InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C3InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C3InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C3InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}

										//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
										FLBLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
										FLBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);

										//�ڳ� ������ 
										Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;
										for (int k = 0; k < C3InEdgeCount; k++) {
											Dis = Math.GetLengthFrLineToPoint(FLBLPoint, FLBBPoint, C3InPoint[k]);
											TotalGrindWidth += Dis;
										}
										if (C3InEdgeCount != 0) {
											AvgGrindWidth = (TotalGrindWidth / C3InEdgeCount);
											C3CorGrindWd = AvgGrindWidth;
										}
									}
									// �ڳ� ������ ���� ������ ã�´� 
									// ROI ���� ����(3���� ��� ���� ���� ������ ã�´�)
									if (CorGrindMeaThresHold != 0) {
										EasyImage::Threshold(&RotateImg, &RotateGrindBinaryImg, CorGrindMeaThresHold); // Corner Measure GrindThresHold(3Point)
										if (CorGrindMeaThresHold == 0) { SingleLine->GImage = &RotateImg;  SingleLine->FThreshold = 5; }
										else { SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold = 10; }

										SingleLine->IRoi->SetOrgX((int)FLBPoint.x);
										SingleLine->IRoi->SetOrgY((int)FLBPoint.y - LBCornerHeight + 20);
										SingleLine->IRoi->SetWidth((int)LBCornerWidth + 50);
										SingleLine->IRoi->SetHeight((int)LBCornerHeight);

										SingleLine->SetOrientation(orWest);
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();

										// C3Corner Grind Measure Stand Data 
										if ((SingleLine->FEdgePointCount) > 0) {
											if (C3MeaPoint != NULL) delete[] C3MeaPoint;
											C3MeaPoint = new TFPoint[(SingleLine->FEdgePointCount)];
											C3MeaEdgeCount = (SingleLine->FEdgePointCount);

											CornerCount = 0;
											for (int k = 0; k < (SingleLine->FEdgePointCount); k++) {
												if (CornerCount >= MAX_CORNER_POINT) break;
												if (k == 0                 ) CornerP1 = SingleLine->FEdgePoint[k]; //Chamber StaPos
												if (k == C3MeaEdgeCount - 1) CornerP2 = SingleLine->FEdgePoint[k]; //Chamber EndPos
												C3MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
												C3MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
												CornerCount++;
											}
										}
										//�ڳ� ������ ���� ���ΰ� ���ο� ������ ���� 
										FLBLMeaPoint = Math.OnIntersect(CornerP1, CornerP2, Line[waLeftWall  ].FP0, Line[waLeftWall  ].FP1);
										FLBBMeaPoint = Math.OnIntersect(CornerP1, CornerP2, Line[waBottomWall].FP0, Line[waBottomWall].FP1);

										//�ڳ� ������ ���  
										Dis = 0.0f, AvgGrindMea = 0.0f, TotalGrindMeasure = 0.0f;
										for (int k = 0; k < C3MeaEdgeCount; k++) {
											Dis = Math.GetLengthFrLineToPoint(FLBLPoint, FLBBPoint, C3MeaPoint[k]);
											TotalGrindMeasure += Dis;
										}
										if (C3MeaEdgeCount != 0) {
											AvgGrindMea = (TotalGrindMeasure / C3MeaEdgeCount);
											C3CorGrindMea = AvgGrindMea;
										}
									}
								}
							}
							else { //GrindMark���� ����,������ ���� 
								Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;
								// LEFTBTM START 
								SingleLine->IRoi->SetOrgX((int)FLBPoint.x - 10);
								SingleLine->IRoi->SetOrgY((int)FLBPoint.y - LBCornerHeight);
								SingleLine->IRoi->SetWidth((int)LBCornerWidth + 20);
								SingleLine->IRoi->SetHeight((int)LBCornerHeight);

								SingleLine->SetOrientation(orWest);
								SingleLine->FIncrement = 1;
								SingleLine->FThreshold = 15;
								SingleLine->SetFindLineId(0);
								SingleLine->SetEdgeType(1);  //White To Black 
								SingleLine->OnExecute();

								Line[7].FP0 = SingleLine->GetResultP0();
								Line[7].FP1 = SingleLine->GetResultP1();

								// ORG 
								FLBLPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
								FLBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);

								if ((FLBLPoint.x == 0 || FLBLPoint.y == 0 || FLBBPoint.x == 0 || FLBBPoint.y == 0) || (SingleLine->FEdgePointCount < 30)) {
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
									if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
										if (C3Point != NULL) delete[] C3Point;
										C3Point = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
										C3EdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

										CornerCount = 0;
										for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
											if (CornerCount >= MAX_CORNER_POINT) break;
											C3Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C3Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									// Corner ���� ���ʶ��� 
									if (CorGrindInThresHold != 0) {
										EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindInThresHold);
										SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;

										// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
										// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
										if (LBCornerWidth > (LBCornerHeight * 1.5)) {
											SingleLine->IRoi->SetOrgX((int)FLBPoint.x);
											SingleLine->IRoi->SetOrgY((int)FLBPoint.y - LBCornerHeight);
											SingleLine->IRoi->SetWidth((int)((FLBBPoint.x - FLBPoint.x) + 20));
											SingleLine->IRoi->SetHeight((int)((FLBPoint.y - FLBLPoint.y)));

											if (FLBPoint.x <= 0 || FLBPoint.y - LBCornerHeight <= 0 || (FLBBPoint.x - FLBPoint.x) <= 0 || ((FLBPoint.y - FLBLPoint.y) + 20) <= 0) return false;
											if (FLBPoint.x >= FrameImgW || FLBPoint.y - LBCornerHeight >= FrameImgH || (FLBBPoint.x - FLBPoint.x) >= FrameImgW || ((FLBPoint.y - FLBLPoint.y) + 20) >= FrameImgH) return false;

											SingleLine->SetOrientation(orSouth);
											SingleLine->FIncrement = 1;
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C3Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C3InPoint != NULL) delete[] C3InPoint;
												C3InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C3InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C3InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C3InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}
										else {
											// Corner ���� ���ʶ��� ã�´� 
											SingleLine->SetOrientation(orWest);
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C1Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C3InPoint != NULL) delete[] C3InPoint;
												C3InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C3InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C3InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C3InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}

										//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
										FLBLInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waLeftWall].FP0, Line[waLeftWall].FP1);
										FLBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);
									}

									if(CamId==Cam_Top){
										for (int i = 0; i < 4; i++) {
											if (CGrindMark[i].DvX == 0 || CGrindMark[i].DvY == 0) continue;
											// �̹��� ȸ�� ���� ���� ��ǥ ȸ�� 
											OrgMarkDv.x = (float)CGrindMark[i].DvX; OrgMarkDv.y = (float)CGrindMark[i].DvY;
											deg = dCSideDeg; //Train ����� ���ذ��� 
											NewMarkDv = Math.OnRotate(FLBPoint, OrgMarkDv, deg);
											CGrindMark[i].DvX = (int)NewMarkDv.x; CGrindMark[i].DvY = (int)NewMarkDv.y;

											//ROI�������� 
											CGrindMark[i].Rect.left   = (FLTPoint.x + CGrindMark[i].DvX) - CGrindMark[i].Range;
											CGrindMark[i].Rect.top    = (FLTPoint.y + CGrindMark[i].DvY) - CGrindMark[i].Range;
											CGrindMark[i].Rect.right  = (CGrindMark[i].Rect.left) + (CGrindMark[i].Range*2);
											CGrindMark[i].Rect.bottom = (CGrindMark[i].Rect.top ) + (CGrindMark[i].Range*2);
										}

										for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
											if (k >= MAX_GRINDMARK_ID) continue;
											MarkFind[k].OnLoad(true, FindDir, k);
											MarkFind[k].GImage = &RotateImg;
											MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
											MarkFind[k].SetMinScore(GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
											MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
											MarkFind[k].SetMaxInstances(1);
											MarkFind[k].SetAngleTolerance(20.0f);
											MarkFind[k].SetScaleBias(1.00f);
											MarkFind[k].SetScaleTolerance(0.0f);
											MarkFind[k].Property.Ox = (float)FrameImgW / 2;
											MarkFind[k].Property.Oy = (float)FrameImgH / 2;

										}

										for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) {
											if (CGrindMark[i].Rect.left == 0 || CGrindMark[i].Rect.top == 0 || CGrindMark[i].Rect.right == 0 || CGrindMark[i].Rect.bottom == 0) continue;
											GrindRectWidth  = (CGrindMark[i].Rect.right - CGrindMark[i].Rect.left);
											GrindRectHeight = (CGrindMark[i].Rect.bottom - CGrindMark[i].Rect.top);

											if (CGrindMark[i].Rect.left <= 0 || CGrindMark[i].Rect.top <= 0 || GrindRectWidth <= 0 || GrindRectHeight <= 0) continue;
											if (CGrindMark[i].Rect.left >= FrameImgW || CGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;
											if (CGrindMark[i].Point.x != 0.0f && CGrindMark[i].Point.y != 0.0f) continue;

											MarkID = CGrindMark[i].MarkID; // Multi Teaching MarkID 
											deg = CGrindMark[i].degree;
											MarkFind[MarkID].SetAngleBias(deg);
											MarkFind[MarkID].IRoi->SetPlacement(CGrindMark[i].Rect.left, CGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
											CGrindMark[i].Score = 0.0f;

											if (MarkFind[MarkID].OnExecute()) { //GrindMark Search OK 
												CGrindMark[i].Point.x = MarkFind[MarkID].Result[0].Px;
												CGrindMark[i].Point.y = MarkFind[MarkID].Result[0].Py;
												CGrindMark[i].Score = MarkFind[MarkID].Result[0].Score;
											}
											else {
												MaxScore = 0.0f;
												for (int j = 0; j < MAX_GRINDMARK_ID; j++) {
													MarkFind[j].IRoi->SetPlacement(CGrindMark[i].Rect.left, CGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
													MarkFind[j].SetAngleBias(deg);

													if (MarkFind[j].OnExecute() && MarkFind[j].Result[0].Score > MaxScore) { //GrindMark Search OK 
														CGrindMark[i].Point.x = MarkFind[j].Result[0].Px;
														CGrindMark[i].Point.y = MarkFind[j].Result[0].Py;
														CGrindMark[i].Score = MarkFind[j].Result[0].Score;
														MaxScore = MarkFind[j].Result[0].Score;
													}
												}
											}

											// GrindMakrã����� ������,������ ������
											if (CGrindMark[i].Point.x != 0.0f && CGrindMark[i].Point.y != 0.0f) {
												// GrindMark���� OutData �ִܰŸ� Count���� Calc 
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
												for (int k = 0; k < C3EdgeCount; k++) {
													if (C3Point[k].x == 0.0f || C3Point[k].y == 0.0f) continue;
													if (CGrindMark[i].Point.x == 0.0f || CGrindMark[i].Point.y == 0.0f) continue;

													GrindMeaDis = Math.GetLength(CGrindMark[i].Point, C3Point[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkOutCnt = k;
													}
												}
												// OutData���� InData �ִܰŸ� Count���� Calc 
												if (GrindMarkOutCnt != 0) {
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
													for (int k = 0; k < C3InEdgeCount; k++) {
														if (C3InPoint[k].x == 0.0f || C3InPoint[k].y == 0.0f) continue;
														if (C3Point[GrindMarkOutCnt].x == 0.0f || C3Point[GrindMarkOutCnt].y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(C3Point[GrindMarkOutCnt], C3InPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkInCnt = k;
														}
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
												if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
													for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
														if (C3Point[k].x == 0.0f || C3Point[k].y == 0.0f) continue;
														if (k >= C3EdgeCount) continue;

														GrindMeaDis = Math.GetLength(CGrindMark[i].Point, C3Point[k]);
														TotalCirDis += GrindMeaDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														CGrindMark[i].GrindMea = TotalCirDis / CircleCount;
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
												if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
													for (int k = -5; k < 5; k++) {
														if (C3Point[GrindMarkOutCnt + k].x == 0.0f && C3Point[GrindMarkOutCnt + k].y == 0.0f) continue;
														if (C3InPoint[GrindMarkInCnt + k].x == 0.0f && C3InPoint[GrindMarkInCnt + k].y == 0.0f) continue;
														if ((GrindMarkOutCnt + k) >= C3EdgeCount) continue;
														if ((GrindMarkInCnt + k) >= C3InEdgeCount) continue;

														GrindWidthDis = Math.GetLength(C3InPoint[GrindMarkInCnt + k], C3Point[GrindMarkOutCnt + k]);
														TotalCirDis += GrindWidthDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														CGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
													}
												}
											}
											else { // GrindMrk ��ã����� �������� ������ 
												// OutData���� �ּҰ� ������ Count���� 
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
												for (int k = 0; k < C3EdgeCount; k++) {
													if (C3Point[k].x == 0.0f || C3Point[k].y == 0.0f) continue;

													CGrindMark[i].RectPoint.x = CGrindMark[i].Rect.left + (CGrindMark[i].Rect.right - CGrindMark[i].Rect.left) / 2;
													CGrindMark[i].RectPoint.y = CGrindMark[i].Rect.top + (CGrindMark[i].Rect.bottom - CGrindMark[i].Rect.top) / 2;

													GrindMeaDis = Math.GetLength(CGrindMark[i].RectPoint, C3Point[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkOutCnt = k;
													}
												}

												// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
												if (GrindMarkOutCnt != 0) {
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
													for (int k = 0; k < C3InEdgeCount; k++) {
														if (C3InPoint[k].x == 0.0f || C3InPoint[k].y == 0.0f) continue;
														if (C3Point[GrindMarkOutCnt].x == 0.0f || C3Point[GrindMarkOutCnt].y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(C3Point[GrindMarkOutCnt], C3InPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkInCnt = k;
														}
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
												if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
													for (int k = -5; k < 5; k++) {
														if (C3Point[GrindMarkOutCnt + k].x == 0.0f && C3Point[GrindMarkOutCnt + k].y == 0.0f) continue;
														if (C3InPoint[GrindMarkInCnt + k].x == 0.0f && C3InPoint[GrindMarkInCnt + k].y == 0.0f) continue;
														if ((GrindMarkOutCnt + k) >= C3EdgeCount) continue;
														if ((GrindMarkInCnt + k) >= C3InEdgeCount) continue;

														GrindWidthDis = Math.GetLength(C3InPoint[GrindMarkInCnt + k], C3Point[GrindMarkOutCnt + k]);
														TotalCirDis += GrindWidthDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														CGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
													}
												}
											}
										}
									}
								}
							}
						}
						else if (IsRectBtm) {
							LBDx = 0.0f;
							LBDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FLBLPoint.x = FLBPoint.x;
							FLBLPoint.y = FLBPoint.y;
							FLBBPoint.x = FLBPoint.x;
							FLBBPoint.y = FLBPoint.y;
						}
						else if (IsCircleBtm) { //FLeftBtm 
							GrindRectWidth  = 0;
							GrindRectHeight = 0;
							EasyImage::Threshold(&RotateImg, &RotateBinaryImg, NotchSideThresHold);
							if (NotchSideThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
							else                         { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

							// Find Mark Search Start
							////////////////////////////////////////////////////////////////////////////
							// NOTCH LeftBtm
							if (FInspectMode == Insp_Polygon) { // ���� � �˰��� 
								if (LBCircleWidth != 0 && LBCircleHeight != 0) {
									P1.x =0, P1.y=0, P2.x=0, P2.y=0, P3.x=0, P3.y=0, P4.x=0, P4.y=0;

									P1.x = (int)(FLBPoint.x); P1.y = (int)(FLBPoint.y - LBCircleHeight); P2.x = (int)(P1.x + LBCircleWidth); P2.y = (int)(P1.y);
									P3.x = (int)(P1.x);       P3.y = (int)(P1.y + LBCircleHeight);       P4.x = (int)(P2.x);                 P4.y = (int)(P3.y);

									// �˻�ȭ�� Display ���� 
									NotchPolyRect[0].left  = P1.x;   NotchPolyRect[0].top    = P1.y;
									NotchPolyRect[0].right = P2.x;   NotchPolyRect[0].bottom = P3.y;

									int PixelCnt = 0;

									// Median Filter ���� 
									if (P1.x <= 0 || P1.y <= 0 || (P2.x - P1.x) <= 0 || (P3.y - P1.y) <= 0) return false;
									if (P1.x >= FrameImgW || P1.y >= FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) >= FrameImgH) return false;
									if ((P1.x + (P2.x - P1.x)) >= FrameImgW || (P1.y + (P3.y - P1.y)) > FrameImgH) return false;

									FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
									FilterROI.Attach(&RotateBinaryImg);
									FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
									LogUnit.SetLog(L"LeftBtm ==>Px:%d, Py:%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
//									EasyImage::Median(&FilterROI, &FilterROI);
									OnFilter(ftMedian, &FilterROI, &FilterROI);

									Polygon->GImage               = &RotateBinaryImg;
									Polygon->IWidth               = FrameImgW;
									Polygon->IHeight              = FrameImgH;
									Polygon->PoPa.FMinWidth       = (int)((P2.x - P1.x) / 2);
									Polygon->PoPa.FMaxWidth       = (int)((P2.x - P1.x) + 100);
									Polygon->PoPa.FMinHeight      = 2;
									Polygon->PoPa.FMaxHeight      = (int)((P3.y - P1.y) + 100);
									Polygon->PoPa.MinPxCnt        = 100;
									Polygon->PoPa.MaxPxCnt        = 20000;
									Polygon->PoPa.MaxBbCnt        = 10;
									Polygon->PoPa.IsSeparate      = true;
									Polygon->PoPa.IsInToOut       = true;
									Polygon->PoPa.HiThHold        = NotchSideThresHold;
									Polygon->PoPa.LoThHold        = 0;
									Polygon->PoPa.IgInSide        = true;
									Polygon->PoPa.IsHorSort       = true;
									Polygon->PoPa.IsVerSort       = false;
									Polygon->PoPa.IsInToLeft      = true;
									Polygon->PoPa.LeftFirstPoint  = true;
									Polygon->PoPa.RightFirstPoint = false;


									if (P1.x != 0 && P1.y != 0 && P2.x != 0 && P2.y != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
										Polygon->OnExecute(P1, P2, P3, P4);
									}

									// Polygon Algorithm InEdgeData/OutEdgeData Separate 
									if (Polygon->BlobCount == 1) {
										PolygonMarginCnt = (CamId==Cam_Top) ? 5 : 20;
										if (Polygon->Blob[0].InCnt > (PolygonMarginCnt * 2)) {
											C1CirInEdgeCount = 0;
											C1CirInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt * 2));
											if (C1CirInEdgeCount > 0) {
												if (C1CirInPoint != NULL) delete[] C1CirInPoint;
												C1CirInPoint = new TFPoint[C1CirInEdgeCount];

												PixelCnt = 0;
												for (int k = 0; k < Polygon->Blob[0].InCnt; k++) {
													if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].InCnt - PolygonMarginCnt)) continue;
													C1CirInPoint[PixelCnt].x = Polygon->Blob[0].InPoint[k].x;
													C1CirInPoint[PixelCnt].y = Polygon->Blob[0].InPoint[k].y;
													PixelCnt++;
												}
											}
										}

										if (Polygon->Blob[0].OutCnt > (PolygonMarginCnt * 2)) {
											C1CirOutEdgeCount = 0;
											C1CirOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt * 2));
											if (C1CirOutEdgeCount > 0) {
												if (C1CirOutPoint != NULL) delete[] C1CirOutPoint;
												C1CirOutPoint = new TFPoint[C1CirOutEdgeCount];

												PixelCnt = 0;
												for (int k = 0; k < Polygon->Blob[0].OutCnt; k++) {
													if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].OutCnt - PolygonMarginCnt)) continue;
													C1CirOutPoint[PixelCnt].x = Polygon->Blob[0].OutPoint[k].x;
													C1CirOutPoint[PixelCnt].y = Polygon->Blob[0].OutPoint[k].y;
													PixelCnt++;
												}
											}
										}
									}

									// InEdgeData/OutEdgeData ���� ������ �� ������ ���
									// Polygon �˰��򿡼��� GrindMark���� OutData,InData �ִܰŸ� 2�� Tag�� �����ؾ��� 
									if (CamId==Cam_Top) { // LTPS ������,������ ���
										for (int i = 0; i < 4; i++) {
											if (CGrindMark[i].DvX == 0 || CGrindMark[i].DvY == 0) continue;
											// �̹��� ȸ�� ���� ���� ��ǥ ȸ�� 
											OrgMarkDv.x = (float)CGrindMark[i].DvX; OrgMarkDv.y = (float)CGrindMark[i].DvY;
											deg = dCSideDeg; //Train ����� ���ذ��� 
											NewMarkDv = Math.OnRotate(FLBPoint, OrgMarkDv, deg);
											CGrindMark[i].DvX = (int)NewMarkDv.x; CGrindMark[i].DvY = (int)NewMarkDv.y;

											//ROI�������� 
											CGrindMark[i].Rect.left   = (FLTPoint.x + CGrindMark[i].DvX) - CGrindMark[i].Range;
											CGrindMark[i].Rect.top    = (FLTPoint.y + CGrindMark[i].DvY) - CGrindMark[i].Range;
											CGrindMark[i].Rect.right  = (CGrindMark[i].Rect.left) + (CGrindMark[i].Range*2);
											CGrindMark[i].Rect.bottom = (CGrindMark[i].Rect.top ) + (CGrindMark[i].Range*2);
										}

										for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
											if (k >= MAX_GRINDMARK_ID) continue;
											MarkFind[k].OnLoad(true, FindDir, k);
											MarkFind[k].GImage = &RotateImg;
											MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
											MarkFind[k].SetMinScore(GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
											MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
											MarkFind[k].SetMaxInstances(1);
											MarkFind[k].SetAngleTolerance(20.0f);
											MarkFind[k].SetScaleBias(1.00f);
											MarkFind[k].SetScaleTolerance(0.0f);
											MarkFind[k].Property.Ox = (float)FrameImgW / 2;
											MarkFind[k].Property.Oy = (float)FrameImgH / 2;

										}

										for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) {
											if (CGrindMark[i].Rect.left == 0 || CGrindMark[i].Rect.top == 0 || CGrindMark[i].Rect.right == 0 || CGrindMark[i].Rect.bottom == 0) continue;
											GrindRectWidth  = (CGrindMark[i].Rect.right - CGrindMark[i].Rect.left);
											GrindRectHeight = (CGrindMark[i].Rect.bottom - CGrindMark[i].Rect.top);

											if (CGrindMark[i].Rect.left <= 0 || CGrindMark[i].Rect.top <= 0 || GrindRectWidth <= 0 || GrindRectHeight <= 0) continue;
											if (CGrindMark[i].Rect.left >= FrameImgW || CGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;
											if (CGrindMark[i].Point.x != 0.0f && CGrindMark[i].Point.y != 0.0f) continue;

											MarkID = CGrindMark[i].MarkID; // Multi Teaching MarkID 
											deg = CGrindMark[i].degree;
											MarkFind[MarkID].SetAngleBias(deg);
											MarkFind[MarkID].IRoi->SetPlacement(CGrindMark[i].Rect.left, CGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
											CGrindMark[i].Score = 0.0f;

											if (MarkFind[MarkID].OnExecute()) { //GrindMark Search OK 
												CGrindMark[i].Point.x = MarkFind[MarkID].Result[0].Px;
												CGrindMark[i].Point.y = MarkFind[MarkID].Result[0].Py;
												CGrindMark[i].Score   = MarkFind[MarkID].Result[0].Score;
											}
											else {
												MaxScore = 0.0f;
												for (int j = 0; j < MAX_GRINDMARK_ID; j++) {
													MarkFind[j].IRoi->SetPlacement(CGrindMark[i].Rect.left, CGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
													MarkFind[j].SetAngleBias(deg);

													if (MarkFind[j].OnExecute() && MarkFind[j].Result[0].Score > MaxScore) { //GrindMark Search OK 
														CGrindMark[i].Point.x = MarkFind[j].Result[0].Px;
														CGrindMark[i].Point.y = MarkFind[j].Result[0].Py;
														CGrindMark[i].Score = MarkFind[j].Result[0].Score;
														MaxScore = MarkFind[j].Result[0].Score;
													}
												}
											}
											//20230320 HSKIM

											if (C1CirOutEdgeCount != 0 && C1CirInEdgeCount != 0) { // MarkMode��� ������ �˻縦 ���� ���� �� ������ ���� 
												// 25��° GrindMark Search �� ����,���� ������ ���
												/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
												if (CGrindMark[i].Point.x != 0.0f && CGrindMark[i].Point.y != 0.0f) {  //GrindMark ã������� 
													// GrindMark���� OutData �ִܰŸ� Count���� Calc 
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
													for (int k = 0; k < C1CirOutEdgeCount; k++) {
														if (C1CirOutPoint[k].x == 0.0f || C1CirOutPoint[k].y == 0.0f) continue;
														if (CGrindMark[i].Point.x == 0.0f || CGrindMark[i].Point.y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(CGrindMark[i].Point, C1CirOutPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkOutCnt = k;
														}
													}
													// GrindMark���� InData �ִܰŸ� Count���� Calc 
													if (GrindMarkOutCnt != 0) {
														GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
														for (int k = 0; k < C1CirInEdgeCount; k++) {
															if (C1CirInPoint[k].x == 0.0f || C1CirInPoint[k].y == 0.0f) continue;
															if (C1CirOutPoint[GrindMarkOutCnt].x == 0.0f || C1CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(C1CirOutPoint[GrindMarkOutCnt], C1CirInPoint[k]);
															if (GrindMeaDis < MinDis) {
																MinDis = GrindMeaDis;
																GrindMarkInCnt = k;
															}
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
													if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
														for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
															if (C1CirOutPoint[k].x == 0.0f || C1CirOutPoint[k].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(CGrindMark[i].Point, C1CirOutPoint[k]);
															TotalCirDis += GrindMeaDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															CGrindMark[i].GrindMea = TotalCirDis / CircleCount;
															CGrindMark[i].GrindMea = CGrindMark[i].GrindMea + 0.5; //SubPixel 
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
													if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
														for (int k = -5; k < 5; k++) {
															if (C1CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && C1CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
															if (C1CirInPoint[GrindMarkInCnt + k].x == 0.0f && C1CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
															GrindWidthDis = Math.GetLength(C1CirInPoint[GrindMarkInCnt + k], C1CirOutPoint[GrindMarkOutCnt + k]);
															TotalCirDis += GrindWidthDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															CGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
															CGrindMark[i].GrindWidth = CGrindMark[i].GrindWidth + 0.5; //SubPixel
														}
													}
												}
												else if (CGrindMark[i].Rect.left != 0 && CGrindMark[i].Rect.top != 0 && CGrindMark[i].Rect.right != 0 && CGrindMark[i].Rect.bottom != 0) {  //GrindMark �� ã�������
													// OutData���� �ּҰ� ������ Count���� 
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
													for (int k = 0; k < C1CirOutEdgeCount; k++) {
														if (C1CirOutPoint[k].x == 0.0f || C1CirOutPoint[k].y == 0.0f) continue;

														CGrindMark[i].RectPoint.x = CGrindMark[i].Rect.left + (CGrindMark[i].Rect.right - CGrindMark[i].Rect.left) / 2;
														CGrindMark[i].RectPoint.y = CGrindMark[i].Rect.top + (CGrindMark[i].Rect.bottom - CGrindMark[i].Rect.top) / 2;

														GrindMeaDis = Math.GetLength(CGrindMark[i].RectPoint, C1CirOutPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkOutCnt = k;
														}
													}

													// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
													if (GrindMarkOutCnt != 0) {
														GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
														for (int k = 0; k < C1CirInEdgeCount; k++) {
															if (C1CirInPoint[k].x == 0.0f || C1CirInPoint[k].y == 0.0f) continue;
															if (C1CirOutPoint[GrindMarkOutCnt].x == 0.0f || C1CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(C1CirOutPoint[GrindMarkOutCnt], C1CirInPoint[k]);
															if (GrindMeaDis < MinDis) {
																MinDis = GrindMeaDis;
																GrindMarkInCnt = k;
															}
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
													if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
														for (int k = -5; k < 5; k++) {
															if (C1CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && C1CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
															if (C1CirInPoint[GrindMarkInCnt + k].x == 0.0f && C1CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
															GrindWidthDis = Math.GetLength(C1CirInPoint[GrindMarkInCnt + k], C1CirOutPoint[GrindMarkOutCnt + k]);
															TotalCirDis += GrindWidthDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															CGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
															CGrindMark[i].GrindWidth = CGrindMark[i].GrindWidth + 0.5; //SubPixel
														}
													}
												}
											}
										}
									}
									else { //Polygon EnCap ���� OutPoint �߰��� ���� ���� �Ÿ� ��հ��� ����Ѵ�(LeftTop) 
										if (C1CirInEdgeCount != 0 && C1CirOutEdgeCount != 0) {
											GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
											GrindMarkOutCnt = (int)(C1CirOutEdgeCount / 2);
											// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
											if (GrindMarkOutCnt != 0) {
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
												for (int k = 0; k < C1CirInEdgeCount; k++) {
													if (C1CirInPoint[k].x == 0.0f || C1CirInPoint[k].y == 0.0f) continue;
													if (C1CirOutPoint[GrindMarkOutCnt].x == 0.0f || C1CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

													GrindMeaDis = Math.GetLength(C1CirOutPoint[GrindMarkOutCnt], C1CirInPoint[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkInCnt = k;
													}
												}
											}

											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
											if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
												CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
												for (int k = -5; k < 5; k++) {
													if (C1CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && C1CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
													if (C1CirInPoint[GrindMarkInCnt + k].x == 0.0f && C1CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
													GrindWidthDis = Math.GetLength(C1CirInPoint[GrindMarkInCnt + k], C1CirOutPoint[GrindMarkOutCnt + k]);
													TotalCirDis += GrindWidthDis;
													CircleCount++;
												}
											}
										}
									}

									// Polygon �ڸ� �˻� ������(LeftTop) 
									// ���� �ʿ� ������ �ϴ�...�ּ�ó�� 
									if (CamId==Cam_Top && BrokenThresHold != 0) {
										RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
										EasyImage::Oper(EArithmeticLogicOperation_Copy, &RotateImg, &RotateBrokenImg);
										EasyImage::Threshold(&RotateImg, &RotateBrokenImg, BrokenThresHold);
										Edge->GImage = &RotateBrokenImg;
										Edge->FImgHeight = ImageHeight;
										Edge->SetEdgeType(0);

										int DvCnt = 0, DataCount = 0;
										double deg = 0.0f;
										TFPoint Target(0.0f, 0.0f);

										if (GrindMarkInCnt > GrindMarkOutCnt) {
											DvCnt = GrindMarkInCnt - GrindMarkOutCnt;
											for (int k = 0; k < C1CirOutEdgeCount; k++) {
												deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
												if (((k + DvCnt) < C1CirInEdgeCount) && C1CirInPoint[k + DvCnt].x > 0 && C1CirInPoint[k + DvCnt].y > 0) {
													deg = Math.GetDegree(C1CirOutPoint[k], C1CirInPoint[k + DvCnt]);

													Target = Math.OnFindPerpendicularPointXY(C1CirOutPoint[k], dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
													Edge->OnExecute(C1CirOutPoint[k], Target); // �����ܰ����ο��� Target���� White Point Search 
													if (Edge->GetResultEdgePointCount() > 0) {
														C1CircleData[DataCount].Dis = 0.0f;
														C1CircleData[DataCount].Dv = 0.0f;
														C1CircleData[DataCount].P = Edge->GetResultEdgePoint(0);
														C1CircleData[DataCount].P.y = C1CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
														DataCount++;
													}
												}
											}
											C1CirInCrackCount = DataCount;
										}
										else if (GrindMarkInCnt < GrindMarkOutCnt) {
											DvCnt = GrindMarkOutCnt - GrindMarkInCnt;
											for (int k = DvCnt; k < C1CirOutEdgeCount; k++) {
												deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
												if (((k - DvCnt) < C1CirInEdgeCount) && C1CirInPoint[k - DvCnt].x > 0 && C1CirInPoint[k - DvCnt].y > 0 && k - DvCnt >= 0) {
													deg = Math.GetDegree(C1CirOutPoint[k], C1CirInPoint[k - DvCnt]);

													Target = Math.OnFindPerpendicularPointXY(C1CirOutPoint[k], dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
													Edge->OnExecute(C1CirOutPoint[k], Target); // �����ܰ����ο��� Target���� White Point Search 
													if (Edge->GetResultEdgePointCount() > 0) {
														C1CircleData[DataCount].Dis = 0.0f;
														C1CircleData[DataCount].Dv = 0.0f;
														C1CircleData[DataCount].P = Edge->GetResultEdgePoint(0);
														C1CircleData[DataCount].P.y = C1CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
														DataCount++;
													}
												}
											}
											C1CirInCrackCount = DataCount;
										}
									}
								}
							}
						}
					}
					if (FRightBottom) { //C2 �κп��� 
						  
						OrgPoint = Math.OnIntersect(Line[waRightWall].FP0, Line[waRightWall].FP1, Line[waBottomWall].FP0, Line[waBottomWall].FP1);
						FRBPoint = OrgPoint;

						if (IsCornerBtm) {
							EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindOutThresHold);
							if (CorGrindOutThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
							else                           { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

							// Corner_Cut���� 3Point vs GrindMark �����ؾ���
							IsGrindMarkUse = false;
							for (int k = 19; k < 23; k++) {
								if (CGrindMark[k].DvX != 0 && CGrindMark[k].DvY != 0) { IsGrindMarkUse = true; break; }
							}
							if (!IsGrindMarkUse) { //3Point ���� ����,������ ���� 
								// RIGHTBTOM START 
								SingleLine->IRoi->SetOrgX((int)FRBPoint.x - RBCornerWidth);
								SingleLine->IRoi->SetOrgY((int)FRBPoint.y - RBCornerHeight);
								SingleLine->IRoi->SetWidth((int)RBCornerWidth + 10);
								SingleLine->IRoi->SetHeight((int)RBCornerHeight);

								SingleLine->SetOrientation(orEast);
								SingleLine->FIncrement = 1;
								SingleLine->FThreshold = 15;
								SingleLine->SetFindLineId(0);
								SingleLine->SetEdgeType(1);  //White To Black 
								SingleLine->OnExecute();

								Line[6].FP0 = SingleLine->GetResultP0();
								Line[6].FP1 = SingleLine->GetResultP1();

								// ORG 
								FRBRPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
								FRBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);

								if ((FRBRPoint.x == 0 || FRBRPoint.y == 0 || FRBBPoint.x == 0 || FRBBPoint.y == 0) || (SingleLine->FEdgePointCount < 30)) {
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
									if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
										if (C4Point != NULL) delete[] C4Point;
										C4Point = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
										C4EdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

										CornerCount = 0;
										for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
											if (CornerCount >= MAX_CORNER_POINT) break;
											C4Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C4Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
									// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
									if (CorGrindInThresHold != 0) {
										EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindInThresHold);
										SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;

										if (RBCornerWidth > (RBCornerHeight * 1.5)) {
											SingleLine->IRoi->SetOrgX((int)FRBBPoint.x);
											SingleLine->IRoi->SetOrgY((int)FRBBPoint.y - RBCornerHeight);
											SingleLine->IRoi->SetWidth((int)(RBCornerWidth + 10));
											SingleLine->IRoi->SetHeight((int)(RBCornerHeight));

											if (FRBBPoint.x <= 0 || FRBBPoint.y - RBCornerHeight <= 0 || RBCornerWidth + 10 <= 0 || RBCornerHeight + 10 <= 0) return false;
											if (FRBBPoint.x >= FrameImgW || FRBBPoint.y - RBCornerHeight >= FrameImgH || RBCornerWidth + 10 >= FrameImgW || RBCornerHeight + 10 >= FrameImgH) return false;

											SingleLine->SetOrientation(orSouth);
											SingleLine->FIncrement = 1;
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C4Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C4InPoint != NULL) delete[] C4InPoint;
												C4InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C4InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C4InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C4InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}
										else {
											// Corner ���� ���ʶ��� ã�´� 
											SingleLine->SetOrientation(orEast);
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C4Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C4InPoint != NULL) delete[] C4InPoint;
												C4InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C4InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C4InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C4InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}

										//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
										FRBRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
										FRBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);

										//�ڳ� ������ 
										Dis = 0.0f, AvgGrindWidth = 0.0f, AvgGrindMea = 0.0f, TotalGrindWidth = 0.0f, TotalGrindMeasure = 0.0f;
										for (int k = 0; k < C4InEdgeCount; k++) {
											Dis = Math.GetLengthFrLineToPoint(FLBLPoint, FLBBPoint, C4InPoint[k]);
											TotalGrindWidth += Dis;
										}
										if (C4InEdgeCount != 0) {
											AvgGrindWidth = (TotalGrindWidth / C4InEdgeCount);
											C4CorGrindWd = AvgGrindWidth;
										}
									}

									// �ڳ� ������ ���� ������ ã�´� 
									// ROI ���� ����(3���� ��� ���� ���� ������ ã�´�)
									if (CorGrindMeaThresHold != 0) {
										EasyImage::Threshold(&RotateImg, &RotateGrindBinaryImg, CorGrindMeaThresHold); // Corner Measure GrindThresHold(3Point)
										if (CorGrindMeaThresHold == 0) { SingleLine->GImage = &RotateImg;  SingleLine->FThreshold = 5; }
										else { SingleLine->GImage = &RotateGrindBinaryImg;  SingleLine->FThreshold = 10; }

										SingleLine->IRoi->SetOrgX((int)FRBPoint.x - RBCornerWidth - 20);
										SingleLine->IRoi->SetOrgY((int)FRBPoint.y - RBCornerHeight + 20);
										SingleLine->IRoi->SetWidth((int)RBCornerWidth);
										SingleLine->IRoi->SetHeight((int)RBCornerHeight);

										SingleLine->SetOrientation(orEast);
										SingleLine->SetFindLineId(0);
										SingleLine->SetEdgeType(0);  //Black To White 
										SingleLine->OnExecute();

										// C4Corner Grind Measure Stand Data 
										if ((SingleLine->FEdgePointCount) > 0) {
											if (C4MeaPoint != NULL) delete[] C4MeaPoint;
											C4MeaPoint = new TFPoint[(SingleLine->FEdgePointCount)];
											C4MeaEdgeCount = (SingleLine->FEdgePointCount);

											CornerCount = 0;
											for (int k = 0; k < (SingleLine->FEdgePointCount); k++) {
												if (CornerCount >= MAX_CORNER_POINT) break;
												if (k == 0                 ) CornerP1 = SingleLine->FEdgePoint[k]; //Chamber StaPos
												if (k == C4MeaEdgeCount - 1) CornerP2 = SingleLine->FEdgePoint[k]; //Chamber EndPos
												C4MeaPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
												C4MeaPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
												CornerCount++;
											}
										}

										//�ڳ� ������ ���� ���ΰ� ���ο� ������ ���� 
										FRBRMeaPoint = Math.OnIntersect(CornerP1, CornerP1, Line[waRightWall ].FP0, Line[waRightWall ].FP1);
										FRBBMeaPoint = Math.OnIntersect(CornerP2, CornerP2, Line[waBottomWall].FP0, Line[waBottomWall].FP1);

										//�ڳ� ������ ���  
										Dis = 0.0f, AvgGrindMea = 0.0f, TotalGrindMeasure = 0.0f;
										for (int k = 0; k < C4MeaEdgeCount; k++) {
											Dis = Math.GetLengthFrLineToPoint(FRBRPoint, FRBBPoint, C4MeaPoint[k]);
											TotalGrindMeasure += Dis;
										}
										if (C4MeaEdgeCount != 0) {
											AvgGrindMea = (TotalGrindMeasure / C4MeaEdgeCount);
											C4CorGrindMea = AvgGrindMea;
										}
									}
								}
							}
							else { //GrindMark���� ����,������ ���� 
								// RIGHTBTOM START 
								SingleLine->IRoi->SetOrgX((int)FRBPoint.x - RBCornerWidth);
								SingleLine->IRoi->SetOrgY((int)FRBPoint.y - RBCornerHeight);
								SingleLine->IRoi->SetWidth((int)RBCornerWidth + 10);
								SingleLine->IRoi->SetHeight((int)RBCornerHeight);

								SingleLine->SetOrientation(orEast);
								SingleLine->FIncrement = 1;
								SingleLine->FThreshold = 15;
								SingleLine->SetFindLineId(0);
								SingleLine->SetEdgeType(1);  //White To Black 
								SingleLine->OnExecute();

								Line[6].FP0 = SingleLine->GetResultP0();
								Line[6].FP1 = SingleLine->GetResultP1();

								// ORG 
								FRBRPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
								FRBBPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);

								if ((FRBRPoint.x == 0 || FRBRPoint.y == 0 || FRBBPoint.x == 0 || FRBBPoint.y == 0) || (SingleLine->FEdgePointCount < 30)) {
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
									if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
										if (C4Point != NULL) delete[] C4Point;
										C4Point = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
										C4EdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

										CornerCount = 0;
										for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
											if (CornerCount >= MAX_CORNER_POINT) break;
											C4Point[CornerCount].x = SingleLine->FEdgePoint[k].x;
											C4Point[CornerCount].y = SingleLine->FEdgePoint[k].y;
											CornerCount++;
										}
									}

									// Corner ���� �ϸ��Ͽ� Edgeã�� ��� Defect ����ų �߻��� 
									// ������ Top���⿡�� �ٽ� ������ ã�� (Corner Defect OverKill ���� ����)
									if (CorGrindInThresHold != 0) {
										EasyImage::Threshold(&RotateImg, &RotateBinaryImg, CorGrindInThresHold);
										SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;

										if (RBCornerWidth > (RBCornerHeight * 1.5)) {
											SingleLine->IRoi->SetOrgX((int)FRBBPoint.x);
											SingleLine->IRoi->SetOrgY((int)FRBBPoint.y - RBCornerHeight);
											SingleLine->IRoi->SetWidth((int)(RBCornerWidth + 10));
											SingleLine->IRoi->SetHeight((int)(RBCornerHeight));

											if (FRBBPoint.x <= 0 || FRBBPoint.y - RBCornerHeight <= 0 || RBCornerWidth + 10 <= 0 || RBCornerHeight + 10 <= 0) return false;
											if (FRBBPoint.x >= FrameImgW || FRBBPoint.y - RBCornerHeight >= FrameImgH || RBCornerWidth + 10 >= FrameImgW || RBCornerHeight + 10 >= FrameImgH) return false;

											SingleLine->SetOrientation(orSouth);
											SingleLine->FIncrement = 1;
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C4Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C4InPoint != NULL) delete[] C4InPoint;
												C4InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C4InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C4InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C4InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}
										else {
											// Corner ���� ���ʶ��� ã�´� 
											SingleLine->SetOrientation(orEast);
											SingleLine->SetFindLineId(0);
											SingleLine->SetEdgeType(0);  //Black To White 
											SingleLine->OnExecute();

											// InLine C4Corner Data 
											if ((SingleLine->FEdgePointCount) - (CornerMargin * 2) > 0) {
												if (C4InPoint != NULL) delete[] C4InPoint;
												C4InPoint = new TFPoint[(SingleLine->FEdgePointCount) - (CornerMargin * 2)];
												C4InEdgeCount = (SingleLine->FEdgePointCount) - (CornerMargin * 2);

												CornerCount = 0;
												for (int k = CornerMargin; k < ((SingleLine->FEdgePointCount) - CornerMargin); k++) {
													if (CornerCount >= MAX_CORNER_POINT) break;
													C4InPoint[CornerCount].x = SingleLine->FEdgePoint[k].x;
													C4InPoint[CornerCount].y = SingleLine->FEdgePoint[k].y;
													CornerCount++;
												}
											}
										}

										//�ڳ� �������ʶ��� ���� ���ο� ������ ���� 
										FRBRInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waRightWall].FP0, Line[waRightWall].FP1);
										FRBBInPoint = Math.OnIntersect(SingleLine->GetResultP0(), SingleLine->GetResultP1(), Line[waBottomWall].FP0, Line[waBottomWall].FP1);
									}

									if(CamId==Cam_Top){
										// GrindMark���� ������,������ ������ 
										for (int i = 19; i < 23; i++) {
											if (CGrindMark[i].DvX == 0 || CGrindMark[i].DvY == 0) continue;
											// �̹��� ȸ�� ���� ���� ��ǥ ȸ�� 
											OrgMarkDv.x = (float)CGrindMark[i].DvX; OrgMarkDv.y = (float)CGrindMark[i].DvY;
											deg = dCSideDeg; //Train ����� ���ذ��� 
											NewMarkDv = Math.OnRotate(FLBPoint, OrgMarkDv, deg);
											CGrindMark[i].DvX = (int)NewMarkDv.x; CGrindMark[i].DvY = (int)NewMarkDv.y;

											//ROI�������� 
											CGrindMark[i].Rect.left   = (FLBPoint.x + CGrindMark[i].DvX) - CGrindMark[i].Range;
											CGrindMark[i].Rect.top    = (FLBPoint.y + CGrindMark[i].DvY) - CGrindMark[i].Range;
											CGrindMark[i].Rect.right  = (CGrindMark[i].Rect.left) + (CGrindMark[i].Range*2);
											CGrindMark[i].Rect.bottom = (CGrindMark[i].Rect.top ) + (CGrindMark[i].Range*2);
										}

										for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
											if (k >= MAX_GRINDMARK_ID) continue;
											MarkFind[k].OnLoad(true, FindDir, k);
											MarkFind[k].GImage = &RotateImg;
											MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
											MarkFind[k].SetMinScore(GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
											MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
											MarkFind[k].SetMaxInstances(1);
											MarkFind[k].SetAngleTolerance(20.0f);
											MarkFind[k].SetScaleBias(1.00f);
											MarkFind[k].SetScaleTolerance(0.0f);
											MarkFind[k].Property.Ox = (float)FrameImgW / 2;
											MarkFind[k].Property.Oy = (float)FrameImgH / 2;
										}

										for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) {
											if (CGrindMark[i].Rect.left == 0 || CGrindMark[i].Rect.top == 0 || CGrindMark[i].Rect.right == 0 || CGrindMark[i].Rect.bottom == 0) continue;
											GrindRectWidth  = (CGrindMark[i].Rect.right - CGrindMark[i].Rect.left);
											GrindRectHeight = (CGrindMark[i].Rect.bottom - CGrindMark[i].Rect.top);

											if (CGrindMark[i].Rect.left <= 0 || CGrindMark[i].Rect.top <= 0 || GrindRectWidth <= 0 || GrindRectHeight <= 0) continue;
											if (CGrindMark[i].Rect.left >= FrameImgW || CGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;
											if (CGrindMark[i].Point.x != 0.0f && CGrindMark[i].Point.y != 0.0f) continue;

											MarkID = CGrindMark[i].MarkID; // Multi Teaching MarkID 
											deg = CGrindMark[i].degree;
											MarkFind[MarkID].SetAngleBias(deg);
											MarkFind[MarkID].IRoi->SetPlacement(CGrindMark[i].Rect.left, CGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
											CGrindMark[i].Score = 0.0f;

											if (MarkFind[MarkID].OnExecute()) { //GrindMark Search OK 
												CGrindMark[i].Point.x = MarkFind[MarkID].Result[0].Px;
												CGrindMark[i].Point.y = MarkFind[MarkID].Result[0].Py;
												CGrindMark[i].Score = MarkFind[MarkID].Result[0].Score;
											}
											else {
												MaxScore = 0.0f;
												for (int j = 0; j < MAX_GRINDMARK_ID; j++) {
													MarkFind[j].IRoi->SetPlacement(CGrindMark[i].Rect.left, CGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
													MarkFind[j].SetAngleBias(deg);

													if (MarkFind[j].OnExecute() && MarkFind[j].Result[0].Score > MaxScore) { //GrindMark Search OK 
														CGrindMark[i].Point.x = MarkFind[j].Result[0].Px;
														CGrindMark[i].Point.y = MarkFind[j].Result[0].Py;
														CGrindMark[i].Score = MarkFind[j].Result[0].Score;
														MaxScore = MarkFind[j].Result[0].Score;

													}
												}
											}
											//20230320 HSKIM

											// GrindMakrã����� ������,������ ������
											if (CGrindMark[i].Point.x != 0.0f && CGrindMark[i].Point.y != 0.0f) {
												// GrindMark���� OutData �ִܰŸ� Count���� Calc 
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
												for (int k = 0; k < C4EdgeCount; k++) {
													if (C4Point[k].x == 0.0f || C4Point[k].y == 0.0f) continue;
													if (CGrindMark[i].Point.x == 0.0f || CGrindMark[i].Point.y == 0.0f) continue;

													GrindMeaDis = Math.GetLength(CGrindMark[i].Point, C4Point[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkOutCnt = k;
													}
												}
												// OutData���� InData �ִܰŸ� Count���� Calc 
												if (GrindMarkOutCnt != 0) {
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
													for (int k = 0; k < C4InEdgeCount; k++) {
														if (C4InPoint[k].x == 0.0f || C4InPoint[k].y == 0.0f) continue;
														if (C4Point[GrindMarkOutCnt].x == 0.0f || C4Point[GrindMarkOutCnt].y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(C4Point[GrindMarkOutCnt], C4InPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkInCnt = k;
														}
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
												if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
													for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
														if (C4Point[k].x == 0.0f || C4Point[k].y == 0.0f) continue;
														if (k >= C4EdgeCount) continue;

														GrindMeaDis = Math.GetLength(CGrindMark[i].Point, C4Point[k]);
														TotalCirDis += GrindMeaDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														CGrindMark[i].GrindMea = TotalCirDis / CircleCount;
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
												if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
													for (int k = -5; k < 5; k++) {
														if (C4Point[GrindMarkOutCnt + k].x == 0.0f && C4Point[GrindMarkOutCnt + k].y == 0.0f) continue;
														if (C4InPoint[GrindMarkInCnt + k].x == 0.0f && C4InPoint[GrindMarkInCnt + k].y == 0.0f) continue;
														if ((GrindMarkOutCnt + k) >= C4EdgeCount) continue;
														if ((GrindMarkInCnt + k) >= C4InEdgeCount) continue;

														GrindWidthDis = Math.GetLength(C4InPoint[GrindMarkInCnt + k], C4Point[GrindMarkOutCnt + k]);
														TotalCirDis += GrindWidthDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														CGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
													}
												}
											}
											else { // GrindMrk ��ã����� �������� ������ 
												// OutData���� �ּҰ� ������ Count���� 
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
												for (int k = 0; k < C4EdgeCount; k++) {
													if (C4Point[k].x == 0.0f || C4Point[k].y == 0.0f) continue;

													CGrindMark[i].RectPoint.x = CGrindMark[i].Rect.left + (CGrindMark[i].Rect.right - CGrindMark[i].Rect.left) / 2;
													CGrindMark[i].RectPoint.y = CGrindMark[i].Rect.top + (CGrindMark[i].Rect.bottom - CGrindMark[i].Rect.top) / 2;

													GrindMeaDis = Math.GetLength(CGrindMark[i].RectPoint, C4Point[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkOutCnt = k;
													}
												}

												// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
												if (GrindMarkOutCnt != 0) {
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
													for (int k = 0; k < C4InEdgeCount; k++) {
														if (C4InPoint[k].x == 0.0f || C4InPoint[k].y == 0.0f) continue;
														if (C4Point[GrindMarkOutCnt].x == 0.0f || C4Point[GrindMarkOutCnt].y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(C4Point[GrindMarkOutCnt], C4InPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkInCnt = k;
														}
													}
												}

												// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
												if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
													CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
													for (int k = -5; k < 5; k++) {
														if (C4Point[GrindMarkOutCnt + k].x == 0.0f && C4Point[GrindMarkOutCnt + k].y == 0.0f) continue;
														if (C4InPoint[GrindMarkInCnt + k].x == 0.0f && C4InPoint[GrindMarkInCnt + k].y == 0.0f) continue;
														if ((GrindMarkOutCnt + k) >= C4EdgeCount) continue;
														if ((GrindMarkInCnt + k) >= C4InEdgeCount) continue;

														GrindWidthDis = Math.GetLength(C4InPoint[GrindMarkInCnt + k], C4Point[GrindMarkOutCnt + k]);
														TotalCirDis += GrindWidthDis;
														CircleCount++;
													}
													if (TotalCirDis != 0.0f && CircleCount != 0) {
														CGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
													}
												}
											}
										}
									}
								}
							}
						}
						else if (IsRectBtm) {
							RBDx = 0.0f;
							RBDy = 0.0f;
							//ȭ�鿡 ���÷��� ���ؼ� 
							FRBRPoint.x = FRBPoint.x;
							FRBRPoint.y = FRBPoint.y;
							FRBBPoint.x = FRBPoint.x;
							FRBBPoint.y = FRBPoint.y;
						}
						else if (IsCircleBtm) { // BTM���� RightBtm Circle �˻�  C2Circle 
							GrindRectWidth  = 0;
							GrindRectHeight = 0;
							EasyImage::Threshold(&RotateImg, &RotateBinaryImg, NotchSideThresHold);
							if (NotchSideThresHold == 0) { SingleLine->GImage = &RotateImg      ;  SingleLine->FThreshold = 5; }
							else                         { SingleLine->GImage = &RotateBinaryImg;  SingleLine->FThreshold = 10;}

							// NOTCH RightBtm
							if (FInspectMode == Insp_Polygon) {
								if (RBCircleHeight != 0 && RBCircleWidth != 0) {
					                P1.x =0, P1.y=0, P2.x=0, P2.y=0, P3.x=0, P3.y=0, P4.x=0, P4.y=0;

									P1.x = (int)(FRBPoint.x - RBCircleWidth); P1.y = (int)(FRBPoint.y - RBCircleHeight); P2.x = (int)(P1.x + RBCircleWidth); P2.y = (int)(P1.y);
									P3.x = (int)(P1.x);                       P3.y = (int)(P1.y + RBCircleHeight);       P4.x = (int)(P2.x);                 P4.y = (int)(P3.y);

									// �˻�ȭ�� Display ���� 
									NotchPolyRect[4].left = P1.x;   NotchPolyRect[4].top = P1.y;
									NotchPolyRect[4].right = P2.x;   NotchPolyRect[4].bottom = P3.y;

									// Median Filter ���� 
									if (P1.x <= 0         || P1.y <= 0         || (P2.x - P1.x) <= 0         || (P3.y - P1.y) <= 0        ) return false;
									if (P1.x >= FrameImgW || P1.y >= FrameImgH || (P2.x - P1.x) >= FrameImgW || (P3.y - P1.y) >= FrameImgH) return false;
									if ((P1.x + (P2.x - P1.x)) >= FrameImgW    || (P1.y + (P3.y - P1.y)) > FrameImgH) return false;

									FilterROI.SetSize(int(P2.x - P1.x), int(P3.y - P1.y));
									FilterROI.Attach(&RotateBinaryImg);
									FilterROI.SetPlacement(P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
									LogUnit.SetLog(L"RightBtm ==>Px :%d, Py :%d, Width:%d, Height:%d ==>Median Filter", P1.x, P1.y, int(P2.x - P1.x), int(P3.y - P1.y));
//									EasyImage::Median(&FilterROI, &FilterROI);
									OnFilter(ftMedian, &FilterROI, &FilterROI);

									Polygon->GImage               = &RotateBinaryImg;
									Polygon->IWidth               = FrameImgW;
									Polygon->IHeight              = FrameImgH;
									Polygon->PoPa.FMinWidth       = (int)((P2.x - P1.x) / 2);
									Polygon->PoPa.FMaxWidth       = (int)((P2.x - P1.x) + 100);
									Polygon->PoPa.FMinHeight      = 2;
									Polygon->PoPa.FMaxHeight      = (int)((P3.y - P1.y) + 100);
									Polygon->PoPa.MinPxCnt        = 100;
									Polygon->PoPa.MaxPxCnt        = 20000;
									Polygon->PoPa.MaxBbCnt        = 10;
									Polygon->PoPa.IsSeparate      = true;
									Polygon->PoPa.IsInToOut       = true;
									Polygon->PoPa.HiThHold        = NotchSideThresHold;
									Polygon->PoPa.LoThHold        = 0;
									Polygon->PoPa.IgInSide        = true;
									Polygon->PoPa.IsHorSort       = true;
									Polygon->PoPa.IsVerSort       = false;
									Polygon->PoPa.IsInToLeft      = true;
									Polygon->PoPa.LeftFirstPoint  = true;
									Polygon->PoPa.RightFirstPoint = false;


									if (P1.x != 0 && P1.y != 0 && P2.x != 0 && P2.y != 0 && P3.x != 0 && P3.y != 0 && P4.x != 0 && P4.y != 0) {
										Polygon->OnExecute(P1, P2, P3, P4);
									}
									// Polygon Algorithm InEdgeData/OutEdgeData Separate 
									if (Polygon->BlobCount == 1) {
										PolygonMarginCnt = (CamId==Cam_Top) ? 5 : 20;
										if (Polygon->Blob[0].InCnt > (PolygonMarginCnt * 2)) {
											C2CirInEdgeCount = 0;
											C2CirInEdgeCount = (Polygon->Blob[0].InCnt - (PolygonMarginCnt * 2));
											if (C2CirInEdgeCount > 0) {
												if (C2CirInPoint != NULL) delete[] C2CirInPoint;
												C2CirInPoint = new TFPoint[C2CirInEdgeCount];

												PixelCnt = 0;
												for (int k = 0; k < Polygon->Blob[0].InCnt; k++) {
													if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].InCnt - PolygonMarginCnt)) continue;
													C2CirInPoint[PixelCnt].x = Polygon->Blob[0].InPoint[k].x;
													C2CirInPoint[PixelCnt].y = Polygon->Blob[0].InPoint[k].y;
													PixelCnt++;
												}
											}
										}

										if (Polygon->Blob[0].OutCnt > (PolygonMarginCnt * 2)) {
											C2CirOutEdgeCount = 0;
											C2CirOutEdgeCount = (Polygon->Blob[0].OutCnt - (PolygonMarginCnt * 2));
											if (C2CirOutEdgeCount > 0) {
												if (C2CirOutPoint != NULL) delete[] C2CirOutPoint;
												C2CirOutPoint = new TFPoint[C2CirOutEdgeCount];

												PixelCnt = 0;
												for (int k = 0; k < Polygon->Blob[0].OutCnt; k++) {
													if (k < PolygonMarginCnt || k >= (Polygon->Blob[0].OutCnt - PolygonMarginCnt)) continue;
													C2CirOutPoint[PixelCnt].x = Polygon->Blob[0].OutPoint[k].x;
													C2CirOutPoint[PixelCnt].y = Polygon->Blob[0].OutPoint[k].y;
													PixelCnt++;
												}
											}
										}
									}


									// InEdgeData/OutEdgeData ���� ������ �� ������ ���
									if (CamId==Cam_Top) { // LTPS ������,������ ��� �κп���
										for (int i = 19; i < 23; i++) {
											if (CGrindMark[i].DvX == 0 || CGrindMark[i].DvY == 0) continue;
											// �̹��� ȸ�� ���� ���� ��ǥ ȸ�� 
											OrgMarkDv.x = (float)CGrindMark[i].DvX; OrgMarkDv.y = (float)CGrindMark[i].DvY;
											deg = dCSideDeg; //Train ����� ���ذ��� 
											NewMarkDv = Math.OnRotate(FLBPoint, OrgMarkDv, deg);
											CGrindMark[i].DvX = (int)NewMarkDv.x; CGrindMark[i].DvY = (int)NewMarkDv.y;

											//ROI�������� 
											CGrindMark[i].Rect.left   = (FLBPoint.x + CGrindMark[i].DvX) - CGrindMark[i].Range;
											CGrindMark[i].Rect.top    = (FLBPoint.y + CGrindMark[i].DvY) - CGrindMark[i].Range;
											CGrindMark[i].Rect.right  = (CGrindMark[i].Rect.left) + (CGrindMark[i].Range*2);
											CGrindMark[i].Rect.bottom = (CGrindMark[i].Rect.top ) + (CGrindMark[i].Range*2);
										}

										for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
											if (k >= MAX_GRINDMARK_ID) continue;
											MarkFind[k].OnLoad(true, FindDir, k);
											MarkFind[k].GImage = &RotateImg;
											MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
											MarkFind[k].SetMinScore(GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
											MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
											MarkFind[k].SetMaxInstances(1);
											MarkFind[k].SetAngleTolerance(20.0f);
											MarkFind[k].SetScaleBias(1.00f);
											MarkFind[k].SetScaleTolerance(0.0f);
											MarkFind[k].Property.Ox = (float)FrameImgW / 2;
											MarkFind[k].Property.Oy = (float)FrameImgH / 2;
										}

										for (int i = 0; i < MAX_GRINDMARK_COUNT; i++) {
											if (CGrindMark[i].Rect.left == 0 || CGrindMark[i].Rect.top == 0 || CGrindMark[i].Rect.right == 0 || CGrindMark[i].Rect.bottom == 0) continue;
											GrindRectWidth  = (CGrindMark[i].Rect.right - CGrindMark[i].Rect.left);
											GrindRectHeight = (CGrindMark[i].Rect.bottom - CGrindMark[i].Rect.top);

											if (CGrindMark[i].Rect.left <= 0 || CGrindMark[i].Rect.top <= 0 || GrindRectWidth <= 0 || GrindRectHeight <= 0) continue;
											if (CGrindMark[i].Rect.left >= FrameImgW || CGrindMark[i].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;
											if (CGrindMark[i].Point.x != 0.0f && CGrindMark[i].Point.y != 0.0f) continue;

											MarkID = CGrindMark[i].MarkID; // Multi Teaching MarkID 
											deg = CGrindMark[i].degree;
											MarkFind[MarkID].SetAngleBias(deg);
											MarkFind[MarkID].IRoi->SetPlacement(CGrindMark[i].Rect.left, CGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
											CGrindMark[i].Score = 0.0f;

											if (MarkFind[MarkID].OnExecute()) { //GrindMark Search OK 
												CGrindMark[i].Point.x = MarkFind[MarkID].Result[0].Px;
												CGrindMark[i].Point.y = MarkFind[MarkID].Result[0].Py;
												CGrindMark[i].Score = MarkFind[MarkID].Result[0].Score;
											}
											else {
												MaxScore = 0.0f;
												for (int j = 0; j < MAX_GRINDMARK_ID; j++) {
													MarkFind[j].IRoi->SetPlacement(CGrindMark[i].Rect.left, CGrindMark[i].Rect.top, GrindRectWidth, GrindRectHeight);
													MarkFind[j].SetAngleBias(deg);

													if (MarkFind[j].OnExecute() && MarkFind[j].Result[0].Score > MaxScore) { //GrindMark Search OK 
														CGrindMark[i].Point.x = MarkFind[j].Result[0].Px;
														CGrindMark[i].Point.y = MarkFind[j].Result[0].Py;
														CGrindMark[i].Score = MarkFind[j].Result[0].Score;
														MaxScore = MarkFind[j].Result[0].Score;
													}
												}
											}
											//20230320 HSKIM

											if (C2CirOutEdgeCount != 0 && C2CirInEdgeCount != 0) { // MarkMode��� ������ �˻縦 ���� ���� �� ������ ����
												////////////////////////////////////////////////////////////////////////////////////////////////////
												if (CGrindMark[i].Point.x != 0.0f && CGrindMark[i].Point.y != 0.0f) {  //GrindMark ã������� 
													// GrindMark���� OutData �ִܰŸ� Count���� Calc 
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
													for (int k = 0; k < C2CirOutEdgeCount; k++) {
														if (C2CirOutPoint[k].x == 0.0f || C2CirOutPoint[k].y == 0.0f) continue;
														if (CGrindMark[i].Point.x == 0.0f || CGrindMark[i].Point.y == 0.0f) continue;

														GrindMeaDis = Math.GetLength(CGrindMark[i].Point, C2CirOutPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkOutCnt = k;
														}
													}
													if (GrindMarkOutCnt != 0) {
														// GrindMark���� InData �ִܰŸ� Count���� Calc 
														GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
														for (int k = 0; k < C2CirInEdgeCount; k++) {
															if (C2CirInPoint[k].x == 0.0f || C2CirInPoint[k].y == 0.0f) continue;
															if (C2CirOutPoint[GrindMarkOutCnt].x == 0.0f || C2CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(C2CirOutPoint[GrindMarkOutCnt], C2CirInPoint[k]);
															if (GrindMeaDis < MinDis) {
																MinDis = GrindMeaDis;
																GrindMarkInCnt = k;
															}
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
													if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
														for (int k = GrindMarkOutCnt - 5; k < GrindMarkOutCnt + 5; k++) {
															if (C2CirOutPoint[k].x == 0.0f || C2CirOutPoint[k].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(CGrindMark[i].Point, C2CirOutPoint[k]);
															TotalCirDis += GrindMeaDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															CGrindMark[i].GrindMea = TotalCirDis / CircleCount;
															CGrindMark[i].GrindMea = CGrindMark[i].GrindMea + 0.5; //SubPixel 
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
													if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
														for (int k = -5; k < 5; k++) {
															if (C2CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && C2CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
															if (C2CirInPoint[GrindMarkInCnt + k].x == 0.0f && C2CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
															GrindWidthDis = Math.GetLength(C2CirInPoint[GrindMarkInCnt + k], C2CirOutPoint[GrindMarkOutCnt + k]);
															TotalCirDis += GrindWidthDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															CGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
															CGrindMark[i].GrindWidth = CGrindMark[i].GrindWidth + 0.5; //SubPixel
														}
													}
												}
												else if (CGrindMark[i].Rect.left != 0 && CGrindMark[i].Rect.top != 0 && CGrindMark[i].Rect.right != 0 && CGrindMark[i].Rect.bottom != 0) {  //GrindMark �� ã�������
													// OutData���� �ּҰ� ������ Count���� 
													GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
													for (int k = 0; k < C2CirOutEdgeCount; k++) {
														if (C2CirOutPoint[k].x == 0.0f || C2CirOutPoint[k].y == 0.0f) continue;

														CGrindMark[i].RectPoint.x = CGrindMark[i].Rect.left + (CGrindMark[i].Rect.right - CGrindMark[i].Rect.left) / 2;
														CGrindMark[i].RectPoint.y = CGrindMark[i].Rect.top + (CGrindMark[i].Rect.bottom - CGrindMark[i].Rect.top) / 2;

														GrindMeaDis = Math.GetLength(CGrindMark[i].RectPoint, C2CirOutPoint[k]);
														if (GrindMeaDis < MinDis) {
															MinDis = GrindMeaDis;
															GrindMarkOutCnt = k;
														}
													}

													// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
													if (GrindMarkOutCnt != 0) {
														GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
														for (int k = 0; k < C2CirInEdgeCount; k++) {
															if (C2CirInPoint[k].x == 0.0f || C2CirInPoint[k].y == 0.0f) continue;
															if (C2CirOutPoint[GrindMarkOutCnt].x == 0.0f || C2CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

															GrindMeaDis = Math.GetLength(C2CirOutPoint[GrindMarkOutCnt], C2CirInPoint[k]);
															if (GrindMeaDis < MinDis) {
																MinDis = GrindMeaDis;
																GrindMarkInCnt = k;
															}
														}
													}

													// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
													if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
														CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
														for (int k = -5; k < 5; k++) {
															if (C2CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && C2CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
															if (C2CirInPoint[GrindMarkInCnt + k].x == 0.0f && C2CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
															GrindWidthDis = Math.GetLength(C2CirInPoint[GrindMarkInCnt + k], C2CirOutPoint[GrindMarkOutCnt + k]);
															TotalCirDis += GrindWidthDis;
															CircleCount++;
														}
														if (TotalCirDis != 0.0f && CircleCount != 0) {
															CGrindMark[i].GrindWidth = TotalCirDis / CircleCount;
															CGrindMark[i].GrindWidth = CGrindMark[i].GrindWidth + 0.5; //SubPixel
														}
													}
												}
											}
										}
									}
									else { //Polygon EnCap ���� OutPoint �߰��� ���� ���� �Ÿ� ��հ��� ����Ѵ�(RightTop)
										if (C2CirInEdgeCount != 0 && C2CirOutEdgeCount != 0) {
											GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
											GrindMarkOutCnt = (int)(C2CirOutEdgeCount / 2);
											// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
											if (GrindMarkOutCnt != 0) {
												GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
												for (int k = 0; k < C2CirInEdgeCount; k++) {
													if (C2CirInPoint[k].x == 0.0f || C2CirInPoint[k].y == 0.0f) continue;
													if (C2CirOutPoint[GrindMarkOutCnt].x == 0.0f || C2CirOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

													GrindMeaDis = Math.GetLength(C2CirOutPoint[GrindMarkOutCnt], C2CirInPoint[k]);
													if (GrindMeaDis < MinDis) {
														MinDis = GrindMeaDis;
														GrindMarkInCnt = k;
													}
												}
											}

											// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
											if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
												CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
												for (int k = -5; k < 5; k++) {
													if (C2CirOutPoint[GrindMarkOutCnt + k].x == 0.0f && C2CirOutPoint[GrindMarkOutCnt + k].y == 0.0f) continue;
													if (C2CirInPoint[GrindMarkInCnt + k].x == 0.0f && C2CirInPoint[GrindMarkInCnt + k].y == 0.0f) continue;
													GrindWidthDis = Math.GetLength(C2CirInPoint[GrindMarkInCnt + k], C2CirOutPoint[GrindMarkOutCnt + k]);
													TotalCirDis += GrindWidthDis;
													CircleCount++;
												}
												// 												 if(TotalCirDis!=0.0f && CircleCount!=0){
												// 													 A2CirGrindWd[2] = TotalCirDis/CircleCount;
												// 													 A2CirGrindWd[2] = A2CirGrindWd[2]+0.5; //SubPixel
												// 												 }
											}
										}
									}

									// Polygon �ڸ� �˻� ������(RightTop)
									if (CamId==Cam_Top && BrokenThresHold != 0) {
										RotateBrokenImg.SetSize(RotateImg.GetWidth(), RotateImg.GetHeight());
										EasyImage::Oper(EArithmeticLogicOperation_Copy,&RotateImg,&RotateBrokenImg);
										EasyImage::Threshold(&RotateImg,&RotateBrokenImg, BrokenThresHold);
										Edge->GImage = &RotateBrokenImg;
										Edge->FImgHeight = ImageHeight;
										Edge->SetEdgeType(0);

										int DvCnt = 0, DataCount = 0;
										double deg = 0.0f;
										TFPoint Target(0.0f, 0.0f);

										if (GrindMarkInCnt > GrindMarkOutCnt) {
											DvCnt = GrindMarkInCnt - GrindMarkOutCnt;
											for (int k = 0; k < C2CirOutEdgeCount; k++) {
												deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
												if (((k + DvCnt) < C2CirInEdgeCount) && C2CirInPoint[k + DvCnt].x > 0 && C2CirInPoint[k + DvCnt].y > 0) {
													deg = Math.GetDegree(C2CirOutPoint[k], C2CirInPoint[k + DvCnt]);

													Target = Math.OnFindPerpendicularPointXY(C2CirOutPoint[k], -dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
													Edge->OnExecute(C2CirOutPoint[k], Target); // �����ܰ����ο��� Target���� White Point Search 
													if (Edge->GetResultEdgePointCount() > 0) {
														C2CircleData[DataCount].Dis = 0.0f;
														C2CircleData[DataCount].Dv = 0.0f;
														C2CircleData[DataCount].P = Edge->GetResultEdgePoint(0);
														C2CircleData[DataCount].P.y = C2CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
														DataCount++;
													}
												}
											}
											C2CirInCrackCount = DataCount;
										}
										else if (GrindMarkInCnt < GrindMarkOutCnt) {
											DvCnt = GrindMarkOutCnt - GrindMarkInCnt;
											for (int k = DvCnt; k < C2CirOutEdgeCount; k++) {
												deg = 0.0f, Target.x = 0.0f, Target.y = 0.0f;
												if (((k - DvCnt) < C2CirInEdgeCount) && C2CirInPoint[k - DvCnt].x > 0 && C2CirInPoint[k - DvCnt].y > 0 && k - DvCnt >= 0) {
													deg = Math.GetDegree(C2CirOutPoint[k], C2CirInPoint[k - DvCnt]);

													Target = Math.OnFindPerpendicularPointXY(C2CirOutPoint[k], -dEdgeToTrapLineDis, dEdgeToTrapLineDis, deg);
													Edge->OnExecute(C2CirOutPoint[k], Target); // �����ܰ����ο��� Target���� White Point Search 
													if (Edge->GetResultEdgePointCount() > 0) {
														C2CircleData[DataCount].Dis = 0.0f;
														C2CircleData[DataCount].Dv = 0.0f;
														C2CircleData[DataCount].P = Edge->GetResultEdgePoint(0);
														C2CircleData[DataCount].P.y = C2CircleData[DataCount].P.y - ImageHeight; // Frame���� ������ǥ�� ������  
														DataCount++;
													}
												}
											}
											C2CirInCrackCount = DataCount;
										}
									}
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
	case 11:
		DefectImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &DefectImg);
		//EasyImage::Threshold(GImage,&DefectImg, DThresHold);

		if (FProcessMode == pmAutoBevelIn) {
			//BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
			//EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
			//EasyImage::Threshold(GImage,&BinaryImg, ThredHold);
			Edge->GImage = &DefectImg;
			//Edge->GImage = GImage;

			if (!OnAutoBevelIn()) return false;
			Index = 0;
			return true;
		}
		Index++;

	case 12:
		Index = 0;
		break;
	}
	return true;
}
void CFM15xLineDefect::OnExecuteGrind(TFPoint StaPos, TFPoint EndPos, int nSide)
{
	int FrameImgW   = GImage->GetWidth();
	int FrameImgH   = GImage->GetHeight();
	int MaxGrindID  = 0;
	int MarkID      = 0;
	double MarkDeg  = 0.0f;
	double GlassDeg = 0.0f;
	double Orgdeg   = 0.0f;
	double Newdeg   = 0.0f;
	double MaxScore = 0.0f;
	TFPoint Target(0.0f, 0.0f);
	double GrindMeaDis = 0.0f, MinDis = 10000.0f, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
	int GrindMarkOutCnt = 0, GrindMarkInCnt = 0, PreGrindMarkOutCnt = 0, PreGrindMarkInCnt = 0, AllOutCnt = 0, AllInCnt = 0, CircleCount = 0;

	// DvX,DvY �������� �̹��� ȸ�� �������� �����ϱ� ���ؼ� �ʿ��� ���� 
	TFPoint OrgMarkDv; TFPoint NewMarkDv;

	TGrindMark* lpGrindMark; 
	TFPoint   * AllOutPoint;
	TFPoint   * AllInPoint ;

	Orgdeg = 0.0f;
	if     (nSide==BSide){Orgdeg = dBSideDeg;lpGrindMark=BGrindMark; AllOutPoint=AllRightOutData; AllInPoint=AllRightInData; AllOutCnt=AllRightOutEdgeCount; AllInCnt=AllRightInEdgeCount;}
	else if(nSide==DSide){Orgdeg = dDSideDeg;lpGrindMark=DGrindMark; AllOutPoint=AllLeftOutData ; AllInPoint=AllLeftInData ; AllOutCnt=AllLeftOutEdgeCount ; AllInCnt=AllLeftInEdgeCount ;}

	if (StaPos.x!=0.0f && StaPos.y!=0.0f && EndPos.x!=0.0f && EndPos.y!=0.0f) GlassDeg = (Math.GetDegree(StaPos.x, StaPos.y, EndPos.x, EndPos.y));
	Newdeg = GlassDeg - Orgdeg; //Glass���� - Teaching����

	// GrindMark DvX,DvY�������� ���� ���� 
	for (int k = 0; k < MAX_GRINDMARK_COUNT; k++) {
		if (lpGrindMark[k].DvX == 0 || lpGrindMark[k].DvY == 0) continue;

		OrgMarkDv.x = (float)lpGrindMark[k].DvX; OrgMarkDv.y = (float)lpGrindMark[k].DvY;
		NewMarkDv = Math.OnRotate(StaPos, OrgMarkDv, Newdeg); //Teaching ������ ���� Glass���� �������� ȸ�� �� DvX,DvY���� 
		lpGrindMark[k].DvX = (int)NewMarkDv.x; lpGrindMark[k].DvY = (int)NewMarkDv.y;

		Target.x = StaPos.x; Target.y = 0;
		if     (nSide==BSide) Target.x = Target.x - lpGrindMark[k].DvX;
		else if(nSide==DSide) Target.x = Target.x + lpGrindMark[k].DvX;
		Target.y = Target.y + lpGrindMark[k].DvY;

		lpGrindMark[k].Rect.left   = Target.x - lpGrindMark[k].Range;
		lpGrindMark[k].Rect.top    = Target.y - lpGrindMark[k].Range;
		lpGrindMark[k].Rect.right  = lpGrindMark[k].Rect.left + (lpGrindMark[k].Range * 2);
		lpGrindMark[k].Rect.bottom = lpGrindMark[k].Rect.top  + (lpGrindMark[k].Range * 2);
	}

	// ������ Multi_Teaching ������ŭ ������ �ε� �� �⺻�� ������ 
	//20230320 HSKIM
	for (int k = 0; k < MAX_GRINDMARK_ID; k++) {
		if (k >= MAX_GRINDMARK_ID) continue;

		MarkFind[k].OnLoad(true, FindDir, k);
		MarkFind[k].GImage = GImage;
		MarkFind[k].SetContrastMode(EFindContrastMode_Normal);
		MarkFind[k].SetMinScore (GrindMarkMinScore); // PatternrFind �˻� �ּ� Score
		MarkFind[k].SetZeroScore(GrindMarkMinScore); // �˻� �� �ּ� Score 
		MarkFind[k].SetMaxInstances(1);
		MarkFind[k].SetAngleTolerance(20.0f);
		MarkFind[k].SetScaleBias(1.00f);
		MarkFind[k].SetScaleTolerance(0.0f);
		MarkFind[k].SetLightBalance(0.0f);
		MarkFind[k].Property.Ox = (float)FrameImgW / 2;
		MarkFind[k].Property.Oy = (float)FrameImgH / 2;
	}

	int GrindRectWidth = 0, GrindRectHeight = 0;
	//    GImage->Save("D:\\ROTATE_IMG\\OLDTCG_AllBuffer.bmp")        ;
	if (AllOutCnt != 0 && AllInCnt != 0) {
		for (int k = 0; k < MAX_GRINDMARK_COUNT; k++) {
			if (lpGrindMark[k].Rect.left == 0 || lpGrindMark[k].Rect.top == 0 || lpGrindMark[k].Rect.right == 0 || lpGrindMark[k].Rect.bottom == 0) continue;
			GrindRectWidth  = (lpGrindMark[k].Rect.right  - lpGrindMark[k].Rect.left);
			GrindRectHeight = (lpGrindMark[k].Rect.bottom - lpGrindMark[k].Rect.top);

			if (lpGrindMark[k].Rect.left <= 0         || lpGrindMark[k].Rect.top <= 0         || GrindRectWidth <= 0         || GrindRectHeight <= 0        ) continue;
			if (lpGrindMark[k].Rect.left >= FrameImgW || lpGrindMark[k].Rect.top >= FrameImgH || GrindRectWidth >= FrameImgW || GrindRectHeight >= FrameImgH) continue;
			if (lpGrindMark[k].Point.x != 0.0f && lpGrindMark[k].Point.y != 0.0f) continue;

			if     (CamId==Cam_Btm){ //BTM
				//Grind Mark None ==>Grind Rect �߽�Pos ���� ���� ������ ���� ���� 
				lpGrindMark[k].RectPoint.x = lpGrindMark[k].Rect.left + (lpGrindMark[k].Rect.right  - lpGrindMark[k].Rect.left) / 2;
				lpGrindMark[k].RectPoint.y = lpGrindMark[k].Rect.top  + (lpGrindMark[k].Rect.bottom - lpGrindMark[k].Rect.top ) / 2 + StaPos.y;
				lpGrindMark[k].Score       = 100.0f;

				//GrindMark ROI  ���� CenterPos ���� ���� �����͸� ���� 
				if(lpGrindMark[k].RectPoint.x!=0.0f && lpGrindMark[k].RectPoint.y!=0.0f){
					// OutData���� �ּҰ� ������ Count���� 
					GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
					for (int i = PreGrindMarkOutCnt; i < AllOutCnt; i++) {
						if (AllOutPoint[i].x == 0.0f || AllOutPoint[i].y == 0.0f) continue;

						GrindMeaDis = Math.GetLength(lpGrindMark[k].RectPoint, AllOutPoint[i]);
						if (GrindMeaDis < MinDis) {
							MinDis = GrindMeaDis;
							GrindMarkOutCnt = i;
							PreGrindMarkOutCnt = GrindMarkOutCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
						}
					}

					// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
					if (GrindMarkOutCnt != 0) {
						GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
						for (int i = PreGrindMarkInCnt; i < AllInCnt; i++) {
							if (AllInPoint[i].x == 0.0f || AllInPoint[i].y == 0.0f) continue;
							if (AllOutPoint[GrindMarkOutCnt].x == 0.0f || AllOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

							GrindMeaDis = Math.GetLength(AllOutPoint[GrindMarkOutCnt], AllInPoint[i]);
							if (GrindMeaDis < MinDis) {
								MinDis = GrindMeaDis;
								GrindMarkInCnt = i;
								PreGrindMarkInCnt = GrindMarkInCnt;
							}
						}
					}

					// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
					if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
						CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
						for (int i = -5; i < 5; i++) {
							if (AllOutPoint[GrindMarkOutCnt+ i].x == 0.0f && AllOutPoint[GrindMarkOutCnt+ i].y == 0.0f) continue;
							if (AllInPoint [GrindMarkInCnt + i].x == 0.0f && AllInPoint [GrindMarkInCnt + i].y == 0.0f) continue;
							GrindWidthDis = Math.GetLength(AllInPoint[GrindMarkInCnt + i], AllOutPoint[GrindMarkOutCnt + i]);
							TotalCirDis += GrindWidthDis;
							CircleCount++;
						}
						if (TotalCirDis != 0.0f && CircleCount != 0) {
							lpGrindMark[k].GrindWidth = TotalCirDis / CircleCount;
						}
					}
					//StaPos ���� ��ǥ��� �ٽ��� 
					lpGrindMark[k].RectPoint.y = lpGrindMark[k].RectPoint.y - StaPos.y;
				}
			}
			else if(CamId==Cam_Top){ //TOP
				MaxScore = 0.0f;
				for (int i = 0; i < MAX_GRINDMARK_ID; i++) {
					MarkFind[i].IRoi->SetPlacement(lpGrindMark[k].Rect.left, lpGrindMark[k].Rect.top, GrindRectWidth, GrindRectHeight);
					MarkFind[i].SetAngleBias      (lpGrindMark[k].degree);
					MarkFind[i].OnExecute();

					if (MarkFind[i].Result[0].Score > MaxScore) { //GrindMark Search OK 
						MaxScore = MarkFind[i].Result[0].Score;
						lpGrindMark[k].Score   = MarkFind[i].Result[0].Score;
						lpGrindMark[k].Point.x = MarkFind[i].Result[0].Px;
						lpGrindMark[k].Point.y = MarkFind[i].Result[0].Py;
					}
					else                                        { //GrindMark Search NG
						lpGrindMark[k].RectPoint.x = lpGrindMark[k].Rect.left + (lpGrindMark[k].Rect.right  - lpGrindMark[k].Rect.left) / 2;
						lpGrindMark[k].RectPoint.y = lpGrindMark[k].Rect.top  + (lpGrindMark[k].Rect.bottom - lpGrindMark[k].Rect.top ) / 2 + StaPos.y;
					}
				}

				// ������,������ ��� 
				if (lpGrindMark[k].Point.x != 0.0f && lpGrindMark[k].Point.y != 0.0f) { // GrindMark ã�������
					//��ü Buffer���� ��ǥ��� �ٽ��� 
					lpGrindMark[k].Point.y = lpGrindMark[k].Point.y + StaPos.y;
					// GrindMark���� OutData �ִܰŸ� Count���� Calc 
					GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
					for (int i = PreGrindMarkOutCnt; i < AllOutCnt; i++) {
						if (AllOutPoint[i].x == 0.0f || AllOutPoint[i].y == 0.0f) continue;
						GrindMeaDis = Math.GetLength(lpGrindMark[k].Point, AllOutPoint[i]);
						if (GrindMeaDis < MinDis) {
							MinDis = GrindMeaDis;
							GrindMarkOutCnt = i;
							PreGrindMarkOutCnt = GrindMarkOutCnt; //��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
						}
					}

					// OutData���� InData �ִܰŸ� Count���� Calc 
					if (GrindMarkOutCnt != 0) {
						GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
						for (int i = PreGrindMarkInCnt; i < AllInCnt; i++) {
							if (AllInPoint[i].x == 0.0f || AllInPoint[i].y == 0.0f) continue;
							if (AllOutPoint[GrindMarkOutCnt].x == 0.0f || AllOutPoint[GrindMarkOutCnt].y == 0.0f) continue;
							GrindMeaDis = Math.GetLength(AllOutPoint[GrindMarkOutCnt], AllInPoint[i]);
							if (GrindMeaDis < MinDis) {
								MinDis = GrindMeaDis;
								GrindMarkInCnt = i;
								PreGrindMarkInCnt = GrindMarkInCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
							}
						}
					}

					// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData����)
					if (GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) {
						CircleCount = 0, TotalCirDis = 0.0f, GrindMeaDis = 0.0f;
						for (int i = GrindMarkOutCnt - 5; i < GrindMarkOutCnt + 5; i++) {
							if (AllOutPoint[i].x == 0.0f || AllOutPoint[i].y == 0.0f) continue;

							GrindMeaDis = Math.GetLength(lpGrindMark[k].Point, AllOutPoint[i]);
							TotalCirDis += GrindMeaDis;
							CircleCount++;
						}
						if (TotalCirDis != 0.0f && CircleCount != 0) {
							lpGrindMark[k].GrindMea = TotalCirDis / CircleCount;
						}
					}

					// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
//					if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
//						CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
//						for (int i = -5; i < 5; i++) {
//							if (AllOutPoint[GrindMarkOutCnt + i].x == 0.0f && AllOutPoint[GrindMarkOutCnt + i].y == 0.0f) continue;
//							if (AllInPoint[GrindMarkInCnt + i].x == 0.0f && AllInPoint[GrindMarkInCnt + i].y == 0.0f) continue;
//							GrindWidthDis = Math.GetLength(AllInPoint[GrindMarkInCnt + i], AllOutPoint[GrindMarkOutCnt + i]);
//							TotalCirDis += GrindWidthDis;
//							CircleCount++;
//						}
//						if (TotalCirDis != 0.0f && CircleCount != 0) {
//							lpGrindMark[k].GrindWidth = TotalCirDis / CircleCount;
//						}
//					}
					//StaPos ���� ��ǥ��� �ٽ��� 
					lpGrindMark[k].Point.y = lpGrindMark[k].Point.y - StaPos.y;
				}
				else { // GrindMark ��ã�����
					// OutData���� �ּҰ� ������ Count���� 
					/*
					GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkOutCnt = 0;
					for (int i = PreGrindMarkOutCnt; i < AllOutCnt; i++) {
						if (AllOutPoint[i].x == 0.0f || AllOutPoint[i].y == 0.0f) continue;

						lpGrindMark[k].RectPoint.x = lpGrindMark[k].Rect.left + (lpGrindMark[k].Rect.right - lpGrindMark[k].Rect.left) / 2;
						lpGrindMark[k].RectPoint.y = lpGrindMark[k].Rect.top + (lpGrindMark[k].Rect.bottom - lpGrindMark[k].Rect.top ) / 2;

						GrindMeaDis = Math.GetLength(lpGrindMark[k].RectPoint, AllOutPoint[i]);
						if (GrindMeaDis < MinDis) {
							MinDis = GrindMeaDis;
							GrindMarkOutCnt = i;
							PreGrindMarkOutCnt = GrindMarkOutCnt; // ��EdgePoint�� �ּ�ȭ �ϱ� ���ؼ� ���� ������ �ʿ��� 
						}
					}

					// �ִܰŸ�OUtCount���� InData �ִܰŸ� Count���� Calc 
					if (GrindMarkOutCnt != 0) {
						GrindMeaDis = 0.0f, MinDis = 10000.0f; GrindMarkInCnt = 0;
						for (int i = PreGrindMarkInCnt; i < AllInCnt; i++) {
							if (AllInPoint[i].x == 0.0f || AllInPoint[i].y == 0.0f) continue;
							if (AllOutPoint[GrindMarkOutCnt].x == 0.0f || AllOutPoint[GrindMarkOutCnt].y == 0.0f) continue;

							GrindMeaDis = Math.GetLength(AllOutPoint[GrindMarkOutCnt], AllInPoint[i]);
							if (GrindMeaDis < MinDis) {
								MinDis = GrindMeaDis;
								GrindMarkInCnt = i;
								PreGrindMarkInCnt = GrindMarkInCnt;
							}
						}
					}

					// GrindMark���� ���� ����Ʈ �ֺ��� Avg�����Ѵ� ==>������ ����(OutData,InData����)
					if ((GrindMarkOutCnt != 0 && GrindMarkOutCnt > 5) && (GrindMarkInCnt != 0 && GrindMarkInCnt > 5)) {
						CircleCount = 0, TotalCirDis = 0.0f, GrindWidthDis = 0.0f;
						for (int i = -5; i < 5; i++) {
							if (AllOutPoint[GrindMarkOutCnt + i].x == 0.0f && AllOutPoint[GrindMarkOutCnt + i].y == 0.0f) continue;
							if (AllInPoint[GrindMarkInCnt + i].x == 0.0f && AllInPoint[GrindMarkInCnt + i].y == 0.0f) continue;
							GrindWidthDis = Math.GetLength(AllInPoint[GrindMarkInCnt + i], AllOutPoint[GrindMarkOutCnt + i]);
							TotalCirDis += GrindWidthDis;
							CircleCount++;
						}
						if (TotalCirDis != 0.0f && CircleCount != 0) {
							lpGrindMark[k].GrindWidth = TotalCirDis / CircleCount;
						}
					}
					*/
					//StaPos ���� ��ǥ��� �ٽ��� 
					lpGrindMark[k].RectPoint.y = lpGrindMark[k].RectPoint.y - StaPos.y;
				}
			}
		}
	}
}
void CFM15xLineDefect::OnLoad(bool IsLoad, CString Folder)
{
	CIFile Ini;

	if (Folder == "") return;
	CString Path = Folder + "\\PLine.ini";
	if (Path == "") return;

	//Ini.Load(IsLoad, Path, "Parameter", "FMaxLines"  , &FMaxLines);
}
//------------------------------------------------------------------------------------
// ����� Get�Լ� 
//------------------------------------------------------------------------------------------
CFind* CFM15xLineDefect::GetFindPointer()
{
	return Find1;
}
//------------------------------------------------------------------------------------
// ����� Set�Լ� 
//------------------------------------------------------------------------------------------
void CFM15xLineDefect::SetProcessMode(TProcessMode ProcessMode)
{
	FProcessMode = ProcessMode;
}
//------------------------------------------------------------------------------------------
void CFM15xLineDefect::SetInspectMode(TInspectMode InspectMode)
{
	FInspectMode = InspectMode;
}
//------------------------------------------------------------------------------------------
void CFM15xLineDefect::SetShape(TShape Shape)
{
	FShape = Shape;
}
//-------------------------------------------------------------------------------------------
void CFM15xLineDefect::SetCrossLeftPoint(int x, int y)
{
	FP0.x = (float)x;
	FP0.y = (float)y;
	AlignMark1.x = FP0.x;
	AlignMark1.y = FP0.y;
}
//------------------------------------------------------------------------------------------
void CFM15xLineDefect::SetCrossRightPoint(int x, int y)
{
	FP1.x = (float)x;
	FP1.y = (float)y;
	AlignMark2.x = FP1.x;
	AlignMark2.y = FP1.y;
}
//------------------------------------------------------------------------------------------
void CFM15xLineDefect::SetSpecWidth(float MinWidth, float MaxWidth)
{
	FMinWidth = MinWidth;
	FMaxWidth = MaxWidth;
}
//------------------------------------------------------------------------------------------
void CFM15xLineDefect::SetSpecHeight(float MinHeight, float MaxHeight)
{
	FMinHeight = MinHeight;
	FMaxHeight = MaxHeight;
}
//------------------------------------------------------------------------------------------
