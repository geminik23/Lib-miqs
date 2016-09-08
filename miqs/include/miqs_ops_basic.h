#pragma once
#include "miqs_basictype.h"

namespace miqs
{

	// sine wave operator
	struct sine_wave
	{
		sample_t operator()(double phase) { return std::sin(phase); }
	};

	// cosine wave operator
	struct cosine_wave
	{
		sample_t operator()(double phase) { return std::cos(phase); }
	};

	// basic operators
	template <typename _T>
	struct op_add
	{
		op_add(_T const& v):m_value(v) {}

		_T operator() (_T const& v) noexcept
		{
			return v + m_value;
		}
		_T m_value;
	};

	template <typename _T>
	struct op_multiply
	{
		op_multiply(_T const& v):m_value(v) {}

		_T operator() (_T const& v) noexcept
		{
			return v * m_value;
		}
		_T m_value;
	};

	template <typename _T>
	struct op_clip
	{
		op_clip(_T const& max, _T const& min):m_max{ max }, m_min{ min } {}

		_T operator() (_T const& i) noexcept
		{
			auto v = i;
			if (v < m_min) v = m_min;
			if (v > m_max) v = m_max;
			return v;
		}

		_T m_min, m_max;
	};



	template <typename _T>
	struct op_logarithmic_amplitude_20log
	{
		_T operator() (const _T& v)
		{
			return 20 * std::log10(v);
		}
	};

	template <typename _T>
	struct op_logarithmic_amplitude_10log
	{
		_T operator() (const _T& v)
		{
			return 10 * std::log10(v);
		}
	};



}