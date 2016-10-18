#pragma once

#include "miqs_basictype.h"

namespace miqs
{

	/* filter information */
	struct filter_info
	{
		double normalized_cutoff_freqeuncy() noexcept { return 2 * cutoff_frequency / samplerate; }
		double normalized_bandwidth() noexcept { return 2 * bandwidth / samplerate; }

		double samplerate = 48000;
		double bandwidth = 0.0;
		double cutoff_frequency = 0.0;
	};

	enum class filter_type
	{
		allpass, lowpass, highpass, bandpass, bandreject
	};


}