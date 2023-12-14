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

//	3�� circle fit
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
/// Ÿ�� ������ ����.
/// </summary>
/// <param name="c">�߽� ��ǥ.</param>
/// <param name="r">������.</param>
/// <param name="s">���� ����(degree).</param>
/// <param name="offset">ȣ ���� ���� ����.</param>
/// <param name="delta">ȸ�� �ֱ� ����(degree).</param>
/// <param name="outpts">���� ������ ����Ʈ.</param>
/// <returns>���� ����Ʈ ����</returns>
	DllExport int genEllipse(const dPoint c, double deg, double major, double minor, double s, double offset, double delta, std::vector<dPoint>& outpts);

/// <summary>
/// Ÿ�� ������ ����.
/// </summary>
/// <param name="c">�߽� ��ǥ.</param>
/// <param name="r">������.</param>
/// <param name="s">���� ����(degrees).</param>
/// <param name="offset">���� ����(degrees).</param>
/// <param name="n">������ ���� ����.</param>
/// <param name="outpts">���� ������ ����Ʈ.</param>
/// <returns>���� �ֱ�(degrees)</returns>
	DllExport double genEllipse2(const dPoint c, double deg, double major, double minor, double s, double offset, int n, std::vector<dPoint>& outpts);


#ifdef __cplusplus  
}
#endif  