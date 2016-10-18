#pragma once
#include "miqs_basictype.h"

namespace miqs
{

	///* fixed_delay */

	template <size_t N, typename _Ty=sample_t>
	struct fixed_delay
	{

		typedef _Ty value_type;
		typedef value_type argument_type;
		typedef value_type result_type;
		fixed_delay() { reset(); }

		const value_type* data() const noexcept { return m_delay.data(); }
		value_type* data() noexcept { return m_delay.data(); }

		constexpr size_t size() const  noexcept { return N; }

		void reset()  noexcept { 
			for (size_t i{}; i < N; ++i)m_delay[i] = static_cast<value_type>(0);
		}

		result_type peek()  const noexcept
		{
			return m_delay[N - 1];
		}

		void push(sample_t s) noexcept
		{
			for (size_t i{ N - 1 }; i > 0; --i)
				m_delay[i] = m_delay[i - 1];
			m_delay[0] = s;
		}

		result_type operator()(argument_type s)
		{
			result_type t = peek();
			push(s);
			return t;
		}

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
		value_type m_delay[N];
	};





}