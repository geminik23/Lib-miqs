#pragma once

#include "miqs_basictype.h"
#include "miqs_func_combines.h"

namespace miqs
{

	// rename 
	template <typename First, typename Second>
	using combine_filter = combine_unary_transform_operator<First, Second>;




	/*  mkae_combine_filter */
	template <typename First, typename Second>
	combine_filter<First, Second> make_combine_filter(First first, Second second)
	{
		return combine_filter<First, Second>(first, second);
	}

	/* make_cascades_filter */
	template <typename ... Filters>
	auto make_cascades_filter(Filters... filters);





	//
	// Implementation
	// 
	template <typename First>
	auto __filter_cascades(First filter)
	{
		return filter;
	}

	template <typename First, typename... Rest>
	auto __filter_cascades(First first, Rest... rest)
	{
		return make_combine_filter(first, __filter_cascades(rest...));
	}


	template <typename ... Filters>
	auto make_cascades_filter(Filters... filters)
	{
		return __filter_cascades(filters...);
	}

}