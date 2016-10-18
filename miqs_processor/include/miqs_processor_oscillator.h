#pragma once



#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		

		class oscillator:
			public TypeGenerator<gain_module, phase_info_module<double, 1>, internal_process_info_module>
		{
		public:
			typedef oscillator self_type;
			enum class type
			{
				Sine, Sawtooth, Rectangular, Triangular, Phasor
			};

			oscillator();


			void set_frequency(double freq) noexcept;
			double get_frequency() const noexcept { return m_oscfreq; }

			void set_osc_type(type osctype) noexcept;
			type get_osc_type() const noexcept { return m_osctype; }

			sample_t operator()();


			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			self_type::type m_osctype;
			double m_oscfreq;

			sample_t * m_ptr_wavetable;

			miqs::interpolation::linear_data<sample_t> m_interp;
		};



		inline void oscillator::init(process_info&info)
		{
			this->set_internal_info(info);
			this->reset();
			this->set_frequency(440.0);
		}

		inline void oscillator::reset()
		{
			this->m_phaseinfo[0].phase = 0.0;
		}


		inline void oscillator::set_frequency(double freq) noexcept
		{
			this->m_oscfreq = freq;
			this->m_phaseinfo[0].step = (double)WavetableSize*freq /this->m_samplerate;
		}
	
		inline void oscillator::set_osc_type(oscillator::type osctype) noexcept
		{
			this->m_osctype = osctype;

			switch (m_osctype)
			{
			case type::Sine:
				this->m_ptr_wavetable = resource::get_instance().osc_table(OSCTYPE_SINE);
				break;
			case type::Sawtooth:
				this->m_ptr_wavetable = resource::get_instance().osc_table(OSCTYPE_SAW);
				break;
			case type::Phasor:
				this->m_ptr_wavetable = resource::get_instance().osc_table(OSCTYPE_PHASOR);
				break;
			case type::Rectangular:
				this->m_ptr_wavetable = resource::get_instance().osc_table(OSCTYPE_RECT);
				break;
			case type::Triangular:
				this->m_ptr_wavetable = resource::get_instance().osc_table(OSCTYPE_TRI);
				break;
			}
			
		}

		inline void oscillator::process(process_info& info, sample_t **, size_t, sample_t ** outs, size_t)
		{
			int pi_;

			auto out = outs[0];
			auto out_end = outs[0] + info.frame_count();

			for (; out != out_end; ++out)
			{
				pi_ = static_cast<int>(m_phaseinfo[0].phase);
				m_interp.frac = m_phaseinfo[0].phase - pi_;
				m_interp.x1 = m_ptr_wavetable + pi_;
				m_interp.x2 = m_ptr_wavetable + ((pi_ + 1 != WavetableSize) ? pi_ + 1 : 0);
				MIQS_PTR_INTERP_LINEAR(m_interp, out);
				*out *= m_gain;

				MIQS_PHASE_NEXT(m_phaseinfo[0]);
				MIQS_PHASE_MODULAR_UP(m_phaseinfo[0], (double)WavetableSize);
				MIQS_PHASE_MODULAR_DOWN(m_phaseinfo[0], (double)WavetableSize);
			}
		}



		inline sample_t oscillator::operator()()
		{
			sample_t result{};
			int pi_ = static_cast<int>(m_phaseinfo[0].phase);
			
			m_interp.frac = m_phaseinfo[0].phase - pi_;
			m_interp.x1 = m_ptr_wavetable+ pi_;
			m_interp.x2 = m_ptr_wavetable + ((pi_ + 1 != WavetableSize) ? pi_ + 1 : 0);
			MIQS_PTR_INTERP_LINEAR(m_interp, &result);

			MIQS_PHASE_NEXT(m_phaseinfo[0]);
			MIQS_PHASE_MODULAR_UP(m_phaseinfo[0], (double)WavetableSize);
			MIQS_PHASE_MODULAR_DOWN(m_phaseinfo[0], (double)WavetableSize);

			return m_gain * result;
		}

	}
}