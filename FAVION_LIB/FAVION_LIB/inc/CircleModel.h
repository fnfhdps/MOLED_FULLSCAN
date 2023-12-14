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

//////////////////////////////////////////////////////////////////////////
// 임의의 3점을 지나는 원의 중심과 반지름 계산
class Circle3PtModel
	: public GRANSAC::AbstractModel < 3 >
{
public:
	// Parametric form
	double m_cx;	// center x
	double m_cy;	// center y
	double m_r;	// radius
	double m_dist;// 

protected:
public:
	virtual double ComputeDistanceMeasure(std::shared_ptr<dPoint> Param) override
	{
		auto ExtPoint2D = std::dynamic_pointer_cast<dPoint>(Param);
		if (ExtPoint2D == nullptr)
			throw std::runtime_error("Circle3PtModel::ComputeDistanceMeasure() - Passed parameter are not of type Point2D.");

		//	distance between circle to point
		m_dist = abs(sqrt(pow(ExtPoint2D->x - m_cx, 2) + pow(ExtPoint2D->y - m_cy, 2)) - m_r);

		return m_dist;
	};


	Circle3PtModel(std::vector<std::shared_ptr<dPoint>> InputParams, double r)
	{
		Initialize(InputParams);
	};

	virtual void Initialize(std::vector<std::shared_ptr<dPoint>> InputParams) override
	{
		if (InputParams.size() != 3)
			throw std::runtime_error("Circle3PtModel - Number of input parameters does not match minimum number required for this model.");

		// Check for AbstractParamter types
		auto Point1 = std::dynamic_pointer_cast<dPoint>(InputParams[0]);
		auto Point2 = std::dynamic_pointer_cast<dPoint>(InputParams[1]);
		auto Point3 = std::dynamic_pointer_cast<dPoint>(InputParams[2]);
		if (Point1 == nullptr || Point2 == nullptr || Point3 == nullptr)
			throw std::runtime_error("Circle3PtModel - InputParams type mismatch. It is not a Point2D.");

		std::copy(InputParams.begin(), InputParams.end(), m_MinModelParams.begin());

		double	A = Point2->x - Point1->x;
		double	B = Point2->y - Point1->y;
		double	C = Point3->x - Point1->x;
		double	D = Point3->y - Point1->y;
		double	E = A * (Point1->x + Point2->x) + B * (Point1->y + Point2->y);
		double	F = C * (Point1->x + Point3->x) + D * (Point1->y + Point3->y);
		double	G = 2.0 * (A * (Point3->y - Point2->y) - B * (Point3->x - Point2->x));

		// wenn G nahe bei null ist, so sind die drei Punkte
		if (G < 0.000000001) {
			m_cx = m_cy = m_r = std::numeric_limits<double>::min();
			return;
		}

		m_cx = (D * E - B * F) / G;
		m_cy = (A * F - C * E) / G;

		m_r = sqrt(pow(Point1->x - m_cx, 2) + pow(Point1->y - m_cy, 2));
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
};





//////////////////////////////////////////////////////////////////////////
// 반지름과 2점이 주어진 원의 중심 계산 
class Circle2PtModel
	: public GRANSAC::AbstractModel < 2 >
{
public:
	// Parametric form
	double m_cx[2];	// center x
	double m_cy[2];	// center y
	double m_r;		// radius
	double m_dist;	// 
	int				 m_num;		// circle count
	int				 m_bestOne;	// in case two circle exist, close one index between two circle

protected:
public:
	virtual double ComputeDistanceMeasure(std::shared_ptr<dPoint> Param) override
	{
		auto ExtPoint2D = std::dynamic_pointer_cast<dPoint>(Param);
		if (ExtPoint2D == nullptr) {
			throw std::runtime_error("Circle2PtModel::ComputeDistanceMeasure() - Passed parameter are not of type Point2D.");
			return std::numeric_limits<double>::max();
		}

		if (!m_num)	return std::numeric_limits<double>::max();

		//	distance between circle to point

		double dist1 = std::numeric_limits<double>::max();
		double dist2 = std::numeric_limits<double>::max();

		dist1 = abs(sqrt(pow(ExtPoint2D->x - m_cx[0], 2) + pow(ExtPoint2D->y - m_cy[0], 2)) - m_r);

		// in case two circle exist close one select
		if (m_num == 2)
			dist2 = abs(sqrt(pow(ExtPoint2D->x - m_cx[1], 2) + pow(ExtPoint2D->y - m_cy[1], 2)) - m_r);

		m_dist = std::min<double>(dist1, dist2);

		m_bestOne = (m_dist == dist1) ? 0 : 1;

		return m_dist;
	};

public:
	Circle2PtModel(std::vector<std::shared_ptr<dPoint>> InputParams, double r)
		:m_r(r)
	{
		Initialize(InputParams);
	};

	virtual void Initialize(std::vector<std::shared_ptr<dPoint>> InputParams) override
	{
		if (InputParams.size() != 2)
			throw std::runtime_error("Circle2PtModel - Number of input parameters does not match minimum number required for this model.");

		// Check for AbstractParamter types
		auto Point1 = std::dynamic_pointer_cast<dPoint>(InputParams[0]);
		auto Point2 = std::dynamic_pointer_cast<dPoint>(InputParams[1]);

		if (Point1 == nullptr || Point2 == nullptr)
			throw std::runtime_error("Circle2PtModel - InputParams type mismatch. It is not a Point2D.");

		std::copy(InputParams.begin(), InputParams.end(), m_MinModelParams.begin());

		// 2점의 중점 계산
		dLine A(*Point1, *Point2);
		dLine B = A.perpendicular_line();
		dPoint C = A.mid();
		
		
		double half_l = A.distance() / 2.;

		double delta = sqrt(m_r * m_r - half_l * half_l);
		dPoint c0 = C.distanceByRad(B.radAngle(), -delta);
		dPoint c1 = C.distanceByRad(B.radAngle(), delta);

		// 2점이 동일한 좌표 계산 불능.
		if (A.distance() == 0.) {
		//	throw std::runtime_error("Infinitely many circles can be draw through");
			m_num = 0;
		}
		//	2점의 거리가 반지름 보다 크다. 불가
		else if (A.distance() > m_r * 2.) {
		//	throw std::runtime_error("Given points are farther away from each other than diameter of circle");
			m_num = 0;
		}
		//	2점이 원의 중심 기준으로 마주 보고 있는 경우, 1개의 원 존재
		else if (A.distance() == m_r * 2) {
			m_cx[0] = C.x;
			m_cy[0] = C.y;
			m_num = 1;
		}
		//	2점의 원에 존재하는 경우로, 2개의 원이 존재함.
		else {
			m_cx[0] = c0.x;
			m_cy[0] = c0.y;

			m_cx[1] = c1.x;
			m_cy[1] = c1.y;
			m_num = 2;
		}
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
};

#pragma pop_macro("min")
#pragma pop_macro("max")