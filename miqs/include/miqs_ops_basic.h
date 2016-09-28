#pragma once
#include "miqs_basictype.h"

namespace miqs
{

	// sine wave operator
	struct sine_wave
	{
		typedef sample_t result_type;
		typedef double argument_type;

		sample_t operator()(double phase) { return std::sin(phase); }
	};

	// cosine wave operator
	struct cosine_wave
	{
		typedef sample_t result_type;
		typedef double argument_type;

		sample_t operator()(double phase) { return std::cos(phase); }
	};


	// linear

	struct linear
	{
		typedef sample_t result_type;
		typedef size_t argument_type;

		linear(std::pair<sample_t, sample_t> minmax, size_t size):
			m_size{ size }, m_minmax{ minmax } {}

		result_type operator()(argument_type idx)
		{
			int dif = static_cast<int>(m_size - 1) - static_cast<int>(idx);
			if (dif <= 0) return m_minmax.second;
			return (m_minmax.second - m_minmax.first)*(idx) / (m_size - 1) + m_minmax.first;
		}

		void set_size(size_t size) { m_size = size; }
		size_t size() { return m_size; }

	private:
		std::pair<sample_t, sample_t> m_minmax;
		size_t m_size;
	};



	//
	// extend operators
	




	// basic operators
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename _Ty = sample_t>
	struct plus_const
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		plus_const(_Ty const& v):m_value(v) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return v + m_value;
		}
		_Ty m_value;
	};

	template <typename _Ty = sample_t>
	struct multiplies_const
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		multiplies_const(_Ty const& v):m_value(v) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return v * m_value;
		}
		_Ty m_value;
	};

	template <typename _Ty = sample_t>
	struct divides_const
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		divides_const(_Ty const& v):m_value(v) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return v / m_value;
		}
		_Ty m_value;
	};

	template <typename _Ty = sample_t>
	struct minus_const
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		minus_const(_Ty const& v):m_value(v) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return v - m_value;
		}
		_Ty m_value;
	};


	////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename _Ty = sample_t> 
	struct plus_ref
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		plus_ref(_Ty & v):m_value(v) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return v + m_value;
		}
		_Ty& m_value;
	};

	template <typename _Ty = sample_t> 
	struct minus_ref
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		minus_ref(_Ty & v):m_value(v) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return v - m_value;
		}
		_Ty& m_value;
	};

	template <typename _Ty = sample_t> 
	struct multiplies_ref
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		multiplies_ref(_Ty & v):m_value(v) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return v * m_value;
		}
		_Ty& m_value;
	};

	template <typename _Ty = sample_t> 
	struct divides_ref
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		divides_ref(_Ty & v):m_value(v) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return v / m_value;
		}
		_Ty& m_value;
	};


	////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename _Iter, typename _Ty = std::iterator_traits<_Iter>::value_type>
	struct multiplies_iter
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		multiplies_iter(_Iter iter):m_iter(iter) {}

		result_type operator() (argument_type const& v) noexcept
		{
			result_type d = *m_iter++;
			return static_cast<_Ty>(v * d);
		}

		_Iter m_iter;
	};

	template <typename _Iter, typename _Ty = std::iterator_traits<_Iter>::value_type>
	struct plus_iter
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		plus_iter(_Iter iter):m_iter(iter) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return static_cast<_Ty>(v + *m_iter++);
		}

		_Iter m_iter;
	};

	template <typename _Iter, typename _Ty = std::iterator_traits<_Iter>::value_type>
	struct minus_iter
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		minus_iter(_Iter iter):m_iter(iter) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return static_cast<_Ty>(v - *m_iter++);
		}

		_Iter m_iter;
	};

	template <typename _Iter, typename _Ty = std::iterator_traits<_Iter>::value_type>
	struct divides_iter
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		divides_iter(_Iter iter):m_iter(iter) {}

		result_type operator() (argument_type const& v) noexcept
		{
			return static_cast<_Ty>(v / *m_iter++);
		}

		_Iter m_iter;
	};

	
	

	template <typename _Ty = sample_t>
	struct clipping
	{
		clipping(_Ty const& max, _Ty const& min):m_max{ max }, m_min{ min } {}

		_Ty operator() (_Ty const& i) noexcept
		{
			auto v = i;
			if (v < m_min) v = m_min;
			if (v > m_max) v = m_max;
			return v;
		}

		_Ty m_min, m_max;
	};








	template <typename _Iter, typename _Ty = std::iterator_traits<_Iter>::value_type>
	auto make_multiplies_iter(_Iter iter) { return multiplies_iter<_Iter, _Ty>(iter); }
	template <typename _Iter, typename _Ty = std::iterator_traits<_Iter>::value_type>
	auto make_plus_iter(_Iter iter) { return plus_iter<_Iter, _Ty>(iter); }
	template <typename _Iter, typename _Ty = std::iterator_traits<_Iter>::value_type>
	auto make_minus_iter(_Iter iter) { return minus_iter<_Iter, _Ty>(iter); }
	template <typename _Iter, typename _Ty = std::iterator_traits<_Iter>::value_type>
	auto make_divides_iter(_Iter iter) { return divides_iter<_Iter, _Ty>(iter); }




}