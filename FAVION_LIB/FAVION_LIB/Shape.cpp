#include "stdafx.h"
#include "Shape.h"

namespace The
{	
	//////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// TheMat
	TheMath::TheMath(){}
	
	TheMath::~TheMath()
	{}

	double TheMath::deg2rad(double deg)
	{
		return deg * (M_PI / 180.);
	}

	double TheMath::rad2deg(double rad)
	{
		double deg = rad * (180. / M_PI);
		return deg < 0. ? deg + 360 : deg;
	}

	double TheMath::deg360(double deg)
	{
		return (deg == 360.) ? 0. : ((deg >= 0.) ? deg : deg + 360.);
	}

	double TheMath::rad360(double rad)
	{
		return deg360(rad2deg(rad));
	}

	//////////////////////////////////////////////////////////////////////////
	//http://www.aip.de/groups/soe/local/numres/bookcpdf/c14-1.pdf
	//Given an array of data[1..n], this routine returns its mean ave, average deviation adev,
	//standard deviation sdev, variance var, skewness skew, and kurtosis curt.
	bool moment(const double data[], int n, double *ave, double *adev, double *sdev,
		double *var, double *skew, double *curt)
	{
		int j;
		double ep = 0.0, s = 0.0, p = 0.0;
		if (n <= 1) return false;

		//First pass to get the mean.
		for (j = 0; j < n; j++) s += data[j];
		*ave = s / n;

		//Second pass to get the first(absolute), second,
		//third, and fourth moments of the
		//deviation from the mean.

		*adev = (*var) = (*skew) = (*curt) = 0.0;

		for (j = 0; j < n; j++) {
			*adev += fabs(s = data[j] - (*ave));
			ep += s;
			*var += (p = s*s);
			*skew += (p *= s);
			*curt += (p *= s);
		}

		*adev /= n;
		*var = (*var - ep*ep / n) / (n - 1);	//Corrected two - pass formula.
		*sdev = sqrt(*var);						//Put the pieces together according to the conventional definitions.

		if (*var) {
			*skew /= (n*(*var)*(*sdev));
			*curt = (*curt) / (n*(*var)*(*var)) - 3.0;
		}
		else
			return false;

		return true;
	}

}
