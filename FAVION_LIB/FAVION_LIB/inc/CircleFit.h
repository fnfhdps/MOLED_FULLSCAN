#pragma once
#include <vector>
#include "Shape.h"

#ifdef DLL_EXPORTS
#define DllExport   __declspec( dllexport )
#else
#define DllExport   __declspec( dllimport )
#endif

using namespace std;
using namespace The;

//	3점 circle fit
class DllExport CCircleFit
{
public:
	CCircleFit();
	virtual ~CCircleFit();

	static bool fitCircle(std::vector<fPoint>& inpts, int iter, double threshold, fCircle& fit_circle);
	static bool fitCircle(std::vector<dPoint>& inpts, int iter, double threshold, dCircle& fit_circle);
	static bool fitCircle(std::vector<dPoint>& inpts, int iter, double threshold, dPoint& center, double& radius);
};

// 2점, 반지름 circle fit
class DllExport CCircleFit2
{
public:
	CCircleFit2();
	virtual ~CCircleFit2();

	static bool fitCircle(std::vector<fPoint>& inpts, int iter, double threshold, const double radius, fCircle& fit_circle);
	static bool fitCircle(std::vector<dPoint>& inpts, int iter, double threshold, const double radius, dCircle& fit_circle);
	static bool fitCircle(std::vector<dPoint>& inpts, int iter, double threshold, const double radius, dPoint& center);
};


#ifdef __cplusplus  
extern "C" {  // only need to export C interface if  
	// used by C++ source code  
#endif  

/// <summary>
/// 원 데이터 생성.
/// </summary>
/// <param name="c">중심 좌표.</param>
/// <param name="r">반지름.</param>
/// <param name="s">시작 각도(degree).</param>
/// <param name="offset">호 생성 각도 범위.</param>
/// <param name="delta">회전 주기 각도(degree).</param>
/// <param name="outpts">생성 데이터 포인트.</param>
/// <returns>생성 포인트 수량</returns>
	DllExport int genCircle(const dPoint c, double r, double s, double offset, double delta, std::vector<dPoint>& outpts);

/// <summary>
/// 원 데이터 생성.
/// </summary>
/// <param name="c">중심 좌표.</param>
/// <param name="r">반지름.</param>
/// <param name="s">시작 각도(degrees).</param>
/// <param name="offset">범위 각도(degrees).</param>
/// <param name="n">데이터 생성 개수.</param>
/// <param name="outpts">생성 데이터 포인트.</param>
/// <returns>각도 주기(degrees)</returns>
	DllExport double genCircle2(const dPoint c, double r, double s, double offset, int n, std::vector<dPoint>& outpts);


#ifdef __cplusplus  
}
#endif  