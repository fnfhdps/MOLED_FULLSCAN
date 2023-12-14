// LxEdge.cpp: implementation of the CEdge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LxEdge.h"
#include "LxHeader.h"
#include <vector>
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEdge::CEdge()
{
    FEdgePoint = NULL;
    FEdgePointCount = 0;

    FEdgeType = etBwOrWb;
    FThreshold = 20;
    FMinAmplitude =  10;
    FMinArea =  0;
    FThickness = 1;
    FSmoothing = 1;
    FMaxEdges = 1;
} 
 
CEdge::~CEdge() 
{
}
// Functions - Set
// ---------------------------------------------------------------------
void CEdge::SetMode        (int n) { FMode = n; }
void CEdge::SetOrientation (int n) { FOrientation = n; }
void CEdge::SetStartPoint  (TFPoint SP) { FSP = SP; }
void CEdge::SetEndPoint    (TFPoint EP) { FEP = EP; }
void CEdge::SetMaxEdges    (int MaxEdges    ) { FMaxEdges     = MaxEdges     ; }
void CEdge::SetEdgeType    (int EdgeType    ) { FEdgeType     = EdgeType     ; }
void CEdge::SetMinAmplitude(int MinAmplitude) { FMinAmplitude = MinAmplitude ; }
void CEdge::SetMinArea     (int MinArea     ) { FMinArea      = MinArea      ; }
void CEdge::SetThreshold   (int Threshold   ) { FThreshold    = Threshold    ; }
void CEdge::SetThickness   (int Thickness   ) { FThickness    = Thickness    ; }
void CEdge::SetSmoothing   (int Smoothing   ) { FSmoothing    = Smoothing    ; }
void CEdge::SetImgHeight   (int ImgHeight   ) { FImgHeight    = ImgHeight    ; }

// Functions - Get
// ---------------------------------------------------------------------
int CEdge::GetMode        () { return FMode; }
int CEdge::GetOrientation () { return FOrientation; }
TFPoint CEdge::GetStartPoint  () { return FSP; }
TFPoint CEdge::GetEndPoint    () { return FEP; }
int CEdge::GetMaxEdges    () { return FMaxEdges    ; }
int CEdge::GetEdgeType    () { return FEdgeType    ; }
int CEdge::GetMinAmplitude() { return FMinAmplitude; }
int CEdge::GetMinArea     () { return FMinArea     ; }
int CEdge::GetThreshold   () { return FThreshold   ; }
int CEdge::GetThickness   () { return FThickness   ; }
int CEdge::GetSmoothing   () { return FSmoothing   ; }


void CEdge::OnExecute()
{
// 	 최신소스는 업데이트 했으나, PNP 외관검사기 기준으로는 
// 	 변경점이 많아서 일단 소스는 주석처리함 
// 	 20160923 GOO 
// 	if      (FMode==0) return OnExeOrientation();
// 	else if (FMode==1) return OnExeDistance();
// 	else if (FMode==2) return OnExecute(FSP, FEP);
// 	else {
// 		FMode = 0;
// 		return false;
// 	}
// 
// 	return false;

    OnExecute(FSP, FEP);
}

int CEdge::OnExecute(TFPoint SP, TFPoint EP)
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CEdge GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CEdge GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CEdge GImage Height Size"    )); return false; }

	EPointGauge PointGauge; // EPointGauge instance

    float Length = Math.GetLength(SP, EP);

    if (FEdgePoint != NULL) delete[] FEdgePoint;
    FEdgePoint = new TFPoint[(int)Length];

    //EWorldShape WorldShape; // EWorldShape instance
    //WorldShape.SetSensorSize(Pw, Ph);
    //WorldShape.Process(GImage, true);
    //PointGauge.Attach(&WorldShape);

    PointGauge.SetDragable(true);
    PointGauge.SetResizable(true);
    PointGauge.SetRotatable(true);

    PointGauge.SetMinAmplitude(FMinAmplitude);
    PointGauge.SetMinArea     (FMinArea     );
    PointGauge.SetThreshold   (FThreshold   );
    PointGauge.SetThickness   (FThickness   );
    PointGauge.SetSmoothing   (FSmoothing   );
    PointGauge.SetFound       (FMaxEdges    );

    PointGauge.SetTransitionType(ETransitionType_Wb);
    if      (FEdgeType==0) PointGauge.SetTransitionType(ETransitionType_Bw    );
    else if (FEdgeType==1) PointGauge.SetTransitionType(ETransitionType_Wb    );
    else if (FEdgeType==2) PointGauge.SetTransitionType(ETransitionType_BwOrWb);
    else if (FEdgeType==3) PointGauge.SetTransitionType(ETransitionType_Bwb);
    else if (FEdgeType==4) PointGauge.SetTransitionType(ETransitionType_Wbw);

    FEdgePointCount = 0;

    float Angle = Math.GetAngle(SP.x, SP.y, EP.x, EP.y);
    PointGauge.SetTolerance(Length/2);
    PointGauge.SetToleranceAngle(Angle); //180도를 입력하면 Backward 방향에서 Edge를 찾음
    PointGauge.SetCenterXY(SP.x+(EP.x-SP.x)/2, SP.y+(EP.y-SP.y)/2);
    PointGauge.Measure(GImage);

    FEdgePointCount = PointGauge.GetNumMeasuredPoints();

	// GOO 
	if(FEdgePointCount>200) FEdgePointCount = 200; 

    for (int k=0; k<FEdgePointCount; k++) {
        if (FEdgePointCount>=Length) continue;
        FEdgePoint[k].x = PointGauge.GetMeasuredPoint(k).GetX();
        FEdgePoint[k].y = PointGauge.GetMeasuredPoint(k).GetY()+FImgHeight;
	}

    return FEdgePointCount;
}
int CEdge::OnExeOrientation()
{
	if (IRoi->GetOrgX()<0) return false;
	if (IRoi->GetOrgY()<0) return false;
	if (IRoi->GetWidth()<1) return false;
	if (IRoi->GetHeight()<1) return false;

	int Px = IRoi->GetOrgX();
	int Py = IRoi->GetOrgY();
	int Pw = IRoi->GetWidth();
	int Ph = IRoi->GetHeight();
	int Hw = IRoi->GetWidth()/2;
	int Hh = IRoi->GetHeight()/2;

	WPoint.x = (float)(Px   ); WPoint.y = (float)(Py+Hh);
	EPoint.x = (float)(Px+Pw); EPoint.y = (float)(Py+Hh);
	NPoint.x = (float)(Px+Hw); NPoint.y = (float)(Py   );
	SPoint.x = (float)(Px+Hw); SPoint.y = (float)(Py+Ph);

	if (FOrientation==eoWest ) { return OnExecute(WPoint, EPoint); }
	if (FOrientation==eoNorth) { return OnExecute(NPoint, SPoint); }
	if (FOrientation==eoEast ) { return OnExecute(EPoint, WPoint); }
	if (FOrientation==eoSouth) { return OnExecute(SPoint, NPoint); }

	return false;
}

int CEdge::OnExeDistance()
{
	if (IRoi->GetOrgX()<0) return false;
	if (IRoi->GetOrgY()<0) return false;
	if (IRoi->GetWidth()<1) return false;
	if (IRoi->GetHeight()<1) return false;

	int Px = IRoi->GetOrgX();
	int Py = IRoi->GetOrgY();
	int Pw = IRoi->GetWidth();
	int Ph = IRoi->GetHeight();
	int Hw = IRoi->GetWidth() /2;
	int Hh = IRoi->GetHeight()/2;

	WPoint.x = (float)(Px   ); WPoint.y = (float)(Py+Hh);
	EPoint.x = (float)(Px+Pw); EPoint.y = (float)(Py+Hh);
	NPoint.x = (float)(Px+Hw); NPoint.y = (float)(Py   );
	SPoint.x = (float)(Px+Hw); SPoint.y = (float)(Py+Ph);

	if (FOrientation==eoWest  || FOrientation==eoEast ) {
		if (!OnExecute(WPoint, EPoint)) return false;
		FSPoint = GetResultEdgePoint(0);
		if (!OnExecute(EPoint, WPoint)) return false;
		FEPoint = GetResultEdgePoint(0);
	}
	if (FOrientation==eoNorth || FOrientation==eoSouth) {
		if (!OnExecute(NPoint, SPoint)) return false;
		FSPoint = GetResultEdgePoint(0);
		if (!OnExecute(SPoint, NPoint)) return false;
		FEPoint = GetResultEdgePoint(0);
	}
	FDistance.x = fabs(FSPoint.x-FEPoint.x);
	FDistance.y = fabs(FSPoint.y-FEPoint.y);

	return true;
}

void CEdge::OnLoad(bool IsLoad, CString Folder)
{
	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

    //if (Folder.Trim()=="") return;
    CString Path = Folder + "\\Edge.ini";
    if (Path=="") return;

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

	Ini.Load(IsLoad, Path, "Parameter", "FMode        "  , &FMode        );
	Ini.Load(IsLoad, Path, "Parameter", "FOrientation "  , &FOrientation );
    Ini.Load(IsLoad, Path, "Parameter", "FMaxEdges    "  , &FMaxEdges    );
    Ini.Load(IsLoad, Path, "Parameter", "FEdgeType    "  , &FEdgeType    );
    Ini.Load(IsLoad, Path, "Parameter", "FMinAmplitude"  , &FMinAmplitude);
    Ini.Load(IsLoad, Path, "Parameter", "FMinArea     "  , &FMinArea     );
    Ini.Load(IsLoad, Path, "Parameter", "FThreshold   "  , &FThreshold   );
    Ini.Load(IsLoad, Path, "Parameter", "FThickness   "  , &FThickness   );
    Ini.Load(IsLoad, Path, "Parameter", "FSmoothing   "  , &FSmoothing   );

	//Inspection Roi
	int Roi_x = IRoi->GetOrgX();
	int Roi_y = IRoi->GetOrgY();
	int Roi_w = IRoi->GetWidth();
	int Roi_h = IRoi->GetHeight();

	Ini.Load(IsLoad, Path, "Property", "IRoi_x", &Roi_x);
	Ini.Load(IsLoad, Path, "Property", "IRoi_y", &Roi_y);
	Ini.Load(IsLoad, Path, "Property", "IRoi_w", &Roi_w);
	Ini.Load(IsLoad, Path, "Property", "IRoi_h", &Roi_h);

	IRoi->SetOrgX  (Roi_x);
	IRoi->SetOrgY  (Roi_y);
	IRoi->SetWidth (Roi_w);
	IRoi->SetHeight(Roi_h);
}
void CEdge::OnPaintEdgePoint(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;
    if (GImage==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);
    TPoint Point1, Point2;
    Gdi->CText.Color  = YELLOW_COLOR;
    Gdi->CFont.Height = 12;

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    if (FEdgePointCount) {
        for (int i=0; i<FEdgePointCount; i++) {
            Gdi->OnLinePlus((int)FEdgePoint[i].x, (int)FEdgePoint[i].y, 4);
        }
    }

    delete Gdi;
}

void CEdge::OnPaint(TDrawTool Tool)
{
    OnPaintEdgePoint(Tool);
}
//---------------------------------------------------------------------------
CRectEdge::CRectEdge()
{
    FEdgePoint = NULL;
    FEdgePointCount = 0;

    FEdgeType = etBwOrWb;
    FThreshold = 5;
    FMinAmplitude =  10;
    FMinArea =  0;
    FThickness = 1;
    FSmoothing = 1;
    FMaxEdges = 1;

//     IRoi = new EROIBW8();
}
CRectEdge::~CRectEdge()
{
}
//------------------------------------------------------------------------------

/*
void CRectEdge::SetRoi(TFPoint FP, TFPoint SP)
{
    int l = FP.x <= SP.x ? FP.x : SP.x;
    int t = FP.y <= SP.y ? FP.y : SP.y;
    int r = FP.x >= SP.x ? FP.x : SP.x;
    int b = FP.y >= SP.y ? FP.y : SP.y;

    IRoi->SetOrgX(l);
    IRoi->SetOrgY(t);
    IRoi->SetWidth (r-l);
    IRoi->SetHeight(b-t);
}
*/

//------------------------------------------------------------------------------
void CRectEdge::OnExecute(bool l, bool t, bool r, bool b)
{
    FWest = l;
    FNorth = t;
    FEast = r;
    FSouth = b;
    OnExecute();
}

int CRectEdge::FindVEdge(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromX, int toX, bool Top2Down, int minEdgeCnt, int& Y)
{
	GImage = new EImageBW8();
	GImage->SetImagePtr(nFrmW, nFrmH, pImgPtr);

    const int xIncrement = 5;

	EPointGauge PointGauge; // EPointGauge instance
	PointGauge.SetDragable(true);
    PointGauge.SetResizable(true);
    PointGauge.SetRotatable(true);
	PointGauge.SetTransitionType(ETransitionType_BwOrWb);
	PointGauge.SetTransitionChoice(Top2Down ? ETransitionChoice_NthFromBegin : ETransitionChoice_NthFromEnd);
	PointGauge.SetThickness(1);
	PointGauge.SetTransitionIndex(0);
	PointGauge.SetThreshold(5);
	PointGauge.SetMinAmplitude(10);
	PointGauge.SetMinArea(0);
	PointGauge.SetTolerance((float)(nFrmH / 2));
	PointGauge.SetToleranceAngle(Top2Down ? 90.0f : 270.0f);

	std::vector<float> result;
	for (int x = fromX; x < toX; x+=xIncrement) {
		PointGauge.SetCenterXY((float)x, (float)nFrmH/2);

		PointGauge.Measure(GImage);
		if (PointGauge.GetNumMeasuredPoints() > 0) {
			result.push_back(PointGauge.GetMeasuredPoint(0).GetY());
		}
	}

	if (result.size() > (size_t)minEdgeCnt) {
		std::sort(result.begin(), result.end());
		if (minEdgeCnt == 1) {
			Y = (int)result[0];
		}
		else if (minEdgeCnt >= 2) {
			size_t mid = result.size() / 2;
			Y = (int)result[mid];
		}
	}

	delete GImage;
	return (int)result.size();
}

int	CRectEdge::FindHEdge(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromY, int toY, bool Left2Right, int minEdgeCnt, int& X)
{
	GImage = new EImageBW8();
	GImage->SetImagePtr(nFrmW, nFrmH, pImgPtr);

    const int yIncrement = 2;
	
	EPointGauge PointGauge; // EPointGauge instance
	PointGauge.SetDragable(true);
    PointGauge.SetResizable(true);
    PointGauge.SetRotatable(true);
	PointGauge.SetTransitionType(ETransitionType_BwOrWb);
	PointGauge.SetTransitionChoice(Left2Right ? ETransitionChoice_NthFromBegin : ETransitionChoice_NthFromEnd);
	PointGauge.SetThickness(1);
	PointGauge.SetTransitionIndex(0);
	PointGauge.SetThreshold(5);
	PointGauge.SetMinAmplitude(10);
	PointGauge.SetMinArea(0);
	PointGauge.SetTolerance((float)(nFrmW / 2));
	PointGauge.SetToleranceAngle(Left2Right ? 0.0f : 180.0f);
	
	std::vector<float> result;
	for (int y = fromY; y < toY; y+=yIncrement) {
		PointGauge.SetCenterXY((float)(nFrmW/2), (float)y);
		
		PointGauge.Measure(GImage);
		if (PointGauge.GetNumMeasuredPoints() > 0) {
			result.push_back(PointGauge.GetMeasuredPoint(0).GetX());
		}
	}
	
	if (result.size() > (size_t)minEdgeCnt) {
		std::sort(result.begin(), result.end());
		if (minEdgeCnt == 1) {
			X = (int)result[0];
		}
		else if (minEdgeCnt >= 2) {
			size_t mid = result.size() / 2;
			X = (int)result[mid];
		}
	}
	
	delete GImage;
	return (int)result.size();
}
int CRectEdge::FindVEdge(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromX, int toX, bool Top2Down, int minEdgeCnt, int& Y, int ThresHold, int Type)
{
	GImage = new EImageBW8();
	GImage->SetImagePtr(nFrmW, nFrmH, pImgPtr);

	if(GImage->GetImagePtr() && ThresHold!=0){
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
		EasyImage::Threshold(GImage,&BinaryImg, ThresHold);
// 		BinaryImg.Save("D:\\ROTATE_IMG\\RectImage.bmp");
	}

	const int xIncrement = 10;

	EPointGauge PointGauge; // EPointGauge instance
	PointGauge.SetDragable(true);
	PointGauge.SetResizable(true);
	PointGauge.SetRotatable(true);
	PointGauge.SetTransitionType(ETransitionType_BwOrWb);
	PointGauge.SetTransitionChoice(Top2Down ? ETransitionChoice_NthFromBegin : ETransitionChoice_NthFromEnd);
	PointGauge.SetThickness(1);
	PointGauge.SetTransitionIndex(0);
	PointGauge.SetThreshold(5);
	PointGauge.SetMinAmplitude(10);
	PointGauge.SetMinArea(0);
	PointGauge.SetTolerance((float)(nFrmH / 2));
	PointGauge.SetToleranceAngle(Top2Down ? 90.0f : 270.0f);

	if      (Type==0) PointGauge.SetTransitionType(ETransitionType_Bw    ); // Black To White 
	else if (Type==1) PointGauge.SetTransitionType(ETransitionType_Wb    ); // White To Black  
	else if (Type==2) PointGauge.SetTransitionType(ETransitionType_BwOrWb); // NORMAL


	std::vector<float> result;
	for (int x = fromX; x < toX; x+=xIncrement) {
		PointGauge.SetCenterXY((float)x, (float)nFrmH/2);

		// 		PointGauge.Measure(GImage);
		if(ThresHold==0)  PointGauge.Measure(GImage    );
		else              PointGauge.Measure(&BinaryImg);

		if (PointGauge.GetNumMeasuredPoints() > 0) {
			result.push_back(PointGauge.GetMeasuredPoint(0).GetY());
		}
	}

	if (result.size() > (size_t)minEdgeCnt) {
		std::sort(result.begin(), result.end());
		if (minEdgeCnt == 1) {
			Y = (int)result[0];
		}
		else if (minEdgeCnt >= 2) {
			size_t mid = result.size() / 2;
			Y = (int)result[mid];
		}
	}

	delete GImage;
	return (int)result.size();
}

int	CRectEdge::FindHEdge(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromY, int toY, bool Left2Right, int minEdgeCnt, int& X, int ThresHold, int Type)
{
	GImage = new EImageBW8();
	GImage->SetImagePtr(nFrmW, nFrmH, pImgPtr);

	if(GImage->GetImagePtr() && ThresHold!=0){
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
		EasyImage::Threshold(GImage,&BinaryImg, ThresHold);
	}

	const int yIncrement = 2;

	EPointGauge PointGauge; // EPointGauge instance
	PointGauge.SetDragable(true);
	PointGauge.SetResizable(true);
	PointGauge.SetRotatable(true);
	PointGauge.SetTransitionType(ETransitionType_BwOrWb);
	PointGauge.SetTransitionChoice(Left2Right ? ETransitionChoice_NthFromBegin : ETransitionChoice_NthFromEnd);
	PointGauge.SetThickness(1);
	PointGauge.SetTransitionIndex(0);
	PointGauge.SetThreshold(5);
	PointGauge.SetMinAmplitude(10);
	PointGauge.SetMinArea(0);
	PointGauge.SetTolerance((float)(nFrmW / 2));
	PointGauge.SetToleranceAngle(Left2Right ? 0.0f : 180.0f);

	if      (Type==0) PointGauge.SetTransitionType(ETransitionType_Bw    ); // Black To White 
	else if (Type==1) PointGauge.SetTransitionType(ETransitionType_Wb    ); // White To Black  
	else if (Type==2) PointGauge.SetTransitionType(ETransitionType_BwOrWb); // NORMAL

	std::vector<float> result;
	for (int y = fromY; y < toY; y+=yIncrement) {
		PointGauge.SetCenterXY((float)(nFrmW/2), (float)y);

		if(ThresHold==0)  PointGauge.Measure(GImage    );
		else              PointGauge.Measure(&BinaryImg);

		if (PointGauge.GetNumMeasuredPoints() > 0) {
			result.push_back(PointGauge.GetMeasuredPoint(0).GetX());
		}
	}

	if (result.size() > (size_t)minEdgeCnt) {
		std::sort(result.begin(), result.end());
		if (minEdgeCnt == 1) {
			X = (int)result[0];
		}
		else if (minEdgeCnt >= 2) {
			size_t mid = result.size() / 2;
			X = (int)result[mid];
		}
	}

	delete GImage;
	return (int)result.size();
}

int CRectEdge::FindVEdgeFit(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromX, int toX, bool Top2Down, int minEdgeCnt, int& Y, int ThresHold, int Type, int Offset)
{
	GImage = new EImageBW8();
	GImage->SetImagePtr(nFrmW, nFrmH, pImgPtr);

	if(GImage->GetImagePtr() && ThresHold!=0){
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
		EasyImage::Threshold(GImage,&BinaryImg, ThresHold);
// 		BinaryImg.Save("D:\\ROTATE_IMG\\TEST_VEDGE.bmp");
	}

	if((float)((nFrmW / 2)-Offset)<=0) return false; //Offset값 적용시 허용 범위를 벗어나는 경우 

	const int xIncrement = 10;
	int Start = 0;
	int End   = 0;
	int Buffer= 0;

	// 동적 메모리 생성 
	// Vertical 방향기준 Offset값은 검사범위를 줄여가는 개념 
	if(Offset==0){
		Start  = fromX;
		End    = toX  ;
	}
	else         {
		Start  = Offset      ;
		End    = nFrmW-Offset;
	}

	Buffer = ((End - Start)/xIncrement);
	if(Buffer<=0) return false;

	if (FEdgePoint != NULL) delete[] FEdgePoint;
	FEdgePoint = new TFPoint[Buffer];
	FEdgePointCount=0;

	EPointGauge PointGauge; // EPointGauge instance
	PointGauge.SetDragable(true);
	PointGauge.SetResizable(true);
	PointGauge.SetRotatable(true);
	PointGauge.SetTransitionType(ETransitionType_BwOrWb);
	PointGauge.SetTransitionChoice(Top2Down ? ETransitionChoice_NthFromBegin : ETransitionChoice_NthFromEnd);
	PointGauge.SetThickness(1);
	PointGauge.SetTransitionIndex(0);
	PointGauge.SetThreshold(5);
	PointGauge.SetMinAmplitude(10);
	PointGauge.SetMinArea(0);
	//Offset 만큼 Edge검출 범위를 조정함
// 	if(Offset==0) PointGauge.SetTolerance((float)( nFrmH / 2)        );
// 	else          PointGauge.SetTolerance((float)((nFrmH / 2)-Offset));
	PointGauge.SetTolerance((float)( nFrmH / 2)        );

	PointGauge.SetToleranceAngle(Top2Down ? 90.0f : 270.0f);


	if      (Type==0) PointGauge.SetTransitionType(ETransitionType_Bw    ); // Black To White 
	else if (Type==1) PointGauge.SetTransitionType(ETransitionType_Wb    ); // White To Black  
	else if (Type==2) PointGauge.SetTransitionType(ETransitionType_BwOrWb); // NORMAL

	for (int x = Start; x < End; x+=xIncrement) {
		PointGauge.SetCenterXY((float)x, (float)nFrmH/2);

		// 		PointGauge.Measure(GImage);
		if(ThresHold==0)  PointGauge.Measure(GImage    );
		else              PointGauge.Measure(&BinaryImg);

		if (PointGauge.GetNumMeasuredPoints() > 0) {
			if(FEdgePointCount>=Buffer) continue;

			FEdgePoint[FEdgePointCount].x = PointGauge.GetMeasuredPoint(0).GetX();
			FEdgePoint[FEdgePointCount].y = PointGauge.GetMeasuredPoint(0).GetY();
			FEdgePointCount++;
		}
	}

	if(FEdgePointCount<minEdgeCnt) return false;

	// LineFitting 데이터 선언 
	double dist_th  = 1;
	int max_iter = FEdgePointCount/3;
	int num_th   = 1;
	int mid = FEdgePointCount/2;

	bool IsFind = OnLineFitRANSAC_2(true, &FEdgePoint[0], FEdgePointCount, dist_th, max_iter, 1);

	// LineFitting 성공시 결과물 전송...실패시 중간값 전송 

	if(IsFind){
		Y = (int)((FP0.y + FP1.y)/2);
	}
	else      {
		Y = (int)(FEdgePoint[mid].y);
	}

	delete GImage;
	return FEdgePointCount;
}

int	CRectEdge::FindHEdgeFit(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromY, int toY, bool Left2Right, int minEdgeCnt, int& X, int ThresHold, int Type, int Offset)
{
	int EdgeCnt   =0;
	int ChoiceCnt =0;
	GImage = new EImageBW8();
	GImage->SetImagePtr(nFrmW, nFrmH, pImgPtr);

	if(GImage->GetImagePtr() && ThresHold!=0){
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
		EasyImage::Threshold(GImage,&BinaryImg, ThresHold);
// 		BinaryImg.Save("D:\\ROTATE_IMG\\TEST_HEDGE.bmp");
		// 		GImage->Save("D:\\ROTATE_IMG\\15인치_Org.bmp");
	}

	const int yIncrement = 2;
	if((float)((nFrmH / 2)-Offset)<=0) return false; //Offset값 적용시 허용 범위를 벗어나는 경우 

	int Start  = 0;
	int End    = 0;
	int Buffer = 0;

	// 동적 메모리 생성 
	if(Offset==0){
		Start  = fromY;
		End    = toY;
	}
	else         {
		Start  = Offset      ;
		End    = nFrmH-Offset;
	}

	Buffer = ((End - Start)/yIncrement);
	if(Buffer<=0) return false;

	if (FEdgePoint != NULL) delete[] FEdgePoint;
	FEdgePoint = new TFPoint[Buffer];
	FEdgePointCount =0;

	EPointGauge PointGauge; // EPointGauge instance
	PointGauge.SetDragable(true);
	PointGauge.SetResizable(true);
	PointGauge.SetRotatable(true);
	PointGauge.SetTransitionType(ETransitionType_BwOrWb);
	PointGauge.SetTransitionChoice(Left2Right ? ETransitionChoice_NthFromBegin : ETransitionChoice_NthFromEnd);
	PointGauge.SetThickness(1);
	PointGauge.SetTransitionIndex(0);
	PointGauge.SetThreshold(5);
	PointGauge.SetMinAmplitude(10);
	PointGauge.SetMinArea(0);
	//Offset 만큼 Edge검출 범위를 조정함
//	if(Offset==0) PointGauge.SetTolerance((float)( nFrmW / 2)        );
//	else          PointGauge.SetTolerance((float)((nFrmW / 2)-Offset));

	PointGauge.SetTolerance((float)(nFrmW / 2));

	PointGauge.SetToleranceAngle(Left2Right ? 0.0f : 180.0f);

	ChoiceCnt = PointGauge.GetTransitionChoice();

	if      (Type==0) PointGauge.SetTransitionType(ETransitionType_Bw    ); // Black To White 
	else if (Type==1) PointGauge.SetTransitionType(ETransitionType_Wb    ); // White To Black  
	else if (Type==2) PointGauge.SetTransitionType(ETransitionType_BwOrWb); // NORMAL

	for (int y = Start; y < End; y+=yIncrement) {
		PointGauge.SetCenterXY((float)(nFrmW/2), (float)y);

		if(ThresHold==0)  PointGauge.Measure(GImage    );
		else              PointGauge.Measure(&BinaryImg);

		EdgeCnt   =0;
		ChoiceCnt =0;
		EdgeCnt = PointGauge.GetNumMeasuredPoints();
		ChoiceCnt = PointGauge.GetTransitionChoice();

		if (PointGauge.GetNumMeasuredPoints() > 0) {
			if(FEdgePointCount>=Buffer) continue;

			FEdgePoint[FEdgePointCount].x = PointGauge.GetMeasuredPoint(0).GetX();
			FEdgePoint[FEdgePointCount].y = PointGauge.GetMeasuredPoint(0).GetY();
			FEdgePointCount++;
		}
	}

	if(FEdgePointCount<minEdgeCnt) return false;

	// LineFitting 데이터 선언 
	double dist_th  = 1;
	int max_iter = FEdgePointCount/3;
	int num_th   = 1;
	int mid = FEdgePointCount/2;

	bool IsFind = OnLineFitRANSAC_2(true, &FEdgePoint[0], FEdgePointCount, dist_th, max_iter, 1);

	// LineFitting 성공시 결과물 전송...실패시 중간값 전송 

	if(IsFind){
		X = (int)((FP0.x + FP1.x)/2);
	}
	else      {
		X = (int)(FEdgePoint[mid].x);
	}

	delete GImage;
	return FEdgePointCount;
}
int CRectEdge::FindVEdgeFitRange(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromX, int toX, bool Top2Down, int minEdgeCnt, int& Y, int ThresHold, int Type, int Offset, int Range)
{
	int ROI_L     =0;
	int ROI_T     =0;
	int ROI_W     =0;
	int ROI_H     =0;

	GImage = new EImageBW8();
	GImage->SetImagePtr(nFrmW, nFrmH, pImgPtr);
	EROIBW8   FilterROI                       ;

	if(GImage->GetImagePtr() && ThresHold!=0){
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
		EasyImage::Threshold(GImage,&BinaryImg, ThresHold);
// 	    GImage->Save("D:\\ROTATE_IMG\\RECT_VFIT_ORG.bmp");
// 	    BinaryImg.Save("D:\\ROTATE_IMG\\TEST_VEDGE_ORG.bmp");
	}

	if (!BinaryImg.GetImagePtr()     )                                                         return false; //BinaryImage NULL값일 경우 retrun 
	if((float)((nFrmW / 2)-Offset)<=0) {AfxMessageBox(L"FindVEdge Offset 범위가 초과했습니다");return false;} //Offset값 적용시 허용 범위를 벗어나는 경우 
	if(Range >=nFrmH                 ) {AfxMessageBox(L"FindVEdge Range  범위가 초과했습니다");return false;} //검사 Range값이 범위를 벗어나는 경우

	const int xIncrement = 10;
	int Start = 0;
	int End   = 0;
	int Buffer= 0;

	// 동적 메모리 생성 
	// Vertical 방향기준 Offset값은 검사범위를 줄여가는 개념 
	if(Offset==0){
		Start  = fromX;
		End    = toX  ;
	}
	else         {
		Start  = Offset      ;
		End    = nFrmW-Offset;
	}

    Buffer = ((End - Start)/xIncrement);
	if(Buffer<=0) return false;

	if (FEdgePoint != NULL) delete[] FEdgePoint;
	FEdgePoint = new TFPoint[Buffer];
	FEdgePointCount=0;

	// Median Filter ROI Setting 
	ROI_L = Start                           ;
	ROI_T = (Range==0)?0:((nFrmH/2)-Range/2);
	ROI_W = End - Start                     ;
	ROI_H = (Range==0)?nFrmH:(Range)        ;

	// Median Filter 적용 
	FilterROI.SetSize(ROI_W,ROI_H)                    ;
	FilterROI.Attach(&BinaryImg)                      ;
	FilterROI.SetPlacement(ROI_L, ROI_T, ROI_W, ROI_H);
	EasyImage::Median(&FilterROI, &FilterROI)         ;

// 	BinaryImg.Save("D:\\ROTATE_IMG\\TEST_VEDGE_FILTER.bmp");

	EPointGauge PointGauge; // EPointGauge instance
	PointGauge.SetDragable(true);
	PointGauge.SetResizable(true);
	PointGauge.SetRotatable(true);
	PointGauge.SetTransitionType(ETransitionType_BwOrWb);
	PointGauge.SetTransitionChoice(Top2Down ? ETransitionChoice_NthFromBegin : ETransitionChoice_NthFromEnd);
	PointGauge.SetThickness(1);
	PointGauge.SetTransitionIndex(0);
	PointGauge.SetThreshold(5);
	PointGauge.SetMinAmplitude(10);
	PointGauge.SetMinArea(0);
	//Offset 만큼 Edge검출 범위를 조정함
	if(Range==0) PointGauge.SetTolerance((float)(nFrmH / 2));
	else         PointGauge.SetTolerance((float)(Range / 2));

	PointGauge.SetToleranceAngle(Top2Down ? 90.0f : 270.0f);

	if      (Type==0) PointGauge.SetTransitionType(ETransitionType_Bw    ); // Black To White 
	else if (Type==1) PointGauge.SetTransitionType(ETransitionType_Wb    ); // White To Black  
	else if (Type==2) PointGauge.SetTransitionType(ETransitionType_BwOrWb); // NORMAL

	for (int x = Start; x < End; x+=xIncrement) {
		PointGauge.SetCenterXY((float)x, (float)nFrmH/2);

		// 		PointGauge.Measure(GImage);
		if(ThresHold==0)  PointGauge.Measure(GImage    );
		else              PointGauge.Measure(&BinaryImg);

		if (PointGauge.GetNumMeasuredPoints() > 0) {
			if(FEdgePointCount>=Buffer) continue;

			FEdgePoint[FEdgePointCount].x = PointGauge.GetMeasuredPoint(0).GetX();
			FEdgePoint[FEdgePointCount].y = PointGauge.GetMeasuredPoint(0).GetY();
			FEdgePointCount++;
		}
	}

	if(FEdgePointCount<minEdgeCnt) return false;

	// LineFitting 데이터 선언 
	double dist_th  = 1;
	int max_iter = FEdgePointCount/3;
	int num_th   = 1;
	int mid = FEdgePointCount/2;

	bool IsFind = OnLineFitRANSAC_2(true, &FEdgePoint[0], FEdgePointCount, dist_th, max_iter, 1);

	// LineFitting 성공시 결과물 전송...실패시 중간값 전송 

	if(IsFind){
		Y = (int)((FP0.y + FP1.y)/2);
	}
	else      {
		Y = (int)(FEdgePoint[mid].y);
	}

	delete GImage;
	return FEdgePointCount;
}

int	CRectEdge::FindHEdgeFitRange(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromY, int toY, bool Left2Right, int minEdgeCnt, int& X, int ThresHold, int Type, int Offset, int Range)
{
	int EdgeCnt   =0;
	int ChoiceCnt =0;

	int ROI_L     =0;
	int ROI_T     =0;
	int ROI_W     =0;
	int ROI_H     =0;

	GImage = new EImageBW8();
	GImage->SetImagePtr(nFrmW, nFrmH, pImgPtr);
	EROIBW8   FilterROI                       ;

	if(GImage->GetImagePtr() && ThresHold!=0){
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&BinaryImg);
		EasyImage::Threshold(GImage,&BinaryImg, ThresHold);
// 	    GImage->Save("D:\\ROTATE_IMG\\RECT_HFIT_ORG.bmp");
// 		BinaryImg.Save("D:\\ROTATE_IMG\\RECT_H_ORG.bmp");
	}

	if(!BinaryImg.GetImagePtr())       return false; //BinaryImage NULL값일 경우 retrun 
	if((float)((nFrmW / 2)-Offset)<=0) {AfxMessageBox(L"FindHEdge Offset 범위가 초과했습니다");return false;} //Offset값 적용시 허용 범위를 벗어나는 경우 
	if(Range >=nFrmW                 ) {AfxMessageBox(L"FindHEdge Range  범위가 초과했습니다");return false;} //검사 Range값이 범위를 벗어나는 경우


	const int yIncrement = 2;

	int Start  = 0;
	int End    = 0;
	int Buffer = 0;

	// 동적 메모리 생성 
	if(Offset==0){
		Start  = fromY;
		End    = toY;
	}
	else         {
		Start  = Offset      ;
		End    = nFrmH-Offset;
	}

	Buffer = ((End - Start)/yIncrement);
	if(Buffer<=0) return false;

	if (FEdgePoint != NULL) delete[] FEdgePoint;
	FEdgePoint = new TFPoint[Buffer];
	FEdgePointCount =0;

	// Median Filter ROI Setting 
	ROI_L = (Range==0)?0:((nFrmW/2)-Range/2);
	ROI_T = Start                           ;
	ROI_W = (Range==0)?nFrmW:(Range)        ;
	ROI_H = End - Start                     ;

	// Median Filter 적용 
	FilterROI.SetSize(ROI_W,ROI_H)                        ;
	FilterROI.Attach(&BinaryImg)                          ;
	FilterROI.SetPlacement(ROI_L, ROI_T, ROI_W, ROI_H)    ;
	EasyImage::Median(&FilterROI, &FilterROI)             ;

// 	BinaryImg.Save("D:\\ROTATE_IMG\\RECT_H_FILTER.bmp")   ;

	EPointGauge PointGauge; // EPointGauge instance
	PointGauge.SetDragable(true);
	PointGauge.SetResizable(true);
	PointGauge.SetRotatable(true);
	PointGauge.SetTransitionType(ETransitionType_BwOrWb);
// 	PointGauge.SetTransitionChoice(Left2Right ? ETransitionChoice_NthFromBegin : ETransitionChoice_NthFromEnd);
	PointGauge.SetTransitionChoice(ETransitionChoice_NthFromBegin);
	PointGauge.SetThickness(1);
	PointGauge.SetTransitionIndex(0);
	PointGauge.SetThreshold(5);
	PointGauge.SetMinAmplitude(10);
	PointGauge.SetMinArea(0);
	//Offset 만큼 Edge검출 범위를 조정함
	if(Range==0) PointGauge.SetTolerance((float)(nFrmW / 2));
	else         PointGauge.SetTolerance((float)(Range / 2));

	PointGauge.SetToleranceAngle(Left2Right ? 0.0f : 180.0f);

	ChoiceCnt = PointGauge.GetTransitionChoice();

	if      (Type==0) PointGauge.SetTransitionType(ETransitionType_Bw    ); // Black To White 
	else if (Type==1) PointGauge.SetTransitionType(ETransitionType_Wb    ); // White To Black  
	else if (Type==2) PointGauge.SetTransitionType(ETransitionType_BwOrWb); // NORMAL

	for (int y = fromY; y < toY; y+=yIncrement) {
//		if(Left2Right) PointGauge.SetCenterXY((float)(        Offset+(Range/2)), (float)y); //Left 영역 PointGage Center Setting 
//		else           PointGauge.SetCenterXY((float)(nFrmW - Offset-(Range/2)), (float)y); //RIght영역 PointGage Center Setting 

		PointGauge.SetCenterXY((float)(nFrmW / 2), (float)y);

		if(ThresHold==0)  PointGauge.Measure(GImage    );
		else              PointGauge.Measure(&BinaryImg);

		EdgeCnt   =0;
		ChoiceCnt =0;
		EdgeCnt = PointGauge.GetNumMeasuredPoints();
		ChoiceCnt = PointGauge.GetTransitionChoice();

		if (PointGauge.GetNumMeasuredPoints() > 0) {
			if(FEdgePointCount>=Buffer) continue;

			FEdgePoint[FEdgePointCount].x = PointGauge.GetMeasuredPoint(0).GetX();
			FEdgePoint[FEdgePointCount].y = PointGauge.GetMeasuredPoint(0).GetY();
			FEdgePointCount++;
		}
	}

	if(FEdgePointCount<minEdgeCnt) return false;

	// LineFitting 데이터 선언 
	double dist_th  = 1;
	int max_iter = FEdgePointCount/3;
	int num_th   = 1;
	int mid = FEdgePointCount/2;

	bool IsFind = OnLineFitRANSAC_2(true, &FEdgePoint[0], FEdgePointCount, dist_th, max_iter, 1);

	// LineFitting 성공시 결과물 전송...실패시 중간값 전송 

	if(IsFind){
		X = (int)((FP0.x + FP1.x)/2);
	}
	else      {
		X = (int)(FEdgePoint[mid].x);
	}

	delete GImage;
	return FEdgePointCount;
}

int CRectEdge::OnExecute()
{
    int szX = IRoi->GetWidth()*4;
    int szY = IRoi->GetHeight()*4;
    int szBuffer = szX+szY;

    if (szBuffer<10) {
        //ShowMessage("Check Roi Size");
        return false;
    }

    if (FEdgePoint != NULL) delete[] FEdgePoint;

    FEdgePoint = new TFPoint[szBuffer];
    FEdgePointCount = 0;

    TPoint SP, EP;

    //EWorldShape WorldShape; // EWorldShape instance
    EPointGauge PointGauge; // EPointGauge instance

    // Set Default Data.
    int xIncrement = FIncrement;
    int yIncrement = FIncrement;
    int Px, Py, Pw, Ph;
    bool FFwdDirection = true;

    int Index = 0;

    switch (Index) {
        case 0 :
                 IRoi->Attach(GImage);
                 FEdgePointCount = 0;
                 Index++;

        case 1 :
                 Px = IRoi->GetOrgX  ();
                 Py = IRoi->GetOrgY  ();
                 Pw = IRoi->GetWidth ();
                 Ph = IRoi->GetHeight();

                 if (FIncrement<1) FIncrement = 1;

                 //WorldShape.SetSensorSize(Pw, Ph);
                 //WorldShape.Process(GImage, true);

                 PointGauge.SetDragable(true);
                 PointGauge.SetResizable(true);
                 PointGauge.SetRotatable(true);

                 //PointGauge.Attach(&WorldShape);
                 PointGauge.SetMinAmplitude(FMinAmplitude);
                 PointGauge.SetMinArea     (FMinArea     );
                 PointGauge.SetThreshold   (FThreshold   );
                 PointGauge.SetThickness   (FThickness   );
                 PointGauge.SetSmoothing   (FSmoothing   );
                 PointGauge.SetFound       (FMaxEdges    );

                 PointGauge.SetTransitionType(ETransitionType_Wb);
                 if      (FEdgeType==0) PointGauge.SetTransitionType(ETransitionType_Bw    );
                 else if (FEdgeType==1) PointGauge.SetTransitionType(ETransitionType_Wb    );
                 else if (FEdgeType==2) PointGauge.SetTransitionType(ETransitionType_BwOrWb);
                 else if (FEdgeType==3) PointGauge.SetTransitionType(ETransitionType_Bwb);
                 else if (FEdgeType==4) PointGauge.SetTransitionType(ETransitionType_Wbw);

                 xIncrement = FIncrement;
                 yIncrement = FIncrement;
                 FEdgePointCount = 0;

                 Index++;

        // Left -> Right Direction
        //----------------------------------------------------------------------
        case 2 :
                 if (FWest) {
                     FFwdDirection = true;
                     for (int y=Py; y<Py+Ph; y+=yIncrement) {
                         SP.x = Px;
                         EP.x = Px+Pw;
                         SP.y = y;
                         EP.y = y;

                         PointGauge.SetTolerance((float)(Pw/2));
                         PointGauge.SetToleranceAngle(FFwdDirection ? 0.0f : 180.0f);
                         PointGauge.SetCenterXY((float)(SP.x+(EP.x-SP.x)/2), (float)(SP.y+(EP.y-SP.y)/2));
                         PointGauge.Measure(GImage);

                         int n = PointGauge.GetNumMeasuredPoints();

                         if (n) {
                             FEdgePoint[FEdgePointCount].x = PointGauge.GetMeasuredPoint(0).GetX();
                             FEdgePoint[FEdgePointCount].y = PointGauge.GetMeasuredPoint(0).GetY();
                             FEdgePointCount++;
                         }
                     }
                 }
                 Index++;


        // Top -> Botttom Direction
        //----------------------------------------------------------------------
        case 3 :
                 if (FNorth) {
                     FFwdDirection = true;
                     for (int x=Px; x<Px+Pw; x+=xIncrement) {
                         SP.x = x;
                         EP.x = x;
                         SP.y = Py;
                         EP.y = Py+Ph;

                         PointGauge.SetCenterXY((float)(SP.x+(EP.x-SP.x)/2), (float)(SP.y+(EP.y-SP.y)/2));
                         PointGauge.SetTolerance((float)(Ph/2));
                         PointGauge.SetToleranceAngle(FFwdDirection ? 90.0f : 270.0f);
                         PointGauge.Measure(GImage);

                         int n= PointGauge.GetNumMeasuredPoints();

                         if (n) {
                             FEdgePoint[FEdgePointCount].x = PointGauge.GetMeasuredPoint(0).GetX();
                             FEdgePoint[FEdgePointCount].y = PointGauge.GetMeasuredPoint(0).GetY();
                             FEdgePointCount++;
                         }
                     }
                 }
                 Index++;


        // Right -> Left Direction
        //----------------------------------------------------------------------
        case 4 :
                 if (FEast) {
                     FFwdDirection = false;
                     for (int y=Py; y<Py+Ph; y+=yIncrement) {
                         SP.x = Px;
                         EP.x = Px+Pw;
                         SP.y = y;
                         EP.y = y;

                         PointGauge.SetTolerance((float)(Pw/2));
                         PointGauge.SetToleranceAngle(FFwdDirection ? 0.0f : 180.0f);
                         PointGauge.SetCenterXY((float)(SP.x+(EP.x-SP.x)/2), (float)(SP.y+(EP.y-SP.y)/2));
                         PointGauge.Measure(GImage);

                         int n = PointGauge.GetNumMeasuredPoints();

                         if (n) {
                             FEdgePoint[FEdgePointCount].x = PointGauge.GetMeasuredPoint(0).GetX();
                             FEdgePoint[FEdgePointCount].y = PointGauge.GetMeasuredPoint(0).GetY();
                             FEdgePointCount++;
                         }
                     }
                 }
                 Index++;


        // Bottom -> Top Direction
        //----------------------------------------------------------------------
        case 5 :
                 if (FSouth) {
                     FFwdDirection = false;
                     for (int x=Px; x<Px+Pw; x+=xIncrement) {
                         SP.x = x;
                         EP.x = x;
                         SP.y = Py;
                         EP.y = Py+Ph;

                         PointGauge.SetCenterXY((float)(SP.x+(EP.x-SP.x)/2), (float)(SP.y+(EP.y-SP.y)/2));
                         PointGauge.SetTolerance((float)(Ph/2));
                         PointGauge.SetToleranceAngle(FFwdDirection ? 90.0f : 270.0f);
                         PointGauge.Measure(GImage);

                         int n= PointGauge.GetNumMeasuredPoints();

                         if (n) {
                             FEdgePoint[FEdgePointCount].x = PointGauge.GetMeasuredPoint(0).GetX();
                             FEdgePoint[FEdgePointCount].y = PointGauge.GetMeasuredPoint(0).GetY();
                             FEdgePointCount++;
                         }
                     }
                 }
                 Index++;

        case 6 :
                 Index=0;
                 return true;

    }
    return false;
}

void CRectEdge::OnPaint(TDrawTool Tool)
{
    CEdge::OnPaint(Tool);
}
bool __fastcall CRectEdge::OnLineFitRANSAC_2(bool IsHor, TFPoint *Q1, int Count, double Deviation, int max_iter, int MinCount)
{
	bool IsFind = false;
	//     const int MaxPoint = 1500;
	const int MaxPoint  = 3500;
	const int DelayTime =  300;

	if (Count<4) return false;
	if (Count>=MaxPoint) return false;

	TFPoint PxData [MaxPoint];
	TFPoint PxData1[MaxPoint];

	// 1
	// -------------------------------------------------------------------------
	int PxCount = 0;
	for (int n=1; n<Count-1; n++) {
		TFPoint f1 = Q1[n-1];
		TFPoint f2 = Q1[n  ];
		TFPoint f3 = Q1[n+1];

		float Dv = 0.0f;
		if (IsHor) Dv = 2*f2.x-(f1.x+f3.x);
		else       Dv = 2*f2.y-(f1.y+f3.y);

		if (Dv<=Deviation) {
			PxData[PxCount++] = Q1[n];
		}
	}
	Count = PxCount;
	if (Count<3) return false;
	if (Count>=MaxPoint) return false;

	// 2
	// -------------------------------------------------------------------------
	PxCount = 0;
	for (int i=1; i<Count-1; i++) {
		TFPoint f1 = PxData[i-1];
		TFPoint f2 = PxData[i  ];
		TFPoint f3 = PxData[i+1];

		float Dv = 0.0f;
		if (IsHor) Dv = 2*f2.x-(f1.x+f3.x);
		else       Dv = 2*f2.y-(f1.y+f3.y);

		if (Dv<=Deviation) {
			PxData1[PxCount++] = PxData[i];
		}
	}
	Count = PxCount;
	if (Count<3) return false;
	if (Count>=MaxPoint) return false;

	for (int k=0; k<Count; k++) {
		PxData[k] = PxData1[k];
	}

	int MaxNum = 3;
	int index  = 0;
	int Prev_n = 0;

	TFPoint P[2];
	int Inc = 2;

	Time.Clear();
	for (int e=Count-1; e>0; e-=Inc) {
		for (int s=0; s<Count; s+=Inc) {
			P[0]=PxData[s];
			P[1]=PxData[e];
			if(Time.On(true,DelayTime)) return false; //Cell상태에 따라서 LineFitting 진행시 일정시간 이상 소요될경우 강제 return 해준다 

			if (P[1].x==P[0].x && P[1].y==P[0].y) continue;

			int n = 2;
			int Cnt = 0;
			double dist = 0;

			for(int j=0; j<Count; j++) {
				dist = Math.GetLengthFrLineToPoint(P[0], P[1], PxData[j]);
				if (dist<=Deviation) {
					n++;
				}
			}
			if (n>=MinCount) {
				if (n>Prev_n) {
					Prev_n = n;
					FP0 = P[0];
					FP1 = P[1];
					IsFind = true;
					double BkCount = Count*0.50;
					// 				if(RansacScore==0.0f) RansacScore =0.06;
					// 				double BkCount = Count*RansacScore;
					if (n>BkCount) return true;
					//return true;
				}
			}
		}}
	return IsFind;
}





