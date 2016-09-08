#pragma once

#include "miqs_basictype.h"
#include "miqs_filter_2nd_allpass.h"

namespace miqs
{

	/* allpass_based_bandpass_filter */
	struct allpass_based_bandpass_filter
	{
		allpass_based_bandpass_filter(second_order_allpass_filter& base):m_base_filter{ base } {}

		sample_t operator()(sample_t in)
		{
			return 0.5*(in - m_base_filter(in));
		}
	private:
		second_order_allpass_filter& m_base_filter;
	};

	/* allpass_based_bandreject_filter */
	struct allpass_based_bandreject_filter
	{
		allpass_based_bandreject_filter(second_order_allpass_filter& base):m_base_filter{ base } {}

		sample_t operator()(sample_t in)
		{
			return 0.5*(in + m_base_filter(in));
		}
	private:
		second_order_allpass_filter& m_base_filter;
	};



}
