#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	/**
	combine two unary transform operator
	*/
	template <typename FirstOp, typename SecondOp>
	struct combine_unary_transform_operator
	{
		combine_unary_transform_operator(FirstOp first, SecondOp second): firstOperator{ first }, secondOperator{ second } {}

		sample_t operator()(sample_t in) noexcept
		{
			return secondOperator(firstOperator(in));
		}

		FirstOp firstOperator;
		SecondOp secondOperator;
	};


	template <typename FirstOp, typename SecondOp>
	combine_unary_transform_operator<FirstOp, SecondOp> make_combine_unary_transform_operator(FirstOp first, SecondOp second)
	{
		return combine_unary_transform_operator<FirstOp, SecondOp>(first, second);
	}
}