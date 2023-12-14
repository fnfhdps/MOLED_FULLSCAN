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
class DllExport CCircleFit
{
public:
	CCircleFit();
	virtual ~CCircleFit();

	static bool fitCircle(std::vector<fPoint>& inpts, int iter, double threshold, fCircle& fit_circle);
	static bool fitCircle(std::vector<dPoint>& inpts, int iter, double threshold, dCircle& fit_circle);
	static bool fitCircle(std::vector<dPoint>& inpts, int iter, double threshold, dPoint& center, double& radius);
};

// 2��, ������ circle fit
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
/// �� ������ ����.
/// </summary>
/// <param name="c">�߽� ��ǥ.</param>
/// <param name="r">������.</param>
/// <param name="s">���� ����(degree).</param>
/// <param name="offset">ȣ ���� ���� ����.</param>
/// <param name="delta">ȸ�� �ֱ� ����(degree).</param>
/// <param name="outpts">���� ������ ����Ʈ.</param>
/// <returns>���� ����Ʈ ����</returns>
	DllExport int genCircle(const dPoint c, double r, double s, double offset, double delta, std::vector<dPoint>& outpts);

/// <summary>
/// �� ������ ����.
/// </summary>
/// <param name="c">�߽� ��ǥ.</param>
/// <param name="r">������.</param>
/// <param name="s">���� ����(degrees).</param>
/// <param name="offset">���� ����(degrees).</param>
/// <param name="n">������ ���� ����.</param>
/// <param name="outpts">���� ������ ����Ʈ.</param>
/// <returns>���� �ֱ�(degrees)</returns>
	DllExport double genCircle2(const dPoint c, double r, double s, double offset, int n, std::vector<dPoint>& outpts);


#ifdef __cplusplus  
}
#endif  