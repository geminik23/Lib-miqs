#include "miqs_test.h"



using namespace miqs_test;
using namespace miqs;

void miqs_test::funcs::cepstrum_cross()
{
	using SampleComplex = std::complex<sample_t>;


	std::cout << std::setprecision(5);


	//---- properties
	size_t blockLength{ 1024 };
	size_t hopsize{ 256 };
	constexpr size_t env_order{ 80 };
	constexpr size_t src_order{ 80 };

	/* original samples */
	auto sndSrc = Miqs::SoundIn::ReadSoundFile("moore_guitar.wav");
	auto sndEnv = Miqs::SoundIn::ReadSoundFile("Toms_diner.wav");
	auto sndSrcSize = sndSrc.size();
	auto sndEnvSize = sndEnv.size();

	size_t outLength = std::min(sndSrcSize, sndEnvSize);


	// read sound file
	std::vector<sample_t> src_x(sndSrcSize + (blockLength - (outLength%blockLength)));
	std::vector<sample_t> env_x(sndEnvSize + (blockLength - (outLength%blockLength)));

	sndSrc.readDoubleFrames(src_x.data(), sndSrcSize);
	sndEnv.readDoubleFrames(env_x.data(), sndEnvSize);

	std::fill(std::begin(src_x) + sndSrcSize, std::end(src_x), 0.0);
	std::fill(std::begin(env_x) + sndEnvSize, std::end(env_x), 0.0);

	// out samples
	std::vector<sample_t> out(outLength);

	// normalize
	std::transform(std::begin(src_x), std::end(src_x), std::begin(src_x),
				   miqs::divides_const<sample_t>(
					   std::abs(*std::max_element(std::begin(src_x), std::end(src_x)))
					   )
	);

	std::transform(std::begin(env_x), std::end(env_x), std::begin(env_x),
				   miqs::divides_const<sample_t>(
					   std::abs(*std::max_element(std::begin(env_x), std::end(env_x)))
					   )
	);

	// window
	std::vector<sample_t> win(blockLength);

	miqs::phasor_index wp;
	miqs::window::hamming ham(blockLength);
	std::generate(std::begin(win), std::end(win), miqs::make_generator(ham, wp));




	std::vector<sample_t> grain(blockLength);
	std::vector<sample_t> win_src(blockLength);
	std::vector<sample_t> win_env(blockLength);


	size_t pend = outLength - blockLength;

	auto src_begin = std::begin(src_x);
	auto src_end = src_begin + blockLength;
	auto env_begin = std::begin(env_x);
	auto env_end = env_begin + blockLength;


	// complex
	std::vector<SampleComplex> src_fft(blockLength);
	std::vector<SampleComplex> env_fft(blockLength);
	std::vector<SampleComplex> temp_fft(blockLength);

	miqs::multichannel_array_accessor<sample_t> acc_srcfft(reinterpret_cast<sample_t*>(src_fft.data()), 2, blockLength);
	miqs::multichannel_array_accessor<sample_t> acc_envfft(reinterpret_cast<sample_t*>(env_fft.data()), 2, blockLength);
	miqs::multichannel_array_accessor<sample_t> acc_tempfft(reinterpret_cast<sample_t*>(temp_fft.data()), 2, blockLength);


	miqs::transforms_forward_fft fft;
	miqs::transforms_backward_fft ifft;

	auto op_mag = miqs::make_cascades_functions<SampleComplex>(divides_const<SampleComplex>({ static_cast<sample_t>(blockLength / 2) ,0.0 }), miqs::cartesian_to_polar<>());
	auto op_rev_mag = miqs::make_cascades_functions<SampleComplex>(miqs::polar_to_cartesian<>(), miqs::multiplies_const<SampleComplex>({ static_cast<sample_t>(blockLength / 2) ,0 }));
	auto op_log = miqs::make_cascades_functions<sample_t>(miqs::plus_const<sample_t>(std::numeric_limits<sample_t>::epsilon()),
														  miqs::log<>());
	auto op_exp = miqs::exp<>();

	// liftering
	miqs::phasor_index liftPhase;

	std::vector<size_t> interval{ 0,1,src_order + 1 };
	std::vector<sample_t> weight{ 1.0, 2.0 };
	miqs::envelope::piecewise_constant<sample_t> srcpw{ std::begin(interval), std::end(interval), std::begin(weight) };
	interval[2] = env_order + 1;
	miqs::envelope::piecewise_constant<sample_t> envpw{ std::begin(interval), std::end(interval), std::begin(weight) };
	auto src_liftering = make_generator(srcpw, liftPhase);
	auto env_liftering = make_generator(envpw, liftPhase);


	size_t count{};
	for (size_t pin{};
		 pin < pend;
		 pin += hopsize, src_begin += hopsize, src_end += hopsize, env_begin += hopsize, env_end += hopsize, ++count)
	{
		std::transform(src_begin, src_end, std::begin(win), std::begin(win_src), std::multiplies<sample_t>());
		std::transform(env_begin, env_end, std::begin(win), std::begin(win_env), std::multiplies<sample_t>());


		miqs::fft_two_real_signal<sample_t>(std::begin(win_src), std::end(win_src), std::begin(win_env), std::begin(src_fft), std::begin(env_fft));

		//================Compute cepstrum
		std::transform(std::begin(src_fft), std::end(src_fft), std::begin(src_fft), op_mag);
		std::copy(std::begin(src_fft), std::end(src_fft), std::begin(temp_fft));

		std::transform(std::begin(env_fft), std::end(env_fft), std::begin(env_fft), op_mag);

		std::transform(miqs::begin(acc_srcfft, 0), miqs::end(acc_srcfft, 0), miqs::begin(acc_srcfft, 0), op_log);
		std::transform(miqs::begin(acc_envfft, 0), miqs::end(acc_envfft, 0), miqs::begin(acc_envfft, 0), op_log);

		std::fill(miqs::begin(acc_srcfft, 1), miqs::end(acc_srcfft, 1), 0.0);
		std::fill(miqs::begin(acc_envfft, 1), miqs::end(acc_envfft, 1), 0.0);

		ifft(src_fft);
		ifft(env_fft);

		//=======liftering
		liftPhase.reset();
		std::transform(miqs::begin(acc_srcfft, 0), miqs::end(acc_srcfft, 0), miqs::begin(acc_srcfft, 0),
					   miqs::make_multiplies_iter(make_generate_iterator(src_liftering)));
		liftPhase.reset();
		std::transform(miqs::begin(acc_envfft, 0), miqs::end(acc_envfft, 0), miqs::begin(acc_envfft, 0),
					   miqs::make_multiplies_iter(make_generate_iterator(env_liftering)));

		fft(src_fft);
		fft(env_fft);



		std::transform(miqs::begin(acc_envfft, 0), miqs::end(acc_envfft, 0), miqs::begin(acc_srcfft, 0), miqs::begin(acc_envfft, 0), std::minus<sample_t>());
		std::transform(miqs::begin(acc_envfft, 0), miqs::end(acc_envfft, 0), miqs::begin(acc_envfft, 0), op_exp);

		std::transform(miqs::begin(acc_tempfft, 0), miqs::end(acc_tempfft, 0), miqs::begin(acc_envfft, 0),
					   miqs::begin(acc_tempfft, 0),
					   std::multiplies<sample_t>());
		std::transform(std::begin(temp_fft), std::end(temp_fft), std::begin(temp_fft), op_rev_mag);


		ifft(temp_fft);

		std::transform(miqs::begin(acc_tempfft, 0), miqs::end(acc_tempfft, 0), std::begin(win), std::begin(grain), std::multiplies<sample_t>());

		std::transform(std::begin(out) + pin, std::begin(out) + pin + blockLength, std::begin(grain), std::begin(out) + pin, std::plus<sample_t>());

	}


	//--------------
	auto m = std::max_element(std::begin(out), std::end(out));
	std::transform(std::begin(out), std::end(out), std::begin(out), miqs::divides_const<sample_t>(0.98* std::abs(*m)));



	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = 1;
	props.srate = sndSrc.property.srate;
	props.samptype = sndSrc.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	sndout.createSoundFile("result_cross_cepstrum.wav");
	sndout.writeDoubleFrames(out.data(), out.size());



}