#pragma once

#include "miqs_basictype.h"

namespace miqs
{

	/** universal_comb_filter
	* bl (blend param)
	* fb (feedback param)
	* ff (feedforward param)
	*
	* xh = x[n] + fb*xh[n-M];
	* y[n] = ff*xh[n-M] + bl*xh;
	*                      BL   FB   FF
	* FIR comb filter -    1    0    g
	* IIR comb filter -    c    g    0
	* Allpass         -    a    -a   1
	* Delay           -    0    0    1
	* _Delay::reset(), _Delay::peek()
	*/
	template <typename _Delay>
	struct universal_comb_filter
	{
		universal_comb_filter(_Delay& delay):m_delay{ delay } {}

		universal_comb_filter(double param_blend, double param_feedback, double param_feedforward, _Delay& delay)
			:param_blend{ param_blend }
			, param_feedback{ param_feedback }
			, param_feedforward{ param_feedforward }
			, m_delay{ delay }
		{}

		void set_fir_comb_filter(double gain) { param_blend = 1.0; param_feedback = 0.0; param_feedforward = gain; }
		void set_iir_comb_filter(double scale, double gain) { param_blend = scale; param_feedback = gain; param_feedforward = 0.0; }
		void set_allpass_filter(double c) { param_blend = c; param_feedback = -c; param_feedforward = 1.0; }
		void set_delay() { param_blend = 0.0; param_feedback = 0.0; param_feedforward = 1.0; }

		void reset() { m_delay.reset(); }

		_Delay& _Get_container() { return m_delay; }

		sample_t operator()(sample_t in)
		{
			sample_t x_new = in + param_feedback*m_delay.peek();
			sample_t out = param_feedforward*m_delay.peek() + param_blend*x_new;
			m_delay.push(x_new);
			return out;
		}

		double param_blend{}; // blend
		double param_feedback{}; // feedback
		double param_feedforward{}; // feedforward

	private:
		_Delay& m_delay;
	};

}