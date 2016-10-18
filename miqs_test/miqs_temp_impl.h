#pragma once

#include <miqs>

namespace miqs
{



	namespace process
	{


	}



	template <typename _Ty>
	struct random_range: _base_value_type<_Ty>
	{
		random_range():re{ std::random_device()() } {}

		value_type operator()(value_type l, value_type u)
		{
			return static_cast<value_type>(l + (dist(re))*(u - l));
		}

		std::default_random_engine re;
		std::uniform_real_distribution<double> dist;
	};

	struct grain
	{
		double freq;
		size_t pos;
		size_t dur;
	};

	//===============


	template <typename _InIt, typename _OutIt, typename _ContIdx>
	void copy_index(_InIt first, _OutIt dst, _ContIdx const& cont)
	{
		for (auto i : cont) *dst++ = *(first + i);
	}

	inline double phase_wrapping(double in)
	{
		return std::remainder(in + miqs::pi, -2 * miqs::pi) + pi;
	}






}
