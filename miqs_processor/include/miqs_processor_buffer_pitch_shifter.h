#pragma once



#include "miqs_processor_base.h"
#include "miqs_processor_fraction_delay.h"

namespace miqs
{
	namespace process
	{

		class buffer_pitch_shifter:
			public TypeGenerator<gain_module, buffer_module, internal_process_info_module>
		{
		public:
			enum { tap_count = 2 };

			buffer_pitch_shifter();


			void set_pitch(double pitch) noexcept;
			double get_pitch() const noexcept { return m_pitch; }

			void set_window_ratio(double win_ratio);
			double get_window_ratio() const noexcept { return m_win_ratio; }

			void set_sound_buffer(sample_t * ptr, size_t size);

			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

			sample_t operator()();

		private:
			double m_delaypos[tap_count];

			double m_pitch;
			double m_rate; //internal

			double m_win_ratio;
			size_t m_win_size; //internal


			miqs::interpolation::linear_data<value_type> m_interp;
			miqs::circular_data<value_type> m_circular_buffer;
		};



		inline void buffer_pitch_shifter::set_window_ratio(double win_ratio)
		{
			m_win_ratio = win_ratio;
			size_t old = m_win_size;
			m_win_size = static_cast<int>(m_win_ratio* m_samplerate);

			if (old == m_win_size) return;

			this->reset_buffer(m_win_size);

			miqs::phase_info<int> phase(0, 1);
			miqs::ptr_fill_window_hanning(
				ptr_begin(m_buffer), ptr_at(m_buffer, m_win_size),
				phase, m_win_size);
		}


		inline void buffer_pitch_shifter::init(process_info&info)
		{
			this->set_internal_info(info);
			this->reset();
			this->set_window_ratio(m_win_ratio);
			
		}

		inline void buffer_pitch_shifter::reset()
		{
			// set delay position in range [0. - 1.0]
			for (size_t i{}; i < tap_count; ++i) m_delaypos[i] = (double)i / tap_count;
			
			m_circular_buffer.head = 0;
			m_circular_buffer.tail = m_circular_buffer.size - 1;
		}

		inline void buffer_pitch_shifter::set_sound_buffer(sample_t * ptr, size_t size)
		{
			this->m_circular_buffer.ptr = ptr;
			this->m_circular_buffer.size = size;
			this->reset();
			this->set_window_ratio(m_win_ratio);
		}

		inline void buffer_pitch_shifter::set_pitch(double pitch) noexcept
		{
			m_pitch = pitch;
		}

		inline void buffer_pitch_shifter::process(process_info& info, sample_t **, size_t, sample_t ** outs, size_t)
		{
			auto out = outs[0];
			auto out_end = out + info.frame_count();

			m_rate = ((m_pitch-1.0) / m_win_ratio) / m_samplerate;
			sample_t otemp{};
			for (; out != out_end; )
			{
				*out = 0.0;
				for (size_t i{}; i < tap_count; ++i)
				{
					auto ppos = m_circular_buffer.head + (m_delaypos[i] * m_win_ratio)* m_samplerate;
					MIQS_PTR_MODULAR_UP(ppos, m_circular_buffer.size);

					int ipos = static_cast<size_t>(ppos);
					m_interp.frac = ppos - ipos;
					m_interp.x1 = m_circular_buffer.ptr + ipos;
					m_interp.x2 = m_circular_buffer.ptr + ((ipos + 1 >= (int)m_circular_buffer.size) ? 0 : ipos + 1);
					MIQS_PTR_INTERP_LINEAR(m_interp, &otemp);

					*out += m_buffer[(int)(m_delaypos[i] * m_win_size)] * otemp;

					m_delaypos[i] += m_rate;
					MIQS_PTR_MODULAR_DOWN(m_delaypos[i], 1.0);
					MIQS_PTR_MODULAR_UP(m_delaypos[i], 1.0);
				}

				m_circular_buffer.increase_head();
				m_circular_buffer.increase_tail();

				*out++ *= (m_gain *2.0 / tap_count);
			}
		}



		inline buffer_pitch_shifter::value_type buffer_pitch_shifter::operator()()
		{
			sample_t result{};
			m_rate = ((1.0 - m_pitch) / m_win_ratio) / m_samplerate;
			sample_t otemp;
			for (size_t i{}; i < tap_count; ++i)
			{
				auto ppos = m_circular_buffer.head + (m_delaypos[i] * m_win_ratio)* m_samplerate;
				MIQS_PTR_MODULAR_UP(ppos, m_circular_buffer.size);

				int ipos = static_cast<size_t>(ppos);
				m_interp.frac = ppos - ipos;
				m_interp.x1 = m_circular_buffer.ptr + ipos;
				m_interp.x2 = m_circular_buffer.ptr + ((ipos + 1 >= (int)m_circular_buffer.size) ? 0 : ipos + 1);
				MIQS_PTR_INTERP_LINEAR(m_interp, &otemp);

				result += m_buffer[(int)(m_delaypos[i] * m_win_size)] * otemp;

				m_delaypos[i] += m_rate;
				MIQS_PTR_MODULAR_DOWN(m_delaypos[i], 1.0);
				MIQS_PTR_MODULAR_UP(m_delaypos[i], 1.0);
			}

			m_circular_buffer.increase_head();
			m_circular_buffer.increase_tail();

			return result*m_gain *2.0 / tap_count;
		}



	}
}