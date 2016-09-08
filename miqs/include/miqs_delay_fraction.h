#pragma once

#include "miqs_basictype.h"
#include "miqs_cont_circular_adaptor.h"

namespace miqs
{
	/* fraction delay 
	*/

	template <typename _Interpolator, typename _Cont = std::vector<sample_t>>
	struct fraction_delay
	{
		enum
		{
			MINIMUM_DELAY = _Interpolator::REQUIRE_PRE_SIZE + 1,
			MINIMUM_BUFFER_SIZE = MINIMUM_DELAY + _Interpolator::REQUIRE_POST_SIZE
		};

		static size_t get_minimum_buffer_size(double delay) { return MINIMUM_BUFFER_SIZE + static_cast<size_t>(delay); }

		fraction_delay(double delay, _Cont& container)
			:m_buffer(container),
			m_delay{ container, container.size() }
			//_Interpolator::REQUIRE_PRE_SIZE + (size_t)delay + _Interpolator::REQUIRE_POST_SIZE + 1 }  // 1 is current value.
		{
			if (delay < MINIMUM_DELAY)
				throw std::invalid_argument("fraction_delay():: delay amount must be equal or greater than " + MINIMUM_DELAY);
			if (get_minimum_buffer_size(delay) > container.size())
				throw std::invalid_argument("fraction_delay():: delay buffer size must be equal or greater than " + get_minimum_buffer_size(delay));
			m_tap = (size_t)delay;
			m_fraction = delay - m_tap;
			if (m_tap + _Interpolator::REQUIRE_PRE_SIZE + _Interpolator::REQUIRE_POST_SIZE > container.size()) throw (std::invalid_argument("container size must be equal or greater than std::ceiling(delay)"));
			reset();
		}

		// set delay
		void set_delay(double delay)
		{
			m_tap = (size_t)delay;
			m_fraction = delay - m_tap;
		}

		void set_size(size_t newsize) { this->m_buffer.set_size(size); }
		size_t size() const noexcept { return m_buffer.size(); }
		size_t capacity() const noexcept { return m_buffer.capacity(); }

		const sample_t* data() const noexcept { return m_buffer.data(); }
		sample_t* data() noexcept { return m_buffer.data(); }
		void rotate_to_origin() noexcept { m_delay.rotate_to_origin(); }
		circular_adaptor<std::vector<sample_t>>& _Get_container() noexcept { return m_delay; }

		void reset() noexcept { m_delay.reset(); interpolator.reset(); }

		void push(sample_t s) noexcept
		{
			++m_delay;
			m_delay.front() = s;
		}

		sample_t peek() noexcept
		{
			return interpolator(std::begin(m_delay) + (m_tap - 1 - _Interpolator::REQUIRE_PRE_SIZE), m_fraction);
		}

		sample_t operator()(sample_t s)
		{
			// interpolation
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

		

		_Interpolator interpolator;
	private:
		size_t m_tap;
		double m_fraction;
		_Cont& m_buffer;
		circular_adaptor<_Cont> m_delay;
	};
}







//
//template <typename _Interpolator, typename _Cont = std::vector<sample_t>>
//struct fraction_delay
//{
//	enum
//	{
//		MINIMUM_DELAY = _Interpolator::REQUIRE_PRE_SIZE,
//		MINIMUM_BUFFER_SIZE = 1 + MINIMUM_DELAY + _Interpolator::REQUIRE_POST_SIZE
//	};
//
//	static size_t get_minimum_buffer_size(double delay) { return MINIMUM_BUFFER_SIZE + static_cast<size_t>(delay); }
//
//	fraction_delay(double delay, _Cont& container)
//		:m_buffer(container),
//		m_delay{ container, container.size() }
//		//_Interpolator::REQUIRE_PRE_SIZE + (size_t)delay + _Interpolator::REQUIRE_POST_SIZE + 1 }  // 1 is current value.
//	{
//		if (delay < MINIMUM_DELAY)
//			throw std::invalid_argument("fraction_delay():: delay amount must be equal or greater than " + MINIMUM_DELAY);
//		if (get_minimum_buffer_size(delay) > container.size())
//			throw std::invalid_argument("fraction_delay():: delay buffer size must be equal or greater than " + get_minimum_buffer_size(delay));
//		m_tap = (size_t)delay;
//		m_fraction = delay - m_tap;
//		if (m_tap + _Interpolator::REQUIRE_PRE_SIZE + _Interpolator::REQUIRE_POST_SIZE > container.size()) throw (std::invalid_argument("container size must be equal or greater than std::ceiling(delay)"));
//		reset();
//	}
//
//	// set delay
//	void set_delay(double delay)
//	{
//		m_tap = (size_t)delay;
//		m_fraction = delay - m_tap;
//	}
//
//	size_t size() const noexcept { return m_buffer.size(); }
//	size_t capacity() const noexcept { return m_buffer.capacity(); }
//
//	const sample_t* data() const noexcept { return m_buffer.data(); }
//	sample_t* data() noexcept { return m_buffer.data(); }
//	void rotate_to_origin() noexcept { m_delay.rotate_to_origin(); }
//	circular_adaptor<std::vector<sample_t>>& _Get_container() noexcept { return m_delay; }
//
//	void reset() noexcept { m_delay.reset(); interpolator.reset(); }
//
//	void push(sample_t s) noexcept
//	{
//		++m_delay;
//		m_delay.front() = s;
//	}
//
//	sample_t peek() noexcept
//	{
//		return interpolator(std::begin(m_delay) + (m_tap - _Interpolator::REQUIRE_PRE_SIZE), m_fraction);
//	}
//
//	sample_t operator()(sample_t s)
//	{
//		// interpolation
//		push(s);
//		sample_t t = peek();
//		return t;
//	}
//
//	template <typename Iter>
//	void operator()(Iter ibegin, Iter iend)
//	{
//		while (ibegin != iend)
//		{
//			*ibegin = this->operator()(*ibegin);
//			++ibegin;
//		}
//	}
//
//	_Interpolator interpolator;
//private:
//	size_t m_tap;
//	double m_fraction;
//	_Cont& m_buffer;
//	circular_adaptor<_Cont> m_delay;
//};