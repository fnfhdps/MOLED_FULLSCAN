#include "stdafx.h"
#include "LxDoubleLine.h"

//---------------------------------------------------------------------------
//Class DoubleLine 
//---------------------------------------------------------------------------
CDoubleLine::CDoubleLine()
{
	FFstOrientation = orNorth;
	FSndOrientation = orSouth;
}
CDoubleLine::~CDoubleLine()
{
}
int CDoubleLine::OnExecute(TOrientaion FstOrientation, TOrientaion SndOrientation)
{
    FFstOrientation = FstOrientation;
    FSndOrientation = SndOrientation;
    OnExecute();
	return true;
}

int CDoubleLine::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check DouGImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check DouGImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check DouGImage Height Size"    )); return false; }

    int Index = 0;
    float d0=0, d1=0, d2=0, d3=0;

    switch (Index) {
        case 0 :
                 Index++;

        case 1 :
                 FMinAmplitude = 10;
                 FMinArea      =  0;
				 FThreshold    = 20;
				 FThickness    = 1 ;
				 FSmoothing    = 1 ;

                 //FFstOrientation = orNorth;
                 FOrientation = FFstOrientation;
                 CSingleLine::OnExecute();
                 Line[0].FP0 = CSingleLine::FP0;
                 Line[0].FP1 = CSingleLine::FP1;

				 FMinAmplitude = 10;
				 FMinArea      =  0;
				 FThreshold    = 10;
				 FThickness    = 1 ;
				 FSmoothing    = 1 ;

                 //FSndOrientation = orSouth;
                 FOrientation = FSndOrientation;
                 CSingleLine::OnExecute();
                 Line[1].FP0 = CSingleLine::FP0;
                 Line[1].FP1 = CSingleLine::FP1;

                 Index++;

        case 2 :
                 FIPoint = Math.OnIntersect(Line[0].FP0, Line[0].FP1, Line[1].FP0, Line[1].FP1);

                 FL0CP.x = (Line[0].FP0.x+Line[0].FP1.x)/2.0;
                 FL0CP.y = (Line[0].FP0.y+Line[0].FP1.y)/2.0;
                 FL1CP.x = (Line[1].FP0.x+Line[1].FP1.x)/2.0;
                 FL1CP.y = (Line[1].FP0.y+Line[1].FP1.y)/2.0;

                 FL0MP = Math.OnIntersect(Line[0].FP0, Line[0].FP1, FL1CP, Math.OnRotate(FL1CP, Line[1].FP1, 90));
                 FL1MP = Math.OnIntersect(Line[1].FP0, Line[1].FP1, FL0CP, Math.OnRotate(FL0CP, Line[0].FP1, 90));

                 d0 = GetLengthFrLineToPoint(Line[1].FP0, Line[1].FP1, Line[0].FP0);
                 d1 = GetLengthFrLineToPoint(Line[1].FP0, Line[1].FP1, Line[0].FP1);
                 d2 = GetLengthFrLineToPoint(Line[0].FP0, Line[0].FP1, Line[1].FP0);
                 d3 = GetLengthFrLineToPoint(Line[0].FP0, Line[0].FP1, Line[1].FP1);

                 FDistance = (d0+d1+d2+d3)/4.0;

                 Index=0;
                 break;

    }

    return true;
}

float CDoubleLine::GetLength(TFPoint p1, TFPoint p2)
{
    return sqrt(pow(p1.x-p2.x, 2) + pow(p1.y-p2.y, 2));
}

void CDoubleLine::OnLoad(bool IsLoad, CString Folder)
{
    CSingleLine::OnLoad(IsLoad, Folder);

	CIFile Ini;

    if (Folder.Trim()=="") return;
    CString Path = Folder + "\\DoubleLine.ini";
    if (Path=="") return;

    // Roi Load & Save
    // -------------------------------------------------------------------------
    CString IRoiPath = Folder + "\\DoubleLineIRoi.ini";
    OnLoadRoi(IsLoad, IRoiPath, IRoi);

    Ini.Load(IsLoad, Path, "Parameter", "FFstOrientation"  , &FFstOrientation);
    Ini.Load(IsLoad, Path, "Parameter", "FSndOrientation"  , &FSndOrientation);
}

void CDoubleLine::OnPaint(TDrawTool Tool)
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

    Gdi->CPen.Color = Tool.Color;

    int ImgW = GImage->GetWidth();
    int ImgH = GImage->GetHeight();

    // Screen
    TFPoint LT(0, 0);
    TFPoint RT(ImgW, 0);
    TFPoint RB(ImgW, ImgH);
    TFPoint LB(0, ImgH);
    TFPoint SP, EP;

    //First Line
    //--------------------------------------------------------------------------
    Gdi->CPen.Color = Tool.Color;
    if (FFstOrientation==orWest || FFstOrientation==orEast) {
        SP = Math.OnIntersect(Line[0].FP0, Line[0].FP1, LT, RT);
        EP = Math.OnIntersect(Line[0].FP0, Line[0].FP1, LB, RB);
    }
    else {
        SP = Math.OnIntersect(Line[0].FP0, Line[0].FP1, LT, LB);
        EP = Math.OnIntersect(Line[0].FP0, Line[0].FP1, RT, RB);
    }
    Gdi->OnMove(SP.x, SP.y);
    Gdi->OnLine(EP.x, EP.y);
    Gdi->CPen.Color = RED_COLOR  ;

    //Second Line
    //--------------------------------------------------------------------------
    Gdi->CPen.Color = Tool.Color;
    if (FSndOrientation==orWest || FSndOrientation==orEast) {
        SP = Math.OnIntersect(Line[1].FP0, Line[1].FP1, LT, RT);
        EP = Math.OnIntersect(Line[1].FP0, Line[1].FP1, LB, RB);
    }
    else {
        SP = Math.OnIntersect(Line[1].FP0, Line[1].FP1, LT, LB);
        EP = Math.OnIntersect(Line[1].FP0, Line[1].FP1, RT, RB);
    }
    Gdi->OnMove(SP.x, SP.y);
    Gdi->OnLine(EP.x, EP.y);
    Gdi->CPen.Color = RED_COLOR;

    //Two Point Line
    //--------------------------------------------------------------------------
    int n0 = FFstOrientation;
    int n1 = FSndOrientation;

    bool IsCrx = false;
    if (n0==0 && n1==1) IsCrx = true;
    if (n0==1 && n1==2) IsCrx = true;
    if (n0==2 && n1==3) IsCrx = true;
    if (n0==3 && n1==0) IsCrx = true;

    if (n1==0 && n0==1) IsCrx = true;
    if (n1==1 && n0==2) IsCrx = true;
    if (n1==2 && n0==3) IsCrx = true;
    if (n1==3 && n0==0) IsCrx = true;

    if (!IsCrx) {
        Gdi->CPen.Color = LIME_COLOR       ;
        Gdi->OnMove(FL0MP.x, FL0MP.y);
        Gdi->OnLine(FL1CP.x, FL1CP.y);

        Gdi->OnMove(FL1MP.x, FL1MP.y);
        Gdi->OnLine(FL0CP.x, FL0CP.y);
        Gdi->CPen.Color = RED_COLOR;
    }
    delete Gdi;
}
//---------------------------------------------------------------------------
//Class CircleArray 
//---------------------------------------------------------------------------

/*
CCircleArray::CCircleArray()
{
    FCount = 10;
    FTolerance = 20;

    WorldShape = new EWorldShape;
    CircleGauge = new ECircleGauge;

    for (int k=0; k<MAX_CIRCLE; k++) {
        ARoi[k] = new EROIBW8();
    }
}

CCircleArray::~CCircleArray()
{
    if (WorldShape    !=NULL) { delete WorldShape    ; WorldShape     = NULL; }
    if (CircleGauge   !=NULL) { delete CircleGauge   ; CircleGauge    = NULL; }
}

int CCircleArray::OnExecute()
{
    try
    {
        for (int k=0; k<FCount; k++) {
            int x = ARoi[k]->GetOrgX();
            int y = ARoi[k]->GetOrgY();
            int w = ARoi[k]->GetWidth();
            int h = ARoi[k]->GetHeight();

            if (w<10) continue;
            if (h<10) continue;

            FTolerance = w/10;
            if (FTolerance<10) FTolerance = 10;

            CircleGauge->SetCenterXY(x+w/2.0, y+h/2.0);
            CircleGauge->3(FTolerance);
            CircleGauge->SetDiameter (ARoi[k]->GetWidth());

            WorldShape->SetSensorSize(GImage->GetWidth(), GImage->GetHeight());
            WorldShape->Process(GImage, TRUE);
            CircleGauge->Attach(WorldShape);
            CircleGauge->SetDragable(true);
            CircleGauge->SetResizable(true);
            CircleGauge->SetRotatable(true);
            //CircleGauge->SetCircle(Circle);
            CircleGauge->Measure(GImage);
            Circle = CircleGauge->GetMeasuredCircle();

            FCP[k].x = Circle.GetCenterX ();
            FCP[k].y = Circle.GetCenterY ();
            FDiameter[k] = Circle.GetDiameter();
        }
    }
    catch(const EException&)
    {
      // Insert exception handling code here
    }

    return true;
}

void CCircleArray::OnPaint(HWND Handle, TColor RColor)
{
    if (Handle==NULL) return;

    HGdi *Gdi = new HGdi(Handle);
    TPoint Point1, Point2;
    Gdi->CText.Color  = clYellow;
    Gdi->CFont.Height = 12;

    Gdi->CPen.Color = RColor;

    CircleGauge->SetZoom(FHs, FVs);

    HDC hdc = GetDC(Handle);
    CircleGauge->Draw(hdc, EDrawingMode_Nominal     );
    CircleGauge->Draw(hdc, EDrawingMode_Actual      );
    //CircleGauge->Draw(hdc, EDrawingMode_SampledPaths);
    CircleGauge->Draw(hdc, EDrawingMode_SampledPath );
    CircleGauge->Draw(hdc, EDrawingMode_SampledPoints);

    ReleaseDC(Handle, hdc);

    for (int k=0; k<FCount; k++) {
        if (k==0) Gdi->CPen.Color = clRed;
        if (k==1) Gdi->CPen.Color = clFuchsia;
        if (k==2) Gdi->CPen.Color = clYellow;
        if (k==3) Gdi->CPen.Color = clGreen;
        if (k==4) Gdi->CPen.Color = clBlue;
        if (k==5) Gdi->CPen.Color = clNavy;
        if (k==6) Gdi->CPen.Color = clPurple;
        Gdi->OnCircle(false, FCP[k].x, FCP[k].y, FDiameter[k]/2.0, 10);
    }

    delete Gdi;
}

void CCircleArray::OnLoad(bool IsLoad, AnsiString Folder)
{
    //CFilter::OnLoad(IsLoad, Folder);

    HIFile Ini;

    if (Folder.Trim()=="") return;
    AnsiString Path = Folder + "\\CircleArray.ini";
    if (Path=="") return;

    // Roi Load & Save
    // -------------------------------------------------------------------------
    AnsiString IRoiPath = Folder + "\\CircleArray.ini";
    OnLoadRoi(IsLoad, IRoiPath, IRoi);

    for (int k=0; k<MAX_CIRCLE; k++) {
        AnsiString ARoiPath = Folder + "\\CircleArray" + k + ".roi";
        OnLoadRoi(IsLoad, ARoiPath, ARoi[k]);
    }

    Ini.Load(IsLoad, Path, "Parameter", "FCount"  , &FCount);
    Ini.Load(IsLoad, Path, "Parameter", "FTolerance"  , &FTolerance);

}

*/

