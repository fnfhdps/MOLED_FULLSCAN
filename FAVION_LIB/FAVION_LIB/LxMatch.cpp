#include "stdafx.h"
#include "LxMatch.h"
//------------------------------------------------------------------------------
CMatch::CMatch()
{
    FFindCount = 0;

    FMaxAngle        = 5;
    FMaxScale        = 1;
    FMinAngle        = 0;
    FMinScale        = 1;
    FMinScore        = 0.6f;
    FMaxPositions    = 5;
	FContrastMode    = 0;
	FCorrelationMode = 0;

	IRoi    = new EROIBW8() ;
	TRoi    = new EROIBW8() ;
    Matcher = new EMatcher();
	GImage  = NULL          ; 
}
//------------------------------------------------------------------------------
CMatch::~CMatch()
{
	delete (IRoi);
	delete (TRoi);
    delete Matcher;
    Matcher = NULL;
}
//------------------------------------------------------------------------------

void CMatch::SetMaxAngle       (float MaxAngle       ) { FMaxAngle        = MaxAngle    ;   }
void CMatch::SetMaxScale       (float MaxScale       ) { FMaxScale        = MaxScale    ;   }
void CMatch::SetMinAngle       (float MinAngle       ) { FMinAngle        = MinAngle    ;   }
void CMatch::SetMinScale       (float MinScale       ) { FMinScale        = MinScale    ;   }
void CMatch::SetMinScore       (float MinScore       ) { FMinScore        = MinScore    ;   }
void CMatch::SetMaxPositions   (int   MaxPositions   ) { FMaxPositions    = MaxPositions;   }
void CMatch::SetContrastMode   (int   ContrastMode   ) { FContrastMode    = ContrastMode;   }
void CMatch::SetCorrelationMode(int   CorrelationMode) { FCorrelationMode = CorrelationMode;}

float CMatch::GetMaxAngle    () { return FMaxAngle    ; }
float CMatch::GetMaxScale    () { return FMaxScale    ; }
float CMatch::GetMinAngle    () { return FMinAngle    ; }
float CMatch::GetMinScale    () { return FMinScale    ; }
float CMatch::GetMinScore    () { return FMinScore    ; }
int   CMatch::GetMaxPositions() { return FMaxPositions; }

//------------------------------------------------------------------------------
int CMatch::OnTrain()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CMatch Train GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CMatch Train GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CMatch Train GImage Height Size"    )); return false; }

    int x = TRoi->GetOrgX  ();
    int y = TRoi->GetOrgY  ();
    int w = TRoi->GetWidth ();
    int h = TRoi->GetHeight();

	int ix = IRoi->GetOrgX  ();
	int iy = IRoi->GetOrgY  ();
	int iw = IRoi->GetWidth ();
	int ih = IRoi->GetHeight();

    TPx = TRoi->GetOrgX()+TRoi->GetWidth ()/2.0f;
    TPy = TRoi->GetOrgY()+TRoi->GetHeight()/2.0f;

    FTrainImageWidth = w;
    FTrainImageHeight = h;

    TRoi->Detach();
    TRoi->Attach(GImage);

    try {
        Matcher->LearnPattern(TRoi);
        OnExecute();
    }
    catch (EException& exc)
    {
        MessageBox(NULL, CA2W(exc.What().c_str()), L"LearnAndFind", MB_OK | MB_ICONWARNING);
		return 0;
    }

	return 1;
}
//------------------------------------------------------------------------------

void CMatch::OnClear()
{
	FPass = false;
	FFindCount = 0;
	for (int i=0; i<MAX_SRH; i++) {
		Result[i].FPass = false;
		Result[i].FScale  = 0;
		Result[i].FAngle  = 0;
		Result[i].FPx     = 0;
		Result[i].FPy     = 0;
		Result[i].FId     = 0;
		Result[i].FScore  = 0;
	}
}
//------------------------------------------------------------------------------
int CMatch::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CMatch Execute GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CMatch Execute GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CMatch Execute GImage Height Size"    )); return false; }

    FPass = false;
    FFindCount = 0;

	OnClear();

	IRoi->Detach();
    IRoi->Attach(GImage);

    if (!Matcher->GetPatternLearnt()) return 0;

    Matcher->SetMaxAngle    (FMaxAngle    );
    Matcher->SetMaxScale    (FMaxScale    );
    Matcher->SetMinAngle    (FMinAngle    );
    Matcher->SetMinScale    (FMinScale    );
    Matcher->SetMinScore    (FMinScore    );
	Matcher->SetMaxPositions(FMaxPositions);
//     Matcher->SetMaxPositions((OEV_UINT32)FMaxPositions);

	// Match ContrastMode 설정 
	if(FContrastMode==0)    Matcher->SetContrastMode(EMatchContrastMode_Normal );
	if(FContrastMode==1)    Matcher->SetContrastMode(EMatchContrastMode_Inverse);
	if(FContrastMode==2)    Matcher->SetContrastMode(EMatchContrastMode_Any    );
	// Match CorrelationMode 설정 
	if(FCorrelationMode==0) Matcher->SetCorrelationMode(ECorrelationMode_Normalized       );
	if(FCorrelationMode==1) Matcher->SetCorrelationMode(ECorrelationMode_Standard         );
	if(FCorrelationMode==2) Matcher->SetCorrelationMode(ECorrelationMode_OffsetNormalized );
	if(FCorrelationMode==3) Matcher->SetCorrelationMode(ECorrelationMode_GainNormalized   );

    if (IRoi->GetOrgX()<0) return false;
    if (IRoi->GetOrgY()<0) return false;
    if ((IRoi->GetOrgX()+IRoi->GetWidth ())>GImage->GetWidth ()) return false;
    if ((IRoi->GetOrgY()+IRoi->GetHeight())>GImage->GetHeight()) return false;

    try {
        Matcher->Match(IRoi);
    }
    catch (EException& exc)
    {
        MessageBox(NULL, CA2W(exc.What().c_str()), L"Match Exe", MB_OK | MB_ICONWARNING);
    }

    int Count = Matcher->GetNumPositions();
    FFindCount = Count;
    if (Count<1)  return 0;
	if (Count>=MAX_SRH) return 0;

    float PvScore = 0.0f;
    int BestId = 0;

    double ImageCx = GImage->GetWidth()/2.0;
    double ImageCy = GImage->GetHeight()/2.0;

    for (int i=0; i<Count; i++) {
        Result[i].FPass = true;
        Result[i].FScale  = Matcher->GetPosition(i).Scale;
        Result[i].FAngle  = Matcher->GetPosition(i).Angle*(-1);
        Result[i].FPx     = Matcher->GetPosition(i).CenterX + IRoi->GetOrgX();
        Result[i].FPy     = Matcher->GetPosition(i).CenterY + IRoi->GetOrgY();
        Result[i].FId     = i;
        Result[i].FScore  = Matcher->GetPosition(i).Score*100.0f;
        Result[i].FRect.l = (long)(Result[i].FPx-FTrainImageWidth /2.0);
        Result[i].FRect.t = (long)(Result[i].FPy-FTrainImageHeight/2.0);
        Result[i].FRect.r = (long)(Result[i].FPx+FTrainImageWidth /2.0);
        Result[i].FRect.b = (long)(Result[i].FPy+FTrainImageHeight/2.0);
    }
    FPass = true;

    return FPass;
}
//------------------------------------------------------------------------------

void CMatch::OnLoad(bool IsLoad, CString Folder, int Type)
{
	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

    if (Folder.Trim()=="") return;
    CString Path = Folder + "\\Match.ini";

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

    Ini.Load(IsLoad, Path, L"Train", L"TPx", &TPx);
    Ini.Load(IsLoad, Path, L"Train", L"TPy", &TPy);

    Ini.Load(IsLoad, Path, L"Property", L"FTrainImageWidth ", &FTrainImageWidth );
    Ini.Load(IsLoad, Path, L"Property", L"FTrainImageHeight", &FTrainImageHeight);

    Ini.Load(IsLoad, Path, L"Property", L"FMaxAngle    ", &FMaxAngle    );
    Ini.Load(IsLoad, Path, L"Property", L"FMaxScale    ", &FMaxScale    );
    Ini.Load(IsLoad, Path, L"Property", L"FMinAngle    ", &FMinAngle    );
    Ini.Load(IsLoad, Path, L"Property", L"FMinScale    ", &FMinScale    );
    Ini.Load(IsLoad, Path, L"Property", L"FMinScore    ", &FMinScore    );
    Ini.Load(IsLoad, Path, L"Property", L"FMaxPositions", &FMaxPositions);

	// Train Blob Data
// 	Ini.Load(IsLoad, Path, "Property", "HiThresHold" , &Trace.FHiThHold );
// 	Ini.Load(IsLoad, Path, "Property", "LoThresHold" , &Trace.FLoThHold );
// 	Ini.Load(IsLoad, Path, "Property", "MinVerLength", &Trace.FMinHeight);
// 	Ini.Load(IsLoad, Path, "Property", "MaxVerLength", &Trace.FMaxHeight);
// 	Ini.Load(IsLoad, Path, "Property", "MinHorLength", &Trace.FMinWidth );
// 	Ini.Load(IsLoad, Path, "Property", "MaxHorLength", &Trace.FMaxWidth );

    // Roi Load & Save
    // -------------------------------------------------------------------------
    CString IRoiPath = Folder + "\\MatchIRoi.ini";
    OnLoadRoi(IsLoad, IRoiPath, IRoi);

    CString TRoiPath = Folder + "\\MatchTRoi.ini";
    OnLoadRoi(IsLoad, TRoiPath, TRoi);

	// ROI DATA (TEST)
	Roi_x = IRoi->GetOrgX();
	Roi_y = IRoi->GetOrgY();
	Roi_w = IRoi->GetWidth();
	Roi_h = IRoi->GetHeight();

	Ini.Load(IsLoad, Path, L"ROI_DATA", L"Roi_x", &Roi_x);
	Ini.Load(IsLoad, Path, L"ROI_DATA", L"Roi_y", &Roi_y);
	Ini.Load(IsLoad, Path, L"ROI_DATA", L"Roi_h", &Roi_h);
	Ini.Load(IsLoad, Path, L"ROI_DATA", L"Roi_w", &Roi_w);

    CString PatternPath  = "";
    CString MatchImgPath = "";
    CString FileName     = "";
    CString ImgName      = "";

    try {
        FileName.Format(L"MatchImage_%d.mch", Type);
        ImgName.Format (L"Match_Mark_%d.bmp", Type);
        PatternPath  = Folder + FileName;
		MatchImgPath = Folder + ImgName ;

        if (IsLoad) {
            CFolder Folder;
            if (Folder.ExistsDir(PatternPath)) {
                if (Matcher!=NULL) Matcher->Load(CW2A(PatternPath).m_psz);
            }
        }
        else {
            BOOL IsLearnDone;
			IsLearnDone = Matcher->GetPatternLearnt();
            if (IsLearnDone) {
                if (Matcher!=NULL){
					Matcher->CopyLearntPattern(Train);
					Train.Save(CW2A(MatchImgPath).m_psz);
					Matcher->Save(CW2A(PatternPath).m_psz);
				}
            }
        }
    }
    catch (EException& exc)
    {
		MessageBox(NULL, CA2W(exc.What().c_str()), L"LearnAndFind", MB_OK | MB_ICONWARNING);
		return ;
    }
}
//------------------------------------------------------------------------------

TPoint CMatch::OnRotate(TPoint c, TPoint t, double deg)
{
    return OnRotate(c.x, c.y, t.x, t.y, deg);
}
//------------------------------------------------------------------------------

TPoint CMatch::OnRotate(double cx, double cy, double tx, double ty, double deg)
{
    TPoint np;
    double Rad = deg * M_PI/180.0;
    double COS = cos(Rad);
    double SIN = sin(Rad);

    if (deg ==  90) {COS =  0; SIN =  1;}
    if (deg == 180) {COS = -1; SIN =  0;}
    if (deg == 270) {COS =  0; SIN = -1;}
    if (deg == 360) {COS =  1; SIN =  0;}

    np.x =(int)((tx-cx)*COS - (ty-cy)*SIN);
    np.y =(int)((tx-cx)*SIN + (ty-cy)*COS);
    np.x =(int)(np.x+ cx);
    np.y =(int)(np.y+ cy);

    return np;
}
//------------------------------------------------------------------------------

void CMatch::OnPaintResult(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    Gdi->CPen.Style = 0;
    Gdi->CFont.Height = 28;

    int x = IRoi->GetOrgX();
    int y = IRoi->GetOrgY();
    CString Txt = FPass ? "PASS" : "FAIL";
    Gdi->CText.Color = FPass ? BLUE_COLOR : RED_COLOR;
    Gdi->OnText(x+20, y+60, Txt);

    delete Gdi;
}

/*
void CMatch::OnPaint(HDC hdc)
{
    int Count = results.size();

    for (int i=0; i<Count; i++) {
        if (FDrawBoundingBox  ) results[i].SetDrawBoundingBox(true);
        if (FDrawCenter       ) results[i].SetDrawCenter(true);
        if (FDrawFeaturePoints) results[i].SetDrawFeaturePoints(true);

        results[i].Draw(hdc, FHs, FVs);
    }
}
*/
void CMatch::OnPaint(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
	if(Matcher==NULL)     return;

    HDC hdc = ::GetDC(Tool.Handle);

    int Count = Matcher->GetNumPositions();

//     추후에 다시 수정예정 
//     CString RGBNum = IntToHex(Tool.Color, 6);
//     CString BLUE_COLOR   = "0x" + RGBNum.SubString(1, 2);
//     CString GREEN_COLOR  = "0x" + RGBNum.SubString(3, 2);
//     CString RED_COLOR    = "0x" + RGBNum.SubString(5, 2);
// 
//     ERGBColor RGBColor;
//     RGBColor.RED_COLOR = RED_COLOR.ToIntDef(0);
//     RGBColor.GREEN_COLOR = GREEN_COLOR.ToIntDef(0);
//     RGBColor.BLUE_COLOR = BLUE_COLOR.ToIntDef(0);
// 
//     for (int i=0; i<Count; i++) {
//         Matcher->DrawPosition(hdc, RGBColor, i, 0, FHs, FVs);
//     }
//     ReleaseDC(Handle, hdc);

	CString RGBNum;

	CString Blue   = "0x008000";
	CString Green  = "0x008000";
	CString Red    = "0x008000";

	ERGBColor RGBColor;

	RGBColor.Red   = _tstoi(Red  );
	RGBColor.Green = _tstoi(Green);
	RGBColor.Blue  = _tstoi(Blue );

	for (int i=0; i<Count; i++) {
		Matcher->DrawPosition(hdc, RGBColor, i, 0,Tool.FHs, Tool.FVs);
	}
	ReleaseDC(Tool.Handle, hdc);
}
void CMatch::OnPaintPattern(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
	if(Matcher==NULL)     return;
	if(!Matcher->GetPatternLearnt()) return;

	EBaseROI *ROI = Matcher->GetPattern(0);

	if (ROI==NULL) return;
	if (ROI->GetWidth()<1) return;
	if (ROI->GetHeight()<1) return;

	HDC hdc = NULL;

	hdc = GetDC(Tool.Handle);
	CRect Rect;
	GetWindowRect(Tool.Handle, &Rect); // Window Handle에서 Control 크기 받아오는 함수 

	float wr = (float)Rect.Width()/(float)Matcher->GetPatternWidth();
	float vr = (float)Rect.Height()/(float)Matcher->GetPatternHeight();

	ROI->Draw(hdc, wr, vr);

	ReleaseDC(Tool.Handle, hdc);
}

