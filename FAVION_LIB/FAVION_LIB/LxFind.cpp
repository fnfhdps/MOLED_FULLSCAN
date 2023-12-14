// LxFind.cpp: implementation of the CFind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LxFind.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFind::CFind()
{
    Property.DrawBoundingBox = true;
    Property.DrawCenter = true;
    Property.DrawFeaturePoints = true;
	
//     Image = new EImageBW8();
    IRoi  = new EROIBW8();
    TRoi  = new EROIBW8();
    MRoi  = new EROIBW8();
	GImage=NULL          ;
	
    PatternFinder = new EPatternFinder();
}
CFind::~CFind()
{
// 	delete (Image);
    delete (IRoi) ;
    delete (TRoi) ;
    delete (MRoi) ;

//     delete (PatternFinder);
// 	PatternFinder = NULL;

// 	delete (Image);
// 	delete (IRoi);
// 	delete (TRoi);
// 	delete (MRoi);
// 	delete (PatternFinder);
// 	PatternFinder = NULL;

}
void CFind::SetDrawBoundingBox   (bool  DrawBoundingBox   ) { Property.DrawBoundingBox  = DrawBoundingBox  ; }
void CFind::SetDrawCenter        (bool  DrawCenter        ) { Property.DrawCenter       = DrawCenter       ; }
void CFind::SetDrawFeaturePoints (bool  DrawFeaturePoints ) { Property.DrawFeaturePoints= DrawFeaturePoints; }
void CFind::SetIsUseIgnoreRoi    (bool  IsUseIgnoreRoi    ) { Property.IsUseIgnoreRoi   = IsUseIgnoreRoi   ; }
void CFind::SetIsDarkIgnoreRoi   (bool  IsDarkIgnoreRoi   ) { Property.IsDarkIgnoreRoi  = IsDarkIgnoreRoi  ; }
void CFind::SetOpIgnoreSamePos   (bool  OpIgnoreSamePos   ) { Property.OpIgnoreSamePos  = OpIgnoreSamePos  ; }
void CFind::SetReductionMode     (EReductionMode Mode     ) { PatternFinder->SetReductionMode     (Mode             ); Property.ReductionMode    = Mode             ; }
void CFind::SetLightBalance      (float LightBalance      ) { PatternFinder->SetLightBalance      (LightBalance     ); Property.LightBalance     = LightBalance     ; }
void CFind::SetAngleBias         (float AngleBias         ) { PatternFinder->SetAngleBias         (AngleBias        ); Property.AngleBias        = AngleBias        ; }
void CFind::SetAngleTolerance    (float AngleTolerance    ) { PatternFinder->SetAngleTolerance    (AngleTolerance   ); Property.AngleTolerance   = AngleTolerance   ; }
void CFind::SetScaleBias         (float ScaleBias         ) { PatternFinder->SetScaleBias         (ScaleBias        ); Property.ScaleBias        = ScaleBias        ; }
void CFind::SetScaleTolerance    (float ScaleTolerance    ) { PatternFinder->SetScaleTolerance    (ScaleTolerance   ); Property.ScaleTolerance   = ScaleTolerance   ; }
void CFind::SetFindExtension     (float FindExtension     ) { PatternFinder->SetFindExtension     (FindExtension    ); Property.FindExtension    = FindExtension    ; }
void CFind::SetReductionStrength (float ReductionStrength ) { PatternFinder->SetReductionStrength (ReductionStrength); Property.ReductionStrength= ReductionStrength; }
void CFind::SetMinFeaturePoints  (float MinFeaturePoints  ) { PatternFinder->SetMinFeaturePoints  (MinFeaturePoints ); Property.MinFeaturePoints = MinFeaturePoints ; }
void CFind::SetMaxFeaturePoints  (float MaxFeaturePoints  ) { PatternFinder->SetMaxFeaturePoints  (MaxFeaturePoints ); Property.MaxFeaturePoints = MaxFeaturePoints ; }
void CFind::SetLocalSearchMode   (ELocalSearchMode Mode   ) { PatternFinder->SetLocalSearchMode   (Mode             ); Property.LocalSearchMode  = Mode             ; }
void CFind::SetAngleSearchExtent (float AngleSearchExtent ) { PatternFinder->SetAngleSearchExtent (AngleSearchExtent); Property.AngleSearchExtent= AngleSearchExtent; }
void CFind::SetScaleSearchExtent (float ScaleSearchExtent ) { PatternFinder->SetScaleSearchExtent (ScaleSearchExtent); Property.ScaleSearchExtent= ScaleSearchExtent; }
void CFind::SetXSearchExtent     (float XSearchExtent     ) { PatternFinder->SetXSearchExtent     (XSearchExtent    ); Property.XSearchExtent    = XSearchExtent    ; }
void CFind::SetYSearchExtent     (float YSearchExtent     ) { PatternFinder->SetYSearchExtent     (YSearchExtent    ); Property.YSearchExtent    = YSearchExtent    ; }
void CFind::SetMaxInstances      (int   MaxInstances      ) { PatternFinder->SetMaxInstances      (MaxInstances     ); Property.MaxInstances     = MaxInstances     ; }
void CFind::SetContrastMode      (EFindContrastMode Mode  ) { PatternFinder->SetContrastMode      (Mode             ); Property.ContrastMode     = Mode             ; }
void CFind::SetIgnoreSamePosX    (int   IgnoreSamePosX    ) { IgnoreSamePosX   = IgnoreSamePosX   ; }
void CFind::SetIgnoreSamePosY    (int   IgnoreSamePosY    ) { IgnoreSamePosY   = IgnoreSamePosY   ; }
void CFind::SetOx                (float Ox                ) { Ox               = Ox               ; }
void CFind::SetOy                (float Oy                ) { Oy               = Oy               ; }
void CFind::SetMinScore          (float MinScore          ) { PatternFinder->SetMinScore          (MinScore         ); Property.MinScore         = MinScore         ; }
void CFind::SetZeroScore         (float ZeroScore         ) { Property.ZeroScore        = ZeroScore        ; }
void CFind::SetPatternType       (EPatternType     Type   ) { PatternFinder->SetPatternType       (Type             ); Property.PatternType      = Type             ; }

//------------------------------------------------------------------------------

bool  CFind::GetDrawBoundingBox   () { return Property.DrawBoundingBox  ; }
bool  CFind::GetDrawCenter        () { return Property.DrawCenter       ; }
bool  CFind::GetDrawFeaturePoints () { return Property.DrawFeaturePoints; }
bool  CFind::GetIsUseIgnoreRoi    () { return Property.IsUseIgnoreRoi   ; }
bool  CFind::GetIsDarkIgnoreRoi   () { return Property.IsDarkIgnoreRoi  ; }
bool  CFind::GetOpIgnoreSamePos   () { return Property.OpIgnoreSamePos  ; }
bool  CFind::GetReductionMode     () { return Property.ReductionMode     = PatternFinder->GetReductionMode     (); }
float CFind::GetLightBalance      () { return Property.LightBalance      = PatternFinder->GetLightBalance      (); }
float CFind::GetAngleBias         () { return Property.AngleBias         = PatternFinder->GetAngleBias         (); }
float CFind::GetAngleTolerance    () { return Property.AngleTolerance    = PatternFinder->GetAngleTolerance    (); }
float CFind::GetScaleBias         () { return Property.ScaleBias         = PatternFinder->GetScaleBias         (); }
float CFind::GetScaleTolerance    () { return Property.ScaleTolerance    = PatternFinder->GetScaleTolerance    (); }
float CFind::GetFindExtension     () { return Property.FindExtension     = PatternFinder->GetFindExtension     (); }
float CFind::GetReductionStrength () { return Property.ReductionStrength = PatternFinder->GetReductionStrength (); }
float CFind::GetMinFeaturePoints  () { return Property.MinFeaturePoints  = PatternFinder->GetMinFeaturePoints  (); }
float CFind::GetMaxFeaturePoints  () { return Property.MaxFeaturePoints  = PatternFinder->GetMaxFeaturePoints  (); }
float CFind::GetLocalSearchMode   () { return Property.LocalSearchMode   = PatternFinder->GetLocalSearchMode   (); }
float CFind::GetAngleSearchExtent () { return Property.AngleSearchExtent = PatternFinder->GetAngleSearchExtent (); }
float CFind::GetScaleSearchExtent () { return Property.ScaleSearchExtent = PatternFinder->GetScaleSearchExtent (); }
float CFind::GetXSearchExtent     () { return Property.XSearchExtent     = PatternFinder->GetXSearchExtent     (); }
float CFind::GetYSearchExtent     () { return Property.YSearchExtent     = PatternFinder->GetYSearchExtent     (); }
int   CFind::GetMaxInstances      () { return Property.MaxInstances      = PatternFinder->GetMaxInstances      (); }
int   CFind::GetContrastMode      () { return Property.ContrastMode      = PatternFinder->GetContrastMode      (); }
int   CFind::GetIgnoreSamePosX    () { return Property.IgnoreSamePosX    ; }
int   CFind::GetIgnoreSamePosY    () { return Property.IgnoreSamePosY    ; }
float CFind::GetOx                () { return Property.Ox                ; }
float CFind::GetOy                () { return Property.Oy                ; }
float CFind::GetMinScore          () { return Property.MinScore          = PatternFinder->GetMinScore          (); }
float CFind::GetZeroScore         () { return Property.ZeroScore         ; }
int   CFind::GetPatternType       () { return Property.PatternType       = PatternFinder->GetPatternType       (); }

//------------------------------------------------------------------------------
int CFind::OnTrain(BYTE* imgPtr, int imgW, int imgH, int roix, int roiy, int roiW, int roiH)
{
	try
	{
		PatternFinder->SetInterpolate(true);

// 		Image->SetImagePtr(imgW, imgH, imgPtr);
// 		TRoi->Attach(Image);

		GImage->SetImagePtr(imgW, imgH, imgPtr);
		TRoi->Attach(GImage);

		TRoi->SetPlacement(roix, roiy, roiW, roiH);
		PatternFinder->Learn(TRoi);

		IRoi->SetPlacement(1, 1, imgW-2, imgH-2);
		OnExecute();
	}
    catch (EException& exc)
    {
        MessageBox(NULL, CA2W(exc.What().c_str()), L"LearnAndFind", MB_OK | MB_ICONWARNING);
		return 0;
    }

	return 1;
}

//int CFind::OnTrain()
//{
//	if (GImage==NULL         ) { AfxMessageBox(_T("Check CFind Train GImage Memory Allocate")); return false; }
//	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CFind Train GImage Width Size"     )); return false; }
//	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CFind Train GImage Height Size"    )); return false; }
//
//    int x = TRoi->GetOrgX();
//    int y = TRoi->GetOrgY();
//    int w = TRoi->GetWidth();
//    int h = TRoi->GetHeight();
//
//    int mx = MRoi->GetOrgX();
//    int my = MRoi->GetOrgY();
//    int mw = MRoi->GetWidth();
//    int mh = MRoi->GetHeight();
//
//
//	Property.TPx = TRoi->GetOrgX()+TRoi->GetWidth ()/2.0;
//	Property.TPy = TRoi->GetOrgY()+TRoi->GetHeight()/2.0;
//
//    TrImageWidth = w;
//    TrImageHeight = h;
//
//	TRoi->Detach();
//    TRoi->Attach(GImage);
//	int PixelValue;
//
//    try {
//        if (Property.IsUseIgnoreRoi) {
//            EImageBW8 MaskImage;
//            int iw      = GImage->GetWidth();
//            int ih      = GImage->GetHeight();
//			BufferWidth = GImage->GetWidth();
//            MaskImage.SetSize(iw, ih);
//			EasyImage::Oper(EArithmeticLogicOperation_Copy, GImage, &MaskImage);
//
////            memcpy((BYTE *)MaskImage.GetImagePtr(), (BYTE *)GImage->GetImagePtr(), (iw*ih));
////             void *MAddress = (BYTE *) MaskImage.GetImagePtr();
//			MAddress = MaskImage.GetImagePtr();
//// 			EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage,&MaskImage);
////             #define MElement(x, y)  *((BYTE *)MAddress + x + iw * y)
//
//            for (int x=mx; x<(mx+mw); x++) {
//            for (int y=my; y<(my+mh); y++) {
//                MElement(x, y) = Property.IsDarkIgnoreRoi ? 0 : 255;
//			}
//			}
//
//            TRoi->Attach(&MaskImage);
////			MaskImage.Save("D:\\ROTATE_IMG\\Train.bmp");
//            PatternFinder->Learn(TRoi, &MaskImage);
////			MRoi->Attach(&MaskImage);
////			PatternFinder->Learn(TRoi, MRoi);
//		}
//        else {
//            PatternFinder->Learn(TRoi);
//        }
//        OnExecute();
//    }
//    catch (EException& exc)
//    {
//        MessageBox(NULL, CA2W(exc.What().c_str()), L"LearnAndFind", MB_OK | MB_ICONWARNING);
//		return false;
//    }
//
//	return true;
//}
int CFind::OnTrain()
{
	if (GImage == NULL         ) { AfxMessageBox(_T("Check CFind Train GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()  < 1) { AfxMessageBox(_T("Check CFind Train GImage Width Size"     )); return false; }
	if (GImage->GetHeight() < 1) { AfxMessageBox(_T("Check CFind Train GImage Height Size"    )); return false; }

	int x = TRoi->GetOrgX();
	int y = TRoi->GetOrgY();
	int w = TRoi->GetWidth();
	int h = TRoi->GetHeight();

	int mx = MRoi->GetOrgX();
	int my = MRoi->GetOrgY();
	int mw = MRoi->GetWidth();
	int mh = MRoi->GetHeight();

	Property.TPx = TRoi->GetOrgX() + TRoi->GetWidth() / 2.0;
	Property.TPy = TRoi->GetOrgY() + TRoi->GetHeight() / 2.0;

	TrImageWidth  = w;
	TrImageHeight = h;

	TRoi->Detach();
	TRoi->Attach(GImage);
	int PixelValue;

	try {
		if (Property.IsUseIgnoreRoi) {
//			EImageBW8 MaskImage;

			//TROI 크기만큼에 화이트 이미지 
//			MaskImage.SetSize(TRoi); //Masking Image Train ROI영역 크기만큼 만든다 
//			EasyImage::Oper(EArithmeticLogicOperation_Copy, EBW8(255), &MaskImage);
//			MaskImage.Save("D:\\ROTATE_IMG\\MaskImage1.bmp");
			//			EasyImage::Oper(EArithmeticLogicOperation_Copy, EBW8(0  ), &MaskImage2);

//			EasyImage::Oper(EArithmeticLogicOperation_Copy, TRoi, &MaskImage1);
//			EasyImage::Oper(EArithmeticLogicOperation_Copy, TRoi, &MaskImage2);
			//MROI해당하는 
//			MRoi->Attach(&MaskImage);
//			MRoi->SetPlacement(mx - x, my - y, mw, mh);
//			EasyImage::Oper(EArithmeticLogicOperation_Copy, EBW8(0), MRoi);
//			MaskImage.Save("D:\\ROTATE_IMG\\MaskImage2.bmp");

//			MaskImage2.Save("D:\\ROTATE_IMG\\MaskImage2.bmp");
//			MRoi->Attach(&MaskImage2);
//			MRoi->SetPlacement(mx - x, my - y, mw, mh);
//			EasyImage::Oper(EArithmeticLogicOperation_Copy, EBW8(255), MRoi);
//			MaskImage2.Save("D:\\ROTATE_IMG\\MaskImage2.bmp");

//			TRoi->Attach(&MaskImage);
			//			MaskImage.Save("D:\\ROTATE_IMG\\Train.bmp");
//			MaskImg.Save("D:\\ROTATE_IMG\\Train.bmp");
			PatternFinder->Learn(TRoi, &MaskImg);
			//			MRoi->Attach(&MaskImage);
			//			PatternFinder->Learn(TRoi, MRoi);
		}
		else {
			PatternFinder->Learn(TRoi);
		}
		OnExecute();
	}
	catch (EException & exc)
	{
		MessageBox(NULL, CA2W(exc.What().c_str()), L"LearnAndFind", MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;
}
//------------------------------------------------------------------------------

int CFind::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CFind GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CFind GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CFind GImage Height Size"    )); return false; }

    IsPass = false;

    for (int i=0; i<MAX_SRH; i++) {
        Result[i].IsPass = false;
        Result[i].Scale  = 0;
        Result[i].Angle  = 0;
        Result[i].Px     = 0;
        Result[i].Py     = 0;
        Result[i].Id     = 0;
        Result[i].Score  = 0;
    }

	int x = IRoi->GetOrgX();
	int y = IRoi->GetOrgY();
	int w = IRoi->GetWidth();
	int h = IRoi->GetHeight();

	if (x<0) return false;
	if (y<0) return false;
	if (w>GImage->GetWidth()) return false;
	if (h>GImage->GetHeight()) return false;

	IRoi->Detach();
    IRoi->Attach(GImage);

    if (!PatternFinder->GetLearningDone()) return 0;

    results.clear();
	try {
		results = PatternFinder->Find(IRoi);
	}
	catch (EException& exc)
	{
		SetProcessMessage("int CFind::OnExecute()");
		return false;
	}
//     results = PatternFinder->Find(IRoi);

    int Count = results.size();
    FindCount = Count;
    if (Count<1)  return 0;

    float PvScore = 0.0f;
    int BestId = 0;

    //if (PixleResolution<=0) PixleResolution = 1.0;
	double ImageCx = GImage->GetWidth()/2.0;
	double ImageCy = GImage->GetHeight()/2.0;

    float Ox = Property.Ox;
    float Oy = Property.Oy;

    bool OpIgnoreSamePos = Property.OpIgnoreSamePos;

    for (int k=0; k<MAX_SRH; k++) {
        IgnoreResult[k] = false;
    }

	float CenterX = results[0].GetCenter().GetX();
	float CenterY = results[0].GetCenter().GetY();

    if (OpIgnoreSamePos) {
        FindCount = 0;
        for (int i=0; i<Count; i++) {
            Result[FindCount].IsPass = true;
            Result[FindCount].Scale  = results[i].GetScale();
            Result[FindCount].Angle  = results[i].GetAngle()*(-1);
            Result[FindCount].Px     = (float)(results[i].GetCenter().GetX()+IRoi->GetOrgX()-ImageCx+Ox);
            Result[FindCount].Py     = (float)(results[i].GetCenter().GetY()+IRoi->GetOrgY()-ImageCy+Oy);
            Result[FindCount].Id     = (float)i;
            Result[FindCount].Score  = (float)(results[i].GetScore()*100.0);
            Result[FindCount].Rect.left   = (long)(Result[FindCount].Px-TrImageWidth /2.0);
            Result[FindCount].Rect.top    = (long)(Result[FindCount].Py-TrImageHeight/2.0);
            Result[FindCount].Rect.right  = (long)(Result[FindCount].Px+TrImageWidth /2.0);
            Result[FindCount].Rect.bottom = (long)(Result[FindCount].Py+TrImageHeight/2.0);

            bool IsSamePos = false;
            for (int k=0; k<FindCount; k++) {
                float pPx = Result[k].Px;
                float pPy = Result[k].Py;
                float cPx = Result[FindCount].Px;
                float cPy = Result[FindCount].Py;

                if (Property.IgnoreSamePosX==0) Property.IgnoreSamePosX = 10;
                if (Property.IgnoreSamePosY==0) Property.IgnoreSamePosY = 10;

                if ((fabs(pPx-cPx)<Property.IgnoreSamePosX) && (fabs(pPy-cPy)<Property.IgnoreSamePosY)) {
                    IsSamePos = true;
                    IgnoreResult[k] = true;
                    break;
                }
            }

            if (IsSamePos) continue;


            // 설정된 값보다 작을 경우 Score 값을 0으로 Masking
            //----------------------------------------------------------------------
            if (Result[FindCount].Score<(Property.ZeroScore*100.0)) {
                Result[FindCount].Score  = 0;
            }

            if (Result[FindCount].Score>PvScore) {
                PvScore = Result[FindCount].Score;
                BestId = i;
            }
            FindCount++;
        }
    }
    else {
        for (int i=0; i<Count; i++) {
            Result[i].IsPass = true;
            Result[i].Scale  = results[i].GetScale();
            Result[i].Angle  = results[i].GetAngle()*(-1);
            Result[i].Px     = (results[i].GetCenter().GetX()+IRoi->GetOrgX());
            Result[i].Py     = (results[i].GetCenter().GetY()+IRoi->GetOrgY());
            Result[i].Id     = (float)i;
            Result[i].Score  = (float)(results[i].GetScore()*100.0);
            Result[i].Rect.left   = (long)(Result[i].Px-TrImageWidth /2.0);
            Result[i].Rect.top    = (long)(Result[i].Py-TrImageHeight/2.0);
            Result[i].Rect.right  = (long)(Result[i].Px+TrImageWidth /2.0);
            Result[i].Rect.bottom = (long)(Result[i].Py+TrImageHeight/2.0);

            // 설정된 값보다 작을 경우 Score 값을 0으로 Masking
            //----------------------------------------------------------------------
            if (Result[i].Score<(Property.ZeroScore*100.0)) {
                Result[i].Score  = 0;
            }

            if (Result[i].Score>PvScore) {
                PvScore = Result[i].Score;
                BestId = i;
            }
        }
    }

    Result[0].IsPass = Result[BestId].IsPass;
    Result[0].Scale  = Result[BestId].Scale ;
    Result[0].Angle  = Result[BestId].Angle ;
    Result[0].Px     = Result[BestId].Px    ;
    Result[0].Py     = Result[BestId].Py    ;
    Result[0].Id     = Result[BestId].Id    ;
    Result[0].Score  = Result[BestId].Score ;

    IsPass= true;

    return 1;
}
//------------------------------------------------------------------------------
/*
void CFind::OnLoad(bool IsLoad, CString Folder)
{
	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

    if (Folder=="") return;
    CString Path = Folder + "\\Find.ini";

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

    Ini.Load(IsLoad, Path, "Property", "TrImageWidth ", &TrImageWidth );
    Ini.Load(IsLoad, Path, "Property", "TrImageHeight", &TrImageHeight);

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

    //Train Roi
    Roi_x = TRoi->GetOrgX();
    Roi_y = TRoi->GetOrgY();
    Roi_w = TRoi->GetWidth();
    Roi_h = TRoi->GetHeight();

    Ini.Load(IsLoad, Path, "Property", "TRoi_x", &Roi_x);
    Ini.Load(IsLoad, Path, "Property", "TRoi_y", &Roi_y);
    Ini.Load(IsLoad, Path, "Property", "TRoi_w", &Roi_w);
    Ini.Load(IsLoad, Path, "Property", "TRoi_h", &Roi_h);

    Ini.Load(IsLoad, Path, "Property", "Ox", &Property.Ox);
    Ini.Load(IsLoad, Path, "Property", "Oy", &Property.Oy);
    Ini.Load(IsLoad, Path, "Property", "ZeroScore", &Property.ZeroScore);
    Ini.Load(IsLoad, Path, "Property", "IsUseIgnoreRoi", &Property.IsUseIgnoreRoi);
    Ini.Load(IsLoad, Path, "Property", "IsDarkIgnoreRoi", &Property.IsDarkIgnoreRoi);

    Ini.Load(IsLoad, Path, "Property", "OpIgnoreSamePos", &Property.OpIgnoreSamePos);
    Ini.Load(IsLoad, Path, "Property", "IgnoreSamePosX ", &Property.IgnoreSamePosX );
    Ini.Load(IsLoad, Path, "Property", "IgnoreSamePosY ", &Property.IgnoreSamePosY );

    TRoi->SetOrgX  (Roi_x);
    TRoi->SetOrgY  (Roi_y);
    TRoi->SetWidth (Roi_w);
    TRoi->SetHeight(Roi_h);

    try {
        CString PatternPath = Folder + "\\FindImage.fnd";
		CString FindImgPath = Folder + "\\Mark.bmp";

        if (IsLoad) {
            CFolder Folder;
            if (Folder.ExistsDir(PatternPath)) {
                PatternFinder->Load(LPSTR(LPCSTR(PatternPath)));
            }
        }
        else {
            bool IsLearnDone = PatternFinder->GetLearningDone();
            if (IsLearnDone) {
				PatternFinder->CopyLearntPattern(Train);
				Train.Save(LPSTR(LPCSTR(FindImgPath)));
                PatternFinder->Save(LPSTR(LPCSTR(PatternPath)));
            }
        }
    }
    catch (EException& exc)
    {
        MessageBox(NULL, exc.What().c_str(), "LearnAndFind", MB_OK | MB_ICONWARNING);
    }
}
*/
//------------------------------------------------------------------------------------
/*
void CFind::OnLoad(bool IsLoad, CString Folder, int Type, int Count)
{
	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

	if (Folder=="") return;
	CString Path = Folder + "\\Find.ini";

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

	Ini.Load(IsLoad, Path, "Property", "TrImageWidth ", &TrImageWidth );
	Ini.Load(IsLoad, Path, "Property", "TrImageHeight", &TrImageHeight);

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

	//Train Roi
	Roi_x = TRoi->GetOrgX();
	Roi_y = TRoi->GetOrgY();
	Roi_w = TRoi->GetWidth();
	Roi_h = TRoi->GetHeight();

	Ini.Load(IsLoad, Path, "Property", "TRoi_x", &Roi_x);
	Ini.Load(IsLoad, Path, "Property", "TRoi_y", &Roi_y);
	Ini.Load(IsLoad, Path, "Property", "TRoi_w", &Roi_w);
	Ini.Load(IsLoad, Path, "Property", "TRoi_h", &Roi_h);

	Ini.Load(IsLoad, Path, "Property", "Ox", &Property.Ox);
	Ini.Load(IsLoad, Path, "Property", "Oy", &Property.Oy);
	Ini.Load(IsLoad, Path, "Property", "ZeroScore", &Property.ZeroScore);
	Ini.Load(IsLoad, Path, "Property", "IsUseIgnoreRoi", &Property.IsUseIgnoreRoi);
	Ini.Load(IsLoad, Path, "Property", "IsDarkIgnoreRoi", &Property.IsDarkIgnoreRoi);

	Ini.Load(IsLoad, Path, "Property", "OpIgnoreSamePos", &Property.OpIgnoreSamePos);
	Ini.Load(IsLoad, Path, "Property", "IgnoreSamePosX ", &Property.IgnoreSamePosX );
	Ini.Load(IsLoad, Path, "Property", "IgnoreSamePosY ", &Property.IgnoreSamePosY );

	TRoi->SetOrgX  (Roi_x);
	TRoi->SetOrgY  (Roi_y);
	TRoi->SetWidth (Roi_w);
	TRoi->SetHeight(Roi_h);

	CString PatternPath ="";
	CString FindImgPath ="";

	try {
		if(Type==0)     {
			if     (Count==0 ) PatternPath = Folder + "\\FindImage0.fnd";
			else if(Count==1 ) PatternPath = Folder + "\\FindImage1.fnd";
			else if(Count==2 ) PatternPath = Folder + "\\FindImage2.fnd";
			else if(Count==3 ) PatternPath = Folder + "\\FindImage3.fnd";
			else if(Count==4 ) PatternPath = Folder + "\\FindImage4.fnd";
			else if(Count==5 ) PatternPath = Folder + "\\FindImage5.fnd";
			else if(Count==6 ) PatternPath = Folder + "\\FindImage6.fnd";
			else if(Count==7 ) PatternPath = Folder + "\\FindImage7.fnd";
			else if(Count==8 ) PatternPath = Folder + "\\FindImage8.fnd";
			else if(Count==9 ) PatternPath = Folder + "\\FindImage9.fnd";
			else if(Count==10) PatternPath = Folder + "\\FindImage10.fnd";
			else if(Count==11) PatternPath = Folder + "\\FindImage.fnd"  ;

// 			PatternPath = Folder + "\\FindImage.fnd";
			FindImgPath = Folder + "\\Mark.bmp";
		}
		else if(Type==1){
			PatternPath = Folder + "\\AFindImage.fnd";
			FindImgPath = Folder + "\\Mark.bmp";
		}

		if (IsLoad) {
			CFolder Folder;
			if (Folder.ExistsDir(PatternPath)) {
				PatternFinder->Load(LPSTR(LPCSTR(PatternPath)));
			}
		}
		else {
			bool IsLearnDone = PatternFinder->GetLearningDone();
			if (IsLearnDone) {
				PatternFinder->CopyLearntPattern(Train);
				Train.Save(LPSTR(LPCSTR(FindImgPath)));
				PatternFinder->Save(LPSTR(LPCSTR(PatternPath)));
			}
		}
	}
	catch (EException& exc)
	{
		MessageBox(NULL, exc.What().c_str(), "LearnAndFind", MB_OK | MB_ICONWARNING);
	}
}
*/
//-----------------------------------------------------------------------------------
/*
void CFind::OnLoad(bool IsLoad, CString Folder, int Type)
{
	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

	if (Folder=="") return;
	CString Path = Folder + "\\Find.ini";

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

	Ini.Load(IsLoad, Path, "Property", "TrImageWidth ", &TrImageWidth );
	Ini.Load(IsLoad, Path, "Property", "TrImageHeight", &TrImageHeight);

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

	//Train Roi
	Roi_x = TRoi->GetOrgX();
	Roi_y = TRoi->GetOrgY();
	Roi_w = TRoi->GetWidth();
	Roi_h = TRoi->GetHeight();

	Ini.Load(IsLoad, Path, "Property", "TRoi_x", &Roi_x);
	Ini.Load(IsLoad, Path, "Property", "TRoi_y", &Roi_y);
	Ini.Load(IsLoad, Path, "Property", "TRoi_w", &Roi_w);
	Ini.Load(IsLoad, Path, "Property", "TRoi_h", &Roi_h);

	Ini.Load(IsLoad, Path, "Property", "Ox", &Property.Ox);
	Ini.Load(IsLoad, Path, "Property", "Oy", &Property.Oy);
	Ini.Load(IsLoad, Path, "Property", "ZeroScore", &Property.ZeroScore);
	Ini.Load(IsLoad, Path, "Property", "IsUseIgnoreRoi", &Property.IsUseIgnoreRoi);
	Ini.Load(IsLoad, Path, "Property", "IsDarkIgnoreRoi", &Property.IsDarkIgnoreRoi);

	Ini.Load(IsLoad, Path, "Property", "OpIgnoreSamePos", &Property.OpIgnoreSamePos);
	Ini.Load(IsLoad, Path, "Property", "IgnoreSamePosX ", &Property.IgnoreSamePosX );
	Ini.Load(IsLoad, Path, "Property", "IgnoreSamePosY ", &Property.IgnoreSamePosY );

	TRoi->SetOrgX  (Roi_x);
	TRoi->SetOrgY  (Roi_y);
	TRoi->SetWidth (Roi_w);
	TRoi->SetHeight(Roi_h);

	CString PatternPath ="";
	CString FindImgPath ="";


	try {
		if(Type==0)     { //Notch Grind Mark Train Data 
			PatternPath = Folder + "\\FindImage.fnd";
			FindImgPath = Folder + "\\Mark.bmp";
		}
		else if(Type==1){ //Pad Grind Mark Train Data 
			PatternPath = Folder + "\\AFindImage.fnd";
			FindImgPath = Folder + "\\Fad_Mark.bmp";
		}
		else if(Type==2){ //A_Side Grind Mark Train Data 
			PatternPath = Folder + "\\ASIDE_FindImage.fnd";
			FindImgPath = Folder + "\\ASIDE_Mark.bmp";
		}


		if (IsLoad) {
			CFolder Folder;
			if (Folder.ExistsDir(PatternPath)) {
				PatternFinder->Load(LPSTR(LPCSTR(PatternPath)));
			}
		}
		else {
			bool IsLearnDone = PatternFinder->GetLearningDone();
			if (IsLearnDone) {
				PatternFinder->CopyLearntPattern(Train);
				Train.Save(LPSTR(LPCSTR(FindImgPath)));
				PatternFinder->Save(LPSTR(LPCSTR(PatternPath)));
			}
		}
	}
	catch (EException& exc)
	{
		MessageBox(NULL, exc.What().c_str(), "LearnAndFind", MB_OK | MB_ICONWARNING);
	}
}
*/
//------------------------------------------------------------------------------
void CFind::OnLoad(bool IsLoad, CString Folder, int Type)
{
	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

	if (Folder=="") return;
	CString Path = Folder + "\\Find.ini";

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

//	int atemp;
	Ini.Load(IsLoad, Path, "Property", "TrImageWidth ", &TrImageWidth);
	Ini.Load(IsLoad, Path, "Property", "TrImageHeight", &TrImageHeight);

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

	//Train Roi
	Roi_x = TRoi->GetOrgX();
	Roi_y = TRoi->GetOrgY();
	Roi_w = TRoi->GetWidth();
	Roi_h = TRoi->GetHeight();

	Ini.Load(IsLoad, Path, "Property", "TRoi_x", &Roi_x);
	Ini.Load(IsLoad, Path, "Property", "TRoi_y", &Roi_y);
	Ini.Load(IsLoad, Path, "Property", "TRoi_w", &Roi_w);
	Ini.Load(IsLoad, Path, "Property", "TRoi_h", &Roi_h);

	Ini.Load(IsLoad, Path, "Property", "Ox", &Property.Ox);
	Ini.Load(IsLoad, Path, "Property", "Oy", &Property.Oy);
	Ini.Load(IsLoad, Path, "Property", "ZeroScore", &Property.ZeroScore);
	Ini.Load(IsLoad, Path, "Property", "IsUseIgnoreRoi", &Property.IsUseIgnoreRoi);
	Ini.Load(IsLoad, Path, "Property", "IsDarkIgnoreRoi", &Property.IsDarkIgnoreRoi);

	Ini.Load(IsLoad, Path, "Property", "OpIgnoreSamePos", &Property.OpIgnoreSamePos);
	Ini.Load(IsLoad, Path, "Property", "IgnoreSamePosX ", &Property.IgnoreSamePosX );
	Ini.Load(IsLoad, Path, "Property", "IgnoreSamePosY ", &Property.IgnoreSamePosY );

	TRoi->SetOrgX  (Roi_x);
	TRoi->SetOrgY  (Roi_y);
	TRoi->SetWidth (Roi_w);
	TRoi->SetHeight(Roi_h);

	CString PatternPath ="";
	CString FindImgPath ="";
	CString FileName    ="";
	CString ImgName     ="";

	
	try {
	    if (Type == -1) { //Calibration or Rotate Axis 
			PatternPath = Folder + "\\CalMark.fnd";
			FindImgPath = Folder + "\\CalMark.bmp";
	    }
	    else            { //Type 
			FileName.Format(L"\\FindImage_%d.fnd", Type); //Train File Name 
			ImgName.Format (L"\\Find_Mark_%d.bmp", Type); //Image File Name  
			PatternPath = Folder + FileName;
			FindImgPath = Folder + ImgName ;
		}

		if (IsLoad) {
			CFolder Folder;
			if (Folder.ExistsDir(PatternPath)) {
				PatternFinder->Load(CW2A(PatternPath).m_psz);
			}
		}
		else {
			bool IsLearnDone = PatternFinder->GetLearningDone();
			if (IsLearnDone) {
				PatternFinder->CopyLearntPattern(Train);
				Train.Save(CW2A(FindImgPath).m_psz);
				PatternFinder->Save(CW2A(PatternPath).m_psz);
			}
		}
	}
	catch (EException & exc)
	{
		MessageBox(NULL, CA2W(exc.What().c_str()), L"LearnAndFind", MB_OK | MB_ICONWARNING);
	}

//	try {
//		if(Type==0)     { //Grind Mark Train Data_1
//// 			PatternPath = Folder + "\\FindImage_1.fnd";
//			PatternPath = Folder + "\\FindImage_0.fnd";
//			FindImgPath = Folder + "\\Mark_0.bmp";
//		}
//		else if(Type==1){ //Grind Mark Train Data_2
//			PatternPath = Folder + "\\FindImage_1.fnd";
//			FindImgPath = Folder + "\\Mark_1.bmp";
//		}
//		else if(Type==2){ //Grind Mark Train Data_2
//			PatternPath = Folder + "\\FindImage_2.fnd";
//			FindImgPath = Folder + "\\Mark_2.bmp";
//		}
//		else if(Type==3){ //Grind Mark Train Data_2
//			PatternPath = Folder + "\\FindImage_3.fnd";
//			FindImgPath = Folder + "\\Mark_3.bmp";
//		}
//		else if(Type==4){ //Grind Mark Train Data_2
//			PatternPath = Folder + "\\FindImage_4.fnd";
//			FindImgPath = Folder + "\\Mark_4.bmp";
//		}
//		else if(Type==5){ //Calibration Train Mark 
//			PatternPath = Folder + "\\CalMark.fnd";
//			FindImgPath = Folder + "\\CalMark.bmp";
//		}
//
//		if (IsLoad) {
//			CFolder Folder;
//			if (Folder.ExistsDir(PatternPath)) {
//				PatternFinder->Load(CW2A(PatternPath).m_psz);
//			}
//		}
//		else {
//			bool IsLearnDone = PatternFinder->GetLearningDone();
//			if (IsLearnDone) {
//				PatternFinder->CopyLearntPattern(Train);
//				Train.Save(CW2A(FindImgPath).m_psz);
//				PatternFinder->Save(CW2A(PatternPath).m_psz);
//			}
//		}
//	}
//	catch (EException& exc)
//	{
//		MessageBox(NULL, CA2W(exc.What().c_str()), L"LearnAndFind", MB_OK | MB_ICONWARNING);
//	}
}
//------------------------------------------------------------------------------

TPoint CFind::OnRotate(TPoint c, TPoint t, double deg)
{
    return OnRotate(c.x, c.y, t.x, t.y, deg);
}
//------------------------------------------------------------------------------

TPoint CFind::OnRotate(double cx, double cy, double tx, double ty, double deg)
{
    TPoint np;
    double Rad = deg * M_PI/180.0;
    double COS = cos(Rad);
    double SIN = sin(Rad);

    if (deg ==  90) {COS =  0; SIN =  1;}
    if (deg == 180) {COS = -1; SIN =  0;}
    if (deg == 270) {COS =  0; SIN = -1;}
    if (deg == 360) {COS =  1; SIN =  0;}

    np.x = (int)((tx-cx)*COS - (ty-cy)*SIN);
    np.y = (int)((tx-cx)*SIN + (ty-cy)*COS);
    np.x +=(int)cx;
    np.y +=(int)cy;

    return np;
}
//------------------------------------------------------------------------------

void CFind::OnPaintResult(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->CPen.Style = 0;
    Gdi->CFont.Height = 28;
	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    int x = IRoi->GetOrgX();
    int y = IRoi->GetOrgY();
    CString Txt = IsPass ? L"PASS" : L"FAIL";
    Gdi->CText.Color = IsPass ? BLUE_COLOR : RED_COLOR;
    Gdi->OnText((x+20), (y+60), CW2A(Txt));

    delete Gdi;
}
void CFind::OnPaint(TDrawTool Tool, int RColor)
{
    HDC hdc = ::GetDC(Tool.Handle);

    int Count = results.size();

    for (int i=0; i<Count; i++) {
		results[i].SetDrawBoundingBox  (Property.DrawBoundingBox  );
		results[i].SetDrawCenter       (Property.DrawCenter       );
		results[i].SetDrawFeaturePoints(Property.DrawFeaturePoints);

        results[i].Draw(hdc, RED_COLOR, Tool.FHs, Tool.FVs);
    }
    ReleaseDC(Tool.Handle, hdc);
}
void CFind::OnPaintPattern(TDrawTool Tool)
{
	if (Tool.Handle==NULL) return;
	if (PatternFinder==NULL) return;
	if (!PatternFinder->GetLearningDone()) return;

	EImageBW8 ROI;
	PatternFinder->CopyLearntPattern(ROI);

	if (ROI.GetWidth()<1) return;
	if (ROI.GetHeight()<1) return;

	HDC hdc = NULL;

	hdc = GetDC(Tool.Handle);

	float wr = Tool.FHs;
	float vr = Tool.FVs;

	ROI.Draw(hdc, wr, vr);

	ReleaseDC(Tool.Handle, hdc);
}

