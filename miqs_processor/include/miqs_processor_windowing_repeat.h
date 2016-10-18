#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class windowing_repeat:
			public TypeProcessor<buffer_module>
		{
		public:

			windowing_repeat();

			void set_windowsize(size_t size);
			size_t get_windowsize() const noexcept { return m_windowsize; }

			void set_windowtype(window_type t);
			window_type get_windowtype() const noexcept { return m_windowtype; }
			


			//-------- override
			virtual void init(process_info&) override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		private:
			void fill_window();

			size_t m_windowsize;
			window_type m_windowtype;
		};


		inline void windowing_repeat::init(process_info&info)
		{
			this->set_windowsize(info.frame_count());
			this->reset();
		}


		inline void windowing_repeat::set_windowsize(size_t size)
		{
			m_windowsize = size;
			//check
			if (m_buffer.size() < size)
			{
				m_buffer.reset(size);
			}
			this->fill_window();
		}

		inline void windowing_repeat::set_windowtype(window_type t)
		{
			if (t == m_windowtype) return;
			this->fill_window();
		}

		inline void windowing_repeat::fill_window()
		{
			sample_t *ptr = nullptr;
			switch (m_windowtype)
			{
			case window_type::Hanning:
				ptr = resource::get_instance().window_table(WINTYPE_HANNING);
				break;
			case window_type::Hamming:
				ptr = resource::get_instance().window_table(WINTYPE_HAMMING);
				break;
			case window_type::Blackman:
				ptr = resource::get_instance().window_table(WINTYPE_BLACK);
				break;
			}
			if (ptr == nullptr) return;

			double ratio = (double)(WavetableSize) / (this->m_windowsize-1);
			double idxf;
			int idxi;

			for (size_t i{}; i < m_windowsize; ++i)
			{
				idxf = (i)*ratio;
				idxi = (int)idxf;
				this->m_buffer[i] = ptr[idxi] + (ptr[idxi + 1] - ptr[idxi]) * (idxf - idxi);
			}
		}

		inline void windowing_repeat::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_middle = in + std::min(info.frame_count(), m_windowsize);
			auto in_end = in + info.frame_count();
			auto out = outs[0];
			auto win = std::begin(m_buffer);

			for (; in != in_middle; )*out++ = *in++ * *win++;
			for (; in != in_end; ++in)*out++ = 0.0;
		}


	}
}