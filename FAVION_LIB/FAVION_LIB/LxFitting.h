#pragma once
#include "LxType.h"
#include "math.h"
//-------------------------------------------------
#include <vector>
class AFX_EXT_CLASS CCircleFit
{
    public:
    	CCircleFit ();
    	~CCircleFit();
    
    protected :
    	int  OnCircum          (double x1, double x2, double x3, double y1, double y2, double y3, double *cx, double *cy, double *rad);
    	bool OnSolveLinearEQ3X3(double A[9], double bb[3], double x[3]);
    	int  OnLS              (int N, double xp[], double yp[], double *cx, double *cy, double *rad);
    	void GetRandomTriplet  (int max_num, int triplet[3]);
    public :
    	int  OnRansac(TPoint  Q[], int N, int sample_th, double dist_deviate_th, TFPoint *C, double *radius);
    	int  OnRansac(TFPoint Q[], int N, int sample_th, double dist_deviate_th, TFPoint *C, double *radius);
};

class AFX_EXT_CLASS CLineFit
{
    public:
		CLineFit ();
		~CLineFit();
    
    protected :
    	int OnPCA(TFPoint P[], int N, double weight[]);
    public :
    	TFPoint P0;
    	TFPoint P1;
    
    	float GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);
    	float GetDistance(float x1, float y1, float x2, float y2, float x, float y);
    	bool  OnRANSAC(TFPoint *Q, int N, double dist_th=1.5);
    	bool  OnRANSAC(TFPoint *Q1, int N1, TFPoint *Q2, int N2, double dist_th, int max_iter, int num_th);
    
};



