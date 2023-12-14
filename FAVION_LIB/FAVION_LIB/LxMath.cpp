// LxMath.cpp: implementation of the HMath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LxMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void HRect::GetRotateRoi(HRect *lpRect, int xMv, int yMv, float aMv)
{
	HRect RvRect = *lpRect;
	HRect TgRect;
	HMath Math;

	TPoint RvCP(RvRect.x+RvRect.w/2, RvRect.y+RvRect.h/2);
	TPoint RvLT(RvRect.x           , RvRect.y           );
	TPoint RvRT(RvRect.x+RvRect.w  , RvRect.y           );
	TPoint RvRB(RvRect.x+RvRect.w  , RvRect.y+RvRect.h  );
	TPoint RvLB(RvRect.x           , RvRect.y+RvRect.h  );

	TPoint TgLT = Math.OnRotate(RvCP.x, RvCP.y, RvLT.x, RvLT.y, aMv);
	TPoint TgRT = Math.OnRotate(RvCP.x, RvCP.y, RvRT.x, RvRT.y, aMv);
	TPoint TgRB = Math.OnRotate(RvCP.x, RvCP.y, RvRB.x, RvRB.y, aMv);
	TPoint TgLB = Math.OnRotate(RvCP.x, RvCP.y, RvLB.x, RvLB.y, aMv);

	int l = TgLT.x<TgLB.x ? TgLT.x:TgLB.x;
	int t = TgLT.y<TgRT.y ? TgLT.y:TgRT.y;
	int r = TgRT.x>TgRB.x ? TgRT.x:TgRB.x;
	int b = TgLB.y>TgRB.y ? TgLB.y:TgRB.y;

	TgRect.x = l;
	TgRect.y = t;
	TgRect.w = r-l;
	TgRect.h = b-t;

	*lpRect = TgRect;
}

void HRect::GetRotateRoi(HRect *lpRect, int xMv, int yMv, float aMv, TFPoint *TgLT, TFPoint *TgRT, TFPoint *TgRB, TFPoint *TgLB)
{
	HRect RvRect = *lpRect;
	HMath Math;

	TFPoint RvCP((float)(RvRect.x+RvRect.w/2), (float)(RvRect.y+RvRect.h/2));
	TFPoint RvLT((float)(RvRect.x           ), (float)(RvRect.y           ));
	TFPoint RvRT((float)(RvRect.x+RvRect.w  ), (float) RvRect.y            );
	TFPoint RvRB((float)(RvRect.x+RvRect.w  ), (float)(RvRect.y+RvRect.h)  );
	TFPoint RvLB((float)(RvRect.x           ), (float)(RvRect.y+RvRect.h)  );

	*TgLT = Math.OnRotate(RvCP.x, RvCP.y, RvLT.x, RvLT.y, aMv);
	*TgRT = Math.OnRotate(RvCP.x, RvCP.y, RvRT.x, RvRT.y, aMv);
	*TgRB = Math.OnRotate(RvCP.x, RvCP.y, RvRB.x, RvRB.y, aMv);
	*TgLB = Math.OnRotate(RvCP.x, RvCP.y, RvLB.x, RvLB.y, aMv);
}

void HRect::GetRotateRoi(HRect *lpRect, TFPoint CP, int xMv, int yMv, float aMv, TFPoint *TgLT, TFPoint *TgRT, TFPoint *TgRB, TFPoint *TgLB)
{
	HRect RvRect = *lpRect;
	HMath Math;

	TFPoint RvCP((float)CP.x, (float)CP.y);
	TFPoint RvLT((float)RvRect.x           ,(float)RvRect.y           );
	TFPoint RvRT((float)(RvRect.x+RvRect.w),(float)RvRect.y           );
	TFPoint RvRB((float)(RvRect.x+RvRect.w),(float)(RvRect.y+RvRect.h));
	TFPoint RvLB((float)RvRect.x           ,(float)(RvRect.y+RvRect.h));

	*TgLT = Math.OnRotate(RvCP.x, RvCP.y, RvLT.x, RvLT.y, aMv);
	*TgRT = Math.OnRotate(RvCP.x, RvCP.y, RvRT.x, RvRT.y, aMv);
	*TgRB = Math.OnRotate(RvCP.x, RvCP.y, RvRB.x, RvRB.y, aMv);
	*TgLB = Math.OnRotate(RvCP.x, RvCP.y, RvLB.x, RvLB.y, aMv);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------
// Math Unit
//--------------------------------------------------------------------------------------------------------------------------------------------------
HMath Math;

HMath::HMath()
{
	
}
//--------------------------------------------------------------------------------

HMath::~HMath()
{
	
}
//--------------------------------------------------------------------------------

float __fastcall HMath::GetRenewDegree(float x1, float y1, float x2, float y2, float Angle)
{
	Angle = (float)fabs(Angle);

	TFPoint P1;
	P1.x = x1;
	P1.y = y1;

	TFPoint P2;
	P2.x = x2;
	P2.y = y2;

	bool IsHor = (fabs(P1.x-P2.x))>(fabs(P1.y-P2.y));

	int IsPlus = 1;

	if (IsHor) {
		if (x1<x2) { IsPlus = y1>y2 ?  1 : -1; }
		else       { IsPlus = y1>y2 ? -1 :  1; }
	}
	else {
		if (y1>y2) { IsPlus = x1>x2 ? 1 : -1; }
		else       { IsPlus = x1>x2 ? 1 : -1; }
	}

	if (Angle>90) Angle -= 90;
	if (Angle>90) Angle -= 90;
	if (Angle>90) Angle -= 90;
	if (Angle>90) Angle -= 90;
	if (Angle>45) Angle -= 90;

	Angle = (float)fabs(Angle);

	Angle *= IsPlus;

	return Angle;
}
//--------------------------------------------------------------------------------

float __fastcall HMath::GetAngle(float x1, float y1, float x2, float y2)
{
    double rad;
    double dx, dy, dl;
	
    dx = x2 - x1;
    dy = y2 - y1;
    dl = sqrt(pow(dx, 2) + pow(dy, 2));                       
	
    if (dl==0) return 0.0;
	
    rad = asin(fabs(dy/dl));
	
    if (dx >= 0 && dy >= 0) rad =          rad;
    if (dx  < 0 && dy >= 0) rad = M_PI   - rad;
    if (dx  < 0 && dy  < 0) rad = M_PI   + rad;
    if (dx >= 0 && dy  < 0) rad = M_PI*2 - rad;
	
    float Angle = (float)((rad*180)/M_PI);
    return Angle;
}
//--------------------------------------------------------------------------
double __fastcall HMath::GetAngle(TPoint P1, TPoint P2, TPoint P3)
{
	//--------------------------------------------------------------------------
	// 세 점 사이의 각 구하기 (중심점은 P2)
	//--------------------------------------------------------------------------
	double dAngle;
	double dLine1Gradient, dLine2Gradient;
	if(P1.y-P2.y==0 || P2.y-P3.y==0) return 0;

	dLine1Gradient = (P1.x-P2.x)/(P1.y-P2.y);
	dLine2Gradient = (P2.x-P3.x)/(P2.y-P3.y);
	dAngle = atan(dLine1Gradient);
	dAngle += atan(dLine2Gradient);
	return (dAngle*180)/M_PI * -1;
}
//--------------------------------------------------------------------------
float __fastcall HMath::GetDegree(float x1, float y1, float x2, float y2)
{
	double rad;
	double dx, dy, dl;

	dx = x2 - x1;
	dy = y2 - y1;
	dl = sqrt(pow(dx, 2) + pow(dy, 2));

	if (dl==0) return 0.0;

	rad = asin(fabs(dy/dl));

	if (dx >= 0 && dy >= 0) rad =          rad;
	if (dx  < 0 && dy >= 0) rad = M_PI   - rad;
	if (dx  < 0 && dy  < 0) rad = M_PI   + rad;
	if (dx >= 0 && dy  < 0) rad = M_PI*2 - rad;

	float Angle = (float)((rad*180)/M_PI);
	Angle = GetRenewDegree(x1, y1, x2, y2, Angle);
	return Angle;
}
//--------------------------------------------------------------------------------

float __fastcall HMath::GetDegree(int x1, int y1, int x2, int y2)
{
	double rad;
	double dx, dy, dl;

	dx = x2 - x1;
	dy = y2 - y1;
	dl = sqrt(pow(dx, 2) + pow(dy, 2));

	if (dl==0) return 0.0;

	rad = asin(fabs(dy/dl));

	if (dx >= 0 && dy >= 0) rad =          rad;
	if (dx  < 0 && dy >= 0) rad = M_PI   - rad;
	if (dx  < 0 && dy  < 0) rad = M_PI   + rad;
	if (dx >= 0 && dy  < 0) rad = M_PI*2 - rad;

	return (rad*180)/M_PI;
}
//--------------------------------------------------------------------------------

TFPoint __fastcall HMath::OnIntersect(TFPoint P1, TFPoint P2, TFPoint P3, TFPoint P4)
{
	//Line #1
	int sign = 1;
	float a1 = P2.y - P1.y;
	if (a1<0){
		sign = -1;
		a1 = sign*a1;
	}

	float b1 = sign*(P1.x-P2.x);
	float c1 = sign*(P1.y*P2.x-P1.x*P2.y);

	//Line #2
	sign = 1;
	float a2 = P4.y - P3.y;
	if (a1<0){
		sign = -1;
		a2 = sign*a2;
	}

	float b2 = sign*(P3.x-P4.x);
	float c2 = sign*(P3.y*P4.x-P3.x*P4.y);

	//Intersection Point
	TFPoint Point(0,0);
	float d = a1*b2 - a2*b1;
	if (d==0) return Point;
	Point.x = (c2*b1-c1*b2)/d;
	Point.y = (a2*c1-a1*c2)/d;
	return Point;
}
//--------------------------------------------------------------------------------

float __fastcall HMath::GetLengthFrPointToPoint(float x, float y, float x1, float y1)
{
	return (float)(sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1)));
}
//--------------------------------------------------------------------------------

float __fastcall HMath::GetLengthFrPointToPoint(TFPoint P1, TFPoint P2)
{
	return GetLengthFrPointToPoint(P1.x, P1.y, P2.x, P2.y);
}
//--------------------------------------------------------------------------------

float __fastcall HMath::GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3)
{
	float xDiff = P2.x-P1.x;
	float yDiff = P2.y-P1.y;

	if      (xDiff == 0) return (float)((fabs(P3.x-P1.x)));
	else if (yDiff == 0) return (float)((fabs(P3.y-P1.y)));

	float m    = yDiff/(float)xDiff;
	float dist = (float)(fabs((-m*P3.x + P3.y + m*P1.x - P1.y)/sqrt(m*m + 1)));

	return dist;
}
//--------------------------------------------------------------------------------
float __fastcall HMath::GetLengthFrLineToPoint(TPoint P1, TPoint P2, TPoint P3)
{
    //--------------------------------------------------------------------------
    // 직선과 한점의 수직한 교차점에 의한 치수 측정
    // 공식 : ax + by + c = 0, (m, n) -> Distance = (ma + nb +c) / sqrt(a*a + b*b)
    //--------------------------------------------------------------------------
	
    int xDiff = P2.x-P1.x;
    int yDiff = P2.y-P1.y;
	
	if      (xDiff == 0) return (float)((fabs((double)(P3.x-P1.x))));
	else if (yDiff == 0) return (float)((fabs((double)(P3.y-P1.y))));
	
    float m    = (float)(yDiff/xDiff);
    float dist = (float)(fabs((-m*P3.x + P3.y + m*P1.x - P1.y)/sqrt(m*m + 1)));
	
    return dist;
}
//--------------------------------------------------------------------------------
double __fastcall HMath::OnLineDistance(double x1, double y1, double x2, double y2, double x, double y)
{
	if ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) != 0) {
		double u  = ((x-x1)*(x2-x1) + (y-y1)*(y2-y1));
		double xp = x1 + u * (x2-x1);
		double yp = y1 + u * (y2-y1);
		return sqrt((xp-x)*(xp-x) + (yp-y)*(yp-y));
	}
	return sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1));
}
//--------------------------------------------------------------------------------
TFPoint __fastcall HMath::OnRotate(double cx, double cy, double tx, double ty, double deg)
{
	TFPoint np;

	double Rad = deg * M_PI/180.0;
	double COS = cos(Rad);
	double SIN = sin(Rad);

	if (deg ==  90) {COS =  0; SIN =  1;}
	if (deg == 180) {COS = -1; SIN =  0;}
	if (deg == 270) {COS =  0; SIN = -1;}
	if (deg == 360) {COS =  1; SIN =  0;}

	np.x  = (float)((tx-cx)*COS - (ty-cy)*SIN);
	np.y  = (float)((tx-cx)*SIN + (ty-cy)*COS);
	np.x += (float)cx;
	np.y += (float)cy;

	return np;
}
//------------------------------------------------------------------------------
TPoint __fastcall HMath::OnRotate(int cx, int cy, int tx, int ty, float deg)
{
	TPoint np;
	double Rad = deg *M_PI/180.0;
	double COS = cos(Rad);
	double SIN = sin(Rad);

	if (deg ==  90) {COS =  0; SIN =  1;}
	if (deg == 180) {COS = -1; SIN =  0;}
	if (deg == 270) {COS =  0; SIN = -1;}
	if (deg == 360) {COS =  1; SIN =  0;}

	np.x =(int)((tx-cx)*COS - (ty-cy)*SIN);
	np.y =(int)((tx-cx)*SIN + (ty-cy)*COS);

	np.x += (int)cx;
	np.y += (int)cy;

	return np;                       
}
//------------------------------------------------------------------------------
TFPoint __fastcall HMath::OnRotate(float cx, float cy, float tx, float ty, float deg)
{
	TFPoint np;

	double Rad = deg * M_PI/180.0;
	double COS = cos(Rad);
	double SIN = sin(Rad);

	if (deg ==  90) {COS =  0; SIN =  1;}
	if (deg == 180) {COS = -1; SIN =  0;}
	if (deg == 270) {COS =  0; SIN = -1;}
	if (deg == 360) {COS =  1; SIN =  0;}

	np.x = (float)((tx-cx)*COS - (ty-cy)*SIN);
	np.y = (float)((tx-cx)*SIN + (ty-cy)*COS);

	np.x += cx;
	np.y += cy;

	return np;
}
//----------------------------------------------------------------------------
float __fastcall HMath::GetLength(TPoint p1, TPoint p2)
{
    return (float)(sqrt(pow((double)(p1.x-p2.x),(double)(2)) + pow((double)(p1.y-p2.y), (double)(2))));
}
//--------------------------------------------------------------------------------
float __fastcall HMath::GetLength(TFPoint p1, TFPoint p2)
{
    return (float)(sqrt(pow((double)(p1.x-p2.x),(double)(2)) + pow((double)(p1.y-p2.y),(double)(2))));
}
//--------------------------------------------------------------------------------
TFPoint __fastcall HMath::GetPointFrLineToPoint(TFPoint P1 , TFPoint P2 , TFPoint P3 )
{
	//--------------------------------------------------------------------------
	// 직선과 한점을 알경우 수직으로 교차하는 점을 찾아내는 방정식
	//--------------------------------------------------------------------------
// 	if ((P2.x-P1.x)==0) return P1;
	if((P2.x-P1.x)==0){P3.x = P1.x; return P3;}
	if((P2.y-P1.y)==0){P3.y = P1.y; return P3;}

	TFPoint Point(0.0f,0.0f);
	float a = (P2.y -P1.y)/(P2.x - P1.x);
	Point.x = ((pow(a,2)*P1.x) + a*(P3.y - P1.y) + P3.x)/(pow(a,2)+1);
	Point.y = a*(Point.x - P1.x) + P1.y;

	//if(((Point.x - P1.x)*(Point.y - P2.x))<=0 && ((Point.y - P1.y)*(Point.y - P2.y))<=0){

	return Point;
}
//----------------------------------------------------------------------------------
TPoint __fastcall HMath::GetPointFrLineToPoint(TPoint P1 , TPoint P2 , TPoint P3 )
{
	//--------------------------------------------------------------------------
	// 직선과 한점을 알경우 수직으로 교차하는 점을 찾아내는 방정식
	//--------------------------------------------------------------------------
// 	if ((P2.x-P1.x)==0) return P1;
	if((P2.x-P1.x)==0){P3.x = P1.x; return P3;}
	if((P2.y-P1.y)==0){P3.y = P1.y; return P3;}

	TPoint Point(0,0);
	float a = (P2.y -P1.y)/(P2.x - P1.x);
	Point.x = ((pow(a,2)*P1.x) + a*(P3.y - P1.y) + P3.x)/(pow(a,2)+1);
	Point.y = a*(Point.x - P1.x) + P1.y;

	//if(((Point.x - P1.x)*(Point.y - P2.x))<=0 && ((Point.y - P1.y)*(Point.y - P2.y))<=0) return;
	return Point;
}
//--------------------------------------------------------------------------------
// TFPoint __fastcall HMath::OnFindFIntersection(TFLine l1,TFLine l2){
// 
// 	TFPoint Point(0,0);
// 	float d = l1.a*l2.b - l2.a*l1.b;
// 	if (d==0) return Point;
// 	Point.x = (l2.c*l1.b - l1.c*l2.b)/d;
// 	Point.y = (l2.a*l1.c - l1.a*l2.c)/d;
// 	return Point;
// }
// //--------------------------------------------------------------------------------
// TPoint __fastcall HMath::OnFindIntersection(TLinear Lx, TLinear Ly)
// {
// 	TPoint Point(0, 0);
// 
// 	if (Lx.a==Ly.a && !Lx.IsUnLimit && !Ly.IsUnLimit) return Point;
// 
// 	if (Lx.IsUnLimit) {
// 		Point.x = Lx.x;
// 		Point.y = Ly.a*Point.x + Ly.b;
// 		return Point;
// 	}
// 
// 	if (Ly.IsUnLimit) {
// 		Point.x = Ly.x;
// 		Point.y = Lx.a*Point.x + Lx.b;
// 		return Point;
// 	}
// 
// 	Point.x = (Lx.b-Ly.b)/float(Ly.a-Lx.a);
// 	Point.y = Lx.a*Point.x + Lx.b;
// 
// 	return Point;
// }
//--------------------------------------------------------------------------------
TPoint __fastcall HMath::OnFindIntersectionPoint(TPoint P1, TPoint P2, TPoint P3)
{
	TPoint P(0,0);
	if (P1.x==P2.x) return P;
	double k = (P2.y-P1.y)/(P2.x-P1.x);
	// linesge function Y = k*(x-P1.x)+p1.y;
	// -1/k;
	// Y = (-1/k)*(x-P.x)+P.y;
	P.x = (k*k*P1.x + k*(P3.y-P1.y) + P3.x) / (k*k+1);
	P.y = k*(P.x - P1.x) + P1.y;
	return P;
}
//--------------------------------------------------------------------------------
float __fastcall HMath::GetTwoLineAngle(TFPoint p1,TFPoint p2,TFPoint p3,TFPoint p4){

	float k,h,s,angle;
	k=(p2.x-p1.x)*(p4.x-p3.x)+(p2.y-p1.y)*(p4.y-p3.y);
	h=sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y))*sqrt((p3.x-p4.x)*(p3.x-p4.x)+(p3.y-p4.y)*(p3.y-p4.y));
	if(h==0) return 0.0;
	s=acos(fabs(k)/(h*1.0));
	angle = s*180/M_PI;
	return angle;
}
//--------------------------------------------------------------------------------
/*
void __fastcall HMath::Intersection4Point(TFPoint *P, TPoint *Plt, TPoint *Prt, TPoint *Plb, TPoint *Prb){

	TFLine ll1,ll2,ll3,ll4;
	float angle1,angle2,angle3,angle4,ang[4];
	float width,height;
	TFPoint point1,point2,point3,point4;
	TFPoint pot1,pot2,pot3,pot4;

	TFLine l1(P[0],P[1]);
	TFLine l2(P[2],P[3]);
	TFLine l3(P[4],P[5]);
	TFLine l4(P[6],P[7]);

	angle1 = GetTwoLineAngle(P[0],P[1],P[2],P[3]); ang[0] = fabs(angle1)-90;
	angle2 = GetTwoLineAngle(P[2],P[3],P[4],P[5]); ang[1] = fabs(angle2)-90;
	angle3 = GetTwoLineAngle(P[4],P[5],P[6],P[7]); ang[2] = fabs(angle3)-90;
	angle4 = GetTwoLineAngle(P[6],P[7],P[0],P[1]); ang[3] = fabs(angle4)-90;

	float t;
	for(int i=0;i<4;i++){
		for(int j=1;j<4-i;j++){
			if(ang[i]>ang[j]){
				t=ang[i]; ang[i]=ang[j]; ang[j]=t;
			}
		}
	}

	if ((ang[0]+90)==angle1) {
		point1 = OnFindFIntersection(l1,l2);
		point2 = OnFindFIntersection(l2,l3);
		point4 = OnFindFIntersection(l1,l4);
		float dist1,dist2;
		dist1 = GetLength(P[1],P[2]);
		dist2 = GetLength(P[1],P[4]);
		width = (dist1<dist2)?dist1:dist2;
		height = (dist1>dist2)?dist1:dist2;

		ll3.a = l1.a;
		ll3.b = l1.b;
		ll3.c = l1.c - dist1*sqrt(l1.a*l1.a+l1.b*l1.b);

		ll4.a = l2.a;
		ll4.b = l2.b;
		ll4.c = l2.c - dist2*sqrt(l2.a*l2.a+l2.b*l2.b);

		pot1 = point1;
		pot2 = point2;
		pot3 = OnFindFIntersection(ll3,ll4);
		pot4 = point4;
	}

	if ((ang[0]+90)==angle2) {
		point1 = OnFindFIntersection(l1,l2);
		point2 = OnFindFIntersection(l2,l3);
		point3 = OnFindFIntersection(l3,l4);
		float dist1,dist2;
		dist1 = GetLength(P[1],P[2]);
		dist2 = GetLength(P[2],P[3]);
		width = (dist1<dist2)?dist1:dist2;
		height = (dist1>dist2)?dist1:dist2;

		ll1.a = l3.a;
		ll1.b = l3.b;
		ll1.c = l3.c + dist1*sqrt(l3.a*l3.a+l3.b*l3.b);

		ll4.a = l2.a;
		ll4.b = l2.b;
		ll4.c = l2.c - dist2*sqrt(l2.a*l2.a+l2.b*l2.b);

		pot1 = point1;
		pot2 = point2;
		pot3 = point3;
		pot4 = OnFindFIntersection(ll1,ll4);
	}

	if ((ang[0]+90)==angle3) {
		point2 = OnFindFIntersection(l2,l3);
		point3 = OnFindFIntersection(l3,l4);
		point4 = OnFindFIntersection(l1,l4);
		float dist1,dist2;
		dist1 = GetLength(P[2],P[3]);
		dist2 = GetLength(P[3],P[4]);
		width = (dist1<dist2)?dist1:dist2;
		height = (dist1>dist2)?dist1:dist2;

		ll1.a = l3.a;
		ll1.b = l3.b;
		ll1.c = l3.c + dist1*sqrt(l3.a*l3.a+l3.b*l3.b);

		ll2.a = l4.a;
		ll2.b = l4.b;
		ll2.c = l4.c + dist2*sqrt(l4.a*l4.a+l4.b*l4.b);

		pot1 = OnFindFIntersection(ll1,ll2);
		pot2 = point2;
		pot3 = point3;
		pot4 = point4;
	}

	if ((ang[0]+90)==angle4) {
		point1 = OnFindFIntersection(l1,l2);
		point3 = OnFindFIntersection(l3,l4);
		point4 = OnFindFIntersection(l1,l4);
		float dist1,dist2;
		dist1 = GetLength(P[3],P[4]);
		dist2 = GetLength(P[1],P[4]);
		width = (dist1<dist2)?dist1:dist2;
		height = (dist1>dist2)?dist1:dist2;

		ll2.a = l4.a;
		ll2.b = l4.b;
		ll2.c = l4.c + dist1*sqrt(l4.a*l4.a+l4.b*l4.b);

		ll3.a = l1.a;
		ll3.b = l1.b;
		ll3.c = l1.c - dist2*sqrt(l1.a*l1.a+l1.b*l1.b);

		pot1 = point1;
		pot2 = OnFindFIntersection(ll2,ll3);
		pot3 = point3;
		pot4 = point4;
	}

	Plt->x = pot1.x;   Plt->y = pot1.y;
	Prt->x = pot2.x;   Prt->y = pot2.y;
	Plb->x = pot3.x;   Plb->y = pot3.y;
	Prb->x = pot4.x;   Prb->y = pot4.y;
}
*/
//--------------------------------------------------------------------------------
TPoint __fastcall HMath::OnFindPerpendicularPointX(TPoint Pt, float len, float ap)
{
	TPoint Point(0,0);
	float  m   = ap!=0 ? -1/ap : 0;
	int   x1   = Pt.x;
	int   y1   = Pt.y;
	float  a    ;
	float  b    ;
	float  c    ;
	double temp ;

	int x2_1    ;                                                            // (m2^2+1)*x2^2 - 2*(m2^2+1)*x1*x2 + (m2^2+1)*x1^2 - l^2 = 0
	int x2_2    ;                                                            // ax^2 + bx + c = 0 -> (-b+- sqrt(b^2-4ac))/2a

	if (m==0) {Point.x = x1+len, Point.y = y1; return Point;}
	//Calculation Point
	a     = pow(m,2)+1 ;
	b     = -2*x1*(pow(m,2)+1);
	c     = (pow(m,2)+1)*pow((float)x1,2)-pow(len,2);
	temp  = sqrt(fabs(pow(b,2)-4*a*c));

	x2_1  = (-b+temp)/(2*a)+0.5;
	x2_2  = (-b-temp)/(2*a)+0.5;

	if (len>=0) Point.x = x2_1 > x2_2 ? x2_1 : x2_2;
	if (len<0 ) Point.x = x2_1 < x2_2 ? x2_1 : x2_2;

	Point.y = m*(Point.x-x1)+y1+0.5;                                             // y2 = m(x2-x1)+y1

	return Point;
}
//--------------------------------------------------------------------------------
TFPoint __fastcall HMath::OnFindPerpendicularPointX(TFPoint Pt, float len, float ap)
{
	TFPoint Point(0.0f, 0.0f);
	float  m   = ap!=0 ? -1/ap : 0;
	int    x1   = Pt.x;
	int    y1   = Pt.y;
	float  a    ;
	float  b    ;
	float  c    ;
	double temp ;

	int x2_1    ;                                                            // (m2^2+1)*x2^2 - 2*(m2^2+1)*x1*x2 + (m2^2+1)*x1^2 - l^2 = 0
	int x2_2    ;                                                            // ax^2 + bx + c = 0 -> (-b+- sqrt(b^2-4ac))/2a

	if (m==0) {Point.x = x1+len, Point.y = y1; return Point;}
	//Calculation Point
	a     = pow(m,2)+1 ;
	b     = -2*x1*(pow(m,2)+1);
	c     = (pow(m,2)+1)*pow((float)x1,2)-pow(len,2);
	temp  = sqrt(fabs(pow(b,2)-4*a*c));

	x2_1  = (-b+temp)/(2*a);
	x2_2  = (-b-temp)/(2*a);

	if (len>=0) Point.x = x2_1 > x2_2 ? x2_1 : x2_2;
	if (len<0 ) Point.x = x2_1 < x2_2 ? x2_1 : x2_2;

	Point.y = m*(Point.x-x1)+y1;                                             // y2 = m(x2-x1)+y1

	return Point;
}
//--------------------------------------------------------------------------------
TPoint __fastcall HMath::OnFindPerpendicularPointY(TPoint Pt, float len, float ap)
{
	TPoint Point(0,0);
	float  m   = ap!=0 ? -1/ap : 0;
	int   x1   = Pt.x;
	int   y1   = Pt.y;
	float  a    ;
	float  b    ;
	float  c    ;
	double temp ;

	int y2_1;                                                                // (1/m2^2+1)*y2^2 - 2*(1/m2^2+1)*y1*y2 + (1/m2^2+1)*y1^2 - l^2 = 0
	int y2_2;                                                                // ax^2 + bx + c = 0 -> (-b+- sqrt(b^2-4ac))/2a

	if (m==0) {Point.x = x1, Point.y = y1+len; return Point;}
	//Calculation Point
	a     = 1/pow(m,2)+1 ;
	b     = -2*y1*(1/pow(m,2)+1);
	c     = (1/pow(m,2)+1)*pow((float)y1,2)-pow(len,2);
	temp  = sqrt(fabs(pow(b,2)-4*a*c));

	y2_1  = (-b+temp)/(2*a);
	y2_2  = (-b-temp)/(2*a);

	if (len>=0) Point.y = y2_1 > y2_2 ? y2_1 : y2_2;
	if (len< 0) Point.y = y2_1 < y2_2 ? y2_1 : y2_2;

	Point.x = 1/m*(Point.y-y1)+x1;                                          // x2 = 1/m(y2-y1)+x1

	return Point;
}
//--------------------------------------------------------------------------------
TFPoint __fastcall HMath::OnFindPerpendicularPointY(TFPoint Pt, float len, float ap)
{
	TFPoint Point(0.0f, 0.0f);
	float  m   = ap!=0 ? -1/ap : 0;
	int   x1   = Pt.x;
	int   y1   = Pt.y;
	float  a    ;
	float  b    ;
	float  c    ;
	double temp ;

	int y2_1;                                                                // (1/m2^2+1)*y2^2 - 2*(1/m2^2+1)*y1*y2 + (1/m2^2+1)*y1^2 - l^2 = 0
	int y2_2;                                                                // ax^2 + bx + c = 0 -> (-b+- sqrt(b^2-4ac))/2a

	if (m==0) {Point.x = x1, Point.y = y1+len; return Point;}
	//Calculation Point
	a     = 1/pow(m,2)+1 ;
	b     = -2*y1*(1/pow(m,2)+1);
	c     = (1/pow(m,2)+1)*pow((float)y1,2)-pow(len,2);
	temp  = sqrt(fabs(pow(b,2)-4*a*c));

	y2_1  = (-b+temp)/(2*a);
	y2_2  = (-b-temp)/(2*a);

	if (len>=0) Point.y = y2_1 > y2_2 ? y2_1 : y2_2;
	if (len< 0) Point.y = y2_1 < y2_2 ? y2_1 : y2_2;

	Point.x = 1/m*(Point.y-y1)+x1;                                          // x2 = 1/m(y2-y1)+x1

	return Point;
}
//--------------------------------------------------------------------------------
double __fastcall HMath::GetAngle3Point(TPoint P1, TPoint P2, TPoint P3)
{
	double c = Math.GetLength(P1, P3);
	double b = Math.GetLength(P2, P1);
	double a = Math.GetLength(P2, P3);

	double angle = (a*a + b*b - c*c) / (2*a*b);
	angle = acos(angle);

	return (angle*180)/M_PI;
}
//--------------------------------------------------------------------------------
TPoint __fastcall HMath::distanceByDeg(TPoint pt, double deg, float Length)
{
	double rad = deg *M_PI/180.0;
// 	double rad = TheMath::deg2rad(deg);
	return TPoint(int(pt.x + Length * cos(rad)), int(pt.y + Length * sin(rad)));
}



