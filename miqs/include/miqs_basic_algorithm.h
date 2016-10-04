#pragma once
#include "miqs_basictype.h"

namespace miqs
{

	template <typename _Ptr>
	void ptr_swap(_Ptr l, _Ptr r)
	{
		_Ptr t = l;
		l = r;
		r = t;
	}

	template <typename _InPtr, typename _OutPtr, typename _Func>
	void ptr_transform(_InPtr first, _InPtr last, _OutPtr dst, _Func func)
	{
		for (; first != last; ++first)
		{
			*dst++ = func(*first);
		}
	}


	template <typename _InPtr1, typename _InPtr2, typename _OutPtr, typename _Func>
	void ptr_transform(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst, _Func func)
	{
		for (; lf != ll; ++lf, ++rf)
		{
			*dst++ = func(*lf, *rf);
		}
	}
	template <typename _OutPtr, typename _Func>
	void ptr_generate(_OutPtr first, _OutPtr last, _Func func)
	{
		for (; first != last; ++first)
		{
			*first = func();
		}
	}
	
	template <typename _Ptr>
	void ptr_rotate(_Ptr first, _Ptr mid, _Ptr last)
	{
		if (first == mid)return last;
		if (mid == last) return first;
		for (_Ptr next=mid, res = last; ; )
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
	void ptr_for_each(_Ptr first, _Ptr last, _Func func)
	{
		for (; first != last;)
			func(*first++);
	}

	template <typename _InPtr, typename _OutPtr>
	void ptr_copy(_InPtr first, _InPtr last, _OutPtr dst)
	{
		for (; first != last;)
		{
			*dst++ = *first++;
		}
	}

}