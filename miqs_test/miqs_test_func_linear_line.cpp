
#include "miqs_test.h"

using namespace miqs_test;
using namespace miqs;


void miqs_test::funcs::linear_line()
{
	std::vector<sample_t> samples(512);

	miqs::phasor phase{ 6, 512 };
	miqs::generator<miqs::sine_wave, miqs::phasor> sineGen{ phase };
	std::generate(std::begin(samples), std::end(samples), sineGen);


	miqs::phasor_index idx;
	miqs::linear lin({ 0.0,1.0 }, 8);
	miqs::generator_ref<miqs::linear, miqs::phasor_index> gen{ lin, idx };

	std::transform(std::begin(samples), std::begin(samples) + 8,
				   std::begin(samples),
				   miqs::make_multiplies_iter(miqs::make_generate_iterator(gen)));
//std:copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));




}