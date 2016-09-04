#include "miqs_test.h"


using namespace miqs_test;
using namespace miqs;

void miqs_test::generator::initialize()
{
	get_data()->phasor.set_frequency(440.0);
	get_data()->phasor.set_samplerate(get_info().samplerate);
	get_data()->phasor.update();
}

void miqs_test::generator::process(sample_t * samples, size_t length)
{
	auto * data = this->get_data();
	//audio_info& info = this->get_info();
	auto& phase = data->phasor;

	miqs::array_accessor<sample_t> arr{ samples, length };

	miqs::generator<miqs::sine_wave, miqs::phasor> osc{phase};
	std::generate(std::begin(arr), std::end(arr), osc);
}

