
#pragma once


#include "miqs_basictype.h"
#include "miqs_math.h"

namespace miqs
{

	inline std::pair<double, double> stereo_panning(double theta_angle, double half_angle_of_loudspeaker)
	{
		theta_angle = miqs::degree_to_radian(theta_angle);
		half_angle_of_loudspeaker = miqs::degree_to_radian(half_angle_of_loudspeaker);


		double t1 = std::tan(theta_angle);
		double t2 = std::tan(half_angle_of_loudspeaker);

		std::pair<double, double> result{ -(t1 - t2) / (t1 + t2 + std::numeric_limits<double>::epsilon()), 1.0 };
		auto a = std::sqrt(result.first*result.first + result.second + result.second);

		result.first /= a;
		result.second /= a;

		return result;
	}
}