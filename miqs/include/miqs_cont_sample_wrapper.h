#pragma once

#include "miqs_basictype.h"

namespace miqs
{

	template <int N>
	struct sample_wrapper
	{
		typedef ::std::allocator<sample_t>          allocator_type;

		typedef typename allocator_type::value_type        value_type;
		typedef typename allocator_type::pointer           pointer;
		typedef typename allocator_type::const_pointer     const_pointer;
		typedef typename allocator_type::reference         reference;
		typedef typename allocator_type::const_reference   const_reference;

		typedef typename allocator_type::size_type         size_type;
		typedef typename allocator_type::difference_type   difference_type;


		typedef sample_t* iterator;
		typedef const sample_t* const_iterator;
		sample_wrapper(sample_t * data):_s{ data } {}

		sample_t * data()  noexcept
		{
			return _s;
		}

		const sample_t& operator [] (unsigned i) const
		{
			return _s[i];
		}

		sample_t& operator [] (unsigned i)
		{
			return _s[i];
		}

		iterator begin()
		{
			return &_s[0];
		}

		iterator end()
		{
			return &_s[N];
		}

		const_iterator begin() const
		{
			return &_s[0];
		}

		const_iterator end() const
		{
			return &_s[N];
		}

		const sample_t& back() const
		{
			return _s[N - 1];
		}

		sample_t& back()
		{
			return _s[N - 1];
		}

		const sample_t& front() const
		{
			return _s[0];
		}

		sample_t& front()
		{
			return _s[0];
		}

		bool is_capable(size_t size)
		{
			return size <= N;
		}

		void reset() { for (int i = 0; i < N; ++i) _s[i] = 0; }
		size_t size() const { return N; }
	private:
		sample_t * _s;
	};

}