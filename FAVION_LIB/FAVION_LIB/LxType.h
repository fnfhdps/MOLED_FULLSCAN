// LxType.h: interface for the TPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXTYPE_H__8623A42C_77F4_400E_B811_F8AB8124F778__INCLUDED_)
#define AFX_LXTYPE_H__8623A42C_77F4_400E_B811_F8AB8124F778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//----------------------------------------------
// define
//----------------------------------------------
#define STRING_CHAR_SIZE	0xFF

class HGdi;
class TDrawTool;
/*****************************************
	Float Point
*****************************************/
class AFX_EXT_CLASS TFPoint 
{
public:
	TFPoint() 
	{ 
		x = y = 0.0f; 
	};
	
	TFPoint(float xx, float yy) 
	{ 
		x= xx; 
		y= yy; 
	}
	
	~TFPoint() { };

	float	x;
	float	y;
	
	float GetX() { return x; }
	float GetY() { return y; }
	
	void  SetX(float xx) { x= xx; }
	void  SetY(float yy) { y= yy; }
	void  SetFPoint(float xx, float yy) 
	{ 
		x=xx; 
		y=yy; 
	}
	
};
/*****************************************
	int Point
*****************************************/
class  TPoint 
{
public:
	TPoint() 
	{ 
		x = y = 0; 
	};

	TPoint(int xx, int yy) 
	{ 
		x= xx; 
		y= yy; 
	}

	~TPoint() { };

	int	x;
	int	y;

	int GetX() { return x; }
	int GetY() { return y; }

	void  SetX(int xx) { x= xx; }
	void  SetY(int yy) { y= yy; }
	void  SetFPoint(int xx, int yy) 
	{ 
		x= xx; 
		y= yy; 
	}

};
/*****************************************
	Float Line
*****************************************/
class AFX_EXT_CLASS FLine
{
public:
	FLine() 
	{ 
		spt.SetFPoint(0.0f, 0.0f); 
		ept.SetFPoint(0.0f, 0.0f); 
	}
	
	FLine(TFPoint pt1, TFPoint pt2) 
	{ 
		spt= pt1; 
		ept= pt2; 
	}
	
	~FLine() { }
	
	TFPoint	spt;
	TFPoint	ept;

	void SetFLine(TFPoint pt1, TFPoint pt2) 
	{ 
		spt= pt1; 
		ept= pt2; 
	}
	
};

/*****************************************
	Float Rectangle
*****************************************/
class AFX_EXT_CLASS FRect
{
public:
	FRect() 
	{ 
		point1.SetFPoint(0.0f, 0.0f); 
		point2.SetFPoint(0.0f, 0.0f); 
		point3.SetFPoint(0.0f, 0.0f); 
		point4.SetFPoint(0.0f, 0.0f); 
	}
	
	FRect(TFPoint l, TFPoint t, TFPoint r, TFPoint b) 
	{ 
		point1= l; 
		point2= t; 
		point3= r; 
		point4= b; 
	}
	
	~FRect() { }
	
	TFPoint point1;
	TFPoint point2;
	TFPoint point3;
	TFPoint point4;

	void SetFRect(TFPoint l, TFPoint t, TFPoint r, TFPoint b) 
	{ 
		point1= l; 
		point2= r; 
		point3= t; 
		point4= b; 
	}
	
};
//--------------------------------------------------------------------------------------------------------------------
class AFX_EXT_CLASS HRect  
{
	
public:
	int    x, y, w, h;
	HRect() { x = y = w = h = 0; }
	HRect(int initX, int initY, int initW, int initH) { x = initX; y = initY; w = initW; h = initH; }
	HRect(const HRect& rect) { x = rect.x; y = rect.y; w = rect.w; h = rect.h; }
	HRect& operator= (const HRect &rect) { x = rect.x; y = rect.y; w = rect.w; h = rect.h; return *this; }
	TPoint Center()
	{
		TPoint Point((x+w/2), (y+h/2));
		return Point;
	}
	void GetRotateRoi(HRect *lpRect, int xMv, int yMv, float aMv);
	void GetRotateRoi(HRect *lpRect, int xMv, int yMv, float aMv, TFPoint *TgLT, TFPoint *TgRT, TFPoint *TgRB, TFPoint *TgLB);
	void GetRotateRoi(HRect *lpRect, TFPoint CP, int xMv, int yMv, float aMv, TFPoint *TgLT, TFPoint *TgRT, TFPoint *TgRB, TFPoint *TgLB);
	
};
//--------------------------------------------------------------------------
/*
class CRect
{
public:
	int l, t, r, b;
	FRect() { l = t = r = b = 0; }
	FRect(int initL, int initT, int initR, int initB) { l = initL; t = initT; r = initR; b = initB; }
	FRect(const FRect& rect) { l = rect.l; t = rect.t; r = rect.r; b = rect.b; }
	FRect& operator= (const FRect &rect) { l = rect.l; t = rect.t; r = rect.r; b = rect.b; return *this; }
	TPoint Center()
	{
		TPoint Point(l+(r-l)/2, t+(b-t)/2);
		return Point;
	}
	float GetWidth() { return (r-l); }
	float GetHeight() { return (b-t); }

	TPoint __fastcall Point() {TPoint p; p.x = l+GetWidth()/2; p.y = t+GetHeight()/2; return p;} //Center x,y

	bool   __fastcall IsInclude(int x, int y) {TPoint Point(x, y); return IsInclude(Point);}
	bool   __fastcall IsInclude(FRect  &rect) {return (l>rect.l || t>rect.t || r<rect.r || b<rect.b) ? false : true;}
	bool   __fastcall IsInclude(TPoint Point) {return (l<=Point.x && r>=Point.x && t<=Point.y && b>=Point.y) ? true : false;}

	FRect OnMerge(FRect ar, FRect br) {
		FRect r;
		r.l = (ar.l < br.l) ? ar.l : br.l;
		r.t = (ar.t < br.t) ? ar.t : br.t;
		r.r = (ar.r > br.r) ? ar.r : br.r;
		r.b = (ar.b > br.b) ? ar.b : br.b;
		return r;
	}
};
*/
class AFX_EXT_CLASS JRect
{
public:
	int l, t, r, b;
	TPoint lt, rt, rb, lb;
	JRect() {
		l = t = r = b = 0;
		lt.x = rt.x = rb.x = lb.x = 0; lt.y = rt.y = rb.y = lb.y = 0;
	}

	JRect(int initL, int initT, int initR, int initB) {
		l = initL; t = initT; r = initR; b = initB;
	}

	JRect(const JRect& rect) {
		l = rect.l; t = rect.t; r = rect.r; b = rect.b;
		lt=rect.lt; rt=rect.rt; rb=rect.rb; lb=rect.lb;
	}

	JRect& operator= (const JRect &rect) {
		l = rect.l; t = rect.t; r = rect.r; b = rect.b;
		lt=rect.lt; rt=rect.rt; rb=rect.rb; lb=rect.lb;
		return *this;
	}

	TFPoint Center()
	{
		TFPoint Point((float)(l+(r-l)/2),(float)(t+(b-t)/2));
		return Point;
	}
	void  Clear() { l = t = r = b = 0; }
	float GetWidth() { return (float)((r-l)); }
	float GetHeight() { return (float)((b-t)); }
	float GetDemension() { return (GetWidth()*GetHeight()); }

	TFPoint __fastcall Point()       {TFPoint p; p.x = l+GetWidth()/2; p.y = t+GetHeight()/2; return p;} //Center x,y

	bool __fastcall IsInclude(int x, int y) {TPoint Point(x, y); return IsInclude(Point);}
	bool __fastcall IsInclude(JRect  &rect) {return (l>rect.l || t>rect.t || r<rect.r || b<rect.b) ? false : true;}
	bool __fastcall IsInclude(TPoint Point) {return (l<=Point.x && r>=Point.x && t<=Point.y && b>=Point.y) ? true : false;}

	int  __fastcall GetHorLength(JRect ar, JRect br) {return (ar.l > br.l) ? (ar.r > br.r ? ar.r : br.r) - br.l : (ar.r > br.r ? ar.r : br.r) - ar.l; }
	int  __fastcall GetVerLength(JRect ar, JRect br) {return (ar.t > br.t) ? (ar.b > br.b ? ar.b : br.b) - br.t : (ar.b > br.b ? ar.b : br.b) - ar.t; }
	int  __fastcall GetHorDist  (JRect ar, JRect br) {return (ar.l < br.l) ? br.l - ar.r : ar.l - br.r;}
	int  __fastcall GetVerDist  (JRect ar, JRect br) {return (ar.t < br.t) ? br.t - ar.b : ar.t - br.b;}

	int __fastcall xLength  (){return r-l;}
	int __fastcall yLength  (){return b-t;}
	int __fastcall Demension(){return xLength() * yLength();}

	JRect OnMerge(JRect ar, JRect br) {
		JRect r;
		r.l = (ar.l < br.l) ? ar.l : br.l;
		r.t = (ar.t < br.t) ? ar.t : br.t;
		r.r = (ar.r > br.r) ? ar.r : br.r;
		r.b = (ar.b > br.b) ? ar.b : br.b;
		return r;
	}

	void OnPaint(TDrawTool Tool, int lbStyle=BS_HOLLOW, int lbHatch=HS_BDIAGONAL);

// 	void OnPaint(TDrawTool Tool, int lbStyle=BS_HOLLOW, int lbHatch=HS_BDIAGONAL) {
// 		if (Tool.Handle==NULL) return;
// 
// 		HGdi *Gdi = new HGdi(Tool.Handle);
// 
// 		Gdi->FVx = Tool.FVx;
// 		Gdi->FVy = Tool.FVy;
// 		Gdi->FHs = Tool.FHs;
// 		Gdi->FVs = Tool.FVs;
// 		Gdi->CPen  .Color = Tool.Color;
// 		Gdi->CBrush.Color = Tool.Color;
// 		Gdi->CBrush.Style = lbStyle;
// 		Gdi->CBrush.Hatch = lbHatch;
// 
// 		if (lt.x || rt.x || lb.x || rb.x) {
// 			Gdi->OnLine(lt.x, lt.y, rt.x, rt.y);
// 			Gdi->OnLine(rt.x, rt.y, rb.x, rb.y);
// 			Gdi->OnLine(rb.x, rb.y, lb.x, lb.y);
// 			Gdi->OnLine(lb.x, lb.y, lt.x, lt.y);
// 		}
// 		else Gdi->OnRect(0, l, t, r, b, 0);
// 
// 		delete Gdi;
// 	}
};
//----------------------------------------------------------------------------------------------------------------------
#endif // !defined(AFX_LXTYPE_H__8623A42C_77F4_400E_B811_F8AB8124F778__INCLUDED_)
