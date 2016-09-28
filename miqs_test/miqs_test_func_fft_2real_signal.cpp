#include "miqs_test.h"


using namespace miqs_test;
using namespace miqs;


void miqs_test::funcs::fft_2real_signal()
{
	std::cout << std::setprecision(4) << std::fixed;

	/* fast convolution using fft */
	/* fft 2 real signals <same length> */
	// fast convolution -> 2 real signal fft,  divide and multiplication, ifft

	size_t size{ 32 };

	std::vector<sample_t> signal1(size);
	std::vector<sample_t> signal2(size);

	std::vector<std::complex<sample_t>> fft_signal1(size);
	std::vector<std::complex<sample_t>> fft_signal2(size);



	// generate sine signal
	miqs::phasor phase(1, size);
	auto gen_sinewave = miqs::make_generator<miqs::sine_wave>(phase);

	std::generate(std::begin(signal1), std::end(signal1) - 16, gen_sinewave);
	signal2[0] = 1.0;
	signal2[1] = 1.0;
	signal2[3] = 1.0;
	signal2[9] = 1.0;



	miqs::fft_two_real_signal<sample_t>(std::begin(signal1), std::end(signal1), std::begin(signal2),
										std::begin(fft_signal1), std::begin(fft_signal2));



	miqs::transforms_fft<false> ifft;

	// CHECK!!!!
	ifft(fft_signal1);
	ifft(fft_signal2);

	std::cout << ":::::Check:::::\n\n";


	std::cout << "::ORIGINAL Signal1::\n";
	std::copy(std::begin(signal1), std::end(signal1), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";
	std::cout << "::RESULT Signal1::\n";
	std::copy(std::begin(fft_signal1), std::end(fft_signal1), std::ostream_iterator < std::complex<sample_t> >(std::cout, " "));
	std::cout << "\n";

	std::cout << "::ORIGINAL Signal2::\n";
	std::copy(std::begin(signal2), std::end(signal2), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";
	std::cout << "::RESULT Signal2::\n";
	std::copy(std::begin(fft_signal2), std::end(fft_signal2), std::ostream_iterator < std::complex<sample_t> >(std::cout, " "));
	std::cout << "\n\n";
}