#pragma once

#include "miqs_basictype.h"

namespace miqs
{

	template<typename _ResultTy, typename _CoeffIt, typename... _Rest>
	struct _operator_linear_combination;

	template<typename _ResultTy,
		typename _CoeffIt>
		struct _operator_linear_combination<_ResultTy, _CoeffIt>
	{
		typedef _ResultTy result_type;

		_operator_linear_combination(_CoeffIt) {}

		result_type operator()()
		{
			return  static_cast<result_type>(0);
		}
	};


	template<
		typename _ResultTy,
		typename _CoeffIt,
		typename First,
		typename ..._Rest>
		struct _operator_linear_combination<_ResultTy, _CoeffIt, First, _Rest...>: private _operator_linear_combination<_ResultTy, _CoeffIt, _Rest...>
	{
	private:
		typedef _operator_linear_combination<_ResultTy, _CoeffIt, _Rest...> base_type;
		typedef _operator_linear_combination<_ResultTy, _CoeffIt, First, _Rest...> self_type;

	public:
		typedef typename _ResultTy result_type;
		typedef typename std::iterator_traits<_CoeffIt>::value_type coefficient_type;

		_operator_linear_combination(_CoeffIt coeff, First first, _Rest ... rest):
			base_type(++coeff, std::forward<_Rest>(rest)...),
			coeffi(*(--coeff)), _iterator(first)
		{}

		result_type operator()()
		{
			return  coeffi*(*_iterator++) + base_type::operator()();
		}

		First _iterator;
		coefficient_type coeffi;
	};



	template <typename _InIt, typename _OutIt, typename _CoeffIt, typename ... _Rest>
	void linear_combination(_InIt first, _InIt last, _OutIt dst, _CoeffIt coeff, _Rest... args)
	{
		_operator_linear_combination<
			typename std::iterator_traits<_OutIt>::value_type,
			_CoeffIt, _Rest...> op{ coeff, std::forward<_Rest>(args)... };
		for (; first != last; ++first, ++dst) *dst = *first + op();
	}

	template <typename _InIt, typename _OutIt, typename _CoeffIt, typename _Func, typename ... _Rest>
	void linear_combination_with_func(_InIt first, _InIt last, _OutIt dst, _CoeffIt coeff, _Func func, _Rest... args)
	{
		_operator_linear_combination<
			typename std::iterator_traits<_OutIt>::value_type,
			_CoeffIt, _Rest...> op{ coeff, std::forward<_Rest>(args)... };
		for (; first != last; ++first, ++dst) *dst = func(*first + op());
	}

}