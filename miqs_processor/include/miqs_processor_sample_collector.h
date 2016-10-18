#pragma once



#include "miqs_processor_base.h"
#include "miqs_processor_fraction_delay.h"

namespace miqs
{
	namespace process
	{
		//TODO how to send output?
		class sample_collector:
			public TypeProcessor<buffer_module>
		{
		public:

			typedef std::function<void(size_t)> callback_type;
			sample_collector();

			void set_reserve_size(size_t size);
			size_t get_reserve_size() const noexcept { return m_buffer.size(); }

			void set_collect_size(size_t size);
			size_t get_collect_size() const noexcept { return m_collect_size; }

			void operator()(sample_t i);

			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;



		private:
			void reload_wrapper();

			size_t m_counter;
			size_t m_collect_size;
			miqs::circular_data<sample_t> m_buffwrapper;
			miqs::process::signal<callback_type> m_signal;
		};

		inline void sample_collector::set_collect_size(size_t size)
		{
			this->m_collect_size = size;
			if (this->m_buffer.size() < m_collect_size)
			{
				this->set_reserve_size(m_collect_size);
			}
			this->set_outport_length(0, size);
		}

		inline void sample_collector::set_reserve_size(size_t size)
		{
			if (this->m_buffer.size() >= size) return;

			if(m_buffwrapper.ptr)this->m_buffwrapper.rotate_to_origin();
			this->m_buffer.resize(size);
		}


		inline void sample_collector::init(process_info&)
		{
			this->reset();
		}

		inline void sample_collector::reset(){	
			miqs::ptr_fill_zero(std::begin(m_buffer), std::end(m_buffer)); 
			reload_wrapper();
			m_counter = 0;
		}

		inline void sample_collector::reload_wrapper()
		{
			m_buffwrapper.head = 0;
			m_buffwrapper.tail = m_collect_size - 1;
			m_buffwrapper.ptr = m_buffer.data();
			m_buffwrapper.size = m_buffer.size();
		}


		inline void sample_collector::process(process_info& info, sample_t ** ins, size_t, sample_t **, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();

			for (; in != in_end; ++m_counter)
			{
				m_buffwrapper.increase_head();
				m_buffwrapper.increase_tail();
				*(m_buffwrapper.at_tail()) = *in++;
				
				if (m_counter >= m_collect_size)
				{
					m_counter = 0;
					this->m_signal.emit(m_collect_size);
				}
			}
		}



		inline void sample_collector::operator()(sample_t in)
		{
			m_buffwrapper.increase_head();
			m_buffwrapper.increase_tail();
			*(m_buffwrapper.at_tail()) = in;

			if (m_counter >= m_collect_size)
			{
				m_counter = 0;
				this->m_signal.emit(m_collect_size);
			}

			++m_counter;
		}

	}
}