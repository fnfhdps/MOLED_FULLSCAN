// LxFilter.cpp: implementation of the CSeed class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LxFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool IsEvtPrcessMessage;

CString PROCESS_MESSAGE;
void SetProcessMessage(CString Msg)
{
	IsEvtPrcessMessage = true;
	PROCESS_MESSAGE = Msg;
}
CString GetProcessMessage()
{
	return PROCESS_MESSAGE;
}

//---------------------------------------------------------------
CSeed::CSeed()
{
//  IRoi = new EROIBW8();
// 	TRoi = new EROIBW8();
// 	FRoi = new EROIBW8();
// 	MRoi = new EROIBW8();
}

CSeed::~CSeed()
{
//	delete (IRoi);
}

TFPoint CSeed::GetIntersectPointWithImageLeft(TFPoint P0, TFPoint P1)
{
    TFPoint FPoint;
	
    int ImgW = GImage->GetWidth();
    int ImgH = GImage->GetHeight();
	
    TFPoint LT(0,                     0);
    TFPoint RT((float)ImgW,           0);
    TFPoint RB((float)ImgW, (float)ImgH);
    TFPoint LB(0          , (float)ImgH);
	
    return FPoint = Math.OnIntersect(P0, P1, LT, LB);
}

TFPoint CSeed::GetIntersectPointWithImageTop(TFPoint P0, TFPoint P1)
{
    TFPoint FPoint;
	
    int ImgW = GImage->GetWidth();
    int ImgH = GImage->GetHeight();
	
    TFPoint LT(0          ,           0);
    TFPoint RT((float)ImgW,           0);
    TFPoint RB((float)ImgW, (float)ImgH);
    TFPoint LB(0          , (float)ImgH);
	
    return FPoint = Math.OnIntersect(P0, P1, LT, RT);
}

TFPoint CSeed::GetIntersectPointWithImageRight(TFPoint P0, TFPoint P1)
{
    TFPoint FPoint;
	
    int ImgW = GImage->GetWidth();
    int ImgH = GImage->GetHeight();
	
    TFPoint LT(0          ,           0);
    TFPoint RT((float)ImgW,           0);
    TFPoint RB((float)ImgW, (float)ImgH);
    TFPoint LB(0          , (float)ImgH);
	
    return FPoint = Math.OnIntersect(P0, P1, RT, RB);
}
TFPoint CSeed::GetIntersectPointWithImageBottom(TFPoint P0, TFPoint P1)
{
    TFPoint FPoint;
	
    int ImgW = GImage->GetWidth();
    int ImgH = GImage->GetHeight();
	
    TFPoint LT(0          ,           0);
    TFPoint RT((float)ImgW,           0);
    TFPoint RB((float)ImgW, (float)ImgH);
    TFPoint LB(0          , (float)ImgH);
	
    return FPoint = Math.OnIntersect(P0, P1, LB, RB);
}
void CSeed::OnLoadRoi(bool IsLoad, CString Path, EROIBW8 *lpRoi)
{
    CIFile Ini;

    if (Path.Trim()=="") return;

    //Inspection Roi
    int Roi_x = lpRoi->GetOrgX();
    int Roi_y = lpRoi->GetOrgY();
    int Roi_w = lpRoi->GetWidth();
    int Roi_h = lpRoi->GetHeight();

    Ini.Load(IsLoad, Path, "Property", "IRoi_x", &Roi_x);
    Ini.Load(IsLoad, Path, "Property", "IRoi_y", &Roi_y);
    Ini.Load(IsLoad, Path, "Property", "IRoi_w", &Roi_w);
    Ini.Load(IsLoad, Path, "Property", "IRoi_h", &Roi_h);

    lpRoi->SetOrgX  (Roi_x);
    lpRoi->SetOrgY  (Roi_y);
    lpRoi->SetWidth (Roi_w);
    lpRoi->SetHeight(Roi_h);
}
//------------------------------------------------------------------------------
bool CFilter::OnFilter(TFilterType FilterType, EImageBW8 *Org, EImageBW8 *New)
{
    try {
        int n = FilterType;

        switch (n) {
			case ftErodeDisk       : EasyImage::ErodeDisk      (Org, New, 1); return true; //minimum of the gray values in a defined neighborhood
			case ftDilateDisk      : EasyImage::DilateDisk     (Org, New, 1); return true; //maximum of the gray values in a defined neighborhood
			case ftMedian          : EasyImage::Median         (Org, New   ); return true; //median of the gray values in a 3x3 neighborhood
			case ftCloseDisk       : EasyImage::CloseDisk      (Org, New, 1); return true; //Performs a closing on an image 
			case ftOpenDisk        : EasyImage::OpenDisk       (Org, New, 1); return true; //Performs a opening on an image 
			case ftConvolGaussian  : EasyImage::ConvolGaussian (Org, New, 1); return true; //Applies Gaussian filtering
			case ftConvolLowpass1  : EasyImage::ConvolLowpass1 (Org, New   ); return true; //Au¨¬¨¢CEAI(AuAO¨¡A Ae¡Æu CEAI) 
			case ftConvolHighpass1 : EasyImage::ConvolHighpass1(Org, New   ); return true; //©öI¨¬¨¢CEAI(¡ÆiAO¨¡A Ae¡Æu CEAI) 

        }
        return false;
    }
    catch (EException& exc)
    {
        MessageBox(NULL, (CString)exc.What().c_str(), L"bool CFilter::OnExecute(TFilterType FilterType, ", MB_OK || MB_ICONWARNING);
        return false;
    }
}
//------------------------------------------------------------------------------
bool CFilter::OnFilter(TFilterType FilterType, EROIBW8 *Org, EROIBW8 *New)
{
	try {
		int n = FilterType;

		switch (n) {
		    case ftErodeDisk       : EasyImage::ErodeDisk      (Org, New, 1); return true; //minimum of the gray values in a defined neighborhood
		    case ftDilateDisk      : EasyImage::DilateDisk     (Org, New, 1); return true; //maximum of the gray values in a defined neighborhood
		    case ftMedian          : EasyImage::Median         (Org, New   ); return true; //median of the gray values in a 3x3 neighborhood
		    case ftCloseDisk       : EasyImage::CloseDisk      (Org, New, 1); return true; //Performs a closing on an image 
		    case ftOpenDisk        : EasyImage::OpenDisk       (Org, New, 1); return true; //Performs a opening on an image 
		    case ftConvolGaussian  : EasyImage::ConvolGaussian (Org, New, 1); return true; //Applies Gaussian filtering
		    case ftConvolLowpass1  : EasyImage::ConvolLowpass1 (Org, New   ); return true; //Au¨¬¨¢CEAI(AuAO¨¡A Ae¡Æu CEAI) 
		    case ftConvolHighpass1 : EasyImage::ConvolHighpass1(Org, New   ); return true; //©öI¨¬¨¢CEAI(¡ÆiAO¨¡A Ae¡Æu CEAI) 
		}
		return false;
	}
	catch (EException& exc)
	{
		MessageBox(NULL, (CString)exc.What().c_str(), L"bool CFilter::OnExecute(TFilterType FilterType, ", MB_OK || MB_ICONWARNING);
		return false;
	}
}

