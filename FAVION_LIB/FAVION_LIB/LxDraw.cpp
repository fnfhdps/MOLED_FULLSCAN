#include "stdafx.h"
#include "LxDraw.h"

CDraw::CDraw(void)
{
}
CDraw::~CDraw(void)
{
}
//------------------------------------------------------------------------------

float CDraw::GetHoCtPos(EImageBW8 *lpImage, float Value)
{
	int Width = lpImage->GetWidth();

	float hw = Width/2.0;
	if (Value>0) {
		float __Value = Value-hw;
		return __Value;
	}
	return 0.0f;
}
//------------------------------------------------------------------------------

float CDraw::GetVtCtPos(EImageBW8 *lpImage, float Value)
{
	int Height = lpImage->GetHeight();

	float hh = Height/2.0;
	if (Value>0) {
		float __Value = hh-Value;
		return __Value;
	}
	return 0.0f;
}
//-------------------------------------------------------------------------------
void CDraw::OnCrossLine(bool IsDraw, TDrawTool Tool, EImageBW8 *lpImage, int Mx, int My)
{
	if (Tool.Handle==NULL) return;
	//HGdi::HGdi(HWND h) { hwnd = h; hdc = GetDC(hwnd); Init(); }

	HGdi *Gdi = new HGdi(Tool.Handle);

	int Width  = lpImage->GetWidth ();
	int Height = lpImage->GetHeight();

	if (IsDraw) {

		Gdi->FVx = Tool.FVx;
		Gdi->FVy = Tool.FVy;
		Gdi->FHs = Tool.FHs;
		Gdi->FVs = Tool.FVs;
		Gdi->CPen.Color = Tool.Color;

		Gdi->CPen.Style = 2;
		Gdi->CBrush.Style = BS_SOLID;
		int x1 =  Mx     ;
		int y1 =  0      ;
		int x2 =  Mx     ;
		int y2 = Height  ;
		int x3 =  0      ;
		int y3 =  My     ;
		int x4 = Width   ;
		int y4 =  My     ;

		Gdi->OnLine(x1,y1,x2,y2);
		Gdi->OnLine(x3,y3,x4,y4);
		Gdi->CBrush.Style = BS_SOLID;
	}

	delete Gdi;
}
//--------------------------------------------------------------------
void CDraw::OnImage(TDrawTool Tool, EImageBW8 *lpImage)
{
	CRect Rect;
	GetWindowRect(Tool.Handle , &Rect);
	OnImage(Tool, lpImage, Rect.Width(), Rect.Height()); 
}
//--------------------------------------------------------------
void CDraw::OnImage(TDrawTool Tool, EImageBW8 *lpImage, int w, int h)
{
	if (IsBusy) return;
	IsBusy = true;

	if (Tool.Handle==NULL) { IsBusy = false; return; }
	if (lpImage==NULL) {
		RedrawWindow(Tool.Handle, NULL, NULL, RDW_UPDATENOW|RDW_INVALIDATE);
		IsBusy = false;
		return;
	}
	if (!lpImage->GetImagePtr()) {
		RedrawWindow(Tool.Handle, NULL, NULL, RDW_UPDATENOW|RDW_INVALIDATE);
		IsBusy = false;
		return;
	}

	int Width  = lpImage->GetWidth();
	int Height = lpImage->GetHeight();

	HDC hdc = GetDC(Tool.Handle);
	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

// 	Gdi->FHs = ((float)w/(float)Width );
// 	Gdi->FVs = ((float)h/(float)Height);

	bool IsDrawDirect    = false;
	bool IsDrawTeachRoi  = true;
	bool IsDrawIpCntArea = true;
	bool IsDrawShowRoi   = true;
	bool IsDrawBox       = true;

	int xCntAreaOffset = 7;
	int yCntAreaOffset = 7;

	if (!IsDrawDirect) {
		//lpImage->Draw(hdc, FHs, FVs);
		lpImage->Draw(hdc, Gdi->FHs, Gdi->FVs);
	}
	else {
		Gdi->DisplayColor = DisplayColor;
		//Gdi->OnDrawDirect(Gdi->GetGdiFVx(), Gdi->GetGdiFVy(), Width, Height, 8, lpImage->GetImagePtr(), IsFalseColor, 0, 0, 255, 0);
	}

	bool DwCrossLine = true;
	if (DwCrossLine) {
		Gdi->CPen.Color = RED_COLOR;
		Gdi->CPen.Style = 2;
		Gdi->CBrush.Style = BS_SOLID;
		int x1 = Width/2.0 ;
		int y1 =  0        ;
		int x2 = Width/2.0 ;
		int y2 = Height    ;
		int x3 =  0        ;
		int y3 = Height/2.0;
		int x4 = Width     ;
		int y4 = Height/2.0;

		Gdi->OnLine(x1,y1,x2,y2);
		Gdi->OnLine(x3,y3,x4,y4);
		Gdi->CBrush.Style = BS_SOLID;
	}

	Gdi->CText.Color = YELLOW_COLOR;
	Gdi->CFont.Height = 28;
	Gdi->CPen.Style = 0;

	Gdi->CPen.Style = 2;
	Gdi->CPen.Color = RED_COLOR;
	Gdi->CPen.Width = 4;
	Gdi->CPen.Style = BS_SOLID;

	delete Gdi;
	ReleaseDC(Tool.Handle, hdc);

	IsBusy = false;
}
//------------------------------------------------------------------------------
void CDraw::OnCrossCenterLine(TDrawTool Tool, EImageBW8 *lpImage, int Mx, int My)
{
	if (Tool.Handle==NULL) return;
	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

	Gdi->CBrush.Style = BS_SOLID;
	CRect Rect;
	GetWindowRect(Tool.Handle, &Rect);

	int Width  = Rect.Width();
	int Height = Rect.Height();
	
	int x1 =  Width/2;
	int y1 =  0      ;
	int x2 = Width/2 ;
	int y2 = Height  ;
	int x3 =  0      ;
	int y3 = Height/2;
	int x4 = Width   ;
	int y4 = Height/2;

	Gdi->OnMove(x1, y1);
	Gdi->OnLine(x2, y2);

	Gdi->OnMove(x3, y3);
	Gdi->OnLine(x4, y4);

	delete Gdi;
}
void CDraw::OnCrossMark(bool IsDraw, TDrawTool Tool, EImageBW8 *lpImage, int cx, int cy, int size, COLORREF Color)
{
    if (Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    int Width  = lpImage->GetWidth ();
    int Height = lpImage->GetHeight();

    if (IsDraw) {
        Gdi->CPen.Color = Color;
        Gdi->CPen.Style = 0;
        Gdi->CBrush.Style = BS_SOLID;

        Gdi->OnRect(false, cx-(size*2), cy-(size*2), cx+(size*2), cy+(size*2));
    }
    delete Gdi;
}
//------------------------------------------------------------------------------
void CDraw::OnWindowSize(TDrawTool Tool, EImageBW8 *lpImage)
{
	CRect Rect;
	GetWindowRect(Tool.Handle , &Rect);
    OnWindowSize(Tool, lpImage, Rect.Width(), Rect.Height());
}
//------------------------------------------------------------------------------

void CDraw::OnWindowSize(TDrawTool Tool, EImageBW8 *lpImage, int w, int h)
{
    if (Tool.Handle==NULL) return;
    if (!lpImage->GetImagePtr()) {
        return;
    }

    int Width = lpImage->GetWidth();
    int Height = lpImage->GetHeight();

    HDC hdc = GetDC(Tool.Handle);
    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    lpImage->Draw(hdc, Gdi->FHs, Gdi->FVs);
	lpImage->GetPixel(w,h);
    //Gdi->OnDrawDirect(FVx, FVy, Width, Height, 8, lpImage->GetImagePtr(), false, 0, 0, 255, 0);

    /*
    bool DwCrossLine = true;
    if (DwCrossLine) {
        Gdi->CPen.Color = clRed;
        Gdi->CPen.Style = 2;
        Gdi->CBrush.Style = BS_SOLID;
        int x1 = Width/2.0 ;
        int y1 =  0        ;
        int x2 = Width/2.0 ;
        int y2 = Height    ;
        int x3 =  0        ;
        int y3 = Height/2.0;
        int x4 = Width     ;
        int y4 = Height/2.0;

        Gdi->OnLine(x1,y1,x2,y2);
        Gdi->OnLine(x3,y3,x4,y4);
        Gdi->CBrush.Style = BS_SOLID;
    }
    */

    //FHs = OldFHs;
    //FVs = OldFVs;

    delete Gdi;
    ReleaseDC(Tool.Handle, hdc);
}
//------------------------------------------------------------------------------
void CDraw::OnImageBinary(TDrawTool Tool, EImageBW8 *lpImage, int w, int h, int ThHold)
{
    if (Tool.Handle==NULL) return;
    if (!lpImage->GetImagePtr()) return;

    int Width = lpImage->GetWidth();
    int Height = lpImage->GetHeight();

    HDC hdc = GetDC(Tool.Handle);
    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    bool IsDrawDirect    = false;
    bool IsDrawTeachRoi  = true;
    bool IsDrawIpCntArea = true;
    bool IsDrawShowRoi   = true;
    bool IsDrawBox       = true;

    int xCntAreaOffset = 7;
    int yCntAreaOffset = 7;

    EImageBW8 BImage;

    if (!IsDrawDirect) {
        BImage.SetSize(lpImage->GetWidth(), lpImage->GetHeight());
        EasyImage::Threshold(lpImage, &BImage, ThHold);
        BImage.Draw(hdc, Gdi->FHs, Gdi->FVs);
    }
    else {
        //Gdi->OnDrawDirect(0, 0, Width, Height, 8, BImage.GetImagePtr(), false, 0, 0, 255, 0);
    }

    bool DwCrossLine = true;
    if (DwCrossLine) {
        Gdi->CPen.Color = LIME_COLOR;
        Gdi->CPen.Style = 2;
        Gdi->CBrush.Style = BS_SOLID;

        int x1 = Width/2.0 ;
        int y1 =  0        ;
        int x2 = Width/2.0 ;
        int y2 = Height    ;
        int x3 =  0        ;
        int y3 = Height/2.0;
        int x4 = Width     ;
        int y4 = Height/2.0;

        Gdi->OnLine(x1,y1,x2,y2);
        Gdi->OnLine(x3,y3,x4,y4);
        Gdi->CBrush.Style = BS_SOLID;
    }

    Gdi->CText.Color = YELLOW_COLOR;
    Gdi->CFont.Height = 28;
    Gdi->CPen.Style = 0;

    Gdi->CFont.Height = 12;
    int ys = 100;
    int yd = 15;
    CString Txt;

//    Gdi->CText.Color = clFuchsia; Gdi->OnText(50, ys + yd*2, Txt.c_str()); Txt = AnsiString().sprintf("GRAB %3.3f ms", Time.Trig);
//    Gdi->CText.Color = clYellow ; Gdi->OnText(50, ys + yd*3, Txt.c_str()); Txt = AnsiString().sprintf("CIRC %3.3f ms", Time.Disp);
//    Gdi->CText.Color = clNavy   ; Gdi->OnText(50, ys + yd*4, Txt.c_str()); Txt = EnGrab ? "CAMERA : EXIST" : "CAMERA : NO EXIST";
//    Gdi->CText.Color = clPurple ; Gdi->OnText(50, ys + yd*5, Txt.c_str()); Txt = AnsiString().sprintf("HORI ZOOM %3.2f %", (OldFHs*100.0));
//    Gdi->CText.Color = clTeal   ; Gdi->OnText(50, ys + yd*6, Txt.c_str()); Txt = AnsiString().sprintf("VERT ZOOM %3.2f %", (OldFVs*100.0));
	//GOO 
    //Gdi->CText.Color = clTeal   +random(256); Gdi->OnText(50, ys + yd*7, Txt.c_str());
	Gdi->CText.Color = LIME_COLOR;  Gdi->OnText(50, ys + yd*7, Txt);

    Gdi->CPen.Style = 2;
    Gdi->CPen.Color = LIME_COLOR;
    Gdi->CPen.Width = 4;
    Gdi->CPen.Style = BS_SOLID;

    delete Gdi;
    ReleaseDC(Tool.Handle, hdc);
}
//------------------------------------------------------------------------------
/*
void CDraw::OnImageBinary(CWnd *target1, CWnd *target2, EImageBW8 *lpImage, int ThHold)
{
    if (target1==NULL) return;
    if (target2==NULL) return;

    int IWidth = lpImage->GetWidth();
    int IHeight = lpImage->GetHeight();
    int TWidth = Table->Width;
    int THeight = Table->Height;

    if (IWidth  <1) return;
    if (IHeight <1) return;
    if (TWidth  <1) return;
    if (THeight <1) return;

    Table->Top    = 0;
    Table->Height = Base->Height;
    Table->Width  = (float)((float)Base->Height/(float)lpImage->GetHeight())*(float)lpImage->GetWidth();
    Table->Left   = (Base->Width-Table->Width)/2.0;

    return OnImageBinary(Table->Handle, lpImage, Table->Width, Table->Height, ThHold);
}
*/
//------------------------------------------------------------------------------
void CDraw::OnRoi(TDrawTool Tool, EImageBW8 *lpImage, EROIBW8 *lpRoi, COLORREF Color)
{
	HDC hdc = GetDC(Tool.Handle);
	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    HPEN hNewPen, hOldPen;
    hNewPen = CreatePen(1, 1, Color);
    hOldPen = (HPEN)SelectObject(hdc, hNewPen);
    SetROP2   (hdc, R2_COPYPEN);
    SetBkMode (hdc, TRANSPARENT);

    lpRoi->SetTitle("ROI");
    lpRoi->Detach();
    lpRoi->Attach(lpImage);
    if (lpRoi->GetWidth ()<5) lpRoi->SetPlacement(100, 100, 200, 200);
    if (lpRoi->GetHeight()<5) lpRoi->SetPlacement(100, 100, 200, 200);

    lpRoi->DrawFrameWithCurrentPen(hdc, EFramePosition_On, true, Gdi->FHs, Gdi->FVs);

    SelectObject(hdc, hOldPen);
    DeleteObject(hNewPen);

	delete Gdi;
	ReleaseDC(Tool.Handle, hdc);
}
//------------------------------------------------------------------------------
// 화면을 직접 클릭해서 움직이는 방식이기때문에 현재 라이브러리 작업중에서 화면좌표를 얻어올수 없어서 일시적으로 주석처리함 
/*
void CDraw::DrawImage(TDrawTool Tool, EImageBW8 *lpImage, EROIBW8 *IspRoi, COLORREF IspColor, EROIBW8 *TrnRoi, COLORREF TrnColor, bool drawROI)
{
	if (lpImage->IsVoid()) return;

	// 핸들에서 윈도우 포인터 얻어오기
	CWnd *pWnd = CWnd::FromHandle(Tool.Handle);

	CClientDC dc(pWnd);
	HDC hdc = dc.m_hDC;

	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

	IspRoi->Detach();
	IspRoi->Attach(lpImage);
	if (IspRoi->GetWidth ()<5) IspRoi->SetPlacement(100, 100, 300, 300);
	if (IspRoi->GetHeight()<5) IspRoi->SetPlacement(100, 100, 300, 300);

	TrnRoi->Detach();
	TrnRoi->Attach(lpImage);
	if (TrnRoi->GetWidth ()<5) TrnRoi->SetPlacement(100, 100, 200, 200);
	if (TrnRoi->GetHeight()<5) TrnRoi->SetPlacement(100, 100, 200, 200);

	//////////////////////////////////////////////////////////////////////////

	CImage m_Image;
	int w, h, bpp;
	BYTE* ptr = (BYTE*)lpImage->GetImagePtr();
	w = lpImage->GetWidth();
	h = lpImage->GetHeight();
	bpp = lpImage->GetBitsPerPixel();
	m_Image.Create(w,h,bpp);

	int lineSize;
	if (bpp == 8)
		lineSize = w * 1;
	else if (bpp == 24)
		lineSize = w * 1 * 3;

	if (bpp == 8)
	{
		// Define the color table
		RGBQUAD* tab = new RGBQUAD[256];
		for (int i = 0; i < 256; ++i)
		{
			tab[i].rgbRed = i;
			tab[i].rgbGreen = i;
			tab[i].rgbBlue = i;
			tab[i].rgbReserved = 0;
		}
		m_Image.SetColorTable(0, 256, tab);
		delete[] tab;
	}

	for ( int i = 0; i < h; i++ )
	{
		const void* src;
		void*       dst = m_Image.GetPixelAddress(0, i);
		if (bpp == 8)
			src = ptr + (i*lpImage->GetRowPitch());
		else if (bpp == 24)
			src = ptr + (i*w*3);

		memcpy(dst, src, lineSize);
	}

	CDC		Memdc; 
	CDC		ImageDC;
	CBitmap bmpbkg;
	CBitmap *pOldBmp;

	CRect rect;
	GetWindowRect(Tool.Handle, &rect); // 윈도우 핸들에서 영역의 크기 얻어오기 

	CRect Isp_Rect;
	Isp_Rect.left	= IspRoi->GetOrgX() * Gdi->FHs - pFAVION->m_ptResult.x * Gdi->FHs;
	Isp_Rect.top	= IspRoi->GetOrgY() * Gdi->FVs - pFAVION->m_ptResult.y * Gdi->FVs;
	Isp_Rect.right	= Isp_Rect.left	    + IspRoi->GetWidth()  * Gdi->FHs;
	Isp_Rect.bottom = Isp_Rect.top	    + IspRoi->GetHeight() * Gdi->FVs;

	CRect Trn_Rect;
	Trn_Rect.left	= TrnRoi->GetOrgX() * Gdi->FHs - pFAVION->m_ptResult.x * Gdi->FHs;
	Trn_Rect.top	= TrnRoi->GetOrgY() * Gdi->FVs - pFAVION->m_ptResult.y * Gdi->FVs;
	Trn_Rect.right	= Trn_Rect.left	    + TrnRoi->GetWidth()  * Gdi->FHs;
	Trn_Rect.bottom = Trn_Rect.top	    + TrnRoi->GetHeight() * Gdi->FVs;

	CDC *pCDC = CDC::FromHandle(hdc);

	Memdc.CreateCompatibleDC(pCDC); 
	ImageDC.CreateCompatibleDC(pCDC);

	bmpbkg.CreateCompatibleBitmap(pCDC,rect.Width(),rect.Height());
	pOldBmp = (CBitmap*)Memdc.SelectObject(bmpbkg);

	ImageDC.SelectObject(m_Image);
	Memdc.SetStretchBltMode(STRETCH_HALFTONE);

	if (w < rect.Width()/Gdi->FHs && h < rect.Height()/Gdi->FVs)
	{
		Memdc.StretchBlt(
			0,
			0,
			w*Gdi->FHs,	
			h*Gdi->FVs,	
			&ImageDC,
			0,
			0,
			w,
			h,
			SRCCOPY
			);
	}
	else if (w < rect.Width()/Gdi->FHs)
	{
		Memdc.StretchBlt(
			0,
			0,
			w*Gdi->FHs,
			rect.Height(),
			&ImageDC,
			0,
			pFAVION->m_ptResult.y,
			w,
			rect.Height()/Gdi->FVs,
			SRCCOPY
			);
	}
	else if (h < rect.Height()/Gdi->FVs)
	{
		Memdc.StretchBlt(
			0,
			0,
			rect.Width(),
			h*Gdi->FVs,
			&ImageDC,
			pFAVION->m_ptResult.x,
			0,
			rect.Width()/Gdi->FHs,
			h,
			SRCCOPY
			);
	}
	else
	{
		Memdc.StretchBlt(
			0,
			0,
			rect.Width(),
			rect.Height(),
			&ImageDC,
			pFAVION->m_ptResult.x,
			pFAVION->m_ptResult.y,
			rect.Width() /Gdi->FHs,
			rect.Height()/Gdi->FVs,
			SRCCOPY
			);
	}

	//////////////////////////////////////////////////////////////////////////

	CBrush Ispbrush(IspColor);
	CBrush Trnbrush(TrnColor);
	CBrush *pOldIspBrush;
	CBrush *pOldTrnBrush;

	if (drawROI)
	{
		pOldIspBrush = Memdc.SelectObject(&Ispbrush);	
		Memdc.FrameRect(Isp_Rect,&Ispbrush);

		pOldTrnBrush = Memdc.SelectObject(&Trnbrush);
		Memdc.FrameRect(Trn_Rect,&Trnbrush);	
	}

	pCDC->BitBlt(0,0,rect.Width(),rect.Height(),&Memdc,0,0,SRCCOPY);

	//Memdc.SelectObject(pOldBmp);
	//Memdc.SelectObject(pOldIspBrush);
	//Memdc.SelectObject(pOldTrnBrush);

	DeleteDC(Memdc);
	DeleteDC(ImageDC);

	delete Gdi;

	//////////////////////////////////////////////////////////////////////////

	//SelectObject(hdc, hOldPen);
	//DeleteObject(hNewPen);

	//ReleaseDC(Handle, hdc);
}
*/
// 화면을 직접 클릭해서 움직이는 방식이기때문에 현재 라이브러리 작업중에서 화면좌표를 얻어올수 없어서 일시적으로 주석처리함 
/*
void CDraw::DrawBinaryImage(TDrawTool Tool, EImageBW8 *lpImage, int ThHold)
{
	if (target==NULL) return;
	if (!lpImage->GetImagePtr()) return;

	int Width = lpImage->GetWidth();
	int Height = lpImage->GetHeight();

	CClientDC dc(target);
	HDC hdc = dc.m_hDC;

	// Link Error
	HGdi *Gdi = new HGdi(target->GetSafeHwnd());

	CRect rect;
	target->GetClientRect(&rect);

	EImageBW8 BImage;
	BImage.SetSize(lpImage);
	EasyImage::Threshold(lpImage, &BImage, ThHold);

	//////////////////////////////////////////////////////////////////////////
	CDC Memdc;
	CBitmap bmpCamera;
	//BITMAP bmpInfo;
	//HBITMAP hBmp;
	//CBitmap *pOldBmp;

	CImage m_Image;
	int w, h, bpp;
	BYTE* ptr = (BYTE*)BImage.GetImagePtr();
	w = lpImage->GetWidth();
	h = lpImage->GetHeight();
	bpp = lpImage->GetBitsPerPixel();
	m_Image.Create(w,h,bpp);

	int lineSize;
	if (bpp == 8)
		lineSize = w * 1;
	else if (bpp == 24)
		lineSize = w * 1 * 3;

	if (bpp == 8)
	{
		// Define the color table
		RGBQUAD* tab = new RGBQUAD[256];
		for (int i = 0; i < 256; ++i)
		{
			tab[i].rgbRed = i;
			tab[i].rgbGreen = i;
			tab[i].rgbBlue = i;
			tab[i].rgbReserved = 0;
		}
		m_Image.SetColorTable(0, 256, tab);
		delete[] tab;
	}

	for ( int i = 0; i < h; i++ )
	{
		const void* src;
		void*       dst = m_Image.GetPixelAddress(0, i);
		if (bpp == 8)
			src = ptr + (i*w);
		else if (bpp == 24)
			src = ptr + (i*w*3);

		memcpy(dst, src, lineSize);
	}

	SetStretchBltMode(dc.m_hDC, HALFTONE);
	//if (m_nWidth < rect.Width()/m_nZoom && m_nHeight < rect.Height()/m_nZoom)
	if (w < rect.Width()/Gdi->GetGdiFHs() && h < rect.Height()/Gdi->GetGdiFVs())
	{
		m_Image.Draw( dc.m_hDC,
			0,
			0,
			w*Gdi->GetGdiFHs(),	
			h*Gdi->GetGdiFVs(),	
			0,
			0,
			w,
			h
			);
	}
	//else if (m_nWidth < rect.Width()/m_nZoom)
	else if (w < rect.Width()/Gdi->GetGdiFHs())
	{
		m_Image.Draw( dc.m_hDC,
			0,
			0,
			w*Gdi->GetGdiFHs(),
			rect.Height(),
			0,
			pFAVION->m_ptResult.y,
			w,
			rect.Height()/Gdi->GetGdiFVs()
			);
	}
	//else if (m_nHeight < rect.Height()/m_nZoom)
	else if (h < rect.Height()/Gdi->GetGdiFVs())
	{
		m_Image.Draw( dc.m_hDC,
			0,
			0,
			rect.Width(),
			h*Gdi->GetGdiFVs(),
			pFAVION->m_ptResult.x,
			0,
			rect.Width()/Gdi->GetGdiFHs(),
			h
			);
	}
	else
	{
		m_Image.Draw( dc.m_hDC,
			0,
			0,
			rect.Width(),
			rect.Height(),
			pFAVION->m_ptResult.x,
			pFAVION->m_ptResult.y,
			rect.Width()/Gdi->GetGdiFHs(),
			rect.Height()/Gdi->GetGdiFVs()
			);
	}

	m_Image.Destroy();
	delete Gdi;
	//////////////////////////////////////////////////////////////////////////
}
*/
void CDraw::DrawStrechImage(TDrawTool Tool, EImageBW8 *lpImage, EROIBW8 *lspRoi, COLORREF IspColor, EROIBW8 *TrnRoi, COLORREF TrnColor)
{
	if (lpImage->IsVoid()) return;
	if(Tool.Handle==NULL)  return;

	// 핸들에서 윈도우 포인터 얻어오기
	CWnd *pWnd = CWnd::FromHandle(Tool.Handle);

	CClientDC dc(pWnd);
	HDC hdc = dc.m_hDC;

	CRect rect;
	pWnd->GetClientRect(&rect);

	float nFHs;
	float nFVs;
	int w = lpImage->GetWidth();
	int h = lpImage->GetHeight();

	if (w==0) w = 1600;
	if (h==0) h = 1216;

	nFHs = (float)rect.Width()  / (float)w;
	nFVs = (float)rect.Height() / (float)h;

	lpImage->Draw(dc.m_hDC, nFHs, nFVs);
}
//----------------------------------------------------------------------------------------------



