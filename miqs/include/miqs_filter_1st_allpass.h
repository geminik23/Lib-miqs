#pragma once

#include "miqs_basictype.h"
#include "miqs_cont_sample_wrapper.h"

namespace miqs
{
	/**============================================**/
	/*     First Order Allpass Filter Based         */
	/*      A(z) = (z^-1 + c ) / (1 + cz^-1)        */
	/**============================================**/

	/* first_order_allpass_filter */
	struct first_order_allpass_filter
	{
		template<typename _D_Cont>
		first_order_allpass_filter(sample_t* c, _D_Cont& dx)
			: m_c{ *c }, m_dx{ dx.data() } {}

		first_order_allpass_filter(sample_t* c, sample_t * dx)
			: m_c{ *c }, m_dx{ dx } {}

		sample_t operator()(sample_t in)
		{
			sample_t t = in - m_c * m_dx[0];
			sample_t out = m_c * t + m_dx[0];
			m_dx[0] = t;
			return out;
		}

	private:
		sample_wrapper<1> m_dx;
		sample_t& m_c;
	};


}