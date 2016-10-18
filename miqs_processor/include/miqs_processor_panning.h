#pragma once



#include "miqs_processor_base.h"
#include "miqs_processor_fraction_delay.h"

namespace miqs
{
	namespace process
	{

		//template <typename _Interpolator>
		class stereo_panning:
			public TypeProcessor<gain_module>
		{
		public:
			stereo_panning();

			
			void set_panning(double p) noexcept;
			double get_panning()const noexcept { return m_panning; }

			std::pair<sample_t, sample_t>  operator()(sample_t i);

			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;


			
		private:
			double m_panning;
		};



		inline void stereo_panning::init(process_info&)
		{
			this->reset();
			
		}

		inline void stereo_panning::reset()
		{}


		inline void stereo_panning::set_panning(double p) noexcept{ 
			m_panning = miqs::clipping<double>(1.0, 0.0)(p);
			//m_panning = p;
			//MIQS_CLIPPING(m_panning, 1.0,0.0);
		}

		inline void stereo_panning::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out2 = outs[0];
			auto out1 = outs[1];

			for (; in != in_end; ++in)
			{
				*out1++ = (this->low_cos(miqs::two_pi * (m_panning - 0.0))*0.5 + 0.5)*m_gain* *in;
				*out2++ = (this->low_cos(miqs::two_pi * (m_panning - 0.5))*0.5 + 0.5)*m_gain* *in;
			}
		}



		inline std::pair<sample_t, sample_t> stereo_panning::operator()(sample_t in)
		{
			// for multichannel
			//phase (ch-1)/nch
			//std::pow(value, spread...) for later
			//cost((pan - phase) * TWOPI) * 0.5 + 0.5
			return{ (this->low_cos(miqs::two_pi * (m_panning - 0.0))*0.5 + 0.5)*m_gain*in, (this->low_cos(miqs::two_pi * (m_panning - 0.5))*0.5 + 0.5)*m_gain*in };
		}

	}
}