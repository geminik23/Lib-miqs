#pragma once



#include "macros.h"


#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <cfloat>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <ctype.h>
#include <complex>
#include <iostream>

#include <streambuf>
#include <sstream>

#include <stack>
#include <deque>
#include <string>
#include <vector>
#include <list>
#include <cinttypes>

#include <exception>

#include <algorithm>
#include <numeric>
#include <random>
#include <functional>
#include <chrono>
#include <iterator>
#include <memory>

namespace miqs
{

	typedef std::int8_t sbyte_t;
	typedef std::uint8_t byte_t;

	typedef float float32_t;
	typedef double float64_t;

	typedef double sample_t;
	typedef sample_t * sample_ptr;



	////=================================
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




	template <typename _Cont> inline auto* ptr_2begin(_Cont& container)
	{ return reinterpret_cast<std::pair<typename _Cont::value_type, typename _Cont::value_type>*>(container.data()); }
	template <typename _Cont> inline auto* ptr_2end(_Cont& container)
	{ return reinterpret_cast<std::pair<typename _Cont::value_type, typename _Cont::value_type>*>(container.data() + container.size()); }

	template <typename _Cont> inline auto* ptr_begin(_Cont& container) { return container.data(); }
	template <typename _Cont> inline auto* ptr_end(_Cont& container) { return container.data() + container.size(); }

	template <typename _Cont> inline auto* ptr_rev_at(_Cont& container, int off) { return container.data() + ((int)container.size() - off); }
	template <typename _Cont> inline auto* ptr_at(_Cont& container, int at) { return container.data() + at; }


	//==================================================================

	template <typename _Ty = sample_t>
	class array{
	public:
		typedef _Ty value_type;
		typedef array<value_type> self_type;
		typedef ::std::allocator<_Ty> allocator_type;
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
			if (this->m_size != 0) for (size_t i{}; i < m_size; ++i) m_ptr[i] = other.m_ptr[i];
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
			if (!m_ptr)this->free();
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
		value_type * m_ptr{ nullptr };
		size_t m_size{ 0 };
	};


	//----------------------
	template <typename _Ty = sample_t>
	class array_ref
	{
	public:

		typedef _Ty value_type;
		typedef array<value_type> self_type;
		typedef std::allocator<value_type> allocator_type;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type size_type;
		typedef typename allocator_type::difference_type difference_type;

		array_ref() = default;
		array_ref(pointer data, size_t size):m_ptr(data), m_size(size) {}

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

	//------------------
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
		pointer data()  noexcept { return m_ptr; }
		const pointer operator [] (unsigned i) const { return m_ptr[i]; }
		value_type& operator [] (unsigned i) { return m_ptr[i]; }
		iterator begin() { return &m_ptr[0]; }
		iterator end() { return &m_ptr[N]; }
		const_iterator begin() const { return &m_ptr[0]; }
		const_iterator end() const { return &m_ptr[N]; }
		const value_type& back() const { return m_ptr[N - 1]; }
		value_type& back() { return m_ptr[N - 1]; }
		const value_type& front() const { return m_ptr[0]; }
		value_type& front() { return m_ptr[0]; }
		bool is_capable(size_t size) { return size <= N; }

		void reset() { for (int i = 0; i < N; ++i) m_ptr[i] = 0; }
		size_t size() const { return N; }
	private:
		pointer m_ptr{ nullptr };
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
			for (; i < N; ++i)
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


	//====================================circular_adaptor
	template <typename _Ty = sample_t>
	struct circular_data:_base_pointer_type<_Ty>
	{
		void increase_tail() { tail = (tail + 1 == size) ? 0 : tail + 1; }
		void increase_head() { head = (head + 1 == size) ? 0 : head + 1; }
		void decrease_tail() { tail = (tail == 0) ? size - 1 : tail - 1; }
		void decrease_head() { head = (head == 0) ? size - 1 : head - 1; }

		pointer at_head() { return ptr + head; }
		pointer at_tail() { return ptr + tail; }

		void rotate_to_origin()
		{
			ptr_rotate(ptr, ptr + head, ptr + size);
			head = 0;
			tail = size - 1;
		}

		size_t head, tail;
		value_type * ptr{ nullptr };
		size_t size{};
	};



	////==================================for_event
	//struct none {};

	//template <typename _Ty>
	//struct type_struct
	//{
	//	type_struct() = default;
	//	type_struct(_Ty const& v):value{ v } {}
	//	_Ty value;
	//};


	//template <typename _Ty = double, typename _Content = none>
	//struct event_point:_Content
	//{
	//	typedef _Ty value_type;
	//	typedef event_point<value_type, _Content> self_type;

	//	event_point() = default;
	//	template <typename ... Args>
	//	event_point(value_type t, Args... args): point{ t }, _Content(std::forward<Args>(args)...) {}

	//	value_type point;
	//};

	//template <typename _PtTy, typename _Ty>
	//using primitive_event_point = event_point<_PtTy, type_struct<_Ty>>;





	//======Utils
	static union
	{
		int s = 1;
		char c[sizeof(int)];
	} __big_endian;

	// check : is big endian system?
	template <typename None = miqs::none>
	struct is_big_endian_system
	{
		static const bool value;
	};
	template <typename None>
	const bool is_big_endian_system<None>::value = (__big_endian.c[0] != 1);


	template <int N>
	inline void swap_bytes(byte_t *p)
	{
		register byte_t t;
		for (size_t i{}; i < N / 2; ++i)
		{
			t = p[i];
			p[i] = p[N - 1 - i];
			p[N - 1 - i] = t;
		}
	}





	//==========================================phase
	template <typename _Ty = double>
	struct phase_data: _base_value_type<_Ty>
	{
		phase_data() = default;
		phase_data(value_type p, value_type inc):phase{ p }, step{ inc }{}

		value_type phase{};
		value_type step{};
	};

	template <typename _Ty = double>
	struct phase_info: phase_data<_Ty>
	{
		typedef _Ty value_type;
		typedef phase_info<value_type> self_type;

		phase_info() = default;
		phase_info(value_type p, value_type inc):phase_data{ p, inc } {}

		void reset() noexcept { this->phase = 0; }
		inline void next() { this->phase += step; }
		operator value_type() { return this->phase; }

		self_type& operator+=(value_type v) { this->phase += v; return *this; }
		self_type& operator=(value_type v) { this->phase = v; return *this; }
	};

	template <typename _PhaseInfo>
	struct phase_helper
	{
		typedef _PhaseInfo info_type;
		typedef typename info_type::value_type value_type;
		typedef phase_helper<info_type> self_type;

		phase_helper(info_type& pinfo):phase{ pinfo }, samplerate{ 44100 } {}
		//void reset() noexcept { phase.reset(); }
		//inline void next() noexcept { this->phase.phase += this->phase.step; }
		//operator value_type() { return this->phase; }

		void set_samplerate(double sr) noexcept { samplerate = sr; }
		void set_freq(double freq) { phase.step = freq* miqs::two_pi / samplerate; }
		void set_info(double freq, double sr) { this->samplerate = sr; phase.step = freq* miqs::two_pi / samplerate; }

		info_type& phase;
		double samplerate;
	};

	template <typename _PhaseInfo>
	phase_helper<_PhaseInfo> make_phase_helper(_PhaseInfo & info)
	{
		return phase_helper<_PhaseInfo>(info);
	}


	template <typename _PhaseInfo>
	struct phase_operator
	{
		typedef _PhaseInfo info_type;
		typedef typename info_type::value_type value_type;

		phase_operator(info_type& pinfo):phase{ pinfo } {}

		void reset() noexcept { phase.reset(); }
		inline void next() noexcept { this->phase.phase += this->phase.step; }
		operator value_type() { return this->phase; }

		value_type operator()() { value_type t = phase.phase; phase.phase += phase.step; return t; }

		info_type& phase;
	};


	template <typename _PhaseInfo>
	struct phase_pi_modular
	{
		typedef _PhaseInfo info_type;
		typedef typename info_type::value_type value_type;

		phase_pi_modular(info_type& pinfo):phase{ pinfo } {}

		void reset() noexcept { phase.reset(); }
		inline void next() noexcept { this->phase.phase += this->phase.step; bound_check(); }
		operator value_type() { return this->phase; }
		void bound_check() { while (phase.phase >= miqs::two_pi) phase.phase -= miqs::two_pi; while (phase.phase < 0) phase.phase += miqs::two_pi; }

		value_type operator()()
		{
			value_type t = phase.phase;
			phase.phase += phase.step;
			bound_check();
			return t;
		}

		info_type& phase;
	};


	template <typename _PhaseInfo>
	struct phase_modular
	{
		typedef _PhaseInfo info_type;
		typedef typename info_type::value_type value_type;

		phase_modular(info_type& pinfo, value_type bound_):phase{ pinfo }, bound{ bound_ } {}

		void reset() noexcept { phase.reset(); }
		void next() noexcept { this->phase.phase += this->phase.step; this->bound_check(); }
		void bound_check() { while (phase.phase >= bound)	phase.phase -= bound; while (phase.phase < 0.0) phase.phase += bound; }

		operator value_type() { return this->phase; }

		value_type operator()()
		{
			value_type t = phase.phase;
			phase.phase += phase.step;
			this->bound_check();
			return t;
		}

		info_type& phase;
		value_type bound;
	};













	//==========================================Interpolation

	namespace interpolation
	{
		//
		/* interpolation
		_Interpolator::REQUIRE_PRE_SIZE     x[n-N]
		_Interpolator::REQUIRE_POST_SIZE    x[n + N]
		_Interpolator::reset()
		_Interpolator::operator() (_Iter delay, double fraction)
		*/
		/* ==== linear_interpolation ==== */
		// y[n] = x[n - (M+1)]*frac + x[n-M]*(1-frac)
		struct nointerpol
		{

			typedef sample_t value_type;
			enum { REQUIRE_PRE_SIZE = 0, REQUIRE_POST_SIZE = 0, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1 };

			void reset() {}

			template <typename _Ptr>
			sample_t operator()(_Ptr delay, double fraction)
			{
				return *delay;
			}
		};



		//============================================================================
		template <typename _Ty>
		struct linear_data:miqs::_base_pointer_type<_Ty>
		{
			pointer x1;
			pointer x2;
			value_type frac;
		};
		template <typename _Ty = sample_t>
		struct linear:_base_value_type<_Ty>
		{
			enum { REQUIRE_PRE_SIZE = 0, REQUIRE_POST_SIZE = 1, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1 };
			void reset() {}
			template <typename _Ptr>
			sample_t operator()(_Ptr delay, double fraction)
			{
				sample_t x1 = *delay++;
				return x1 + (*delay - x1) * fraction;
			}
		};

		/* ==== cosine-interpolation === */
		//TODO data
		struct cosine
		{
			typedef sample_t value_type;

			enum { REQUIRE_PRE_SIZE = 0, REQUIRE_POST_SIZE = 1, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1 };

			void reset() {}

			template <typename _Ptr>
			sample_t operator()(_Ptr delay, double fraction)
			{
				sample_t x1 = *delay++;
				return x1 + (*delay - x1) * ((1.0 - std::cos(fraction * miqs::pi)) * 0.5);
			}
		};

		/* ==== cubic-interpolation === */
		template <typename _Ty = sample_t>
		struct cubic_data:miqs::_base_pointer_type<_Ty>
		{
			value_type a0, a1, a2, a3;
			value_type x0;
			pointer x1;
			pointer x2;
			pointer x3;
			value_type frac;
		};
		template <typename _Ty>
		struct cubic : miqs::_base_pointer_type<_Ty>
		{
			typedef sample_t value_type;

			enum { REQUIRE_PRE_SIZE = 0, REQUIRE_POST_SIZE = 2, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1 };
			void reset() {}
			template <typename _Ptr>
			sample_t operator()(_Ptr delay, double fraction)
			{
				double x1 = *delay++;
				double x2 = *delay++;
				double x3 = *delay;
				double x0 = x1 + (x1 - x2);
				double a0{}, a1{}, a2{}, a3{};

				a3 = fraction * fraction; a3 -= 1.0; a3 *= (1.0 / 6.0);
				a2 = (fraction + 1.0) * 0.5; a0 = a2 - 1.0;
				a1 = a3 * 3.0; a2 -= a1; a0 -= a3; a1 -= fraction;
				a0 *= fraction; a1 *= fraction; a2 *= fraction; a3 *= fraction; a1 += 1.0;

				return (a0*x0 + a1*x1 + a2*x2 + a3*x3);
			}
		};


		//TODO data
		///* ==== allpass_interpolation ==== */
		//// y[n] = x[n-(M+1)]*frac + x[n-M]*(1-frac) - y[n-1]*(1-frac)
		struct allpass
		{
			typedef sample_t value_type;

			enum { REQUIRE_PRE_SIZE = 0, REQUIRE_POST_SIZE = 1, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1 };

			void reset() { y_delay = 0; }

			template <typename _Ptr>
			sample_t operator()(_Ptr delay, double fraction)
			{
				sample_t out = *delay++*(1 - fraction);
				out += *delay*fraction;
				out -= y_delay*(1 - fraction);
				y_delay = out;
				return out;
			}

			sample_t y_delay{};
		};

		/* ==== spline_interpolation ==== */
		// y[n] = x[n - [M+1]) * ( frac^3 / 6) 
		//				+ x[n - M) * ( ((1+frac)^3 -4*frac^3) /6 )
		//				+ x[n - [M-1]) * ( ((2-frac)^3 -4*(1-frac)^3) / 6 )
		//				+ x[n - [M-2]) * ( (1-frac)^3 / 6 )

		//TODO data
		struct spline
		{
			typedef sample_t value_type;

			enum { REQUIRE_PRE_SIZE = 2, REQUIRE_POST_SIZE = 1, REQUIRE_PARAM_SIZE = REQUIRE_PRE_SIZE + REQUIRE_POST_SIZE + 1 };

			void reset() {}

			template <typename _Ptr>
			sample_t operator()(_Ptr delay, double fraction)
			{
				double f3 = fraction*fraction*fraction;
				double f1p = (1 + fraction);
				double f2m = (2 - fraction);
				double f1m = (1 - fraction);
				sample_t out = *delay++*(f3 / 6);
				out += *delay++*(((f1p*f1p*f1p) - 4 * f3) / 6);
				out += *delay++*((f2m*f2m*f2m - 4 * f1m*f1m*f1m) / 6);
				out += *delay*(f1m *f1m *f1m / 6);
				return out;
			}
		};

	}




	//============================== generator
	//template <typename _OSC, typename _PHASOR>
	//struct generator
	//{
	//	typedef sample_t result_type;

	//	generator(_PHASOR& phasor):m_phasor{ phasor } {}

	//	sample_t operator()()
	//	{
	//		sample_t t = m_osc(m_phasor());
	//		return t;
	//	}

	//private:
	//	_OSC m_osc;
	//	_PHASOR& m_phasor;
	//};


	//// generator_ref
	//template <typename _OSC, typename _PHASOR>
	//struct generator_ref
	//{
	//	typedef sample_t result_type;

	//	generator_ref(_OSC& osc, _PHASOR& phasor):m_osc{ osc }, m_phasor{ phasor } {}

	//	sample_t operator()()
	//	{
	//		sample_t t = m_osc(m_phasor());
	//		return t;
	//	}

	//private:
	//	_OSC& m_osc;
	//	_PHASOR& m_phasor;
	//};

	//template <typename _OSC, typename _Phase> auto make_generator(_Phase& phase) -> generator<_OSC, _Phase> 
	//{ return generator<_OSC, _Phase>(std::ref(phase)); }

	//template <typename _OSC, typename _Phase> auto make_generator(_OSC& osc, _Phase& phase) -> generator_ref<_OSC, _Phase>
	//{return generator_ref<_OSC, _Phase>(std::ref(osc), std::ref(phase));}

	
}