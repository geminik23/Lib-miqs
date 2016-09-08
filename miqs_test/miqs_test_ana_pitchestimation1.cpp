#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;
#define miqs_TEST_OBJ_NAME pitch_estimation1



#include <iomanip>
#include <numeric>




void miqs_TEST_OBJ_NAME::process()
{

	double freq = 440.0, freq2 = 20000.0;
	double minFreq = 300.0, maxFreq = 500.0;
	miqs::uint32_t sr = 48000;
	size_t size = 48000;

	// sample interval... ( samplerate / freq / 2.0 )
	auto bmin = static_cast<size_t>(std::round(sr / maxFreq/2)); 
	auto bmax = static_cast<size_t>(std::round(sr / minFreq/2));

	std::cout << std::setprecision(4) << std::fixed;


	std::vector<sample_t> s1(size);
	std::vector<sample_t> s2(size);

	miqs::phasor phase{ freq, sr };
	miqs::generator<miqs::sine_wave, miqs::phasor> gen{ phase };

	std::generate(std::begin(s1), std::end(s1), gen);

	phase.set_frequency(freq2);
	phase.reset();
	std::generate(std::begin(s2), std::end(s2), gen);

	std::transform(std::begin(s1), std::end(s1), std::begin(s2), std::begin(s1), [](auto &&v1, auto&& v2) {
		return v1 + v2;
	});



	// normal method
	double result = 0;
	result = zero_cross_to_freq(miqs::zero_cross(std::begin(s1), std::end(s1)), size, sr);
	
	std::cout << "::Normal result::\n";
	std::cout << result << " Hz\n\n";

	
	// problem....
	// bounded method
	result = zero_cross_to_freq(miqs::bounded_zero_cross(std::begin(s1), std::end(s1), std::make_pair(bmin, bmax)), size, sr);
	std::cout << "::Bounded:: min: " << minFreq << ", max: " << maxFreq << "\n";
	std::cout << result << " Hz\n\n";


	// use moving average
	std::transform(std::begin(s1) + 1, std::end(s1), std::begin(s1), std::begin(s1),[](auto&&v1, auto&&v2) {
		return v1 + v2 / 2.0;
	});
	result = zero_cross_to_freq(miqs::zero_cross(std::begin(s1), std::end(s1)), size, sr);

	std::cout << "::moving average result::\n";
	std::cout << result << " Hz\n\n";


}

#undef miqs_TEST_OBJ_NAME
