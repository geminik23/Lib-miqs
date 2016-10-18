#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{



		class delay: public TypeProcessor<gain_module, buffer_module>
		{
		public:
			delay(size_t delay, size_t max_delay = DefaultFrameLength);
			
			size_t get_maximum_delay() const noexcept { return m_buffer.size(); }
			void set_maximum_delay(size_t max_delay) { m_circular_buffer.rotate_to_origin(); m_buffer.resize(max_delay); reload(); }

			void set_delay(size_t delay_);
			size_t get_delay() const noexcept { return m_delay; }

			void push(value_type i);
			value_type operator()();
			value_type operator()(value_type i);


			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override; 
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
			
		private:
			void reload();

			size_t m_delay;
			miqs::circular_data<> m_circular_buffer;

		};


		inline void delay::reload()
		{
			m_circular_buffer.ptr = m_buffer;
			m_circular_buffer.size = m_buffer.size();
			m_circular_buffer.head = 0;
			m_circular_buffer.tail = m_circular_buffer.size - 1;
		}
		inline void delay::init(process_info&)
		{
			reset();
		}
		inline void delay::reset()
		{
			reload();
			std::fill(ptr_begin(m_buffer), ptr_end(m_buffer), 0.0);
			m_gain = 1.0;
		}

		inline void delay::set_delay(size_t delay_) { 
			if (m_circular_buffer.size < delay_)
			{
				set_maximum_delay(delay_);
			}
			m_delay = delay_; 
		}


		inline void delay::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto nframe = info.frame_count();
			auto iptr = ins[0];
			auto optr = outs[0];
			int ppos;
			for (size_t i{}; i < nframe; ++i)
			{


				ppos = (int)m_circular_buffer.tail - (m_delay -1);
				if (ppos < 0) ppos += m_circular_buffer.size;
				optr[i] = m_gain * *(m_circular_buffer.ptr + ppos);

				m_circular_buffer.increase_head();
				m_circular_buffer.increase_tail();
				*(m_circular_buffer.at_tail()) = iptr[i];

			}
		}

		inline delay::value_type delay::operator()()
		{
			size_t ppos = m_circular_buffer.head + m_delay - 1;
			if (ppos > m_circular_buffer.size) ppos -= m_circular_buffer.size;
			return *(m_circular_buffer.ptr + ppos);
		}

		inline delay::value_type delay::operator()(value_type i)
		{
			value_type result = this->operator()();
			this->push(i);
			return result;
		}

		inline void delay::push(value_type i)
		{
			m_circular_buffer.increase_head();
			m_circular_buffer.increase_tail();
			*(m_circular_buffer.at_tail()) = i;
		}
	}
}