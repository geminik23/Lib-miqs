#pragma once
#include <miqs.h>

#include <iomanip>
#include "miqs_temp_impl.h"

using namespace miqs;
namespace miqs_test
{


	void newfea_normalize()
	{
		std::vector<sample_t> samples(256);
		// normalize
		//std::transform(std::begin(samples), std::end(samples), std::begin(samples),
		//			   miqs::divides_const<sample_t>(std::abs(*std::max_element(std::begin(samples), std::end(samples))))
		//);
		miqs::normalize(std::begin(samples), std::end(samples), 1.0);
		miqs::normalize_with_max(std::begin(samples), std::end(samples));


	}
}