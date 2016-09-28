
#include "miqs_test.h"

using namespace miqs_test;
using namespace miqs;


void miqs_test::funcs::fast_convolution()
{

	std::cout << std::setprecision(4) << std::fixed;

	/* fast convolution using fft */
	/* fft 2 real signals <same length> */
	// fast convolution -> 2 real signal fft,  divide and multiplication, ifft

	constexpr size_t size{ 32 };

	std::vector<sample_t> signal1(size);
	std::vector<sample_t> signal2(size);
	std::vector<sample_t> result(size);

	// generate sine signal
	miqs::phasor phase(2, size);
	auto gen_sinewave = miqs::make_generator<miqs::sine_wave>(phase);
	std::generate(std::begin(signal1), std::begin(signal1) + size / 2, gen_sinewave);
	//std::generate(std::begin(signal2), std::end(signal2), gen_sinewave);


	signal2[0] = 1.0;
	signal2[0] = 0.8;
	signal2[0] = 0.2;
	signal2[0] = 0.012;


	miqs::fast_convolution<sample_t>(std::begin(signal1), std::end(signal1), std::begin(signal2), std::begin(result));



	std::cout << ":::::RESULT:::::\n\n";
	std::copy(std::begin(result), std::end(result), std::ostream_iterator < sample_t >(std::cout, " "));
	std::cout << "\n\n";


	std::vector<sample_t> dl(4, 0.0);
	miqs::all_zero_filter<3> f{ signal2, dl };
	std::transform(std::begin(signal1), std::end(signal1), std::begin(result), f);
	std::cout << ":::::Compare with Traditional Convolutional :::::\n\n";
	std::copy(std::begin(result), std::end(result), std::ostream_iterator < sample_t >(std::cout, " "));
	std::cout << "\n\n";



}