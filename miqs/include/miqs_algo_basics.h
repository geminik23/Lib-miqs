#pragma once
#include "miqs_basictype.h"

namespace miqs
{

	template <typename _Ptr>
	inline _Ptr ptr_max(_Ptr first, _Ptr last)
	{
		_Ptr r = first;
		for (; first != last; ++first)
		{
			if (*r < *first)
				r = first;
		}
		return r;
	}

	template <typename _Ptr>
	inline _Ptr ptr_max(_Ptr first, _Ptr last, int i)
	{
		_Ptr r = first;
		for (; first < last; first += i)
		{
			if (*r < *first)
				r = first;
		}
		return r;
	}

	template <typename _Ptr>
	inline _Ptr ptr_min(_Ptr first, _Ptr last)
	{
		_Ptr r = first;
		for (; first != last; ++first)
		{
			if (*r > *first)
				r = first;
		}
		return r;

	}

	template <typename _Ptr>
	inline _Ptr ptr_min(_Ptr first, _Ptr last, int i)
	{
		_Ptr r = first;
		for (; first < last; first += i)
		{
			if (*r > *first)
				r = first;
		}
		return r;

	}

	template <typename _Ptr>
	inline void ptr_swap(_Ptr l, _Ptr r)
	{
		std::iterator_traits<_Ptr>::value_type t = *l;
		*l = *r;
		*r = t;
	}

	//=========================================

	template <typename _OutPtr, typename _Func>
	inline void ptr_generate(_OutPtr first, _OutPtr last, _Func func)
	{
		for (; first != last; ++first)
		{
			*first = func();
		}
	}

	template <typename _OutPtr, typename _Func>
	inline void ptr_generate(_OutPtr first, _OutPtr last, int ii, _Func func)
	{
		for (; first != last; first+=ii)
		{
			*first = func();
		}
	}

	template <typename _Ptr>
	inline _Ptr ptr_rotate(_Ptr first, _Ptr mid, _Ptr last)
	{
		if (first == mid)return last;
		if (mid == last) return first;
		for (_Ptr next = mid, res = last; ; )
		{	// swap [_First, ...) into place
			ptr_swap(first, next);
			if (++first == mid)
			{	// quit if done, else define next interval
				if (++next == last)
					return (res == last ? mid : res);
				else
					mid = next;	// mark end of next interval
			}
			else if (++next == last)
			{	// wrap to last end
				if (res == last)
					res = first;
				next = mid;
			}
		}
	}

	template <typename _Ptr, typename _Func>
	inline void ptr_for_each(_Ptr first, _Ptr last, _Func func)
	{
		for (; first != last;)
			func(*first++);
	}

	template <typename _Ptr, typename _Func>
	inline void ptr_for_each(_Ptr first, _Ptr last, int ii, _Func func)
	{
		for (; first != last;first+=ii)
			func(*first);
	}

	template <typename _InPtr, typename _OutPtr>
	inline void ptr_copy(_InPtr first, _InPtr last, _OutPtr dst)
	{
		for (; first != last;)
		{
			*dst++ = *first++;
		}
	}

	template <typename _InPtr, typename _OutPtr>
	inline void ptr_copy(_InPtr first, _InPtr last, size_t ii, _OutPtr dst, size_t oi)
	{
		for (; first < last; first += ii, dst += oi)
		{
			*dst = *first;
		}
	}

	template <typename _InPtr, typename _OutPtr>
	inline void ptr_copy(_InPtr first, _InPtr last, size_t ii, _OutPtr dst)
	{
		for (; first < last; first += ii, ++dst)
		{
			*dst = *first;
		}
	}




	template <typename _InPtr, typename _OutPtr, typename _Ty>
	inline void ptr_multiplies(_InPtr first, _InPtr last, _OutPtr dst, _Ty _Val)
	{
		for (; first != last; ++first, ++dst)*dst = _Val * *first;
	}
	template <typename _InPtr, typename _OutPtr, typename _Ty>
	inline void ptr_divides(_InPtr first, _InPtr last, _OutPtr dst, _Ty _Val)
	{
		for (; first != last; ++first, ++dst)*dst =  *first / _Val;
	}
	template <typename _InPtr, typename _OutPtr, typename _Ty>
	inline void ptr_plus(_InPtr first, _InPtr last, _OutPtr dst, _Ty _Val)
	{
		for (; first != last; ++first, ++dst)*dst = _Val + *first;
	}
	template <typename _InPtr, typename _OutPtr, typename _Ty>
	inline void ptr_minus(_InPtr first, _InPtr last, _OutPtr dst, _Ty _Val)
	{
		for (; first != last; ++first, ++dst)*dst = _Val - *first;
	}

}