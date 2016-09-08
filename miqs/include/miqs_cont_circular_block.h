#pragma once
#include "miqs_basictype.h"

#include "miqs_cont_circular_iterator.h"

namespace miqs
{

	template <int N>
	struct circular_sample_block
	{
		typedef ::std::allocator<sample_t>          allocator_type;
		typedef typename allocator_type::value_type value_type;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type size_type;
		typedef typename allocator_type::difference_type difference_type;

		typedef circular_sample_block<N> self_type;



		typedef circular_iterator<self_type> iterator;
		typedef const_circular_iterator<self_type> const_iterator;

		//typedef std::reverse_iterator<iterator> reverse_iterator;
		//typedef std::reverse_iterator<const_iterator> const_reverse_iterator;




		circular_sample_block() {}
		circular_sample_block(std::initializer_list<sample_t> l)
		{
			size_t i = 0;
			for (auto&& v : l)
			{
				if (i > N) break;
				m_data[i++] = v;
			}
		}


		iterator begin()  noexcept { return iterator(*this, 0); }
		iterator end()  noexcept { return iterator(*this, this->m_size); }

		const_iterator begin() const  noexcept { return iterator(*this, 0); }
		const_iterator end() const  noexcept { return iterator(*this, this->m_size); }

		//reverse_iterator rbegin()  noexcept { return reverse_iterator(end()); }
		//reverse_iterator rend()  noexcept { return reverse_iterator(begin()); }


		//const_reverse_iterator rbegin() const
		//{
		//	return const_reverse_iterator(end());
		//}
		//const_reverse_iterator rend() const
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

		sample_t* data() noexcept
		{
			return this->m_data;
		}

		const value_type& back() const
		{
			return m_data[m_tail];
		}

		value_type& back()
		{
			return m_data[m_tail];
		}

		const value_type& front() const
		{
			return m_data[m_head];
		}

		value_type& front()
		{
			return m_data[m_head];
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

		void reset() { for (size_t i{}; i < N; ++i) m_data[i] = 0; }
		size_t size() const { return N; }
	private:
		value_type& at_unchecked(size_t index)
		{
			return m_container[abs_n_normalized(index)];
		}

		size_t abs_n_normalized(size_t index) const
		{
			size_t t = index + m_head;
			t %= m_container.size();
			return t;
		}

		void increment_tail()
		{
			m_tail = (m_tail + 1 == N) ? 0 : m_tail + 1;
		}

		void increment_head()
		{
			++m_head;
			if (m_head == N) m_head = 0;
		}

		void decrement_tail()
		{
			m_tail = (m_tail == 0) ? N - 1 : m_tail - 1;
		}

		void decrement_head()
		{
			m_head = (m_head == 0) ? N - 1 : m_head - 1;
		}

		int m_head = 0;
		int m_tail = N - 1;
		sample_t m_data[N] = {};
	};
}