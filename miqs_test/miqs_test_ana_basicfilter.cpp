#pragma once

#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;
#define miqs_TEST_OBJ_NAME ana_basicfilter

void miqs_TEST_OBJ_NAME::process(){
	std::vector<sample_t> x(10, 0.0);
	x[0] = 1.0;


	//original
	std::copy(std::begin(x), std::end(x), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";

	//
	//fir filter
	//
	std::vector<sample_t> fir_x{ x };
	std::vector<sample_t> firc{ 1.0, 0.9, 0.0 };
	std::vector<sample_t> firbuff{ 0,0 };

	miqs::fir_filter<2> firfilter{ firc, firbuff };


	//filtering
	std::transform(std::begin(fir_x), std::end(fir_x), std::begin(fir_x), firfilter);

	//result
	std::copy(std::begin(fir_x), std::end(fir_x), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	//
	//all_pole filter
	//
	std::vector<sample_t> allpole_x{ x };
	std::vector<sample_t> polec{ 0.0, 0.9, 0 };
	std::vector<sample_t> polebuff{ 0,0 };

	miqs::all_pole_filter<2> allpole{ polec, polebuff };

	//filtering
	std::transform(std::begin(allpole_x), std::end(allpole_x), std::begin(allpole_x), allpole);

	//result
	std::copy(std::begin(allpole_x), std::end(allpole_x), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	//
	//combine two filters
	//
	std::vector<sample_t> combined_x{ x };

	auto combinedFilter = make_cascades_filter(firfilter, allpole);

	//filtering
	std::transform(std::begin(combined_x), std::end(combined_x), std::begin(combined_x), combinedFilter);

	//result
	std::copy(std::begin(combined_x), std::end(combined_x), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";

}

#undef miqs_TEST_OBJ_NAME