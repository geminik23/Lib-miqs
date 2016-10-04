#pragma once
#include <miqs.h>

#include <iomanip>
#include "miqs_temp_impl.h"

using namespace miqs;
namespace miqs_test
{

	void newfea_bind_diff_type_func()
	{

		// to bind this two function(one is complex<T> and one is T then)
		// there has two possibilities 

		using SampleComplex = std::complex<sample_t>;
		miqs::cartesian_to_polar<> cp;
		miqs::log<> l;

		SampleComplex dst(1.0, 0.8);
		SampleComplex temp = dst;


		auto acp = miqs::bind_complex_func_1arg<ARG, miqs::real_part>(miqs::cartesian_to_polar<>(), miqs::log<>());
		auto rcp = miqs::bind_complex_func_1arg<RESULT, miqs::real_part>(miqs::cartesian_to_polar<>(), miqs::log<>());


		temp = cp(temp);
		temp.real(l(temp.real()));
		std::cout << temp << std::endl;

		temp = dst;
		temp = rcp(temp);
		std::cout << temp << std::endl;



		temp = dst;
		temp.real(l(temp.real()));
		temp = cp(temp);
		std::cout << temp << std::endl;

		temp = dst;
		temp = acp(temp);
		std::cout << temp << std::endl;




		//miqs::bind_complex_func_1arg<RESULT, real_part>(miqs::cartesian_to_polar<>(),
		//												miqs::make_cascades_functions<sample_t>(
		//													miqs::log<>(), miqs::plus_const<>(1.2)
		//													));
	}
}