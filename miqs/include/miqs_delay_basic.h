#pragma once
#include "miqs_basictype.h"
#include "miqs_cont_circular_adaptor.h"
#include "miqs_cont_circular_block.h"

namespace miqs
{
	
	/**
	delay::peek()
	delay::push(sample_t)
	*/



	/* one sample delay */
	struct one_sample_delay
	{
		const sample_t* data() const { return &m_d; }
		sample_t* data() { return &m_d; }

		sample_t operator()(sample_t s)
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
		sample_t m_d{};
	};







}