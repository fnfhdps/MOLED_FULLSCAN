// LxLine.cpp: implementation of the CSingleLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LxLine.h"
#include "LxHeader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSingleLine::CSingleLine()
{
    FEdgeType = etBwOrWb;
    FEdgePoint = NULL;
    FEdgePointCount = 0;


	FThickness   = 1 ;
	FSmoothing   = 1 ;
	FThreshold   = 10;
	FMinAmplitude= 10;
	FMinArea     =  0;
	FEdgeType    = etBwOrWb;

    FIncrement = 10;
    FFindLineId = 0;
	RansacScore =0.0f;

    IRoi = new EROIBW8();
}

CSingleLine::~CSingleLine()
{
}
void CSingleLine::SetOrientation(int Orientation)
{
    FOrientation = Orientation;
}
void CSingleLine::SetFindLineId(int FindLineId)
{
    FFindLineId  = FindLineId ;
}
void CSingleLine::SetIncrement(int Increment)
{
    FIncrement   = Increment  ;
}
void CSingleLine::SetEdgeType(int EdgeType)
{
    FEdgeType  = EdgeType;
}
// void CSingleLine::SetMinEdgeCount(int EdgeCount)
// {
// 	FMinEdgeCount  = EdgeCount;
// }

TFPoint CSingleLine::GetResultP0()
{
    return FP0;
}
TFPoint CSingleLine::GetResultP1()
{
    return FP1;
}
TFPoint CSingleLine::GetResultCP()
{
    return FCP;
}
double __fastcall CSingleLine::GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3)
{
    //--------------------------------------------------------------------------
    // 직선과 한점의 수직한 교차점에 의한 치수 측정
    // 공식 : ax + by + c = 0, (m, n) -> Distance = (ma + nb +c) / sqrt(a*a + b*b)
    //--------------------------------------------------------------------------

    double xDiff = P2.x-P1.x;
    double yDiff = P2.y-P1.y;

    if      (xDiff == 0) return fabs(P3.x-P1.x);
    else if (yDiff == 0) return fabs(P3.y-P1.y);

    double m    = yDiff/(float)xDiff;
    double dist = fabs((-m*P3.x + P3.y + m*P1.x - P1.y)/sqrt(m*m + 1));

    return dist;
}
double __fastcall CSingleLine::OnLineDistance(double x1, double y1, double x2, double y2, double x, double y)
{
    if ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) != 0) {
        double u  = ((x-x1)*(x2-x1) + (y-y1)*(y2-y1));
        double xp = x1 + u * (x2-x1);
        double yp = y1 + u * (y2-y1);
        return sqrt((xp-x)*(xp-x) + (yp-y)*(yp-y));
    }
    return sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1));
}

bool __fastcall CSingleLine::OnLineFitRANSAC_2(bool IsHor, TFPoint *Q1, int Count, double Deviation, int max_iter, int MinCount)
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

        int n = 0;
        int Cnt = 0;
        double dist = 0;

        for(int j=0; j<Count; j++) {
            dist = GetLengthFrLineToPoint(P[0], P[1], PxData[j]);
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
                RansacScore = 0.0f;
                RansacScore = ((double)n/(double)Count)*100;
				// 				if(RansacScore==0.0f) RansacScore =0.06;
// 				double BkCount = Count*RansacScore;
                if (n>BkCount) return true;
                //return true;
            }
        }
    }}
    return IsFind;
}

int CSingleLine::OnExecute()
{
    //EWorldShape WorldShape; // EWorldShape instance
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CSingleLine GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CSingleLine GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CSingleLine GImage Height Size"    )); return false; }

    EPointGauge PointGauge; // EPointGauge instance

    int szX = IRoi->GetWidth();
    int szY = IRoi->GetHeight();
    int szBuffer = szX > szY ? szX : szY;

    if (FEdgePoint != NULL) delete[] FEdgePoint;
    FEdgePoint = new TFPoint[szBuffer];

    int Px = IRoi->GetOrgX();
    int Py = IRoi->GetOrgY();
    int Pw = IRoi->GetWidth();
    int Ph = IRoi->GetHeight();

    if (FIncrement<1) FIncrement = 5;

    //WorldShape.SetSensorSize(Pw, Ph);
    //WorldShape.Process(GImage, true);

    PointGauge.SetDragable(true);
    PointGauge.SetResizable(true);
    PointGauge.SetRotatable(true);

    if (FThickness<1) FThickness = 1;
    if (FMinAmplitude==0 && FThreshold==0) {
        FMinAmplitude = 1 ;
        FThreshold    = 1 ;
        FThickness    = 20;
    }

    // Set Default Data.
    FMinAmplitude =  1;
    FMinArea      =  0;
	FThickness    =  1;

    //PointGauge.Attach(&WorldShape);
    //PointGauge.SetMinAmplitude(FMinAmplitude);
    PointGauge.SetMinArea     (FMinArea     );
    PointGauge.SetThreshold   (FThreshold   );
    PointGauge.SetThickness   (FThickness   );
    PointGauge.SetSmoothing   (FSmoothing   );

    PointGauge.SetTransitionType(ETransitionType_BwOrWb);

    if      (FEdgeType==0) PointGauge.SetTransitionType(ETransitionType_Bw    ); // BTM
    else if (FEdgeType==1) PointGauge.SetTransitionType(ETransitionType_Wb    ); // TOP 
    else if (FEdgeType==2) PointGauge.SetTransitionType(ETransitionType_BwOrWb); // NORMAL
    else if (FEdgeType==3) PointGauge.SetTransitionType(ETransitionType_Bwb   );
    else if (FEdgeType==4) PointGauge.SetTransitionType(ETransitionType_Wbw   );

    const int xIncrement = FIncrement;
    const int yIncrement = FIncrement;

    FP0.x = 0; FP0.y = 0; FP1.x = 0; FP1.y = 0;

    TFPoint SP;
    TFPoint EP;

    int FMinEdgeCount = 1;
    int FFitDistance  = 1;
    FEdgePointCount = 0;

    bool FFindVerticalLine;
    bool FFwdDirection;
    if (FOrientation==orWest ) { FFindVerticalLine = true ; FFwdDirection = true ; }
    if (FOrientation==orEast ) { FFindVerticalLine = true ; FFwdDirection = false; }
    if (FOrientation==orNorth) { FFindVerticalLine = false; FFwdDirection = true ; }
    if (FOrientation==orSouth) { FFindVerticalLine = false; FFwdDirection = false; }

    if (FFindVerticalLine) {
        for (int y=Py; y<Py+Ph; y+=yIncrement) {
            SP.x = (float)Px;
            EP.x = (float)(Px+Pw);
            SP.y = (float)y;
            EP.y = (float)y;

            PointGauge.SetTolerance((float)(Pw/2));
            PointGauge.SetToleranceAngle(FFwdDirection ? 0.0f : 180.0f);
            PointGauge.SetCenterXY((SP.x+(EP.x-SP.x)/2), SP.y+(EP.y-SP.y)/2);
            PointGauge.Measure(GImage);
            //PointGauge.Measure(IRoi);

            int n = PointGauge.GetNumMeasuredPoints();

            if (n<=FFindLineId) continue;
            if (FEdgePointCount>=szBuffer) continue;
			if (PointGauge.GetMeasuredPoint(FFindLineId).GetX()==0.0f) continue;
			if (PointGauge.GetMeasuredPoint(FFindLineId).GetY()==0.0f) continue;

            FEdgePoint[FEdgePointCount].x = PointGauge.GetMeasuredPoint(FFindLineId).GetX();
            FEdgePoint[FEdgePointCount].y = PointGauge.GetMeasuredPoint(FFindLineId).GetY();
            FEdgePointCount++;
        }
    }

    else {
        for (int x=Px; x<Px+Pw; x+=xIncrement) {
            SP.x = (float)x;
            EP.x = (float)x;
            SP.y = (float)Py;
            EP.y = (float)(Py+Ph);

            PointGauge.SetCenterXY(SP.x+(EP.x-SP.x)/2, SP.y+(EP.y-SP.y)/2);
            PointGauge.SetTolerance((float)(Ph/2));
            PointGauge.SetToleranceAngle(FFwdDirection ? 90.0f : 270.0f);
            PointGauge.Measure(GImage);

            int n = PointGauge.GetNumMeasuredPoints();

            if (n<=FFindLineId) continue;
            if (FEdgePointCount>=szBuffer) continue;
			if (PointGauge.GetMeasuredPoint(FFindLineId).GetX()==0.0f) continue;
			if (PointGauge.GetMeasuredPoint(FFindLineId).GetY()==0.0f) continue;

            FEdgePoint[FEdgePointCount].x = PointGauge.GetMeasuredPoint(FFindLineId).GetX();
            FEdgePoint[FEdgePointCount].y = PointGauge.GetMeasuredPoint(FFindLineId).GetY();
            FEdgePointCount++;
        }
    }

    if (FEdgePointCount<FMinEdgeCount) return false;

    double dist_th  = FFitDistance;
    int max_iter = FEdgePointCount/3;
    int num_th   = 1;
    int mid = FEdgePointCount/2;

    bool FFind = OnLineFitRANSAC_2(true, &FEdgePoint[FFindLineId], FEdgePointCount, dist_th, max_iter, FMinEdgeCount);
    if (!FFind) return false;

    FAngle = Math.GetDegree(FP0.x, FP0.y, FP1.x, FP1.y);


    int w = GImage->GetWidth();
    int h = GImage->GetHeight();

    TFPoint HP1(0             ,(float)(h/2.0)); TFPoint HP2((float)w      , (float)(h/2.0));
    TFPoint VP1((float)(w/2.0),             0); TFPoint VP2((float)(w/2.0), (float)h      ); //세로

    if (FFindVerticalLine) FCP = Math.OnIntersect(FP0, FP1, HP1, HP2);
    else                   FCP = Math.OnIntersect(FP0, FP1, VP1, VP2);

    int FIgnoreAngle = 0;

    if (FIgnoreAngle) {
        if (fabs(FAngle)>FIgnoreAngle) {
            FCP.x = 0;
            FCP.y = 0;
        }
    }
    return true;
}

/*
int CSingleLine::OnExecute()
{
    int Px = IRoi->GetOrgX();
    int Py = IRoi->GetOrgY();
    int Pw = IRoi->GetWidth();
    int Ph = IRoi->GetHeight();

    FAngle = 0.0f;

    int Index = 0;
    double P1x = 0;
    double P1y = 0;
    double P2x = 0;
    double P2y = 0;
    int P[9] = {0,};

    switch (Index) {
        case 0 :
                 OnFindExecute(FFindVerticalLine, FFwdDirection);
                 Index++;

        case 1 :
                 int w = GImage->GetWidth();
                 int h = GImage->GetHeight();

                 TFPoint HP1(0    , h/2.0); TFPoint HP2(w    , h/2.0);
                 TFPoint VP1(w/2.0,     0); TFPoint VP2(w/2.0, h    ); //세로

                 if (FFindVerticalLine) FCP = Math.OnIntersect(FP0, FP1, HP1, HP2);
                 else                   FCP = Math.OnIntersect(FP0, FP1, VP1, VP2);

                 if (FIgnoreAngle) {
                     if (fabs(FAngle)>FIgnoreAngle) {
                         FCP.x = 0;
                         FCP.y = 0;
                     }
                 }

                 Index=0;
                 break;

    }

    return true;
}
*/

void CSingleLine::OnLoad(bool IsLoad, CString Folder)
{
    CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

    //if (Folder.Trim()=="") return;
    CString Path = Folder + "\\SLine.ini";
    if (Path=="") return;

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

    Ini.Load(IsLoad, Path, "Parameter", "FOrientation "  , &FOrientation );
    Ini.Load(IsLoad, Path, "Parameter", "FFindLineId  "  , &FFindLineId  );
    Ini.Load(IsLoad, Path, "Parameter", "FAngle       "  , &FAngle       );
    Ini.Load(IsLoad, Path, "Parameter", "FEdgeType    "  , &FEdgeType    );
    Ini.Load(IsLoad, Path, "Parameter", "FIncrement   "  , &FIncrement   );
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

void CSingleLine::OnPaintLine(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;
    if (GImage==NULL     ) return;

	int ImgW = GImage->GetWidth();
	int ImgH = GImage->GetHeight();

	if (ImgW<1) return;
	if (ImgH<1) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    TPoint Point1, Point2;
    Gdi->CText.Color  = YELLOW_COLOR;
    Gdi->CFont.Height = 12;

    TFPoint LT(0          ,           0);
    TFPoint RT((float)ImgW,           0);
    TFPoint RB((float)ImgW, (float)ImgH);
    TFPoint LB(0          , (float)ImgH);
    TFPoint SP, EP;

    if (FOrientation==orWest || FOrientation==orEast) {
        SP = Math.OnIntersect(FP0, FP1, LT, RT);
        EP = Math.OnIntersect(FP0, FP1, LB, RB);
    }
    else {
        SP = Math.OnIntersect(FP0, FP1, LT, LB);
        EP = Math.OnIntersect(FP0, FP1, RT, RB);
    }
    Gdi->OnMove((int)SP.x, (int)SP.y);
    Gdi->OnLine((int)EP.x, (int)EP.y);
    Gdi->CPen.Color = RED_COLOR;
    Gdi->OnLinePlus((int)FCP.x, (int)FCP.y, 15);

    delete Gdi;
}
void CSingleLine::OnPaintLineWidthIRoi(TDrawTool Tool)
{
	if (Tool.Handle==NULL) return;
	if (GImage==NULL) return;
	if (IRoi==NULL) return;

	int l = IRoi->GetOrgX();
	int t = IRoi->GetOrgY();
	int r = l+IRoi->GetWidth();
	int b = t+IRoi->GetHeight();

	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

	TPoint Point1, Point2;
	Gdi->CText.Color  = YELLOW_COLOR;
	Gdi->CFont.Height = 12;

	TFPoint LT(l, t);
	TFPoint RT(r, t);
	TFPoint RB(r, b);
	TFPoint LB(l, b);
	TFPoint SP, EP;

	Gdi->CPen.Color = Tool.Color;
	if (FOrientation==orWest || FOrientation==orEast) {
		SP = Math.OnIntersect(FP0, FP1, LT, RT);
		EP = Math.OnIntersect(FP0, FP1, LB, RB);
	}
	else {
		SP = Math.OnIntersect(FP0, FP1, LT, LB);
		EP = Math.OnIntersect(FP0, FP1, RT, RB);
	}

	if (SP.x < l) SP.x = l; if (EP.x < l) EP.x = l;
	if (SP.y < t) SP.y = t; if (EP.y < t) EP.y = t;
	if (SP.x > r) SP.x = r; if (EP.x > r) EP.x = r;
	if (SP.y > b) SP.y = b; if (EP.y > b) EP.y = b;

	Gdi->OnMove(SP.x, SP.y);
	Gdi->OnLine(EP.x, EP.y);

	delete Gdi;
}
void CSingleLine::OnPaintPoint(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;
    if (GImage==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    TPoint Point1, Point2;
    Gdi->CText.Color  = YELLOW_COLOR;
    Gdi->CFont.Height = 12;

    Gdi->OnLinePlus((int)FP0.x, (int)FP0.y, 5);
    Gdi->OnLinePlus((int)FP1.x, (int)FP1.y, 5);

    delete Gdi;
}
void CSingleLine::OnPaintEdgePoint(TDrawTool Tool)
{
	if (Tool.Handle==NULL) return;
	if (GImage==NULL) return;

	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

	for (int k=0; k<FEdgePointCount; k++) {
		if (FEdgePoint!=NULL)
			Gdi->OnLinePlus(FEdgePoint[k].x, FEdgePoint[k].y, 5);
	}

	delete Gdi;
}

void CSingleLine::OnPaintText(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;
    if (GImage==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    TPoint Point1, Point2;
    Gdi->CText.Color  = YELLOW_COLOR;
    Gdi->CFont.Height = 12;

	CString Str;

    Str.Format(L"Ag : %3.2f", FAngle);
    Gdi->OnText((int)(FCP.x+5), (int)(FCP.y+20), CW2A(Str));

    delete Gdi;
}

void CSingleLine::OnPaintRoi(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;
    if (GImage==NULL     ) return;
	if (IRoi==NULL       ) return;

	int ImgW = GImage->GetWidth();
	int ImgH = GImage->GetHeight();
	if (ImgW<1) return;
	if (ImgH<1) return;

	if (IRoi->GetWidth()<1) return;
	if (IRoi->GetHeight()<1) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    TPoint Point1, Point2;
    Gdi->CText.Color  = YELLOW_COLOR;
    Gdi->CFont.Height = 12;

    int Px = IRoi->GetOrgX();
    int Py = IRoi->GetOrgY();
    int Pw = IRoi->GetWidth();
    int Ph = IRoi->GetHeight();

    TFPoint LT(0          ,           0);
    TFPoint RT((float)ImgW,           0);
    TFPoint RB((float)ImgW, (float)ImgH);
    TFPoint LB(0          , (float)ImgH);
    TFPoint SP, EP;

    Gdi->CPen.Style = 2;
    Gdi->CBrush.Style = BS_HOLLOW;

    float l = (float)Px;
    float t = (float)Py;
    float r = (float)(Px+Pw);
    float b = (float)(Py+Ph);
    Gdi->OnMove((int)l, (int)t);
    Gdi->OnLine((int)r, (int)t);
    Gdi->OnLine((int)r, (int)b);
    Gdi->OnLine((int)l, (int)b);
    Gdi->OnLine((int)l, (int)t);

    float x1 = Px + Pw/2.0f;
    float y1 = (float)Py;
    float x2 = Px + Pw/2.0f;
    float y2 = (float)(Py + Ph);
    float x3 = (float)Px;
    float y3 = Py + Ph/2.0f;
    float x4 = (float)(Px + Pw);
    float y4 = Py + Ph/2.0f;

    Gdi->OnLine((int)x1,(int)y1,(int)x2,(int)y2);
    Gdi->OnLine((int)x3,(int)y3,(int)x4,(int)y4);

    delete Gdi;
}

void CSingleLine::OnPaint(TDrawTool Tool)
{
    OnPaintLine(Tool);
}

//---------------------------------------------------------------------------

CPolygoneLine::CPolygoneLine()
{
    for (int k=0; k<MAX_POLYGONE_LINE; k++) {
        ARoi[k] = new EROIBW8();
    }
    FMaxLines = 0;
	GImage     = NULL  ;
}

CPolygoneLine::~CPolygoneLine()
{
}

int CPolygoneLine::OnExecute()
{
    if (FMaxLines<1)           { MessageBox(NULL,L"Check MaxLines Property",     L"Error",MB_ICONERROR||MB_OK); return false; }
    if (GImage==NULL)          { MessageBox(NULL,L"Check GImage Memory Allocate",L"Error",MB_ICONERROR||MB_OK); return false; }
    if (GImage->GetWidth()<1)  { MessageBox(NULL,L"Check GImage Width Size",     L"Error",MB_ICONERROR||MB_OK); return false; }
    if (GImage->GetHeight()<1) { MessageBox(NULL,L"Check GImage Height Size",    L"Error",MB_ICONERROR||MB_OK); return false; }

    int Index = 0;
    int n =0;
  
    switch (Index) {
        case 0 :
                 Index++;

        case 1 :
			     // GOO 
                 for (n=0; n<FMaxLines; n++) {
                     IRoi = ARoi[n];
                     CSingleLine::OnExecute();
					 // 비선형 형태가 아님, 다각형 형태일경우 해당함 
					 if(FP0.x!=0.0f) Line[n].FP0.x = FP0.GetX();
					 if(FP0.y!=0.0f) Line[n].FP0.y = FP0.GetY();
					 if(FP1.x!=0.0f) Line[n].FP1.x = FP1.GetX();
					 if(FP1.y!=0.0f) Line[n].FP1.y = FP1.GetY();
                 }
                 Index++;

        case 2 :
                 for (int i=0; i<FMaxLines-1; i++) {
					 //다각형 형태지만...수직 교차점은 부정확함....왜해야 하는지 모르겠음....ㅜ.ㅜ;;
                     FIPoint[i] = Math.OnIntersect(Line[i].FP0, Line[i].FP1, Line[i+1].FP0, Line[i+1].FP1);
                 }
                 Index=0;
                 break;

    }

    return true;
}

float CPolygoneLine::GetLength(TFPoint p1, TFPoint p2)
{
    return (float)(sqrt(pow(p1.x-p2.x, 2) + pow(p1.y-p2.y, 2)));
}

void CPolygoneLine::OnLoad(bool IsLoad, CString Folder)
{
    CSingleLine::OnLoad(IsLoad, Folder);

	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

    //if (Folder.Trim()=="") return;
    CString Path = Folder + "\\PLine.ini";
    if (Path=="") return;

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

    Ini.Load(IsLoad, Path, "Parameter", "FMaxLines"  , &FMaxLines);
}

void CPolygoneLine::OnPaint(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);
	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    TPoint Point1, Point2;
    Gdi->CText.Color  = YELLOW_COLOR;
    Gdi->CFont.Height = 12;

    TFPoint SP, EP;


    for (int k=0; k<FMaxLines; k++) {
        Gdi->OnLinePlus((int)Line[k].FP0.x, (int)Line[k].FP0.y, 15);
    }
    
    delete Gdi;
}


//--------------------------------------------------------------------------
CLine::CLine()
{
    DwAllLine = false;
    DwPoint   = false;
    DwIpRoiCrossLine = false;
    DwHoRoi     = false;
    DwVtRoi     = false;
    DwAllLine   = false;
    DwPoint     = false;
    DwLine      = false;
    DwIntersect = false;
    DwRectangle = false;
    DwText      = false;
    DwEdgePoint = false;
	IsInOut     = false;
	Parameter.OpBinary = false;
	GImage      = NULL  ;


//     Image = new EImageBW8();

    //HRoi = new EROIBW8();
    //VRoi = new EROIBW8();
}
//------------------------------------------------------------------------------
CLine::~CLine()
{
}
//------------------------------------------------------------------------------

float __fastcall CLine::GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3)
{
    //--------------------------------------------------------------------------
    // 직선과 한점의 수직한 교차점에 의한 치수 측정
    // 공식 : ax + by + c = 0, (m, n) -> Distance = (ma + nb +c) / sqrt(a*a + b*b)
    //--------------------------------------------------------------------------

    float xDiff = P2.x-P1.x;
    float yDiff = P2.y-P1.y;

    if      (xDiff == 0) return fabs(P3.x-P1.x);
    else if (yDiff == 0) return fabs(P3.y-P1.y);

    float m    = yDiff/(float)xDiff;
    float dist = fabs((-m*P3.x + P3.y + m*P1.x - P1.y)/sqrt(m*m + 1));

    return dist;
}
//------------------------------------------------------------------------------

double __fastcall CLine::OnLineDistance(double x1, double y1, double x2, double y2, double x, double y)
{
    if ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) != 0) {
        double u  = ((x-x1)*(x2-x1) + (y-y1)*(y2-y1));
        double xp = x1 + u * (x2-x1);
        double yp = y1 + u * (y2-y1);
        return sqrt((xp-x)*(xp-x) + (yp-y)*(yp-y));
    }
    return sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1));
}
//------------------------------------------------------------------------------
bool __fastcall CLine::OnLineFitRANSAC_2(bool IsHor, TFPoint *Q1, int Count, double Deviation, int max_iter, int MinCount)
{
    bool IsFind = false;
    const int MaxPoint = 1500;

    if (Count<4) return false;
    if (Count>=MaxPoint) return false;

    TFPoint PxData [MaxPoint];
    TFPoint PxData1[MaxPoint];

    ////////////////////////////////////////////////////////////////////////////
    // 1
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
    if (Count<4) return false;
    if (Count>=MaxPoint) return false;

    ////////////////////////////////////////////////////////////////////////////
    // 2
    PxCount = 0;
    for (int n=1; n<Count-1; n++) {
        TFPoint f1 = PxData[n-1];
        TFPoint f2 = PxData[n  ];
        TFPoint f3 = PxData[n+1];

        float Dv = 0.0f;
        if (IsHor) Dv = 2*f2.x-(f1.x+f3.x);
        else       Dv = 2*f2.y-(f1.y+f3.y);

        if (Dv<=Deviation) {
            PxData1[PxCount++] = PxData[n];
        }
    }
    Count = PxCount;
    if (Count<4) return false;
    if (Count>=MaxPoint) return false;

    for (int k=0; k<Count; k++) {
        PxData[k] = PxData1[k];
    }

    int MaxNum = 3;
    int index  = 0;
    int Prev_n = 0;

    TFPoint P[2];
    int Inc = 2;

    for (int e=Count-1; e>0; e-=Inc) {
    for (int s=0; s<Count; s+=Inc) {
        P[0]=PxData[s];
        P[1]=PxData[e];

        if (P[1].x==P[0].x && P[1].y==P[0].y) continue;

        int n = 2;
        int Cnt = 0;
        float dist = 0;

        for(int j=0; j<Count; j++) {
            dist = GetLengthFrLineToPoint(P[0], P[1], PxData[j]);
            if (dist<=Deviation) {
                n++;
            }
        }

        if (n>=MinCount) {
            if (n>Prev_n) {
                Prev_n = n;
                P0 = P[0];
                P1 = P[1];
                IsFind = true;
                int BkCount = (int)(Count*0.60);
                if (n>BkCount) return true;
                //return true;
            }
        }
    }}
    return IsFind;
}
//------------------------------------------------------------------------------
int CLine::OnFindExecute(bool IsHorEdge, bool FwdDirection, int LineId)
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CLine GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CLine GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CLine GImage Height Size"    )); return false; }

    HRect IpRect;

	if (IsHorEdge) IpRect = HoRect;
	else           IpRect = VtRect;

    if (IpRect.x<0) return false;
    if (IpRect.y<0) return false;
    if (IpRect.w<0) return false;
    if (IpRect.h<0) return false;

    if (Parameter.Increment<8) Parameter.Increment = 8;
    Parameter.MaxEdges = LineId+1;
    if (Parameter.MaxEdges>MAX_LINE_COUNT) Parameter.MaxEdges = MAX_LINE_COUNT;

//     EWorldShape1.SetSensorSize(Width, Height);
//     Image->SetSize(Width, Height);
//     EWorldShape1.Process(Image, true);

    EPointGauge1.SetDragable(true);
    EPointGauge1.SetResizable(true);
    EPointGauge1.SetRotatable(true);

    if (Parameter.Thickness<1) Parameter.Thickness = 1;
    if (Parameter.MinAmplitude==0 && Parameter.Threshold==0) {
        Parameter.MinAmplitude = 1;
        Parameter.Threshold = 1;
        Parameter.Thickness = 20;
    }

    // Set Default Data.
    //Parameter.MinAmplitude=  1;
    //Parameter.MinArea     =  0;
    //Parameter.Thickness   = 20;

//     EPointGauge1.Attach(&EWorldShape1);

    EPointGauge1.SetMinAmplitude(Parameter.MinAmplitude);
    EPointGauge1.SetMinArea     (Parameter.MinArea     );

    EPointGauge1.SetThreshold   (Parameter.Threshold  );
    EPointGauge1.SetThickness   (Parameter.Thickness  );
    EPointGauge1.SetSmoothing   (Parameter.Smoothing  );

    EPointGauge1.SetTransitionType(ETransitionType_Wb);
    if      (Parameter.EdgeType==0) EPointGauge1.SetTransitionType(ETransitionType_Bw    );
    else if (Parameter.EdgeType==1) EPointGauge1.SetTransitionType(ETransitionType_Wb    );
    else if (Parameter.EdgeType==2) EPointGauge1.SetTransitionType(ETransitionType_BwOrWb);
    else if (Parameter.EdgeType==3) EPointGauge1.SetTransitionType(ETransitionType_Bwb);
    else if (Parameter.EdgeType==4) EPointGauge1.SetTransitionType(ETransitionType_Wbw);


    const int xIncrement = Parameter.Increment;
    const int yIncrement = Parameter.Increment;

    P0.x = 0;
    P0.y = 0;
    P1.x = 0;
    P1.y = 0;

    TFPoint SP;
    TFPoint EP;

    if (Parameter.MinEdgeCount<5  ) Parameter.MinEdgeCount = 5;
    if (Parameter.FitDistance <0.6) Parameter.FitDistance  = 1.0;

    if (IsHorEdge) {
        for (int k=0; k<MAX_LINE_COUNT; k++) {
            P0.x = 0;
            P0.y = 0;
            P1.x = 0;
            P1.y = 0;
            HFindLine[k].P1.x = 0;
            HFindLine[k].P1.y = 0;
            HFindLine[k].P2.x = 0;
            HFindLine[k].P2.y = 0;
            HFindEdgeCount[k] = 0;
        }

        for (int y=IpRect.y; y<IpRect.y+IpRect.h; y+=yIncrement) {
            SP.x = (float)IpRect.x;
            EP.x = (float)(IpRect.x+IpRect.w);
            SP.y = (float)y;
            EP.y = (float)y;

            EPointGauge1.SetTolerance((float)(IpRect.w/2));
            //EPointGauge1.SetTolerance(IpRect.w-2);
            EPointGauge1.SetToleranceAngle(FwdDirection ? 0.0f : 180.0f);
            EPointGauge1.SetCenterXY(SP.x+(EP.x-SP.x)/2, SP.y+(EP.y-SP.y)/2);
            EPointGauge1.Measure(GImage);

            int HFindCount = EPointGauge1.GetNumMeasuredPoints();
            //if (HFindCount>Parameter.MaxEdges) continue;

            for (int k=0; k<HFindCount; k++) {
                int n = HFindEdgeCount[k];
                if (k>=MAX_LINE_COUNT) continue;
                if (n>=MAX_EDGE_COUNT) continue;

                HFindEdge[k][n].x = EPointGauge1.GetMeasuredPoint(k).GetX();
                HFindEdge[k][n].y = EPointGauge1.GetMeasuredPoint(k).GetY();
                HFindEdgeCount[k]++;
            }
        }

        int Count = 0;
        for (int k=0; k<MAX_LINE_COUNT; k++) {
            if (Count>LineId) break;
            if (HFindEdgeCount[k]<Parameter.MinEdgeCount) continue;
            if (Count>=MAX_LINE_COUNT) continue;
            double dist_th  = Parameter.FitDistance;
            int max_iter = HFindEdgeCount[k]/3;
            int num_th   = 1;
            int mid = HFindEdgeCount[k]/2;

            bool IsFind = OnLineFitRANSAC_2(true, &HFindEdge[k][0], HFindEdgeCount[k], dist_th, max_iter, Parameter.MinEdgeCount);
            if (!IsFind) continue;

            float CuAngle = Math.GetDegree(P0.x, P0.y, P1.x, P1.y);
            if (Parameter.Angle) {
                if (fabs(CuAngle)>Parameter.Angle) {
                    continue;
                }
            }

            HFindLine[Count].P1.x = P0.x;
            HFindLine[Count].P1.y = P0.y;
            HFindLine[Count].P2.x = P1.x;
            HFindLine[Count].P2.y = P1.y;
            Count++;
            HFindLineCount = Count;
        }
    }

    else {
        for (int k=0; k<MAX_LINE_COUNT; k++) {
            P0.x = 0;
            P0.y = 0;
            P1.x = 0;
            P1.y = 0;
            VFindLine[k].P1.x = 0;
            VFindLine[k].P1.y = 0;
            VFindLine[k].P2.x = 0;
            VFindLine[k].P2.y = 0;
            VFindEdgeCount[k] = 0;
        }

        for (int x=IpRect.x; x<IpRect.x+IpRect.w; x+=xIncrement) {
            SP.x =(float)x;
            EP.x =(float)x;
            SP.y =(float)IpRect.y;
            EP.y =(float)(IpRect.y+IpRect.h);

            EPointGauge1.SetCenterXY(SP.x+(EP.x-SP.x)/2, SP.y+(EP.y-SP.y)/2);
            EPointGauge1.SetTolerance((float)IpRect.h/2);
            //EPointGauge1.SetTolerance(IpRect.h-2);
            EPointGauge1.SetToleranceAngle(FwdDirection ? 90.0f : 270.0f);
            EPointGauge1.Measure(GImage);

            int VFindCount = EPointGauge1.GetNumMeasuredPoints();
            //if (VFindCount>Parameter.MaxEdges) continue;

            for (int k=0; k<VFindCount; k++) {
                int n = VFindEdgeCount[k];
                if (k>=MAX_LINE_COUNT) continue;
                if (n>=MAX_EDGE_COUNT) continue;
                VFindEdge[k][n].x = EPointGauge1.GetMeasuredPoint(k).GetX();
                VFindEdge[k][n].y = EPointGauge1.GetMeasuredPoint(k).GetY();
                VFindEdgeCount[k]++;
            }
        }

        int Count = 0;
        for (int k=0; k<MAX_LINE_COUNT; k++) {
            if (Count>LineId) break;
            if (VFindEdgeCount[k]<Parameter.MinEdgeCount) continue;
            if (Count>=MAX_LINE_COUNT) continue;
            double dist_th  = Parameter.FitDistance;
            int max_iter = VFindEdgeCount[k]/3;
            int num_th   = 1;
            int mid = VFindEdgeCount[k]/2;

            bool IsFind = OnLineFitRANSAC_2(false, &VFindEdge[k][0], VFindEdgeCount[k], dist_th, max_iter, Parameter.MinEdgeCount);
            if (!IsFind) continue;

            float CuAngle = Math.GetDegree(P0.x, P0.y, P1.x, P1.y);
            if (Parameter.Angle) {
                if (fabs(CuAngle)>Parameter.Angle) {
                    continue;
                }
            }

            VFindLine[Count].P1.x = P0.x;
            VFindLine[Count].P1.y = P0.y;
            VFindLine[Count].P2.x = P1.x;
            VFindLine[Count].P2.y = P1.y;
            Count++;
            VFindLineCount = Count;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
int CLine::OnFindExecuteAll(bool IsHorEdge, bool FwdDirection, int LineId)
{
	//ROI영역 미사용, 전체 이미지 자동으로 라인을 찾을경우 
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CLine GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CLine GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CLine GImage Height Size"    )); return false; }

	HRect IpRect;

	IpRect.x = GImage->GetOrgX();
	IpRect.y = GImage->GetOrgY();
	IpRect.w = GImage->GetWidth();
	IpRect.h = GImage->GetHeight();

	if (IpRect.x<0) return false;
	if (IpRect.y<0) return false;
	if (IpRect.w<0) return false;
	if (IpRect.h<0) return false;

	if (Parameter.Increment<8) Parameter.Increment = 8;
	Parameter.MaxEdges = LineId+1;
	if (Parameter.MaxEdges>MAX_LINE_COUNT) Parameter.MaxEdges = MAX_LINE_COUNT;

	EPointGauge1.SetDragable(true);
	EPointGauge1.SetResizable(true);
	EPointGauge1.SetRotatable(true);

	if (Parameter.Thickness<1) Parameter.Thickness = 1;
	if (Parameter.MinAmplitude==0 && Parameter.Threshold==0) {
		Parameter.MinAmplitude = 1;
		Parameter.Threshold = 1;
		Parameter.Thickness = 20;
	}

#ifdef WORLD_SHAPE
	EWorldShape1.SetSensorSize(Width, Height);
	EWorldShape1.Process(Image, true);
	EPointGauge1.Attach(&EWorldShape1);
#endif

	EPointGauge1.SetMinAmplitude(Parameter.MinAmplitude);
	EPointGauge1.SetMinArea     (Parameter.MinArea     );

	EPointGauge1.SetThreshold   (Parameter.Threshold  );
	EPointGauge1.SetThickness   (Parameter.Thickness  );
	EPointGauge1.SetSmoothing   (Parameter.Smoothing  );

	EPointGauge1.SetTransitionType(ETransitionType_Wb);
	if      (Parameter.EdgeType==0) EPointGauge1.SetTransitionType(ETransitionType_Bw    );
	else if (Parameter.EdgeType==1) EPointGauge1.SetTransitionType(ETransitionType_Wb    );
	else if (Parameter.EdgeType==2) EPointGauge1.SetTransitionType(ETransitionType_BwOrWb);
	else if (Parameter.EdgeType==3) EPointGauge1.SetTransitionType(ETransitionType_Bwb);
	else if (Parameter.EdgeType==4) EPointGauge1.SetTransitionType(ETransitionType_Wbw);


	const int xIncrement = Parameter.Increment;
	const int yIncrement = Parameter.Increment;

	P0.SetFPoint(0, 0);	P1.SetFPoint(0, 0);

	TFPoint SP;
	TFPoint EP;

	if (Parameter.MinEdgeCount<5  ) Parameter.MinEdgeCount = 5;
	if (Parameter.FitDistance <0.6) Parameter.FitDistance  = 1.0;

#ifndef WORLD_SHAPE
	EROIBW8 roi;
	roi.SetPlacement(0, 0, IpRect.w, IpRect.h);
	roi.Attach(GImage);
#endif

	if (IsHorEdge) 
	{
		for (int k=0; k<MAX_LINE_COUNT; k++) {
			HFindLine[k].P1.SetFPoint(0, 0);	HFindLine[k].P2.SetFPoint(0, 0);	HFindEdgeCount[k] = 0;
		}

		for (int y=IpRect.y; y<IpRect.y+IpRect.h; y+=yIncrement) {
			SP.x = (float)IpRect.x;
			EP.x = (float)(IpRect.x+IpRect.w);
			SP.y = (float)y;
			EP.y = (float)y;

			EPointGauge1.SetTolerance(IpRect.w/4.f);
			EPointGauge1.SetToleranceAngle(FwdDirection ? 0.f : 180.f);

			if(!FwdDirection){
				if(SP.x+ IpRect.w/4.f < Width) EPointGauge1.SetCenterXY(SP.x+IpRect.w/4.f, SP.y+(EP.y-SP.y)/2.f);
			}
			else  {
				if(EP.x- IpRect.w/4.f > 0   ) EPointGauge1.SetCenterXY(EP.x-IpRect.w/4.f, SP.y+(EP.y-SP.y)/2.f);
			}
#ifdef WORLD_SHAPE
			EPointGauge1.Measure(GImage);
#else
			EPointGauge1.Measure(&roi);
#endif
			int HFindCount = EPointGauge1.GetNumMeasuredPoints();
			//if (HFindCount>Parameter.MaxEdges) continue;

			for (int k=0; k<HFindCount; k++) {
				int n = HFindEdgeCount[k];
				if (k>=MAX_LINE_COUNT) continue;
				if (n>=MAX_EDGE_COUNT) continue;

				HFindEdge[k][n].x = EPointGauge1.GetMeasuredPoint(k).GetX();
				HFindEdge[k][n].y = EPointGauge1.GetMeasuredPoint(k).GetY();
				HFindEdgeCount[k]++;
			}
		}

		int Count = 0;
		for (int k=0; k<MAX_LINE_COUNT; k++) {
			if (Count>LineId) break;
			if (HFindEdgeCount[k]<Parameter.MinEdgeCount) continue;
			if (Count>=MAX_LINE_COUNT) continue;
			double dist_th  = Parameter.FitDistance;
			int max_iter = HFindEdgeCount[k]/3;
			int num_th   = 1;
			int mid = HFindEdgeCount[k]/2;

			bool IsFind = OnLineFitRANSAC_2(true, &HFindEdge[k][0], HFindEdgeCount[k], dist_th, max_iter, Parameter.MinEdgeCount);
			if (!IsFind) continue;

			float CuAngle = Math.GetDegree(P0.x, P0.y, P1.x, P1.y);
			if (Parameter.Angle) {
				if (fabs(CuAngle)>Parameter.Angle) {
					continue;
				}
			}

			HFindLine[Count].P1.x = P0.x;
			HFindLine[Count].P1.y = P0.y;
			HFindLine[Count].P2.x = P1.x;
			HFindLine[Count].P2.y = P1.y;
			Count++;
			HFindLineCount = Count;
		}
	}
	else 
	{
		for (int k=0; k<MAX_LINE_COUNT; k++) {
			VFindLine[k].P1.SetFPoint(0,0);		VFindLine[k].P2.SetFPoint(0,0);		VFindEdgeCount[k] = 0;
		}

		for (int x=IpRect.x; x<IpRect.x+IpRect.w; x+=xIncrement) {
			SP.x =(float)x;
			EP.x =(float)x;
			SP.y =(float)IpRect.y;
			EP.y =(float)(IpRect.y+IpRect.h);

			EPointGauge1.SetTolerance(IpRect.h/4.f);
			EPointGauge1.SetToleranceAngle(FwdDirection ? 90.f : 270.f);

			if(!FwdDirection) {
				if(SP.y + IpRect.h/4.f < Height) EPointGauge1.SetCenterXY(SP.x+(EP.x-SP.x)/2.f, SP.y+ IpRect.h/4.f);
			}
			else  {
				if(EP.y- IpRect.h/4.f > 0     ) EPointGauge1.SetCenterXY(SP.x+(EP.x-SP.x)/2.f, EP.y-IpRect.h/4.f);
			}
#ifdef WORLD_SHAPE
			EPointGauge1.Measure(GImage);
#else
			EPointGauge1.Measure(&roi);
#endif
			int VFindCount = EPointGauge1.GetNumMeasuredPoints();
			//if (VFindCount>Parameter.MaxEdges) continue;

			for (int k=0; k<VFindCount; k++) {
				int n = VFindEdgeCount[k];
				if (k>=MAX_LINE_COUNT) continue;
				if (n>=MAX_EDGE_COUNT) continue;
				VFindEdge[k][n].x = EPointGauge1.GetMeasuredPoint(k).GetX();
				VFindEdge[k][n].y = EPointGauge1.GetMeasuredPoint(k).GetY();
				VFindEdgeCount[k]++;
			}
		}

		int Count = 0;
		for (int k=0; k<MAX_LINE_COUNT; k++) {
			if (Count>LineId) break;
			if (VFindEdgeCount[k]<Parameter.MinEdgeCount) continue;
			if (Count>=MAX_LINE_COUNT) continue;
			double dist_th  = Parameter.FitDistance;
			int max_iter = VFindEdgeCount[k]/3;
			int num_th   = 1;
			int mid = VFindEdgeCount[k]/2;

			bool IsFind = OnLineFitRANSAC_2(false, &VFindEdge[k][0], VFindEdgeCount[k], dist_th, max_iter, Parameter.MinEdgeCount);
			if (!IsFind) continue;

			float CuAngle = Math.GetDegree(P0.x, P0.y, P1.x, P1.y);
			if (Parameter.Angle) {
				if (fabs(CuAngle)>Parameter.Angle) {
					continue;
				}
			}

			VFindLine[Count].P1.x = P0.x;
			VFindLine[Count].P1.y = P0.y;
			VFindLine[Count].P2.x = P1.x;
			VFindLine[Count].P2.y = P1.y;
			Count++;
			VFindLineCount = Count;
		}
	}

	return true;
}
//------------------------------------------------------------------------------
int CLine::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CLine GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CLine GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CLine GImage Height Size"    )); return false; }

	HFindLineCount = 0;
    VFindLineCount = 0;
    Result.IsFindLeftLine=0;
    Result.IsFindTopLine =0;
    Result.IsFindRightLine =0;
    Result.IsFindBottomLine=0;
    Result.Angle = 0;
    Result.CP .x=0; Result.CP .y=0;
    Result.LT .x=0; Result.LT .y=0;
    Result.TR .x=0; Result.TR .y=0;
    Result.RB .x=0; Result.RB .y=0;
    Result.LB .x=0; Result.LB .y=0;
    Result.LP1.x=0; Result.LP1.y=0;
    Result.LP2.x=0; Result.LP2.y=0;
    Result.TP1.x=0; Result.TP1.y=0;
    Result.TP2.x=0; Result.TP2.y=0;
    Result.RP1.x=0; Result.RP1.y=0;
    Result.RP2.x=0; Result.RP2.y=0;
    Result.BP1.x=0; Result.BP1.y=0;
    Result.BP2.x=0; Result.BP2.y=0;

    int Index = 0;
    double P1x = 0;
    double P1y = 0;
    double P2x = 0;
    double P2y = 0;

    int P[9] = {0,};

    //HRect __IpRect;

    //__IpRect.x = IpRect.x+10;
    //__IpRect.y = IpRect.y+10;
    //__IpRect.w = IpRect.w-20;
    //__IpRect.h = IpRect.h-20;

    if (Parameter.LeftLineId  >=MAX_LINE_COUNT) return false;
    if (Parameter.TopLineId   >=MAX_LINE_COUNT) return false;
    if (Parameter.RightLineId >=MAX_LINE_COUNT) return false;
    if (Parameter.BottomLineId>=MAX_LINE_COUNT) return false;

    int LeftLineId   = Parameter.LeftLineId  ;
    int TopLineId    = Parameter.TopLineId   ;
    int RightLineId  = Parameter.RightLineId ;
    int BottomLineId = Parameter.BottomLineId;

	// 4개의 라인을 전부 안쪽에서 바깥방향으로 검출 할때
	if (Parameter.OpFindTopLine && Parameter.OpFindRightLine && Parameter.OpFindBottomLine && Parameter.OpFindLeftLine){
		Index =8; IsInOut= true ;
    }

	// Gray 영상 상태가 좋지 않을경우 BinaryImg 변환해서 검사진행함 
	if(Parameter.OpBinary){
		BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
		EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage, &BinaryImg);
		EasyImage::Threshold(GImage, &BinaryImg,Parameter.Threshold);
	}


    switch (Index) {
        case 0 :
                 Index++;

        case 1 :
			     /*
                 if (Parameter.OpBinary) {
                     //memcpy((BYTE *)MAddress, (BYTE *)IAddress, Width*Height);

                     #define MElement(x, y)  *((BYTE *)MAddress + x + Width * y)
                     #define IElement(x, y)  *((BYTE *)IAddress + x + Width * y)

                     for (int x=0; x<Width ; x++) {
                     for (int y=0; y<Height; y++) {
                         MElement(x, y) = (IElement(x, y)>=Parameter.ThHold) ? 255 : 0;
                         MElement(x, y) = (IElement(x, y)>=Parameter.ThHold) ? 255 : 0;
                     }}
                 }
                 else {
                     memcpy((BYTE *)MAddress, (BYTE *)IAddress, Width*Height);
                 }
				 */
				 IsInOut= false;
				 Index++ ; 

        case 2 :// Out -> In Search Line 
                 if (Parameter.OpFindLeftLine) {
                     OnFindExecute(true, true, LeftLineId);
                     Result.LAG   = Math.GetDegree(HFindLine[LeftLineId].P1.x, HFindLine[LeftLineId].P1.y, HFindLine[LeftLineId].P2.x, HFindLine[LeftLineId].P2.y);
                     Result.LP1.x = HFindLine[LeftLineId].P1.x;
                     Result.LP1.y = HFindLine[LeftLineId].P1.y;
                     Result.LP2.x = HFindLine[LeftLineId].P2.x;
                     Result.LP2.y = HFindLine[LeftLineId].P2.y;
                 }
                 Index++;

        case 3 :// Out -> In Search Line
                 if (Parameter.OpFindRightLine) {
                     OnFindExecute(true, false, RightLineId);
                     Result.RAG   = Math.GetDegree(HFindLine[RightLineId].P1.x, HFindLine[RightLineId].P1.y, HFindLine[RightLineId].P2.x, HFindLine[RightLineId].P2.y);;
                     Result.RP1.x = HFindLine[RightLineId].P1.x;
                     Result.RP1.y = HFindLine[RightLineId].P1.y;
                     Result.RP2.x = HFindLine[RightLineId].P2.x;
                     Result.RP2.y = HFindLine[RightLineId].P2.y;
                 }
                 Index++;

        case 4 :// Out -> In Search Line
                 if (Parameter.OpFindTopLine) {
                     OnFindExecute(false, true, TopLineId);
                     Result.TAG   = Math.GetDegree(VFindLine[TopLineId].P1.x, VFindLine[TopLineId].P1.y, VFindLine[TopLineId].P2.x, VFindLine[TopLineId].P2.y);
                     Result.TP1.x = VFindLine[TopLineId].P1.x;
                     Result.TP1.y = VFindLine[TopLineId].P1.y;
                     Result.TP2.x = VFindLine[TopLineId].P2.x;
                     Result.TP2.y = VFindLine[TopLineId].P2.y;
                 }
                 Index++;

        case 5 :// Out -> In Search Line
                 if (Parameter.OpFindBottomLine) {
                     OnFindExecute(false, false, BottomLineId);
                     Result.BAG   = Math.GetDegree(VFindLine[BottomLineId].P1.x, VFindLine[BottomLineId].P1.y, VFindLine[BottomLineId].P2.x, VFindLine[BottomLineId].P2.y);
                     Result.BP1.x = VFindLine[BottomLineId].P1.x;
                     Result.BP1.y = VFindLine[BottomLineId].P1.y;
                     Result.BP2.x = VFindLine[BottomLineId].P2.x;
                     Result.BP2.y = VFindLine[BottomLineId].P2.y;
                 }
                 Index++;

        //Line Intersection
        case 6 :
                 if ((Parameter.OpFindTopLine + Parameter.OpFindRightLine + Parameter.OpFindBottomLine + Parameter.OpFindLeftLine)==2) {
                     //if      (Parameter.OpFindRightLine  && Parameter.OpFindBottomLine) { Result.CP = Math.OnIntersectPoint(Result.RP1, Result.RP2, Result.BP1, Result.BP2); Result.Angle = Result.RAG;}
                     //else if (Parameter.OpFindBottomLine && Parameter.OpFindLeftLine  ) { Result.CP = Math.OnIntersectPoint(Result.LP1, Result.LP2, Result.BP1, Result.BP2); Result.Angle = Result.BAG;}
                     //else if (Parameter.OpFindLeftLine   && Parameter.OpFindTopLine   ) { Result.CP = Math.OnIntersectPoint(Result.LP1, Result.LP2, Result.TP1, Result.TP2); Result.Angle = Result.TAG;}
                     //else if (Parameter.OpFindTopLine    && Parameter.OpFindRightLine ) { Result.CP = Math.OnIntersectPoint(Result.TP1, Result.TP2, Result.RP1, Result.RP2); Result.Angle = Result.TAG;}
                     if      (Parameter.OpFindRightLine  && Parameter.OpFindBottomLine) { Result.CP = Math.OnIntersect(Result.RP1, Result.RP2, Result.BP1, Result.BP2); Result.Angle = 90.0 - Result.RAG - Result.BAG;}
                     else if (Parameter.OpFindBottomLine && Parameter.OpFindLeftLine  ) { Result.CP = Math.OnIntersect(Result.LP1, Result.LP2, Result.BP1, Result.BP2); Result.Angle = 90.0 - Result.BAG - Result.LAG;}
                     else if (Parameter.OpFindLeftLine   && Parameter.OpFindTopLine   ) { Result.CP = Math.OnIntersect(Result.LP1, Result.LP2, Result.TP1, Result.TP2); Result.Angle = 90.0 - Result.TAG - Result.LAG;}
                     else if (Parameter.OpFindTopLine    && Parameter.OpFindRightLine ) { Result.CP = Math.OnIntersect(Result.TP1, Result.TP2, Result.RP1, Result.RP2); Result.Angle = 90.0 - Result.TAG - Result.RAG;}
                 }

				 /* 검출 기준은 단순하게 교차점 좌표 산출 목적 
                 if ((Parameter.OpFindTopLine + Parameter.OpFindRightLine + Parameter.OpFindBottomLine + Parameter.OpFindLeftLine)==2) {
                     if      (Parameter.OpFindRightLine  && Parameter.OpFindBottomLine && Parameter.Angle) { if (fabs(Result.Angle)>Parameter.Angle) { Result.CP.x = 0; Result.CP.y = 0;} }
                     else if (Parameter.OpFindBottomLine && Parameter.OpFindLeftLine   && Parameter.Angle) { if (fabs(Result.Angle)>Parameter.Angle) { Result.CP.x = 0; Result.CP.y = 0;} }
                     else if (Parameter.OpFindLeftLine   && Parameter.OpFindTopLine    && Parameter.Angle) { if (fabs(Result.Angle)>Parameter.Angle) { Result.CP.x = 0; Result.CP.y = 0;} }
                     else if (Parameter.OpFindTopLine    && Parameter.OpFindRightLine  && Parameter.Angle) { if (fabs(Result.Angle)>Parameter.Angle) { Result.CP.x = 0; Result.CP.y = 0;} }
                 }
				 */

                 /*
                 if ((Parameter.OpFindTopLine + Parameter.OpFindRightLine + Parameter.OpFindBottomLine + Parameter.OpFindLeftLine)==2) {
                     if (Parameter.Angle) {
                         if (fabs(Result.Angle)>Parameter.Angle) {
                             Result.CP.x = 0;
                             Result.CP.y = 0;
                         }
                     }
                 }
                 */
                 Index++;

        //One Line
        case 7 :
			     // 1개의 옵션이 설정되어 있을경우 검사 시퀀스 
			/*   // 무조건 2개의 검사 옵션이 설정되어 있는 상태에서 검사를 진행한다 
                 if ((Parameter.OpFindTopLine + Parameter.OpFindRightLine + Parameter.OpFindBottomLine + Parameter.OpFindLeftLine)==1) {
                     TFPoint HP1(0, Height/2.0); TFPoint HP2(Width, Height/2.0);
                     TFPoint VP1(Width/2.0, 0); TFPoint VP2(Width/2.0, Height); //세로

                     if      (Parameter.OpFindLeftLine  ) Result.CP = Math.OnIntersect(Result.LP1, Result.LP2, HP1, HP2);
                     else if (Parameter.OpFindTopLine   ) Result.CP = Math.OnIntersect(Result.TP1, Result.TP2, VP1, VP2);
                     else if (Parameter.OpFindRightLine ) Result.CP = Math.OnIntersect(Result.RP1, Result.RP2, HP1, HP2);
                     else if (Parameter.OpFindBottomLine) Result.CP = Math.OnIntersect(Result.BP1, Result.BP2, VP1, VP2);

                     if      (Parameter.OpFindLeftLine  ) Result.Angle = Result.LAG;
                     else if (Parameter.OpFindTopLine   ) Result.Angle = Result.TAG;
                     else if (Parameter.OpFindRightLine ) Result.Angle = Result.RAG;
                     else if (Parameter.OpFindBottomLine) Result.Angle = Result.BAG;

                     if ((Parameter.OpFindTopLine + Parameter.OpFindRightLine + Parameter.OpFindBottomLine + Parameter.OpFindLeftLine)==1) {
                         if (Parameter.Angle) {
                             if (fabs(Result.Angle)>Parameter.Angle) {
                                 Result.CP.x = 0;
                                 Result.CP.y = 0;
                             }
                         }
                     }
                 }
				 */

                 Index++;

        //Rectangle Intersection
        case 8 :
			     // 4개의 옵션이 설정되어 있을경우 시퀀스 전부 안쪽에서 바깥쪽 라인 검출  (In -> Out)
                 if (Parameter.OpFindTopLine && Parameter.OpFindRightLine && Parameter.OpFindBottomLine && Parameter.OpFindLeftLine) {
					 if (Parameter.OpFindLeftLine) {
						 OnFindExecuteAll(true, false, LeftLineId);
						 Result.LAG   = Math.GetDegree(HFindLine[LeftLineId].P1.x, HFindLine[LeftLineId].P1.y, HFindLine[LeftLineId].P2.x, HFindLine[LeftLineId].P2.y);
						 Result.LP1.x = HFindLine[LeftLineId].P1.x;
						 Result.LP1.y = HFindLine[LeftLineId].P1.y;
						 Result.LP2.x = HFindLine[LeftLineId].P2.x;
						 Result.LP2.y = HFindLine[LeftLineId].P2.y;
					 }
					 if (Parameter.OpFindRightLine) {
						 OnFindExecuteAll(true, true, RightLineId);
						 Result.RAG   = Math.GetDegree(HFindLine[RightLineId].P1.x, HFindLine[RightLineId].P1.y, HFindLine[RightLineId].P2.x, HFindLine[RightLineId].P2.y);;
						 Result.RP1.x = HFindLine[RightLineId].P1.x;
						 Result.RP1.y = HFindLine[RightLineId].P1.y;
						 Result.RP2.x = HFindLine[RightLineId].P2.x;
						 Result.RP2.y = HFindLine[RightLineId].P2.y;
					 }
					 if (Parameter.OpFindTopLine) {
						 OnFindExecuteAll(false, false, TopLineId);
						 Result.TAG   = Math.GetDegree(VFindLine[TopLineId].P1.x, VFindLine[TopLineId].P1.y, VFindLine[TopLineId].P2.x, VFindLine[TopLineId].P2.y);
						 Result.TP1.x = VFindLine[TopLineId].P1.x;
						 Result.TP1.y = VFindLine[TopLineId].P1.y;
						 Result.TP2.x = VFindLine[TopLineId].P2.x;
						 Result.TP2.y = VFindLine[TopLineId].P2.y;
					 }
					 if (Parameter.OpFindBottomLine) {
						 OnFindExecuteAll(false, true, BottomLineId);
						 Result.BAG   = Math.GetDegree(VFindLine[BottomLineId].P1.x, VFindLine[BottomLineId].P1.y, VFindLine[BottomLineId].P2.x, VFindLine[BottomLineId].P2.y);
						 Result.BP1.x = VFindLine[BottomLineId].P1.x;
						 Result.BP1.y = VFindLine[BottomLineId].P1.y;
						 Result.BP2.x = VFindLine[BottomLineId].P2.x;
						 Result.BP2.y = VFindLine[BottomLineId].P2.y;
					 }
                 }
				 Index++;

		case 9 :
			// 4개의 옵션이 설정되어 있을경우 시퀀스 
			if (Parameter.OpFindTopLine && Parameter.OpFindRightLine && Parameter.OpFindBottomLine && Parameter.OpFindLeftLine) {
				Result.LT = Math.OnIntersect(Result.LP1, Result.LP2, Result.TP1, Result.TP2);
				Result.TR = Math.OnIntersect(Result.TP1, Result.TP2, Result.RP1, Result.RP2);
				Result.RB = Math.OnIntersect(Result.RP1, Result.RP2, Result.BP1, Result.BP2);
				Result.LB = Math.OnIntersect(Result.LP1, Result.LP2, Result.BP1, Result.BP2);

				float lx = Result.LT.x<Result.LB.x ? Result.LT.x:Result.LB.x;
				float rx = Result.TR.x>Result.RB.x ? Result.TR.x:Result.RB.x;
				Result.CP.x = (float)(lx + (rx - lx)/2.0);

				float ty = Result.LT.y<Result.TR.y ? Result.LT.y:Result.TR.y;
				float by = Result.LB.y>Result.RB.y ? Result.LB.y:Result.RB.y;
				Result.CP.y = (float)(ty + (by - ty)/2.0);
			}
			Index=0;
			break;

    }

    return true;
}
//------------------------------------------------------------------------------

void CLine::OnLoad(bool IsLoad, CString Folder, int ItemId)
{
	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;
	CString Path="";
	CString PatternPath="";
	CString FindImgPath="";
	CString ItemNum="";


	ItemNum.Format(L"%d", ItemId);

	if (Folder.Trim()=="") return;

	Path = Folder +"\\Line" + ItemNum + L".ini";

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

    if (Path.Trim()=="") Path = PvPath;
    PvPath = Path;
    if (Path=="") return;

    Ini.Load(IsLoad, Path, "Parameter", "OpFindTopLine   "  , &Parameter.OpFindTopLine   );
    Ini.Load(IsLoad, Path, "Parameter", "OpFindRightLine "  , &Parameter.OpFindRightLine );
    Ini.Load(IsLoad, Path, "Parameter", "OpFindBottomLine"  , &Parameter.OpFindBottomLine);
    Ini.Load(IsLoad, Path, "Parameter", "OpFindLeftLine  "  , &Parameter.OpFindLeftLine  );
    Ini.Load(IsLoad, Path, "Parameter", "OpBinary        "  , &Parameter.OpBinary        );
    Ini.Load(IsLoad, Path, "Parameter", "ThHold          "  , &Parameter.ThHold          );
    Ini.Load(IsLoad, Path, "Parameter", "MinEdgeCount    "  , &Parameter.MinEdgeCount    );
    Ini.Load(IsLoad, Path, "Parameter", "Angle           "  , &Parameter.Angle           );
    Ini.Load(IsLoad, Path, "Parameter", "MaxEdges        "  , &Parameter.MaxEdges        );
    Ini.Load(IsLoad, Path, "Parameter", "EdgeType        "  , &Parameter.EdgeType        );
    Ini.Load(IsLoad, Path, "Parameter", "Increment       "  , &Parameter.Increment       );
    Ini.Load(IsLoad, Path, "Parameter", "FitDistance     "  , &Parameter.FitDistance     );

    Ini.Load(IsLoad, Path, "Parameter", "MinAmplitude"  , &Parameter.MinAmplitude);
    Ini.Load(IsLoad, Path, "Parameter", "MinArea     "  , &Parameter.MinArea     );
    Ini.Load(IsLoad, Path, "Parameter", "Threshold   "  , &Parameter.Threshold   );
    Ini.Load(IsLoad, Path, "Parameter", "Thickness   "  , &Parameter.Thickness   );
    Ini.Load(IsLoad, Path, "Parameter", "Smoothing   "  , &Parameter.Smoothing   );

    Ini.Load(IsLoad, Path, "Parameter", "JDThreshold   "  , &Parameter.JDThreshold   );
    Ini.Load(IsLoad, Path, "Parameter", "JDThickness   "  , &Parameter.JDThickness   );
    Ini.Load(IsLoad, Path, "Parameter", "JDSmoothing   "  , &Parameter.JDSmoothing   );

    Ini.Load(IsLoad, Path, "Parameter", "LeftLineId  "  , &Parameter.LeftLineId  );
    Ini.Load(IsLoad, Path, "Parameter", "TopLineId   "  , &Parameter.TopLineId   );
    Ini.Load(IsLoad, Path, "Parameter", "RightLineId "  , &Parameter.RightLineId );
    Ini.Load(IsLoad, Path, "Parameter", "BottomLineId"  , &Parameter.BottomLineId);

    if (!IsLoad) {
        HoRect.x = HRoi.GetOrgX  ();
        HoRect.y = HRoi.GetOrgY  ();
        HoRect.w = HRoi.GetWidth ();
        HoRect.h = HRoi.GetHeight();
        VtRect.x = VRoi.GetOrgX  ();
        VtRect.y = VRoi.GetOrgY  ();
        VtRect.w = VRoi.GetWidth ();
        VtRect.h = VRoi.GetHeight();
    }

    Ini.Load(IsLoad, Path, "HoRect", "x", &HoRect.x);
    Ini.Load(IsLoad, Path, "HoRect", "y", &HoRect.y);
    Ini.Load(IsLoad, Path, "HoRect", "w", &HoRect.w);
    Ini.Load(IsLoad, Path, "HoRect", "h", &HoRect.h);
    Ini.Load(IsLoad, Path, "VtRect", "x", &VtRect.x);
    Ini.Load(IsLoad, Path, "VtRect", "y", &VtRect.y);
    Ini.Load(IsLoad, Path, "VtRect", "w", &VtRect.w);
    Ini.Load(IsLoad, Path, "VtRect", "h", &VtRect.h);

    if (IsLoad) {
        HRoi.SetOrgX  (HoRect.x);
        HRoi.SetOrgY  (HoRect.y);
        HRoi.SetWidth (HoRect.w);
        HRoi.SetHeight(HoRect.h);
        VRoi.SetOrgX  (VtRect.x);
        VRoi.SetOrgY  (VtRect.y);
        VRoi.SetWidth (VtRect.w);
        VRoi.SetHeight(VtRect.h);
    }

    if (HoRect.x<1) { HoRect.x = Width/2; HoRect.y = Height/2; HoRect.w = 200; HoRect.h = 200; }
    if (HoRect.y<1) { HoRect.x = Width/2; HoRect.y = Height/2; HoRect.w = 200; HoRect.h = 200; }
    if (HoRect.w<1) { HoRect.x = Width/2; HoRect.y = Height/2; HoRect.w = 200; HoRect.h = 200; }
    if (HoRect.h<1) { HoRect.x = Width/2; HoRect.y = Height/2; HoRect.w = 200; HoRect.h = 200; }
    if (VtRect.x<1) { VtRect.x = Width/2; VtRect.y = Height/2; VtRect.w = 100; VtRect.h = 100; }
    if (VtRect.y<1) { VtRect.x = Width/2; VtRect.y = Height/2; VtRect.w = 100; VtRect.h = 100; }
    if (VtRect.w<1) { VtRect.x = Width/2; VtRect.y = Height/2; VtRect.w = 100; VtRect.h = 100; }
    if (VtRect.h<1) { VtRect.x = Width/2; VtRect.y = Height/2; VtRect.w = 100; VtRect.h = 100; }
}
//------------------------------------------------------------------------------

/*
void CLine::OnPaint(TPanel *PTable, TColor RColor)
{
    OnPaint(PTable->Handle, RColor);
}
*/
//------------------------------------------------------------------------------
/*
void CLine::OnPaint(CWnd *target,TFPoint CP, COLORREF Color,EImageBW8 *lpImage)
{
	if (target==NULL) return;
	
    CClientDC dc(target);
	HDC hdc = dc.m_hDC;
	if(!Image->GetImagePtr()){ 
        ::MessageBox(NULL,"Current Inspection Image Error!","Error",MB_ICONERROR||MB_OK);
		return;
	}

	if(Image->GetImagePtr()){
	   int w = Image->GetWidth ();
	   int h = Image->GetHeight();
	   
	   if (w==0) return;
	   if (h==0) return;
	   
	   CRect Rect;
	   target->GetClientRect(&Rect);
	   
	   FHs = (float)Rect.Width() /(float)w;
	   FVs = (float)Rect.Height()/(float)h;
    }
	EPointGauge1.Draw(hdc,EDrawingMode_Actual, true);
}
*/
//-----------------------------------------------------------------------------------
void CLine::OnPaint(TDrawTool Tool,TFPoint CP, COLORREF Color,EImageBW8 *lpImage)
{
	if (Tool.Handle==NULL) return;
	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;
	
	int x1 = Width/2 ;
	int y1 =  0      ;
	int x2 = Width/2 ;
	int y2 = Height  ;
	int x3 =  0      ;
	int y3 = Height/2;
	int x4 = Width   ;
	int y4 = Height/2;

	Gdi->OnMove((int)(CP.x*Gdi->FHs+0.5), y1); //  시작점으로 이동 
	Gdi->OnLine((int)(CP.x*Gdi->FHs+0.5), y2); //  끝점으로 선그리기 
	
	Gdi->OnMove(x3, (int)(CP.y*Gdi->FVs+0.5)); // 시작점으로 이동 
	Gdi->OnLine(x4, (int)(CP.y*Gdi->FVs+0.5)); // 끝점으로 선그리기 

	delete Gdi;
}
//-----------------------------------------------------------------------------
void CLine::OnPaintRectangle(TDrawTool Tool, EImageBW8*lpImage)
{
	if (Tool.Handle==NULL) return;

	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

	Gdi->CPen.Style = 2;
	Gdi->CBrush.Style = BS_SOLID;

	// Draw 
	Gdi->OnMove((int)Result.LT.x, (int)Result.LT.y);
	Gdi->OnLine((int)Result.TR.x, (int)Result.TR.y);
	Gdi->OnLine((int)Result.RB.x, (int)Result.RB.y);
	Gdi->OnLine((int)Result.LB.x, (int)Result.LB.y);
	Gdi->OnLine((int)Result.LT.x, (int)Result.LT.y);

	delete Gdi;
}
//------------------------------------------------------------------------------

//Class RectLine
//------------------------------------------------------------------------------
CRectLine::CRectLine()
{
	SLLine = new CSingleLine;
	STLine = new CSingleLine;
	SRLine = new CSingleLine;
	SBLine = new CSingleLine;

	SLLine->SetOrientation(0);
	STLine->SetOrientation(1);
	SRLine->SetOrientation(2);
	SBLine->SetOrientation(3);

	DwIRoi = false;
	DwSingleLine = true;
	DwTwoPoint = false;
	DwFindRect = true;

	FRoiRange[0] = 20;
	FRoiRange[1] = 20;
	FRoiRange[2] = 20;
	FRoiRange[3] = 20;
	GImage       = NULL;
}

CRectLine::~CRectLine()
{
}

int CRectLine::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CRectLine GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CRectLine GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CRectLine GImage Height Size"    )); return false; }

	SLLine->GImage = GImage;
	STLine->GImage = GImage;
	SRLine->GImage = GImage;
	SBLine->GImage = GImage;

	int x = IRoi->GetOrgX  ();
	int y = IRoi->GetOrgY  ();
	int w = IRoi->GetWidth ();
	int h = IRoi->GetHeight();

	//SLLine->IRoi->SetOrgX  (x  ); STLine->IRoi->SetOrgX  (x  ); SRLine->IRoi->SetOrgX  (x+w/2); SBLine->IRoi->SetOrgX  (x    );
	//SLLine->IRoi->SetOrgY  (y  ); STLine->IRoi->SetOrgY  (y  ); SRLine->IRoi->SetOrgY  (y    ); SBLine->IRoi->SetOrgY  (y+h/2);
	//SLLine->IRoi->SetWidth (w/2); STLine->IRoi->SetWidth (w  ); SRLine->IRoi->SetWidth (w    ); SBLine->IRoi->SetWidth (w    );
	//SLLine->IRoi->SetHeight(h  ); STLine->IRoi->SetHeight(h/2); SRLine->IRoi->SetHeight(h    ); SBLine->IRoi->SetHeight(h    );


	SLLine->IRoi->SetOrgX  (x-FRoiRange[0]  );
	SLLine->IRoi->SetOrgY  (y               );
	SLLine->IRoi->SetWidth (  FRoiRange[0]*2);
	SLLine->IRoi->SetHeight(h               );

	LRect.l = SLLine->IRoi->GetOrgX  ();
	LRect.t = SLLine->IRoi->GetOrgY  ();
	LRect.r = LRect.l+SLLine->IRoi->GetWidth ();
	LRect.b = LRect.t+SLLine->IRoi->GetHeight();

	STLine->IRoi->SetOrgX  (x               );
	STLine->IRoi->SetOrgY  (y-FRoiRange[1]  );
	STLine->IRoi->SetWidth (w               );
	STLine->IRoi->SetHeight(  FRoiRange[1]*2);

	TRect.l = STLine->IRoi->GetOrgX  ();
	TRect.t = STLine->IRoi->GetOrgY  ();
	TRect.r = TRect.l+STLine->IRoi->GetWidth ();
	TRect.b = TRect.t+STLine->IRoi->GetHeight();

	SRLine->IRoi->SetOrgX  (x+w-FRoiRange[2]  );
	SRLine->IRoi->SetOrgY  (y                 );
	SRLine->IRoi->SetWidth (    FRoiRange[2]*2);
	SRLine->IRoi->SetHeight(h                 );

	RRect.l = SRLine->IRoi->GetOrgX  ();
	RRect.t = SRLine->IRoi->GetOrgY  ();
	RRect.r = RRect.l+SRLine->IRoi->GetWidth ();
	RRect.b = RRect.t+SRLine->IRoi->GetHeight();

	SBLine->IRoi->SetOrgX  (x               );
	SBLine->IRoi->SetOrgY  (y+h-FRoiRange[3]);
	SBLine->IRoi->SetWidth (w               );
	SBLine->IRoi->SetHeight(  FRoiRange[3]*2);

	BRect.l = SBLine->IRoi->GetOrgX  ();
	BRect.t = SBLine->IRoi->GetOrgY  ();
	BRect.r = BRect.l+SBLine->IRoi->GetWidth ();
	BRect.b = BRect.t+SBLine->IRoi->GetHeight();

	//LLine->SetOrientation(0);
	//TLine->SetOrientation(1);
	//RLine->SetOrientation(2);
	//BLine->SetOrientation(3);

	SLLine->FEdgeType = 0; //Black To White 
	STLine->FEdgeType = 0; //Black To White 
	SRLine->FEdgeType = 0; //Black To White 
	SBLine->FEdgeType = 0; //Black To White 

	int Index = 0;

	switch (Index) {
	case 0 :
		Index++;

	case 1 :
		SLLine->OnExecute();
		Line[0].FP0 = SLLine->GetResultP0();
		Line[0].FP1 = SLLine->GetResultP1();

		STLine->OnExecute();
		Line[1].FP0 = STLine->GetResultP0();
		Line[1].FP1 = STLine->GetResultP1();

		SRLine->OnExecute();
		Line[2].FP0 = SRLine->GetResultP0();
		Line[2].FP1 = SRLine->GetResultP1();

		SBLine->OnExecute();
		Line[3].FP0 = SBLine->GetResultP0();
		Line[3].FP1 = SBLine->GetResultP1();

		Index++;

	case 2 :
		FIPoint[0] = Math.OnIntersect(Line[0].FP0, Line[0].FP1, Line[1].FP0, Line[1].FP1);  //LT
		FIPoint[1] = Math.OnIntersect(Line[1].FP0, Line[1].FP1, Line[2].FP0, Line[2].FP1);  //TR
		FIPoint[2] = Math.OnIntersect(Line[2].FP0, Line[2].FP1, Line[3].FP0, Line[3].FP1);  //RB
		FIPoint[3] = Math.OnIntersect(Line[3].FP0, Line[3].FP1, Line[0].FP0, Line[0].FP1);  //LB
		FCPoint.x = FIPoint[0].x+(FIPoint[2].x-FIPoint[0].x)/2.0;
		FCPoint.y = FIPoint[0].y+(FIPoint[2].y-FIPoint[0].y)/2.0;
		Index=0;
		break;
	}
	return true;
}

float CRectLine::GetLength(TFPoint p1, TFPoint p2)
{
	return sqrt(pow(p1.x-p2.x, 2) + pow(p1.y-p2.y, 2));
}

void CRectLine::OnLoad(bool IsLoad, CString Folder)
{
	CFileUnit FileUnit;

	if (!FileUnit.ExistDir(Folder+"\\LLine")) FileUnit.CreateDir(Folder+"\\LLine");
	if (!FileUnit.ExistDir(Folder+"\\TLine")) FileUnit.CreateDir(Folder+"\\TLine");
	if (!FileUnit.ExistDir(Folder+"\\RLine")) FileUnit.CreateDir(Folder+"\\RLine");
	if (!FileUnit.ExistDir(Folder+"\\BLine")) FileUnit.CreateDir(Folder+"\\BLine");

	SLLine->OnLoad(IsLoad, Folder+"\\LLine");
	STLine->OnLoad(IsLoad, Folder+"\\TLine");
	SRLine->OnLoad(IsLoad, Folder+"\\RLine");
	SBLine->OnLoad(IsLoad, Folder+"\\BLine");

	CIFile Ini;

	if (Folder.Trim()=="") return;
	CString Path = Folder + "\\RectLine.ini";
	if (Path=="") return;

	Ini.Load(IsLoad, Path, "Parameter", "FRoiRange0"  , &FRoiRange[0]);
	Ini.Load(IsLoad, Path, "Parameter", "FRoiRange1"  , &FRoiRange[1]);
	Ini.Load(IsLoad, Path, "Parameter", "FRoiRange2"  , &FRoiRange[2]);
	Ini.Load(IsLoad, Path, "Parameter", "FRoiRange3"  , &FRoiRange[3]);

	// Roi Load & Save
	// -------------------------------------------------------------------------
	CString IRoiPath = Folder + "\\RectLine.ini";
	OnLoadRoi(IsLoad, IRoiPath, IRoi);
}

void CRectLine::OnPaint(TDrawTool Tool)
{
	if (Tool.Handle==NULL) return;

	//Roi's
	//--------------------------------------------------------------------------

	if (DwIRoi) {
		Tool.Color = AQUA;

// 		LRect.OnPaint(Tool);
// 		TRect.OnPaint(Tool);
// 		RRect.OnPaint(Tool);
// 		BRect.OnPaint(Tool);
	}

	//Result
	//--------------------------------------------------------------------------

	if (DwSingleLine) {
		SLLine->OnPaint(Tool);
		STLine->OnPaint(Tool);
		SRLine->OnPaint(Tool);
		SBLine->OnPaint(Tool);
	}

	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

	TPoint Point1, Point2;
	Gdi->CText.Color  = YELLOW_COLOR;
	Gdi->CFont.Height = 12;

	TFPoint SP, EP;

	Gdi->CPen.Color = Tool.Color;

	if (DwFindRect) {
		Gdi->OnMove(FIPoint[0].x, FIPoint[0].y);
		Gdi->OnLine(FIPoint[1].x, FIPoint[1].y);
		Gdi->OnLine(FIPoint[2].x, FIPoint[2].y);
		Gdi->OnLine(FIPoint[3].x, FIPoint[3].y);
		Gdi->OnLine(FIPoint[0].x, FIPoint[0].y);
	}

	if (DwTwoPoint) {
		Gdi->OnLinePlus(Line[0].FP0.x, Line[0].FP0.y, 10); Gdi->OnLinePlus(Line[0].FP1.x, Line[0].FP1.y, 10);
		Gdi->OnLinePlus(Line[1].FP0.x, Line[1].FP0.y, 10); Gdi->OnLinePlus(Line[1].FP1.x, Line[1].FP1.y, 10);
		Gdi->OnLinePlus(Line[2].FP0.x, Line[2].FP0.y, 10); Gdi->OnLinePlus(Line[2].FP1.x, Line[2].FP1.y, 10);
		Gdi->OnLinePlus(Line[3].FP0.x, Line[3].FP0.y, 10); Gdi->OnLinePlus(Line[3].FP1.x, Line[3].FP1.y, 10);
	}
	Gdi->CPen.Color = LIME_COLOR;
	Gdi->OnLinePlus(FCPoint.x, FCPoint.y, 30);

	delete Gdi;
}

//------------------------------------------------------------------------------

