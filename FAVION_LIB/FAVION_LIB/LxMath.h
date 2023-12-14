// LxMath.h: interface for the HMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXMATH_H__6707EF25_9EEB_4163_BAFB_A1562D01DD09__INCLUDED_)
#define AFX_LXMATH_H__6707EF25_9EEB_4163_BAFB_A1562D01DD09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LxType.h"
#include "math.h"

//----------------------------------------------------------------------------------
//#define M_PI					(float)3.141592
struct TFLine
{
	float a;
	float b;
	float c;
	float angle;

	TFLine() {a = b = c = angle = 0;}
	TFLine(TFPoint P1, TFPoint P2) {
		int sign = 1;
		a = P2.y - P1.y;
		if (a<0){
			sign = -1;
			a = sign*a;
		}
		b = sign*(P1.x-P2.x);
		c = sign*(P1.y*P2.x-P1.x*P2.y);
	}
};
struct __TLinear {float y, a, x, b, Angle; bool IsUnLimit;};

struct TLinear : public __TLinear
{
	TLinear() {y = a = x = b = 0; Angle = 0; IsUnLimit = false;}
	TLinear& operator = (const TLinear& Linear) {
		y = Linear.y;
		a = Linear.a;
		x = Linear.x;
		b = Linear.b;
		Angle = Linear.Angle;
		IsUnLimit = Linear.IsUnLimit;
		return *this;
	}
	float xdis;
	TLinear(TPoint P1, TPoint P2) {
		y = (float)P1.y;
		x = (float)P1.x;
		a = (fabs(xdis))==0 ? 0 : (P2.y-P1.y)/float(P2.x-P1.x);
// 		a = (fabs(P2.x-P1.x))==0 ? 0 : (P2.y-P1.y)/float(P2.x-P1.x);
		b = P1.y - a*P1.x;

		IsUnLimit = (P1.x==P2.x);

		float rad;
		float dx = (float)(P2.x - P1.x);
		float dy = (float)(P2.y - P1.y);
		float dl = sqrt(pow(dx, 2) + pow(dy, 2));

		if (dl==0) Angle = 0.0;
		else {
			rad = asin(fabs(dy/dl));
			if (dx >= 0 && dy >= 0) rad = (float)          rad ;
			if (dx  < 0 && dy >= 0) rad = (float)(M_PI   - rad);
			if (dx  < 0 && dy  < 0) rad = (float)(M_PI   + rad);
			if (dx >= 0 && dy  < 0) rad = (float)(M_PI*2 - rad);

			Angle = (float)((rad*180)/M_PI);
		}
	}
};

struct __TFLinear {float y, a, x, b, Angle; bool IsUnLimit;};

struct TFLinear : public __TFLinear
{
	TFLinear() {y = a = x = b = 0; Angle = 0; IsUnLimit = false;}
	TFLinear& operator = (const TFLinear& FLinear) {
		y = FLinear.y;
		a = FLinear.a;
		x = FLinear.x;
		b = FLinear.b;
		Angle = FLinear.Angle;
		IsUnLimit = FLinear.IsUnLimit;
		return *this;
	}
	TFLinear(TFPoint P1, TFPoint P2) {
		y = P1.y;
		x = P1.x;
		a = fabs(P2.x-P1.x)==0 ? 0 : (P2.y-P1.y)/float(P2.x-P1.x);
		b = P1.y - a*P1.x;

		IsUnLimit = (P1.x==P2.x);

		float rad;
		float dx = P2.x - P1.x;
		float dy = P2.y - P1.y;
		float dl = sqrt(pow(dx, 2) + pow(dy, 2));

		if (dl==0) Angle = 0.0;
		else {
			rad = asin(fabs(dy/dl));
			if (dx >= 0 && dy >= 0) rad = (float)          rad ;
			if (dx  < 0 && dy >= 0) rad = (float)(M_PI   - rad);
			if (dx  < 0 && dy  < 0) rad = (float)(M_PI   + rad);
			if (dx >= 0 && dy  < 0) rad = (float)(M_PI*2 - rad);

			Angle = (float)((rad*180)/M_PI);
		}
	}
};

class AFX_EXT_CLASS HMath  
{
public:
	HMath();
	~HMath();
	
	struct TRect : public RECT
	{
		TRect() {}
		TRect(const TPoint& TL, const TPoint& BR) { left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }
		TRect(int l, int t, int r, int b)         { left=l;    top=t;    right=r;    bottom=b;    }
		TRect(RECT& r)
		{
			left    = r.left;
			top     = r.top;
			right   = r.right;
			bottom  = r.bottom;
		}
		int Width () const { return right  - left; }
		int Height() const { return bottom - top ; }
		bool operator ==(const TRect& rc) const 
		{
			return left ==  rc.left  && top==rc.top &&     
				right == rc.right && bottom==rc.bottom; 
		}
		bool operator !=(const TRect& rc) const 
		{  return !(rc==*this); }
		
		// GOO 
		//__property LONG Left    = { read=left,   write=left   }; 
		//__property LONG Top     = { read=top,       write=top }; 
		//__property LONG Right   = { read=right,  write=right  }; 
		//__property LONG Bottom  = { read=bottom, write=bottom }; 
	};

	// 기울기 및 X절편, Y절편 구하기 
	float __fastcall y(float  a, float  x, float b) {return (a*x)+b;}
	float __fastcall a(float  y, float  x, float b) {return (y-b)/x;}
	float __fastcall x(float  y, float  a, float b) {return (y-b)/a;}
	float __fastcall b(float  y, float  a, float x) {return y-(a*x);}

	//x1 : Start Point x Of Linear, y1 : Start Point y Of Linear,
	//x2 : End Point x Of Linear, y2 : End Point x Of Linear,
	float __fastcall a(float x1, float x2, float y1, float y2) {return fabs(x2-x1)==0 ? 0 : (y2-y1)/float(x2-x1);}
	float __fastcall b(float x1, float x2, float y1, float y2) {return b(y1, a(x1, x2, y1, y2), x1);}

	float __fastcall a(TPoint p1, TPoint p2) {return a((float)p1.x, (float)p2.x, (float)p1.y, (float)p2.y);}
	float __fastcall b(TPoint p1, TPoint p2) {return b((float)p1.x, (float)p2.x, (float)p1.y, (float)p2.y);}
	
	float   __fastcall GetRenewDegree(float x1, float y1, float x2, float y2, float Angle);
	float   __fastcall GetDegree(float x1, float y1, float x2, float y2);
	float   __fastcall GetDegree(TPoint p1, TPoint p2) {return GetDegree((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);}
	float   __fastcall GetDegree(int   x1, int   y1, int   x2, int   y2);
	float   __fastcall GetDegree(TFPoint p1, TFPoint p2) {return GetDegree((float)p1.x, (float)p1.y, (float)p2.x, (float)p2.y);}
	float   __fastcall GetAngle(float x1, float y1, float x2, float y2);
	TFPoint __fastcall OnIntersect(TFPoint P1, TFPoint P2, TFPoint P3, TFPoint P4);
	
	TPoint  __fastcall OnRotate(TPoint cp, TPoint tp, float deg) {return OnRotate((int)cp.x, (int)cp.y, (int)tp.x, (int)tp.y, deg);}
	TPoint  __fastcall OnRotate(int cx, int cy, int tx, int ty, float deg);
	TFPoint __fastcall OnRotate(TFPoint cp, TFPoint tp, float deg) {return OnRotate((float)cp.x, (float)cp.y, (float)tp.x, (float)tp.y, deg);}
	TFPoint __fastcall OnRotate(float cx, float cy, float tx, float ty, float deg);
	TFPoint __fastcall OnRotate(double cx, double cy, double tx, double ty, double deg);
	
	// Distance
	float  __fastcall GetLengthFrPointToPoint(float x, float y, float x1, float y1);
	float  __fastcall GetLengthFrPointToPoint(TFPoint P1, TFPoint P2);

	float  __fastcall GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);
	float  __fastcall GetLengthFrLineToPoint(TPoint P1, TPoint P2, TPoint P3);

	double __fastcall OnLineDistance(double x1, double y1, double x2, double y2, double x, double y);

	float __fastcall GetLength(TPoint p1, TPoint p2);
	float __fastcall GetLength(TFPoint p1, TFPoint p2);

	TPoint __fastcall GetPointFrLineToPoint(TPoint P1 , TPoint P2 , TPoint P3);
	TFPoint __fastcall GetPointFrLineToPoint(TFPoint P1 , TFPoint P2 , TFPoint P3);

	// WLP 소스 기준 추가 내용 
// 	TLinear __fastcall GetLinear(TPoint P1, TPoint P2) {TLinear Linear(P1, P2); return Linear;}
// 	TPoint  __fastcall OnFindIntersection(TPoint Phs, TPoint Phe, TPoint Pvs, TPoint Pve) {return OnFindIntersection(GetLinear(Phs, Phe), GetLinear(Pvs, Pve));}
// 	TPoint  __fastcall OnFindIntersection(TLinear Lx, TLinear Ly);
// 
// 	TFLine  __fastcall GetLinear(TFPoint P1, TFPoint P2) {TFLine Linear(P1, P2); return Linear;}
// 	TFPoint __fastcall OnFindFIntersection(TFPoint Phs, TFPoint Phe, TFPoint Pvs, TFPoint Pve) {return OnFindFIntersection(GetLinear(Phs, Phe), GetLinear(Pvs, Pve));}
// 	TFPoint __fastcall OnFindFIntersection(TFLine l1,TFLine l2);

	TPoint  __fastcall  OnFindIntersectionPoint(TPoint P1, TPoint P2, TPoint P3);

	float __fastcall GetTwoLineAngle(TFPoint p1,TFPoint p2,TFPoint p3,TFPoint p4);
	void  __fastcall Intersection4Point(TFPoint *P, TPoint *Plt, TPoint *Prt, TPoint *Plb, TPoint *Prb);

	TPoint __fastcall OnFindPerpendicularPointX (TPoint Pt, float len, float ap);               // 수직점 교차점 찾는 함수(2차 방정식 기준)
	TPoint __fastcall OnFindPerpendicularPointY (TPoint Pt, float len, float ap);
	TPoint __fastcall OnFindPerpendicularPointXY(TPoint Pt, float xlen, float ylen, float ap) {
		return OnFindPerpendicularPointY(OnFindPerpendicularPointX(Pt, xlen, ap), ylen, ap!=0 ? -1/ap : 0);
	}
	TFPoint __fastcall OnFindPerpendicularPointX (TFPoint Pt, float len, float ap);
	TFPoint __fastcall OnFindPerpendicularPointY (TFPoint Pt, float len, float ap);
	TFPoint __fastcall OnFindPerpendicularPointXY(TFPoint Pt, float xlen, float ylen, float ap) {
		return OnFindPerpendicularPointY(OnFindPerpendicularPointX(Pt, xlen, ap), ylen, ap!=0 ? -1/ap : 0);
	}

	double __fastcall GetAngle(TPoint P1, TPoint P2, TPoint P3);
	double __fastcall GetAngle3Point(TPoint P1, TPoint P2, TPoint P3);

	TPoint __fastcall distanceByDeg (TPoint Pt, double deg, float Length); //from point로 부터 deg각도로 length 만큼 떨어진 점의 좌표              
};
extern HMath Math;

#endif // !defined(AFX_LXMATH_H__6707EF25_9EEB_4163_BAFB_A1562D01DD09__INCLUDED_)
