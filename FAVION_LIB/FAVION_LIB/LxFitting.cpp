#include "stdafx.h"
#include "LxFitting.h"

//------------------------------------------------
//Class CCircleFit
//------------------------------------------------
CCircleFit::CCircleFit()
{
}

CCircleFit::~CCircleFit()
{
}
int CCircleFit::OnCircum(double x1, double x2, double x3, double y1, double y2, double y3, double *cx, double *cy, double *rad)
{
	double bax = x2 - x1;
	double bay = y2 - y1;
	double cax = x3 - x1;
	double cay = y3 - y1;

	double E = bax*(x1+x2) + bay*(y1+y2);
	double F = cax*(x1+x3) + cay*(y1+y3);
	double G = 2*(bax*(y3-y2)-bay*(x3-x2));

	if (G==0) return 0;

	*cx = (cay*E - bay*F)/G;
	*cy = (bax*F - cax*E)/G;

	double dx = *cx - x1;
	double dy = *cy - y1;

	*rad = sqrt(dx*dx + dy*dy);
	return 1;
}

bool CCircleFit::OnSolveLinearEQ3X3(double A[9], double bb[3], double x[3])
{
	double invA[9];
	double det = (A[0]*(A[4]*A[8]-A[5]*A[7]) - A[1]*(A[3]*A[8] - A[5]*A[6]) + A[2]*(A[3]*A[7] - A[4]*A[6]));

	if (det!=0) {
		det = 1 / det;
		invA[0] = (A[4]*A[8] - A[5]*A[7]) * det;
		invA[1] = (A[2]*A[7] - A[1]*A[8]) * det;
		invA[2] = (A[1]*A[5] - A[2]*A[4]) * det;
		invA[3] = (A[5]*A[6] - A[3]*A[8]) * det;
		invA[4] = (A[0]*A[8] - A[2]*A[6]) * det;
		invA[5] = (A[2]*A[3] - A[0]*A[5]) * det;
		invA[6] = (A[3]*A[7] - A[4]*A[6]) * det;
		invA[7] = (A[1]*A[6] - A[0]*A[7]) * det;
		invA[8] = (A[0]*A[4] - A[1]*A[3]) * det;

		x[0] = invA[0] * bb[0] + invA[1] * bb[1] + invA[2] * bb[2];
		x[1] = invA[3] * bb[0] + invA[4] * bb[1] + invA[5] * bb[2];
		x[2] = invA[6] * bb[0] + invA[7] * bb[1] + invA[8] * bb[2];
		return true;
	}
	else {
		x[0] = x[1] = x[2] = 0;
		return false;
	}
}

int CCircleFit::OnLS(int N, double xp[], double yp[], double *cx, double *cy, double *rad)
{
	double sx=0, sy=0, sxx=0, sxy=0, syy=0, sxxx=0, sxxy=0, sxyy=0, syyy=0;
	for (int i=0; i<N; i++) {
		double x = xp[i], y = yp[i];
		double xx = x*x;
		double yy = y*y;
		sx += x;
		sy += y;
		sxx += xx;
		sxy += x*y;
		syy += yy;
		sxxx += xx*x;
		sxxy += xx*y;
		sxyy += x*yy;
		syyy += yy*y;
	}
	double A[9], b[3], sol[3];
	A[0] = sxx, A[1] = sxy, A[2] = sx,
		A[3] = sxy, A[4] = syy, A[5] = sy,
		A[6] = sx , A[7] = sy , A[8] = N,
		b[0] = -sxxx-sxyy,
		b[1] = -sxxy-syyy,
		b[2] = -sxx-syy;

	if (!OnSolveLinearEQ3X3(A, b, sol)) return 0;

	double det = sol[0]*sol[0] + sol[1]*sol[1] - 4*sol[2];
	if (det<=0) return 0;

	*cx = -sol[0]/2;
	*cy = -sol[1]/2;
	*rad = sqrt(det)/2;
	return 1;
}

void CCircleFit::GetRandomTriplet(int max_num, int triplet[3])
{
	int index = 0;
	while (index<3) {
		bool newone = true;
		int r = (int)((double)(rand())/RAND_MAX * max_num);
		for (int i=0; i<index; i++) {
			if (r == triplet[i]) {
				newone = false;
				break;
			}
		}
		if (newone) {
			triplet[index] = r;
			++index;
		}
	}
}

int CCircleFit::OnRansac(TPoint Q[], int N, int sample_th, double dist_deviate_th, TFPoint *C, double *radius)
{
	if (N<10) return 0;

	double pr = double(sample_th)/double(N);
	double trials99=log(1-0.99)/log(1-pow(pr,3));
	//int iter=min(max_iter, trials99);

	int iter = trials99;
	std::vector<double> consensusx(N);
	std::vector<double> consensusy(N);
	double min_dev = 1.e+10;
	while(iter) {
		int triplet[3];
		GetRandomTriplet(N, triplet);
		double cx, cy, rad;
		if (!OnCircum(Q[triplet[0]].x, Q[triplet[1]].x, Q[triplet[2]].x, Q[triplet[0]].y, Q[triplet[1]].y, Q[triplet[2]].y, &cx, &cy, &rad)) continue;

		int ninlier=0;
		for (int k=0; k<N; ++k) {
			double dx = Q[k].x-cx;
			double dy = Q[k].y-cy;
			double dist = sqrt(dx*dx + dy*dy);
			double distdeviate = fabs(dist-rad)/rad;
			if (distdeviate<=dist_deviate_th) {
				consensusx[ninlier] = Q[k].x;
				consensusy[ninlier] = Q[k].y;
				ninlier++;
			}
		}
		if (ninlier>=sample_th) {
			double cxx, cyy, radi;
			if (!OnLS(ninlier, &consensusx[0], &consensusy[0], &cxx, &cyy, &radi)) continue;

			double sdev = 0;
			ninlier = 0;
			for (int i=0; i<N; i++) {
				double dx = Q[i].x - cxx;
				double dy = Q[i].y - cyy;
				double dist = sqrt(dx*dx + dy*dy);
				sdev += (dist-radi) * (dist-radi);
				double distdeviate = fabs(dist-radi)/radi;
				if (distdeviate < dist_deviate_th/2) {
					consensusx[ninlier] = Q[i].x;
					consensusy[ninlier] = Q[i].y;
					ninlier++;
				}
			}
			if (ninlier<sample_th) continue;
			sdev = sqrt(sdev/ninlier);

			if (!OnLS(ninlier, &consensusx[0], &consensusy[0], &cxx, &cyy, &radi)) continue;

			if (sdev<min_dev) {
				C->x = cxx;
				C->y = cyy;
				*radius  = radi;
			}
		}
		--iter;
	}
	return 1;
}

int CCircleFit::OnRansac(TFPoint Q[], int N, int sample_th, double dist_deviate_th, TFPoint *C, double *radius)
{
	if (N<10) return 0;

	double pr = double(sample_th)/double(N);
	double trials99=log(1-0.99)/log(1-pow(pr,3));
	//int iter=min(max_iter, trials99);

	int iter = trials99;

	std::vector<double> consensusx(N);
	std::vector<double> consensusy(N);
	double min_dev = 1.e+10;
	while(iter) {
		int triplet[3];
		GetRandomTriplet(N, triplet);
		double cx, cy, rad;
		if (!OnCircum(Q[triplet[0]].x, Q[triplet[1]].x, Q[triplet[2]].x, Q[triplet[0]].y, Q[triplet[1]].y, Q[triplet[2]].y, &cx, &cy, &rad)) continue;

		int ninlier=0;
		for (int k=0; k<N; ++k) {
			double dx = Q[k].x-cx;
			double dy = Q[k].y-cy;
			double dist = sqrt(dx*dx + dy*dy);
			double distdeviate = fabs(dist-rad)/rad;
			if (distdeviate<=dist_deviate_th) {
				consensusx[ninlier] = Q[k].x;
				consensusy[ninlier] = Q[k].y;
				ninlier++;
			}
		}
		if (ninlier>=sample_th) {
			double cxx, cyy, radi;
			if (!OnLS(ninlier, &consensusx[0], &consensusy[0], &cxx, &cyy, &radi)) continue;

			double sdev = 0;
			ninlier = 0;
			for (int i=0; i<N; i++) {
				double dx = Q[i].x - cxx;
				double dy = Q[i].y - cyy;
				double dist = sqrt(dx*dx + dy*dy);
				sdev += (dist-radi) * (dist-radi);
				double distdeviate = fabs(dist-radi)/radi;
				if (distdeviate < dist_deviate_th/2) {
					consensusx[ninlier] = Q[i].x;
					consensusy[ninlier] = Q[i].y;
					ninlier++;
				}
			}
			if (ninlier<sample_th) continue;
			sdev = sqrt(sdev/ninlier);

			if (!OnLS(ninlier, &consensusx[0], &consensusy[0], &cxx, &cyy, &radi)) continue;

			if (sdev<min_dev) {
				C->x = cxx;
				C->y = cyy;
				*radius  = radi;
			}
		}
		--iter;
	}
	return 1;
}

//------------------------------------------------
//Class CLineFit
//------------------------------------------------

CLineFit::CLineFit()
{
}
CLineFit::~CLineFit()
{
}
//------------------------------------------------------------------------------
//START : CLineFit

float CLineFit::GetDistance(float x1, float y1, float x2, float y2, float x, float y)
{
	if ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) != 0) {
		float u  = ((x-x1)*(x2-x1) + (y-y1)*(y2-y1));
		float xp = x1 + u * (x2-x1);
		float yp = y1 + u * (y2-y1);
		return sqrt((xp-x)*(xp-x) + (yp-y)*(yp-y));
	}
	return sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1));
}

bool CLineFit::OnRANSAC(TFPoint *Q, int N, double dist_th)
{
	P0.x=0; P0.y=0;
	P1.x=0; P1.y=0;

	if (N< 1) { P0.x=0; P0.y=0; P1.x=0; P1.y=0; return true; }
	if (N==1) { P0 = Q[0]; P1 = Q[0]; return true; }
	if (N==2) { P0 = Q[0]; P1 = Q[1]; return true; }

	const int MaxPoint = 1000;

	int MaxNum = 3;
	int index  = 0;
	TFPoint P    [MaxPoint];
	TFPoint MaxP [MaxPoint];
	double Weight[MaxPoint];
	int Prev_n = 0;

	int TryCount = N/2;
	if (N<10) TryCount = N;

	srand(unsigned(time(0)));

	for (int i=0; i<TryCount; i++) {
		int i0, i1;
		i0 = N==0 ? 0 : rand() % N; P[0] = Q[i0];
		i1 = N==0 ? 0 : rand() % N; P[1] = Q[i1];

		if (P[1].x==P[0].x && P[1].y==P[0].y) continue;

		int n = 2;
		int Cnt = 0;
		float dist = 0;
		for (int k=0; k<N; k++) Weight[k] = 0.0;

		for(int j=0;j<N; j++) {
			if (j==i0) continue;
			if (j==i1) continue;
			dist = GetLengthFrLineToPoint(P[0], P[1], Q[j]);
			if (dist<=dist_th) {
				P[n++]=Q[j];
				Weight[Cnt++] = 1/sqrt(1+dist*dist);
			}
		}

		//if (n>=num_th) {
		if (index++ > MaxPoint) break;
		if (n>Prev_n) {
			Prev_n = n;
			MaxNum = n;
			for (int k=0; k<N; k++) MaxP[k] = P[k];
			//OnPCA(MaxP, MaxNum, Weight);
			P0.x = P[0].x; P0.y = P[0].y;
			P1.x = P[1].x; P1.y = P[1].y;

		}
		//}
	}
	return 1;
}

bool CLineFit::OnRANSAC(TFPoint *Q1, int N1, TFPoint *Q2, int N2, double dist_th, int max_iter, int num_th)
{
	if (N1<4 || N1>500) return false;
	if (N2<4 || N2>500) return false;

	const int MaxPoint = 1000;

	int MaxNum = 3;
	int index  = 0;
	TFPoint P    [MaxPoint];
	TFPoint MaxP [MaxPoint];
	double Weight[MaxPoint];
	int Prev_n = 0;

	srand(unsigned(time(0)));

	for (int i=0; i<max_iter; i++) {
		int i0, i1;
		i0 = N1==0 ? 0 : rand() % N1; P[0] = Q1[i0];
		i1 = N2==0 ? 0 : rand() % N2; P[1] = Q2[i1];

		if(P[1].x==P[0].x && P[1].y==P[0].y) continue;

		int n = 2;
		int Cnt = 0;
		float dist = 0;
		for (int k=0; k<N1+N2 ; k++) Weight[k] = 0.0;

		for(int j=0;j<N1; j++){ if (j==i0) continue; dist = GetLengthFrLineToPoint(P[0], P[1], Q1[j]); if (dist<=dist_th) { P[n++]=Q1[j]; Weight[Cnt++] = 1/sqrt(1+dist*dist); }}
		for(int j=0;j<N2; j++){ if (j==i1) continue; dist = GetLengthFrLineToPoint(P[0], P[1], Q2[j]); if (dist<=dist_th) { P[n++]=Q2[j]; Weight[Cnt++] = 1/sqrt(1+dist*dist); }}

		if (n>=num_th) {
			if (index++ > 500) break;
			if (n>Prev_n && MaxNum < n) {
				Prev_n = n;
				MaxNum = n;
				for (int k=0; k<N1+N2 ; k++) MaxP[k] = P[k];
				OnPCA(MaxP, MaxNum, Weight);

			}
		}
	}
	return 1;
}

int CLineFit::OnPCA(TFPoint P[], int N, double weight[])
{
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

	P0.x = P[0].x; P0.y = m*P[0].x - m*xc + yc;
	P1.x = P[1].x; P1.y = m*P[1].x - m*xc + yc;

	return 1;
}

float CLineFit::GetLengthFrLineToPoint(TFPoint P1, TFPoint P2, TFPoint P3)
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
//END : CLineFit
//------------------------------------------------------------------------------




