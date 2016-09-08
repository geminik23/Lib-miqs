#pragma once

#include "miqs_basictype.h"

namespace miqs
{

	/* filter information */
	struct filter_info
	{
		double normalized_cutoff_freqeuncy() noexcept { return 2 * cutoff_frequency / samplerate; }
		double normalized_bandwidth() noexcept { return 2 * bandwidth / samplerate; }

		miqs::uint32_t samplerate = 48000;
		double bandwidth = 0.0;
		double cutoff_frequency = 0.0;
	};


	/* filter norm information */
	struct filter_norm_info
	{
		double normalized_cutoff_freqeuncy() noexcept { return norm_cutoff_frequency; }
		double normalized_bandwidth() noexcept { return norm_bandwidth; }

		double norm_bandwidth = 0.01;
		double norm_cutoff_frequency = 0;
	};


}