#pragma once

#include "miqs_basictype.h"

namespace miqs
{


	template <typename _Type>
	class multichannel_array_accessor
	{
	public:
		typedef base_allocator<_Type> Allocator;
		typedef multichannel_array_accessor<_Type> self_type;
		typedef typename Allocator::reference reference;
		typedef typename Allocator::const_reference const_reference;
		typedef typename Allocator::size_type size_type;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
		typedef typename Allocator::difference_type difference_type;
		typedef _Type value_type;



		class iterator
		{
		public:
			typedef iterator self_type;
			typedef _Type value_type;
			typedef _Type& reference;
			typedef _Type* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef int difference_type;

			typedef pointer _Unchecked_type;


			iterator(pointer ptr, size_type tc): ptr_(ptr), cont_ch_(tc) {}
			iterator(self_type const& other):ptr_(other.ptr_), cont_ch_(other.cont_ch_) {}
			self_type operator++() { self_type i = *this; ptr_ += cont_ch_; return i; }
			self_type operator++(int junk) { ptr_ += cont_ch_; return *this; }
			self_type operator--() { self_type i = *this; ptr_ -= cont_ch_; return i; }
			self_type operator--(int junk) { ptr_ -= cont_ch_; return *this; }
			self_type& operator+=(difference_type off) { ptr_ += off*cont_ch_; return *this; }
			self_type operator+(difference_type off) { self_type s = *this; s.ptr_ += off*cont_ch_; return s; }
			self_type& operator-=(difference_type off) { ptr_ -= off*cont_ch_; return *this; }
			self_type operator-(difference_type off) { self_type s = *this; s.ptr_ -= off*cont_ch_; return s;}
			difference_type operator-(self_type const& _right)const { return (this->ptr_ - _right.ptr_) / sizeof(T) / cont_ch_; }

			reference operator*() { return *ptr_; }
			pointer operator->() { return ptr_; }
			bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
			bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }

			self_type& operator =(self_type const& other)
			{ if (this == &other) return *this; this->cont_ch_ = other.cont_ch_; this->ptr_ = other.ptr_; return *this; }


		private:
			pointer ptr_;
			size_t cont_ch_;
		};

		class const_iterator
		{
		public:
			typedef const_iterator self_type;
			typedef _Type value_type;
			typedef _Type& reference;
			typedef _Type* pointer;
			typedef int difference_type;
			typedef std::forward_iterator_tag iterator_category;

			typedef pointer _Unchecked_type;

			const_iterator(pointer ptr, size_type tc): ptr_(ptr), cont_ch_(tc) {}
			const_iterator(self_type const& other):ptr_(other.ptr_), cont_ch_(other.cont_ch_) {}
			self_type operator++() { self_type i = *this; ptr_ += cont_ch_; return i; }
			self_type operator++(int junk) { ptr_ += cont_ch_; return *this; }
			self_type operator--() { self_type i = *this; ptr_ -= cont_ch_; return i; }
			self_type operator--(int junk) { ptr_ -= cont_ch_; return *this; }
			self_type& operator+=(difference_type off) { ptr_ += off*cont_ch_; return *this; }
			self_type operator+(difference_type off) { self_type s = *this; s.ptr_ += off*cont_ch_; return s; }
			self_type& operator-=(difference_type off) { ptr_ -= off*cont_ch_; return *this; }
			self_type operator-(difference_type off) { self_type s = *this; s.ptr_ -= off*cont_ch_; return s; }
			difference_type operator-(self_type const& _right)const { return (this->ptr_ - _right.ptr_) / sizeof(T) / cont_ch_; }


			const reference operator*() { return *ptr_; }
			const pointer operator->() { return ptr_; }
			bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
			bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }

			self_type& operator =(self_type const& other)
			{ if (this == &other) return *this; this->cont_ch_ = other.cont_ch_; this->ptr_ = other.ptr_; return *this; }

		private:
			pointer ptr_;
			size_t cont_ch_;
		};


		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;


		/* constructor */
		multichannel_array_accessor():m_array{ nullptr }, m_nch{ 0 }, m_size{ 0 } {}
		multichannel_array_accessor(pointer arr, size_type nchannel, size_type size):
			m_array{ arr }, m_nch{ nchannel }, m_size{ size } {}

		multichannel_array_accessor(self_type const& other):
			m_array(other.m_array), m_nch(other.m_nch), m_size(other.size) {}

		multichannel_array_accessor(self_type && other):
			m_array(other.m_array), m_nch(other.m_nch), m_size(other.size)
		{
			other.attach(nullptr, 0, 0);
		}


		bool operator !=(const self_type& dst)
		{
			if (this->m_array == dst.m_array &&
				this->m_nch == dst.m_nch &&
				this->m_size == dst.m_size)
				return false;
			return true;
		}

		bool operator ==(const self_type& dst)
		{
			if (this->m_array == dst.m_array &&
				this->m_nch == dst.m_nch &&
				this->m_size == dst.m_size)
				return true;
			return false;
		}

		void clear() noexcept
		{
			m_array = nullptr;
			m_nch = 0;
			m_size = 0;
		}

		void attach(pointer array, size_type nchannel, size_type size) noexcept
		{
			m_array = array;
			m_nch = nchannel;
			m_size = size;
		}

		const size_type channel() const noexcept { return m_nch; }
		const size_type frame_length() const noexcept { return m_size; }
		const size_type size() const noexcept { return m_size*m_nch; }
		pointer data() noexcept { return m_array; }


		const value_type& operator () (size_t c, size_t i) const
		{
			return m_array[i*this->m_nch + c];
		}

		value_type& operator () (size_t c, size_t i)
		{
			return m_array[i*this->m_nch + c];
		}

		const value_type& at(size_t c, size_t i) const
		{
			return m_array[i*this->m_nch + c];
		}

		value_type& at(size_t c, size_t i)
		{
			return m_array[i*this->m_nch + c];
		}




		iterator begin(size_t channel)
		{
			return  iterator(m_array + (channel), this->m_nch);
		}

		iterator end(size_t channel)
		{
			return  iterator(&m_array[m_size*m_nch + (channel)], this->m_nch);
		}

		const_iterator cbegin(size_t channel) const
		{
			return  const_iterator(m_array + (channel), this->m_nch);
		}

		const_iterator cend(size_t channel) const
		{
			return  const_iterator(&m_array[m_size*m_nch + (channel)], this->m_nch);
		}

		reverse_iterator rbegin(size_t channel)
		{
			return  std::make_reverse_iterator(iterator(&m_array[(m_size - 1)*m_nch + (channel)] + (channel), this->m_nch));
		}

		reverse_iterator rend(size_t channel)
		{
			return  std::make_reverse_iterator(iterator(m_array + (channel) - m_nch, this->m_nch));
		}

		const_reverse_iterator crbegin(size_t channel) const
		{
			return  std::make_reverse_iterator(const_iterator(&m_array[(m_size - 1)*m_nch + (channel)] + (channel), this->m_nch));
		}

		const_reverse_iterator crend(size_t channel) const
		{
			return  std::make_reverse_iterator(const_iterator(m_array + (channel ) - m_nch, this->m_nch));
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
		size_type m_nch;
		size_type m_size;

	};

}