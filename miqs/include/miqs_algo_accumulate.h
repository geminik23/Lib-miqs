#pragma once
#include "miqs_algo_basics.h"

namespace miqs
{
	
	template <typename _Ptr, typename _Ty>
	_Ty ptr_accumulate(_Ptr first, _Ptr last, _Ty v)
	{
		for (; first < last; ++first)
		{
			v += *first;
		}
		return v;
	}

	template <typename _Ptr, typename _Ty, typename _Func>
	_Ty ptr_accumulate(_Ptr first, _Ptr last, _Ty v, _Func func)
	{
		for (; first < last; ++first)
		{
			v = func(v, *first);
		}
		return v;
	}

	template <typename _Ptr, typename _Ty>
	_Ty ptr_accumulate_minus(_Ptr first, _Ptr last, _Ty v)
	{
		for (; first < last; ++first)
		{
			v -= *first;
		}
		return v;
	}

	template <typename _Ptr, typename _Ty>
	_Ty ptr_accumulate_multiplies(_Ptr first, _Ptr last, _Ty v)
	{
		for (; first < last; ++first)
		{
			v *= *first;
		}
		return v;
	}

	template <typename _Ptr, typename _Ty>
	_Ty ptr_accumulate_divides(_Ptr first, _Ptr last, _Ty v)
	{
		for (; first < last; ++first)
		{
			v /= *first;
		}
		return v;
	}


}