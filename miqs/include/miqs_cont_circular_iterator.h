#pragma once
#include "miqs_basictype.h"

namespace miqs
{

	// circular adaptor's iterator
	template <typename ContainerType>
	class circular_iterator
	{
	public:
		typedef ContainerType container_type;
		typedef circular_iterator self_type;
		typedef typename container_type::value_type value_type;
		typedef typename container_type::reference reference;
		typedef typename container_type::pointer pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef int difference_type;

		typedef pointer _Unchecked_type;


		circular_iterator(container_type& cont, int cur_idx): m_cont(cont), m_idx(cur_idx) {}
		circular_iterator(self_type const& other):m_cont{ other.m_cont }, m_idx{other.m_idx}{}


		self_type& operator=(self_type const& other) noexcept
		{ 
			this->m_cont = other.m_cont; 
			this->m_idx = other.m_idx; 
			return *this; 
		}

		self_type operator++(int) noexcept { self_type i = *this; this->operator++(); return i; }
		self_type operator++() noexcept { check_index(1); return *this; }
		self_type operator--(int) noexcept { self_type i = *this; this->operator--(); return i; }
		self_type operator--() noexcept { check_index(-1); return *this; }

		self_type& operator+=(difference_type off) { check_index(off); return *this; }
		self_type operator+(difference_type off) { self_type s = *this; return (s += off); }
		self_type& operator-=(difference_type off) { check_index(off); return *this; }
		self_type operator-(difference_type off) { self_type s = *this; return (s -= off); }

		difference_type operator-(self_type const& rh)const { return (this->m_idx - rh.m_idx); }

		


		reference operator*() { if (_is_ended()) return m_cont.end_value; return m_cont[_normalized(m_idx)]; }
		pointer operator->() { if (_is_ended()) return &m_cont.end_value; return &m_cont[_normalized(m_idx)]; }

		bool operator==(const self_type& rhs) { return m_idx == rhs.m_idx; }
		bool operator!=(const self_type& rhs) { return m_idx != rhs.m_idx; }
	protected:
		size_t _normalized(int idx)
		{
			idx %= m_cont.size();
			if (idx < 0) idx += m_cont.size();
			return static_cast<size_t>(idx);
		}

		bool _is_ended()
		{
			return m_idx >= static_cast<int>(m_cont.size());
		}


		void check_index(int dif)
		{
			if (dif>0 && _is_ended()) return;
			if (m_idx + dif >= static_cast<int>(m_cont.size()))
			{
				m_idx = m_cont.size();
				return;
			}

			m_idx += dif;
		}

		container_type& m_cont;
		int m_idx;
	};


	template <typename ContainerType>
	class const_circular_iterator
	{
	public:
		typedef ContainerType container_type;
		typedef container_type const_container_type;
		typedef const_circular_iterator self_type;
		typedef typename container_type::value_type value_type;
		typedef typename container_type::reference reference;
		typedef typename container_type::pointer pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef int difference_type;

		typedef pointer _Unchecked_type;


		const_circular_iterator(container_type& cont, int cur_idx): m_cont(cont), m_idx(cur_idx) {}
		self_type operator++(int) noexcept { self_type i = *this; this->operator++(); return i; }
		self_type operator++() noexcept { check_index(1); return *this; }
		self_type operator--(int) noexcept { self_type i = *this; this->operator--(); return i; }
		self_type operator--() noexcept { check_index(-1); return *this; }

		self_type& operator+=(difference_type off) { check_index(off); return *this; }
		self_type operator+(difference_type off) { self_type s = *this; return (s += off); }
		self_type& operator-=(difference_type off) { check_index(off); return *this; }
		self_type operator-(difference_type off) { self_type s = *this; return (s -= off); }

		difference_type operator-(self_type const& rh)const { return (this->m_idx - rh.m_idx); }


		self_type& operator=(self_type const& other) { this->m_cont = other.m_cont; this->m_idx = other.m_idx; return *this; }

		const reference operator*() { if (_is_ended()) return m_cont.end_value; return m_cont[_normalized(m_idx)]; }
		const pointer operator->() { if (_is_ended()) return &m_cont.end_value; return &m_cont[_normalized(m_idx)]; }
		bool operator==(const self_type& rhs) { return m_idx == rhs.m_idx; }
		bool operator!=(const self_type& rhs) { return m_idx != rhs.m_idx; }

	private:
		size_t _normalized(int idx)
		{
			idx %= m_cont.size();
			if (idx < 0) idx += m_cont.size();
			return static_cast<size_t>(idx);
		}

		bool _is_ended()
		{
			return m_idx == static_cast<int>(m_cont.size());
		}


		void check_index(int dif)
		{
			if (_is_ended()) return;
			if (m_idx + dif >= static_cast<int>(m_cont.size()))
			{
				m_idx = m_cont.size();
				return;
			}
			m_idx += dif;
		}

		container_type& m_cont;
		int m_idx;
	};


}