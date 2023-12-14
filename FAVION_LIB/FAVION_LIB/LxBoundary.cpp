#include "stdafx.h"
#include "LxBoundary.h"
#include "LxHeader.h"
#include "math.h"

//---------------------------------------------------------
// #define IElement(x, y)  *((BYTE *)IAddress + x + IWidth * y)
// #define TElement(x, y)  *((BYTE *)TAddress + x + IWidth * y)
// #define MElement(x, y)  *((BYTE *)MAddress + x + IWidth * y)
//----------------------------------------------------------
int IpPixelCount;
TPoint IpPixel[MAX_INSPECT_PIXEL];

JRect IPRECT;

//--------------------------------------------------------------------------
//TBlob
//---------------------------------------------------------------------------
CBoundary::TBlob::TBlob()
{
    TPoint T(0,0);
    lPoint = tPoint = rPoint = bPoint = T;
    n = 0;
    Quantity2 = Quantity8 = Tag = 0;
    Rect.Clear();
    Point=NULL;
    Pixel=NULL;
}

void CBoundary::TBlob::OnClear()
{
    TPoint T(0,0);
    lPoint = tPoint = rPoint = bPoint = T;
    n = 0;
    Quantity2 = Quantity8 = Tag = 0;
    Rect.Clear();
    if (Point!=NULL) {delete[] Point; Point = NULL;}
    if (Pixel!=NULL) {delete[] Pixel; Pixel = NULL;}
}

void CBoundary::TBlob::OnCopy(TBlob *lpBlob)
{
    OnClear();

    if (lpBlob->Pixel!=NULL) {Pixel = new BYTE  [n]; memcpy(lpBlob->Pixel, Pixel, lpBlob->n);}
    if (lpBlob->Point!=NULL) {Point = new TPoint[n]; memcpy(lpBlob->Point, Point, lpBlob->n);}
    n        = lpBlob->n;
    Rect     = lpBlob->Rect;
    Quantity2= lpBlob->Quantity2;
    Quantity8= lpBlob->Quantity8;
    Tag      = lpBlob->Tag;
}

void CBoundary::TBlob::OnLoad(bool IsLoad, CString Path)
{
/*
     if (IsLoad) OnClear();

     FILE *fp;

     if (IsLoad) fp = fopen(Path.c_str(),"rb");
     else        fp = fopen(Path.c_str(),"wb");
     if (fp == NULL) return;

     if (IsLoad) {
         fread (&Rect.l, 1, sizeof(Rect.l), fp);
         fread (&Rect.t, 1, sizeof(Rect.t), fp);
         fread (&Rect.r, 1, sizeof(Rect.r), fp);
         fread (&Rect.b, 1, sizeof(Rect.b), fp);
         fread (&n     , 1, sizeof(n     ), fp);
         fread (&Tag   , 1, sizeof(Tag   ), fp);
     }

     else {
         fwrite(&Rect.l, 1, sizeof(Rect.l), fp);
         fwrite(&Rect.t, 1, sizeof(Rect.t), fp);
         fwrite(&Rect.r, 1, sizeof(Rect.r), fp);
         fwrite(&Rect.b, 1, sizeof(Rect.b), fp);
         fwrite(&n     , 1, sizeof(n     ), fp);
         fwrite(&Tag   , 1, sizeof(Tag   ), fp);
     }

     int w = Rect.r - Rect.l;
     int h = Rect.b - Rect.t;

     if (IsLoad) {
         Pixel = new BYTE[w*h];
         fread (&Pixel, 1, sizeof(w*h), fp);
     }
     else {
         fwrite(&Pixel, 1, sizeof(w*h), fp);
     }

     fclose(fp);
*/
}
//---------------------------------------------------------------------------

CBoundary::CBoundary()
{
	BlobCount = 0;
	Blob = NULL;
	IAddress = NULL;
	MAddress = NULL;
	TAddress = NULL;
	GImage   = NULL;

	IRoi  = new EROIBW8();

	IgBlobCount = 0;
	IgBlobMargin = 1;
}
CBoundary::~CBoundary()
{
}
CBoundary::TTrPa::TTrPa()
{
   IgSort        =  true;
   SMode         =     0;
   IgInSide      =  true;
   MaxBbCnt      =   500;
   MinPxCnt      =    20;
   MaxPxCnt      = 10000;
   OffsetX       =     0;
   StepX         =     1;
   OffsetY       =     0;
   StepY         =     1;
   MgCount       =    50;
   xMgPxCnt      =    10;
   yMgPxCnt      =    60;
   MgTgHorLength =    20;
   MgTgVerLength =    20;
   HiThHold      =     0;
   LoThHold      =   100;
   MinHorLength  =     0;
   MaxHorLength  =  1000;
   MinVerLength  =     0;
   MaxVerLength  =  1000;
   MinPxBlob     =     0;
   MinHorPitch   =     0;
   MinVerPitch   =     0;
   IgBlobFarX    =     0;
   IgBlobFarY    =     0;
   IsQuantity2   =     0;
   IsQuantity8   =     0;

   OpCircleFit   = true;
   OpMinMaxRadious = true;

   UseIgnoreBlob = false;
}
CBoundary::TTrPa::~TTrPa()
{
}

TSMode SMODE;
int BOnSortFunction(const void *b1, const void *b2)
{
    const int RIGHT_POS =  1;
    const int LEFT_POS  = -1;
    const int ORIG_POS  =  0;

    if (b1==NULL) return ORIG_POS;
    if (b2==NULL) return ORIG_POS;

    int r1 = ((CBoundary::TBlob *)b1)->Rect.Point().y - ((CBoundary::TBlob *)b2)->Rect.Point().y;
    int r2 = ((CBoundary::TBlob *)b1)->Rect.Point().x - ((CBoundary::TBlob *)b2)->Rect.Point().x;

    bool IsYLine = ((CBoundary::TBlob *)b2)->Rect.t <= ((CBoundary::TBlob *)b1)->Rect.b && ((CBoundary::TBlob *)b2)->Rect.b >= ((CBoundary::TBlob *)b1)->Rect.t;
    bool IsXLine = ((CBoundary::TBlob *)b2)->Rect.l <= ((CBoundary::TBlob *)b1)->Rect.r && ((CBoundary::TBlob *)b2)->Rect.r >= ((CBoundary::TBlob *)b1)->Rect.l;


    int B1y = ((CBoundary::TBlob *)b1)->Rect.Point().y;
    int B2y = ((CBoundary::TBlob *)b2)->Rect.Point().y;
    int yLg = ((CBoundary::TBlob *)b1)->Rect.GetHeight()+5;
    bool IsM_YLine = abs(B2y - B1y) < ( yLg );

    bool IsM_XLine = ((CBoundary::TBlob *)b2)->Rect.l <= ((CBoundary::TBlob *)b1)->Rect.r && ((CBoundary::TBlob *)b2)->Rect.r >= ((CBoundary::TBlob *)b1)->Rect.l;

    int  M_yMg = 0;
    bool M_IsYLine = 0;
    bool M_IsXLine = 0;

    int BODY_DIR = 0;
    int BLOB_DIR = 0;
    int DIR = 0;

    switch (SMODE) {
        default         : break;

        case sbRightBtm :
                          if ( IsYLine && r2>0) return RIGHT_POS;
                          if (!IsYLine && r1>0) return RIGHT_POS;
                          break;

        case sbLeftBtm  : if ( IsYLine && r2<0) return RIGHT_POS;
                          if (!IsYLine && r1>0) return RIGHT_POS;
                          break;

        case sbRightTop : if ( IsYLine && r2>0) return RIGHT_POS;
                          if (!IsYLine && r1<0) return RIGHT_POS;
                          break;

        case sbLeftTop  : if ( IsYLine && r2<0) return RIGHT_POS;
                          if (!IsYLine && r1<0) return RIGHT_POS;
                          break;

        case sbBtmRight : if ( IsXLine && r1>0) return RIGHT_POS;
                          if (!IsXLine && r2>0) return RIGHT_POS;
                          break;

        case sbBtmLeft  : if ( IsXLine && r1>0) return RIGHT_POS;
                          if (!IsXLine && r2<0) return RIGHT_POS;
                          break;

        case sbTopRight : if ( IsXLine && r1<0) return RIGHT_POS;
                          if (!IsXLine && r2>0) return RIGHT_POS;
                          break;

        case sbTopLeft  : if ( IsXLine && r1<0) return RIGHT_POS;
                          if (!IsXLine && r2<0) return RIGHT_POS;
                          break;

        case sbMatrix   :
                          if ( IsM_YLine && r2>0) return RIGHT_POS;
                          if (!IsM_YLine && r1>0) return RIGHT_POS;
                          break;

    }
    return LEFT_POS;
}

void CBoundary::OnSortBlob(int StartId)
{
    if (TrPa.IgSort) return;

    SMODE = (TSMode)TrPa.SMode;
    TBlob *lpBlob = &Blob[StartId];
    qsort((void *)lpBlob, BlobCount-StartId, sizeof(Blob[0]), BOnSortFunction);
}

void CBoundary::OnSortBlob()
{
    if (TrPa.IgSort) return;

    SMODE = (TSMode)TrPa.SMode;
    qsort((void *)Blob, BlobCount, sizeof(Blob[0]), BOnSortFunction);
}

int CBoundary::GetMeanThHold(JRect Rect)
{
    #define MAX_GRAY 256

    double Hist[MAX_GRAY];
    memset(Hist, 0, sizeof(Hist));

    int Px = 0;
    for (register int y=Rect.t; y<Rect.b; y++) {
    for (register int x=Rect.l; x<Rect.r; x++) {
        if ((x-1)<=0      || x>=IWidth || y<=0      || y>=IHeight) continue;
        if ((x-1)<=Rect.l || x>=Rect.r       || y<=Rect.t || y>=Rect.b       ) continue;
        Px = IElement(x, y);
        if (Px==0) continue;
        Hist[Px]++;
    }}

    int Count = 0;
    int Sum = 0;
    for (int i=0; i<MAX_GRAY; i++) {
        Count += Hist[i];
        Sum += (i * Hist[i]);
    }

    if (Count<1) return 0;

    int ThHold = Sum/Count;
    return ThHold;
}

int CBoundary::GetThresHold(JRect Rect)
{
    #define MAX_GRAY 256

    double hist[MAX_GRAY], chist[MAX_GRAY], cxhist[MAX_GRAY];
    memset(hist  , 0, sizeof(hist  ));
    memset(chist , 0, sizeof(chist ));
    memset(cxhist, 0, sizeof(cxhist));
    int ntot = Rect.GetDemension();

    if (ntot<1) {
        //ShowMessage("int CBoundary::OnFindThresHold(JRect Rect)");
        return 0;
    }

    for (int y=Rect.t; y<Rect.b; y++) {
    for (int x=Rect.l; x<Rect.r; x++) {
        if ((x-1)<=0      || x>=IWidth || y<=0      || y>=IHeight) continue;
        if ((x-1)<=Rect.l || x>=Rect.r       || y<=Rect.t || y>=Rect.b       ) continue;

        hist[IElement(x, y)]++;
   }}

    for (int i=0; i<MAX_GRAY; i++) hist[i]/=(double)ntot;

    chist[0] = hist[0];
    cxhist[0] = 0;
    for (int i=0; i<MAX_GRAY; i++) {
        chist[i] = chist[i-1] + hist[i];
        cxhist[i] = cxhist[i-1] + (double)i*hist[i];
    }
    double cost_max = 0;
    int thresh = 0;
    double m = cxhist[255];

    for (int i=0; i<MAX_GRAY; i++) {
        if (chist[i]==0) continue;
        double q1 = chist[i];
        double q2 = 1 - q1;
        if (q2==0) break;
        double m1 = cxhist[i]/q1;
        double m2 = (m-cxhist[i])/q2;
        double cost = q1*q2*(m1-m2)*(m1-m2);
        if (cost_max < cost) {
            cost_max = cost;
            thresh = i;
        }
    }
    return thresh;
}

int CBoundary::GetMountThHold(JRect Rect)
{
    int Histo[256];
    memset(Histo, 0, sizeof(Histo));
    int Px = 0;

    register int Width   = IWidth ;
    register int Height  = IHeight;

    for (register int y=Rect.t; y<Rect.b; y++) {
    for (register int x=Rect.l; x<Rect.r; x++) {
        if ((x-1)<=0      || x>=IWidth || y<=0      || y>=IHeight) continue;
        if ((x-1)<=Rect.l || x>=Rect.r       || y<=Rect.t || y>=Rect.b       ) continue;

        Px = IElement(x, y);
        if (Px==0) continue;
        Histo[Px]++;
    }}

    //--------------------------------------------------------------------------
    // START : Avg Threshold
    int Count = 0;
    int Sum = 0;
    for (int i=0; i<256; i++) {
        Count += Histo[i];
        Sum += (i * Histo[i]);
    }

    int AvgHisto = 0;
    if (Count) {
        AvgHisto = Sum/Count;
    }
    // END : Avg Threshold
    //--------------------------------------------------------------------------

    int MaxCount = 500;
    int Mount = 0;

    //Mount
    //for (register int n=AvgHisto+5; n<256; n++) {
    for (register int n=255; n>0; n--) {
        if (Histo[n]>=MaxCount) {
            if (n>10) Mount = n-10;
            break;
        }
    }

    return Mount;
}
/*
CBoundary::THisto CBoundary::OnRectHistoAnalysis(TPoint p1, TPoint p2, TPoint p3, TPoint p4)
{

    int Histo[256];
    memset(Histo, 0, sizeof(Histo));
    int Px = 0;

    register int Width   = IWidth ;
    register int Height  = IHeight;
    JRect rect;
    rect.Clear();
    rect.l = p1.x < p3.x ? p1.x : p3.x;
    rect.t = p1.y < p2.y ? p1.y : p2.y;
    rect.r = p2.x > p4.x ? p2.x : p4.x;
    rect.b = p3.y > p4.y ? p3.y : p4.y;

    register float aTop   = Math.a(p1, p2);
    register float bTop   = Math.b(p1, p2);
    register float aRight = Math.a(p2, p4);
    register float bRight = Math.b(p2, p4);
    register float aBtm   = Math.a(p3, p4);
    register float bBtm   = Math.b(p3, p4);
    register float aLeft  = Math.a(p1, p3);
    register float bLeft  = Math.b(p1, p3);

    int staX, endX;

    //--------------------------------------------------------------------------
    for (register int y=rect.t; y<rect.b; y++) {
        if (p1.y > p2.y){
            staX = (y < p1.y) ? aTop  ==0 ? p1.x : (y-bTop  )/aTop   : aLeft ==0 ? p1.x : (y-bLeft )/aLeft ;
            endX = (y < p4.y) ? aRight==0 ? p4.x : (y-bRight)/aRight : aBtm  ==0 ? p4.x : (y-bBtm  )/aBtm  ;
        }
        else {
            staX = (y < p3.y) ? aLeft ==0 ? p1.x : (y-bLeft )/aLeft  : aBtm  ==0 ? p1.x : (y-bBtm  )/aBtm  ;
            endX = (y < p2.y) ? aTop  ==0 ? p2.x : (y-bTop  )/aTop   : aRight==0 ? p2.x : (y-bRight)/aRight;
        }
    for (register int x=staX; x<endX; x++) {
        if ((x-1)<=0      || x>=IWidth || y<=0      || y>=IHeight) continue;
        if ((x-1)<=rect.l || x>=rect.r       || y<=rect.t || y>=rect.b       ) continue;

        if (MElement(x, y))
            Histo[IElement(x, y)]++;
    }}

    //--------------------------------------------------------------------------
    // START : Avg Threshold
    int Count = 0;
    int Sum = 0;
    for (int i=0; i<256; i++) {
        Count += Histo[i];
        Sum += (i * Histo[i]);
    }

    int AvgHisto = 0;
    if (Count) {
        AvgHisto = Sum/Count;
    }
    // END : Avg Threshold
    //--------------------------------------------------------------------------

    int Mount  = AvgHisto;
    int Valley = AvgHisto;

    //Mount
    for (register int n=AvgHisto+5; n<256; n++) {
        if (Histo[n]>=3) { Mount = n; }
    }

    //Valley
    for (register int n=AvgHisto-5; n>0; n--) {
        if (Histo[n]>=3) { Valley = n; }
    }

    THisto Histogram;
    Histogram.Avg = AvgHisto;
    Histogram.Mount = Mount;
    Histogram.Valley = Valley;

    return Histogram;

}
*/
/*
CBoundary::THisto CBoundary::OnBlobHistoAnalysis(JRect Rect, bool IsOnlyDefect)
{

    int Histo[256];
    memset(Histo, 0, sizeof(Histo));
    int Px = 0;

    register int Width   = IWidth ;
    register int Height  = IHeight;

    for (register int y=Rect.t; y<Rect.b; y++) {
    for (register int x=Rect.l; x<Rect.r; x++) {
        if ((x-1)<=0      || x>=IWidth || y<=0      || y>=IHeight) continue;
        if ((x-1)<=Rect.l || x>=Rect.r       || y<=Rect.t || y>=Rect.b       ) continue;

        if (MElement(x, y))
            Px = IElement(x, y);
        else Px = 0;

        if (IsOnlyDefect) {
            if      (TrPa.HiThHold && Px >= TrPa.HiThHold) continue;
            else if (TrPa.LoThHold && Px <= TrPa.LoThHold) continue;
        }

        Histo[Px]++;
    }}

    //--------------------------------------------------------------------------
    // START : Avg Threshold
    int Count = 0;
    int Sum = 0;
    for (int i=0; i<256; i++) {
        Count += Histo[i];
        Sum += (i * Histo[i]);
    }

    int AvgHisto = 0;
    if (Count) {
        AvgHisto = Sum/Count;
    }
    // END : Avg Threshold
    //--------------------------------------------------------------------------

    int Mount  = AvgHisto;
    int Valley = AvgHisto;

    //Mount
    for (register int n=AvgHisto+5; n<256; n++) {
        if (Histo[n]>=3) { Mount = n; }
    }

    //Valley
    for (register int n=AvgHisto-5; n>0; n--) {
        if (Histo[n]>=3) { Valley = n; }
    }

    THisto Histogram;
    Histogram.Avg = AvgHisto;
    Histogram.Mount = Mount;
    Histogram.Valley = Valley;

    return Histogram;

}
*/

int CBoundary::GetPixelCount(int BlobId, bool IsBright, int ThHold, TFPoint C, float r)
{
    int Count = GetPixelCount(&Blob[BlobId].Rect, IsBright, ThHold, C, r);
    return Count;
}

int CBoundary::GetPixelCount(JRect *lpRect, bool IsBright, int ThHold, TFPoint C, float r)
{
    if (lpRect==NULL) return 0;

    int Count = 0; float Distance = 0.0f; TFPoint P1;
    for (register int n=lpRect->t; n<lpRect->b; n++) {
    for (register int m=lpRect->l; m<lpRect->r; m++) {
        P1.x = m, P1.y = n;
        Distance = sqrt(pow(fabs(C.x-P1.x), 2)+pow(fabs(C.y-P1.y), 2));
        if(Distance>r) continue;

        if      ( IsBright && IElement(m, n) > ThHold) Count++;
        else if (!IsBright && IElement(m, n) < ThHold) Count++;
    }}
    return Count;
}

int CBoundary::GetAvgPixelCount(int BlobId, bool IsBright, int Offset, TFPoint C, int r, bool IsValue)
{
    int PixelCount = GetAvgPixelCount(&Blob[BlobId].Rect, IsBright, Offset, C, r, IsValue);
    return PixelCount;
}

int CBoundary::GetAvgPixelCount(JRect *lpRect, bool IsBright,int Offset, TFPoint C, int r, bool IsValue)
{
    if (lpRect==NULL) return 0;

    int Count = 0, SumPixel = 0, AvgPixel=0;
    int Distance = 0; TFPoint P1;
    for (register int n=lpRect->t; n<lpRect->b; n++) {
    for (register int m=lpRect->l; m<lpRect->r; m++) {
        P1.x = m, P1.y = n;
        Distance = sqrt(pow(fabs(C.x-P1.x), 2)+pow(fabs(C.y-P1.y), 2));
        if(Distance>r) continue;
        SumPixel+=IElement(m, n);
        Count++;
    }}
    AvgPixel = SumPixel/Count;
    if(IsValue) return AvgPixel;

    int PixelCount =0;
    for (register int n=lpRect->t; n<lpRect->b; n++) {
    for (register int m=lpRect->l; m<lpRect->r; m++) {
        P1.x = m, P1.y = n;
        Distance = sqrt(pow(fabs(C.x-P1.x), 2)+pow(fabs(C.y-P1.y), 2));
        if(Distance>r) continue;

        if      ( IsBright && IElement(m, n) > AvgPixel+Offset  ) PixelCount++;
        else if (!IsBright && IElement(m, n) < AvgPixel-Offset  ) PixelCount++;
    }}
    return PixelCount;
}

int CBoundary::GetMaskPixelCount(JRect *lpRect)
{
    if (lpRect==NULL) return 0;

    int Count = 0;
    for (register int n=lpRect->t; n<lpRect->b; n++) {
    for (register int m=lpRect->l; m<lpRect->r; m++) {
        if (MElement(m, n)) Count++;
    }}
    return Count;
}

void CBoundary::OnMaskIpArea(JRect Rect, int Mask)
{
    register int Width  = IWidth ;
    register int Height = IHeight;

    for (register int y=Rect.t; y<Rect.b; y++) {
    for (register int x=Rect.l; x<Rect.r; x++) {
        if (Mask==0) {
            if (x<=0     || x>=Width || y<=0     || y>=Height) continue;
            if (x<Rect.l || x>Rect.r || y<Rect.t || y>Rect.b) continue;
        }
        if (Mask==1) {
            if ((x-1)<=0      || x>=Width  || y<=0      || y>=Height) continue;
            if ((x-1)<=Rect.l || x>=Rect.r || y<=Rect.t || y>=Rect.b) continue;
        }
        MElement(x,y) = Mask;
    }}
}

void CBoundary::OnMaskIpArea(TPoint p1, TPoint p2, TPoint p3, TPoint p4, int Mask)
{
    register int Width   = IWidth ;
    register int Height  = IHeight;
    JRect rect;
    rect.Clear();
    rect.l = p1.x < p3.x ? p1.x : p3.x;
    rect.t = p1.y < p2.y ? p1.y : p2.y;
    rect.r = p2.x > p4.x ? p2.x : p4.x;
    rect.b = p3.y > p4.y ? p3.y : p4.y;

    register float aTop   = Math.a(p1, p2);
    register float bTop   = Math.b(p1, p2);
    register float aRight = Math.a(p2, p4);
    register float bRight = Math.b(p2, p4);
    register float aBtm   = Math.a(p3, p4);
    register float bBtm   = Math.b(p3, p4);
    register float aLeft  = Math.a(p1, p3);
    register float bLeft  = Math.b(p1, p3);

    int staX, endX;

    //--------------------------------------------------------------------------
    for (register int y=rect.t; y<=rect.b; y++) {
        if (p1.y > p2.y){
            staX = (y < p1.y) ? aTop  ==0 ? p1.x : (y-bTop  )/aTop   : aLeft ==0 ? p1.x : (y-bLeft )/aLeft ;
            endX = (y < p4.y) ? aRight==0 ? p4.x : (y-bRight)/aRight : aBtm  ==0 ? p4.x : (y-bBtm  )/aBtm  ;
        }
        else {
            staX = (y < p3.y) ? aLeft ==0 ? p1.x : (y-bLeft )/aLeft  : aBtm  ==0 ? p1.x : (y-bBtm  )/aBtm  ;
            endX = (y < p2.y) ? aTop  ==0 ? p2.x : (y-bTop  )/aTop   : aRight==0 ? p2.x : (y-bRight)/aRight;
        }
    for (register int x=staX; x<=endX; x++) {
        if (Mask==0) {
            if (x<=0     || x>=Width || y<=0     || y>=Height) continue;
            if (x<rect.l || x>rect.r || y<rect.t || y>rect.b) continue;
        }
        if (Mask==1) {
            if ((x-1)<=0      || x>=Width  || y<=0      || y>=Height) continue;
            if ((x-1)<=rect.l || x>=rect.r || y<=rect.t || y>=rect.b) continue;
        }
        MElement(x,y) = Mask;
    }}
    //--------------------------------------------------------------------------
}

int CBoundary::OnExecute(bool IsRotation)
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CBoundary GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CBoundary GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CBoundary GImage Height Size"    )); return false; }

	IWidth      = GImage->GetWidth()   ;
	IHeight     = GImage->GetHeight()  ;

	// Image,IRoi 최상위 클래스 상속 
	BufferWidth = GImage->GetWidth()   ; 
	IAddress    = GImage->GetImagePtr();

	IpRect.l = IRoi->GetOrgX();
	IpRect.t = IRoi->GetOrgY();
	IpRect.r = IpRect.l+IRoi->GetWidth();
	IpRect.b = IpRect.t+IRoi->GetHeight();

//     if (IWidth<1) return false;
//     if (IHeight<1) return false;
//     if (IAddress==NULL) return false;

    int FindCount = 0;

    if (IsRotation) FindCount = OnFindBlobFast(IpRect); //회전하지 않고 블럽 찾음
    else            FindCount = OnFindBlob    (IpRect); //회전해서 블럽 찾음

    return FindCount;
}

int CBoundary::OnFindBlob(TPoint p1, TPoint p2, TPoint p3, TPoint p4, bool IsBlobClear)
{
    TrPa.MaxBbCnt = 500;
    TrPa.MaxPxCnt = 10000;
    if (TrPa.MinPxCnt==  0) TrPa.MinPxCnt   = 5;
    if (TrPa.OffsetY ==  0) TrPa.OffsetY    = 1;
    if (TrPa.StepY   ==  0) TrPa.StepY      = 1;
    if (TrPa.OffsetX ==  0) TrPa.OffsetX    = 1;
    if (TrPa.StepX   ==  0) TrPa.StepX      = 1;

    register int FixMargin   = 1;
    register int Width       = IWidth  ;
    register int Height      = IHeight ;
    register int IgInSide    = TrPa.IgInSide ;
    register int MaxBbCnt    = TrPa.MaxBbCnt ;
    register int MinPxCnt    = TrPa.MinPxCnt ;
    register int MaxPxCnt    = TrPa.MaxPxCnt ;
    register int OffsetY     = TrPa.OffsetY  ;
    register int StepY       = TrPa.StepY    ;
    register int OffsetX     = TrPa.OffsetX  ;
    register int StepX       = TrPa.StepX    ;
    register int HiThHold    = TrPa.HiThHold ;
    register int LoThHold    = TrPa.LoThHold ;
    register int Is4WayTrace = false;
    const bool   IgMask      = false;
    register int OpLocalThHold = TrPa.OpLocalThHold;
    const bool   OpfiMax33 = TrPa.OpfiMax33;

    //IsBlobClear = true;
    if (IsBlobClear) {
        if (Blob!=NULL) { delete[] Blob; Blob=NULL; }
        BlobCount = 0;
        Blob = new TBlob[MaxBbCnt];
    }

    int SortStartId = BlobCount;

    if (IWidth <1) return 0;
    if (IHeight<1) return 0;

    if (TAddress!=NULL) { delete[] TAddress; }
    if (MAddress!=NULL) { delete[] MAddress; }
    TAddress = new BYTE[IWidth*IHeight];
    MAddress = new BYTE[IWidth*IHeight];
    memset(TAddress, 0, IWidth*IHeight);
    memset(MAddress, 0, IWidth*IHeight);

    //--------------------------------------------------------------------------
    // START : 잘못된 영역으로 인해 RETURN
    //--------------------------------------------------------------------------
    if (p1.x<0) p1.x=1; if (p1.y<0) p1.y=1;
    if (p2.x<0) p2.x=1; if (p2.y<0) p2.y=1;
    if (p3.x<0) p3.x=1; if (p3.y<0) p3.y=1;
    if (p4.x<0) p4.x=1; if (p4.y<0) p4.y=1;

    if (p1.x>=Width) p1.x=Width-2; if (p1.y>=Height) p1.y=Height-2;
    if (p2.x>=Width) p2.x=Width-2; if (p2.y>=Height) p2.y=Height-2;
    if (p3.x>=Width) p3.x=Width-2; if (p3.y>=Height) p3.y=Height-2;
    if (p4.x>=Width) p4.x=Width-2; if (p4.y>=Height) p4.y=Height-2;

    //--------------------------------------------------------------------------
    // END : 잘못된 영역으로 인해 RETURN
    //--------------------------------------------------------------------------

    register const POINT nei[8] =
    {
        {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
    };

    register int IncNei = Is4WayTrace ? 2 : 1;
    register int c0, c1, c2, cm, cp;
    register int x0, y0, k, n, PixelCount;

    TPoint *Point = new TPoint[MaxPxCnt];

    if (StepX < 1) StepX = 1;
    if (StepY < 1) StepY = 1;

    JRect rect;
    rect.Clear();
    rect.l = p1.x < p3.x ? p1.x : p3.x;
    rect.t = p1.y < p2.y ? p1.y : p2.y;
    rect.r = p2.x > p4.x ? p2.x : p4.x;
    rect.b = p3.y > p4.y ? p3.y : p4.y;

    register float aTop   = Math.a(p1, p2);
    register float bTop   = Math.b(p1, p2);
    register float aRight = Math.a(p2, p4);
    register float bRight = Math.b(p2, p4);
    register float aBtm   = Math.a(p3, p4);
    register float bBtm   = Math.b(p3, p4);
    register float aLeft  = Math.a(p1, p3);
    register float bLeft  = Math.b(p1, p3);

    int staX, endX;

    if (!TrPa.LoThHold && !TrPa.HiThHold) {
        LoThHold = GetMeanThHold(rect)+3;
        if (LoThHold<1) return 0;
    }

    const int MatrixSize = 8;
    int xCuLocalPos = 0;
    int yCuLocalPos = 0;
    int xLocalSize = (p2.x-p1.x)/MatrixSize;
    int yLocalSize = (p4.y-p1.y)/MatrixSize;
    int LocalThHold[MatrixSize][MatrixSize];
    if (OpLocalThHold) {
        JRect LocalRect;
        for (int i=0; i<MatrixSize; i++) {
        for (int j=0; j<MatrixSize; j++) {
            LocalRect.l = rect.l+xLocalSize*i;
            LocalRect.t = rect.t+yLocalSize*j;
            LocalRect.r = rect.l+xLocalSize*i+xLocalSize;
            LocalRect.b = rect.t+yLocalSize*j+yLocalSize;
            LocalThHold[i][j] = GetMeanThHold(LocalRect)+LoThHold;
        }}
    }

    //--------------------------------------------------------------------------
    // START : Ignore Area By Rect
    OnMaskIpArea (p1, p2, p3, p4, 1);

    if (TrPa.UseIgnoreBlob) {
        if (IgBlobMargin<1) IgBlobMargin = 1;

        for (register int i=0; i<IgBlobCount; i++) {
            if (IgBlobCount>=MAX_IGNORE_RECT) {
                AfxMessageBox(_T("IGNORE RECT COUNT 초과"));
                break;
            }

            if (IgBlob[i].lt.x<1 || IgBlob[i].lt.y<1) {
                IgBlob[i].lt.x=IgBlob[i].l; IgBlob[i].lt.y=IgBlob[i].t-IgBlobMargin;
                IgBlob[i].rt.x=IgBlob[i].r; IgBlob[i].rt.y=IgBlob[i].t-IgBlobMargin;
                IgBlob[i].lb.x=IgBlob[i].l; IgBlob[i].lb.y=IgBlob[i].b+IgBlobMargin*2;
                IgBlob[i].rb.x=IgBlob[i].r; IgBlob[i].rb.y=IgBlob[i].b+IgBlobMargin*2;
            };

            OnMaskIpArea(IgBlob[i].lt, IgBlob[i].rt, IgBlob[i].lb, IgBlob[i].rb, 0);
        }
    }
    // END : Ignore Area By Rect
    //--------------------------------------------------------------------------

    for (register int y=rect.t+OffsetY+FixMargin; y<rect.b-OffsetY-FixMargin; y+=StepY) {
        if (p1.y > p2.y){
            staX = (y < p1.y) ? aTop  ==0 ? p1.x : (y-bTop  )/aTop   : aLeft ==0 ? p1.x : (y-bLeft )/aLeft ;
            endX = (y < p4.y) ? aRight==0 ? p4.x : (y-bRight)/aRight : aBtm  ==0 ? p4.x : (y-bBtm  )/aBtm  ;
        }
        else {
            staX = (y < p3.y) ? aLeft ==0 ? p1.x : (y-bLeft )/aLeft  : aBtm  ==0 ? p1.x : (y-bBtm  )/aBtm  ;
            endX = (y < p2.y) ? aTop  ==0 ? p2.x : (y-bTop  )/aTop   : aRight==0 ? p2.x : (y-bRight)/aRight;
        }
    for (register int x=staX+OffsetX+FixMargin; x<endX-OffsetX-FixMargin; x+=StepX) {
        if ((x-1)<=0 || x>=Width || y<=0 || y>=Height) continue;

        if (OpLocalThHold) {
            xCuLocalPos = (x-rect.l)/xLocalSize;
            yCuLocalPos = (y-rect.t)/yLocalSize;
            LoThHold = LocalThHold[xCuLocalPos][yCuLocalPos];
        }

        int AP = IElement(x, y);

        if (OpfiMax33) {
            if (IElement(x-1, y-1)>AP) AP = IElement(x-1, y-1);
            if (IElement(x  , y-1)>AP) AP = IElement(x  , y-1);
            if (IElement(x+1, y-1)>AP) AP = IElement(x+1, y-1);
            if (IElement(x-1, y  )>AP) AP = IElement(x-1, y  );
            if (IElement(x+1, y  )>AP) AP = IElement(x+1, y  );
            if (IElement(x-1, y+1)>AP) AP = IElement(x-1, y+1);
            if (IElement(x  , y+1)>AP) AP = IElement(x  , y+1);
            if (IElement(x+1, y+1)>AP) AP = IElement(x+1, y+1);
        }

        bool IsTrace;
        int p[8], p1[8], p2[8];

        c0 = HiThHold ? AP < HiThHold : true && LoThHold ? AP > LoThHold : true;

        IsTrace = c0;
        bool pIsVisited = TElement(x, y) ? true : false;

        bool IsMask = IgMask || MElement(x,y);

        if (IsTrace && !pIsVisited && IsMask) {
            bool IsEdge = false;
            PixelCount = 0;
            x0 = x; y0 = y;
            n = 4;
            do {
                register short u;
                register short v;
                for (k=0; k<8; k+=IncNei, n=((n+IncNei) & 7)) {
                    u = (short)(x + nei[n].x);
                    v = (short)(y + nei[n].y);
                    if ((u-1)<=0 || (u+1)>=Width || (v-1)<=0 || (v+1)>=Height) continue;

                    int E_AP = IElement(u, v);

                    if (OpfiMax33) {
                        if (IElement(u-1, v-1)>E_AP) E_AP = IElement(u-1, v-1);
                        if (IElement(u  , v-1)>E_AP) E_AP = IElement(u  , v-1);
                        if (IElement(u+1, v-1)>E_AP) E_AP = IElement(u+1, v-1);
                        if (IElement(u-1, v  )>E_AP) E_AP = IElement(u-1, v  );
                        if (IElement(u+1, v  )>E_AP) E_AP = IElement(u+1, v  );
                        if (IElement(u-1, v+1)>E_AP) E_AP = IElement(u-1, v+1);
                        if (IElement(u  , v+1)>E_AP) E_AP = IElement(u  , v+1);
                        if (IElement(u+1, v+1)>E_AP) E_AP = IElement(u+1, v+1);
                    }

                    bool IsMask = IgMask || MElement(u,v);
                    c2 = HiThHold ? E_AP < HiThHold : true && LoThHold ? E_AP > LoThHold : true;
                    IsEdge = (c2==c0) && IsMask;

                    if (IsEdge) break;
                }
                if (k==8) break;

                if (!TElement(x, y)) {
                    Point[PixelCount].x = x;
                    Point[PixelCount].y = y;
                    PixelCount++;
                }

                TElement(x, y) = true;

                if (PixelCount>=MaxPxCnt) break;

                x = x + nei[n].x;
                y = y + nei[n].y;

                n = (n+4+IncNei) & 7;
            }
            while(!(x==x0 && y==y0));

            x=x0; y=y0;

            if (k==8) continue;

            if (PixelCount<MinPxCnt) continue;
            if (PixelCount>MaxPxCnt) continue;

            for (register int k=0; k<PixelCount; k++) {
                JRect *Rect = &Blob[BlobCount].Rect;
                Rect->l = Rect->l<Point[k].x && Rect->l!=0 ? Rect->l : Point[k].x;
                Rect->t = Rect->t<Point[k].y && Rect->t!=0 ? Rect->t : Point[k].y;
                Rect->r = Rect->r>Point[k].x               ? Rect->r : Point[k].x;
                Rect->b = Rect->b>Point[k].y               ? Rect->b : Point[k].y;

                if(k==0) {
                    Blob[BlobCount].lPoint = Point[k];
                    Blob[BlobCount].tPoint = Point[k];
                    Blob[BlobCount].rPoint = Point[k];
                    Blob[BlobCount].bPoint = Point[k];
                }
                else {
                    if (Point[k].x < Blob[BlobCount].lPoint.x) Blob[BlobCount].lPoint = Point[k];
                    if (Point[k].y < Blob[BlobCount].tPoint.y) Blob[BlobCount].tPoint = Point[k];
                    if (Point[k].x > Blob[BlobCount].rPoint.x) Blob[BlobCount].rPoint = Point[k];
                    if (Point[k].y > Blob[BlobCount].bPoint.y) Blob[BlobCount].bPoint = Point[k];
                }
            }

            Blob[BlobCount].Rect.lt.x = Blob[BlobCount].Rect.l; Blob[BlobCount].Rect.lt.y = Blob[BlobCount].Rect.t;
            Blob[BlobCount].Rect.rt.x = Blob[BlobCount].Rect.r; Blob[BlobCount].Rect.rt.y = Blob[BlobCount].Rect.t;
            Blob[BlobCount].Rect.rb.x = Blob[BlobCount].Rect.r; Blob[BlobCount].Rect.rb.y = Blob[BlobCount].Rect.b;
            Blob[BlobCount].Rect.lb.x = Blob[BlobCount].Rect.l; Blob[BlobCount].Rect.lb.y = Blob[BlobCount].Rect.b;

            // Diamter
            //------------------------------------------------------------------
            float rw = Blob[BlobCount].Rect.GetWidth();
            float rh = Blob[BlobCount].Rect.GetHeight();

            Blob[BlobCount].Diameter = (rw+rh)/2.0;
            Blob[BlobCount].Radius = Blob[BlobCount].Diameter/2.0;
            Blob[BlobCount].MaxDiameter = rw>rh ? rw : rh;
            Blob[BlobCount].MinDiameter = rw<rh ? rw : rh;

            int PvltLg = 10000;
            int PvrtLg = 10000;
            int PvrbLg = 10000;
            int PvlbLg = 10000;

            for (register int k=0; k<PixelCount; k++) {
                int CultLg = Math.GetLength(Blob[BlobCount].Rect.lt, Point[k]);
                int CurtLg = Math.GetLength(Blob[BlobCount].Rect.rt, Point[k]);
                int CurbLg = Math.GetLength(Blob[BlobCount].Rect.rb, Point[k]);
                int CulbLg = Math.GetLength(Blob[BlobCount].Rect.lb, Point[k]);

                if (PvltLg > CultLg) {PvltLg = CultLg; Blob[BlobCount].ltPoint = Point[k]; }
                if (PvrtLg > CurtLg) {PvrtLg = CurtLg; Blob[BlobCount].rtPoint = Point[k]; }
                if (PvrbLg > CurbLg) {PvrbLg = CurbLg; Blob[BlobCount].rbPoint = Point[k]; }
                if (PvlbLg > CulbLg) {PvlbLg = CulbLg; Blob[BlobCount].lbPoint = Point[k]; }
            }

            if (TrPa.OpMinMaxRadious) {
                Blob[BlobCount].MinRadius = 100000;
                Blob[BlobCount].MaxRadius = 0;
                TFPoint C = Blob[BlobCount].Rect.Point();
                for (register int k=0; k<PixelCount; k++) {
                    float dx = Point[k].x-C.x;
                    float dy = Point[k].y-C.y;
                    float CuLg = sqrt(pow(dx, 2) + pow(dy, 2));
                    if (CuLg<Blob[BlobCount].MinRadius) Blob[BlobCount].MinRadius = CuLg;
                    if (CuLg>Blob[BlobCount].MaxRadius) Blob[BlobCount].MaxRadius = CuLg;
                }
            }

            if (IgInSide) {
                for (register int n=Blob[BlobCount].Rect.t; n<Blob[BlobCount].Rect.b; n++) {
                for (register int m=Blob[BlobCount].Rect.l; m<Blob[BlobCount].Rect.r; m++) {
                    TElement(m, n) = true;
                }}
            }

            if (TrPa.IsQuantity2 || TrPa.IsQuantity8) {
                bool r;
                for (register int n=Blob[BlobCount].Rect.t; n<Blob[BlobCount].Rect.b; n++) {
                for (register int m=Blob[BlobCount].Rect.l; m<Blob[BlobCount].Rect.r; m++) {
                    if      (  HiThHold &&   LoThHold) r = IElement(m, n) >  LoThHold && IElement(m, n) <  HiThHold ? true : false;
                    else if ( !HiThHold &&   LoThHold) r = IElement(m, n) >  LoThHold ? true : false;
                    else if (  HiThHold &&  !LoThHold) r = IElement(m, n) <  HiThHold ? true : false;
                    else                               r = IElement(m, n)             ? true : false;

                    if (TrPa.IsQuantity2 && r) Blob[BlobCount].Quantity2++;
                    if (TrPa.IsQuantity8 && r) Blob[BlobCount].Quantity8+=IElement(m, n);
                }}
            }

            //------------------------------------------------------------------
            // START : 최소 자승법(Least Square Circle Fitting)에 의한 원형 구하기
            if (TrPa.OpCircleFit) {
                float SampleSize = PixelCount*0.75; //0.5~0.8이하로 입력할것
                float DistDevRatio = 0.15; //0.1~0.25 이하로 입력할것

                TFPoint __C(0, 0);
                double Radius = 0;

                if (SampleSize>3) {
                    CircleFit.OnRansac(Point, PixelCount, SampleSize, DistDevRatio, &__C, &Radius);
                    if (fabs(Radius)>1) {
                        Blob[BlobCount].C = __C;
                        Blob[BlobCount].Radius = Radius;
                        Blob[BlobCount].Diameter = Radius*2.0;
                    }

                }
            }
            // END : 최소 자승법(Least Square Circle Fitting)에 의한 원형 구하기
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            // START : REMOVE MIN SIZE
            //if (TrPa.xMinLength || TrPa.yMinLength) {
            //    bool IsMinHorLg = TrPa.xMinLength && TrPa.xMinLength > Blob[BlobCount].Rect.GetWidth();
            //    bool IsMinVerLg = TrPa.yMinLength && TrPa.yMinLength > Blob[BlobCount].Rect.GetHeight();
            //
            //    if (IsMinHorLg) continue;
            //    if (IsMinVerLg) continue;
            //}
            // END : REMOVE MIN SIZE
            //------------------------------------------------------------------

            Blob[BlobCount].n = PixelCount;
            BlobCount++;

            if (BlobCount>=MaxBbCnt) break;
        }
        if (BlobCount>=MaxBbCnt) break;
    }
    if (BlobCount>=MaxBbCnt) break;
    }

    /*
    //--------------------------------------------------------------------------
    //START : MERGE BLOB간의 간격
    //if (TrPa.MgCount) {
    //    int Gab = TrPa.xMgPxCnt;
    //    register bool IsxMg, IsyMg;
    //    JRect r1, r2;
    //    int sort = 0;
    //    int tmpBlobCount = BlobCount;
    //    TFPoint SPoint;
    //    bool IsNeedMerge = false;
    //    TFPoint lCPoint;
    //
    //    for (register int k=0; k<tmpBlobCount; k++) {
    //        sort = 0;
    //        if (BlobCount<2) break;
    //        if (Blob[k].Rect.l==0 && Blob[k].Rect.t==0 && Blob[k].Rect.r==0 && Blob[k].Rect.b==0) continue;
    //
    //        for (register int l=k+1; l<BlobCount; l++) {
    //            if (l >= BlobCount) break;
    //            if (l > (tmpBlobCount-1)) break;
    //            if (Blob[l].Rect.l==0 && Blob[l].Rect.t==0 && Blob[l].Rect.r==0 && Blob[l].Rect.b==0) continue;
    //
    //            lCPoint = Blob[l].Rect.Point();
    //            if (Blob[k].Rect.l < lCPoint.x)
    //            if (Blob[k].Rect.t < lCPoint.y)
    //            if (Blob[k].Rect.r > lCPoint.x)
    //            if (Blob[k].Rect.b > lCPoint.y) IsNeedMerge = true;
    //
    //            if (!IsNeedMerge) {
    //                for (register int v=0; v<4; v++) {
    //                    if (v==0) SPoint = Blob[k].Rect.lt;
    //                    if (v==1) SPoint = Blob[k].Rect.rt;
    //                    if (v==2) SPoint = Blob[k].Rect.rb;
    //                    if (v==3) SPoint = Blob[k].Rect.lb;
    //
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.lt))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.lt))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.lt))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.lt))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.rt))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.rt))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.rt))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.rt))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.rb))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.rb))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.rb))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.rb))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.lb))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.lb))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.lb))<=Gab) { IsNeedMerge = true; break; }
    //                    if ((Math.GetLength(SPoint, Blob[l].Rect.lb))<=Gab) { IsNeedMerge = true; break; }
    //                }
    //            }
    //            if (!IsNeedMerge) continue;
    //
    //            Blob[k].n     += Blob[l].n;
    //            Blob[k].Rect   = Blob[k].Rect.OnMerge(Blob[k].Rect, Blob[l].Rect);
    //            Blob[l].Rect.l = Blob[l].Rect.t = Blob[l].Rect.r = Blob[l].Rect.b = 0;
    //
    //            Blob[k].Rect.lt.x = Blob[k].Rect.l; Blob[k].Rect.lt.y = Blob[k].Rect.t;
    //            Blob[k].Rect.rt.x = Blob[k].Rect.r; Blob[k].Rect.rt.y = Blob[k].Rect.t;
    //            Blob[k].Rect.rb.x = Blob[k].Rect.r; Blob[k].Rect.rb.y = Blob[k].Rect.b;
    //            Blob[k].Rect.lb.x = Blob[k].Rect.l; Blob[k].Rect.lb.y = Blob[k].Rect.b;
    //
    //            BlobCount = BlobCount - 1;
    //            k = -1;
    //
    //            break;
    //        }
    //        if (IsNeedMerge) {
    //            for (int j=0; j<BlobCount+1; j++) {
    //                if (Blob[j].Rect.l==0 && Blob[j].Rect.t==0 && Blob[j].Rect.r==0 && Blob[j].Rect.b==0) continue;
    //                Blob[sort] = Blob[j];
    //                sort++;
    //            }
    //        }
    //        IsNeedMerge = false;
    //    }
    //}
    ////END : MERGE BLOB간의 간격
    //--------------------------------------------------------------------------
    */

    //--------------------------------------------------------------------------
    //START : 합성된 BLOB의 크기에 의해 MERGE
    if (TrPa.MgCount) {
        if (TrPa.MgTgHorLength && TrPa.MgTgVerLength) {
            register bool IsxMg, IsyMg ;
            JRect r1, r2;
            int sort = 0;
            int tmpBlobCount = BlobCount;

            for (register int k=0; k<tmpBlobCount; k++) {
                if (Blob[k].Rect.l==0 && Blob[k].Rect.t==0 && Blob[k].Rect.r==0 && Blob[k].Rect.b==0) continue;
                for (register int l=k+1; l<k+TrPa.MgCount; l++) {
                    IsxMg = false; IsyMg = false;
                    if (l > (tmpBlobCount-1)) break;
                    if (Blob[l].Rect.l==0 && Blob[l].Rect.t==0 && Blob[l].Rect.r==0 && Blob[l].Rect.b==0) continue;

                    r1 = Blob[k].Rect;
                    r2 = Blob[l].Rect;

                    JRect mrect = Blob[k].Rect.OnMerge(Blob[k].Rect, Blob[l].Rect);
                    register int HorLength   = mrect.GetWidth();
                    register int VerLength   = mrect.GetHeight();
                    IsxMg = (HorLength<TrPa.MgTgHorLength) ? true : false;
                    IsyMg = (VerLength<TrPa.MgTgVerLength) ? true : false;

                    if (IsxMg && IsyMg) {
                        Blob[k].n     += Blob[l].n;
                        Blob[k].Rect   = mrect    ;
                        Blob[l].Rect.l = Blob[l].Rect.t = Blob[l].Rect.r = Blob[l].Rect.b = 0;
                        BlobCount--;
                        k = -1;
                        break;
                    }
                }
            }

            for (int j=0; j<tmpBlobCount; j++) {
                if (Blob[j].Rect.l==0 && Blob[j].Rect.t==0 && Blob[j].Rect.r==0 && Blob[j].Rect.b==0) continue;
                Blob[sort] = Blob[j];
                sort++;
            }
        }
    }
    //END : 합성된 BLOB의 크기에 의해 MERGE
    //--------------------------------------------------------------------------

    if (!SortStartId) {
        OnSortBlob();
    }
    else {
        OnSortBlob(SortStartId);
    }

    //Check Ignore blob
    if (TrPa.IgBlobFarX   || TrPa.IgBlobFarY   ||
        TrPa.MinHorPitch  || TrPa.MinVerPitch  ||
        TrPa.MinHorLength || TrPa.MinVerLength ||
        TrPa.MaxHorLength || TrPa.MaxVerLength ||
        TrPa.MinPxBlob                         ){
        register bool IsFarX , IsFarY , IsMinPitX , IsMinPitY , IsMinVerLen , IsMinHorLen , IsMaxVerLen , IsMaxHorLen , IsMinPx;
        JRect pr , cr , nr ;
        bool RemBlob ;
        for (register int k=0; k<BlobCount; k++) {
            IsFarX = IsFarY = IsMinPitX = IsMinPitY = IsMinVerLen = IsMinHorLen = IsMaxVerLen = IsMinHorLen = IsMinPx = false ;
            RemBlob = false ;
            pr = k > 0             ? Blob[k-1].Rect : pr ;
            cr =                     Blob[k  ].Rect      ;
            nr = (k+1) < BlobCount ? Blob[k+1].Rect : nr ;

            IsFarX      = TrPa.IgBlobFarX   && cr.GetHorDist(cr,pr)>TrPa.IgBlobFarX && cr.GetHorDist(cr,nr)>TrPa.IgBlobFarX ;
            IsFarY      = TrPa.IgBlobFarY   && cr.GetVerDist(cr,pr)>TrPa.IgBlobFarY && cr.GetVerDist(cr,nr)>TrPa.IgBlobFarY ;
            IsMinPitX   = TrPa.MinHorPitch  && (cr.t <= pr.b && cr.b >= pr.t) ? abs(cr.Point().x - pr.Point().x) < TrPa.MinHorPitch : false ;
            IsMinPitY   = TrPa.MinVerPitch  && (cr.l <= pr.r && cr.r >= pr.l) ? abs(cr.Point().y - pr.Point().y) < TrPa.MinVerPitch : false ;
            IsMinVerLen = TrPa.MinVerLength && TrPa.MinVerLength > abs(cr.b - cr.t) ;
            IsMinHorLen = TrPa.MinHorLength && TrPa.MinHorLength > abs(cr.r - cr.l) ;
            IsMaxVerLen = TrPa.MaxVerLength && TrPa.MaxVerLength < abs(cr.b - cr.t) ;
            IsMaxHorLen = TrPa.MaxHorLength && TrPa.MaxHorLength < abs(cr.r - cr.l) ;
            IsMinPx     = TrPa.MinPxBlob    && TrPa.MinPxBlob    > Blob[k].n        ;

            //if(TrPa.IsFindAnd){
            //    RemBlob = IsFarX || IsFarY || IsMinPitX || IsMinPitY || IsMinVerLen || IsMinHorLen || IsMaxVerLen || IsMaxHorLen || IsMinPx;
            //}
            //else {
            //    RemBlob = IsFarX && IsFarY && IsMinPitX && IsMinPitY && IsMinVerLen && IsMinHorLen && IsMaxVerLen && IsMaxHorLen && IsMinPx;
            //}
            RemBlob = IsFarX || IsFarY || IsMinPitX || IsMinPitY || IsMinVerLen || IsMinHorLen || IsMaxVerLen || IsMaxHorLen || IsMinPx;

            if (RemBlob) {
                for (int j=k; j<BlobCount-1; j++) Blob[j] = Blob[j+1];          // array overflow 발생. 120216 by kjh
                BlobCount--;
                k--;
            }
        }
    }

    //if (TrPa.OpWeight) OnWeight();
    delete[] Point;
    return BlobCount;
}

int CBoundary::OnFindBlobFast(JRect Rect)
{
    TrPa.MaxBbCnt = 2000;
    TrPa.MaxPxCnt = 10000;
    if (TrPa.MinPxCnt==  0) TrPa.MinPxCnt   = 5;
    if (TrPa.OffsetY ==  0) TrPa.OffsetY    = 1;
    if (TrPa.StepY   ==  0) TrPa.StepY      = 1;
    if (TrPa.OffsetX ==  0) TrPa.OffsetX    = 1;
    if (TrPa.StepX   ==  0) TrPa.StepX      = 1;

    double MinVerLength = TrPa.MinVerLength;
    double MinHorLength = TrPa.MinHorLength;
    if (MinVerLength<=1) MinVerLength = TrPa.MinVerLength = 2;
    if (MinHorLength<=1) MinHorLength = TrPa.MinHorLength = 2;

    register int FixMargin   = 1;
    register int Width       = IWidth  ;
    register int Height      = IHeight ;
    register int IgInSide    = true          ;
    register int MaxBbCnt    = TrPa.MaxBbCnt ;
    register int MinPxCnt    = TrPa.MinPxCnt;
    register int MaxPxCnt    = TrPa.MaxPxCnt ;
    register int OffsetY     = MinVerLength/2;
    register int StepY       = MinVerLength/2;
    register int OffsetX     = 1;
    register int StepX       = 1;
    register int HiThHold    = TrPa.HiThHold ;
    register int LoThHold    = TrPa.LoThHold ;
    register int Is4WayTrace = false;

    if (Blob!=NULL) { delete[] Blob; Blob=NULL; }
    BlobCount = 0;
    Blob = new TBlob[MaxBbCnt];

    //if (TAddress!=NULL) { delete[] TAddress; }
    //TAddress = new BYTE[IWidth*IHeight];
    //memset(TAddress, 0, IWidth*IHeight);

    if (TAddress!=NULL) { delete[] TAddress; }
    if (MAddress!=NULL) { delete[] MAddress; }
    TAddress = new BYTE[IWidth*IHeight];
    MAddress = new BYTE[IWidth*IHeight];
    memset(TAddress, 0, IWidth*IHeight);
    memset(MAddress, 0, IWidth*IHeight);

    //--------------------------------------------------------------------------
    // START : 잘못된 영역으로 인해 RETURN
    if (Rect.l<=0) return 0;
    if (Rect.t<=0) return 0;
    if (Rect.r<=0) return 0;
    if (Rect.b<=0) return 0;

    if (Rect.l>=IWidth ) return 0;
    if (Rect.t>=IHeight) return 0;
    if (Rect.r>=IWidth ) return 0;
    if (Rect.b>=IHeight) return 0;

    // END : 잘못된 영역으로 인해 RETURN
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // START : Ignore Area By Rect
    OnMaskIpArea(Rect, 1);

    if (TrPa.UseIgnoreBlob) {
        for (register int i=0; i<IgBlobCount; i++) {
            if (IgBlobCount>=MAX_IGNORE_RECT) {
                AfxMessageBox(_T("IGNORE RECT COUNT 초과"));
                break;
            }

            if (IgBlob[i].lt.x<1 || IgBlob[i].lt.y<1) {
                IgBlob[i].lt.x=IgBlob[i].l; IgBlob[i].lt.y=IgBlob[i].t;
                IgBlob[i].rt.x=IgBlob[i].r; IgBlob[i].rt.y=IgBlob[i].t;
                IgBlob[i].lb.x=IgBlob[i].l; IgBlob[i].lb.y=IgBlob[i].b;
                IgBlob[i].rb.x=IgBlob[i].r; IgBlob[i].rb.y=IgBlob[i].b;
            };

            OnMaskIpArea(IgBlob[i], 0);
        }
    }
    // END : Ignore Area By Rect
    //--------------------------------------------------------------------------

    register const POINT nei[8] =
    {
        {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
    };

    register int IncNei = Is4WayTrace ? 2 : 1;
    register int c0, c1, c2, cm, cp;
    register int x0, y0, k, n, PixelCount;

    TPoint *Point = new TPoint[MaxPxCnt];

    if (!TrPa.LoThHold && !TrPa.HiThHold) {
        LoThHold = GetMeanThHold(Rect)+3;
        if (LoThHold<1) return 0;
    }

    for (register int y=Rect.t+OffsetY+FixMargin; y<Rect.b-OffsetY-FixMargin; y+=StepY) {
    for (register int x=Rect.l+OffsetX+FixMargin; x<Rect.r-OffsetX-FixMargin; x+=StepX) {
        if ((x-1)<=0 || x>=Width || y<=0 || y>=Height) continue;

        int AP = IElement(x, y);

        bool IsTrace;
        int p[8], p1[8], p2[8];

        c0 = HiThHold ? AP < HiThHold : true && LoThHold ? AP > LoThHold : true;

        IsTrace = c0;
        bool pIsVisited = TElement(x, y) ? true : false;
        bool IsMask = MElement(x,y);

        if (IsTrace && !pIsVisited && IsMask) {
            bool IsEdge = false;
            PixelCount = 0;
            x0 = x; y0 = y;
            n = 4;
            do {
                register short u;
                register short v;
                for (k=0; k<8; k+=IncNei, n=((n+IncNei) & 7)) {
                    u = (short)(x + nei[n].x);
                    v = (short)(y + nei[n].y);
                    if ((u-1)<=0 || (u+1)>=Width || (v-1)<=0 || (v+1)>=Height) continue;

                    int E_AP = IElement(u, v);

                    //c2 = HiThHold ? E_AP < HiThHold : true && LoThHold ? E_AP > LoThHold : true;
                    //IsEdge = (c2==c0);
                    //if (IsEdge) break;

                    bool IsMask = MElement(u,v);
                    c2 = HiThHold ? E_AP < HiThHold : true && LoThHold ? E_AP > LoThHold : true;
                    IsEdge = (c2==c0) && IsMask;

                    if (IsEdge) break;

                }
                if (k==8) break;

                if (!TElement(x, y)) {
                    Point[PixelCount].x = x;
                    Point[PixelCount].y = y;
                    PixelCount++;
                }

                TElement(x, y) = true;

                if (PixelCount>=MaxPxCnt) break;

                x = x + nei[n].x;
                y = y + nei[n].y;

                n = (n+4+IncNei) & 7;
            }
            while(!(x==x0 && y==y0));

            x=x0; y=y0;

            if (k==8) continue;

            if (PixelCount<MinPxCnt) continue;
            if (PixelCount>MaxPxCnt) continue;

            Blob[BlobCount].Rect.l = 0;
            Blob[BlobCount].Rect.t = 0;
            Blob[BlobCount].Rect.r = 0;
            Blob[BlobCount].Rect.b = 0;

            for (register int k=0; k<PixelCount; k++) {
                JRect *Rect = &Blob[BlobCount].Rect;
                Rect->l = Rect->l<Point[k].x && Rect->l!=0 ? Rect->l : Point[k].x;
                Rect->t = Rect->t<Point[k].y && Rect->t!=0 ? Rect->t : Point[k].y;
                Rect->r = Rect->r>Point[k].x               ? Rect->r : Point[k].x;
                Rect->b = Rect->b>Point[k].y               ? Rect->b : Point[k].y;
            }

            Blob[BlobCount].Rect.lt.x = Blob[BlobCount].Rect.l; Blob[BlobCount].Rect.lt.y = Blob[BlobCount].Rect.t;
            Blob[BlobCount].Rect.rt.x = Blob[BlobCount].Rect.r; Blob[BlobCount].Rect.rt.y = Blob[BlobCount].Rect.t;
            Blob[BlobCount].Rect.rb.x = Blob[BlobCount].Rect.r; Blob[BlobCount].Rect.rb.y = Blob[BlobCount].Rect.b;
            Blob[BlobCount].Rect.lb.x = Blob[BlobCount].Rect.l; Blob[BlobCount].Rect.lb.y = Blob[BlobCount].Rect.b;

            //Check Ignore blob
            if (TrPa.MinHorLength || TrPa.MinVerLength ||
                TrPa.MaxHorLength || TrPa.MaxVerLength){
                register bool IsMinVerLen, IsMinHorLen, IsMaxVerLen, IsMaxHorLen;
                JRect cr;
                bool RemBlob ;
                IsMinVerLen = IsMinHorLen = IsMaxVerLen = IsMinHorLen = false ;
                RemBlob = false ;
                cr = Blob[BlobCount].Rect;

                IsMinVerLen = TrPa.MinVerLength && TrPa.MinVerLength > abs(cr.b - cr.t) ;
                IsMinHorLen = TrPa.MinHorLength && TrPa.MinHorLength > abs(cr.r - cr.l) ;
                IsMaxVerLen = TrPa.MaxVerLength && TrPa.MaxVerLength < abs(cr.b - cr.t) ;
                IsMaxHorLen = TrPa.MaxHorLength && TrPa.MaxHorLength < abs(cr.r - cr.l) ;
                if(TrPa.IsFindAnd){
                    RemBlob = (IsMinVerLen || IsMinHorLen) || IsMaxVerLen || IsMaxHorLen;
                }
                else {
                    RemBlob = (IsMinVerLen && IsMinHorLen) || IsMaxVerLen || IsMaxHorLen;
                }

                if (RemBlob) continue;
            }


            if (IgInSide) {
                for (register int n=Blob[BlobCount].Rect.t; n<Blob[BlobCount].Rect.b; n++) {
                for (register int m=Blob[BlobCount].Rect.l; m<Blob[BlobCount].Rect.r; m++) {
                    TElement(m, n) = true;
                }}
            }

            Blob[BlobCount].n = PixelCount;
            BlobCount++;

            if (BlobCount>=MaxBbCnt) break;
        }
        if (BlobCount>=MaxBbCnt) break;
    }
    if (BlobCount>=MaxBbCnt) break;
    }

    OnSortBlob();

    //if (TrPa.OpWeight) OnWeight();
    delete[] Point;
    return BlobCount;
}


int CBoundary::OnFindBlob(JRect rect, bool IsBlobClear)
{
    IPRECT = rect;

    TPoint p1, p2, p3, p4;

    if (   rect.lt.x && rect.rt.x && rect.rb.x && rect.lb.x
        && rect.lt.y && rect.rt.y && rect.rb.y && rect.lb.y) {
        p1 = rect.lt;
        p2 = rect.rt;
        p3 = rect.lb;
        p4 = rect.rb;
    }
    else {
        p1.x = rect.l; p1.y = rect.t;
        p2.x = rect.r; p2.y = rect.t;
        p3.x = rect.l; p3.y = rect.b;
        p4.x = rect.r; p4.y = rect.b;
    }
    return OnFindBlob(p1, p2, p3, p4, IsBlobClear);
}

int CBoundary::OnFindBubble(JRect rect, bool IsBlobClear)
{
    IPRECT = rect;

    TPoint p1, p2, p3, p4;

    if (   rect.lt.x && rect.rt.x && rect.rb.x && rect.lb.x
        && rect.lt.y && rect.rt.y && rect.rb.y && rect.lb.y) {
        p1 = rect.lt;
        p2 = rect.rt;
        p3 = rect.lb;
        p4 = rect.rb;
    }
    else {
        p1.x = rect.l; p1.y = rect.t;
        p2.x = rect.r; p2.y = rect.t;
        p3.x = rect.l; p3.y = rect.b;
        p4.x = rect.r; p4.y = rect.b;
    }
    return OnFindBubble(p1, p2, p3, p4, IsBlobClear);
}

int CBoundary::OnFindBubble(TPoint p1, TPoint p2, TPoint p3, TPoint p4, bool IsBlobClear)
{
    TrPa.MaxBbCnt = 2000;
    TrPa.MaxPxCnt = 10000;
    if (TrPa.MinPxCnt==  0) TrPa.MinPxCnt   = 5;
    if (TrPa.OffsetY ==  0) TrPa.OffsetY    = 1;
    if (TrPa.StepY   ==  0) TrPa.StepY      = 1;
    if (TrPa.OffsetX ==  0) TrPa.OffsetX    = 1;
    if (TrPa.StepX   ==  0) TrPa.StepX      = 1;

    register int FixMargin   = 4;
    register int Width       = IWidth  ;
    register int Height      = IHeight ;
    register int IgInSide    = TrPa.IgInSide ;
    register int MaxBbCnt    = TrPa.MaxBbCnt ;
    register int MinPxCnt    = TrPa.MinPxCnt ;
    register int MaxPxCnt    = TrPa.MaxPxCnt ;
    register int OffsetY     = TrPa.OffsetY  ;
    register int StepY       = TrPa.StepY    ;
    register int OffsetX     = TrPa.OffsetX  ;
    register int StepX       = TrPa.StepX    ;
    register int HiThHold    = TrPa.HiThHold ;
    register int LoThHold    = TrPa.LoThHold ;
    register int Is4WayTrace = false;
    const bool   IgMask      = false;
    register int OpLocalThHold = TrPa.OpLocalThHold;

    //IsBlobClear = true;
    if (IsBlobClear) {
        if (Blob!=NULL) { delete[] Blob; Blob=NULL; }
        BlobCount = 0;
        Blob = new TBlob[MaxBbCnt];
    }

    int SortStartId = BlobCount;

    memset(TAddress, 0, IWidth*IHeight);
    memset(MAddress, 0, IWidth*IHeight);

    //--------------------------------------------------------------------------
    // START : 잘못된 영역으로 인해 RETURN
    //--------------------------------------------------------------------------
    if (p1.x<0) p1.x=1; if (p1.y<0) p1.y=1;
    if (p2.x<0) p2.x=1; if (p2.y<0) p2.y=1;
    if (p3.x<0) p3.x=1; if (p3.y<0) p3.y=1;
    if (p4.x<0) p4.x=1; if (p4.y<0) p4.y=1;

    if (p1.x>=Width) p1.x=Width-2; if (p1.y>=Height) p1.y=Height-2;
    if (p2.x>=Width) p2.x=Width-2; if (p2.y>=Height) p2.y=Height-2;
    if (p3.x>=Width) p3.x=Width-2; if (p3.y>=Height) p3.y=Height-2;
    if (p4.x>=Width) p4.x=Width-2; if (p4.y>=Height) p4.y=Height-2;

    //--------------------------------------------------------------------------
    // END : 잘못된 영역으로 인해 RETURN
    //--------------------------------------------------------------------------

    register const POINT nei[8] =
    {
        {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
    };

    register int IncNei = Is4WayTrace ? 2 : 1;
    register int c0, c1, c2, cm, cp;
    register int x0, y0, k, n, PixelCount;

    TPoint *Point = new TPoint[MaxPxCnt];

    if (StepX < 1) StepX = 1;
    if (StepY < 1) StepY = 1;

    JRect rect;
    rect.Clear();
    rect.l = p1.x < p3.x ? p1.x : p3.x;
    rect.t = p1.y < p2.y ? p1.y : p2.y;
    rect.r = p2.x > p4.x ? p2.x : p4.x;
    rect.b = p3.y > p4.y ? p3.y : p4.y;

    register float aTop   = Math.a(p1, p2);
    register float bTop   = Math.b(p1, p2);
    register float aRight = Math.a(p2, p4);
    register float bRight = Math.b(p2, p4);
    register float aBtm   = Math.a(p3, p4);
    register float bBtm   = Math.b(p3, p4);
    register float aLeft  = Math.a(p1, p3);
    register float bLeft  = Math.b(p1, p3);

    int staX, endX;

    if (!TrPa.LoThHold && !TrPa.HiThHold) {
        //LoThHold = GetMountThHold(rect)+3;
        LoThHold = GetMeanThHold(rect)+3;
        if (LoThHold<1) return 0;
    }

    const int MatrixSize = 8;
    int xCuLocalPos = 0;
    int yCuLocalPos = 0;
    int xLocalSize = (p2.x-p1.x)/MatrixSize;
    int yLocalSize = (p4.y-p1.y)/MatrixSize;
    int LocalThHold[MatrixSize][MatrixSize];
    if (OpLocalThHold) {
        JRect LocalRect;
        for (int i=0; i<MatrixSize; i++) {
        for (int j=0; j<MatrixSize; j++) {
            LocalRect.l = rect.l+xLocalSize*i;
            LocalRect.t = rect.t+yLocalSize*j;
            LocalRect.r = rect.l+xLocalSize*i+xLocalSize;
            LocalRect.b = rect.t+yLocalSize*j+yLocalSize;
            LocalThHold[i][j] = GetMeanThHold(LocalRect)+LoThHold;
        }}
    }

    //--------------------------------------------------------------------------
    // START : Ignore Area By Rect
    OnMaskIpArea (p1, p2, p3, p4, 1);
    for (register int i=0; i<IgBlobCount; i++) {
        if (IgBlobCount>=MAX_IGNORE_RECT) {
            AfxMessageBox(_T("IGNORE RECT COUNT 초과"));
            break;
        }

        if (IgBlob[i].lt.x<1 || IgBlob[i].lt.y<1) {
            IgBlob[i].lt.x=IgBlob[i].l; IgBlob[i].lt.y=IgBlob[i].t;
            IgBlob[i].rt.x=IgBlob[i].r; IgBlob[i].rt.y=IgBlob[i].t;
            IgBlob[i].lb.x=IgBlob[i].l; IgBlob[i].lb.y=IgBlob[i].b;
            IgBlob[i].rb.x=IgBlob[i].r; IgBlob[i].rb.y=IgBlob[i].b;

        };

        OnMaskIpArea(IgBlob[i].lt, IgBlob[i].rt, IgBlob[i].lb, IgBlob[i].rb, 0);
    }
    // END : Ignore Area By Rect
    //--------------------------------------------------------------------------

    for (register int y=rect.t+OffsetY+FixMargin; y<rect.b-OffsetY-FixMargin; y+=StepY) {
        if (p1.y > p2.y){
            staX = (y < p1.y) ? aTop  ==0 ? p1.x : (y-bTop  )/aTop   : aLeft ==0 ? p1.x : (y-bLeft )/aLeft ;
            endX = (y < p4.y) ? aRight==0 ? p4.x : (y-bRight)/aRight : aBtm  ==0 ? p4.x : (y-bBtm  )/aBtm  ;
        }
        else {
            staX = (y < p3.y) ? aLeft ==0 ? p1.x : (y-bLeft )/aLeft  : aBtm  ==0 ? p1.x : (y-bBtm  )/aBtm  ;
            endX = (y < p2.y) ? aTop  ==0 ? p2.x : (y-bTop  )/aTop   : aRight==0 ? p2.x : (y-bRight)/aRight;
        }
    for (register int x=staX+OffsetX+FixMargin; x<endX-OffsetX-FixMargin; x+=StepX) {
        if ((x-1)<=0 || x>=Width || y<=0 || y>=Height) continue;

        if (OpLocalThHold) {
            xCuLocalPos = (x-rect.l)/xLocalSize;
            yCuLocalPos = (y-rect.t)/yLocalSize;
            LoThHold = LocalThHold[xCuLocalPos][yCuLocalPos];
        }

        int AP = IElement(x, y);

        bool IsTrace;
        int p[8], p1[8], p2[8];

        c0 = HiThHold ? AP < HiThHold : true && LoThHold ? AP > LoThHold : true;

        IsTrace = c0;
        bool pIsVisited = TElement(x, y) ? true : false;

        bool IsMask = IgMask || MElement(x,y);

        if (IsTrace && !pIsVisited && IsMask) {
            bool IsEdge = false;
            PixelCount = 0;
            x0 = x; y0 = y;
            n = 4;
            do {
                register short u;
                register short v;
                for (k=0; k<8; k+=IncNei, n=((n+IncNei) & 7)) {
                    u = (short)(x + nei[n].x);
                    v = (short)(y + nei[n].y);
                    if ((u-1)<=0  || u>=Width  || v<=0      || v>=Height) continue;

                    int E_AP = IElement(u, v);

                    bool IsMask = IgMask || MElement(u,v);
                    c2 = HiThHold ? E_AP < HiThHold : true && LoThHold ? E_AP > LoThHold : true;
                    IsEdge = (c2==c0) && IsMask;

                    if (IsEdge) break;
                }
                if (k==8) break;

                if (!TElement(x, y)) {
                    Point[PixelCount].x = x;
                    Point[PixelCount].y = y;
                    PixelCount++;
                }

                TElement(x, y) = true;

                if (PixelCount>=MaxPxCnt) break;

                x = x + nei[n].x;
                y = y + nei[n].y;

                n = (n+4+IncNei) & 7;
            }
            while(!(x==x0 && y==y0));

            x=x0; y=y0;

            if (k==8) continue;

            if (PixelCount<MinPxCnt) continue;
            if (PixelCount>MaxPxCnt) continue;

            for (register int k=0; k<PixelCount; k++) {
                JRect *Rect = &Blob[BlobCount].Rect;
                Rect->l = Rect->l<Point[k].x && Rect->l!=0 ? Rect->l : Point[k].x;
                Rect->t = Rect->t<Point[k].y && Rect->t!=0 ? Rect->t : Point[k].y;
                Rect->r = Rect->r>Point[k].x               ? Rect->r : Point[k].x;
                Rect->b = Rect->b>Point[k].y               ? Rect->b : Point[k].y;

                if(k==0) {
                    Blob[BlobCount].lPoint = Point[k];
                    Blob[BlobCount].tPoint = Point[k];
                    Blob[BlobCount].rPoint = Point[k];
                    Blob[BlobCount].bPoint = Point[k];
                }
                else {
                    if (Point[k].x < Blob[BlobCount].lPoint.x) Blob[BlobCount].lPoint = Point[k];
                    if (Point[k].y < Blob[BlobCount].tPoint.y) Blob[BlobCount].tPoint = Point[k];
                    if (Point[k].x > Blob[BlobCount].rPoint.x) Blob[BlobCount].rPoint = Point[k];
                    if (Point[k].y > Blob[BlobCount].bPoint.y) Blob[BlobCount].bPoint = Point[k];
                }
            }

            Blob[BlobCount].Rect.lt.x = Blob[BlobCount].Rect.l; Blob[BlobCount].Rect.lt.y = Blob[BlobCount].Rect.t;
            Blob[BlobCount].Rect.rt.x = Blob[BlobCount].Rect.r; Blob[BlobCount].Rect.rt.y = Blob[BlobCount].Rect.t;
            Blob[BlobCount].Rect.rb.x = Blob[BlobCount].Rect.r; Blob[BlobCount].Rect.rb.y = Blob[BlobCount].Rect.b;
            Blob[BlobCount].Rect.lb.x = Blob[BlobCount].Rect.l; Blob[BlobCount].Rect.lb.y = Blob[BlobCount].Rect.b;

            if (TrPa.OpMinMaxRadious) {
                Blob[BlobCount].MinRadius = 100000;
                Blob[BlobCount].MaxRadius = 0;
                TFPoint C = Blob[BlobCount].Rect.Point();
                for (register int k=0; k<PixelCount; k++) {
                    float dx = Point[k].x-C.x;
                    float dy = Point[k].y-C.y;
                    float CuLg = sqrt(pow(dx, 2) + pow(dy, 2));
                    if (CuLg<Blob[BlobCount].MinRadius) Blob[BlobCount].MinRadius = CuLg;
                    if (CuLg>Blob[BlobCount].MaxRadius) Blob[BlobCount].MaxRadius = CuLg;
                }
            }

            if (IgInSide) {
                for (register int n=Blob[BlobCount].Rect.t; n<Blob[BlobCount].Rect.b; n++) {
                for (register int m=Blob[BlobCount].Rect.l; m<Blob[BlobCount].Rect.r; m++) {
                    TElement(m, n) = true;
                }}
            }

            if (TrPa.IsQuantity2 || TrPa.IsQuantity8) {
                bool r;
                for (register int n=Blob[BlobCount].Rect.t; n<Blob[BlobCount].Rect.b; n++) {
                for (register int m=Blob[BlobCount].Rect.l; m<Blob[BlobCount].Rect.r; m++) {
                    if      (  HiThHold &&   LoThHold) r = IElement(m, n) >  LoThHold && IElement(m, n) <  HiThHold ? true : false;
                    else if ( !HiThHold &&   LoThHold) r = IElement(m, n) >  LoThHold ? true : false;
                    else if (  HiThHold &&  !LoThHold) r = IElement(m, n) <  HiThHold ? true : false;
                    else                               r = IElement(m, n)             ? true : false;

                    if (TrPa.IsQuantity2 && r) Blob[BlobCount].Quantity2++;
                    if (TrPa.IsQuantity8 && r) Blob[BlobCount].Quantity8+=IElement(m, n);
                }}
            }

            int Count = 0;
            if (TrPa.MinPxBlob) {
                for (register int n=Blob[BlobCount].Rect.t-5; n<Blob[BlobCount].Rect.b+5; n++) {
                for (register int m=Blob[BlobCount].Rect.l-5; m<Blob[BlobCount].Rect.r+5; m++) {
                    if (IElement(m, n) >  LoThHold) {
                        Count++;
                    }
                }}
            }

            if (TrPa.MinPxBlob) {
                if (TrPa.MinPxBlob>Count) continue;
            }

            Blob[BlobCount].n = PixelCount;
            BlobCount++;

            if (BlobCount>=MaxBbCnt) break;
        }
        if (BlobCount>=MaxBbCnt) break;
    }
    if (BlobCount>=MaxBbCnt) break;
    }

    //--------------------------------------------------------------------------
    //START : MERGE BLOB간의 간격
    if (TrPa.MgCount) {
        int Gab = TrPa.xMgPxCnt;
        register bool IsxMg, IsyMg;
        JRect r1, r2;
        int sort = 0;
        int tmpBlobCount = BlobCount;
        TPoint SPoint;
        bool IsNeedMerge = false;
        TFPoint lCPoint;

        for (register int k=0; k<tmpBlobCount; k++) {
            sort = 0;
            if (BlobCount<2) break;
            if (Blob[k].Rect.l==0 && Blob[k].Rect.t==0 && Blob[k].Rect.r==0 && Blob[k].Rect.b==0) continue;

            for (register int l=k+1; l<BlobCount; l++) {
                if (l >= BlobCount) break;
                if (l > (tmpBlobCount-1)) break;
                if (Blob[l].Rect.l==0 && Blob[l].Rect.t==0 && Blob[l].Rect.r==0 && Blob[l].Rect.b==0) continue;

                lCPoint = Blob[l].Rect.Point();
                if (Blob[k].Rect.l < lCPoint.x)
                if (Blob[k].Rect.t < lCPoint.y)
                if (Blob[k].Rect.r > lCPoint.x)
                if (Blob[k].Rect.b > lCPoint.y) IsNeedMerge = true;

                if (!IsNeedMerge) {
                    for (register int v=0; v<4; v++) {
                        if (v==0) SPoint = Blob[k].Rect.lt;
                        if (v==1) SPoint = Blob[k].Rect.rt;
                        if (v==2) SPoint = Blob[k].Rect.rb;
                        if (v==3) SPoint = Blob[k].Rect.lb;

                        if ((Math.GetLength(SPoint, Blob[l].Rect.lt))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.lt))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.lt))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.lt))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.rt))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.rt))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.rt))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.rt))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.rb))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.rb))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.rb))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.rb))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.lb))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.lb))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.lb))<=Gab) { IsNeedMerge = true; break; }
                        if ((Math.GetLength(SPoint, Blob[l].Rect.lb))<=Gab) { IsNeedMerge = true; break; }
                    }
                }
                if (!IsNeedMerge) continue;

                Blob[k].n     += Blob[l].n;
                Blob[k].Rect   = Blob[k].Rect.OnMerge(Blob[k].Rect, Blob[l].Rect);
                Blob[l].Rect.l = Blob[l].Rect.t = Blob[l].Rect.r = Blob[l].Rect.b = 0;

                Blob[k].Rect.lt.x = Blob[k].Rect.l; Blob[k].Rect.lt.y = Blob[k].Rect.t;
                Blob[k].Rect.rt.x = Blob[k].Rect.r; Blob[k].Rect.rt.y = Blob[k].Rect.t;
                Blob[k].Rect.rb.x = Blob[k].Rect.r; Blob[k].Rect.rb.y = Blob[k].Rect.b;
                Blob[k].Rect.lb.x = Blob[k].Rect.l; Blob[k].Rect.lb.y = Blob[k].Rect.b;

                BlobCount = BlobCount - 1;
                k = -1;

                break;
            }
            if (IsNeedMerge) {
                for (int j=0; j<BlobCount+1; j++) {
                    if (Blob[j].Rect.l==0 && Blob[j].Rect.t==0 && Blob[j].Rect.r==0 && Blob[j].Rect.b==0) continue;
                    Blob[sort] = Blob[j];
                    sort++;
                }
            }
            IsNeedMerge = false;
        }
    }
    //END : MERGE BLOB간의 간격
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //START : 합성된 BLOB의 크기에 의해 MERGE
    if (TrPa.MgCount) {
        if (TrPa.MgTgHorLength && TrPa.MgTgVerLength) {
            register bool IsxMg, IsyMg ;
            JRect r1, r2;
            int sort = 0;
            int tmpBlobCount = BlobCount;

            for (register int k=0; k<tmpBlobCount; k++) {
                if (Blob[k].Rect.l==0 && Blob[k].Rect.t==0 && Blob[k].Rect.r==0 && Blob[k].Rect.b==0) continue;
                for (register int l=k+1; l<k+TrPa.MgCount; l++) {
                    IsxMg = false; IsyMg = false;
                    if (l > (tmpBlobCount-1)) break;
                    if (Blob[l].Rect.l==0 && Blob[l].Rect.t==0 && Blob[l].Rect.r==0 && Blob[l].Rect.b==0) continue;

                    r1 = Blob[k].Rect;
                    r2 = Blob[l].Rect;

                    JRect mrect = Blob[k].Rect.OnMerge(Blob[k].Rect, Blob[l].Rect);
                    register int HorLength   = mrect.GetWidth();
                    register int VerLength   = mrect.GetHeight();
                    IsxMg = (HorLength<TrPa.MgTgHorLength) ? true : false;
                    IsyMg = (VerLength<TrPa.MgTgVerLength) ? true : false;

                    if (IsxMg && IsyMg) {
                        Blob[k].n     += Blob[l].n;
                        Blob[k].Rect   = mrect    ;
                        Blob[l].Rect.l = Blob[l].Rect.t = Blob[l].Rect.r = Blob[l].Rect.b = 0;
                        BlobCount--;
                        k = -1;
                        break;
                    }
                }
            }

            for (int j=0; j<tmpBlobCount; j++) {
                if (Blob[j].Rect.l==0 && Blob[j].Rect.t==0 && Blob[j].Rect.r==0 && Blob[j].Rect.b==0) continue;
                Blob[sort] = Blob[j];
                sort++;
            }
        }
    }
    //END : 합성된 BLOB의 크기에 의해 MERGE
    //--------------------------------------------------------------------------

    if (!SortStartId) {
        OnSortBlob();
    }
    else {
        OnSortBlob(SortStartId);
    }

    //Check Ignore blob
    if (TrPa.IgBlobFarX   || TrPa.IgBlobFarY   ||
        TrPa.MinHorPitch  || TrPa.MinVerPitch  ||
        TrPa.MinHorLength || TrPa.MinVerLength ||
        TrPa.MaxHorLength || TrPa.MaxVerLength ||
        TrPa.MinPxBlob                         ){
        register bool IsFarX , IsFarY , IsMinPitX , IsMinPitY , IsMinVerLen , IsMinHorLen , IsMaxVerLen , IsMaxHorLen , IsMinPx;
        JRect pr , cr , nr ;
        bool RemBlob ;
        for (register int k=0; k<BlobCount; k++) {
            IsFarX = IsFarY = IsMinPitX = IsMinPitY = IsMinVerLen = IsMinHorLen = IsMaxVerLen = IsMinHorLen = IsMinPx = false ;
            RemBlob = false ;
            pr = k > 0             ? Blob[k-1].Rect : pr ;
            cr =                     Blob[k  ].Rect      ;
            nr = (k+1) < BlobCount ? Blob[k+1].Rect : nr ;

            IsFarX      = TrPa.IgBlobFarX   && cr.GetHorDist(cr,pr)>TrPa.IgBlobFarX && cr.GetHorDist(cr,nr)>TrPa.IgBlobFarX ;
            IsFarY      = TrPa.IgBlobFarY   && cr.GetVerDist(cr,pr)>TrPa.IgBlobFarY && cr.GetVerDist(cr,nr)>TrPa.IgBlobFarY ;
            IsMinPitX   = TrPa.MinHorPitch  && (cr.t <= pr.b && cr.b >= pr.t) ? abs(cr.Point().x - pr.Point().x) < TrPa.MinHorPitch : false ;
            IsMinPitY   = TrPa.MinVerPitch  && (cr.l <= pr.r && cr.r >= pr.l) ? abs(cr.Point().y - pr.Point().y) < TrPa.MinVerPitch : false ;
            IsMinVerLen = TrPa.MinVerLength && TrPa.MinVerLength > abs(cr.b - cr.t) ;
            IsMinHorLen = TrPa.MinHorLength && TrPa.MinHorLength > abs(cr.r - cr.l) ;
            IsMaxVerLen = TrPa.MaxVerLength && TrPa.MaxVerLength < abs(cr.b - cr.t) ;
            IsMaxHorLen = TrPa.MaxHorLength && TrPa.MaxHorLength < abs(cr.r - cr.l) ;
            IsMinPx     = TrPa.MinPxBlob    && TrPa.MinPxBlob    > Blob[k].n        ;
            IsMinPx     = false;
            RemBlob = IsFarX || IsFarY || IsMinPitX || IsMinPitY || IsMinVerLen || IsMinHorLen || IsMaxVerLen || IsMaxHorLen || IsMinPx;

            if (RemBlob) {
                for (int j=k; j<BlobCount; j++) Blob[j] = Blob[j+1];
                BlobCount--;
                k--;
            }
        }
    }

    //if (TrPa.OpWeight) OnWeight();
    delete[] Point;
    return BlobCount;
}

JRect CBoundary::OnFindRectOne(JRect Rect, int MinPxCnt, int MaxPxCnt)
{
    bool IsBreak = false;
    JRect lpRect;
    lpRect.Clear();

    //--------------------------------------------------------------------------
    // START : 잘못된 영역으로 인해 RETURN
    //--------------------------------------------------------------------------
    if (Rect.l<0 || Rect.l>=IWidth ) return lpRect;
    if (Rect.t<0 || Rect.t>=IHeight) return lpRect;
    if (Rect.r<0 || Rect.r>=IWidth ) return lpRect;
    if (Rect.b<0 || Rect.b>=IHeight) return lpRect;
    //--------------------------------------------------------------------------
    // END : 잘못된 영역으로 인해 RETURN
    //--------------------------------------------------------------------------

    register const POINT nei[8] =
    {
        {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
    };

    register int c0, c1, c2, cm, cp;
    register int x0, y0, k, n, PixelCount;

    TPoint *Point = new TPoint[MaxPxCnt];

    for (register int y=Rect.t; y<Rect.b; y++) {
    for (register int x=Rect.l; x<Rect.r; x++) {
        bool IsTrace;
        int p[8], p1[8], p2[8];
        c0 = TrPa.HiThHold ? IElement(x    ,   y  ) < TrPa.HiThHold : true && TrPa.LoThHold ? IElement( x   ,  y   ) > TrPa.LoThHold : true;
        //c1 = HiThHold ? IElement((x-1),   y  ) < HiThHold : true && LoThHold ? IElement((x-1),  y   ) > LoThHold : true;
        //cm = HiThHold ? IElement(x    , (y-1)) < HiThHold : true && LoThHold ? IElement( x   , (y-1)) > LoThHold : true;
        //cp = HiThHold ? IElement(x    , (y+1)) < HiThHold : true && LoThHold ? IElement( x   , (y+1)) > LoThHold : true;

        IsTrace = c0;
        bool pIsVisited = TElement(x, y) ? true : false;

        if (IsTrace && !pIsVisited) {
            bool IsEdge = false;
            PixelCount = 0;
            x0 = x; y0 = y;
            n = 4;
            do {
                register short u;
                register short v;
                for (k=0; k<8; k++, n=((n+1) & 7)) {
                    u = (short)(x + nei[n].x);
                    v = (short)(y + nei[n].y);
                    if ((u-1)<=0 || u>=IWidth || v<=0 || v>=IHeight) continue;

                    c2 = TrPa.HiThHold ? IElement(u, v) < TrPa.HiThHold : true && TrPa.LoThHold ? IElement(u, v) > TrPa.LoThHold : true;
                    IsEdge = (c2==c0);
                    if (IsEdge) break;
                }
                if (k==8) break;

                if (!TElement(x, y)) {
                    Point[PixelCount].x = x;
                    Point[PixelCount].y = y;
                    PixelCount++;
                }

                TElement(x, y) = true;

                if (PixelCount>=MaxPxCnt) break;

                x = x + nei[n].x;
                y = y + nei[n].y;

                n = (n+4+1) & 7;
            }
            while(!(x==x0 && y==y0));

            x=x0; y=y0;

            if (k==8) continue;

            if (PixelCount<MinPxCnt) continue;
            if (PixelCount>MaxPxCnt) continue;

            for (register int k=0; k<PixelCount; k++) {
                lpRect.l = lpRect.l<Point[k].x && lpRect.l!=0 ? lpRect.l : Point[k].x;
                lpRect.t = lpRect.t<Point[k].y && lpRect.t!=0 ? lpRect.t : Point[k].y;
                lpRect.r = lpRect.r>Point[k].x                ? lpRect.r : Point[k].x;
                lpRect.b = lpRect.b>Point[k].y                ? lpRect.b : Point[k].y;
            }

            IsBreak = true;
            break;
        }
        if (IsBreak) break;
    }
    if (IsBreak) break;
    }

    delete[] Point;
    return lpRect;
}

void CBoundary::OnWeightBga()
{
    if (BlobCount<1) return;

    register int HiThHold = TrPa.HiThHold;
    register int LoThHold = TrPa.LoThHold;

    int xWeight[MAX_WEIGHT_PIXEL], yWeight[MAX_WEIGHT_PIXEL];
    bool r1, r2;
    for(int i=0; i<MAX_WEIGHT_PIXEL; i++){xWeight[i]=0; yWeight[i]=0;}

    for (register int k=0; k<BlobCount; k++) {
        int xTotalPixel= 0      , yTotalPixel= 0      ;
        int xMin       = 1000000, yMin       = 1000000;
        int xCount     = 0      , yCount     = 0      ;
        int Totalx     = 0      , Totaly     = 0      ;
        int x          = 0      , y          = 0      ;

        //Calculate Total Pixel Value
        if (TrPa.OpGrayWeight){
            for (register int n=Blob[k].Rect.t; n<=Blob[k].Rect.b; n++) {
                for (register int m=Blob[k].Rect.l; m<=Blob[k].Rect.r; m++) {
                    yTotalPixel +=IElement(m, n);
                }
                if(yCount==0) y = yTotalPixel    ;
                yWeight[yCount] = yTotalPixel    ;
                Totaly         += yWeight[yCount];
                yCount++                         ;
                yTotalPixel     = 0              ;
            }
            for(int i=1; i<yCount-1; i++){
                int yValue = yWeight[i];
                int yWeightValue = abs(Totaly - (yValue+ y*2));
                if(yWeightValue < yMin){
                    yMin =  yWeightValue;
                    Blob[k].WPoint.y = Blob[k].Rect.t + i;
                }
                y +=yValue;
            }

            for (register int m=Blob[k].Rect.l; m<=Blob[k].Rect.r; m++) {
                for (register int n=Blob[k].Rect.t; n<=Blob[k].Rect.b; n++) {
                    xTotalPixel +=IElement(m, n);
                }
                if (xCount>=(MAX_WEIGHT_PIXEL-1)) break; 

                if(xCount==0) x = xTotalPixel    ;
                xWeight[xCount] = xTotalPixel    ;
                Totalx         += xWeight[xCount];
                xCount++                         ;
                xTotalPixel     = 0              ;
            }
            for(int i=1; i<xCount-1; i++){
                int xValue = xWeight[i];
                int xWeightValue = abs(Totalx - (xValue+ x*2));
                if(xWeightValue < xMin){
                    xMin =  xWeightValue;
                    Blob[k].WPoint.x = Blob[k].Rect.l + i;
                }
                x +=xValue;
            }
        }
        else {
            for (register int n=Blob[k].Rect.t; n<=Blob[k].Rect.b; n++) {
                for (register int m=Blob[k].Rect.l; m<=Blob[k].Rect.r; m++) {
                    if      (  HiThHold &&   LoThHold) r1 = IElement(m, n) >  LoThHold && IElement(m, n) <  HiThHold ? true : false;
                    else if ( !HiThHold &&   LoThHold) r1 = IElement(m, n) >  LoThHold ? true : false;
                    else if (  HiThHold &&  !LoThHold) r1 = IElement(m, n) <  HiThHold ? true : false;
                    else                               r1 = IElement(m, n)             ? true : false;

                    if(r1) yTotalPixel +=IElement(m, n);
                }
                if(yCount==0) y = yTotalPixel    ;
                yWeight[yCount] = yTotalPixel    ;
                Totaly         += yWeight[yCount];
                yCount++                         ;
                yTotalPixel     = 0              ;
            }
            for(int i=1; i<yCount-1; i++){
                int yValue = yWeight[i];
                int yWeightValue = abs(Totaly - (yValue+ y*2));
                if(yWeightValue < yMin){
                    yMin =  yWeightValue;
                    Blob[k].WPoint.y = Blob[k].Rect.t + i;
                }
                y +=yValue;
            }

            for (register int m=Blob[k].Rect.l; m<=Blob[k].Rect.r; m++) {
                for (register int n=Blob[k].Rect.t; n<=Blob[k].Rect.b; n++) {
                    if      (  HiThHold &&   LoThHold) r2 = IElement(m, n) >  LoThHold && IElement(m, n) <  HiThHold ? true : false;
                    else if ( !HiThHold &&   LoThHold) r2 = IElement(m, n) >  LoThHold ? true : false;
                    else if (  HiThHold &&  !LoThHold) r2 = IElement(m, n) <  HiThHold ? true : false;
                    else                               r2 = IElement(m, n)             ? true : false;

                    if(r2) xTotalPixel +=IElement(m, n);
                }
                if(xCount==0) x = xTotalPixel    ;
                xWeight[xCount] = xTotalPixel    ;
                Totalx         += xWeight[xCount];
                xCount++                         ;
                xTotalPixel     = 0              ;
            }
            for(int i=1; i<xCount-1; i++){
                int xValue = xWeight[i];
                int xWeightValue = abs(Totalx - (xValue+ x*2));
                if(xWeightValue < xMin){
                    xMin =  xWeightValue;
                    Blob[k].WPoint.x = Blob[k].Rect.l + i;
                }
                x +=xValue;
            }
        }
    }
}

int CBoundary::OnFindBlobWidthMElement(JRect rect, bool IsBlobClear)
{
    TPoint p1, p2, p3, p4;

    if (   rect.lt.x && rect.rt.x && rect.rb.x && rect.lb.x
        && rect.lt.y && rect.rt.y && rect.rb.y && rect.lb.y) {
        p1 = rect.lt;
        p2 = rect.rt;
        p3 = rect.lb;
        p4 = rect.rb;
    }
    else {
        p1.x = rect.l; p1.y = rect.t;
        p2.x = rect.r; p2.y = rect.t;
        p3.x = rect.l; p3.y = rect.b;
        p4.x = rect.r; p4.y = rect.b;
    }
    return OnFindBlobWidthMElement(p1, p2, p3, p4,IsBlobClear);
}

int CBoundary::OnFindBlobWidthMElement(TPoint p1, TPoint p2, TPoint p3, TPoint p4, bool IsBlobClear)
{
    register int Width       = IWidth ;
    register int Height      = IHeight;
    register int IgInSide    = true;
    register int MaxBbCnt    = 100   ;
    register int MinPxCnt    = 8;
    register int MaxPxCnt    = 10000;
    register int OffsetY     = 1;
    register int StepY       = 2;
    register int OffsetX     = 1;
    register int StepX       = 1;
    register int Is4WayTrace = false;

    if(IsBlobClear){
        if (MBlob!=NULL) { delete[] MBlob; MBlob=NULL; }
        MBlobCount = 0;
        MBlob = new TMBlob[MaxBbCnt];
    }
    memset(TAddress, 0, IWidth*IHeight);
    memset(MAddress, 0, IWidth*IHeight);

    //--------------------------------------------------------------------------
    // START : 잘못된 영역으로 인해 RETURN
    //--------------------------------------------------------------------------
    if (p1.x<0 || p1.x>=Width) return false;
    if (p2.x<0 || p2.x>=Width) return false;
    if (p3.x<0 || p3.x>=Width) return false;
    if (p4.x<0 || p4.x>=Width) return false;

    if (p1.y<0 || p1.y>=Height) return false;
    if (p2.y<0 || p2.y>=Height) return false;
    if (p3.y<0 || p3.y>=Height) return false;
    if (p4.y<0 || p4.y>=Height) return false;
    //--------------------------------------------------------------------------
    // END : 잘못된 영역으로 인해 RETURN
    //--------------------------------------------------------------------------

    register const POINT nei[8] =
    {
        {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
    };

    register int IncNei = Is4WayTrace ? 2 : 1;
    register int c0, c1, c2, cm, cp;
    register int x0, y0, k, n, PixelCount;

    TPoint *Point = new TPoint[MaxPxCnt];

    if (StepX < 1) StepX = 1;
    if (StepY < 1) StepY = 1;

    JRect rect;
    rect.Clear();
    rect.l = p1.x < p3.x ? p1.x : p3.x;
    rect.t = p1.y < p2.y ? p1.y : p2.y;
    rect.r = p2.x > p4.x ? p2.x : p4.x;
    rect.b = p3.y > p4.y ? p3.y : p4.y;

    register float aTop   = Math.a(p1, p2);
    register float bTop   = Math.b(p1, p2);
    register float aRight = Math.a(p2, p4);
    register float bRight = Math.b(p2, p4);
    register float aBtm   = Math.a(p3, p4);
    register float bBtm   = Math.b(p3, p4);
    register float aLeft  = Math.a(p1, p3);
    register float bLeft  = Math.b(p1, p3);

    int staX, endX;

    for (register int k=0; k<IpPixelCount; k++) {
        MElement(IpPixel[k].x,IpPixel[k].y) = 1;
    }

    for (register int y=rect.t+OffsetY; y<rect.b-OffsetY; y+=StepY) {
        if (p1.y > p2.y){
            staX = (y < p1.y) ? aTop  ==0 ? p1.x : (y-bTop  )/aTop   : aLeft ==0 ? p1.x : (y-bLeft )/aLeft ;
            endX = (y < p4.y) ? aRight==0 ? p4.x : (y-bRight)/aRight : aBtm  ==0 ? p4.x : (y-bBtm  )/aBtm  ;
        }
        else {
            staX = (y < p3.y) ? aLeft ==0 ? p1.x : (y-bLeft )/aLeft  : aBtm  ==0 ? p1.x : (y-bBtm  )/aBtm  ;
            endX = (y < p2.y) ? aTop  ==0 ? p2.x : (y-bTop  )/aTop   : aRight==0 ? p2.x : (y-bRight)/aRight;
        }
    for (register int x=staX+OffsetX; x<endX-OffsetX; x+=StepX) {
        if ((x-1)<=0      || x>=Width  || y<=0      || y>=Height) continue;
        //if ((x-1)<=rect.l || x>=rect.r || y<=rect.t || y>=rect.b) continue;

        int p[8], p1[8], p2[8];
        bool IsTrace = MElement(x,y);
        bool pIsVisited = TElement(x, y) ? true : false;

        if (IsTrace && !pIsVisited) {
            bool IsEdge = false;
            PixelCount = 0;
            x0 = x; y0 = y;
            n = 4;
            do {
                register short u;
                register short v;
                for (k=0; k<8; k+=IncNei, n=((n+IncNei) & 7)) {
                    u = (short)(x + nei[n].x);
                    v = (short)(y + nei[n].y);
                    if ((u-1)<=0 || u>=Width || v<=0 || v>=Height) continue;

                    IsEdge = MElement(u, v);
                    if (IsEdge) break;
                }
                if (k==8) break;

                if (!TElement(x, y)) {
                    Point[PixelCount].x = x;
                    Point[PixelCount].y = y;
                    PixelCount++;
                }

                TElement(x, y) = true;

                if (PixelCount>=MaxPxCnt) break;

                x = x + nei[n].x;
                y = y + nei[n].y;

                n = (n+4+IncNei) & 7;
            }
            while(!(x==x0 && y==y0));

            x=x0; y=y0;

            if (k==8) continue;

            if (PixelCount<MinPxCnt) continue;
            if (PixelCount>MaxPxCnt) continue;

            for (register int k=0; k<PixelCount; k++) {
                JRect *Rect = &MBlob[MBlobCount].Rect;
                Rect->l = Rect->l<Point[k].x && Rect->l!=0 ? Rect->l : Point[k].x;
                Rect->t = Rect->t<Point[k].y && Rect->t!=0 ? Rect->t : Point[k].y;
                Rect->r = Rect->r>Point[k].x               ? Rect->r : Point[k].x;
                Rect->b = Rect->b>Point[k].y               ? Rect->b : Point[k].y;

                if(k==0) {
                    MBlob[MBlobCount].lPoint = Point[k];
                    MBlob[MBlobCount].tPoint = Point[k];
                    MBlob[MBlobCount].rPoint = Point[k];
                    MBlob[MBlobCount].bPoint = Point[k];
                }
                else {
                    if (Point[k].x < MBlob[MBlobCount].lPoint.x) MBlob[MBlobCount].lPoint = Point[k];
                    if (Point[k].y < MBlob[MBlobCount].tPoint.y) MBlob[MBlobCount].tPoint = Point[k];
                    if (Point[k].x > MBlob[MBlobCount].rPoint.x) MBlob[MBlobCount].rPoint = Point[k];
                    if (Point[k].y > MBlob[MBlobCount].bPoint.y) MBlob[MBlobCount].bPoint = Point[k];
                }
            }

            if (IgInSide) {
                for (register int n=MBlob[MBlobCount].Rect.t; n<MBlob[MBlobCount].Rect.b; n++) {
                for (register int m=MBlob[MBlobCount].Rect.l; m<MBlob[MBlobCount].Rect.r; m++) {
                    TElement(m, n) = true;
                }}
            }

            bool OpSumPixel=false;
            if (OpSumPixel) {
                int SumPixel = 0;
                for (register int n=MBlob[MBlobCount].Rect.t; n<MBlob[MBlobCount].Rect.b; n++) {
                for (register int m=MBlob[MBlobCount].Rect.l; m<MBlob[MBlobCount].Rect.r; m++) {
                    if (MElement(m, n)) SumPixel++;
                }}
                if (SumPixel<MinPxCnt) continue;
            }

            MBlob[MBlobCount].n = PixelCount;
            MBlobCount++;

            if (MBlobCount>=MaxBbCnt) break;
        }
        if (MBlobCount>=MaxBbCnt) break;
    }
    if (MBlobCount>=MaxBbCnt) break;
    }

    delete[] Point;
    return MBlobCount;
}

void CBoundary::OnPaint(TDrawTool Tool)
{
    OnPaintBlobId  (Tool);
    OnPaintBlobRect(Tool);

    TDrawTool IgTool = Tool;

    IgTool.Color = Tool.Color;

    if (TrPa.UseIgnoreBlob) OnPaintIgBlob(IgTool);
}

void CBoundary::OnPaintBlobTag(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    for (register int k=0; k<BlobCount; k++) {
        int px = Blob[k].Rect.r;
        int py = Blob[k].Rect.t;
        int Tag = Blob[k].Tag;
        Gdi->CText.Color = BLUE_COLOR;

        if      (Tag==200) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("" ))));
        else if (Tag==201) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("!"))));
		else if (Tag==202) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("@"))));
		else if (Tag==203) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("#"))));
		else if (Tag==204) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("$"))));
		else if (Tag==205) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("%"))));
		else if (Tag==206) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("^"))));
		else if (Tag==207) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("&"))));
		else if (Tag==208) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("*"))));
		else if (Tag==209) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("("))));
		else if (Tag==210) Gdi->OnText(px, py, CString(LPSTR(LPCSTR(")"))));
		else if (Tag==211) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("-"))));
		else if (Tag==212) Gdi->OnText(px, py, CString(LPSTR(LPCSTR("+"))));
		else               Gdi->OnText(px, py, CString(LPSTR(LPCSTR(Tag))));
    }

    delete Gdi;
}

void CBoundary::OnPaintBlobId(TDrawTool Tool)
{
    HGdi *Gdi = new HGdi(Tool.Handle);
    Gdi->CPen.Color = Tool.Color;

    for (register int k=0; k<BlobCount; k++) {
        int px = Blob[k].Rect.l;
        int py = Blob[k].Rect.t;
        Gdi->OnText(px, py, CString(LPSTR(LPCSTR(k))));
    }

    delete Gdi;
}

void CBoundary::OnPaintBlobRect(TDrawTool Tool, TPoint P1, TPoint P2, TPoint P3, TPoint P4, COLORREF Color)
{
    if (Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    Gdi->CBrush.Color = Color;
    Gdi->CBrush.Style = BS_HOLLOW;

    Gdi->OnLine(P1, P2);
    Gdi->OnLine(P2, P4);
    Gdi->OnLine(P4, P3);
    Gdi->OnLine(P3, P1);

    delete Gdi;
}

void CBoundary::OnPaintBlobRectConer(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    Gdi->CBrush.Color = Tool.Color;
    Gdi->CBrush.Style = BS_HOLLOW;

    for (register int k=0; k<BlobCount; k++) {
        Gdi->OnLine(Blob[k].ltPoint, Blob[k].rtPoint);
        Gdi->OnLine(Blob[k].rtPoint, Blob[k].rbPoint);
        Gdi->OnLine(Blob[k].rbPoint, Blob[k].lbPoint);
        Gdi->OnLine(Blob[k].lbPoint, Blob[k].ltPoint);
    }

    delete Gdi;
}

void CBoundary::OnPaintBlobCircle(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    Gdi->CBrush.Style = BS_HOLLOW;

    for (register int k=0; k<BlobCount; k++) {
        JRect Rect = Blob[k].Rect;
        TPoint C;

        C.x = (int)Blob[k].C.x+0.5;
        C.y = (int)Blob[k].C.y+0.5;
        double Radius = Blob[k].Radius+0.5;

        Gdi->OnCircle(0, C, Radius, Radius);

        Gdi->OnText(Rect.l, Rect.t, CString(LPSTR(LPCSTR(k))));
    }
    delete Gdi;
}

void CBoundary::OnPaintBlobWeight(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    Gdi->CBrush.Style = BS_HOLLOW;

    for (register int k=0; k<BlobCount; k++) {
        TPoint C = Blob[k].WPoint;

        C.x += 0.5;
        C.y += 0.5;

        Gdi->OnLinePlus(C, 4);
    }
    delete Gdi;
}

void CBoundary::OnPaintBlobRect(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    Gdi->CBrush.Style = BS_HOLLOW;

    for (register int k=0; k<BlobCount; k++) {
        int Tag = Blob[k].Tag;

        if (Tag==200) {
            Gdi->CPen.Color = GRAYGREEN;
            Gdi->CBrush.Style = BS_HOLLOW;
            Gdi->CBrush.Hatch = BS_HATCHED;
        }
        else {
            Gdi->CPen.Color = LIME_COLOR;
            Gdi->CBrush.Style = BS_HOLLOW;
        }

        JRect Rect = Blob[k].Rect;

        if      (Rect.r-Rect.l<5) Gdi->OnRect(0, Rect.l-2, Rect.t-2, Rect.r+2, Rect.b+2, 0);
        else if (Rect.b-Rect.t<5) Gdi->OnRect(0, Rect.l-2, Rect.t-2, Rect.r+2, Rect.b+2, 0);
        else                      Gdi->OnRect(0, Rect.l  , Rect.t  , Rect.r+1, Rect.b+1, 0);
        Gdi->OnText(Rect.l, Rect.t, CString(LPSTR(LPCSTR(k))));
    }
    delete Gdi;
}

void CBoundary::TBlob::OnPaintBlobId(TDrawTool Tool, COLORREF Color, CString Id)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;
    //Gdi->CBrush.Style = lbStyle;
    //Gdi->CBrush.Hatch = lbHatch;

    Gdi->OnText(Rect.l+10, Rect.t+10, LPWSTR(LPCWSTR(Id)));

    delete Gdi;
}

void CBoundary::TBlob::OnPaintBlobRect(TDrawTool Tool, COLORREF Color, int lbStyle, int lbHatch)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    Gdi->CBrush.Style = lbStyle;
    Gdi->CBrush.Hatch = lbHatch;

    if (Tag==200) Gdi->CPen.Color = GRAYGREEN;
    else          Gdi->CPen.Color = Color;

    Gdi->OnRect(0, Rect.l, Rect.t, Rect.r+1, Rect.b+1, 0);

    delete Gdi;
}

void CBoundary::OnPaintIgBlob(TDrawTool Tool, int lbStyle, int lbHatch)
{
    if (IgBlobCount<1) return;
    if (IgBlob==NULL) return;

	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    Gdi->CBrush.Color = Tool.Color;
    Gdi->CBrush.Style = lbStyle;
    Gdi->CBrush.Hatch = lbHatch;

    for (int i=0; i<IgBlobCount; i++) {
        if (IgBlob[i].lt.x>0) {
            Gdi->OnLine(IgBlob[i].lt, IgBlob[i].rt);
            Gdi->OnLine(IgBlob[i].rt, IgBlob[i].rb);
            Gdi->OnLine(IgBlob[i].rb, IgBlob[i].lb);
            Gdi->OnLine(IgBlob[i].lb, IgBlob[i].lt);
        }
        else {
            Gdi->OnRect(0, IgBlob[i].l, IgBlob[i].t, IgBlob[i].r+1, IgBlob[i].b+1, 0);
        }
    }
    delete Gdi;
}

void CBoundary::OnPaintIgBlobAsCircle(TDrawTool Tool, bool IsFull)
{
    if (IgBlobCount<1) return;
    if (IgBlob==NULL) return;

	if(Tool.Handle =NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    Gdi->CBrush.Color = Tool.Color;

    for (int k=0; k<IgBlobCount; k++) {
        TPoint C;
        C.x = (int)IgBlob[k].Point().x+0.5;
        C.y = (int)IgBlob[k].Point().y+0.5;
        double Radius = IgBlob[k].GetWidth()/2.0+0.5;
        Gdi->OnCircle(IsFull, C, Radius, Radius);
    }
    delete Gdi;
}

void CBoundary::OnPaintBlobPixel(TDrawTool Tool, COLORREF FColor, COLORREF BColor)
{
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    for (int i=0; i<BlobCount; i++) {
        int Tag = Blob[i].Tag;
        if (Tag==200) {
            for (int u=Blob[i].Rect.l; u<Blob[i].Rect.r; u++) {
            for (int v=Blob[i].Rect.t; v<Blob[i].Rect.b; v++) {
                BYTE Px = IElement(u, v);
                TPoint Point(u, v);
                if (TrPa.LoThHold>0) Gdi->OnPixel(Point, Px>TrPa.LoThHold ? GRAYGREEN : BColor);
                if (TrPa.HiThHold>0) Gdi->OnPixel(Point, Px>TrPa.HiThHold ? GRAYGREEN : BColor);
            }}
        }
        else {
            for (int u=Blob[i].Rect.l; u<Blob[i].Rect.r; u++) {
            for (int v=Blob[i].Rect.t; v<Blob[i].Rect.b; v++) {
                BYTE Px = IElement(u, v);
                TPoint Point(u, v);
                if (TrPa.LoThHold>0) Gdi->OnPixel(Point, Px>TrPa.LoThHold ? FColor : BColor);
                if (TrPa.HiThHold>0) Gdi->OnPixel(Point, Px>TrPa.HiThHold ? FColor : BColor);
            }}
        }
    }

    delete Gdi;
}

void CBoundary::OnPaintMkPixel(TDrawTool Tool, COLORREF FColor)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    for (int y=0; y<IHeight; y++) {
    for (int x=0; x<IWidth ; x++) {
        bool IsMasking = MElement(x,y);
        if (IsMasking) {
            TPoint MkPx(x,y);
            Gdi->OnPixel(MkPx, FColor);
        }
    }}

    delete Gdi;
}

void CBoundary::OnPaintBlobPixel(JRect Rect, TDrawTool Tool, bool IsBright, bool IsDark, int ThHold, COLORREF BrightColor, COLORREF DarkColor)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    for (int u=Rect.l; u<Rect.r; u++) {
    for (int v=Rect.t; v<Rect.b; v++) {
        BYTE Px = IElement(u, v);
        TPoint Point(u, v);
        if (IsBright) {
            if      (Px>ThHold) Gdi->OnPixel(Point, BrightColor);
            else                Gdi->OnPixel(Point, BLACK      );
        }
        if (IsDark) {
            if      (Px<ThHold) Gdi->OnPixel(Point, DarkColor);
            else                Gdi->OnPixel(Point, BLACK    );
        }
    }}

    delete Gdi;
}

void CBoundary::OnLoad(bool IsLoad, CString Path)
{
    //JMS (nonoboy)
    TrPa.OffsetY = 1;
    TrPa.StepY   = 1;
    TrPa.OffsetX = 1;
    TrPa.StepX   = 1;

	CIFile Ini;

    Ini.Load(IsLoad, Path, "TrPa", "IgSort       ",  &TrPa.IgSort        );
    Ini.Load(IsLoad, Path, "TrPa", "SMODE        ",  &TrPa.SMode      );
    Ini.Load(IsLoad, Path, "TrPa", "IgInSide     ",  &TrPa.IgInSide      );
    //Ini.Load(IsLoad, Path, "TrPa", "MaxBbCnt     ",  &TrPa.MaxBbCnt      );
    Ini.Load(IsLoad, Path, "TrPa", "MinPxCnt     ",  &TrPa.MinPxCnt      );
    //Ini.Load(IsLoad, Path, "TrPa", "MaxPxCnt     ",  &TrPa.MaxPxCnt      );
    //Ini.Load(IsLoad, Path, "TrPa", "OffsetY      ",  &TrPa.OffsetY       );
    //Ini.Load(IsLoad, Path, "TrPa", "StepY        ",  &TrPa.StepY         );
    //Ini.Load(IsLoad, Path, "TrPa", "OffsetX      ",  &TrPa.OffsetX       );
    //Ini.Load(IsLoad, Path, "TrPa", "StepX        ",  &TrPa.StepX         );
    Ini.Load(IsLoad, Path, "TrPa", "MgCount      ",  &TrPa.MgCount       );
    Ini.Load(IsLoad, Path, "TrPa", "xMgPxCnt     ",  &TrPa.xMgPxCnt      );
    Ini.Load(IsLoad, Path, "TrPa", "yMgPxCnt     ",  &TrPa.yMgPxCnt      );
    Ini.Load(IsLoad, Path, "TrPa", "MgTgHorLength",  &TrPa.MgTgHorLength );
    Ini.Load(IsLoad, Path, "TrPa", "MgTgVerLength",  &TrPa.MgTgVerLength );
    Ini.Load(IsLoad, Path, "TrPa", "HiThHold     ",  &TrPa.HiThHold      );
    Ini.Load(IsLoad, Path, "TrPa", "LoThHold     ",  &TrPa.LoThHold      );
    Ini.Load(IsLoad, Path, "TrPa", "OpGrayWeight ",  &TrPa.OpGrayWeight  );


    Ini.Load(IsLoad, Path, "TrPa", "MinHorLength ",  &TrPa.MinHorLength  );
    Ini.Load(IsLoad, Path, "TrPa", "MaxHorLength ",  &TrPa.MaxHorLength  );
    Ini.Load(IsLoad, Path, "TrPa", "MinVerLength ",  &TrPa.MinVerLength  );
    Ini.Load(IsLoad, Path, "TrPa", "MaxVerLength ",  &TrPa.MaxVerLength  );
    Ini.Load(IsLoad, Path, "TrPa", "MinPxBlob    ",  &TrPa.MinPxBlob     );
    Ini.Load(IsLoad, Path, "TrPa", "IsQuantity2  ",  &TrPa.IsQuantity2   );
    //Ini.Load(IsLoad, Path, "TrPa", "IsQuantity8  ",  &TrPa.IsQuantity8   );

    Ini.Load(IsLoad, Path, "TrPa", "OpfiMax33", &TrPa.OpfiMax33);
    Ini.Load(IsLoad, Path, "TrPa", "IsFindAnd", &TrPa.IsFindAnd);
}
//---------------------------------------------------------------------------
// Class BlobTable
CBlobTable::CBlobTable()
{
}


CBlobTable::~CBlobTable()
{
}
int CBlobTable::Width    (TBlob* Blob) { return Blob->Rect.r-Blob->Rect.l; }
int CBlobTable::Height   (TBlob* Blob) { return Blob->Rect.b-Blob->Rect.t; }
int CBlobTable::Demension(TBlob* Blob) { return (Blob->Rect.r - Blob->Rect.l) * (Blob->Rect.b - Blob->Rect.t); }
CBoundary::TBlob* CBlobTable::lpBigWidth (TBlob* b1, TBlob* b2) { return ((b1->Rect.r - b1->Rect.l) > (b2->Rect.r - b2->Rect.l)) ? b1 : b2; }
CBoundary::TBlob* CBlobTable::lpBigHeight(TBlob* b1, TBlob* b2) { return ((b1->Rect.b - b1->Rect.t) > (b2->Rect.b - b2->Rect.t)) ? b1 : b2; }

CBoundary::TBlob* CBlobTable::lpMinWidthHeightRatio()
{
    float min = 0; int n = 0;
    for (int i=0; i<BlobCount; i++) {
        float xlen = Blob[i].Rect.GetWidth();
        float ylen = Blob[i].Rect.GetHeight();
        if (xlen==0) xlen = 1;
        if (ylen==0) ylen = 1;
        float v = fabs(1 - (xlen<ylen) ? fabs(ylen / xlen) : fabs(xlen / ylen));
        if (i==0) {min = v, n = i;}
        if (v < min) {
            min = v; n = i;
        }
    }
    return &Blob[n];
}

CBoundary::TBlob* CBlobTable::lpMaxWidthBlob()
{
    int max = 0, n = 0;
    for (int i=0; i<BlobCount; i++) {
        int v = Width(&Blob[i]);
        if (v > max) {
            max = v; n = i;
        }
    }
    return &Blob[n];
}

CBoundary::TBlob* CBlobTable::lpMaxHeightBlob()
{
    int max = 0, n = 0;
    for (int i=0; i<BlobCount; i++) {
        int v = Height(&Blob[i]);
        if (v > max) {
            max = v; n = i;
        }
    }
    return &Blob[n];
}

CBoundary::TBlob* CBlobTable::lpMaxDemensionBlob()
{
    int max = 0, n = 0;
    for (int i=0; i<BlobCount; i++) {
        int v = Demension(&Blob[i]);
        if (v > max) {
            max = v; n = i;
        }
    }
    return &Blob[n];
}

JRect CBlobTable::OnMerge()
{
    if (BlobCount==1)
        return Blob[0].Rect;

    JRect MgRect;
    for (int i=0; i<BlobCount-1; i++) {
        if (i==0) MgRect = MgRect.OnMerge(Blob[i].Rect, Blob[i+1].Rect);
        else MgRect = MgRect.OnMerge(MgRect, Blob[i+1].Rect);
    }

    return MgRect;
}

int CBlobTable::OnDemensionRatio(JRect R1, JRect R2)
{
    int r1d = R1.GetDemension();
    int r2d = R2.GetDemension();
    return (r1d*100)/r2d;
}

int CBlobTable::OnCompareRatio(JRect R1, JRect R2)
{
    int r1d = R1.GetDemension();
    int r2d = R2.GetDemension();

    int dv = (r1d>r2d) ? r1d : r2d;
    return (abs(r1d-r2d))*100/dv;
}

int CBlobTable::GetPxCount(JRect rect, int HiThHold, int LoThHold)
{
    int n = 0;
    for (register int i=rect.l; i<rect.r; i++) {
    for (register int j=rect.t; j<rect.b; j++) {
        BYTE Dg = IElement(i, j);
        if      ( HiThHold &&  LoThHold) {if (Dg > LoThHold && Dg < HiThHold) n++;}
        else if (!HiThHold &&  LoThHold) {if (Dg > LoThHold                 ) n++;}
        else if ( HiThHold && !LoThHold) {if (Dg < HiThHold                 ) n++;}
        else                                                                  n++;
    }}
    return n;
}

int CBlobTable::GetPxAvg(JRect rect, int HiThHold, int LoThHold)
{
    int n = 0;
    int sum = 0;
    for (register int i=rect.l; i<rect.r; i++) {
    for (register int j=rect.t; j<rect.b; j++) {
        BYTE Dg = IElement(i, j);
        if      ( HiThHold &&  LoThHold) {if (Dg > LoThHold && Dg < HiThHold) {n++; sum+=Dg;}}
        else if (!HiThHold &&  LoThHold) {if (Dg > LoThHold                 ) {n++; sum+=Dg;}}
        else if ( HiThHold && !LoThHold) {if (Dg < HiThHold                 ) {n++; sum+=Dg;}}
        else                                                                  {n++; sum+=Dg;}
    }}
    return n==0? 0 : sum/n;
}

int CBlobTable::GetPxMax(JRect rect, int HiThHold, int LoThHold)
{
    int max = 0;
    for (register int i=rect.l; i<rect.r; i++) {
    for (register int j=rect.t; j<rect.b; j++) {
        BYTE Dg = IElement(i, j);
        if      ( HiThHold &&  LoThHold) {if (Dg > LoThHold && Dg < HiThHold && Dg > max) max = Dg;}
        else if (!HiThHold &&  LoThHold) {if (Dg > LoThHold                  && Dg > max) max = Dg;}
        else if ( HiThHold && !LoThHold) {if (Dg < HiThHold                  && Dg > max) max = Dg;}
        else if (!HiThHold && !LoThHold) {if (                                  Dg > max) max = Dg;}
    }}
    return max;
}

int CBlobTable::GetPxMin(JRect rect, int HiThHold, int LoThHold)
{
    int min = 255;
    for (register int i=rect.l; i<rect.r; i++) {
    for (register int j=rect.t; j<rect.b; j++) {
        BYTE Dg = IElement(i, j);
        if      ( HiThHold &&  LoThHold) {if (Dg > LoThHold && Dg < HiThHold && Dg < min) min = Dg;}
        else if (!HiThHold &&  LoThHold) {if (Dg > LoThHold                  && Dg < min) min = Dg;}
        else if ( HiThHold && !LoThHold) {if (Dg < HiThHold                  && Dg < min) min = Dg;}
        else if (!HiThHold && !LoThHold) {if (                                  Dg < min) min = Dg;}
    }}
    return min;
}

int CBlobTable::GetParentBlob(int x, int y)
{
    for (int i=0; i<BlobCount; i++)
        if (Blob[i].Rect.IsInclude(x, y)) return i;

    return -1;
}

int CBlobTable::GetParentBlob(TPoint Point)
{
    for (int i=0; i<BlobCount; i++)
        if (Blob[i].Rect.IsInclude(Point)) return i;

    return -1;
}

//------------------------------------------------------------------------
//Compare
//------------------------------------------------------------------------
bool CBlobTable::OnQuantity2(int Min, int Max)
{
    for (register int k=0; k<BlobCount-1; k++) {
        int val = Blob[k].Quantity2;

        if (Min>0 && val<Min) return false;
        if (Max>0 && val>Max) return false;
    }
    return true;
}

bool CBlobTable::OnQuantity8(int Min, int Max)
{
    for (register int k=0; k<BlobCount-1; k++) {
        int val = Blob[k].Quantity8;

        if (Min>0 && val<Min) return false;
        if (Max>0 && val>Max) return false;
    }
    return true;
}

bool CBlobTable::OnBlobCount(int Min, int Max)
{
    int val = BlobCount;

    if (Min>0 && val<Min) return false;
    if (Max>0 && val>Max) return false;
    return true;
}

bool CBlobTable::OnHorPitch(int Min, int Max)
{
    for (register int k=0; k<BlobCount-1; k++) {
        int val = abs(Blob[k].Rect.Point().x - Blob[k+1].Rect.Point().x);

        if (Min>0 && val<Min) return false;
        if (Max>0 && val>Max) return false;
    }
    return true;
}


bool CBlobTable::OnVerPitch(int Min, int Max)
{
    for (register int k=0; k<BlobCount-1; k++) {
        int val = abs(Blob[k].Rect.Point().y - Blob[k+1].Rect.Point().y);

        if (Min>0 && val<Min) return false;
        if (Max>0 && val>Max) return false;
    }
    return true;
}

bool CBlobTable::OnHorAlign(int Min, int Max, bool IsSingle)
{
    for (register int k=0; k<BlobCount-1; k++) {
        int val = abs(Blob[k].Rect.Point().y - Blob[k+1].Rect.Point().y);

        int fx = Blob[k  ].Rect.Point().x;
        int sx = Blob[k+1].Rect.Point().x;

        if (IsSingle) {
            if ((Min>0 && val<Min)) return false;
            if ((Max>0 && val>Max)) return false;
        }
        else {
            if ((fx<sx) && (Min>0 && val<Min)) return false;
            if ((fx<sx) && (Max>0 && val>Max)) return false;
        }
    }
    return true;
}

bool CBlobTable::OnVerAlign(int Min, int Max, bool IsSingle)
{
    for (register int k=0; k<BlobCount-1; k++) {
        int val = abs(Blob[k].Rect.Point().x - Blob[k+1].Rect.Point().x);

        int fy = Blob[k  ].Rect.Point().y;
        int sy = Blob[k+1].Rect.Point().y;

        if (IsSingle) {
            if ((Min>0 && val<Min)) return false;
            if ((Max>0 && val>Max)) return false;
        }
        else {
            if ((fy<sy) && (Min>0 && val<Min)) return false;
            if ((fy<sy) && (Max>0 && val>Max)) return false;
        }
    }
    return true;
}

bool CBlobTable::OnDemension(int Min, int Max)
{
    for (register int k=0; k<BlobCount; k++) {
        int val = Blob[k].Rect.GetDemension();

        if (Min>0 && val<Min) return false;
        if (Max>0 && val>Max) return false;
    }
    return true;
}

bool CBlobTable::OnHorLength(int Min, int Max)
{
    for (register int k=0; k<BlobCount; k++) {
        int val = Blob[k].Rect.GetWidth();

        if (Min>0 && val<Min) return false;
        if (Max>0 && val>Max) return false;
    }
    return true;
}

bool CBlobTable::OnVerLength(int Min, int Max)
{
    for (register int k=0; k<BlobCount; k++) {
        int val = Blob[k].Rect.GetHeight();

        if (Min>0 && val<Min) return false;
        if (Max>0 && val>Max) return false;
    }
    return true;
}

bool CBlobTable::OnBorderCnt(int Min, int Max)
{
    for (register int k=0; k<BlobCount; k++) {
        int val = Blob[k].n;

        if (Min>0 && val<Min) return false;
        if (Max>0 && val>Max) return false;
    }
    return true;
}

bool CBlobTable::OnAngle(int Min, int Max)
{
    TFPoint P1 = Blob[0          ].Rect.Point();
    TFPoint P2 = Blob[BlobCount-1].Rect.Point();

    float val = Math.GetDegree(P1, P2);

    if (val>180) val -= 360.0;

    if (Min!=0 && val<Min) return false;
    if (Max!=0 && val>Max) return false;

    return true;
}

bool CBlobTable::OnHorPos(int Min, int Max, JRect IpRt)
{
    if (Min>0 && IpRt.l<Min) return false;
    if (Max>0 && IpRt.t>Max) return false;

    return true;
}

bool CBlobTable::OnVerPos(int Min, int Max, JRect IpRt)
{
    if (Min>0 && IpRt.t<Min) return false;
    if (Max>0 && IpRt.t>Max) return false;

    return true;
}


//------------------------------------------------------------------------
//Get Max Data
//------------------------------------------------------------------------
int CBlobTable::GetMaxQuantity2()
{
    int max = 0;

    for (register int k=0; k<BlobCount-1; k++) {
        int val = Blob[k].Quantity2;
        if (val>max) max = val;
    }
    return max;
}

int CBlobTable::GetMaxQuantity8()
{
    int max = 0;

    for (register int k=0; k<BlobCount-1; k++) {
        int val = Blob[k].Quantity8;
        if (val>max) max = val;
    }
    return max;
}

int CBlobTable::GetMaxBlobCount()
{
    int max = BlobCount;
    return max;
}

int CBlobTable::GetMaxHorPitch()
{
    int max = 0;

    for (register int k=0; k<BlobCount-1; k++) {
        int val = abs(Blob[k].Rect.Point().x - Blob[k+1].Rect.Point().x);
        if (val>max) max = val;
    }
    return max;
}

int CBlobTable::GetMaxVerPitch()
{
    int max = 0;

    for (register int k=0; k<BlobCount-1; k++) {
        int val = abs(Blob[k].Rect.Point().y - Blob[k+1].Rect.Point().y);

        if (val>max) max = val;
    }
    return max;
}

/*
int CBlobTable::GetMaxDemension()
{
    int max = 0;

    for (register int k=0; k<BlobCount; k++) {
        int val = Blob[k].Rect.GetDemension();
        if (val>max) max = val;
    }
    return max;
}
*/


int CBlobTable::GetMaxHorAlign()
{
    int max = 0;

    for (register int k=0; k<BlobCount-1; k++) {
        int val = abs(Blob[k].Rect.Point().y - Blob[k+1].Rect.Point().y);

        int fx = Blob[k  ].Rect.Point().x;
        int sx = Blob[k+1].Rect.Point().x;

        if ((fx<sx) && (val>max)) max = val;
    }
    return max;
}

int CBlobTable::GetMaxVerAlign()
{
    int max = 0;

    for (register int k=0; k<BlobCount-1; k++) {
        int val = abs(Blob[k].Rect.Point().x - Blob[k+1].Rect.Point().x);

        int fy = Blob[k  ].Rect.Point().y;
        int sy = Blob[k+1].Rect.Point().y;

        if ((fy<sy) && (val>max)) max = val;
    }
    return max;
}

int CBlobTable::GetMaxDemension()
{
    int max = 0;

    for (register int k=0; k<BlobCount; k++) {
        int val = Blob[k].Rect.GetDemension();
        if (val>max) max = val;
    }
    return max;
}



int CBlobTable::GetMaxHorLength()
{
    int max = 0;

    for (register int k=0; k<BlobCount; k++) {
        int val = Blob[k].Rect.GetWidth();
        if (val>max) max = val;
    }
    return max;
}

int CBlobTable::GetMaxVerLength()
{
    int max = 0;

    for (register int k=0; k<BlobCount; k++) {
        int val = Blob[k].Rect.GetHeight();
        if (val>max) max = val;
    }
    return max;
}

int CBlobTable::GetMaxBorderCnt()
{
    int max = 0;

    for (register int k=0; k<BlobCount; k++) {
        int val = Blob[k].n;
        if (val>max) max = val;
    }
    return max;
}

int CBlobTable::GetMaxAngle()
{
    int max = 0;

    TFPoint P1 = Blob[0          ].Rect.Point();
    TFPoint P2 = Blob[BlobCount-1].Rect.Point();

    float val = Math.GetDegree(P1, P2);

    if (val>180) val -= 360.0;

    if (val>max) max = val;

    return max;
}

int CBlobTable::GetMaxHorPos()
{
    JRect Rt = OnMerge();
    int max =  Rt.l;
    return max;
}

int CBlobTable::GetMaxVerPos()
{
    JRect Rt = OnMerge();
    int max =  Rt.t;
    return max;
}

int CBlobTable::GetMatchPxScore(TBlob *CuBlob, TBlob *TrBlob, int OutLineArea, bool IsCorner)
{
   int DiffArea = 0;
   int ViewArea = 0;

   int cx = CuBlob->Rect.l; 
   int cy = CuBlob->Rect.t; 
   int cw = CuBlob->Rect.r; 
   int ch = CuBlob->Rect.b;
   
   int w=cw-cx;
   int h=ch-cy;

   for (int i=0; i<w; i++) {
   for (int j=0; j<h; j++) {
       bool IsView1 = (w-i)<OutLineArea;
       bool IsView2 = (h-j)<OutLineArea;
       bool IsView3 = i>=0 && i<OutLineArea;
       bool IsView4 = j>=0 && j<OutLineArea;
       bool IsView =   (!IsCorner && (IsView1 || IsView2 || IsView3 || IsView4))
                     ||( IsCorner && ((IsView3&&IsView4)||(IsView1&&IsView4)||(IsView3&&IsView2)||(IsView1&&IsView2)));

       if (IsView || OutLineArea<1) {
           //bool op = CuBlob->PElement((cx+i), (cy+j)) ? true : false;
           //bool tp = TrBlob->PElement( i,          j) ? true : false;

           //DiffArea += op ^ tp;
           ViewArea ++;
       }
   }}

   //Calculate Score.
   float SameArea = ViewArea - DiffArea;
   float CurrScore = SameArea/ViewArea*100.0;

   return CurrScore;
}

int CBlobTable::GetMatchQtyScore(TBlob *CuBlob, TBlob *TrBlob, int OutLineArea, bool IsCorner)
{
   int CurrQty = 0;
   int TrainQty = 0;

   int tx = TrBlob->Rect.l;
   int ty = TrBlob->Rect.t;
   int tw = TrBlob->Rect.r;
   int th = TrBlob->Rect.b;

   int w=tw-tx;
   int h=th-ty;

   for (int i=0; i<w; i++) {
   for (int j=0; j<h; j++) {
       bool IsView1 = (w-i)<OutLineArea;
       bool IsView2 = (h-j)<OutLineArea;
       bool IsView3 = i>=0 && i<OutLineArea;
       bool IsView4 = j>=0 && j<OutLineArea;
       bool IsView =   (!IsCorner && (IsView1 || IsView2 || IsView3 || IsView4))
                     ||( IsCorner && ((IsView3&&IsView4)||(IsView1&&IsView4)||(IsView3&&IsView2)||(IsView1&&IsView2)));

       if (IsView || OutLineArea<1) {
           //if (CuBlob->PElement((cx+i), (cy+j))) CurrQty++;
           //if (TrBlob->PElement( i,          j)) TrainQty++;
       }
   }}

   //Calculate Score.

   float Score = 0;
   if (TrainQty>0) Score = ((float)CurrQty*100.0)/(float)TrainQty;
   if (Score>100) Score = 100.0-(Score-100.0);

   return Score;
}

void CBlobTable::OnLoad(bool IsLoad, CString Path)
{
    CBoundary::OnLoad(IsLoad, Path);
}
//--------------------------------------------------------------------------

