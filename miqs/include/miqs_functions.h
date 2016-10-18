#pragma once

#include "miqs_basictype.h"



namespace miqs
{
	/* cartesian & polar coordinates */
	template <typename _Ty= sample_t>
	struct cartesian_to_polar : _base_trans_arg1<std::pair<_Ty, _Ty>>
	{
		result_type operator()(argument_type const& c)
		{
			return result_type(std::sqrt(c.first * c.first + c.second * c.second), std::atan2(c.second, c.first));
		}
	};

	template <typename _Ty = sample_t>
	struct polar_to_cartesian: _base_trans_arg1<std::pair<_Ty, _Ty>>
	{
		result_type operator()(argument_type const& c)
		{
			return result_type(c.first*std::cos(c.second), c.first*std::sin(c.second));
		}
	};


	/* sign */
	template<typename T>
	inline int32_t sign(T a)
	{
		return ((a >= 0) ? 1 : (-1));
	}

	template<typename T>
	inline double half_sign(T a)
	{
		return ((a >= 0) ? 0.5 : (-0.5));
	}






	//
	// zero_crossing
	//

	/* zero-cross */
	template <typename _Iter>
	uint32_t zero_cross(_Iter biter, _Iter eiter)
	{
		uint32_t result = 0;
		for (_Iter i = biter; i != eiter - 1; i++)
			result += static_cast<uint32_t>(std::abs(half_sign(*(i + 1)) - half_sign((*i))));
		return result;
	}

	/* bounded zero-cross*/
	// bound is samples
	template <typename _Iter>
	uint32_t bounded_zero_cross(_Iter biter, _Iter eiter, std::pair<size_t, size_t> bound)
	{
		uint32_t result = 0;
		size_t count = 0;
		bool first = true;
		for (_Iter i = biter; i != eiter - 1; i++)
		{
			++count;
			if (static_cast<uint32_t>(std::abs(half_sign(*(i + 1)) - half_sign((*i)))))
			{
				if (first || (count >= bound.first && count <= bound.second))
				{
					first = false;
					++result;
					count = 0;
				}
			}

			if (count > bound.second)
			{
				first = true;
				count = 0;
			}

		}
		return result;
	}


	/* zero_cross_to_freqeuncy */
	inline double zero_cross_to_freq(size_t zc, size_t buffsize, miqs::uint32_t sr)
	{
		return 	((double)zc) / 2.0 * (double)sr / (double)buffsize;
	}









}