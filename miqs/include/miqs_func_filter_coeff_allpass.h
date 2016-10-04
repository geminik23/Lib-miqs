#pragma once
#include "miqs_basictype.h"


namespace miqs
{
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