#pragma once
#include "miqs_basictype.h"


namespace miqs
{

	/* Inner product */
	template <typename _Iter1, typename _Iter2, typename _ResultIter>
	void xcorrelation(_Iter1 lbegin, _Iter1 lend, _Iter2 rbegin, _ResultIter result)
	{
		for (; lbegin != lend; ++lbegin, ++result)
		{
			*result = std::inner_product(lbegin, lend, rbegin, 0.0);
		}
	}


	template <typename _Iter1, typename _Iter2, typename _ResultIter>
	void xcorrelation(_Iter1 lbegin, _Iter1 lend, _Iter2 rbegin, size_t max_lag, _ResultIter result)
	{
		for (size_t i{}; (lbegin != lend) && (i <= max_lag); ++lbegin, ++result, ++i)
		{
			*result = std::inner_product(lbegin, lend, rbegin, 0.0);
		}
	}
}