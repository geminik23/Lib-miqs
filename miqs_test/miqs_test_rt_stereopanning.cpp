#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;
#define miqs_TEST_OBJ_NAME stereopanning

miqs_TEST_OBJ_NAME::miqs_TEST_OBJ_NAME():
	phase(440.0, 48000)
{
	angle = 0;
}


void miqs_TEST_OBJ_NAME::process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length)
{

	(void)in; (void)nchi; (void)ncho;
	miqs::array_accessor<sample_t> i1{ in , length };
	miqs::array_accessor<sample_t> i2{ in + length , length };

	miqs::array_accessor<sample_t> o1{ out , length };
	miqs::array_accessor<sample_t> o2{ out + length , length };


	std::vector<sample_t> s(length);
	miqs::generator<miqs::sine_wave, miqs::phasor> gen{ phase };
	std::generate(std::begin(s), std::end(s), gen);


	auto panning = miqs::stereo_panning(angle, 45);
	std::transform(std::begin(s), std::end(s), std::begin(o1), miqs::multiplies_const<sample_t>(panning.first));
	std::transform(std::begin(s), std::end(s), std::begin(o2), miqs::multiplies_const<sample_t>(panning.second));




	angle += 0.3;
	angle = std::fmod(angle, 360.0);
}
#undef miqs_TEST_OBJ_NAME
