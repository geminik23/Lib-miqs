#pragma once
#include "filter.h"

namespace miqs
{

	/** canonical filter
	- a[0] is ignored.
	- xh[n] = x[n] - a[1]*x[n-1] - a[2]*x[n-2];
	- y[n] = b[0]xh[n] + b[1]xh[n-1] + b[2]xh[n-2]
	*/

	struct canonical_filter
	{
		template<typename _A_Cont, typename _B_Cont, typename _D_Cont>
		canonical_filter(_A_Cont& a, _B_Cont& b, _D_Cont& dx)
			: m_a{ a.data() }, m_b{ b.data() }, m_dx{ dx.data() } {}

		canonical_filter(sample_t * a, sample_t* b, sample_t * dx)
			: m_a{ a }, m_b{ b }, m_dx{ dx } {}


		sample_t operator()(sample_t in)
		{
			sample_t t = in - m_a[1] * m_dx[0] - m_a[2] * m_dx[1];
			sample_t out = m_b[0] * t + m_b[1] * m_dx[0] + m_b[2] * m_dx[1];
			m_dx[1] = m_dx[0];
			m_dx[0] = t;
			return out;
		}

	private:
		array_ref_n<sample_t, 2> m_dx;
		array_ref_n<sample_t, 3> m_a;
		array_ref_n<sample_t, 3> m_b;
	};




	/* canonical_calculator */
	template <typename _FilterTy, typename _InfoTy>
	struct canonical_coefficients_calculator
	{
		canonical_coefficients_calculator(_InfoTy& info):m_info{ info } {}

		template <typename _Iter1, typename _Iter2>
		void operator()(_Iter1 a_begin, _Iter2 b_begin)
		{
			_FilterTy()(std::tan(miqs::pi * m_info.normalized_cutoff_freqeuncy() / 2.0)
						, m_info.normalized_cutoff_freqeuncy() / m_info.normalized_bandwidth()
						, a_begin, b_begin);
		};

	private:
		_InfoTy& m_info;
	};




	//=================================
	template <typename _FilterTy, typename _Info, typename _Iter1, typename _Iter2>
	void calculate_canonical_coefficients(_Info & info, _Iter1 a_begin, _Iter2 b_begin)
	{
		canonical_coefficients_calculator<_FilterTy, _Info>{info}(a_begin, b_begin);
	}


	namespace canonical
	{

		/* first order filters */
		struct first_order_lowpass
		{

			template <typename _Iter1, typename _Iter2>
			void operator()(double K, double Q, _Iter1 a_begin, _Iter2 b_begin)
			{
				*a_begin++ = 1.0;
				*a_begin = (K - 1) / (K + 1);
				*b_begin++ = K / (K + 1);
				*b_begin = K / (K + 1);
			}
		};

		struct first_order_highpass
		{

			template <typename _Iter1, typename _Iter2>
			void operator()(double K, double Q, _Iter1 a_begin, _Iter2 b_begin)
			{
				*a_begin++ = 1.0;
				*a_begin = (K - 1) / (K + 1);
				*b_begin++ = 1 / (K + 1);
				*b_begin = -1 / (K + 1);
			}
		};
		struct first_order_allpass
		{

			template <typename _Iter1, typename _Iter2>
			void operator()(double K, double Q, _Iter1 a_begin, _Iter2 b_begin)
			{
				*a_begin++ = 1.0;
				*a_begin = (K - 1) / (K + 1);
				*b_begin++ = (K - 1) / (K + 1);
				*b_begin = 1;
			}
		};


		/* second order filters*/
		struct second_order_lowpass
		{

			template <typename _Iter1, typename _Iter2>
			void operator()(double K, double Q, _Iter1 a_begin, _Iter2 b_begin)
			{
				double kkq = K*K*Q;
				*a_begin++ = 1.0;
				*a_begin++ = (2 * Q*(K*K - 1)) / (kkq + K + Q);
				*a_begin = (kkq - K + Q) / (kkq + K + Q);

				double t = kkq / (kkq + K + Q);
				*b_begin++ = t;
				*b_begin++ = 2 * t;
				*b_begin = t;
			}
		};
		struct second_order_highpass
		{

			template <typename _Iter1, typename _Iter2>
			void operator()(double K, double Q, _Iter1 a_begin, _Iter2 b_begin)
			{
				double kkq = K*K*Q;
				*a_begin++ = 1.0;
				*a_begin++ = (2 * Q*(K*K - 1)) / (kkq + K + Q);
				*a_begin = (kkq - K + Q) / (kkq + K + Q);

				double t = Q / (kkq + K + Q);
				*b_begin++ = t;
				*b_begin++ = -2 * t;
				*b_begin = t;
			}
		};

		struct second_order_bandpass
		{

			template <typename _Iter1, typename _Iter2>
			void operator()(double K, double Q, _Iter1 a_begin, _Iter2 b_begin)
			{
				double kkq = K*K*Q;
				*a_begin++ = 1.0;
				*a_begin++ = (2 * Q*(K*K - 1)) / (kkq + K + Q);
				*a_begin = (kkq - K + Q) / (kkq + K + Q);

				double t = K / (kkq + K + Q);
				*b_begin++ = t;
				*b_begin++ = 0.0;
				*b_begin = -t;
			}
		};

		struct second_order_bandreject
		{

			template <typename _Iter1, typename _Iter2>
			void operator()(double K, double Q, _Iter1 a_begin, _Iter2 b_begin)
			{
				double kkq = K*K*Q;
				*a_begin++ = 1.0;
				*a_begin++ = (2 * Q*(K*K - 1)) / (kkq + K + Q);
				*a_begin = (kkq - K + Q) / (kkq + K + Q);

				double t = 1 / (kkq + K + Q);
				*b_begin++ = Q*(1 + K*K) / t;
				*b_begin++ = 2 * Q*(K*K - 1) / t;
				*b_begin = Q*(1 + K*K) / t;
			}
		};

		struct second_order_allpass
		{

			template <typename _Iter1, typename _Iter2>
			void operator()(double K, double Q, _Iter1 a_begin, _Iter2 b_begin)
			{
				double kkq = K*K*Q;
				*a_begin++ = 1.0;
				*a_begin++ = (2 * Q*(K*K - 1)) / (kkq + K + Q);
				*a_begin = (kkq - K + Q) / (kkq + K + Q);

				double t = 1 / (kkq + K + Q);
				*b_begin++ = (kkq - K + Q) / t;
				*b_begin++ = (2 * Q*(K*K - 1)) / t;
				*b_begin = 1.0;
			}
		};
	}

}