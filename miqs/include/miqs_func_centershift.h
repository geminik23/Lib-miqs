#pragma once
#include "miqs_basictype.h"

namespace miqs
{

	template <typename _Cont>
	inline auto centershift(_Cont& container) -> typename _Cont::iterator
	{
		return std::rotate(std::begin(container),
						   std::begin(container) + (static_cast<size_t>(container.size() / 2.0 + 0.5)),
						   std::end(container));
	}

	template <typename _Cont>
	inline auto icentershift(_Cont& container) -> typename _Cont::iterator
	{
		return std::rotate(std::begin(container),
						   std::end(container) - (static_cast<size_t>(container.size() / 2.0 + 0.5)),
						   std::end(container));
	}

}