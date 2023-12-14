#include "stdafx.h"
#include "LxBarCode.h"


CBarCode::CBarCode(void)
{
	BarCode = new EBarCode();

    FSymbologies_Standard = true;
    FSymbologies_Additional = true;
}
CBarCode::~CBarCode(void)
{
	delete BarCode;
    BarCode = NULL;
}
bool CBarCode::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CBarCode GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CBarCode GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CBarCode GImage Height Size"    )); return false; }

    IRoi->Detach();
    IRoi->Attach(GImage);

    try
    {
        if (FSymbologies_Additional!=0) BarCode->SetAdditionalSymbologies(ESymbologies_Additional);
        if (FSymbologies_Additional==0) BarCode->SetAdditionalSymbologies(0);
        if (FSymbologies_Standard  !=0) BarCode->SetStandardSymbologies(ESymbologies_Standard);
        if (FSymbologies_Standard  ==0) BarCode->SetStandardSymbologies(0);

        //BarCode.SetKnownLocation(true);
        std::string BarCodeText, BarCodeId;
        BarCodeText = BarCode->Read(GImage);
        BarCodeId   = BarCode->GetDecodedSymbology(0);
        FText = BarCodeText.data();
        FId   = BarCodeId  .data();
        return true;
    }
    catch(const EException&)
    {
      // Insert exception handling code here
    }

    return false;
}

void CBarCode::OnPaint(TDrawTool Tool)
{
	if(Tool.Handle =NULL) return;
    HDC hdc = GetDC(Tool.Handle);

    BarCode->SetZoom(Tool.FHs, Tool.FVs);
    BarCode->Draw(hdc);
    BarCode->Draw(hdc,EDrawingMode_Actual);

    ReleaseDC(Tool.Handle, hdc);
}

void CBarCode::OnLoad(bool IsLoad, CString Folder)
{
	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

    if (Folder.Trim()=="") return;
    CString Path = Folder + "\\BarCode.ini";
    if (Path=="") return;

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

    // Roi Load & Save
    // -------------------------------------------------------------------------
    CString IRoiPath = Folder + "\\BarCodeIRoi.ini";
    OnLoadRoi(IsLoad, IRoiPath, IRoi);
}

