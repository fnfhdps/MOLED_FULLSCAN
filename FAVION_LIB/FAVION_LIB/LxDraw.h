#pragma once
#include "LxHeader.h"

class AFX_EXT_CLASS CDraw
{
public:
	CDraw (void);
	~CDraw(void);

public :
	int DrawXCount;
	int DrawYCount;
	bool IsBusy;
	int DisplayColor;
	bool IsFalseColor;

public :
	float GetHoCtPos(EImageBW8 *lpImage, float Value);
	float GetVtCtPos(EImageBW8 *lpImage, float Value);

	void OnImage(TDrawTool Tool, EImageBW8 *lpImage);
	void OnImage(TDrawTool Tool, EImageBW8 *lpImage, int w, int h);
	void OnImageBinary(TDrawTool Tool, EImageBW8 *lpImage, int w, int h, int ThHold);
	void OnWindowSize(TDrawTool Tool, EImageBW8 *lpImage);
	void OnWindowSize(TDrawTool Tool, EImageBW8 *lpImage, int w, int h);
	void OnCrossLine(bool IsDraw, TDrawTool Tool, EImageBW8 *lpImage, int Mx, int My);
	void OnCrossCenterLine(TDrawTool Tool, EImageBW8 *lpImage, int Mx, int My);
	void OnCrossMark(bool IsDraw, TDrawTool Tool, EImageBW8 *lpImage, int cx, int cy, int Size=20,COLORREF Color=AQUA);
	void OnRoi  (TDrawTool Tool, EImageBW8 *lpImage, EROIBW8 *lpRoi, COLORREF Color);
	void DrawStrechImage(TDrawTool Tool, EImageBW8 *lpImage, EROIBW8 *lspRoi, COLORREF IspColor, EROIBW8 *TrnRoi, COLORREF TrnColor);

	// 임시로 주석처리함 
	void DrawImage(TDrawTool Tool, EImageBW8 *lpImage, EROIBW8 *IspRoi, COLORREF IspColor, EROIBW8 *TrnRoi, COLORREF TrnColor, bool drawROI);
	void DrawBinaryImage(TDrawTool Tool, EImageBW8 *lpImage, int ThHold);
};

