#pragma once

#include "miqs_algo_basics.h"
#include "miqs_math.h"

namespace miqs
{
	template <typename _Ty=sample_t>
	struct real_noise_data
	{
		std::default_random_engine random_engine{std::random_device()()};
		std::uniform_real_distribution<_Ty> distribution{ -1,1 };
	};


	template <typename _PtrIt, typename _NoiseData>
	inline void ptr_fill_noise(_PtrIt first, _PtrIt last, _NoiseData data)
	{
		for (; first != last; ++first)
		{
			*first = data.distribution(data.random_engine);
		}
	}
	
	template <typename _PtrIt>
	inline void ptr_fill_zero(_PtrIt first, _PtrIt last)
	{
		for (; first != last; ++first)
		{
			*first = static_cast<std::iterator_traits<_PtrIt>::value_type>(0);
		}
	}


	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_phasor(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	{
		for (; first != last; ++first)
		{
			*first = phase_info.phase / miqs::two_pi;

			phase_info.next();
			MIQS_PHASE_MODULAR_DOWN(phase_info, miqs::two_pi);
			MIQS_PHASE_MODULAR_UP(phase_info, miqs::two_pi);
		}
	}

	//template <typename _PtrIt, typename _PhaseInfo>
	//inline void ptr_fill_impulse(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	//{
	//	bool check{ phase_info.phase == 0 };
	//	for (; first != last; ++first)
	//	{
	//		*first = (check) ? 1.0 : 0.0;
	//		phase_info.next();
	//		if (phase_info.phase >= miqs::two_pi)
	//		{
	//			phase_info.phase -= miqs::two_pi;
	//			*first = 1.0;
	//		}
	//		else
	//			*first = 0.0;
	//	}
	//}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_sine(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	{
		for (; first != last; ++first)
		{
			*first = std::sin(phase_info.phase);
			phase_info.next();
			MIQS_PHASE_MODULAR_DOWN(phase_info, miqs::two_pi);
			MIQS_PHASE_MODULAR_UP(phase_info, miqs::two_pi);
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_rectangular(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	{
		for (; first != last; ++first)
		{
			*first = (phase_info.phase <= miqs::pi) ? 1.0 : -1.0;
			phase_info.next();
			MIQS_PHASE_MODULAR_DOWN(phase_info, miqs::two_pi);
			MIQS_PHASE_MODULAR_UP(phase_info, miqs::two_pi);
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_saw(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	{
		for (; first != last; ++first)
		{
			*first = (2.0 *(phase_info.phase *(1.0 / miqs::two_pi))) - 1.0;
			phase_info.next();
			MIQS_PHASE_MODULAR_DOWN(phase_info, miqs::two_pi);
			MIQS_PHASE_MODULAR_UP(phase_info, miqs::two_pi);
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_triangular(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	{
		for (; first != last; ++first)
		{
			*first = (2.0 *(phase_info.phase *(1.0 / miqs::two_pi))) - 1.0;
			if (*first < 0.0) *first *= -1;
			*first = 2.0*(*first - 0.5); 
			phase_info.next();
			MIQS_PHASE_MODULAR_DOWN(phase_info, miqs::two_pi);
			MIQS_PHASE_MODULAR_UP(phase_info, miqs::two_pi);
		}
	}

	//---------- WINDOW

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_window_hanning(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info, size_t wsize)
	{
		for (; first != last; ++first)
		{
			if (phase_info.phase >= (decltype(phase_info.phase))wsize) *first = 0.0;
			else *first =  (0.5) - (0.5 * std::cos(miqs::pi * 2 * phase_info.phase / (double)(wsize - 1.0)));
			phase_info.next();
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_window_hamming(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info, size_t wsize)
	{
		for (; first != last; ++first)
		{
			if (phase_info.phase >= (decltype(phase_info.phase))wsize) *first = 0.0;
			else *first = (0.54) - (0.46 * std::cos(miqs::pi * 2 * phase_info.phase / (double)(wsize - 1.0)));
			phase_info.next();
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_window_blackman(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info, size_t wsize)
	{
		double arg = 2 * miqs::pi / (wsize - 1);
		double x;
		for (; first != last; ++first)
		{
			if (phase_info.phase >= (decltype(phase_info.phase))wsize) *first = 0.0;
			else
			{
				x = arg * phase_info.phase;
				*first = 0.42659 - 0.49656 * std::cos(x) + 0.076849 * std::cos(x + x);
			}
			phase_info.next();
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_window_triangular(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info, size_t wsize)
	{
		for (; first != last; ++first)
		{
			if (phase_info.phase >= (decltype(phase_info.phase))wsize) *first = 0.0;
			else *first = 1.0 - (std::abs((int)phase_info.phase * 2 - (int)wsize + 1) / (double)(wsize - 1.0));
			phase_info.next();
		}
	}





}