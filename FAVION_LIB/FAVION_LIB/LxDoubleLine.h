#pragma once
#include "LxLine.h"
#include "LxTrace.h"

class AFX_EXT_CLASS CDoubleLine : public CSingleLine 
{
    public:
		CDoubleLine ();
		~CDoubleLine();
    private :
        // Architecture
        // ---------------------------------------------------------------------

    protected :
        // Architecture
        // ---------------------------------------------------------------------

        // Property
        // ---------------------------------------------------------------------
        int FFstOrientation;
        int FSndOrientation;

    public :
        // Result
        // ---------------------------------------------------------------------
        struct TLine { TFPoint FP0; TFPoint FP1; } Line[2];
        TFPoint FIPoint;
        float FDistance;
        TFPoint FL0CP; // 첫번째 선위 두점의 중간지점
        TFPoint FL1CP; // 두번째 선위 두점의 중간지점
        TFPoint FL0MP; // 두번째 선의 중간지점과의 첫번째 선의 교차점
        TFPoint FL1MP; // 첫번째 선의 중간지점과의 두번째 선의 교차점

    public :
        // Architecture
        // ---------------------------------------------------------------------

        // Creat & Destory
        // ---------------------------------------------------------------------

        // Functions - Set
        // ---------------------------------------------------------------------
        void SetFstOrientation(TOrientaion FstOrientation) { FFstOrientation = FstOrientation; }
        void SetSndOrientation(TOrientaion SndOrientation) { FSndOrientation = SndOrientation; }

        // Functions - Get
        // ---------------------------------------------------------------------
        TOrientaion GetFstOrientation() { return (TOrientaion)FFstOrientation; }
        TOrientaion GetSndOrientation() { return (TOrientaion)FSndOrientation; }

        // Functions - Execute
        // ---------------------------------------------------------------------
        int OnExecute();
        int OnExecute(TOrientaion FstOrientation, TOrientaion SndOrientation);

        // Functions - Result
        // ---------------------------------------------------------------------
        TFPoint GetResultIntersectPoint() { return FIPoint; }
        TFPoint GetResultLinePointP0(int n) { return Line[n].FP0; }
        TFPoint GetResultLinePointP1(int n) { return Line[n].FP1; }
        float GetResultDistance() { return FDistance; }

        // Functions - Paint
        // ---------------------------------------------------------------------
        void OnPaint(TDrawTool Tool);

        // Functions - Load & Save
        // ---------------------------------------------------------------------
        void OnLoad(bool IsLoad, CString Folder);

        float GetLength(TFPoint p1, TFPoint p2);
};
//--------------------------------------------------------------
/*
class CCircleArray : public CFilter {
    #define MAX_CIRCLE 10
    private :
        // Architecture
        // ---------------------------------------------------------------------
        EWorldShape *WorldShape; // EWorldShape instance
        ECircleGauge *CircleGauge; // ECircleGauge instance
        ECircle Circle; // ECircle instance

    protected :
        // Architecture
        // ---------------------------------------------------------------------

        // Property
        // ---------------------------------------------------------------------
        int FCount;
        float FTolerance;

        // Result
        // ---------------------------------------------------------------------
        TFPoint FCP[MAX_CIRCLE];
        float FDiameter[MAX_CIRCLE];

    public :
        // Architecture
        // ---------------------------------------------------------------------
        EROIBW8 *ARoi[MAX_CIRCLE];

        // Creat & Destory
        // ---------------------------------------------------------------------
        CCircleArray();
        ~CCircleArray();

        // Functions - Set
        // ---------------------------------------------------------------------
        void SetCount(int Count) { FCount = Count; }
        void SetTolerance(float Tolerance) { FTolerance = Tolerance; }

        // Functions - Get
        // ---------------------------------------------------------------------
        EWorldShape *GetWorldShape() { return WorldShape; }
        int GetCount() { return FCount; }
        float GetTolerance() { return FTolerance; }

        // Functions - Execute
        // ---------------------------------------------------------------------
        int OnExecute();

        // Functions - Result
        // ---------------------------------------------------------------------
        TFPoint GetResultCenterPoint(int n) { return FCP[n]; }
        float GetResultDiameter(int n) { return FDiameter[n]; }

        // Functions - Paint
        // ---------------------------------------------------------------------
        void OnPaint(HWND Handle, TColor RColor=clRed);

        // Functions - Load & Save
        // ---------------------------------------------------------------------
        void OnLoad(bool IsLoad, AnsiString Folder);

};
*/

