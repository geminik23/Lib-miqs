#pragma once

#include "miqs_basictype.h"
#include "miqs_math.h"

namespace miqs
{

	namespace envelope
	{

		// base structures
		using _base_env_noarg = _base_idx_gen_size;

		struct _base_env_2args:_base_idx_gen_size
		{
			_base_env_2args(size_t a, size_t r):
				_base_idx_gen_size(a + r), m_asr(a, r) {}

		protected:
			std::pair<size_t, size_t> m_asr;
		};


		struct _base_env_3args:_base_idx_gen_size
		{
			_base_env_3args(size_t a, size_t s, size_t r):
				_base_idx_gen_size(a + s + r), m_asr(a, s, r) {}

		protected:
			std::tuple<size_t, size_t, size_t> m_asr;
		};




		
		template <typename _Ty>
		struct piecewise_constant : ::miqs::_base_idx_gen
		{
			typedef size_t intervals_type;
			typedef _Ty weights_type;

			template <typename _FirstIter, typename _SecondIter> 
			piecewise_constant(_FirstIter first, _FirstIter last, _SecondIter second):
				m_intervals(first, last), m_weights(second, second + (std::distance(first, last)-1)) {}

			std::vector<intervals_type> intervals() const noexcept { return m_intervals; }
			std::vector<weights_type> weights() const noexcept { return m_weights; }
			weights_type max_value() const noexcept { return *std::max_element(std::begin(m_weights), std::end(weights)); }
			weights_type min_value() const noexcept { return *std::min_element(std::begin(m_weights), std::end(weights)); }

			// function operator

			result_type operator()(argument_type const& i)
			{
				auto iter = std::adjacent_find(std::begin(m_intervals), std::end(m_intervals), 
										 [i](intervals_type const& v1 , intervals_type const& v2) { return i>=v1 &&  i<v2; });
				auto idx = static_cast<size_t>(std::abs(std::distance(std::begin(m_intervals), iter)));
				if( idx <m_weights.size())return m_weights[idx];

				return static_cast<result_type>(0);
			}
		private:
			std::vector<intervals_type> m_intervals;
			std::vector<weights_type> m_weights;
		};

		/* 3 args */
		struct trapezoidal:_base_env_3args
		{
			typedef _base_env_3args base_type;
			typedef trapezoidal self_type;

			using _base_env_3args::_base_env_3args;

			result_type operator()(argument_type const& i)
			{
				if (i >= m_size) return 0.0;

				if (i < std::get<0>(m_asr))
				{
					return static_cast<result_type>((1.0 / std::get<0>(m_asr))*i);
				}
				else if (i >= std::get<0>(m_asr) + std::get<1>(m_asr))
				{
					return static_cast<result_type>(1.0 - ((1.0 / std::get<2>(m_asr))*(i - (std::get<0>(m_asr) + std::get<1>(m_asr)))));
				}
				return 1.0;
			}
		};

		struct bell:_base_env_3args
		{
			typedef _base_env_3args base_type;
			typedef bell self_type;

			using _base_env_3args::_base_env_3args;

			result_type operator()(argument_type const& i)
			{
				if (i >= m_size) return 0.0;

				if (i < std::get<0>(m_asr))
				{
					return static_cast<result_type>((1.0 + cos(miqs::Pi + (miqs::Pi * static_cast<double>(i) / std::get<0>(m_asr)))) / 2.0);
				}
				else if (i >= std::get<0>(m_asr) + std::get<1>(m_asr))
				{
					return static_cast<result_type>((1.0 + cos(miqs::Pi * static_cast<double>(i - (std::get<0>(m_asr) + std::get<1>(m_asr))) / std::get<0>(m_asr))) / 2.0);
				}
				return 1.0;
			}
		};




	}
}