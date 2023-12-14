#pragma once
#include "LxFitting.h"
#include "LxMath.h"

//---------------------------------------------------------------------------
#define MAX_FIT_LINE 50
//---------------------------------------------------------------------------
class AFX_EXT_CLASS CFLine
{
    public :
    	CFLine ();
    	~CFLine();
    
    	int FittingType;
    
    	TFPoint P0;
    	TFPoint P1;
    	CLineFit LineFit;
    	int DataCount;
    
    	void OnRansac(int Count, TFPoint *Q);
};

class AFX_EXT_CLASS CFPattern
{
    public:
    	CFPattern ();
    	~CFPattern();
    
    	int ColCount;
    	int RowCount;
    
    	CFLine Col[MAX_FIT_LINE];
    	CFLine Row[MAX_FIT_LINE];
    
    	TFPoint WP[MAX_FIT_LINE][MAX_FIT_LINE]; 
    	TFPoint CP[MAX_FIT_LINE][MAX_FIT_LINE]; 
    	TFPoint BP[MAX_FIT_LINE][MAX_FIT_LINE]; 
    	TFPoint Obj[MAX_FIT_LINE][MAX_FIT_LINE]; //실제위치
    	TFPoint Pos[MAX_FIT_LINE][MAX_FIT_LINE]; //격자위치
    
    	void OnLineIntersection(TFPoint L1_P0, TFPoint L1_P1, TFPoint L2_P0, TFPoint L2_P1, TFPoint *P);
    
    	float GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);
    	void OnUpdatePos(int IntersectMode, int __ColCount, int __RowCount);
    	void LinePoint(int __RowCount,int __ColCount,int pitchx,int pitchy);
    	TFLine LineFitting(int count, int row);
    
    	bool IsRectFitting(TFPoint *lpPoint)
    	{
    		//*lpPoint[0] = ;
    		//*lpPoint[1] = ;
    		//*lpPoint[2] = ;
    		//*lpPoint[3] = ;
    		//*lpPoint[4] = ;
    		//*lpPoint[5] = ;
    		//*lpPoint[6] = ;
    		//*lpPoint[7] = ;
    		return true;
    	}
};

