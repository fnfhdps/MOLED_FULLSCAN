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

class DllExport CLineFit
{
public:
	CLineFit();
	virtual ~CLineFit();

	static bool fitLine(std::vector<fPoint>& inpts, int iter, double threshold, fLine& fit_line);
	static bool fitLine(std::vector<dPoint>& inpts, int iter, double threshold, dLine& fit_line);
	static bool fitLine(std::vector<dPoint>& inpts, int iter, double threshold, dPoint& p0, dPoint& p1);
};

