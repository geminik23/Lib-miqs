#pragma once

#include "miqs_basictype.h"
#include "miqs_trans_fft.h"

namespace miqs
{

	template <typename _Ty, typename _InIt1, typename _InIt2, typename _OutIt1, typename _OutIt2>
	void fft_two_real_signal(_InIt1 first1, _InIt1 last1, _InIt2 first2, _OutIt1 dst1, _OutIt2 dst2)
	{
		typedef _Ty value_type;

		auto size = std::distance(first1, last1);

		std::vector<std::complex<value_type>> fft_comb(size);
		miqs::multichannel_array_accessor<sample_t> acc_comp(reinterpret_cast<sample_t *>(fft_comb.data()), 2, size);

		// copy signal1 to realpart
		std::copy(first1, last1, miqs::begin(acc_comp, 0));
		// copy signal2 to imagpart
		std::copy(first2, first2 + size, miqs::begin(acc_comp, 1));

		//fft
		miqs::transforms_fft<true> fft;
		fft(fft_comb);

		// ready operators
		//X1(k) = 1/2*[Y(k) + Y*(N - k)] 
		//X2(k) = -j / 2*[Y(k) - Y*(N - k)]
		auto plushalf = miqs::bind_func_2arg<RESULT>(
			miqs::bind_func_2arg<RIGHT>(std::plus<std::complex<sample_t>>(),
										miqs::wrap_function<std::complex<sample_t>>(
											[](std::complex<sample_t> const& v) {return std::conj(v); })),
			miqs::divides_const<std::complex<sample_t>>({ 2 ,0 }));

		auto minushalf = miqs::bind_func_2arg<RESULT>(
			miqs::bind_func_2arg<RIGHT>(std::minus<std::complex<sample_t>>(),
										miqs::wrap_function<std::complex<sample_t>>(
											[](std::complex<sample_t> const& v) {return std::conj(v); })),
			miqs::divides_const<std::complex<sample_t>>({ 0 ,2 }));


		std::transform(std::begin(fft_comb) + 1, std::end(fft_comb), std::rbegin(fft_comb), dst1 + 1, plushalf);
		*dst1 = plushalf(fft_comb[0], fft_comb[0]);
		std::transform(std::begin(fft_comb) + 1, std::end(fft_comb), std::rbegin(fft_comb), dst2 + 1, minushalf);
		*dst2 = minushalf(fft_comb[0], fft_comb[0]);


	}
}