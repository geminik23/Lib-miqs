#pragma once
#include "base.h"

namespace miqs
{
	enum class filter_type
	{
		allpass, lowpass, highpass, bandpass, bandreject
	};

	/* filter data */
	struct filter_info
	{
		double normalized_cutoff_freqeuncy() noexcept { return 2 * cutoff_frequency / samplerate; }
		double normalized_bandwidth() noexcept { return 2 * bandwidth / samplerate; }

		double samplerate = 48000;
		double bandwidth = 0.0;
		double cutoff_frequency = 0.0;
	};






	//========================

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
		array_ref_n<sample_t, _ORDER + 1> m_c;
		array_ref_n<sample_t, _ORDER> m_dx;
	};

	template <int _ORDER>
	using fir_filter = all_zero_filter<_ORDER>;

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
		array_ref_n<sample_t, _ORDER + 1> m_c;
		array_ref_n<sample_t, _ORDER> m_dy;
	};


}