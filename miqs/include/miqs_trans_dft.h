#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	template <typename _Ty =sample_t>
	struct dft
	{
		typedef _Ty value_type;
		typedef value_type* pointer;

		void operator()(pointer first, pointer  last, pointer dst)
		{
			size_t N = std::distance(first, last);
			int i, j, k;
			for (i = 0, k = 0; k < N; i += 2, ++k)
			{
				dst[i] = dst[i + 1] = 0;
				for (j = 0; j < N; ++j)
				{
					dst[i] += first[j] * cos(j*k*miqs::two_pi / N);
					dst[i+1] -= first[j] * sin(j*k*miqs::two_pi / N);
				}
				dst[i] /= N;
				dst[i + 1] /= N;
			}
		}
	};

	template <typename _Ty = sample_t>
	struct idft
	{
		typedef _Ty value_type;
		typedef value_type* pointer;

		void operator()(pointer first, pointer  last, pointer dst)
		{
			size_t N = std::distance(first, last) / 2;
			int i, j, k;
			for (i = 0; i < N; ++i)
			{
				dst[i] = 0;
				for (j = 0, k = 0; k < N; ++k, j += 2)
				{
					dst[i] += first[j] * cos(i*k*miqs::two_pi / N) - first[j + 1] * sin(i*k*miqs::two_pi / N);
				}
			}
		}
	};
}