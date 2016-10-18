#pragma once
#include "miqs_basictype.h"


namespace miqs
{

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