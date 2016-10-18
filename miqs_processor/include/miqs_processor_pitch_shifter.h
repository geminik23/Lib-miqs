#pragma once



#include "miqs_processor_base.h"
#include "miqs_processor_fraction_delay.h"

namespace miqs
{
	namespace process
	{

		class pitch_shifter:
			public TypeProcessor<gain_module, buffer_module, internal_process_info_module>
		{
		public:
			enum { tap_count = 2 };

			pitch_shifter();

			void set_pitch(double pitch) noexcept;
			double get_pitch() const noexcept { return m_pitch; }

			void set_maximum_delay(size_t maxdelay);
			size_t get_maximum_delay() const noexcept { return m_delayline.get_maximum_delay(); }

			void set_window_ratio(double win_ratio);
			double get_window_ratio() const noexcept { return m_win_ratio; }

			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

			sample_t operator()(value_type i);

		private:
			delay_cubic m_delayline;
			double m_delaypos[tap_count];

			double m_pitch;
			double m_rate; //internal

			double m_win_ratio;
			size_t m_win_size; //internal
		};



		inline void pitch_shifter::set_window_ratio(double win_ratio)
		{
			m_win_ratio = win_ratio;
			size_t old = m_win_size;
			m_win_size = static_cast<int>(m_win_ratio* m_delayline.get_maximum_delay());

			if (old == m_win_size) return;

			this->reset_buffer(m_win_size);

			miqs::phase_info<int> phase(0, 1);
			miqs::ptr_fill_window_hanning(
				ptr_begin(m_buffer), ptr_at(m_buffer, m_win_size),
				phase, m_win_size);
		}


		inline void pitch_shifter::init(process_info&info)
		{
			this->set_internal_info(info);

			this->set_maximum_delay(static_cast<size_t>(m_samplerate));
			this->reset();
			this->set_window_ratio(m_win_ratio);
			
		}

		inline void pitch_shifter::set_maximum_delay(size_t maxdelay)
		{
			this->m_delayline.set_maximum_delay(maxdelay);
		}

		inline void pitch_shifter::reset()
		{
			// set delay position in range [0. - 1.0]
			for (size_t i{}; i < tap_count; ++i) m_delaypos[i] = (double)i / tap_count;

			m_delayline.reset();
			m_delayline.set_delay(m_delaypos[0]);
		}

		inline void pitch_shifter::set_pitch(double pitch) noexcept
		{
			m_pitch = pitch;
		}


		inline void pitch_shifter::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			m_rate = ((1.0 - m_pitch) / m_win_ratio) / m_samplerate;
			for (; in != in_end; )
			{
				*out = 0.0;
				for (size_t i{}; i < tap_count; ++i)
				{
					m_delayline.set_delay(((m_delaypos[i] * m_win_ratio)* m_samplerate) + 1);
					*out += m_buffer[(int)(m_delaypos[i] * m_win_size)] * m_delayline();

					m_delaypos[i] += m_rate;
					MIQS_PTR_MODULAR_DOWN(m_delaypos[i], 1.0);
					MIQS_PTR_MODULAR_UP(m_delaypos[i], 1.0);
				}
				m_delayline.push(*in++);

				*out++ *= (m_gain *2.0 / tap_count);
			}
		}



		inline pitch_shifter::value_type pitch_shifter::operator()(value_type in)
		{
			sample_t result{};
			m_rate = ((1.0 - m_pitch) / m_win_ratio) / m_samplerate;

			for (size_t i{}; i < tap_count; ++i)
			{
				m_delayline.set_delay(((m_delaypos[i] * m_win_ratio)* m_samplerate) + 1);
				result += m_buffer[(int)(m_delaypos[i] * m_win_size)] * m_delayline();

				m_delaypos[i] += m_rate;
				MIQS_PTR_MODULAR_DOWN(m_delaypos[i], 1.0);
				MIQS_PTR_MODULAR_UP(m_delaypos[i], 1.0);
			}
			m_delayline.push(in);

			return result*m_gain *2.0 / tap_count;
		}

	}
}