#pragma once
#include "miqs_basictype.h"


namespace miqs
{

	

	template <int POS, typename _MainFunc, typename _AdjustFunc>
	struct _combine_func_a2:public _type_combine_func_a2<typename _MainFunc::result_type, typename _MainFunc::first_argument_type, typename _MainFunc::second_argument_type >
	{
	private:
		typedef _MainFunc main_type;
		typedef _AdjustFunc adjust_type;
	public:
		_combine_func_a2(main_type&& , adjust_type&&){}
	};

	//
	template <typename _MainFunc, typename _AdjustFunc>
	struct _combine_func_a2<LEFT, _MainFunc, _AdjustFunc>:
		_type_combine_func_a2<
		typename _MainFunc::result_type,
		typename _AdjustFunc::argument_type,
		typename _MainFunc::second_argument_type >
	{
	private:
		typedef _MainFunc main_type;
		typedef _AdjustFunc adjust_type;

		main_type _main_func;
		adjust_type _adjust_func;
	public:
		_combine_func_a2(main_type&& main, adjust_type&& adjust):
			_main_func(std::forward<main_type>(main)), _adjust_func(std::forward<adjust_type>(adjust)) {}

		result_type operator()(first_argument_type const& l, second_argument_type const & r)
		{
			return _main_func(_adjust_func(l), r);
		}
	};

	template <typename _MainFunc, typename _AdjustFunc>
	struct _combine_func_a2<RIGHT, _MainFunc, _AdjustFunc>:
		_type_combine_func_a2<
		typename _MainFunc::result_type,
		typename _MainFunc::first_argument_type,
		typename _AdjustFunc::argument_type >
	{
	private:
		typedef _MainFunc main_type;
		typedef _AdjustFunc adjust_type;

		main_type _main_func;
		adjust_type _adjust_func;
	public:
		_combine_func_a2(main_type&& main, adjust_type&& adjust):
			_main_func(std::forward<main_type>(main)), _adjust_func(std::forward<adjust_type>(adjust)) {}

		result_type operator()(first_argument_type const& l, second_argument_type const & r)
		{
			return _main_func(l, _adjust_func(r));
		}
	};

	template <typename _MainFunc, typename _AdjustFunc>
	struct _combine_func_a2<RESULT, _MainFunc, _AdjustFunc>:
		_type_combine_func_a2<
		typename _AdjustFunc::result_type,
		typename _MainFunc::first_argument_type,
		typename _MainFunc::second_argument_type >
	{
	private:
		typedef _MainFunc main_type;
		typedef _AdjustFunc adjust_type;

		main_type _main_func;
		adjust_type _adjust_func;
	public:
		_combine_func_a2(main_type&& main, adjust_type&& adjust):
			_main_func(std::forward<main_type>(main)), _adjust_func(std::forward<adjust_type>(adjust)) {}

		result_type operator()(first_argument_type const& l, second_argument_type const & r)
		{
			return _adjust_func(_main_func(l, r));
		}
	};


	template <int POS, typename _MainFunc, typename _AdjustFunc>
	struct _binder_func_two_with_one_arg:public _combine_func_a2<POS, _MainFunc, _AdjustFunc>
	{
	private:
		typedef _combine_func_a2<POS, _MainFunc, _AdjustFunc> base_type;
	public:
		_binder_func_two_with_one_arg(_MainFunc&& main, _AdjustFunc&& adjust):
			base_type(std::forward<_MainFunc>(main), std::forward<_AdjustFunc>(adjust)) {}
	};

	template <int POS, typename _MainFunc, typename _AdjustFunc>
	auto bind_func_2arg(_MainFunc&& main, _AdjustFunc&& adjust) ->_binder_func_two_with_one_arg<POS, _MainFunc, _AdjustFunc>
	{
		return _binder_func_two_with_one_arg<POS, _MainFunc, _AdjustFunc>(std::forward<_MainFunc>(main), std::forward<_AdjustFunc>(adjust));
	}


}
