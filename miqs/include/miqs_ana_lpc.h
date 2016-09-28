#pragma once
#include "miqs_basictype.h"
#include "miqs_func_xcorrelation.h"
#include "miqs_func_levinson_durbin_method.h"
#include "miqs_func_schur_recursion.h"

namespace miqs
{

	/* bandwidth expansion function */
	//	move the zeros of A(z) toward the origin.
	//	gamma : 0.994
	template <typename _Type>
	inline void lpc_bandwidth_expansion(_Type* a, _Type* aw, double gamma, size_t order)
	{
		double gk = gamma;
		for (size_t i{ 1 };
			 i <= order;
			 ++i, gk *= gamma)
		{
			aw[i] = static_cast<_Type>(a[i] * gk);
		}
	}




	/* LPC calculator */
	/*	lp, refl, acc size : order + 1*/
	/*	lp[0] = 1.0, [1] = a1, [2] = a2 ....[p] = ap	*/

	template <typename _Iter, typename _Cont1, typename _Cont2>
	double lpc_levinson_durbin_method(_Iter biter, _Iter eiter, _Cont1& lp_coeff, _Cont2& ref_coeff, size_t lpc_order)
	{
		std::vector<double> ac;
		ac.reserve(lpc_order + 1);
		miqs::xcorrelation(biter, eiter, biter, lpc_order, std::back_inserter(ac));

		miqs::levinson_durbin_method(ac.data(), lp_coeff.data(), ref_coeff.data(), lpc_order);
		lpc_bandwidth_expansion(lp_coeff.data(), lp_coeff.data(), 0.994, lpc_order);

		return std::sqrt(std::inner_product(std::begin(ac), std::end(ac),
											std::begin(lp_coeff), 0.0));
	}

	template <typename _Iter, typename _Cont1, typename _Cont2>
	double lpc_schur_recursion(_Iter biter, _Iter eiter, _Cont1& lp_coeff, _Cont2& ref_coeff, size_t lpc_order)
	{
		std::vector<double> ac;
		ac.reserve(lpc_order + 1);
		miqs::xcorrelation(biter, eiter, biter, lpc_order, std::back_inserter(ac));

		lp_coeff[0] = 1.0;
		schur_recursion(ac.data(), lp_coeff.data(), ref_coeff.data(), lpc_order);
		lpc_bandwidth_expansion(lp_coeff.data(), lp_coeff.data(), 0.994, lpc_order);

		return std::sqrt(std::inner_product(std::begin(ac), std::end(ac),
											std::begin(lp_coeff), 0.0));
	}

}