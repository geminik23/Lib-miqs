#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class zerophase_windowing:
			public TypeProcessor<buffer_module>
		{
		public:

			zerophase_windowing();

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


		inline void zerophase_windowing::init(process_info&info)
		{
			this->set_windowsize(info.frame_count());
			this->reset();
		}


		inline void zerophase_windowing::set_windowsize(size_t size)
		{
			m_windowsize = size;
			//check
			if (m_buffer.size() < size)
			{
				m_buffer.reset(size);
			}
			this->fill_window();
		}

		inline void zerophase_windowing::set_windowtype(window_type t)
		{
			if (t == m_windowtype) return;
			this->fill_window();
		}

		inline void zerophase_windowing::fill_window()
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

		inline void zerophase_windowing::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			bool odd = (bool)(m_windowsize & 1);
			size_t frame_count = info.frame_count();
			size_t middle = ((m_windowsize + 1) / 2 - 1);
			if (!odd) middle += 1;

			auto in = ins[0];
			auto in_middle = in + (odd ? middle + 1 : middle);
			auto in_end = in + info.frame_count();
			auto out = outs[0];
			auto win = std::begin(m_buffer);
			auto win_middle = win + (odd ? middle + 1 : middle);

			
			miqs::ptr_fill_zero(out, out + frame_count);

			miqs::ptr_transform_multiplies(in, in_middle, win, out + (frame_count - middle));
			miqs::ptr_transform_multiplies(in_middle, in+m_windowsize, win_middle, out);

		}


	}
}