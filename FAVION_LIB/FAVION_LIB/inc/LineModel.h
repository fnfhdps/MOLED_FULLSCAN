#pragma once

#include "AbstractModel.h"


using namespace The;
using namespace GRANSAC;

/*typedef std::array<double, 2> Vector2VP;*/

class Line2DModel
	: public GRANSAC::AbstractModel < 2 >
{
protected:
	// Parametric form
	double m_a, m_b, m_c; // ax + by + c = 0
	double m_DistDenominator; // = sqrt(a^2 + b^2). Stored for efficiency reasons

	// Another parametrization y = mx + d
	double m_m; // Slope
	double m_d; // Intercept

public:
	virtual double ComputeDistanceMeasure(std::shared_ptr<dPoint> Param) override
	{
		auto ExtPoint2D = std::dynamic_pointer_cast<dPoint>(Param);
		if (ExtPoint2D == nullptr)
			throw std::runtime_error("Line2DModel::ComputeDistanceMeasure() - Passed parameter are not of type Point2D.");

		// Return distance between passed "point" and this line
		// http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
		double Numer = fabs(m_a * ExtPoint2D->x + m_b * ExtPoint2D->y + m_c);
		double Dist = Numer / m_DistDenominator;

		// // Debug
		// std::cout << "Point: " << ExtPoint2D->m_Point2D[0] << ", " << ExtPoint2D->m_Point2D[1] << std::endl;
		// std::cout << "Line: " << m_a << " x + " << m_b << " y + "  << m_c << std::endl;
		// std::cout << "Distance: " << Dist << std::endl << std::endl;

		return Dist;
	};


	Line2DModel(std::vector<std::shared_ptr<dPoint>> InputParams, double v)
	{
		Initialize(InputParams);
	};

	virtual void Initialize(std::vector<std::shared_ptr<dPoint>> InputParams) override
	{
		if (InputParams.size() != 2)
			throw std::runtime_error("Line2DModel - Number of input parameters does not match minimum number required for this model.");

		// Check for AbstractParamter types
		auto Point1 = std::dynamic_pointer_cast<dPoint>(InputParams[0]);
		auto Point2 = std::dynamic_pointer_cast<dPoint>(InputParams[1]);
		if (Point1 == nullptr || Point2 == nullptr)
			throw std::runtime_error("Line2DModel - InputParams type mismatch. It is not a Point2D.");

		std::copy(InputParams.begin(), InputParams.end(), m_MinModelParams.begin());

		// Compute the line parameters
		m_m = (Point2->y - Point1->y) / (Point2->x - Point1->x);	// Slope
		m_d = Point1->y - m_m * Point1->x;							// Intercept
		// m_d = Point2->m_Point2D[1] - m_m * Point2->m_Point2D[0]; // Intercept - alternative should be the same as above

		// mx - y + d = 0
		m_a = m_m;
		m_b = -1.0;
		m_c = m_d;

		m_DistDenominator = sqrt(m_a * m_a + m_b * m_b); // Cache square root for efficiency
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
