#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	namespace interpolation
	{
		//
		/* interpolation
			_Interpolator::REQUIRE_PRE_SIZE     x[n-N]
			_Interpolator::REQUIRE_POST_SIZE    x[n + N]
			_Interpolator::reset()
			_Interpolator::operator() (_Iter delay, double fraction)
		*/
		/* ==== linear_interpolation ==== */
		// y[n] = x[n - (M+1)]*frac + x[n-M]*(1-frac)
		struct nointerpol
		{
			enum { REQUIRE_PRE_SIZE = 0, REQUIRE_POST_SIZE = 0, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1 };

			void reset() {}

			template <typename _Iter>
			sample_t operator()(_Iter delay, double fraction)
			{
				return *delay;
			}
		};


		struct linear
		{
			enum { REQUIRE_PRE_SIZE = 0, REQUIRE_POST_SIZE = 1, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1};

			void reset() {}

			template <typename _Iter>
			sample_t operator()(_Iter delay, double fraction)
			{
				sample_t x1 = *delay++;
				return x1 + (*delay - x1) * fraction;
			}
		};

		/* ==== cosine-interpolation === */
		struct cosine
		{
			enum { REQUIRE_PRE_SIZE = 0, REQUIRE_POST_SIZE = 1, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1 };

			void reset() {}

			template <typename _Iter>
			sample_t operator()(_Iter delay, double fraction)
			{
				sample_t x1 = *delay++;
				return x1 + (*delay - x1) * ((1.0 - std::cos(frac * miqs::Pi)) * 0.5);
			}
		};

		/* ==== cubic-interpolation === */
		struct cubic
		{
			enum { REQUIRE_PRE_SIZE = 1, REQUIRE_POST_SIZE = 1, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1 };

			void reset() {}

			template <typename _Iter>
			sample_t operator()(_Iter delay, double fraction)
			{
				
				double x1 = *delay++;
				double x2 = *delay++;
				double x3 = *delay;
				double x0 = x1 + (x1 - x2);
				double a0{}, a1{}, a2{}, a3{};

				a3 = fraction * fraction; a3 -= 1.0; a3 *= (1.0 / 6.0);
				a2 = (fraction + 1.0) * 0.5; a0 = a2 - 1.0;
				a1 = a3 * 3.0; a2 -= a1; a0 -= a3; a1 -= fraction;
				a0 *= fraction; a1 *= fraction; a2 *= fraction; a3 *= fraction; a1 += 1.0;

				return (a0*x0 + a1*x1 + a2*x2 + a3*x3);
			}
		};


		
		///* ==== allpass_interpolation ==== */
		//// y[n] = x[n-(M+1)]*frac + x[n-M]*(1-frac) - y[n-1]*(1-frac)
		struct allpass
		{
			enum { REQUIRE_PRE_SIZE = 0, REQUIRE_POST_SIZE = 1, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1};

			void reset() { y_delay = 0; }

			template <typename _Iter>
			sample_t operator()(_Iter delay, double fraction)
			{
				sample_t out = *delay++*(1 - fraction);
				out += *delay*fraction;
				out -= y_delay*(1 - fraction);
				y_delay = out;
				return out;
			}

			sample_t y_delay{};
		};
		
		/* ==== spline_interpolation ==== */
		// y[n] = x[n - [M+1]) * ( frac^3 / 6) 
		//				+ x[n - M) * ( ((1+frac)^3 -4*frac^3) /6 )
		//				+ x[n - [M-1]) * ( ((2-frac)^3 -4*(1-frac)^3) / 6 )
		//				+ x[n - [M-2]) * ( (1-frac)^3 / 6 )
		struct spline
		{
			enum { REQUIRE_PRE_SIZE = 2, REQUIRE_POST_SIZE = 1, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1 };

			void reset() {}

			template <typename _Iter>
			sample_t operator()(_Iter delay, double fraction)
			{
				double f3 = fraction*fraction*fraction;
				double f1p = (1 + fraction);
				double f2m = (2 - fraction);
				double f1m = (1 - fraction);
				sample_t out = *delay++*(f3 / 6);
				out += *delay++*(((f1p*f1p*f1p) - 4 * f3) / 6);
				out += *delay++*((f2m*f2m*f2m - 4 * f1m*f1m*f1m) / 6);
				out += *delay*(f1m *f1m *f1m / 6);
				return out;
			}
		};

	}
}