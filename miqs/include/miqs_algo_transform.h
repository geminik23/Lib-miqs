#pragma once
#include "miqs_algo_basics.h"

namespace miqs
{

	//=====================================================================

	template <typename _InPtr, typename _OutPtr, typename _Func>
	inline void ptr_transform(_InPtr first, _InPtr last, _OutPtr dst, _Func func)
	{
		for (; first != last; ++first)
		{
			*dst++ = func(*first);
		}
	}


	template <typename _InPtr1, typename _InPtr2, typename _OutPtr, typename _Func>
	inline void ptr_transform(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst, _Func func)
	{
		for (; lf != ll; ++lf, ++rf)
		{
			*dst++ = func(*lf, *rf);
		}
	}

	template <typename _InPtr, typename _OutPtr, typename _Func>
	inline void ptr_transform(_InPtr first, _InPtr last, size_t ii, _OutPtr dst, size_t oi, _Func func)
	{
		for (; first < last; first += ii, dst += oi)
		{
			*dst = func(*first);
		}
	}


	template <typename _InPtr1, typename _InPtr2, typename _OutPtr, typename _Func>
	inline void ptr_transform(_InPtr1 lf, _InPtr1 ll, size_t i1i, _InPtr2 rf, size_t i2i, _OutPtr dst, size_t oi, _Func func)
	{
		for (; lf < ll; lf += i1i, rf += i2i, dst += oi)
		{
			*dst = func(*lf, *rf);
		}
	}


//=========================================== BASICS

	template <typename _InPtr1, typename _InPtr2, typename _OutPtr>
	inline void ptr_transform_plus(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst)
	{
		for (; lf != ll; ++lf, ++rf)
		{
			*dst++ = *lf+ *rf;
		}
	}
	template <typename _InPtr1, typename _InPtr2, typename _OutPtr>
	inline void ptr_transform_minus(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst)
	{
		for (; lf != ll; ++lf, ++rf)
		{
			*dst++ = *lf- *rf;
		}
	}
	template <typename _InPtr1, typename _InPtr2, typename _OutPtr>
	inline void ptr_transform_multiplies(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst)
	{
		for (; lf != ll; ++lf, ++rf)
		{
			*dst++ = *lf* *rf;
		}
	}
	template <typename _InPtr1, typename _InPtr2, typename _OutPtr>
	inline void ptr_transform_divides(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst)
	{
		for (; lf != ll; ++lf, ++rf)
		{
			*dst++ = *lf/ *rf;
		}
	}
}