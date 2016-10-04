
#include "miqs_test.h"

using namespace miqs_test;
using namespace miqs;


void miqs_test::funcs::lpc_analysis()
{
	std::cout << std::setprecision(5);
	/////////////////////////
	size_t blockLength{ 512 };
	constexpr size_t predictionOrder = 10;

	/////////////////////////
	/* original samples */
	auto sndIn = Miqs::SoundIn::ReadSoundFile("Ah.wav");
	std::vector<sample_t> samples(sndIn.size());
	sndIn.readDoubleFrames(samples.data(), samples.size());

	std::vector<sample_t> residual;
	residual.reserve(samples.size());

	unsigned int sampleRate = sndIn.property.srate;






	std::vector<sample_t> hamming(1024);

	miqs::phasor_index phase;
	miqs::generator<miqs::window::hamming_f<1024>, miqs::phasor_index> gen_ham{ phase };
	std::generate(std::begin(hamming), std::end(hamming), gen_ham);






	std::vector<double> p(predictionOrder + 1);
	std::vector<double> r(predictionOrder + 1);
	std::vector<double> delay(predictionOrder, 0.0);

	miqs::all_zero_filter<predictionOrder> anafilter(p, delay);
	std::vector<double> delay2(predictionOrder, 0.0);
	miqs::all_pole_filter<predictionOrder> synfilter(p, delay2);

	size_t nFrame = samples.size() / blockLength;

	std::vector<sample_t> win_x(blockLength);
	std::vector<sample_t> temp_result(blockLength);

	auto s_begin = std::begin(samples);
	auto s_end = s_begin + blockLength;
	for (size_t fr{}; fr < nFrame - 1; ++fr)
	{

		auto gain = miqs::lpc_levinson_durbin_method(s_begin, s_end, p, r, predictionOrder);
		std::transform(s_begin, s_end, std::begin(temp_result), anafilter);


		//std::transform(std::begin(temp_result), std::end(temp_result), std::begin(temp_result), synfilter);

		//std::transform(s_begin, s_end, std::begin(temp_result), std::back_inserter(residual), std::minus<sample_t>());
		std::copy(std::begin(temp_result), std::end(temp_result), std::back_inserter(residual));

		s_begin = s_end;
		s_end = s_end + blockLength;
	}


	Miqs::SoundOut out;
	auto& props = out.property;
	props.chans = 1;
	props.srate = sndIn.property.srate;
	props.samptype = PSF_SAMP_IEEE_FLOAT;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	out.createSoundFile("Ah_residual.wav");
	out.writeDoubleFrames(residual.data(), residual.size());
}