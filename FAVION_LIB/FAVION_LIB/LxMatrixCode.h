#pragma once
#include "LxHeader.h"
//----------------------------------------------------------------
class AFX_EXT_CLASS  CMatrixCode :  public CFilter 
{
private :
	// Architecture
	// ---------------------------------------------------------------------
	EMatrixCodeReader *MatrixCodeReader; // EMatrixCodeReader instance
	EMatrixCode MatrixCode; // EMatrixCode instance

protected :
	// Property
	// ---------------------------------------------------------------------

	// Result
	// ---------------------------------------------------------------------
	CString FText;

public :
	// Functions - Creat & Destory
	// ---------------------------------------------------------------------
	CMatrixCode();
	~CMatrixCode();

	// Functions - Execute
	// ---------------------------------------------------------------------
	bool OnExecute();

	// Functions - Result
	// ---------------------------------------------------------------------
	CString GetResultText() { return FText;}

	// Functions - Paint
	// ---------------------------------------------------------------------
	void OnPaint(TDrawTool Tool);

	// Functions - Load & Save
	// ---------------------------------------------------------------------
	void OnLoad(bool IsLoad, CString Folder);
};

