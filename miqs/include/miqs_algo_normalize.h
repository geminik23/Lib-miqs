#pragma once

#include "miqs_basictype.h"

namespace miqs
{
	
	template <typename _InIt, typename _OutIt, typename _Ty>
	inline void ptr_normalize(_InIt first, _InIt last, _OutIt dst, _Ty const& value)
	{
		for (; first != last; ++first,++dst)
		{
			*dst = *first / value;
		}
	}

	template <typename _InIt, typename _OutIt, typename _Ty>
	inline void ptr_normalize(_InIt first, _InIt last, int ii, _OutIt dst, int oi, _Ty const& value)
	{
		for (; first < last; first+=ii, dst+=oi)
		{
			*dst = *first / value;
		}
	}

	template <typename _InIt, typename _Ty>
	inline void ptr_normalize(_InIt firstIn, _InIt lastIn, _Ty const& value)
	{
		ptr_normalize(firstIn, lastIn, firstIn, value);
	}

	template <typename _InIt, typename _Ty>
	inline void ptr_normalize(_InIt firstIn, _InIt lastIn, int ii, _Ty const& value)
	{
		ptr_normalize(firstIn, lastIn, ii, firstIn, ii, value);
	}


}