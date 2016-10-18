#pragma once
#include "miqs_basictype.h"
#include "miqs_algo_xcorrelation.h"
#include "miqs_algo_levinson_durbin_method.h"
#include "miqs_algo_schur_recursion.h"

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

	template <typename _Ptr, typename _Cont1, typename _Cont2>
	double lpc_levinson_durbin_method(_Ptr biter, _Ptr eiter, _Cont1& lp_coeff, _Cont2& ref_coeff, size_t lpc_order)
	{
		miqs::array<std::iterator_traits<_Ptr>::value_type> ac(lpc_order + 1);
		auto * ac_last = miqs::xcorrelation(biter, eiter- lpc_order, biter, lpc_order, std::begin(ac));

		miqs::levinson_durbin_method<std::iterator_traits<_Ptr>::value_type>(ac.data(), lp_coeff.data(), ref_coeff.data(), lpc_order);
		lpc_bandwidth_expansion(lp_coeff.data(), lp_coeff.data(), 0.994, lpc_order);
		return std::sqrt(miqs::ptr_inner_product(std::begin(ac), ac_last,
											std::begin(lp_coeff), 0.0));
	}

	template <typename _Ptr, typename _Cont1, typename _Cont2>
	double lpc_schur_recursion(_Ptr biter, _Ptr eiter, _Cont1& lp_coeff, _Cont2& ref_coeff, size_t lpc_order)
	{

		miqs::array<std::iterator_traits<_Ptr>::value_type> ac(lpc_order + 1);
		auto * ac_last = miqs::xcorrelation(biter, eiter - lpc_order, biter, lpc_order, std::begin(ac));


		lp_coeff[0] = 1.0;
		schur_recursion<std::iterator_traits<_Ptr>::value_type>(ac.data(), lp_coeff.data(), ref_coeff.data(), lpc_order);
		lpc_bandwidth_expansion(lp_coeff.data(), lp_coeff.data(), 0.994, lpc_order);

		return std::sqrt(miqs::ptr_inner_product(std::begin(ac), ac_last
											std::begin(lp_coeff), 0.0));
	}

}