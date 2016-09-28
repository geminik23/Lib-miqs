#pragma once

#include "miqs_basictype.h"

namespace miqs
{

	template<typename _Ty, typename... _Rest>
	struct _Cascade_func;

	template <typename _Ty>
	struct _Cascade_func<_Ty>
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		result_type operator() (argument_type const& v) noexcept
		{
			return v;
		}
	};



	template <typename _Ty, typename _First, typename... _Rest>
	struct _Cascade_func<_Ty, _First, _Rest...>: private _Cascade_func<_Ty, _Rest...>
	{
		typedef _Cascade_func<_Ty, _Rest...> _Base_type;
		typedef _Ty argument_type;
		typedef _Ty result_type;

		_Cascade_func(_First&& func1, _Rest ... rest):_Base_type(std::forward<_Rest>(rest)...), _function{ func1 } {}

		result_type operator() (argument_type const& v) noexcept
		{
			return _Base_type::operator()(_function(v));
		}

		_First _function;
	};





	template <typename _Ty, typename ...Args>
	auto make_cascades_functions(Args... args)
	{
		return _Cascade_func<_Ty, Args...>(std::forward<Args>(args)...);
	}
}