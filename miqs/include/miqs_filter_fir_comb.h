#pragma once

#include "miqs_basictype.h"

namespace miqs
{


	/* fir comb filter
	y[n] = x[n] + g*x[n-M]
	*/
	template <typename _Delay>
	struct fir_comb_filter
	{
		fir_comb_filter(_Delay& delay): m_delay{ delay } {}

		fir_comb_filter(double g, _Delay& delay):gain{ g }, m_delay{ delay } {}

		void reset() noexcept { m_delay.reset(); }


		_Delay& _Get_container() noexcept { return m_delay; }

		sample_t operator()(sample_t in) noexcept
		{
			return in + gain*m_delay.operator()(in);
		}

		double gain;
	private:
		_Delay& m_delay;
	};

}