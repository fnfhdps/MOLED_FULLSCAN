// LxType.cpp: implementation of the TPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LxType.h"
#include "LxGdi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void JRect::OnPaint(TDrawTool Tool, int lbStyle, int lbHatch)
{
    if (Tool.Handle==NULL) return;
	
	HGdi *Gdi = new HGdi(Tool.Handle);
	
	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen  .Color = Tool.Color;
	Gdi->CBrush.Color = Tool.Color;
	Gdi->CBrush.Style = lbStyle;
	Gdi->CBrush.Hatch = lbHatch;
	
	if (lt.x || rt.x || lb.x || rb.x) {
		Gdi->OnLine(lt.x, lt.y, rt.x, rt.y);
		Gdi->OnLine(rt.x, rt.y, rb.x, rb.y);
		Gdi->OnLine(rb.x, rb.y, lb.x, lb.y);
		Gdi->OnLine(lb.x, lb.y, lt.x, lt.y);
	}
	else Gdi->OnRect(0, l, t, r, b, 0);
	
	delete Gdi;
}