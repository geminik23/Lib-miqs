
#pragma once


#include "miqs_basictype.h"
#include "miqs_math.h"

namespace miqs
{

	inline std::pair<double, double> stereo_panning(double theta_rad, double half_rad_of_loudspeaker)
	{

		double t1 = std::tan(theta_rad);
		double t2 = std::tan(half_rad_of_loudspeaker);

		std::pair<double, double> result{ -(t1 - t2) / (t1 + t2 + std::numeric_limits<double>::epsilon()), 1.0 };
		auto a = std::sqrt(result.first*result.first + result.second + result.second);

		result.first /= a;
		result.second /= a;

		return result;
	}




}