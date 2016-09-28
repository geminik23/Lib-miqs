#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	namespace window
	{
		

		
		
		/* blackman */
		template <size_t m_size>
		struct blackman_f: public _base_idx_gen_fixed<m_size>
		{
			blackman_f() { m_arg = 2 * miqs::PI / (m_size - 1); }
		
			sample_t operator()(size_t idx) noexcept
			{
				if (idx >= m_size) return 0.0;
				double x = m_arg * idx;
				return 0.42659 - 0.49656 * std::cos(x) + 0.076849 * std::cos(x + x);
			}

		private:
			double m_arg;
		};
		
		/* raised cosine */
		template <size_t m_size>
		struct raised_cosine_f: public _base_idx_gen_fixed<m_size>
		{
			raised_cosine_f(double alpha)
			{
				m_alpha = alpha;
			}
		
			sample_t operator()(size_t idx) noexcept
			{
				if (idx >= m_size) return 0.0;
				return (1.0 - m_alpha) - (m_alpha * std::cos(miqs::PI * 2 * idx++ / (double)(m_size - 1.0)));
			}
		private:
			double m_alpha;
		};
		
		
		
		/* hamming window : alpha - 0.46 */
		template <size_t m_size>
		struct hamming_f: public raised_cosine_f<m_size>
		{
			hamming_f():raised_cosine_f<m_size>(0.46) {}
		};
		
		/* hanning window : alpha - 0.5 */
		template <size_t m_size>
		struct hanning_f:public raised_cosine_f<m_size>
		{
			hanning_f():raised_cosine_f<m_size>(0.5) {}
		};
		
		/* triangular window */
		template <size_t m_size>
		struct triangular_f: public _base_idx_gen_fixed<m_size>
		{
			sample_t operator ()(size_t idx) noexcept
			{
				if (idx >= m_size) return 0.0;
				return 1.0 - (std::abs((int)idx * 2 - (int)m_size + 1) / (double)(m_size - 1.0));
			}
		};
		
		/* rectangular window */
		template <size_t m_size>
		struct rectangular_f: public _base_idx_gen_fixed<m_size>
		{
			sample_t operator ()(size_t idx) noexcept
			{
				if (idx >= m_size) return 0.0;
				return 1.0;
			}
		};





		/* blackman */
		struct blackman: public _base_idx_gen_size
		{
			blackman(size_t size):_base_idx_gen_size(size) { m_arg = 2 * miqs::PI / (m_size - 1); }

			sample_t operator()(size_t idx) noexcept
			{
				if (idx >= m_size) return 0.0;
				double x = m_arg * idx;
				return 0.42659 - 0.49656 * std::cos(x) + 0.076849 * std::cos(x + x);
			}
		private:
			double m_arg;
		};

		/* raised cosine */
		struct raised_cosine: public _base_idx_gen_size
		{
			raised_cosine(size_t size, double alpha):_base_idx_gen_size(size)
			{
				m_alpha = alpha;
			}

			sample_t operator()(size_t idx) noexcept
			{
				if (idx >= m_size) return 0.0;
				return (1.0 - m_alpha) - (m_alpha * std::cos(miqs::PI * 2 * idx++ / (double)(m_size - 1.0)));
			}
		private:
			double m_alpha;
		};



		/* hamming window : alpha - 0.46 */
		struct hamming: public raised_cosine
		{
			hamming(size_t size):raised_cosine(size, 0.46){}
		};

		/* hanning window : alpha - 0.5 */
		struct hanning:public raised_cosine
		{
			hanning(size_t size):raised_cosine(size, 0.5){}
		};

		/* triangular window */
		struct triangular: public _base_idx_gen_size
		{
			sample_t operator ()(size_t idx) noexcept
			{
				if (idx >= m_size) return 0.0;
				return 1.0 - (std::abs((int)idx * 2 - (int)m_size + 1) / (double)(m_size - 1.0));
			}
		};

		/* rectangular window */
		struct rectangular: public _base_idx_gen_size
		{
			sample_t operator ()(size_t idx) noexcept
			{
				if (idx >= m_size) return 0.0;
				return 1.0;
			}
		};


	}
}
