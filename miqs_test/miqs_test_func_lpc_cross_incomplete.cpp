#include "miqs_test.h"


using namespace miqs_test;
using namespace miqs;




void miqs_test::funcs::lpc_cross()
{

	std::cout << std::setprecision(5);



	/* original samples */
	auto sndSrc = Miqs::SoundIn::ReadSoundFile("moore_guitar.wav");
	auto sndEnv = Miqs::SoundIn::ReadSoundFile("Toms_diner.wav");

	std::vector<sample_t> src_x(sndSrc.size());
	std::vector<sample_t> env_x(sndEnv.size());

	sndSrc.readDoubleFrames(src_x.data(), src_x.size());
	sndEnv.readDoubleFrames(env_x.data(), env_x.size());

	//---- properties
	size_t blockLength{ 400 };
	size_t hopsize{ 160 };
	constexpr size_t env_order{ 20 };
	constexpr size_t src_order{ 6 };


	size_t outLength = std::min(src_x.size(), env_x.size());
	// out samples
	std::vector<sample_t> out;
	out.reserve(outLength);

	size_t nFrame{ (outLength - blockLength) / hopsize };


	auto src_begin = std::begin(src_x);
	auto src_end = src_begin + blockLength;
	auto env_begin = std::begin(env_x);
	auto env_end = env_begin + blockLength;

	std::vector<sample_t> src_lc(src_order + 1), src_rc(src_order + 1);
	std::vector<sample_t> env_lc(env_order + 1), env_rc(env_order + 1);



	std::vector<sample_t> excitation1(blockLength);

	//filters
	std::vector<double> delay(src_order, 0.0);
	miqs::all_zero_filter<src_order> src_anafilter(src_lc, delay);
	std::vector<double> delay2(env_order, 0.0);
	miqs::all_pole_filter<env_order> dst_synfilter(env_lc, delay2);

	for (size_t frame{}; frame < nFrame; ++frame)
	{
		if (frame == 100)
		{
			double a = 3;
			a++;
		}
		auto g_env = miqs::lpc_levinson_durbin_method(env_begin, env_end, env_lc, env_rc, env_order);
		auto g_src = miqs::lpc_levinson_durbin_method(src_begin, src_end, src_lc, src_rc, src_order);

		//gain?
		std::transform(std::begin(src_lc), std::end(src_lc), std::begin(src_lc), miqs::divides_const<sample_t>(g_src));
		std::transform(src_begin, src_end, std::begin(excitation1), src_anafilter);

		std::transform(std::begin(excitation1), std::end(excitation1), std::begin(excitation1), miqs::multiplies_const<sample_t>(g_env));
		std::transform(std::begin(excitation1), std::begin(excitation1) + hopsize, std::back_inserter(out), dst_synfilter);







		src_begin += hopsize;
		src_end += hopsize;
		env_begin += hopsize;
		env_end += hopsize;
	}


	//--------------
	auto m = std::max_element(std::begin(out), std::end(out));
	std::transform(std::begin(out), std::end(out), std::begin(out), miqs::divides_const<sample_t>(*m));



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