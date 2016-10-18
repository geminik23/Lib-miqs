
#include "miqs_test.h"
#include "miqs_temp_impl.h"

using namespace miqs_test;
using namespace miqs;
//p475 -> for env interpolation

#define __USE_SOUNDFILE 1

namespace miqs
{

	namespace process
	{


		class granulator_0: public TypeGenerator<gain_module, internal_process_info_module>
		{
		public:
			granulator_0();

			// set sound buffer
			void set_soundbuffer(sample_t * ptr, size_t size) { this->m_soundbuffer.assign(ptr, size); }

			void set_random_factor(double randomness = 0.1) noexcept
			{
				if (randomness < 0) randomness = 0.0;
				else if (randomness > 1.0) randomness = 0.97;
				m_random_factor = 0.97*randomness;
			}

			void set_grain_parameters(size_t duration = 30, size_t rampPercent = 50,
									  int offset = 0, size_t delay = 0)
			{
				if (duration == 0) duration = 1;
				m_duration = duration;

				if (rampPercent > 100) rampPercent = 100;
				m_ramp_percent = rampPercent;
				m_delay = delay;
				m_offset = offset;
			}

			void set_voices(size_t nvoice);

			sample_t operator()();

			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;


		private:
			enum class grain_state
			{
				Stopped = 0, FadeIn, Sustain, FadeOut
			};

			struct grain
			{
				double scaler;
				double rate;
				size_t attack_count;
				size_t sustain_count;
				size_t decay_count;
				size_t delay_count;
				size_t counter;

				double pointer;
				size_t start_pointer;
				size_t repeats;
				grain_state state;

				grain():scaler(0), rate(0), attack_count(0), sustain_count(0), decay_count(0), delay_count(0), counter(0),
					pointer(0), start_pointer(0), repeats(0), state(grain_state::Stopped) {}
			};

			void calculate_grain(grain& g);




			//StkFrames data_;
			std::vector<grain> m_grains;
			miqs::process::noise m_noise;
			//long gPointer_;
			double m_pointer;



			double m_random_factor;
			size_t m_duration;
			size_t m_ramp_percent;
			size_t m_delay;
			int m_offset;

			miqs::array_ref<sample_t> m_soundbuffer;
		};


		granulator_0::granulator_0()
		{
			this->set_name("Granulator1");

			this->set_random_factor();
			this->set_grain_parameters();
			this->set_gain(1.0);
		}

		inline void granulator_0::init(process_info&info)
		{
			this->m_samplerate = info.samplerate();
			this->m_nframe = info.frame_count();
			reset();
		}

		inline void granulator_0::reset() { m_pointer = 0; }

		inline void granulator_0::set_voices(size_t nvoice)
		{
			this->m_grains.resize(nvoice);
			size_t count;
			for (size_t i{}; i < nvoice; ++i)
			{
				m_grains[i].repeats = 0;
				count = static_cast<size_t>(i*m_duration*0.001*m_samplerate / nvoice);
				m_grains[i].counter = count;
				m_grains[i].pointer = m_pointer;
				m_grains[i].state = grain_state::Stopped;
			}
			m_gain = 1.0 / m_grains.size();
		}






		inline void granulator_0::calculate_grain(grain& g)
		{
			if (g.repeats > 0)
			{
				g.repeats--;
				g.pointer = g.start_pointer;
				if (g.attack_count > 0)
				{
					g.scaler = 0.0;
					g.rate = -g.rate;
					g.counter = g.attack_count;
					g.state = grain_state::FadeIn;
				}
				else
				{
					g.counter = g.sustain_count;
					g.state = grain_state::Sustain;
				}
				return;
			}


			double seconds = m_duration*0.001;
			seconds += (seconds * m_random_factor * m_noise());
			size_t count = static_cast<size_t>(seconds * m_samplerate);
			g.attack_count = static_cast<size_t>(m_ramp_percent * 0.005 * count);
			g.decay_count = g.attack_count;
			g.sustain_count = count - 2 * g.attack_count;
			g.scaler = 0.0;
			if (g.attack_count > 0)
			{
				g.rate = 1.0 / g.attack_count;
				g.counter = g.attack_count;
				g.state = grain_state::FadeIn;
			}
			else
			{
				g.counter = g.sustain_count;
				g.state = grain_state::Sustain;
			}

			seconds = m_delay * 0.001;
			seconds += (seconds * m_random_factor * m_noise());
			count = (size_t)(seconds * m_samplerate);

			g.decay_count = count;

			seconds = m_offset*0.001;
			seconds += (seconds * m_random_factor * std::abs(m_noise()));
			int offset = (int)(seconds* m_samplerate);

			seconds = m_duration * 0.001 * m_random_factor * m_noise();
			offset += (int)(seconds * m_samplerate);
			g.pointer += offset;
			while (g.pointer >= m_soundbuffer.size()) g.pointer -= m_soundbuffer.size();
			if (g.pointer < 0) g.pointer = 0;
			g.start_pointer = (size_t)g.pointer;
		}



		void granulator_0::process(process_info& info, sample_t **, size_t, sample_t ** outs, size_t)
		{
			auto count = info.frame_count();
			auto ptr = outs[0];
			for (size_t i{}; i < count; ++i)
			{
				*ptr++ = this->operator()();
			}
		}

		inline sample_t granulator_0::operator()()
		{
			sample_t result{}, sample{};
			if (m_soundbuffer.size() == 0) return 0.0;

			for (auto&& g : m_grains)
			{
				if (g.counter == 0)
				{
					switch (g.state)
					{
					case grain_state::Stopped:
						this->calculate_grain(g);
						break;
					case grain_state::FadeIn:
						if (g.sustain_count > 0)
						{
							g.counter = g.sustain_count;
							g.state = grain_state::Sustain;
							break;
						}
					case grain_state::Sustain:
						if (g.decay_count > 0)
						{
							g.counter = g.decay_count;
							g.rate = -g.rate;
							g.state = grain_state::FadeOut;
						}
						break;
					case grain_state::FadeOut:
						if (g.delay_count > 0)
						{
							g.counter = g.delay_count;
							g.state = grain_state::Stopped;
							break;
						}
						this->calculate_grain(g);
					}
				}

				if ((int)g.state > 0)
				{
					sample = m_soundbuffer[(int)g.pointer];
					if (g.state == grain_state::FadeIn || g.state == grain_state::FadeOut)
					{
						sample *= g.scaler;
						g.scaler += g.rate;
					}
					result += sample;


					g.pointer++;
					if (g.pointer >= m_soundbuffer.size())
						g.pointer = 0;
				}

				g.counter--;
			}

			return result*m_gain;
		}


	}

}

void miqs_test::funcs::past()
{

	std::string infilename{ "Toms_diner.wav" };
	std::string outfilename{ "testing.wav" };

	//===============================in
	auto sndSrcIn = Miqs::SoundIn::ReadSoundFile(infilename);
	auto sndSrcSize = sndSrcIn.size();
	auto inSamplerate = sndSrcIn.property.srate;
	auto inCh = sndSrcIn.property.chans;

	//================================out

	int outCh = inCh;


	// samples
	miqs::array<> in(sndSrcSize);
	miqs::array<> out(sndSrcSize*outCh);

	// read source file
	sndSrcIn.readDoubleFrames(in, in.size());



	//============properties


	size_t nframe = 512;
	//auto ptr = std::begin(in);
	//auto ptr_e = ptr + (sndSrcSize / nframe)*nframe;
	//auto dst_ptr = std::begin(out);


	miqs::process::process_proxy px;
	px.m_samplerate = inSamplerate;
	px.m_nframe = nframe;



	
	//==================================== divided cepstrum with real_cepstrum
	//// properties
	//constexpr size_t signal_size{ 64 };
	//auto print_signal = [](miqs::array<>& arr) {
	//	std::copy(std::begin(arr), std::end(arr), std::ostream_iterator<sample_t>(std::cout, " "));
	//	std::cout << "\n\n";
	//};
	//auto do_process = [](miqs::process::process_proxy& px, miqs::process::iprocessor* p, sample_t * i, sample_t * o) {
	//	px.in = i;
	//	px.out = o;
	//	px.set_current_processor(p);
	//	p->tick(px);
	//};

	//px.m_nframe = signal_size;


	//miqs::array<> original(signal_size);
	//miqs::array<> ps1(signal_size);
	//miqs::array<> ps2(signal_size);

	//miqs::array<> halfs1(signal_size/2);
	//miqs::array<> halfs2(signal_size/2);
	//miqs::array<> halfs3(signal_size/2);
	//miqs::array<> halfs4(signal_size/2);

	//sample_t * branch[2];
	//sample_t * pointerptr;
	//sample_t ** ppointerptr;

	//// generate signal
	//miqs::phase_info<> phase;
	//miqs::phase_helper<miqs::phase_info<>> helper(phase);
	//helper.set_info(3, signal_size);
	//miqs::ptr_fill_sine(ptr_begin(original), ptr_end(original), phase);
	//
	//miqs::process::realfft pfft;
	//miqs::process::realifft pifft;
	//miqs::process::multiplies pmul;
	//miqs::process::interleaved_separator psep;
	//miqs::process::interleaved_merger pmeg;
	//miqs::process::cartesian_to_polar c2p;
	//miqs::process::amp_to_db pln;

	//miqs::process::real_cepstrum pcep;

	//pfft.init(px);
	//pifft.init(px);
	//pmul.init(px);
	//psep.init(px);
	//pmeg.init(px);
	//c2p.init(px);
	//pln.init(px);
	//pcep.init(px);

	//pfft.set_fftsize(signal_size);
	//pifft.set_fftsize(signal_size);
	//pcep.set_fftsize(signal_size);
	//
	//printf("======start processing\n");
	//print_signal(original);
	//
	//printf("======fft\n");
	//do_process(px, &pfft, original, ps1);
	//print_signal(ps1);

	//printf("======normalize\n");
	//pmul.set_value(1.0 / (signal_size/2));
	//do_process(px, &pmul, ps1, ps2);
	//print_signal(ps2);

	//printf("======car2pol\n");
	//do_process(px, &c2p, ps2, ps1);
	//print_signal(ps1);

	//printf("======separate\n");
	//pointerptr = ps1.data();
	//ppointerptr = &pointerptr;
	//branch[0] = halfs1;
	//branch[1] = halfs2;
	//psep.process(px, ppointerptr, 1, branch, 2);
	//print_signal(halfs1);
	//print_signal(halfs2);

	//printf("=======amp2db\n");
	//px.m_nframe = signal_size / 2;
	//do_process(px, &pln, halfs1, halfs3);
	//print_signal(halfs3);

	//printf("=======merge\n");
	//miqs::ptr_fill_zero(std::begin(halfs4), std::end(halfs4));
	//pointerptr = ps1.data();
	//ppointerptr = &pointerptr;
	//branch[0] = halfs3;
	//branch[1] = halfs4;
	//pmeg.process(px, branch, 2, ppointerptr, 1);
	//print_signal(ps1);
	//
	//px.m_nframe = signal_size;

	//printf("=======cepstrum\n");
	//do_process(px, &pifft, ps1, ps2);
	//print_signal(ps2);
	//
	//
	//printf("=======real cepstrum\n");
	//do_process(px, &pcep, original, ps1);
	//print_signal(ps1);


	//
	//====================================cepstrum
	//size_t fftsize = 64;
	//px.m_nframe = fftsize;
	//// containers
	//miqs::array<> sinewave(fftsize);
	//miqs::array<> result(fftsize);
	//miqs::array<> result2(fftsize);
	//miqs::array<> imagpart(fftsize/2);

	//// generate sinwave
	//miqs::phase_info<> phase;
	//miqs::phase_helper<miqs::phase_info<>> helper(phase);
	//helper.set_info(4, fftsize);

	//auto s_ptr = sinewave.begin();
	//auto s_ptr_e = sinewave.end();
	//miqs::ptr_fill_sine(s_ptr, s_ptr_e, phase);
	//miqs::ptr_multiplies(s_ptr, s_ptr_e, s_ptr, 0.99);
	//std::copy(std::begin(sinewave), std::end(sinewave), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";


	//// processors
	//miqs::process::realfft realfft;
	//miqs::process::realifft realifft;
	//miqs::process::cartesian_to_polar c2p;
	//miqs::process::polar_to_cartesian p2c;
	//miqs::process::realpart_db2mag d2m;
	//miqs::process::realpart_mag2db m2d;
	//miqs::process::imagpart_zero izero;


	//realfft.init(px);
	//realifft.init(px);
	//c2p.init(px);
	//p2c.init(px);
	//d2m.init(px);
	//m2d.init(px);
	//izero.init(px);

	//realfft.set_fftsize(fftsize);
	//realifft.set_fftsize(fftsize);
	////fft-> c2p print -> p2c print
	//
	//px.set_current_processor(&realfft);
	//px.in = std::begin(sinewave);
	//px.out = std::begin(result);
	//realfft.tick(px);
	//std::copy(std::begin(result), std::end(result), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";

	//px.set_current_processor(&c2p);
	//px.in = std::begin(result);
	//px.out = std::begin(result2);
	//c2p.tick(px);
	//std::copy(std::begin(result2), std::end(result2), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";
	////------ dB--------------------------------
	//std::cout << "mag to db\n";
	//
	//px.set_current_processor(&m2d);
	//px.in = std::begin(result2);
	//px.out = std::begin(result);
	//m2d.tick(px);
	//std::copy(std::begin(result), std::end(result), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";

	////-------------------------------------CEPSTRUM
	//std::cout << "=============================cepstrum\n";
	//miqs::ptr_copy(result.begin() + 1, result.end(), 2, imagpart.begin());

	//px.set_current_processor(&izero);
	//px.in = std::begin(result);
	//px.out = std::begin(result2);
	//izero.tick(px);
	//std::copy(std::begin(result2), std::end(result2), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";

	//px.set_current_processor(&realifft);
	//px.in = std::begin(result2);
	//px.out = std::begin(sinewave);
	//realifft.tick(px);
	//std::copy(std::begin(sinewave), std::end(sinewave), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";


	//px.set_current_processor(&realfft);
	//px.in = std::begin(sinewave);
	//px.out = std::begin(result);
	//realfft.tick(px);
	//std::copy(std::begin(result), std::end(result), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";
	//miqs::ptr_copy(imagpart.begin(), imagpart.end(), 1, result.begin()+1, 2);


	//std::copy(std::begin(result), std::end(result), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";
	//std::cout << "=============================end\n";


	////not
	//px.set_current_processor(&d2m);
	//px.in = std::begin(result);
	//px.out = std::begin(result2);
	//d2m.tick(px);
	//std::copy(std::begin(result2), std::end(result2), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";



	////--------------------------
	//px.set_current_processor(&p2c);
	//px.in = std::begin(result2);
	//px.out = std::begin(result);
	//p2c.tick(px);
	//std::copy(std::begin(result), std::end(result), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";

	////=== back to time-domain
	//px.set_current_processor(&realifft);
	//px.in = std::begin(result);
	//px.out = std::begin(sinewave);
	//realifft.tick(px);
	//std::copy(std::begin(sinewave), std::end(sinewave), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";


	//=========================================== fill impulse
	//miqs::array<> impulse(64);

	//miqs::phase_info<> phase;
	//miqs::phase_helper<miqs::phase_info<>> helper(phase);
	//helper.set_info(2, impulse.size());


	//miqs::ptr_fill_impulse(std::begin(impulse), std::end(impulse), phase);
	//impulse[0] = 1.0;
	//std::copy(std::begin(impulse), std::end(impulse), std::ostream_iterator<sample_t>(std::cout, "  "));
	//std::cout << "\n\n";













	//=====================================oscillator
	//miqs::process::oscillator osc;
	//osc.init(px);

	//osc.set_osc_type(miqs::process::oscillator::type::Phasor);
	//osc.set_frequency(440);
	//osc.set_gain(1.0);
	//px.set_current_processor(&osc);
	//
	//for (; ptr < ptr_e;)
	//{
	//	px.out = dst_ptr;
	//	osc.tick(px);

	//	ptr += nframe;
	//	dst_ptr += nframe;
	//}





	//===================================== sinc arrange
	//// generate sinc functions and adjust hanning window
	//size_t n_num{ miqs::process::WavetableSize };
	//miqs::array<> sinc_array(n_num);
	//miqs::phase_info<> phase;
	//phase.step = (double)6 * miqs::two_pi / n_num;
	//for (size_t i{}; i < n_num; ++i)
	//{
	//	sinc_array[i] = 0.99*miqs::sinc(phase.step*((double)i - (n_num / 2) + 0.5));
	//}
	//miqs::ptr_transform_multiplies(ptr_begin(sinc_array), ptr_end(sinc_array),
	//							   ptr_begin(miqs::process::resource::get_instance().window_table(miqs::process::WINTYPE_HANNING)), 
	//							   ptr_begin(sinc_array));

	//miqs::ptr_fill_zero(std::begin(out), std::end(out));
	//


	////----------------------------------------------------------
	//miqs::real_noise_data<> data;
	//data.distribution.param(decltype(data.distribution)::param_type(0.0, 1.0));

	//
	//double freq = 440;
	//size_t sfreq = static_cast<size_t>(inSamplerate/freq);
	//
	//double ratio = (double)n_num / sfreq;
	//size_t i{}, j{}, k{};

	//

	//size_t GrainCount{ 10000 };
	//
	//for (i=0; i < GrainCount; ++i)
	//{
	//	k = static_cast<size_t>((data.distribution(data.random_engine)) * (out.size() - sfreq));
	//	for (j = 0; j < sfreq; ++j)
	//	{
	//		out[k + j] = 0.99*sinc_array[static_cast<size_t>(j*ratio)];
	//	}
	//}

	//miqs::ptr_multiplies(std::begin(out), std::end(out), std::begin(out), (double)GrainCount/out.size());
	////-----------------------------------------------------------




	////-------------------------------------


	//miqs::process::biquad_filter filter;
	//filter.init(px);

	//filter.set_filter_type(miqs::filter_type::bandpass);

	//px.set_current_processor(&filter);


	//miqs::phase_info<> info;
	//miqs::phase_helper<miqs::phase_info<>> helper{ info };
	//helper.set_info(130, inSamplerate);


	//miqs::array<> n(nframe);
	//miqs::real_noise_data<sample_t> data;
	//std::random_device rd;
	//data.random_engine.seed(rd());
	//
	//miqs::ptr_fill_noise(std::begin(n), std::end(n), data);
	//for (; ptr < ptr_end;)
	//{
	//	filter.set_frequency_info(1200 + 800*std::sin(info.phase), 100);
	//	

	//	px.in = ptr;
	//	px.out = dst_ptr;
	//	filter.tick(px);

	//	ptr += nframe;
	//	dst_ptr += nframe;
	//	info.next();

	//}



	//==========================granular1
	// search used envelopes





	//==========================granular0
	//miqs::process::granulator_0 granular;
	//granular.set_grain_parameters(3, 100, 1000, 50);
	//granular.set_random_factor(1);
	//granular.set_voices(100);
	//granular.set_soundbuffer(in, in.size());

	//granular.init(px);

	//for (size_t i{}; i < sndSrcSize; ++i)
	//{
	//	*dst_ptr++ = granular();
	//}




















	//=============================noise
	//miqs::process::noise noise;
	//for (size_t i{}; i < 20; i++)
	//	std::cout << noise()<<" ";
	//std::cout << "\n";




	//=============================biquad filter

	//miqs::process::biquad_filter biquad;
	//biquad.init(px);
	//biquad.set_filter_type(miqs::filter_type::bandpass);
	//biquad.set_frequency_info(440.0, 5.0);
	//px.set_current_processor(&biquad);
	//for (size_t i{}; i < sndSrcSize - nframe; i += nframe, ptr+= nframe, dst_ptr += nframe)
	//{
	//	px.in = ptr;
	//	px.out = dst_ptr;
	//	biquad.tick(px);
	//}



	//===================================lpc test
	//miqs::process::lpc lpc;
	//lpc.set_order(14);

	//miqs::array<> lpc_result(lpc.get_order() + 1);
	//
	//
	//for (size_t i{44100}; i < 56000; i+=300)
	//{
	//	sample_ptr a = in + i;
	//	sample_ptr b = lpc_result.data();

	//	lpc.process(px, &a, 1, &b, 1);

	//	std::copy(std::begin(lpc_result), std::end(lpc_result), std::ostream_iterator<double>(std::cout, " "));
	//	std::cout << "\n\n";
	//}



	//=================================pitch shift test plus buffer_pitch_shifter
	//miqs::process::process_proxy px;
	//px.m_samplerate = inSamplerate;
	//px.m_nframe = nframe;

	//miqs::process::buffer_pitch_shifter bps;
	//bps.init(px);
	//bps.set_sound_buffer(in, sndSrcSize);
	//bps.set_pitch(1.5);
	//px.set_current_processor(&bps);

	//for (size_t i{}; 
	//	 i < sndSrcSize - nframe; 
	//	 i += nframe, ptr+= nframe, dst_ptr += nframe)
	//{
	//	px.in = ptr;
	//	px.out = dst_ptr;
	//	bps.tick(px);
	//}





	//===========================================Delay Test
	//std::cout << std::setprecision(3);

	//miqs::array<> data(10,0.0);
	//data[0] = 1;
	//data[1] = 2;
	//data[2] = 2.3;
	//data[3] = 3.6;
	//data[4] = 1.2;
	//miqs::array<> result(10);

	//miqs::process::delay_cubic d(1.2);
	//d.init(px);
	//px.set_current_processor(&d);
	//px.m_nframe = 10;
	//px.in = data;
	//px.out = result;
	//d.tick(px);

	//miqs::ptr_copy(std::begin(data), std::end(data), std::ostream_iterator<double>(std::cout, " "));
	//std::cout << "\n";
	//miqs::ptr_copy(std::begin(result), std::end(result), std::ostream_iterator<double>(std::cout, " "));
	//std::cout << "\n";





	//===============================out
#if __USE_SOUNDFILE
	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = outCh;
	props.srate = inSamplerate;
	props.samptype = sndSrcIn.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	sndout.createSoundFile(outfilename);
	sndout.writeDoubleFrames(out, out.size() / outCh);
#endif
}




