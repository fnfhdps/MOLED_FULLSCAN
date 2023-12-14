#include "stdafx.h"
#include "LxTrace.h"
//---------------------------------------------------------------------------
// #define IElement(x, y)  *((BYTE *)IAddress + x + Width * y)
// #define TElement(x, y)  *((BYTE *)TAddress + x + Width * y)
// #define MElement(x, y)  *((BYTE *)MAddress + x + Width * y)
 //---------------------------------------------------------------------------

void JTrace::TBlob::OnClear()
{
    TPoint T(0,0);
    lPoint = tPoint = rPoint = bPoint = T;
    n = 0;
    Rect.l = Rect.t = Rect.r = Rect.b = 0;
    //if (Point!=NULL) {delete[] Point; Point = NULL;}
    //if (Pixel!=NULL) {delete[] Pixel; Pixel = NULL;}
}

JTrace::JTrace()
{
    TAddress = NULL;
    MAddress = NULL;
	GImage   = NULL;
//    FPoint = NULL;
		
    BlobCount = 0;
    Blob = NULL;

    FHiThHold = 0;
    FLoThHold = 60;
    IgSort = false;
    SortMode = smRightBtm;

    FMinX  = 5;
    FMinY  = 5;
    FMinArea = 0;
    FMaxArea = 0;
    FMinHeight = 0;
    FMaxHeight = 0;
    FMinWidth = 0;
    FMaxWidth = 0;

    FShape = 0;

    FDarkPixelCount = 0;
    FBrightPixelCount = 0;

    OpJuncCount = true;
    JuncCount = 0;
    LtJunc = false;

    for (int k=0; k<MAX_JUNC; k++) {
        Junc[k].x = 0;
        Junc[k].y = 0;
    }
}

JTrace::~JTrace()
{
}

int JTrace::GetMeanThHold(JRect Rect)
{
    #define MAX_GRAY 256

    int Width  = GImage->GetWidth();
    int Height = GImage->GetHeight();

	IAddress    = GImage->GetImagePtr(); //Image Buffer Pointer
	BufferWidth = GImage->GetWidth()   ; //Image Buffer Width 

    double Hist[MAX_GRAY];
    memset(Hist, 0, sizeof(Hist));

    int Px = 0;
    for (register int y=Rect.t; y<Rect.b; y++) {
    for (register int x=Rect.l; x<Rect.r; x++) {
        if ((x-1)<=0      || x>=Width || y<=0      || y>=Height) continue;
        if ((x-1)<=Rect.l || x>=Rect.r       || y<=Rect.t || y>=Rect.b       ) continue;
        Px = IElement(x, y);
        if (Px==0) continue;
        Hist[Px]++;
    }}

    int Count = 0;
    int Sum = 0;
    for (int i=0; i<MAX_GRAY; i++) {
        Count += (int)Hist[i];
        Sum += (int)(i * Hist[i]);
    }

    if (Count<1) return 0;

    int ThHold = Sum/Count;
    return ThHold;
}

int JTrace::GetDarkPixelCount(int ThHold)
{
    FDarkPixelCount = 0;

    bool IsCircle = (FShape==1) ? true : false;

    if (ThHold==-1) ThHold = FHiThHold;

    JRect Rect;
    Rect.l = IRoi->GetOrgX();
    Rect.t = IRoi->GetOrgY();
    Rect.r = Rect.l+IRoi->GetWidth();
    Rect.b = Rect.t+IRoi->GetHeight();

    int Width  = GImage->GetWidth();
    int Height = GImage->GetHeight();

	IAddress    = GImage->GetImagePtr(); //Image Buffer Pointer
	BufferWidth = GImage->GetWidth()   ; //Image Buffer Width 

    TFPoint C(Rect.Point().x, Rect.Point().y);
    float Radius = (float)(Width/2.0);

    int Count = 0;
    int Px = 0;
    for (register int y=Rect.t; y<Rect.b; y++) {
    for (register int x=Rect.l; x<Rect.r; x++) {
        if ((x-1)<=0 || x>=Width || y<=0 || y>=Height) continue;
        if ((x-1)<=Rect.l || x>=Rect.r || y<=Rect.t || y>=Rect.b) continue;
        Px = IElement(x, y);

        if (!IsCircle) {
            if (Px<=ThHold) Count++;
        }
        else {
            float dx = x-C.x;
            float dy = y-C.y;
            float CuLg = sqrt(pow(dx, 2) + pow(dy, 2));
            if (CuLg<Radius && (Px<=ThHold)) Count++;
        }
    }}

    FDarkPixelCount = Count;
    return Count;
}

int JTrace::GetBrightPixelCount(int ThHold)
{
    FBrightPixelCount = 0;

    bool IsCircle = (FShape==1) ? true : false;

    if (ThHold==-1) ThHold = FHiThHold;

    JRect Rect;
    Rect.l = IRoi->GetOrgX();
    Rect.t = IRoi->GetOrgY();
    Rect.r = Rect.l+IRoi->GetWidth();
    Rect.b = Rect.t+IRoi->GetHeight();

    int Width  = GImage->GetWidth();
    int Height = GImage->GetHeight();

	IAddress    = GImage->GetImagePtr(); //Image Buffer Pointer
	BufferWidth = GImage->GetWidth()   ; //Image Buffer Width 

    TFPoint C(Rect.Point().x, Rect.Point().y);
    float Radius = (float)(Width/2.0);

    int Count = 0;
    int Px = 0;
    for (register int y=Rect.t; y<Rect.b; y++) {
    for (register int x=Rect.l; x<Rect.r; x++) {
        if ((x-1)<=0 || x>=Width || y<=0 || y>=Height) continue;
        if ((x-1)<=Rect.l || x>=Rect.r || y<=Rect.t || y>=Rect.b) continue;
        Px = IElement(x, y);

        if (!IsCircle) {
            if (Px>ThHold) Count++;
        }
        else {
            float dx = x-C.x;
            float dy = y-C.y;
            float CuLg = sqrt(pow(dx, 2) + pow(dy, 2));
            if (CuLg<Radius && (Px>ThHold)) Count++;
        }
    }}

    FBrightPixelCount = Count;
    return Count;
}

void JTrace::OnMasking(JRect Rect, int Mask)
{
    register int Width  = GImage->GetWidth ();
    register int Height = GImage->GetHeight();

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

int JTrace::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CTrace Execute GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CTrace Execute GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CTrace Execute GImage Height Size"    )); return false; }

    int xMin = FMinX;
    int yMin = FMinY;
    int HiTh = FHiThHold;
    int LoTh = FLoThHold;

    HRect Rect;
    Rect.x = IRoi->GetOrgX();
    Rect.y = IRoi->GetOrgY();
    Rect.w = IRoi->GetWidth();
    Rect.h = IRoi->GetHeight();

    DRoi.SetOrgX  (Rect.x);
    DRoi.SetOrgY  (Rect.y);
    DRoi.SetWidth (Rect.w);
    DRoi.SetHeight(Rect.h);

    if (IRoi->GetOrgX()<0) return false;
    if (IRoi->GetOrgY()<0) return false;
    if ((IRoi->GetOrgX()+IRoi->GetWidth ())>GImage->GetWidth ()) return false;
    if ((IRoi->GetOrgY()+IRoi->GetHeight())>GImage->GetHeight()) return false;

    if (xMin<1) xMin = 2;
    if (yMin<1) yMin = 2;

	IAddress    = GImage->GetImagePtr(); //Image Buffer Pointer
	BufferWidth = GImage->GetWidth()   ; //Image Buffer Width 

    register int FixMargin   = 4;
    register int Width       = GImage->GetWidth ();
    register int Height      = GImage->GetHeight();
    register int IgInSide    = true         ;
    register int MaxBbCnt    = 100;
    register int MinPxCnt    = 5;
    register int MaxPxCnt    = 100000;
    register int OffsetY     = xMin/2;
    register int StepY       = yMin/2;
    register int OffsetX     = 1;
    register int StepX       = 1;
    register int HiThHold    = HiTh;
    register int LoThHold    = LoTh;
    register int Is4WayTrace = false;

    if (Blob!=NULL) { delete[] Blob; Blob=NULL; }
    BlobCount = 0;
    Blob = new TBlob[MaxBbCnt];

    for (int k=0; k<MaxBbCnt; k++) {
        Blob[k].OnClear();
    }

    if (TAddress==NULL) {
        TAddress = new BYTE[Width*Height];
    }
    memset(TAddress, 0, Width*Height);

    if (MAddress==NULL) {
        MAddress = new BYTE[Width*Height];
    }
    memset(MAddress, 0, Width*Height);

    //--------------------------------------------------------------------------
    // START : 잘못된 영역으로 인해 RETURN
    if (Rect.x         <0) return 0;
    if (Rect.y         <0) return 0;
    if ((Rect.w+Rect.x)<0) return 0;
    if ((Rect.h+Rect.y)<0) return 0;

    if (Rect.x         >=Width ) return 0;
    if (Rect.y         >=Height) return 0;
    if ((Rect.w+Rect.x)> Width ) return 0;
    if ((Rect.h+Rect.y)> Height) return 0;
    // END : 잘못된 영역으로 인해 RETURN
    //--------------------------------------------------------------------------

    register const POINT nei[8] =
    {
        {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
    };

    register int IncNei = Is4WayTrace ? 2 : 1;
    register int c0, c1, c2, cm, cp;
    register int x0, y0, k, n;
    PixelCount = 0;

    // Inspection & Ignore Masking Area 설정
    // -------------------------------------------------------------------------
    JRect IpRect;
    IpRect.l = Rect.x;
    IpRect.t = Rect.y;
    IpRect.r = Rect.w+Rect.x;
    IpRect.b = Rect.h+Rect.y;

    OnMasking(IpRect, 1);

    if (FUseIgnoreBlob) {
        for (register int i=0; i<IgBlobCount; i++) {
            if (IgBlobCount>=MAX_IGNORE_RECT) {
				AfxMessageBox(_T("IGNORE RECT COUNT 초과"));
				
                break;
            }
            OnMasking(IgBlob[i], 0);
        }
    }

    TPoint *Point = new TPoint[MaxPxCnt];

    for (register int y=Rect.y+OffsetY+FixMargin; y<(Rect.h+Rect.y)-OffsetY-FixMargin; y+=StepY) {
    for (register int x=Rect.x+OffsetX+FixMargin; x<(Rect.w+Rect.x)-OffsetX-FixMargin; x+=StepX) {
        if ((x-1)<=0 || x>=Width || y<=0 || y>=Height) continue;

        int AP = IElement(x, y);

        bool IsTrace;
        int p[8], p1[8], p2[8];

        c0 = HiThHold ? AP < HiThHold : true && LoThHold ? AP > LoThHold : true;

        IsTrace = c0;
        bool IsMask = MElement(x, y) ? true : false;
        bool pIsVisited = TElement(x, y) ? true : false;

        if (IsTrace && !pIsVisited && IsMask) {
            PixelCount = 0;
            bool IsEdge = false;
            x0 = x; y0 = y;
            n = 4;
            do {
                register short u;                 
                register short v;

                k=0;
                for (k=0; k<8; k+=IncNei, n=((n+IncNei) & 7)) {
                    u = (short)(x + nei[n].x);
                    v = (short)(y + nei[n].y);
                    //if ((u-1)<=0 || (u+1)>=Width || (v-1)<=0 || (v+1)>=Height) continue;
                    if (u<0 || u>Width || v<0 || v>Height) continue;

                    bool IsMask = MElement(u, v) ? true : false;
                    int E_AP = IElement(u, v);

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

            //Rect 계산
            //------------------------------------------------------------------
            for (register int k=0; k<PixelCount; k++) {
                JRect *Rect = &Blob[BlobCount].Rect;

                //Rect
                Rect->l = Rect->l<Point[k].x && Rect->l!=0 ? Rect->l : Point[k].x;
                Rect->t = Rect->t<Point[k].y && Rect->t!=0 ? Rect->t : Point[k].y;
                Rect->r = Rect->r>Point[k].x               ? Rect->r : Point[k].x;
                Rect->b = Rect->b>Point[k].y               ? Rect->b : Point[k].y;

                //Rect Point
                Blob[BlobCount].lPoint = Blob[BlobCount].lPoint.x<Point[k].x && Blob[BlobCount].lPoint.x!=0 ? Blob[BlobCount].lPoint : Point[k];
                Blob[BlobCount].tPoint = Blob[BlobCount].tPoint.y<Point[k].y && Blob[BlobCount].tPoint.y!=0 ? Blob[BlobCount].tPoint : Point[k];
                Blob[BlobCount].rPoint = Blob[BlobCount].rPoint.x>Point[k].x                                ? Blob[BlobCount].rPoint : Point[k];
                Blob[BlobCount].bPoint = Blob[BlobCount].bPoint.y>Point[k].y                                ? Blob[BlobCount].bPoint : Point[k];
            }

            //Check Ignore blob
            //------------------------------------------------------------------
            if (xMin || yMin){
                JRect cr;
                cr = Blob[BlobCount].Rect;

                bool xIsMin = xMin > abs(cr.r - cr.l);
                bool yIsMin = yMin > abs(cr.b - cr.t);
                bool IsRemove = xIsMin || yIsMin;

                if (IsRemove) continue;
            }

            if (FMinArea){
                JRect cr;
                cr = Blob[BlobCount].Rect;
                int v = (int)(cr.GetWidth()*cr.GetHeight());
                bool IsRemove = v<FMinArea;
                if (IsRemove) continue;
            }

            if (FMaxArea){
                JRect cr;
                cr = Blob[BlobCount].Rect;
                int v = (int)(cr.GetWidth()*cr.GetHeight());
                bool IsRemove = v>FMaxArea;
                if (IsRemove) continue;
            }

            if (FMinWidth){
                JRect cr;
                cr = Blob[BlobCount].Rect;
                int v = (int)(cr.GetWidth());
                bool IsRemove = v<FMinWidth;
                if (IsRemove) continue;
            }

            if (FMaxWidth){
                JRect cr;
                cr = Blob[BlobCount].Rect;
                int v = (int)(cr.GetWidth());
                bool IsRemove = v>FMaxWidth;
                if (IsRemove) continue;
            }

            if (FMinHeight){
                JRect cr;
                cr = Blob[BlobCount].Rect;
                int v = (int)(cr.GetHeight());
                bool IsRemove = v<FMinHeight;
                if (IsRemove) continue;
            }

            if (FMaxHeight){
                JRect cr;
                cr = Blob[BlobCount].Rect;
                int v = (int)(cr.GetHeight());
                bool IsRemove = v>FMaxHeight;
                if (IsRemove) continue;
            }

            if (IgInSide) {
                for (register int n=Blob[BlobCount].Rect.t; n<Blob[BlobCount].Rect.b; n++) {
                for (register int m=Blob[BlobCount].Rect.l; m<Blob[BlobCount].Rect.r; m++) {
                    TElement(m, n) = true;
                }}
            }

            bool IsMinMaxRadious = false;
            if (IsMinMaxRadious) {
                Blob[BlobCount].MinRadius = 100000;
                Blob[BlobCount].MaxRadius = 0;
//                 TFPoint C = Blob[BlobCount].C;
				TFPoint C = Blob[BlobCount].Rect.Point();
                for (register int k=0; k<PixelCount; k++) {
                    float dx = Point[k].x-C.x;
                    float dy = Point[k].y-C.y;
                    float CuLg = sqrt(pow(dx, 2) + pow(dy, 2));
                    if (CuLg<Blob[BlobCount].MinRadius) Blob[BlobCount].MinRadius = CuLg;
                    if (CuLg>Blob[BlobCount].MaxRadius) Blob[BlobCount].MaxRadius = CuLg;
                }
            }

            //Junction Point
            //------------------------------------------------------------------
            JuncCount = 0;
            if (OpJuncCount) {
                for (register int k=0; k<PixelCount; k++) {

                    if (JuncCount>MAX_JUNC) break;

                    int cx = Point[k].x;
                    int cy = Point[k].y;

                    JRect jRect = IpRect;
                    jRect.l = IpRect.l+2;
                    jRect.t = IpRect.t+2;
                    jRect.r = IpRect.r-2;
                    jRect.b = IpRect.b-2;

                    if (cx>jRect.l && cy>jRect.t && cx<jRect.r && cy>jRect.b) {
                        LtJunc = false;
                    }
                    if (cx<=jRect.l || cy<=jRect.t || cx>=jRect.r && cy>=jRect.b) {
                        if (!LtJunc) {
                            Junc[JuncCount] = Point[k];
                            JuncCount++;
                            LtJunc = true;
                        }
                    }
                }
            }

            Blob[BlobCount].n = PixelCount;
            BlobCount++;

            if (BlobCount>=MaxBbCnt) break;
        }
        if (BlobCount>=MaxBbCnt) break;
    }
    if (BlobCount>=MaxBbCnt) break;
    }

    MgRect.l = 0;
    MgRect.t = 0;
    MgRect.r = 0;
    MgRect.b = 0;
    for (register int k=0; k<BlobCount; k++) {
        JRect *Rect = &Blob[k].Rect;
        MgRect.l = MgRect.l<Rect->l && MgRect.l!=0 ? MgRect.l : Rect->l;
        MgRect.t = MgRect.t<Rect->t && MgRect.t!=0 ? MgRect.t : Rect->t;
        MgRect.r = MgRect.r>Rect->r                ? MgRect.r : Rect->r;
        MgRect.b = MgRect.b>Rect->b                ? MgRect.b : Rect->b;
    }

    OnSortBlob();

    delete[] Point;
    return BlobCount;
}


TSortMode SORTMODE;
int OnSortFunction(const void *b1, const void *b2)
{
    const int RIGHT_POS =  1;
    const int LEFT_POS  = -1;
    const int ORIG_POS  =  0;

    if (b1==NULL) return ORIG_POS;
    if (b2==NULL) return ORIG_POS;

    int rg = (int)(fabs(((JTrace::TBlob *)b1)->Rect.Point().y - ((JTrace::TBlob *)b2)->Rect.Point().y));
    int r1 = (int)(((JTrace::TBlob *)b1)->Rect.Point().y - ((JTrace::TBlob *)b2)->Rect.Point().y);
    int r2 = (int)(((JTrace::TBlob *)b1)->Rect.Point().x - ((JTrace::TBlob *)b2)->Rect.Point().x);

    bool IsYLine = ((JTrace::TBlob *)b2)->Rect.t <= ((JTrace::TBlob *)b1)->Rect.b && ((JTrace::TBlob *)b2)->Rect.b >= ((JTrace::TBlob *)b1)->Rect.t;
    bool IsXLine = ((JTrace::TBlob *)b2)->Rect.l <= ((JTrace::TBlob *)b1)->Rect.r && ((JTrace::TBlob *)b2)->Rect.r >= ((JTrace::TBlob *)b1)->Rect.l;

    //bool IsYLine = ((COcr::TResult *)b2)->Rect.Point().y <= ((COcr::TResult *)b1)->Rect.Point().y && ((COcr::TResult *)b2)->Rect.Point().y >= ((COcr::TResult *)b1)->Rect.Point().y;
    //bool IsXLine = ((COcr::TResult *)b2)->Rect.Point().x <= ((COcr::TResult *)b1)->Rect.Point().x && ((COcr::TResult *)b2)->Rect.Point().x >= ((COcr::TResult *)b1)->Rect.Point().x;

    switch (SORTMODE) {
        default         : break;

        case smRightBtm : if ( rg< 20 && r2>0) return RIGHT_POS;
                          if ( rg>=20 && r1>0) return RIGHT_POS;
                          break;

        //case smRightBtm : if ( IsYLine && r2>0) return RIGHT_POS;
        //                  if (!IsYLine && r1>0) return RIGHT_POS;
        //                  break;

        case smLeftBtm  : if ( IsYLine && r2<0) return RIGHT_POS;
                          if (!IsYLine && r1>0) return RIGHT_POS;
                          break;

        case smRightTop : if ( IsYLine && r2>0) return RIGHT_POS;
                          if (!IsYLine && r1<0) return RIGHT_POS;
                          break;

        case smLeftTop  : if ( IsYLine && r2<0) return RIGHT_POS;
                          if (!IsYLine && r1<0) return RIGHT_POS;
                          break;

        case smBtmRight : if ( IsXLine && r1>0) return RIGHT_POS;
                          if (!IsXLine && r2>0) return RIGHT_POS;
                          break;               

        case smBtmLeft  : if ( IsXLine && r1>0) return RIGHT_POS;
                          if (!IsXLine && r2<0) return RIGHT_POS;
                          break;

        case smTopRight : if ( IsXLine && r1<0) return RIGHT_POS;
                          if (!IsXLine && r2>0) return RIGHT_POS;
                          break;

        case smTopLeft  : if ( IsXLine && r1<0) return RIGHT_POS;
                          if (!IsXLine && r2<0) return RIGHT_POS;
                          break;

    }
    return LEFT_POS;
}

void JTrace::OnSortBlob()
{
    if (IgSort) return;

    SORTMODE = (TSortMode)SortMode;
    qsort((void *)Blob, BlobCount, sizeof(Blob[0]), OnSortFunction);
}

void JTrace::OnPaintIRoi(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;

    if (FShape==1) return OnPaintIRoiByCircle(Tool);

    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;

    Gdi->CPen.Color = Tool.Color;

    int l = IRoi->GetOrgX();
    int t = IRoi->GetOrgY();
    int r = IRoi->GetOrgX()+IRoi->GetWidth();
    int b = IRoi->GetOrgY()+IRoi->GetHeight();

    Gdi->OnMove(l, t);
    Gdi->OnLine(r, t);
    Gdi->OnLine(r, b);
    Gdi->OnLine(l, b);
    Gdi->OnLine(l, t);

    delete Gdi;
}

void JTrace::OnPaintIRoiByCircle(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;

    Gdi->CPen.Color = Tool.Color;

    int Radius = (int)(IRoi->GetWidth()/2.0+0.5);
    int Cx     = (int)(IRoi->GetOrgX()+IRoi->GetWidth()/2.0+0.5 );
    int Cy     = (int)(IRoi->GetOrgY()+IRoi->GetHeight()/2.0+0.5);

    Gdi->OnCircle(0, Cx, Cy, Radius, 0);

    delete Gdi;
}

void JTrace::OnPaintDRoiByCircle(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
 
    Gdi->CPen.Color = Tool.Color;

    int Radius = (int)(DRoi.GetWidth()/2.0+0.5                );
    int Cx     = (int)(DRoi.GetOrgX()+DRoi.GetWidth()/2.0+0.5 );
    int Cy     = (int)(DRoi.GetOrgY()+DRoi.GetHeight()/2.0+0.5);

    Gdi->OnCircle(0, Cx, Cy, Radius, 0);

    delete Gdi;
}

void JTrace::OnPaintDRoi(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
    if (FShape==1) return OnPaintDRoiByCircle(Tool);

    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;

    Gdi->CPen.Color = Tool.Color;

    int l = DRoi.GetOrgX();
    int t = DRoi.GetOrgY();
    int r = DRoi.GetOrgX()+DRoi.GetWidth();
    int b = DRoi.GetOrgY()+DRoi.GetHeight();

    Gdi->OnMove(l, t);
    Gdi->OnLine(r, t);
    Gdi->OnLine(r, b);
    Gdi->OnLine(l, b);
    Gdi->OnLine(l, t);

    delete Gdi;
}

void JTrace::OnPaintRectPoint(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;

    Gdi->CPen.Color = Tool.Color;

    for (register int k=0; k<BlobCount; k++) {
        Gdi->OnMove(Blob[k].lPoint.x, Blob[k].lPoint.y);
        Gdi->OnLine(Blob[k].tPoint.x, Blob[k].tPoint.y);
        Gdi->OnLine(Blob[k].rPoint.x, Blob[k].rPoint.y);
        Gdi->OnLine(Blob[k].bPoint.x, Blob[k].bPoint.y);
        Gdi->OnLine(Blob[k].lPoint.x, Blob[k].lPoint.y);
    }
    delete Gdi;
}

void JTrace::OnPaintJuncPoint(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;

    Gdi->CPen.Color = Tool.Color;

    for (register int k=0; k<JuncCount; k++) {
        Gdi->OnMove(Junc[k].x, Junc[k].y);
        Gdi->OnLine(Junc[k].x, Junc[k].y);
        Gdi->OnLine(Junc[k].x, Junc[k].y);
        Gdi->OnLine(Junc[k].x, Junc[k].y);
        Gdi->OnLine(Junc[k].x, Junc[k].y);
    }
    delete Gdi;
}

void JTrace::OnPaintBlobId(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
 
    Gdi->CPen.Color = Tool.Color;
	CString Temp ="";
    for (register int k=0; k<BlobCount; k++) {
        int px = Blob[k].Rect.l;
        int py = Blob[k].Rect.t;
		Temp.Format(L"%d", k);
        Gdi->OnText(px, py, Temp);
    }

    delete Gdi;
}

void JTrace::OnPaintBlobRect(TDrawTool Tool, TPoint P1, TPoint P2, TPoint P3, TPoint P4, COLORREF Color)
{
	if(Tool.Handle==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;

    Gdi->CPen.Color = Tool.Color;
    Gdi->CBrush.Color = Tool.Color;
	Gdi->CPen.Color   = LIME_COLOR     ;
    Gdi->CBrush.Style = BS_HOLLOW;

    Gdi->OnLine(P1, P2);
    Gdi->OnLine(P2, P4);
    Gdi->OnLine(P4, P3);
    Gdi->OnLine(P3, P1);

    delete Gdi;
}

void JTrace::OnPaintBlobRectConer(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;

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

void JTrace::OnPaintBlobCircle(TDrawTool Tool)
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
        TPoint C;
        C.x = (int)(Blob[k].C.x+0.5);
        C.y = (int)(Blob[k].C.y+0.5);
        double Radius = Blob[k].Radius+0.5;
        Gdi->OnCircle(0, C, (int)Radius, (int)Radius);
    }
    delete Gdi;
}

void JTrace::OnPaintBlobWeight(TDrawTool Tool)
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

//         C.x += 0.5;
//         C.y += 0.5;

		C.x  = (int)(C.x + 0.5);
		C.y  = (int)(C.y + 0.5);

        Gdi->OnLinePlus(C, 4);
    }
    delete Gdi;
}

void JTrace::OnPaintBlobRect(TDrawTool Tool)
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

        Gdi->OnMove(Rect.l, Rect.t);
        Gdi->OnLine(Rect.r, Rect.t);
        Gdi->OnLine(Rect.r, Rect.b);
        Gdi->OnLine(Rect.l, Rect.b);
        Gdi->OnLine(Rect.l, Rect.t);
    }

    delete Gdi;
}

void JTrace::OnPaintBlobMgRect(TDrawTool Tool)
{
	if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
 
    Gdi->CPen.Color = Tool.Color;
    Gdi->CBrush.Style = BS_HOLLOW;

    Gdi->OnMove(MgRect.l, MgRect.t);
    Gdi->OnLine(MgRect.r, MgRect.t);
    Gdi->OnLine(MgRect.r, MgRect.b);
    Gdi->OnLine(MgRect.l, MgRect.b);
    Gdi->OnLine(MgRect.l, MgRect.t);

    delete Gdi;
}

void JTrace::OnPaintBlobBrightPixel(TDrawTool Tool, int ThHold)
{
    int Width  = GImage->GetWidth();
    int Height = GImage->GetHeight();

	if(Tool.Handle==NULL) return;
//     void *IAddress = GImage->GetImagePtr();

    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
 
    Gdi->CPen.Color = Tool.Color;
     for (int i=0; i<BlobCount; i++) {
        for (int u=Blob[i].Rect.l; u<Blob[i].Rect.r; u++) {
        for (int v=Blob[i].Rect.t; v<Blob[i].Rect.b; v++) {
            BYTE Px = IElement(u, v);
            TPoint Point(u, v);
            if (Px>ThHold) Gdi->OnPixel(Point, RED_COLOR);
        }}
    }

    delete Gdi;
}

void JTrace::OnPaintBrightPixel(HWND Handle, HRect Rect, COLORREF FColor, int ThHold)
{
    int Width  = GImage->GetWidth();
    int Height = GImage->GetHeight();
//     void *IAddress = GImage->GetImagePtr();

	if(Handle==NULL) return;
    HGdi *Gdi = new HGdi(Handle);

    for (int u=Rect.x; u<Rect.w+Rect.x; u++) {
    for (int v=Rect.y; v<Rect.h+Rect.y; v++) {
        BYTE Px = IElement(u, v);
        TPoint Point(u, v);
        if (Px>=ThHold) Gdi->OnPixel(Point, FColor);
    }}

    delete Gdi;
}

void JTrace::OnPaintDarkPixel(HWND Handle, HRect Rect, COLORREF FColor, int ThHold)
{
    int Width  = GImage->GetWidth();
    int Height = GImage->GetHeight();
//     void *IAddress = GImage->GetImagePtr();

	if(Handle==NULL) return;
    HGdi *Gdi = new HGdi(Handle);

    for (int u=Rect.x; u<Rect.w+Rect.x; u++) {
    for (int v=Rect.y; v<Rect.h+Rect.y; v++) {
        BYTE Px = IElement(u, v);
        TPoint Point(u, v);
        if (Px<ThHold) Gdi->OnPixel(Point, FColor);
    }}

    delete Gdi;
}

void JTrace::OnPaintPoint(TDrawTool Tool)
{
return;
/*
    HGdi *Gdi = new HGdi(Handle);

    for (int i=0; i<Blob[0].n; i++) {
        Gdi->OnPixel(FPoint[i], Color);
    }

    delete Gdi;
*/
}

void JTrace::OnPaintBlobPixel(TDrawTool Tool, COLORREF FColor, COLORREF BColor, int LoThHold, int HiThHold)
{
    int Width  = GImage->GetWidth();
    int Height = GImage->GetHeight();
//     void *IAddress = GImage->GetImagePtr();

    if(Tool.Handle==NULL) return;
    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    for (int i=0; i<BlobCount; i++) {
        for (int u=Blob[i].Rect.l; u<Blob[i].Rect.r; u++) {
        for (int v=Blob[i].Rect.t; v<Blob[i].Rect.b; v++) {
            BYTE Px = IElement(u, v);
            TPoint Point(u, v);
            if (LoThHold>0) Gdi->OnPixel(Point, Px>LoThHold ? FColor : BColor);
            if (HiThHold>0) Gdi->OnPixel(Point, Px>HiThHold ? FColor : BColor);
        }}
    }

    delete Gdi;
}

void JTrace::OnPaintBlobPixel(JRect Rect, HWND Handle, bool IsBright, bool IsDark, int ThHold, COLORREF BrightColor, COLORREF DarkColor)
{
    int Width  = GImage->GetWidth();
    int Height = GImage->GetHeight();

	if(Handle==NULL) return;
    HGdi *Gdi = new HGdi(Handle);

    for (int u=Rect.l; u<Rect.r; u++) {
    for (int v=Rect.t; v<Rect.b; v++) {
        BYTE Px = IElement(u, v);
        TPoint Point(u, v);
        if (IsBright) {
            if      (Px>ThHold) Gdi->OnPixel(Point, BrightColor);
            else                Gdi->OnPixel(Point, BLUE_COLOR       );
        }
        if (IsDark) {
            if      (Px<ThHold) Gdi->OnPixel(Point, DarkColor);
            else                Gdi->OnPixel(Point, BLUE_COLOR     );
        }
    }}


    delete Gdi;
}
//---------------------------------------------------------------------------
void JTrace::OnLoad(bool IsLoad, CString Folder)
{
	CIFile Ini;

    if (Folder.Trim()=="") return;
    CString Path = Folder + "\\Trace.ini";
    if (Path=="") return;

    Ini.Load(IsLoad, Path, "Property", "FMinX"    , &FMinX    );
    Ini.Load(IsLoad, Path, "Property", "FMinY"    , &FMinY    );
    Ini.Load(IsLoad, Path, "Property", "FHiThHold", &FHiThHold);
    Ini.Load(IsLoad, Path, "Property", "FLoThHold", &FLoThHold);
    Ini.Load(IsLoad, Path, "Property", "FUseIgnoreBlob", &FUseIgnoreBlob);

    Ini.Load(IsLoad, Path, "Ignore", "FMinArea  ", &FMinArea  );
    Ini.Load(IsLoad, Path, "Ignore", "FMaxArea  ", &FMaxArea  );
    Ini.Load(IsLoad, Path, "Ignore", "FMinHeight", &FMinHeight);
    Ini.Load(IsLoad, Path, "Ignore", "FMaxHeight", &FMaxHeight);
    Ini.Load(IsLoad, Path, "Ignore", "FMinWidth ", &FMinWidth );
    Ini.Load(IsLoad, Path, "Ignore", "FMaxWidth ", &FMaxWidth );

    // Roi Load & Save
    // -------------------------------------------------------------------------
    CString IRoiPath = Folder + "\\TraceIRoi.ini";
    OnLoadRoi(IsLoad, IRoiPath, IRoi);

}
