#pragma once



#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class realfft:
			public TypeProcessor<realfft_module>
		{
		public:
			realfft();

			size_t get_fftsize() const noexcept { return m_fftdata.get_fftsize(); }
			void set_fftsize(size_t size) { m_fftdata.set_fftsize(size); this->set_inport_length(0, m_fftdata.fftsize); this->set_outport_length(0, m_fftdata.fftsize); }

			void operator()(sample_t *in, sample_t * out, size_t nframe);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			realfft_data<value_type> m_fftdata;
		};

		
		inline void realfft::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto out = outs[0];
			this->operator()(in, out, info.frame_count());
		}
		
		inline void realfft::operator()(sample_t *in, sample_t *out, size_t nframe)
		{
			miqs::ptr_copy(in, in + nframe, out);
			func_realfft(out, out + nframe, this->m_fftdata.twiddle);
		}

	}
}