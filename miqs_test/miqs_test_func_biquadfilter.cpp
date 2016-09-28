#pragma once

#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;

#include <iomanip>

void miqs_test::funcs::biquadfilter()
{
	std::cout << std::setprecision(4) << std::fixed;

	size_t size = 512;
	std::vector<sample_t> data(size);

	miqs::phasor phase{ 440.0, 48000 };
	miqs::generator<miqs::sine_wave, miqs::phasor> gen{ phase };

	std::generate(std::begin(data), std::end(data), gen);
	

	std::vector<sample_t> coeff_a(3), coeff_b(3), buff{ 0.0,0.0 };


	// filtering
	miqs::filter_info info;
	info.bandwidth = 100;
	info.cutoff_frequency = 1000;

	//miqs::canonical_coefficients_calculator<miqs::canonical::second_order_lowpass, miqs::filter_info>{info}(std::begin(coeff_a), std::begin(coeff_b));
	miqs::calculate_canonical_coefficients<miqs::canonical::second_order_lowpass>(info, std::begin(coeff_a), std::begin(coeff_b));
	

	miqs::canonical_filter filter{coeff_a, coeff_b, buff};
	std::transform(std::begin(data), std::end(data), std::begin(data), filter);




}
