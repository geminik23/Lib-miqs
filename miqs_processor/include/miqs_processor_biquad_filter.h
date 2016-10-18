#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class biquad_filter:
			public TypeProcessor<gain_module, buffer_module>
		{
		public:
			biquad_filter();

			void set_cutoff_frequency(double freq) noexcept { this->m_filterinfo.cutoff_frequency = freq; update_filter(); }
			void set_bandwidth(double bw) noexcept { this->m_filterinfo.bandwidth = bw; update_filter(); }
			void set_frequency_info(double freq, double bw) { this->m_filterinfo.bandwidth = bw; this->m_filterinfo.cutoff_frequency = freq; update_filter(); }
			void set_filter_type(miqs::filter_type type) noexcept { this->m_filtetype = type; update_filter(); }

			void set_coefficients(sample_t * a, sample_t *b);

			double get_cutoff_frequency() const noexcept { return m_filterinfo.cutoff_frequency; }
			double get_bandwidth() const noexcept { return m_filterinfo.bandwidth; }
			miqs::filter_type get_filter_type() const noexcept { return m_filtetype; }


			sample_t operator()(sample_t i);

			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			void update_filter();

			array_ref_n<value_type, 2> m_dx;
			array_ref_n<value_type, 3> m_a;
			array_ref_n<value_type, 3> m_b;

			miqs::filter_info m_filterinfo;
			miqs::filter_type m_filtetype;
		};



		inline void biquad_filter::init(process_info& info)
		{
			this->reset();
			m_filterinfo.samplerate = info.samplerate();
			m_filterinfo.bandwidth = 100;
			m_filterinfo.cutoff_frequency = 1200.0;
			update_filter();
		}

		inline void biquad_filter::reset()
		{
			m_dx.reset();
			update_filter();
		}

		inline void biquad_filter::set_coefficients(sample_t * a, sample_t *b)
		{
			size_t i;
			for (i = 0; i < 3; ++i)
			{
				m_a[i] = a[i];
				m_b[i] = b[i];
			}
		}

		inline void biquad_filter::update_filter()
		{
			switch (m_filtetype)
			{
			case miqs::filter_type::allpass:
				miqs::calculate_canonical_coefficients<canonical::second_order_allpass>(m_filterinfo, m_a.data(), m_b.data());
				break;
			case miqs::filter_type::bandpass:
				miqs::calculate_canonical_coefficients<canonical::second_order_bandpass>(m_filterinfo, m_a.data(), m_b.data());
				break;
			case miqs::filter_type::bandreject:
				miqs::calculate_canonical_coefficients<canonical::second_order_bandreject>(m_filterinfo, m_a.data(), m_b.data());
				break;
			case miqs::filter_type::highpass:
				miqs::calculate_canonical_coefficients<canonical::second_order_highpass>(m_filterinfo, m_a.data(), m_b.data());
				break;
			case miqs::filter_type::lowpass:
				miqs::calculate_canonical_coefficients<canonical::second_order_lowpass>(m_filterinfo, m_a.data(), m_b.data());
				break;

			}
		}

		inline void biquad_filter::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				*out++ = this->operator()(*in++);
			}
		}



		inline sample_t biquad_filter::operator()(sample_t in)
		{
			sample_t t = in - m_a[1] * m_dx[0] - m_a[2] * m_dx[1];
			sample_t out = m_b[0] * t + m_b[1] * m_dx[0] + m_b[2] * m_dx[1];
			m_dx[1] = m_dx[0];
			m_dx[0] = t;
			return out;
		}

	}
}