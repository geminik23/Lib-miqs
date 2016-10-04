#pragma once
#include "miqs_basictype.h"


namespace miqs
{
	template <func_param_pos POS, typename _CompPart, typename _MainFunc, typename _AdjustFunc>
	struct _combine_complex_func_a1:
		public _type_combine_func_a1<typename _MainFunc::result_type, typename _MainFunc::argument_type >
	{
	private:
		typedef _MainFunc main_type;
		typedef _AdjustFunc adjust_type;
	public:
		_combine_complex_func_a1(main_type&& main, adjust_type&& adjust) {}
		//:main_type(std::forward<main_type>(main)), _adjust_func(std::forward<adjust_type>(adjust)) {}
	};

	template <typename _CompPart, typename _MainFunc, typename _AdjustFunc>
	struct _combine_complex_func_a1<ARG, _CompPart, _MainFunc, _AdjustFunc>:
		public _type_combine_func_a1<typename _MainFunc::result_type, typename _MainFunc::argument_type >
	{
	private:
		typedef _MainFunc main_type;
		typedef _AdjustFunc adjust_type;

		main_type _main_func;
		adjust_type _adjust_func;
	public:
		_combine_complex_func_a1(main_type&& main, adjust_type&& adjust):
			_main_func(std::forward<main_type>(main)), _adjust_func(std::forward<adjust_type>(adjust)) {}

		result_type operator()(argument_type const& arg)
		{
			auto t = arg;
			_CompPart::set(t,
						   static_cast<typename argument_type::value_type>(_adjust_func(_CompPart::get(t))));
			return _main_func(t);
		}
	};

	template <typename _CompPart, typename _MainFunc, typename _AdjustFunc>
	struct _combine_complex_func_a1<RESULT, _CompPart, _MainFunc, _AdjustFunc>:
		public _type_combine_func_a1<typename _MainFunc::result_type, typename _MainFunc::argument_type >
	{
	private:
		typedef _MainFunc main_type;
		typedef _AdjustFunc adjust_type;

		main_type _main_func;
		adjust_type _adjust_func;
	public:
		_combine_complex_func_a1(main_type&& main, adjust_type&& adjust):
			_main_func(std::forward<main_type>(main)), _adjust_func(std::forward<adjust_type>(adjust)) {}

		result_type operator()(argument_type const& arg)
		{
			auto t = _main_func(arg);
			_CompPart::set(t,
						   static_cast<typename argument_type::value_type>(_adjust_func(_CompPart::get(t))));
			return t;
		}
	};


	template <func_param_pos POS, typename _CompPart, typename _MainFunc, typename _AdjustFunc>
	auto  bind_complex_func_1arg(_MainFunc&& main, _AdjustFunc&& adjust)
	{
		return _combine_complex_func_a1<POS, _CompPart, _MainFunc, _AdjustFunc>(std::forward<_MainFunc>(main), std::forward<_AdjustFunc>(adjust));
	}

}