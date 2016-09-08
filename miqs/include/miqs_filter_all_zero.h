#pragma once

#include "miqs_basictype.h"
#include "miqs_cont_sample_wrapper.h"

namespace miqs
{

	/* all zero filter : FIR */
	template <int _ORDER>
	struct all_zero_filter
	{
		template<typename _C_Cont, typename _D_Cont>
		all_zero_filter(_C_Cont& coef, _D_Cont& delay):m_c{ coef.data() }, m_dx{ delay.data() } {}
		all_zero_filter(sample_t* coef, sample_t* delay): m_c{ coef }, m_dx{ delay } {}

		sample_t operator()(sample_t in) noexcept
		{
			sample_t out = in * m_c[0];
			for (int j = _ORDER - 1; j > 0; j--)
			{
				out += m_c[j + 1] * m_dx[j];
				m_dx[j] = m_dx[j - 1];
			}
			out += m_c[1] * m_dx[0];
			m_dx[0] = in;
			return out;
		}

		/** [Transpose version] right?
		sample_t out = m_c[0] * in + m_dx[0];
		for (int j = 1; j < _ORDER; j++)
		m_dx[j - 1] = m_c[j] * in + m_dx[j];

		m_dx[_ORDER - 1] = m_c[_ORDER] * in;

		return out;
		*/

	private:
		sample_wrapper<_ORDER + 1> m_c;
		sample_wrapper<_ORDER> m_dx;
	};



	template <int _ORDER>
	using fir_filter = all_zero_filter<_ORDER>;
}