#pragma once
#include "base.h"
#include "math.h"
namespace miqs
{

	template <typename _Ty = sample_t>
	struct clipping: _base_trans_arg1<_Ty>, _base_value_type<_Ty>
	{
		clipping(value_type maxv, value_type minv):max{ maxv }, min{ minv } {}
	
		result_type operator()(argument_type v)
		{
			if (v > max) v = max;
			if (v < min) v = min;
			return v;
		}
		
		value_type max, min;
	};

	// sine wave operator
	template <typename _Ty = sample_t>
	struct sine_wave:_base_trans_arg1<_Ty>
	{
		result_type operator()(argument_type phase) { return std::sin(phase); }
	};

	// cosine wave operator
	template <typename _Ty = sample_t>
	struct cosine_wave:_base_trans_arg1<_Ty>
	{
		result_type operator()(argument_type phase) { return std::cos(phase); }
	};

	// rectangular_wave operator
	template <typename _Ty = sample_t>
	struct rectangular_wave:_base_trans_arg1<_Ty>
	{
		result_type operator()(argument_type phase) { return (phase <= miqs::pi) ? 1.0 : -1.0; }
	};

	// triagular_wave operator
	template <typename _Ty = sample_t>
	struct triagular_wave:_base_trans_arg1<_Ty>
	{
		result_type operator()(argument_type phase)
		{
			result_type t = (2.0 *(phase *(1.0 / miqs::two_pi))) - 1.0;
			if (t < 0.0) t *= -1;
			t = 2.0*(t - 0.5);
			return t;
		}
	};

	// saw_wave operator
	template <typename _Ty = sample_t>
	struct saw_wave:_base_trans_arg1<_Ty>
	{
		result_type operator()(argument_type phase)	{return (2.0 *(phase *(1.0 / miqs::two_pi))) - 1.0;	}
	};


	//====================================Math

	/* cartesian & polar coordinates */
	template <typename _Ty = sample_t>
	struct cartesian_to_polar: _base_trans_arg1<std::pair<_Ty, _Ty>>
	{
		result_type operator()(argument_type const& c)	{return result_type(std::sqrt(c.first * c.first + c.second * c.second), std::atan2(c.second, c.first));}
	};

	template <typename _Ty = sample_t>
	struct polar_to_cartesian: _base_trans_arg1<std::pair<_Ty, _Ty>>
	{
		result_type operator()(argument_type const& c)	{return result_type(c.first*std::cos(c.second), c.first*std::sin(c.second));}
	};


	template <typename _Ty = sample_t>
	struct exponential_amplitude_exp20
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;
		
		result_type operator() (const argument_type& v)	{	return std::exp(v / 20.0);	}
	};

	template <typename _Ty = sample_t>
	struct exponential_amplitude_exp10
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		result_type operator() (const argument_type& v)	{	return std::exp(v / 10.0);	}
	};

	template <typename _Ty = sample_t>
	struct logarithmic_amplitude_20log
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		result_type operator() (const argument_type& v)	{return 20 * std::log(v + std::numeric_limits<result_type>::epsilon());	}
	};

	template <typename _Ty = sample_t>
	struct logarithmic_amplitude_10log
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		result_type operator() (const argument_type& v){return 10 * std::log(v + std::numeric_limits<result_type>::epsilon());}
	};

	template <typename _Ty = sample_t>
	struct log10: _base_trans_arg1<_Ty>
	{
		result_type operator() (const argument_type& v)	{return std::log10(v);}
	};

	template <typename _Ty = sample_t>
	struct log: _base_trans_arg1<_Ty>
	{
		result_type operator() (const argument_type& v){return std::log(v);}
	};

	template <typename _Ty = sample_t>
	struct exp: _base_trans_arg1<_Ty>
	{
		result_type operator() (const argument_type& v)	{return std::exp(v);}
	};


	//==================================windows
	namespace window
	{
		/* blackman */
		template <size_t m_size>
		struct blackman_f: public _base_idx_gen_fixed<m_size>
		{
			blackman_f() { m_arg = 2 * miqs::pi / (m_size - 1); }

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
				return (1.0 - m_alpha) - (m_alpha * std::cos(miqs::pi * 2 * idx++ / (double)(m_size - 1)));
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
			blackman(size_t size):_base_idx_gen_size(size) { m_arg = 2 * miqs::pi / (m_size - 1); }

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
				return (1.0 - m_alpha) - (m_alpha * std::cos(miqs::pi * 2 * idx++ / (double)(m_size - 1.0)));
			}
		private:
			double m_alpha;
		};



		/* hamming window : alpha - 0.46 */
		struct hamming: public raised_cosine
		{
			hamming(size_t size):raised_cosine(size, 0.46) {}
		};

		/* hanning window : alpha - 0.5 */
		struct hanning:public raised_cosine
		{
			hanning(size_t size):raised_cosine(size, 0.5) {}
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



	//==================== Envelope
	namespace envelope
	{
		// base structures
		using _base_env_noarg = _base_idx_gen_size;

		struct _base_env_2args:_base_idx_gen_size
		{
			_base_env_2args(size_t a, size_t r):_base_idx_gen_size(a + r), m_asr(a, r) {}
		protected:
			std::pair<size_t, size_t> m_asr;
		};

		struct _base_env_3args:_base_idx_gen_size
		{
			_base_env_3args(size_t a, size_t s, size_t r):_base_idx_gen_size(a + s + r), m_asr(a, s, r) {}
		protected:
			std::tuple<size_t, size_t, size_t> m_asr;
		};

		//
		template <typename _Ty = sample_t>
		struct linear:_base_trans_arg1<_Ty>
		{
			linear(std::pair<sample_t, sample_t>const& minmax, size_t size):
				m_size{ size }, m_minmax{ minmax } {}

			result_type operator()(argument_type phase)
			{
				double dif = m_size - 1 - phase;
				if (dif <= 0) return m_minmax.second;
				return (m_minmax.second - m_minmax.first)*(phase) / (m_size - 1) + m_minmax.first;
			}

			void set_size(size_t size) { m_size = size; }
			size_t size() { return m_size; }

		private:
			std::pair<sample_t, sample_t> m_minmax;
			size_t m_size;
		};

		template <typename _Ty = sample_t>
		struct exponential:_base_trans_arg1<_Ty>
		{

			exponential(std::pair<sample_t, sample_t>const& minmax, size_t size):
				m_size{ size }, m_minmax{ minmax } {}

			result_type operator()(argument_type phase)
			{
				double dif = m_size - 1 - phase;
				if (dif <= 0) return m_minmax.second;
				return static_cast<result_type>(m_minmax.first * std::pow(m_minmax.second / m_minmax.first, phase));
			}

			void set_size(size_t size) { m_size = size; }
			size_t size() { return m_size; }

		private:
			std::pair<sample_t, sample_t> m_minmax;
			size_t m_size;
		};


		template <typename _Ty = sample_t>
		struct adsr:_base_trans_arg1<_Ty>
		{
			adsr(double max_amp, std::tuple<sample_t, sample_t, sample_t> const& adsr, double amp_sus, size_t size):
				m_maxamp{ max_amp }, m_size{ size }, m_adsr{ adsr }, m_amp_sus{ amp_sus } {}


			result_type operator()(argument_type phase)
			{
				double dif = m_size - 1 - phase;
				if (dif <= 0) return static_cast<result_type>(0);
				if (phase <= std::get<0>(m_adsr))
				{
					return phase*(m_maxamp / std::get<0>(m_adsr));
				}
				else if (phase <= std::get<0>(m_adsr) + std::get<1>(m_adsr))
				{
					return ((m_amp_sus - m_maxamp) / std::get<1>(m_adsr))*(m_size - std::get<0>(m_adsr)) + m_maxamp;
				}
				else if (phase <= m_size - std::get<2>(m_adsr))
					return m_amp_sus;
				return -(m_amp_sus / std::get<2>(m_adsr))*(phase - (m_size - std::get<2>(m_adsr))) + m_amp_sus;
			}

			void set_size(size_t size) { m_size = size; }
			size_t size() { return m_size; }

		private:
			std::tuple<sample_t, sample_t, sample_t> m_adsr;
			double m_maxamp;
			double m_amp_sus;
			size_t m_size;
		};



		template <typename _Ty>
		struct piecewise_constant: ::miqs::_base_idx_gen
		{
			typedef size_t intervals_type;
			typedef _Ty weights_type;

			template <typename _FirstIter, typename _SecondIter>
			piecewise_constant(_FirstIter first, _FirstIter last, _SecondIter second):
				m_intervals(first, last), m_weights(second, second + (std::distance(first, last) - 1)) {}

			std::vector<intervals_type> intervals() const noexcept { return m_intervals; }
			std::vector<weights_type> weights() const noexcept { return m_weights; }
			weights_type max_value() const noexcept { return *std::max_element(std::begin(m_weights), std::end(weights)); }
			weights_type min_value() const noexcept { return *std::min_element(std::begin(m_weights), std::end(weights)); }

			// function operator

			result_type operator()(argument_type const& i)
			{
				auto iter = std::adjacent_find(std::begin(m_intervals), std::end(m_intervals),
											   [i](intervals_type const& v1, intervals_type const& v2) { return i >= v1 &&  i<v2; });
				auto idx = static_cast<size_t>(std::abs(std::distance(std::begin(m_intervals), iter)));
				if (idx <m_weights.size())return m_weights[idx];

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
					return static_cast<result_type>((1.0 + cos(miqs::pi + (miqs::pi * static_cast<double>(i) / std::get<0>(m_asr)))) / 2.0);
				}
				else if (i >= std::get<0>(m_asr) + std::get<1>(m_asr))
				{
					return static_cast<result_type>((1.0 + cos(miqs::pi * static_cast<double>(i - (std::get<0>(m_asr) + std::get<1>(m_asr))) / std::get<0>(m_asr))) / 2.0);
				}
				return 1.0;
			}
		};
	}


	
}