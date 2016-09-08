#pragma once

#include "miqs_basictype.h"

namespace miqs
{

	/**=========================================================**/
	/*           Second Order Allpass Filter Based               */
	/*   A(z) = (-c + d(1-c)z-1 + z-2 ) / (1 + d(1-c)z-1 - cz-2) */
	/**=========================================================**/

	/* second_order_allpass_filter */
	struct second_order_allpass_filter
	{
		template<typename _C_Cont, typename _D_Cont>
		second_order_allpass_filter(_C_Cont& c, _D_Cont& dx)
			: m_c{ c.data() }, m_dx{ dx.data() } {}

		second_order_allpass_filter(sample_t * c, sample_t * dx)
			: m_c{ c }, m_dx{ dx } {}


		sample_t operator()(sample_t in)
		{
			sample_t t = in - m_c[1] * (1 - m_c[0])*m_dx[0] + m_c[0] * m_dx[1];
			sample_t out = -m_c[0] * t + m_c[1] * (1 - m_c[0])*m_dx[0] + m_dx[1];
			m_dx[1] = m_dx[0];
			m_dx[0] = t;
			return out;
		}

	private:
		sample_wrapper<2> m_dx;
		sample_wrapper<2> m_c;
	};
}