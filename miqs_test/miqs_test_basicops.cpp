#include "miqs_test.h"


using namespace miqs_test;
using namespace miqs;



void miqs_test::basic_ops::initialize()
{
	get_data()->phasor.set_frequency(440.0);
	get_data()->phasor.set_samplerate(get_info().samplerate);
	get_data()->phasor.update();
}

void miqs_test::basic_ops::process(sample_t * samples, size_t length)
{
	auto * data = this->get_data();
	//audio_info& info = this->get_info();
	auto& phase = data->phasor;

	miqs::array_accessor<sample_t> arr{ samples, length };


	miqs::generator<sine_wave, decltype(phase)> osc{ phase };
	std::generate(std::begin(arr), std::end(arr), osc);


	std::for_each(std::begin(arr), std::end(arr), miqs::op_add<sample_t>(0.2));
	std::for_each(std::begin(arr), std::end(arr), miqs::op_multiply<sample_t>(1.87));
	std::for_each(std::begin(arr), std::end(arr), miqs::op_add<sample_t>(-0.2));
	std::for_each(std::begin(arr), std::end(arr), miqs::op_clip<sample_t>(-0.999, 0.999));


}

