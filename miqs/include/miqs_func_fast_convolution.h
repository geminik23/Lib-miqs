#pragma once
#include "miqs_basictype.h"
#include "miqs_func_fft_two_signal.h"

namespace miqs
{

	/* 2 Iterator's length must be same */
	template <typename _Ty, typename _InIt1, typename _InIt2, typename _OutIt>
	void fast_convolution(_InIt1 first1, _InIt1 last1, _InIt2 first2, _OutIt dst)
	{
		typedef _Ty value_type;

		auto size = std::distance(first1, last1);

		std::vector<std::complex<value_type>> fft_signal1(size);
		std::vector<std::complex<value_type>> fft_signal2(size);

		miqs::multichannel_array_accessor<value_type> acc_s1(reinterpret_cast<value_type *>(fft_signal1.data()), 2, size);


		miqs::fft_two_real_signal<sample_t>(first1, last1, first2,
											std::begin(fft_signal1), std::begin(fft_signal2));

		miqs::transforms_backward_fft ifft;
		std::transform(std::begin(fft_signal1), std::end(fft_signal1),
					   std::begin(fft_signal2),
					   std::begin(fft_signal1),
					   std::multiplies<std::complex<value_type>>());


		ifft(fft_signal1);

		std::copy(miqs::begin(acc_s1, 0), miqs::end(acc_s1, 0), dst);
	}


}