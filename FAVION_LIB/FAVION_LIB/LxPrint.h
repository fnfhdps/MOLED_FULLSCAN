//---------------------------------------------------------------------------

#pragma once
//---------------------------------------------------------------------------
//#include "vcl.h"

//#include "LxDraw.h"
#include "stdafx.h"
#include "LxBlob.h"
#include "LxMatch.h"
#include "LxGdi.h"
#include "LxHeader.h"
#include "LxTrace.h"
//---------------------------------------------------------------------------
#define MAX_PRINT_FONT 50
#define MAX_OV_PRINT_PIXEL 1000
#define MAX_UN_PRINT_PIXEL 1000
//---------------------------------------------------------------------------

enum TOcvId {
    ocvPass        ,
    ocvStringCount ,
    ocvAreaMatch   ,
    ocvFontMatch   ,
    ocvOverPrint   ,
    ocvUnderPrint  ,
    ocvAngle       ,
    ocvGabLeft     ,
    ocvGabTop      ,
    ocvGabRight    ,
    ocvGabBottom   ,
};

class AFX_EXT_CLASS CPrint : public CFilter 
{
    public :
        bool DwAreaFind;
        bool DwReferString;
        bool DwScore;
        bool DwOverPrintPixel;
        bool DwUnderPrintPixel;
        bool DwAngle;
        bool DwFont;
        bool DwIRoi;

    public :
        TOcvId BinId;
        int Mode; // 0: BySrh, 1:ByBlob
        bool IsPass;
        int FindAreaCount;
        int FindFontCount;
        int FindDefectCount;
        int OverPrintPixelCount;
        int UnderPrintPixelCount;
        TPoint OvPrintPixel[MAX_OV_PRINT_PIXEL];
        TPoint UnPrintPixel[MAX_UN_PRINT_PIXEL];
        float ResultAngle;

        float RectMatchScore;
        bool IsRectMatch;
        float BlobMatchScore[MAX_PRINT_FONT];
        bool IsBlobMatch[MAX_PRINT_FONT];
        bool IsIgnore[MAX_PRINT_FONT];

//         EImageBW8 Image;
//         EROIBW8 IRoi;
//         EROIBW8 TRoi;
        EROIBW8 AreaRoi;
        EROIBW8 FontRoi;
        //CBlob Trace;
		JTrace  Trace;
		CMatch  AreaFind;
		CMatch  FontFind[MAX_PRINT_FONT];
		HMath Math;
public :
        struct TProperty{
            int IpThHold;
            int DvThHold;
            int TrThHold;
            int StaAgId;
            int EndAgId;
            //20160718
            double ShRectAngle;
            double ShFontAngle;
            bool OpStringCount;
            bool OpGabAngle    ; double GabAngle ;
            bool OpGabLeft     ; double GabLeft  ;
            bool OpGabTop      ; double GabTop   ;
            bool OpGabRight    ; double GabRight ;
            bool OpGabBottom   ; double GabBottom;
            bool OpOverPrinted ; double OverPrintedX ; double OverPrintedY;
            bool OpUnderPrinted; double UnderPrintedX; double UnderPrintedY;
            int xBleed         ; int yBleed     ;
            double ShRectScore    ; double ShBlobScore;
            int TrMargin;
            int ShMargin;
            double PassRectScore;
            double PassBlobScore;
            CString ReferString;
        };
		TProperty Property;

		struct TTrain {
			bool IsEnabled;
			int Count;
			float Gx[MAX_PRINT_FONT];
			float Gy[MAX_PRINT_FONT];
			JRect AreaRect;
			JRect FontRect[MAX_PRINT_FONT];
			EImageBW8 Image;
			CString Simbol[MAX_PRINT_FONT];
		} Train;


    public :
        CPrint();
        ~CPrint();

        bool IsEnabled;
        int MarkId;
        int PxTrOffset;
        bool OnOverPrint(int TrThHold, int CuThHold);
        bool OnUnderPrint(int TrThHold, int CuThHold);
        bool OnAngle();
        float GetAngle();

		CString GetOcvName();
        int OnTrain(CString ReferStr);
        int OnTrainWidthRect(int RectCount, JRect *Rect, CString ReferStr);
        int OnExecute(); // 0: BySrh, 1:ByBlob
        int OnExecuteBySrh();
        int OnExecuteByBlob();
        void OnLoadRoi(bool IsLoad, CString Path, EROIBW8 *lpRoi);
        void OnLoad(bool IsLoad, CString Folder);
		void OnPaint      (TDrawTool Tool);
		void OnPaintResult(TDrawTool Tool);
        void OnUpdate();

    public :
        float FResultGabLeft   ;
        float FResultGabTop    ;
        float FResultGabRight  ;
        float FResultGabBottom ;
        float FResultOverPrintLength ;
        float FResultUnderPrintLength;

        float GetResultScore() { return AreaFind.Result[0].FAngle; }
        float GetResultAngle() { return AreaFind.GetResultScore(0); }

        float GetResultOverPrintLength () { return FResultOverPrintLength ; }
        float GetResultUnderPrintLength() { return FResultUnderPrintLength; }

        float GetResultGabLeft  () { return FResultGabLeft  ; }
        float GetResultGabTop   () { return FResultGabTop   ; }
        float GetResultGabRight () { return FResultGabRight ; }
        float GetResultGabBottom() { return FResultGabBottom; }

};

