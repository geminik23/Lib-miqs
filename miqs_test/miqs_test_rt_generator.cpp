#include "miqs_test.h"


using namespace miqs_test;
using namespace miqs;

miqs_test::generator::generator()
{
	this->phasor.set_frequency(440.0);
	this->phasor.set_samplerate(48000);
	this->phasor.update();
}

void miqs_test::generator::process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length)
{
	(void)in; (void)nchi; (void)ncho;
	//audio_info& info = this->get_info();
	auto& phase = this->phasor;

	miqs::array_accessor<sample_t> arr{ out, length };

	miqs::generator<miqs::sine_wave, miqs::phasor> osc{phase};
	std::generate(std::begin(arr), std::end(arr), osc);
}

