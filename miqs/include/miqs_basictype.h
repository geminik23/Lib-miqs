#pragma once

#include "miqs_headers.h"
#include "miqs_macros.h"

namespace miqs
{

	typedef std::int8_t sbyte_t;
	typedef std::int16_t int16_t;
	typedef std::int32_t int32_t;
	typedef std::int64_t int64_t;

	typedef std::uint8_t byte_t;
	typedef std::uint16_t uint16_t;
	typedef std::uint32_t uint32_t;
	typedef std::uint64_t uint64_t;

	typedef float float32_t;
	typedef double float64_t;

	typedef double sample_t;
	typedef sample_t * sample_ptr;

	enum class type_format
	{
		int_8bit = 0,
		int_16bit,
		int_24bit,
		int_32bit,
		float_32bit,
		float_64bit
	};


	/*	allocator	*/
	template<class T> struct base_allocator
	{
	public:
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
	};


	//=================================
	/** base transform arg1*/
	template <typename _Ty>
	struct _base_trans_arg1
	{
		typedef _Ty result_type;
		typedef _Ty argument_type;
	};

	/** this is base functor. need phasor_index */
	template <size_t m_size>
	struct _base_idx_gen_fixed
	{
		typedef sample_t result_type;
		typedef size_t argument_type;

		size_t size() const noexcept { return m_size; }
	};

	/** this is base functor. need phasor_index */
	struct _base_idx_gen
	{
		typedef sample_t result_type;
		typedef size_t argument_type;
	};

	struct _base_idx_gen_size: _base_idx_gen
	{
		_base_idx_gen_size(size_t size):m_size(size) {}
		size_t size() const noexcept { return m_size; }
		void set_size(size_t size) noexcept { m_size = size; }
	protected:
		size_t m_size;
	};



	//============================function
	typedef enum func_parameter_position
	{
		LEFT = 1, RIGHT, ARG1 = LEFT, ARG2 = RIGHT, ARG, RESULT
	} func_param_pos;


	template <typename _R, typename _A1, typename _A2>
	struct _type_combine_func_a2
	{
		typedef typename _A1 first_argument_type;
		typedef typename _A2 second_argument_type;
		typedef typename _R result_type;
	};

	template <typename _R, typename _A>
	struct _type_combine_func_a1
	{
		typedef typename _A argument_type;
		typedef typename _R result_type;
	};

	
	template <typename _Ty>
	struct _base_value_type
	{
		typedef _Ty value_type;
	};

	template <typename _Ty>
	struct _base_pointer_type
	{
		typedef _Ty value_type;
		typedef value_type * pointer;
	};




	template <typename _Wrapper>
	auto& container(_Wrapper& wrapper)
	{
		return wrapper._Get_container();
	}
	// get data from Container
	template <typename _Ty, typename _Cont>
	_Ty* data(_Cont& container)
	{
		return reinterpret_cast<_Ty*>(container.data());
	}

	template <typename _Cont>
	typename _Cont::value_type* data(_Cont& container)
	{
		return container.data();
	}



	template <typename _Cont>
	inline auto* ptr_2begin(_Cont& container) { return reinterpret_cast<std::pair<typename _Cont::value_type, typename _Cont::value_type>*>(container.data()); }

	template <typename _Cont>
	inline auto* ptr_2end(_Cont& container) { return reinterpret_cast<std::pair<typename _Cont::value_type, typename _Cont::value_type>*>(container.data() + container.size()); }


	template <typename _Cont>
	inline auto* ptr_begin(_Cont& container) { return container.data(); }

	template <typename _Cont>
	inline auto* ptr_end(_Cont& container) { return container.data() + container.size(); }




	template <typename _Cont>
	inline auto* ptr_rev_at(_Cont& container, int off) { return container.data() + ((int)container.size() - off); }

	template <typename _Cont>
	inline auto* ptr_at(_Cont& container, int at) { return container.data() + at; }



	//============================

	template <typename _Ty = sample_t>
	class array{
	public:

		typedef _Ty value_type;
		typedef array<value_type> self_type;
		typedef base_allocator<value_type> allocator_type;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type size_type;
		typedef typename allocator_type::difference_type difference_type;


		array() : m_size{}, m_ptr{ nullptr } {}
		array(size_t size) : m_size{ size } { m_ptr = new value_type[size]; }
		array(size_t size, value_type const& value) : m_size{ size }, m_ptr{ nullptr }
		{
			if (m_size == 0) return;
			m_ptr = new value_type[size];
			for (size_t i{}; i < size; ++i) m_ptr[i] = value;
		}


		array(std::initializer_list<value_type> list) : m_size{ list.size() }, m_ptr{ nullptr }
		{
			if (m_size == 0) return;
			m_ptr = new value_type[m_size];
			size_t i{};
			for (auto&& v : list)
			{
				m_ptr[i++] = v;
			}
		}

		array(self_type const&other)
		{
			this->reset(other.size());
			for (size_t i{}; i < m_size; ++i) m_ptr[i] = other.m_ptr[i];
		}
		array(self_type && other) : m_size{ other.m_size }, m_ptr{ other.m_ptr } { other.free(); }
		~array() { free(); }



		void free() { if (m_ptr) { delete[] m_ptr; m_ptr = nullptr; m_size = 0; } }

		value_type* data() noexcept { return m_ptr; }
		size_t size() const noexcept { return m_size; }
		

		void resize(size_t nsize)
		{
			if (nsize == m_size &&nsize == 0) return;
			if (nsize < m_size) { this->m_size = nsize; return; }

			auto * temp = m_ptr;
			auto olds = std::min(nsize, m_size);

			this->m_size = nsize;
			this->m_ptr = new value_type[m_size];
			for (size_t i{}; i < olds; ++i) m_ptr[i] = temp[i];
			delete[] temp;
		}


		pointer begin() { return m_ptr; }
		pointer end() { return m_ptr + m_size; }
		const_pointer begin() const { return m_ptr; }
		const_pointer end() const { return m_ptr + m_size; }

		value_type& operator[](int i) { return m_ptr[i]; }
		value_type operator[](int i) const { return m_ptr[i]; }

		operator pointer () { return m_ptr; }

		void reset(size_t size = 0)
		{
			if (size == m_size) return;
			if (size < m_size) { this->m_size = size; return; }
			this->free();
			if (size == 0) return;
			this->m_size = size;
			this->m_ptr = new value_type[m_size];
		}

		void reset(size_t size, value_type&& v)
		{
			reset(size);
			if (size == 0) return;
			for (size_t i{}; i < size; ++i) m_ptr[i] = v;
		}


	private:
		value_type * m_ptr;
		size_t m_size;
	};

	//----------------------
	template <typename _Ty = sample_t>
	class array_ref
	{
	public:

		typedef _Ty value_type;
		typedef array<value_type> self_type;
		typedef base_allocator<value_type> allocator_type;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type size_type;
		typedef typename allocator_type::difference_type difference_type;

		array_ref() = default;
		array_ref(pointer data, size_t size):m_ptr(data), m_size(size){}

		size_t size() const noexcept { return m_size; }

		void assign(pointer data, size_t size) { m_ptr = data; m_size = size; }
		pointer begin() const noexcept { return m_ptr; }
		pointer end() const noexcept { return m_ptr + m_size; }
		const_pointer cbegin() const noexcept { return m_ptr; }
		const_pointer cend() const noexcept { return m_ptr + m_size; }

		pointer data()const noexcept { return m_ptr; }

		operator pointer () { return m_ptr; }

	private:
		pointer m_ptr{ nullptr };
		size_t m_size{};
	};



	//===================array of array
	template <typename _Ty, int N>
	class array_of_array
	{
	public:
		typedef _Ty value_type;
		typedef miqs::array<value_type> container_type;
		array_of_array() = default;
		array_of_array(std::initializer_list<size_t> list)
		{
			size_t i{};
			size_t last{};
			std::for_each(std::begin(list), std::end(list), [this, &i, &last](auto&& v) {
				this->m_arrays[i].reset(v);
				last = v;
				++i;
			});
			for (; i < N;++i)
			{
				this->m_arrays[i].reset(last);
			}
		}
		array_of_array(array_of_array const&) = delete;
		array_of_array(array_of_array&&) = delete;

		container_type& operator[](size_t n) { return m_arrays[n]; }

	private:
		container_type m_arrays[N];
	};


	//==================
	template <typename _Ty, int N>
	class array_ref_n
	{
	public:
		typedef ::std::allocator<_Ty>          allocator_type;

		typedef typename allocator_type::value_type        value_type;
		typedef typename allocator_type::pointer           pointer;
		typedef typename allocator_type::const_pointer     const_pointer;
		typedef typename allocator_type::reference         reference;
		typedef typename allocator_type::const_reference   const_reference;

		typedef typename allocator_type::size_type         size_type;
		typedef typename allocator_type::difference_type   difference_type;


		typedef pointer iterator;
		typedef const iterator const_iterator;

		array_ref_n() = default;
		array_ref_n(pointer data):m_ptr{ data } {}

		void assign(pointer data) { m_ptr = data; }

		pointer data()  noexcept
		{
			return m_ptr;
		}

		const pointer operator [] (unsigned i) const
		{
			return m_ptr[i];
		}

		value_type& operator [] (unsigned i)
		{
			return m_ptr[i];
		}

		iterator begin()
		{
			return &m_ptr[0];
		}

		iterator end()
		{
			return &m_ptr[N];
		}

		const_iterator begin() const
		{
			return &m_ptr[0];
		}

		const_iterator end() const
		{
			return &m_ptr[N];
		}

		const value_type& back() const
		{
			return m_ptr[N - 1];
		}

		value_type& back()
		{
			return m_ptr[N - 1];
		}

		const value_type& front() const
		{
			return m_ptr[0];
		}

		value_type& front()
		{
			return m_ptr[0];
		}

		bool is_capable(size_t size)
		{
			return size <= N;
		}

		void reset() { for (int i = 0; i < N; ++i) m_ptr[i] = 0; }
		size_t size() const { return N; }


	private:
		pointer m_ptr{nullptr};
	};


	//========================pointer
	template <typename _Ty = sample_t>
	struct ptr_proxy_interleaved
	{

		typedef _Ty value_type;
		typedef value_type * pointer;
		typedef ptr_proxy_interleaved<value_type> self_type;

		pointer ptr{ nullptr };
		int strip;
		ptr_proxy_interleaved(pointer p, int s):ptr{ p }, strip{ s } {}

		inline self_type& operator++() { ptr += strip; return *this; }
		inline self_type operator++(int) { self_type t = *this; ptr += strip; return t; }

		inline self_type& operator+=(int v) { ptr += strip*v; return *this; }


		inline pointer begin(int n = 0) { return ptr + n; }
		inline pointer at(int count, int n = 0) { return ptr + count*strip + n; }


		inline operator pointer() { return ptr; }
	};



	//==================================for_time_events

	struct none {};

	template <typename _Ty>
	struct type_struct
	{
		type_struct() = default;
		type_struct(_Ty const& v):value{ v } {}
		_Ty value;
	};


	template <typename _Ty = double, typename _Content = none>
	struct event_point:_Content
	{
		typedef _Ty value_type;
		typedef event_point<value_type, _Content> self_type;

		event_point() = default;
		template <typename ... Args>
		event_point(value_type t, Args... args): point{ t }, _Content(std::forward<Args>(args)...) {}

		value_type point;
	};




	template <typename _PtTy, typename _Ty>
	using primitive_event_point = event_point<_PtTy, type_struct<_Ty>>;








}