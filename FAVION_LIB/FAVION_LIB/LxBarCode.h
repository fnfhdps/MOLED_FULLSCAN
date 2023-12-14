#pragma once
#include "LxHeader.h"
//------------------------------------------------
class AFX_EXT_CLASS CBarCode :   public CFilter
{
    private :
        // Architecture
        // ---------------------------------------------------------------------
        EBarCode *BarCode;

    protected :
        // Property
        // ---------------------------------------------------------------------
        int FSymbologies_Additional;
        int FSymbologies_Standard;

        // Result
        // ---------------------------------------------------------------------
        CString FText;
        CString FId  ;

    public :
        // Creast & Destory
        // ---------------------------------------------------------------------
        CBarCode();
        ~CBarCode();

        // Functions - Set
        // ---------------------------------------------------------------------
        void SetSymbologies_Additional(int Symbologies_Additional) {FSymbologies_Additional = Symbologies_Additional;}
        void SetSymbologies_Standard  (int Symbologies_Standard  ) {FSymbologies_Standard   = Symbologies_Standard  ;}

        // Functions - Get
        // ---------------------------------------------------------------------
        int GetSymbologies_Additional() {return FSymbologies_Additional;}
        int GetSymbologies_Standard  () {return FSymbologies_Standard  ;}

        // Functions - Execute
        // ---------------------------------------------------------------------
        bool OnExecute();

        // Functions - Execute
        // ---------------------------------------------------------------------
        CString GetResultText() { return FText; }
        CString GetResultId  () { return CString(FId); }

        // Functions - Paint
        // ---------------------------------------------------------------------
        void OnPaint(TDrawTool Tool);

        // Functions - Load & Save
        // ---------------------------------------------------------------------
        void OnLoad(bool IsLoad, CString Folder);
};

