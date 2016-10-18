#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{
		class base_fraction_delay: public TypeProcessor<gain_module, buffer_module>
		{
		public:
			base_fraction_delay(double delay = 1.0, size_t max_delay = DefaultFrameLength);

			size_t get_maximum_delay() const noexcept { return m_buffer.size(); }
			void set_maximum_delay(size_t max_delay) { 
				if (max_delay == m_buffer.size())return; 
				m_circular_buffer.rotate_to_origin(); 
				m_buffer.resize(max_delay); 
				reload(); 
			}

			void set_delay(double delay_) noexcept{m_delay = delay_; }
			double get_delay() const noexcept { return m_delay; }

			miqs::circular_data<value_type>& _Get_container() { return m_circular_buffer; }

			void push(value_type i);

			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }

		protected:
			void reload();

			double m_delay;
			miqs::circular_data<value_type> m_circular_buffer;

		};

		inline void base_fraction_delay::push(value_type i)
		{
			m_circular_buffer.increase_head();
			m_circular_buffer.increase_tail();
			*(m_circular_buffer.at_tail()) = i;
		}


		inline void base_fraction_delay::reload()
		{
			m_circular_buffer.ptr = m_buffer;
			m_circular_buffer.size = m_buffer.size();
			m_circular_buffer.head = 0;
			m_circular_buffer.tail = m_circular_buffer.size - 1;
		}

		inline void base_fraction_delay::init(process_info&)
		{
			reset();
		}
		inline void base_fraction_delay::reset()
		{
			reload();
			::std::fill(ptr_begin(m_buffer), ptr_end(m_buffer), 0.0);
		}


		//=============================================
		// linear interpolation delay
		//
		class delay_linear:public base_fraction_delay
		{
		public:
			using base_fraction_delay::base_fraction_delay;

			value_type operator()();
			value_type operator()(value_type in);
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		private:
			miqs::interpolation::linear_data<value_type> m_interp;
		};





		//=============================================
		// cubic interpolation delay
		//
		class delay_cubic:public base_fraction_delay
		{
		public:
			using base_fraction_delay::base_fraction_delay;


			value_type operator()();
			value_type operator()(value_type in);
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		private:
			miqs::interpolation::cubic_data<value_type> m_interp;
		};


	}
}