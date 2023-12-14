// LxFind.h: interface for the CFind class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXFIND_H__47E8EEAE_ED4B_4F04_A88E_A3A2CEB34196__INCLUDED_)
#define AFX_LXFIND_H__47E8EEAE_ED4B_4F04_A88E_A3A2CEB34196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LxHeader.h"

class AFX_EXT_CLASS CFind : public CFilter 
{
private:
	TPoint OnRotate(TPoint c, TPoint t, double deg);
	TPoint OnRotate(double cx, double cy, double tx, double ty, double deg);

public :
	bool IsPass;
	CString PvPath;
// 	EImageBW8 *Image;
// 	EROIBW8 *IRoi;
// 	EROIBW8 *TRoi;
// 	EROIBW8 *MRoi;
	EImageBW8 Train  ;
	EImageBW8 MaskImg;
	
public :
	EPatternFinder *PatternFinder;          // EPatternFinder Object
	std::vector< EFoundPattern > results;  // result of a find
	//enum EDragHandle m_eHandle;            // Currently found ROI handle
	int TrImageWidth;
	int TrImageHeight;
	int FindCount;
	
	struct TProperty {
		int MaxInstances;
		int ContrastMode;
		int PatternType ;
		float LightBalance;
		float AngleBias     ;
		float AngleTolerance;
		float ScaleBias     ;
		float ScaleTolerance;
		float FindExtension ;
		bool  ReductionMode;
		float ReductionStrength;
		float MinFeaturePoints;
		float MaxFeaturePoints;
		float LocalSearchMode;
		float AngleSearchExtent;
		float ScaleSearchExtent;
		float XSearchExtent;
		float YSearchExtent;
		bool DrawBoundingBox  ;
		bool DrawCenter       ;
		bool DrawFeaturePoints;
		bool IsUseIgnoreRoi;
		bool IsDarkIgnoreRoi;
		bool OpIgnoreSamePos;
		int  IgnoreSamePosX ;
		int  IgnoreSamePosY ;
		
		float Ox; // Offset x value
		float Oy; // Offset y value

		float TPx; // Train Center x value
		float TPy; // Train Center y value
		
		float MinScore; //
		float ZeroScore; // Max Score 이하 인경우는 Score 값을 0으로 처리
		
	} Property;
	
#define MAX_SRH 100
	struct TResult {
		bool IsPass;
		float Scale;
		float Angle;
		float Px   ;
		float Py   ;
		float Id   ;
		CRect Rect;
		float Score;
	} Result[MAX_SRH];
	
	bool IgnoreResult[MAX_SRH];
	
	CFind();
	~CFind();

	// Functions -Set
	// ---------------------------------------------------------------------
	void SetDrawBoundingBox   (bool  DrawBoundingBox   );
	void SetDrawCenter        (bool  DrawCenter        ); 
	void SetDrawFeaturePoints (bool  DrawFeaturePoints );
	void SetIsUseIgnoreRoi    (bool  IsUseIgnoreRoi    );
	void SetIsDarkIgnoreRoi   (bool  IsDarkIgnoreRoi   );
	void SetOpIgnoreSamePos   (bool  OpIgnoreSamePos   );
	void SetReductionMode     (EReductionMode Mode     );
	void SetLightBalance      (float LightBalance      );
	void SetAngleBias         (float AngleBias         );
	void SetAngleTolerance    (float AngleTolerance    );
	void SetScaleBias         (float ScaleBias         );
	void SetScaleTolerance    (float ScaleTolerance    );
	void SetFindExtension     (float FindExtension     );
	void SetReductionStrength (float ReductionStrength );
	void SetMinFeaturePoints  (float MinFeaturePoints  );
	void SetMaxFeaturePoints  (float MaxFeaturePoints  );
	void SetLocalSearchMode   (ELocalSearchMode Mode   );
	void SetAngleSearchExtent (float AngleSearchExtent );
	void SetScaleSearchExtent (float ScaleSearchExtent );
	void SetXSearchExtent     (float XSearchExtent     );
	void SetYSearchExtent     (float YSearchExtent     );
	void SetMaxInstances      (int   MaxInstances      );
	void SetContrastMode      (EFindContrastMode Mode  );
	void SetIgnoreSamePosX    (int   IgnoreSamePosX    );
	void SetIgnoreSamePosY    (int   IgnoreSamePosY    );
	void SetOx                (float Ox                );
	void SetOy                (float Oy                );
	void SetMinScore          (float MinScore          );
	void SetZeroScore         (float ZeroScore         );
	void SetPatternType       (EPatternType Type       );

	// Functions -Get
	// ---------------------------------------------------------------------
	bool  GetDrawBoundingBox   ();
	bool  GetDrawCenter        ();
	bool  GetDrawFeaturePoints ();
	bool  GetIsUseIgnoreRoi    ();
	bool  GetIsDarkIgnoreRoi   ();
	bool  GetOpIgnoreSamePos   ();
	bool  GetReductionMode     ();
	float GetLightBalance      ();
	float GetAngleBias         ();
	float GetAngleTolerance    ();
	float GetScaleBias         ();
	float GetScaleTolerance    ();
	float GetFindExtension     ();
	float GetReductionStrength ();
	float GetMinFeaturePoints  ();
	float GetMaxFeaturePoints  ();
	float GetLocalSearchMode   ();
	float GetAngleSearchExtent ();
	float GetScaleSearchExtent ();
	float GetXSearchExtent     ();
	float GetYSearchExtent     ();
	int   GetMaxInstances      ();
	int   GetContrastMode      ();
	int   GetIgnoreSamePosX    ();
	int   GetIgnoreSamePosY    ();
	int   GetPatternType       ();
	float GetOx                ();
	float GetOy                ();
	float GetMinScore          ();
	float GetZeroScore         ();

	// Functions - Result
	// ---------------------------------------------------------------------
// 	bool  GetResultPass     (     ) { return FPass; }
// 	int   GetResultFindCount(     ) { return FFindCount; }
	int   GetResultPass     (int n) { return Result[n].IsPass ; }
	float GetResultScale    (int n) { return Result[n].Scale; }
	float GetResultAngle    (int n) { return Result[n].Angle; }
	float GetResultPx       (int n) { return Result[n].Px   ; }
	float GetResultPy       (int n) { return Result[n].Py   ; }
	int   GetResultId       (int n) { return (int)(Result[n].Id); }
	CRect GetResultRect     (int n) { return Result[n].Rect ; }
	float GetResultScore    (int n) { return Result[n].Score; }
// 	int   GetResultIgnore   (int n) { return Result[n].Ignore; }


	// Train & Inspection
	//---------------------------------------------------------------------------------
	int OnTrain();
	int OnTrain(BYTE* imgPtr, int imgW, int imgH, int roix, int roiy, int roiW, int roiH);
	int OnExecute();

	// Paint
	//-------------------------------------------------------------------
	void OnPaint(TDrawTool Tool, int RColor);
	void OnPaintResult(TDrawTool Tool);
	void OnPaintPattern(TDrawTool Tool);

    // Load
	//------------------------------------------------------------------
// 	void OnLoad(bool IsLoad, CString Folder);
	void OnLoad(bool IsLoad, CString Folder, int Type=0);
// 	void OnLoad(bool IsLoad, CString Folder, int Type=0, int Count=0);
};

#endif // !defined(AFX_LXFIND_H__47E8EEAE_ED4B_4F04_A88E_A3A2CEB34196__INCLUDED_)
