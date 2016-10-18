#pragma once
#include "miqs_basictype.h"
#include "miqs_circular_adaptor.h"

namespace miqs
{



	/* delay : dynamic delay */
	template <typename _Ty = sample_t>
	struct delay_f
	{
		typedef _Ty value_type;
		typedef value_type argument_type;
		typedef value_type result_type;

		delay_f(size_t delay_ = 0, size_t reserve = 256)
			:m_delay{ delay_ }, m_buffer(std::minmax(delay_, reserve).second, 0), m_delaybuffer{ m_buffer, m_buffer.size() }
		{
			if (delay_ == 0) throw std::invalid_argument("miqs::delay::delay() - delay value must be greater than 0.");
		}

		size_t get_delay() const noexcept { return this->m_delay; }
		void set_delay(size_t size) noexcept
		{
			if (m_delay == size) return;
			if (size > m_buffer.size())
			{
				auto prev_size = m_buffer.size();
				m_delaybuffer.rotate_to_origin();
				m_buffer.resize(size);
				m_delaybuffer.assign(m_buffer, m_buffer.size());
				for (size_t i{ prev_size }; i < size - prev_size; ++i) m_delaybuffer[prev_size] = 0.0;
			}

			m_delay = size;
		}

		size_t capacity() const noexcept { return m_buffer.capacity(); }

		const sample_t* data() const noexcept { return m_buffer.data(); }
		sample_t* data() noexcept { return m_buffer.data(); }
		void rotate_to_origin() noexcept { m_delaybuffer.rotate_to_origin(); }
		size_t size() const noexcept { return m_delaybuffer.size(); }
		void reset() noexcept { m_delaybuffer.reset(); }


		sample_t peek() noexcept
		{
			return m_delaybuffer[m_delay - 1];
		}

		void push(sample_t s) noexcept
		{
			++m_delaybuffer;
			m_delaybuffer.front() = s;
		}

		result_type operator()(argument_type s)
		{
			sample_t t = peek();
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
		miqs::array<value_type> m_buffer;
		miqs::circular_adaptor<value_type> m_delaybuffer;
		size_t m_delay;
	};



	/* delay : dynamic delay */
	template <typename _Ty = sample_t>
	struct delay
	{
		typedef _Ty value_type;
		typedef value_type argument_type;
		typedef value_type result_type;

		delay(value_type * data, size_t size, size_t delay_=0)
			:m_delay{ delay_ }, m_delaybuffer{ data, size }
		{
			if (delay_ == 0) throw std::invalid_argument("miqs::delay::delay() - delay value must be greater than 0.");
		}

		delay(value_type * first, value_type * last, size_t delay_=0)
			:m_delay{ delay_ }, m_delaybuffer{ data, std::distance(first, last) }
		{
			if (delay_ == 0) throw std::invalid_argument("miqs::delay::delay() - delay value must be greater than 0.");
		}

		size_t get_delay() const noexcept { return this->m_delay; }
		void set_delay(size_t delay) 
		{
			if (delay == 0) throw std::invalid_argument("miqs::delay::delay() - delay value must be greater than 0.");
			m_delay = delay;
		}


		const sample_t* data() const noexcept { return m_delaybuffer.data(); }
		sample_t* data() noexcept { return m_delaybuffer.data(); }
		void rotate_to_origin() noexcept { m_delaybuffer.rotate_to_origin(); }
		size_t size() const noexcept { return m_delaybuffer.size(); }
		void reset() noexcept { m_delaybuffer.reset(); }


		sample_t peek() noexcept
		{
			return m_delaybuffer[m_delay - 1];
		}

		void push(sample_t s) noexcept
		{
			++m_delaybuffer;
			m_delaybuffer.front() = s;
		}

		result_type operator()(argument_type s)
		{
			sample_t t = peek();
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
		miqs::circular_adaptor<value_type> m_delaybuffer;
		size_t m_delay;
	};
}