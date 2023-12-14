#include "stdafx.h"
#include "LxBga.h"
#include "LxHeader.h"


void CGrid::OnClear2D()
{
	AvgCuPx      = AvgDvPx      = 0;
	AvgCuPy      = AvgDvPy      = 0;
	AvgCuPitchx  = AvgDvPitchx  = 0;
	AvgCuPitchy  = AvgDvPitchy  = 0;
	AvgCuDiameter= AvgDvDiameter= 0;
	AvgCuShape   = AvgDvShape   = 0;

	MinCuPx      = MinDvPx      = 0;
	MinCuPy      = MinDvPy      = 0;
	MinCuPitchx  = MinDvPitchx  = 0;
	MinCuPitchy  = MinDvPitchy  = 0;
	MinCuDiameter= MinDvDiameter= 0;
	MinCuShape   = MinDvShape   = 0;

	MaxCuPx      = MaxDvPx      = 0;
	MaxCuPy      = MaxDvPy      = 0;
	MaxCuPitchx  = MaxDvPitchx  = 0;
	MaxCuPitchy  = MaxDvPitchy  = 0;
	MaxCuDiameter= MaxDvDiameter= 0;
	MaxCuShape   = MaxDvShape   = 0;

	for (int y=0; y<MAX_BGA_Y; y++) {
		for (int x=0; x<MAX_BGA_X; x++) {
			Ball[x][y].OnClear2D();
		}}
}

void CGrid::TBall::OnClear2D()
{
	IsCuExist = 0;
	IsPass    = 0;
	IsUnknown = 0;
	IsMissing = 0;

	CuPx      = DvPx      = 0;
	CuPy      = DvPy      = 0;
	CuPitchx  = DvPitchx  = 0;
	CuPitchy  = DvPitchy  = 0;
	CuDiameter= DvDiameter= 0;
	CuShape   = DvShape   = 0;

	CuPitch_n = DvPitch_n = 0;
	CuPitch_e = DvPitch_e = 0;
	CuPitch_s = DvPitch_s = 0;
	CuPitch_w = DvPitch_w = 0;
}

TPoint CGrid::IsInclude(float cp, float cy)
{
	TPoint RC(-1, -1);

	for (int y=0; y<RowCount; y++) {
		for (int x=0; x<ColCount; x++) {
			int Sx = IRoi.l + ColStart + ColPitch * x;
			int Sy = IRoi.t + RowStart + RowPitch * y;

			Ball[x][y].IRoi.l = Sx;
			Ball[x][y].IRoi.t = Sy;
			Ball[x][y].IRoi.r = Sx+ColPitch;
			Ball[x][y].IRoi.b = Sy+RowPitch;

			bool IsInclude = Ball[x][y].IRoi.IsInclude((int)cp, (int)cy);

			if (IsInclude) {
				RC.x = x;
				RC.y = y;
				return RC;
			}
		}}
	return RC;
}

TPoint CGrid::GetGridRowCol(float cp, float cy, float IgAngle)
{
	TPoint RC(-1, -1);

	for (int y=0; y<RowCount; y++) {
		for (int x=0; x<ColCount; x++) {
			if (!Ball[x][y].IsTrExist) continue;

			int Sx = IRoi.l + ColStart + ColPitch * x;
			int Sy = IRoi.t + RowStart + RowPitch * y;

			Ball[x][y].IRoi.l = Sx;
			Ball[x][y].IRoi.t = Sy;
			Ball[x][y].IRoi.r = Sx+ColPitch;
			Ball[x][y].IRoi.b = Sy+RowPitch;

			Ball[x][y].IRoi = GetConsiderAngle(Ball[x][y].IRoi, IgAngle);

			bool IsInclude = Ball[x][y].IRoi.IsInclude((int)cp, (int)cy);

			if (IsInclude) {
				RC.x = x;
				RC.y = y;
				return RC;
			}
		}}
	return RC;
}

TPoint CGrid::GetGridRowColWithoutTrExist(float cp, float cy, float IgAngle)
{
	TPoint RC(-1, -1);

	for (int y=0; y<RowCount; y++) {
		for (int x=0; x<ColCount; x++) {
			int Sx = IRoi.l + ColStart + ColPitch * x;
			int Sy = IRoi.t + RowStart + RowPitch * y;

			Ball[x][y].IRoi.l = Sx;
			Ball[x][y].IRoi.t = Sy;
			Ball[x][y].IRoi.r = Sx+ColPitch;
			Ball[x][y].IRoi.b = Sy+RowPitch;

			Ball[x][y].IRoi = GetConsiderAngle(Ball[x][y].IRoi, IgAngle);

			bool IsInclude = Ball[x][y].IRoi.IsInclude((int)cp, (int)cy);

			if (IsInclude) {
				RC.x = x;
				RC.y = y;
				return RC;
			}
		}}
	return RC;
}

TPoint CGrid::GetGridRowCol(int __Px, int __Py, float cp, float cy, float dxDv, float dyDv, float IgAngle)
{
	TPoint RC(-1, -1);

	float dx = dxDv;
	float dy = dyDv;

	int StaPx = __Px-2;
	int StaPy = __Py-2;
	int EndPx = __Px+2;
	int EndPy = __Py+2;

	if (StaPx<0) StaPx = 0;
	if (StaPy<0) StaPy = 0;

	if (EndPx>=MAX_BGA_X) EndPx = __Px;
	if (EndPy>=MAX_BGA_Y) EndPy = __Py;

	for (register int y=StaPy; y<EndPy; y++) {
		for (register int x=StaPx; x<EndPx; x++) {

			if (!Ball[x][y].IsTrExist) continue;

			int Sx = IRoi.l + ColStart + ColPitch * x;
			int Sy = IRoi.t + RowStart + RowPitch * y;

			Ball[x][y].IRoi.l = Sx+dx;
			Ball[x][y].IRoi.t = Sy+dy;
			Ball[x][y].IRoi.r = Sx+ColPitch +dx;
			Ball[x][y].IRoi.b = Sy+RowPitch +dy;

			Ball[x][y].IRoi = GetConsiderAngle(Ball[x][y].IRoi, IgAngle);

			bool IsInclude = Ball[x][y].IRoi.IsInclude((int)cp, (int)cy);

			if (IsInclude) {
				RC.x = x;
				RC.y = y;
				return RC;
			}
		}}
	return RC;
}

void CGrid::OnPaintBallIpRect(TDrawTool Tool)
{
	if (Tool.Handle==NULL) return;

	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx        = Tool.FVx;
	Gdi->FVy        = Tool.FVy;
	Gdi->FHs        = Tool.FHs;
	Gdi->FVs        = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

	for (int y=0; y<RowCount; y++) {
		for (int x=0; x<ColCount; x++) {
			Gdi->CPen  .Style = PS_SOLID;
			Gdi->CBrush.Style = BS_SOLID;
			Gdi->CBrush.Color = BLACK   ;
			Gdi->CPen.Color = CoIpArea;

			if (Ball[x][y].IsTrExist) {
				Ball[x][y].IRoi.OnPaint(Tool, BS_HOLLOW, HS_BDIAGONAL);
			}
			else {

				Gdi->CPen  .Style = PS_SOLID;
				Gdi->CBrush.Style = BS_HATCHED;
				Gdi->CBrush.Hatch = HS_BDIAGONAL;
				Gdi->CBrush.Color = BLACK;

				Gdi->CPen.Color = CoIpArea;
				Gdi->CBrush.Color = GRAYGREEN;
				JRect PaRt = Ball[x][y].IRoi;
				Gdi->OnRect(1, PaRt.l, PaRt.t, PaRt.r, PaRt.b, 5);
			}
		}}
	delete Gdi;
}


void CGrid::OnPaint(TDrawTool Tool, bool IsDwMatrix)
{
	if (Tool.Handle==NULL) return;

	if (!IsDwMatrix) {
		return OnPaintBallIpRect(Tool);
	}

	HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;

	Gdi->CPen.Color = AQUA;
	Gdi->CText.Color = YELLOW_COLOR;

	for (int y=0; y<RowCount; y++) {
		for (int x=0; x<ColCount; x++) {
			int Sx = IRoi.l + ColStart + ColPitch * x;
			int Sy = IRoi.t + RowStart + RowPitch * y;

			Ball[x][y].IRoi.l = Sx;
			Ball[x][y].IRoi.t = Sy;
			Ball[x][y].IRoi.r = Sx+ColPitch;
			Ball[x][y].IRoi.b = Sy+RowPitch;

			Gdi->CPen  .Style = PS_SOLID;
			Gdi->CBrush.Style = BS_SOLID;
			Gdi->CBrush.Color = BLACK   ;

			Gdi->CPen.Color = CoIpArea;
			if (Ball[x][y].IsTrExist) {
				Ball[x][y].IRoi.OnPaint(Tool, BS_HOLLOW, HS_BDIAGONAL);
			}
			else {

				Gdi->CPen  .Style = PS_SOLID;
				Gdi->CBrush.Style = BS_HATCHED;
				Gdi->CBrush.Hatch = HS_BDIAGONAL;
				Gdi->CBrush.Color = BLACK;

				Gdi->CPen.Color = CoIpArea;
				Gdi->CBrush.Color = GRAYGREEN;
				JRect PaRt = Ball[x][y].IRoi;
				Gdi->OnRect(1, PaRt.l, PaRt.t, PaRt.r, PaRt.b, 5);
			}
		}}
	delete Gdi;
}

JRect CGrid::GetConsiderAngle(JRect TgRect, float IgAngle)
{
	float dg = Math.GetDegree(IRoi.lt, IRoi.rt);
	float Angle = dg >180 ? dg-360 : dg;

	TFPoint PStart = IRoi.Point();
	TFPoint PCenter = TgRect.Point();
	double xLg = TgRect.GetWidth();
	double yLg = TgRect.GetHeight();

	if (fabs(Angle)<IgAngle) {
		JRect AgRt;
		AgRt.l = PCenter.x - xLg/2 + 0.5;
		AgRt.t = PCenter.y - yLg/2 + 0.5;
		AgRt.r = PCenter.x + xLg/2 + 0.5;
		AgRt.b = PCenter.y + yLg/2 + 0.5;

		return AgRt;
	}

	PCenter = Math.OnRotate(PStart.x, PStart.y, PCenter.x, PCenter.y, Angle);

	JRect AgRt;
	AgRt.l = PCenter.x - xLg/2 + 0.5;
	AgRt.t = PCenter.y - yLg/2 + 0.5;
	AgRt.r = PCenter.x + xLg/2 + 0.5;
	AgRt.b = PCenter.y + yLg/2 + 0.5;

	return AgRt;
}

float CGrid::GetIpRectAngle()
{
	float dg = Math.GetDegree(IRoi.lt, IRoi.rt);
	return dg;
}

// void CGrid::OnUpdateTableMap(TStringGrid *Table, bool ToTable)
// {
// 	if (Table==NULL) return;
// 
// 	Table->RowCount = RowCount+1;
// 	Table->ColCount = ColCount+1;
// 
// 	for (int y=0; y<RowCount; y++) {
// 		for (int x=0; x<ColCount; x++) {
// 			if (y==0) Table->Cells[x+1][y  ] = AnsiString("X:")+x;
// 			if (x==0) Table->Cells[x  ][y+1] = AnsiString("Y:")+y;
// 
// 			if (Ball[x][y].IsTrExist) Table->Cells[x+1][y+1] = "O";
// 			else Table->Cells[x+1][y+1] = "";
// 		}}
// }

void CGrid::OnLoad(bool IsLoad, CString Path)
{
	return;

	if (Path.Trim()=="") return;

	// CString 사용시 Left,Mid,Right 사용하여 문자 추출해야함 
	//CString root = Path.SubString(1, Path.Pos(".ini")-1) ;

	//Path = root+"Grid.map";

	FILE *fp;

	if (IsLoad) fp = fopen(CW2A(Path),"rb");
	else        fp = fopen(CW2A(Path),"wb");

	if (fp == NULL) return;

	if (IsLoad) fread (this, 1, sizeof(CGrid), fp);
	else        fwrite(this, 1, sizeof(CGrid), fp);

	fclose(fp);
}
//-----------------------------------------------------------------------
// Class CBga 
//-----------------------------------------------------------------------
CBga::CBga(void)
{
	OpIpArea = false; CoIpArea = AQUA     ;
	OpIgArea = false; CoIgArea = GRAYGREEN;
	OpRsArea = true ; CoRsArea = BLUE_COLOR     ;
	OpRsAnls = true ; CoRsAnls = LIME_COLOR;

	OpMakeRC = false;
	OpUseMapCoordinate = true;
	Spec.OpPosition = false;
	IAddress = NULL;
	GImage   = NULL;
	IRoi  = new EROIBW8();
}
CBga::~CBga(void)
{
}
void CBga::OnClearData()
{
    for(int k=0; k<Grid.BallCount; k++) Quality[k].BinId = bgaPass;
}
bool CBga::OnLineFitRANSAC(TPoint *Q1, int N1, TPoint *Q2, int N2, double dist_th, int max_iter, int num_th)
{
    if (N1<4 || N1>500) return false;
    if (N2<4 || N2>500) return false;

    const int MaxPoint = 1000;

    int MaxNum = 3;
    int index  = 0;
    TPoint P     [MaxPoint];
    TPoint MaxP  [MaxPoint];
    double Weight[MaxPoint];

    srand(unsigned(time(0)));

    for (int i=0; i<max_iter; i++) {
        int i0, i1;
        i0 = N1==0 ? 0 : rand() % N1; P[0]=Q1[i0];
        i1 = N2==0 ? 0 : rand() % N2; P[1]=Q2[i1];

        if(P[1].x==P[0].x && P[1].y==P[0].y) continue;

        int n = 2;
        int Cnt = 0;
        float dist = 0;
        for (int k=0; k<N1+N2 ; k++) Weight[k] = 0.0;

        for(int j=0;j<N1; j++){ if (j==i0) continue; dist = GetLengthFrLineToPoint(P[0], P[1], Q1[j]); if (dist<=dist_th) { P[n++]=Q1[j]; Weight[Cnt++] = 1/sqrt(1+dist*dist); }}
        for(int j=0;j<N2; j++){ if (j==i1) continue; dist = GetLengthFrLineToPoint(P[0], P[1], Q2[j]); if (dist<=dist_th) { P[n++]=Q2[j]; Weight[Cnt++] = 1/sqrt(1+dist*dist); }}

        if(n>=num_th) {
            if (index++ > MAX_BGA_LINE) break;
            if (MaxNum < n) {
                MaxNum = n;
                for (int k=0; k<N1+N2 ; k++) MaxP[k] = P[k];
                //for (int k=0; k<MaxNum; k++) weight[k] = 0.6;
                OnLineFitPCA(MaxP, MaxNum, Weight);
            }
            for (int k=0; k<N1+N2; k++) P[k].x = P[k].y = 0;
            //break;
        }

    }

    return 1;

}

float CBga::GetLengthFrLineToPoint(TPoint P1, TPoint P2, TPoint P3)
{
    //--------------------------------------------------------------------------
    // 직선과 한점의 수직한 교차점에 의한 치수 측정
    // 공식 : ax + by + c = 0, (m, n) -> Distance = (ma + nb +c) / sqrt(a*a + b*b)
    //--------------------------------------------------------------------------

    int xDiff = P2.x-P1.x;
    int yDiff = P2.y-P1.y;

	if      (xDiff == 0) return fabs((float)(P3.x-P1.x));
	else if (yDiff == 0) return fabs((float)(P3.y-P1.y));

    float m    = yDiff/(float)xDiff;
    float dist = fabs((-m*P3.x + P3.y + m*P1.x - P1.y)/sqrt(m*m + 1));

    return dist;
}

float CBga::GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3)
{
    //--------------------------------------------------------------------------
    // 직선과 한점의 수직한 교차점에 의한 치수 측정
    // 공식 : ax + by + c = 0, (m, n) -> Distance = (ma + nb +c) / sqrt(a*a + b*b)
    //--------------------------------------------------------------------------

    float xDiff = P2.x-P1.x;
    float yDiff = P2.y-P1.y;

    if      (xDiff == 0) return fabs(P3.x-P1.x);
    else if (yDiff == 0) return fabs(P3.y-P1.y);

    float m    = yDiff/(float)xDiff;
    float dist = fabs((-m*P3.x + P3.y + m*P1.x - P1.y)/sqrt(m*m + 1));

    return dist;
}

int CBga::OnLineFitPCA(TPoint P[], int N, double weight[])
{
    //초기화시 weight[i]=1.;

    double sx=0,sy=0,sxx=0, syy=0,sxy=0, sw=0;

    for(int i=0; i<N; ++i){

         int x=P[i].x, y=P[i].y ;

         sx  += weight[i]*x  ;
         sy  += weight[i]*y  ;
         sxx += weight[i]*x*x;
         sxy += weight[i]*x*y;
         syy += weight[i]*y*y;
         sw  += weight[i]    ;
    }

    if (sw==0) return false;

    double vxx = (sxx - sx*sx/sw)/sw;
    double vxy = (sxy - sx*sy/sw)/sw;
    double vyy = (syy - sy*sy/sw)/sw;

    double xc = sx/sw;
    double yc = sy/sw;
    double theta = atan2(2*vxy, vxx-vyy)/2;
    double m = sin(theta)/cos(theta);

    bool IsHor = abs(P[0].x-P[1].x)>abs(P[0].y-P[1].y);

    P0.x = P[0].x; P0.y = m*P[0].x - m*xc + yc;
    P1.x = P[1].x; P1.y = m*P[1].x - m*xc + yc;

    return 1;
}

// Euresys Program
//------------------------------------------------------------------------------

bool CBga::OnCoordinate()
{
    Grid.IRoi = IpRect;

    for (int k=0; k <Boundary.BlobCount; k++) {
        int sx = Boundary.Blob[k].Rect.Point().x;
        int sy = Boundary.Blob[k].Rect.Point().y;

        TFPoint RPoint(sx, sy);

        int x = (RPoint.x-IpRect.l)/Grid.ColPitch;
        int y = (RPoint.y-IpRect.t)/Grid.RowPitch;

        Quality[k].x = x;
        Quality[k].y = y;
    }
    return true;
}

bool CBga::OnPosition()
{
    bool IsPass = true;

    for (int k=0; k <Boundary.BlobCount-1; k++) {
        //Position X, Y검사 루틴이지만 현재는 각도의 걔념이 없어 사용안함
        //----------------------------------------------------------------------
        //float Cx = Boundary.Blob[k].Rect.Point().x-Boundary.Blob[0].Rect.Point().x;
        //float Cy = Boundary.Blob[k].Rect.Point().y-Boundary.Blob[0].Rect.Point().y;
        //Position[k].Dx = fabs(Position[k].Px-Cx);
        //Position[k].Dy = fabs(Position[k].Py-Cy);
        //if (Position[k].Dy>Position[k].Ty) { IsPass=false; Position[k].IsPass = false; }
        //if (Position[k].Dy>Position[k].Ty) { IsPass=false; Position[k].IsPass = false; }

        //TRAIN BALL의 BALL간 간격
        //----------------------------------------------------------------------
        TFPoint FP0(Position[k  ].Px, Position[k  ].Py);
        TFPoint FP1(Position[k+1].Px, Position[k+1].Py);
        double Dp = Math.GetLength(FP1, FP0);

        //CURRENT BALL의 BALL간 간격
        //----------------------------------------------------------------------
        TFPoint FC0(Boundary.Blob[k  ].Rect.Point().x, Boundary.Blob[k  ].Rect.Point().y);
        TFPoint FC1(Boundary.Blob[k+1].Rect.Point().x, Boundary.Blob[k+1].Rect.Point().y);
        double Dc = Math.GetLength(FC1, FC0);

        Position[k].Do = fabs(Dp-Dc);

        Position[k].IsPass = true;
        if (Position[k].Do>Spec.TPosition) { IsPass=false; Position[k].IsPass = false; }
        // Position 검사시 마지막 볼은 검사하지 않으므로 무조건 True
        //20160711
        if(k==(Boundary.BlobCount-2)) Position[Boundary.BlobCount-1].IsPass = true;
    }

    if (!IsPass) BinId = bgaPosition;

    return IsPass;
}

bool CBga::OnRingQuality()
{
    int x = IpRect.l;
    int y = IpRect.t;
    int w = IpRect.r-x;
    int h = IpRect.b-y;

    float fw = 0;
    float fh = 0;
    float DiaMean =   0;
    float DiaMax  =   0;
    float DiaMin  =   0;
    float Roundness = 0;
    float BallArea  = 0;
    float BlobArea  = 0;
    float AreaRatio = 0;
    float FormFactor= 0;

    float d0 = 0;
    float d1 = 0;
    float d2 = 0;

    int RingDarkOffset        = 0;
    int RingBrightOffset      = 0;
    int InnerRingThholdOffset = 0;
    int OuterRingThholdOffset = 0;
    int PreX                  = 0;
    int PreY                  = 0;

    int Wd   = 0;
    int Ht   = 0;
    int Diam = 0;

    TPoint __C;
    int SumPixel = 0;
    int AutoThHold = 0;

    int cnt = -1;
    bool isBreak = false;
    bool IsPass = true;

    int Iw = IWidth;
    int Ih = IHeight;

//     if (IAddress==NULL) return false;
//     #define IElement(x, y)  *((BYTE *)IAddress + x + Iw * y)

    // 20160707
    int ThHold =0;
    if(Spec.OpSideBright){
        ThHold = Boundary.TrPa.LoThHold;
    }
    else {
        ThHold = Boundary.TrPa.HiThHold;
    }
    
    FResultInRingScore  = 0;
    FResultOutRingScore = 0;
    double PvInRingScore = 0;
    double PvOutRingScore = 0;

    int OQIndex = 10;

    switch (OQIndex) {

        default : OQIndex=0; return 0;

        case 10 :
                  if (Spec.TInRingScore <1) Spec.TInRingScore  = 70;
                  if (Spec.TOutRingScore<1) Spec.TOutRingScore = 70;

                  OQIndex++;

        case 11 :
                  OQIndex++;

        case 12 :
                  OQIndex++;

        case 13 :
                  OQIndex++;

        case 14 :
                  if(!Spec.OpSideBright){
                      for (register int k=0; k<BallCount; k++) {
                          int MaxInRingScore  = 0;
                          int MaxOutRingScore = 0;
                          Quality[k].BinId = bgaPass;

                          for (register int z=0; z<5; z++) {
                              int MvCnt_X = 0;
                              int MvCnt_Y = 0;
                              int RInRingOffset =0;
                              if (z==0) RInRingOffset = InRingOffset  ;
                              if (z==1) RInRingOffset = InRingOffset+1;
                              if (z==2) RInRingOffset = InRingOffset+2;
                              if (z==3) RInRingOffset = InRingOffset+3;
                              if (z==4) RInRingOffset = InRingOffset+4;

                              x  = Boundary.Blob[k].Rect.Point().x + MvCnt_X + 0.5;
                              y  = Boundary.Blob[k].Rect.Point().y + MvCnt_Y + 0.5;
                              w  = Boundary.Blob[k].Rect.GetWidth();
                              h  = Boundary.Blob[k].Rect.GetHeight();

                              //fw = Boundary.Blob[k].Radius*2;
                              //fh = Boundary.Blob[k].Radius*2;
                              //DiaMean = Boundary.Blob[k].Radius*2;
                              //DiaMax  = Boundary.Blob[k].MaxRadius*2;
                              //DiaMin  = Boundary.Blob[k].MinRadius*2;
                              //Roundness = h/w * 100.0;

                              DiaMean = Boundary.Blob[k].Diameter;
                              DiaMax  = (Boundary.Blob[k].Rect.GetWidth()>Boundary.Blob[k].Rect.GetHeight()) ? Boundary.Blob[k].Rect.GetWidth() : Boundary.Blob[k].Rect.GetHeight();
                              DiaMin  = (Boundary.Blob[k].Rect.GetWidth()<Boundary.Blob[k].Rect.GetHeight()) ? Boundary.Blob[k].Rect.GetWidth() : Boundary.Blob[k].Rect.GetHeight();
                              Roundness = h/w * 100.0;

                              if (Boundary.Blob[k].Rect.Point().x<1) break;
                              if (Boundary.Blob[k].Rect.Point().y<1) break;

                              TFPoint C(x, y);

                              if (InRingOffset <1) InRingOffset =DiaMean/4;
                              if (OutRingOffset<1) OutRingOffset=DiaMean/4;

                              if (x>3 && y>3) {
                                  TFPoint DS(x, y-(DiaMean/2.0-RInRingOffset));
                                  TFPoint DR (0,0);
                                  TFPoint PreDR(0,0);
                                  TFPoint BS(x, y-((DiaMean+1)/2.0+OutRingOffset));
                                  TFPoint BR (0,0);
                                  TFPoint PreBR(0,0);
                                  bool IsDarkPx = true;
                                  bool IsBrightPx = true;

                                  int InRingCount = 0;
                                  int OutRingCount = 0;
                                  int RingCount = 0;

                                  for (int i=0; i<360; i+=8) {
                                      DR = Math.OnRotate(C, DS, i);
                                      BR = Math.OnRotate(C, BS, i);
                                      if(PreDR.x==DR.x && PreDR.y==DR.y) continue;
                                      if(PreBR.x==BR.x && PreBR.y==BR.y) continue;

                                      bool IsDarkPx = true;
                                      bool IsBrightPx = true;

                                      // 20160707
                                      if (Spec.OpSideBright){
                                          if (IElement(int(DR.x), int(DR.y)) > ThHold) IsDarkPx = false;
                                          if (IElement(int(BR.x), int(BR.y)) < ThHold) IsBrightPx = false;
                                      }
                                      else {
                                          if (IElement(int(DR.x), int(DR.y)) < ThHold) IsDarkPx = false;
                                          if (IElement(int(BR.x), int(BR.y)) > ThHold) IsBrightPx = false;
                                      }

                                      if (IsDarkPx  ) InRingCount++;
                                      if (IsBrightPx) OutRingCount++;

                                      PreDR = DR; PreBR = BR;
                                      RingCount++;
                                  }
                                  Quality[k].InRingScore  = 100-((InRingCount*100 )/RingCount);
                                  Quality[k].OutRingScore = 100-((OutRingCount*100)/RingCount);
                              }

                              if (Quality[k].InRingScore<Spec.TInRingScore || Quality[k].OutRingScore<Spec.TOutRingScore){
                                  if ((Quality[k].InRingScore <Spec.TInRingScore ) && (Quality[k].InRingScore >MaxInRingScore )) MaxInRingScore  = Quality[k].InRingScore ;
                                  if ((Quality[k].OutRingScore<Spec.TOutRingScore) && (Quality[k].OutRingScore>MaxOutRingScore)) MaxOutRingScore = Quality[k].OutRingScore;
                                  break;
                              }
                              Quality[k].MvCx = MvCnt_X;
                              Quality[k].MvCy = MvCnt_Y;
                              continue;
                          }

                          if (Spec.OpInRing  && Quality[k].InRingScore <Spec.TInRingScore ) Quality[k].BinId = bgaInRingQuality ;
                          if (Spec.OpOutRing && Quality[k].OutRingScore<Spec.TOutRingScore) Quality[k].BinId = bgaOutRingQuality;

                          if (Quality[k].BinId==bgaInRingQuality) {
                              Quality[k].InRingScore = MaxInRingScore;
                              if (MaxInRingScore>PvInRingScore) {
                                   PvInRingScore = MaxInRingScore;
                                   FResultInRingScore = MaxInRingScore;
                              }
                          }

                          if (Quality[k].BinId==bgaOutRingQuality) {
                              Quality[k].OutRingScore= MaxOutRingScore;
                              if (MaxOutRingScore>PvOutRingScore) {
                                   PvOutRingScore = MaxOutRingScore;
                                   FResultOutRingScore = MaxOutRingScore;
                              }
                          }
                      }
                  }
                  else {
                      // 20160715 저각조명 사용시 검서 시퀀스
                      for (register int k=0; k<BallCount; k++) {
                          int MaxInRingScore  = 0;
                          int MaxOutRingScore = 0;
                          Quality[k].BinId = bgaPass;

                          for (register int z=0; z<6; z++) {
                              int MvCnt_X = 0;
                              int MvCnt_Y = 0;
                              if (z==1) { MvCnt_X =  1; MvCnt_Y = -1;}
                              if (z==2) { MvCnt_X =  1; MvCnt_Y =  1;}
                              if (z==3) { MvCnt_X = -1; MvCnt_Y =  1;}
                              if (z==4) { MvCnt_X = -1; MvCnt_Y = -1;}
                              if (z==5) { MvCnt_X =  0; MvCnt_Y =  0;}

                              //Boundary.Blob[k].MvCx = 0;
                              //Boundary.Blob[k].MvCy = 0;
                              x  = Boundary.Blob[k].Rect.Point().x + MvCnt_X + 0.5;
                              y  = Boundary.Blob[k].Rect.Point().y + MvCnt_Y + 0.5;
                              w  = Boundary.Blob[k].Rect.GetWidth();
                              h  = Boundary.Blob[k].Rect.GetHeight();

                              //fw = Boundary.Blob[k].Radius*2;
                              //fh = Boundary.Blob[k].Radius*2;
                              //DiaMean = Boundary.Blob[k].Radius*2;
                              //DiaMax  = Boundary.Blob[k].MaxRadius*2;
                              //DiaMin  = Boundary.Blob[k].MinRadius*2;
                              //Roundness = h/w * 100.0;

                              DiaMean = Boundary.Blob[k].Diameter;
                              DiaMax  = (Boundary.Blob[k].Rect.GetWidth()>Boundary.Blob[k].Rect.GetHeight()) ? Boundary.Blob[k].Rect.GetWidth() : Boundary.Blob[k].Rect.GetHeight();
                              DiaMin  = (Boundary.Blob[k].Rect.GetWidth()<Boundary.Blob[k].Rect.GetHeight()) ? Boundary.Blob[k].Rect.GetWidth() : Boundary.Blob[k].Rect.GetHeight();
                              Roundness = h/w * 100.0;

                              if (Boundary.Blob[k].Rect.Point().x<1) break;
                              if (Boundary.Blob[k].Rect.Point().y<1) break;

                              TFPoint C(x, y);

                              if (InRingOffset <1) InRingOffset =DiaMean/4;
                              if (OutRingOffset<1) OutRingOffset=DiaMean/4;

                              if (x>3 && y>3) {
                                  TFPoint DS(x, y-(DiaMean/2.0-InRingOffset));
                                  TFPoint DR (0,0);
                                  TFPoint PreDR(0,0);
                                  TFPoint BS(x, y-((DiaMean+1)/2.0+OutRingOffset));
                                  TFPoint BR (0,0);
                                  TFPoint PreBR(0,0);
                                  bool IsDarkPx = true;
                                  bool IsBrightPx = true;

                                  int InRingCount = 0;
                                  int OutRingCount = 0;
                                  int RingCount = 0;

                                  for (int i=0; i<360; i+=8) {
                                      DR = Math.OnRotate(C, DS, i);
                                      BR = Math.OnRotate(C, BS, i);
                                      if(PreDR.x==DR.x && PreDR.y==DR.y) continue;
                                      if(PreBR.x==BR.x && PreBR.y==BR.y) continue;

                                      bool IsDarkPx = true;
                                      bool IsBrightPx = true;

                                      // 20160707
                                      if (Spec.OpSideBright){
                                          if (IElement(int(DR.x), int(DR.y)) > ThHold) IsDarkPx = false;
                                          if (IElement(int(BR.x), int(BR.y)) < ThHold) IsBrightPx = false;
                                      }
                                      else {
                                          if (IElement(int(DR.x), int(DR.y)) < ThHold) IsDarkPx = false;
                                          if (IElement(int(BR.x), int(BR.y)) > ThHold) IsBrightPx = false;
                                      }

                                      if (IsDarkPx  ) InRingCount++;
                                      if (IsBrightPx) OutRingCount++;

                                      PreDR = DR; PreBR = BR;
                                      RingCount++;
                                  }
                                  //Quality[k].InRingScore  = 100-(InRingCount /RingCount)*100;
                                  //Quality[k].OutRingScore = 100-(OutRingCount/RingCount)*100;
                                  Quality[k].InRingScore  = 100-((InRingCount*100 )/RingCount);
                                  Quality[k].OutRingScore = 100-((OutRingCount*100)/RingCount);
                              }

                              if (Quality[k].InRingScore<Spec.TInRingScore || Quality[k].OutRingScore<Spec.TOutRingScore){
                                  if ((Quality[k].InRingScore <Spec.TInRingScore ) && (Quality[k].InRingScore >MaxInRingScore )) MaxInRingScore  = Quality[k].InRingScore ;
                                  if ((Quality[k].OutRingScore<Spec.TOutRingScore) && (Quality[k].OutRingScore>MaxOutRingScore)) MaxOutRingScore = Quality[k].OutRingScore;
                                  continue;
                              }
                              Quality[k].MvCx = MvCnt_X;
                              Quality[k].MvCy = MvCnt_Y;
                              break;
                          }

                          if (Spec.OpInRing  && Quality[k].InRingScore <Spec.TInRingScore ) Quality[k].BinId = bgaInRingQuality ;
                          if (Spec.OpOutRing && Quality[k].OutRingScore<Spec.TOutRingScore) Quality[k].BinId = bgaOutRingQuality;

                          if (Quality[k].BinId==bgaInRingQuality) {
                              Quality[k].InRingScore = MaxInRingScore;
                              if (MaxInRingScore>PvInRingScore) {
                                   PvInRingScore = MaxInRingScore;
                                   FResultInRingScore = MaxInRingScore;
                              }
                          }

                          if (Quality[k].BinId==bgaOutRingQuality) {
                              Quality[k].OutRingScore= MaxOutRingScore;
                              if (MaxOutRingScore>PvOutRingScore) {
                                   PvOutRingScore = MaxOutRingScore;
                                   FResultOutRingScore = MaxOutRingScore;
                              }
                          }
                      }
                  }
                  OQIndex++;

        case 15 :
                  for(register int k=0; k<BallCount; k++){
                      if (Quality[k].BinId!=bgaPass) {
                          IsPass = false;
                          break;
                      }
                  }
                  if(!IsPass) BinId = bgaInRingQuality;
                  OQIndex=0;
    }
    return IsPass;
}

int CBga::OnMeasuring()
{
    int Ix = IpRect.l;
    int Iy = IpRect.t;
    int Iw = IpRect.r-Ix;
    int Ih = IpRect.b-Iy;

    for (int y=0; y<Grid.RowCount; y++) {        //초기화
    for (int x=0; x<Grid.ColCount; x++) {
        FPattern.WP[x][y].x  = 0;
        FPattern.WP[x][y].y  = 0;
        FPattern.CP[x][y].x  = 0;
        FPattern.CP[x][y].y  = 0;
        FPattern.BP[x][y].x  = 0;
        FPattern.BP[x][y].y  = 0;
        FPattern.Obj[x][y].x = 0;
        FPattern.Obj[x][y].y = 0;

        FPattern.Col[x].DataCount = 0; FPattern.Col[x].P0.x = 0; FPattern.Col[x].P0.y = 0; FPattern.Col[x].P1.x = 0; FPattern.Col[x].P1.y = 0;
        FPattern.Col[x].DataCount = 0; FPattern.Col[x].P0.x = 0; FPattern.Col[x].P0.y = 0; FPattern.Col[x].P1.x = 0; FPattern.Col[x].P1.y = 0;
        FPattern.Col[x].DataCount = 0; FPattern.Col[x].P0.x = 0; FPattern.Col[x].P0.y = 0; FPattern.Col[x].P1.x = 0; FPattern.Col[x].P1.y = 0;
        FPattern.Col[x].DataCount = 0; FPattern.Col[x].P0.x = 0; FPattern.Col[x].P0.y = 0; FPattern.Col[x].P1.x = 0; FPattern.Col[x].P1.y = 0;
        FPattern.Row[y].DataCount = 0; FPattern.Row[y].P0.x = 0; FPattern.Row[y].P0.y = 0; FPattern.Row[y].P1.x = 0; FPattern.Row[y].P1.y = 0;
        FPattern.Row[y].DataCount = 0; FPattern.Row[y].P0.x = 0; FPattern.Row[y].P0.y = 0; FPattern.Row[y].P1.x = 0; FPattern.Row[y].P1.y = 0;
        FPattern.Row[y].DataCount = 0; FPattern.Row[y].P0.x = 0; FPattern.Row[y].P0.y = 0; FPattern.Row[y].P1.x = 0; FPattern.Row[y].P1.y = 0;
        FPattern.Row[y].DataCount = 0; FPattern.Row[y].P0.x = 0; FPattern.Row[y].P0.y = 0; FPattern.Row[y].P1.x = 0; FPattern.Row[y].P1.y = 0;

        Grid.Ball[x][y].CuPitchx   = 0;
        Grid.Ball[x][y].CuPitchy   = 0;
        Grid.Ball[x][y].CuPx       = 0;
        Grid.Ball[x][y].CuPy       = 0;
        Grid.Ball[x][y].CuDiameter = 0;
        Grid.Ball[x][y].CuShape    = 0;

    }}

    FPattern.RowCount = Grid.RowCount;
    FPattern.ColCount = Grid.ColCount;

    JRect GridRect;
    for (int y=0; y<Grid.RowCount; y++) {
    for (int x=0; x<Grid.ColCount; x++) {

        //Grid Ball Position
        int Sx = IpRect.l + Grid.ColStart + Grid.ColPitch * x;
        int Sy = IpRect.t + Grid.RowStart + Grid.RowPitch * y;
        Grid.Ball[x][y].IRoi.l = Sx;
        Grid.Ball[x][y].IRoi.t = Sy;
        Grid.Ball[x][y].IRoi.r = Sx+Grid.ColPitch;
        Grid.Ball[x][y].IRoi.b = Sy+Grid.RowPitch;

        if(x==0 && y==0){
            GridRect.l = Grid.Ball[x][y].IRoi.l;
            GridRect.t = Grid.Ball[x][y].IRoi.t;
        }
        if(x==Grid.ColCount-1 && y==Grid.RowCount-1){
            GridRect.r = Grid.Ball[x][y].IRoi.r;
            GridRect.b = Grid.Ball[x][y].IRoi.b;
        }
    }}

    float dxDv = IpRect.Point().x - GridRect.Point().x;
    float dyDv = IpRect.Point().y - GridRect.Point().y;

    int MatchCount =0;

    OpUseMapCoordinate = false;

    if (OpUseMapCoordinate) {
        //--------------------------------------------------------------------------
        // START : MAPPING GRID에 의한 돠표설정
        for (int n=0; n<BallCount; n++) {
            float Shape = Boundary.Blob[n].Radius/Boundary.Blob[n].MaxRadius * 100.0;

            int xp = Boundary.Blob[n].Rect.Point().x;
            int yp = Boundary.Blob[n].Rect.Point().y;

            TPoint CP;

            if (OpMakeRC) {
                CP = Grid.GetGridRowCol(Quality[n].x, Quality[n].y, xp, yp, dxDv, dxDv);
            }
            else {
                CP = Grid.GetGridRowCol(xp, yp);
            }

            if (CP.x<0 || CP.y<0 || !Grid.Ball[CP.x][CP.y].IsTrExist) {
                int Um = 3;
                continue;
            }
            Grid.Ball[CP.x][CP.y].CuPx = Boundary.Blob[n].Rect.Point().x;
            Grid.Ball[CP.x][CP.y].CuPy = Boundary.Blob[n].Rect.Point().y;
            Grid.Ball[CP.x][CP.y].CuShape = Shape;
            Grid.Ball[CP.x][CP.y].CuDiameter = Boundary.Blob[n].Diameter;

            FPattern.WP[CP.x][CP.y].x = Boundary.Blob[n].Rect.Point().x;
            FPattern.WP[CP.x][CP.y].y = Boundary.Blob[n].Rect.Point().y;
            FPattern.CP[CP.x][CP.y].x = Boundary.Blob[n].Rect.Point().x;
            FPattern.CP[CP.x][CP.y].y = Boundary.Blob[n].Rect.Point().y;
            FPattern.BP[CP.x][CP.y].x = Boundary.Blob[n].Rect.Point().x;
            FPattern.BP[CP.x][CP.y].y = Boundary.Blob[n].Rect.Point().y;

            FPattern.Obj[CP.x][CP.y].x = FPattern.WP[CP.x][CP.y].x;
            FPattern.Obj[CP.x][CP.y].y = FPattern.WP[CP.x][CP.y].y;
            MatchCount++;
        }
        // END : MAPPING GRID에 의한 돠표설정
        //----------------------------------------------------------------------
    }
    if (!OpUseMapCoordinate) {
        int MapCount = 0;
        for (int y=0; y<Grid.RowCount; y++) {
        for (int x=0; x<Grid.ColCount; x++) {
            Grid.Ball[x][y].CuPx = 0;
            Grid.Ball[x][y].CuPy = 0;
            Grid.Ball[x][y].CuPitchx = 0;
            Grid.Ball[x][y].CuPitchy = 0;

            //Grid.Ball[x][y].IsCuExist = Grid.Ball[x][y].IsTrExist;
            if (!Grid.Ball[x][y].IsTrExist) continue;

            float Shape = Boundary.Blob[MapCount].Radius/Boundary.Blob[MapCount].MaxRadius * 100.0;

            Grid.Ball[x][y].CuPx = Boundary.Blob[MapCount].Rect.Point().x;
            Grid.Ball[x][y].CuPy = Boundary.Blob[MapCount].Rect.Point().y;
            Grid.Ball[x][y].CuShape = Shape;
            Grid.Ball[x][y].CuDiameter = Boundary.Blob[MapCount].Diameter;

            FPattern.WP[x][y].x = Boundary.Blob[MapCount].Rect.Point().x;
            FPattern.WP[x][y].y = Boundary.Blob[MapCount].Rect.Point().y;
            FPattern.CP[x][y].x = Boundary.Blob[MapCount].Rect.Point().x;
            FPattern.CP[x][y].y = Boundary.Blob[MapCount].Rect.Point().y;
            FPattern.BP[x][y].x = Boundary.Blob[MapCount].Rect.Point().x;
            FPattern.BP[x][y].y = Boundary.Blob[MapCount].Rect.Point().y;

            FPattern.Obj[x][y].x = FPattern.WP[x][y].x;
            FPattern.Obj[x][y].y = FPattern.WP[x][y].y;

            MapCount++;
        }}
    }
//------------------------------------------------------------------------------
// Grid에의한 Mapping이 완료되지 못한경우 다음검사 없이 리턴처리
//------------------------------------------------------------------------------
//    for (int y=0; y<Grid.RowCount; y++) {
//    for (int x=0; x<Grid.ColCount; x++) {
//        if(!Grid.Ball[x][y].IsTrExist) continue;
//        if(Grid.Ball[x][y].CuPx ==0 && Grid.Ball[x][y].CuPy == 0) return false;
//    }}
//------------------------------------------------------------------------------
    //if (MatchCount!=BallCount){
    //    BinId = biBallMissMatch;
    //    EMessage = AnsiString().sprintf("[FAIL] Match Count %4d , Ball Count %4d", MatchCount, BallCount);
    //    return false;
    //}

    TFPoint FPoint[MAX_FIT_LINE];
    for (int y=0; y<Grid.RowCount; y++) {
        int Count = 0;
        for (int x=0; x<Grid.ColCount; x++) {
            FPoint[Count].x = 0;
            FPoint[Count].y = 0;

            if (!Grid.Ball[x][y].IsTrExist) continue;

            FPoint[Count].x = FPattern.Obj[x][y].x;
            FPoint[Count].y = FPattern.Obj[x][y].y;
            Count++;

        }
        FPattern.Row[y].OnRansac(Count, FPoint);
    }

    for (int x=0; x<Grid.ColCount; x++) {
        int Count=0;
        for (int y=0; y<Grid.RowCount; y++) {
            FPoint[Count].x = 0;
            FPoint[Count].y = 0;

            if (!Grid.Ball[x][y].IsTrExist) continue;

            FPoint[Count].x = FPattern.Obj[x][y].x;
            FPoint[Count].y = FPattern.Obj[x][y].y;

            Count++;

        }
        FPattern.Col[x].OnRansac(Count, FPoint);
    }

    FPattern.OnUpdatePos(OpUseLineIntersect, Grid.ColCount, Grid.RowCount);  // Pos[x][y]좌표에 실제 교차점을 찾아서 넘겨준다

    for (int y=0; y<Grid.RowCount; y++) {
    for (int x=0; x<Grid.ColCount; x++) {
        Grid.Ball[x][y].CuPx = 0;
        Grid.Ball[x][y].CuPy = 0;
        Grid.Ball[x][y].CuPitchx = 0;
        Grid.Ball[x][y].CuPitchy = 0;

        if (!Grid.Ball[x][y].IsTrExist) continue;

        //Px Py
        float Wx = fabs(FPattern.Pos[x][y].x-FPattern.WP[x][y].x);
        float Cx = fabs(FPattern.Pos[x][y].x-FPattern.CP[x][y].x);
        float Bx = fabs(FPattern.Pos[x][y].x-FPattern.BP[x][y].x);
        float Tx = Wx;
        if (Cx<Tx) { Tx = Cx; FPattern.Obj[x][y] = FPattern.CP[x][y]; }
        if (Bx<Tx) { Tx = Bx; FPattern.Obj[x][y] = FPattern.BP[x][y]; }
        Grid.Ball[x][y].CuPx = Tx;

        float Wy = fabs(FPattern.Pos[x][y].y-FPattern.WP[x][y].y);
        float Cy = fabs(FPattern.Pos[x][y].y-FPattern.CP[x][y].y);
        float By = fabs(FPattern.Pos[x][y].y-FPattern.BP[x][y].y);
        float Ty = Wy;
        if (Cy<Ty) { Ty = Cy; FPattern.Obj[x][y] = FPattern.CP[x][y]; }
        if (By<Ty) { Ty = By; FPattern.Obj[x][y] = FPattern.BP[x][y]; }
        Grid.Ball[x][y].CuPy = Ty;

        //Pitch
        if ((x!=(Grid.ColCount-1)) && FPattern.Pos[x+1][y  ].x && FPattern.Pos[x+1][y  ].y) Grid.Ball[x][y].CuPitchx = Math.GetLength(FPattern.Pos[x+1][y  ], FPattern.Pos[x][y]);
        if ((x==(Grid.ColCount-1)) && FPattern.Pos[x-1][y  ].x && FPattern.Pos[x-1][y  ].y) Grid.Ball[x][y].CuPitchx = Math.GetLength(FPattern.Pos[x-1][y  ], FPattern.Pos[x][y]);

        if ((y!=(Grid.RowCount-1)) && FPattern.Pos[x  ][y+1].x && FPattern.Pos[x  ][y+1].y) Grid.Ball[x][y].CuPitchy = Math.GetLength(FPattern.Pos[x  ][y+1], FPattern.Pos[x][y]);
        if ((y==(Grid.RowCount-1)) && FPattern.Pos[x  ][y-1].x && FPattern.Pos[x  ][y-1].y) Grid.Ball[x][y].CuPitchy = Math.GetLength(FPattern.Pos[x  ][y-1], FPattern.Pos[x][y]);
    }}

    float SumPitchx   = 0; int PitchCountx  = 0;
    float SumPitchy   = 0; int PitchCounty  = 0;
    float SumPx       = 0; int PxCount      = 0;
    float SumPy       = 0; int PyCount      = 0;
    float SumDiameter = 0; int DiameterCount= 0;
    float SumShape    = 0; int ShapeCount   = 0;

    //--------------------------------------------------------------------------
    // START : Absloute Value Calculate & Pitch 동서남북
    for (register int y=0; y<Grid.RowCount; y++) {
    for (register int x=0; x<Grid.ColCount; x++) {
        if (!Grid.Ball[x][y].IsTrExist) continue;

        if (Grid.Ball[x][y].CuPitchx  ) { SumPitchx   += Grid.Ball[x][y].CuPitchx  ; PitchCountx  ++; }
        if (Grid.Ball[x][y].CuPitchy  ) { SumPitchy   += Grid.Ball[x][y].CuPitchy  ; PitchCounty  ++; }
        if (Grid.Ball[x][y].CuPx      ) { SumPx       += Grid.Ball[x][y].CuPx      ; PxCount      ++; }
        if (Grid.Ball[x][y].CuPy      ) { SumPy       += Grid.Ball[x][y].CuPy      ; PyCount      ++; }
        if (Grid.Ball[x][y].CuDiameter) { SumDiameter += Grid.Ball[x][y].CuDiameter; DiameterCount++; }
        if (Grid.Ball[x][y].CuShape   ) { SumShape    += Grid.Ball[x][y].CuShape   ; ShapeCount   ++; }

        if (Grid.Ball[x][y].CuPitchx  >Grid.MaxCuPitchx  ) { Grid.MaxCuPitchx   = Grid.Ball[x][y].CuPitchx  ; }
        if (Grid.Ball[x][y].CuPitchy  >Grid.MaxCuPitchy  ) { Grid.MaxCuPitchy   = Grid.Ball[x][y].CuPitchy  ; }
        if (Grid.Ball[x][y].CuPx      >Grid.MaxCuPx      ) { Grid.MaxCuPx       = Grid.Ball[x][y].CuPx      ; }
        if (Grid.Ball[x][y].CuPy      >Grid.MaxCuPy      ) { Grid.MaxCuPy       = Grid.Ball[x][y].CuPy      ; }
        if (Grid.Ball[x][y].CuDiameter>Grid.MaxCuDiameter) { Grid.MaxCuDiameter = Grid.Ball[x][y].CuDiameter; }
        if (Grid.Ball[x][y].CuShape   >Grid.MaxCuShape   ) { Grid.MaxCuShape    = Grid.Ball[x][y].CuShape   ; }

        if (Grid.Ball[x][y].CuPitchx  <Grid.MinCuPitchx  ) { Grid.MinCuPitchx   = Grid.Ball[x][y].CuPitchx  ; }
        if (Grid.Ball[x][y].CuPitchy  <Grid.MinCuPitchy  ) { Grid.MinCuPitchy   = Grid.Ball[x][y].CuPitchy  ; }
        if (Grid.Ball[x][y].CuPx      <Grid.MinCuPx      ) { Grid.MinCuPx       = Grid.Ball[x][y].CuPx      ; }
        if (Grid.Ball[x][y].CuPy      <Grid.MinCuPy      ) { Grid.MinCuPy       = Grid.Ball[x][y].CuPy      ; }
        if (Grid.Ball[x][y].CuDiameter<Grid.MinCuDiameter) { Grid.MinCuDiameter = Grid.Ball[x][y].CuDiameter; }
        if (Grid.Ball[x][y].CuShape   <Grid.MinCuShape   ) { Grid.MinCuShape    = Grid.Ball[x][y].CuShape   ; }

        if (DgTrain) {
            Grid.Ball[x][y].TrPitchx   = Grid.Ball[x][y].CuPitchx  ;
            Grid.Ball[x][y].TrPitchy   = Grid.Ball[x][y].CuPitchy  ;
            Grid.Ball[x][y].TrPx       = Grid.Ball[x][y].CuPx      ;
            Grid.Ball[x][y].TrPy       = Grid.Ball[x][y].CuPy      ;
            Grid.Ball[x][y].TrDiameter = Grid.Ball[x][y].CuDiameter;
            Grid.Ball[x][y].TrShape    = Grid.Ball[x][y].CuShape   ;
        }
    }}

    if (PitchCountx  ) Grid.AvgCuPitchx   = SumPitchx  /PitchCountx  ;
    if (PitchCounty  ) Grid.AvgCuPitchy   = SumPitchy  /PitchCounty  ;
    if (PxCount      ) Grid.AvgCuPx       = SumPx      /PxCount      ;
    if (PyCount      ) Grid.AvgCuPy       = SumPy      /PyCount      ;
    if (DiameterCount) Grid.AvgCuDiameter = SumDiameter/DiameterCount;
    if (ShapeCount   ) Grid.AvgCuShape    = SumShape   /ShapeCount   ;

    if (DgTrain) {
        Grid.AvgTrPitchx   = Grid.AvgCuPitchx  ;
        Grid.AvgTrPitchy   = Grid.AvgCuPitchy  ;
        Grid.AvgTrPx       = Grid.AvgCuPx      ;
        Grid.AvgTrPy       = Grid.AvgCuPy      ;
        Grid.AvgTrDiameter = Grid.AvgCuDiameter;
        Grid.AvgTrShape    = Grid.AvgCuShape   ;
    }

    //Default Value
    for (int y=0; y<Grid.RowCount; y++) {
    for (int x=0; x<Grid.ColCount; x++) {
        //if (!Grid.Ball[x][y].IsTrExist) continue;

        if (Grid.Ball[x][y].CuPitchx==0) Grid.Ball[x][y].CuPitchx = Grid.AvgCuPitchx;
        if (Grid.Ball[x][y].CuPitchy==0) Grid.Ball[x][y].CuPitchy = Grid.AvgCuPitchy;
    }}

    //--------------------------------------------------------------------------
    // START : Calculate Deviation
    SumPitchx   = 0; PitchCountx  = 0;
    SumPitchy   = 0; PitchCounty  = 0;
    SumPx       = 0; PxCount      = 0;
    SumPy       = 0; PyCount      = 0;
    SumDiameter = 0; DiameterCount= 0;
    SumShape    = 0; ShapeCount   = 0;

    for (register int y=0; y<Grid.RowCount; y++) {
    for (register int x=0; x<Grid.ColCount; x++) {
        if (!Grid.Ball[x][y].IsTrExist) continue;

        Grid.Ball[x][y].DvPx = fabs(Grid.Ball[x][y].CuPx) - fabs(Grid.AvgCuPx);
        Grid.Ball[x][y].DvPy = fabs(Grid.Ball[x][y].CuPy) - fabs(Grid.AvgCuPy);

        //Grid.Ball[x][y].DvPitchx   = fabs(Grid.Ball[x][y].CuPitchx)  - fabs(Grid.AvgCuPitchx);
        //Grid.Ball[x][y].DvPitchy   = fabs(Grid.Ball[x][y].CuPitchy)  - fabs(Grid.AvgCuPitchy);
        Grid.Ball[x][y].DvPitchx = Grid.Ball[x][y].CuPx;
        Grid.Ball[x][y].DvPitchy = Grid.Ball[x][y].CuPy;

        Grid.Ball[x][y].DvDiameter = Grid.Ball[x][y].CuDiameter- Grid.AvgCuDiameter;
        Grid.Ball[x][y].DvShape    = Grid.Ball[x][y].CuShape   - Grid.AvgCuShape   ;

        if (Grid.Ball[x][y].DvPitchx  ) { SumPitchx   += Grid.Ball[x][y].DvPitchx  ; PitchCountx  ++; }
        if (Grid.Ball[x][y].DvPitchy  ) { SumPitchy   += Grid.Ball[x][y].DvPitchy  ; PitchCounty  ++; }
        if (Grid.Ball[x][y].DvPx      ) { SumPx       += Grid.Ball[x][y].DvPx      ; PxCount      ++; }
        if (Grid.Ball[x][y].DvPy      ) { SumPy       += Grid.Ball[x][y].DvPy      ; PyCount      ++; }
        if (Grid.Ball[x][y].DvDiameter) { SumDiameter += Grid.Ball[x][y].DvDiameter; DiameterCount++; }
        if (Grid.Ball[x][y].DvShape   ) { SumShape    += Grid.Ball[x][y].DvShape   ; ShapeCount   ++; }

        if (Grid.Ball[x][y].DvPitchx  >Grid.MaxDvPitchx  ) { Grid.MaxDvPitchx   = Grid.Ball[x][y].DvPitchx  ; }
        if (Grid.Ball[x][y].DvPitchy  >Grid.MaxDvPitchy  ) { Grid.MaxDvPitchy   = Grid.Ball[x][y].DvPitchy  ; }
        if (Grid.Ball[x][y].DvPx      >Grid.MaxDvPx      ) { Grid.MaxDvPx       = Grid.Ball[x][y].DvPx      ; }
        if (Grid.Ball[x][y].DvPy      >Grid.MaxDvPy      ) { Grid.MaxDvPy       = Grid.Ball[x][y].DvPy      ; }
        if (Grid.Ball[x][y].DvDiameter>Grid.MaxDvDiameter) { Grid.MaxDvDiameter = Grid.Ball[x][y].DvDiameter; }
        if (Grid.Ball[x][y].DvShape   >Grid.MaxDvShape   ) { Grid.MaxDvShape    = Grid.Ball[x][y].DvShape   ; }

        if (Grid.Ball[x][y].DvPitchx  <Grid.MinDvPitchx  ) { Grid.MinDvPitchx   = Grid.Ball[x][y].DvPitchx  ; }
        if (Grid.Ball[x][y].DvPitchy  <Grid.MinDvPitchy  ) { Grid.MinDvPitchy   = Grid.Ball[x][y].DvPitchy  ; }
        if (Grid.Ball[x][y].DvPx      <Grid.MinDvPx      ) { Grid.MinDvPx       = Grid.Ball[x][y].DvPx      ; }
        if (Grid.Ball[x][y].DvPy      <Grid.MinDvPy      ) { Grid.MinDvPy       = Grid.Ball[x][y].DvPy      ; }
        if (Grid.Ball[x][y].DvDiameter<Grid.MinDvDiameter) { Grid.MinDvDiameter = Grid.Ball[x][y].DvDiameter; }
        if (Grid.Ball[x][y].DvShape   <Grid.MinDvShape   ) { Grid.MinDvShape    = Grid.Ball[x][y].DvShape   ; }
    }}

    if (PitchCountx  ) Grid.AvgDvPitchx   = SumPitchx  /PitchCountx  ;
    if (PitchCounty  ) Grid.AvgDvPitchy   = SumPitchy  /PitchCounty  ;
    if (PxCount      ) Grid.AvgDvPx       = SumPx      /PxCount      ;
    if (PyCount      ) Grid.AvgDvPy       = SumPy      /PyCount      ;
    if (DiameterCount) Grid.AvgDvDiameter = SumDiameter/DiameterCount;
    if (ShapeCount   ) Grid.AvgDvShape    = SumShape   /ShapeCount   ;
    // END : Calculate Deviation
    //--------------------------------------------------------------------------

    Grid.IsPass = true;
    for (register int y=0; y<Grid.RowCount; y++) {
    for (register int x=0; x<Grid.ColCount; x++) {
        Grid.Ball[x][y].IsPass = true;
    }}

    for (register int y=0; y<Grid.RowCount; y++) {
    for (register int x=0; x<Grid.ColCount; x++) {
        if (!Grid.Ball[x][y].IsTrExist) continue;

        double DvPx       = Grid.Ball[x][y].DvPx      ;
        double DvPy       = Grid.Ball[x][y].DvPy      ;
        double DvPitchx   = Grid.Ball[x][y].DvPitchx  ;
        double DvPitchy   = Grid.Ball[x][y].DvPitchy  ;
        double DvDiameter = Grid.Ball[x][y].DvDiameter;
        double CuShape    = Grid.Ball[x][y].CuShape   ;

        bool IsTrExist = Grid.Ball[x][y].IsTrExist;
        bool IsCuExist = Grid.Ball[x][y].IsCuExist;

        if (Spec.OpPx      && fabs(DvPx      )>Spec.TPx      ) { Grid.IsPass = false; Grid.Ball[x][y].IsPass = false; BinId=bgaPx      ; EMessage.Format(L"PX       R:%d, C:%d", y, x);}
        if (Spec.OpPy      && fabs(DvPy      )>Spec.TPy      ) { Grid.IsPass = false; Grid.Ball[x][y].IsPass = false; BinId=bgaPy      ; EMessage.Format(L"PY       R:%d, C:%d", y, x);}
        if (Spec.OpPitchx  && fabs(DvPitchx  )>Spec.TPitchx  ) { Grid.IsPass = false; Grid.Ball[x][y].IsPass = false; BinId=bgaPitch   ; EMessage.Format(L"PITCHX   R:%d, C:%d", y, x);}
        if (Spec.OpPitchy  && fabs(DvPitchy  )>Spec.TPitchy  ) { Grid.IsPass = false; Grid.Ball[x][y].IsPass = false; BinId=bgaPitch   ; EMessage.Format(L"PITCHY   R:%d, C:%d", y, x);}
        if (Spec.OpDiameter&& fabs(DvDiameter)>Spec.TDiameter) {
            Grid.IsPass = false;
            Grid.Ball[x][y].IsPass = false;
            BinId=bgaDiameter; EMessage.Format(L"DIAMETER R:%d, C:%d", y, x);
        }
        if (Spec.OpShape   && fabs(CuShape   )<Spec.TShape   ) { Grid.IsPass = false; Grid.Ball[x][y].IsPass = false; BinId=bgaShape   ; EMessage.Format(L"SHAPE    R:%d, C:%d", y, x);}
    }}

    return Grid.IsPass;
}

bool CBga::OnFindMap()
{
    for (register int y=0; y<Grid.RowCount; y++) {
    for (register int x=0; x<Grid.ColCount; x++) {
        Grid.Ball[x][y].IsTrExist  = false;
    }}

    Grid.IRoi = IpRect;

    for (int k=0; k<BallCount; k++) {
        TPoint RC = Grid.GetGridRowColWithoutTrExist(Boundary.Blob[k].Rect.Point().x, Boundary.Blob[k].Rect.Point().y);

        int x = RC.x;
        int y = RC.y;

        if (RC.x<0 || RC.y<0) continue;

        Grid.Ball[RC.x][RC.y].IsTrExist = true;
    }

    return true;
}

bool CBga::OnTrain()
{
    int IIndex = 0;

    if (IIndex==0) {
        IIndex=10;
    }

	if (GImage==NULL         ) { AfxMessageBox(_T("Check CBga Train GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CBga Train GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CBga Train GImage Height Size"    )); return false; }

	IAddress    = GImage->GetImagePtr(); //Image Buffer Pointer
	BufferWidth = GImage->GetWidth()   ; //Image Buffer Width 

	IpRect.l = IRoi->GetOrgX();
	IpRect.t = IRoi->GetOrgY();
	IpRect.r = IpRect.l+IRoi->GetWidth();
	IpRect.b = IpRect.t+IRoi->GetHeight();

    switch (IIndex) {
        case 10 :

        /*
            for (register int y=0; y<Grid.RowCount; y++) {
            for (register int x=0; x<Grid.ColCount; x++) {
                Grid.Ball[x][y].IsPass     = false;
                Grid.Ball[x][y].IsUnknown  = false;
                Grid.Ball[x][y].IsMissing  = false;
                Grid.Ball[x][y].IsCuExist  = false;
                Grid.Ball[x][y].IsTrExist  = false;
            }}

            Grid.OnClear2D();
        */
            IIndex++;

        case 11 :
			Boundary.GImage   = GImage  ;
//             Boundary.IAddress = IAddress;
            Boundary.IRoi     = IRoi;
            Boundary.IWidth   = IWidth;
            Boundary.IHeight  = IHeight;

            // 20160707
            if(Spec.OpSideBright){
                Boundary.TrPa.LoThHold = BlobThHold;
                Boundary.TrPa.HiThHold = 0         ;
            }
            else {
                Boundary.TrPa.LoThHold = 0         ;
                Boundary.TrPa.HiThHold = BlobThHold;
            }
            Boundary.TrPa.IgSort = false;
            Boundary.OnExecute();
            IIndex++;

        case 12 :
            if (Boundary.BlobCount<1) {
                BinId = bgaMissing;
                AfxMessageBox(_T("BALL FINDING FAIL"));
                return 0;
            }
            IIndex++;

        case 13 :
            IIndex++;

        case 14 :
            for (int k=0; k <Boundary.BlobCount; k++) {
                Position[k].Px = Boundary.Blob[k].Rect.Point().x-Boundary.Blob[0].Rect.Point().x;
                Position[k].Py = Boundary.Blob[k].Rect.Point().y-Boundary.Blob[0].Rect.Point().y;
                Position[k].IsPass = true;
            }
            return IsPass;

            IIndex++;

        case 15 :
            //if (!OnFindMap()) {
            //    ShowMessage("BALL FINDING FAIL");
            //    return false;
            //}
            IIndex++;

        case 16 :
            IIndex=0;
            AfxMessageBox(_T("BALL FINDING SUCCESS"));
            return true;
    }

    return false;
}

int CBga::OnExecute()
{
    int IIndex = 0;

    if (IIndex==0) {
        IIndex=10;
    }

	if (GImage==NULL         ) { AfxMessageBox(L"Check CBga Train GImage Memory Allocate"); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(L"Check CBga Train GImage Width Size"     ); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(L"Check CBga Train GImage Height Size"    ); return false; }

	// Class Seed(최상위 클래스)
	IAddress    = GImage->GetImagePtr(); //Image Buffer Pointer
	BufferWidth = GImage->GetWidth()   ; //Image Buffer Width 

	IpRect.l = IRoi->GetOrgX();
	IpRect.t = IRoi->GetOrgY();
	IpRect.r = IpRect.l+IRoi->GetWidth();
	IpRect.b = IpRect.t+IRoi->GetHeight();

    BinId = bgaPass;

    switch (IIndex) {
        case 10 :
            IsPass = false;

            Grid.OnClear2D();
            OnClearData(); // Ball_Quality data Init 
            IIndex++;

        case 11 :

            //OnTrain();
            IIndex++;

        case 12 :
            Grid.BallCount = Spec.BallCount;
            IIndex++;

        case 13 :
			Boundary.GImage   = GImage  ;
			//             Boundary.IAddress = IAddress;
			Boundary.IRoi     = IRoi;
			Boundary.IWidth   = IWidth;
			Boundary.IHeight  = IHeight;

            // 20160707
            if(Spec.OpSideBright){
                Boundary.TrPa.LoThHold = BlobThHold;
                Boundary.TrPa.HiThHold = 0         ;
            }
            else {
                Boundary.TrPa.LoThHold = 0         ;
                Boundary.TrPa.HiThHold = BlobThHold;
            }
            Boundary.TrPa.IgSort = false;
            Boundary.OnExecute();
            IIndex++;

        case 14 :
            BallCount = Boundary.BlobCount;      // UnKnown, MissingBall Inspection
            if (BallCount<1             ) { BinId = bgaMissing; return false; }
            if (BallCount>Grid.BallCount) { BinId = bgaUnKnown; return false; }
            if (BallCount<Grid.BallCount) { BinId = bgaMissing; return false; }
            IIndex++;

        case 15 :
            OnCoordinate();
            IIndex++;

        case 16 :
            if (Spec.OpPosition) { // 포지션 검사
                if (!OnPosition()) return false;
            }
            IIndex++;

        case 17 :
            if (Spec.OpInRing || Spec.OpOutRing) {// 퀄리티 검사
                if (!OnRingQuality()) return false;
            }
            IIndex++;

        case 18 :
            OpUseMatrixFitting = true; // Px, Py 검사
            if (!OnMeasuring()) return false;
            IIndex++;

        case 19 :
            IsPass = true;
            IIndex=0;
            break;
    }

    return IsPass;
}

void CBga::OnPaint(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;

    Boundary.OnPaintBlobRect(Tool); // Ball Rect Display

    Grid.OpIpArea=OpIpArea; Grid.CoIpArea=CoIpArea;
    Grid.OpIgArea=OpIgArea; Grid.CoIgArea=CoIgArea;
    Grid.OpRsArea=OpRsArea; Grid.CoRsArea=CoRsArea;
    Grid.OpRsAnls=OpRsAnls; Grid.CoRsAnls=CoRsAnls;

    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;
    Gdi->CPen.Color = Tool.Color;

    Gdi->CText.Color = YELLOW_COLOR;
    TPoint C; C.x =0; C.y =0;
    int Radius=0;

    if (OpIpArea) {
        IpRect.OnPaint(Tool);
    }
    if (OpIgArea) {
        Boundary.OnPaintIgBlob(Tool);
        Boundary.OnPaintBlobId(Tool);
    }
    if (OpRsArea) {
        Gdi->CPen.Color = CoRsArea;
        Gdi->CBrush.Style = BS_HOLLOW;

        bool IsQuality = true;
        CString Str =  L"[FAIL] QUALITY : ";
        Gdi->CText.Color   = RED_COLOR;
        Gdi->CPen.Color    = RED_COLOR;
        Gdi->CBrush.Color  = RED_COLOR;

        for (int k=0; k<BallCount; k++) {
            C.x    = Boundary.Blob[k].Rect.Point().x+1.0      ;
            C.y    = Boundary.Blob[k].Rect.Point().y+1.0      ;
            CString Msg ="";

            if(Quality[k].BinId==bgaInRingQuality){
                Msg.Format(L"I_RING: %4.3f",(Quality[k].InRingScore)) ;
                Gdi->OnText(C.x+Radius, C.y+Radius-10, LPSTR(LPCTSTR(Msg)));
            }
            if(Quality[k].BinId==bgaOutRingQuality){
				Msg.Format(L"O_RING: %4.3f",(Quality[k].OutRingScore)) ;
                Gdi->OnText(C.x+Radius, C.y+Radius-10, LPSTR(LPCTSTR(Msg)));
             }
        }

        if (BinId==bgaMissing      ) Gdi->OnText(IpRect.l, IpRect.t-15, LPSTR(LPCTSTR(EMessage)));
        if (BinId==bgaUnKnown      ) Gdi->OnText(IpRect.l, IpRect.t-15, L"UnKnown Ball");
        else if (!IsQuality){
            Str = Str + EMessage;
            Gdi->OnText(IpRect.l, IpRect.t-15, LPSTR(LPCTSTR(Str)));
        }
        else {
            bool LtStr = false;
            //Grid.OnPaint(PTable);
            for (int y=0; y<Grid.RowCount; y++) {
            for (int x=0; x<Grid.ColCount; x++) {
                if (!Grid.Ball[x][y].IsTrExist) continue;
                TPoint C;
                C.x = (int)(Grid.Ball[x][y].CuPx);
                C.y = (int)(Grid.Ball[x][y].CuPy);

                if (!LtStr) {
                    LtStr = true;
                    Gdi->CText.Color = Grid.IsPass ? CoRsArea : RED_COLOR;
                    CString Str = Grid.IsPass ? L"[BALL] PASS" : L"[FAIL] MEASUREMENT : ";
                    if (!Grid.IsPass) Str = Str + EMessage;
                    Gdi->OnText(IpRect.l, IpRect.t-10, CW2A(Str));
                }

                Gdi->CPen.Color   = Grid.Ball[x][y].IsPass ? CoRsArea : RED_COLOR;
                Gdi->CBrush.Color = Grid.Ball[x][y].IsPass ? CoRsArea : RED_COLOR;
                Gdi->CText.Color  = Grid.Ball[x][y].IsPass ? CoRsArea : RED_COLOR;
                if (!Grid.Ball[x][y].IsPass) {
                    TPoint __Point(FPattern.Obj[x][y].x+0.5, FPattern.Obj[x][y].y+0.5);
                    Gdi->OnCircle(0, __Point, 30, 10);
                }
            }}
        }
    }

    if (OpRsAnls) {
        float DwMg = 0.5;
        bool OpDwColStdPoint   = false;
        bool OpDwRowStdPoint   = false;
        bool OpDwPatternPoint  = false;
        bool OpDwGrid          = false;
        bool OpDwPatternLine   = true ;
        bool OpDwPatternCircle = false;
        bool OpDwRingQuality   = false;
        bool OpDwPosition      = true ;
        bool OpDwBlob          = false;

        if (OpDwBlob) {
            if (!Grid.IsPass) OpDwPatternLine = false;
            Gdi->CPen.Color    = BLUE_COLOR;
            Gdi->CBrush.Color  = BLUE_COLOR;

            for (int k=0; k<BallCount; k++) {
                if (Quality[k].BinId!=0)continue;

                C.x = Boundary.Blob[k].Rect.Point().x+1.0      ;
                C.y = Boundary.Blob[k].Rect.Point().y+1.0      ;
                Radius = Boundary.Blob[k].Radius+1.0;
                Gdi->OnCircle(0,C ,Radius, Radius);
            }
        }

        if (OpUseMatrixFitting) {
            if (OpDwColStdPoint) {
                for (int n=0; n<FPattern.ColCount; n++) {
                    Gdi->CPen.Color = RED_COLOR;
                    TPoint P0(FPattern.Col[n].P0.x+DwMg, FPattern.Col[n].P0.y+DwMg);
                    TPoint P1(FPattern.Col[n].P1.x+DwMg, FPattern.Col[n].P1.y+DwMg);
                    Gdi->OnCircle(0, P0, 5, 10);
                    Gdi->OnCircle(0, P1, 5, 10);
                }
            }

            if (OpDwRowStdPoint) {
                 for (int n=0; n<FPattern.RowCount; n++) {
                     Gdi->CPen.Color = BLUE_COLOR;
                     TPoint P0(FPattern.Row[n].P0.x+DwMg, FPattern.Row[n].P0.y+DwMg);
                     TPoint P1(FPattern.Row[n].P1.x+DwMg, FPattern.Row[n].P1.y+DwMg);
                     Gdi->OnCircle(0, P0, 5, 10);
                     Gdi->OnCircle(0, P1, 5, 10);
                 }
            }
            if (OpDwPatternPoint) {
                Gdi->CPen.Color = BLUE_COLOR;
                for (int y=0; y<FPattern.RowCount; y++) {
                for (int x=0; x<FPattern.ColCount; x++) {
                    TPoint Point(FPattern.Pos[x][y].x+DwMg, FPattern.Pos[x][y].y+DwMg);
                    Gdi->OnCircle(0, Point, 4, 10);
                }}
            }
            if (OpDwGrid) {
                Grid.OnPaint(Tool, true);
            }

            if (OpDwPatternLine) {
                Gdi->CPen.Color = BLUE_COLOR;
                for (int n=0; n<FPattern.ColCount; n++) {
                    TPoint PX1(FPattern.Pos[n][0                  ].x+DwMg, FPattern.Pos[n][0                  ].y+DwMg);
                    TPoint PX2(FPattern.Pos[n][FPattern.RowCount-1].x+DwMg, FPattern.Pos[n][FPattern.RowCount-1].y+DwMg);
                    Gdi->OnLine(PX1, PX2);
                }

                for (int n=0; n<FPattern.RowCount; n++) {
                    TPoint PX1(FPattern.Pos[0                  ][n].x+DwMg, FPattern.Pos[0                  ][n].y+DwMg);
                    TPoint PX2(FPattern.Pos[FPattern.ColCount-1][n].x+DwMg, FPattern.Pos[FPattern.ColCount-1][n].y+DwMg);
                    Gdi->OnLine(PX1, PX2);
                }
            }

            if (OpDwPatternCircle) {
                Gdi->CPen.Color = BLUE_COLOR;
                for (int y=0; y<FPattern.RowCount; y++) {
                for (int x=0; x<FPattern.ColCount; x++) {
                    TPoint Point(FPattern.Pos[x][y].x+DwMg, FPattern.Pos[x][y].y+DwMg);
                    Gdi->OnCircle(0, Point, Grid.AvgCuDiameter/2.0, 1);
                }}
            }
        }

        if (Spec.OpPosition && OpDwPosition) {
            for (int k=0; k<BallCount; k++) {
                Gdi->CPen.Color = Position[k].IsPass ? Tool.Color : RED_COLOR;

                C.x = Boundary.Blob[k].Rect.Point().x+1.0;
                C.y = Boundary.Blob[k].Rect.Point().y+1.0;
                Radius = Boundary.Blob[k].Radius+1.0;
                Gdi->OnCircle(0,C ,Radius, Radius);

                if (!Position[k].IsPass) {
                    CString Str="";
                    Str.Format(L"%3.2f Px", Position[k].Do);
                    Gdi->OnText(Boundary.Blob[k].Rect.r, Boundary.Blob[k].Rect.t, LPSTR(LPCTSTR(Str)));
                }
            }
        }

        if (OpDwRingQuality) {
            OnPaintRingQuality(Tool);
        }
    }

    delete Gdi;
}

void CBga::OnPaintRingQuality(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

    bool IsQualityPaint = true;

    for (int k=0; k<Boundary.BlobCount; k++) {
        float x  = Boundary.Blob[k].Rect.Point    ().x + 0.5 + Quality[k].MvCx;
        float y  = Boundary.Blob[k].Rect.Point    ().y + 0.5 + Quality[k].MvCy;
        float w  = Boundary.Blob[k].Rect.GetWidth ()   + 0.5;
        float h  = Boundary.Blob[k].Rect.GetHeight()   + 0.5;
        float fw = Boundary.Blob[k].Radius*2 + 0.5;
        float fh = Boundary.Blob[k].Radius*2 + 0.5;
        //int DiaMean = SapBlob[k].FretDimtrMean + 0.5;
        int DiaMean = Boundary.Blob[k].Radius   *2+0.5;
        int DiaMax  = Boundary.Blob[k].MaxRadius*2 + 0.5;
        int DiaMin  = Boundary.Blob[k].MinRadius*2 + 0.5;

        TPoint C(x, y);

        int InDiameter  = DiaMean/2.0-InRingOffset;
        int OutDiameter = DiaMean/2.0+OutRingOffset;

        Gdi->CPen.Color   = AQUA;
        Gdi->CBrush.Color = AQUA;
        Gdi->CText.Color  = AQUA;

        Gdi->OnCircle(0, C, DiaMean/2.0, DiaMean/2.0);

        Gdi->CPen.Color   = Quality[k].BinId==bgaPass ? BLUE_COLOR : RED_COLOR;
        Gdi->CBrush.Color = Quality[k].BinId==bgaPass ? BLUE_COLOR : RED_COLOR;
        Gdi->CText.Color  = Quality[k].BinId==bgaPass ? BLUE_COLOR : RED_COLOR;
        if (Quality[k].BinId==bgaInRingQuality) Gdi->OnText(Boundary.Blob[k].Rect.l-5, Boundary.Blob[k].Rect.t-8,"IRING");
        Gdi->OnCircle(0, C, InDiameter , InDiameter );

        Gdi->CPen.Color   = Quality[k].BinId==bgaPass ? BLUE_COLOR : RED_COLOR;
        Gdi->CBrush.Color = Quality[k].BinId==bgaPass ? BLUE_COLOR : RED_COLOR;
        Gdi->CText.Color  = Quality[k].BinId==bgaPass ? BLUE_COLOR : RED_COLOR;
        if (Quality[k].BinId==bgaOutRingQuality) Gdi->OnText(Boundary.Blob[k].Rect.l-5, Boundary.Blob[k].Rect.t-3,"OUTRING");
        Gdi->OnCircle(0, C, OutDiameter, OutDiameter);
    }

    Gdi->CText.Color = YELLOW_COLOR;
    enum TGraphic {GrOutputEllipse, GrOutputBBox, GrOutputOutsidePerimeter, GrOutputConvexPerimeter};
    TGraphic Graphic = GrOutputEllipse;

    delete Gdi;
}

/*
void CBga::OnUpdateTableResult(TStringGrid *Table, float PxResolution)
{
    if (Table==NULL) return;

    float PR = PxResolution;

    if (PR==0) PR=1;

    Table->RowCount = Grid.BallCount+4;
    Table->ColCount = 15;

    Table->Cells[ 0][0] = "NO";
    Table->Cells[ 1][0] = "좌표";
    Table->Cells[ 2][0] = "CuPx      ";
    Table->Cells[ 3][0] = "CuPy      ";
    Table->Cells[ 4][0] = "CuPitchx  ";
    Table->Cells[ 5][0] = "CuPitchy  ";
    Table->Cells[ 6][0] = "CuDiameter";
    Table->Cells[ 7][0] = "DvPx      ";
    Table->Cells[ 8][0] = "DvPy      ";
    Table->Cells[ 9][0] = "DvPitchx  ";
    Table->Cells[10][0] = "DvPitchy  ";
    Table->Cells[11][0] = "DvDiameter";
    Table->Cells[12][0] = "DvShape   ";
    Table->Cells[13][0] = "In Ring   ";
    Table->Cells[14][0] = "Out Ring  ";

    double value = 0;

    int cnt = 0;
    for (int y=0; y<Grid.RowCount; y++) {
    for (int x=0; x<Grid.ColCount; x++) {
        if (!Grid.Ball[x][y].IsTrExist) continue;

        cnt++;
                                               Table->Cells[ 0][cnt] = AnsiString().sprintf("B # %2d", cnt);
                                               Table->Cells[ 1][cnt] = AnsiString().sprintf("R%2d, C%2d", y, x);
        value = Grid.Ball[x][y].CuPx      *PR; Table->Cells[ 2][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Grid.Ball[x][y].CuPy      *PR; Table->Cells[ 3][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Grid.Ball[x][y].CuPitchx  *PR; Table->Cells[ 4][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Grid.Ball[x][y].CuPitchy  *PR; Table->Cells[ 5][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Grid.Ball[x][y].CuDiameter*PR; Table->Cells[ 6][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Grid.Ball[x][y].DvPx      *PR; Table->Cells[ 7][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Grid.Ball[x][y].DvPy      *PR; Table->Cells[ 8][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Grid.Ball[x][y].DvPitchx  *PR; Table->Cells[ 9][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Grid.Ball[x][y].DvPitchy  *PR; Table->Cells[10][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Grid.Ball[x][y].DvDiameter*PR; Table->Cells[11][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Grid.Ball[x][y].CuShape      ; Table->Cells[12][cnt] = AnsiString().sprintf("%3.1f %", value);
        value = Quality  [cnt-1].InRingScore ; Table->Cells[13][cnt] = AnsiString().FormatFloat("0.000", value);
        value = Quality  [cnt-1].OutRingScore; Table->Cells[14][cnt] = AnsiString().FormatFloat("0.000", value);
        //value = Quality  [cnt-1].OutRingScore; Table->Cells[15][cnt] = AnsiString().FormatFloat("0.000", value);
    }}


    cnt++;
    Table->Cells[0][cnt] = "MIN";
    for (int j=1; j<Table->ColCount; j++) {
        double value = 0;

             if (j== 2) { value = Grid.MinCuPx      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 3) { value = Grid.MinCuPy      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 4) { value = Grid.MinCuPitchx  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 5) { value = Grid.MinCuPitchy  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 6) { value = Grid.MinCuDiameter*PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 7) { value = Grid.MinDvPx      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 8) { value = Grid.MinDvPy      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 9) { value = Grid.MinDvPitchx  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j==10) { value = Grid.MinDvPitchy  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j==11) { value = Grid.MinDvDiameter*PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j==12) { value = Grid.MinCuShape      ; Table->Cells[j][cnt] = AnsiString().sprintf("%3.1f %", value); }
    }

    cnt++;
    Table->Cells[0][cnt] = "MAX";
    for (int j=1; j<Table->ColCount; j++) {
        double value = 0;

             if (j== 2) { value = Grid.MaxCuPx      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 3) { value = Grid.MaxCuPy      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 4) { value = Grid.MaxCuPitchx  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 5) { value = Grid.MaxCuPitchy  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 6) { value = Grid.MaxCuDiameter*PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 7) { value = Grid.MaxDvPx      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 8) { value = Grid.MaxDvPy      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 9) { value = Grid.MaxDvPitchx  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j==10) { value = Grid.MaxDvPitchy  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j==11) { value = Grid.MaxDvDiameter*PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j==12) { value = Grid.MaxCuShape      ; Table->Cells[j][cnt] = AnsiString().sprintf("%3.1f %", value); }
    }

    cnt++;
    Table->Cells[0][cnt] = "AVG";
    for (int j=1; j<Table->ColCount; j++) {
        double value = 0;

             if (j== 2) { value = Grid.AvgCuPx      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 3) { value = Grid.AvgCuPy      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 4) { value = Grid.AvgCuPitchx  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 5) { value = Grid.AvgCuPitchy  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 6) { value = Grid.AvgCuDiameter*PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 7) { value = Grid.AvgDvPx      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 8) { value = Grid.AvgDvPy      *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j== 9) { value = Grid.AvgDvPitchx  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j==10) { value = Grid.AvgDvPitchy  *PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j==11) { value = Grid.AvgDvDiameter*PR; Table->Cells[j][cnt] = AnsiString().FormatFloat("0.000", value); }
        else if (j==12) { value = Grid.AvgCuShape      ; Table->Cells[j][cnt] = AnsiString().sprintf("%3.1f %", value); }
    }
}
*/
void CBga::OnLoad(bool IsLoad, CString Folder)
{
	CIFile Ini;

    if (Folder.Trim()=="") return;
    CString Path = Folder + "\\Bga.ini";
    if (Path=="") return;

    Grid.OnLoad(IsLoad, Folder);

	Ini.Load(IsLoad, Path, L"Parameter", L"OpMakeRC", &OpMakeRC);
	Ini.Load(IsLoad, Path, L"Parameter", L"OpFastFindBlob", &OpFastFindBlob);

	Ini.Load(IsLoad, Path, L"Spec", L"OpUnknown", &Spec.OpUnknown);
	Ini.Load(IsLoad, Path, L"Spec", L"OpMissing", &Spec.OpMissing);
	Ini.Load(IsLoad, Path, L"Spec", L"OpInRing", &Spec.OpInRing);
	Ini.Load(IsLoad, Path, L"Spec", L"OpOutRing", &Spec.OpOutRing);
	Ini.Load(IsLoad, Path, L"Spec", L"OpPosition", &Spec.OpPosition);

    Ini.Load(IsLoad, Path, L"Spec", L"OpPx        ", &Spec.OpPx        );
    Ini.Load(IsLoad, Path, L"Spec", L"OpPy        ", &Spec.OpPy        );
    Ini.Load(IsLoad, Path, L"Spec", L"OpPitchx    ", &Spec.OpPitchx    );
    Ini.Load(IsLoad, Path, L"Spec", L"OpPitchy    ", &Spec.OpPitchy    );
    Ini.Load(IsLoad, Path, L"Spec", L"OpDiametIer ", &Spec.OpDiameter  );
    Ini.Load(IsLoad, Path, L"Spec", L"OpShape     ", &Spec.OpShape     );
    Ini.Load(IsLoad, Path, L"Spec", L"OpSideBright", &Spec.OpSideBright);

	Ini.Load(IsLoad, Path, L"Spec", L"BallCount   ", &Spec.BallCount);
	Ini.Load(IsLoad, Path, L"Spec", L"IgnoreOffset", &Spec.IgnoreOffset);
	Ini.Load(IsLoad, Path, L"Spec", L"Px       ", &Spec.Px);
	Ini.Load(IsLoad, Path, L"Spec", L"Py       ", &Spec.Py);
	Ini.Load(IsLoad, Path, L"Spec", L"Pitchx   ", &Spec.Pitchx);
	Ini.Load(IsLoad, Path, L"Spec", L"Pitchy   ", &Spec.Pitchy);
	Ini.Load(IsLoad, Path, L"Spec", L"Diameter ", &Spec.Diameter);
	Ini.Load(IsLoad, Path, L"Spec", L"Shape    ", &Spec.Shape);

    Ini.Load(IsLoad, Path, L"Spec", L"TPx          ", &Spec.TPx          );
    Ini.Load(IsLoad, Path, L"Spec", L"TPy          ", &Spec.TPy          );
    Ini.Load(IsLoad, Path, L"Spec", L"TPitchx      ", &Spec.TPitchx      );
    Ini.Load(IsLoad, Path, L"Spec", L"TPitchy      ", &Spec.TPitchy      );
    Ini.Load(IsLoad, Path, L"Spec", L"TDiameter    ", &Spec.TDiameter    );
    Ini.Load(IsLoad, Path, L"Spec", L"TShape       ", &Spec.TShape       );
    Ini.Load(IsLoad, Path, L"Spec", L"TPosition    ", &Spec.TPosition    );
    Ini.Load(IsLoad, Path, L"Spec", L"TInRingScore ", &Spec.TInRingScore );
    Ini.Load(IsLoad, Path, L"Spec", L"TOutRingScore", &Spec.TOutRingScore);

    Ini.Load(IsLoad, Path, L"Spec", L"BlobThHold   ", &BlobThHold );
    Ini.Load(IsLoad, Path, L"Spec", L"InRingOffset ", &InRingOffset );
    Ini.Load(IsLoad, Path, L"Spec", L"OutRingOffset", &OutRingOffset);
    Ini.Load(IsLoad, Path, L"Spec", L"InRingThHold ", &InRingThHold );
    Ini.Load(IsLoad, Path, L"Spec", L"OutRingThHold", &OutRingThHold);

    Ini.Load(IsLoad, Path, L"Spec", L"OpUseMatrixFitting", &OpUseMatrixFitting);
    Ini.Load(IsLoad, Path, L"Spec", L"OpUseMapCoordinate", &OpUseMapCoordinate);
    Ini.Load(IsLoad, Path, L"Spec", L"OpUseLineIntersect", &OpUseLineIntersect);

    Ini.Load(IsLoad, Path, L"TrPa", L"MinHorLength ",  &Boundary.TrPa.MinHorLength  );
    Ini.Load(IsLoad, Path, L"TrPa", L"MaxHorLength ",  &Boundary.TrPa.MaxHorLength  );
    Ini.Load(IsLoad, Path, L"TrPa", L"MinVerLength ",  &Boundary.TrPa.MinVerLength  );
    Ini.Load(IsLoad, Path, L"TrPa", L"MaxVerLength ",  &Boundary.TrPa.MaxVerLength  );


    Ini.Load(IsLoad, Path, L"Grid", L"BallCount",  &Grid.BallCount);
    Ini.Load(IsLoad, Path, L"Grid", L"RowCount",  &Grid.RowCount);
    Ini.Load(IsLoad, Path, L"Grid", L"ColCount",  &Grid.ColCount);
    Ini.Load(IsLoad, Path, L"Grid", L"RowPitch",  &Grid.RowPitch);
    Ini.Load(IsLoad, Path, L"Grid", L"ColPitch",  &Grid.ColPitch);

	CString Data="";

    for (int y=0; y<Grid.RowCount; y++) {
    for (int x=0; x<Grid.ColCount; x++) {
		Data.Format(L"X:%d , Y: %d",x,y);
        Ini.Load(IsLoad, Path, L"Grid Map", Data , &Grid.Ball[x][y].IsTrExist);
    }}

    for (int k=0; k<MAX_BGA; k++) {
        Data.Format(L"PX:%d ",k);
        Ini.Load(IsLoad, Path, L"POSITION", Data, &Position[k].Px);
		Data.Format(L"PY:%d ",k);
        Ini.Load(IsLoad, Path, L"POSITION", Data, &Position[k].Py);
    }
}
