#pragma once

#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;

#include <iomanip>
void miqs_test::funcs::delay()
{
	std::cout << std::fixed << std::setprecision(3);
	std::vector<sample_t> data(12, 0.0);
	data[0] = 1.0;
	

	std::cout << "::Original::\n";
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	miqs::delay d{ 3 };
	std::transform(std::begin(data), std::end(data), std::begin(data), d);


	std::cout << "miqs::delay(3)\n";
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	d.set_delay(5);
	std::cout << "miqs::delay::set_delay(5)\n";
	std::transform(std::begin(data), std::end(data), std::begin(data), d);
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	data[0] = 1.0;
	std::cout << "::Current::\n";
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	miqs::fixed_delay<2> fixdelay;
	std::cout << "miqs::fixed_delay<2>\n";
	std::transform(std::begin(data), std::end(data), std::begin(data), fixdelay);
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	miqs::one_sample_delay onedelay;
	std::cout << "miqs::one_sample_delay\n";
	std::transform(std::begin(data), std::end(data), std::begin(data), onedelay);
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	data[0] = 1.0;
	std::cout << "::Current::\n";
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	double delay = 4.4;
	using MyFractionDelay = miqs::fraction_delay<miqs::interpolation::linear>;
	std::vector<sample_t> buff(MyFractionDelay::get_minimum_buffer_size(delay), 0.0);
	MyFractionDelay fdelay{ delay,buff };
	std::cout << "miqs::fraction_delay<miqs::interpolation::linear>\n";
	std::transform(std::begin(data), std::end(data), std::begin(data), fdelay);
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";

}
