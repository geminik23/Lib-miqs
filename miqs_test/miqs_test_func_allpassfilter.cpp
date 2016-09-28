
#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;

void miqs_test::funcs::allpassfilter()
{
	std::vector<sample_t> buffer(2,0.0);

	miqs::filter_info allpass_info;
	allpass_info.samplerate = 48000; // samplerate
	allpass_info.cutoff_frequency = 440.0; // 440hz;
	allpass_info.bandwidth = 100; // 100hz;

	std::vector<sample_t> data(512);


	double coeff[2];
	
	// calculate allpass coefficient from filter info(filter_info)
	miqs::first_order_allpass_calculator<miqs::filter_info>{allpass_info}(coeff);


	miqs::first_order_allpass_filter allpass1{ coeff, buffer };

	miqs::allpass_based_lowpass_filter lowpass{ allpass1 };
	miqs::allpass_based_highpass_filter highpass{ allpass1 };






	// reset
	buffer[0] = buffer[1] = 0.0;

	miqs::second_order_allpass_calculator < miqs::filter_info> { allpass_info }(coeff);
	miqs::second_order_allpass_filter allpass2{ coeff, buffer };
	miqs::allpass_based_bandpass_filter bandpass{ allpass2 };
	miqs::allpass_based_bandreject_filter bandreject{ allpass2 };




	std::transform(std::begin(data), std::end(data), std::begin(data), bandpass);

	std::transform(std::begin(data), std::end(data), std::begin(data), miqs::allpass_based_highpass_filter{ allpass1 });




}
