#include "miqs_test.h"


using namespace miqs_test;
using namespace miqs;



miqs_test::basic_ops::basic_ops()
{
	this->phasor.set_frequency(440.0);
	this->phasor.set_samplerate(get_info().samplerate);
	this->phasor.update();
}

void miqs_test::basic_ops::process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length)
{
	(void)in;(void)nchi;(void)ncho;
	//audio_info& info = this->get_info();
	auto& phase = this->phasor;

	miqs::array_accessor<sample_t> arr{ out, length };


	miqs::generator<sine_wave, decltype(phase)> osc{ phase };
	std::generate(std::begin(arr), std::end(arr), osc);


	std::for_each(std::begin(arr), std::end(arr), miqs::plus_const<sample_t>(0.2));
	std::for_each(std::begin(arr), std::end(arr), miqs::multiplies_const<sample_t>(1.87));
	std::for_each(std::begin(arr), std::end(arr), miqs::minus_const<sample_t>(-0.2));
	std::for_each(std::begin(arr), std::end(arr), miqs::clipping<sample_t>(-0.999, 0.999));

	
}

