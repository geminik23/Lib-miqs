#pragma once

#include "miqs_basictype.h"
#include "miqs_filter_1st_allpass.h"

namespace miqs
{


	/* allpass_based_lowpass_filter */
	struct allpass_based_lowpass_filter
	{
		allpass_based_lowpass_filter(first_order_allpass_filter& base):m_base_filter{ base } {}

		sample_t operator()(sample_t in)
		{
			return 0.5*(in + m_base_filter(in));
		}
	private:
		first_order_allpass_filter& m_base_filter;
	};


	/* allpass_based_highpass_filter */
	struct allpass_based_highpass_filter
	{
		allpass_based_highpass_filter(first_order_allpass_filter& base):m_base_filter{ base } {}

		sample_t operator()(sample_t in)
		{
			return 0.5*(in - m_base_filter(in));
		}
	private:
		first_order_allpass_filter& m_base_filter;
	};


}