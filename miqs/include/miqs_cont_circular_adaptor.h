#pragma once

#include "miqs_basictype.h"

#include "miqs_cont_circular_iterator.h"

namespace miqs
{
	/* circular adapt container */
	template <typename _Cont, bool AdjustAfterRelease = true>
	class circular_adaptor
	{
	public:
		typedef typename _Cont::value_type value_type;
		typedef typename _Cont::pointer pointer;
		typedef typename _Cont::const_pointer const_pointer;
		typedef typename _Cont::reference reference;
		typedef typename _Cont::const_reference const_reference;
		typedef typename _Cont::size_type size_type;
		typedef typename _Cont::difference_type difference_type;

		typedef circular_adaptor<_Cont, AdjustAfterRelease> self_type;


		typedef circular_iterator<self_type> iterator;
		typedef const_circular_iterator<self_type> const_iterator;

		/*typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
*/


		circular_adaptor(_Cont& container):
			m_container(container),
			m_head{ 0 },
			m_size{ m_container.size() }
		{
			m_tail = m_size - 1;
		}

		circular_adaptor(_Cont& container, size_t size):
			m_container(container),
			m_head{ 0 },
			m_size{ size }
		{
			m_tail = m_size - 1;
		}

		virtual ~circular_adaptor()
		{
			if (AdjustAfterRelease) rotate_to_origin();
		}

		void rotate_to_origin()
		{
			std::rotate(std::begin(m_container), std::begin(m_container) + m_head, std::end(m_container));
			m_head = 0;
			m_tail = m_size - 1;
		}

		iterator begin()  noexcept { return iterator(*this,0); }
		iterator end()  noexcept{return iterator(*this, this->m_size);}

		const_iterator begin() const  noexcept { return iterator(*this, 0); }
		const_iterator end() const  noexcept { return iterator(*this, this->m_size); }

		//reverse_iterator rbegin()  noexcept { return reverse_iterator(end()); }
		//reverse_iterator rend()  noexcept { return reverse_iterator(begin()); }

		//const_reverse_iterator rbegin() const noexcept
		//{
		//	return const_reverse_iterator(end());
		//}
		//const_reverse_iterator rend() const noexcept
		//{
		//	return const_reverse_iterator(begin());
		//}

		value_type& operator [] (unsigned i) noexcept
		{
			return at_unchecked(i);
		}

		const value_type& operator [] (unsigned i) const noexcept
		{
			return at_unchecked(i);
		}

		const value_type& at(unsigned i) const noexcept
		{
			return at_unchecked(i);
		}

		value_type& at(unsigned i) noexcept
		{
			return at_unchecked(i);
		}

		_Cont& _Get_container() const noexcept
		{
			return m_container;
		}

		const value_type& back() const
		{
			return m_container[m_tail];
		}

		value_type& back()
		{
			return m_container[m_tail];
		}

		const value_type& front() const
		{
			return m_container[m_head];
		}

		value_type& front()
		{
			return m_container[m_head];
		}

		self_type &operator--()
		{
			this->increment_head();
			this->increment_tail();
			return *this;
		}
		self_type operator--(int)
		{
			self_type tmp(*this);
			--(*this);
			return tmp;
		}


		self_type &operator++()
		{
			this->decrement_head();
			this->decrement_tail();
			return *this;
		}
		self_type operator++(int)
		{
			self_type tmp(*this);
			++(*this);
			return tmp;
		}

		void reset() noexcept
		{
			reload_container();
			std::fill(std::begin(m_container), std::end(m_container), 0);
		}

		void reload_container() noexcept
		{
			m_head = 0;
			m_tail = m_size - 1;
		}

		// this return changed size
		size_t set_size(size_t size) noexcept
		{
			if (m_size == size) return m_size;
			auto newsize = (this->m_container.size() < size) ? this->m_container.size() : size;
			
			// calculate new tail value
			int value = newsize - m_size;
			auto result = (static_cast<int>(m_tail) + value)%newsize;
			if (result < 0) result = newsize;
			m_tail = static_cast<size_t>(result);
			//
			
			m_size = newsize;
			return m_size;
		}
		size_t size() const noexcept { return m_size; }

	private:
		value_type& at_unchecked(size_t index)
		{
			return m_container[abs_n_normalized(index)];
		}

		size_t abs_n_normalized(size_t index) const
		{
			size_t t = index + m_head;
			t %= m_size;
			return t;
		}

		void increment_tail()
		{
			m_tail = (m_tail + 1 == m_size) ? 0 : m_tail + 1;
		}

		void increment_head()
		{
			// precondition: !empty()
			++m_head;
			if (m_head == m_size) m_head = 0;
		}

		void decrement_tail()
		{
			m_tail = (m_tail == 0) ? m_size - 1 : m_tail - 1;
		}

		void decrement_head()
		{
			m_head = (m_head == 0) ? m_size - 1 : m_head - 1;
		}

		friend class circular_iterator<self_type>;
		friend class const_circular_iterator<self_type>;

		static miqs::sample_t end_value;
		// properties
		_Cont& m_container;
		size_t m_head;
		size_t m_tail;
		size_t m_size;
	};

	template <typename _Cont, bool AdjustAfterRelease>
	sample_t circular_adaptor<_Cont, AdjustAfterRelease>::end_value = 0.0;

}