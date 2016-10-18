#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	namespace osc
	{

		// sine wave operator
		struct sine_wave
		{
			typedef sample_t result_type;
			typedef double argument_type;

			result_type operator()(argument_type phase) { return std::sin(phase); }
		};

		// cosine wave operator
		struct cosine_wave
		{
			typedef sample_t result_type;
			typedef double argument_type;

			result_type operator()(argument_type phase) { return std::cos(phase); }
		};

		// rectangular_wave operator
		struct rectangular_wave
		{
			typedef sample_t result_type;
			typedef double argument_type;

			result_type operator()(argument_type phase) { return (phase <= miqs::pi) ? 1.0 : -1.0; }
		};

		// triagular_wave operator
		struct triagular_wave
		{
			typedef sample_t result_type;
			typedef double argument_type;

			result_type operator()(argument_type phase)
			{
				result_type t = (2.0 *(phase *(1.0 / miqs::two_pi))) - 1.0;
				if (t < 0.0) t *= -1;
				t = 2.0*(t - 0.5);
				return t;
			}
		};

		// saw_wave operator
		struct saw_wave
		{
			typedef sample_t result_type;
			typedef double argument_type;

			result_type operator()(argument_type phase)
			{
				return (2.0 *(phase *(1.0 / miqs::two_pi))) - 1.0;
			}
		};
	}


}