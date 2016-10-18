#pragma once
#include "miqs_basictype.h"



#define MIQS_PTR_CIRCULAR_INCR_TAIL(CIRCULAR_TYPE) CIRCULAR_TYPE.tail = (CIRCULAR_TYPE.tail + 1 ==  CIRCULAR_TYPE.size) ? 0 : CIRCULAR_TYPE.tail + 1
#define MIQS_PTR_CIRCULAR_INCR_HEAD(CIRCULAR_TYPE) CIRCULAR_TYPE.head = ( CIRCULAR_TYPE.head + 1 == CIRCULAR_TYPE.size) ? 0 :  CIRCULAR_TYPE.head + 1
#define MIQS_PTR_CIRCULAR_DECR_TAIL(CIRCULAR_TYPE) CIRCULAR_TYPE.tail = (CIRCULAR_TYPE.tail == 0) ? CIRCULAR_TYPE.size - 1 : CIRCULAR_TYPE.tail - 1
#define MIQS_PTR_CIRCULAR_DECR_HEAD(CIRCULAR_TYPE)  CIRCULAR_TYPE.head = ( CIRCULAR_TYPE.head == 0) ? CIRCULAR_TYPE.size - 1 :  CIRCULAR_TYPE.head - 1
#define MIQS_PTR_CIRCULAR_AT_TAIL(CIRCULAR_TYPE) (CIRCULAR.ptr + CIRCULAR.tail)
#define MIQS_PTR_CIRCULAR_AT_HEAD(CIRCULAR_TYPE) (CIRCULAR.ptr + CIRCULAR.head)
#define MIQS_PTR_MODULAR(VALUE, SIZE) while(VALUE>=SIZE) VALUE-=SIZE;while(VALUE<0) VALUE+=SIZE
#define MIQS_PTR_MODULAR_UP(VALUE, SIZE) VALUE = (VALUE>=SIZE)?VALUE-SIZE:VALUE
#define MIQS_PTR_MODULAR_DOWN(VALUE, SIZE) VALUE = (VALUE<0)?VALUE+SIZE:VALUE

namespace miqs
{

	template <typename _Ty = sample_t>
	struct circular_data
	{
		typedef _Ty value_type;
		typedef _Ty* pointer;

		void increase_tail() { tail = (tail + 1 == size) ? 0 : tail + 1; }
		void increase_head() { head = (head + 1 == size) ? 0 : head + 1; }
		void decrease_tail() { tail = (tail == 0) ? size - 1 : tail - 1; }
		void decrease_head() { head = (head == 0) ? size - 1 : head - 1; }

		pointer at_head() { return ptr + head; }
		pointer at_tail() { return ptr + tail; }

		void rotate_to_origin(){
			ptr_rotate(ptr, ptr + head, ptr + size);
			head = 0;
			tail = size - 1;
		}

		size_t head, tail;
		value_type * ptr{ nullptr };
		size_t size{};
	};

	//------------------------------------------------------------
	template <typename _Ty = sample_t>
	class circular_adaptor
	{
	public:
		typedef _Ty value_type;
		typedef value_type* pointer;
		typedef circular_adaptor<value_type> self_type;

		circular_adaptor(pointer first, pointer last):
			m_ptr(first), m_size(std::distance(first, last)), offset_head(0), offset_tail(m_size - 1) {}

		circular_adaptor(pointer data, size_t size):
			m_ptr(data), m_size(size), offset_head(0), offset_tail(m_size - 1) {}


		void assign(pointer first, pointer last)
		{
			m_ptr = first; m_size = std::distance(first, last);
			reload();
		}

		void assign(pointer data, size_t size)
		{
			m_ptr = first; m_size = size;
			reload();
		}

		void resize(size_t size)
		{
			rotate_to_origin();
			m_size = size;
			reload();
		}



		void rotate_to_origin()
		{
			ptr_rotate(m_ptr, m_ptr + offset_head, m_ptr + m_size);
			offset_head = 0;
			offset_tail = m_size - 1;
		}

		pointer begin() noexcept { return m_ptr; }
		pointer end() noexcept { return m_ptr + this->m_size; }

		pointer rel_begin() noexcept { return m_ptr + offset_head; }
		pointer rel_end() noexcept { return m_ptr + offset_tail; }

		value_type& operator [] (int i) noexcept
		{
			return at_unchecked(i);
		}

		const value_type& operator [] (int i) const noexcept
		{
			return at_unchecked(i);
		}

		const value_type& at(int i) const noexcept
		{
			return at_unchecked(i);
		}

		value_type& at(int i) noexcept
		{
			return at_unchecked(i);
		}

		const value_type& back() const
		{
			return m_ptr[offset_tail];
		}

		value_type& back()
		{
			return m_ptr[offset_tail];
		}

		const value_type& front() const
		{
			return m_ptr[offset_head];
		}

		value_type& front()
		{
			return m_ptr[offset_head];
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
			reload();
			std::fill(m_ptr, m_ptr + m_size, 0);
		}

		void reload() noexcept
		{
			offset_head = 0;
			offset_tail = m_size - 1;
		}

		size_t head() const noexcept { return offset_head; }
		size_t tail() const noexcept { return offset_tail; }
		size_t size() const noexcept { return m_size; }
		pointer data() noexcept { return m_ptr; }


	private:
		value_type& at_unchecked(size_t index)
		{
			return m_ptr[abs_n_normalized(index)];
		}

		size_t abs_n_normalized(size_t index) const
		{
			size_t t = index + offset_head;
			t %= m_size;
			return t;
		}

		void increment_tail()
		{
			offset_tail = (offset_tail + 1 == m_size) ? 0 : offset_tail + 1;
		}

		void increment_head()
		{
			offset_head = (offset_head + 1 == m_size) ? 0 : offset_head + 1;
		}

		void decrement_tail()
		{
			offset_tail = (offset_tail == 0) ? m_size - 1 : offset_tail - 1;
		}

		void decrement_head()
		{
			offset_head = (offset_head == 0) ? m_size - 1 : offset_head - 1;
		}

		pointer m_ptr;
		size_t m_size;
	public:

		size_t offset_head;
		size_t offset_tail;
	};

}