// Trace.cpp: implementation of the CTrace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LxBlob.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// #if LINESCAN_CAMERA_MODE == 0
// #define IElement(x, y)  *((BYTE *)IAddress + x + 4096* y)
// #define TElement(x, y)  *((BYTE *)TAddress + x + 4096* y)
// #elif LINESCAN_CAMERA_MODE ==1
// #define IElement(x, y)  *((BYTE *)IAddress + x + 16384* y)
// #define TElement(x, y)  *((BYTE *)TAddress + x + 16384* y)
// #elif LINESCAN_CAMERA_MODE ==2
// #define IElement(x, y)  *((BYTE *)IAddress + x + 23360* y)
// #define TElement(x, y)  *((BYTE *)TAddress + x + 23360* y)
// #endif

void CBlob::TBlob::OnClear()
{
    TPoint T(0,0);
    lPoint = tPoint = rPoint = bPoint = T;
    n = 0;
    Rect.l = Rect.t = Rect.r = Rect.b = 0;
    if (Point!=NULL) {delete[] Point; Point = NULL;}
    if (Pixel!=NULL) {delete[] Pixel; Pixel = NULL;}
}
//---------------------------------------------------------------
CBlob::CBlob()
{
	TAddress = NULL;
	IAddress = NULL;
	GImage   = NULL;
	
    BlobCount = 0;
    Blob = NULL;
	
    FMinX     = 5;
    FMinY     = 5;
    FHiThHold = 0;
    FLoThHold = 60;
}
//--------------------------------------------------------------
CBlob::~CBlob()
{

}
//--------------------------------------------------------------
int CBlob::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(L"Check CBlob GImage Memory Allocate"); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(L"Check CBlob GImage Width Size"     ); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(L"Check CBlob GImage Height Size"    ); return false; }

    int xMin = FMinX;
    int yMin = FMinY;
    int HiTh = FHiThHold;
    int LoTh = FLoThHold;

    HRect Rect;

    Rect.x = IRoi->GetOrgX();
    Rect.y = IRoi->GetOrgY();
    Rect.w = IRoi->GetWidth();
    Rect.h = IRoi->GetHeight();

    if (xMin<1) xMin = 2;
    if (yMin<1) yMin = 2;

//     void *IAddress = GImage->GetImagePtr();
	IAddress    = GImage->GetImagePtr(); //Image Buffer Pointer
	BufferWidth = GImage->GetWidth()   ; //Image Buffer Width 

    //register int FixMargin   = 4;
    register int Width       = GImage->GetWidth ();
    register int Height      = GImage->GetHeight();
    register int IgInSide    = true         ;
    register int MaxBbCnt    = 100;
    register int MinPxCnt    = 5;
    register int MaxPxCnt    = 100000;
    //register int OffsetY     = xMin/2;
    register int StepY       = yMin/2;
    //register int OffsetX     = 1;
    register int StepX       = 1;
    register int HiThHold    = HiTh;
    register int LoThHold    = LoTh;
    register int Is4WayTrace = false;
    register int FixMargin   = 0;
    register int OffsetY     = 0;
    register int OffsetX     = 0;


    if (Blob!=NULL) { delete[] Blob; Blob=NULL; }
    BlobCount = 0;
    Blob = new TBlob[MaxBbCnt];

    if (TAddress==NULL) {
        TAddress = new BYTE[Width*Height];
    }
    memset(TAddress, 0, Width*Height);

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
    //register int c0, c1, c2, cm, cp;
    register int c0, c2;
    register int x0, y0, k, n;
    PixelCount = 0;

//    if (FPoint==NULL) {
//        delete[] FPoint;
//        FPoint = NULL;
//    }

    TPoint *Point = new TPoint[MaxPxCnt];

    for (register int y=Rect.y+OffsetY+FixMargin; y<(Rect.h+Rect.y)-OffsetY-FixMargin; y+=StepY) {
    for (register int x=Rect.x+OffsetX+FixMargin; x<(Rect.w+Rect.x)-OffsetX-FixMargin; x+=StepX) {
        if ((x-1)<=0 || x>=Width || y<=0 || y>=Height) continue;

        int AP = IElement(x, y);

        int IsTrace;
        //int p[8], p1[8], p2[8];

        c0 = HiThHold ? AP < HiThHold : true && LoThHold ? AP > LoThHold : true;

        IsTrace = c0;
        bool pIsVisited = TElement(x, y) ? true : false;

        if (IsTrace && !pIsVisited) {
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
                    if (u>(Rect.x+Rect.w) || v>(Rect.y+Rect.h)) continue;


                    int E_AP = IElement(u, v);

                    c2 = HiThHold ? E_AP < HiThHold : true && LoThHold ? E_AP > LoThHold : true;
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
                JRect *Rect  = &Blob[BlobCount].Rect;
                Rect->l = Rect->l<Point[k].x && Rect->l!=0 ? Rect->l : Point[k].x;
                Rect->t = Rect->t<Point[k].y && Rect->t !=0 ? Rect->t : Point[k].y;
                Rect->r = Rect->r>Point[k].x                  ? Rect->r : Point[k].x;
                Rect->b = Rect->b>Point[k].y                  ? Rect->b : Point[k].y;
            }

            //Check Ignore blob
            if (xMin || yMin){
                JRect cr;
                cr = Blob[BlobCount].Rect;

                bool xIsMin = xMin > abs(cr.b - cr.t );
                bool yIsMin = yMin > abs(cr.r - cr.l);
                bool IsRemove = xIsMin || yIsMin;

                if (IsRemove) continue;
            }

            if (IgInSide) {
                for (register int n=Blob[BlobCount].Rect.t ; n<Blob[BlobCount].Rect.b; n++) {
                for (register int m=Blob[BlobCount].Rect.l ; m<Blob[BlobCount].Rect.r ; m++) {
                    TElement(m, n) = true;
                }}
            }

            bool IsMinMaxRadious = false;
            if (IsMinMaxRadious) {
                Blob[BlobCount].MinRadius = 100000;
                Blob[BlobCount].MaxRadius = 0;
                TFPoint C = Blob[BlobCount].C;
                for (register int k=0; k<PixelCount; k++) {
                    float dx = Point[k].x-C.x;
                    float dy = Point[k].y-C.y;
                    float CuLg = (float)(sqrt(pow(dx, 2) + pow(dy, 2)));
                    if (CuLg<Blob[BlobCount].MinRadius) Blob[BlobCount].MinRadius = CuLg;
                    if (CuLg>Blob[BlobCount].MaxRadius) Blob[BlobCount].MaxRadius = CuLg;
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

//    if (Blob[0].n) {
//        FPoint = new TPoint[Blob[0].n];
//    }

//    PixelCount = Blob[0].n;
//    for (int k=0; k<PixelCount; k++) {
//        FPoint[k] = Point[k];
//    }


    MgRect.l = 0;
    MgRect.t = 0;
    MgRect.r = 0;
    MgRect.b = 0;
    for (register int i=0; i<BlobCount; i++) {
        JRect *Rect = &Blob[i].Rect;
        MgRect.l = MgRect.l<Rect->l&& MgRect.l!=0 ? MgRect.l : Rect->l;
        MgRect.t = MgRect.t<Rect->t&& MgRect.t!=0 ? MgRect.t : Rect->t;
		MgRect.r = MgRect.r>Rect->r               ? MgRect.r : Rect->r;
		MgRect.b = MgRect.b>Rect->b               ? MgRect.b : Rect->b;
    }

    delete[] Point;
    return BlobCount;
}
//---------------------------------------------------------------------------------
void CBlob::OnLoad(bool IsLoad, CString Folder)
{
	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;

	if (Folder=="") return;
	CString Path = Folder + "\\Blob.ini";

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}

	Ini.Load(IsLoad, Path, "Property", "FMinX    ", &FMinX    );
	Ini.Load(IsLoad, Path, "Property", "FMinY    ", &FMinY    );
	Ini.Load(IsLoad, Path, "Property", "FHiThHold", &FHiThHold);
	Ini.Load(IsLoad, Path, "Property", "FLoThHold", &FLoThHold);
}

//-----------------------------------------------------------------
// Euresys Blob
//-----------------------------------------------------------------
CEBlob::CEBlob()
{
	IAddress = NULL;
	TAddress = NULL;
	GImage   = NULL;
	OnClearData();
}
//--------------------------------------------------------------
CEBlob::~CEBlob()
{

}
//----------------------------------------------------------------
void CEBlob::OnClearData()
{
	m_iPixelValue =0;
	m_iPixelCount =0;
	m_iBlobCount  =0;
    m_iDefectCount=0;

	//Parameter.m_iThresHold    =0;
	//Parameter.m_iMergeCount   =0;
	//Parameter.m_iRoiOffset    =0;
    //Parameter.m_iDarkThresHold=0;
	//Parameter.m_fMinWidth   =0.0f;
	//Parameter.m_fMaxWidth   =0.0f;
	//Parameter.m_fMinHeight  =0.0f;
	//Parameter.m_fMaxHeight  =0.0f;


	//Parameter.m_bDarkDefect     = false;
    //Parameter.m_bBinaryInsp     = false;
	//Parameter.m_bMerge          = false;

	for(int k=0; k<MAX_DEFECT_POINT; k++){
		//Init
		Blob[k].Height		=0;
		Blob[k].Width		=0;
		Blob[k].Round		=0;
		Blob[k].P.x			=0;
		Blob[k].P.y			=0;
		Blob[k].Rect.l      =0;
		Blob[k].Rect.t      =0;
		Blob[k].Rect.r      =0;
		Blob[k].Rect.b      =0;
	}
}
//-----------------------------------------------------------------
bool CEBlob::OnExecute()
{
	//OnClearData();
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CEBlob GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CEBlob GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CEBlob GImage Height Size"    )); return false; }
	// Euresys 
	EImageEncoder CodeImageEncoder     ;
	ECodedImage2  CodeImage            ;
	EObjectSelection CodeImageSelection;
	EROIBW8   DefectROI                ;

	// Value
	m_iPixelValue =0;
	m_iPixelCount =0;
	m_iBlobCount  =0;

    // Defect Merge Local Value
	const int xMerge     =3;
	const int yMerge     =3;
	//const int MergeCount =3;
	bool IsxMg=false, IsyMg=false, IsInMg=false;
	JRect r1, r2;
	int sort =0;
	int tmpCount=0;

	BinaryImg.SetSize(GImage->GetWidth(), GImage->GetHeight());
	EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage, &BinaryImg);
	BufferWidth = GImage->GetWidth();

	// Image Process Gray VS Binary
	if(Parameter.m_bBinaryInsp){ //Binary Image Blob 
		EasyImage::Threshold(GImage, &BinaryImg,Parameter.m_iThresHold);
		DefectROI.Attach(&BinaryImg);
		CodeImageSelection.SetFeretAngle(0.00f);
		if(Parameter.m_bDarkDefect){
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
		}
		else{
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
		} 
		IAddress = BinaryImg.GetImagePtr();
	}
	else {                      //Gray Image Blob 
		DefectROI.Attach(GImage);
		CodeImageSelection.SetFeretAngle(0.00f);
		if(Parameter.m_bDarkDefect){
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true );
		}
		else{
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true );
			CodeImageEncoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
		} 
		IAddress = GImage->GetImagePtr();
	}


	// Inspect Process 
	try{

		int roi_sx  = IRoi->GetOrgX();
		int roi_sy  = IRoi->GetOrgY();
		int roi_w   = IRoi->GetWidth();
		int roi_h   = IRoi->GetHeight();


		DefectROI.SetPlacement(roi_sx, roi_sy, roi_w, roi_h);

		if(roi_sx<=0 || roi_sy<=0 || roi_w<=0 || roi_h<=0) return false;
		if(roi_sx>=GImage->GetWidth() || roi_sy>=GImage->GetHeight() || roi_w>=GImage->GetWidth() || roi_h>=GImage->GetHeight()) return false;

		CodeImageSelection.Clear();
		CodeImageEncoder.Encode(DefectROI,CodeImage);
		CodeImageSelection.Clear();
		CodeImageSelection.AddObjects(CodeImage);
		// Blob 필터링 
		CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxWidth ,Parameter.m_fMinWidth , Parameter.m_fMaxWidth  , EDoubleThresholdMode_Outside);
		CodeImageSelection.RemoveUsingFloatFeature(EFeature_BoundingBoxHeight,Parameter.m_fMinHeight, Parameter.m_fMaxHeight , EDoubleThresholdMode_Outside);
		//CodeImageSelection.RemoveUsingIntegerFeature(EFeature_LeftLimit        ,0       , 1          , EDoubleThresholdMode_Outside);

		m_iBlobCount = CodeImageSelection.GetElementCount();

		// Blob Sorting 
		//CodeImageSelection.Sort(EFeature_TopLimit,ESortDirection_Ascending);

		if(m_iBlobCount!=0){
			for(int k=0; k<m_iBlobCount; k++){
				if(k>=MAX_DEFECT_POINT) break; 
				Blob[m_iDefectCount].Width  = CodeImageSelection.GetElement(k).GetBoundingBoxWidth() ;
				Blob[m_iDefectCount].Height = CodeImageSelection.GetElement(k).GetBoundingBoxHeight();

				Blob[m_iDefectCount].Rect.l      =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterX() - Blob[m_iDefectCount].Width/2 )+roi_sx);
				Blob[m_iDefectCount].Rect.t      =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - Blob[m_iDefectCount].Height/2)+roi_sy);
				Blob[m_iDefectCount].Rect.r      =(long)(Blob[m_iDefectCount].Rect.l + Blob[m_iDefectCount].Width) ; 
				Blob[m_iDefectCount].Rect.b      =(long)(Blob[m_iDefectCount].Rect.t + Blob[m_iDefectCount].Height);
				Blob[m_iDefectCount].Center.x    = CodeImageSelection.GetElement(k).GetBoundingBoxCenterX()+roi_sx;
				Blob[m_iDefectCount].Center.y    = CodeImageSelection.GetElement(k).GetBoundingBoxCenterY()+roi_sy;

				// 내부 Pixel Data 축출 
				for(int n=Blob[m_iDefectCount].Rect.t ; n<Blob[m_iDefectCount].Rect.b; n++){
					for(int m=Blob[m_iDefectCount].Rect.l; m<Blob[m_iDefectCount].Rect.r ; m++){
						m_iPixelValue = IElement(m,n);
						if(m_iPixelValue > Parameter.m_iThresHold) m_iPixelCount++;
					}
				}

				Blob[m_iDefectCount].Round = m_iPixelCount;
				Blob[m_iDefectCount].P.x    =(float)(Blob[m_iDefectCount].Rect.l); 
				Blob[m_iDefectCount].P.y    =(float)(Blob[m_iDefectCount].Rect.t );

				// Merge 위해서 데이터 필요함 
				//Blob[m_iDefectCount].Rect.top    =(long)((CodeImageSelection.GetElement(k).GetBoundingBoxCenterY() - Blob[m_iDefectCount].Height/2));
				//Blob[m_iDefectCount].Rect.bottom =(long)(Blob[m_iDefectCount].Rect.top + Blob[m_iDefectCount].Height);
				m_iDefectCount++;
			}

			// Merge Inspection 조건 
			Parameter.m_bMerge = (Parameter.m_iMergeCount==0)? false: true;

			// DefectBlob Merge Start 
			if(Parameter.m_bMerge){
				tmpCount = m_iDefectCount;
				for(int k=0; k<tmpCount; k++){
					if(Blob[k].Rect.l==0 && Blob[k].Rect.t==0 && Blob[k].Rect.r==0 && Blob[k].Rect.b==0) continue;

					for(int l=k+1; l<k+Parameter.m_iMergeCount; l++ ){
						IsxMg = false; IsyMg = false;
						if(l>(tmpCount-1)) break;
						if(Blob[l].Rect.l==0 && Blob[l].Rect.t==0 && Blob[k].Rect.r==0 && Blob[k].Rect.b==0) continue;

						r1 = Blob[k].Rect;
						r2 = Blob[l].Rect; 

						register int HorDist   = (r1.l < r2.l) ?  r2.l - r1.r  : r1.l - r2.r ;
						register int VerDist   = (r1.t  < r2.t ) ?  r2.t  - r1.b : r1.t  - r2.b;
						IsInMg    = ((r1.l<=r2.l) && (r1.t<r2.t) && (r1.b>r2.b) && (r1.r >r2.r))? true: false;

						if(HorDist>xMerge && VerDist>yMerge && !IsInMg ) continue;

						IsxMg = (HorDist<xMerge) ? true: false;
						IsyMg = (VerDist<yMerge) ? true: false;

						if((IsxMg && IsyMg) || IsInMg){
							Blob[k].Round    += Blob[l].Round;
							Blob[k].Rect      = OnMerge(Blob[k].Rect, Blob[l].Rect);
							Blob[k].Width     = Blob[k].Rect.r  - Blob[k].Rect.l;
							Blob[k].Height    = Blob[k].Rect.b - Blob[k].Rect.t ;
							Blob[k].P.x       = (float)(Blob[k].Rect.l);
							Blob[k].P.y       = (float)(Blob[k].Rect.t );
							Blob[l].Rect.l = Blob[l].Rect.t = Blob[l].Rect.r = Blob[l].Rect.b =0;
							m_iDefectCount--;
							k=-1;
							break;
						}
					}
				}
				for(int j=0; j<tmpCount; j++){
					if(Blob[j].Rect.l==0 && Blob[j].Rect.t==0 && Blob[j].Rect.r==0 && Blob[j].Rect.b==0) continue;
					Blob[sort] = Blob[j];
					sort++;
				}
			}
			return false;
		}
	}
	catch(EException& exc)
	{
		MessageBox(NULL, (CString)exc.What().c_str(), L"Inspection Blob Fail", MB_OK || MB_ICONWARNING);
	}

	return true;
}
//-------------------------------------------------------------------
JRect CEBlob::OnMerge( JRect ar, JRect br)
{
	JRect r;
	r.l = (ar.l < br.l) ? ar.l : br.l;
	r.t = (ar.t < br.t) ? ar.t : br.t;
	r.r = (ar.r > br.r) ? ar.r : br.r;
	r.b = (ar.b > br.b) ? ar.b : br.b;

	return r;
}
//-----------------------------------------------------------------
void CEBlob::OnPaint(TDrawTool Tool,EImageBW8*lpImage, int RColor)
{
	if (Tool.Handle==NULL) return;
	if (GImage==NULL     ) return;

	HGdi *Gdi = new HGdi(Tool.Handle);
	TPoint Point1, Point2;

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

	Gdi->CText.Color  = YELLOW_COLOR;
	Gdi->CFont.Height = 12;
	Gdi->CPen.Color = RColor;

	if(m_iDefectCount){
		for(int i=0; i<m_iDefectCount; i++){
	        Gdi->OnRect(false,(int)Blob[i].Rect.l,(int)Blob[i].Rect.t, (int)Blob[i].Rect.r, (int)Blob[i].Rect.b);
			Gdi->OnLinePlus((int)Blob[i].Center.x,(int)Blob[i].Center.y, 2);
		}
	}
	delete Gdi;

}
//-----------------------------------------------------------------
void CEBlob::OnLoad(bool IsLoad, CString Folder, int ItemId)
{

	CIFile Ini;
	CFileUnit FileUnit;
	CFile File;
	CString Path="";
	CString ItemNum="";

	ItemNum.Format(L"%d", ItemId);

	if (Folder.Trim()=="") return;

	Path = Folder +"\\EBlob" + ItemNum + ".ini";

	if(!FileUnit.FileExist(Path)) 	{
		File.Open(Path,CFile::modeCreate);
	}
	
	if (Path.Trim()=="") Path = PvPath;
	PvPath = Path;
	if (Path=="") return;

	Ini.Load(IsLoad, Path, "Parameter", "m_iThresHold"       , &Parameter.m_iThresHold	   );
	Ini.Load(IsLoad, Path, "Parameter", "m_iMergeCount"      , &Parameter.m_iMergeCount	   );
	Ini.Load(IsLoad, Path, "Parameter", "m_iRoiOffset"       , &Parameter.m_iRoiOffset 	   );
	Ini.Load(IsLoad, Path, "Parameter", "m_iDarkThresHold"   , &Parameter.m_iDarkThresHold );
	Ini.Load(IsLoad, Path, "Parameter", "m_fMinWidth"        , &Parameter.m_fMinWidth	   );
	Ini.Load(IsLoad, Path, "Parameter", "m_fMaxWidth"        , &Parameter.m_fMaxWidth	   );
	Ini.Load(IsLoad, Path, "Parameter", "m_fMinHeight"       , &Parameter.m_fMinHeight	   );
	Ini.Load(IsLoad, Path, "Parameter", "m_fMaxHeight"       , &Parameter.m_fMaxHeight	   );
	Ini.Load(IsLoad, Path, "Parameter", "m_bDarkBackGround"  , &Parameter.m_bDarkDefect    );
	Ini.Load(IsLoad, Path, "Parameter", "m_bBinaryInsp"      , &Parameter.m_bBinaryInsp    );
}

//-----------------------------------------------------------------
// CFBlob (Finder)
//-----------------------------------------------------------------
CFBlob::CFBlob(void)
{
	if(Parameter.MinWidth  ==0)Parameter.MinWidth  = 10 ;
	if(Parameter.MinHeight ==0)Parameter.MinHeight = 10 ;
	if(Parameter.MinWidth  ==0)Parameter.MaxWidth  = 500;
	if(Parameter.MinHeight ==0)Parameter.MaxHeight = 500;
	IAddress = NULL;
	GImage   = NULL;
}
CFBlob::~CFBlob(void)
{
}
bool CFBlob::OnInspection(HRect Rect, int SortMode)
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CFBlob GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CFBlob GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CFBlob GImage Height Size"    )); return false; }

	ECodedImage2     codedImage1               ; // ECodedImage2 instance
	EImageEncoder    codedImage1Encoder        ; // EImageEncoder instance
	EObjectSelection codedImage1ObjectSelection; // EObjectSelection instance
	EROIBW8          DefectROI                 ; // EasyObject ROI 

    Rect = Rect;
    Result.FindBlobCount = 0;
    if (SortMode) Parameter.AutoThHold = true;
    else          Parameter.AutoThHold = true;

//     void *IAddr = Image.GetImagePtr();
//     void *MAddr = Image.GetImagePtr();
//     IAddress = (BYTE *)IAddr;
//     MAddress = (BYTE *)MAddr;

	
// 중복되어 있어서 주석처리함 
//     if (!Parameter.AutoThHold) {
//         #define IElement(x, y)  *((BYTE *)IAddress + x + Width * y)
//         #define MElement(x, y)  *((BYTE *)MAddress + x + Width * y)
// 
//         for (int x=0; x<Width; x++) {
//         for (int y=0; y<Height; y++) {
//             if (Parameter.IsDarkBackGround) {
//                 MElement(x, y) = IElement(x, y)>=Parameter.ThHold ? 250 : 0;
//             }
//             else {
//                 MElement(x, y) = IElement(x, y)<Parameter.ThHold ? 250 : 0;
//             }
//         }}
//     }

    Result.RsRect = Rect;
    int FindBlob = 0;

	EBW8Image1.SetSize(GImage->GetWidth(), GImage->GetHeight());
	EasyImage::Oper(EArithmeticLogicOperation_Copy,GImage, &EBW8Image1);

    try
    {
        /*
        codedImage1ObjectSelection.SetFeretAngle(0.00f);
        EBW8Image1.SetSize(Width, Height);
        memcpy(EBW8Image1.GetImagePtr(), MAddress, (Width*Height));

        // Attach the roi to its parent
        EBW8Image1Roi1.SetPlacement(Rect.x, Rect.y, Rect.w, Rect.h);
        EBW8Image1Roi1.Attach(&EBW8Image1);
        codedImage1Encoder.Encode(EBW8Image1Roi1, codedImage1);
        codedImage1ObjectSelection.Clear();
        codedImage1ObjectSelection.AddObjects(codedImage1);
        codedImage1ObjectSelection.SetAttachedImage(&EBW8Image1);
        FindBlob = codedImage1.GetObjCount();
        int PvArea = 0;
        for (int k=0; k<FindBlob; k++) {
            int CuArea = codedImage1.GetObj(k).GetArea();
            if (CuArea>PvArea) {
                PvArea = CuArea;
                Result.RsRect.x = Rect.x+codedImage1.GetObj(k).GetBoundingBoxCenterX()-Rect.w/2;
                Result.RsRect.y = Rect.y+codedImage1.GetObj(k).GetBoundingBoxCenterY()-Rect.h/2;
            }
        }
        */
        if (SortMode) {
            codedImage1ObjectSelection.SetFeretAngle(0.00f);
//             EBW8Image1.SetSize(Width, Height);
//             memcpy(EBW8Image1.GetImagePtr(), Image.GetImagePtr(), (Width*Height));

            // Attach the roi to its parent
            DefectROI.Attach(&EBW8Image1);
            DefectROI.SetPlacement(Rect.x, Rect.y, Rect.w, Rect.h);
            codedImage1Encoder.Encode(DefectROI, codedImage1);
            codedImage1ObjectSelection.Clear();
            codedImage1ObjectSelection.AddObjects(codedImage1);

			//Blob Area(둘레길이) 면적이 설정값 이하는 Blob을 제거한다 
            codedImage1ObjectSelection.RemoveUsingUnsignedIntegerFeature(EFeature_Area, 20000, ESingleThresholdMode_Less);
            codedImage1ObjectSelection.Sort(EFeature_Area, ESortDirection_Ascending);

            FindBlob = codedImage1ObjectSelection.GetElementCount();

            //codedImage1ObjectSelection.SetAttachedImage(&EBW8Image1);
            //FindBlob = codedImage1.GetObjCount();

            if (FindBlob) {
                float cenX = codedImage1ObjectSelection.GetElement(0).GetBoundingBoxCenter().GetX();
                float cenY = codedImage1ObjectSelection.GetElement(0).GetBoundingBoxCenter().GetY();
                float Wid  = codedImage1ObjectSelection.GetElement(0).GetBoundingBoxWidth ();
                float Hei  = codedImage1ObjectSelection.GetElement(0).GetBoundingBoxHeight();

                Result.RsRect.x =(int)(cenX - Wid/2);
                Result.RsRect.y =(int)(cenY - Hei/2);
                Result.RsRect.w =(int)Wid;
                Result.RsRect.h =(int)Hei;
            }

            /*
            int PvArea = 0;
            for (int k=0; k<FindBlob; k++) {
                int CuArea = codedImage1.GetObj(k).GetArea();
                if (CuArea>PvArea) {
                    PvArea = CuArea;
                    Result.RsRect.x = Rect.x+codedImage1.GetObj(k).GetBoundingBoxCenterX()-Rect.w/2;
                    Result.RsRect.y = Rect.y+codedImage1.GetObj(k).GetBoundingBoxCenterY()-Rect.h/2;
                }
            }
            */
        }
        else { //Gray Image기준 제일 큰 Blob 데이터만 취합함(이유를 모르겠음.....)
            codedImage1ObjectSelection.SetFeretAngle(0.00f);
//             EBW8Image1.SetSize(Width, Height);
//             memcpy(EBW8Image1.GetImagePtr(), Image.GetImagePtr(), (Width*Height));

            codedImage1Encoder.SetSegmentationMethod(ESegmentationMethod_GrayscaleSingleThreshold);
			if (Parameter.IsDarkBackGround) {
				codedImage1Encoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(true);
				codedImage1Encoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(false);
			}
			else {
                codedImage1Encoder.GetGrayscaleSingleThresholdSegmenter().SetWhiteLayerEncoded(false);
                codedImage1Encoder.GetGrayscaleSingleThresholdSegmenter().SetBlackLayerEncoded(true);
			}
            codedImage1Encoder.GetGrayscaleSingleThresholdSegmenter().SetMode(EGrayscaleSingleThreshold_MinResidue);
            codedImage1Encoder.SetEncodingConnexity(EEncodingConnexity_Eight);

            // Attach the roi to its parent
            DefectROI.Attach(&EBW8Image1);
            DefectROI.SetPlacement(Rect.x, Rect.y, Rect.w, Rect.h);

            codedImage1Encoder.Encode(DefectROI, codedImage1);
            codedImage1ObjectSelection.Clear();
            codedImage1ObjectSelection.AddObjects(codedImage1);
            codedImage1ObjectSelection.SetAttachedImage(&EBW8Image1);
            FindBlob = codedImage1.GetObjCount();
            int PvArea = 0, CuArea =0, brWidth=0, brHeight=0;
            const int Gab =3;
            for (int k=0; k<FindBlob; k++) {
                CuArea   = (int)(codedImage1.GetObj(k).GetArea());
                brWidth  = (int)(codedImage1.GetObj(k).GetBoundingBoxWidth());
                brHeight = (int)(codedImage1.GetObj(k).GetBoundingBoxHeight());

                //if(brWidth >Parameter.MaxWidth  || brWidth <Parameter.MinWidth ) continue;
                //if(brHeight>Parameter.MaxHeight || brHeight<Parameter.MinHeight) continue;

                if (CuArea>PvArea) {
                    PvArea = CuArea;
                    //Result.RsRect.x = Rect.x+codedImage1.GetObj(k).GetBoundingBoxCenterX()-Rect.w/2;
                    //Result.RsRect.y = Rect.y+codedImage1.GetObj(k).GetBoundingBoxCenterY()-Rect.h/2;

                    //Result.RsRect.x = Rect.x+codedImage1.GetObj(k).GetLeftLimit()-Gab;
                    //Result.RsRect.y = Rect.y+codedImage1.GetObj(k).GetTopLimit ()-Gab;
                    Result.RsRect.x = (int)(Rect.x+codedImage1.GetObj(k).GetBoundingBoxCenterX() - brWidth /2 - Gab);
                    Result.RsRect.y = (int)(Rect.y+codedImage1.GetObj(k).GetBoundingBoxCenterY() - brHeight/2 - Gab);
                    Result.RsRect.w = brWidth +(2*Gab);
                    Result.RsRect.h = brHeight+(2*Gab);
                }
            }
        }
        Parameter.AutoThHold = false;
		return true;
        //return FindBlob;
    }
    catch(const EException&)
    {
      // Insert exception handling code here
    }
	return false;
}
//------------------------------------------------------------------------------
/* GOO 
void CBlob::OnPaint(TPanel *PTable, int Mode)
{
    OnPaint(PTable->Handle, Mode);
}
*/
//------------------------------------------------------------------------------
void CFBlob::OnPaint(TDrawTool Tool, int Mode)
{
    if (Tool.Handle==NULL) return;
    if (!Result.FindBlobCount) return;

	// GOO 
	HGdi *Gdi = new HGdi(Tool.Handle);
	
    Gdi->CText.Color = YELLOW_COLOR;
	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    Gdi->CBrush.Style = BS_HOLLOW;

    Gdi->OnLinePlus(Result.WPoint, 10);

    delete Gdi;
}
//--------------------------------------------------------------------------------
//CPolygon(Contour)
//--------------------------------------------------------------------------------
CPolygon::TBlob::TBlob()
{
	TPoint T(0,0);
	n = 0;
	Rect.Clear();
	Point    = NULL;
	OutPoint = NULL;
	InPoint  = NULL;
}

void CPolygon::TBlob::OnClear()
{
	TPoint T(0,0);
	n = 0;
	Rect.Clear();
	if (Point!=NULL) {delete[] Point; Point = NULL;}
}

CPolygon::CPolygon(void)
{
	TAddress       = NULL ;
	Blob           = NULL ;
	BlobCount      =    0 ;
	IAddress       = NULL ;
	GImage         = NULL ;

	PoPa.IgInSide         =  true;
	PoPa.IsSeparate       = false;
	PoPa.IsInToOut        = false;
	PoPa.MaxBbCnt         =  2000;
	PoPa.MinPxCnt         =    5 ;
	PoPa.MaxPxCnt         = 10000;
	PoPa.HiThHold         =     0;
	PoPa.LoThHold         =   100;
	PoPa.FMinWidth        =     0;
	PoPa.FMinHeight       =     0;
	PoPa.FMaxWidth        =     0;
	PoPa.FMaxHeight       =     0;
	PoPa.IsVerSort        = false;
	PoPa.IsHorSort        = false;
	PoPa.IsInToLeft       = false;
	PoPa.LeftFirstPoint   = false; //PolygonRect에서 LeftPoint 기준 First vs End 선택 옵션 
	PoPa.TopFirstPoint    = false; //PolygonRect에서 TopPoint  기준 First vs End 선택 옵션 
	PoPa.RightFirstPoint  = false; //PolygonRect에서 RightPoint기준 First vs End 선택 옵션 
	PoPa.BtmFirstPoint    = false; //PolygonRect에서 BtmPoint  기준 First vs End 선택 옵션 
}
CPolygon::~CPolygon(void)
{
}
int CPolygon::OnExecute(TPoint p1, TPoint p2, TPoint p3, TPoint p4)
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CPolygon GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CPolygon GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CPolygon GImage Height Size"    )); return false; }

	PoPa.MaxBbCnt = 2000;
	PoPa.MaxPxCnt = 65000;

	if (PoPa.MinPxCnt==0) PoPa.MinPxCnt = 5;

	register int FixMargin     = 4;
	register int Width         = IWidth              ; // Buffer Width
	register int Height        = IHeight             ; // Buffer Height
	register int IgInSide      = PoPa.IgInSide       ;
	register int IsSeparate    = PoPa.IsSeparate     ;
	register int MaxBbCnt      = PoPa.MaxBbCnt       ;
	register int MinPxCnt      = PoPa.MinPxCnt       ;
	register int MaxPxCnt      = PoPa.MaxPxCnt       ;
	register int HiThHold      = PoPa.HiThHold       ;
	register int LoThHold      = PoPa.LoThHold       ;
	register int IsVerSort     = PoPa.IsVerSort      ;
	register int IsHorSort     = PoPa.IsHorSort      ;
	register int IsLeftFristPt = PoPa.LeftFirstPoint ; //PolygonRect에서 LeftPoint 기준 First vs End 선택 옵션 
	register int IsTopFristPt  = PoPa.TopFirstPoint  ; //PolygonRect에서 TopPoint  기준 First vs End 선택 옵션 
	register int IsRightFristPt= PoPa.RightFirstPoint; //PolygonRect에서 RightPoint기준 First vs End 선택 옵션 
	register int IsBtmFristPt  = PoPa.BtmFirstPoint  ; //PolygonRect에서 BtmPoint  기준 First vs End 선택 옵션 


	// Polygon EdgePoint Data Init 
// 	for (int k=0; k<BlobCount; k++) {
// 		if (Blob[BlobCount].Point!=NULL) {
// 			delete[] Blob[k].Point;
// 			Blob[k].Point = NULL;
// 		}
// 		if(Blob[BlobCount].OutPoint!=NULL){
// 			delete[] Blob[k].OutPoint;
// 			Blob[k].OutPoint = NULL;
// 		}
// 		if(Blob[BlobCount].InPoint!=NULL){
// 			delete[] Blob[k].InPoint;
// 			Blob[k].InPoint = NULL;
// 		}
// 	}

	// Polygon Blob Data Init 
// 	if (BlobCount!=0) {
// 		for(int k=0; k<BlobCount; k++){
// 			if(Blob[k].OutPoint!=NULL) {delete[] Blob[k].OutPoint; Blob[k].OutPoint=NULL;}
// 			if(Blob[k].InPoint !=NULL) {delete[] Blob[k].InPoint ; Blob[k].InPoint =NULL;}
// 			if(Blob[k].Point   !=NULL) {delete[] Blob[k].Point   ; Blob[k].Point   =NULL;}
// 		}
// 	}

	if (Blob!=NULL) { delete[] Blob; Blob=NULL;} // 구조체 메모리 삭제 
	BlobCount = 0;
	Blob = new TBlob[MaxBbCnt];                  // 구조체 메모리 생성 

	if (IWidth <1                 ) return 0;
	if (IHeight<1                 ) return 0;
	if (HiThHold==0 && LoThHold==0) return 0;

// 	void *IAddress = GImage->GetImagePtr();
	IAddress    = GImage->GetImagePtr();
	BufferWidth = GImage->GetWidth()   ; 

	if (TAddress!=NULL) { delete[] TAddress; }
	TAddress = new BYTE[IWidth*IHeight];
	memset(TAddress, 0, IWidth*IHeight);

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

	register int c0, c1, c2, cm, cp;
	register int x0, y0, k, n, PixelCount;
	register int LeftCnt=0, RightCnt=0, OutDataCnt=0,InDataCnt=0, TopCnt=0, BtmCnt=0;

	// LeftPos기준 실제 EdgePoint 순서를 다시 정렬한다 
	double MinDv =100000000.0f;
	int    MinCnt=0;
	double PosDv =0.0f;

	// OutData,InData 재배치 변수 
	int ReverseInCnt  =0;
	int ReverseOutCnt =0;

	TFPoint *Point = new TFPoint[MaxPxCnt];

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
			if ((x-1)<=0 || x>=Width || y<=0 || y>=Height) continue;

			int AP = IElement(x, y);

			bool IsTrace;
			int p[8], p1[8], p2[8];

			c0 = HiThHold ? AP < HiThHold : true && LoThHold ? AP > LoThHold : true;

			IsTrace = c0;
			bool pIsVisited = TElement(x, y) ? true : false;

			if (IsTrace && !pIsVisited) {
				bool IsWall = false;
				bool IsEdge = false;
				PixelCount = 0;
				x0 = x; y0 = y;
				n = 4;
				bool IsBreak = false;
				do {
					register short u;
					register short v;

					register short IncNei = 1; // 1 Or 2

					for (k=0; k<8; k+=IncNei, n=((n+IncNei) & 7)) {
						u = (short)(x + nei[n].x);
						v = (short)(y + nei[n].y);
						if (u<0 || u>=Width || v<0 || v>=Height) {
							IsWall = true;
							continue;
						}

						// ROI영역을 벗어나는 경우 
// 						if(u<=rect.l || u>=rect.r || v<rect.t || v>rect.b) continue;
// 						if(u<rect.l || u>=rect.r || v<rect.t || v>rect.b) continue;
						if(u<rect.l || u>rect.r || v<rect.t || v>rect.b) continue;

						int E_AP = IElement(u, v);

						c2 = HiThHold ? E_AP < HiThHold : true && LoThHold ? E_AP > LoThHold : true;
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

					n = (n+4+IncNei) & 7;
				}
				while(!(x==x0 && y==y0));

				x=x0; y=y0;

				if (k==8) continue;

				if (PixelCount<MinPxCnt) continue;
				if (PixelCount>MaxPxCnt) continue;

				// Polygon Blob EdgePoint 데이터 생성...추후에 Defect검사시 필요함 
				Blob[BlobCount].Point = new TFPoint[PixelCount];
				Blob[BlobCount].n     = PixelCount;

				// Blob EdgePoint데이터 기준 Left포지션 Count,Right 포지션 Count 정보 Calc 
				LeftCnt =0, RightCnt=0, TopCnt=0, BtmCnt=0;

				// Blob 개별 외곽포인트 초기화 
				Blob[BlobCount].LeftPoint  .x =0.0f, Blob[BlobCount].LeftPoint  .y =0.0f; 
				Blob[BlobCount].TopPoint   .x =0.0f, Blob[BlobCount].TopPoint   .y =0.0f; 
				Blob[BlobCount].RightPoint .x =0.0f, Blob[BlobCount].RightPoint .y =0.0f; 
				Blob[BlobCount].BottomPoint.x =0.0f, Blob[BlobCount].BottomPoint.y =0.0f; 

				for (register int k=0; k<PixelCount; k++) {
					JRect *Rect = &Blob[BlobCount].Rect;
					 //기존 OrgSource기준 LeftPoint,TopPoint FirstPoint지정 /RightPoint,BtmPoint EndPoint지정 
					if(IsLeftFristPt){ //동일Point기준 첫번째 LeftPoint값을 인정  
						if     (Rect->l<= Point[k].x && Rect->l!=0) Rect->l = Rect->l;
						else {
							Rect->l = Point[k].x; LeftCnt = k ;
							Blob[BlobCount].LeftPoint.x = Point[k].x;
							Blob[BlobCount].LeftPoint.y = Point[k].y;
						}
					}
					else             { //동일Point기준 마지막 LeftPoint값을 인정  
						if     (Rect->l< Point[k].x && Rect->l!=0)  Rect->l = Rect->l;
						else {
							Rect->l = Point[k].x; LeftCnt = k ;
							Blob[BlobCount].LeftPoint.x = Point[k].x;
							Blob[BlobCount].LeftPoint.y = Point[k].y;
						}
					}

					if(IsRightFristPt){ //동일Point기준 첫번째 RightPoint값을 인정  
						if(Rect->r>=Point[k].x  && Rect->r!=0)      Rect->r = Rect->r;
						else {
							Rect->r = Point[k].x; RightCnt = k;
							Blob[BlobCount].RightPoint.x = Point[k].x;
							Blob[BlobCount].RightPoint.y = Point[k].y;
						}
					}
					else              { //동일Point기준 마지막 RightPoint값을 인정  
						if(Rect->r>Point[k].x && Rect->r!=0)        Rect->r = Rect->r;
						else {
							Rect->r = Point[k].x; RightCnt = k;
							Blob[BlobCount].RightPoint.x = Point[k].x;
							Blob[BlobCount].RightPoint.y = Point[k].y;
						}
					}

					if(IsTopFristPt){ //동일Point기준 첫번째 TopPoint값을 인정  
						if(Rect->t<=Point[k].y && Rect->t!=0)       Rect->t = Rect->t;
						else {
							Rect->t = Point[k].y; TopCnt =k;
							Blob[BlobCount].TopPoint.x = Point[k].x;
							Blob[BlobCount].TopPoint.y = Point[k].y;
						}
					}
					else            { //동일Point기준 마지막 TopPoint값을 인정  
						if(Rect->t<Point[k].y && Rect->t!=0)        Rect->t = Rect->t;
						else {
							Rect->t = Point[k].y; TopCnt =k;
							Blob[BlobCount].TopPoint.x = Point[k].x;
							Blob[BlobCount].TopPoint.y = Point[k].y;
						}
					}

					if(IsBtmFristPt){ //동일Point기준 첫번째 BtmPoint값을 인정  
						if(Rect->b>=Point[k].y)                     Rect->b = Rect->b;
						else {
							Rect->b = Point[k].y; BtmCnt =k;
							Blob[BlobCount].BottomPoint.x = Point[k].x;
							Blob[BlobCount].BottomPoint.y = Point[k].y;
						}
					}
					else            { //동일Point기준 마지막 BtmPoint값을 인정  
						if(Rect->b>Point[k].y)                      Rect->b = Rect->b;
						else {
							Rect->b = Point[k].y; BtmCnt =k;
							Blob[BlobCount].BottomPoint.x = Point[k].x;
							Blob[BlobCount].BottomPoint.y = Point[k].y;
						}
					}
					Blob[BlobCount].Point[k] = Point[k];
				}

				// MinWidth,MinHeight,MaxWidth,MaxHeight 설정값에 따라서 Blob제거
				if (PoPa.FMinWidth){
					JRect cr;
					cr = Blob[BlobCount].Rect;
					int v = (int)(cr.GetWidth());
					bool IsRemove = v<PoPa.FMinWidth;
					if (IsRemove) {
						Blob[BlobCount].Rect.l=0;
						Blob[BlobCount].Rect.t=0;
						Blob[BlobCount].Rect.r=0;
						Blob[BlobCount].Rect.b=0;
						continue;
					}
				}

				if (PoPa.FMaxWidth){
					JRect cr;
					cr = Blob[BlobCount].Rect;
					int v = (int)(cr.GetWidth());
					bool IsRemove = v>PoPa.FMaxWidth;
					if (IsRemove) {
						Blob[BlobCount].Rect.l=0;
						Blob[BlobCount].Rect.t=0;
						Blob[BlobCount].Rect.r=0;
						Blob[BlobCount].Rect.b=0;
						continue;
					}
				}

				if (PoPa.FMinHeight){
					JRect cr;
					cr = Blob[BlobCount].Rect;
					int v = (int)(cr.GetHeight());
					bool IsRemove = v<PoPa.FMinHeight;
					if (IsRemove) {
						Blob[BlobCount].Rect.l=0;
						Blob[BlobCount].Rect.t=0;
						Blob[BlobCount].Rect.r=0;
						Blob[BlobCount].Rect.b=0;
						continue;
					}
				}

				if (PoPa.FMaxHeight){
					JRect cr;
					cr = Blob[BlobCount].Rect;
					int v = (int)(cr.GetHeight());
					bool IsRemove = v>PoPa.FMaxHeight;
					if (IsRemove) {
						Blob[BlobCount].Rect.l=0;
						Blob[BlobCount].Rect.t=0;
						Blob[BlobCount].Rect.r=0;
						Blob[BlobCount].Rect.b=0;
						continue;
					}
				}

				if (IgInSide) { // Blob내부 Pixel데이터 IgNore처리함 
					for (register int n=Blob[BlobCount].Rect.t; n<Blob[BlobCount].Rect.b; n++) {
						for (register int m=Blob[BlobCount].Rect.l; m<Blob[BlobCount].Rect.r; m++) {
							TElement(m, n) = true;
						}}
				}

				// Pixel Data기준 InData,OutData 분리 
				if(IsSeparate){
					// 비선형 Blob 전체 PixelCount 기준으로 InDataCount, OutDataCount 분류할수 없음 
					// InData,OutData분류하여 메모리 동적생성 위해서는 정확한 PixelCount 필요함 
					Blob[BlobCount].OutCnt = 0         ;
					Blob[BlobCount].InCnt  = 0         ;
					OutDataCnt             = 0         ; // 전체 PixelCount중 실제 OutDatCnt 
					InDataCnt              = 0         ; // 전체 PixelCount중 실제 InDatCnt 
					ReverseInCnt           = PixelCount;
					ReverseOutCnt          = PixelCount;	

					// 전체 PixelCount기준 OutEdgePoint_Count, InEdgePoint_Count Calc
					if(IsHorSort){ //Left,Right기준 In/Out데이터 분류 
						for(register int k=0; k<PixelCount; k++){
							if(PoPa.IsInToOut){              // Notch영역
								if(k<LeftCnt || k>RightCnt){     // InPoint
									if(k<LeftCnt)   InDataCnt++; // InPoint
									if(k>RightCnt)  InDataCnt++; // InPoint
								}
								else                       {     // OutPoint              
									OutDataCnt++;
								}
							}
							else              {              // Pad 영역  
								// Org
								if(k<LeftCnt || k>RightCnt){       // OutPoint
									if(k<LeftCnt)   OutDataCnt++;  // OutPoint              
									if(k>RightCnt)  OutDataCnt++;  // OutPoint              
								}
// 								if(k<=LeftCnt || k>=RightCnt){     // OutPoint
// 									if(k<=LeftCnt)   OutDataCnt++; // OutPoint              
// 									if(k>=RightCnt)  OutDataCnt++; // OutPoint              
// 								}
								else                         {     // InPoint              
									InDataCnt++;
								}
							}
						}

						if(OutDataCnt>0 && InDataCnt>0){
							// 정확한 OutEdgePoint_Count, InEdgePoint_Count기준 메모리 동적 생성 
							Blob[BlobCount].OutPoint = new TFPoint[OutDataCnt];
							Blob[BlobCount].InPoint  = new TFPoint[InDataCnt ];

							for(register int k=0; k<PixelCount; k++){
								if(PoPa.IsInToOut){              // Notch영역
									if(k<LeftCnt || k>RightCnt){ // InPoint
										if(k<LeftCnt){
											Blob[BlobCount].InPoint[Blob[BlobCount].InCnt].x =Blob[BlobCount].Point[(LeftCnt-1)-k].x;
											Blob[BlobCount].InPoint[Blob[BlobCount].InCnt].y =Blob[BlobCount].Point[(LeftCnt-1)-k].y;
											Blob[BlobCount].InCnt++;
										}
										if(k>RightCnt){          // InPoint
											Blob[BlobCount].InPoint[Blob[BlobCount].InCnt].x =Blob[BlobCount].Point[ReverseInCnt-1].x;
											Blob[BlobCount].InPoint[Blob[BlobCount].InCnt].y =Blob[BlobCount].Point[ReverseInCnt-1].y;
											Blob[BlobCount].InCnt++;
											ReverseInCnt--;
										}
									}
									else                       { // OutPoint              
										Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].x =Blob[BlobCount].Point[k].x;
										Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].y =Blob[BlobCount].Point[k].y;
										Blob[BlobCount].OutCnt++;
									}
								}
								else              {              // Pad 영역  
									if(k<LeftCnt || k>RightCnt){ // OutPoint
										if(k<LeftCnt){
											Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].x =Blob[BlobCount].Point[(LeftCnt-1)-k].x;
											Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].y =Blob[BlobCount].Point[(LeftCnt-1)-k].y;
											Blob[BlobCount].OutCnt++;
										}
										if(k>RightCnt){   
											Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].x =Blob[BlobCount].Point[ReverseOutCnt-1].x;
											Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].y =Blob[BlobCount].Point[ReverseOutCnt-1].y;
											Blob[BlobCount].OutCnt++;
											ReverseOutCnt--;
										}
									}
// 									if(k<=LeftCnt || k>=RightCnt){ // OutPoint
// 										if(k<=LeftCnt){
// 											Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].x =Blob[BlobCount].Point[(LeftCnt-1)-k].x;
// 											Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].y =Blob[BlobCount].Point[(LeftCnt-1)-k].y;
// 											Blob[BlobCount].OutCnt++;
// 										}
// 										if(k>=RightCnt){   
// 											Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].x =Blob[BlobCount].Point[ReverseOutCnt-1].x;
// 											Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].y =Blob[BlobCount].Point[ReverseOutCnt-1].y;
// 											Blob[BlobCount].OutCnt++;
// 											ReverseOutCnt--;
// 										}
// 									}
									else                       { // InPoint              
										Blob[BlobCount].InPoint[Blob[BlobCount].InCnt].x =Blob[BlobCount].Point[k].x;
										Blob[BlobCount].InPoint[Blob[BlobCount].InCnt].y =Blob[BlobCount].Point[k].y;
										Blob[BlobCount].InCnt++;
									}
								}
							}
						}
					}
					else         { //Top,Btm기준 In/Out데이터 분류 
						//특정상황에서 문제 발생시 추가 분류 목적 
						if(TopCnt>BtmCnt) TopCnt =0;
						for(register int k=0; k<PixelCount; k++){
							if(PoPa.IsInToLeft){           // Top영역
								if(k>TopCnt && k<BtmCnt){  // OutPoint
									OutDataCnt++;
								}
								else                    {  // InPoint              
									InDataCnt++;
								}
							}
							else                        {  //Btm 영역  
								if(k>TopCnt && k<BtmCnt){  // InPoint
									InDataCnt++;
								}
								else                    {  // OutPoint              
									OutDataCnt++;
								}
							}
						}

						if(OutDataCnt>0 && InDataCnt>0){
							// 정확한 OutEdgePoint_Count, InEdgePoint_Count기준 메모리 동적 생성 
							Blob[BlobCount].OutPoint = new TFPoint[OutDataCnt];
							Blob[BlobCount].InPoint  = new TFPoint[InDataCnt ];

							for(register int k=0; k<PixelCount; k++){
								if(PoPa.IsInToLeft){              // Left영역
									if(k>TopCnt && k<BtmCnt){     // OutPoint
										Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].x =Blob[BlobCount].Point[k].x;
										Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].y =Blob[BlobCount].Point[k].y;
										Blob[BlobCount].OutCnt++;	
									}
									else                       { // InPoint              
										Blob[BlobCount].InPoint[Blob[BlobCount].InCnt].x =Blob[BlobCount].Point[ReverseInCnt-1].x;
										Blob[BlobCount].InPoint[Blob[BlobCount].InCnt].y =Blob[BlobCount].Point[ReverseInCnt-1].y;
										Blob[BlobCount].InCnt++;
										ReverseInCnt--;
									}
								}
								else               {              // Right 영역  
									if(k>TopCnt && k<BtmCnt){     // InPoint
										Blob[BlobCount].InPoint[Blob[BlobCount].InCnt].x =Blob[BlobCount].Point[k].x;
										Blob[BlobCount].InPoint[Blob[BlobCount].InCnt].y =Blob[BlobCount].Point[k].y;
										Blob[BlobCount].InCnt++;
									}
									else                       { // OutPoint              
										Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].x =Blob[BlobCount].Point[ReverseOutCnt-1].x;
										Blob[BlobCount].OutPoint[Blob[BlobCount].OutCnt].y =Blob[BlobCount].Point[ReverseOutCnt-1].y;
										Blob[BlobCount].OutCnt++;
										ReverseOutCnt--;
									}
								}
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

	delete[] Point;
	return BlobCount;
}
void CPolygon::ClearData()
{
	if (BlobCount!=0) {
		for(int k=0; k<BlobCount; k++){
			if(Blob[k].OutPoint!=NULL) {delete[] Blob[k].OutPoint;Blob[k].OutPoint=NULL; }
			if(Blob[k].InPoint !=NULL) {delete[] Blob[k].InPoint ;Blob[k].InPoint =NULL; }
			if(Blob[k].Point   !=NULL) {delete[] Blob[k].Point   ;Blob[k].Point   =NULL; }
		}
	}
}
int CPolygon::OnExecute(JRect rect)
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
	return OnExecute(p1, p2, p3, p4);
}
/*
int CPolygon::OnExecute(TPoint p1, TPoint p2, TPoint p3, TPoint p4)
{
	PoPa.MaxBbCnt = 2000;
	PoPa.MaxPxCnt = 65000;

	if (PoPa.MinPxCnt==0) PoPa.MinPxCnt = 5;

	register int FixMargin   = 4;
	register int Width       = IWidth  ; // Buffer Width
	register int Height      = IHeight ; // Buffer Height
	register int IgInSide    = PoPa.IgInSide;
	register int MaxBbCnt    = PoPa.MaxBbCnt;
	register int MinPxCnt    = PoPa.MinPxCnt;
	register int MaxPxCnt    = PoPa.MaxPxCnt;
	register int HiThHold    = PoPa.HiThHold;
	register int LoThHold    = PoPa.LoThHold;

	for (int k=0; k<BlobCount; k++) {
		if (Blob[BlobCount].Point!=NULL) {
			delete[] Blob[k].Point;
			Blob[k].Point = NULL;
		}
	}

	if (Blob!=NULL) {
		delete[] Blob;
		Blob=NULL;
	}
	BlobCount = 0;
	Blob = new TBlob[MaxBbCnt];

	if (IWidth <1) return 0;
	if (IHeight<1) return 0;

	void *IAddress = GImage->GetImagePtr();

	if (TAddress!=NULL) { delete[] TAddress; }
	TAddress = new BYTE[IWidth*IHeight];
	memset(TAddress, 0, IWidth*IHeight);

	//--------------------------------------------------------------------------
	// START : ??? ???? ?? RETURN
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
	// END : ??? ???? ?? RETURN
	//--------------------------------------------------------------------------

	register const POINT nei[8] =
	{
		{1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
	};

	register int c0, c1, c2, cm, cp;
	register int x0, y0, k, n, PixelCount;

	TPoint *Point = new TPoint[MaxPxCnt];

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
		if ((x-1)<=0 || x>=Width || y<=0 || y>=Height) continue;

		int AP = IElement(x, y);

		bool IsTrace;
		int p[8], p1[8], p2[8];

		c0 = HiThHold ? AP < HiThHold : true && LoThHold ? AP > LoThHold : true;

		IsTrace = c0;
		bool pIsVisited = TElement(x, y) ? true : false;

		if (IsTrace && !pIsVisited) {
			bool IsWall = false;
			bool IsEdge = false;
			PixelCount = 0;
			x0 = x; y0 = y;
			n = 4;
			bool IsBreak = false;
			do {
				register short u;
				register short v;

				register short IncNei = 1; // 1 Or 2

				for (k=0; k<8; k+=IncNei, n=((n+IncNei) & 7)) {
					u = (short)(x + nei[n].x);
					v = (short)(y + nei[n].y);
					if (u<0 || u>=Width || v<0 || v>=Height) {
						IsWall = true;
						continue;
					}

					int E_AP = IElement(u, v);

					c2 = HiThHold ? E_AP < HiThHold : true && LoThHold ? E_AP > LoThHold : true;
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

				n = (n+4+IncNei) & 7;
			}
			while(!(x==x0 && y==y0));

			x=x0; y=y0;

			if (k==8) continue;

			if (PixelCount<MinPxCnt) continue;
			if (PixelCount>MaxPxCnt) continue;

			// Wall
			//------------------------------------------------------------------

            WallCount = 0;
			for (register int k=0; k<10; k++) {
				Wall[k].StaId = -1;
				Wall[k].EndId = -1;
				Wall[k].SPoint.x = 0;
				Wall[k].EPoint.y = 0;
				Wall[k].SPoint.x = 0;
				Wall[k].EPoint.y = 0;
			}

			if (IsWall) {
			    for (register int k=0; k<PixelCount; k++) {
					bool IsWallPoint = false;
					if (Point[k].x<=2       ) IsWallPoint = true;
					if (Point[k].y<=2       ) IsWallPoint = true;
					if (Point[k].x>=Width-2 ) IsWallPoint = true;
					if (Point[k].y>=Height-2) IsWallPoint = true;

					if (IsWallPoint) {
						if (Wall[WallCount].StaId!=-1 && Wall[WallCount].EndId==-1) {
							Wall[WallCount].EndId = k;
							Wall[WallCount].EPoint = Point[k];
							WallCount++;
						}
					}
					else if (k==PixelCount-1) { // ??? Point 
						Wall[WallCount].EndId = k;
						Wall[WallCount].EPoint = Point[k];
						WallCount++;
					}
					else {
						if (Wall[WallCount].StaId==-1) {
							Wall[WallCount].StaId = k;
							Wall[WallCount].SPoint = Point[k];
							//WallCount++;
						}
						else {
							//Wall[WallCount].EndId = k;
							//Wall[WallCount].EPoint = Point[k];
						}
			    	}
				}
			}

			if (IsWall) {
				float Dist = Math.GetLength(Wall[WallCount-1].EPoint,  Wall[0].SPoint);
				if (Dist<3) {
// 					int Count1 = Wall[0          ].EndId - Wall[0          ].StaId;
// 					int Count2 = Wall[WallCount-1].EndId - Wall[WallCount-1].StaId;
// 					int Count = Count1+Count2
// 
// 					Blob[BlobCount].Point = new TPoint[Count];
// 
// 					Blob[BlobCount].n = 0;
// 					for (register int k=Wall[WallCount-1].StaId; k<=Wall[WallCount-1].Wall.EndId; k++) {
// 						Blob[BlobCount].Point[Blob[BlobCount].n] = Point[k];
// 						Blob[BlobCount].n++;
// 					}
// 
// 					for (register int k=Wall[0].StaId; k<=Wall[0].Wall.EndId; k++) {
// 						Blob[BlobCount].Point[Blob[BlobCount].n] = Point[k];
// 						Blob[BlobCount].n++;
// 					}

					for (int i=0; i<WallCount; i++) {
						int Count = Wall[i].EndId-Wall[i].StaId;

						Blob[BlobCount].Point = new TPoint[Count+1];

						for (register int k=Wall[i].StaId; k<=Wall[i].EndId; k++) {
							Blob[BlobCount].Point[Blob[BlobCount].n] = Point[k];
							Blob[BlobCount].n++;

							JRect *Rect = &Blob[BlobCount].Rect;
							Rect->l = Rect->l<Point[k].x && Rect->l!=0 ? Rect->l : Point[k].x;
							Rect->t = Rect->t<Point[k].y && Rect->t!=0 ? Rect->t : Point[k].y;
							Rect->r = Rect->r>Point[k].x               ? Rect->r : Point[k].x;
							Rect->b = Rect->b>Point[k].y               ? Rect->b : Point[k].y;
						}

						if (IgInSide) {
							for (register int n=Blob[BlobCount].Rect.t; n<Blob[BlobCount].Rect.b; n++) {
							for (register int m=Blob[BlobCount].Rect.l; m<Blob[BlobCount].Rect.r; m++) {
								TElement(m, n) = true;
							}}
						}

						BlobCount++;

						if (BlobCount>=MaxBbCnt) break;
					}
				}
				else {
					for (int i=0; i<WallCount; i++) {
						int Count = Wall[i].EndId-Wall[i].StaId;

						Blob[BlobCount].Point = new TPoint[Count+1];

						for (register int k=Wall[i].StaId; k<=Wall[i].EndId; k++) {
							Blob[BlobCount].Point[Blob[BlobCount].n] = Point[k];
							Blob[BlobCount].n++;

							JRect *Rect = &Blob[BlobCount].Rect;
							Rect->l = Rect->l<Point[k].x && Rect->l!=0 ? Rect->l : Point[k].x;
							Rect->t = Rect->t<Point[k].y && Rect->t!=0 ? Rect->t : Point[k].y;
							Rect->r = Rect->r>Point[k].x               ? Rect->r : Point[k].x;
							Rect->b = Rect->b>Point[k].y               ? Rect->b : Point[k].y;
						}

						if (IgInSide) {
							for (register int n=Blob[BlobCount].Rect.t; n<Blob[BlobCount].Rect.b; n++) {
							for (register int m=Blob[BlobCount].Rect.l; m<Blob[BlobCount].Rect.r; m++) {
								TElement(m, n) = true;
							}}
						}
						int test = Blob[0].Point[3].x;

						BlobCount++;

						if (BlobCount>=MaxBbCnt) break;
					}
				}
			}
			else {
				Blob[BlobCount].Point = new TPoint[PixelCount];

				for (register int k=0; k<PixelCount; k++) {
					JRect *Rect = &Blob[BlobCount].Rect;
					Rect->l = Rect->l<Point[k].x && Rect->l!=0 ? Rect->l : Point[k].x;
					Rect->t = Rect->t<Point[k].y && Rect->t!=0 ? Rect->t : Point[k].y;
					Rect->r = Rect->r>Point[k].x               ? Rect->r : Point[k].x;
					Rect->b = Rect->b>Point[k].y               ? Rect->b : Point[k].y;

					Blob[BlobCount].Point[k] = Point[k];
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
		}
		if (BlobCount>=MaxBbCnt) break;
	}
	if (BlobCount>=MaxBbCnt) break;
	}

	delete[] Point;
    return BlobCount;
}
*/
