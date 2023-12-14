#include "stdafx.h"
#include "LxMatrixCode.h"


CMatrixCode::CMatrixCode()
{
	MatrixCodeReader = new EMatrixCodeReader();
	GImage           = NULL;
}

CMatrixCode::~CMatrixCode()
{
	delete MatrixCodeReader;
	MatrixCodeReader = NULL;
}

bool CMatrixCode::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CMatrixCode GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CMatrixCode GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CMatrixCode GImage Height Size"    )); return false; }

	IRoi->Detach();
	IRoi->Attach(GImage);

	FText = "";

	try
	{
		//MatrixCode = MatrixCodeReader->Read(GImage);
		MatrixCode = MatrixCodeReader->Read(*IRoi);
		std::string BarCodeText = MatrixCode.GetDecodedString();
		FText = BarCodeText.data();
		return true;
	}
	catch(const EException&)
	{
		// Insert exception handling code here
		return false;
	}

	return false;
}

void CMatrixCode::OnPaint(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
	HDC hdc = GetDC(Tool.Handle);

	MatrixCode.Draw(hdc, Tool.FHs, Tool.FVs);

	ReleaseDC(Tool.Handle, hdc);
}

void CMatrixCode::OnLoad(bool IsLoad, CString Folder)
{
	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

	if (Folder.Trim()=="") return;
	CString Path = Folder + "\\MatrixCode.ini";
	if (Path=="") return;

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}
	// Roi Load & Save
	// -------------------------------------------------------------------------
	CString IRoiPath = Folder + "\\MatrixCodeIRoi.ini";
	OnLoadRoi(IsLoad, IRoiPath, IRoi);

}
