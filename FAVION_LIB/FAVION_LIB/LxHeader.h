// LxHeader.h: interface for the CHeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXHEADER_H__02E014D9_7FCA_4473_8102_90F40E07D306__INCLUDED_)
#define AFX_LXHEADER_H__02E014D9_7FCA_4473_8102_90F40E07D306__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//---------------------------------------------
//define 정의 
//---------------------------------------------
#define MAX_DEFECT_POINT    200
#define LINESCAN_CAMERA_MODE  0
//----------------------------------------------
//LxLineDefct Define 정의
#define MAX_DEFECT_POINT         200
#define MAX_CORNER_POINT         1500
#define MAX_LINE_POINT           20
#define MAX_PRELINE_POINT        4
#define MAX_BMLINE_COUNT         50
#define MAX_CIRCLE_EDGEPOINT     3500
#define MAX_BTM_COUNT            5
#define MAX_BTM_LINE_POINT       10
#define MAX_TOP_LINE_POINT       4 
#define MAX_FIND_MARK_COUNT      27
#define MAX_POLYGON_DEFECT_POINT 1000
#define MAX_POLYGON_DEFECT_COUNT 100 
#define MAX_ASIDE_GRIND_POINT    19
#define MAX_ALL_EDGE_COUNT       100000
#define MAX_GRIND_MARK_COUNT     23
#define MAX_HOLE_GRIND_MARK_COUNT 8
#define MAX_GRINDMARK_ID          6
#define MAX_ROI_ID                3
#define MAX_SIDE_COUNT            4
#define MAX_CUT_GRIND_MARK_ROI    54  //FullScan?? ROI?? 1?? Cutting ??? ??? ???? ?? ??(?? ??? ??TCG? ???)
#define MAX_CELL_SIZE_POS_COUNT   3   //EdgeScan?? Optical Scale ???? 3Point(??? ???? ???)
#define MAX_LINEFITTING_POINT     1   //FullScan?? 1Frame?? LineFitting Count (LAMI???? ???)
#define MAX_FILM_POINT_COUNT      30  //FullScan?? 1Frame?? Film LineFitting(2?)?? ?? Buffer Count(LAMI ???? ???)
#define MAX_FILM_HANDLE_POINT     2   //FullScan?? Film Handle Total Count(LAMI ???? ???)
#define MAX_MASKING_COUNT         20  //Area Camera ?? Max Masking Count 
#define MAX_CROSS_POINT_COUNT     4   //Cross Point Count 
#define MAX_MARK_FIND_COUNT       10
#define MAX_PRINT_MARK_COUNT      10
#define MAX_GRINDMARK_COUNT       23
#define MAX_HOLE_GRIND_COUNT      4
#define MAX_REAL_FRAME_COUNT      20
#define MAX_CUT_GRIND_ROI         54
#define MAX_GRINDMARK_ID          6
#define MAX_DEFECT_ROI_COUNT      3
#define MAX_GRIND_ROI_COUNT       5


//----------------------------------------------
#include "Open_eVision_1_2.h"
#include "multicam.h"

using namespace Euresys::Open_eVision_1_2;

#include "LxMath.h"
#include "LxGdi.h"
#include "LxFileUnit.h"
#include "LxTimer.h"
#include "LxType.h"
#include "LxFilter.h"

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <typeinfo>
#include "math.h"
#include "stdafx.h"
//---------------------------------------------------------------------------

extern CString ToString(int    n) ;
extern CString ToString(float  n) ;
extern CString ToString(double n) ;
extern LPSTR ToChar(CString Txt) ;
extern char* IntToHex(int val, int n) ;
extern double GetCount();

#endif // !defined(AFX_LXHEADER_H__02E014D9_7FCA_4473_8102_90F40E07D306__INCLUDED_)
