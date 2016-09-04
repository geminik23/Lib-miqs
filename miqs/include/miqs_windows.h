#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	namespace window
	{
		/** this is base window functor. need phasor_index */
		template <size_t _N>
		struct _base_window
		{
			size_t size() const noexcept { return _N; }
		};

		/* blackman */
		template <size_t _N>
		struct blackman: public _base_window<_N>
		{
			blackman() { m_arg = 2 * miqs::PI / (_N - 1); }

			sample_t operator()(size_t idx) noexcept
			{
				if (idx >= _N) return 0.0;
				double x = m_arg * idx;
				return 0.42659 - 0.49656 * std::cos(x) + 0.076849 * std::cos(x + x);
			}
		private:
			double m_arg;
		};

		/* raised cosine */
		template <size_t _N>
		struct raised_cosine: public _base_window<_N>
		{
			raised_cosine(double alpha)
			{
				m_alpha = alpha;
			}

			sample_t operator()(size_t idx) noexcept
			{
				if (idx >= _N) return 0.0;
				return (1.0 - m_alpha) - (m_alpha * std::cos(miqs::PI * 2 * idx++ / (double)(_N - 1.0)));
			}
		private:
			double m_alpha;
		};



		/* hamming window : alpha - 0.46 */
		template <size_t _N>
		struct hamming: public raised_cosine<_N>
		{
			hamming():raised_cosine<_N>(0.46){}
		};

		/* hanning window : alpha - 0.5 */
		template <size_t _N>
		struct hanning:public raised_cosine<_N>
		{
			hanning():raised_cosine<_N>(0.5){}
		};

		/* triangular window */
		template <size_t _N>
		struct triangular: public _base_window<_N>
		{
			sample_t operator ()(size_t idx) noexcept
			{
				if (idx >= _N) return 0.0;
				return 1.0 - (std::abs((int)idx * 2 - (int)_N + 1) / (double)(_N - 1.0));
			}
		};

		/* rectangular window */
		template <size_t _N>
		struct rectangular: public _base_window<_N>
		{
			sample_t operator ()(size_t idx) noexcept
			{
				if (idx >= _N) return 0.0;
				return 1.0;
			}
		};


	}
}