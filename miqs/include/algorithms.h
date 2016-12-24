#pragma once
#include "base.h"

namespace miqs
{

	template <typename _Ptr>
	inline _Ptr ptr_max(_Ptr first, _Ptr last)
	{
		_Ptr r = first;
		for (; first != last; ++first)
		{
			if (*r < *first)
				r = first;
		}
		return r;
	}

	template <typename _Ptr>
	inline _Ptr ptr_max(_Ptr first, _Ptr last, int i)
	{
		_Ptr r = first;
		for (; first < last; first += i)
		{
			if (*r < *first)
				r = first;
		}
		return r;
	}

	template <typename _Ptr>
	inline _Ptr ptr_min(_Ptr first, _Ptr last)
	{
		_Ptr r = first;
		for (; first != last; ++first)
		{
			if (*r > *first)
				r = first;
		}
		return r;

	}

	template <typename _Ptr>
	inline _Ptr ptr_min(_Ptr first, _Ptr last, int i)
	{
		_Ptr r = first;
		for (; first < last; first += i)
		{
			if (*r > *first)
				r = first;
		}
		return r;

	}

	template <typename _Ptr>
	inline void ptr_swap(_Ptr l, _Ptr r)
	{
		std::iterator_traits<_Ptr>::value_type t = *l;
		*l = *r;
		*r = t;
	}

	//=========================================

	template <typename _OutPtr, typename _Func>
	inline void ptr_generate(_OutPtr first, _OutPtr last, _Func func)
	{
		for (; first != last; ++first)
		{
			*first = func();
		}
	}

	template <typename _OutPtr, typename _Func>
	inline void ptr_generate(_OutPtr first, _OutPtr last, int ii, _Func func)
	{
		for (; first != last; first += ii)
		{
			*first = func();
		}
	}

	template <typename _Ptr>
	inline _Ptr ptr_rotate(_Ptr first, _Ptr mid, _Ptr last)
	{
		if (first == mid)return last;
		if (mid == last) return first;
		for (_Ptr next = mid, res = last; ; )
		{	// swap [_First, ...) into place
			ptr_swap(first, next);
			if (++first == mid)
			{	// quit if done, else define next interval
				if (++next == last)
					return (res == last ? mid : res);
				else
					mid = next;	// mark end of next interval
			}
			else if (++next == last)
			{	// wrap to last end
				if (res == last)
					res = first;
				next = mid;
			}
		}
	}

	template <typename _Ptr, typename _Func>
	inline void ptr_for_each(_Ptr first, _Ptr last, _Func func)
	{
		for (; first != last;) func(*first++);
	}

	template <typename _Ptr, typename _Func>
	inline void ptr_for_each(_Ptr first, _Ptr last, int ii, _Func func)
	{
		for (; first != last; first += ii)	func(*first);
	}
	template <typename _InPtr, typename _OutPtr>
	inline void ptr_copy(_InPtr first, _InPtr last, _OutPtr dst)
	{
		for (; first != last;)*dst++ = *first++;
	}

	template <typename _InPtr, typename _OutPtr>
	inline void ptr_copy
	(_InPtr first, _InPtr last, size_t ii, _OutPtr dst, size_t oi)
	{
		for (; first < last; first += ii, dst += oi) *dst = *first;
	}

	template <typename _InPtr, typename _OutPtr>
	inline void ptr_copy
	(_InPtr first, _InPtr last, size_t ii, _OutPtr dst)
	{
		for (; first < last; first += ii, ++dst) *dst = *first;
	}

	template <typename _InPtr, typename _OutPtr, typename _Ty>
	inline void ptr_multiplies
	(_InPtr first, _InPtr last, _OutPtr dst, _Ty _Val) { for (; first != last; ++first, ++dst)*dst = _Val * *first; }
	template <typename _InPtr, typename _OutPtr, typename _Ty>
	inline void ptr_divides
	(_InPtr first, _InPtr last, _OutPtr dst, _Ty _Val) { for (; first != last; ++first, ++dst)*dst = *first / _Val; }
	template <typename _InPtr, typename _OutPtr, typename _Ty>
	inline void ptr_plus
	(_InPtr first, _InPtr last, _OutPtr dst, _Ty _Val) { for (; first != last; ++first, ++dst)*dst = _Val + *first; }
	template <typename _InPtr, typename _OutPtr, typename _Ty>
	inline void ptr_minus
	(_InPtr first, _InPtr last, _OutPtr dst, _Ty _Val) { for (; first != last; ++first, ++dst)*dst = _Val - *first; }



	//=====================================================================
	//Transforms
	//=====================================================================

	template <typename _InPtr, typename _OutPtr, typename _Func>
	inline void ptr_transform(_InPtr first, _InPtr last, _OutPtr dst, _Func func)
	{
		for (; first != last; ++first)	*dst++ = func(*first);
	}


	template <typename _InPtr1, typename _InPtr2, typename _OutPtr, typename _Func>
	inline void ptr_transform(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst, _Func func)
	{
		for (; lf != ll; ++lf, ++rf) *dst++ = func(*lf, *rf);
	}

	template <typename _InPtr, typename _OutPtr, typename _Func>
	inline void ptr_transform(_InPtr first, _InPtr last, size_t ii, _OutPtr dst, size_t oi, _Func func)
	{
		for (; first < last; first += ii, dst += oi)
		{
			*dst = func(*first);
		}
	}


	template <typename _InPtr1, typename _InPtr2, typename _OutPtr, typename _Func>
	inline void ptr_transform(_InPtr1 lf, _InPtr1 ll, size_t i1i, _InPtr2 rf, size_t i2i, _OutPtr dst, size_t oi, _Func func)
	{
		for (; lf < ll; lf += i1i, rf += i2i, dst += oi)*dst = func(*lf, *rf);
	}


	//------------------------------------------------------

	template <typename _InPtr1, typename _InPtr2, typename _OutPtr>
	inline void ptr_transform_plus(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst)
	{
		for (; lf != ll; ++lf, ++rf)*dst++ = *lf + *rf;
	}
	template <typename _InPtr1, typename _InPtr2, typename _OutPtr>
	inline void ptr_transform_minus(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst)
	{
		for (; lf != ll; ++lf, ++rf)*dst++ = *lf - *rf;
	}
	template <typename _InPtr1, typename _InPtr2, typename _OutPtr>
	inline void ptr_transform_multiplies(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst)
	{
		for (; lf != ll; ++lf, ++rf)*dst++ = *lf* *rf;
	}
	template <typename _InPtr1, typename _InPtr2, typename _OutPtr>
	inline void ptr_transform_divides(_InPtr1 lf, _InPtr1 ll, _InPtr2 rf, _OutPtr dst)
	{
		for (; lf != ll; ++lf, ++rf) *dst++ = *lf / *rf;
	}


	//=====================================================================
	//Normalize
	//=====================================================================
	template <typename _InIt, typename _OutIt, typename _Ty>
	inline void ptr_normalize(_InIt first, _InIt last, _OutIt dst, _Ty const& value)
	{
		for (; first != last; ++first, ++dst)*dst = *first / value;
	}

	template <typename _InIt, typename _OutIt, typename _Ty>
	inline void ptr_normalize(_InIt first, _InIt last, int ii, _OutIt dst, int oi, _Ty const& value)
	{
		for (; first < last; first += ii, dst += oi)*dst = *first / value;
	}

	template <typename _InIt, typename _Ty>
	inline void ptr_normalize(_InIt firstIn, _InIt lastIn, _Ty const& value)
	{
		ptr_normalize(firstIn, lastIn, firstIn, value);
	}

	template <typename _InIt, typename _Ty>
	inline void ptr_normalize(_InIt firstIn, _InIt lastIn, int ii, _Ty const& value)
	{
		ptr_normalize(firstIn, lastIn, ii, firstIn, ii, value);
	}



	//=====================================================================
	// Fill
	//=====================================================================
	template <typename _Ty = sample_t>
	struct real_noise_data
	{
		std::default_random_engine random_engine{ std::random_device()() };
		std::uniform_real_distribution<_Ty> distribution{ -1,1 };
	};


	template <typename _PtrIt, typename _NoiseData>
	inline void ptr_fill_noise(_PtrIt first, _PtrIt last, _NoiseData data)
	{
		for (; first != last; ++first)*first = data.distribution(data.random_engine);
	}

	template <typename _PtrIt>
	inline void ptr_fill_zero(_PtrIt first, _PtrIt last)
	{
		for (; first != last; ++first)*first = static_cast<std::iterator_traits<_PtrIt>::value_type>(0);
	}


	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_phasor(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	{
		for (; first != last; ++first)
		{
			*first = phase_info.phase / miqs::two_pi;

			phase_info.next();
			MIQS_PHASE_MODULAR_DOWN(phase_info, miqs::two_pi);
			MIQS_PHASE_MODULAR_UP(phase_info, miqs::two_pi);
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_sine(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	{
		for (; first != last; ++first)
		{
			*first = std::sin(phase_info.phase);
			phase_info.next();
			MIQS_PHASE_MODULAR_DOWN(phase_info, miqs::two_pi);
			MIQS_PHASE_MODULAR_UP(phase_info, miqs::two_pi);
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_rectangular(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	{
		for (; first != last; ++first)
		{
			*first = (phase_info.phase <= miqs::pi) ? 1.0 : -1.0;
			phase_info.next();
			MIQS_PHASE_MODULAR_DOWN(phase_info, miqs::two_pi);
			MIQS_PHASE_MODULAR_UP(phase_info, miqs::two_pi);
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_saw(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	{
		for (; first != last; ++first)
		{
			*first = (2.0 *(phase_info.phase *(1.0 / miqs::two_pi))) - 1.0;
			phase_info.next();
			MIQS_PHASE_MODULAR_DOWN(phase_info, miqs::two_pi);
			MIQS_PHASE_MODULAR_UP(phase_info, miqs::two_pi);
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_triangular(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info)
	{
		for (; first != last; ++first)
		{
			*first = (2.0 *(phase_info.phase *(1.0 / miqs::two_pi))) - 1.0;
			if (*first < 0.0) *first *= -1;
			*first = 2.0*(*first - 0.5);
			phase_info.next();
			MIQS_PHASE_MODULAR_DOWN(phase_info, miqs::two_pi);
			MIQS_PHASE_MODULAR_UP(phase_info, miqs::two_pi);
		}
	}

	//---------- WINDOW

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_window_hanning(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info, size_t wsize)
	{
		for (; first != last; ++first)
		{
			if (phase_info.phase >= (decltype(phase_info.phase))wsize) *first = 0.0;
			else *first = (0.5) - (0.5 * std::cos(miqs::pi * 2 * phase_info.phase / (double)(wsize - 1.0)));
			phase_info.next();
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_window_hamming(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info, size_t wsize)
	{
		for (; first != last; ++first)
		{
			if (phase_info.phase >= (decltype(phase_info.phase))wsize) *first = 0.0;
			else *first = (0.54) - (0.46 * std::cos(miqs::pi * 2 * phase_info.phase / (double)(wsize - 1.0)));
			phase_info.next();
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_window_blackman(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info, size_t wsize)
	{
		double arg = 2 * miqs::pi / (wsize - 1);
		double x;
		for (; first != last; ++first)
		{
			if (phase_info.phase >= (decltype(phase_info.phase))wsize) *first = 0.0;
			else
			{
				x = arg * phase_info.phase;
				*first = 0.42659 - 0.49656 * std::cos(x) + 0.076849 * std::cos(x + x);
			}
			phase_info.next();
		}
	}

	template <typename _PtrIt, typename _PhaseInfo>
	inline void ptr_fill_window_triangular(_PtrIt first, _PtrIt last, _PhaseInfo& phase_info, size_t wsize)
	{
		for (; first != last; ++first)
		{
			if (phase_info.phase >= (decltype(phase_info.phase))wsize) *first = 0.0;
			else *first = 1.0 - (std::abs((int)phase_info.phase * 2 - (int)wsize + 1) / (double)(wsize - 1.0));
			phase_info.next();
		}
	}



	//=====================================================================
	// Inner Product
	//=====================================================================
	template<class _PtrIt1, class _PtrIt2, class _Ty, class _Fn21, class _Fn22>
	inline _Ty __inner_product(_PtrIt1 first1, _PtrIt1 last1,
							   _PtrIt2 first2, _Ty val,
							   _Fn21& func1, _Fn22& func2)
	{
		for (; first1 != last1; ++first1, ++first2)
			val = func1(val, func2(*first1, *first2));
		return (val);
	}

	template<class _PtrIt1, class _PtrIt2, class _Ty, class _Fn21, class _Fn22>
	inline _Ty ptr_inner_product(_PtrIt1 first1, _PtrIt1 last1,
								 _PtrIt2 first2, _Ty val,
								 _Fn21 func1, _Fn22 func2)
	{
		return __inner_product(first1, last1, first2, val, func1, func2);
	}

	template<class _PtrIt1, class _PtrIt2, class _Ty>
	inline _Ty ptr_inner_product(_PtrIt1 first1, _PtrIt1 last1,
								 _PtrIt2 first2, _Ty val)
	{
		for (; first1 != last1; ++first1, ++first2)
			val += *first1 *  *first2;
		return (val);
	}





	//=====================================================================
	// Table Lookup
	//=====================================================================
	template <typename _PtrOut, typename _PtrTable, typename _PhaseInfo>
	void ptr_table_lookup(_PtrOut first, _PtrOut last, _PtrTable table, _PhaseInfo& info)
	{
		int idx;
		for (; first < last; ++first)
		{
			*first = table[static_cast<int>(info.phase + 0.5)];
			info.next();
		}
	}


	template <typename _PtrOut, typename _PtrTable, typename _PhaseInfo>
	void ptr_table_lookup(_PtrOut first, _PtrOut last, int oi, _PtrTable table, _PhaseInfo& info)
	{
		int idx;
		for (; first < last; first += oi)
		{
			*first = table[static_cast<int>(info.phase + 0.5)];
			info.next();
		}
	}


	template <typename _Interpolator, typename _PtrOut, typename _PtrTable, typename _PhaseInfo>
	void ptr_table_lookup(_PtrOut first, _PtrOut last, _PtrTable table, _PhaseInfo& info)
	{
		_Interpolator interp;
		int idx;
		for (; first < last; ++first)
		{
			idx = static_cast<int>(info.phase);
			*first = interp(table + (idx - _Interpolator::REQUIRE_PRE_SIZE), info.phase - idx);
			info.next();
		}
	}


	template <typename _Interpolator, typename _PtrOut, typename _PtrTable, typename _PhaseInfo>
	void ptr_table_lookup(_PtrOut first, _PtrOut last, int oi, _PtrTable table, _PhaseInfo& info)
	{
		_Interpolator interp;
		int idx;
		for (; first < last; first += oi)
		{
			idx = static_cast<int>(info.phase);
			*first = interp(table + (idx - _Interpolator::REQUIRE_PRE_SIZE), info.phase - idx);
			info.next();
		}
	}

	//-------------------rotating
	// use phase_modular
	template <typename _Interpolator, typename _PtrOut, typename _PtrTable, typename _PhaseInfo>
	void ptr_table_lookup_rotating(_PtrOut first, _PtrOut last, _PtrTable tfirst, _PtrTable tlast, _PhaseInfo& info)
	{
		_Interpolator interp;
		int i, idx, k;
		size_t tsize = std::distance(tfirst, tlast);
		std::iterator_traits<_PtrTable>::value_type param[_Interpolator::REQUIRE_PARAM_SIZE];

		for (; first != last; ++first)
		{
			idx = static_cast<int>(info.phase);
			if (idx + _Interpolator::REQUIRE_PARAM_SIZE - -_Interpolator::REQUIRE_PRE_SIZE >= (int)tsize)
			{
				k = (idx + _Interpolator::REQUIRE_PARAM_SIZE - -_Interpolator::REQUIRE_PRE_SIZE - tsize);
				for (i = 0; i < (_Interpolator::REQUIRE_PARAM_SIZE - k); ++i)
				{
					param[i] = *(tfirst + (idx - _Interpolator::REQUIRE_PRE_SIZE));
				}
				for (; i < _Interpolator::REQUIRE_PARAM_SIZE; ++i, ++k)
				{
					param[i] = *(tfirst + k);
				}

				*first = interp(param, info.phase - idx);
			}
			else
			{
				*first = interp(tfirst + (idx - _Interpolator::REQUIRE_PRE_SIZE), info.phase - idx);
			}
			info.next();
		}
	}







	//=====================================================================
	// Center Shift
	//=====================================================================
	template <typename _Ptr> inline _Ptr ptr_centershift(_Ptr first, _Ptr last)
	{
		return ptr_rotate(first, first + (static_cast<int>(std::distance(first, last) / 2.0 + 0.5)), last);
	}

	template <typename _Ptr> inline _Ptr ptr_icentershift(_Ptr first, _Ptr last)
	{
		return ptr_rotate(first, last - (static_cast<int>(std::distance(first, last) / 2.0 + 0.5)), last);
	}




	//=====================================================================
	// Accumulate
	//=====================================================================
	template <typename _Ptr, typename _Ty>
	_Ty ptr_accumulate(_Ptr first, _Ptr last, _Ty v)
	{
		for (; first < last; ++first)v += *first;
		return v;
	}

	template <typename _Ptr, typename _Ty, typename _Func>
	_Ty ptr_accumulate(_Ptr first, _Ptr last, _Ty v, _Func func)
	{
		for (; first < last; ++first)v = func(v, *first);
		return v;
	}

	template <typename _Ptr, typename _Ty>
	_Ty ptr_accumulate_minus(_Ptr first, _Ptr last, _Ty v)
	{
		for (; first < last; ++first)v -= *first;
		return v;
	}

	template <typename _Ptr, typename _Ty>
	_Ty ptr_accumulate_multiplies(_Ptr first, _Ptr last, _Ty v)
	{
		for (; first < last; ++first)v *= *first;
		return v;
	}

	template <typename _Ptr, typename _Ty>
	_Ty ptr_accumulate_divides(_Ptr first, _Ptr last, _Ty v)
	{
		for (; first < last; ++first)v /= *first;
		return v;
	}



	//=====================================================================
	// Correlation
	//=====================================================================
	template <typename _Ptr1, typename _Ptr2, typename __ResultPtr>
	__ResultPtr xcorrelation(_Ptr1 lbegin, _Ptr1 lend, _Ptr2 rbegin, __ResultPtr result)
	{
		for (; lbegin != lend; ++lbegin, ++result)	*result = miqs::ptr_inner_product(lbegin, lend, rbegin, 0.0);
		return result;
	}

	template <typename _Ptr1, typename _Ptr2, typename __ResultPtr>
	__ResultPtr xcorrelation(_Ptr1 lbegin, _Ptr1 lend, _Ptr2 rbegin, size_t max_lag, __ResultPtr result)
	{
		for (size_t i{}; (lbegin != lend) && (i <= max_lag); ++lbegin, ++result, ++i)*result = miqs::ptr_inner_product(lbegin, lend, rbegin, 0.0);
		return result;
	}






	//=====================================================================
	// Linear Prediction
	//=====================================================================
	template <typename _Ty>
	inline _Ty levinson_durbin_method(_Ty* ac, _Ty* lc, _Ty* rc, size_t lpc_order)
	{
		size_t i, m;
		_Ty ee;
		//	int errF = 1;
		_Ty k_m;

		if ((ac[0] == 0.0) || (ac[0] == -0.0))
		{
			for (i = 0; i < lpc_order; i++) lc[i] = 0.0;
			return 0.0;
		}

		rc[0] = 1.0;
		lc[0] = 1.0;

		ee = ac[0];
		for (m = 1; m <= lpc_order; m++)
		{
			_Ty sum = (_Ty)1.0 * ac[m];
			for (i = 1; i < m; i++)
			{
				sum += lc[i] * ac[m - i];
			}
			k_m = ((_Ty)-1.0 / ee) * sum;

			// save reflection coefficients
			rc[m] = k_m;
			lc[m] = k_m;


			for (i = 1; i <= m / 2; i++)
			{
				_Ty x = lc[i];
				lc[i] += k_m * lc[m - i];
				if ((i < (m / 2)) || ((m & 1) == 1)) lc[m - i] += k_m * x;
			}

			// update the error:
			ee *= ((_Ty)1.0 - k_m*k_m);
			if (ee == 0.0)
			{
				for (i = m; i <= lpc_order; i++)
				{
					lc[i] = 0.0;
					rc[m] = 0.0;
				}
				break;
			}
		}
		return ee;
	}



	template <typename _Ty>
	inline _Ty schur_recursion(_Ty* ac, _Ty* lc, _Ty* rc, size_t lpc_order)
	{
		size_t i, j;
		_Ty temp, alphap;

		_Ty *y1, *y2;

		y1 = new _Ty[lpc_order + 2];
		y2 = new _Ty[lpc_order + 2];

		rc[1] = -ac[1] / ac[0];
		alphap = ac[0] * (1 - (rc[1] * rc[1]));

		y2[1] = ac[1];
		y2[2] = ac[0] + rc[1] * ac[1];

		for (i = 2; i <= lpc_order; i++)
		{
			y1[1] = temp = ac[i];

			for (j = 1; j < i; j++)
			{
				y1[j + 1] = y2[j] + rc[j] * temp;
				temp += rc[j] * y2[j];
				y2[j] = y1[j];
			}

			rc[i] = -temp / y2[i];
			y2[i + 1] = y2[i] + rc[i] * temp;
			y2[i] = y1[i];

			alphap *= 1 - (rc[i] * rc[i]);
		}

		reflect_to_lpc(rc, lc, lpc_order);

		delete[] y1;
		delete[] y2;
		return(alphap);
	}



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
		auto * ac_last = miqs::xcorrelation(biter, eiter - lpc_order, biter, lpc_order, std::begin(ac));

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


	//=================================sterero panning
	inline std::pair<double, double> stereo_panning(double theta_rad, double half_rad_of_loudspeaker)
	{
		double t1 = std::tan(theta_rad), t2 = std::tan(half_rad_of_loudspeaker);
		std::pair<double, double> result{ -(t1 - t2) / (t1 + t2 + std::numeric_limits<double>::epsilon()), 1.0 };
		auto a = std::sqrt(result.first*result.first + result.second + result.second);
		result.first /= a;
		result.second /= a;
		return result;
	}



}