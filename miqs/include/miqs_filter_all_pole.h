#pragma once

#include "miqs_basictype.h"
#include "miqs_cont_sample_wrapper.h"

namespace miqs
{
	/** all pole filter :
	- coefficients[0]  is ignored
	- y[n] = x - a[1]y[n - 1] - a[2]y[n - 2] ...
	*/
	template <int _ORDER>
	struct all_pole_filter
	{
		template<typename _C_Cont, typename _D_Cont>
		all_pole_filter(_C_Cont& coef, _D_Cont& delay):m_c{ coef.data() }, m_dy{ delay.data() } {}
		all_pole_filter(sample_t * coef, sample_t * delay):m_c{ conef }, m_dy{ delay } {}

		sample_t operator()(sample_t in)
		{
			sample_t out = in;
			for (int j = _ORDER - 1; j > 0; j--)
			{
				out -= m_c[j + 1] * m_dy[j];
				m_dy[j] = m_dy[j - 1];
			}
			out -= m_c[1] * m_dy[0];
			m_dy[0] = out;

			return out;
		}

		/** [Transpose version]
		sample_t out = in - m_dy[0];
		for (int j = 1; j < _ORDER; ++j)
		m_dy[j - 1] = m_dy[j] + m_c[j] * out;
		m_dy[_ORDER - 1] = m_dy[_ORDER] * out;
		return out;
		*/

	private:
		sample_wrapper<_ORDER + 1> m_c;
		sample_wrapper<_ORDER> m_dy;
	};
}