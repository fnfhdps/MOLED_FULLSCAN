// LxGdi.cpp: implementation of the HGdi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LxGdi.h"
#include "LxHeader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BYTE FALSE_COLOR[] = {
      0,   0, 143,  // RGB(0);
      0,   0, 159,  // RGB(1);
      0,   0, 175,  // RGB(2);
      0,   0, 191,  // RGB(3);
      0,   0, 207,  // RGB(4);
      0,   0, 223,  // RGB(5);
      0,   0, 239,  // RGB(6);
      0,   0, 255,  // RGB(7);
      0,  15, 255,  // RGB(8);
      0,  31, 255,  // RGB(9);
      0,  47, 255,  // RGB(10);
      0,  63, 255,  // RGB(11);
      0,  79, 255,  // RGB(12);
      0,  95, 255,  // RGB(13);
      0, 111, 255,  // RGB(14);
      0, 127, 255,  // RGB(15);
      0, 143, 255,  // RGB(16);
      0, 159, 255,  // RGB(17);
      0, 175, 255,  // RGB(18);
      0, 191, 255,  // RGB(19);
      0, 207, 255,  // RGB(20);
      0, 223, 255,  // RGB(21);
      0, 239, 255,  // RGB(22);
      0, 255, 255,  // RGB(23);
     15, 255, 239,  // RGB(24);
     31, 255, 223,  // RGB(25);
     47, 255, 207,  // RGB(26);
     63, 255, 191,  // RGB(27);
     79, 255, 175,  // RGB(28);
     95, 255, 159,  // RGB(29);
    111, 255, 143,  // RGB(30);
    127, 255, 127,  // RGB(31);
    143, 255, 111,  // RGB(32);
    159, 255,  95,  // RGB(33);
    175, 255,  79,  // RGB(34);
    191, 255,  63,  // RGB(35);
    207, 255,  47,  // RGB(36);
    223, 255,  31,  // RGB(37);
    239, 255,  15,  // RGB(38);
    255, 255,   0,  // RGB(39);
    255, 239,   0,  // RGB(40);
    255, 223,   0,  // RGB(41);
    255, 207,   0,  // RGB(42);
    255, 191,   0,  // RGB(43);
    255, 175,   0,  // RGB(44);
    255, 159,   0,  // RGB(45);
    255, 143,   0,  // RGB(46);
    255, 127,   0,  // RGB(47);
    255, 111,   0,  // RGB(48);
    255,  95,   0,  // RGB(49);
    255,  79,   0,  // RGB(50);
    255,  63,   0,  // RGB(51);
    255,  47,   0,  // RGB(52);
    255,  31,   0,  // RGB(53);
    255,  15,   0,  // RGB(54);
    255,   0,   0,  // RGB(55);
    239,   0,   0,  // RGB(56);
    223,   0,   0,  // RGB(57);
    207,   0,   0,  // RGB(58);
    191,   0,   0,  // RGB(59);
    175,   0,   0,  // RGB(60);
    159,   0,   0,  // RGB(61);
    143,   0,   0,  // RGB(62);
    127,   0,   0,  // RGB(63);
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

float FHs;
float FVs;
int   FVx;
int   FVy;

HGdi::HGdi(HWND h) { hwnd = h; hdc = GetDC(hwnd); Init(); }
HGdi::~HGdi() {
   ReleaseDC(hwnd, hdc);
}

void HGdi::Init()
{
    CText.Color    = YELLOW_COLOR;
    CText.BkColor  = WHITE;

    CPen.Style     = PS_SOLID;
    CPen.EndStyle  = PS_ENDCAP_FLAT;
    CPen.JoinStyle = PS_JOIN_MITER;
    CPen.Width     = 1;
    CPen.Color     = BLACK;

    CBrush.Style   = BS_SOLID;
    CBrush.Color   = BLACK;
    CBrush.Hatch   = HS_HORIZONTAL;

    CFont.Height     = 10;
    CFont.Escapement =  0;
    CFont.Italic     =  0;
    CFont.Underline  =  0;
    CFont.StrikeOut  =  0;
    CFont.Face       = "";

    DisplayColor = WHITE;
}
void HGdi::DrawStretchDIBitsData(HDC hDC, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int bitcnt, void* data)
{
	//------ Create Bitmapinfo -----//
	BITMAPINFO* pBitmapInfo;
	pBitmapInfo = NULL;

	try
	{
		if		(bitcnt == 8 ) pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
		else if (bitcnt == 24) pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO)];
	}
	catch (CMemoryException* e)
	{
		LPTSTR lptstr = NULL;
		e->GetErrorMessage(lptstr,MAX_PATH);
		TypeCast cast;
		CString str = cast.LPTSTRToCString(lptstr);
		AfxMessageBox(str);
		return;
	}

	pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBitmapInfo->bmiHeader.biPlanes = 1;
	pBitmapInfo->bmiHeader.biBitCount = bitcnt;
	pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	pBitmapInfo->bmiHeader.biSizeImage = 0;
	pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	pBitmapInfo->bmiHeader.biClrUsed = 0;
	pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bitcnt == 8)
	{
		for (int i = 0 ; i < 256 ; i++)
		{
			pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	pBitmapInfo->bmiHeader.biWidth = sw;
	pBitmapInfo->bmiHeader.biHeight = sh;
	//------------------------------//

	SetStretchBltMode(hDC, COLORONCOLOR); 
	StretchDIBits(hDC, dx, dy, dw, dh, sx, sy, sw, sh, data, pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	delete []pBitmapInfo;
}

/*
HBITMAP HGdi::PtrToHBITMAP(int w, int h, int bitcnt, BYTE* ptr)
{
	BITMAPINFO * pBitmapInfo;
	BITMAPINFOHEADER* pBmIh;

	if (bitcnt == 8)
	{
		// Build bitmap info Y8
		pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];	
	}
	else if (bitcnt == 24)
	{
		// Build bitmap info RGB24
		pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO)];	
	}

	pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBitmapInfo->bmiHeader.biPlanes = 1;
	pBitmapInfo->bmiHeader.biBitCount = bitcnt;
	pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	pBitmapInfo->bmiHeader.biSizeImage = 0;
	pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	pBitmapInfo->bmiHeader.biClrUsed = 0;
	pBitmapInfo->bmiHeader.biClrImportant = 0;
	
	if (bitcnt == 8)
	{
		for (int i = 0 ; i < 256 ; i++)
		{
			pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}
	
	pBitmapInfo->bmiHeader.biWidth = w;
	pBitmapInfo->bmiHeader.biHeight = -h;

	pBmIh = (BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER)];
	pBmIh->biBitCount = bitcnt;
	pBmIh->biClrImportant = 0;
	pBmIh->biClrUsed = 0;
	pBmIh->biCompression = BI_RGB;
	pBmIh->biPlanes = 1;
	pBmIh->biSize = sizeof(BITMAPINFOHEADER);
	pBmIh->biSizeImage = 0;
	pBmIh->biWidth = w;
	pBmIh->biHeight = -h;

	HDC hdc = ::GetDC(NULL);

	CImage image;
	HBITMAP hbmp = CreateDIBitmap(hdc, pBmIh, CBM_INIT, ptr, pBitmapInfo, DIB_RGB_COLORS) ;
	
	::ReleaseDC(NULL, hdc);

	delete pBitmapInfo;
	delete pBmIh;

	return hbmp;
}
*/

//////////////////////////////////////////////////////////////////////////
// Load Bitmap file and return HBITMAP
//////////////////////////////////////////////////////////////////////////
HBITMAP HGdi::LoadBMPToHBITMAP(CString path)
{
	return (HBITMAP)::LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

//////////////////////////////////////////////////////////////////////////
// Load Bitmap file and return ByteArray(pointer) 
//////////////////////////////////////////////////////////////////////////
BYTE* HGdi::LoadBMPtoArray(CString path)
{
	CFile file;
	BITMAPFILEHEADER bmfh;
	DWORD dwFileSize, dwDibSize;

	//file.Open(path, CFile::modeRead | CFile::shareDenyWrite, NULL);
	file.Open(path, CFile::modeReadWrite | CFile::shareDenyWrite, NULL);

	dwFileSize = (DWORD)file.GetLength();
	dwDibSize = dwFileSize - sizeof(BITMAPFILEHEADER);

	if (m_pDib != NULL)
	{
		delete []m_pDib;
		m_pDib = NULL;
	}

	try
	{
		m_pDib = new BYTE[dwDibSize];
	}
	catch (CMemoryException* e)
	{
		LPTSTR lptstr = NULL;
		e->GetErrorMessage(lptstr,MAX_PATH);
		TypeCast cast;
		CString str = cast.LPTSTRToCString(lptstr);
		AfxMessageBox(str);
		return NULL;
	}

	file.Read(&bmfh, sizeof(BITMAPFILEHEADER));
	file.Read(m_pDib, dwDibSize);

	file.Close();

	BITMAPINFOHEADER* lpbmih;
	int w, h, c;

	lpbmih = (BITMAPINFOHEADER*)m_pDib;
	w = lpbmih->biWidth;
	h = lpbmih->biHeight;
	c = lpbmih->biBitCount;

	/*m_nWidth  = w;
	m_nHeight = h;
	m_nBitCnt = c;*/

	if (m_pBuffer != NULL)
	{
		delete []m_pBuffer;
		m_pBuffer = NULL;
	}

	if( c == 24)
	{
		try
		{
			m_pBuffer = new BYTE[dwDibSize - sizeof(BITMAPINFOHEADER)];
		}
		catch (CMemoryException* e)
		{
			LPTSTR lptstr = NULL;
			e->GetErrorMessage(lptstr,MAX_PATH);
			TypeCast cast;
			CString str = cast.LPTSTRToCString(lptstr);
			AfxMessageBox(str);
			return NULL;
		}
		
		memcpy(m_pBuffer, (BYTE*)m_pDib + sizeof(BITMAPINFOHEADER), dwDibSize - sizeof(BITMAPINFOHEADER));
	}
	else
	{
		try
		{
			m_pBuffer = new BYTE[dwDibSize - sizeof(BITMAPINFOHEADER) - sizeof(RGBQUAD)*256];
		}
		catch (CMemoryException* e)
		{
			LPTSTR lptstr = NULL;
			e->GetErrorMessage(lptstr,MAX_PATH);
			TypeCast cast;
			CString str = cast.LPTSTRToCString(lptstr);
			AfxMessageBox(str);
			return NULL;
		}
		memcpy(m_pBuffer, (BYTE*)m_pDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256, dwDibSize - sizeof(BITMAPINFOHEADER) - sizeof(RGBQUAD)*256);
	}
	
	return m_pBuffer;
}

int HGdi::GetPixelValueY(int x, int y)
{
	if (m_pBuffer == NULL)
		return -1;

	// BITMAP은 buttom-up 구조이므로 뒤집어서 값을 돌려 줌.
	return m_pBuffer[m_nWidth*(m_nHeight-1-y) + x];
}

/*
RGBBYTE HGdi::GetPixelValueC24(int x, int y)
{
	if (m_pBuffer == NULL)
		return RGBBYTE(0,0,0);

	// BITMAP은 buttom-up 구조이므로 뒤집어서 값을 돌려 줌.
	RGBBYTE rgb;
	x *= 3;
	rgb.b = m_pBuffer[3*m_nWidth*(m_nHeight-1-y) + x  ];
	rgb.g = m_pBuffer[3*m_nWidth*(m_nHeight-1-y) + x+1];
	rgb.r = m_pBuffer[3*m_nWidth*(m_nHeight-1-y) + x+2];
	return rgb;

}
*/
void HGdi::DrawStretchBlt(HDC hDC, int dw, int dh, HBITMAP hBmp, bool stretch)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	BITMAP bit;

	if (hBmp == NULL) return;

	hMemDC = CreateCompatibleDC(hDC);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBmp);

	GetObject(hBmp, sizeof(BITMAP), &bit);

	if (stretch)
	{
		SetStretchBltMode(hDC, COLORONCOLOR);
		StretchBlt(hDC, 0, 0, dw, dh, hMemDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);
	}
	else
	{
		BitBlt(hDC,0,0,dw,dh,hMemDC,0,0,SRCCOPY);
	}
	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}
void HGdi::OnMakeText()
{
	SetBkMode   (hdc, TRANSPARENT);
	SetBkColor  (hdc, CText.BkColor);
	SetTextColor(hdc, CText.Color);
}

void HGdi::OnMakePen()
{
	hNewPen   = CreatePen(CPen.Style, CPen.Width, CPen.Color);
	hOldPen = (HPEN)SelectObject(hdc, hNewPen);
	SetROP2   (hdc, R2_COPYPEN);
	SetBkMode (hdc, TRANSPARENT);
}

void HGdi::OnDelePen()
{
	SelectObject(hdc, hOldPen);
	DeleteObject(hNewPen);
}

void HGdi::OnMakeBrush()
{
	logbrush.lbStyle = CBrush.Style;
	logbrush.lbColor = CBrush.Color;
	logbrush.lbHatch = CBrush.Hatch;

	hNewBrush = CreateBrushIndirect(&logbrush);

	hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
	SetROP2   (hdc, R2_COPYPEN);
	SetBkMode (hdc, TRANSPARENT);
}

void HGdi::OnDeleBrush()
{
	SelectObject(hdc, hOldBrush);
	DeleteObject(hNewBrush);
}

void HGdi::OnMakeFont()
{
	hNewFont = CreateFont(CFont.Height, 0, CFont.Escapement*10, 0, FW_MEDIUM, CFont.Italic, CFont.Underline, CFont.StrikeOut,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS,(CFont.Face) );

	hOldFont = (HFONT)SelectObject(hdc, hNewFont);
}

void HGdi::OnDeleFont()
{
	SelectObject(hdc, hOldFont);
	DeleteObject(hNewFont);
}

void HGdi::OnText(int x, int y, char *str)
{
	if (hdc==NULL) return;
	OnMakeFont();
	OnMakeText();

	x -= FVx; y -= FVy;
	//x *= FHs;
	//y *= FVs;
	x = (int)(x*FHs);
	y = (int)(y*FVs);

	TextOut(hdc, x, y, CA2W(str), strlen(str));
	OnDeleFont();
}

void HGdi::OnTextFix(int x, int y, char *str)
{
	if (hdc==NULL) return;
	OnMakeFont();
	OnMakeText();

	TextOut(hdc, x, y, CA2W(str), strlen(str));
	OnDeleFont();
}
void HGdi::OnText(int x, int y, CString str)
{
	if (hdc==NULL) return;
	OnMakeFont();
	OnMakeText();

	x -= FVx; y -= FVy;
	//x *= FHs;
	//y *= FVs;
	x = (int)(x*FHs);
	y = (int)(y*FVs);

	TextOut(hdc, (int)x, (int)y, str ,_ttoi(str));
	OnDeleFont();
}
void HGdi::OnTextFix(int x, int y, CString str)
{
	if (hdc==NULL) return;
	OnMakeFont();
	OnMakeText();

	TextOut(hdc, x, y, str ,_ttoi(str));
    OnDeleFont();
}
void HGdi::OnMove(int x , int y)
{
	if (hdc==NULL) return;

	POINT p;
	x = x-FVx; y = y-FVy;

	//x *= FHs;
	//y *= FVs;
	x = (int)(x*FHs);
	y = (int)(y*FVs);

	MoveToEx(hdc, x, y, &p);
}

void HGdi::OnLine(int x1, int y1, int x2, int y2)
{
	if (hdc==NULL) return;

	OnMakePen();

	x1 -= FVx; //x1 *= FHs;
	y1 -= FVy; //y1 *= FVs;
	x2 -= FVx; //x2 *= FHs;
	y2 -= FVy; //y2 *= FVs;

	x1 = (int)(x1*FHs);
	y1 = (int)(y1*FVs);
	x2 = (int)(x2*FHs);
	y2 = (int)(y2*FVs);

	POINT p;
	MoveToEx(hdc, x1, y1, &p);
	LineTo  (hdc, x2, y2    );
	OnDelePen();
}
void HGdi::OnLine(int x1, int y1)
{
	if (hdc==NULL) return;

	OnMakePen();

	x1 -= FVx; //x1 *= FHs;
	y1 -= FVy; //y1 *= FVs;

	x1 = (int)(x1*FHs);
	y1 = (int)(y1*FVs);

	LineTo(hdc, x1, y1);
	OnDelePen();
}
void HGdi::OnLine(TPoint p1, TPoint p2)
{
	OnLine(p1.x, p1.y, p2.x, p2.y);
}
void HGdi::OnLinePlus(int x, int y, int s)
{
	OnLine(x-s, y  , x+s, y  );
	OnLine(x  , y-s, x  , y+s);
}

void HGdi::OnLinePlus(TPoint p, int s)
{
	OnLinePlus(p.x, p.y, s);
}

void HGdi::OnRect(bool IsFill, int x1, int y1, int x2, int y2, int cs)
{
	if (hdc==NULL) return;
	if (CBrush.Style!=BS_HATCHED) CBrush.Style = IsFill ? BS_SOLID : BS_HOLLOW;

	if (cs!=0) OnLinePlus((x1+abs((int)(x2-x1))/2), (y1+abs((int)(y2-y1))/2), cs);

	OnMakePen();
	OnMakeBrush();

	if (x1==x2 || y1==y2) OnLine(x1, y1, x2, y2);
	else {
		x1 -= FVx; //x1 *= FHs;
		y1 -= FVy; //y1 *= FVs;
		x2 -= FVx; //x2 *= FHs;
		y2 -= FVy; //y2 *= FVs;

		x1 = (int)(x1*FHs);
		y1 = (int)(y1*FVs);
		x2 = (int)(x2*FHs);
		y2 = (int)(y2*FVs);

		Rectangle(hdc, x1, y1, x2, y2);
	}

	OnDelePen();
	OnDeleBrush();
}

void HGdi::OnRect(bool IsFill, TPoint p1, TPoint p2, int cs)
{
	OnRect(IsFill, p1.x, p1.y, p2.x, p2.y, cs);
}

void HGdi::OnRect(bool IsFill, TPoint p, int length, int cs)
{
	OnRect(IsFill,(p.x-length), (p.y-length), (p.x+length), (p.y+length), cs);
}

void HGdi::OnRect(bool IsFill, RECT r, int cs)
{
	OnRect(IsFill, r.left, r.top, r.right, r.bottom, cs);
}
void HGdi::OnGRect(int x1, int y1, int x2, int y2, int style, COLORREF color1, COLORREF color2)
{
	x1 -= FVx;
	y1 -= FVy;
	x2 -= FVx;
	y2 -= FVy;

	TRIVERTEX vert[2];
	GRADIENT_RECT gRect;

	vert[0].x     = x1;
	vert[0].y     = y1;
	vert[0].Red   = GetRValue(color1)<<8;
	vert[0].Green = GetGValue(color1)<<8;
	vert[0].Blue  = GetBValue(color1)<<8;
	vert[0].Alpha = 0;

	vert[1].x     = x2;
	vert[1].y     = y2;
	vert[1].Red   = GetRValue(color2)<<8;
	vert[1].Green = GetGValue(color2)<<8;
	vert[1].Blue  = GetBValue(color2)<<8;
	vert[1].Alpha = 0;

	gRect.UpperLeft = 0;
	gRect.LowerRight = 1;
	// GOO 
	//GradientFill(hdc, vert, 2, &gRect, 1, style==0 ? GRADIENT_FILL_RECT_H : GRADIENT_FILL_RECT_V);
}

void HGdi::OnGRect(RECT r, int style, COLORREF color1, COLORREF color2)
{
	OnGRect(r.left, r.top , r.right, r.bottom, style, color1, color2);
}

void HGdi::OnCircle(bool IsFill, int x1, int y1, int x2, int y2, int cs)
{
	if (hdc==NULL) return;
	if (CBrush.Style!=BS_HATCHED) CBrush.Style = IsFill ? BS_SOLID : BS_HOLLOW;

	if (cs!=0) OnLinePlus((x1+abs((int)(x2-x1))/2), (y1+abs((int)(y2-y1))/2),cs);

	OnMakePen();
	OnMakeBrush();

	x1 -= FVx; //x1 *= FHs;
	y1 -= FVy; //y1 *= FVs;
	x2 -= FVx; //x2 *= FHs;
	y2 -= FVy; //y2 *= FVs;

	x1 = (int)(x1*FHs);
	y1 = (int)(y1*FVs);
	x2 = (int)(x2*FHs);
	y2 = (int)(y2*FVs);

	Ellipse(hdc, x1, y1, x2, y2);

	OnDelePen();
	OnDeleBrush();
}

void HGdi::OnCircle(bool IsFill, TPoint p1, TPoint p2, int cs) { OnCircle(IsFill, p1.x  , p1.y , p2.x   , p2.y    , cs);}
void HGdi::OnCircle(bool IsFill, RECT   r            , int cs) { OnCircle(IsFill, r.left, r.top, r.right, r.bottom, cs);}

void HGdi::OnCircle(bool IsFill, int  x, int y, int r, int cs) { OnCircle(IsFill, (x-r),   (y-r),   (x+r),   (y+r), cs);}
void HGdi::OnCircle(bool IsFill, TPoint p,      int r, int cs) { OnCircle(IsFill, (p.x-r), (p.y-r), (p.x+r), (p.y+r), cs);}
 
void HGdi::OnTrace(POINT *op, int cnt)
{
	if (hdc==NULL) return;

	POINT *np = new POINT[cnt+1];
	memcpy(np, op, cnt);
	np[cnt] = op[0];

	OnMakePen();
	OnMakeBrush();

	Polyline(hdc, np, cnt+1);

	OnDelePen();
	OnDeleBrush();
	delete[] np;
}

void HGdi::OnPixel(int x, int y, COLORREF color)
{
	x -= FVx; //x *= FHs;
	y -= FVy; //y *= FVs;
	x =(int)(x*FHs);
	y =(int)(y*FVs);

	SetPixelV(hdc, x, y, color);
}

void HGdi::OnPixel(TPoint p, COLORREF color)
{
	p.x -= FVx; //p.x *= FHs;
	p.y -= FVy; //p.y *= FVs;
	p.x =(int)(p.x*FHs);
	p.y =(int)(p.y*FVs);

	SetPixelV(hdc, p.x, p.y, color);
}

void HGdi::OnMakePalette()
{
	LOGPALETTE *pal;
	HPALETTE hPal;

	int palSize = GetDeviceCaps(hdc, SIZEPALETTE);

	pal = (LOGPALETTE *)new char [sizeof(LOGPALETTE) + palSize*sizeof(PALETTEENTRY)];
	pal->palVersion = 0x300;
	pal->palNumEntries = palSize;
	for (int i=0; i<palSize; i++) {
		pal->palPalEntry[i].peRed   = (BYTE)i;
		pal->palPalEntry[i].peGreen = (BYTE)i;
		pal->palPalEntry[i].peBlue  = (BYTE)i;
		pal->palPalEntry[i].peFlags = 0;
	}
	hPal = CreatePalette(pal);
	delete pal;
	SelectPalette(hdc, hPal, FALSE);
	RealizePalette(hdc);
	DeleteObject(hPal);
}
/*
void HGdi::OnDrawDirect(int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int bitcnt, void *Data, bool IsFalse, int HiThHold, int LoThHold, BYTE HiThMask, BYTE LoThMask, bool IsElMask)
{
	dx *= FHs;
	dy *= FVs;
	dw *= FHs;
	dh *= FVs;

	BITMAPINFOHEADER* BitmapHeader;

	int numColors = 1 << bitcnt;

	int mult = 32 / bitcnt;
	int compatibleWidth = sw % mult ? sw + (mult - sw % mult) : sw;

	BitmapHeader = (BITMAPINFOHEADER*) new char [sizeof(BITMAPINFOHEADER) + numColors * sizeof(RGBQUAD)];

	BitmapHeader->biSize          = sizeof(BITMAPINFOHEADER);
	BitmapHeader->biWidth         = compatibleWidth;
	BitmapHeader->biHeight        = -sh;
	BitmapHeader->biPlanes        = 1;
	BitmapHeader->biBitCount      = bitcnt;
	BitmapHeader->biCompression   = BI_RGB;
	BitmapHeader->biSizeImage     = 0;
	BitmapHeader->biXPelsPerMeter = 0;
	BitmapHeader->biYPelsPerMeter = 0;
	BitmapHeader->biClrUsed       = numColors;
	BitmapHeader->biClrImportant  = numColors;

	RGBQUAD* RGB= (RGBQUAD*)(BitmapHeader + 1);

	if (!HiThHold && !LoThHold) {
		if (IsFalse) {
			for (int i=0; i<numColors; i++) {
				int k = i>>2;
				RGB->rgbRed   = FALSE_COLOR[k*3+0];
				RGB->rgbGreen = FALSE_COLOR[k*3+1];
				RGB->rgbBlue  = FALSE_COLOR[k*3+2];

				RGB->rgbReserved = 0;
				RGB++;
			}
		}
        else {
            CString RGBNum = IntToHex(DisplayColor, 6);
			
			CString BLUE_COLOR   = "0x" + RGBNum.Mid(0, 2);
			CString GREEN_COLOR  = "0x" + RGBNum.Mid(2, 2);
			CString RED_COLOR    = "0x" + RGBNum.Mid(4, 2);

			//int RedNum   = StrToIntDef(RED_COLOR  , 0);
            //int GreenNum = StrToIntDef(GREEN_COLOR, 0);
            //int BlueNum  = StrToIntDef(BLUE_COLOR , 0);

            //for(int i=0; i<numColors; i++) {
            //    RGB->rgbRed      = i<=RedNum   ? (BYTE)i : 0;
            //    RGB->rgbGreen    = i<=GreenNum ? (BYTE)i : 0;
            //    RGB->rgbBlue     = i<=BlueNum  ? (BYTE)i : 0;
            //    RGB->rgbReserved = 0;
            //    RGB++;
            //}
		}
	}
	else {
		BYTE CNone = '\0';
		for(int i=0; i<numColors; i++) {
			int level = 0;
			if      (!HiThHold &&  LoThHold && i<=LoThHold) level = LoThMask ? LoThMask : i    ;
			else if ( HiThHold && !LoThHold && i>=HiThHold) level = HiThMask ? HiThMask : i    ;
			else if ( HiThHold &&  LoThHold && i>=HiThHold) level = HiThMask ? HiThMask : CNone;
			else if ( HiThHold &&  LoThHold && i<=LoThHold) level = LoThMask ? LoThMask : CNone;
			else                                            level = IsElMask ? CNone    : i    ;

			RGB->rgbRed      = (BYTE)level;
			RGB->rgbGreen    = (BYTE)level;
			RGB->rgbBlue     = (BYTE)level;
			RGB->rgbReserved = 0;
			RGB++;
		}
	}

	SetStretchBltMode(hdc, COLORONCOLOR);
	if (dw==sw && dh==sh) SetDIBitsToDevice(hdc, dx, dy, dw, dh, sx, sy,  0, sh, (BYTE*)Data, (LPBITMAPINFO)BitmapHeader, DIB_RGB_COLORS);
	else                  StretchDIBits    (hdc, dx, dy, dw, dh, sx, sy, sw, sh, (BYTE*)Data, (LPBITMAPINFO)BitmapHeader, DIB_RGB_COLORS, SRCCOPY);

	delete[] BitmapHeader;
}


void HGdi::OnDrawDirect(int x, int y, int w, int h, int bitcnt, void *Data, bool IsFalse, int HiThHold, int LoThHold, BYTE HiThMask, BYTE LoThMask, bool IsElMask)
{
	OnDrawDirect(x, y, w, h, x, y, w, h, bitcnt, Data, IsFalse, HiThHold, LoThHold, HiThMask, LoThMask, IsElMask);
}
*/
/*
void HGdi::OnDrawBitmap(int x, int y, int w, int h, HBITMAP hBitmap)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	BITMAP bit;

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	GetObject(hBitmap, sizeof(BITMAP), &bit);
	SetStretchBltMode(hdc, COLORONCOLOR);
	if (w==bit.bmWidth && h==bit.bmHeight) BitBlt    (hdc, 0, 0, bit.bmWidth, bit.bmHeight, hMemDC, 0, 0, SRCCOPY);
	else                                   StretchBlt(hdc, x, y, w, h, hMemDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);

}
*/

//------------------------------------------------------------------------------------------------------------------------
//  Type Casting Class
//------------------------------------------------------------------------------------------------------------------------
LPTSTR TypeCast::CStringToLPTSTR(CString str)
{
	LPTSTR lptstr;
	lptstr = str.GetBuffer(str.GetLength() + 1);
	
	return lptstr;
}

CString TypeCast::LPTSTRToCString(LPTSTR lptstr)
{
	CString str = lptstr;
	return str;
}

TCHAR* TypeCast::CStringToPChar(CString str)
{
	return (LPTSTR)(LPCTSTR)str;
}

//////////////////////////////////////////////////////////////////////////
//KImage Class with CImage
//////////////////////////////////////////////////////////////////////////
KImage::KImage(void)
{
	m_pBuffer	 = NULL;
	/*m_nWidth	 = 0;
	m_nHeight	 = 0;
	m_nBitCnt	 = 0;*/
	m_nThickness = 1;
}

KImage::~KImage(void)
{
	if (m_pBuffer != NULL)
	{
		delete []m_pBuffer;
		m_pBuffer = NULL;
	}

	if (m_CImage.IsNull() == FALSE)
		m_CImage.Destroy();
}

void KImage::BilinearResize(KImage& img, int zoom)
{
	register int i,j;

	if (zoom < 1) zoom = 1;

	int w = GetWidth();
	int h = GetHeight();

	int nw = w*zoom;
	int nh = h*zoom;

	BYTE* addr = GetImagePointer();
	BYTE* pResize;
	try
	{
		pResize = new BYTE[(nw)*(nh)];
		memset(pResize,0,(nw)*(nh));
	}
	catch (CMemoryException* e)
	{
		LPTSTR lptstr = NULL;
		e->GetErrorMessage(lptstr,MAX_PATH);
		CString str = (CString)lptstr;
		AfxMessageBox(str);
		return;
	}

	int x1, y1, x2, y2;
	double rx, ry, p, q, temp;

	for (j=0; j<nh; j++)
	for (i=0; i<nw; i++)
	{
		rx = (double)w*i/(nw);
		ry = (double)h*j/(nh);

		x1 = (int)rx;
		y1 = (int)ry;

		x2 = x1 + 1; 
		if (x2 == w) x2 = w-1;

		y2 = y1 + 1;
		if (y2 == h) y2 = h-1;

		p = rx - x1;
		q = ry - y1;

		temp = 
			(1.0-p)*(1.0-q)*addr[y1*w+x1] + p*(1.0-q)*addr[y1*w+x2]+
			(1.0-p)*q*addr[y2*w+x1] + p*q*addr[y2*w+x2];

		pResize[nw*j+i] = (BYTE)limit8bit(temp);
	}
	img.LoadImageFromPtr(nw,nh,8,pResize);

	delete []pResize;
}

void KImage::Save(CString path)
{
	if (m_CImage.IsNull() == TRUE) return;

	m_CImage.Save(path,Gdiplus::ImageFormatBMP);
}

void KImage::DestroyImage()
{
	if (m_CImage.IsNull() == FALSE)
		m_CImage.Destroy();
}

void KImage::LoadImageFromPtr(int w, int h, int bit, BYTE* ptr)
{
	if (m_CImage.IsNull() == false)
		m_CImage.Destroy();

	m_CImage.Create(w,h,bit);
	int lineSize;
	//int lineSize = m_nWidth * 1;
	if (bit == 8)
		lineSize = w * 1;
	else if (bit == 24)
		lineSize = w * 1 * 3;
	//if (m_nBitCnt == 8)
	if (bit == 8)
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
		m_CImage.SetColorTable(0, 256, tab);
		delete[] tab;
	}

	// Copy pixel values
	// Warining: does not convert from RGB to BGR
	//for ( int i = 0; i < m_nHeight; i++ )
	for ( int i = 0; i < h; i++ )
	{
		const void* src;
		void*       dst = m_CImage.GetPixelAddress(0, i);
		//const void* src = ptr + (i*m_nWidth);
		if (bit == 8)
			src = ptr + (i*w);
		else if (bit == 24)
			src = ptr + (i*w*3);
		
		memcpy(dst, src, lineSize);
	}
}

//////////////////////////////////////////////////////////////////////////
// Load Image file and return TRUE or FALSE 
//////////////////////////////////////////////////////////////////////////
BOOL KImage::LoadImageFromPath(CString path)
{
	HRESULT result;
	
	if (m_CImage.IsNull() == false)
		m_CImage.Destroy();

	result = m_CImage.Load(path);
	if (result != S_OK)
	{
		AfxMessageBox(_T("ERROR : Failed to file load, please check path"));
		return FALSE;
	}

	/*m_nWidth  = m_CImage.GetWidth();
	m_nHeight = m_CImage.GetHeight();
	m_nBitCnt = m_CImage.GetBPP();*/
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Return Image's Address
//////////////////////////////////////////////////////////////////////////
BYTE* KImage::GetImagePointer()
{
	if (m_CImage.IsNull() == true)
		return NULL;

	if (m_pBuffer != NULL)
	{
		delete []m_pBuffer;
		m_pBuffer = NULL;
	}

	int w=0, h=0;
	try
	{
		w = m_CImage.GetWidth();
		h = m_CImage.GetHeight();
		m_pBuffer = new BYTE[w*h];
	}
	catch (CException* e)
	{
		LPTSTR lptstr = NULL;
		e->GetErrorMessage(lptstr,MAX_PATH);
		TypeCast cast;
		CString str = cast.LPTSTRToCString(lptstr);
		AfxMessageBox(str);
		return NULL;
	}

	for (int y= 0; y <h; y++){
		BYTE * srcImg = NULL;                                    
		srcImg = (BYTE *) m_CImage.GetPixelAddress(0,y);
		memcpy(&m_pBuffer[y * w], srcImg, w);   
	}
	
	return m_pBuffer;
}

void KImage::DrawBitBlt(HDC hDC, int dx, int dy, int dw, int dh, int sx, int sy)
{
	if (m_CImage.IsNull() == true) return;

	m_CImage.BitBlt(hDC,dx,dy,dw,dh,sx,sy,SRCCOPY);
}

void KImage::DrawStretch(HDC hDC, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, bool bCross)
{
	if (m_CImage.IsNull() == true) return;
	
	//아래 함수는 Cross Line을 그릴 수 없음.
	//SetStretchBltMode(hDC, COLORONCOLOR); 
	//m_CImage.StretchBlt(hDC,dx,dy,dw,dh,sx,sy,sw,sh,SRCCOPY);
	
	CDC		Memdc; 
	CDC		ImageDC;
	CBitmap bmpbkg;
	CBitmap *pOldBmp;

	CDC *dc = CDC::FromHandle(hDC);

	Memdc.CreateCompatibleDC(dc); 
	ImageDC.CreateCompatibleDC(dc);

	bmpbkg.CreateCompatibleBitmap(dc,dw,dh);
	pOldBmp = (CBitmap*)Memdc.SelectObject(bmpbkg);

	ImageDC.SelectObject(m_CImage);
	Memdc.SetStretchBltMode(STRETCH_HALFTONE);
	Memdc.StretchBlt(dx,dy,dw,dh,&ImageDC,sx,sy,sw,sh,SRCCOPY);

	if (bCross == TRUE) 
	{
		CPen *pOldPen;
		CPen pen;
		pen.CreatePen(PS_SOLID,m_nThickness,RGB(255,0,0));

		pOldPen = Memdc.SelectObject(&pen);
		Memdc.MoveTo(dw/2, 0   );
		Memdc.LineTo(dw/2, dh  );
		Memdc.MoveTo(0   , dh/2);
		Memdc.LineTo(dw  , dh/2);
	}

	dc->BitBlt(dx,dy,dw,dh,&Memdc,0,0,SRCCOPY);

	//Memdc.SelectObject(pOldBmp);
	DeleteDC(Memdc);
	DeleteDC(ImageDC);
}

int KImage::GetPixelY(int x, int y)
{
	if (m_CImage.IsNull() == true) return -1;

	BYTE* addr;
	addr = (BYTE*)m_CImage.GetPixelAddress(x,y);
	
	return *addr;
}

/*
RGBBYTE KImage::GetPixelRGB(int x, int y)
{
	if (m_CImage.IsNull() == true) return -1;

	BYTE* addr;
	addr = (BYTE*)m_CImage.GetPixelAddress(x,y);

	RGBBYTE rgb;
	rgb.b = *addr;
	rgb.g = *(addr+1);
	rgb.r = *(addr+2);

	return rgb;
}
*/

//////////////////////////////////////////////////////////////////////////
//RGBBYTE Class 
//////////////////////////////////////////////////////////////////////////
RGBBYTE::RGBBYTE(const BYTE& pixel) : r(pixel), g(pixel), b(pixel)
{
}

RGBBYTE::RGBBYTE(const BYTE& _r, const BYTE& _g, const BYTE& _b) 
: r(_r), g(_g), b(_b)
{
}

RGBBYTE::RGBBYTE(const RGBBYTE& pixel) : r(pixel.r), g(pixel.g), b(pixel.b)
{
}

RGBBYTE::~RGBBYTE()
{
}

RGBBYTE& RGBBYTE::operator=(const RGBBYTE& pixel)
{
	if( this == &pixel )
		return *this;

	this->r = pixel.r;
	this->g = pixel.g;
	this->b = pixel.b;

	return *this;
}

RGBBYTE& RGBBYTE::operator=(const COLORREF& pixel)
{
	r = (BYTE)(pixel);                  // GetRValue(pixel);
	g = (BYTE)((WORD)(pixel) >> 8);     // GetGValue(pixel);
	b = (BYTE)((pixel)>>16);            // GetBValue(pixel);

	return *this;
}

RGBBYTE& RGBBYTE::operator=(const BYTE& pixel)
{
	r = pixel;
	g = pixel;
	b = pixel;

	return *this;
}


int RGBBYTE::operator==(const RGBBYTE& pixel)
{
	if( r == pixel.r && g == pixel.g && b == pixel.b ) return 1;
	else return 0;
}

int RGBBYTE::operator!=(const RGBBYTE& pixel)
{
	if( r != pixel.r || g != pixel.g || b != pixel.b ) return 1;
	else return 0;
}


