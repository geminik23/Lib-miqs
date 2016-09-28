#pragma once
#include "miqs_basictype.h"
#include "miqs_cont_circular_adaptor.h"
#include "miqs_cont_circular_block.h"

namespace miqs
{

	/* delay : dynamic delay */
	struct delay
	{
		delay(size_t delay_, size_t reserve = 256)
			:m_delaysample{ delay_ }, m_buffer(std::minmax(delay_, reserve).second, 0), m_delay{ m_buffer, m_buffer.size() }
		{
			if (delay_ == 0) throw std::invalid_argument("miqs::delay::delay() - delay value must be greater than 0.");
		}

		size_t get_delay() const noexcept { return this->m_delaysample; }
		void set_delay(size_t size) noexcept
		{
			if (m_delaysample == size) return; 
			if (size > m_buffer.size())
			{
				auto prev_size = m_buffer.size();
				m_delay.rotate_to_origin();
				m_buffer.resize(size);
				m_delay.set_size(size);
				for (size_t i{ prev_size }; i < size - prev_size; ++i) m_delay[prev_size] = 0.0;
			}

			m_delaysample = size;	
		}

		size_t capacity() const noexcept { return m_buffer.capacity(); }

		const sample_t* data() const noexcept { return m_buffer.data(); }
		sample_t* data() noexcept { return m_buffer.data(); }
		void rotate_to_origin() noexcept { m_delay.rotate_to_origin(); }
		circular_adaptor<std::vector<sample_t>>& _Get_container() noexcept { return m_delay; }		
		size_t size() const noexcept { return m_delay.size(); }
		void reset() noexcept { m_delay.reset(); }


		sample_t peek() noexcept
		{
			return m_delay[m_delaysample];
		}

		void push(sample_t s) noexcept
		{
			++m_delay;
			m_delay.front() = s;
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
		size_t m_delaysample;
	};

}