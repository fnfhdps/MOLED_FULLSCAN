#pragma once
#include "LxBoundary.h"
#include "LxPattern.h"
#include "math.h"

//---------------------------------------------------------------------------
#define MAX_BGA_LINE 50
#define MAX_BGA   2500
#define MAX_BGA_X 50
#define MAX_BGA_Y 50
//---------------------------------------------------------------------------

struct CGrid {

    protected :
    	bool IsUnknown;
    	bool IsMissing;
    
    public :
    	JRect IRoi;
    	bool OpIpArea; COLORREF CoIpArea;
    	bool OpIgArea; COLORREF CoIgArea;
    	bool OpRsArea; COLORREF CoRsArea;
    	bool OpRsAnls; COLORREF CoRsAnls;
    
    public :
    	bool  IsPass;
    	int   BallCount;
    	float PxLength;
    	float PxLength2D;
    
    public :
    	float ColCount ;
    	float RowCount ;
    	float ColStart ;
    	float RowStart ;
    	float ColPitch ;
    	float RowPitch ;
    
    	// 2D
    	//------------------------------------------------------------------
    	float AvgCuPx      ; float AvgTrPx      ; float AvgDvPx      ;
    	float AvgCuPy      ; float AvgTrPy      ; float AvgDvPy      ;
    	float AvgCuPitchx  ; float AvgTrPitchx  ; float AvgDvPitchx  ;
    	float AvgCuPitchy  ; float AvgTrPitchy  ; float AvgDvPitchy  ;
    	float AvgCuDiameter; float AvgTrDiameter; float AvgDvDiameter;
    	float AvgCuShape   ; float AvgTrShape   ; float AvgDvShape   ;
    
    	float MinCuPx      ; float MinTrPx      ; float MinDvPx      ;
    	float MinCuPy      ; float MinTrPy      ; float MinDvPy      ;
    	float MinCuPitchx  ; float MinTrPitchx  ; float MinDvPitchx  ;
    	float MinCuPitchy  ; float MinTrPitchy  ; float MinDvPitchy  ;
    	float MinCuDiameter; float MinTrDiameter; float MinDvDiameter;
    	float MinCuShape   ; float MinTrShape   ; float MinDvShape   ;
    
    	float MaxCuPx      ; float MaxTrPx      ; float MaxDvPx      ;
    	float MaxCuPy      ; float MaxTrPy      ; float MaxDvPy      ;
    	float MaxCuPitchx  ; float MaxTrPitchx  ; float MaxDvPitchx  ;
    	float MaxCuPitchy  ; float MaxTrPitchy  ; float MaxDvPitchy  ;
    	float MaxCuDiameter; float MaxTrDiameter; float MaxDvDiameter;
    	float MaxCuShape   ; float MaxTrShape   ; float MaxDvShape   ;
    
    	struct TBall {
    		JRect IRoi;
    		float PxLength;
    		float PxLength2D;
    		bool IsTrExist;
    		bool IsCuExist;
    		bool IsPass   ;
    		bool IsUnknown;
    		bool IsMissing;
    
    		// 2D
    		//------------------------------------------------------------------
    		float CuPx      ; float TrPx      ; float DvPx      ;
    		float CuPy      ; float TrPy      ; float DvPy      ;
    		float CuPitchx  ; float TrPitchx  ; float DvPitchx  ;
    		float CuPitchy  ; float TrPitchy  ; float DvPitchy  ;
    		float CuDiameter; float TrDiameter; float DvDiameter;
    		float CuShape   ; float TrShape   ; float DvShape   ;
    
    		float CuPitch_n ; float TrPitch_n ; float DvPitch_n ;
    		float CuPitch_e ; float TrPitch_e ; float DvPitch_e ;
    		float CuPitch_s ; float TrPitch_s ; float DvPitch_s ;
    		float CuPitch_w ; float TrPitch_w ; float DvPitch_w ;
    
    		void OnClear2D();
    
    		void SetTrExist(bool IsExist) {IsTrExist = IsExist;}
    		bool GetTrExist(            ) {return IsTrExist;}
    		void TogTrExist(            ) {SetTrExist(!GetTrExist());}
    
    	} Ball[MAX_BGA_X][MAX_BGA_Y];
    
    	void SetTrExist(int x, int y, bool IsExist) {Ball[x][y].IsTrExist = IsExist;}
    	bool GetTrExist(int x, int y              ) {return Ball[x][y].IsTrExist;}
    
    	void SetTrExist(bool IsExist) {
    		for (int y=0;y<MAX_BGA_Y; y++)
    		for (int x=0;x<MAX_BGA_X; x++)
    			Ball[x][y].IsTrExist = IsExist;
    	}
    	TPoint IsInclude(float cp, float cy);
    	void TogTrExist(float cp, float cy) {
    		TPoint RC = IsInclude(cp, cy);
    		if (RC.x<0) return;
    		if (RC.y<0) return;
    
    		SetTrExist(RC.x, RC.y, !GetTrExist(RC.x, RC.y));
    	}
    
    	void OnClear2D();
    
		// GOO 
    	//void OnUpdateTableMap(TStringGrid *Table, bool ToTable);
    	TPoint GetGridRowCol(float cp, float cy, float IgAngle = 0.0);
    	TPoint GetGridRowCol(int __Px, int __Py, float cp, float cy,float dxDv, float dyDv, float IgAngle = 0.0);
    	TPoint GetGridRowColWithoutTrExist(float cp, float cy, float IgAngle=0.0); //맵 찾기용    JRect GetIpRect();
    	float  GetIpRectAngle();
    	JRect  GetConsiderAngle(JRect TgRect, float IgAngle=0.0); //IpRect:검사영역, TgRect:회전하고 싶은 영역 범위
    	void OnPaint(TDrawTool Tool, bool IsDwMatrix=false);
    	void OnPaintBallIpRect(TDrawTool Tool);
    
    	//--------------------------------------------------------------------------
    	// Path를 입력 할때 자동으로 뒷쪽의 .ini를 검색한뒤 .ini를 제거하고 Grid.map
    	// 를 붙입니다.
    	//--------------------------------------------------------------------------
    	void OnLoad(bool IsLoad, CString Path);
};
//------------------------------------------------------------------------------

enum TBgaId {
	bgaPass          ,
	bgaMissing       ,
	bgaUnKnown       ,
	bgaInRingQuality ,
	bgaOutRingQuality,
	bgaPx            ,
	bgaPy            ,
	bgaPitch         ,
	bgaDiameter      ,
	bgaShape         ,
	bgaPosition      ,
};

class AFX_EXT_CLASS CBga  : public CFilter
{
    public:
    	CBga(void);
    	~CBga(void);

    public :
    	bool IsPass;
    	int BallCount;
    	CString PvPath;
    	TFPoint P0, P1;
//     	JRect IRoi;
		JRect IpRect;
    	int IWidth, IHeight;
//     	void *IAddress;
    
    	//20160927
		// GOO 
    // 	TStringGrid *SGRTable;
    
    	CFPattern FPattern;
    	CGrid Grid;
    	CBoundary Boundary;
    
    public :
    	double FResultInRingScore ;
    	double FResultOutRingScore;
    
    	bool DgTrain;
    	bool OpIpArea; COLORREF CoIpArea;
    	bool OpIgArea; COLORREF CoIgArea;
    	bool OpRsArea; COLORREF CoRsArea;
    	bool OpRsAnls; COLORREF CoRsAnls;
    
    	bool OpMakeRC;
    	bool OpFastFindBlob;
    	bool OpUseMatrixFitting;
    	bool OpUseMapCoordinate;
    	bool OpUseLineIntersect;
    
    	int BlobThHold;
    	int InRingThHold;
    	int OutRingThHold;
    	double InRingOffset ;
    	double OutRingOffset;
    
    	CString EMessage;
    
    	struct TSpec {
    		int BallCount;
    		int IgnoreOffset;
    		double TInRingScore ;
    		double TOutRingScore;
    
    		bool OpUnknown ;
    		bool OpMissing ;
    		bool OpPosition;
    		bool OpInRing ;
    		bool OpOutRing ;
    		bool OpSideBright;
    
    		bool OpPx      ; double Px      ; double TPx      ;
    		bool OpPy      ; double Py      ; double TPy      ;
    		bool OpPitchx  ; double Pitchx  ; double TPitchx  ;
    		bool OpPitchy  ; double Pitchy  ; double TPitchy  ;
    		bool OpDiameter; double Diameter; double TDiameter;
    		bool OpShape   ; double Shape   ; double TShape   ;
    
    		double TPosition;
    	} Spec;
    
    
    public :
    	void OnClearData();
    	//20160927
		//GOO 
    // 	void OnTable(TStringGrid *SGTable);
    	void OnPaintRingQuality(TDrawTool Tool);
    	void OnPaint           (TDrawTool Tool);
    	void OnLoad (bool IsLoad, CString Folder);
    	void OnDisplayResult(float PxResolution);
    
    	float GetLengthFrLineToPoint(TPoint P1, TPoint P2, TPoint P3); //직선과 한점과의 거리
    	float GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3);
    	bool  OnLineFitRANSAC(TPoint *Q1, int N1, TPoint *Q2, int N2, double dist_th, int max_iter, int num_th);
    	int   OnLineFitPCA(TPoint P[], int N, double weight[]);
    
    public :
    	TBgaId BinId;
    	struct TQuality {
    		TBgaId BinId;
    		float InRingScore;
    		float OutRingScore;
    		float MvCx, MvCy;
    		int x, y;
    	} Quality[MAX_BGA];
    
    	struct TPosition {
    		bool IsPass;
    		float Px, Py; //TRAIN BALL POSITION
    		float Cx, Cy; //CURRENT BALL POSITION
    		float Dx, Dy;
    		float Do;
    	} Position[MAX_BGA];
    
    	bool OnTrain();
    	bool OnFindMap();
    	bool OnCoordinate();
    	bool OnRingQuality();
    	int  OnMeasuring();
    	bool OnPosition();
    	int  OnExecute(); //OnCoordinate, OnPosition, OnRingQuality, OnMeasuring
    
    	//20160927
		//GOO 
    // 	void OnUpdateTableResult(TStringGrid *Table, float PxResolution);
};

