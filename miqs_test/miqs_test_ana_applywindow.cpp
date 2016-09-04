#include "miqs_test.h"

#include <iomanip>

using namespace miqs_test;
using namespace miqs;



#define APL_TEST_OBJ_NAME ana_apply_window

/* Fixed Window
*/

constexpr int Window_Length = 128;



void APL_TEST_OBJ_NAME::process()
{
	//auto * data = this->get_data();
	
	miqs::phasor phase{ 440.0, 48000 };
	std::vector<sample_t> signal(256);
	std::vector<sample_t> output(256);

	
	miqs::generator<miqs::sine_wave, miqs::phasor> gen{phase};
	std::generate(std::begin(signal), std::end(signal), gen); // not ref

	
	miqs::phasor_index w_ph;

	std::cout << std::setw(4) << std::fixed<<std::setprecision(3);
	// rectangular 
	std::cout << "::rectangular::\n";
	w_ph.reset();
	miqs::generator<miqs::window::rectangular<Window_Length>, miqs::phasor_index> rec(w_ph);
	std::transform(std::begin(signal), std::end(signal), std::begin(output), miqs::make_trans_wrapper(rec));
	std::copy(std::begin(output), std::end(output), std::ostream_iterator<miqs::sample_t>(std::cout, " "));
	std::cout << "\n\n";

	// hanning
	std::cout << "::hanning::\n";
	w_ph.reset();
	miqs::generator<miqs::window::hanning<Window_Length>, miqs::phasor_index> hanning(w_ph);
	std::transform(std::begin(signal), std::end(signal), std::begin(output), miqs::make_trans_wrapper(hanning));
	std::copy(std::begin(output), std::end(output), std::ostream_iterator<miqs::sample_t>(std::cout, " "));
	std::cout << "\n\n";

	// blackman
	std::cout << "::blackman::\n";
	w_ph.reset();
	miqs::generator<miqs::window::blackman<Window_Length>, miqs::phasor_index> black(w_ph);
	std::transform(std::begin(signal), std::end(signal), std::begin(output), miqs::make_trans_wrapper(black));
	std::copy(std::begin(output), std::end(output), std::ostream_iterator<miqs::sample_t>(std::cout, " "));
	std::cout << "\n\n";

	// triangular
	std::cout << "::triangular::\n";
	w_ph.reset();
	miqs::generator<miqs::window::triangular<Window_Length>, miqs::phasor_index> tri(w_ph);
	std::transform(std::begin(signal), std::end(signal), std::begin(output), miqs::make_trans_wrapper(tri));
	std::copy(std::begin(output), std::end(output), std::ostream_iterator<miqs::sample_t>(std::cout, " "));
	std::cout << "\n\n";
	
}


#undef APL_TEST_OBJ_NAME