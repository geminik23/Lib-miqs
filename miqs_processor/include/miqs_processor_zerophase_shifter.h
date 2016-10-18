#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class zerophase_shifter:
			public base_processor<processor_type::Processor>
		{
		public:
			zerophase_shifter();

			void set_windowsize(size_t size) { this->m_windowsize = size; }
			size_t get_windowsize() const noexcept { return m_windowsize; }


			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		private:
			size_t m_windowsize{};
		};

		inline void zerophase_shifter::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			bool odd = (bool)(m_windowsize&1);
			size_t frame_count = info.frame_count();

			auto in = ins[0];
			auto in_end = in + frame_count;
			auto out = outs[0];
			size_t middle = ((m_windowsize + 1) / 2 - 1);
			if (!odd) middle += 1;

			miqs::ptr_fill_zero(out, out+ frame_count);
			miqs::ptr_copy(in + middle, in + m_windowsize, out);
			miqs::ptr_copy(in, in + middle, out + (frame_count - middle));
		}	



		class zerophase_unshifter:
			public base_processor<processor_type::Processor>
		{
		public:
			zerophase_unshifter();

			void set_windowsize(size_t size) { this->m_windowsize = size; }
			size_t get_windowsize() const noexcept { return m_windowsize; }


			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		private:
			size_t m_windowsize{};
		};

		inline void zerophase_unshifter::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			bool odd = (bool)(m_windowsize & 1);
			size_t frame_count = info.frame_count();

			auto in = ins[0];
			auto in_end = in + frame_count;
			auto out = outs[0];
			size_t middle = ((m_windowsize + 1) / 2 - 1);
			if (!odd) middle += 1;

			miqs::ptr_fill_zero(out, out + frame_count);
			miqs::ptr_copy(in + (frame_count - middle), in_end, out);
			miqs::ptr_copy(in, in + (odd?middle+1:middle),out+middle);
		}
	}
}