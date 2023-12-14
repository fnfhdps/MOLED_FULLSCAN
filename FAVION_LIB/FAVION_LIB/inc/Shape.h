#pragma once

#include <cmath>
#include <limits>
#include <iostream>
#include <vector>
#include <assert.h>


#ifdef DLL_EXPORTS
#define DllExport   __declspec( dllexport )
#else
#define DllExport   __declspec( dllimport )
#endif

namespace The
{

#define mm2um(v)	(v * 1000)
#define um2mm(v)	(v / 1000)

#pragma push_macro("min")
#undef min

//	typedef std::basic_string<TCHAR>		tstring;
// 	typedef std::basic_ofstream<TCHAR>		tifstream;
// 	typedef std::basic_ofstream<TCHAR>		tofstream;
// 	typedef std::basic_stringstream<TCHAR>	tstringstream;

	using namespace std;

	class DllExport TheMath
	{
	public:
		TheMath();
		~TheMath();

		/// <summary>
		/// Degrees각도를 라디안각으로 환산. 0 ~ 2pi
		/// </summary>
		/// <param name="deg">degrees 각도.</param>
		/// <returns>라디안 각도.</returns>
		static double deg2rad(double deg);

		/// <summary>
		/// 라디안 -> 도로 환산 (0 ~ 180, 0 ~ -180)
		//	(0,0) -> (1,1) : 45도
		//	(0,0) -> (0,1) : 90도
		//	(0,0) -> (-1,1): 135도
		//	(0,0) -> (-1,0): 180도

		//	(0,0) -> (-1,-1): -135도
		//	(0,0) -> (0,-1): -90도(== 270)
		//	(0,0) -> (1,-1): -45도	
		//	(0,0) -> (1,0) : 0도(360)
		/// </summary>
		/// <param name="rad"> degrees 각도.</param>
		/// <returns> 라디안 각도.</returns>
		static double rad2deg(double rad);

		/// <summary>
		/// 도를 360도 범위로 변환. (0 ~ 360 단, 360 == 0)
		/// </summary>
		/// <param name="deg"> degrees 각도.</param>
		/// <returns>360 단위 각도.</returns>
		static double deg360(double deg);

		/// <summary>
		/// 라디안 각도를 360도 각도로 환산. (0 ~ 360 단, 360 == 0)
		/// </summary>
		/// <param name="rad">라디안 각도.</param>
		/// <returns>double.</returns>
		static double rad360(double rad);
	};

	//resize hit cursor type
	typedef enum 
	{
		SIZENONE,	//IDC_ARROW
		SIZEALL	,	//IDC_SIZEALL
		SIZENWSE,	//IDC_SIZENWSE
		SIZENESW,	//IDC_SIZENESW
		SIZEWE	,	//IDC_SIZEWE
		SIZENS	,	//IDC_SIZENS
	} Resizer;


	typedef enum
	{
		HitPoint  = 0x100,
		
		HitLineCC = 0x200,
		HitLineP1 = 0x210,
		HitLineP2 = 0x220,
		
		HitRectCC = 0x300,
		HitRectTC = 0x380,
		HitRectRT = 0x330,
		HitRectRC = 0x370,
		HitRectRB = 0x340,
		HitRectBC = 0x360,
		HitRectLB = 0x320,
		HitRectLC = 0x350,
		HitRectLT = 0x310,

		HitCircleCC = 0x400,
		HitCircle00 = 0x410,
		HitCircle45 = 0x420,
		HitCircle90 = 0x430,
		HitCircle135 = 0x440,
		HitCircle180 = 0x450,
		HitCircle225 = 0x460,
		HitCircle270 = 0x470,
		HitCirCle315 = 0x480

	} HitId;

	typedef enum {
		ShapeNone	,
		ShapePoint	,
		ShapeLine	,
		ShapeRect	,
		ShapeCircle	
	} ShapeType;

	//ResizerType 설명
	//최하위 4bits는 커서 타입으로 각 도형별 크기 조정 방향 결정(Resizer)
	//두번째 4bits는 크기 조정 방향 포인트 번호(HitId)
	//세번째 4bits는 도형 유형 결정(ShapeType)
	typedef	int			ResizerType;

	inline ResizerType	ResizerHit(ResizerType	r, HitId h)	{ return (r | h); }
	inline Resizer		GetResizer(ResizerType	rstype)		{ return (Resizer)	(rstype & 0xf);		}
	inline HitId		GetHit(ResizerType		rstype)		{ return (HitId)	(rstype & 0xff0);	}
	inline ShapeType	GetShape(ResizerType	rstype)		{ return (ShapeType)(rstype >> 8);		}


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	template<typename T>
	class Shape
	{
	public:
		virtual	T			mid_x() = 0;		//도형의 중심 좌표 x
		virtual	T			mid_y() = 0;		//도형의 중심 좌표 y
		virtual T			area() = 0;			//도형의 면적
		virtual	T			perimeter() = 0;	//도형의 둘레 길이

		//크기 조정 없는 그리기
		virtual	void		draw(HDC _hdc) = 0;
		//크기 조정하는 그리기(도형 타입과 크기 조정 포인트(히트포인트) 번호의 조합 리턴)
		virtual	ResizerType	drawHit(HDC _hdc, int _x, int _y, int hit_rng = 5) = 0;
		virtual void		drawResizable(HDC _hdc) = 0;
		//drawHit에서 리턴 받은 ResizerType으로 부터 얻은 HitId와 조정 옵셋을 통해 호출(MouseMove이벤트)
		virtual	void		resize(HitId hid, const T& delta_x, const T& delta_y) = 0;

		/// <summary>
		/// Draws the tracker box.
		/// </summary>
		/// <param name="_hdc">The HDC.</param>
		/// <param name="_x">x 좌표.</param>
		/// <param name="_y">y 좌표.</param>
		/// <param name="size"> tracker box의 크기(중심점(x,y)를 기준으로 +/- 크기.</param>
		virtual	void		drawTrackBox(HDC _hdc, int _x, int _y, int size = 5)
		{
			int l = (int)(_x - size);
			int t = (int)(_y - size);
			int r = (int)(_x + size);
			int b = (int)(_y + size);
			::MoveToEx(_hdc, l, t, NULL);
			::LineTo(_hdc, l, b);	::LineTo(_hdc, r, b);	::LineTo(_hdc, r, t);	::LineTo(_hdc, l, t);
		}

		/// <summary>
		/// hit된 tracker의 색깔을 얻는다. 그려진 도형 생상의 2의 보수를 취한 색깔
		/// </summary>
		/// <param name="_hdc">The HDC.</param>
		/// <returns> hit된 trracker를 그리기 위한 색상. </returns>
		virtual COLORREF getHitColor(HDC _hdc)
		{
			HGDIOBJ obj = GetCurrentObject(_hdc, OBJ_PEN);

			LOGPEN pen;
			GetObject(obj, sizeof(LOGPEN), &pen);
			return ~pen.lopnColor;
		}
	};

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	template <typename T>
	class Point : public /*virtual*/ Shape<T>
	{
	public:
		T x;
		T y;

		Point()
			: x(0), y(0) {}

		Point(const Point<T>& p)		{	x = p.x;	y = p.y;	}
		Point(const T _x, const T _y)	{	x = _x;		y = _y;		}

		~Point() {}

		bool empty() const
		{
			return (x == 0 && y == 0);
		}

		//////////////////////////////////////////////////////////////////////////
		//	2017.2.08 type conversion between defferent T type
		template <typename U>
		Point(const Point<U>& p)
		{
			x = static_cast<T>p.x;
			y = static_cast<T>p.y;
		}

		template <typename U>
		operator Point<U>()
		{
			return Point<U>(x, y);
		}
		//////////////////////////////////////////////////////////////////////////


		Point<T>& add(const T _x, const T _y)	{	x += _x;	y += _y;	return *this;	}
		Point<T>& add(const Point<T>& p)		{	return add(p.x, p.y);					}

		Point<T>& sub(const T _x, const T _y)	{	x -= _x;	y -= _y;	return *this;	}
		Point<T>& sub(const Point<T>& p)		{	return sub(p.x, p.y);					}

		Point<T>& mul(T _x, T _y)				{	x *= _x; y *= _y;		return *this;	}
		Point<T>& mul(const Point<T>& p)		{	return mul(p.x, p.y);					}

		Point<T>& div(T _x, T _y)
		{
			x /= ((_x) ? _x : std::numeric_limits<T>::min());
			y /= ((_y) ? _y : std::numeric_limits<T>::min());
			return *this;
		}
		Point<T>& div(const Point<T>& p)	{	return div(p.x, p.y);	}
		
		Point<T>& operator = (const Point<T> &p)		{	if (this != &p) { x = p.x;	y = p.y; }	return *this;	}

		Point<T> operator + (const Point<T>& p) const	{	return Point<T>(x + p.x, y + p.y);	}
		Point<T> operator + (T a) const					{	return Point<T>(x + a, y + a);		}

		Point<T> operator - (const Point<T>& p) const	{	return Point<T>(x - p.x, y - p.y);	}
		Point<T> operator -(T s) const					{	return Point<T>(x - s, y - s);		}

		Point<T> operator * (const Point<T>& p) const	{	return Point<T>(x * p.x, y * p.y);	}
		Point<T> operator * (T m) const					{	return Point<T>(x * m, y * m);		}

		Point<T> operator / (const Point<T>& p) const	
		{
			return Point<T>(x / ((p.x) ? p.x : std::numeric_limits<T>::min()), 
							y / ((p.y) ? p.y : std::numeric_limits<T>::min()));
		}

		Point<T> operator / (T d) const
		{
			return Point<T>(x / ((d) ? d : std::numeric_limits<T>::min()),
							y / ((d) ? d : std::numeric_limits<T>::min()));
		}

		Point<T>& operator+=(const Point<T>& p)			{	x += p.x;	y += p.y;	return *this;	}
		Point<T>& operator+=(T a)						{	x += a;		y += a;		return *this;	}

		Point<T>& operator-=(const Point<T>& p) 		{	x -= p.x;	y -= p.y;	return *this;	}
		Point<T>& operator-=(T s)						{	x -= s;		y -= s;		return *this;	}

		Point<T>& operator*=(const Point<T>& p) 		{	x *= p.x;	y *= p.y;	return *this;	}
		Point<T>& operator*=(T m)						{	x *= m;		y *= m;		return *this;	}

		Point<T>& operator/= (const Point<T>& p)
		{
			x /= ((p.x) ? p.x : std::numeric_limits<T>::min());
			y /= ((p.y) ? p.y : std::numeric_limits<T>::min());
			return *this;
		}

		Point<T>& operator/= (const T d)
		{
			x /= ((d) ? d : std::numeric_limits<T>::min());
			y /= ((d) ? d : std::numeric_limits<T>::min());
			return *this;
		}

		Point<T>& operator++ ()		{	++x;	++y;	return *this; }
		Point<T>& operator++ (int)	{	Point<T> copy(*this);		++*this;	return copy; }
		Point<T>& operator--()		{	--x;	--y;	return *this; }
		Point<T>& operator--(int)	{	Point<T> copy(*this);		--*this;	return copy; }

		friend bool operator== (const Point<T>& p1, const Point<T>& p2) {	return (p1.x == p2.x && p1.y == p2.y);	}
		friend bool operator!= (const Point<T>& p1, const Point<T>& p2) {	return (p1.x != p2.x || p1.y != p2.y);	}

		bool operator <(const Point<T>& p) const	{	return (x < p.x && y < p.y);	}
		bool operator <=(const Point<T>& p) const	{	return (x <= p.x && y <= p.y);	}
		bool operator >(const Point<T>& p) const	{	return (x > p.x && y > p.y);	}
		bool operator >=(const Point<T>& p) const	{	return (x >= p.x && y >= p.y);	}

		void set(T _x, T _y)						{	x = _x;		y = _y;				}

		Point<T> mid(const Point<T>& p)				{	return Point<T>((x + p.x) / 2, (y + p.y) / 2);	}

		T distance(const Point<T>& p) const			
		{
			double dx = x - p.x;
			double dy = y - p.y;
			return static_cast<T> (sqrt(dx*dx + dy*dy));
		}

		static T distance(const Point<T>& p1, const Point<T>& p2) 
		{
			double dx = p1.x - p2.x;
			double dy = p1.y - p2.y;
			return static_cast<T> (sqrt(dx*dx + dy*dy));
		}

		/// <summary>
		/// org를 원점기준으로 라디안 각도 계산.
		/// </summary>
		/// <param name="org">기준점.</param>
		/// <returns>라디안 각도.</returns>
#if (_MSC_VER == 1600)
		double	angleRad(const Point<T>& org) const	{	return atan2((double)(y - org.y), (double)(x - org.x));	}
#else /*if _MSC_VER == 1800*/
		double	angleRad(const Point<T>& org) const	{ return atan2(y - org.y, x - org.x); }
#endif

		/// <summary>
		/// org를 원점으로 degrees 각도 계산.
		/// </summary>
		/// <param name="org">기준점.</param>
		/// <returns>degrees 각도.</returns>
		double angleDeg(const Point<T>& org) const	{	double rad = angleRad(org);	return The::TheMath::rad2deg(rad);	}

		//	this point로 부터 deg각도로 length 만큼 떨어진 점의 좌표
		Point<T> distanceByDeg(double deg, T length) const
		{
			double rad = TheMath::deg2rad(deg);
			return Point<T> ((T)(x + length * cos(rad)), (T)(y + length * sin(rad)));
		}

		Point<T> distanceByRad(double rad, T length) const
		{
			return Point<T> (x + length * cos(rad), y + length * sin(rad));
		}

		//	from point로 부터 deg각도로 length 만큼 떨어진 점의 좌표
		static Point<T> distanceByDeg(const Point<T>& from, double deg, T length) 
		{
			double rad = TheMath::deg2rad(deg);
			return Point<T>(from.x + length * cos(rad), from.y + length * sin(rad));
		}

		Point<T> rotate(double rad) {	return rotate(Point<T>(0, 0), rad);	}
		
		//20171019 오류 수정(그러나, 기존 코드에 문제가(center c를 중심으로 c 자체 회전 계산 오류) 생길 수 있음)
		Point<T> rotate(const Point<T>& c, double rad) const
		{
			double S = sin(rad), C = cos(rad);
			return Point<T>((T)((x - c.x) * C - (y - c.y) * S + c.x), (T)((x - c.x) * S + (y - c.y) * C + c.y));
		}

		static Point<T> rotate(Point<T>& p, Point<T>& c, double deg)	
		{
			return p.rotate(c, TheMath::deg2rad(deg));
		}

		double dot(const Point<T> b) const						{	return (x * b.x + y * b.x);		}
		static double dot(const Point<T> a, const Point<T> b) 	{	return (a.x * b.x + a.y * b.y);	}
		double cross(const Point<T> b) const					{	return (x * b.y - y * b.x);		}
		static double corss(const Point<T> a, const Point<T> b) {	return (a.x * b.y - a.y * b.x);	}

		virtual T		mid_x()			{ return x; }
		virtual	T		mid_y()			{ return y; }
		virtual T		area()			{ return (T)1; };
		virtual	T		perimeter()		{ return (T)1; };
		virtual	void	draw(HDC _hdc)	
		{
		//	COLORREF rgb = GetDCPenColor(_hdc);	//	왜 0인가?
			::SetPixelV(_hdc, (int)x, (int)y, RGB(0,0,255));
		}

		virtual	ResizerType	drawHit(HDC _hdc, int _x, int _y, int hit_rng = 5)
		{
			ResizerType r = SIZENONE;

			draw(_hdc);
			
			if (this->distance(Point<T>((T)_x, (T)_y)) <= hit_rng) 
			{
				drawTrackBox(_hdc, (int)x, (int)y);
				r = ResizerHit(SIZEALL, HitPoint);
			}

			return r;
		}

		virtual void	drawResizable(HDC _hdc)
		{
			draw(_hdc);
			drawTrackBox(_hdc, (int)x, (int)y);
		}

		virtual	void	resize(HitId hid, const T& delta_x, const T& delta_y)
		{
			x += delta_x;
			y += delta_y;
		}

		//	다른 type을 스트림 출력으로 내보낸 후에는 Point<T>를 연이어 내보낼 수 없다.
		//	c++ chaining operator << overload 검색어로 결과는 나오나, 잘 안됨.
		//	단, Point<T> 출력 후 연이어 Point<T>를 출력하는 경우에는 문제가 없는 걸로 봐서
		//	이전 << 호출에서 리턴되는 streamout이 Point<T>의 streamout과 타입이 다르게 인식되어
		//	발생하는 현상으로 예상되나, 확실치 않음.
		friend std::ofstream& operator << (std::ofstream& out, const Point<T>& p)
		{
			out << p.x << " " << p.y;	return out;
		}

		friend std::ifstream& operator >> (std::ifstream& in, Point<T>& p)
		{
			in >> p.x >> p.y;			return in;
		}

		friend std::stringstream& operator << (std::stringstream& out, const Point<T>& p)
		{
			out << p.x << " " << p.y;	return out;
		}

		friend std::stringstream& operator >> (std::stringstream& in, Point<T>& p)
		{
			in >> p.x >> p.y;			return in;
		}
	};
	
	template <typename T>
	class Line : public /*virtual*/ Shape<T>
	{
	public:
		Point<T> P1;
		Point<T> P2;

		Line() 
		{}
		Line(const Point<T>& _p1, const Point<T>& _p2)
		{	P1 = _p1;	P2 = _p2;	}
		void set(const Point<T>& _p1, const Point<T>& _p2)	{	P1 = _p1;	P2 = _p2;	}

		~Line() {}

		Line<T>& operator= (const Line<T>& l) 
		{
			if (this == &l)
				return *this;

			P1 = l.P1;	P2 = l.P2;
			return *this;
		}

		bool empty() const
		{
			return (P1.empty() && P2.empty());
		}

		//////////////////////////////////////////////////////////////////////////
		//	2017.2.08 type conversion between defferent T type
		template <typename U>
		Line(const Line<U>& l)
		{
			P1 = (Point<T>)l.P1;
			P2 = (Point<T>)l.P2;
		}

		template <typename U>
		operator Line<U>()
		{
			return Line<U>(P1, P2);
		}
		//////////////////////////////////////////////////////////////////////////


		Line<T>	operator + (const Point<T>& p)	{ return Line<T>(P1 + p, P2 + p); }
		Line<T> operator - (const Point<T>& p)	{ return Line<T>(P1 - p, P2 - p); }

		Line<T> operator * (const Point<T>& p)	{ return Line<T>(P1 * p, P2 * p); }
		Line<T> operator * (const T& v)			{ return Line<T>(P1 * v, P2 * v); }

		Line<T>	operator / (const Point<T>& p)	{ return Line<T>(P1 / p, P2 / p); }
		Line<T>	operator / (const T& v)			{ return Line<T>(P1 / v, P2 / v); }

		Line<T>& operator += (const Point<T>& p)	{ P1 += p; P2 += p; return *this; }
		Line<T>& operator -= (const Point<T>& p)	{ P1 -= p; P2 -= p; return *this; }

		Line<T>& operator *= (const Point<T>& p)	{ P1 *= p; P2 *= p; return *this; }
		Line<T>& operator *= (const T& v)			{ P1 *= v; P2 *= v; return *this; }

		Line<T>& operator /= (const Point<T>& p)	{ P1 /= p; P2 /= p; return *this; }
		Line<T>& operator /= (const T& v)			{ P1 /= v; P2 /= v; return *this; }

		friend bool operator== (const Line<T>& l1, const Line<T>& l2) {	return (l1.P1 == l2.P1 && l1.P2 == l2.P2);	}
		friend bool operator!= (const Line<T>& l1, const Line<T>& l2) {	return (l1.P1 != l2.P1 || l1.P2 != l2.P2);	}

		double radAngle() 	{	return P2.angleRad(P1);					}
		double degAngle()	{	return TheMath::rad2deg(radAngle());	}

		/// <summary>
		/// l1을 기준으로 l2가 이루고 있는 각을 계산.
		/// </summary>
		/// <param name="l1">기준 라인.</param>
		/// <param name="l2">측정 대상 라인.</param>
		/// <returns>라디안 각도.</returns>
		static double rad_angle(const Line<T>& l1, const Line<T>& l2)
		{
			double angle1 = l1.P2.angleRad(l1.P1);
			double angle2 = l2.P2.angleRad(l2.P1);
			return /*fabs*/(angle2 - angle1);
		}

		//	이 방법도 정상 동작함.(그러나, 계산 정밀도 떨어짐)
		static double rad_angle2(const Line<T>& l1, const Line<T>& l2) 
		{
			// calculate the angle between the line from p1 to p2
			// and the line from p3 to p4
			// uses the theorem : 
			//	given directional vectors v = ai + bj and w = ci + di
			//	then cos(angle) = (ac + bd) / ( |v| * |w| )

			double a = (float)l1.P1.x - l1.P2.x;
			double b = (float)l1.P1.y - l1.P2.y;
			double c = (float)l2.P1.x - l2.P2.x;
			double d = (float)l2.P1.y - l2.P2.y;

			double cos_angle, angle, degree;
			double mag_v1 = sqrt(a*a + b*b);
			double mag_v2 = sqrt(c*c + d*d);

			cos_angle = (a*c + b*d) / (mag_v1 * mag_v2);
			angle = acos(cos_angle);
			degree = TheMath::rad2deg(angle);

			return angle;
		}

		T distance()		{	return (T)P1.distance(P2);	}

		//점과 라인이 직각을 이루는 지점간 거리
		//http://www.intmath.com/plane-analytic-geometry/perpendicular-distance-point-line.php
		T distance(const Point<T>& p) 
		{
			return (T)fabs((P2.y - P1.y) * p.x - (P2.x - P1.x) * p.y + P2.x * P1.y - P2.y * P1.x) / distance();
		}

		Point<T> mid()		{	return Point<T> (static_cast<T>((P1.x + P2.x) / 2.0), static_cast<T>((P1.y + P2.y) / 2.0));	}

		Line<T> transform(double tx, double ty) 
		{
			Point<T> m = mid();

			double dx = tx - m.x;
			double dy = ty - m.y;

			Line<T> newln;
			newln.P1.x = P1.x + dx;
			newln.P1.y = P1.y + dy;
			newln.P2.x = P2.x + dx;
			newln.P2.y = P2.y + dy;

			return newln;
		}

		double	slope() 
		{	
			return (P2.y - P1.y) / (double)(P2.x - P1.x);	
		}

		bool isHline() 	{	return (abs(abs(P2.x) - abs(P1.x)) > abs(abs(P2.y) - abs(P1.y))) ? true : false;	}
		bool isVline() 	{	return (!isHline());	}

		bool isParallel(Line<T>& l1, Line<T>& l2) 		{	return (l1.slope() == l2.slope());				}
		bool isperpendicular(Line<T>& l1, Line<T>& l2)	{	return (l1.neg_reciprocal() == l2.slope());		}

		bool ishorizontal()	{	return (P1.y == P2.y);	}
		bool isvertical() 	{	return (P1.x == P2.x);	}

		//	perpendicular 90 deg.(line에 대해 90인 라인의 기울기)
		double neg_reciprocal() 
		{
			double s = slope();
			if (!_finite(s))
				return -1;

			return (s == 0.) ? -1.0 : (-1.0 / s);
		}

		//	line function
		void linef(double &s, /* slope */ double &inter /* intercept */) 
		{
			Point<T> m = mid();
			s = slope();
			inter = m.y - (s * m.x);
		}

		//선분 함수의 x에 대응하는 y 값
		Point<T> get_y(const T& x)
		{
			double s, inter;
			linef(s, inter);

			return Point<T>(x, (T)(s * x  + inter));
		}

		//선분 함수의 y에 대응하는 x 값 
		Point<T> get_x(const T& y)
		{
			double s = 0, inter = 0;
			linef(s, inter);

			return Point<T>((T)((y - inter) / s), y);
		}

		//x 좌표가 frx ~ tox까지 연장된 라인
		Line<T>& extend_x(const T& frx, const T& tox)
		{
			double s = 0, inter = 0;
			linef(s, inter);

			P1.x = frx;		P1.y = (T)(s * frx + inter);
			P2.x = tox;		P2.y = (T)(s * tox + inter);
			return *this;
		}

		//y 좌표가 fry ~ toy까지 연장된 라인
		Line<T>& extend_y(const T& fry, const T& toy)
		{
			double s = 0, inter = 0;
			linef(s, inter);

			P1.x = (T)((fry - inter) / s);	P1.y = fry;
			P2.x = (T)((toy - inter) / s);	P2.y = toy;

			return *this;
		}

		Line<T>& reverse() 	{ T tmp = P1.y; P1.y = P2.y; P2.y = tmp; return *this; }

		double intercept() 	{	Point<T> m = mid();		return (m.y - (slope() * m.x));	}

		//	line funciton for perpendicular bisector
		void perpendicular_linef(double &s, /* slope */ double &inter /* intercept */) 
		{
			Point<T> m = mid();
			s = neg_reciprocal();
			inter = m.y - (s * m.x);
		}

		//https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
		Line<T> perpendicular_linef(const Point<T>& p)
		{
			double m, k;
			linef(m, k);

			T x = (p.x + m * p.y - m * k) / (m * m + 1);
			T y = m * ((p.x + m * p.y - m * k) / (m * m + 1)) + k;

			return Line<T>(p, Point<T>(x, y));
		}

		/// <summary>
		/// 직교 라인을 구함.
		/// </summary>
		/// <returns> 직교 라인.</returns>
		Line<T> perpendicular_line()					{	return rotate(mid(), 90.);	}

		/// <summary>
		/// 현 라인과 p점에서 직교 라인 구함.
		/// </summary>
		/// <param name="p"> 교차 라인의 대상 좌표점.</param>
		/// <returns> 직교 라인.</returns>
		Line<T> perpendicular_line(const Point<T>& p)	{	return rotate(p, 90.);		}

		/// <summary>
		/// 두 라인의 교차 여부.
		/// </summary>
		/// <param name="l">The l.</param>
		/// <param name="isp">The isp.</param>
		/// <returns> 교차하는 경우, true 아니면 false .</returns>
		bool intersection(Line<T>& l, Point<T>& isp)
		{
			// Store the values for fast access and easy
			// equations-to-code conversion
			double x1 = P1.x, x2 = P2.x, x3 = l.P1.x, x4 = l.P2.x;
			double y1 = P1.y, y2 = P2.y, y3 = l.P1.y, y4 = l.P2.y;

			double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
			// If d is zero, there is no intersection
			if (d == 0) return false;

			// Get the x and y
			double pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
			double x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
			double y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

			// Return the point of intersection
			isp.x = (T)x;
			isp.y = (T)y;
			return true;
		}

		/// <summary>
		/// 두 라인의 교차점 계산.
		/// </summary>
		/// <param name="l1">The l1.</param>
		/// <param name="l2">The l2.</param>
		/// <param name="isp"> 교차 점.</param>
		/// <returns> 교차하는 경우, true, 아니며 false .</returns>
		static bool intersection(Line<T>& l1, Line<T>& l2, Point<T>& isp)
		{
			return l1.intersection(l2, isp);
		}

		/// <summary>
		/// 두 라인의 중심간 거리.
		/// </summary>
		/// <param name="l1">The l1.</param>
		/// <param name="l2">The l2.</param>
		/// <param name="dx"> 두 라인 중심의 delta x.</param>
		/// <param name="dy"> 두 라인 중심의 delta y.</param>
		static void mid_delta(Line<T>& l1, Line<T>& l2, double& dx, double& dy)
		{
			Point<T> l1m = l1.mid();
			Point<T> l2m = l2.mid();

			dx = l1m.x - l2m.x;
			dy = l1m.y - l2m.y;
		}

		/// <summary>
		/// 회전 중심 center를 기준으로 deg 각도 회전된 라인 구함.
		/// </summary>
		/// <param name="center">The center.</param>
		/// <param name="deg">The deg.</param>
		/// <returns>The.Line&lt;T&gt;.</returns>
		Line<T> rotate(Point<T> center, double deg)
		{
			return Line<T>(P1.rotate(P1, center, deg), P2.rotate(P2, center, deg));
		}

		//////////////////////////////////////////////////////////////////////////
		//Functions for clipping a 2D line to the screen, which is the rectangle (0,0)-(w,h)
		//This is the Cohen-Sutherland Clipping Algorithm
		//Each of 9 regions gets an outcode, based on if it's at the top, bottom, left or right of the screen
		// 1001 1000 1010  9 8 10
		// 0001 0000 0010  1 0 2
		// 0101 0100 0110  5 4 6
		//int findregion returns which of the 9 regions a point is in, void clipline does the actual clipping
		int findRegion(const Point<T> LT, const Point<T> RB, Point<T> p)
		{
			int code = 0;
			if (p.y >= RB.y)
				code |= 1; //bottom
			else if (p.y < LT.y)
				code |= 2; //top
			if (p.x >= RB.x)
				code |= 4; //right
			else if (p.x < LT.x)
				code |= 8; //left
			return(code);
		}

		bool clipLine(const Point<T>& LT, const Point<T>& RB, Line<T>& cl)
		{
			int codeout = 0;
			bool accept = 0, done = 0;
			int code1 = findRegion(LT, RB, P1); //the region out codes for the endpoints
			int code2 = findRegion(LT, RB, P2);
			Point<T>	p1 = P1, p2 = P2;

			do  //In theory, this can never end up in an infinite loop, it'll always come in one of the trivial cases eventually
			{
				if (!(code1 | code2)) accept = done = 1;  //accept because both endpoints are in screen or on the border, trivial accept
				else if (code1 & code2) done = 1; //the line isn't visible on screen, trivial reject
				else  //if no trivial reject or accept, continue the loop
				{
					double x, y;
					codeout = code1 ? code1 : code2;
					if (codeout & 1) //bottom
					{
						//	x = p1.x + (p2.x - p1.x) * (h - p1.y) / (p2.y - p1.y);
						//	y = h - 1;
						x = p1.x + (p2.x - p1.x) * (RB.y - p1.y) / (p2.y - p1.y);
						y = RB.y - 1;
					}
					else if (codeout & 2) //top
					{
						x = p1.x + (p2.x - p1.x) * -p1.y / (p2.y - p1.y);
						//	y = 0;
						y = LT.y;
					}
					else if (codeout & 4) //right
					{
						//	y = p1.y + (p2.y - p1.y) * (w - p1.x) / (p2.x - p1.x);
						//	x = w - 1;
						y = p1.y + (p2.y - p1.y) * (RB.x - p1.x) / (p2.x - p1.x);
						x = RB.x - 1;
					}
					else //left
					{
						y = p1.y + (p2.y - p1.y) * -p1.x / (p2.x - p1.x);
						//	x = 0;
						x = LT.x;
					}
					if (codeout == code1) //first endpoint was clipped
					{
						p1.x = x; p1.y = y;
						code1 = findRegion(LT, RB, p1);
					}
					else //second endpoint was clipped
					{
						p2.x = x; p2.y = y;
						code2 = findRegion(LT, RB, p2);
					}
				}
			} while (done == 0);

			if (accept)
			{
				cl.P1 = p1;
				cl.P2 = p2;
				return true;
			}
			else
			{
				cl.P1 = cl.P2 = Point<T>(0, 0);
				return false;
			}
		}

		friend ofstream& operator << (ofstream& out, const Line<T>& L)
		{
			out << L.P1 << " ";		
			out << L.P2;	
			return out;
		}

		friend ifstream& operator >> (ifstream& in, Line<T>& L)
		{
			in >> L.P1 >> L.P2;		
			return in;	
		}

		friend stringstream& operator << (stringstream& out, const Line<T>& L)
		{
			out << L.P1 << " ";
			out << L.P2;
			return out;
		}

		friend stringstream& operator >> (stringstream& in, Line<T>& L)
		{
			in >> L.P1 >> L.P2;
			return in;
		}

		virtual T mid_x()		{ return mid().x; }
		virtual	T mid_y()		{ return mid().y; }
		virtual T area()		{ return (T)distance(); }
		virtual T perimeter()	{ return (T)distance(); }
		virtual void draw(HDC _hdc)
		{
			::MoveToEx(_hdc, (int)P1.x, (int)P1.y, NULL);
			::LineTo(_hdc, (int)P2.x, (int)P2.y);
		}

		virtual ResizerType drawHit(HDC _hdc, int _x, int _y, int hit_rng = 5)
		{
			ResizerType resizer = SIZENONE;

			draw(_hdc);

			if (P1.distance(Point<T>((T)_x, (T)_y)) <= hit_rng)			{ drawTrackBox(_hdc, (int)P1.x, (int)P1.y);		resizer = ResizerHit(SIZEALL, HitLineP1);	}
			else if (P2.distance(Point<T>((T)_x, (T)_y)) <= hit_rng)	{ drawTrackBox(_hdc, (int)P2.x, (int)P2.y);		resizer = ResizerHit(SIZEALL, HitLineP2);	}
			else if (mid().distance(Point<T>((T)_x, (T)_y)) <= hit_rng)	{ drawTrackBox(_hdc, (int)mid().x, (int)mid().y); resizer = ResizerHit(SIZEALL, HitLineCC);	}

			return resizer;
		}

		virtual	void resize(HitId hid, const T& delta_x, const T& delta_y)
		{
			if (hid == HitLineCC) {
				P1 += Point<T>(delta_x, delta_y);	
				P2 += Point<T>(delta_x, delta_y);
			}
			else if (hid == HitLineP1)	P1 += Point<T>(delta_x, delta_y);
			else if (hid == HitLineP2)	P2 += Point<T>(delta_x, delta_y);
		}

		virtual void drawResizable(HDC _hdc)
		{
			draw(_hdc);
			drawTrackBox(_hdc, (int)P1.x, (int)P1.y);
			drawTrackBox(_hdc, (int)P2.x, (int)P2.y);
			drawTrackBox(_hdc, (int)mid().x, (int)mid().y);
		}
	};

	template<typename T>
	class Circle : public /*virtual*/ Shape<T>
	{
	public:
		Circle()
			: radius(0)
		{}

		Circle(Point<T>& c, T r)
			: radius(r)
		{	center = c;	}

		Circle(const Point<T>& p0, const Point<T>& p1, const Point<T>& p2)
		{
			Line<T> a(p0, p1), b(p1, p2);
			Line<T> pa = a.perpendicular_line();
			Line<T> pb = a.perpendicular_line();

			Line<T>::intersection(pa, pb, center);
			radius = Point<T>::distance(center, p0);
		}

		~Circle(){}

		Circle<T>& operator= (const Circle<T>& c) {
			if (this == &c)
				return *this;
			center = c.center;
			return *this;
		}

		Circle<T> operator+(const Point<T>& p) { return center + p; }
		Circle<T> operator-(const Point<T>& p) { return center - p;	}
		Circle<T> operator*(const Point<T>& p) { return center * p; }
		Circle<T> operator/(const Point<T>& p) { return center / p; }
		Circle<T>& operator+=(const Point<T>& p) { center += p;	 return *this; }
		Circle<T>& operator-=(const Point<T>& p) { center -= p;  return *this; }
		Circle<T>& operator*=(const Point<T>& p) { center *= p;  return *this; }
		Circle<T>& operator/=(const Point<T>& p) { center /= p;  return *this; }

		//////////////////////////////////////////////////////////////////////////
		//	2017.2.08 type conversion between defferent T type
		template <typename U>
		Circle(const Circle<U>& c)
		{
			center = (Point<T>)c.center;
			radius = (T)c.radius;
		}

		template <typename U>
		operator Circle<U>()
		{
			return Circle<U>(center, radius);
		}
		//////////////////////////////////////////////////////////////////////////


		friend bool operator== (const Circle<T>& l1, const Circle<T>& c)
		{
			return (center == c.center && radius == c.radius);
		}

		friend bool operator!= (const Circle<T>& c, const Circle<T>& c2)
		{
			return (center != c.center || radius != c.radius);
		}

		friend ofstream& operator << (ofstream& out, const Circle<T>& c)
		{
			out << c.center << " " << c.radius;
			return out;
		}

		friend ifstream& operator >> (ifstream& in, Circle<T>& c)
		{
			in >> c.center >> c.radius;
			return in;
		}

		friend stringstream& operator << (stringstream& out, const Circle<T>& c)
		{
			out << c.center << " " << c.radius;
			return out;
		}

		friend stringstream& operator >> (stringstream& in, Circle<T>& c)
		{
			in >> c.center >> c.radius;
			return in;
		}

		virtual T mid_x()		{ return center.x;   }
		virtual T mid_y()		{ return center.y;	 }	
		T diameter()			{ return 2 * radius; }
		virtual T area()		{ return (T)(M_PI * radius * radius); }
		virtual	T perimeter()	{ return (T)(2 * M_PI * radius); }

		virtual void draw(HDC _hdc)
		{
			int l = (int)(center.x - radius);
			int t = (int)(center.y - radius);
			int r = (int)(center.x + radius);
			int b = (int)(center.y + radius);

			::Arc(_hdc, l, t, r, b, 0, 0, 0, 0);
		}

		virtual ResizerType drawHit(HDC _hdc, int _x, int _y, int hit_rng = 5)
		{
			ResizerType resizer = SIZENONE;

			draw(_hdc);

			Point<T> p((T)_x, (T)_y);
			Point<T> hit_pt[9] = 
			{
				center,									center.distanceByDeg(0, radius),
				center.distanceByDeg(45, radius),		center.distanceByDeg(90, radius),		
				center.distanceByDeg(135, radius),		center.distanceByDeg(180, radius),		
				center.distanceByDeg(225, radius),		center.distanceByDeg(270, radius),
				center.distanceByDeg(315, radius)
			};

			for (int i = 0; i < 9; ++i) 
			{
				if (hit_pt[i].distance(p) <= hit_rng) 
				{ 
					drawTrackBox(_hdc, (int)hit_pt[i].x, (int)hit_pt[i].y);
					switch (i)
					{
					default:
					case 0: resizer = ResizerHit(SIZEALL	, HitCircleCC);		break;
					case 1:	resizer = ResizerHit(SIZENS		, HitCircle00);		break;
					case 2: resizer = ResizerHit(SIZENESW	, HitCircle45);		break;
					case 3:	resizer = ResizerHit(SIZEWE		, HitCircle90);		break;
					case 4: resizer = ResizerHit(SIZENWSE	, HitCircle135);	break;
					case 5: resizer = ResizerHit(SIZENS		, HitCircle180);	break;
					case 6: resizer = ResizerHit(SIZENESW	, HitCircle225);	break;
					case 7: resizer = ResizerHit(SIZEWE		, HitCircle270);	break;
					case 8: resizer = ResizerHit(SIZENWSE	, HitCirCle315);	break;
					}
					break; 
				}
			}

			return resizer;
		}

		virtual	void resize(HitId hid, const T& delta_x, const T& delta_y)
		{

		}

		virtual void drawResizable(HDC _hdc)
		{
			draw(_hdc);
			drawTrackBox(_hdc, (int)center.x, (int)center.y);
			drawTrackBox(_hdc, (int)center.distanceByDeg(0, radius).x,	(int)center.distanceByDeg(0, radius).y);
			drawTrackBox(_hdc, (int)center.distanceByDeg(45, radius).x, (int)center.distanceByDeg(45, radius).y);
			drawTrackBox(_hdc, (int)center.distanceByDeg(90, radius).x,	(int)center.distanceByDeg(90, radius).y);
			drawTrackBox(_hdc, (int)center.distanceByDeg(135, radius).x, (int)center.distanceByDeg(135, radius).y);
			drawTrackBox(_hdc, (int)center.distanceByDeg(180, radius).x, (int)center.distanceByDeg(180, radius).y);
			drawTrackBox(_hdc, (int)center.distanceByDeg(225, radius).x, (int)center.distanceByDeg(225, radius).y);
			drawTrackBox(_hdc, (int)center.distanceByDeg(270, radius).x, (int)center.distanceByDeg(270, radius).y);
		}

	public:
		Point<T>	center;
		T			radius;
	};

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	template<typename T>
	class Rect : public /*virtual*/ Shape<T>
	{
	public:
		Rect()
			: R(false)
		{}

		Rect(const Rect<T>& r)
		{
			*this = r;
		}

		bool empty() const
		{
			return (lt.empty() && lb.empty() && rt.empty() && rb.empty());
		}

		//////////////////////////////////////////////////////////////////////////
		//	2017.2.08 type conversion between defferent T type
		template <typename U>
		Rect(const Rect<U>& r)
		{
			lt = (Point<T>)r.lt;
			lb = (Point<T>)r.lb;
			rt = (Point<T>)r.rt;
			rb = (Point<T>)r.rb;
		}

		template <typename U>
		operator Rect<U>()
		{
			return Rect<U>(lt, lb, rt, rb);
		}
		//////////////////////////////////////////////////////////////////////////

		Rect(const T _l, const T _t, const T _r, const T _b)
			: R(false)
		{
			lt = iPoint(_l, _t);
			lb = iPoint(_l, _b);
			rt = iPoint(_r, _t);
			rb = iPoint(_r, _b);
		}


		Rect(const Point<T>& _lt, T w, T h)
			: R(false)
		{
			lt = _lt;
			rb = _lt + Point<T>(w, h);
			lb = _lt + Point<T>(0, h);
			rt = _lt + Point<T>(w, 0);
		}

		Rect(const Point<T>& _rt, T w, T h, bool)
			: R(false)
		{
			lt = _rt - Point<T>(w, 0);
			rb = _rt - Point<T>(w, h);
			lb = _rt - Point<T>(0, h);
			rt = _rt;
		}

		//	2017.02.08 _rt와 _rb의 순서 변경
		Rect(const Point<T>& _lt, const Point<T>& _lb, const Point<T>& _rt, const Point<T>& _rb)
			: R(lt.y == rt.y)
		{
			lt = _lt;
			lb = _lb;
			rb = _rb;
			rt = _rt;
		}

		Rect(const Point<T>& _lt, const Point<T>& _rb)
			: R(false)
		{
			lt = _lt;
			rb = _rb;
			lb = Point<T>(_lt.x, _rb.y);
			rt = Point<T>(_rb.x, _lt.y);
		}

		Rect(const Point<T>& _lt, const Point<T>& _rb, const Point<T>& c, double rad)
			: R(true)
		{
			lt = _lt.rotate(c, rad);
			rb = _rb.rotate(c, rad);
			lb = Point<T>(_lt.x, _rb.y).rotate(c, rad);
			rt = Point<T>(_rb.x, _lt.y).rotate(c, rad);
		}

		~Rect()
		{
		}

		Point<T>	get_lt() { return lt; }
		Point<T>	get_rb() { return rb; }
		Point<T>	get_lb() { return lb; }
		Point<T>	get_rt() { return rt; }

		T width()
		{
			return lt.distance(rt);
		}

		T height()
		{
			return lt.distance(lb);
		}

		/// <summary>
		/// 회전 중심 c를 기준으로 rad각도 회전 시킴.
		/// </summary>
		/// <param name="c">회전 중심.</param>
		/// <param name="rad">라이안 각도.</param>
		/// <returns>회전된 Rect.</returns>
		Rect<T> rotate(const Point<T>& c, double rad)
		{
			Rect<T> nr;
			nr.lt = lt.rotate(c, rad);
			nr.lb = lb.rotate(c, rad);
			nr.rb = rb.rotate(c, rad);
			nr.rt = rt.rotate(c, rad);
			R = true;
			return nr;
		}

		Rect<T>& operator=(const Rect<T>& r)
		{
			lt = r.lt;
			rb = r.rb;
			lb = r.lb;
			rt = r.rt;
			R = r.R;
			return *this;
		}

		Point<T> center()
		{
			Point<T> tc = lt.mid(rt);
			Point<T> lc = lt.mid(lb);
			Point<T> rc = rt.mid(rb);
			Point<T> bc = lb.mid(rb);

			Point<T> isp;
			//	수평선, 수직선 교차점
			Line<T>::intersection(Line<T>(lc, rc), Line<T>(tc, bc), isp);

			return isp;
		}

		/// <summary>
		/// 현 Rect를 r과 합성.
		/// </summary>
		/// <param name="r"> 합성할 Rect.</param>
		/// <returns> 합성된 Rect </returns>
		Rect<T> operator+(const Rect<T>& _r)
		{
			T l = std::min<T>(lt.x, _r.lt.x);
			T t = std::min<T>(lt.y, _r.lt.y);
			T r = std::max<T>(rb.x, _r.rb.x);
			T b = std::max<T>(rb.y, _r.rb.y);
			
			return Rect<T>(Point<T>(l, t), Point<T>(r, b));
		}

		Rect<T> operator+(const Point<T>& p) {	return Rect<T> (lt + p, lb + p, rt + p, rb + p); }
		Rect<T> operator-(const Point<T>& p) {	return Rect<T> (lt - p, lb - p, rt - p, rb - p); }
		Rect<T> operator*(const Point<T>& p) {	return Rect<T> (lt * p, lb * p, rt * p, rb * p); }
		Rect<T> operator/(const Point<T>& p) {  return Rect<T> (lt / p, lb / p, rt / p, rb / p); }

		Rect<T>& operator+=(const Point<T>& p) { lt += p; lb += p; rt += p; rb += p; return *this; }
		Rect<T>& operator-=(const Point<T>& p) { lt -= p; lb -= p; rt -= p; rb -= p; return *this; }
		Rect<T>& operator*=(const Point<T>& p) { lt *= p; lb *= p; rt *= p; rb *= p; return *this; }
		Rect<T>& operator/=(const Point<T>& p) { lt /= p; lb /= p; rt /= p; rb /= p; return *this; }

		Rect<T> operator-(const Rect<T>& r)	{	return getIntersection(r);	}

		bool operator==(const Rect<T>& r) {	return (lt == r.lt && lb == r.lb && rb == r.rb && rt == r.rt) ? true : false;	}
		friend bool operator==(const Rect<T>& r, const Rect<T>& s)
		{
			return (r.lt == s.lt && r.lb == s.lb && r.rb == s.rb && r.rt == s.rt) ? true : false;
		}


		Rect<T>& shift(const Point<T>& delta)
		{
			lt.x += delta.x;	rt.x += delta.x;	lb.x += delta.x;	rb.x += delta.x;
			lt.y += delta.y;	rt.y += delta.y;	lb.y += delta.y;	rb.y += delta.y;

			return *this;
		}

		Rect<T>& shift_x(const T& v)	{	lt.x += v;	rt.x += v;	lb.x += v;	rb.x += v;	return *this;	}
		Rect<T>& shift_y(const T& v)	{	lt.y += v;	rt.y += v;	lb.y += v;	rb.y += v;	return *this;	}

		Rect<T>& extend_x_to(const T& v, const T& lsb/*하한치*/, const T& msb /*상한치*/)
		{
			T _min_x = std::min<T>(std::min<T>(lt.x, lb.x), std::min<T>(rt.x, rb.x));
			T _max_x = std::max<T>(std::max<T>(lt.x, lb.x), std::max<T>(rt.x, rb.x));

			lt.x = lb.x = std::max<T>(lsb, _min_x - v);
			rt.x = rb.x = std::min<T>(msb, _max_x + v);

			T pad_w = ((rt.x - lt.x) * 8 + 31) / 32 * 4;	//	비트맵 패딩 계산
			
			if (lt.x + pad_w < msb)	
				rt.x = rb.x = lt.x + pad_w;	//	상한치 이하면 좌측 기준으로	ROI 결정
			else
				lt.x = lb.x = rt.x - pad_w;	//	상한치 이상이면 우측 기준으로 ROI 결정

			return *this;
		}

		Rect<T> get_slice_x(const T& numerator, const T& nTh)
		{
			assert(nTh < numerator && numerator != 0);

			const T wt = lt.distance(rt) / numerator;
			const T wb = lb.distance(rb) / numerator;

			T lt_x = lt.x + wt * nTh,	rt_x = lt_x + wt;
			T lb_x = lb.x + wb * nTh,	rb_x = lb_x + wb;

			Point<T> nlt = get_top().get_y(lt_x);
			Point<T> nrt = get_top().get_y(rt_x);
			Point<T> nlb = get_bottom().get_y(lb_x);
			Point<T> nrb = get_bottom().get_y(rb_x);

			return Rect<T>(nlt, nlb, nrt, nrb);
		}

		Rect<T> get_slice_y(const T& numerator, const T& nTh)
		{
			assert(nTh < numerator);

			const T hl = lt.distance(lb) / numerator;
			const T hr = rt.distance(rb) / numerator;

			T lt_y = lt.y + hl * nTh, lb_y = lt_y + hl;
			T rt_y = rt.y + hr * nTh, rb_y = rt_y + hr;

			Point<T> nlt = get_left().get_x(lt_y);
			Point<T> nlb = get_left().get_x(lb.y);
			Point<T> nrt = get_right().get_x(rt_y);
			Point<T> nrb = get_right().get_x(rb_y);

			return Rect<T>(nlt, nlb, nrt, nrb);
		}

		//선분각도 배제형
		Rect<T>& grow_x_to(const T& v, const T& lsb/*하한치*/, const T& msb /*상한치*/)
		{
			T _min_x = std::min<T>(std::min<T>(lt.x, lb.x), std::min<T>(rt.x, rb.x));
			T _max_x = std::max<T>(std::max<T>(lt.x, lb.x), std::max<T>(rt.x, rb.x));

			if (v < 0) lt.x = lb.x = std::max<T>(lsb, _min_x + v);
			else       rt.x = rb.x = std::min<T>(msb, _max_x + v);

			T pad_w = ((rt.x - lt.x) * 8 + 31) / 32 * 4;	//	비트맵 패딩 계산

			if (lt.x + pad_w < msb)
				rt.x = rb.x = lt.x + pad_w;	//	상한치 이하면 좌측 기준으로	ROI 결정
			else
				lt.x = lb.x = rt.x - pad_w;	//	상한치 이상이면 우측 기준으로 ROI 결정

			return *this;
		}

		//선분각도 유지형
		Rect<T>& slope_grow_x_to(const T& v, const T& lsb/*하한치*/, const T& msb /*상한치*/)
		{
			if (v < 0) {
				lt.y = rt.y;	lb.y = rb.y;
				lt.x = std::max<T>(lsb, rt.x + v);	lb.x = std::max<T>(lsb, rb.x + v);
			}
			else       {
				rt.y = lt.y;	rb.y = lb.y;
				rt.x = std::min<T>(msb, lt.x + v);	rb.x = std::min<T>(msb, lb.x + v);
			}

			T pad_wt = ((rt.x - lt.x) * 8 + 31) / 32 * 4;	//	비트맵 패딩 계산

			if (lt.x + pad_wt < msb)	rt.x = lt.x + pad_wt;	//	상한치 이하면 좌측 기준으로	ROI 결정
			else						lt.x = rt.x - pad_wt;	//	상한치 이상이면 우측 기준으로 ROI 결정

			T pad_wb = ((rb.x - lb.x) * 8 + 31) / 32 * 4;	//	비트맵 패딩 계산

			if (lb.x + pad_wb < msb)	rb.x = lb.x + pad_wb;	//	상한치 이하면 좌측 기준으로	ROI 결정
			else						lb.x = rb.x - pad_wb;	//	상한치 이상이면 우측 기준으로 ROI 결정

			return *this;
		}



		Rect<T>& extend_y_to(const T& v, const T& lsb/*하한치*/, const T& msb/*상한치*/)
		{
			T _min_y = std::min<T>(std::min<T>(lt.y, lb.y), std::min<T>(rt.y, rb.y));
			T _max_y = std::max<T>(std::max<T>(lt.y, lb.y), std::max<T>(rt.y, rb.y));

			lt.y = rt.y = std::max<T>(lsb, _min_y - v);
			lb.y = rb.y = std::min<T>(msb, _max_y + v);

			return *this;
		}

		//선분 각도 배제형 성장
		Rect<T>& grow_y_to(const T& v, const T& lsb/*하한치*/, const T& msb/*상한치*/)
		{
			T _min_y = std::min<T>(std::min<T>(lt.y, lb.y), std::min<T>(rt.y, rb.y));
			T _max_y = std::max<T>(std::max<T>(lt.y, lb.y), std::max<T>(rt.y, rb.y));

			if (v < 0)	lt.y = rt.y = std::max<T>(lsb, _min_y + v);
			else		lb.y = rb.y = std::min<T>(msb, _max_y + v);

			return *this;
		}

		//선분 각도 유지형 성장(-v top기준 y -방향 성장, +v bottom기준 y + 방향 성장)
		Rect<T>& slope_grow_y_to(const T& v, const T& lsb/*하한치*/, const T& msb/*상한치*/)
		{
			//하단 선분 기준
			if (v < 0)	{
				lt.x = lb.x;	rt.x = rb.x;
				lt.y = std::max<T>(lsb, lb.y + v);	rt.y = std::max<T>(lsb, rb.y + v);
			}
			//상단 선분 기준
			else		{
				lb.x = lt.x;	rb.x = rt.x;
				lb.y = std::min<T>(msb, lt.y + v);	rb.y = std::min<T>(msb, rt.y + v);	
			}

			return *this;
		}

		//	두 사각형의 꼭지점과 중심중 가장 근접한 거리 구함
		//http://stackoverflow.com/questions/4978323/how-to-calculate-distance-between-two-rectangles-context-a-game-in-lua
		//Not in Lua, a Python code based on M Katz's suggestion (considering the rectangles do not intersect):
		T getNearDistance(const Rect<T>& r)
		{
			if (IsIntersect(r))	return 0;

			bool left	= r.rb.x < lt.x;
			bool right	= rb.x < r.lt.x;
			bool bottom = r.rb.y < lt.y;
			bool top	= rb.y < r.lt.y;

			if		(top	&& left	)	return Point<T>::distance(Point<T>(lt.x, rb.y), Point<T>(r.rb.x, r.lt.y));
			else if (left	&& bottom)	return Point<T>::distance(Point<T>(lt.x, lt.y), Point<T>(r.rb.x, r.rb.y));
			else if (bottom && right)	return Point<T>::distance(Point<T>(rb.x, lt.y), Point<T>(r.lt.x, r.rb.y));
			else if (right	&& top)		return Point<T>::distance(Point<T>(rb.x, rb.y), Point<T>(r.lt.x, r.lt.y));
			else if (left)				return lt.x - r.rb.x;
			else if (right)				return r.lt.x - rb.x;
			else if (bottom)			return lt.y - r.rb.y;
		//	else if (top)				
			return r.lt.y - rb.y;
		}

		/// <summary>
		///  두 사각형의 교집합을 구해 새로운 사각형 생성.
		/// </summary>
		/// <param name="rc"> .</param>
		/// <returns> 두 사각형의 교집합.</returns>
		Rect<T> getIntersection(const Rect<T>& rc)
		{
			T lxt = max(lt.x, rc.lt.x);
			T lxb = max(lb.x, rc.lb.x);

			T rxt = min(rt.x, rc.rt.x);
			T rxb = min(rb.x, rc.rb.x);

			T lyt = max(lt.y, rc.lt.y);
			T lyb = min(lb.y, rc.lb.y);

			T ryt = max(rt.y, rc.rt.y);
			T ryb = min(rb.y, rc.rb.y);

			return Rect<T>(Point<T>(lxt, lyt), Point<T>(lxb, lyb), Point<T>(rxt, ryt), Point<T>(rxb, ryb));
		}

		//	두 사각형으로 사각형에 포함된 사각형은 상대 좌표로 생성
		Rect<T> getRelative(const Rect<T>& r)
		{
			Rect<T> s = getIntersection(r);
			T lx = std::max<T>(s.lt.x, s.lb.x);
			T rx = std::min<T>(s.rt.x, s.rb.x);
			T ty = std::max<T>(s.lt.y, s.rt.y);
			T by = std::min<T>(s.lb.y, s.rb.y);

			Rect<T> u(Point<T>(lx, ty), Point<T>(rx, by));

			return u - lt;
		}

		/// <summary>
		/// Determines whether the specified r is intersect.
		/// </summary>
		/// <param name="r"> 사각형.</param>
		/// <returns> 교차시 true, 아니면 false.</returns>
		//https://silentmatt.com/rectangle-intersection/
		bool IsIntersect(const Rect<T>& r)
		{
			return (lt.x < r.rb.x && rb.x > r.lt.x && lt.y < r.rb.y && rb.y > r.lt.y);
		}

		/**
		* Check if two rectangles collide
		* x_1, y_1, width_1, and height_1 define the boundaries of the first rectangle
		* x_2, y_2, width_2, and height_2 define the boundaries of the second rectangle
		*/
		boolean rectangle_collision(float x_1, float y_1, float width_1, float height_1, float x_2, float y_2, float width_2, float height_2)
		{
			return !(x_1 > x_2 + width_2 || x_1 + width_1 < x_2 || y_1 > y_2 + height_2 || y_1 + height_1 < y_2);
		}

		/// <summary>
		/// 현 사각형 left/top을 기준으로 부터 Y 방향으로 y만큼 떨어진 좌표(각도 보상)를 구함.
		/// </summary>
		/// <param name="y">좌상점을 기준한 Y의 수직방향 거리.</param>
		/// <returns> Point Left.</returns>
		Point<T> LeftPoint(T y)
		{
			return lt.distanceByRad(Line<T>(lt, rt).radAngle(), y);
		}

		/// <summary>
		/// 현 사각형의 right/top을 기준으로 Y 방향으로 y만큼 떨어진 좌표(각도 보상)을 구함.
		/// </summary>
		/// <param name="y">우상점을 기준한 Y의 수직 방향 거리.</param>
		/// <returns> 우측 좌표 포인트.</returns>
		Point<T> RightPoint(T y)
		{
			return rt.distanceByRad(Line<T>(lt, rt).radAngle(), y);
		}

		/// <summary>
		/// 라디안 대각선 각도.
		/// </summary>
		/// <returns>T.</returns>
		T diagonalRad()
		{
			return Line<T>(lt, rb).radAngle();
		}

		/// <summary>
		/// 대각선 각도.
		/// </summary>
		/// <returns>T.</returns>
		T diagonalDeg()
		{
			return Line<T>(lt, rb).degAngle();
		}

		Line<T>	get_top()		{ return Line<T>(lt, rt); }
		Line<T>	get_left()		{ return Line<T>(lt, lb); }
		Line<T>	get_right()		{ return Line<T>(rt, rb); }
		Line<T> get_bottom()	{ return Line<T>(lb, rb); }
		Line<T>	get_diagonal()	{ return Line<T>(lt, rb); }

		/// <summary>
		/// 포인트가 Rect의 영역에 포함되는지.
		/// http://alienryderflex.com/polygon/
		/// </summary>
		/// <param name="p">The p.</param>
		/// <returns>bool.</returns>
		bool isPointInRect(const Point<T>& p)
		{
			//  Globals which should be set before calling this function:
			//
			//  int    polyCorners  =  how many corners the polygon has
			//  float  polyX[]      =  horizontal coordinates of corners
			//  float  polyY[]      =  vertical coordinates of corners
			//  float  x, y         =  point to be tested
			//
			//  (Globals are used in this example for purposes of speed.  Change as
			//  desired.)
			//
			//  The function will return YES if the point x,y is inside the polygon, or
			//  NO if it is not.  If the point is exactly on the edge of the polygon,
			//  then the function may return YES or NO.
			//
			//  Note that division by zero is avoided because the division is protected
			//  by the "if" clause which surrounds it.

			Point<T>	r[4] = { lt, lb, rb, rt };

			int   i, j = 3;
			bool  oddNodes = false;

			for (i = 0; i < 4; i++) 
			{
				if ((r[i].y < p.y && r[j].y >= p.y 
					|| r[j].y < p.y && r[i].y >= p.y)
					&& (r[i].x <= p.x || r[j].x <= p.x))
				{
					oddNodes ^= (r[i].x + (p.y - r[i].y) / (r[j].y - r[i].y)*(r[j].x - r[i].x) < p.x);
				}
				j = i;
			}

			return oddNodes;
		}

		bool isRectIn(const Rect<T>& r)
		{
			return (isPointInRect(r.lt) && isPointInRect(r.lb) && isPointInRect(r.rt) && isPointInRect(r.rb));
		}

		bool isRectIn2(const Rect<T>& r)
		{
			T minx = min<T>(min<T>(lt.x, lb.x), min<T>(rt.x,rb.x));
			T maxx = max<T>(max<T>(lt.x, lb.x), max<T>(rt.x,rb.x));
			T miny = min<T>(min<T>(lt.y, lb.y), min<T>(rt.y,rb.y));
			T maxy = max<T>(max<T>(lt.y, lb.y), max<T>(rt.y,rb.y));
			return ((minx <= r.lt.x && r.lt.x <= maxx) &&
					(minx <= r.lb.x && r.lb.x <= maxx) &&
					(minx <= r.rt.x && r.rt.x <= maxx) &&
					(minx <= r.rb.x && r.rb.x <= maxx) &&
					(miny <= r.lt.y && r.lt.y <= maxy) &&
					(miny <= r.lb.y && r.lb.y <= maxy) &&
					(miny <= r.rt.y && r.rt.y <= maxy) &&
					(miny <= r.rb.y && r.rb.y <= maxy));
		}
		//각도고려
		bool isRectIn3(const Rect<T>& r)
		{
			return (
				lt.x <= r.lt.x && lt.y <= r.lt.y &&
				r.rt.x <= rt.x && rt.y <= r.rt.y &&
				lb.x <= r.lb.x && r.lb.y <= lb.y &&
				r.rb.x <= rb.x && r.rb.y <= rb.y	);
		}

		friend ofstream& operator << (ofstream& out, const Rect<T>& r)
		{
			out << r.lt << " ";
			out << r.rb << " ";
			out << r.lb << " ";
			out << r.rt;
			return out;
		}

		friend ifstream& operator >> (ifstream& in, Rect<T>& r)
		{
			in >> r.lt >> r.rb >> r.lb >> r.rt;
			return in;
		}

		friend stringstream& operator << (stringstream& out, const Rect<T>& r)
		{
			out << r.lt << " ";
			out << r.rb << " ";
			out << r.lb << " ";
			out << r.rt;
			return out;
		}

		friend stringstream& operator >> (stringstream& in, Rect<T>& r)
		{
			in >> r.lt >> r.rb >> r.lb >> r.rt;
			return in;
		}
		virtual T mid_x()	{ return center().x; }
		virtual	T mid_y()	{ return center().y; }
		virtual T perimeter()
		{
			return (T)(lt.distance(lb) + lb.distance(rb) + rb.distance(rt) + rt.distance(lt));
		}

		virtual T area()
		{
			double a1 = lt.distance(lb), b1 = lb.distance(rb), c = rb.distance(lt);
			double a2 = lt.distance(rt), b2 = rt.distance(rb)/*, c = rb.distance(lt)*/;
			//Heron's formula
			double t1 = sqrt((a1 + b1 + c)*(b1 - a1 + c)*(a1 - b1 + c)*(a1 + b1 - c)) / 4;	//	삼각형 1
			double t2 = sqrt((a2 + b2 + c)*(b2 - a2 + c)*(a2 - b2 + c)*(a2 + b2 - c)) / 4;	//	삼각형 2
			return (T)(t1 + t2);
		}

		virtual void draw(HDC _hdc)
		{
			::MoveToEx(_hdc, (int)lt.x, (int)lt.y, NULL);
			::LineTo(_hdc, (int)lb.x, (int)lb.y);
			::LineTo(_hdc, (int)rb.x, (int)rb.y);
			::LineTo(_hdc, (int)rt.x, (int)rt.y);
			::LineTo(_hdc, (int)lt.x, (int)lt.y);
		}

		virtual	void draw(HDC _hdc, float rate)
		{
			::MoveToEx(_hdc, (int)lt.x, (int)lt.y, NULL);
			::LineTo(_hdc, (int)lb.x , (int)lb.y );
			::LineTo(_hdc, (int)rb.x , (int)rb.y );
			::LineTo(_hdc, (int)rt.x , (int)rt.y );
			::LineTo(_hdc, (int)lt.x , (int)lt.y );
		}

		virtual ResizerType drawHit(HDC _hdc, int _x, int _y, int hit_rng = 5)
		{
			ResizerType resizer = SIZENONE;

			draw(_hdc);

 			Point<T> p((T)_x, (T)_y);

			Point<T> hit_pt[9] = { center(), lt.mid(rt), rt, rt.mid(rb), rb, lb.mid(rb), lb, lt.mid(lb), lt};

			for (int i = 0; i < 9; ++i) 
			{
				if (hit_pt[i].distance(p) <= hit_rng) 
				{	
					drawTrackBox(_hdc, (int)hit_pt[i].x, (int)hit_pt[i].y);

					switch (i)
					{
					default:
					case 0: resizer = ResizerHit(SIZEALL	, HitRectCC);	break;
					case 1:	resizer = ResizerHit(SIZENS		, HitRectTC);	break;
					case 2: resizer = ResizerHit(SIZENESW	, HitRectRT);	break;
					case 3:	resizer = ResizerHit(SIZEWE		, HitRectRC);	break;
					case 4: resizer = ResizerHit(SIZENWSE	, HitRectRB);	break;
					case 5: resizer = ResizerHit(SIZENS		, HitRectBC);	break;
					case 6: resizer = ResizerHit(SIZENESW	, HitRectLB);	break;
					case 7: resizer = ResizerHit(SIZEWE		, HitRectLC);	break;
					case 8: resizer = ResizerHit(SIZENWSE	, HitRectLT);	break;
					}
					break;
				}
			}

			return resizer;
		}

		virtual	void resize(HitId hid, const T& delta_x, const T& delta_y)
		{
			Point<T> delta(delta_x, delta_y);

			switch (hid)
			{
			case HitRectCC:	lt += delta;		lb += delta;		rt += delta;	rb += delta;	break;
			case HitRectTC:	lt.y += delta_y;	rt.y += delta_y;									break;
			case HitRectRT:	rt += delta;		lt.y += delta_y;	rb.x += delta_x;				break;
			case HitRectRC:	rt.x += delta_x;	rb.x += delta_x;									break;
			case HitRectRB:	rb += delta;		rt.x += delta_x;	lb.y += delta_y;				break;
			case HitRectBC:	lb.y += delta_y;	rb.y += delta_y;									break;
			case HitRectLB:	lb += delta;		lt.x += delta_x;	rb.y += delta_y;				break;
			case HitRectLC:	lt.x += delta_x;	lb.x += delta_x;									break;
			case HitRectLT:	lt += delta;		lb.x += delta_x;	rt.y += delta_y;				break;
			default:	break;
			}
		}

		virtual void drawResizable(HDC _hdc)
		{
			draw(_hdc);
			drawTrackBox(_hdc, (int)lt.x, (int)lt.y);	
			drawTrackBox(_hdc, (int)lb.x, (int)lb.y);	
			drawTrackBox(_hdc, (int)rb.x, (int)rb.y);	
			drawTrackBox(_hdc, (int)rt.x, (int)rt.y);
			drawTrackBox(_hdc, (int)lt.mid(lb).x, (int)lt.mid(lb).y);
			drawTrackBox(_hdc, (int)lb.mid(rb).x, (int)lb.mid(rb).y);
			drawTrackBox(_hdc, (int)rb.mid(rt).x, (int)rb.mid(rt).y);
			drawTrackBox(_hdc, (int)rt.mid(lt).x, (int)rt.mid(lt).y);
		}

// 	private:
// 
// 	private:
		bool		R;			//	rotated rectangle
		Point<T>	lt;			//	left top
		Point<T>	lb;			//	left bottom
		Point<T>	rt;			//	right top
		Point<T>	rb;			//	right bottom
	};

	//http://www.aip.de/groups/soe/local/numres/bookcpdf/c14-1.pdf
	//Given an array of data[1..n], this routine returns its mean ave, average deviation adev,
	//standard deviation sdev, variance var, skewness skew, and kurtosis curt.
	bool moment(const double data[], int n, double *ave, double *adev, double *sdev,
		double *var, double *skew, double *curt);

#pragma pop_macro("min")


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//	Point type 기반 변수
	typedef Point<int>		iPoint;
	typedef Point<long>		lPoint;
	typedef Point<float>	fPoint;
	typedef Point<double>	dPoint;

	//	size type 변수
	typedef Point<int>		iSize;
	typedef Point<long>		lSize;
	typedef Point<float>	fSize;
	typedef Point<double>	dSize;

	// Line type 변수
	typedef	Line<int>		iLine;
	typedef	Line<long>		lLine;
	typedef	Line<float>		fLine;
	typedef	Line<double>	dLine;

	// Circle type 변수
	typedef Circle<int>		iCircle;
	typedef Circle<long>	lCircle;
	typedef Circle<float>	fCircle;
	typedef Circle<double>	dCircle;

	// Rect type 변수
	typedef Rect<int>		iRect;
	typedef Rect<long>		lRect;
	typedef Rect<float>		fRect;
	typedef Rect<double>	dRect;

	//Point sort 함수
	template<typename T>
	bool sort_x(const Point<T>& a, const Point<T>& b) {	return (a.x < b.x);	}
	
	template<typename T>
	bool sort_y(const Point<T>& a, const Point<T>& b) { return (a.y < b.y); }

	template<typename T>
	void sortPoints(std::vector<Point<T>>& points, bool by_x)
	{
		if (by_x)	std::sort(points.begin(), points.end(), sort_x<T>);
		else		std::sort(points.begin(), points.end(), sort_y<T>);
	}

	static void PointUseCase()
	{
		dPoint p0;
		dPoint p1(1, 1);
		dPoint p2(p1);
		p2.add(1, 1);
		p2.sub(1, 1);
		p2.mul(2, 2);
		p2.mul(dPoint(2, 2));
		p2.div(dPoint(2, 2));
		dPoint p6 = p2;
		dPoint p3 = p2 + p1;
		dPoint p4 = p2 - p1;
		dPoint p5 = p4 * p2;
		dPoint p51 = p4 / p2;
		p5 += p2;
		p5 *= p1;
		p5 *= 1.5;
		p5 /= 2;
		bool r0 = p5 < p1;
		bool r1 = p5 <= p1;
		bool r2 = p5 > p1;
		bool r3 = p5 >= p1;
		double r4 = dPoint(0, 0).distance(dPoint(1, 1));
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		dPoint P0(0, 0);
		dPoint P1(0, 0);
		dPoint P2(0, 1);
		dPoint P3(0, -1);
		dPoint P4(1, 0);
		dPoint P5(-1, 0);

		dPoint P6(1, 1);
		dPoint P7(-1, 1);
		dPoint P8(-1, -1);
		dPoint P9(1, -1);

		/**/
		dPoint P10 = P6.rotate(P0, TheMath::deg2rad(45));
		dPoint P11 = P6.rotate(P0, TheMath::deg2rad(90));
		dPoint P12 = P6.rotate(P0, TheMath::deg2rad(135));
		dPoint P13 = P6.rotate(P0, TheMath::deg2rad(225));
		dPoint P14 = P6.rotate(P0, TheMath::deg2rad(270));
		dPoint P15 = P6.rotate(P0, TheMath::deg2rad(315));
		dPoint P16 = P6.rotate(P0, TheMath::deg2rad(360));

		/**/
		double a1 = P1.angleDeg(P0);	//	0도
		double a2 = P2.angleDeg(P0);	//	90도
		double a3 = P3.angleDeg(P0);	//	-90도
		double a4 = P4.angleDeg(P0);	//	0도
		double a5 = P5.angleDeg(P0);	//	180

		double a6 = P6.angleDeg(P0);	//	45도
		double a7 = P7.angleDeg(P0);	//	135도
		double a8 = P8.angleDeg(P0);	//	-135
		double a9 = P9.angleDeg(P0);	//	-45

		/**/
		double a1360 = TheMath::deg360(a1);	//	0도
		double a2360 = TheMath::deg360(a2);	//	90도
		double a3360 = TheMath::deg360(a3);	//	270도
		double a4360 = TheMath::deg360(a4);	//	0도
		double a5360 = TheMath::deg360(a5);	//	180

		double a6360 = TheMath::deg360(a6);	//	45도
		double a7360 = TheMath::deg360(a7);	//	135도
		double a8360 = TheMath::deg360(a8);	//	225도
		double a9360 = TheMath::deg360(a9);	//	315도

		/**/
		double a11 = TheMath::deg2rad(a1);
		double a22 = TheMath::deg2rad(a2);
		double a33 = TheMath::deg2rad(a3);
		double a44 = TheMath::deg2rad(a4);
		double a55 = TheMath::deg2rad(a5);

		double a66 = TheMath::deg2rad(a6);
		double a77 = TheMath::deg2rad(a7);
		double a88 = TheMath::deg2rad(a8);
		double a99 = TheMath::deg2rad(a9);
		
		/**/
		double a111 = TheMath::rad2deg(a11);
		double a222 = TheMath::rad2deg(a22);
		double a333 = TheMath::rad2deg(a33);
		double a444 = TheMath::rad2deg(a44);
		double a555 = TheMath::rad2deg(a55);

		double a666 = TheMath::rad2deg(a66);
		double a777 = TheMath::rad2deg(a77);
		double a888 = TheMath::rad2deg(a88);
		double a999 = TheMath::rad2deg(a99);

		/**/
		double a11360 = TheMath::rad360(a11);	//	0도
		double a22360 = TheMath::rad360(a22);	//	90도
		double a33360 = TheMath::rad360(a33);	//	270도
		double a44360 = TheMath::rad360(a44);	//	0도
		double a55360 = TheMath::rad360(a55);	//	180

		double a66360 = TheMath::rad360(a66);	//	45도
		double a77360 = TheMath::rad360(a77);	//	135도
		double a88360 = TheMath::rad360(a88);	//	225도
		double a99360 = TheMath::rad360(a99);	//	315도

		iLine ln(iPoint(0, 0), iPoint(100, 100));
		ln.drawHit(NULL, 0, 0, 5);	ln.perimeter(); ln.area();	ln.drawResizable(NULL);

		iCircle circle(iPoint(0, 0), 100);
		circle.drawHit(NULL, 0, 0, 5); circle.perimeter(); circle.area();	circle.drawResizable(NULL);

		iRect r(iPoint(0, 0), iPoint(-100, 100), iPoint(100, 100), iPoint(100, 0));
		r.drawHit(NULL, 0, 0, 5); r.perimeter(); r.area();	ln.drawResizable(NULL);

		iRect rrr(iPoint(0, 0), 100, 100), sss(iPoint(200, 200), 100, 100);
		int rrr2sss = rrr.getNearDistance(sss);

		iRect ttt(iPoint(100, 100), 100, 100);
		int rrr2ttt = rrr.getNearDistance(ttt);

		iRect u(iPoint(50, 50), 100, 100);
		int rrr2u = rrr.getNearDistance(u);

		iRect v(iPoint(50, 101), 100, 100);
		int rrr2v = rrr.getNearDistance(v);

		iRect rrs(iPoint(0, 0), iPoint(0, 50), iPoint(100, 50), iPoint(100, 30));
		double rrs_deg = rrs.lt.angleDeg(rrs.rt);

		iRect rslicex = rrs.get_slice_x(2, 0);
		double rsx_deg = rslicex.lt.angleDeg(rslicex.rt);

		iRect rslicey = rrs.get_slice_y(2, 0);
		double rsy_deg = rslicey.lt.angleDeg(rslicey.rt);

	}

}