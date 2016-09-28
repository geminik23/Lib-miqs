#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;
#define miqs_TEST_OBJ_NAME modulations

miqs_TEST_OBJ_NAME::miqs_TEST_OBJ_NAME():
	phase1(440.0, 48000),
	phase2(200.0, 48000)
{}


void miqs_TEST_OBJ_NAME::process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length)
{
	(void)in; (void)nchi; (void)ncho;
	std::vector<sample_t> s1(length);
	std::vector<sample_t> s2(length);
	miqs::array_accessor<sample_t> result(out, length);


	miqs::generator<miqs::sine_wave, miqs::phasor> gen1{ phase1 };
	miqs::generator<miqs::sine_wave, miqs::phasor> gen2{ phase2 };


	std::generate(std::begin(s1), std::end(s1), gen1);
	std::generate(std::begin(s2), std::end(s2), gen2);


	/* ring modulation */

	//use standard functor
	//std::transform(std::begin(s1), std::end(s1), std::begin(s2), std::begin(result), std::multiplies<sample_t>());
	//std::transform(std::begin(s1), std::end(s1), std::begin(result), miqs::make_multiplies_iter(std::begin(s2)));


	/* amplitude modulation */
	// 1+a*modulator

	sample_t scale = 0.9;
	//auto am = miqs::make_cascades_functions<sample_t>(std::bind(std::plus<sample_t>(), 1.0, std::placeholders::_1),
	auto am = miqs::make_cascades_functions<sample_t>(miqs::plus_const<sample_t>(1.0),
													  miqs::multiplies_ref<sample_t>(scale),
													  miqs::make_multiplies_iter(std::begin(s2)));
	std::transform(std::begin(s1), std::end(s1), std::begin(result), am);


}
#undef miqs_TEST_OBJ_NAME
