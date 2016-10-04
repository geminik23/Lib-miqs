#pragma once

#include "miqs_basictype.h"

namespace miqs
{

	template<typename T>
	class array_accessor
	{
	public:
		typedef base_allocator<T> Allocator;
		typedef typename Allocator::reference reference;
		typedef typename Allocator::const_reference const_reference;
		typedef typename Allocator::size_type size_type;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
		typedef typename Allocator::difference_type difference_type;
		typedef T value_type;

		typedef array_accessor<T> self_type;

		class iterator
		{
		public:
			typedef iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef int difference_type;

			typedef pointer _Unchecked_type;


			iterator(pointer ptr): ptr_(ptr) {}
			self_type operator++() { self_type i = *this; ptr_++; return i; }
			self_type operator++(int junk) { ptr_++; return *this; }
			self_type operator--() { self_type i = *this; ptr_--; return i; }
			self_type operator--(int junk) { ptr_--; return *this; }
			self_type& operator+=(difference_type off) { ptr_ += off; return *this; }
			self_type operator+(difference_type off) { self_type s = *this; s.ptr_ += off; return s; }
			self_type& operator-=(difference_type off) { ptr_ -= off; return *this; }
			self_type operator-(difference_type off) { self_type s = *this; s.ptr_ += off; return s; }
			difference_type operator-(self_type const& _right)const { return (this->ptr_ - _right.ptr_) / sizeof(T); }

			reference operator*() { return *ptr_; }
			pointer operator->() { return ptr_; }
			bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
			bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
		private:
			pointer ptr_;
		};

		class const_iterator
		{
		public:
			typedef const_iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef int difference_type;
			typedef std::forward_iterator_tag iterator_category;

			typedef pointer _Unchecked_type;

			const_iterator(pointer ptr): ptr_(ptr) {}
			self_type operator++() { self_type i = *this; ptr_++; return i; }
			self_type operator++(int junk) { ptr_++; return *this; }
			self_type operator--() { self_type i = *this; ptr_--; return i; }
			self_type operator--(int junk) { ptr_--; return *this; }
			self_type& operator+=(difference_type off) { ptr_ += off; return *this; }
			self_type operator+(difference_type off) { self_type s = *this; s.ptr_ += off; return s; }
			self_type& operator-=(difference_type off) { ptr_ -= off; return *this; }
			self_type operator-(difference_type off) { self_type s = *this; s.ptr_ += off; return s; }
			difference_type operator-(self_type const& _right)const { return (this->ptr_ - _right.ptr_) / sizeof(T); }


			const reference operator*() { return *ptr_; }
			const pointer operator->() { return ptr_; }
			bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
			bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
		private:
			pointer ptr_;
		};

		array_accessor()
		{
			attach(nullptr, 0);
		}

		array_accessor(array_accessor const& other):
			m_array(other.m_array), m_size(other.size) {}

		array_accessor(array_accessor && other):
			m_array(other.m_array), m_size(other.size)
		{
			other.attach(nullptr, 0);
		}

		array_accessor(T* array, size_t size):
			m_array(array), m_size(size) {}

		void increase_front() noexcept
		{
			++this->m_size;
			this->m_array -= 1;
		}

		void increase_back() noexcept
		{
			++this->m_size;
		}

		void decrease_front() noexcept
		{
			--this->m_size;
			this->m_array += 1;
		}

		void decrease_back() noexcept
		{
			--this->m_size;
		}


		bool operator !=(const self_type& dst)
		{
			if (this->m_array == dst.m_array &&
				this->m_size == dst.m_size)
				return false;
			return true;
		}

		bool operator ==(const self_type& dst)
		{
			if (this->m_array == dst.m_array &&
				this->m_size == dst.m_size)
				return true;
			return false;
		}

		void clear()
		{
			m_array = nullptr;
			m_size = 0;
		}

		void attach(T* array, size_type size)
		{
			m_array = array;
			m_size = size;
		}

		size_type size()
		{
			return m_size;
		}

		const value_type& operator [] (unsigned i) const
		{
			return m_array[i];
		}

		value_type& operator [] (unsigned i)
		{
			return m_array[i];
		}

		const value_type& at(unsigned i) const
		{
			return m_array[i];
		}

		value_type& at(unsigned i)
		{
			return m_array[i];
		}

		value_type value_at(unsigned i) const
		{
			return m_array[i];
		}

		value_type * data()
		{
			return m_array;
		}

		iterator begin()
		{
			return  iterator(m_array);
		}

		iterator end()
		{
			return  iterator(&m_array[m_size]);
		}

		const_iterator begin() const
		{
			return  const_iterator(m_array);
		}

		const_iterator end() const
		{
			return  const_iterator(&m_array[m_size]);
		}

		explicit operator bool() noexcept
		{
			return m_array == nullptr;
		}

		pointer operator& () noexcept
		{
			return this->m_array;
		}

	protected:
		value_type* m_array;
		size_type m_size;
	};

	template <typename _Ty>
	auto make_array_accessor(_Ty* data, size_t size)
	{
		return array_accessor<_Ty>(data, size);
	}

	template <typename _Ty>
	auto make_array_accessor(miqs::array<_Ty>& data)
	{
		return array_accessor<_Ty>(data.data(), data.size());
	}

	template <typename _Ty>
	auto make_array_accessor(miqs::array<_Ty>& data, size_t size)
	{
		return array_accessor<_Ty>(data.data(), size);
	}



}