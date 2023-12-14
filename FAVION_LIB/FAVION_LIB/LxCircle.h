#pragma once
#include "LxTrace.h"

class AFX_EXT_CLASS CCircle : public CFilter 
{
#define MAX_CIRCLE 10
protected :
	// Architecture
	// ---------------------------------------------------------------------
	//         EWorldShape *WorldShape; // EWorldShape instance
	ECircleGauge *CircleGauge; // ECircleGauge instance
	ECircle Circle; // ECircle instance

protected :
	// Architecture
	// ---------------------------------------------------------------------
	int Mode; //0 : Nomal, 1: Dark Blob
	JTrace Trace;

	// Property
	// ---------------------------------------------------------------------
	int FThickness         ;
	int FThresHold         ;
	int FSmoothing         ;
	int FNumFilteringPasses;

	int FTransitionChoice  ;  // From Begin, From End, Largest Amplitude, Largest Area, Closest
	int FCircleType        ;  // White To Black, Black To White
	int FTransitionIndex   ;
	int FSamplingStep      ;  // Sample Step(Edge ���Ⱓ��)
	float FDiameter        ;
	float FTolerance       ;  // �˻� ����  
	float FAngle           ;  // �˻� ���� 
	float FAmplitude       ;  // �˻� �� 
	float FCenterX         ;  // Circle_Fitting �߽���ǥ 
	float FCenterY         ;  // Circle_Fitting �߽���ǥ 

public :
	// Result
	// ---------------------------------------------------------------------
	TFPoint FCP;
	float Diameter;

public :
	// Architecture
	// ---------------------------------------------------------------------
	// Display (Paint���� ����)
	bool DwNominal      ;
	bool DwActual       ;
	bool DwSampledPaths ;
	bool DwSampledPath  ;
	bool DwSampledPoints;

	// Creat & Destory
	// ---------------------------------------------------------------------
	CCircle();
	~CCircle();
	//----------------------------------------------------------------------

	// Value
	//----------------------------------------------------------------------
	int FCirclePointCount;
	TFPoint *FCirclePoint;
	//----------------------------------------------------------------------

	// Functions - Set
	// ---------------------------------------------------------------------
	// GOO(20170601)	
	void SetCircleChoice (int TransitionChoice       ); //{ FTransitionChoice = TransitionChoice;          ;}
	void SetCircleType   (int CircleType             ); //{ FCircleType       = CircleType                 ;}
	void SetCircleIndex  (int CircleIndex            ); //{ FTransitionIndex  = CircleIndex                ;}
	void SetSamplingStep (float SamplingStep         ); //{ FSamplingStep     = SamplingStep               ;}
	void SetTolerance    (float Tolerance            ); //{ FTolerance        = Tolerance                  ;}
	void SetDiameter     (float Diameter             ); //{ FDiameter         = Diameter                   ;}
	void SetAngle        (float Angle                ); //{ FAngle            = Angle                      ;}
	void SetAmplitude    (float Amplitude            ); //{ FAmplitude        = Amplitude                  ;}
	void SetCenterXY     (float CenterX,float CenterY); //{ FCenterX          = CenterX; FCenterY = CenterY;}

	// Functions - Get
	// ---------------------------------------------------------------------
	//         EWorldShape *GetWorldShape() { return WorldShape; }
	float GetTolerance() { return FTolerance; }

	// Functions - Execute
	// ---------------------------------------------------------------------
	int OnExecute(bool IsValid=false);

	// Functions - Result
	// ---------------------------------------------------------------------
	TFPoint GetResultCenterPoint() { return FCP; }
	float GetResultDiameter() { return Diameter; }

	// Functions - Paint
	// ---------------------------------------------------------------------
	void OnPaint(TDrawTool Tool);

	// Functions - Load & Save
	// ---------------------------------------------------------------------
	void OnLoad(bool IsLoad, CString Folder);
};

