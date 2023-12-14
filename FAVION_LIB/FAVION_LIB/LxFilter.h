// LxFilter.h: interface for the CSeed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXFILTER_H__5BDA8719_119E_4447_B1C8_B38E41E3B89A__INCLUDED_)
#define AFX_LXFILTER_H__5BDA8719_119E_4447_B1C8_B38E41E3B89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LxHeader.h"

#define IElement(x, y)  *((BYTE *)IAddress + x + BufferWidth * y)  
#define TElement(x, y)  *((BYTE *)TAddress + x + BufferWidth * y)  
#define MElement(x, y)  *((BYTE *)MAddress + x + BufferWidth * y)
//---------------------------------------------------------------------------
extern bool IsEvtPrcessMessage;
void SetProcessMessage(CString Msg);
CString GetProcessMessage();
//---------------------------------------------------------------------------

class  CSeed  
{
public :
	// Architecture
	// ---------------------------------------------------------------------
    EImageBW8 *OImage;
	EImageBW8 *GImage;
	EImageBW8 *PImage;
    EROIBW8 *IRoi;
    EROIBW8 *TRoi;
    EROIBW8 *FRoi;
    EROIBW8 *MRoi;
	int BufferWidth ;
	int BufferHeight;
	void * IAddress;
	void * TAddress;
	void * MAddress;
	// Input
	// ---------------------------------------------------------------------

// 	HMath Math;
	// Property
	// ---------------------------------------------------------------------
	
	// Result
	// ---------------------------------------------------------------------
	
public :
	// Creat & Destory
	// ---------------------------------------------------------------------
	CSeed();
	~CSeed();
	
	// Functions
	// ---------------------------------------------------------------------
	
	TFPoint GetIntersectPointWithImageLeft  (TFPoint P0, TFPoint P1);
	TFPoint GetIntersectPointWithImageTop   (TFPoint P0, TFPoint P1);
	TFPoint GetIntersectPointWithImageRight (TFPoint P0, TFPoint P1);
	TFPoint GetIntersectPointWithImageBottom(TFPoint P0, TFPoint P1);
    void OnLoadRoi(bool IsLoad, CString Path, EROIBW8 *lpRoi);
};
// enum
// ---------------------------------------------------------------------
enum TFilterType   {ftNone, ftErodeDisk,ftDilateDisk, ftMedian, ftCloseDisk, ftOpenDisk, ftConvolGaussian,ftConvolLowpass1, ftConvolHighpass1, ftBinary, ftNotUse};
// enum TLineScanType {LineScan_4K,LineScan_6K,LineScan_8K,LineScan_16K,LineScan_23K};
class  CFilter : public CSeed {
	
public :
	// Architecture
	// ---------------------------------------------------------------------
	
	// Input
	// ---------------------------------------------------------------------
	
	// Property
	// ---------------------------------------------------------------------
	
	// Result
	// ---------------------------------------------------------------------
	
public :
	// Creat & Destory
	// ---------------------------------------------------------------------
	CFilter() {}
	~CFilter() {}
	
// 	HMath Math;
	// Functions
	// ---------------------------------------------------------------------
//     bool OnExecute(TFilterType FilterType, EImageBW8 *Org, EImageBW8 *New);
	bool OnFilter(TFilterType FilterType, EImageBW8 *Org, EImageBW8 *New);
	bool OnFilter(TFilterType FilterType, EROIBW8   *Org, EROIBW8   *New);
};

#endif // !defined(AFX_LXFILTER_H__5BDA8719_119E_4447_B1C8_B38E41E3B89A__INCLUDED_)
