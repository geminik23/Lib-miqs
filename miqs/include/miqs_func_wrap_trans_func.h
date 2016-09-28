#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	template <typename _Func, typename _ArgTy, typename _ResultTy>
	struct _func_wrapper_trans1: private _Func
	{
		typedef _Func base_type;
		typedef _ArgTy argument_type;
		typedef _ResultTy result_type;

		_func_wrapper_trans1(base_type&& func):base_type(std::forward<base_type>(func)) {}

		result_type operator()(argument_type const& v)
		{
			return base_type::operator()(v);
		}
	};

	template <typename _ArgTy, typename _ResultTy = _ArgTy, typename _Func>
	_func_wrapper_trans1<_Func, _ArgTy, _ResultTy> wrap_function(_Func&& func)
	{
		return _func_wrapper_trans1<_Func, _ArgTy, _ResultTy>(std::forward<_Func>(func));
	}
}