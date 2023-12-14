#include "stdafx.h"
#include "LxTemplateMatch.h"

// #define IElement(x, y)  *((BYTE *)IAddress + x + Width * y)
// #define TElement(x, y)  *((BYTE *)TAddress + x + Width * y)

CTemplateMatch::CTemplateMatch(void)
{
	TpPa.MaxPxCnt     =  3000;
	//TpPa.MaxDefectCnt =    50;
	TpPa.Offset       =     5;
	TpPa.Samples      =     2;
	TpPa.LineStep     =     2;
	TpPa.CompareStep  =     2;
	TpPa.HiThMargin   =     0;
	TpPa.LoThMargin   =    20;
	TpPa.MinHorLength =     0;
	TpPa.MaxHorLength =  1000;
	TpPa.MinVerLength =     0;
	TpPa.MaxVerLength =  1000;
	TpPa.MinPxCnt     =    50;
	TpPa.xMgPxCnt     =     0;
	TpPa.yMgPxCnt     =     0;

	Point  = NULL;
	Defect = NULL;
	Group  = NULL;
	GImage = NULL;
}
CTemplateMatch::~CTemplateMatch(void)
{
}
TPoint RefPoint;
int OnSort(const void *b1, const void *b2)
{
    const int RIGHT_POS =  1;
    const int LEFT_POS  = -1;
    const int ORIG_POS  =  0;

    if (b1==NULL) return ORIG_POS;
    if (b2==NULL) return ORIG_POS;

    int c1 = ((TPoint *)b1)->x - ((TPoint *)b2)->x;
    int c2 = ((TPoint *)b1)->y - ((TPoint *)b2)->y;

    if (c1<0         ) return LEFT_POS;
    if (c1==0 && c2<0) return LEFT_POS;

    return RIGHT_POS;
    /*
    CMath Math;

    TPoint t1, t2;
    t1.x = ((TPoint *)b1)->x, t1.y = ((TPoint *)b1)->y;
    t2.x = ((TPoint *)b2)->x, t2.y = ((TPoint *)b2)->y;

    float dis1 = Math.GetLength(t1, RefPoint);
    float dis2 = Math.GetLength(t2, RefPoint);

    return dis1 > dis2 ? RIGHT_POS : LEFT_POS;
    */
}
void CTemplateMatch::OnSortPoint()
{
	for (register int i=0; i<GroupCount; i++) {
		qsort((void *)Group[i].Point, Group[i].PointCount, sizeof(Group[i].Point[0]), OnSort );
	}
}

void CTemplateMatch::OnInit(int w, int h, BYTE *IAddr)
{
// 	Buffer.Width    = w;
// 	Buffer.Height   = h;
// 	Buffer.IAddress = IAddr;
}
void CTemplateMatch::OnTrain(TPoint p1, TPoint p2, TPoint p3, TPoint p4)
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CTemplateMatch Train GImage Memory Allocate")); return; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CTemplateMatch Train GImage Width Size"     )); return; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CTemplateMatch Train GImage Height Size"    )); return; }

	p1.x += TpPa.Offset; p1.y += TpPa.Offset;
	p2.x -= TpPa.Offset; p2.y += TpPa.Offset;
	p3.x += TpPa.Offset; p3.y -= TpPa.Offset;
	p4.x -= TpPa.Offset; p4.y -= TpPa.Offset;

	IAddress    = GImage->GetImagePtr(); //Image Buffer Pointer
	BufferWidth = GImage->GetWidth()   ; //Image Buffer Width 

	register int Width    = GImage->GetWidth ();
	register int Height   = GImage->GetHeight();

	register float aTop   = Math.a(p1, p2);
	register float bTop   = Math.b(p1, p2);
	register float aRight = Math.a(p2, p4);
	register float bRight = Math.b(p2, p4);

	register float aBtm   = Math.a(p3, p4);
	register float bBtm   = Math.b(p3, p4);
	register float aLeft  = Math.a(p1, p3);
	register float bLeft  = Math.b(p1, p3);

	register int staX, endX;
	register int Samples = TpPa.Samples<=5 ? (TpPa.Samples>0 ? TpPa.Samples : 0) : 5;

	register int CuMin       = 255;
	register int CuMax       =   0;
	register int CurTh       =   0;
	register int LineCount   =   0;
	register int SampleCount =   0;
	register int y1          =   0;
	register int y2          =   0;

	register int HCount      =   0;
	register int VCount      =   0;

	const int MAX_H = 2000;
	const int MAX_V =    5;

	//Dynamic Allocation & Initialization
	int **Buff = new int*[MAX_V];
	for (register int i=0; i<MAX_V; i++) Buff[i] = new int[MAX_H];
	for (register int i=0; i<MAX_V; i++) {
		for (register int j=0; j<MAX_H; j++) {
			Buff[i][j] = 0;
		}}

	for (y1=p1.y, y2=p2.y; y1<p3.y; y1++, y2++) {
		HCount = 0;
		staX = (aLeft ==0) ? p1.x : (y1-bLeft )/aLeft ;
		endX = (aRight==0) ? p2.x : (y2-bRight)/aRight;

		register float bNew = Math.b(y1, aTop, staX);

		for (register int x=staX; x<endX; x++) {
			register int y = aTop*x + bNew;

			Buff[VCount][HCount] = IElement(x, y);
			HCount++;
			if (HCount>=MAX_H) break;
		}
		VCount++;

		if (VCount%Samples==0) {
			SampleCount = 0;
			for (register int k=0; k<HCount; k+=Samples) {
				CuMin = 255;
				CuMax = 0  ;
				for (register int mv=0; mv<Samples; mv++) {
					for (register int mh=0; mh<Samples; mh++) {
						if (k+mh >= HCount) break;
						if (Buff[mv][k+mh]==0) continue;
						CurTh = Buff[mv][k+mh];

						if (CuMin > CurTh) CuMin = CurTh;
						if (CuMax < CurTh) CuMax = CurTh;

					}}

				Train.MinTh[LineCount][SampleCount] = ((int)Train.MinTh[LineCount][SampleCount] > CuMin) ? (BYTE)CuMin : Train.MinTh[LineCount][SampleCount];
				Train.MaxTh[LineCount][SampleCount] = ((int)Train.MaxTh[LineCount][SampleCount] < CuMax) ? (BYTE)CuMax : Train.MaxTh[LineCount][SampleCount];

				SampleCount++;
				if (SampleCount >= NUMBEROF_SAMPLES) break;
			}
			LineCount++;
			if (LineCount >= NUMBEROF_LINES) break;
			for (register int i=0; i<MAX_V; i++) {
				for (register int j=0; j<MAX_H; j++) {
					Buff[i][j] = 0;
				}}
			VCount = 0;
		}
	}

	for (register int i=0; i<MAX_V; i++) {delete[] Buff[i]; Buff[i] = NULL;}
	delete[] Buff;
	Buff = NULL;
}
bool CTemplateMatch::OnInspection(int SelLight, TPoint p1, TPoint p2, TPoint p3, TPoint p4)
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CTemplateMatch Execute GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CTemplateMatch Execute GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CTemplateMatch Execute GImage Height Size"    )); return false; }

    IsErMisMatch     = false;
    IsErBumpMisMatch = false;
    IsErInkDust      = false;

    if(TpPa.MultiCompare){
        SetROI(SelLight, p1, p2, p3, p4);
    }

    p1.x += TpPa.Offset; p1.y += TpPa.Offset;
    p2.x -= TpPa.Offset; p2.y += TpPa.Offset;
    p3.x += TpPa.Offset; p3.y -= TpPa.Offset;
    p4.x -= TpPa.Offset; p4.y -= TpPa.Offset;

	IAddress    = GImage->GetImagePtr(); //Image Buffer Pointer
	BufferWidth = GImage->GetWidth()   ; //Image Buffer Width 

	register int Width    = GImage->GetWidth ();
	register int Height   = GImage->GetHeight();

    register float aTop   = Math.a(p1, p2);
    register float bTop   = Math.b(p1, p2);
    register float aRight = Math.a(p2, p4);
    register float bRight = Math.b(p2, p4);
    register float aBtm   = Math.a(p3, p4);
    register float bBtm   = Math.b(p3, p4);
    register float aLeft  = Math.a(p1, p3);
    register float bLeft  = Math.b(p1, p3);

    register int staX, endX;
    register int Samples         = TpPa.Samples                                       ;
    register int CompareStep     = TpPa.CompareStep     <=3 ? TpPa.CompareStep     : 3;
    register int CompareBumpStep = TpPa.CompareBumpStep <=2 ? TpPa.CompareBumpStep : 2;
    register int HiThMargin      = TpPa.HiThMargin                                    ;
    register int LoThMargin      = TpPa.LoThMargin                                    ;
    register int LineStep        = TpPa.LineStep                                      ;
    register int HiThBumpMargin  = TpPa.HiThBumpMargin                                ;
    register int LoThBumpMargin  = TpPa.LoThBumpMargin                                ;


    register int CurTh       =   0;
    register int LineCount   =   0;
    register int SampleCount =   0;
    register int y1          =   0;
    register int y2          =   0;
    register int HCount      =   0;
    register int VCount      =   0;
    register bool c0         = false;
    register bool EnableROI  = false;

    if (LineStep       <1) LineStep        = 1;
    if (CompareBumpStep<1) CompareBumpStep = 1;

    //Dynamic Allocation && Initialization   // 기존에 메모리 초기화및 동적 생성
    //if (Point!=NULL) {delete[] Point ; Point =NULL; }
    //Point = new TPoint[NUMBEROF_POINT_GROUP];
    //if(Point==NULL) ShowMessage("Point 메모리가 부족합니다!"  );

    //if (Group!=NULL) {delete[] Group ; Group =NULL; }
    //Group = new TGroup[NUMBEROF_MAX_GROUP];
    //if(Group==NULL) ShowMessage("Group 메모리가 부족합니다!"  );

    //if (Defect!=NULL){delete[] Defect; Defect=NULL; }
    //Defect = new TDefect[NUMBEROF_MAX_DEFECT];
    //if(Defect==NULL) ShowMessage("Defect 메모리가 부족합니다!");

    // 메모리 확인하기 위한 초기화및 동적생성 부분
    if(Point!=NULL) { free(Point); Point=NULL;}
    Point = (TPoint*)malloc(sizeof(TPoint)*NUMBEROF_POINT_PIXEL);
    if(Point==NULL) AfxMessageBox(L"Point 메모리가 부족합니다!"  );

    if (Group!=NULL){ free(Group); Group =NULL;}
    Group = (TGroup*)malloc(sizeof(TGroup)*NUMBEROF_MAX_GROUP);
    if(Group==NULL) AfxMessageBox(L"Group 메모리가 부족합니다!"  );

    if(Defect!=NULL){ free(Defect); Defect=NULL;}
    Defect = (TDefect*)malloc(sizeof(TDefect)*NUMBEROF_MAX_DEFECT);
    if(Defect==NULL) AfxMessageBox(L"Defect 메모리가 부족합니다!");

    //실시간 메모리 크기 Works//JTVDDI//LOG/EVENT 폴더에 크기 로그기록
    //AnsiString DataSize="";
    //AnsiString DataName="";
    //int Memorysize=0;

    //Memorysize = (sizeof(TPoint )*NUMBEROF_POINT_PIXEL);
    //DataSize = IntToStr(Memorysize);
    //DataName = "Point메모리크기:";
    //LogUnit.Trace(DataName+DataSize,0);

    //Memorysize = (sizeof(TGroup )*NUMBEROF_MAX_GROUP  );
    //DataSize = IntToStr(Memorysize);
    //DataName = "Group메모리크기:";
    //LogUnit.Trace(DataName+DataSize,0);

    //Memorysize = (sizeof(TDefect)*NUMBEROF_MAX_DEFECT );
    //DataSize = IntToStr(Memorysize);
    //DataName = "Defect메모리크기:";
    //LogUnit.Trace(DataName+DataSize,0);

    //clear
    for (register int i=0; i<NUMBEROF_MAX_GROUP  ; i++) {
    for (register int j=0; j<NUMBEROF_POINT_PIXEL; j++) {
        Group[i].Point[j].x = Group[i].Point[j].y = 0;
    }}
    PointCount = 0;
    for (register int i=0; i<NUMBEROF_POINT_PIXEL; i++) Point[i].x = Point[i].y = 0;

    GroupCount = 0;
    for (register int i=0; i<NUMBEROF_MAX_GROUP; i++) Group[i].PointCount = 0;

    DefectCount = 0; MissingCount = 0; InkDustCount = 0;
    for (register int i=0; i<NUMBEROF_MAX_DEFECT; i++) Defect[i].OnClear();
    IsErMisMatch = false;

    register const POINT nei[8] =
    {
        {1, 0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
    };
      
    for (y1=p1.y, y2=p2.y, VCount=0; y1<p3.y; y1+=LineStep, y2+=LineStep) {
        SampleCount = 0;
        if ((int)Train.MinTh[LineCount][SampleCount]==255 && (int)Train.MaxTh[LineCount][SampleCount]==0) continue;

        staX = (aLeft ==0) ? p1.x : (y1-bLeft )/aLeft ;
        endX = (aRight==0) ? p2.x : (y2-bRight)/aRight;

        register float bNew = Math.b(y1, aTop, staX);

    for (register int x=staX, HCount=0; x<endX; x++) {
        register int y = aTop*x + bNew;

        register int TrMinTh = (int)Train.MinTh[LineCount][SampleCount];
        register int TrMaxTh = (int)Train.MaxTh[LineCount][SampleCount];

        CurTh = IElement(x, y);

        if(TpPa.MultiCompare){
            if(EnableBumpROI || EnableDustROI || EnableScratchROI )
            { EnableROI = (SetPosition(SelLight, x , y)) ? 1 : 0;}
        }

        if(EnableROI){
            if      ( HiThBumpMargin &&  LoThBumpMargin) { c0 = (CurTh < (TrMinTh - LoThBumpMargin)) || (CurTh > (TrMaxTh + HiThBumpMargin)) ? true : false;}  //    yjh
            else if (!HiThBumpMargin &&  LoThBumpMargin) { c0 = (CurTh < (TrMinTh - LoThBumpMargin))                                         ? true : false;}
            else if ( HiThBumpMargin && !LoThBumpMargin) { c0 = (CurTh > (TrMaxTh + HiThBumpMargin))                                         ? true : false;}  
            else                                         { c0 =                                                                                       false;}

            //if ( HiThBumpMargin ) { c0 = (CurTh > (TrMaxTh + HiThBumpMargin))? true : false;}

            if (CompareBumpStep!=0 && c0) {
                for(register int i=0; i<                8; i++) {
                for(register int k=1; k<CompareBumpStep+1; k++) {
                    if (LineCount + nei[i].y*k < 0               || SampleCount + nei[i].x*k < 0                ) continue;
                    if (LineCount + nei[i].y*k >= NUMBEROF_LINES || SampleCount + nei[i].x*k >= NUMBEROF_SAMPLES) continue;

                    TrMinTh = (int)Train.MinTh[LineCount + nei[i].y*k][SampleCount + nei[i].x*k];
                    TrMaxTh = (int)Train.MaxTh[LineCount + nei[i].y*k][SampleCount + nei[i].x*k];
                    //CurTh   = IElement((x+((nei[i].x*k)*(TpPa.Samples))), (y+((nei[i].y*k)*(TpPa.Samples))));

                    if      ( HiThBumpMargin &&  LoThBumpMargin) { c0 = (CurTh < (TrMinTh - LoThBumpMargin)) || (CurTh > (TrMaxTh + HiThBumpMargin)) ? true : false;}
                    else if (!HiThBumpMargin &&  LoThBumpMargin) { c0 = (CurTh < (TrMinTh - LoThBumpMargin))                                         ? true : false;}
                    else if ( HiThBumpMargin && !LoThBumpMargin) { c0 = (CurTh > (TrMaxTh + HiThBumpMargin))                                         ? true : false;}
                    else                                         { c0 =                                                                                       false;}
                    //if ( HiThBumpMargin ) { c0 = (CurTh > (TrMaxTh + HiThBumpMargin))? true : false;}

                    if (!c0) break;
                }
                if (!c0) break;
                c0 = true;
                }
            }
        }
        else {
            if      ( HiThMargin &&  LoThMargin) { c0 = (CurTh < (TrMinTh - LoThMargin)) || (CurTh > (TrMaxTh + HiThMargin)) ? true : false;}
            else if (!HiThMargin &&  LoThMargin) { c0 = (CurTh < (TrMinTh - LoThMargin))                                     ? true : false;}
            else if ( HiThMargin && !LoThMargin) { c0 = (CurTh > (TrMaxTh + HiThMargin))                                     ? true : false;}
            else                                 { c0 =                                                                               false;}

            if (CompareStep!=0 && c0) {
                for(register int i=0; i<            8; i++) {
                for(register int k=1; k<CompareStep+1; k++) {
                    if (LineCount + nei[i].y*k < 0               || SampleCount + nei[i].x*k < 0                ) continue;
                    if (LineCount + nei[i].y*k >= NUMBEROF_LINES || SampleCount + nei[i].x*k >= NUMBEROF_SAMPLES) continue;

                    TrMinTh = (int)Train.MinTh[LineCount + nei[i].y*k][SampleCount + nei[i].x*k];
                    TrMaxTh = (int)Train.MaxTh[LineCount + nei[i].y*k][SampleCount + nei[i].x*k];
                    //CurTh   = IElement((x+((nei[i].x*k)*(TpPa.Samples))), (y+((nei[i].y*k)*(TpPa.Samples))));

                    if      ( HiThMargin &&  LoThMargin) { c0 = (CurTh < (TrMinTh - LoThMargin)) || (CurTh > (TrMaxTh + HiThMargin)) ? true : false;}
                    else if (!HiThMargin &&  LoThMargin) { c0 = (CurTh < (TrMinTh - LoThMargin))                                     ? true : false;}
                    else if ( HiThMargin && !LoThMargin) { c0 = (CurTh > (TrMaxTh + HiThMargin))                                     ? true : false;}
                    else                                 { c0 =                                                                               false;}

                    if (!c0) break;
                }
                if (!c0) break;
                c0 = true;
                }
            }
        }
        if (c0) {
            Point[PointCount].x = x, Point[PointCount].y = y;
            PointCount++;
            if(PointCount>=NUMBEROF_POINT_PIXEL) break;
        }
        HCount++, SampleCount = HCount/Samples;
        if (PointCount >= NUMBEROF_POINT_PIXEL || SampleCount >= NUMBEROF_SAMPLES) break;
    }

    VCount+=LineStep, LineCount = VCount/Samples;
    if (PointCount >= NUMBEROF_POINT_PIXEL || LineCount >= NUMBEROF_LINES) break;
    }
    for (register int i=0; i<PointCount; i++) {   // Contrast 차이에 기준으로 산출한 Point을 묶어서 Group을 만든다
        if (GroupCount==0) GroupCount++;
        else if (Group[GroupCount-1].PointCount==0) GroupCount++;
        else {
            if (Point[i].y - Group[GroupCount-1].Point[Group[GroupCount-1].PointCount-1].y > TpPa.LineGap) GroupCount++;
        }

        if (GroupCount>=NUMBEROF_MAX_GROUP) break;
        Group[GroupCount-1].Point[Group[GroupCount-1].PointCount] = Point[i];
        Group[GroupCount-1].PointCount++;

        if (Group[GroupCount-1].PointCount >= NUMBEROF_POINT_PIXEL) GroupCount++;
        if (GroupCount>=NUMBEROF_MAX_GROUP) break;
    }
    OnSortPoint();

    //const int DIS_POINT = 40;                         // Group을 가지고 다시 Defect을 만든다
    TDefect Temp;
    Temp.OnClear();
    if(TpPa.DIS_POINT==0) TpPa.DIS_POINT = 20;

    for (register int i=0; i<GroupCount         ; i++) {
    for (register int j=0; j<Group[i].PointCount; j++) {
        if (Temp.n==0) {
            Temp.Rect.l = Group[i].Point[j].x;
            Temp.Rect.t = Group[i].Point[j].y;
            Temp.Rect.r = Group[i].Point[j].x;
            Temp.Rect.b = Group[i].Point[j].y;
            Temp.n++;
            continue;
        }
        else {
            bool r = abs(Group[i].Point[j].x-Group[i].Point[j-1].x)<TpPa.DIS_POINT;

            if (r) {
                Temp.Rect.l = (Temp.Rect.l < Group[i].Point[j].x)? Temp.Rect.l : Group[i].Point[j].x;
                Temp.Rect.t = (Temp.Rect.t < Group[i].Point[j].y)? Temp.Rect.t : Group[i].Point[j].y;
                Temp.Rect.r = (Temp.Rect.r > Group[i].Point[j].x)? Temp.Rect.r : Group[i].Point[j].x;
                Temp.Rect.b = (Temp.Rect.b > Group[i].Point[j].y)? Temp.Rect.b : Group[i].Point[j].y;
                Temp.n++;

                if (j==Group[i].PointCount-1) {
                    if (TpPa.MinPxCnt) {
                        register bool IsMinPxCnt;
                        bool RemDefect = TpPa.MinPxCnt && TpPa.MinPxCnt > Temp.n;

                        if (!RemDefect) {
                            Defect[DefectCount].OnCopy(&Temp);
                            DefectCount++;
                            Temp.OnClear();
                        }
                    }
                    else {
                        Defect[DefectCount].OnCopy(&Temp);
                        DefectCount++;
                        Temp.OnClear();
                        continue;
                    }
                }
            }
            else {
                if (TpPa.MinPxCnt) {
                    register bool IsMinPxCnt;
                    bool RemDefect = TpPa.MinPxCnt && TpPa.MinPxCnt > Temp.n;

                    if (!RemDefect) {
                        Defect[DefectCount].OnCopy(&Temp);
                        DefectCount++;
                        Temp.OnClear();
                    }
                    if ( RemDefect) Temp.OnClear();
                }
                else {
                    Defect[DefectCount].OnCopy(&Temp);
                    DefectCount++;
                    Temp.OnClear();
                    continue;
                }
            }
        }
        if (DefectCount>=NUMBEROF_MAX_DEFECT) break;
    }}

    //Merge
    register bool IsxMg , IsyMg ;
    JRect r1, r2, r3;

    if(TpPa.xMgPxCnt || TpPa.yMgPxCnt){
        for (register int k=0; k<DefectCount; k++) {
            for (register int l=k+1; l<DefectCount; l++) {
                r1 = Defect[k].Rect   ;
                r2 = Defect[l].Rect   ;
                r3 = r3.OnMerge(r1,r2);

                IsxMg = (r3.GetHorDist(r1, r2) < TpPa.xMgPxCnt) && (TpPa.MaxHorLength ? (r3.xLength() < TpPa.MaxHorLength) : true) ;
                IsyMg = (r3.GetVerDist(r1, r2) < TpPa.yMgPxCnt) && (TpPa.MaxVerLength ? (r3.yLength() < TpPa.MaxVerLength) : true) ;

                if (IsxMg && IsyMg) {
                    Defect[k].n  += Defect[l].n;
                    Defect[k].Rect = Defect[k].Rect.OnMerge(Defect[k].Rect, Defect[l].Rect);
                    for (int j=l; j<DefectCount; j++) Defect[j].OnCopy(&Defect[j+1]);
                    DefectCount--;
                    k = -1 ;
                    break;
                }
            }
        }
    }
    //check size
    if(TpPa.MinHorLength || TpPa.MinVerLength || TpPa.MaxHorLength || TpPa.MaxVerLength || TpPa.MinPxCnt) {
        register bool IsMinVerLen , IsMinHorLen , IsMaxVerLen , IsMaxHorLen, IsMinPxCnt;
        bool RemDefect; bool IsBumpDefect = false;
        for (int k=0; k<DefectCount; k++) {
            int xlen = Defect[k].Rect.xLength();
            int ylen = Defect[k].Rect.yLength();
            int   cn = Defect[k].n             ;      // Defect Blob의 둘레길이
            int    x = Defect[k].Rect.Point().x;      // Defect Bolb의 중심점 x
            int    y = Defect[k].Rect.Point().y;      // Defect Bolb의 중심점 y
            int DefectTh = IElement(x,y);

            int startX = Defect[k].Rect.l;
            int startY = Defect[k].Rect.t;
            int n, m, cnt, sum;
            cnt = sum=0;
            for(n=startY; n<=startY+ylen; n++) {
                for(m=startX; m<=startX+xlen; m++) {
                    sum += IElement(m,n);
                    cnt++;
            }}
            int DefectAvgTh = sum/cnt;                // Defect Blob의 Pixel 평균값

            if(TpPa.MultiCompare){
                IsBumpDefect = SetPosition(SelLight, x , y) ? 1 : 0;
            }
            if(IsBumpDefect){
                if(TpPa.And){
                    if(DefectAvgTh>80 || (TpPa.MinDustHorLength!=0 && TpPa.MinDustVerLength!=0)) {                                                                 // Defect의 평균 Pixel값이 80 이상이면(Bright Blob) 해당 Defect을 Missing Bump로 검출
                        IsMinHorLen = (TpPa.MinBumpHorLength ? (TpPa.MinBumpHorLength > xlen) : true);   // 대부분의 Bump의 Pixel값 (Ramp Edge 발생하는 외곽까지  포함)을 확인하여 보니 대략 80 이하의 값을 지내고 있음.
                        IsMaxHorLen = (TpPa.MaxHorLength     ? (TpPa.MaxHorLength     < xlen) : true);
                        IsMinVerLen = (TpPa.MinBumpVerLength ? (TpPa.MinBumpVerLength > ylen) : true);
                        IsMaxVerLen = (TpPa.MaxVerLength     ? (TpPa.MaxVerLength     < ylen) : true);
                        RemDefect = IsMinVerLen || IsMinHorLen || IsMaxVerLen || IsMaxHorLen ;
                        if(!RemDefect){ RemDefect = (cn > TpPa.MinBumpPxCnt) ? false : true;}
                        if(!RemDefect) MissingCount++;
                    }
                    else {                                                                               // Defect의 Pixel값이 100 이하이면(Dark Blob) 해당 Defect을 Mis Match로 검출
                        IsMinHorLen = (TpPa.MinDustHorLength ? (TpPa.MinDustHorLength > xlen) : true);
                        IsMaxHorLen = (TpPa.MaxHorLength     ? (TpPa.MaxHorLength     < xlen) : true);
                        IsMinVerLen = (TpPa.MinDustVerLength ? (TpPa.MinDustVerLength > ylen) : true);
                        IsMaxVerLen = (TpPa.MaxVerLength     ? (TpPa.MaxVerLength     < ylen) : true);
                        RemDefect = IsMinVerLen || IsMinHorLen || IsMaxVerLen || IsMaxHorLen ;
                        if(!RemDefect){ RemDefect = (cn > TpPa.MinBumpPxCnt) ? false : true;}
                        if(!RemDefect) DefectCount++;
                    }
                    //IsMinHorLen = (TpPa.MinBumpHorLength ? (TpPa.MinBumpHorLength > xlen) : true);
                    //IsMaxHorLen = (TpPa.MaxHorLength     ? (TpPa.MaxHorLength     < xlen) : true);
                    //IsMinVerLen = (TpPa.MinBumpVerLength ? (TpPa.MinBumpVerLength > ylen) : true);
                    //IsMaxVerLen = (TpPa.MaxVerLength     ? (TpPa.MaxVerLength     < ylen) : true);
                    //RemDefect = IsMinVerLen || IsMinHorLen || IsMaxVerLen || IsMaxHorLen ;
                    //if(!RemDefect){ RemDefect = (cn > TpPa.MinBumpPxCnt) ? false : true;}
                }
                else{
                    if(DefectAvgTh>80 || (TpPa.MinDustHorLength!=0 && TpPa.MinDustVerLength!=0)) {                                                                 // Defect의 평균 Pixel값이 80 이상이면(Bright Blob) 해당 Defect을 Missing Bump로 검출
                        IsMinHorLen = (TpPa.MinBumpHorLength ? (TpPa.MinBumpHorLength > xlen) : true);
                        IsMaxHorLen = (TpPa.MaxHorLength     ? (TpPa.MaxHorLength     < xlen) : true);
                        IsMinVerLen = (TpPa.MinBumpVerLength ? (TpPa.MinBumpVerLength > ylen) : true);
                        IsMaxVerLen = (TpPa.MaxVerLength     ? (TpPa.MaxVerLength     < ylen) : true);
                        RemDefect  = IsMinVerLen && IsMinHorLen && IsMaxVerLen && IsMaxHorLen ;
                        if(!RemDefect){ RemDefect = (cn > TpPa.MinBumpPxCnt) ? false : true;}
                        if(!RemDefect) MissingCount++;
                    }
                    else {
                        IsMinHorLen = (TpPa.MinDustHorLength ? (TpPa.MinDustHorLength > xlen) : true);
                        IsMaxHorLen = (TpPa.MaxHorLength     ? (TpPa.MaxHorLength     < xlen) : true);
                        IsMinVerLen = (TpPa.MinDustVerLength ? (TpPa.MinDustVerLength > ylen) : true);
                        IsMaxVerLen = (TpPa.MaxVerLength     ? (TpPa.MaxVerLength     < ylen) : true);
                        RemDefect  = IsMinVerLen && IsMinHorLen && IsMaxVerLen && IsMaxHorLen ;
                        if(!RemDefect){ RemDefect = (cn > TpPa.MinBumpPxCnt) ? false : true;}
                        if(!RemDefect) DefectCount++;
                    }
                    //IsMinHorLen = (TpPa.MinBumpHorLength ? (TpPa.MinBumpHorLength > xlen) : true);
                    //IsMaxHorLen = (TpPa.MaxHorLength     ? (TpPa.MaxHorLength     < xlen) : true);
                    //IsMinVerLen = (TpPa.MinBumpVerLength ? (TpPa.MinBumpVerLength > ylen) : true);
                    //IsMaxVerLen = (TpPa.MaxVerLength     ? (TpPa.MaxVerLength     < ylen) : true);
                    //RemDefect  = IsMinVerLen && IsMinHorLen && IsMaxVerLen && IsMaxHorLen ;
                    //if(!RemDefect) { RemDefect = (cn > TpPa.MinBumpPxCnt) ? false : true;}
                }
                //if(!RemDefect) MissingCount++;
            }
            else {
                if(TpPa.And){
                    IsMinHorLen = (TpPa.MinHorLength ? (TpPa.MinHorLength > xlen) : true );
                    //IsMaxHorLen = (TpPa.MaxHorLength ? (TpPa.MaxHorLength < xlen) : false);
                    IsMinVerLen = (TpPa.MinVerLength ? (TpPa.MinVerLength > ylen) : true );
                    //IsMaxVerLen = (TpPa.MaxVerLength ? (TpPa.MaxVerLength < ylen) : false);

                    RemDefect = IsMinVerLen || IsMinHorLen ;
                }
                else{
                    IsMinHorLen = (TpPa.MinHorLength ? (TpPa.MinHorLength > xlen) : true );
                    //IsMaxHorLen = (TpPa.MaxHorLength ? (TpPa.MaxHorLength < xlen) : false);
                    IsMinVerLen = (TpPa.MinVerLength ? (TpPa.MinVerLength > ylen) : true );
                    //IsMaxVerLen = (TpPa.MaxVerLength ? (TpPa.MaxVerLength < ylen) : false);

                    RemDefect = IsMinVerLen && IsMinHorLen;
                }
            }

            if (RemDefect) {
                for (int j=k; j<DefectCount; j++) Defect[j].OnCopy(&Defect[j+1]);
                DefectCount--;
                k--;
            }
        }

        if (DefectCount > 0) {
            int Count, PixelCount, SumPxValue, PxValue =0;   //khj
            for (register int k=0; k<DefectCount; k++) {
               //int xlen     = Defect[k].Rect.xLength();
               //int ylen     = Defect[k].Rect.yLength();
               Count      = Defect[k].n ;
               PixelCount = 0           ;
               SumPxValue = 0           ;
               PxValue    = 0           ;

               if(Count>TpPa.InkDustPixel){
                   for (int y = Defect[k].Rect.t; y<=Defect[k].Rect.b; y++) {
                   for (int x = Defect[k].Rect.l; x<=Defect[k].Rect.r; x++) {
//                        SumPxValue+=Camera[Vision.CameraId].GetElement(x, y);
					   SumPxValue = IElement(x,y);
                       PixelCount++;
                   }}
                   PxValue = SumPxValue / PixelCount;
                   if(PxValue <70) InkDustCount++ ;
               }
            }
        }
    }
    /*
    if(TpPa.MinHorLength || TpPa.MinVerLength || TpPa.MaxHorLength || TpPa.MaxVerLength || TpPa.MinPxCnt) {
        register bool IsMinVerLen1 , IsMinHorLen1 , IsMinVerLen , IsMinHorLen , IsMaxVerLen , IsMaxHorLen, IsMinPxCnt;
        bool RemDefect, RemDefect2; bool IsBumpDefect = false;
        for (register int k=0; k<DefectCount; k++) {
            int xlen = Defect[k].Rect.xLength();
            int ylen = Defect[k].Rect.yLength();
            int   cn = Defect[k].n             ;
            int x    = Defect[k].Rect.Point().x;
            int y    = Defect[k].Rect.Point().y;

            //int DefectCenterX = Defect[k].Rect.l + (Defect[k].Rect.xLength()/2);  //yjh
            //int DefectCenterY = Defect[k].Rect.t + (Defect[k].Rect.yLength()/2);
            //int DefectTh = IElement(DefectCenterX, DefectCenterY);
            int DefectTh = IElement(x,y);

            if(TpPa.MultiCompare){
                IsBumpDefect = SetPosition(SelLight, x , y) ? 1 : 0;
            }
            if(IsBumpDefect){
                if(TpPa.And){
                    IsMinHorLen = (TpPa.MinBumpHorLength ? (TpPa.MinBumpHorLength > xlen) : true);
                    IsMaxHorLen = (TpPa.MaxHorLength     ? (TpPa.MaxHorLength     < xlen) : true);
                    IsMinVerLen = (TpPa.MinBumpVerLength ? (TpPa.MinBumpVerLength > ylen) : true);
                    IsMaxVerLen = (TpPa.MaxVerLength     ? (TpPa.MaxVerLength     < ylen) : true);

                    RemDefect = IsMinVerLen || IsMinHorLen || IsMaxVerLen || IsMaxHorLen ;
                    if(!RemDefect){ RemDefect = (cn > TpPa.MinBumpPxCnt) ? false : true;}
                }
                else{
                    IsMinHorLen = (TpPa.MinBumpHorLength ? (TpPa.MinBumpHorLength > xlen) : true);
                    IsMaxHorLen = (TpPa.MaxHorLength     ? (TpPa.MaxHorLength     < xlen) : true);
                    IsMinVerLen = (TpPa.MinBumpVerLength ? (TpPa.MinBumpVerLength > ylen) : true);
                    IsMaxVerLen = (TpPa.MaxVerLength     ? (TpPa.MaxVerLength     < ylen) : true);

                    RemDefect  = IsMinVerLen  && IsMinHorLen  && IsMaxVerLen && IsMaxHorLen ;
                    if(!RemDefect) { RemDefect = (cn > TpPa.MinBumpPxCnt) ? false : true;}

                    //(DefectTh > HiThBumpMargin) { if(!RemDefect) MissingCount++;}
                    //else                          { if(!RemDefect) DefectCount++;}

                    //IsMinHorLen1 = (TpPa.MinHorLength ? (TpPa.MinHorLength > xlen) : true );  //yjh
                    //IsMinVerLen1 = (TpPa.MinVerLength ? (TpPa.MinVerLength > ylen) : true );  //yjh
                    //RemDefect2 = IsMinVerLen1 && IsMinHorLen1 && IsMaxVerLen && IsMaxHorLen ; //yjh

                    //if(!RemDefect){ RemDefect = (cn > TpPa.MinBumpPxCnt) ? false : true;}
                }
                //if(!RemDefect)  { MissingCount++; RemDefect2 = true; }
                //if(!RemDefect2) { DefectCount++ ; }  //yjh
                if(!RemDefect) MissingCount++;
            }
            else {
                if(TpPa.And){
                    IsMinHorLen = (TpPa.MinHorLength ? (TpPa.MinHorLength > xlen) : true );
                    IsMaxHorLen = (TpPa.MaxHorLength ? (TpPa.MaxHorLength < xlen) : false);
                    IsMinVerLen = (TpPa.MinVerLength ? (TpPa.MinVerLength > ylen) : true );
                    IsMaxVerLen = (TpPa.MaxVerLength ? (TpPa.MaxVerLength < ylen) : false);

                    RemDefect = IsMinVerLen || IsMinHorLen || IsMaxVerLen || IsMaxHorLen  ;
                }
                else{
                    IsMinHorLen = (TpPa.MinHorLength ? (TpPa.MinHorLength > xlen) : true);
                    IsMaxHorLen = (TpPa.MaxHorLength ? (TpPa.MaxHorLength < xlen) : true);
                    IsMinVerLen = (TpPa.MinVerLength ? (TpPa.MinVerLength > ylen) : true);
                    IsMaxVerLen = (TpPa.MaxVerLength ? (TpPa.MaxVerLength < ylen) : true);

                    RemDefect = IsMinVerLen && IsMinHorLen && IsMaxVerLen && IsMaxHorLen ;
                }
            }

            if (RemDefect) {
                for (int j=k; j<DefectCount; j++) Defect[j].OnCopy(&Defect[j+1]);
                DefectCount--;
                k--;
            }
        }

        if (DefectCount > 0) {
            int Count, PixelCount, SumPxValue, PxValue =0;   //khj
            for (register int k=0; k<DefectCount; k++) {
               //int xlen     = Defect[k].Rect.xLength();
               //int ylen     = Defect[k].Rect.yLength();
               Count      = Defect[k].n ;
               PixelCount = 0           ;
               SumPxValue = 0           ;
               PxValue    = 0           ;

               if(Count>TpPa.InkDustPixel){
                   for (int y = Defect[k].Rect.t; y<=Defect[k].Rect.b; y++) {
                   for (int x = Defect[k].Rect.l; x<=Defect[k].Rect.r; x++) {
                       SumPxValue+=Camera[Vision.CameraId].GetElement(x, y);
                       PixelCount++;
                   }}
                   PxValue = SumPxValue / PixelCount;
                   if(PxValue <70) InkDustCount++ ;
               }
            }
        }
    }
    */

    IsErBumpMisMatch = MissingCount ? true : false;
    IsErMisMatch     = DefectCount  ? true : false;
    IsErInkDust      = InkDustCount ? true : false;

    return (!IsErMisMatch && !IsErBumpMisMatch && !IsErInkDust);
}
void CTemplateMatch::ClrTrain()
{
    for (register int i=0; i<NUMBEROF_LINES  ; i++) {
    for (register int j=0; j<NUMBEROF_SAMPLES; j++) {
        Train.MinTh[i][j] = (BYTE)255;
        Train.MaxTh[i][j] = (BYTE)  0;
    }}
}
void CTemplateMatch::ClrResult()
{
    for(int k=0; k<4; k++){
        LBumpROI[k].x    = 0;   LBumpROI[k].y    = 0;
        TBumpROI[k].x    = 0;   TBumpROI[k].y    = 0;
        RBumpROI[k].x    = 0;   RBumpROI[k].y    = 0;
        BBumpROI[k].x    = 0;   BBumpROI[k].y    = 0;

        LDustROI[k].x    = 0;   LDustROI[k].y    = 0;
        TDustROI[k].x    = 0;   TDustROI[k].y    = 0;
        RDustROI[k].x    = 0;   RDustROI[k].y    = 0;
        BDustROI[k].x    = 0;   BDustROI[k].y    = 0;

        LScratchROI[k].x = 0;   LScratchROI[k].y = 0;
        TScratchROI[k].x = 0;   TScratchROI[k].y = 0;
        RScratchROI[k].x = 0;   RScratchROI[k].y = 0;
        BScratchROI[k].x = 0;   BScratchROI[k].y = 0;
    }
    IsErMisMatch      = false;
    IsErBumpMisMatch  = false;
    IsErInkDust       = false;
    EnableBumpROI     = false;
    EnableDustROI     = false;
    EnableScratchROI  = false;
}
void CTemplateMatch::SetROI(int SelLight, TPoint p1, TPoint p2, TPoint p3, TPoint p4)
{
//     CBump  *lpBump    = NULL;
//     if      (SelLight==0) lpBump = &Vision.Bump        ;
//     else if (SelLight==1) lpBump = &Vision.DkBump      ;
//     else return;
// 
//     CCrack *lpDust    = NULL;
//     if      (SelLight==0) lpDust = &Vision.Dust        ;
//     else if (SelLight==1) lpDust = &Vision.DkDust      ;
//     else return;
// 
//     CCrack *lpScratch = NULL;
//     if      (SelLight==0) lpScratch = &Vision.Scratch  ;
//     else if (SelLight==1) lpScratch = &Vision.DkScratch;
//     else return;
// 
// 
//     float degTop = Math.GetDegree(p1, p2);
//     float degBtm = Math.GetDegree(p3, p4);
// 
//     if(Vision.ItemEnabled[SelLight][3] || Vision.ItemEnabled[SelLight][4] ||
//        Vision.ItemEnabled[SelLight][5] || Vision.ItemEnabled[SelLight][6]   ){
//         EnableBumpROI = true;
// 
//         register int dxLROI = lpBump->BmPa.dxLROI;
//         register int dyLROI = lpBump->BmPa.dyLROI;
//         register int dwLROI = lpBump->BmPa.dwLROI;
//         register int dhLROI = lpBump->BmPa.dhLROI;
//         register int dxTROI = lpBump->BmPa.dxTROI;
//         register int dyTROI = lpBump->BmPa.dyTROI;
//         register int dwTROI = lpBump->BmPa.dwTROI;
//         register int dhTROI = lpBump->BmPa.dhTROI;
//         register int dxRROI = lpBump->BmPa.dxRROI;
//         register int dyRROI = lpBump->BmPa.dyRROI;
//         register int dwRROI = lpBump->BmPa.dwRROI;
//         register int dhRROI = lpBump->BmPa.dhRROI;
//         register int dxBROI = lpBump->BmPa.dxBROI;
//         register int dyBROI = lpBump->BmPa.dyBROI;
//         register int dwBROI = lpBump->BmPa.dwBROI;
//         register int dhBROI = lpBump->BmPa.dhBROI;
// 
//         LBumpROI[0] = Math.OnRotate(p1.x, p1.y, p1.x+dxLROI       , p1.y+dyLROI       , degTop);
//         LBumpROI[1] = Math.OnRotate(p1.x, p1.y, p1.x+dxLROI+dwLROI, p1.y+dyLROI       , degTop);
//         LBumpROI[2] = Math.OnRotate(p3.x, p3.y, p3.x+dxLROI       , p3.y-dhLROI       , degBtm);
//         LBumpROI[3] = Math.OnRotate(p3.x, p3.y, p3.x+dxLROI+dwLROI, p3.y-dhLROI       , degBtm);
// 
//         TBumpROI[0] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI       , p1.y+dyTROI       , degTop);
//         TBumpROI[1] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI+dwTROI, p1.y+dyTROI       , degTop);
//         TBumpROI[2] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI       , p1.y+dyTROI+dhTROI, degTop);
//         TBumpROI[3] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI+dwTROI, p1.y+dyTROI+dhTROI, degTop);
// 
//         RBumpROI[0] = Math.OnRotate(p1.x, p1.y, p1.x+dxRROI       , p1.y+dyRROI       , degTop);
//         RBumpROI[1] = Math.OnRotate(p1.x, p1.y, p1.x+dxRROI+dwRROI, p1.y+dyRROI       , degTop);
//         RBumpROI[2] = Math.OnRotate(p3.x, p3.y, p3.x+dxRROI       , p3.y-dhRROI       , degBtm);
//         RBumpROI[3] = Math.OnRotate(p3.x, p3.y, p3.x+dxRROI+dwRROI, p3.y-dhRROI       , degBtm);
// 
//         BBumpROI[0] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI       , p3.y-dyBROI       , degBtm);
//         BBumpROI[1] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI+dwBROI, p3.y-dyBROI       , degBtm);
//         BBumpROI[2] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI       , p3.y-dyBROI+dhBROI, degBtm);
//         BBumpROI[3] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI+dwBROI, p3.y-dyBROI+dhBROI, degBtm);
//     }
//     if(Vision.ItemEnabled[SelLight][7] || Vision.ItemEnabled[SelLight][8] ||
//        Vision.ItemEnabled[SelLight][9] || Vision.ItemEnabled[SelLight][10]   ){
//         EnableDustROI = true;
// 
//         register int dxLROI = lpDust->CrPa.dxLROI;
//         register int dyLROI = lpDust->CrPa.dyLROI;
//         register int dwLROI = lpDust->CrPa.dwLROI;
//         register int dhLROI = lpDust->CrPa.dhLROI;
//         register int dxTROI = lpDust->CrPa.dxTROI;
//         register int dyTROI = lpDust->CrPa.dyTROI;
//         register int dwTROI = lpDust->CrPa.dwTROI;
//         register int dhTROI = lpDust->CrPa.dhTROI;
//         register int dxRROI = lpDust->CrPa.dxRROI;
//         register int dyRROI = lpDust->CrPa.dyRROI;
//         register int dwRROI = lpDust->CrPa.dwRROI;
//         register int dhRROI = lpDust->CrPa.dhRROI;
//         register int dxBROI = lpDust->CrPa.dxBROI;
//         register int dyBROI = lpDust->CrPa.dyBROI;
//         register int dwBROI = lpDust->CrPa.dwBROI;
//         register int dhBROI = lpDust->CrPa.dhBROI;
// 
//         LDustROI[0] = Math.OnRotate(p1.x, p1.y, p1.x+dxLROI       , p1.y+dyLROI       , degTop);
//         LDustROI[1] = Math.OnRotate(p1.x, p1.y, p1.x+dxLROI+dwLROI, p1.y+dyLROI       , degTop);
//         LDustROI[2] = Math.OnRotate(p3.x, p3.y, p3.x+dxLROI       , p3.y-dhLROI       , degBtm);
//         LDustROI[3] = Math.OnRotate(p3.x, p3.y, p3.x+dxLROI+dwLROI, p3.y-dhLROI       , degBtm);
// 
//         TDustROI[0] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI       , p1.y+dyTROI       , degTop);
//         TDustROI[1] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI+dwTROI, p1.y+dyTROI       , degTop);
//         TDustROI[2] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI       , p1.y+dyTROI+dhTROI, degTop);
//         TDustROI[3] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI+dwTROI, p1.y+dyTROI+dhTROI, degTop);
// 
//         RDustROI[0] = Math.OnRotate(p1.x, p1.y, p1.x+dxRROI       , p1.y+dyRROI       , degTop);
//         RDustROI[1] = Math.OnRotate(p1.x, p1.y, p1.x+dxRROI+dwRROI, p1.y+dyRROI       , degTop);
//         RDustROI[2] = Math.OnRotate(p3.x, p3.y, p3.x+dxRROI       , p3.y-dhRROI       , degBtm);
//         RDustROI[3] = Math.OnRotate(p3.x, p3.y, p3.x+dxRROI+dwRROI, p3.y-dhRROI       , degBtm);
// 
//         BDustROI[0] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI       , p3.y-dyBROI       , degBtm);
//         BDustROI[1] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI+dwBROI, p3.y-dyBROI       , degBtm);
//         BDustROI[2] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI       , p3.y-dyBROI+dhBROI, degBtm);
//         BDustROI[3] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI+dwBROI, p3.y-dyBROI+dhBROI, degBtm);
//     }
//     if(Vision.ItemEnabled[SelLight][11] || Vision.ItemEnabled[SelLight][12] ||
//        Vision.ItemEnabled[SelLight][13] || Vision.ItemEnabled[SelLight][14]   ){
//         EnableScratchROI = true;
// 
//         register int dxLROI = lpScratch->CrPa.dxLROI;
//         register int dyLROI = lpScratch->CrPa.dyLROI;
//         register int dwLROI = lpScratch->CrPa.dwLROI;
//         register int dhLROI = lpScratch->CrPa.dhLROI;
//         register int dxTROI = lpScratch->CrPa.dxTROI;
//         register int dyTROI = lpScratch->CrPa.dyTROI;
//         register int dwTROI = lpScratch->CrPa.dwTROI;
//         register int dhTROI = lpScratch->CrPa.dhTROI;
//         register int dxRROI = lpScratch->CrPa.dxRROI;
//         register int dyRROI = lpScratch->CrPa.dyRROI;
//         register int dwRROI = lpScratch->CrPa.dwRROI;
//         register int dhRROI = lpScratch->CrPa.dhRROI;
//         register int dxBROI = lpScratch->CrPa.dxBROI;
//         register int dyBROI = lpScratch->CrPa.dyBROI;
//         register int dwBROI = lpScratch->CrPa.dwBROI;
//         register int dhBROI = lpScratch->CrPa.dhBROI;
// 
//         LScratchROI[0] = Math.OnRotate(p1.x, p1.y, p1.x+dxLROI       , p1.y+dyLROI       , degTop);
//         LScratchROI[1] = Math.OnRotate(p1.x, p1.y, p1.x+dxLROI+dwLROI, p1.y+dyLROI       , degTop);
//         LScratchROI[2] = Math.OnRotate(p3.x, p3.y, p3.x+dxLROI       , p3.y-dhLROI       , degBtm);
//         LScratchROI[3] = Math.OnRotate(p3.x, p3.y, p3.x+dxLROI+dwLROI, p3.y-dhLROI       , degBtm);
// 
//         TScratchROI[0] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI       , p1.y+dyTROI       , degTop);
//         TScratchROI[1] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI+dwTROI, p1.y+dyTROI       , degTop);
//         TScratchROI[2] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI       , p1.y+dyTROI+dhTROI, degTop);
//         TScratchROI[3] = Math.OnRotate(p1.x, p1.y, p1.x+dxTROI+dwTROI, p1.y+dyTROI+dhTROI, degTop);
// 
//         RScratchROI[0] = Math.OnRotate(p1.x, p1.y, p1.x+dxRROI       , p1.y+dyRROI       , degTop);
//         RScratchROI[1] = Math.OnRotate(p1.x, p1.y, p1.x+dxRROI+dwRROI, p1.y+dyRROI       , degTop);
//         RScratchROI[2] = Math.OnRotate(p3.x, p3.y, p3.x+dxRROI       , p3.y-dhRROI       , degBtm);
//         RScratchROI[3] = Math.OnRotate(p3.x, p3.y, p3.x+dxRROI+dwRROI, p3.y-dhRROI       , degBtm);
// 
//         BScratchROI[0] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI       , p3.y-dyBROI       , degBtm);
//         BScratchROI[1] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI+dwBROI, p3.y-dyBROI       , degBtm);
//         BScratchROI[2] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI       , p3.y-dyBROI+dhBROI, degBtm);
//         BScratchROI[3] = Math.OnRotate(p3.x, p3.y, p3.x+dxBROI+dwBROI, p3.y-dyBROI+dhBROI, degBtm);
//     }
}
/*
bool CTemplateMatch::SetPosition(int SelLight, int x, int y)
{
    if(!EnableBumpROI && !EnableDustROI && !EnableScratchROI) return false;
    int PosX = x; int PosY = y;

    if(EnableBumpROI){
        if(Vision.ItemEnabled[SelLight][3]){
            if((PosX > LBumpROI[0].x) && (PosX < LBumpROI[1].x) && (PosY> LBumpROI[0].y) && (PosY< LBumpROI[2].y)) return true;
            if((PosX > LBumpROI[2].x) && (PosX < LBumpROI[3].x) && (PosY> LBumpROI[0].y) && (PosY< LBumpROI[2].y)) return true;
            if(LBumpROI[2].x > LBumpROI[1].x){
                if((PosX > LBumpROI[1].x) && (PosX < LBumpROI[2].x) && (PosY> LBumpROI[0].y) && (PosY< LBumpROI[2].y)) return true;
            }
            if(LBumpROI[0].x > LBumpROI[3].x){
                if((PosX > LBumpROI[3].x) && (PosX < LBumpROI[0].x) && (PosY> LBumpROI[0].y) && (PosY< LBumpROI[2].y)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][4]){
            if((PosX > TBumpROI[0].x) && (PosX < TBumpROI[1].x) && (PosY> TBumpROI[0].y) && (PosY< TBumpROI[2].y)) return true;
            if((PosX > TBumpROI[2].x) && (PosX < TBumpROI[3].x) && (PosY> TBumpROI[0].y) && (PosY< TBumpROI[2].y)) return true;
        }
        if(Vision.ItemEnabled[SelLight][5]){
            if((PosX > RBumpROI[0].x) && (PosX < RBumpROI[1].x) && (PosY> RBumpROI[0].y) && (PosY< RBumpROI[2].y)) return true;
            if((PosX > RBumpROI[2].x) && (PosX < RBumpROI[3].x) && (PosY> RBumpROI[0].y) && (PosY< RBumpROI[2].y)) return true;
            if(RBumpROI[2].x > RBumpROI[1].x){
                if((PosX > RBumpROI[1].x) && (PosX < RBumpROI[2].x) && (PosY> RBumpROI[0].y) && (PosY< RBumpROI[2].y)) return true;
            }
            if(RBumpROI[0].x > RBumpROI[3].x){
                if((PosX > RBumpROI[3].x) && (PosX < RBumpROI[0].x) && (PosY> RBumpROI[0].y) && (PosY< RBumpROI[2].y)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][6]){
            if((PosX > BBumpROI[0].x) && (PosX < BBumpROI[1].x) && (PosY> BBumpROI[0].y) && (PosY< BBumpROI[2].y)) return true;
            if((PosX > BBumpROI[2].x) && (PosX < BBumpROI[3].x) && (PosY> BBumpROI[0].y) && (PosY< BBumpROI[2].y)) return true;
        }
    }
    if(EnableDustROI){
        if(Vision.ItemEnabled[SelLight][7]){
            if((PosX > LDustROI[0].x) && (PosX < LDustROI[1].x) && (PosY> LDustROI[0].y) && (PosY< LDustROI[2].y)) return true;
            if((PosX > LDustROI[2].x) && (PosX < LDustROI[3].x) && (PosY> LDustROI[0].y) && (PosY< LDustROI[2].y)) return true;
            if(LDustROI[2].x > LDustROI[1].x){
                if((PosX > LDustROI[1].x) && (PosX < LDustROI[2].x) && (PosY> LDustROI[0].y) && (PosY< LDustROI[2].y)) return true;
            }
            if(LDustROI[0].x > LDustROI[3].x){
                if((PosX > LDustROI[3].x) && (PosX < LDustROI[0].x) && (PosY> LDustROI[0].y) && (PosY< LDustROI[2].y)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][8]){
            if((PosX > TDustROI[0].x) && (PosX < TDustROI[1].x) && (PosY> TDustROI[0].y) && (PosY< TDustROI[2].y)) return true;
            if((PosX > TDustROI[2].x) && (PosX < TDustROI[3].x) && (PosY> TDustROI[0].y) && (PosY< TDustROI[2].y)) return true;
        }
        if(Vision.ItemEnabled[SelLight][9]){
            if((PosX > RDustROI[0].x) && (PosX < RDustROI[1].x) && (PosY> RDustROI[0].y) && (PosY< RDustROI[2].y)) return true;
            if((PosX > RDustROI[2].x) && (PosX < RDustROI[3].x) && (PosY> RDustROI[0].y) && (PosY< RDustROI[2].y)) return true;
            if(RDustROI[2].x > RDustROI[1].x){
                if((PosX > RDustROI[1].x) && (PosX < RDustROI[2].x) && (PosY> RDustROI[0].y) && (PosY< RDustROI[2].y)) return true;
            }
            if(RDustROI[0].x > RDustROI[3].x){
                if((PosX > RDustROI[3].x) && (PosX < RDustROI[0].x) && (PosY> RDustROI[0].y) && (PosY< RDustROI[2].y)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][10]){
            if((PosX > BDustROI[0].x) && (PosX < BDustROI[1].x) && (PosY> BDustROI[0].y) && (PosY< BDustROI[2].y)) return true;
            if((PosX > BDustROI[2].x) && (PosX < BDustROI[3].x) && (PosY> BDustROI[0].y) && (PosY< BDustROI[2].y)) return true;
        }
    }
    if(EnableScratchROI){
        if(Vision.ItemEnabled[SelLight][11]){
            if((PosX > LScratchROI[0].x) && (PosX < LScratchROI[1].x) && (PosY> LScratchROI[0].y) && (PosY< LScratchROI[2].y)) return true;
            if((PosX > LScratchROI[2].x) && (PosX < LScratchROI[3].x) && (PosY> LScratchROI[0].y) && (PosY< LScratchROI[2].y)) return true;
            if(LScratchROI[2].x > LScratchROI[1].x){
                if((PosX > LScratchROI[1].x) && (PosX < LScratchROI[2].x) && (PosY> LScratchROI[0].y) && (PosY< LScratchROI[2].y)) return true;
            }
            if(LScratchROI[0].x > LScratchROI[3].x){
                if((PosX > LScratchROI[3].x) && (PosX < LScratchROI[0].x) && (PosY> LScratchROI[0].y) && (PosY< LScratchROI[2].y)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][12]){
            if((PosX > TScratchROI[0].x) && (PosX < TScratchROI[1].x) && (PosY> TScratchROI[0].y) && (PosY< TScratchROI[2].y)) return true;
            if((PosX > TScratchROI[2].x) && (PosX < TScratchROI[3].x) && (PosY> TScratchROI[0].y) && (PosY< TScratchROI[2].y)) return true;
        }
        if(Vision.ItemEnabled[SelLight][13]){
            if((PosX > RScratchROI[0].x) && (PosX < RScratchROI[1].x) && (PosY> RScratchROI[0].y) && (PosY< RScratchROI[2].y)) return true;
            if((PosX > RScratchROI[2].x) && (PosX < RScratchROI[3].x) && (PosY> RScratchROI[0].y) && (PosY< RScratchROI[2].y)) return true;
            if(RScratchROI[2].x > RScratchROI[1].x){
                if((PosX > RScratchROI[1].x) && (PosX < RScratchROI[2].x) && (PosY> RScratchROI[0].y) && (PosY< RScratchROI[2].y)) return true;
            }
            if(RScratchROI[0].x > RScratchROI[3].x){
                if((PosX > RScratchROI[3].x) && (PosX < RScratchROI[0].x) && (PosY> RScratchROI[0].y) && (PosY< RScratchROI[2].y)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][14]){
            if((PosX > BScratchROI[0].x) && (PosX < BScratchROI[1].x) && (PosY> BScratchROI[0].y) && (PosY< BScratchROI[2].y)) return true;
            if((PosX > BScratchROI[2].x) && (PosX < BScratchROI[3].x) && (PosY> BScratchROI[0].y) && (PosY< BScratchROI[2].y)) return true;
        }
    }
    return false;
}
*/
bool CTemplateMatch::SetPosition(int SelLight, int x, int y)
{
//     if(!EnableBumpROI && !EnableDustROI && !EnableScratchROI) return false;
//     int PosX = x; int PosY = y;
// 
//     if(EnableBumpROI){
//         if(Vision.Bump.BmPa.dxLROI!=0 && Vision.Bump.BmPa.dyLROI!=0 && Vision.Bump.BmPa.dwLROI!=0 && Vision.Bump.BmPa.dhLROI!=0 ){
//             if((PosX > LBumpROI[0].x) && (PosX < LBumpROI[1].x) && (PosY> LBumpROI[0].y) && (PosY< LBumpROI[2].y)) return true;
//             if((PosX > LBumpROI[2].x) && (PosX < LBumpROI[3].x) && (PosY> LBumpROI[0].y) && (PosY< LBumpROI[2].y)) return true;
//             if(LBumpROI[2].x > LBumpROI[1].x){
//                 if((PosX > LBumpROI[1].x) && (PosX < LBumpROI[2].x) && (PosY> LBumpROI[0].y) && (PosY< LBumpROI[2].y)) return true;
//             }
//             if(LBumpROI[0].x > LBumpROI[3].x){
//                 if((PosX > LBumpROI[3].x) && (PosX < LBumpROI[0].x) && (PosY> LBumpROI[0].y) && (PosY< LBumpROI[2].y)) return true;
//             }
//         }
//         if(Vision.Bump.BmPa.dxTROI!=0 && Vision.Bump.BmPa.dyTROI!=0 && Vision.Bump.BmPa.dwTROI!=0 && Vision.Bump.BmPa.dhTROI!=0 ){
//             if((PosX > TBumpROI[0].x) && (PosX < TBumpROI[1].x) && (PosY> TBumpROI[0].y) && (PosY< TBumpROI[2].y)) return true;
//             if((PosX > TBumpROI[2].x) && (PosX < TBumpROI[3].x) && (PosY> TBumpROI[0].y) && (PosY< TBumpROI[2].y)) return true;
//         }
//         if(Vision.Bump.BmPa.dxRROI!=0 && Vision.Bump.BmPa.dyRROI!=0 && Vision.Bump.BmPa.dwRROI!=0 && Vision.Bump.BmPa.dhRROI!=0 ){
//             if((PosX > RBumpROI[0].x) && (PosX < RBumpROI[1].x) && (PosY> RBumpROI[0].y) && (PosY< RBumpROI[2].y)) return true;
//             if((PosX > RBumpROI[2].x) && (PosX < RBumpROI[3].x) && (PosY> RBumpROI[0].y) && (PosY< RBumpROI[2].y)) return true;
//             if(RBumpROI[2].x > RBumpROI[1].x){
//                 if((PosX > RBumpROI[1].x) && (PosX < RBumpROI[2].x) && (PosY> RBumpROI[0].y) && (PosY< RBumpROI[2].y)) return true;
//             }
//             if(RBumpROI[0].x > RBumpROI[3].x){
//                 if((PosX > RBumpROI[3].x) && (PosX < RBumpROI[0].x) && (PosY> RBumpROI[0].y) && (PosY< RBumpROI[2].y)) return true;
//             }
//         }
//         if(Vision.Bump.BmPa.dxBROI!=0 && Vision.Bump.BmPa.dyBROI!=0 && Vision.Bump.BmPa.dwBROI!=0 && Vision.Bump.BmPa.dhBROI!=0 ){
//             if((PosX > BBumpROI[0].x) && (PosX < BBumpROI[1].x) && (PosY> BBumpROI[0].y) && (PosY< BBumpROI[2].y)) return true;
//             if((PosX > BBumpROI[2].x) && (PosX < BBumpROI[3].x) && (PosY> BBumpROI[0].y) && (PosY< BBumpROI[2].y)) return true;
//         }
//     }
//     if(EnableDustROI){
//         if(Vision.ItemEnabled[SelLight][7]){
//             if((PosX > LDustROI[0].x) && (PosX < LDustROI[1].x) && (PosY> LDustROI[0].y) && (PosY< LDustROI[2].y)) return true;
//             if((PosX > LDustROI[2].x) && (PosX < LDustROI[3].x) && (PosY> LDustROI[0].y) && (PosY< LDustROI[2].y)) return true;
//             if(LDustROI[2].x > LDustROI[1].x){
//                 if((PosX > LDustROI[1].x) && (PosX < LDustROI[2].x) && (PosY> LDustROI[0].y) && (PosY< LDustROI[2].y)) return true;
//             }
//             if(LDustROI[0].x > LDustROI[3].x){
//                 if((PosX > LDustROI[3].x) && (PosX < LDustROI[0].x) && (PosY> LDustROI[0].y) && (PosY< LDustROI[2].y)) return true;
//             }
//         }
//         if(Vision.ItemEnabled[SelLight][8]){
//             if((PosX > TDustROI[0].x) && (PosX < TDustROI[1].x) && (PosY> TDustROI[0].y) && (PosY< TDustROI[2].y)) return true;
//             if((PosX > TDustROI[2].x) && (PosX < TDustROI[3].x) && (PosY> TDustROI[0].y) && (PosY< TDustROI[2].y)) return true;
//         }
//         if(Vision.ItemEnabled[SelLight][9]){
//             if((PosX > RDustROI[0].x) && (PosX < RDustROI[1].x) && (PosY> RDustROI[0].y) && (PosY< RDustROI[2].y)) return true;
//             if((PosX > RDustROI[2].x) && (PosX < RDustROI[3].x) && (PosY> RDustROI[0].y) && (PosY< RDustROI[2].y)) return true;
//             if(RDustROI[2].x > RDustROI[1].x){
//                 if((PosX > RDustROI[1].x) && (PosX < RDustROI[2].x) && (PosY> RDustROI[0].y) && (PosY< RDustROI[2].y)) return true;
//             }
//             if(RDustROI[0].x > RDustROI[3].x){
//                 if((PosX > RDustROI[3].x) && (PosX < RDustROI[0].x) && (PosY> RDustROI[0].y) && (PosY< RDustROI[2].y)) return true;
//             }
//         }
//         if(Vision.ItemEnabled[SelLight][10]){
//             if((PosX > BDustROI[0].x) && (PosX < BDustROI[1].x) && (PosY> BDustROI[0].y) && (PosY< BDustROI[2].y)) return true;
//             if((PosX > BDustROI[2].x) && (PosX < BDustROI[3].x) && (PosY> BDustROI[0].y) && (PosY< BDustROI[2].y)) return true;
//         }
//     }
//     if(EnableScratchROI){
//         if(Vision.ItemEnabled[SelLight][11]){
//             if((PosX > LScratchROI[0].x) && (PosX < LScratchROI[1].x) && (PosY> LScratchROI[0].y) && (PosY< LScratchROI[2].y)) return true;
//             if((PosX > LScratchROI[2].x) && (PosX < LScratchROI[3].x) && (PosY> LScratchROI[0].y) && (PosY< LScratchROI[2].y)) return true;
//             if(LScratchROI[2].x > LScratchROI[1].x){
//                 if((PosX > LScratchROI[1].x) && (PosX < LScratchROI[2].x) && (PosY> LScratchROI[0].y) && (PosY< LScratchROI[2].y)) return true;
//             }
//             if(LScratchROI[0].x > LScratchROI[3].x){
//                 if((PosX > LScratchROI[3].x) && (PosX < LScratchROI[0].x) && (PosY> LScratchROI[0].y) && (PosY< LScratchROI[2].y)) return true;
//             }
//         }
//         if(Vision.ItemEnabled[SelLight][12]){
//             if((PosX > TScratchROI[0].x) && (PosX < TScratchROI[1].x) && (PosY> TScratchROI[0].y) && (PosY< TScratchROI[2].y)) return true;
//             if((PosX > TScratchROI[2].x) && (PosX < TScratchROI[3].x) && (PosY> TScratchROI[0].y) && (PosY< TScratchROI[2].y)) return true;
//         }
//         if(Vision.ItemEnabled[SelLight][13]){
//             if((PosX > RScratchROI[0].x) && (PosX < RScratchROI[1].x) && (PosY> RScratchROI[0].y) && (PosY< RScratchROI[2].y)) return true;
//             if((PosX > RScratchROI[2].x) && (PosX < RScratchROI[3].x) && (PosY> RScratchROI[0].y) && (PosY< RScratchROI[2].y)) return true;
//             if(RScratchROI[2].x > RScratchROI[1].x){
//                 if((PosX > RScratchROI[1].x) && (PosX < RScratchROI[2].x) && (PosY> RScratchROI[0].y) && (PosY< RScratchROI[2].y)) return true;
//             }
//             if(RScratchROI[0].x > RScratchROI[3].x){
//                 if((PosX > RScratchROI[3].x) && (PosX < RScratchROI[0].x) && (PosY> RScratchROI[0].y) && (PosY< RScratchROI[2].y)) return true;
//             }
//         }
//         if(Vision.ItemEnabled[SelLight][14]){
//             if((PosX > BScratchROI[0].x) && (PosX < BScratchROI[1].x) && (PosY> BScratchROI[0].y) && (PosY< BScratchROI[2].y)) return true;
//             if((PosX > BScratchROI[2].x) && (PosX < BScratchROI[3].x) && (PosY> BScratchROI[0].y) && (PosY< BScratchROI[2].y)) return true;
//         }
//     }
    return false;
}
/*
bool CTemplateMatch::SetPosition(int SelLight, int x, int y)
{
    if(!EnableBumpROI && !EnableDustROI && !EnableScratchROI) return false;

    int PosX = x; int PosY = y;
    int LxLength, LyLength, RxLength , RyLength ;
    int LdifferY, RdifferY, LPosition, RPosition;

    if(EnableBumpROI){
        if(Vision.ItemEnabled[SelLight][3]){
            LxLength = abs(LBumpROI[0].x - LBumpROI[2].x); LyLength = abs(LBumpROI[0].y - LBumpROI[2].y);
            RxLength = abs(LBumpROI[1].x - LBumpROI[3].x); RyLength = abs(LBumpROI[1].y - LBumpROI[3].y);
            if(PosY > LBumpROI[0].y && PosY < LBumpROI[2].y){
                LdifferY = PosY - LBumpROI[0].y;
                RdifferY = PosY - LBumpROI[1].y;
                LPosition  = (LBumpROI[0].x - LBumpROI[2].x) > 0 ? LBumpROI[2].x + LxLength - (LxLength * LdifferY)/LyLength : LBumpROI[0].x + (LxLength * LdifferY)/LyLength; //x1 : x2 = y1 : y2;
                RPosition  = (LBumpROI[1].x - LBumpROI[3].x) > 0 ? LBumpROI[3].x + RxLength - (RxLength * RdifferY)/RyLength : LBumpROI[1].x + (RxLength * RdifferY)/RyLength; //x1 : x2 = y1 : y2;

                if((PosX > LPosition) && (PosX < RPosition)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][4]){
            if((PosX > TBumpROI[0].x) && (PosX < TBumpROI[1].x) && (PosY> TBumpROI[0].y) && (PosY< TBumpROI[2].y)) return true;
            if((PosX > TBumpROI[2].x) && (PosX < TBumpROI[3].x) && (PosY> TBumpROI[0].y) && (PosY< TBumpROI[2].y)) return true;
        }
        if(Vision.ItemEnabled[SelLight][5]){
            LxLength = abs(RBumpROI[0].x - RBumpROI[2].x); LyLength = abs(RBumpROI[0].y - RBumpROI[2].y);
            RxLength = abs(RBumpROI[1].x - RBumpROI[3].x); RyLength = abs(RBumpROI[1].y - RBumpROI[3].y);
            if(PosY > RBumpROI[0].y && PosY < RBumpROI[2].y){
                LdifferY = PosY - RBumpROI[0].y;
                RdifferY = PosY - RBumpROI[1].y;
                LPosition  = (RBumpROI[0].x - RBumpROI[2].x) > 0 ? RBumpROI[2].x + LxLength - (LxLength * LdifferY)/LyLength : RBumpROI[0].x + (LxLength * LdifferY)/LyLength; //x1 : x2 = y1 : y2;
                RPosition  = (RBumpROI[1].x - RBumpROI[3].x) > 0 ? RBumpROI[3].x + RxLength - (RxLength * RdifferY)/RyLength : RBumpROI[1].x + (RxLength * RdifferY)/RyLength; //x1 : x2 = y1 : y2;

                if((PosX > LPosition) && (PosX < RPosition)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][6]){
            if((PosX > BBumpROI[0].x) && (PosX < BBumpROI[1].x) && (PosY> BBumpROI[0].y) && (PosY< BBumpROI[2].y)) return true;
            if((PosX > BBumpROI[2].x) && (PosX < BBumpROI[3].x)
             && (PosY> BBumpROI[0].y) && (PosY< BBumpROI[2].y)) return true;
        }
    }
    if(EnableDustROI){
        if(Vision.ItemEnabled[SelLight][7]){
            LxLength = abs(LDustROI[0].x - LDustROI[2].x); LyLength = abs(LDustROI[0].y - LDustROI[2].y);
            RxLength = abs(LDustROI[1].x - LDustROI[3].x); RyLength = abs(LDustROI[1].y - LDustROI[3].y);
            if(PosY > LDustROI[0].y && PosY < LDustROI[2].y){
                LdifferY = PosY - LDustROI[0].y;
                RdifferY = PosY - LDustROI[1].y;
                LPosition  = (LDustROI[0].x - LDustROI[2].x) > 0 ? LDustROI[2].x + LxLength - (LxLength * LdifferY)/LyLength : LDustROI[0].x + (LxLength * LdifferY)/LyLength; //x1 : x2 = y1 : y2;
                RPosition  = (LDustROI[1].x - LDustROI[3].x) > 0 ? LDustROI[3].x + RxLength - (RxLength * RdifferY)/RyLength : LDustROI[1].x + (RxLength * RdifferY)/RyLength; //x1 : x2 = y1 : y2;

                if((PosX > LPosition) && (PosX < RPosition)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][8]){
            if((PosX > TDustROI[0].x) && (PosX < TDustROI[1].x) && (PosY> TDustROI[0].y) && (PosY< TDustROI[2].y)) return true;
            if((PosX > TDustROI[2].x) && (PosX < TDustROI[3].x) && (PosY> TDustROI[0].y) && (PosY< TDustROI[2].y)) return true;
        }
        if(Vision.ItemEnabled[SelLight][9]){
            LxLength = abs(RDustROI[0].x - RDustROI[2].x); LyLength = abs(RDustROI[0].y - RDustROI[2].y);
            RxLength = abs(RDustROI[1].x - RDustROI[3].x); RyLength = abs(RDustROI[1].y - RDustROI[3].y);
            if(PosY > RDustROI[0].y && PosY < RDustROI[2].y){
                LdifferY = PosY - RDustROI[0].y;
                RdifferY = PosY - RDustROI[1].y;
                LPosition  = (RDustROI[0].x - RDustROI[2].x) > 0 ? RDustROI[2].x + LxLength - (LxLength * LdifferY)/LyLength : RDustROI[0].x + (LxLength * LdifferY)/LyLength; //x1 : x2 = y1 : y2;
                RPosition  = (RDustROI[1].x - RDustROI[3].x) > 0 ? RDustROI[3].x + RxLength - (RxLength * RdifferY)/RyLength : RDustROI[1].x + (RxLength * RdifferY)/RyLength; //x1 : x2 = y1 : y2;

                if((PosX > LPosition) && (PosX < RPosition)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][10]){
            if((PosX > BDustROI[0].x) && (PosX < BDustROI[1].x) && (PosY> BDustROI[0].y) && (PosY< BDustROI[2].y)) return true;
            if((PosX > BDustROI[2].x) && (PosX < BDustROI[3].x) && (PosY> BDustROI[0].y) && (PosY< BDustROI[2].y)) return true;
        }
    }
    if(EnableScratchROI){
        if(Vision.ItemEnabled[SelLight][11]){
            LxLength = abs(LScratchROI[0].x - LScratchROI[2].x); LyLength = abs(LScratchROI[0].y - LScratchROI[2].y);
            RxLength = abs(LScratchROI[1].x - LScratchROI[3].x); RyLength = abs(LScratchROI[1].y - LScratchROI[3].y);
            if(PosY > LScratchROI[0].y && PosY < LScratchROI[2].y){
                LdifferY = PosY - LScratchROI[0].y;
                RdifferY = PosY - LScratchROI[1].y;
                LPosition  = (LScratchROI[0].x - LScratchROI[2].x) > 0 ? LScratchROI[2].x + LxLength - (LxLength * LdifferY)/LyLength : LScratchROI[0].x + (LxLength * LdifferY)/LyLength; //x1 : x2 = y1 : y2;
                RPosition  = (LScratchROI[1].x - LScratchROI[3].x) > 0 ? LScratchROI[3].x + RxLength - (RxLength * RdifferY)/RyLength : LScratchROI[1].x + (RxLength * RdifferY)/RyLength; //x1 : x2 = y1 : y2;

                if((PosX > LPosition) && (PosX < RPosition)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][12]){
            if((PosX > TScratchROI[0].x) && (PosX < TScratchROI[1].x) && (PosY> TScratchROI[0].y) && (PosY< TScratchROI[2].y)) return true;
            if((PosX > TScratchROI[2].x) && (PosX < TScratchROI[3].x) && (PosY> TScratchROI[0].y) && (PosY< TScratchROI[2].y)) return true;
        }
        if(Vision.ItemEnabled[SelLight][13]){
            LxLength = abs(RScratchROI[0].x - RScratchROI[2].x); LyLength = abs(RScratchROI[0].y - RScratchROI[2].y);
            RxLength = abs(RScratchROI[1].x - RScratchROI[3].x); RyLength = abs(RScratchROI[1].y - RScratchROI[3].y);
            if(PosY > RScratchROI[0].y && PosY < RScratchROI[2].y){
                LdifferY = PosY - RScratchROI[0].y;
                RdifferY = PosY - RScratchROI[1].y;
                LPosition  = (RScratchROI[0].x - RScratchROI[2].x) > 0 ? RScratchROI[2].x + LxLength - (LxLength * LdifferY)/LyLength : RScratchROI[0].x + (LxLength * LdifferY)/LyLength; //x1 : x2 = y1 : y2;
                RPosition  = (RScratchROI[1].x - RScratchROI[3].x) > 0 ? RScratchROI[3].x + RxLength - (RxLength * RdifferY)/RyLength : RScratchROI[1].x + (RxLength * RdifferY)/RyLength; //x1 : x2 = y1 : y2;

                if((PosX > LPosition) && (PosX < RPosition)) return true;
            }
        }
        if(Vision.ItemEnabled[SelLight][14]){
            if((PosX > BScratchROI[0].x) && (PosX < BScratchROI[1].x) && (PosY> BScratchROI[0].y) && (PosY< BScratchROI[2].y)) return true;
            if((PosX > BScratchROI[2].x) && (PosX < BScratchROI[3].x) && (PosY> BScratchROI[0].y) && (PosY< BScratchROI[2].y)) return true;
        }
    }
    return false;
}
*/
TFPoint *CTemplateMatch::lpErCoordinate()
{
    TFPoint Pt[NUMBEROF_MAX_GROUP];
    register int Cnt = 0;

    for (register int k=0; k<DefectCount; k++) {
        Pt[Cnt++]  = Defect[k].Rect.Point();
    }
    for (register int k=Cnt; k<NUMBEROF_MAX_DEFECT; k++) Pt[k].x = Pt[k].y = 0;
    return &Pt[0];
}
void CTemplateMatch::OnLoadTrain(bool IsLoad, CString Path)
{
     FILE *fp;

	 if (IsLoad) fp = fopen(CW2A(Path),"rb");
	 else        fp = fopen(CW2A(Path),"wb");

     if (fp == NULL) return;

     if (IsLoad) fread (&Train, 1, sizeof(Train), fp);
     else        fwrite(&Train, 1, sizeof(Train), fp);

     fclose(fp);
}
void CTemplateMatch::OnPaintDefectRect(TDrawTool Tool, COLORREF FColor)
{
    if (Tool.Handle==NULL) return;
    HGdi*Gdi = new HGdi(Tool.Handle);
    Gdi->CPen.Color = Tool.Color;

    if(Defect==NULL) return;
    for (register int k=0; k<DefectCount; k++) {
        JRect Rect = Defect[k].Rect;
		CString Msg="";

        if      (Rect.r-Rect.l<5) Gdi->OnRect(0, Rect.l-2, Rect.t-2, Rect.r+2, Rect.b+2, 0);
        else if (Rect.b-Rect.t<5) Gdi->OnRect(0, Rect.l-2, Rect.t-2, Rect.r+2, Rect.b+2, 0);
        else                      Gdi->OnRect(0, Rect.l  , Rect.t  , Rect.r+1, Rect.b+1, 0);

		Msg.Format(L"%d",k) ;
        Gdi->OnText(Rect.l, Rect.t-10, CW2A(Msg));
        Gdi->CPen.Color  = BLUE_COLOR;
        Gdi->CText.Color = BLUE_COLOR;
        if(TpPa.Micron){
			Msg.Format(L"4.3f",(Defect[k].Rect.xLength()*2.8));
            CString TextW = "Width  : " + Msg + " um ";

			Msg.Format(L"4.3f",(Defect[k].Rect.yLength()*2.8));
            CString TextH = "Height : " + Msg + " um ";

            Gdi->OnText(Rect.r, Rect.t+10, CW2A(TextW));
            Gdi->OnText(Rect.r, Rect.t+20, CW2A(TextW));
        }
        else{
			Msg.Format(L"4.3f",(Defect[k].Rect.xLength()*2.8));
			CString TextW = "Width  : " + Msg + " Pixel ";

			Msg.Format(L"4.3f",(Defect[k].Rect.yLength()*2.8));
			CString TextH = "Height : " + Msg + " Pixel ";

			Gdi->OnText(Rect.r, Rect.t + 10, CW2A(TextW));
			Gdi->OnText(Rect.r, Rect.t + 20, CW2A(TextW));
        }
    }

    //AnsiString DataSize="";
    //AnsiString DataName="";
    //int Memorysize=0;

    //Memorysize = (sizeof(TPoint )*NUMBEROF_POINT_PIXEL);
    //DataSize = IntToStr(Memorysize);
    //DataName = "Point여분 메모리:";
    //LogUnit.Trace(DataName+DataSize,0);

    //Memorysize = (sizeof(TGroup )*NUMBEROF_MAX_GROUP  );
    //DataSize = IntToStr(Memorysize);
    //DataName = "Group여분 메모리:";
    //LogUnit.Trace(DataName+DataSize,0);

    //Memorysize = (sizeof(TDefect)*NUMBEROF_MAX_DEFECT );
    //DataSize = IntToStr(Memorysize);
    //DataName = "Defect여분 메모리:";
    //LogUnit.Trace(DataName+DataSize,0);

    delete Gdi;
}
void CTemplateMatch::MemoryClear()          //khj
{
    //delete[] Point ; Point =NULL;
    free(Point); Point =NULL;
    if(Point!=NULL)  AfxMessageBox(L"Point 초기화되지 않았습니다!" );

    //delete[] Group ; Group =NULL;
    free(Group); Group =NULL;
    if(Group!=NULL)  AfxMessageBox(L"Group 초기화되지 않았습니다!" );

    //delete[] Defect; Defect=NULL;
    free(Defect); Defect=NULL;
    if(Defect!=NULL) AfxMessageBox(L"Defect 초기화되지 않았습니다!");
}
void CTemplateMatch::OnPaintDefectPoint(TDrawTool Tool, COLORREF FColor)
{
	if (Tool.Handle==NULL) return;
	HGdi*Gdi = new HGdi(Tool.Handle);
	Gdi->CPen.Color = Tool.Color;

    if(Point==NULL || Group==NULL) return;
    for (register int i=0; i<GroupCount         ; i++) {
    for (register int j=0; j<Group[i].PointCount; j++) {
        Gdi->OnPixel(Group[i].Point[j], FColor);
    }}

    delete Gdi;
}
void CTemplateMatch::OnLoad(bool IsLoad, CString Path)
{
    CIFile Ini;

    Ini.Load(IsLoad, Path, "TpPa", "Micron          ", &TpPa.Micron          );
    Ini.Load(IsLoad, Path, "TpPa", "And             ", &TpPa.And             );
    Ini.Load(IsLoad, Path, "TpPa", "MaxPxCnt        ", &TpPa.MaxPxCnt        );
    Ini.Load(IsLoad, Path, "TpPa", "MinPxCnt        ", &TpPa.MinPxCnt        );
    Ini.Load(IsLoad, Path, "TpPa", "MinBumpPxCnt    ", &TpPa.MinBumpPxCnt    );
    //Ini.Load(IsLoad, Path, "TpPa", "MaxDefectCnt    ", &TpPa.MaxDefectCnt    );
    //Ini.Load(IsLoad, Path, "TpPa", "MaxGroupCnt     ", &TpPa.MaxGroupCnt    );
    Ini.Load(IsLoad, Path, "TpPa", "LineGap         ", &TpPa.LineGap         );
    Ini.Load(IsLoad, Path, "TpPa", "DIS_POINT       ", &TpPa.DIS_POINT       );
    Ini.Load(IsLoad, Path, "TpPa", "Offset          ", &TpPa.Offset          );
    Ini.Load(IsLoad, Path, "TpPa", "Samples         ", &TpPa.Samples         );
    Ini.Load(IsLoad, Path, "TpPa", "LineStep        ", &TpPa.LineStep        );
    Ini.Load(IsLoad, Path, "TpPa", "CompareStep     ", &TpPa.CompareStep     );
    Ini.Load(IsLoad, Path, "TpPa", "CompareBumpStep ", &TpPa.CompareBumpStep );
    Ini.Load(IsLoad, Path, "TpPa", "HiThMargin      ", &TpPa.HiThMargin      );
    Ini.Load(IsLoad, Path, "TpPa", "LoThMargin      ", &TpPa.LoThMargin      );
    Ini.Load(IsLoad, Path, "TpPa", "HiThBumpMargin  ", &TpPa.HiThBumpMargin  );
    Ini.Load(IsLoad, Path, "TpPa", "LoThBumpMargin  ", &TpPa.LoThBumpMargin  );
    Ini.Load(IsLoad, Path, "TpPa", "MinHorLength    ", &TpPa.MinHorLength    );
    Ini.Load(IsLoad, Path, "TpPa", "MaxHorLength    ", &TpPa.MaxHorLength    );
    Ini.Load(IsLoad, Path, "TpPa", "MinVerLength    ", &TpPa.MinVerLength    );
    Ini.Load(IsLoad, Path, "TpPa", "MaxVerLength    ", &TpPa.MaxVerLength    );
    Ini.Load(IsLoad, Path, "TpPa", "MinBumpHorLength", &TpPa.MinBumpHorLength);
    Ini.Load(IsLoad, Path, "TpPa", "MinBumpVerLength", &TpPa.MinBumpVerLength);
    Ini.Load(IsLoad, Path, "TpPa", "MinDustHorLength", &TpPa.MinDustHorLength);
    Ini.Load(IsLoad, Path, "TpPa", "MinDustVerLength", &TpPa.MinDustVerLength);
    Ini.Load(IsLoad, Path, "TpPa", "xMgPxCnt        ", &TpPa.xMgPxCnt        );
    Ini.Load(IsLoad, Path, "TpPa", "yMgPxCnt        ", &TpPa.yMgPxCnt        );
    Ini.Load(IsLoad, Path, "TpPa", "MulTiTeach      ", &TpPa.MulTiTeach      );
    Ini.Load(IsLoad, Path, "TpPa", "StandardChip    ", &TpPa.StandardChip    );
    Ini.Load(IsLoad, Path, "TpPa", "MultiCompare    ", &TpPa.MultiCompare    );
    Ini.Load(IsLoad, Path, "TpPa", "InkDustPixel    ", &TpPa.InkDustPixel    );
}




