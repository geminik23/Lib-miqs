#include "miqs_test.h"
#include "miqs_temp_impl.h"
using namespace miqs;
using namespace miqs_test;


void miqs_test::funcs::for_pitchshift_1()
{

}



void miqs_test::funcs::grain_1()
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
	auto ptr = std::begin(in);
	auto ptr_e = ptr + (sndSrcSize / nframe)*nframe;
	auto dst_ptr = std::begin(out);


	miqs::process::process_proxy px;
	px.m_samplerate = inSamplerate;
	px.m_nframe = nframe;


	//==================================== setting frequency and width

	double specified_freq = 1200.0; //500~2000
	double bandwidth = 200.0;
	double samplerate = inSamplerate;

	double bfreq_up = specified_freq * 2;
	double bfreq_down = specified_freq / 2;

	miqs::process::buffer_pitch_shifter pshifter;
	pshifter.init(px);

	miqs::process::biquad_filter pfilter;
	pfilter.init(px);
	pfilter.set_filter_type(miqs::filter_type::bandpass);
	pfilter.set_frequency_info(specified_freq, bandwidth);

	miqs::process::windowing pwindow;
	pwindow.init(px);
	pwindow.set_windowtype(miqs::process::window_type::Hanning);


	auto get_grain_info = [bfreq_up, bfreq_down, sndSrcSize](grain& g) {
		//position, freq, length

		random_range<sample_t> _range;
		g.freq = _range(bfreq_down, bfreq_up);
		g.dur = static_cast<size_t>(_range(1700, 5600));
		g.pos = static_cast<size_t>(_range(1000, sndSrcSize - g.dur - 2000));
	};

	int count{ -1 };
	grain _grain;
	for (size_t i{};
		 i < sndSrcSize;
		 ++i, ++dst_ptr, --count)
	{
		if (count<0)
		{
			get_grain_info(_grain);
			pshifter.set_sound_buffer(in.data() + _grain.pos, _grain.dur);
			pshifter.set_pitch(specified_freq / _grain.freq);
			count = static_cast<int>(_grain.dur);
			pfilter.reset();
			pwindow.set_windowsize(_grain.dur);
			pwindow.reset();

		}

		// from pitch_shifter
		// bandpass
		*dst_ptr = pshifter();
		*dst_ptr = pfilter(*dst_ptr);
		*dst_ptr = pwindow(*dst_ptr);
	}


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



void miqs_test::funcs::lpc_cross()
{
	std::cout << std::setprecision(5);


	/* original samples */
	auto sndSrc = Miqs::SoundIn::ReadSoundFile("moore_guitar.wav");
	auto sndEnv = Miqs::SoundIn::ReadSoundFile("Toms_diner.wav");

	miqs::array<> src_x(sndSrc.size());
	miqs::array<> env_x(sndEnv.size());

	sndSrc.readDoubleFrames(src_x, src_x.size());
	sndEnv.readDoubleFrames(env_x, env_x.size());

	//---- properties
	size_t blockLength{ 400 };
	size_t hopsize{ 200 };
	constexpr size_t env_order{ 20 };
	constexpr size_t src_order{ 6 };


	size_t outLength = std::min(src_x.size(), env_x.size());
	// out samples
	miqs::array<> out(outLength);

	size_t nFrame{ (outLength - blockLength) / hopsize };


	auto src_begin = std::begin(src_x);
	auto src_end = src_begin + blockLength;
	auto env_begin = std::begin(env_x);
	auto env_end = env_begin + blockLength;
	auto out_ptr = std::begin(out);

	miqs::array<> src_lc(src_order + 1), src_rc(src_order + 1);
	miqs::array<> env_lc(env_order + 1), env_rc(env_order + 1);



	miqs::array<> excitation1(blockLength);

	//filters
	miqs::array<> delay(src_order, 0.0);
	miqs::all_zero_filter<src_order> src_anafilter(src_lc, delay);
	miqs::array<> delay2(env_order, 0.0);
	miqs::all_pole_filter<env_order> dst_synfilter(env_lc, delay2);

	for (size_t frame{}; frame < nFrame; ++frame)
	{
		std::fill(std::begin(delay), std::end(delay), 0.0);
		std::fill(std::begin(delay2), std::end(delay2), 0.0);

		auto g_env = miqs::lpc_levinson_durbin_method(env_begin, env_end, env_lc, env_rc, env_order);
		auto g_src = miqs::lpc_levinson_durbin_method(src_begin, src_end, src_lc, src_rc, src_order);

		////gain
		ptr_transform(std::begin(src_lc), std::end(src_lc), std::begin(src_lc), miqs::divides_const<sample_t>(g_src));
		ptr_transform(src_begin, src_end, std::begin(excitation1), src_anafilter);

		ptr_transform(std::begin(excitation1), std::end(excitation1), std::begin(excitation1), miqs::multiplies_const<sample_t>(g_env));
		ptr_transform(std::begin(excitation1), std::begin(excitation1) + env_order, out_ptr, dst_synfilter);
		ptr_transform(std::begin(excitation1), std::begin(excitation1) + hopsize, out_ptr, dst_synfilter);







		src_begin += hopsize;
		src_end += hopsize;
		env_begin += hopsize;
		env_end += hopsize;
		out_ptr += hopsize;
	}


	//--------------

	//miqs::normalize_with_max(std::begin(out), std::end(out), std::begin(out));



	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = 1;
	props.srate = sndSrc.property.srate;
	props.samptype = sndSrc.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	sndout.createSoundFile("result_cross_lpc.wav");
	sndout.writeDoubleFrames(out.data(), out.size());

}

//
//void miqs_test::funcs::convert_samplerate()
//{
//	/* change samplerate */
//	//==================================Sound Sample
//	auto sndSrcIn = Miqs::SoundIn::ReadSoundFile("moore_guitar.wav");
//	auto sndSrcSize = sndSrcIn.size();
//	auto inSamplerate = sndSrcIn.property.srate;
//
//	int outSamplerate = inSamplerate == 48000 ? 44100 : 48000;
//	double ratioSr = (double)inSamplerate / outSamplerate;
//
//
//	// samples
//	miqs::array<> in(sndSrcSize);
//	miqs::array<> out(static_cast<size_t>(ratioSr*sndSrcSize + 0.5));
//
//	// read source file
//	sndSrcIn.readDoubleFrames(in, in.size());
//
//	//====================================
//
//	auto adaptor = miqs::make_circular_adaptor(in);
//	auto wavetable = miqs::make_wavetable<miqs::interpolation::linear>(adaptor);
//
//
//	//============== using generator
//	miqs::phasor_step phase{ ratioSr };
//	auto generator = miqs::make_generator(wavetable, phase);
//	ptr_generate(std::begin(out), std::end(out), generator);
//
//
//	//====================================Sound Out
//	Miqs::SoundOut sndout;
//	auto& props = sndout.property;
//	props.chans = 1;
//	props.srate = outSamplerate;
//	props.samptype = sndSrcIn.property.samptype;
//	props.format = PSF_WAVE_EX;
//	props.chformat = MC_MONO;
//	sndout.createSoundFile("result_moore_changed_sr.wav");
//	sndout.writeDoubleFrames(out, out.size());
//
//
//}
//
//
//void miqs_test::funcs::fft()
//{
//
//	std::cout << std::setprecision(4) << std::fixed;
//
//	size_t size = 32;
//	miqs::array<sample_t> data(size);
//
//	auto begin = std::begin(data);
//	auto end = std::end(data);
//
//	miqs::phasor_rad phase{ 2, size };
//	miqs::generator<miqs::sine_wave, miqs::phasor_rad> gen{ phase };
//	ptr_generate(begin, end, gen);
//
//	std::copy(begin, end, std::ostream_iterator<sample_t>(std::cout, " "));
//	std::cout << "\n\n";
//
//
//	miqs::array<> twiddle(size);
//	miqs::fft_twiddle<sample_t>()(twiddle, size);
//
//	miqs::array<> fft_data(size);
//	miqs::real_fft<> fft;
//	miqs::real_ifft<> ifft;
//	ptr_copy(begin, end, ptr_begin(fft_data));
//
//	fft(std::begin(fft_data), std::end(fft_data), twiddle);
//
//	std::cout << "::FFT::\n";
//	std::copy(std::begin(fft_data), std::end(fft_data), std::ostream_iterator<sample_t>(std::cout, " "));
//	std::cout << "\n\n";
//
//
//	miqs::ptr_transform(miqs::ptr_2begin(fft_data), miqs::ptr_2end(fft_data), miqs::ptr_2begin(fft_data), miqs::cartesian_to_polar<>());
//
//
//
//
//	miqs::ptr_transform(std::begin(fft_data), std::end(fft_data), 2, std::begin(fft_data), 2, miqs::logarithmic_amplitude_20log<>());
//
//
//	std::copy(std::begin(fft_data), std::end(fft_data), std::ostream_iterator<sample_t>(std::cout, " "));
//	std::cout << "\n\n";	std::cout << "\n\n";
//	miqs::ptr_transform(std::begin(fft_data), std::end(fft_data), 2, std::begin(fft_data), 2, miqs::exponential_amplitude_exp20<>());
//
//
//
//
//	//
//
//	miqs::ptr_transform(miqs::ptr_2begin(fft_data), miqs::ptr_2end(fft_data), miqs::ptr_2begin(fft_data), miqs::polar_to_cartesian<>());
//
//
//
//	ifft(std::begin(fft_data), std::end(fft_data), twiddle);
//	std::cout << "::IFFT::\n";
//	std::copy(std::begin(fft_data), std::end(fft_data), std::ostream_iterator<sample_t>(std::cout, " "));
//	std::cout << "\n\n";
//
//}
//
//void miqs_test::funcs::biquadfilter()
//{
//
//
//	//std::cout << std::setprecision(4) << std::fixed;
//
//	size_t size = 512;
//	miqs::array<sample_t> data(size);
//
//	miqs::phasor_rad phase{ 440.0, 48000 };
//	miqs::generator<miqs::sine_wave, miqs::phasor_rad> gen{ phase };
//
//	ptr_generate(std::begin(data), std::end(data), gen);
//
//
//	miqs::array<sample_t> coeff_a(3), coeff_b(3), buff{ 0.0,0.0 };
//
//
//	// filtering
//	miqs::filter_info info;
//	info.bandwidth = 100;
//	info.cutoff_frequency = 1000;
//
//	//miqs::canonical_coefficients_calculator<miqs::canonical::second_order_lowpass, miqs::filter_info>{info}(std::begin(coeff_a), std::begin(coeff_b));
//	miqs::calculate_canonical_coefficients<miqs::canonical::second_order_lowpass>(info, std::begin(coeff_a), std::begin(coeff_b));
//
//
//	miqs::canonical_filter filter{ coeff_a, coeff_b, buff };
//	ptr_transform(std::begin(data), std::end(data), std::begin(data), filter);
//
//}



void miqs_test::funcs::delay()
{

	std::cout << std::fixed << std::setprecision(3);
	miqs::array<> data(12, 0.0);
	*ptr_begin(data) = 1.0;


	std::cout << "::Original::\n";
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	miqs::array<> dbuff(12, 0.0);
	miqs::delay<sample_t> d{ dbuff, dbuff.size(), 3 };
	ptr_transform(std::begin(data), std::end(data), std::begin(data), d);

	std::cout << "miqs::delay(3)\n";
	std::copy(ptr_begin(data), ptr_end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	d.set_delay(5);
	std::cout << "miqs::delay::set_delay(5)\n";
	ptr_transform(ptr_begin(data), ptr_end(data), ptr_begin(data), d);
	std::copy(ptr_begin(data), ptr_end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	data[0] = 1.0;
	std::cout << "::Current::\n";
	std::copy(ptr_begin(data), ptr_end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	miqs::fixed_delay<2> fixdelay;
	std::cout << "miqs::fixed_delay<2>\n";
	//ptr_transform(ptr_begin(data), ptr_end(data), ptr_begin(data), std::ref(fixdelay));
	fixdelay(ptr_begin(data), ptr_end(data), ptr_begin(data));
	std::copy(ptr_begin(data), ptr_end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	miqs::onesample_delay_f<> onedelay;
	std::cout << "miqs::one_sample_delay\n";
	ptr_transform(ptr_begin(data), ptr_end(data), ptr_begin(data), std::ref(onedelay));
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	data[0] = 1.0;
	std::cout << "::Current::\n";
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	double delay = 4.4;
	using MyFractionDelay = miqs::fraction_delay<miqs::interpolation::cosine>;
	miqs::array<> buff(120, 0.0);
	MyFractionDelay fdelay{ buff, buff.size(), delay };
	std::cout << "miqs::fraction_delay<miqs::interpolation::cosine>\n";
	//miqs::ptr_transform(std::begin(data), std::end(data), std::begin(data), fdelay);
	fdelay(std::begin(data), std::end(data), std::begin(data));
	std::copy(std::begin(data), std::end(data), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";
}




void miqs_test::funcs::ptr_normalize()
{


	std::string infilename{ "moore_guitar.wav" };
	std::string outfilename{ "result_moore_guitar.wav" };

	//===============================in
	auto sndSrcIn = Miqs::SoundIn::ReadSoundFile(infilename);
	auto sndSrcSize = sndSrcIn.size();
	auto inSamplerate = sndSrcIn.property.srate;
	size_t inCh = sndSrcIn.property.chans;


	// samples
	miqs::array<> in(sndSrcSize);
	miqs::array<> out(sndSrcSize);

	// read source file
	sndSrcIn.readDoubleFrames(in, in.size());



	//============properties
	size_t framesize = 512;

	size_t frame_count = sndSrcSize / 512;

	ptr_proxy_interleaved<> ptr{ in, (int)inCh };
	ptr_proxy_interleaved<> dstptr{ out, (int)inCh };

	sample_t max[8];
	for (size_t i{}; i < inCh; ++i)
	{
		max[i] = std::abs(*miqs::ptr_max(ptr.begin(i), ptr.at(sndSrcSize / inCh, i), inCh));
	}


	for (size_t i{}; i < frame_count; ++i, ptr += framesize, dstptr += framesize)
	{
		for (size_t c{}; c < inCh; ++c)
		{
			auto first = ptr.begin(c);
			auto last = ptr.at(framesize, c);
			auto dst = dstptr.begin(c);
			for (; first < last; first += inCh, dst += inCh)
			{
				*dst = *first / max[c];
			}
		}
	}


	//ptr_normalize_with_max(std::begin(in), std::end(in));

	//===============================out
	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = inCh;
	props.srate = inSamplerate;
	props.samptype = sndSrcIn.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	sndout.createSoundFile(outfilename);
	sndout.writeDoubleFrames(out, out.size());
}



void miqs_test::funcs::ptr_simple_panning(){


	std::string infilename{ "moore_guitar.wav" };
	std::string outfilename{ "result_moore_guitar.wav" };

	//===============================in
	auto sndSrcIn = Miqs::SoundIn::ReadSoundFile(infilename);
	auto sndSrcSize = sndSrcIn.size();
	auto inSamplerate = sndSrcIn.property.srate;
	auto inCh = sndSrcIn.property.chans;

	//================================out
	int outCh = 2;


	// samples
	miqs::array<> in(sndSrcSize);
	miqs::array<> out(sndSrcSize*outCh);

	// read source file
	sndSrcIn.readDoubleFrames(in, in.size());



	//============properties
	size_t framesize = 512;

	size_t frame_count = sndSrcSize / 512;

	ptr_proxy_interleaved<> dstptr{ out, outCh };
	auto * ptr = std::begin(in);

	auto pan = miqs::stereo_panning(miqs::degree_to_radian(30) , miqs::degree_to_radian(45));



	for (size_t i{};
		 i < frame_count;
		 ++i, ptr += framesize, dstptr += framesize)
	{
		miqs::ptr_transform(ptr, ptr + framesize, 1, dstptr.begin(0), outCh, miqs::multiplies_const<sample_t>(pan.first));
		miqs::ptr_transform(ptr, ptr + framesize, 1, dstptr.begin(1), outCh, miqs::multiplies_const<sample_t>(pan.second));
	}



	//===============================out
	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = outCh;
	props.srate = inSamplerate;
	props.samptype = sndSrcIn.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_STD;
	sndout.createSoundFile(outfilename);
	sndout.writeDoubleFrames(out, out.size() / outCh);
}







inline double event_value_at(event_point<double, type_struct<double>>* events,
							 size_t nevents,
							 double current_time)
{
	double value{ 1.0 };
	using EP = event_point<double, type_struct<double>>;

	size_t i;
	EP * l, *r;
	double dt;
	for (i = 1; i < nevents; ++i)
	{
		if (current_time <= events[i].point) break;
	}

	if (i == nevents) return events[nevents - 1].value;

	l = &events[i - 1];
	r = &events[i];

	dt = r->point - l->point;
	if (dt == 0.0) return r->value;

	// interpolation
	double frac = (current_time - l->point) / dt;
	value = l->value + ((r->value - l->value) * frac);

	return value;
}

void miqs_test::funcs::ptr_simple_panning_timeevents()
{

	std::string infilename{ "moore_guitar.wav" };
	std::string outfilename{ "result_moore_guitar.wav" };

	//===============================in
	auto sndSrcIn = Miqs::SoundIn::ReadSoundFile(infilename);
	auto sndSrcSize = sndSrcIn.size();
	auto inSamplerate = sndSrcIn.property.srate;
	auto inCh = sndSrcIn.property.chans;

	//================================out
	int outCh = 2;


	// samples
	miqs::array<> in(sndSrcSize);
	miqs::array<> out(sndSrcSize*outCh);

	// read source file
	sndSrcIn.readDoubleFrames(in, in.size());



	//============properties
	size_t framesize = 512;

	size_t frame_count = sndSrcSize / 512;

	ptr_proxy_interleaved<> dstptr{ out, outCh };
	auto * ptr = std::begin(in);



	constexpr double loud_pos = 1.0;
	std::pair<double, double> pan;


	using panning_event = event_point<double, type_struct<double>>;
	panning_event events[5];
	{
		events[0].point = 0.0; events[0].value = miqs::degree_to_radian(30);
		events[1].point = 1.2; events[1].value = miqs::degree_to_radian(-20);
		events[2].point = 1.7; events[2].value = miqs::degree_to_radian(-100);
		events[3].point = 2.9; events[3].value = miqs::degree_to_radian(100);
		events[4].point = 3.2; events[4].value = miqs::degree_to_radian(-70);
	}


	double tincr = 1.0 / inSamplerate;
	double currentSTime{};

	for (size_t i{};
		 i < frame_count;
		 ++i, ptr += framesize, dstptr += framesize)
	{
		for (size_t s{}; s < framesize; ++s, currentSTime += tincr)
		{
			pan = miqs::stereo_panning(event_value_at(events, 5, currentSTime), loud_pos);
			*dstptr.at(s, 0) = *(ptr + s)*pan.first;
			*dstptr.at(s, 1) = *(ptr + s)*pan.second;
		}
	}


	//===============================out
	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = outCh;
	props.srate = inSamplerate;
	props.samptype = sndSrcIn.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_STD;
	sndout.createSoundFile(outfilename);
	sndout.writeDoubleFrames(out, out.size() / outCh);
}








void miqs_test::funcs::ptr_extract_adjust_env()
{

	std::string infilename{ "moore_guitar.wav" };
	std::string outfilename{ "result_moore_guitar.wav" };

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
	double win_dur{ 0.100 };
	size_t winsize = static_cast<size_t>(win_dur * inSamplerate);

	//size_t n_tp{};
	double cur_time{};


	auto dst_ptr = std::begin(out);
	auto ptr = std::begin(in);
	auto ptr_end = std::end(in) - winsize;

	miqs::array<miqs::primitive_event_point<double, double>> values(static_cast<int>(((double)sndSrcSize / winsize) + 0.5));


	for (size_t i{}; ptr < ptr_end; cur_time += win_dur, ptr += winsize, ++i)
	{
		values[i].value = *miqs::ptr_max(ptr, ptr + winsize);
		values[i].point = cur_time;
	}

	//adjusting to out file.
	ptr = std::begin(in);



	size_t framesize = 512;
	size_t frame_count = sndSrcSize / 512;

	double tincr = 1.0 / inSamplerate;
	cur_time = 0.0;

	for (size_t i{};
		 i < frame_count;
		 ++i, ptr += framesize, dst_ptr += framesize)
	{
		for (size_t s{}; s < framesize; ++s, cur_time += tincr)
		{
			auto value = event_value_at(values, values.size(), cur_time);
			*(dst_ptr + s) = *(ptr + s)* value;
		}
	}




	//===============================out
	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = outCh;
	props.srate = inSamplerate;
	props.samptype = sndSrcIn.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_STD;
	sndout.createSoundFile(outfilename);
	sndout.writeDoubleFrames(out, out.size() / outCh);
}




void miqs_test::funcs::ptr_gen_wave()
{
	std::string infilename{ "moore_guitar.wav" };
	std::string outfilename{ "result_moore_guitar.wav" };

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


	//fftpolar_to_cepstrum, cepstrum_to_fftpolar


	//============properties


	//auto dst_ptr = std::begin(out);
	//auto ptr = std::begin(in);
	//auto ptr_end = std::end(in);
	//phase<double> d;

	miqs::phase_info<> phase;
	miqs::phase_helper<miqs::phase_info<>> phase_helper(phase);
	phase_helper.set_info(440.0, 44100);

	miqs::phase_pi_modular<miqs::phase_info<>> op(phase);
	auto gen = miqs::make_generator<miqs::osc::saw_wave>(op);
	miqs::ptr_generate(std::begin(out), std::end(out), gen);
	miqs::ptr_normalize(std::begin(out), std::end(out), 1.5);

	

	//===============================out
	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = outCh;
	props.srate = inSamplerate;
	props.samptype = sndSrcIn.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	sndout.createSoundFile(outfilename);
	sndout.writeDoubleFrames(out, out.size() / outCh);
}


void miqs_test::funcs::ptr_interpolator()
{

	//====================
	std::cout << std::setprecision(4);
	size_t datalength = 120;
	auto& resource = miqs::process::resource::get_instance();
	auto& table = resource.window_table(process::WINTYPE_HANNING);
	/*
	miqs::array<> table(10248);
	std::iota(std::begin(table), std::end(table), 0);
	*/
	miqs::phase_info<> info{ 0, (double)table.size() / datalength };
	miqs::phase_modular<miqs::phase_info<>> phase{ info , static_cast<double>(miqs::process::WavetableSize) };
	miqs::array<> data(datalength * 3);

	//miqs::ptr_table_lookup<miqs::interpolation::linear>(std::begin(data), std::end(data),
	//													std::begin(table), phase);
	miqs::ptr_table_lookup_rotating<miqs::interpolation::linear>(std::begin(data), std::end(data),
																 std::begin(table), std::end(table),
																 phase);

	std::copy(std::begin(data), std::end(data), std::ostream_iterator<double>(std::cout, " "));
	std::cout << "\n";

}