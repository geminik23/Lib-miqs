#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;

void miqs_test::funcs::combfilter() {
	std::cout << std::fixed << std::setprecision(3);

	std::vector<sample_t> original(12, 0.0);
	original[0] = 1.0;


	std::vector<sample_t> samples = original;


	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	/* fir comb filter */
	std::vector<sample_t> dbuff(10, 0.0);
	miqs::fraction_delay<miqs::interpolation::linear> fir_delay(2.0, dbuff);
	miqs::fir_comb_filter<decltype(fir_delay)> fircomb{ 0.9, fir_delay };


	std::cout << "::FIR COMB FILTER - fraction_delay(2.0)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), fircomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";



	//reset
	samples = original;


	/* iir comb filter */
	miqs::one_sample_delay iir_delay;
	miqs::iir_comb_filter<decltype(iir_delay)> iircomb{ 0.6,0.6, iir_delay };

	std::cout << "::IIR COMB FILTER - delay(1)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), iircomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";



	//reset
	samples = original;


	fir_delay.reset();

	/* universal comb filter */
	miqs::universal_comb_filter <decltype(fir_delay)> ucomb{ fir_delay };
	// fir comb filter
	ucomb.set_fir_comb_filter(0.9);

	std::cout << "::UNIVERSAL COMB FILTER [FIR COMB FILTER]- delay(2.0)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), ucomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";



	//reset
	samples = original;
	fir_delay.reset();
	fir_delay.set_delay(1.0);
	// iir comb filter
	ucomb.set_iir_comb_filter(0.6, 0.6);

	std::cout << "::UNIVERSAL COMB FILTER [IIR COMB FILTER]- delay(1.0)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), ucomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	//reset
	samples = original;
	fir_delay.reset();
	fir_delay.set_delay(4.0);

	// delay
	ucomb.set_delay();
	std::cout << "::UNIVERSAL COMB FILTER [Delay]- delay(4.0)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), ucomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	//reset
	samples = original;
	fir_delay.reset();
	fir_delay.set_delay(1.0);

	// allpass
	sample_t c = 0.9;

	ucomb.set_allpass_filter(c);
	std::cout << "::UNIVERSAL COMB FILTER [Allpass]- delay(1.0)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), ucomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	//reset
	samples = original;
	fir_delay.reset();

	miqs::first_order_allpass_filter allpass{ &c, fir_delay._Get_container()._Get_container() };
	std::cout << "::1st ORDER ALLPASS FILTER::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), allpass);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";
}