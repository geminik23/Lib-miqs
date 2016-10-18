#pragma once

#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class all_zero_filter:
			public TypeProcessor<gain_module,  buffer_module>
		{
		public:
			all_zero_filter();

			void set_order(size_t order);
			size_t get_order() const noexcept { return m_order; }

			void reset_delay();

			void set_coefficients(sample_t * coeff, size_t size);

			sample_t operator()(sample_t i);

			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;


		private:
			array_ref<sample_t> m_dx;
			array_ref<sample_t> m_c;

			size_t m_order;
		};



		inline void all_zero_filter::init(process_info&info){this->reset();}

		inline void all_zero_filter::reset(){this->reset_delay();}

		inline void all_zero_filter::set_order(size_t order)
		{
			if (this->m_order == order) return;
			m_order = order;
			if (m_buffer.size() < (m_order + m_order + 1))
			{
				m_buffer.reset();
			}

			// set to sample_wrapper
			auto ptr = m_buffer.data();
			m_dx.assign(ptr, order);
			ptr += order;
			m_c.assign(ptr, order + 1);

			this->reset_delay();
		}

		void all_zero_filter::reset_delay()
		{
			for (sample_t * ptr = this->m_dx.data();
				 ptr < m_dx.end(); ++ptr)*ptr = 0.0;
		}

		void all_zero_filter::set_coefficients(sample_t * coeff, size_t size)
		{
			size_t i{};
			for (; i < size; ++i) m_c[i] = coeff[i];
			for (; i < m_order + 1; ++i) m_c[i] = 0.0;
		}

		inline void all_zero_filter::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; ++in, ++out)
			{
				*out = *in * m_c[0];
				for (int j = m_order - 1; j > 0; j--)
				{
					*out += m_c[j + 1] * m_dx[j];
					m_dx[j] = m_dx[j - 1];
				}
				*out += m_c[1] * m_dx[0];
				m_dx[0] = *in * m_gain;
			}
		}



		inline sample_t all_zero_filter::operator()(sample_t in)
		{
			sample_t out = in * m_c[0];
			for (int j = m_order - 1; j > 0; j--)
			{
				out += m_c[j + 1] * m_dx[j];
				m_dx[j] = m_dx[j - 1];
			}
			out += m_c[1] * m_dx[0];
			m_dx[0] = in;
			return out * m_gain;
		}

	}
}