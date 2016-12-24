#pragma once
#include "filter.h"

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
		array_ref_n<sample_t, 1> m_dx;
		sample_t& m_c;
	};




	/* allpass_based_lowpass_filter */
	struct allpass_based_lowpass_filter
	{
		allpass_based_lowpass_filter(first_order_allpass_filter& base):m_base_filter{ base } {}

		sample_t operator()(sample_t in)
		{
			return 0.5*(in + m_base_filter(in));
		}
	private:
		first_order_allpass_filter& m_base_filter;
	};


	/* allpass_based_highpass_filter */
	struct allpass_based_highpass_filter
	{
		allpass_based_highpass_filter(first_order_allpass_filter& base):m_base_filter{ base } {}

		sample_t operator()(sample_t in)
		{
			return 0.5*(in - m_base_filter(in));
		}
	private:
		first_order_allpass_filter& m_base_filter;
	};


	
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
		array_ref_n<sample_t, 2> m_dx;
		array_ref_n<sample_t, 2> m_c;
	};



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
















	//===========================

	/* first_order_allpass_based_calculator

	c = tan((pi*fc/fs-1) / (pi*fc/fs+1))

	_IntfoTy = miqs::filter_info or miqs::filter_norm_info

	*/

	template <typename _InfoTy>
	struct first_order_allpass_calculator
	{
		first_order_allpass_calculator(_InfoTy& info):m_info{ info } {}

		void operator()(double * c)
		{
			double t = std::tan(math::Pi * m_info.normalized_cutoff_freqeuncy() / 2.0);
			*c = (t - 1) / (t + 1);
		}

	private:
		_InfoTy& m_info;
	};


	/* second_order_allpass_based_calculator

	c[0] = tan((pi*fc/fs-1) / (pi*fc/fs+1))
	c[1] = -cos(2*PI*fc/fs)

	_IntfoTy = miqs::filter_info or miqs::filter_norm_info

	*/
	template <typename _InfoTy = filter_info>
	struct second_order_allpass_calculator
	{
		second_order_allpass_calculator(_InfoTy& info):m_info{ info } {}

		template <typename _Iter>
		void operator()(_Iter c)
		{
			double t = std::tan(math::Pi * m_info.normalized_bandwidth() / 2.0);
			*c++ = (t - 1) / (t + 1);
			*c = -1 * std::cos(math::Pi*m_info.normalized_cutoff_freqeuncy() / 2.0);
		}

	private:
		_InfoTy& m_info;
	};



}