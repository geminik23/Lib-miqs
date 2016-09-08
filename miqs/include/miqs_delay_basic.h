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


	/* delay : dynamic delay */
	struct delay
	{
		delay(size_t size, size_t reserve = 256)
			:m_buffer(std::minmax(size, reserve).second, 0), m_delay{ m_buffer, m_buffer.size() }
		{
			m_buffer.resize(size);
			m_delay.set_size(size);
		}

		void set_delay(size_t size) noexcept
		{
			auto prev_size = this->size();
			m_delay.rotate_to_origin();
			m_buffer.resize(size);
			m_delay.set_size(size);

			if (prev_size < size)
				for (size_t i{ prev_size }; i < size - prev_size; ++i) m_delay[prev_size] = 0.0;
		}

		size_t capacity() const noexcept { return m_buffer.capacity(); }

		const sample_t* data() const noexcept { return m_buffer.data(); }
		sample_t* data() noexcept { return m_buffer.data(); }
		void rotate_to_origin() noexcept { m_delay.rotate_to_origin(); }
		circular_adaptor<std::vector<sample_t>>& _Get_container() noexcept { return m_delay; }

		size_t size() const noexcept { return m_delay.size(); }

		void reset() noexcept { m_delay.reset(); }

		

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
		std::vector<sample_t> m_buffer;
		circular_adaptor<std::vector<sample_t>> m_delay;
	};






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