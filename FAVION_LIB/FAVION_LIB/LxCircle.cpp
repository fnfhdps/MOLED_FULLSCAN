#include "stdafx.h"
#include "LxCircle.h"
CCircle::CCircle()
{
//     FTransitionChoice = ETransitionChoice_NthFromBegin;

    DwNominal = false;
    DwActual = true;
    DwSampledPaths = false;
    DwSampledPath = false;
    DwSampledPoints = true;

//     WorldShape  = new EWorldShape ;
    CircleGauge  = new ECircleGauge;
	IRoi = new EROIBW8();

	FCirclePoint = NULL;
	FCirclePointCount =0;

//     Trace.FMinX = 30;
//     Trace.FMinY = 30;
//     Trace.FLoThHold = 0;
//     Trace.FHiThHold = 50;
// 
//     FCP.x = 0;
//     FCP.y = 0;
//     FDiameter = 0.0f;

	// Circle Fitting Init
	// Not Change 
	FThickness = 1 ;
	FSmoothing = 1 ;
	FThresHold = 10;
	FNumFilteringPasses = 5;

	// Change 
	FTransitionChoice = 0     ;
	FCircleType       = 0     ;
	FTransitionIndex  = 0     ;
	FSamplingStep     = 10    ;
	FTolerance        = 20    ;
	FAngle            = 0.0f  ;
    FAmplitude        = 90.0f ;
    FCenterX          = 100.0f;
    FCenterY          = 100.0f;
	FDiameter         = 0.0f  ;
}
//---------------------------------------------------------------------------
//Class Circle
//---------------------------------------------------------------------------

CCircle::~CCircle()
{
//     if (WorldShape    !=NULL) { delete WorldShape    ; WorldShape     = NULL; }
    if (CircleGauge   !=NULL) { delete CircleGauge   ; CircleGauge    = NULL; }
}
int CCircle::OnExecute(bool IsValid)
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CCircle GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CCircle GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CCircle GImage Height Size"    )); return false; }

    try
    {
//         int x = IRoi->GetOrgX();
//         int y = IRoi->GetOrgY();
//         int w = IRoi->GetWidth();
//         int h = IRoi->GetHeight();
        int gw = GImage->GetWidth();
        int gh = GImage->GetHeight();

//         if (x<0) { SetProcessMessage("int CCircle::OnExecute() >> x<0"); return false; }
//         if (y<0) { SetProcessMessage("int CCircle::OnExecute() >> y<0"); return false; }
//         if (w>gw) { SetProcessMessage("int CCircle::OnExecute() >> w>gw"); return false; }
//         if (h>gh) { SetProcessMessage("int CCircle::OnExecute() >> h>gh"); return false; }

		// 기본 Parameter 조건확인 
        if (FSamplingStep>20 ) FSamplingStep = 20  ;
		if (FTolerance   <5  ) FTolerance    =150  ;
		if (FAngle       <0  ) FAngle        =  0  ;
		if (FAmplitude   <0  ) FAmplitude    =100  ;
		if (FCenterX     <=0 ) FCenterX      =100.f;
		if (FCenterY     <=0 ) FCenterY      =100.f;

		// 기본 Parameter 설정 
		CircleGauge->SetSamplingStep(FSamplingStep) ;
		CircleGauge->SetTolerance(FTolerance)       ;
		CircleGauge->SetDiameter(FDiameter)         ;
		CircleGauge->SetAngle(FAngle)               ;
		CircleGauge->SetAmplitude(FAmplitude)       ;
		CircleGauge->SetCenterXY(FCenterX, FCenterY);
		// Circle Index 설정 
		CircleGauge->SetTransitionIndex(FTransitionIndex);

		// Circle Choice  
		if(FTransitionChoice==0)  CircleGauge->SetTransitionChoice(ETransitionChoice_NthFromBegin    ); 
		if(FTransitionChoice==1)  CircleGauge->SetTransitionChoice(ETransitionChoice_NthFromEnd      ); 
		if(FTransitionChoice==2)  CircleGauge->SetTransitionChoice(ETransitionChoice_LargestAmplitude); 
		if(FTransitionChoice==3)  CircleGauge->SetTransitionChoice(ETransitionChoice_LargestArea     ); 
		if(FTransitionChoice==4)  CircleGauge->SetTransitionChoice(ETransitionChoice_Closest         ); 

		// Circle Type
		if(FCircleType==0)  CircleGauge->SetTransitionType(ETransitionType_Bw    ); //Black to White
		if(FCircleType==1)  CircleGauge->SetTransitionType(ETransitionType_Wb    ); //White To Black
		if(FCircleType==2)  CircleGauge->SetTransitionType(ETransitionType_BwOrWb); 
		if(FCircleType==3)  CircleGauge->SetTransitionType(ETransitionType_Bwb   );
        if(FCircleType==4)  CircleGauge->SetTransitionType(ETransitionType_Wbw   );

//         WorldShape->SetSensorSize(GImage->GetWidth(), GImage->GetHeight());
//         WorldShape->Process(GImage, TRUE);
//         CircleGauge->Attach(WorldShape);
//         CircleGauge->SetDragable (true);
//         CircleGauge->SetResizable(true);
//         CircleGauge->SetRotatable(true);

		// Circle Gauge Inspection
        CircleGauge->Measure(GImage);
// 		CircleGauge->MeasureSample(GImage,0);

		// Circle Information 
        Circle = CircleGauge->GetMeasuredCircle(); 
// 		int CircleCount = CircleGauge->GetNumSamples();
		int GetNumSamples        = CircleGauge->GetNumSamples();        // 전체 샘플수량 
		int CircleCount          = CircleGauge->GetNumValidSamples();   // 유효 샘플수량 
// 		CircleGauge->MeasureSample(GImage,CircleCount);

		// Center Coordinate Init 
// 		FCP.x =0.0f; FCP.y =0.0f;
		FCP.x          = Circle.GetCenterX ();
		FCP.y          = Circle.GetCenterY ();
		Diameter       = Circle.GetDiameter();
		int StartAngle = Circle.GetAngle();
		int Amplitude  = Circle.GetAmplitude();
		int EndAngle   = Circle.GetEndAngle();

		if(IsValid){ // 유효 샘플수량 메모리 동적생성
			if (FCirclePoint != NULL) delete[] FCirclePoint;
			FCirclePoint = new TFPoint[CircleCount];
			
			FCirclePointCount=0;
			bool bValid = false;
			EPoint P(0,0);
			for(int k=0; k<GetNumSamples; k++){
				CircleGauge->MeasureSample(GImage,k);
				bValid = CircleGauge->GetValid();
				if(bValid){
					P = CircleGauge->GetMeasuredPoint();
					FCirclePoint[FCirclePointCount].x  = P.GetX();
					FCirclePoint[FCirclePointCount].y  = P.GetY();
					FCirclePointCount++;
				}
			}
		}
		else {       // 전체 샘풀수량 메모리 동적 생성
			// Memory Allocate 
			if (FCirclePoint != NULL) delete[] FCirclePoint;
			FCirclePoint = new TFPoint[GetNumSamples];

			FCirclePointCount=0;
			bool bValid = false;
			EPoint P(0,0);
			for(int k=0; k<GetNumSamples; k++){
				CircleGauge->MeasureSample(GImage,k);
				bValid = CircleGauge->GetValid();
				if(bValid){   // Sample 기준 데이터가 존재할경우 
					P = CircleGauge->GetMeasuredPoint();
					FCirclePoint[FCirclePointCount].x  = P.GetX();
					FCirclePoint[FCirclePointCount].y  = P.GetY();
					FCirclePointCount++;
				}
				else { // Sample 기준 데이터가 존재하지 않을경우 
					FCirclePoint[FCirclePointCount].x  = 0.0f;
					FCirclePoint[FCirclePointCount].y  = 0.0f;
					FCirclePointCount++;
				}
			}
		}
    }
    catch(const EException&)
    {
        AfxMessageBox(L"Circle Fitting Inspection Error");    
	}

    return true;
}
// ORG Source 
/*
int CCircle::OnExecute()
{
	try
	{
		//Find Dark Blob
		if (Mode==1) {
			Trace.GImage = GImage;
			Trace.IRoi = IRoi;
			Trace.OnExecute();

			if (Trace.BlobCount<1) {
				SetProcessMessage("int CCircle::OnExecute() >> x<0"); 
				return false;
			}

			Trace.Blob[0].Rect.l;
			Trace.Blob[0].Rect.t;
			Trace.Blob[0].Rect.r;
			Trace.Blob[0].Rect.b;

			IRoi->SetOrgX  (Trace.Blob[0].Rect.l);
			IRoi->SetOrgY  (Trace.Blob[0].Rect.t);
			IRoi->SetWidth (Trace.Blob[0].Rect.r-Trace.Blob[0].Rect.l);
			IRoi->SetHeight(Trace.Blob[0].Rect.b-Trace.Blob[0].Rect.t);
		}

		int x = IRoi->GetOrgX();
		int y = IRoi->GetOrgY();
		int w = IRoi->GetWidth();
		int h = IRoi->GetHeight();
		int gw = GImage->GetWidth();
		int gh = GImage->GetHeight();

		if (x<0) { SetProcessMessage("int CCircle::OnExecute() >> x<0"); return false; }
		if (y<0) { SetProcessMessage("int CCircle::OnExecute() >> y<0"); return false; }
		if (w>gw) { SetProcessMessage("int CCircle::OnExecute() >> w>gw"); return false; }
		if (h>gh) { SetProcessMessage("int CCircle::OnExecute() >> h>gh"); return false; }

		FSamplingStep = float(w*3.14)/24;
		if (FSamplingStep>20) FSamplingStep = 20;

		CircleGauge->SetTransitionChoice(ETransitionChoice_NthFromBegin);
		CircleGauge->SetNumFilteringPasses(FNumFilteringPasses);
		CircleGauge->SetSamplingStep(FSamplingStep);

		FTolerance = w/10;
		if (FTolerance<10) FTolerance = 10;

		CircleGauge->SetCenterXY(x+w/2.0, y+h/2.0);
		CircleGauge->SetTolerance(FTolerance);
		CircleGauge->SetDiameter (IRoi->GetWidth());

		//         WorldShape->SetSensorSize(GImage->GetWidth(), GImage->GetHeight());
		//         WorldShape->Process(GImage, TRUE);
		//         CircleGauge->Attach(WorldShape);
		CircleGauge->SetDragable (true);
		CircleGauge->SetResizable(true);
		CircleGauge->SetRotatable(true);
		CircleGauge->Measure(GImage);
		Circle = CircleGauge->GetMeasuredCircle();

		FCP.x = Circle.GetCenterX ();
		FCP.y = Circle.GetCenterY ();
		FDiameter = Circle.GetDiameter();
	}
	catch(const EException&)
	{
		// Insert exception handling code here
	}
	return true;
}
*/
// Set Function
//-------------------------------------------------------------------------
void CCircle::SetCircleChoice(int TransitionChoice)
{
	FTransitionChoice = TransitionChoice;
}
void CCircle::SetCircleType(int CircleType)
{
	FCircleType = CircleType;
}
void CCircle::SetCircleIndex(int CircleIndex)
{
	FTransitionIndex = CircleIndex;
}
void CCircle::SetSamplingStep(float SamplingStep)
{
	FSamplingStep = SamplingStep;
}
void CCircle::SetTolerance(float Tolerance)
{
    FTolerance  = Tolerance;
}
void CCircle::SetDiameter(float Diameter)
{
	FDiameter  = Diameter;
}
void CCircle::SetAngle(float Angle)
{
	FAngle  = Angle;
}
void CCircle::SetAmplitude(float Amplitude)
{
	FAmplitude  = Amplitude;
}
void CCircle::SetCenterXY(float CenterX,float CenterY)
{
	FCenterX  = CenterX;
	FCenterY  = CenterY;
}
//----------------------------------------------------------------------------
void CCircle::OnPaint(TDrawTool Tool)
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

    CircleGauge->SetZoom(Tool.FHs, Tool.FVs);

    HDC hdc = GetDC(Tool.Handle);

    if (DwNominal      ) CircleGauge->Draw(hdc, EDrawingMode_Nominal      );
    if (DwActual       ) CircleGauge->Draw(hdc, EDrawingMode_Actual       );
    if (DwSampledPaths ) CircleGauge->Draw(hdc, EDrawingMode_SampledPaths );
    if (DwSampledPath  ) CircleGauge->Draw(hdc, EDrawingMode_SampledPath  );
    if (DwSampledPoints) CircleGauge->Draw(hdc, EDrawingMode_SampledPoints);

    ReleaseDC(Tool.Handle, hdc);

    Gdi->CPen.Color = RED_COLOR;
    Gdi->OnLinePlus((FCP.x+0.5), (FCP.y+0.5), (Diameter/2.0+0.5));

    delete Gdi;
}

void CCircle::OnLoad(bool IsLoad, CString Folder)
{
    //CFilter::OnLoad(IsLoad, Folder);

    CIFile Ini;

    if (Folder.Trim()=="") return;
    CString Path = Folder + "\\Circle.ini";
    if (Path=="") return;

    // Roi Load & Save
    // -------------------------------------------------------------------------
    CString IRoiPath = Folder + "\\Circle.ini";
    OnLoadRoi(IsLoad, IRoiPath, IRoi);

    Ini.Load(IsLoad, Path, "Parameter", "FTolerance"  , &FTolerance);
}

//------------------------------------------------------------------------------
