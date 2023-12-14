#include "stdafx.h"
#include "LxOcr.h"

COcr::COcr(void)
{
	Property.ShBlobScore = 50;
	Property.TrMargin = 3;
	Property.ShMarginX = 30;
	Property.ShMarginY = 30;
	DwTrainId = false;
	DwScore = false;
	DwIRoi = true;
	SortMode = (TSortMode)0;
	ResultFindCount = 0;
	ResultFindCount = 0;
	PrevFindCount = 0;

	FindReferStringId = 0;

	ReferEnable0 = true;
	ReferEnable1 = true;
	ReferEnable2 = true;

	Property.Mode = 0;
	Property.ShMove = 20;
	Property.ShMinusX = 0;
	Property.ShMinusY = 0;

	IRoi  = new EROIBW8();
	TRoi  = new EROIBW8();
	GImage= NULL;
}
COcr::~COcr(void)
{
	delete (IRoi);
	delete (TRoi);
}
int COcr::OnTrainByTRoi(CString Simbol)
{
    int TrIndex = 10;

    if (!Simbol.Trim().GetLength()) {
        SetProcessMessage("int COcr::OnTrainByTRoi(...." );
        TrIndex = 0;
        return 0;
    }

    switch (TrIndex) {
        default : return 0;

        case 10 :
                  TrIndex++;


        case 11 :
                  TrIndex = 0;
                  TrIndex++;

        case 12 :
                  FontRoi.SetOrgX  (TRoi->GetOrgX() - Property.TrMargin  );
                  FontRoi.SetOrgY  (TRoi->GetOrgY() - Property.TrMargin  );
                  FontRoi.SetWidth (TRoi->GetWidth () + Property.TrMargin*2);
                  FontRoi.SetHeight(TRoi->GetHeight() + Property.TrMargin*2);

                  FontTrain.GImage = GImage;
                  FontTrain.TRoi = &FontRoi;
                  FontTrain.IRoi = &FontRoi;
                  FontTrain.OnTrain();

                  TrIndex++;

        case 13 :
                  TrIndex++;

        case 14 : TrIndex=0;
                  OnSaveByReferString(Simbol);
                  OnLoadByReferString();
                  AfxMessageBox(_T("등록되었습니다"));

    }
	return true;
}

int COcr::OnRoiSetting(CString Simbol)
{
    int l=0, t=0, r=0, b=0;
    int TrIndex = 11;
    int w = 0;
    int h = 0;

    switch (TrIndex) {
        default : return 0;

        case 10 :
                  w = GImage->GetWidth();
                  h = GImage->GetHeight();

                  Trace.GImage = GImage;
                  Trace.IRoi = IRoi;

                  Trace.OnExecute();

                  if (Trace.BlobCount<1) {
                      SetProcessMessage("int COcr::OnTrain(AnsiString Simbol)>>if (Trace.BlobCount<1)" );
                      TrIndex = 0;
                      return 0;
                  }

                  if (Trace.BlobCount!=Simbol.Trim().GetLength()) {
                      SetProcessMessage("int COcr::OnTrain(AnsiString Simbol)>>if (Trace.BlobCount!=Simbol.Trim().Length())" );
                      TrIndex = 0;
                      return 0;
                  }

                  TrIndex++;

        case 11 :
                  Area.Body.l = 10000;
                  Area.Body.t = 10000;
                  Area.Body.r = 0;
                  Area.Body.b = 0;

                  for (int i=0; i<ReferString.Trim().GetLength(); i++) {
					  if (ReferString.Mid(i+1, 1)=="?") continue; // Mid(int First, int Count) First:추출을 시작할 문자열의 위치, Count: 추출할 문자의 수 
                      Area.Body.l = Result[i].Rect.l<Area.Body.l ? Result[i].Rect.l : Area.Body.l;
                      Area.Body.t = Result[i].Rect.t<Area.Body.t ? Result[i].Rect.t : Area.Body.t;
                      Area.Body.r = Result[i].Rect.r>Area.Body.r ? Result[i].Rect.r : Area.Body.r;
                      Area.Body.b = Result[i].Rect.b>Area.Body.b ? Result[i].Rect.b : Area.Body.b;
                  }

                  TrIndex++;


        case 12 :
                  AreaRoi.SetOrgX  (Area.Body.l            );
                  AreaRoi.SetOrgY  (Area.Body.t            );
                  AreaRoi.SetWidth (Area.Body.r-Area.Body.l);
                  AreaRoi.SetHeight(Area.Body.b-Area.Body.t);

                  AreaFind.GImage = GImage;
                  AreaFind.TRoi = &AreaRoi;
                  AreaFind.IRoi = IRoi;

                  AreaFind.OnTrain();
                  TrIndex++;

        case 13 :
                  for (int i=0; i<ReferString.Trim().GetLength(); i++) {
					  if (ReferString.Mid(i+1, 1)=="?") continue; // Mid(int First, int Count) First:추출을 시작할 문자열의 위치, Count: 추출할 문자의 수 

                      Area.Rect[i].l = Result[i].Rect.l;
                      Area.Rect[i].t = Result[i].Rect.t;
                      Area.Rect[i].r = Result[i].Rect.r;
                      Area.Rect[i].b = Result[i].Rect.b;

                      //좌측 상단기준으로 옷셉값 적용
                      //--------------------------------------------------------
                      //Area.Gx[i] = (Area.Rect[i].l-Area.Body.l);
                      //Area.Gy[i] = (Area.Rect[i].t-Area.Body.t);

                      //중심 기준으로 옷셉값 적용
                      //--------------------------------------------------------
                      Area.Gx[i] = Area.Rect[i].l-Area.Body.Point().x;
                      Area.Gy[i] = Area.Rect[i].t-Area.Body.Point().y;

                  }

                  TrIndex++;

        case 14 :
                  Train.ReferString = Simbol;
                  TrIndex++;

        case 15 : TrIndex=0;
                  return true;

    }
}

int COcr::OnManualRoiSetting(CString Simbol)
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check COcr Train GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check COcr Train GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check COcr Train GImage Height Size"    )); return false; }

    int l=0, t=0, r=0, b=0;
    int TrIndex = 11;
    int w = 0;
    int h = 0;

    ReferString = Simbol;
    Property.ReferString0 = Simbol;

    switch (TrIndex) {
        default : return 0;

        case 10 :
                  TrIndex++;

        case 11 :
                  TrIndex++;


        case 12 :
                  Area.Body.l = TRoi->GetOrgX();
                  Area.Body.t = TRoi->GetOrgY();
                  Area.Body.r = TRoi->GetOrgX() + TRoi->GetWidth ();
                  Area.Body.b = TRoi->GetOrgY() + TRoi->GetHeight();

				  //Ocr Image를 Match 알고리즘에 넘겨준다 
                  AreaFind.GImage = GImage;
                  AreaFind.TRoi = TRoi;
                  AreaFind.IRoi = IRoi;

                  AreaFind.OnTrain();
                  TrIndex++;

        case 13 :
			//ORoi데이터를 어디서 가져오는지 알수없음...나중에 확인 필요함 
                  for (int i=0; i<ReferString.Trim().GetLength(); i++) {
					  if (ReferString.Mid(i+1, 1)=="?") continue; // Mid(int First, int Count) First:추출을 시작할 문자열의 위치, Count: 추출할 문자의 수 

                      Area.Rect[i].l = ORoi[i].GetOrgX();
                      Area.Rect[i].t = ORoi[i].GetOrgY();
                      Area.Rect[i].r = ORoi[i].GetOrgX() + ORoi[i].GetWidth ();
                      Area.Rect[i].b = ORoi[i].GetOrgY() + ORoi[i].GetHeight();

                      //좌측 상단기준으로 옷셉값 적용
                      //--------------------------------------------------------
                      //Area.Gx[i] = (Area.Rect[i].l-Area.Body.l);
                      //Area.Gy[i] = (Area.Rect[i].t-Area.Body.t);

                      //중심 기준으로 옷셉값 적용
                      //--------------------------------------------------------
                      Area.Gx[i] = Area.Rect[i].l-Area.Body.Point().x;
                      Area.Gy[i] = Area.Rect[i].t-Area.Body.Point().y;
                  }

                  TrIndex++;

        case 14 :
                  Train.ReferString = Simbol;
                  IsTrain = true;
                  OnLoadByReferString();
                  OnExecuteBySrh();
                  TrIndex++;

        case 15 : TrIndex=0;
                  return true;

    }
}

int COcr::OnExecuteBySrh()
{
    CString ReferString = Property.ReferString0;
    CString TrainChar;
    int ReferStringCount = 0;
    float PvScore = 0;
    float CuScore = 0;
    bool IsFind = false;
    IsPass = false;
    ReadString = "";
    ResultFindCount = 0;

    IpIndex = 0;

    TFPoint OPoint(0, 0);
    TFPoint TPoint(0, 0);
    TFPoint RPoint(0, 0);
    float AreaAngle = 0.0f;

    if (IpIndex==0) {
        if (ReferString!=Train.ReferString) IsTrain = false;

        DRoi.SetOrgX  (IRoi->GetOrgX  ());
        DRoi.SetOrgY  (IRoi->GetOrgY  ());
        DRoi.SetWidth (IRoi->GetWidth ());
        DRoi.SetHeight(IRoi->GetHeight());

        if (IsMaunalTrain) IpIndex = 20;
        else if (!IsTrain) IpIndex = 10;
        else IpIndex = 20;
    }

    switch (IpIndex) {
        default : IpIndex = 0; return 0;


        case 10 :
                  DwFRoi = false;

                  if (!ReferString.GetLength()) return false;

                  if      (Property.Mode==0) { if (!OnExecuteBlob(ReferString)) return false; }
                  else if (Property.Mode==1) { if (!OnExecuteSort(ReferString)) return false; }
                  else if (Property.Mode==2) { if (!OnExecuteRoi (ReferString)) return false; }
                  else return false;

                  IpIndex++;

        case 11 : if (!OnRoiSetting(ReferString)) return false;
                  IpIndex=0;
                  IsTrain = true;
                  return true;

        case 20 : //Match Algorithm 이용하여 Marking 영역부터 찾는다 
                  AreaFind.GImage = GImage;
                  AreaFind.TRoi = &AreaRoi;
                  AreaFind.IRoi = IRoi;

                  AreaFind.SetMinScore(0.1);
                  AreaFind.SetMaxPositions(1);
                  AreaFind.SetMaxAngle(10.0);
                  AreaFind.SetMinAngle(-10.0);

                  if (!AreaFind.OnExecute()) return 0;

                  //RectMatchScore = AreaFind.GetResultScore(0);
                  //IsRectMatch = (RectMatchScore>=Property.PassRectScore) ? true : false;
                  //FindAreaCount = AreaFind.FFindCount;

                  IsTrain = true;
                  IpIndex++;

        case 21 :
                  ReferStringCount = ReferString.Trim().GetLength();
                  for (int k=0; k<ReferStringCount; k++) {
                      PvScore = 0;
                      IsFind = false;
                      Result[k].IsPass = false;
                      Result[k].Score  = 0.0f;
                      Result[k].Rect.l = 0.0f;
                      Result[k].Rect.t = 0.0f;
                      Result[k].Rect.r = 0.0f;
                      Result[k].Rect.b = 0.0f;
                      Result[k].TrainId  = -1;

                      CString ReferChar = ReferString.Trim().Mid(k+1, 1);
                      if (ReferChar=="?") {
                          ResultFindCount++;
                          Result[k].IsPass  = true;
                          Result[k].Simbol  = ReferChar;
                          //Result[k].Rect = Trace.Blob[k].Rect;
                          Result[k].Score = 0;
                          Result[k].TrainId = -1;
                          ReadString = ReadString+ReferChar;
                          continue;
                      }

                      for (int i=0; i<Train.Count; i++) {
                          TrainChar = Train.Simbol[i];;

                          if (TrainChar.Trim()!=ReferChar) continue;

                          float Ax = Area.Gx[k];
                          float Ay = Area.Gy[k];

                          //좌측 상단기준으로 옷셉값 적용
                          //--------------------------------------------------------
                          //FontRoi.SetOrgX  (AreaFind.Result[0].FRect.l + Ax - Property.ShMarginX);
                          //FontRoi.SetOrgY  (AreaFind.Result[0].FRect.t + Ay - Property.ShMarginY);
                          //FontRoi.SetWidth (Area.Rect[k].r - Area.Rect[k].l + Property.ShMarginX*2);
                          //FontRoi.SetHeight(Area.Rect[k].b - Area.Rect[k].t + Property.ShMarginY*2);

                          //중심 기준으로 옷셉값 적용
                          //--------------------------------------------------------
                          OPoint.x = AreaFind.Result[0].FRect.Point().x;
                          OPoint.y = AreaFind.Result[0].FRect.Point().y;
                          TPoint.x = OPoint.x + Ax;
                          TPoint.y = OPoint.y + Ay;
                          AreaAngle = AreaFind.GetResultAngle(0);

                          RPoint = Math.OnRotate(OPoint, TPoint, -1*AreaAngle);

                          FontRoi.SetOrgX  (RPoint.x - Property.ShMarginX);
                          FontRoi.SetOrgY  (RPoint.y - Property.ShMarginY);
                          FontRoi.SetWidth (Area.Rect[k].r - Area.Rect[k].l + Property.ShMarginX*2);
                          FontRoi.SetHeight(Area.Rect[k].b - Area.Rect[k].t + Property.ShMarginY*2);

                          FRoi[i] = FontRoi;
                          DwFRoi = true;

                          FontFind[i].GImage = GImage;
                          FontFind[i].IRoi = &FontRoi;


#ifdef USING_FIND
                          FontFind[i].SetMaxInstances(1);
#endif

#ifdef USING_MATCH
                          FontFind[i].SetMinScore(0.1);
                          FontFind[i].SetMaxAngle(10.0);
                          FontFind[i].SetMinAngle(-10.0);
                          FontFind[i].SetMaxPositions(1);
#endif

                          bool r = FontFind[i].OnExecute();

                          // 같은 문자가 동시에 두자 이상 존재 할 경우 대비
                          //----------------------------------------------------
                          int FindCount = FontFind[i].FFindCount;
                          int RstId = 0;

                          if (r) {
                              IsFind = true;
                              CuScore = FontFind[i].GetResultScore(RstId);
                              if (CuScore>PvScore) {
                                  PvScore = CuScore;
                                  Result[k].TrainId = i;
                                  Result[k].Rect = FontFind[i].Result[RstId].FRect;
                                  Result[k].Score   = CuScore;
                                  Result[k].IsPass  = (PvScore>=Property.PassBlobScore) ? true : false;
                                  Result[k].Simbol  = ReferChar;
                              }
                          }
                      }

                      if (IsFind) ResultFindCount++;
                      if (Result[k].IsPass) ReadString = ReadString+ReferChar;
                      else ReadString = ReadString = ReadString+"?";
                  }


        case 22 :
                  IpIndex++;

        case 23 :
                  IpIndex = 0;
    }

    IsPass = true;

    return 1;
}

int COcr::OnExecute()
{
	if (GImage==NULL         ) { AfxMessageBox(_T("Check COcr Execute GImage Memory Allocate")); return false; }
	if (GImage->GetWidth()<1 ) { AfxMessageBox(_T("Check COcr Execute GImage Width Size"     )); return false; }
	if (GImage->GetHeight()<1) { AfxMessageBox(_T("Check COcr Execute GImage Height Size"    )); return false; }

    OnExecuteBySrh();
    FindReferStringId=0;
    IsPass = (Property.ReferString0.Trim()==ReadString);
    return IsPass;

    /*
    if (Property.ReferString0.Length())
        if (Property.ReferString1.Length())
            if (Property.ReferString2.Length())
                SetProcessMessage("int COcr::OnExecute()>>if (Property.ReferString0.Length()) ...." );


    bool On0 = ReferEnable0;
    bool On1 = ReferEnable1;
    bool On2 = ReferEnable2;

    ReferEnable0 = true;
    ReferEnable1 = true;
    ReferEnable2 = true;

    if (Property.ReferString0.Length() && On0) if (OnExecuteBySrh()) { FindReferStringId=0; IsPass = (ReferString.Trim()==ReadString); if (IsPass) return true; }
    if (Property.ReferString1.Length() && On1) if (OnExecuteBySrh()) { FindReferStringId=1; IsPass = (ReferString.Trim()==ReadString); if (IsPass) return true; }
    if (Property.ReferString2.Length() && On2) if (OnExecuteBySrh()) { FindReferStringId=2; IsPass = (ReferString.Trim()==ReadString); if (IsPass) return true; }
    */

    return false;
}

/*
int COcr::OnCompare(AnsiString RChar, JRect Rect)
{
    for (int i=0; i<Train.Count; i++) {
        FontFind[i].FFindCount = 0;
    }

    int IpIndex = 10;
    double PvScore = 0;
    double CuScore = 0;

    AnsiString TrainChar = "";
    bool IsFind = false;

    switch (IpIndex) {
        default : IpIndex = 0; return 0;

        case 10 :
                  IpIndex++;

        case 11 :
                  PvScore = 0;
                  IsFind = false;
                  if (RChar=="?") return false;

                  for (int i=0; i<Train.Count; i++) {
                      TrainChar = Train.Simbol[i];;

                      if (TrainChar.Trim()!=RChar) continue;

                      FontRoi.SetOrgX  (Rect.l              - Property.ShMargin  );
                      FontRoi.SetOrgY  (Rect.t              - Property.ShMargin  );
                      FontRoi.SetWidth (Rect.r-Trace.Rect.l + Property.ShMargin*2);
                      FontRoi.SetHeight(Rect.b-Trace.Rect.t + Property.ShMargin*2);

                      FontFind[i].GImage = GImage;
                      FontFind[i].IRoi = &FontRoi;

                      FontFind[i].SetMaxAngle(3.0);
                      FontFind[i].SetMinAngle(-3.0);
                      FontFind[i].SetMaxPositions(1);

                      bool r = FontFind[i].OnExecute();

                      if (r) {
                          IsFind = true;
                          CuScore = FontFind[i].GetResultScore(0);
                          if (CuScore>PvScore) {
                              PvScore = CuScore;
                              //Result[k].TrainId = i;
                              //Result[k].Rect = FontFind[i].Result[0].FRect;
                              //Result[k].Score   = CuScore;
                              //Result[k].IsPass  = (PvScore>=Property.PassBlobScore) ? true : false;
                              //Result[k].Simbol  = ReferChar;
                          }
                      }
                  }

                  IpIndex++;


        case 12 :
                  IpIndex = 0;
                  break;

    }
    return PvScore;
}
*/

int COcr::OnExecuteBlob(CString ReferStr)
{
    ReferString = ReferStr;

    IsPass = false;
    ResultFindCount = 0;


#ifdef USING_FIND
    for (int i=0; i<Train.Count; i++) {
        FontFind[i].results.clear();
    }
#endif

#ifdef USING_MATCH
    for (int i=0; i<Train.Count; i++) {
        FontFind[i].FFindCount = 0;
    }
#endif

    int IpIndex = 10;
    bool FontFindString = false;
    double PvScore = 0;
    double CuScore = 0;

    ReadString = "";
    CString ReferChar = "";
    CString TrainChar = "";
    int ReferStringCount = 0;
    bool IsFind = false;
    bool IsBreaking = false;
    int ix,iy,iw, ih;
    int ShMove = Property.ShMove;
    int MaxMove = 8;

    switch (IpIndex) {
        default : IpIndex = 0; return 0;

        case 10 :
                  Trace.GImage = GImage;
                  //Trace.IRoi = IRoi;
                  ix = IRoi->GetOrgX  () + Property.ShMinusX;
                  iy = IRoi->GetOrgY  () + Property.ShMinusY;
                  iw = IRoi->GetWidth () - Property.ShMinusX*2;
                  ih = IRoi->GetHeight() - Property.ShMinusY*2;

                  Trace.IRoi->SetOrgX  (ix);
                  Trace.IRoi->SetOrgY  (iy);
                  Trace.IRoi->SetWidth (iw);
                  Trace.IRoi->SetHeight(ih);

                  if (ShMove<1) MaxMove = 1;
                  else MaxMove = 8;

                  for (int m=0; m<MaxMove; m++) {
                      if (m==0) { Trace.IRoi->SetOrgX(ix       ); Trace.IRoi->SetOrgY(iy       ); }
                      if (m==1) { Trace.IRoi->SetOrgX(ix+ShMove); Trace.IRoi->SetOrgY(iy       ); }
                      if (m==2) { Trace.IRoi->SetOrgX(ix-ShMove); Trace.IRoi->SetOrgY(iy       ); }
                      if (m==3) { Trace.IRoi->SetOrgX(ix       ); Trace.IRoi->SetOrgY(iy+ShMove); }
                      if (m==4) { Trace.IRoi->SetOrgX(ix       ); Trace.IRoi->SetOrgY(iy-ShMove); }
                      if (m==5) { Trace.IRoi->SetOrgX(ix+ShMove); Trace.IRoi->SetOrgY(iy+ShMove); }
                      if (m==6) { Trace.IRoi->SetOrgX(ix-ShMove); Trace.IRoi->SetOrgY(iy-ShMove); }
                      if (m==7) { Trace.IRoi->SetOrgX(ix       ); Trace.IRoi->SetOrgY(iy       ); }

                      for (int k=0; k<17; k++) {
                          int LoThHold = Trace.FLoThHold;
                          if (k== 0) Trace.FLoThHold +=  0;
                          if (k== 1) Trace.FLoThHold +=  2;
                          if (k== 2) Trace.FLoThHold -=  2;
                          if (k== 3) Trace.FLoThHold +=  4;
                          if (k== 4) Trace.FLoThHold -=  4;
                          if (k== 5) Trace.FLoThHold +=  6;
                          if (k== 6) Trace.FLoThHold -=  6;
                          if (k== 7) Trace.FLoThHold +=  8;
                          if (k== 8) Trace.FLoThHold -=  8;
                          if (k== 9) Trace.FLoThHold += 10;
                          if (k==10) Trace.FLoThHold -= 10;
                          if (k==12) Trace.FLoThHold += 12;
                          if (k==13) Trace.FLoThHold -= 12;
                          if (k==14) Trace.FLoThHold += 14;
                          if (k==15) Trace.FLoThHold -= 14;

                          if (k==16) Trace.FLoThHold =  0;


                          Trace.OnExecute();
                          Trace.FLoThHold = LoThHold;
                          if (Trace.BlobCount==ReferString.Trim().GetLength()) {
                              IsBreaking = true;
                              break;
                          }
                      }
                      if (IsBreaking) break;
                  }

                  if (Trace.BlobCount<1) { IpIndex = 0; return 0; }
                  if (Trace.BlobCount!=ReferString.Trim().GetLength()) { IpIndex = 0; return 0; }

                  IpIndex++;

        case 11 :
                  ReferStringCount = ReferString.Trim().GetLength();
                  for (int k=0; k<ReferStringCount; k++) {
                      PvScore = 0;
                      IsFind = false;
                      Result[k].IsPass = false;
                      Result[k].Score  = 0.0f;
                      Result[k].Rect.l = 0.0f;
                      Result[k].Rect.t = 0.0f;
                      Result[k].Rect.r = 0.0f;
                      Result[k].Rect.b = 0.0f;
                      Result[k].TrainId  = -1;

                      ReferChar = ReferString.Trim().Mid(k+1, 1);
                      if (ReferChar=="?") {
                          ResultFindCount++;
                          Result[k].IsPass  = true;
                          Result[k].Simbol  = ReferChar;
                          Result[k].Rect = Trace.Blob[k].Rect;
                          Result[k].Score = 0;
                          Result[k].TrainId = -1;
                          ReadString = ReadString+ReferChar;
                          continue;
                      }

                      for (int i=0; i<Train.Count; i++) {
                          TrainChar = Train.Simbol[i];;

                          if (TrainChar.Trim()!=ReferChar) continue;

                          FontRoi.SetOrgX  (Trace.Blob[k].Rect.l                      - Property.ShMarginX  );
                          FontRoi.SetOrgY  (Trace.Blob[k].Rect.t                      - Property.ShMarginY  );
                          FontRoi.SetWidth (Trace.Blob[k].Rect.r-Trace.Blob[k].Rect.l + Property.ShMarginX*2);
                          FontRoi.SetHeight(Trace.Blob[k].Rect.b-Trace.Blob[k].Rect.t + Property.ShMarginY*2);

                          FontFind[i].GImage = GImage;
                          FontFind[i].IRoi = &FontRoi;


#ifdef USING_FIND
                          FontFind[i].SetMaxInstances(1);
#endif

#ifdef USING_MATCH
                          FontFind[i].SetMinScore(0.1);
                          FontFind[i].SetMaxAngle(10.0);
                          FontFind[i].SetMinAngle(-10.0);
                          FontFind[i].SetMaxPositions(1);
#endif

                          bool r = FontFind[i].OnExecute();

                          // 같은 문자가 동시에 두자 이상 존재 할 경우 대비
                          //----------------------------------------------------
                          int FindCount = FontFind[i].FFindCount;
                          int RstId = 0;

                          if (r) {
                              IsFind = true;
                              CuScore = FontFind[i].GetResultScore(RstId);
                              if (CuScore>PvScore) {
                                  PvScore = CuScore;
                                  Result[k].TrainId = i;
                                  Result[k].Rect = FontFind[i].Result[RstId].FRect;
                                  Result[k].Score   = CuScore;
                                  Result[k].IsPass  = (PvScore>=Property.PassBlobScore) ? true : false;
                                  Result[k].Simbol  = ReferChar;
                              }
                          }
                      }

                      if (IsFind) ResultFindCount++;
                      if (Result[k].IsPass) ReadString = ReadString+ReferChar;
                      else ReadString = ReadString = ReadString+"?";
                  }
                  IpIndex++;

        case 12 :
                  IpIndex = 0;
                  break;

    }

    IsPass = (ReferString.Trim()==ReadString) ? true : false;
    return IsPass;

//    for (int k=0; k<ReferStringCount; k++) {
//        AnsiString ReferChar = ReferString.Trim().SubString(k+1, 1);
//        if (ReferChar=="?") {
//            Result[k].Simbol = "?";
//            continue;
//        }
//        if (Result[k].Simbol!=ReferChar) {
//            //Result[k].Simbol = "?";
//            Result[k].IsPass = false;
//        }
//    }
//
//    return IsPass;
}

int COcr::OnExecuteRoi(CString ReferStr)
{
    ReferString = ReferStr;

    IsPass = false;
    ResultFindCount = 0;


#ifdef USING_FIND
    for (int i=0; i<Train.Count; i++) {
        FontFind[i].results.clear();
    }
#endif

#ifdef USING_MATCH
    for (int i=0; i<Train.Count; i++) {
        FontFind[i].FFindCount = 0;
    }
#endif

    int IpIndex = 10;
    bool FontFindString = false;
    //if (Property.ShMargin<10) Property.ShMargin = 10;
    double PvScore = 0;
    double CuScore = 0;

    ReadString = "";
    CString ReferChar = "";
    CString TrainChar = "";
    int ReferStringCount = 0;
    bool IsFind = false;

    switch (IpIndex) {
        default : IpIndex = 0; return 0;

        case 10 :
                  IpIndex++;

        case 11 :
                  ReferStringCount = ReferString.Trim().GetLength();
                  for (int k=0; k<ReferStringCount; k++) {
                      PvScore = 0;
                      IsFind = false;
                      Result[k].IsPass = false;
                      Result[k].Score  = 0.0f;
                      Result[k].Rect  .l = 0.0f;
                      Result[k].Rect  .t = 0.0f;
                      Result[k].Rect  .r = 0.0f;
                      Result[k].Rect  .b = 0.0f;

                      ReferChar = ReferString.Trim().Mid(k+1, 1);

                      if (ReferChar=="?") {
                          ResultFindCount++;
                          Result[k].IsPass  = true;
                          Result[k].Simbol  = ReferChar;
                          //Result[k].Rect = Trace.Blob[k].Rect;
                          Result[k].Score = 0;
                          Result[k].TrainId = -1;
                          ReadString = ReadString+ReferChar;
                          continue;
                      }

                      for (int i=0; i<Train.Count; i++) {
                          TrainChar = Train.Simbol[i];;

                          if (TrainChar.Trim()!=ReferChar) continue;

                          FontRoi.SetOrgX  (IRoi->GetOrgX  () - Property.ShMarginX  );
                          FontRoi.SetOrgY  (IRoi->GetOrgY  () - Property.ShMarginY  );
                          FontRoi.SetWidth (IRoi->GetWidth () + Property.ShMarginX*2);
                          FontRoi.SetHeight(IRoi->GetHeight() + Property.ShMarginY*2);

                          FontFind[i].GImage = GImage;
                          FontFind[i].IRoi = &FontRoi;

                          //FontFind[i].SetMinScore(Property.ShBlobScore);
                          FontFind[i].SetMinScore(0.5);


#ifdef USING_FIND
                          FontFind[i].SetMaxInstances(1);
#endif

#ifdef USING_MATCH
                          FontFind[i].SetMaxAngle(5.0);
                          FontFind[i].SetMinAngle(-5.0);
                          FontFind[i].SetMaxPositions(3);
#endif

                          bool r = FontFind[i].OnExecute();

                          // 같은 문자가 동시에 두자 이상 존재 할 경우 대비
                          //----------------------------------------------------
                          int FindCount = FontFind[i].FFindCount;
                          int RstId = 0;

                          if (r) {
                              IsFind = true;
                              CuScore = FontFind[i].GetResultScore(RstId);
                              if (CuScore>PvScore) {
                                  PvScore = CuScore;
                                  Result[k].TrainId = i;
                                  Result[k].Rect   .l = FontFind[i].Result[RstId].FRect.l;
                                  Result[k].Rect   .t = FontFind[i].Result[RstId].FRect.t;
                                  Result[k].Rect   .r = FontFind[i].Result[RstId].FRect.r;
                                  Result[k].Rect   .b = FontFind[i].Result[RstId].FRect.b;
                                  Result[k].Score   = CuScore;
                                  Result[k].IsPass  = (PvScore>=Property.PassBlobScore) ? true : false;
                                  Result[k].Simbol  = ReferChar;
                              }
                          }
                      }

                      if (IsFind) ResultFindCount++;
                      if (Result[k].IsPass) ReadString = ReadString+ReferChar;
                      else ReadString = ReadString = ReadString+"?";
                  }
                  IpIndex++;

        case 12 :
                  IpIndex = 0;
                  break;

    }

    IsPass = true;

    for (int k=0; k<ReferStringCount; k++) {
        CString ReferChar = ReferString.Trim().Mid(k+1, 1);
        if (ReferChar=="?") {
            Result[k].Simbol = "?";
            continue;
        }
        if (Result[k].Simbol!=ReferChar) {
            //Result[k].Simbol = "?";
            Result[k].IsPass = false;
        }
    }

    return IsPass;
}

int COcr::OnExecuteSort(CString ReferStr)
{
    ReferString = ReferStr;

    IsPass = false;
    ResultFindCount = 0;


#ifdef USING_FIND
    for (int i=0; i<Train.Count; i++) {
        FontFind[i].results.clear();
    }
#endif

#ifdef USING_MATCH
    for (int i=0; i<Train.Count; i++) {
        FontFind[i].FFindCount = 0;
    }
#endif

    int IpIndex = 10;
    bool FontFindString = false;
    //Property.ShMarginX = 20;
    //Property.ShMarginY = 20;

    //if (Property.ShMargin<10) Property.ShMargin = 10;
    double PvScore = 0;
    double CuScore = 0;

    ReadString = "";
    CString ReferChar = "";
    CString TrainChar = "";
    int ReferStringCount = 0;
    bool IsFind = false;
    PrevFindCount = 0;
    CString FullString = "";
    int ReferLength = 0;
    int FstCharNum = 0;

    switch (IpIndex) {
        default : IpIndex = 0; return 0;

        case 10 :
                  IpIndex++;

        case 11 :
                  ReferStringCount = ReferString.Trim().GetLength();
                  for (int k=0; k<ReferStringCount; k++) {
                      PvScore = 0;
                      IsFind = false;
                      Result[k].IsPass = false;
                      Result[k].Score  = 0.0f;
                      Result[k].Rect  .l = 0.0f;
                      Result[k].Rect  .t = 0.0f;
                      Result[k].Rect  .r = 0.0f;
                      Result[k].Rect  .b = 0.0f;

                      ReferChar = ReferString.Trim().Mid(k+1, 1);

                      for (int i=0; i<Train.Count; i++) {
                          TrainChar = Train.Simbol[i];;

                          if (TrainChar.Trim()!=ReferChar) continue;

                          FontRoi.SetOrgX  (IRoi->GetOrgX  () - Property.ShMarginX  );
                          FontRoi.SetOrgY  (IRoi->GetOrgY  () - Property.ShMarginY  );
                          FontRoi.SetWidth (IRoi->GetWidth () + Property.ShMarginX*2);
                          FontRoi.SetHeight(IRoi->GetHeight() + Property.ShMarginY*2);

                          FontFind[i].GImage = GImage;
                          FontFind[i].IRoi = &FontRoi;

                          FontFind[i].SetMinScore((float)Property.ShBlobScore/100.0);
                          //FontFind[i].SetMinScore(0.5);


#ifdef USING_FIND
                          FontFind[i].SetMaxInstances(1);
#endif

#ifdef USING_MATCH
                          FontFind[i].SetMaxAngle(5.0);
                          FontFind[i].SetMinAngle(-5.0);
                          FontFind[i].SetMaxPositions(3);
#endif

                          IsFind = FontFind[i].OnExecute();

                          // 같은 문자가 동시에 두자 이상 존재 할 경우 대비
                          //----------------------------------------------------
                          int FindCount = FontFind[i].FFindCount;
                          int RstId = 0;
                          bool IsNew = true;

                          for (int j=0; j<FindCount; j++) {
                              Prev[PrevFindCount].Rect.l = FontFind[i].Result[j].FRect.l;
                              Prev[PrevFindCount].Rect.t = FontFind[i].Result[j].FRect.t;
                              Prev[PrevFindCount].Rect.r = FontFind[i].Result[j].FRect.r;
                              Prev[PrevFindCount].Rect.b = FontFind[i].Result[j].FRect.b;

                              Prev[PrevFindCount].Simbol = ReferChar;
                              Prev[PrevFindCount].Score  = FontFind[i].GetResultScore(j);
                              PrevFindCount++;
                          }
                      }

                  }
                  IpIndex++;



        case 12 : ResultFindCount = 0;

                  for (int k=0; k<PrevFindCount; k++) {

                      //if (Prev[k].Score<Property.PassBlobScore) continue;

                      bool IsPos = false;
                      for (int m=0; m<ResultFindCount; m++) {
                          if (abs(Prev[k].Rect.l-Result[m].Rect.l)<20)
                              if (abs(Prev[k].Rect.t-Result[m].Rect.t)<20) {
                                  IsPos = true;

                                  if (Prev[k].Score>Result[m].Score) {
                                      Result[m].Rect  .l = Prev[k].Rect.l;
                                      Result[m].Rect  .t = Prev[k].Rect.t;
                                      Result[m].Rect  .r = Prev[k].Rect.r;
                                      Result[m].Rect  .b = Prev[k].Rect.b;
                                      Result[m].Score  = Prev[k].Score;
                                      Result[m].IsPass = (Prev[k].Score>=Property.PassBlobScore) ? true : false;
                                      Result[m].Simbol = Prev[k].Simbol;
                                  }
                              }
                      }

                      if (!IsPos) {
                          Result[ResultFindCount].Rect  .l = Prev[k].Rect.l;
                          Result[ResultFindCount].Rect  .t = Prev[k].Rect.t;
                          Result[ResultFindCount].Rect  .r = Prev[k].Rect.r;
                          Result[ResultFindCount].Rect  .b = Prev[k].Rect.b;
                          Result[ResultFindCount].Score  = Prev[k].Score;
                          Result[ResultFindCount].IsPass = (Prev[k].Score>=Property.PassBlobScore) ? true : false;
                          //if (Prev[k].Score>=Property.PassBlobScore) Result[ResultFindCount].Simbol = Prev[k].Simbol;
                          //else Result[ResultFindCount].Simbol = "?";

                          Result[ResultFindCount].Simbol = Prev[k].Simbol;
                          ResultFindCount++;
                      }
                  }

                  IpIndex++;

        case 13 :
                  OnSortRect();

                  for (int k=0; k<ResultFindCount; k++) {
                      ReadString = ReadString+Result[k].Simbol;
                  }
                  IpIndex++;

        case 14 :
                  FullString = "";
                  for (int k=0; k<ResultFindCount; k++) {
                      FullString = FullString+Result[k].Simbol;
                  }

                  ReferLength = ReferString.Trim().GetLength();
                  FstCharNum = FullString.Find(ReferString); // AnsiString Pos와 같은 기능으로 CString Find함수를 사용한다 

                  IpIndex++;

        case 15 :
                  if (FstCharNum>0) {
                      ResultFindCount = ReferLength;
                      ReadString = "";
                      for (int k=0; k<ReferLength; k++) {
                          Result[k].Rect  .l = Result[k+FstCharNum-1].Rect  .l;
                          Result[k].Rect  .t = Result[k+FstCharNum-1].Rect  .t;
                          Result[k].Rect  .r = Result[k+FstCharNum-1].Rect  .r;
                          Result[k].Rect  .b = Result[k+FstCharNum-1].Rect  .b;
                          Result[k].Score    = Result[k+FstCharNum-1].Score   ;
                          Result[k].IsPass   = Result[k+FstCharNum-1].IsPass  ;
                          Result[k].Simbol   = Result[k+FstCharNum-1].Simbol  ;
                      }
                      for (int k=0; k<ReferLength; k++) {
                          ReadString = ReadString+Result[k].Simbol;
                      }
                  }
                  IpIndex++;

        case 16 :
                  if (ReadString.GetLength()>=ReferLength) {
                      PvScore = 0;
                      ReadString = "";
                      for (int k=0; k<ReferLength; k++) {
                          ReferChar = ReferString.Trim().Mid(k+1, 1);
                          CString ResultChar = Result[k].Simbol;

                          if (ReferChar==ResultChar) continue;

                          for (int m=0; m<PrevFindCount; m++) {
                              if (abs(Prev[m].Rect.l-Result[k].Rect.l)<20) {
                                  if (abs(Prev[m].Rect.t-Result[k].Rect.t)<20) {

                                      if (Prev[m].Simbol!=ReferChar) continue;

                                      if (Prev[m].Score>PvScore) {
                                          PvScore = Prev[m].Score;
                                          Result[k].Rect  .l = Prev[m].Rect.l;
                                          Result[k].Rect  .t = Prev[m].Rect.t;
                                          Result[k].Rect  .r = Prev[m].Rect.r;
                                          Result[k].Rect  .b = Prev[m].Rect.b;
                                          Result[k].Score  = Prev[m].Score;
                                          Result[k].IsPass = (Prev[m].Score>=Property.PassBlobScore) ? true : false;
                                          Result[k].Simbol = Prev[m].Simbol;
                                      }
                                  }
                              }
                          }
                      }
                      ResultFindCount = ReferLength;
                      for (int k=0; k<ReferLength; k++) {
                          ReadString = ReadString+Result[k].Simbol;
                      }
                  }
                  IpIndex++;

        case 17 :
                  IpIndex = 0;
                  break;

    }

    IsPass = true;

    for (int k=0; k<ReferLength; k++) {
        CString ReferChar = ReferString.Trim().Mid(k+1, 1);
        if (ReferChar=="?") {
            Result[k].Simbol = "?";
            continue;
        }
        if (Result[k].Simbol!=ReferChar) {
            //Result[k].Simbol = "?";
            Result[k].IsPass = false;
        }
    }

    return IsPass;
}

TSortMode __SORTMODE;
int OnSorting(const void *b1, const void *b2)
{
    const int RIGHT_POS =  1;
    const int LEFT_POS  = -1;
    const int ORIG_POS  =  0;

    if (b1==NULL) return ORIG_POS;
    if (b2==NULL) return ORIG_POS;

    int rg = fabs(((COcr::TResult *)b1)->Rect.Point().y - ((COcr::TResult *)b2)->Rect.Point().y);
    int r1 = ((COcr::TResult *)b1)->Rect.Point().y - ((COcr::TResult *)b2)->Rect.Point().y;
    int r2 = ((COcr::TResult *)b1)->Rect.Point().x - ((COcr::TResult *)b2)->Rect.Point().x;

    bool IsYLine = ((COcr::TResult *)b2)->Rect.t <= ((COcr::TResult *)b1)->Rect.b && ((COcr::TResult *)b2)->Rect.b >= ((COcr::TResult *)b1)->Rect.t;
    bool IsXLine = ((COcr::TResult *)b2)->Rect.l <= ((COcr::TResult *)b1)->Rect.r && ((COcr::TResult *)b2)->Rect.r >= ((COcr::TResult *)b1)->Rect.l;

    //bool IsYLine = ((COcr::TResult *)b2)->Rect.Point().y <= ((COcr::TResult *)b1)->Rect.Point().y && ((COcr::TResult *)b2)->Rect.Point().y >= ((COcr::TResult *)b1)->Rect.Point().y;
    //bool IsXLine = ((COcr::TResult *)b2)->Rect.Point().x <= ((COcr::TResult *)b1)->Rect.Point().x && ((COcr::TResult *)b2)->Rect.Point().x >= ((COcr::TResult *)b1)->Rect.Point().x;

    switch (__SORTMODE) {
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

void COcr::OnSortRect()
{
    __SORTMODE = (TSortMode)SortMode;
    qsort((void *)Result, ResultFindCount, sizeof(Result[0]), OnSorting);
}

void COcr::OnPaint(TDrawTool Tool)
{
    if (Tool.Handle==NULL) return;

    HGdi *Gdi = new HGdi(Tool.Handle);

    Gdi->FVx = Tool.FVx;
    Gdi->FVy = Tool.FVy;
    Gdi->FHs = Tool.FHs;
    Gdi->FVs = Tool.FVs;

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

    int ShMargin = 0;
    CString Score = "";
    int ReferCount = 0;

    switch(PaIndex) {
        case 10 :
                  l = IRoi->GetOrgX();
                  t = IRoi->GetOrgY();;

                  Gdi->CText.Color = IsPass ? YELLOW_COLOR : RED_COLOR;
                  Gdi->OnText(l, t-10, LPSTR(LPCTSTR(ReadString)));

                  if (DwIRoi) {
                      Gdi->CPen.Color = LIME_COLOR;

                      l = DRoi.GetOrgX();
                      t = DRoi.GetOrgY();
                      r = DRoi.GetOrgX()+DRoi.GetWidth();
                      b = DRoi.GetOrgY()+DRoi.GetHeight();

                      Gdi->OnMove(l, t);
                      Gdi->OnLine(r, t);
                      Gdi->OnLine(r, b);
                      Gdi->OnLine(l, b);
                      Gdi->OnLine(l, t);

					  Gdi->CPen.Color = AQUA;

                      if (DwFRoi) {
                          for (int i=0; i<Train.Count; i++) {
                              if (Train.Simbol[i]=="?") continue;

                              l = FRoi[i].GetOrgX();
                              t = FRoi[i].GetOrgY();
                              r = FRoi[i].GetOrgX()+FRoi[i].GetWidth();
                              b = FRoi[i].GetOrgY()+FRoi[i].GetHeight();

                              Gdi->OnMove(l, t);
                              Gdi->OnLine(r, t);
                              Gdi->OnLine(r, b);
                              Gdi->OnLine(l, b);
                              Gdi->OnLine(l, t);
                          }
                      }
                  }

                  //if (Trace.BlobCount!=ReferString.Trim().Length()) {
                  if (!IsPass) {
                      Trace.OnPaintBlobRect(Tool);
                      Trace.OnPaintBlobId(Tool);
                  }
                  else {
                      for (int i=0; i<ResultFindCount; i++) {
                          Gdi->CText.Color = YELLOW_COLOR;
                          Gdi->CPen.Color = Result[i].IsPass ? LIME_COLOR : RED_COLOR;
                          if (Result[i].Simbol=="?") continue;

                          int l = Result[i].Rect.l - ShMargin  ;
                          int t = Result[i].Rect.t - ShMargin  ;
                          int r = Result[i].Rect.r + ShMargin*2;
                          int b = Result[i].Rect.b + ShMargin*2;

                          int x = Result[i].Rect.Center().x;
                          int y = Result[i].Rect.Center().y;

                          Gdi->OnMove(l, t);
                          Gdi->OnLine(r, t);
                          Gdi->OnLine(r, b);
                          Gdi->OnLine(l, b);
                          Gdi->OnLine(l, t);

                          if (DwScore) {
                              CString Score ="";
							  Score.Format(L"%3.1f", Result[i].Score);
                              Gdi->OnText(l, b+5, LPSTR(LPCTSTR(Score)));
                          }
                          if (DwTrainId) {
							  CString Id="";
                              Id.Format(L"%d", Result[i].TrainId);
                              Gdi->OnText(l, t-5, LPSTR(LPCTSTR(Id)));
                          }

                          CString Simbol = LPSTR(LPCTSTR(Result[i].Simbol));
                          Gdi->OnText(x, y, LPSTR(LPCTSTR(Simbol)));
                      }
                  }
                  break;

        case 20 :
                  break;

        case 30 :
                  break;

        case 40 :
                  break;

        case 50 :

#ifdef USING_FIND
                  HDC hdc = GetDC(Handle);
                  for (int i=0; i<Train.Count; i++) {
                      int Count = FontFind[i].results.size();
                      if (Count>0) {
                          FontFind[i].results[0].SetDrawBoundingBox(true);
                          FontFind[i].results[0].SetDrawCenter(true);
                          FontFind[i].results[0].SetDrawFeaturePoints(true);
                          FontFind[i].results[0].Draw(hdc, FHs, FVs);
                      }
                  }
                  ReleaseDC(Handle, hdc);
#endif

#ifdef USING_MATCH
                  //HDC hdc = GetDC(Tool.Table->Handle);
                  for (int i=0; i<Train.Count; i++) {
                      FontFind[i].OnPaint(Tool);
                  }
                  //ReleaseDC(Tool.Table->Handle, hdc);
#endif

    }

    delete Gdi;
}

void COcr::OnLoadRoi(bool IsLoad, CString Path, EROIBW8 *lpRoi)
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
void COcr::OnLoad(bool IsLoad, CString Folder)
{
    CIFile Ini;

    if (Folder.Trim()=="") return;
    CString Path = Folder + "\\Ocr.ini";
	CString Data="";
    for (int k=0; k<MAX_OCR_FONT; k++) {
		Data.Format(L"FTrainImageWidth:%d" ,k);
        Ini.Load(IsLoad, Path, "FontFind", Data, &FontFind[k].FTrainImageWidth );

		Data.Format(L"FTrainImageHeight:%d" ,k);
        Ini.Load(IsLoad, Path, "FontFind", Data, &FontFind[k].FTrainImageHeight);
    }

    Ini.Load(IsLoad, Path, "Trace", "FMinX"     , &Trace.FMinX    );
    Ini.Load(IsLoad, Path, "Trace", "FMinY"     , &Trace.FMinY    );
    Ini.Load(IsLoad, Path, "Trace", "FHiThHold" , &Trace.FHiThHold);
    Ini.Load(IsLoad, Path, "Trace", "FLoThHold" , &Trace.FLoThHold);

    Ini.Load(IsLoad, Path, "Ignore", "FMinArea  ", &Trace.FMinArea  );
    Ini.Load(IsLoad, Path, "Ignore", "FMaxArea  ", &Trace.FMaxArea  );
    Ini.Load(IsLoad, Path, "Ignore", "FMinHeight", &Trace.FMinHeight);
    Ini.Load(IsLoad, Path, "Ignore", "FMaxHeight", &Trace.FMaxHeight);
    Ini.Load(IsLoad, Path, "Ignore", "FMinWidth ", &Trace.FMinWidth );
    Ini.Load(IsLoad, Path, "Ignore", "FMaxWidth ", &Trace.FMaxWidth );

    Ini.Load(IsLoad, Path, "Property", "ShBlobScore   ", &Property.ShBlobScore   );
    Ini.Load(IsLoad, Path, "Property", "TrMargin      ", &Property.TrMargin      );
    Ini.Load(IsLoad, Path, "Property", "ShMarginX     ", &Property.ShMarginX     );
    Ini.Load(IsLoad, Path, "Property", "ShMarginY     ", &Property.ShMarginY     );
    Ini.Load(IsLoad, Path, "Property", "PassBlobScore ", &Property.PassBlobScore );
    Ini.Load(IsLoad, Path, "Property", "ReferString0"  , &Property.ReferString0  );
    Ini.Load(IsLoad, Path, "Property", "ReferString1"  , &Property.ReferString1  );
    Ini.Load(IsLoad, Path, "Property", "ReferString2"  , &Property.ReferString2  );
    Ini.Load(IsLoad, Path, "Property", "Simbol        ", &Property.Simbol        );
    Ini.Load(IsLoad, Path, "Property", "Mode"          , &Property.Mode);
    Ini.Load(IsLoad, Path, "Property", "ShMove"        , &Property.ShMove);
    Ini.Load(IsLoad, Path, "Property", "ShMinusX"      , &Property.ShMinusX);
    Ini.Load(IsLoad, Path, "Property", "ShMinusY"      , &Property.ShMinusY);

    if (Property.ShMarginX==0) Property.ShMarginX = 30;
    if (Property.ShMarginY==0) Property.ShMarginY = 30;


    Ini.Load(IsLoad, Path, "Path", "PathFont", &PathFont);

    Ini.Load(IsLoad, Path, "Train", "Count", &Train.Count);

    for (int k=0; k<MAX_OCR_FONT; k++) {
		Data.Format(L"Simbol:%d" ,k);
        Ini.Load(IsLoad, Path, "Train", Data, &Train.Simbol[k]);
    }

    // Roi Load & Save
    // -------------------------------------------------------------------------
    CString IRoiPath = Folder + "\\OcrIRoi.ini";
    OnLoadRoi(IsLoad, IRoiPath, IRoi);

    CString TRoiPath = Folder + "\\OcrTRoi.ini";
    OnLoadRoi(IsLoad, TRoiPath, TRoi);

#ifdef USING_FIND
    try {
        HFolder File;
        for (int k=0; k<MAX_OCR_FONT; k++) {
            AnsiString FontPath = Folder + "\\FontFind"+k+".fnd";
            if (IsLoad) {
                if (File.FileExist(FontPath)) {
                    FontFind[k].PatternFinder->Load(FontPath.c_str());
                }
            }
            else {
                FontFind[k].PatternFinder->Save(FontPath.c_str());
            }
        }
    }
    catch (EException& exc)
    {
    }
#endif

#ifdef USING_MATCH
    try {
        OnLoadByReferString();
        /*
        HFolder File;
        for (int k=0; k<MAX_OCR_FONT; k++) {
            AnsiString FontPath = Folder + "\\FontMatch"+k+".mch";
            if (IsLoad) {
                if (File.FileExist(FontPath)) {
                    FontFind[k].Matcher->Load(FontPath.c_str());
                }
            }
            else {
                FontFind[k].Matcher->Save(FontPath.c_str());
            }
        }
        */
    }
    catch (EException& exc)
    {
    }
#endif
}

void COcr::OnLoadByPath(CString Path)
{
    CIFile Ini;
    CFileUnit File;

    try {
        if (File.FileExist(Path)) {
            FontLoad.Matcher->Load(LPSTR(LPCTSTR(Path)));
        }
    }
    catch (EException& exc)
    {
    }
}


void COcr::OnLoadByReferString()
{
	CIFile Ini;
	CFileUnit FileUnit;

	CString Data="";
    CString Path = "D:\\OCRDB";
    if (!FileUnit.ExistDir(Path)) FileUnit.CreateDir(Path);

    Path = Path + "\\" + "FONT";
    if (!FileUnit.ExistDir(Path)) FileUnit.CreateDir(Path);

    if (PathFont.Trim()!="") {
        Path = Path + "\\" + PathFont;
        if (!FileUnit.ExistDir(Path)) FileUnit.CreateDir(Path);
    }

    try {
        CString ReferString = "";
        Train.Count = 0;

        for (int i=0; i<3; i++) {
            if (i==0) ReferString = Property.ReferString0;
            if (i==1) ReferString = Property.ReferString1;
            if (i==2) ReferString = Property.ReferString2;

            int Count = ReferString.Trim().GetLength();
            for (int k=0; k<Count; k++) {
                if (k>=MAX_OCR_FONT) continue;

                CString Char = ReferString.Trim().Mid(k+1, 1);

				// Marking 아닌경우 
                if (Char == "?") continue;
                if (Char == "*") continue;
                if (Char == ".") continue;
                if (Char == ".") continue;
                if (Char == "$") continue;
                if (Char == "%") continue;
                if (Char == "@") continue;

                bool IsBig = true;

				// Character 소문자 or 대문자 구별 
                if (Char.Mid(1, 1)=="a") IsBig = false;
                if (Char.Mid(1, 1)=="b") IsBig = false;
                if (Char.Mid(1, 1)=="c") IsBig = false;
                if (Char.Mid(1, 1)=="d") IsBig = false;
                if (Char.Mid(1, 1)=="e") IsBig = false;
                if (Char.Mid(1, 1)=="f") IsBig = false;
                if (Char.Mid(1, 1)=="g") IsBig = false;
                if (Char.Mid(1, 1)=="h") IsBig = false;
                if (Char.Mid(1, 1)=="i") IsBig = false;
                if (Char.Mid(1, 1)=="j") IsBig = false;
                if (Char.Mid(1, 1)=="k") IsBig = false;
                if (Char.Mid(1, 1)=="l") IsBig = false;
                if (Char.Mid(1, 1)=="m") IsBig = false;
                if (Char.Mid(1, 1)=="n") IsBig = false;
                if (Char.Mid(1, 1)=="o") IsBig = false;
                if (Char.Mid(1, 1)=="p") IsBig = false;
                if (Char.Mid(1, 1)=="q") IsBig = false;
                if (Char.Mid(1, 1)=="r") IsBig = false;
                if (Char.Mid(1, 1)=="s") IsBig = false;
                if (Char.Mid(1, 1)=="t") IsBig = false;
                if (Char.Mid(1, 1)=="u") IsBig = false;
                if (Char.Mid(1, 1)=="v") IsBig = false;
                if (Char.Mid(1, 1)=="w") IsBig = false;
                if (Char.Mid(1, 1)=="x") IsBig = false;
                if (Char.Mid(1, 1)=="y") IsBig = false;
                if (Char.Mid(1, 1)=="z") IsBig = false;

                //대문자 Loading
                //--------------------------------------------------------------
                if (IsBig) {
                    CString FontPath = Path + "\\"+Char+".mch";
                    if (FileUnit.FileExist(FontPath)) {
                        FontFind[Train.Count].Matcher->Load(LPSTR(LPCTSTR(FontPath)));
                        Train.Simbol[Train.Count] = Char;
                        Train.Count++;
                    }

                    for (int j=0; j<10; j++) {
						Data.Format(L"%d:",j);
                        FontPath = Path + "\\"+Char+Data+".mch";
                        if (FileUnit.FileExist(FontPath)) {
                            FontFind[Train.Count].Matcher->Load(LPSTR(LPCTSTR(FontPath)));
                            Train.Simbol[Train.Count] = Char;
                            Train.Count++;
                        }
                    }
                }

                //소문자 Loading
                //--------------------------------------------------------------
                else {
                    CString FontPath = Path + "\\"+Char+"_Small.mch";
                    if (FileUnit.FileExist(FontPath)) {
                        FontFind[Train.Count].Matcher->Load(LPSTR(LPCTSTR(FontPath)));
                        Train.Simbol[Train.Count] = Char;
                        Train.Count++;
                    }

                    for (int j=0; j<10; j++) {
						Data.Format(L"%d:",j);
                        FontPath = Path + "\\"+Char+Data+"_Small.mch";
                        if (FileUnit.FileExist(FontPath)) {
                            FontFind[Train.Count].Matcher->Load(LPSTR(LPCTSTR(FontPath)));
                            Train.Simbol[Train.Count] = Char;
                            Train.Count++;
                        }
                    }
                }
            }
        }
    }
    catch (EException& exc)
    {
    }
}

void COcr::OnSaveByReferString(CString Char)
{
    if (Char.Mid(1, 1)=="a") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="b") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="c") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="d") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="e") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="f") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="g") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="h") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="i") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="j") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="k") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="l") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="m") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="n") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="o") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="p") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="q") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="r") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="s") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="t") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="u") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="v") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="w") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="x") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="y") Char = Char + "_Small";
    if (Char.Mid(1, 1)=="z") Char = Char + "_Small";

    CIFile Ini;
	CFileUnit FileUnit;


    CString Path = "D:\\OCRDB";
    if (!FileUnit.ExistDir(Path)) FileUnit.CreateDir(Path);

    Path = Path + "\\" + "FONT";
	if (!FileUnit.ExistDir(Path)) FileUnit.CreateDir(Path);

    if (PathFont.Trim()!="") {
        Path = Path + "\\" + PathFont;
		if (!FileUnit.ExistDir(Path)) FileUnit.CreateDir(Path);
    }

    try {
        CString FontPath = Path + "\\"+Char+".mch";
        FontTrain.Matcher->Save(LPSTR(LPCTSTR(FontPath)));  
    }
    catch (EException& exc)
    {
    }
}

