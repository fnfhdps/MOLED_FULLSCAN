#include "stdafx.h"
#include "LxPattern.h"
#include "LxMath.h"


CFLine::CFLine()
{
}


CFLine::~CFLine()
{
}
void CFLine::OnRansac(int Count, TFPoint *Q)
{
	DataCount = Count;

	if (DataCount<1) return;

	switch (FittingType) {
	case 0 :
		if (DataCount==1) {
			P0 = Q[0];
			P1 = Q[0];
			return;
		}
		P0 = Q[0];
		P1 = Q[DataCount-1];
		return;

	case 1 :
		LineFit.OnRANSAC(Q, Count);
		P0 = LineFit.P0;
		P1 = LineFit.P1;
		return;
	}
}
//----------------------------------------------------------
//Class CFPattern 

CFPattern::CFPattern()
{
}


CFPattern::~CFPattern()
{
}
float CFPattern::GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3)
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

	if (dist<0.0001) dist = 0;

	return dist;
}

void CFPattern::OnLineIntersection(TFPoint L1_P0, TFPoint L1_P1, TFPoint L2_P0, TFPoint L2_P1, TFPoint *P)
{
}

void CFPattern::OnUpdatePos(int IntersectMode, int __ColCount, int __RowCount)
{
	ColCount = __ColCount;
	RowCount = __RowCount;

	for (int y=0; y<RowCount; y++) {
		for (int x=0; x<ColCount; x++) {
			Pos[x][y].x = 0;
			Pos[x][y].y = 0;

			if (Col[x].P0.x<1) continue; if (Col[x].P0.y<1) continue;
			if (Col[x].P1.x<1) continue; if (Col[x].P1.y<1) continue; 
			if (Row[y].P0.x<1) continue; if (Row[y].P0.y<1) continue; 
			if (Row[y].P1.x<1) continue; if (Row[y].P1.y<1) continue; 

			TFPoint P0 (Col[x].P0.x, Col[x].P0.y);
			TFPoint P1 (Col[x].P1.x, Col[x].P1.y);
			TFPoint P2 (Row[y].P0.x, Row[y].P0.y);
			TFPoint P3 (Row[y].P1.x, Row[y].P1.y);

			TFPoint C(0,0);

			IntersectMode = 0;

			switch (IntersectMode) {
			case 0 :
				C = Math.GetPointFrLineToPoint(P2, P3, P0); break;
			case 1 :
// 				C = Math.OnFindFIntersection(P0, P1, P2, P3);
				break;
			}

			Pos[x][y].x = C.x;
			Pos[x][y].y = C.y;
		}}
}

void CFPattern::LinePoint(int __RowCount,int __ColCount,int pitchx,int pitchy){

	ColCount = __ColCount;
	RowCount = __RowCount;

	float angleRow=0.0f,angleCol=0.0f,avgangR=0.0f,avgangC=0.0f;
	TFLine l1[MAX_FIT_LINE];
	TFLine l2[MAX_FIT_LINE];

	for(int y=0;y<RowCount;y++){
		for(int x=0;x<ColCount;x++){
			Pos[x][y].x=0;
			Pos[x][y].y=0;
		}}

	for(int j=0;j<RowCount;j++){
		l1[j] = LineFitting(ColCount, j);
		angleRow+=l1[j].angle;
	}
	avgangR = angleRow/RowCount;

	float min = 10.000;
	int minIndex = 0;
	for(int j=0;j<RowCount;j++){
		if (min > fabs(l1[j].angle - avgangR)) minIndex = j;
	}

	for(int j=0;j<RowCount;j++){
		if (fabs(l1[j].angle-avgangR)>0.1) {
			l1[j].c = abs(minIndex-j)*pitchx*sqrt(l1[minIndex].a*l1[minIndex].a+l1[minIndex].b*l1[minIndex].b);
			l1[j].a = l1[minIndex].a;
			l1[j].b = l1[minIndex].b;
			l1[j].angle = l1[minIndex].angle;
		}
	}

	for(int i=0;i<ColCount;i++){
		l2[i] = LineFitting(RowCount, i);
		angleCol+=l2[i].angle;
	}
	avgangC = angleCol/ColCount;

	for(int i=0;i<ColCount;i++){
		if (min > fabs(l2[i].angle - avgangC)) minIndex = i;
	}

	for(int i=0;i<ColCount;i++){
		if (fabs(l1[i].angle-avgangC)>0.1) {
			l2[i].c = abs(minIndex-i)*pitchy*sqrt(l2[minIndex].a*l2[minIndex].a+l2[minIndex].b*l2[minIndex].b);
			l2[i].a = l2[minIndex].a;
			l2[i].b = l2[minIndex].b;
			l2[i].angle = l2[minIndex].angle;
		}
	}
	for(int y=0; y<RowCount; y++){
		for(int x=0; x<ColCount; x++){
// 			TFPoint point = Math.OnFindFIntersection(l1[y],l2[x]);
// 			Pos[x][y].x = point.x;
// 			Pos[x][y].y = point.y;
		}}
}

TFLine CFPattern::LineFitting(int count, int row){
	TFLine l;
	float sx1 = 0.0, sy1 = 0.0, stt1 = 0.0, sts1 = 0.0;
	float k,angle;

	for (int i = 0; i < count; i++){
		sx1 += CP[row][i].x;
		sy1 += CP[row][i].y;
	}
	for (int i = 0; i < count; i++){
		float t1 = CP[row][i].x - sx1/count;
		stt1 += t1*t1;
		sts1 += t1*CP[row][i].y;
	}
	l.a = sts1;
	l.b = (-1)*stt1;
	l.c = (sy1*stt1-sx1*sts1)*count;

	if (l.b==0) angle =(float)(M_PI/2);
	angle = atan((-1)*l.a/l.b)*180/M_PI;

	return l;
}
//-------------------------------------------------------------------------
