#pragma once
#include "miqs_algo_basics.h"

namespace miqs
{


	//==============================

	template<class _PtrIt1, class _PtrIt2, class _Ty, class _Fn21, class _Fn22>
	inline _Ty __inner_product(_PtrIt1 first1, _PtrIt1 last1,
							   _PtrIt2 first2, _Ty val,
							   _Fn21& func1, _Fn22& func2)
	{
		for (; first1 != last1; ++first1, ++first2)
			val = func1(val, func2(*first1, *first2));
		return (val);
	}

	template<class _PtrIt1, class _PtrIt2, class _Ty, class _Fn21, class _Fn22>
	inline _Ty ptr_inner_product(_PtrIt1 first1, _PtrIt1 last1,
								 _PtrIt2 first2, _Ty val,
								 _Fn21 func1, _Fn22 func2)
	{
		return __inner_product(first1, last1, first2, val, func1, func2);
	}

	template<class _PtrIt1, class _PtrIt2, class _Ty>
	inline _Ty ptr_inner_product(_PtrIt1 first1, _PtrIt1 last1,
								 _PtrIt2 first2, _Ty val)
	{
		for (; first1 != last1; ++first1, ++first2)
			val += *first1 *  *first2;
		return (val);
	}



}