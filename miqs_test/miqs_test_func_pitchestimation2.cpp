#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;



#include <iomanip>
#include <numeric>

//
//
//
//void miqs_test::funcs::pitchestimation2()
//{
//
//	double freq = 440.0;
//	miqs::uint32_t sr = 48000;
//	size_t size = 512;
//
//	size_t min = 0, max = 300;
//
//	std::cout << std::setprecision(2) << std::fixed;
//
//
//	std::vector<sample_t> s1(size);
//	
//	miqs::phasor phase{ freq, sr };
//	miqs::generator<miqs::sine_wave, miqs::phasor> gen{ phase };
//
//	std::generate(std::begin(s1), std::end(s1), gen);
//	
//
//	// 1. get autocorrelation result
//	std::vector<double> result(max - min);
//	for (size_t i{min}; i < max; i++)
//	{
//		result[i] = std::inner_product(std::begin(s1) + i, std::end(s1), std::begin(s1), 0.0);
//	}
//}
