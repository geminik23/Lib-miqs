#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	
	/**
	delay::peek()
	delay::push(sample_t)
	*/



	/* one sample delay */
	template <typename _Ty =sample_t>
	struct onesample_delay_f
	{

		typedef _Ty value_type;
		typedef value_type argument_type;
		typedef value_type result_type;


		const sample_t* data() const { return &m_d; }
		sample_t* data() { return &m_d; }

		result_type operator()(argument_type s)
		{
			std::swap(s, m_d);
			return s;
		}

		void reset() noexcept { m_d = 0; }


		void push(sample_t s) noexcept
		{
			m_d = s;
		}

		sample_t peek() const noexcept { return m_d; }

		template <typename _InPtr, typename _OutPtr>
		void operator()(_InPtr ibegin, _InPtr iend, _OutPtr dst)
		{
			result_type t;
			while (ibegin != iend)
			{
				t = peek();
				push(*ibegin++);
				*dst++ = t;
			}
		}

	private:
		value_type m_d{};
	};




	/* one sample delay */
	template <typename _Ty = sample_t>
	struct onesample_delay
	{

		typedef _Ty value_type;
		typedef value_type argument_type;
		typedef value_type result_type;
		

		onesample_delay(value_type * value):m_d{ value } {}

		const sample_t* data() const { return m_d; }
		sample_t* data() { return m_d; }

		result_type operator()(argument_type s)
		{
			std::swap(s, *m_d);
			return s;
		}

		void reset() noexcept { *m_d = 0; }


		void push(sample_t s) noexcept
		{
			*m_d = s;
		}

		sample_t peek() const noexcept { return *m_d; }

		template <typename _InPtr, typename _OutPtr>
		void operator()(_InPtr ibegin, _InPtr iend, _OutPtr dst)
		{
			result_type t;
			while (ibegin != iend)
			{
				t = peek();
				push(*ibegin++);
				*dst++ = t;
			}
		}

	private:
		value_type*m_d;
	};




}