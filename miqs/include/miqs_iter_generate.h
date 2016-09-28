#pragma once
#include "miqs_basictype.h"


namespace miqs
{
	template <typename _Gen, typename _Type = _Gen::result_type>
	struct generate_iterator
	{
		typedef _Gen generator_type;
		typedef generate_iterator<_Gen, _Type> self_type;
		typedef typename _Type value_type;
		typedef typename _Type& reference;
		typedef typename _Type* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef int difference_type;

		typedef pointer _Unchecked_type;

		generate_iterator(generator_type& generator):
			m_generator(generator), m_first{ true }, m_currentValue{} {}

		generate_iterator(self_type const& other):
			m_generator{ other.m_generator },
			m_first{ other.m_first },
			m_currentValue{ other.m_currentValue }
		{}


		self_type& operator=(self_type const& other) noexcept
		{
			this->m_generator = other.m_generator;
			this->m_first = other.m_first;
			this->m_currentValue = other.m_currentValue;
			return *this;
		}

		self_type operator++(int) noexcept { check(); self_type i = *this; this->operator++(); return i; }
		self_type operator++() noexcept { next(); return *this; }

		reference operator*() { check(); return m_currentValue; }
		pointer operator->() { check(); return &m_currentValue; }

		bool operator==(const self_type& rhs) { return (m_generator == rhs.m_generator) && (m_currentValue == rhs.m_currentValue); }
		bool operator!=(const self_type& rhs) { return (m_generator != rhs.m_generator) || (m_currentValue != rhs.m_currentValue); }

	protected:

		void check() { if (m_first) next(); }
		void next()
		{
			m_currentValue = m_generator();
			if (m_first) m_first = false;
		}

		generator_type& m_generator;
		value_type m_currentValue;
		bool m_first;

	};


	template <typename _Gen, typename _Type = _Gen::result_type>
	generate_iterator<_Gen, _Type> make_generate_iterator(_Gen& generator)
	{
		return generate_iterator<_Gen, _Type>(std::ref(generator));
	}
}