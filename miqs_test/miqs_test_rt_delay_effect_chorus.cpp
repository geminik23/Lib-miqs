#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;
#define miqs_TEST_OBJ_NAME delay_effect_chorus


namespace miqs
{



}

// TODO 2 Version
// 1. use fraction delay.
// 2. get from wavetables.

//delaytime 5 ~ 10ms
//5~ 14hz

miqs_TEST_OBJ_NAME::miqs_TEST_OBJ_NAME():
	width{ 80}, buffer(static_cast<size_t>(width * 2 + 20)){
	phase.set(440, 48000);
	lfphase.set(0.1, 48000);
}


void miqs_TEST_OBJ_NAME::process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length)
{
	(void)in; (void)nchi; (void)ncho;
	auto& p = this->phase;
	auto& lp = this->lfphase;
	double& w = this->width;
	auto& delay = this->buffer;

	miqs::array_accessor<sample_t> arr(out, length);

	miqs::wavetable<miqs::interpolation::linear, decltype(miqs::container(buffer))> wavetable(miqs::container(buffer));
	std::generate(std::begin(arr), std::end(arr), [&]() {
		delay(miqs::sine_wave()(p()));
		return delay.peek() + wavetable(w*miqs::sine_wave()(lp()) + delay.size() / 2);
	});

}
#undef miqs_TEST_OBJ_NAME
