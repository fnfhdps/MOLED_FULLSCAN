//---------------------------------------------------------------------------

#include "stdafx.h"
#include "LxPrint.h"

//---------------------------------------------------------------------------

//#pragma package(smart_init)


CPrint::CPrint()
{
    Property.OpOverPrinted = true;
    Property.OpUnderPrinted = true;
    Property.OverPrintedX = 2;
    Property.OverPrintedY = 2;
    Property.UnderPrintedX = 2;
    Property.UnderPrintedY = 2;
    Property.xBleed = 2;
    Property.yBleed = 2;
    Property.DvThHold = 10;
    Property.IpThHold = 30;
    Property.TrThHold = 30;
    Property.OpGabAngle  = true;
    Property.OpGabLeft   = true;
    Property.OpGabTop    = true;
    Property.OpGabRight  = true;
    Property.OpGabBottom = true;
    Property.GabAngle  = 3;
    Property.GabLeft   = 10;
    Property.GabTop    = 10;
    Property.GabRight  = 10;
    Property.GabBottom = 10;
    Property.ShRectScore = 0.3;
    Property.ShBlobScore = 0.3;
    Property.TrMargin = 3;
    Property.ShMargin = 10;
    Property.OpStringCount = false;

    FindAreaCount = 0;
    FindFontCount = 0;
    FindDefectCount = 0;
    OverPrintPixelCount = 0;
    UnderPrintPixelCount = 0;

    DwAreaFind = false;
    DwReferString = true;
    DwFont = true;
    DwScore = false;
    DwOverPrintPixel = false;
    DwUnderPrintPixel = false;
    DwAngle = false;
    DwIRoi = true;
    ResultAngle = 0.0f;
    Mode = 0;

	//CFilter 상속 
	GImage = NULL;
	IRoi   = new EROIBW8();
	TRoi   = new EROIBW8();
}

CPrint::~CPrint()
{
}

void CPrint::OnUpdate()
{
}

CString CPrint::GetOcvName()
{
    if      (BinId==ocvPass       ) return "MARKING PASS";
    else if (BinId==ocvStringCount) return "검사문자열개수불일치";
    else if (BinId==ocvAreaMatch  ) return "폰트전체매칭";
    else if (BinId==ocvFontMatch  ) return "폰트개별매칭";
    else if (BinId==ocvOverPrint  ) return "오버프린팅";
    else if (BinId==ocvUnderPrint ) return "언더프린팅";
    else if (BinId==ocvAngle      ) return "각도";
    else if (BinId==ocvGabLeft    ) return "좌측간격";
    else if (BinId==ocvGabTop     ) return "상단간격";
    else if (BinId==ocvGabRight   ) return "우측간격";
    else if (BinId==ocvGabBottom  ) return "하단간격";

	return CString();
}

int CPrint::OnTrain(CString ReferStr)
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CPrint Train GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CPrint Train GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CPrint Train GImage Height Size"    )); return false; }

    Property.ReferString = ReferStr;

    int l=0, t=0, r=0, b=0;
    int TrIndex = 10;
    int w = 0;
    int h = 0;

    switch (TrIndex) {
        default : return 0;

        case 10 : //개별 Character Blob Search 및 Merge하여 전체 ROI영역 넘겨줌 
//                   w = Image.GetWidth();
//                   h = Image.GetHeight();

				  w = GImage->GetWidth() ;
				  h = GImage->GetHeight();

                  Train.Image.SetSize(w, h); //OverPrint or UnderPrint 검사시 필요한 Train 원본 이미지 
                  memcpy(Train.Image.GetImagePtr(), GImage->GetImagePtr(), w*h);

                  Trace.GImage    = GImage;
                  Trace.IRoi      = TRoi  ;
                  Trace.BlobCount = 0     ;

                  Trace.OnExecute();
                  if (Trace.BlobCount>=MAX_PRINT_FONT) {
                      TrIndex = 0;
                      return 0;
                  }

                  if (Trace.BlobCount<1) {
                      TrIndex = 0;
                      return 0;
                  }

                  if (Property.OpStringCount) {
                      if (Trace.BlobCount!=ReferStr.Trim()) {
                          //ShowMessage("Check Blob Count");
                          TrIndex = 0;
                          return 0;
                      }
                  }
                  Train.Count = Trace.BlobCount;

                  Train.AreaRect.l = 10000;
                  Train.AreaRect.t = 10000;
                  Train.AreaRect.r = 0;
                  Train.AreaRect.b = 0;

                  for (int i=0; i<Trace.BlobCount; i++) {
					  Train.AreaRect.l = Trace.Blob[i].Rect.l<Train.AreaRect.l ? Trace.Blob[i].Rect.l : Train.AreaRect.l;
					  Train.AreaRect.t = Trace.Blob[i].Rect.t<Train.AreaRect.t ? Trace.Blob[i].Rect.t : Train.AreaRect.t;
					  Train.AreaRect.r = Trace.Blob[i].Rect.r>Train.AreaRect.r ? Trace.Blob[i].Rect.r : Train.AreaRect.r;
					  Train.AreaRect.b = Trace.Blob[i].Rect.b>Train.AreaRect.b ? Trace.Blob[i].Rect.b : Train.AreaRect.b;
                  }

                  TrIndex++;


        case 11 : //전체 Train ROI영역 AreaROI영역에 넘겨줌 (Match알고리즘 적용하여 Train진행함)
                  AreaRoi.SetOrgX  (Train.AreaRect.l               );
                  AreaRoi.SetOrgY  (Train.AreaRect.t              );
                  AreaRoi.SetWidth (Train.AreaRect.r-Train.AreaRect.l);
                  AreaRoi.SetHeight(Train.AreaRect.b-Train.AreaRect.t);

                  AreaFind.GImage = GImage  ;
                  AreaFind.TRoi   = &AreaRoi;
                  AreaFind.IRoi   = IRoi    ;

                  AreaFind.OnTrain();
                  TrIndex = 0;
                  TrIndex++;

        case 12 : //개별 Blob영역기준 개별 Character 영역 Train(Match)진행함 
                  for (int i=0; i<Trace.BlobCount; i++) {
					  Train.FontRect[i].l = Trace.Blob[i].Rect.l - Property.TrMargin  ;
					  Train.FontRect[i].t = Trace.Blob[i].Rect.t - Property.TrMargin  ;
					  Train.FontRect[i].r = Trace.Blob[i].Rect.r + Property.TrMargin*2;
					  Train.FontRect[i].b = Trace.Blob[i].Rect.b + Property.TrMargin*2;

                      Train.Gx[i] = (Train.FontRect[i].l-Train.AreaRect.l);
                      Train.Gy[i] = (Train.FontRect[i].t-Train.AreaRect.t);

                      FontRoi.SetOrgX  (Train.FontRect[i].l                  );
                      FontRoi.SetOrgY  (Train.FontRect[i].t                 );
                      FontRoi.SetWidth (Train.FontRect[i].r-Train.FontRect[i].l);
                      FontRoi.SetHeight(Train.FontRect[i].b-Train.FontRect[i].t);

                      FontFind[i].GImage = GImage  ;
                      FontFind[i].TRoi   = &FontRoi;
                      FontFind[i].IRoi   = &FontRoi;
                      FontFind[i].OnTrain();
                  }
                  TrIndex++;

        case 13 :
                  Train.IsEnabled = true;
                  TrIndex++;

        case 14 : TrIndex=0;
                  return OnExecute();

    }
}

int CPrint::OnTrainWidthRect(int RectCount, JRect *Rect, CString ReferStr)
{
    if (RectCount<1) return false;
    if (Rect==NULL) return false;

    Property.ReferString = ReferStr;

    int l=0, t=0, r=0, b=0;
    int TrIndex = 10;
    int w = 0;
    int h = 0;

    switch (TrIndex) {
        default : return 0;

        case 10 :
//                   w = Image.GetWidth();
//                   h = Image.GetHeight();

				  w = GImage->GetWidth();
				  h = GImage->GetHeight();

                  Train.Image.SetSize(w, h);
                  memcpy(Train.Image.GetImagePtr(), GImage->GetImagePtr(), w*h);

                  if (RectCount>=MAX_PRINT_FONT) { TrIndex = 0; return 0; }
                  if (RectCount<1) { TrIndex = 0; return 0; }

                  Train.Count = RectCount;

				  Train.AreaRect.l = 10000;
				  Train.AreaRect.t = 10000;
				  Train.AreaRect.r = 0;
				  Train.AreaRect.b = 0;


                  for (int i=0; i<RectCount; i++) {
					  Train.AreaRect.l = Rect[i].l<Train.AreaRect.l ? Rect[i].l : Train.AreaRect.l;
					  Train.AreaRect.t = Rect[i].t<Train.AreaRect.t ? Rect[i].t : Train.AreaRect.t;
					  Train.AreaRect.r = Rect[i].r>Train.AreaRect.r ? Rect[i].r : Train.AreaRect.r;
					  Train.AreaRect.b = Rect[i].b>Train.AreaRect.b ? Rect[i].b : Train.AreaRect.b;
                  }

                  TrIndex++;


        case 11 :
                  AreaRoi.SetOrgX  (Train.AreaRect.l               );
                  AreaRoi.SetOrgY  (Train.AreaRect.t              );
                  AreaRoi.SetWidth (Train.AreaRect.r-Train.AreaRect.l);
                  AreaRoi.SetHeight(Train.AreaRect.b-Train.AreaRect.t);

                  AreaFind.GImage = GImage  ;
                  AreaFind.TRoi   = &AreaRoi;
                  AreaFind.IRoi   = IRoi    ;

                  AreaFind.OnTrain();
                  TrIndex = 0;
                  TrIndex++;

        case 12 :
                  for (int i=0; i<RectCount; i++) {
					  Train.FontRect[i].l = Rect[i].l - Property.TrMargin  ;
					  Train.FontRect[i].t = Rect[i].t - Property.TrMargin  ;
					  Train.FontRect[i].r = Rect[i].r + Property.TrMargin*2;
					  Train.FontRect[i].b = Rect[i].b + Property.TrMargin*2;

                      Train.Gx[i] = (Train.FontRect[i].l-Train.AreaRect.l);
                      Train.Gy[i] = (Train.FontRect[i].t-Train.AreaRect.t);

                      FontRoi.SetOrgX  (Train.FontRect[i].l                  );
                      FontRoi.SetOrgY  (Train.FontRect[i].t                 );
                      FontRoi.SetWidth (Train.FontRect[i].r-Train.FontRect[i].l);
                      FontRoi.SetHeight(Train.FontRect[i].b-Train.FontRect[i].t);

                      FontFind[i].GImage = GImage  ;
                      FontFind[i].TRoi   = &FontRoi;
                      FontFind[i].IRoi   = &FontRoi;
                      FontFind[i].OnTrain();
                  }
                  TrIndex++;

        case 13 :
                  Train.IsEnabled = true;
                  TrIndex++;

        case 14 : TrIndex=0;
                  return OnExecute();

    }
}

int CPrint::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check CPrint Train GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check CPrint Train GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check CPrint Train GImage Height Size"    )); return false; }

    int x = IRoi->GetOrgX();
    int y = IRoi->GetOrgY();
    int w = IRoi->GetWidth();
    int h = IRoi->GetHeight();

    int gw = GImage->GetWidth();
    int gh = GImage->GetHeight();

    if (x<1) { /*SetProcessMessage("int CPrint::OnExecute()>> x<1 ,...");*/ return false; }
    if (y<1) { /*SetProcessMessage("int CPrint::OnExecute()>> y<1 ,...");*/ return false; }
    if (w<1) { /*SetProcessMessage("int CPrint::OnExecute()>> w<1 ,...");*/ return false; }
    if (h<1) { /*SetProcessMessage("int CPrint::OnExecute()>> h<1 ,...");*/ return false; }
    if ((x+w)>gw) { /*SetProcessMessage("int CPrint::OnExecute()>>(x+w)>gw");*/ return false; }
    if ((y+h)>gh) { /*SetProcessMessage("int CPrint::OnExecute()>>(y+h)>gh");*/ return false; }

    if      (Mode==0) IsPass = OnExecuteBySrh();
    else if (Mode==1) IsPass = OnExecuteByBlob();
    return IsPass;
}

int CPrint::OnExecuteBySrh()
{
    IsPass = false;

	AreaRoi.SetOrgX  (IRoi->GetOrgX()                  );
	AreaRoi.SetOrgY  (IRoi->GetOrgY()                  );
	AreaRoi.SetWidth (IRoi->GetOrgX()+IRoi->GetWidth ());
	AreaRoi.SetHeight(IRoi->GetOrgY()+IRoi->GetHeight());

    JRect BdRect;
    BdRect.l = AreaRoi.GetOrgX();
    BdRect.t = AreaRoi.GetOrgY();
    BdRect.r = AreaRoi.GetOrgX()+AreaRoi.GetWidth();
    BdRect.b = AreaRoi.GetOrgY()+AreaRoi.GetHeight();

    FindAreaCount = 0;
    FindFontCount = 0;
    FindDefectCount = 0;

    int StaTime = 0;
    int EndTime = 0;

    int IpIndex = 10;
    bool FontFindString = false;
    bool IsBroken = false;
    int IpThHold = 0;
    FindAreaCount = 0;
    if (Property.ShMargin<10) Property.ShMargin = 10;

        switch (IpIndex) {
        default : IpIndex = 0; return 0;

        case 10 : //전체 마킹영역을 Match 알고리즘을 적용하여 찾는다
                  //StaTime = GetCount();

                  AreaFind.GImage = GImage  ;
                  AreaFind.TRoi   = &AreaRoi;
                  AreaFind.IRoi   = IRoi    ;

                  AreaFind.SetMinScore(Property.ShRectScore);
                  AreaFind.SetMaxPositions(1);
                  //AreaFind.SetMaxAngle(20.0);
                  //AreaFind.SetMinAngle(-20.0);
                  //20160718
                  AreaFind.SetMaxAngle(  Property.ShRectAngle );
                  AreaFind.SetMinAngle(-(Property.ShRectAngle));

                  if (!AreaFind.OnExecute()) {
                      BinId = ocvAreaMatch;
                      return 0;
                  }

                  //EndTime = GetCount()-StaTime;

                  RectMatchScore = 0;
                  RectMatchScore = AreaFind.GetResultScore(0);
                  IsRectMatch = (RectMatchScore>=Property.PassRectScore) ? true : false;

                  FindAreaCount = AreaFind.FFindCount;

                  if (Property.OpGabAngle) {
                      if (AreaFind.Result[0].FAngle>Property.GabAngle) {
                          BinId = ocvAngle;
                          return 0;
                      }
                  }


                  FResultGabLeft   = 0;
                  FResultGabTop    = 0;
                  FResultGabRight  = 0;
                  FResultGabBottom = 0;

                  if (Property.OpGabLeft  ) FResultGabLeft   = AreaFind.Result[0].FRect.l-BdRect.l;
                  if (Property.OpGabTop   ) FResultGabTop    = AreaFind.Result[0].FRect.t-BdRect.t;
                  if (Property.OpGabRight ) FResultGabRight  = BdRect.r -AreaFind.Result[0].FRect.r;
                  if (Property.OpGabBottom) FResultGabBottom = BdRect.b -AreaFind.Result[0].FRect.b;


				  if (Property.OpGabLeft  && ((BdRect.l+Property.GabLeft  -AreaFind.Result[0].FRect.l) > 0)) { BinId = ocvGabLeft  ; return 0; }
				  if (Property.OpGabTop   && ((BdRect.t+Property.GabTop   -AreaFind.Result[0].FRect.t) > 0)) { BinId = ocvGabTop   ; return 0; }
				  if (Property.OpGabRight && ((BdRect.r-Property.GabRight -AreaFind.Result[0].FRect.r) < 0)) { BinId = ocvGabRight ; return 0; }
				  if (Property.OpGabBottom&& ((BdRect.b-Property.GabBottom-AreaFind.Result[0].FRect.b) < 0)) { BinId = ocvGabBottom; return 0; }

                  IpIndex++;

        case 11 :
                  //StaTime = GetCount();

                  if (Property.OpStringCount) {
                      if (Train.Count!=Property.ReferString.Trim()) {
                          BinId = ocvStringCount;
                          return false;
                      }
                  }

                  for (int i=0; i<Train.Count; i++) {//개별 글자를 Match 알고리즘을 이용하여 찾는다(Score 추출)
                      CString ReferChar = Property.ReferString.Trim().Mid(i+1, 1);
                      IsIgnore[i] = false;
                      if (ReferChar=="?") {
                          FindFontCount++;
                          BlobMatchScore[i] = 0;
                          IsBlobMatch[i] = true;
                          IsIgnore[i] = true;
                          continue;

                      }

                      int Ax = Train.Gx[i];
                      int Ay = Train.Gy[i];

                      FontRoi.SetOrgX  (AreaFind.Result[0].FRect.l + Ax - Property.ShMargin);
                      FontRoi.SetOrgY  (AreaFind.Result[0].FRect.t + Ay - Property.ShMargin);
                      FontRoi.SetWidth (Train.FontRect[i].r - Train.FontRect[i].l + Property.ShMargin*2);
                      FontRoi.SetHeight(Train.FontRect[i].b - Train.FontRect[i].t + Property.ShMargin*2);

                      FontFind[i].GImage = GImage  ;
                      FontFind[i].IRoi   = &FontRoi;

                      FontFind[i].SetMaxPositions(1);

                      //FontFind[i].SetMaxAngle(50.0);
                      //FontFind[i].SetMinAngle(-50.0);
                      //20160718
                      FontFind[i].SetMaxAngle(  Property.ShFontAngle );
                      FontFind[i].SetMinAngle(-(Property.ShFontAngle));

                      FontFind[i].SetMinScore(Property.ShBlobScore);

                      FontFindString = FontFind[i].OnExecute();
                      if (FontFindString) FindFontCount++;

                      BlobMatchScore[i] = 0;
                      BlobMatchScore[i] = FontFind[i].GetResultScore(0);
                      IsBlobMatch[i] = (BlobMatchScore[i]>=Property.PassBlobScore) ? true : false;
                  }

                  for (int i=0; i<Train.Count; i++) { // 개별 Marking을 찾지 못할경우
                      if (!IsBlobMatch[i]) {
                          BinId = ocvFontMatch;
                          return 0;
                      }
                  }

                  if (Train.Count!=FindFontCount) {
                      BinId = ocvFontMatch;
                      return 0;
                  }

                  //EndTime = GetCount()-StaTime;

                  IpIndex++;

        case 12 : // Over & Under Matching Inspection
                  //StaTime = GetCount();

                  OverPrintPixelCount = 0;
                  UnderPrintPixelCount = 0;
                  // Over Print Inspection
                  IsBroken = true;
                  IpThHold = Property.IpThHold;

                  for (int k=0; k<2; k++) {
                      IpThHold  = Property.IpThHold;
                      if (k==0) { IpThHold  = Property.IpThHold; }
                      if (k==1) { IpThHold += Property.DvThHold ; }

                      if (OnOverPrint(Property.TrThHold, IpThHold)) {
                          BinId = ocvOverPrint;
                          IsBroken = false;
                          break;
                      }
                  }

                  if (IsBroken) return 0;

                  // Under Print Inspection
                  IsBroken = true;
                  IpThHold = Property.IpThHold;

                  for (int k=0; k<2; k++) {
                      IpThHold  = Property.IpThHold;
                      if (k==0) { IpThHold  = Property.IpThHold; }
                      if (k==1) { IpThHold -= Property.DvThHold ; }

                      if (OnUnderPrint(Property.TrThHold, IpThHold)) {
                          BinId = ocvUnderPrint;
                          IsBroken = false;
                          break;
                      }
                  }

                  if (IsBroken) return 0;

                  //EndTime = GetCount()-StaTime;
                  if (!OnAngle()) return 0;
                  IpIndex++;

        case 13 :
                  IpIndex = 0;
    }

    BinId = ocvPass;
    IsPass = true;

    return 1;
}


int CPrint::OnExecuteByBlob()
{
    IsPass = false;

	AreaRoi.SetOrgX  (IRoi->GetOrgX()                  );
	AreaRoi.SetOrgY  (IRoi->GetOrgY()                  );
	AreaRoi.SetWidth (IRoi->GetOrgX()+IRoi->GetWidth ());
	AreaRoi.SetHeight(IRoi->GetOrgY()+IRoi->GetHeight());

    JRect BdRect;
	BdRect.l = AreaRoi.GetOrgX();
	BdRect.t = AreaRoi.GetOrgY();
	BdRect.r = AreaRoi.GetOrgX()+AreaRoi.GetWidth();
	BdRect.b = AreaRoi.GetOrgY()+AreaRoi.GetHeight();

    FindAreaCount = 0;
    FindFontCount = 0;
    FindDefectCount = 0;

    int StaTime = 0;
    int EndTime = 0;

    int IpIndex = 10;
    bool FontFindString = false;
    bool IsBroken = false;
    int IpThHold = 0;
    if (Property.ShMargin<10) Property.ShMargin = 10;

        switch (IpIndex) {
        default : IpIndex = 0; return 0;

        case 10 :
                  Trace.GImage = GImage;
                  Trace.IRoi   = IRoi  ;

                  Trace.OnExecute();

                  if (Trace.BlobCount<1) { IpIndex = 0; return 0; }
                  if (Trace.BlobCount>=MAX_PRINT_FONT) { IpIndex = 0; return 0; }

                  if (Property.OpGabAngle) {
                      if (AreaFind.Result[0].FAngle>Property.GabAngle) return 0;
                  }

				  if (Property.OpGabLeft  && ((BdRect.l+Property.GabLeft  -AreaFind.Result[0].FRect.l) > 0)) return 0;
				  if (Property.OpGabTop   && ((BdRect.t+Property.GabTop   -AreaFind.Result[0].FRect.t) > 0)) return 0;
				  if (Property.OpGabRight && ((BdRect.r-Property.GabRight -AreaFind.Result[0].FRect.r) < 0)) return 0;
				  if (Property.OpGabBottom&& ((BdRect.b-Property.GabBottom-AreaFind.Result[0].FRect.b) < 0)) return 0;

                  IpIndex++;

        case 11 :
                  for (int i=0; i<Train.Count; i++) {
                      FontRoi.SetOrgX  (Trace.Blob[i].Rect.l - Property.ShMargin);
                      FontRoi.SetOrgY  (Trace.Blob[i].Rect.t - Property.ShMargin);
                      FontRoi.SetWidth (Trace.Blob[i].Rect.GetWidth() + Property.ShMargin*2);
                      FontRoi.SetHeight(Trace.Blob[i].Rect.GetHeight() + Property.ShMargin*2);

                      FontFind[i].GImage = GImage  ;
                      FontFind[i].IRoi   = &FontRoi;

                      FontFind[i].SetMaxPositions(1);
                      FontFind[i].SetMaxAngle(5.0);
                      FontFind[i].SetMinAngle(-5.0);

                      FontFind[i].SetMinScore(Property.ShBlobScore);

                      FontFindString = FontFind[i].OnExecute();
                      if (FontFindString) FindFontCount++;

                      BlobMatchScore[i] = 0;
                      BlobMatchScore[i] = FontFind[i].GetResultScore(0);
                      IsBlobMatch[i] = (BlobMatchScore[i]>=Property.PassBlobScore) ? true : false;
                  }

                  for (int i=0; i<Train.Count; i++) {
                      if (!IsBlobMatch[i]) return 0;
                  }

                  if (Train.Count!=FindFontCount) return 0;

                  IpIndex++;

        case 12 :
                  //StaTime = GetCount();

                  OverPrintPixelCount = 0;
                  UnderPrintPixelCount = 0;

                  // Over Print Inspection
                  IsBroken = true;
                  IpThHold = Property.IpThHold;

                  for (int k=0; k<2; k++) {
                      IpThHold  = Property.IpThHold;
                      if (k==0) { IpThHold  = Property.IpThHold; }
                      if (k==1) { IpThHold += Property.DvThHold ; }

                      if (OnOverPrint(Property.TrThHold, IpThHold)) {
                          IsBroken = false;
                          break;
                      }
                  }

                  if (IsBroken) return 0;

                  // Under Print Inspection
                  IsBroken = true;
                  IpThHold = Property.IpThHold;

                  for (int k=0; k<2; k++) {
                      IpThHold  = Property.IpThHold;
                      if (k==0) { IpThHold  = Property.IpThHold; }
                      if (k==1) { IpThHold -= Property.DvThHold ; }

                      if (OnUnderPrint(Property.TrThHold, IpThHold)) {
                          IsBroken = false;
                          break;
                      }
                  }

                  if (IsBroken) return 0;

                  //EndTime = GetCount()-StaTime;
                  if (!OnAngle()) return 0;
                  IpIndex++;

        case 13 :
                  IpIndex = 0;
    }

    IsPass = true;

    return 1;
}

bool CPrint::OnOverPrint(int TrThHold, int CuThHold)
{
    if (!Property.OpOverPrinted) return true;

//     #define IElement(x, y)  *(IAddress + x + ImageWidth * y)
//     #define TElement(x, y)  *(TAddress + x + ImageWidth * y)
//     #define MElement(x, y)  *(MAddress + x + ImageWidth * y)

//     int ImageWidth  = Image.GetWidth ();
//     int ImageHeight = Image.GetHeight();

	int ImageWidth  = GImage->GetWidth ();
	int ImageHeight = GImage->GetHeight();
	BufferHeight    = GImage->GetWidth() ;

    EImageBW8 MxImage;
	IAddress = GImage->GetImagePtr        (); //검사  Image  
	TAddress = Train.Image.GetImagePtr    (); //Train Image
	MxImage.SetSize(ImageWidth, ImageHeight); //Mask  Image 
	MAddress = MxImage.GetImagePtr        ();
// 	BYTE *IAddress = (BYTE *)Image.GetImagePtr();
//  BYTE *TAddress = (BYTE *)Train.Image.GetImagePtr();

    if (IAddress == NULL) return false;
    if (TAddress == NULL) return false;

    OverPrintPixelCount = 0;
    memset(MAddress, 0, ImageWidth*ImageHeight);

    int xOffset = 0;
    int yOffset = 0;
    int Count   = 0;
    bool IsCuPx, IsTrPx ;
    bool CuPx  , TrPx   ;

    const int OpMoveWeight = 1;
    bool IsBroken = false;

    for (register int i=0; i<Train.Count; i++) {
        CString ReferChar = Property.ReferString.Trim().Mid(i+1, 1);
        if (ReferChar=="?") {
            continue;
        }

		int dl = FontFind[i].Result[0].FRect.l-Train.FontRect[i].l;
		int dt = FontFind[i].Result[0].FRect.t-Train.FontRect[i].t;
		int dr = FontFind[i].Result[0].FRect.r-Train.FontRect[i].r;
		int db = FontFind[i].Result[0].FRect.b-Train.FontRect[i].b;

		int l = FontFind[i].Result[0].FRect.l-PxTrOffset;
		int t = FontFind[i].Result[0].FRect.t-PxTrOffset;
		int r = FontFind[i].Result[0].FRect.r+PxTrOffset;
		int b = FontFind[i].Result[0].FRect.b+PxTrOffset;

        int xLength = FontFind[i].Result[0].FRect.GetWidth();
        int yLength = FontFind[i].Result[0].FRect.GetHeight();

        int tx = Train.FontRect[i].l;
        int ty = Train.FontRect[i].t;

        for (register int n=t; n<=b; n++) {
        for (register int m=l; m<=r; m++) {
            if (m<0) return false;
            if (n<0) return false;
            if ((m-l)<0) return false;
            if ((n-t)<0) return false;

            int tm = tx+(m-l);
            int tn = ty+(n-t);

            if(Trace.FLoThHold!=0){
                CuPx   = IElement( m,  n)>CuThHold;
                TrPx   = TElement(tm, tn)>TrThHold;
            }
            else{
                CuPx    = IElement( m,  n)<CuThHold;
                TrPx    = TElement(tm, tn)<TrThHold;
            }

            bool IsSame = (CuPx==TrPx);

            if (IsSame) continue;

            bool IsBreak = false;
            if (Property.xBleed||Property.yBleed) {
                int xBleed = Property.xBleed;
                int yBleed = Property.yBleed;

                register int h = 0;
                register int g = 0;

                for (h=-1*yBleed; h<=yBleed; h++) {
                    for (g=-1*xBleed; g<=xBleed; g++) {
                        if(Trace.FLoThHold!=0) CuPx = IElement((m+g), (n+h))>CuThHold;  // Side조명 사용
                        else                   CuPx = IElement((m+g), (n+h))<CuThHold;  // 동축조명 사용

                        if (CuPx==TrPx) {
                            IsBreak = true;
                            break;
                        }
                    }
                    if (IsBreak) break;
                }
            }

            if (IsBreak) continue;

            if (CuPx && !TrPx) {
                if (Property.OpOverPrinted) {
                    if (OverPrintPixelCount<MAX_OV_PRINT_PIXEL) {
                        OvPrintPixel[OverPrintPixelCount].x = m;
                        OvPrintPixel[OverPrintPixelCount].y = n;
                        OverPrintPixelCount++;
                    }
                }
            }
            else {
            }
        }}
    }

    //Find Broken Over Print Pixel ...
    //----------------------------------------------------------------------
    FResultOverPrintLength = 0;

    if (Property.OpOverPrinted) {
        for (register int k=0; k<OverPrintPixelCount; k++) {
            int x = OvPrintPixel[k].x;
            int y = OvPrintPixel[k].y;
            MElement(x, y) = 255;
        }

        int OldxMin = Trace.FMinX;
        int OldyMin = Trace.FMinY;

        Trace.FMinX = Property.OverPrintedX>0 ? Property.OverPrintedX : 2;
        Trace.FMinY = Property.OverPrintedY>0 ? Property.OverPrintedY : 2;

        Trace.GImage = &MxImage;
        Trace.IRoi   = IRoi    ;
        Trace.OnExecute();

        Trace.FMinX = OldxMin;
        Trace.FMinY = OldyMin;

        FindDefectCount = Trace.BlobCount;

        if (FindDefectCount) {
            FResultOverPrintLength = Trace.Blob[0].Rect.GetWidth();
            return false;
        }
    }

    if (FindDefectCount) return false;
    return true;
}

bool CPrint::OnUnderPrint(int TrThHold, int CuThHold)
{
    if (!Property.OpUnderPrinted) return true;

//     #define IElement(x, y)  *(IAddress + x + ImageWidth * y)
//     #define TElement(x, y)  *(TAddress + x + ImageWidth * y)
//     #define MElement(x, y)  *(MAddress + x + ImageWidth * y)

//     int ImageWidth  = Image.GetWidth ();
//     int ImageHeight = Image.GetHeight();

	int ImageWidth  = GImage->GetWidth ();
	int ImageHeight = GImage->GetHeight();
	BufferHeight    = GImage->GetWidth() ;

	EImageBW8 MxImage;
	IAddress = GImage->GetImagePtr        (); //검사  Image  
	TAddress = Train.Image.GetImagePtr    (); //Train Image
	MxImage.SetSize(ImageWidth, ImageHeight); //Mask  Image 
	MAddress = MxImage.GetImagePtr        ();

    if (IAddress == NULL) return false;
    if (TAddress == NULL) return false;

    UnderPrintPixelCount = 0;
    memset(MAddress, 0, ImageWidth*ImageHeight); //Mask Image 0값으로 초기화

    int xOffset = 0;
    int yOffset = 0;
    int Count   = 0;
    bool IsCuPx, IsTrPx ;
    bool CuPx  , TrPx   ;

    const int OpMoveWeight = 1;
    bool IsBroken = false;

    for (register int i=0; i<Train.Count; i++) {
        CString ReferChar = Property.ReferString.Trim().Mid(i+1, 1);
        if (ReferChar=="?") {
            continue;
        }

		int dl = FontFind[i].Result[0].FRect.l-Train.FontRect[i].l;
		int dt = FontFind[i].Result[0].FRect.t-Train.FontRect[i].t;
		int dr = FontFind[i].Result[0].FRect.r-Train.FontRect[i].r;
		int db = FontFind[i].Result[0].FRect.b-Train.FontRect[i].b;

		int l = FontFind[i].Result[0].FRect.l - PxTrOffset;
		int t = FontFind[i].Result[0].FRect.t - PxTrOffset;
		int r = FontFind[i].Result[0].FRect.r + PxTrOffset;
		int b = FontFind[i].Result[0].FRect.b + PxTrOffset;

        int xLength = FontFind[i].Result[0].FRect.GetWidth();
        int yLength = FontFind[i].Result[0].FRect.GetHeight();

        int tx = Train.FontRect[i].l;
        int ty = Train.FontRect[i].t;

        for (register int n=t; n<=b; n++) {
        for (register int m=l; m<=r; m++) {
            if (m<0) return false;
            if (n<0) return false;
            if ((m-l)<0) return false;
            if ((n-t)<0) return false;

            int tm = tx+(m-l);
            int tn = ty+(n-t);

            //CuPx = IElement( m,  n)>CuThHold;
            //TrPx = TElement(tm, tn)>TrThHold;
            if(Trace.FLoThHold!=0){
                CuPx   = IElement( m,  n)>CuThHold;
                TrPx   = TElement(tm, tn)>TrThHold;
            }
            else{
                CuPx   = IElement( m,  n)<CuThHold;
                TrPx   = TElement(tm, tn)<TrThHold;
            }

            bool IsSame = (CuPx==TrPx);

            if (IsSame) continue;

            bool IsBreak = false;
            if (Property.xBleed||Property.yBleed) {
                int xBleed = Property.xBleed;
                int yBleed = Property.yBleed;

                register int h = 0;
                register int g = 0;

                for (h=-1*yBleed; h<=yBleed; h++) {
                    for (g=-1*xBleed; g<=xBleed; g++) {
                        if(Trace.FLoThHold!=0) CuPx = IElement((m+g), (n+h))>CuThHold;  // Side조명 사용
                        else                   CuPx = IElement((m+g), (n+h))<CuThHold;  // 동축조명 사용

                        if (CuPx==TrPx) {
                            IsBreak = true;
                            break;
                        }
                    }
                    if (IsBreak) break;
                }
            }

            if (IsBreak) continue;

            if (!CuPx && TrPx) {
                if (Property.OpUnderPrinted) {
                    if (UnderPrintPixelCount<MAX_UN_PRINT_PIXEL) {
                        UnPrintPixel[UnderPrintPixelCount].x = m;
                        UnPrintPixel[UnderPrintPixelCount].y = n;
                            UnderPrintPixelCount++;
                    }
                }
            }
            /*
            if (CuPx && !TrPx) {
            }
            else {
                if (Property.OpUnderPrinted) {
                    if (UnderPrintPixelCount<MAX_UN_PRINT_PIXEL) {
                        UnPrintPixel[UnderPrintPixelCount].x = m;
                        UnPrintPixel[UnderPrintPixelCount].y = n;
                            UnderPrintPixelCount++;
                    }
                }
            }
            */

        }}
    }

    //Find Broken Pixel Under Print Pixel ...
    //----------------------------------------------------------------------
    FResultUnderPrintLength = 0;

    if (Property.OpUnderPrinted) {
        memset(MAddress, 0, ImageWidth*ImageHeight);

        for (register int k=0; k<UnderPrintPixelCount; k++) {
            int x = UnPrintPixel[k].x;
            int y = UnPrintPixel[k].y;
            MElement(x, y) = 255;
        }

        int OldxMin = Trace.FMinX;
        int OldyMin = Trace.FMinY;

        Trace.FMinX = Property.UnderPrintedX>0 ? Property.UnderPrintedX : 2;
        Trace.FMinY = Property.UnderPrintedY>0 ? Property.UnderPrintedY : 2;

        Trace.GImage = &MxImage;
        Trace.IRoi   = IRoi    ;
        Trace.OnExecute();

        Trace.FMinX = OldxMin;
        Trace.FMinY = OldyMin;

        FindDefectCount = Trace.BlobCount;

        if (FindDefectCount) {
            FResultUnderPrintLength = Trace.Blob[0].Rect.GetWidth();
            return false;
        }
    }

    if (FindDefectCount) return false;
    return true;
}

bool CPrint::OnAngle()
{
    if (!Property.OpGabAngle) return true;

    float Angle = GetAngle();
    if (fabs(Angle)>Property.GabAngle) return false;

    return true;
}

float CPrint::GetAngle()
{
    TFPoint FBCPoint;
	TFPoint SBCPoint;

	FBCPoint = FontFind[Property.StaAgId].Result[0].FRect.Center();
	SBCPoint = FontFind[Property.EndAgId].Result[0].FRect.Center();
    
	float CuAngle = Math.GetDegree(FBCPoint.x, FBCPoint.y, SBCPoint.x, SBCPoint.y);

    if (CuAngle>90) CuAngle -= 90;
    if (CuAngle>90) CuAngle -= 90;
    if (CuAngle>90) CuAngle -= 90;
    if (CuAngle>90) CuAngle -= 90;

    if (CuAngle>45) CuAngle -= 90;

    ResultAngle = CuAngle;

    return CuAngle;
}

void CPrint::OnPaint(TDrawTool Tool)
{
    if (Tool.Handle == NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    Gdi->CText.Color  = YELLOW_COLOR;
    Gdi->CPen .Color  = RED_COLOR   ;
    Gdi->CFont.Height = 14;
    Gdi->CPen .Width  =  1;

    int l = 0, t = 0, r = 0, b = 0;
    CString Str = "";

    int PaIndex = 0;

    if (PaIndex==0) {
        PaIndex = 10;
    }

    int ShMargin = Property.ShMargin;
    ShMargin = 0;
    CString Score = "";
    CString Angle = "";

    switch(PaIndex) {
        case 10 :
                  if (DwIRoi) {
                      Gdi->CPen.Color = YELLOW_COLOR;
                      l = IRoi->GetOrgX();
                      t = IRoi->GetOrgY();
					  r = IRoi->GetOrgX()+IRoi->GetWidth();
					  b = IRoi->GetOrgY()+IRoi->GetHeight();

                      Gdi->OnMove(l, t);
                      Gdi->OnLine(r, t);
                      Gdi->OnLine(r, b);
                      Gdi->OnLine(l, b);
                      Gdi->OnLine(l, t);
                  }

                  if (Train.Count<1) Gdi->OnText(IRoi->GetOrgX()+10, IRoi->GetOrgY()+30, "NEED TRAIN");
                  if (Property.ReferString=="") Gdi->OnText(IRoi->GetOrgX()+10, IRoi->GetOrgY()+30, "NEED REFER STRING");

                  Gdi->CPen.Color = YELLOW_COLOR;
                  Gdi->OnText(IRoi->GetOrgX(), IRoi->GetOrgY()-25, GetOcvName());

                  if (Mode==0 && FindAreaCount<1) {
                      Gdi->CPen.Color = YELLOW_COLOR;
                      l = AreaRoi.GetOrgX();
                      t = AreaRoi.GetOrgY();
                      r = l+AreaRoi.GetWidth();
                      b = t+AreaRoi.GetHeight();
                      Gdi->OnMove(l, t);
                      Gdi->OnLine(r, t);
                      Gdi->OnLine(r, b);
                      Gdi->OnLine(l, b);
                      Gdi->OnLine(l, t);

					  Gdi->OnText(IRoi->GetOrgX()+10, IRoi->GetOrgY()+30, "FAIL AREA MATCH");
				  }

				  if (DwAreaFind) {
					  Tool.Color = LIME_COLOR;
					  AreaFind.OnPaint(Tool);
				  }

				  if (DwReferString) {
					  Gdi->OnText(IRoi->GetOrgX()+10, IRoi->GetOrgY()+10, Property.ReferString);
				  }

				  if (DwAngle) {
					  Angle.Format(L"%Angle : 3.2f", ResultAngle);
					  Gdi->OnText(IRoi->GetOrgX()+10, IRoi->GetOrgY()+30, Angle);
				  }

                  if (DwFont) {
                      for (int i=0; i<FindFontCount; i++) {
                          if (IsIgnore[i]) continue;
                          //if (!IsBlobMatch[i]) Tool = YELLOW_COLOR;
						  Tool.Color = YELLOW_COLOR;
                          if (FontFind[i].FFindCount) FontFind[i].OnPaint(Tool);
                      }
                  }
                  if (DwScore) {
                      for (int i=0; i<FindFontCount; i++) {
                          if (IsIgnore[i]) continue;
                          CString Score;
						  Score.Format(L"%3.1f", BlobMatchScore[i]);

                          if (FontFind[i].FFindCount) Gdi->OnText(FontFind[i].Result[0].FRect.l, FontFind[i].Result[0].FRect.b+8, Score);
                      }
                  }

                  //Defect Blob
                  if (FindDefectCount) {
                      //Trace.OnPaintBlobRect(RTool);
                      //Trace.OnPaintBlobRect(Tool);

                      if (DwOverPrintPixel) {
                          for (int i=0; i<OverPrintPixelCount; i++) {
                               Gdi->OnPixel(OvPrintPixel[i].x, OvPrintPixel[i].y, YELLOW_COLOR);
                          }
                      }

                      if (DwUnderPrintPixel) {
                          for (int i=0; i<UnderPrintPixelCount; i++) {
                               Gdi->OnPixel(UnPrintPixel[i].x, UnPrintPixel[i].y, YELLOW_COLOR);
                          }
                      }
                  }

                  break;


        // Train Area
        // ---------------------------------------------------------------------
        case 30 :
                  for (int i=0; i<Train.Count; i++) {
                      Gdi->CPen.Color = FontFind[i].FFindCount>0 ? YELLOW_COLOR : YELLOW_COLOR;

                      l = AreaFind.Result[0].FRect.l + Train.Gx[i] - Property.ShMargin;
                      t = AreaFind.Result[0].FRect.t + Train.Gy[i] - Property.ShMargin;
                      r = l+(Train.FontRect[i].r-Train.FontRect[i].l) + Property.ShMargin*2;
                      b = t+(Train.FontRect[i].b-Train.FontRect[i].t) + Property.ShMargin*2;

                      Gdi->OnMove(l, t);
                      Gdi->OnLine(r, t);
                      Gdi->OnLine(r, b);
                      Gdi->OnLine(l, b);
                      Gdi->OnLine(l, t);
                  }
                  break;
    }

    delete Gdi;
}

void CPrint::OnPaintResult(TDrawTool Tool)
{
    HGdi *Gdi = new HGdi(Tool.Handle);

	Gdi->FVx = Tool.FVx;
	Gdi->FVy = Tool.FVy;
	Gdi->FHs = Tool.FHs;
	Gdi->FVs = Tool.FVs;
	Gdi->CPen.Color = Tool.Color;

    Gdi->CPen.Style = 0;
    Gdi->CFont.Height = 28;

	int x = IRoi->GetOrgX();
	int y = IRoi->GetOrgY();
    CString Txt = IsPass ? "PASS" : "FAIL";
    Gdi->CText.Color = IsPass ? YELLOW_COLOR : RED_COLOR;
    Gdi->OnText(x+20, y+60, Txt);

    delete Gdi;
}

void CPrint::OnLoadRoi(bool IsLoad, CString Path, EROIBW8 *lpRoi)
{
    CIFile Ini;

    if (Path.Trim()=="") return;

    //Inspection Roi
    int Roi_x = lpRoi->GetOrgX();
    int Roi_y = lpRoi->GetOrgY();
    int Roi_w = lpRoi->GetWidth();
    int Roi_h = lpRoi->GetHeight();

    Ini.Load(IsLoad, Path, "Property", "IRoi_x", &Roi_x);
    Ini.Load(IsLoad, Path, "Property", "IRoi_y", &Roi_y);
    Ini.Load(IsLoad, Path, "Property", "IRoi_w", &Roi_w);
    Ini.Load(IsLoad, Path, "Property", "IRoi_h", &Roi_h);

    lpRoi->SetOrgX  (Roi_x);
    lpRoi->SetOrgY  (Roi_y);
    lpRoi->SetWidth (Roi_w);
    lpRoi->SetHeight(Roi_h);
}
//------------------------------------------------------------------------------
void CPrint::OnLoad(bool IsLoad, CString Folder)
{
    CIFile Ini;

    if (Folder.Trim()=="") return;
    CString Path = Folder + "\\Print.ini";

    Ini.Load(IsLoad, Path, "AreaFind", "FTrainImageWidth ", &AreaFind.FTrainImageWidth );
    Ini.Load(IsLoad, Path, "AreaFind", "FTrainImageHeight", &AreaFind.FTrainImageHeight);

    for (int k=0; k<MAX_PRINT_FONT; k++) {
		CString ks;
		ks.Format(L"%d",k);
        Ini.Load(IsLoad, Path, "FontFind", CString("FTrainImageWidth" )+ks, &FontFind[k].FTrainImageWidth );
        Ini.Load(IsLoad, Path, "FontFind", CString("FTrainImageHeight")+ks, &FontFind[k].FTrainImageHeight);
    }

    Ini.Load(IsLoad, Path, "Trace", "FMinX"            , &Trace.FMinX    );
    Ini.Load(IsLoad, Path, "Trace", "FMinY"            , &Trace.FMinY    );
    Ini.Load(IsLoad, Path, "Trace", "FHiThHold"        , &Trace.FHiThHold);
    Ini.Load(IsLoad, Path, "Trace", "FLoThHold"        , &Trace.FLoThHold);

    Ini.Load(IsLoad, Path, "Property", "StaAgId", &Property.StaAgId);
    Ini.Load(IsLoad, Path, "Property", "EndAgId", &Property.EndAgId);

    Ini.Load(IsLoad, Path, "Property", "DvThHold      ", &Property.DvThHold      );
    Ini.Load(IsLoad, Path, "Property", "IpThHold      ", &Property.IpThHold      );
    Ini.Load(IsLoad, Path, "Property", "TrThHold      ", &Property.TrThHold      );
    Ini.Load(IsLoad, Path, "Property", "OpGabAngle    ", &Property.OpGabAngle    );
    Ini.Load(IsLoad, Path, "Property", "OpGabLeft     ", &Property.OpGabLeft     );
    Ini.Load(IsLoad, Path, "Property", "OpGabTop      ", &Property.OpGabTop      );
    Ini.Load(IsLoad, Path, "Property", "OpGabRight    ", &Property.OpGabRight    );
    Ini.Load(IsLoad, Path, "Property", "OpGabBottom   ", &Property.OpGabBottom   );
    Ini.Load(IsLoad, Path, "Property", "GabAngle      ", &Property.GabAngle      );
    Ini.Load(IsLoad, Path, "Property", "GabLeft       ", &Property.GabLeft       );
    Ini.Load(IsLoad, Path, "Property", "GabTop        ", &Property.GabTop        );
    Ini.Load(IsLoad, Path, "Property", "GabRight      ", &Property.GabRight      );
    Ini.Load(IsLoad, Path, "Property", "GabBottom     ", &Property.GabBottom     );
    Ini.Load(IsLoad, Path, "Property", "OpOverPrinted ", &Property.OpOverPrinted );
    Ini.Load(IsLoad, Path, "Property", "OpUnderPrinted", &Property.OpUnderPrinted);
    Ini.Load(IsLoad, Path, "Property", "OverPrintedX  ", &Property.OverPrintedX  );
    Ini.Load(IsLoad, Path, "Property", "UnderPrintedX ", &Property.UnderPrintedX );
    Ini.Load(IsLoad, Path, "Property", "OverPrintedY  ", &Property.OverPrintedY  );
    Ini.Load(IsLoad, Path, "Property", "UnderPrintedY ", &Property.UnderPrintedY );
    Ini.Load(IsLoad, Path, "Property", "xBleed        ", &Property.xBleed        );
    Ini.Load(IsLoad, Path, "Property", "yBleed        ", &Property.yBleed        );
    Ini.Load(IsLoad, Path, "Property", "ShRectScore   ", &Property.ShRectScore   );
    Ini.Load(IsLoad, Path, "Property", "ShBlobScore   ", &Property.ShBlobScore   );
    Ini.Load(IsLoad, Path, "Property", "TrMargin      ", &Property.TrMargin      );
    Ini.Load(IsLoad, Path, "Property", "ShMargin      ", &Property.ShMargin      );
    Ini.Load(IsLoad, Path, "Property", "PassRectScore ", &Property.PassRectScore );
    Ini.Load(IsLoad, Path, "Property", "PassBlobScore ", &Property.PassBlobScore );
    Ini.Load(IsLoad, Path, "Property", "ReferString   ", &Property.ReferString   );
    Ini.Load(IsLoad, Path, "Property", "ShRectAngle   ", &Property.ShRectAngle   );
    Ini.Load(IsLoad, Path, "Property", "ShFontAngle   ", &Property.ShFontAngle   );


    Ini.Load(IsLoad, Path, "Train", "IsEnabled", &Train.IsEnabled );
    Ini.Load(IsLoad, Path, "Train", "Count"    , &Train.Count     );
	Ini.Load(IsLoad, Path, "Train", "AreaRectl", (int*)(&Train.AreaRect.l));
	Ini.Load(IsLoad, Path, "Train", "AreaRectt", (int*)(&Train.AreaRect.t));
	Ini.Load(IsLoad, Path, "Train", "AreaRectr", (int*)(&Train.AreaRect.r));
	Ini.Load(IsLoad, Path, "Train", "AreaRectb", (int*)(&Train.AreaRect.b));

    for (int k=0; k<MAX_PRINT_FONT; k++) {
		CString ks;
		ks.Format(L"%d",k);
        Ini.Load(IsLoad, Path, "Train", CString("Gx"       )+ks, &Train.Gx      [k]);
        Ini.Load(IsLoad, Path, "Train", CString("Gy"       )+ks, &Train.Gy      [k]);
		Ini.Load(IsLoad, Path, "Train", CString("FontRectl")+ks, (int*)&Train.FontRect[k].l);
		Ini.Load(IsLoad, Path, "Train", CString("FontRectt")+ks, (int*)&Train.FontRect[k].t);
		Ini.Load(IsLoad, Path, "Train", CString("FontRectr")+ks, (int*)&Train.FontRect[k].r);
		Ini.Load(IsLoad, Path, "Train", CString("FontRectb")+ks, (int*)&Train.FontRect[k].b);
        Ini.Load(IsLoad, Path, "Train", CString("Simbol"   )+ks, &Train.Simbol  [k]);
    }                                                         
                                                              
    // Roi Load & Save
    // -------------------------------------------------------------------------
    CString IRoiPath = Folder + "\\PrintIRoi.ini";
    OnLoadRoi(IsLoad, IRoiPath, IRoi);

    CString TRoiPath = Folder + "\\PrintTRoi.ini";
    OnLoadRoi(IsLoad, TRoiPath, TRoi);

    // Image Load & Save
    // -------------------------------------------------------------------------
    try {
        CFolder File;
        CString AreaPath = Folder + "\\TrainImage.bmp";

        if (IsLoad) {
            if (File.ExistsDir(AreaPath)) {
                Train.Image.Load(CW2A(AreaPath).m_psz);
            }
        }
        else {
            Train.Image.Save(CW2A(AreaPath).m_psz);
        }

    }
    catch (EException& exc)
    {
    }

    try {
        CFolder File;
        CString AreaPath = Folder + "\\AreaFind.mch";

        if (IsLoad) {
            if (File.ExistsDir(AreaPath)) {
                AreaFind.Matcher->Load(CW2A(AreaPath).m_psz);
            }
        }
        else {
            AreaFind.Matcher->Save(CW2A(AreaPath).m_psz);
        }

        for (int k=0; k<MAX_PRINT_FONT; k++) {
			CString ks;
			ks.Format(L"%d",k);
            CString FontPath = Folder + "\\FontFind"+ks+".mch";
            if (IsLoad) {
                if (File.ExistsDir(FontPath)) {
                    FontFind[k].Matcher->Load(CW2A(FontPath).m_psz);
                }
            }
            else {
                FontFind[k].Matcher->Save(CW2A(FontPath).m_psz);
            }
        }
    }
    catch (EException& exc)
    {
    }
}



