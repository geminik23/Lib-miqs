#pragma once

#include "miqs_headers.h"


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

	//==================================

	template <typename _Container>
	auto begin(_Container& container, int channel) -> decltype(container.begin(channel))
	{
		return container.begin(channel);
	}

	template <typename _Container>
	auto end(_Container& container, int channel) -> decltype(container.end(channel))
	{
		return container.end(channel);
	}

	template <typename _Container>
	auto rbegin(_Container& container, int channel) -> decltype(container.rbegin(channel))
	{
		return container.rbegin(channel);
	}

	template <typename _Container>
	auto rend(_Container& container, int channel) -> decltype(container.rend(channel))
	{
		return container.rend(channel);
	}



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
	auto* ptr_begin(_Cont& container) { return container.data(); }

	template <typename _Cont>
	auto* ptr_end(_Cont& container) { return container.data() + container.size(); }

	template <typename _Cont>
	auto* ptr_at(_Cont& container, size_t at) { return container.data() + at; }


	//============================

	template <typename _Ty=sample_t>
	class array{
	public:
		typedef _Ty value_type;

		array() : m_size{}, m_data{ nullptr } {}
		array(size_t size) : m_size{ size } { m_data = new value_type[size]; }
		array(array const&) = delete;
		array(array && other) : m_size{ other.m_size }, m_data{ other.m_data } { other.free(); }
		~array() { free(); }
		
		void free() { if (m_data) { delete[] m_data; m_data = nullptr; m_size = 0; } }

		value_type* data() noexcept { return m_data; }
		size_t size() const noexcept { return m_size; }

		operator value_type* () { return m_data; }

		void reset(size_t size = 0)
		{
			if (size == m_size) return;
			this->free();
			if (size == 0) return;
			this->m_size = size;
			this->m_data = new value_type[m_size];
		}
	private: 
		value_type * m_data;
		size_t m_size;
	};

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
			std::for_each(std::begin(list), std::end(list), [this, &i](auto&& v) {
				this->m_arrays[i].reset(v);
				++i;
			});
		}
		array_of_array(array_of_array const&) = delete;
		array_of_array(array_of_array&&) = delete;

		container_type& operator[](size_t n){ return m_arrays[n]; }

	private:
		container_type m_arrays[N];
	};



}