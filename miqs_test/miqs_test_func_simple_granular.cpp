#include "miqs_test.h"


using namespace miqs_test;
using namespace miqs;



void miqs_test::funcs::simple_granular()
{

	//-----------Properties
	size_t ng = 8000;
	size_t maxL = 200, minL = 50, 
		fade = 20;
	(void)fade;
	std::random_device rd;
	std::default_random_engine re{ rd() };
	std::uniform_int_distribution<> dist_int;
	std::uniform_real_distribution<> dist_real;

	auto irandom = [&dist_int, &re]() {
		return dist_int(re);
	};

	auto frandom = [&dist_real, &re]() {
		return dist_real(re);
	};

	//-----------SoundIn
	auto sndIn = Miqs::SoundIn::ReadSoundFile("x1.wav");
	std::vector<sample_t> samples(sndIn.size());
	sndIn.readDoubleFrames(samples.data(), samples.size());

	std::vector<sample_t> out_sample(samples.size());
	//-------------------

	// informations
	std::vector<size_t> gl(ng);
	std::vector<size_t> gposi(ng);
	std::vector<size_t> gposo(ng);
	std::vector<double> ga(ng);

	std::uniform_int_distribution<>::param_type par;

	par._Init(0, maxL - minL);
	dist_int.param(par);
	std::generate(std::begin(gl), std::end(gl), irandom);

	par._Init(0, samples.size() - maxL);
	dist_int.param(par);
	std::generate(std::begin(gposi), std::end(gposi), irandom);
	std::generate(std::begin(gposo), std::end(gposo), irandom);

	std::generate(std::begin(ga), std::end(ga), frandom);


	// synthesis
	for (size_t i{}; i < ng; ++i)
	{
		auto begin = std::begin(samples) + gposi[i];
		auto end = begin + gl[i];
		auto dst = std::begin(out_sample) + gposo[i];

		// TODO how to envelopes
		//std::transform(begin, end, dst, dst, std::plus<>());
		//std::transform(begin, end, dst, dst, miqs::bind_func_2arg<LEFT>(std::plus<sample_t>(), miqs::multiplies_const<sample_t>(ga[i])));
		//std::transform(begin, end, dst, dst, [i, &ga](auto& v1, auto&v2) { return ga[i] * v1 + v2;});
	}

	//normalize
	auto maxe = std::max_element(std::begin(out_sample), std::end(out_sample));
	std::transform(std::begin(out_sample), std::end(out_sample), std::begin(out_sample), miqs::divides_const<double>(*maxe));




	//-----------SoundOut
	Miqs::SoundOut out;
	auto& props = out.property;
	props.chans = 1;
	props.srate = sndIn.property.srate;
	props.samptype = PSF_SAMP_IEEE_FLOAT;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	out.createSoundFile("result_simple_granular.wav");
	out.writeDoubleFrames(out_sample.data(), out_sample.size());
}