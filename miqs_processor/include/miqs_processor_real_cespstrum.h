#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class real_cepstrum:
			public TypeProcessor<realfft_module, realifft_module>
		{
		public:
			enum class method
			{
				log10, log10_20, ln
			};

			real_cepstrum();

			size_t get_fftsize() const noexcept { return m_fftdata.get_fftsize(); }
			void set_fftsize(size_t size) { 
				m_fftdata.set_fftsize(size);
				this->set_inport_length(0, m_fftdata.fftsize); 
				this->set_outport_length(0, m_fftdata.fftsize);
				this->set_outport_length(1, m_fftdata.fftsize);
				
			}

			void set_pre_method(method m) noexcept { this->m_unary_method = m; }
			method get_pre_method() const noexcept { return m_unary_method; }

			void operator()(sample_t *in, sample_t * out, sample_t * fft_out, size_t nframe);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			realfft_data<value_type> m_fftdata;
			method m_unary_method;
		};


		inline void real_cepstrum::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t nout)
		{
			auto in = ins[0];
			auto out = outs[0];
			sample_t * fft_out = (nout > 1) ? outs[1] : nullptr;
			this->operator()(in, out, fft_out, info.frame_count());
		}

		inline void real_cepstrum::operator()(sample_t *in, sample_t * out, sample_t * fft_out, size_t nframe)
		{
			size_t i{};
			size_t halfn = nframe / 2;
			miqs::ptr_copy(in, in + nframe, out);
			func_realfft(out, out + nframe, this->m_fftdata.twiddle);
			if (fft_out)	miqs::ptr_copy(out, out + nframe, fft_out);

			std::copy(out, out + nframe, std::ostream_iterator<sample_t>(std::cout, " "));
			std::cout << "\n\n";

			for (i = 0; i < halfn; ++i)
			{
				out[i * 2] /= (double)halfn;
				out[i * 2 + 1] /= (double)halfn;

				MIQS_PTR_C2P_ITSELF(out + (i * 2), out + (i * 2 + 1));
			}
			

			std::copy(out, out + nframe, std::ostream_iterator<sample_t>(std::cout, " "));
			std::cout << "\n\n";

			switch (m_unary_method)
			{
			case method::log10:
				for (i = 0; i < halfn; ++i)
				{
					out[i * 2] = std::log10((out[i * 2]) + std::numeric_limits<sample_t>::epsilon());
					out[i * 2 + 1] = 0.0;
				}
				break;
			case method::log10_20:
				for (i = 0; i < halfn; ++i)
				{
					out[i * 2] = 20 * std::log10((out[i * 2]) + std::numeric_limits<sample_t>::epsilon());
					out[i * 2 + 1] = 0.0;
				}
				break;
			case method::ln:
				for (i = 0; i < halfn; ++i)
				{
					out[i * 2] = std::log((out[i * 2]) + std::numeric_limits<sample_t>::epsilon());
					out[i * 2 + 1] = 0.0;
				}
				break;
			}

			std::copy(out, out+nframe, std::ostream_iterator<sample_t>(std::cout, " "));
			std::cout << "\n\n";

			func_realifft(out, out + nframe, this->m_fftdata.twiddle);
		}

	}
}