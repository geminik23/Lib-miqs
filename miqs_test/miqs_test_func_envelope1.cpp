
#include "miqs_test.h"
#include "miqs_temp_impl.h"

using namespace miqs_test;
using namespace miqs;


void miqs_test::funcs::envelope1()
{
	/* Envelope Test */
	std::vector<sample_t> signal(16, 1.0);

	miqs::phasor_index idx;
	miqs::envelope::trapezoidal env_trap(4, 3, 4);
	miqs::generator_ref<miqs::envelope::trapezoidal, miqs::phasor_index> tragen{ env_trap, idx };

	std::transform(std::begin(signal), std::end(signal),
				   std::begin(signal),
				   miqs::make_multiplies_iter(miqs::make_generate_iterator(tragen)));


	std::cout << "::trapezoidal envelope::\n";
	std::copy(std::begin(signal), std::end(signal), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";

	//reset
	idx.reset();
	std::fill(std::begin(signal), std::end(signal), 1.0);


	miqs::envelope::bell env_bell(4, 3, 4);
	miqs::generator_ref<miqs::envelope::bell, miqs::phasor_index> bellgen{ env_bell, idx };

	std::transform(std::begin(signal), std::end(signal),
				   std::begin(signal),
				   miqs::make_multiplies_iter(miqs::make_generate_iterator(bellgen)));

	std::cout << "::bell envelope::\n";
	std::copy(std::begin(signal), std::end(signal), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";
}