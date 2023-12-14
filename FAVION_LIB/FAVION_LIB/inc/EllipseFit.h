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
class DllExport CEllipseFit
{
public:
	CEllipseFit();
	virtual ~CEllipseFit();

	static bool fitEllipse(std::vector<dPoint>& inpts, int iter, double threshold, double& major, double& minor, double& theta, dPoint& center);
	double ArcDistance(double x1, double x2, int N);
};


#ifdef __cplusplus  
extern "C" {  // only need to export C interface if  
	// used by C++ source code  
#endif  

/// <summary>
/// 타원 데이터 생성.
/// </summary>
/// <param name="c">중심 좌표.</param>
/// <param name="r">반지름.</param>
/// <param name="s">시작 각도(degree).</param>
/// <param name="offset">호 생성 각도 범위.</param>
/// <param name="delta">회전 주기 각도(degree).</param>
/// <param name="outpts">생성 데이터 포인트.</param>
/// <returns>생성 포인트 수량</returns>
	DllExport int genEllipse(const dPoint c, double deg, double major, double minor, double s, double offset, double delta, std::vector<dPoint>& outpts);

/// <summary>
/// 타원 데이터 생성.
/// </summary>
/// <param name="c">중심 좌표.</param>
/// <param name="r">반지름.</param>
/// <param name="s">시작 각도(degrees).</param>
/// <param name="offset">범위 각도(degrees).</param>
/// <param name="n">데이터 생성 개수.</param>
/// <param name="outpts">생성 데이터 포인트.</param>
/// <returns>각도 주기(degrees)</returns>
	DllExport double genEllipse2(const dPoint c, double deg, double major, double minor, double s, double offset, int n, std::vector<dPoint>& outpts);


#ifdef __cplusplus  
}
#endif  