#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>
#include <memory>
#include "..\Shape.h"

using namespace The;


namespace GRANSAC
{


//	typedef double double;

	// Each abstract model is made of abstract parameters
	// Could be anything from a point (that make a 2D line or 3D plane or image correspondences) to a line
// 	class AbstractParameter
// 	{
// 	public:
// 		virtual ~AbstractParameter(void) {}; // To make this polymorphic we add dummy destructor
// 	};

	// Abstract model type for generic RANSAC model fitting
	template <int t_NumParams> /* Minimum number of parameters required to define this model*/
	class AbstractModel
	{
	protected:
		std::array<std::shared_ptr<dPoint>, t_NumParams> m_MinModelParams;
	public:
		double	m_totalTh;	/*전체데이터 표준 편차*/
		double	m_inlierTh;	/*계산 결과의 표준 편차*/

	public:
		virtual double ComputeDistanceMeasure(std::shared_ptr<dPoint> Param) = 0;

		virtual void Initialize(std::vector<std::shared_ptr<dPoint>> InputParams) = 0;
		virtual std::pair<double, std::vector<std::shared_ptr<dPoint>>> Evaluate(std::vector<std::shared_ptr<dPoint>> EvaluateParams, double Threshold) = 0;

		virtual std::array<std::shared_ptr<dPoint>, t_NumParams> GetModelParams(void) { return m_MinModelParams; };
	};

// 	class Point2D : public Point<double>, public GRANSAC::AbstractParameter
// 	{
// 	public:
// 		Point2D(double _x, double _y)
// 		{
// 			x = _x;	y = _y;
// 		};
// 	};

} // namespace GRANSAC
