#pragma once
#include "miqs_basictype.h"
#include "miqs_basic_algorithm.h"
#include "miqs_math.h"

namespace miqs
{
	//=====================================================
	template <typename _InIt, typename _OutIt>
	void normalize_with_max(_InIt firstIn, _InIt lastIn, _OutIt dst)
	{
		auto value = std::abs(*std::max_element(firstIn, lastIn));
		std::transform(firstIn, lastIn, dst,
					   miqs::divides_const<decltype(value)>(value));
	}

	template <typename _InIt>
	void normalize_with_max(_InIt firstIn, _InIt lastIn)
	{
		normalize_with_max(firstIn, lastIn, firstIn);
	}

	template <typename _InIt, typename _OutIt, typename _Ty>
	void normalize_after_max(_InIt firstIn, _InIt lastIn, _OutIt dst, _Ty const& value)
	{
		auto maxvalue = std::abs(*std::max_element(firstIn, lastIn));
		std::transform(firstIn, lastIn, dst,
					   miqs::make_cascades_filter(miqs::divides_const<decltype(maxvalue)>(maxvalue), miqs::multiplies_const<sample_t>(value)));
	}

	template <typename _InIt, typename _Ty>
	void normalize_after_max(_InIt firstIn, _InIt lastIn, _Ty const& value)
	{
		normalize_after_max(firstIn, lastIn, firstIn, value);
	}


	template <typename _InIt, typename _OutIt, typename _Ty>
	void normalize(_InIt firstIn, _InIt lastIn, _OutIt dst, _Ty const& value)
	{
		std::transform(firstIn, lastIn, dst,
					   miqs::divides_const<_Ty>(value));
	}

	template <typename _InIt, typename _Ty>
	void normalize(_InIt firstIn, _InIt lastIn, _Ty const& value)
	{
		normalize(firstIn, lastIn, firstIn, value);
	}


	//=====================================================================
	template <typename _InIt, typename _OutIt>
	void ptr_normalize_with_max(_InIt firstIn, _InIt lastIn, _OutIt dst)
	{
		auto value = std::abs(*std::max_element(firstIn, lastIn));
		ptr_transform(firstIn, lastIn, dst,
					   miqs::divides_const<decltype(value)>(value));
	}

	template <typename _InIt>
	void ptr_normalize_with_max(_InIt firstIn, _InIt lastIn)
	{
		ptr_normalize_with_max(firstIn, lastIn, firstIn);
	}

	template <typename _InIt, typename _OutIt, typename _Ty>
	void ptr_normalize_after_max(_InIt firstIn, _InIt lastIn, _OutIt dst, _Ty const& value)
	{
		auto maxvalue = std::abs(*std::max_element(firstIn, lastIn));
		ptr_transform(firstIn, lastIn, dst,
					   miqs::make_cascades_filter(miqs::divides_const<decltype(maxvalue)>(maxvalue), miqs::multiplies_const<sample_t>(value)));
	}

	template <typename _InIt, typename _Ty>
	void ptr_normalize_after_max(_InIt firstIn, _InIt lastIn, _Ty const& value)
	{
		ptr_normalize_after_max(firstIn, lastIn, firstIn, value);
	}


	template <typename _InIt, typename _OutIt, typename _Ty>
	void ptr_normalize(_InIt firstIn, _InIt lastIn, _OutIt dst, _Ty const& value)
	{
		ptr_transform(firstIn, lastIn, dst,
					   miqs::divides_const<_Ty>(value));
	}

	template <typename _InIt, typename _Ty>
	void ptr_normalize(_InIt firstIn, _InIt lastIn, _Ty const& value)
	{
		ptr_normalize(firstIn, lastIn, firstIn, value);
	}

}