#pragma once
#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;

#include <iomanip>
void miqs_test::funcs::fft()
{

	std::cout << std::setprecision(4) << std::fixed;

	size_t size = 64;
	std::vector<sample_t> data(size);

	auto begin = std::begin(data);
	auto end = std::end(data);

	miqs::phasor phase{ 16, size };
	miqs::generator<miqs::sine_wave, miqs::phasor> gen{ phase };


	std::generate(begin, end, gen);
	std::copy(begin, end, std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";



	std::vector<std::complex<sample_t>> comp(size);
	std::vector<std::complex<sample_t>> cp(size);

	miqs::complex_copy_from_single<real_part>(std::begin(data), std::end(data), std::begin(comp));
	
	
	miqs::transforms_fft<true> fft;
	fft(comp);
	std::cout << "::FFT::\n";
	std::copy(std::begin(comp), std::end(comp), std::ostream_iterator<std::complex<sample_t>>(std::cout, " "));
	std::cout << "\n\n";

	//---------------------------------

	std::transform(std::begin(comp), std::end(comp), std::begin(cp), miqs::cartesian_to_polar());

	std::cout << "::polar::\n";
	std::copy(std::begin(cp), std::end(cp), std::ostream_iterator<std::complex<sample_t>>(std::cout, " "));
	std::cout << "\n\n";


	//TODO	magnitude dB
	/*doing something*/
	std::vector<sample_t> magnitude(size);

	miqs::complex_copy_to_single<real_part>(std::begin(cp), std::end(cp), std::begin(magnitude));

	std::transform(std::begin(magnitude), std::end(magnitude), std::begin(magnitude), [size](auto&&v) {
		return miqs::log10_((v / (size / 2.0)), 20.0);
	});

	std::cout << "::magnitude::\n";
	std::copy(std::begin(magnitude), std::end(magnitude), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";





	//
	// back
	std::transform(std::begin(cp), std::end(cp), std::begin(comp), miqs::polar_to_cartesian());

	//---------------------------------


	miqs::transforms_fft<false> rfft;
	rfft(comp);
	std::cout << "::IFFT::\n";
	std::copy(std::begin(comp), std::end(comp), std::ostream_iterator<std::complex<sample_t>>(std::cout, " "));
	std::cout << "\n\n";

	miqs::complex_copy_to_single<real_part>(std::begin(comp), std::end(comp), std::begin(data));
	std::copy(begin, end, std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


}