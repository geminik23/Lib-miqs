#pragma once
#include "miqs_algo_basics.h"

namespace miqs
{

	template <typename _Ptr>
	inline _Ptr ptr_centershift(_Ptr first, _Ptr last) 
	{
		return ptr_rotate(first, first + (static_cast<int>(std::distance(first, last) / 2.0 + 0.5)), last);
	}

	template <typename _Ptr>
	inline _Ptr ptr_icentershift(_Ptr first, _Ptr last)
	{
		return ptr_rotate(first, last - (static_cast<int>(std::distance(first, last) / 2.0 + 0.5)), last);
	}

}