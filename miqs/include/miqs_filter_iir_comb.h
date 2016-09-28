#pragma once

#include "miqs_basictype.h"

namespace miqs
{

	/* iir comb filter
	y[n] = c*x[n] + g*y[n-M]
	*/
	template <typename _Delay>
	struct iir_comb_filter
	{
		iir_comb_filter(_Delay& delay):m_delay{ delay } {}
		iir_comb_filter(double c, double g, _Delay& delay):scale{ c }, gain{ g }, m_delay{ delay } {}

		void reset() { m_delay.reset(); }

		_Delay& _Get_container() { return m_delay; }

		sample_t operator()(sample_t in)
		{
			sample_t out = scale*in + gain*m_delay.peek();
			m_delay.push(out);
			return out;
		}

		double gain{ 1.0 };
		double scale{ 1.0 };
	private:
		_Delay& m_delay;
	};

}