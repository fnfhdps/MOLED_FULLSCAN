// LxEdge.h: interface for the CEdge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXEDGE_H__6F18C7B6_0076_4022_ADD9_F2C30DEE5F05__INCLUDED_)
#define AFX_LXEDGE_H__6F18C7B6_0076_4022_ADD9_F2C30DEE5F05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LxHeader.h"
//#include "LxMath.h"

enum TEdgeType { etBw, etWb, etBwOrWb, etBwb, etWbw };
enum TEdgeOrg  { eoWest, eoNorth, eoEast, eoSouth };
//---------------------------------------------------------------------------

class AFX_EXT_CLASS  CEdge : public CFilter {

    private :
    protected :

    public :
        // Architecture
        // ---------------------------------------------------------------------
		TFPoint WPoint;
		TFPoint EPoint;
		TFPoint NPoint;
		TFPoint SPoint;

        // Property
        // ---------------------------------------------------------------------
		int FMode; // FMode 0:OnExeOrientation FMode 1:OnExecute(TFPoint SP, TFPoint EP)
		int FOrientation;

        TFPoint FSP;
        TFPoint FEP;
        int FMaxEdges;
        int FEdgeType;
        int FMinAmplitude;
        int FMinArea;
        int FThickness;
        int FSmoothing;
		int FImgHeight;
		int FThreshold;

        // Result
        // ---------------------------------------------------------------------
        int FEdgePointCount;
        TFPoint *FEdgePoint;

		TFPoint FSPoint, FEPoint, FDistance;

    public :
        // Creat & Destory
        // ---------------------------------------------------------------------
        CEdge();
        ~CEdge();

        // Functions - Set
        // ---------------------------------------------------------------------
		void SetMode        (int n);
		void SetOrientation (int n);
        void SetStartPoint  (TFPoint SP); 
        void SetEndPoint    (TFPoint EP); 
        void SetMaxEdges    (int MaxEdges    );
        void SetEdgeType    (int EdgeType    );
        void SetMinAmplitude(int MinAmplitude);
        void SetMinArea     (int MinArea     );
        //void SetThreshold   (int Threshold);  
        void SetThreshold   (int Threshold   );
        void SetThickness   (int Thickness   );
        void SetSmoothing   (int Smoothing   );
		void SetImgHeight   (int ImgHeight   );
		
        // Functions - Get
        // ---------------------------------------------------------------------
		int GetMode           ();
		int GetOrientation    ();
        TFPoint GetStartPoint (); 
        TFPoint GetEndPoint   (); 
        int GetMaxEdges       (); 
        int GetEdgeType       (); 
        int GetMinAmplitude   (); 
        int GetMinArea        (); 
        int GetThreshold      (); 
        int GetThickness      (); 
        int GetSmoothing      (); 


        // Functions - Execute
        // ---------------------------------------------------------------------
        //int OnExecute();
        void OnExecute();
        int OnExecute(TFPoint SP, TFPoint EP);

        // Functions - Result
        // ---------------------------------------------------------------------
        int GetResultEdgePointCount() {return FEdgePointCount; }
        TFPoint GetResultEdgePoint(int n) {return FEdgePoint[n]; }
		int OnExeOrientation();
		int OnExeDistance();

        // Functions - Paint
        // ---------------------------------------------------------------------
        void OnPaint(TDrawTool Tool);
        void OnPaintEdgePoint(TDrawTool Tool);

        // Functions - Load & Save
        // ---------------------------------------------------------------------
        void OnLoad(bool IsLoad, CString Folder);
};

//---------------------------------------------------------------------------

class AFX_EXT_CLASS CRectEdge : public CEdge {

    private :

    protected :
        // Property
        //----------------------------------------------------------------------
        bool FEast, FWest, FSouth, FNorth;
        int FIncrement;

		// Value(RectEdge Algorithm 사용시 LineFitting적용에 필요한 변수)
		int FEdgePointCount;
		TFPoint *FEdgePoint;
		TFPoint FP0;
		TFPoint FP1;

    public :
        // Creat & Destory
        // ---------------------------------------------------------------------
        CRectEdge();
        ~CRectEdge();

		HMath Math;
		CTimer Time;


		// Euresys Class
		//----------------------------------------------------------------------
		EImageBW8 BinaryImg;

        // Functions - Set
        // ---------------------------------------------------------------------
        void SetEast (bool East ) { FEast  = East ; }
        void SetWest (bool West ) { FWest  = West ; }
        void SetSouth(bool South) { FSouth = South; }
        void SetNorth(bool North) { FNorth = North; }

        // Functions - Get
        // ---------------------------------------------------------------------
        bool GetEast () { return FEast ; }
        bool GetWest () { return FWest ; }
        bool GetSouth() { return FSouth; }
        bool GetNorth() { return FNorth; }

        // Functions - Execute
        // ---------------------------------------------------------------------
        int OnExecute();
        void OnExecute(bool l, bool t, bool r, bool b);

        // Functions - Paint
        // ---------------------------------------------------------------------
        void OnPaint(TDrawTool Tool);
        //void OnPaint(HWND Handle);

        // Functions - Load & Save
        // ---------------------------------------------------------------------
        //void OnLoad(bool IsLoad, AnsiString Folder);
        void OnLoad(bool IsLoad, CString Folder);

		//	to find front frame or end frame(Top2Down = true top -> down, Top2Down = false bottom -> up 
		// GrayLevel차이 검출
		int	FindVEdge(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromX, int toX, bool Top2Down  , int minEdgeCnt, int& Y);  
		int	FindHEdge(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromY, int toY, bool Left2Right, int minEdgeCnt, int& X);  

		// Binary차이 검출
		int	FindVEdge(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromX, int toX, bool Top2Down  , int minEdgeCnt, int& Y, int ThresHold=0, int Type=0); 
		int	FindHEdge(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromY, int toY, bool Left2Right, int minEdgeCnt, int& X, int ThresHold=0, int Type=0); 

		// EdgePoint±aAØ LineFitting°a°u¹° AAC￥°ª ¸®AI CO¼o(Binary Image±aAØ) 
		int	FindVEdgeFit(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromX, int toX, bool Top2Down  , int minEdgeCnt, int& Y, int ThresHold=0, int Type=0, int Offset=0); 
		int	FindHEdgeFit(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromY, int toY, bool Left2Right, int minEdgeCnt, int& X, int ThresHold=0, int Type=0, int Offset=0); 

		// Offset Au¿e EA Æ?A¤ Range ¹uA§¿¡¼­ EdgeSearch AøCaCO(Surface°E≫c¿¡¼­ CE¿aCO) =>Median Filter Au¿eCO 
		int	FindVEdgeFitRange(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromX, int toX, bool Top2Down  , int minEdgeCnt, int& Y, int ThresHold=0, int Type=0, int Offset=0, int Range=0); 
		int	FindHEdgeFitRange(BYTE* pImgPtr, int nFrmW, int nFrmH, int fromY, int toY, bool Left2Right, int minEdgeCnt, int& X, int ThresHold=0, int Type=0, int Offset=0, int Range=0); 

		// LineFitting Function 
		bool   __fastcall OnLineFitRANSAC_2(bool IsHor, TFPoint *Q1, int Count, double Deviation, int max_iter, int MinCount);
		// Value
		//int FIncrement;
};

#endif // !defined(AFX_LXEDGE_H__6F18C7B6_0076_4022_ADD9_F2C30DEE5F05__INCLUDED_)
