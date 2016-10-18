#pragma once

#include "miqs_basictype.h"

#include "miqs_circular_adaptor.h"
namespace miqs
{
	/* fraction delay 
	*/
	//TODO FIXME ROTATING

	template <typename _Interpolator, typename _Ty = sample_t>
	struct fraction_delay
	{
		typedef _Ty value_type;
		typedef value_type * pointer;
		typedef value_type argument_type;
		typedef value_type result_type;

		enum
		{
			MINIMUM_DELAY = _Interpolator::REQUIRE_PRE_SIZE + 1,
			MINIMUM_BUFFER_SIZE = MINIMUM_DELAY + _Interpolator::REQUIRE_POST_SIZE
		};

		static size_t get_minimum_buffer_size(double delay) { return MINIMUM_BUFFER_SIZE + static_cast<size_t>(delay + 0.5); }

		fraction_delay(pointer data, size_t size, double delay=10.0)
			:m_buffer(data),
			m_delay{ data, size}
		{
			if (delay < MINIMUM_DELAY)
				throw std::invalid_argument("fraction_delay():: delay amount must be equal or greater than " + MINIMUM_DELAY);
			if (get_minimum_buffer_size(delay) > m_delay.size())
				throw std::invalid_argument("fraction_delay():: delay buffer size must be equal or greater than " + get_minimum_buffer_size(delay));
			m_tap = (size_t)delay;
			m_fraction = delay - m_tap;
			if (m_tap + _Interpolator::REQUIRE_PRE_SIZE + _Interpolator::REQUIRE_POST_SIZE > m_delay.size()) throw (std::invalid_argument("container size must be equal or greater than std::ceiling(delay)"));
			reset();
		}

		// set delay
		void set_delay(double delay)
		{
			m_tap = (size_t)delay;
			m_fraction = delay - m_tap;
		}

		size_t size() const noexcept { return m_delay.size(); }
		void resize(size_t size) { m_delay.resize(size); }

		const sample_t* data() const noexcept { return m_buffer; }
		sample_t* data() noexcept { return m_buffer; }
		void rotate_to_origin() noexcept { m_delay.rotate_to_origin(); }

		void reset() noexcept { m_delay.reset(); interpolator.reset(); }

		void push(sample_t s) noexcept
		{
			++m_delay;
			m_delay.front() = s;
		}

		result_type peek() noexcept
		{
			int k{static_cast<int>(m_tap) - 1 - _Interpolator::REQUIRE_PRE_SIZE };

			if (m_delay.offset_head + k+ _Interpolator::REQUIRE_PARAM_SIZE >= m_delay.size())
			{
				for (int i{}; i < _Interpolator::REQUIRE_PARAM_SIZE; ++i, ++k)
				{
					m_param[i] = m_delay[k];
				}
				return interpolator(m_param, m_fraction);
			}

			return interpolator(&m_delay[k], m_fraction);
		}

		result_type operator()(argument_type s)
		{
			// interpolation
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

		

		_Interpolator interpolator;
	private:
		size_t m_tap;
		double m_fraction;

		pointer m_buffer;
		value_type m_param[_Interpolator::REQUIRE_PARAM_SIZE];
		circular_adaptor<value_type> m_delay;
	};



}



