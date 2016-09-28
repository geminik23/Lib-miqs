#pragma once
#include "miqs_basictype.h"
#include "miqs_cont_circular_adaptor.h"
#include "miqs_cont_circular_block.h"

namespace miqs
{

	/* fixed_delay */
	template <size_t N>
	struct fixed_delay
	{
		const sample_t* data() const noexcept { return m_delay.data(); }
		sample_t* data() noexcept { return m_delay.data(); }
		void rotate_to_origin() noexcept { m_delay.rotate_to_origin(); }
		circular_sample_block<N>& _Get_container() noexcept { return m_delay; }

		constexpr size_t size() const  noexcept { return N; }

		void reset()  noexcept { m_delay.reset(); }


		sample_t peek()  const noexcept
		{
			return m_delay.front();
		}

		void push(sample_t s) noexcept
		{
			--m_delay;
			m_delay.back() = s;
		}

		sample_t operator()(sample_t s)
		{
			sample_t t = peek();
			push(s);
			return t;
		}

		template <typename Iter>
		void operator()(Iter ibegin, Iter iend)
		{
			while (ibegin != iend)
			{
				*ibegin = this->operator()(*ibegin);
				++ibegin;
			}
		}

	private:
		circular_sample_block<N> m_delay{};
	};
}