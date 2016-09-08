#pragma once

#include "miqs_basictype.h"
#include "miqs_cont_sample_wrapper.h"

namespace miqs
{

	/** canonical filter
	- a[0] is ignored.
	- xh[n] = x[n] - a[1]*x[n-1] - a[2]*x[n-2];
	- y[n] = b[0]xh[n] + b[1]xh[n-1] + b[2]xh[n-2]
	*/
	struct canonical_filter
	{
		template<typename _A_Cont, typename _B_Cont, typename _D_Cont>
		canonical_filter(_A_Cont& a, _B_Cont& b, _D_Cont& dx)
			: m_a{ a.data() }, m_b{ b.data() }, m_dx{ dx.data() } {}

		canonical_filter(sample_t * a, sample_t* b, sample_t * dx)
			: m_a{ a }, m_b{ b }, m_dx{ dx } {}


		sample_t operator()(sample_t in)
		{
			sample_t t = in - m_a[1] * m_dx[0] - m_a[2] * m_dx[1];
			sample_t out = m_b[0] * t + m_b[1] * m_dx[0] + m_b[2] * m_dx[1];
			m_dx[1] = m_dx[0];
			m_dx[0] = t;
			return out;
		}

	private:
		sample_wrapper<2> m_dx;
		sample_wrapper<3> m_a;
		sample_wrapper<3> m_b;
	};

}