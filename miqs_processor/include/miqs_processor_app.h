#pragma once


namespace miqs
{
	namespace process
	{
		constexpr size_t WavetableSize = 8192;
		constexpr double MaxAmplitude = 0.99;
		enum window_types { 
			WINTYPE_HAMMING = 0, 
			WINTYPE_HANNING, 
			WINTYPE_BLACK,
			WINDOW_COUNT
		};
		enum osc_types { 
			OSCTYPE_SINE = 0,
			OSCTYPE_SAW,
			OSCTYPE_RECT,
			OSCTYPE_TRI,
			OSCTYPE_PHASOR,
			OSC_COUNT
		};
		//enum env_type
		//{
		//	ENVTYPE_LINE =0,
		//	ENVTYPE_EXP
		//};


		struct resource
		{
			typedef sample_t value_type;

			static void resource::init()
			{
				resource::get_instance();
			}
			static resource& get_instance()
			{
				static resource resource;
				return resource;
			}

			miqs::array<value_type>& osc_table(size_t type) { return this->m_osc[type]; }
			//miqs::array<value_type>& envelope_table(size_t type) { return this->m_evn[type]; }
			miqs::array<value_type>& window_table(size_t type) { return this->m_window[type]; }

		private:
			typedef resource self_type;
			resource(self_type const &) = delete;
			resource(self_type &&) = delete;

			self_type& operator=(self_type const&) = delete;


			//-------------------------------buffer for table
			miqs::array_of_array<value_type, WINDOW_COUNT> m_window; // hamming, hanning, 
			miqs::array_of_array<value_type, OSC_COUNT> m_osc; // sin
			//miqs::array_of_array<value_type, 2> m_env; // line, env



			resource():
				m_window{ WavetableSize+1, WavetableSize+1, WavetableSize+1 },
				m_osc{ WavetableSize }
			{
				/* generate samples*/
				// -- window
				miqs::phase_info<int> wp{ 0,1 };
				miqs::phase_operator<miqs::phase_info<int>> wpop(wp);

				miqs::ptr_fill_window_hamming(std::begin(m_window[WINTYPE_HAMMING]), std::end(m_window[WINTYPE_HAMMING]), wp, WavetableSize);
				wp.reset();
				miqs::ptr_fill_window_hanning(std::begin(m_window[WINTYPE_HANNING]), std::end(m_window[WINTYPE_HANNING]), wp, WavetableSize);
				wp.reset();
				miqs::ptr_fill_window_blackman(std::begin(m_window[WINTYPE_BLACK]), std::end(m_window[WINTYPE_BLACK]), wp, WavetableSize);
				wp.reset();


				// -- osc
				miqs::phase_info<> osc_phase;

				miqs::phase_pi_modular<miqs::phase_info<>> op(osc_phase);
				auto gen = miqs::make_generator<miqs::osc::saw_wave>(op);
				miqs::phase_helper<miqs::phase_info<>> phelper(osc_phase);
				phelper.set_info(1, WavetableSize);


				miqs::ptr_fill_sine(std::begin(m_osc[OSCTYPE_SINE]), std::end(m_osc[OSCTYPE_SINE]), osc_phase);
				osc_phase.reset();
				miqs::ptr_fill_saw(std::begin(m_osc[OSCTYPE_SAW]), std::end(m_osc[OSCTYPE_SAW]), osc_phase);
				osc_phase.reset();
				miqs::ptr_fill_rectangular(std::begin(m_osc[OSCTYPE_RECT]), std::end(m_osc[OSCTYPE_RECT]), osc_phase);
				osc_phase.reset();
				miqs::ptr_fill_triangular(std::begin(m_osc[OSCTYPE_TRI]), std::end(m_osc[OSCTYPE_TRI]), osc_phase);
				osc_phase.reset();
				miqs::ptr_fill_triangular(std::begin(m_osc[OSCTYPE_PHASOR]), std::end(m_osc[OSCTYPE_PHASOR]), osc_phase);


				for (size_t i{}; i < OSC_COUNT; ++i)
				{
					miqs::ptr_multiplies(std::begin(m_osc[i]), std::end(m_osc[i]), std::begin(m_osc[i]), MaxAmplitude);
				}
				for (size_t i{}; i < OSC_COUNT; ++i)
				{
					miqs::ptr_multiplies(std::begin(m_osc[i]), std::end(m_osc[i]), std::begin(m_osc[i]), MaxAmplitude);
				}


			}
		};



	}
}