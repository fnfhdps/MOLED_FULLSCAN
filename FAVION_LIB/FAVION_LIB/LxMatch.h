#pragma once

//---------------------------------------------------------------------------
#include "LxHeader.h"
#include "LxTrace.h"
//---------------------------------------------------------------------------

class AFX_EXT_CLASS CMatch : public CFilter
{
private :
	TPoint OnRotate(TPoint c, TPoint t, double deg);
	TPoint OnRotate(double cx, double cy, double tx, double ty, double deg);

protected :
	int FContrastMode;
	int FCorrelationMode;
public :
	// Architecture
	// ---------------------------------------------------------------------
	EMatcher *Matcher; // EMach instance
// 	JTrace    Trace  ; // JTrace instance 
	EImageBW8 Train  ; // Train Match Image

	// Property
	// ---------------------------------------------------------------------
	int FTrainImageWidth  ;
	int FTrainImageHeight ;
	float FMaxAngle       ;
	float FMaxScale       ;
	float FMinAngle       ;
	float FMinScale       ;
	float FMinScore       ;
	int   FMaxPositions   ;

	// ROI
	int Roi_x;
	int Roi_y;
	int Roi_h;
	int Roi_w;

	// Result
	// ---------------------------------------------------------------------
	bool FPass;
	int FFindCount;

#define MAX_SRH 100
	struct TResult {
		bool  FPass;
		float FScale;
		float FAngle;
		float FPx   ;
		float FPy   ;
		int   FId   ;
		JRect FRect ;
		float FScore;
	} Result[MAX_SRH];


public :
	float TPx;
	float TPy;

	// Creat & Destory
	// ---------------------------------------------------------------------
	CMatch();
	~CMatch();

	// Functions -Set
	// ---------------------------------------------------------------------
	void SetMaxAngle    (float MaxAngle        );
	void SetMaxScale    (float MaxScale        );
	void SetMinAngle    (float MinAngle        );
	void SetMinScale    (float MinScale        );
	void SetMinScore    (float MinScore        );
	void SetMaxPositions(int   MaxPositions    );
	void SetContrastMode(int ContrastMode      );
	void SetCorrelationMode(int CorrelationMode);

	// Functions -Get
	// ---------------------------------------------------------------------
	float GetMaxAngle    ();
	float GetMaxScale    ();
	float GetMinAngle    ();
	float GetMinScale    ();
	float GetMinScore    ();
	int   GetMaxPositions();

	// Functions - Excute
	// ---------------------------------------------------------------------
	int OnTrain  ();
	int OnExecute();
	void OnClear ();

	// Functions - Result
	// ---------------------------------------------------------------------
	bool  GetResultPass     (     ) { return FPass; }
	int   GetResultFindCount(     ) { return FFindCount; }
	int   GetResultPass     (int n) {
		if (n>=MAX_SRH) return 0;
		return Result[n].FPass ;
	}
	float GetResultScale    (int n) { if (n>=MAX_SRH) return 0; return Result[n].FScale; }

	float GetResultAngle    (int n) {
		if (n>=MAX_SRH) return 0;
		return Result[n].FAngle;
	}

	float GetResultPx       (int n) {
		if (n>=MAX_SRH) return 0;
		return Result[n].FPx   ;
	}

	float GetResultPy       (int n) {
		if (n>=MAX_SRH) return 0;
		return Result[n].FPy   ;
	}

	int   GetResultId       (int n) { if (n>=MAX_SRH) return 0; return Result[n].FId   ; }
	JRect GetResultRect     (int n) { return Result[n].FRect ; }
	float GetResultScore    (int n) { if (n>=MAX_SRH) return 0; return Result[n].FScore; }

	// Paint
	// ---------------------------------------------------------------------
	void OnPaint       (TDrawTool Tool);
	void OnPaintResult (TDrawTool Tool);
	void OnPaintPattern(TDrawTool Tool);

	// Load & Save
	// ---------------------------------------------------------------------
	void OnLoad(bool IsLoad, CString Folder, int Type=0);
};

