// LxGdi.h: interface for the HGdi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXGDI_H__B52DB297_0244_46A6_B927_213554A43D03__INCLUDED_)
#define AFX_LXGDI_H__B52DB297_0244_46A6_B927_213554A43D03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LxType.h"
#include <atlimage.h>

//*******************************************************
//	Color												*
//*******************************************************
#define		DDARKBLUECOLOR			RGB( 44,     36,    176)
#define		YELLOW_DUCK				RGB( 246,   249,    195)
#define		DUCK1					RGB(  50,    80,    150)
#define		BLACK					RGB(   0,     0,      0)
#define		LIGHTBLACK				RGB(  65,    65,    65 )
#define		HARDBLACK				RGB(  25,    25,    25 )
#define		HARDGRAY				RGB(  41,    41,    41 )
#define		STATUSBLACK				RGB(  50,    50,    50 )
#define		WHITE					RGB( 255,   255,    255)
#define		WHITEGRAY				RGB( 215,   215,    215)
#define		RED_COLOR     			RGB( 255,     0,  	 0 )
#define		LIME_COLOR   			RGB(   0,   255,      0)
#define		BLUE_COLOR				RGB(   0,     0,    255)
#define		YELLOW_COLOR		    RGB( 255,   255,      0)
#define		MAGENTA_COLOR   		RGB( 255,     0,    255)
#define		AQUA					RGB(   0,   255,    255)
#define		CLOUDBLUE				RGB( 128,   184,    223)
#define		LLIGHTBLUE				RGB( 140,   140,    255)
#define		DDARKBLUE				RGB(   0,     0,    130)
#define		LLIGHTGREEN				RGB( 140,   255,    130)
#define		DDARKGREEN				RGB(   0,   200,      0)
#define		LLIGHTCYAN				RGB( 150,   230,    230)
#define		DDARKCYAN				RGB(   0,   130,    130)
#define		LLIGHTRED				RGB( 255,   140,    140)
#define		DDARKRED				RGB( 128,     0,      0)
#define		LLIGHTMAGENTA			RGB( 255,   150,    255)
#define		DDARKMAGENTA			RGB( 150,    70,    150)
#define		DDARKBROWN				RGB( 230,   150,     50)
#define		LLIGHTBROWN				RGB( 255,   200,    150)
#define		LLIGHTGRAY				RGB( 210,   210,    210)
#define		DDARKGRAY				RGB(  50,    50,     50)
#define		MORDARKGRAY				RGB( 100,   100,    100)
#define		LLIGHTYELLOW			RGB( 255,   255,    170)
#define		DDARKYELLOW				RGB( 140,   140,    110)
#define		BACK_COLOR				RGB(  61, 	 5, 1   65 )
#define		SKY_BLUE				RGB( 142,   255,    255)
#define		DARK_GREY				RGB( 128,   128,    128)
#define		NORMAL					RGB( 220,   224,    226)
#define		NEW_COLOR				RGB(  49,   101,    156)
#define		BATANG_COLOR			RGB( 227,   227,    227)
#define		NEWINGOO				RGB( 206,   227,    140)
#define		NEWINGOOCOLOR			RGB( 103,   129,    167)
#define		DKGRAY					RGB( 128,   128,    128)
#define		LTGRAY					RGB( 192,   192,    192)
#define		DKYELLOW				RGB( 128,   128,    0  )
#define		DKRED					RGB( 128,   0  ,    0  )
#define		DKBLUE					RGB( 0  ,   0  ,    128)
#define		DKCYAN					RGB( 0  ,   128,    128)
#define		DKGREEN					RGB( 0  ,   128,    0  )
#define		IVORY					RGB( 243,   245,    203)
#define	    LIGHT_PINK				RGB( 255,	182,	193)
#define	    PINK					RGB( 255,	192,	203)
#define	    LAVENDER_BLUSH			RGB( 255,	240,	245)
#define	    PALE_VILOT_RED			RGB( 219,	112,	147)
#define	    HOT_PINK				RGB( 255,	105,	180)
#define	    DEEP_PINK				RGB( 255,	20,		147)
#define	    MEDIUM_VILOT_RED		RGB( 199,	21,		133)
#define	    ORCHID					RGB( 218,	112,	214)
#define	    THISTLE					RGB( 216,	191,	216)
#define	    PLUM					RGB( 221,	160,	221)
#define	    VIIOLET					RGB( 238,	130,	238)
#define	    FUCHSIA					RGB( 255,	0,		255)
#define	    PURPLE					RGB( 128,	0,		128)
#define	    MEDIUM_ORCHID			RGB( 186,	85,		211)
#define	    DARK_VIOLET				RGB( 148,	0,		211)
#define	    DARK_ORCHID				RGB( 153,	50,		204)
#define	    INDIGO					RGB( 75,     0,		130)
#define	    BLUE_VIOLET				RGB( 138,	43,		226)
#define	    MEDIUM_PURPLE			RGB( 147,	112,	219)
#define	    MEDIUM_SLATE_BLUE		RGB( 123,	104,	238)
#define	    SLATE_BLUE				RGB( 106,	90,		205)
#define	    DARK_SLATE_BLUE			RGB( 72,	61,		139)
#define	    LAVENDER				RGB( 230,	230,	250)
#define	    GHOST_WHITE				RGB( 248,	248,	255)
#define	    MEDIUM_BLUE				RGB( 0,		0,		205)
#define	    MIDNIGHT_BLUE			RGB( 25,	25,		112)
#define	    NAVY					RGB( 0,		0,		128)
#define	    ROYAL_BLUE				RGB( 62,	105,	225)
#define	    CORNFLOWER_BLUE			RGB( 100,	149,	237)
#define	    LIGHT_STEEL_BLUE		RGB( 176,	196,	222)
#define	    LIGHT_SLATE_GRAY		RGB( 119,	136,	153)
#define	    SLATE_GRAY				RGB( 112,	128,	144)
#define	    DODGER_BLUE				RGB( 30,	144,	255)
#define	    ALICE_BLUE				RGB( 240,	248,	255)
#define	    STEEL_BLUE				RGB( 70,	130,	180)
#define	    LIGHT_SKY_BLUE			RGB( 135,	206,	250)
#define	    DEEP_SKY_BLUE			RGB( 0,		191,	255)
#define	    POWEDER_BLUE			RGB( 176,	224,	230)
#define	    CADET_BLUE				RGB( 176,	158,	160)
#define	    AZURE					RGB( 240,	255,	255)
#define	    LIGHT_CYAN				RGB( 224,	255,	255)
#define	    PALE_TURQUOOISE			RGB( 175,	238,	238)
#define	    CYAN					RGB( 0,		255,	255)
#define	    DARK_TURQUOISE			RGB( 0,		206,	209)
#define	    DARK_SLATE_GRAY			RGB( 47,	79,		79 )
#define	    TEAL					RGB( 0,		128,	128)
#define	    MEDIUM_TURQUOISE		RGB( 72,	209,	204)
#define	    LIGHT_SEA_GREEN			RGB( 32,	178,	170)
#define	    TURQUOISE				RGB( 64,	224,	208)
#define	    AQUAMARINE				RGB( 127,	255,	212)
#define	    MEDIUM_AQUAMARINE		RGB( 102,	205,	170)
#define	    MEDIUM_SPRING_GREEN		RGB( 0,		250,	154)
#define	    MINT_CREAM				RGB( 245,	255,	250)
#define	    SPRING_GREEN			RGB( 0,		255,	127)
#define	    MEDIUM_SEA_GREEN		RGB( 60,	179,	113)
#define	    SEA_GREEN				RGB( 46,	139,	87 )
#define	    HONEYDEW				RGB( 240,	255,	240)
#define	    PALE_GREEN				RGB( 152,	251,	152)
#define	    DARK_SEA_FREEN			RGB( 143,	188,	143)
#define	    LIME_GREEN				RGB( 50,	205,	50 )
#define	    FOREST_GREEN			RGB( 34,	139,	34 )
#define	    CHARTREUSE				RGB( 127,	255,	0  )
#define	    LAWN_GREEN				RGB( 124,	252,	0  )
#define	    GREEN_YELLOW			RGB( 173,	255,	47 )
#define	    DARK_OLIVE_GREEN		RGB( 85,	107,	47 )
#define	    YELLOW_GREEN			RGB( 154,	205,	50 )
#define	    OLIVE_DRAB				RGB( 107,	142,	35 )
#define	    BEIGE					RGB( 245,	245,	220)
#define	    LIGHT_GOLDENROD_YELLOW	RGB( 250,	250,	210)
#define	    LIGHT_YELLOW			RGB( 255,	255,	224)
#define	    OLIVE					RGB( 128,	128,	0  )
#define	    DARK_KHAKI				RGB( 128,	128,	0  )
#define	    LEMON_CHIFFON			RGB( 255,	250,	205)
#define	    PALE_GOLDENROD			RGB( 238,	232,	170)
#define	    KHAKI					RGB( 240,	230,	170)
#define	    GOLD					RGB( 255,	215,	0  )
#define	    CORNSILK				RGB( 255,	248,	220)
#define	    GOLDENROD				RGB( 218,	165,	32 )
#define	    DARK_GOLDENROD			RGB( 184,	134,	11 )
#define	    FLORAL_WHITE			RGB( 255,	250,	240)
#define	    OLD_LACE				RGB( 253,	245,	230)
#define	    WHEAT					RGB( 245,	222,	179)
#define	    MOCCASIN				RGB( 255,	228,	181)
#define	    ORANGE					RGB( 255,	165,	0  )
#define	    PAPAYA_WHIP				RGB( 255,	239,	213)
#define	    BLANCHED_ALMOND			RGB( 255,	235,	205)
#define	    NAVAJO_WHITE			RGB( 255,	222,	173)
#define	    ANTIQUE_WHITE			RGB( 250,	235,	215)
#define	    TAN						RGB( 210,	180,	140)
#define	    BURLY_WOOD				RGB( 222,	180,	135)
#define	    BISQUE					RGB( 255,	228,	196)
#define	    DARK_ORANGE				RGB( 255,	140,	230)
#define	    LINEN					RGB( 250,	240,	230)
#define	    PERU					RGB( 205,	133,	63 )
#define	    PEACH_PUFF				RGB( 255,	218,	185)
#define	    SANDY_BROWN				RGB( 244,	164,	96 )
#define	    CHOCOLATE				RGB( 210,	105,	30 )
#define	    SADDLE_BROWN			RGB( 244,	164,	96 )
#define	    SEASHELL				RGB( 255,	245,	238)
#define	    SIENNA					RGB( 160,	82,		45 )
#define	    LIGHT_SALMON			RGB( 255,	160,	122)
#define	    CORAL					RGB( 255,	127,	80 )
#define	    ORANGE_RED				RGB( 255,	69,		0  )
#define	    DARK_SALMON				RGB( 233,	150,	122)
#define	    TOMATO					RGB( 255,	99,		71 )
#define	    MISTY_ROSE				RGB( 255,	228,	225)
#define	    SALMON					RGB( 250,	128,	114)
#define	    SNOW					RGB( 255,	250,	250)
#define	    LIGHT_CORAL				RGB( 240,	128,	128)
#define	    ROSY_BROWN				RGB( 188,	143,	143)
#define	    INDIAN_RED				RGB( 205,	92,		92 )
#define	    BROWN					RGB( 165,	42,		42 )
#define	    FIRE_BRICK				RGB( 178,	34,		34 )
#define	    MAROON					RGB( 128,	0,		0  )
#define	    WHITE_SMOKE				RGB( 245,	245,	245)
#define	    GAINSBORO				RGB( 220,	220,	220)
#define	    SILVER					RGB( 192,	192,	192)
#define	    DARKBLUE				RGB( 000,	000,	128)
#define	    DARK_GREEN				RGB( 000,	064,	000)
#define	    DARK_CYAN				RGB( 000,	064,	064)	
#define	    DARK_RED				RGB( 064,	000,	000)
#define	    DARK_MAGENTA			RGB( 128,	000,	128)
#define	    DARK_GRAY				RGB( 128,	128,	128)
#define	    LIGHT_GRAY				RGB( 192,	192,	192)
#define	    LIGHT_BLUE				RGB( 000,	000,	255)
#define	    LIGHT_GREEN				RGB( 000,	255,	000)
#define	    LIGHT_RED				RGB( 255,	000,	000)
#define	    LIGHT_MAGENTA			RGB( 255,	000,	255)
#define     SCRIPT					RGB( 176,	6,		002)
#define     GREEN_COLOR					RGB( 020,	160,	000)



#define		SKY_C					RGB(187, 250, 255)
#define		SKY_D					RGB(150, 225, 255)
#define		NOR_C					RGB(192, 192, 192)
#define		RED_C					RGB(255, 0, 0)
#define		RED_L					RGB(255, 150, 150)
#define		RED_D					RGB(153, 0, 0)
#define		BLUE_C					RGB(0, 0, 255)
#define		BLACK_C					RGB(25, 1, 1)
#define		PINK_C					RGB(255, 128, 128)
#define		YELLOW_C				RGB(255,255,0)
#define		YELLOW_D				RGB(128, 128, 0)
#define		GREEN_L					RGB(150,255,150)
#define		GREEN_D					RGB(0,100,20)
#define		SERVO_ON				RGB(97,234,93)
#define		SERVO_OFF				RGB(248,96,52)
#define		HOME_ON					RGB(242,250,95)
#define		NORMAL_OFF				RGB(150,150,150)
#define		LIMIT_ON				RGB(255,45,45)
#define		DSERBLE_BC				RGB(210,210,230)
#define		TEXT_BC					RGB(220,230,210)
#define		OK_BC					RGB(0, 0, 255)
#define		NG_BC					RGB(255, 0, 0)
#define		RESULT_BC				RGB(40, 1, 1)
#define		ORANGE_C				RGB(0xFF, 0x68, 0x20)
#define		BLUE_D					RGB(0, 0, 128)
#define		ON_C					RGB(0, 255, 0)
#define		OFF_C					RGB(0, 150, 0)
#define		NG_C					RGB(0, 0, 255)
#define		OK_C					RGB(0, 255, 0)
#define		GRAYGREEN				RGB(64, 128, 128)
#define		LIGHTGRAY				RGB(222,222,222)


#define		LLIGHTPEAGREEN			RGB( 172, 211, 115  )
#define		PUREYELLOWORANGE		RGB( 247, 148,  29  )
#define		LLIGHTYELLOWGREEN		RGB( 124, 197, 118  )
#define		PUREGREEN				RGB(   0, 166,  80  )
#define		PUREVIOLETMAGENTA		RGB( 146,  30, 143  )
#define		PASTELVIOLET			RGB( 161, 134, 190  )
#define		DARKERYELLOWGREEN		RGB(   0,  94,  32  )
#define		LLIGHTPASTELCYANBLUE	RGB( 217, 232, 251  )
#define		DARKGREENJIN			RGB(   0, 100,   0  )
#define		LIGHTGREENJIN			RGB(   0, 255,   0  )
#define		WINDOWBOTTOM			RGB(  49, 101, 156  )
#define		LIGHTSKY				RGB(128,255,255)
#define		LIGHTYELLOW				RGB(255,255,149)
#define		NAVY				    RGB(57,57,81)

#define		BACKGROUNDCOLOR		HARDGRAY	
#define		DIALOGCOLOR			NAVY
#define		SEPARATE_VIEWCOLOR			BLACK

// Form Design
typedef enum tagFORM_VIEW
{
	FORM_MAIN = 0,
	FORM_VISION   ,
	FORM_DB        ,
	FORM_END
}TFORM_VIEW;

/*****************************************
	Draw Box
*****************************************/
#define BOXRECT(pDC,rect) \
{\
	(pDC)->MoveTo(rect.left, rect.top); \
	(pDC)->LineTo(rect.right, rect.top); \
	(pDC)->LineTo(rect.right, rect.bottom); \
	(pDC)->LineTo(rect.left, rect.bottom); \
	(pDC)->LineTo(rect.left, rect.top); \
}


// extern __declspec(dllexport) float FHs;
// extern __declspec(dllexport) float FVs;
// extern __declspec(dllexport) int   FVx;
// extern __declspec(dllexport) int   FVy;

// extern AFX_DATA_EXPORT float FHs;
// extern AFX_DATA_EXPORT float FVs;
// extern AFX_DATA_EXPORT int   FVx;
// extern AFX_DATA_EXPORT int   FVy;
class AFX_EXT_CLASS TDrawTool {
public:
	TDrawTool() {
		Handle = NULL;
		FHs = 1.0;
		FVs = 1.0;
		FVx = 0;
		FVy = 0;
	}

	HWND Handle;
	COLORREF Color;
	float FHs;
	float FVs;
	int   FVx;
	int   FVy;
};

class AFX_EXT_CLASS HGdi  
{
private:
	BYTE*		m_pDib	 ;
	BYTE*		m_pBuffer;
	int 		m_nWidth ;
	int 		m_nHeight;
	int 		m_nBitCnt;
	int			m_nThickness;


protected:
	void OnMakePalette();
	void OnMakeText();
	void OnMakePen();
	void OnDelePen();
	void OnMakeBrush();
	void OnDeleBrush();
	void OnMakeFont();
	void OnDeleFont();

	void Init();

	HBITMAP hBitmap;
	HBITMAP OnMakeBitmap(int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int bitcnt, void *Data);
	void OnDrawBitmap(int x, int y, int w, int h, HBITMAP hBitmap);
	void OnDrawDirect(int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int bitcnt, void *Data, bool IsFalse=false, int HiThHold=0, int LoThHold=0, BYTE HiThMask=0, BYTE LoThMask=0, bool IsElMask=true);

public:
	int   FVx;
	int   FVy;
	float FHs;
	float FVs;


public :
	struct TConfigText {
		COLORREF Color;
		COLORREF BkColor;
	};

	struct TConfigPen {
		int Style;
		int EndStyle;
		int JoinStyle;
		int Width;
		COLORREF Color;
	};

	struct TConfigBrush {
		int Style;
		int Hatch;
		COLORREF Color;
	};

	struct TConfigFont {
		int Height;
		int Escapement;
		int Italic;
		int Underline;
		int StrikeOut;
		//char *Face;
		CString Face;
	};


	HWND hwnd;
	HDC hdc;
	HPEN hNewPen, hOldPen;
	HBRUSH hNewBrush, hOldBrush;
	HFONT hNewFont, hOldFont;
	LOGBRUSH logbrush;
	LOGFONT logfont;
	TConfigText CText;
	TConfigPen CPen;
	TConfigBrush CBrush;
	TConfigFont CFont;
	//COLORREF  DisplayColor;
	int DisplayColor;

	HGdi(HWND h);
	~HGdi();

	HBITMAP		LoadBMPToHBITMAP(CString path);
	BYTE*		LoadBMPtoArray(CString path);
	//HBITMAP		PtrToHBITMAP(int w, int h, int bitcnt, BYTE* ptr);
	void		DrawStretchBlt(HDC hDC, int dw, int dh, HBITMAP hBmp, bool stretch);
	void		DrawStretchDIBitsData(HDC hDC, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int BitCnt, void* data);
	int			GetPixelValueY(int x, int y);
	//RGBBYTE		GetPixelValueC24(int x, int y);

	void OnText    (int x, int y, char *str);
	void OnTextFix (int x, int y, char *str);
	void OnText    (int x, int y, CString str);
	void OnTextFix (int x, int y, CString str);
	void OnMove    (int x , int y);
	void OnLine    (int x1, int y1                );
	void OnLine    (int x1, int y1, int x2, int y2);
	void OnLine    (TPoint p1, TPoint p2          );
	void OnLinePlus(int    x , int y , int s);
	void OnLinePlus(TPoint p ,        int s);

	void OnRect (bool IsFill, int  x1, int y1, int x2, int y2, int cs=0);
	void OnRect (bool IsFill, TPoint p1, TPoint p2, int cs=0);
	void OnRect (bool IsFill, TPoint p, int length, int cs=0);
	void OnRect (bool IsFill, RECT r , int cs);
	void OnGRect(int x1, int y1, int x2, int y2, int style, COLORREF color1, COLORREF color2); // 0 : Hor  1 : Ver
	void OnGRect(RECT r,                         int style, COLORREF color1, COLORREF color2);

	void OnCircle(bool IsFill, int x1, int y1, int x2, int y2, int cs);
	void OnCircle(bool IsFill, TPoint p1, TPoint p2, int cs);
	void OnCircle(bool IsFill, RECT r, int cs);
	void OnCircle(bool IsFill, int    x, int y, int r, int cs);
	void OnCircle(bool IsFill, TPoint p,            int r, int cs);

	//GOO 
	void OnTrace(POINT *op, int cnt);
	void OnPixel(int x, int y, COLORREF color);
	void OnPixel(TPoint p, COLORREF color);
    void OnDrawDirect(int x, int y, int w, int h, int bitcnt, void *Data, bool IsFalse=false, int HiThHold=0, int LoThHold=0, BYTE HiThMask=0, BYTE LoThMask=0, bool IsElMask=true);

};

class TypeCast
{
public :
	int		CStringToInteger(CString str) { return _ttoi(str); }
	TCHAR*  CStringToPChar(CString str);
	LPTSTR  CStringToLPTSTR(CString str);
	CString LPTSTRToCString(LPTSTR lptstr);
};
class KImage
{
public :
	KImage(void);
	~KImage(void);
	/*CImage Class를 이용한 함수*/
	BOOL		IsValid() { return !m_CImage.IsNull(); }
	void		LoadImageFromPtr(int w, int h, int bit, BYTE* ptr);
	BOOL		LoadImageFromPath(CString path);
	BYTE*		GetImagePointer();
	void		DrawBitBlt(HDC hDC, int dx, int dy, int dw, int dh, int sx, int sy);
	void		DrawStretch(HDC hDC, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, bool bCross);
	void		SetCrossLineThickness(int thickness) { m_nThickness = thickness > 0 ? thickness : 1; }
	int			GetThickness() { return m_nThickness; }
	int 		GetWidth   () { return m_CImage.GetWidth();  }
	int 		GetHeight  () { return m_CImage.GetHeight(); }
	int 		GetBitCount() { return m_CImage.GetBPP();	 }
	int			GetPixelY(int x, int y);
	//RGBBYTE		GetPixelRGB(int x, int y);
	void		Save(CString path);
	void		BilinearResize(KImage& img, int zoom);
	void		DestroyImage();
	
private :
	
	CImage		m_CImage ;
	BYTE*		m_pBuffer;
	/*int 		m_nWidth ;
	int 		m_nHeight;
	int 		m_nBitCnt;*/
	int			m_nThickness;
};
class RGBBYTE  
{
public:
	BYTE b;
	BYTE g;
	BYTE r;
public:
	RGBBYTE(const BYTE& pixel = 0);     
	RGBBYTE(const BYTE& _r, const BYTE& _g, const BYTE& _b);
	RGBBYTE(const RGBBYTE& pixel);      
	~RGBBYTE();							

public:
	RGBBYTE&    operator=(const RGBBYTE& pixel);
	RGBBYTE&    operator=(const COLORREF& pixel);
	RGBBYTE&    operator=(const BYTE& pixel);

	int         operator==(const RGBBYTE& pixel);
	int         operator!=(const RGBBYTE& pixel);
};
#endif // !defined(AFX_LXGDI_H__B52DB297_0244_46A6_B927_213554A43D03__INCLUDED_)
