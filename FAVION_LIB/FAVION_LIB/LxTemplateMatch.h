#pragma once
#include "LxHeader.h"

#define NUMBEROF_LINES           7500
#define NUMBEROF_SAMPLES          750

#define NUMBEROF_MAX_DEFECT         50
#define NUMBEROF_MAX_GROUP         500
#define NUMBEROF_POINT_GROUP      3000
#define NUMBEROF_POINT_PIXEL     10000

class AFX_EXT_CLASS CTemplateMatch : public CFilter
{
public:
	struct TTrain {
		BYTE MinTh[NUMBEROF_LINES][NUMBEROF_SAMPLES];
		BYTE MaxTh[NUMBEROF_LINES][NUMBEROF_SAMPLES];
	} Train;

	CTemplateMatch(void);
	~CTemplateMatch(void);

        struct TGroup {
            int PointCount;
            TPoint Point[NUMBEROF_POINT_PIXEL];
        };
        struct TDefect {
            int n;
            JRect Rect;
            void OnCopy (TDefect *lpDefect) {OnClear(); n = lpDefect->n; Rect = lpDefect->Rect; }
            void OnClear()                  {n = 0;  Rect.Clear(); }
        };

        TPoint *Point;
        int PointCount;

        TGroup *Group;
        int GroupCount;

        TDefect *Defect;

        int DefectCount ;
        int MissingCount;
        int InkDustCount;

        bool IsErMisMatch    ;
        bool IsErBumpMisMatch;
        bool IsErInkDust     ;
        bool EnableBumpROI   ;
        bool EnableDustROI   ;
        bool EnableScratchROI;

        TPoint LBumpROI[4]   ;
        TPoint TBumpROI[4]   ;
        TPoint RBumpROI[4]   ;
        TPoint BBumpROI[4]   ;

        TPoint LDustROI[4]   ;
        TPoint TDustROI[4]   ;
        TPoint RDustROI[4]   ;
        TPoint BDustROI[4]   ;

        TPoint LScratchROI[4];
        TPoint TScratchROI[4];
        TPoint RScratchROI[4];
        TPoint BScratchROI[4];

        struct TTpPa {
            int Micron          ;
            int And             ;
            int MaxPxCnt        ;
            int MinPxCnt        ;
            int MinBumpPxCnt    ;
            //int MaxDefectCnt    ;
            //int MaxGroupCnt     ;
            int LineGap         ;
            int DIS_POINT       ;
            int Offset          ;
            int Samples         ;
            int LineStep        ;
            int CompareStep     ;
            int CompareBumpStep ;
            int HiThMargin      ;
            int LoThMargin      ;
            int HiThBumpMargin  ;
            int LoThBumpMargin  ;
            int MinHorLength    ;
            int MaxHorLength    ;
            int MinVerLength    ;
            int MaxVerLength    ;
            int MinBumpHorLength;
            int MinBumpVerLength;
            int MinDustHorLength;
            int MinDustVerLength;
            int xMgPxCnt        ;
            int yMgPxCnt        ;
            int MulTiTeach      ;
            int StandardChip    ;
            int MultiCompare    ;
            int InkDustPixel    ;
        } TpPa;

        TFPoint *lpErCoordinate(); //{
        /*
            TPoint Pt[NUMBEROF_MAX_DEFECT];
            register int Cnt = 0;

            for (register int k=0; k<DefectCount; k++) {
                Pt[Cnt++] = Defect[k].Rect.Point();
            }
            for (register int k=Cnt; k<NUMBEROF_MAX_DEFECT; k++) Pt[k].x = Pt[k].y = 0;

            return &Pt[0];
        }
        */

        void OnSortPoint ();

        void ClrTrain    ();
        void ClrResult   ();
        void MemoryClear ();
        void SetROI      (int SleLight, TPoint p1, TPoint p2, TPoint p3, TPoint p4);
        bool SetPosition (int SleLight, int x, int y)                              ;


        void OnTrain     (TPoint p1, TPoint p2, TPoint p3, TPoint p4              );
        bool OnInspection(int SelLight, TPoint p1, TPoint p2, TPoint p3, TPoint p4);

        void OnPaintDefectRect (TDrawTool Tool, COLORREF FColor);
        void OnPaintDefectPoint(TDrawTool Tool, COLORREF FColor);

        void OnInit      (int w, int h, BYTE *IAddr);
        void OnLoadTrain (bool IsLoad, CString Path);
        void OnLoad      (bool IsLoad, CString Path);
};

