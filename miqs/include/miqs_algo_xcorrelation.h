#pragma once
#include "miqs_basictype.h"
#include "miqs_basic_algorithm.h"

namespace miqs
{

	/* Inner product */
	template <typename _Ptr1, typename _Ptr2, typename __ResultPtr>
	__ResultPtr xcorrelation(_Ptr1 lbegin, _Ptr1 lend, _Ptr2 rbegin, __ResultPtr result)
	{
		for (; lbegin != lend; ++lbegin, ++result)
		{
			*result = miqs::ptr_inner_product(lbegin, lend, rbegin, 0.0);
		}
		return result;
	}


	template <typename _Ptr1, typename _Ptr2, typename __ResultPtr>
	__ResultPtr xcorrelation(_Ptr1 lbegin, _Ptr1 lend, _Ptr2 rbegin, size_t max_lag, __ResultPtr result)
	{
		for (size_t i{}; (lbegin != lend) && (i <= max_lag); ++lbegin, ++result, ++i)
		{
			*result = miqs::ptr_inner_product(lbegin, lend, rbegin, 0.0);
		}
		return result;
	}
}