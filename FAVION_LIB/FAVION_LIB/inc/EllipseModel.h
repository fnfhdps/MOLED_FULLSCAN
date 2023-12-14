#pragma once

#include "AbstractModel.h"
#include <limits>
#include "..\Shape.h"

using namespace The;
using namespace GRANSAC;
using std::numeric_limits;

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

#define cross(a, b, ab) ab[0] = a[1]*b[2] - a[2]*b[1]; \
						ab[1] = a[2]*b[0] - a[0]*b[2]; \
						ab[2] = a[0]*b[1] - a[1]*b[0]

//////////////////////////////////////////////////////////////////////////
// 임의의 3점을 지나는 원의 중심과 반지름 계산
class EllipseModel
	: public GRANSAC::AbstractModel < 5 >
{
public:
	// Parametric form
	double m_dist;// 
	double a, b, c, d, e, f;
	double m_major;
	double m_minor;
	double m_cx;	// center x
	double m_cy;	// center y
	double m_coff;
	dPoint m_focal1;
	dPoint m_focal2;
	double m_focalDist;
	dPoint m_left;
	dPoint m_top;
	dPoint m_right;
	dPoint m_bottom;
	double m_theta;
	bool   m_valid;

protected:
public:
	virtual double ComputeDistanceMeasure(std::shared_ptr<dPoint> Param) override
	{
		auto p = std::dynamic_pointer_cast<dPoint>(Param);

		if (p == nullptr || m_valid == false) {
		//	throw std::runtime_error("EllipseModel::ComputeDistanceMeasure() - Passed parameter are not of type Point2D.");
			m_dist = std::numeric_limits<double>::max();
		}
		else {
		//	distance between circle to point

			m_dist = sqrt(fabs(
				a * p->x * p->x +
				b * p->x * p->y +
				c * p->y * p->y +
				d * p->x +
				e * p->y + f));
		}

		return m_dist;
	};

	bool toconic(double p0[3], double p1[3], double p2[3], double p3[3], double p4[3])
	{
		double L0[3], L1[3], L2[3], L3[3];
		double A, B, C, Q[3];
		double a1, a2, b1, b2, c1, c2;
		double x0, x4, y0, y4, w0, w4;
		double aa, bb, cc, dd, ee, ff;
		double y4w0, w4y0, w4w0, y4y0, x4w0, w4x0, x4x0, y4x0, x4y0;
		double a1a2, a1b2, a1c2, b1a2, b1b2, b1c2, c1a2, c1b2, c1c2;

		aa = bb = cc = dd = ee = ff = 0.;

		cross(p0, p1, L0);
		cross(p1, p2, L1);
		cross(p2, p3, L2);
		cross(p3, p4, L3);
		cross(L0, L3, Q);
		
		A = Q[0]; B = Q[1]; C = Q[2];
		a1 = L1[0]; b1 = L1[1]; c1 = L1[2];
		a2 = L2[0]; b2 = L2[1]; c2 = L2[2];
		x0 = p0[0]; y0 = p0[1]; w0 = p0[2];
		x4 = p4[0]; y4 = p4[1]; w4 = p4[2];

		y4w0 = y4*w0;
		w4y0 = w4*y0;
		w4w0 = w4*w0;
		y4y0 = y4*y0;
		x4w0 = x4*w0;
		w4x0 = w4*x0;
		x4x0 = x4*x0;
		y4x0 = y4*x0;
		x4y0 = x4*y0;
		a1a2 = a1*a2;
		a1b2 = a1*b2;
		a1c2 = a1*c2;
		b1a2 = b1*a2;
		b1b2 = b1*b2;
		b1c2 = b1*c2;
		c1a2 = c1*a2;
		c1b2 = c1*b2;
		c1c2 = c1*c2;

		aa = -A*a1a2*y4w0
			+ A*a1a2*w4y0
			- B*b1a2*y4w0
			- B*c1a2*w4w0
			+ B*a1b2*w4y0
			+ B*a1c2*w4w0
			+ C*b1a2*y4y0
			+ C*c1a2*w4y0
			- C*a1b2*y4y0
			- C*a1c2*y4w0;

		cc = A*c1b2*w4w0
			+ A*a1b2*x4w0
			- A*b1c2*w4w0
			- A*b1a2*w4x0
			+ B*b1b2*x4w0
			- B*b1b2*w4x0
			+ C*b1c2*x4w0
			+ C*b1a2*x4x0
			- C*c1b2*w4x0
			- C*a1b2*x4x0;

		ff = A*c1a2*y4x0
			+ A*c1b2*y4y0
			- A*a1c2*x4y0
			- A*b1c2*y4y0
			- B*c1a2*x4x0
			- B*c1b2*x4y0
			+ B*a1c2*x4x0
			+ B*b1c2*y4x0
			- C*c1c2*x4y0
			+ C*c1c2*y4x0;

		bb = A*c1a2*w4w0
			+ A*a1a2*x4w0
			- A*a1b2*y4w0
			- A*a1c2*w4w0
			- A*a1a2*w4x0
			+ A*b1a2*w4y0
			+ B*b1a2*x4w0
			- B*b1b2*y4w0
			- B*c1b2*w4w0
			- B*a1b2*w4x0
			+ B*b1b2*w4y0
			+ B*b1c2*w4w0
			- C*b1c2*y4w0
			- C*b1a2*x4y0
			- C*b1a2*y4x0
			- C*c1a2*w4x0
			+ C*c1b2*w4y0
			+ C*a1b2*x4y0
			+ C*a1b2*y4x0
			+ C*a1c2*x4w0;

		dd = -A*c1a2*y4w0
			+ A*a1a2*y4x0
			+ A*a1b2*y4y0
			+ A*a1c2*w4y0
			- A*a1a2*x4y0
			- A*b1a2*y4y0
			+ B*b1a2*y4x0
			+ B*c1a2*w4x0
			+ B*c1a2*x4w0
			+ B*c1b2*w4y0
			- B*a1b2*x4y0
			- B*a1c2*w4x0
			- B*a1c2*x4w0
			- B*b1c2*y4w0
			+ C*b1c2*y4y0
			+ C*c1c2*w4y0
			- C*c1a2*x4y0
			- C*c1b2*y4y0
			- C*c1c2*y4w0
			+ C*a1c2*y4x0;

		ee = -A*c1a2*w4x0
			- A*c1b2*y4w0
			- A*c1b2*w4y0
			- A*a1b2*x4y0
			+ A*a1c2*x4w0
			+ A*b1c2*y4w0
			+ A*b1c2*w4y0
			+ A*b1a2*y4x0
			- B*b1a2*x4x0
			- B*b1b2*x4y0
			+ B*c1b2*x4w0
			+ B*a1b2*x4x0
			+ B*b1b2*y4x0
			- B*b1c2*w4x0
			- C*b1c2*x4y0
			+ C*c1c2*x4w0
			+ C*c1a2*x4x0
			+ C*c1b2*y4x0
			- C*c1c2*w4x0
			- C*a1c2*x4x0;

		if (aa != 0.0) {
			bb /= aa; cc /= aa; dd /= aa; ee /= aa; ff /= aa; aa = 1.0;
		}
		else if (bb != 0.0) {
			cc /= bb; dd /= bb; ee /= bb; ff /= bb; bb = 1.0;
		}
		else if (cc != 0.0) {
			dd /= cc; ee /= cc; ff /= cc; cc = 1.0;
		}
		else if (dd != 0.0) {
			ee /= dd; ff /= dd; dd = 1.0;
		}
		else if (ee != 0.0) {
			ff /= ee; ee = 1.0;
		}
		else {
			m_valid = false;
		}

		a = aa;
		b = bb;
		c = cc;
		d = dd;
		e = ee;
		f = ff;

		m_valid = true;

		return m_valid;
	}


public:
	EllipseModel(std::vector<std::shared_ptr<dPoint>> InputParams, double r)
	{
		Initialize(InputParams);
	};

	virtual void Initialize(std::vector<std::shared_ptr<dPoint>> InputParams) override
	{
		if (InputParams.size() != 5) {
			m_valid = false;
			throw std::runtime_error("EllipseModel - Number of input parameters does not match minimum number required for this model.");
			return;
		}

		// Check for AbstractParamter types
		auto p0 = std::dynamic_pointer_cast<dPoint>(InputParams[0]);
		auto p1 = std::dynamic_pointer_cast<dPoint>(InputParams[1]);
		auto p2 = std::dynamic_pointer_cast<dPoint>(InputParams[2]);
		auto p3 = std::dynamic_pointer_cast<dPoint>(InputParams[3]);
		auto p4 = std::dynamic_pointer_cast<dPoint>(InputParams[4]);

		if (p0 == nullptr || p1 == nullptr || p2 == nullptr || p3 == nullptr || p4 == nullptr) {
			m_valid = false;
			throw std::runtime_error("EllipseModel - InputParams type mismatch. It is not a Point2D.");
			return;
		}

		std::copy(InputParams.begin(), InputParams.end(), m_MinModelParams.begin());

		double pp0[3] = { p0->x, p0->y, 1 };
		double pp1[3] = { p1->x, p1->y, 1 };
		double pp2[3] = { p2->x, p2->y, 1 };
		double pp3[3] = { p3->x, p3->y, 1 };
		double pp4[3] = { p4->x, p4->y, 1 };

		bool result = toconic(pp0, pp1, pp2, pp3, pp4);

		///////////////////////////////////////////////////////////////////////////
#if(0)
		//orientation ellipse
		m_theta = atan2(b, a - c) * 0.5;
		double tmp0 = b * b - 4 * a * c;
		//scaled major/minor axies 
		double ct = cos(m_theta);
		double st = sin(m_theta);
		double ap = (a * ct * ct) + (b * ct * st) + (c * st * st);
		double cp = (a * st * st) - (b * ct * st) + (c * ct * ct);

		//tanslation
		m_cx = ((2. * c * d) - (b * e)) / tmp0;
		m_cy = ((2. * a * e) - (b * d)) / tmp0;
		
		//scale factor
		double val = (a * m_cx * m_cx) + (b * m_cx * m_cy) + (c * m_cy * m_cy);
		double scale_inv = val - f;

		if (scale_inv / ap <= 0 || scale_inv / cp < 0.) {	// not ellipse
			m_valid = false;
			return;
		}

		m_major = sqrt(scale_inv / ap);
		m_minor = sqrt(scale_inv / cp);
#endif
#if(0)
		///////////////////////////////////////////////////////////////////////////
		double tmp1 = b * b - 4 * a * c;
		double tmp2 = sqrt((a - c) * (a - c) + b * b);
		double tmp3 = a * e * e + c * d * d - b * d * e + tmp1 * f;
		double r1 = -sqrt(2 * tmp3 * (a + c + tmp2)) / tmp1;
		double r2 = -sqrt(2 * tmp3 * (a + c - tmp2)) / tmp1;

		m_cx = (2 * c * d - b * e) / tmp1;
		m_cy = (2 * a * e - b * d) / tmp1;

		m_major = std::max<double>(r1, r2);
		m_minor = std::min<double>(r1, r2);

		m_theta = 0.5 * atan2(b, a - c);

		if (r1 > r2)
			m_theta += M_PI_2;
#endif
		//https://math.stackexchange.com/questions/616645/determining-the-major-minor-axes-of-an-ellipse-from-general-form
		if ((b*b - 4 * a*c < 0) || ((4 * a*c - b*b) > 0))
			m_valid = true;

		double q = 64 * (f*(4 * a*c - b*b) - a*e*e + b*d*e - c*d*d) / pow((4 * a*c - b*b), 2);	//coefficient normalizing factor
		double s = 0.25 * sqrt(abs(q) * sqrt(b*b + pow(a-c, 2)));								//distance between center and focal point
		double rmax = 0.125 * sqrt(2 * abs(q) * sqrt(b*b + pow(a - c, 2)) - 2 * q*(a + c));		//semi-major axis length
		double rmin = sqrt(rmax*rmax - s*s);													//semi-minor axis length
		double cx = (b*e - 2 * c*d) / (4 * a*c - b*b);											//center of the ellipse x
		double cy = (b*d - 2 * a*e) / (4 * a*c - b*b);											//center of the ellipse y
		double yt = cy + sqrt(pow(2 * b*d - 4 * a*e, 2) + 4 * (4 * a*c - b*b)*(d*d - 4 * a*f)) / (2 * (4 * a*c - b*b));	//top-most point
		double xt = (-b*yt - d) / (2 * a);
		double yb = cy - sqrt(pow(2 * b*d - 4 * a*e, 2) + 4 * (4 * a*c - b*b)*(d*d - 4 * a*f)) / (2 * (4 * a*c - b*b));	//bottom-most point 
		double xb = (-b*yb - d) / (2 * a);
		double xL = cx - sqrt(pow(2 * b*e - 4 * c*d, 2) + 4 * (4 * a*c - b*b)*(e*e - 4 * c*f)) / (2 * (4 * a*c - b*b));	//left-most point
		double yL = (-b*xL - e) / (2 * c);
		double xR = cx + sqrt(pow(2 * b*e - 4 * c*d, 2) + 4 * (4 * a*c - b*b)*(e*e - 4 * c*f)) / (2 * (4 * a*c - b*b));	//right-most point
		double yR = (-b*xR - e) / (2 * c);

		double th = 0;

		if ((q*a - q*c) == 0 && (q*b) == 0)
			th = 0;
		else if ((q*a - q*c) == 0 && (q*b) > 0)
			th = M_PI / 4;
		else if ((q*a - q*c) == 0 && (q*b) < 0)
			th = M_PI * 3 / 4;
		else if ((q*a - q*c) > 0 && (q*b) >= 0)
			th = 0.5 * atan2(b, a - c);
		else if ((q*a - q*c) > 0 && (q*b) < 0)
			th = 0.5 * atan2(b, a - c) + M_PI;
		else if ((q*a - q*c) < 0)
			th = 0.5 * atan2(b, a - c) + 0.5 * M_PI;

		m_coff = q;
		m_focalDist = s;

		m_major = rmax;
		m_minor = rmin;

		m_cx = cx;
		m_cy = cy;

		m_left.x = xL;	m_left.y = yL;
		m_top.x = xt;	m_top.y = yt;
		m_right.x = xR;	m_right.y = yR;
		m_bottom.x = xb; m_bottom.y = yb;

		m_theta = th;

		dPoint centre(cx, cy);

		m_focal1 = centre.distanceByRad(th, s);
		m_focal2 = centre.distanceByRad(th, -s);

		m_valid = true;
	};

	virtual std::pair<double, std::vector<std::shared_ptr<dPoint>>> Evaluate(std::vector<std::shared_ptr<dPoint>> EvaluateParams, double Threshold)
	{
		std::vector<std::shared_ptr<dPoint>> Inliers;
		size_t nTotalParams = EvaluateParams.size();
		int nInliers = 0;

		for (auto& Param : EvaluateParams)
		{
			if (ComputeDistanceMeasure(Param) < Threshold)
			{
				Inliers.push_back(Param);
				nInliers++;
			}
		}

		double InlierFraction = double(nInliers) / double(nTotalParams); // This is the inlier fraction

		return std::make_pair(InlierFraction, Inliers);
	};

	double SimpsonRuleArcDistance(double x1, double x2, int N)
	{
		if (x1 == x2) return 0.;
		double mx = std::min<double>(x1, x2);
		double Mx = std::max<double>(x1, x2);

		double a = m_major;
		double b = m_minor;
		double R = a, r = b;

		double dx = (Mx - mx) / N;
		double int_x = 0;

		// x = R*cos(theta) -> x / R = cos(theta)
		// y = r*sin(theta)
		double p = (mx / R);
		double q = (Mx / R);

		double mt = acos(p < -1 ? -1 : p);
		double Mt = acos(q > 1 ? 1 : q);
		double my = r*sin(mt);
		double My = r*sin(Mt);
		double dt = (Mt - mt) / N;
		double s = my;

		for (int i = 1; i <= N; ++i) 
		{
			double ct = dt * i;
			s += (i % 2) ? r * sin(ct) * 4 : r * sin(ct) * 2;

		//	s += sqrt(1 + (b*b * pow(mt + dt*i, 2)) / (a*a * (a*a - pow(mt + dt*i, 2))));
		}

		return s;
	}
};


#pragma pop_macro("min")
#pragma pop_macro("max")