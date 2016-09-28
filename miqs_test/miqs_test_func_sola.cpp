#include "miqs_test.h"

using namespace miqs_test;
using namespace miqs;


void miqs_test::funcs::sola()
{
	//---------------------------------PROPERTIES
	size_t analysisHopSize = 256;
	size_t blockLength = 2048;
	double timeScalingFactor = 1.2;
	size_t synthesisHopSize = static_cast<size_t>(std::round(analysisHopSize * timeScalingFactor));
	size_t overlapInterval = synthesisHopSize / 2;

	// overlapInterval less than blockLength - synthesisHopSize
	overlapInterval = (overlapInterval >= blockLength - synthesisHopSize) ? blockLength - synthesisHopSize - 1 : overlapInterval;
	//--------------------------------------------
	std::vector<sample_t> grain(blockLength);


	//std::vector<sample_t> result(samples.)

	/* original samples */
	auto sndIn = Miqs::SoundIn::ReadSoundFile("x1.wav");
	std::vector<sample_t> samples(sndIn.size());
	sndIn.readDoubleFrames(samples.data(), samples.size());

	std::vector<sample_t> result;
	result.reserve(static_cast<size_t>(samples.size()*timeScalingFactor*timeScalingFactor));

	auto count = static_cast<size_t>(std::ceil((samples.size() - blockLength) / analysisHopSize)) + 1;

	// Fade in and out
	miqs::phasor_index phase;
	miqs::linear f_in({ 0.0,1.0 }, 0);
	miqs::linear f_out({ 1.0,0.0 }, 10);

	miqs::generator_ref<miqs::linear, miqs::phasor_index> fadein{ f_in, phase };
	miqs::generator_ref<miqs::linear, miqs::phasor_index> fadeout{ f_out, phase };


	// copy first block
	std::copy(std::begin(samples), std::begin(samples) + blockLength, std::back_inserter(result));

	for (size_t i{}; i < count; ++i)
	{
		std::copy(std::begin(samples) + (i*analysisHopSize),
				  std::begin(samples) + (blockLength + i*analysisHopSize),
				  std::begin(grain));

		// correlation
		std::vector<double> xcorr;
		miqs::xcorrelation(std::begin(grain), std::begin(grain) + overlapInterval,
						   std::end(result) - overlapInterval, 
						   //std::end(result),
						   std::back_inserter(xcorr));

		auto max_idx = std::distance(std::begin(xcorr), std::max_element(std::begin(xcorr), std::end(xcorr)));

		// ready fade in and out
		size_t fadesize{ overlapInterval - max_idx };
		f_in.set_size(fadesize);
		f_out.set_size(fadesize);

		// adjust fade in and out
		phase.reset();
		std::transform(std::begin(grain), std::begin(grain) + fadesize,
					   std::begin(grain),
					   miqs::make_multiplies_iter(miqs::make_generate_iterator(fadein)));

		phase.reset();
		std::transform(std::end(result) - fadesize - 1, std::end(result),
					   std::end(result) - fadesize - 1,
					   miqs::make_multiplies_iter(miqs::make_generate_iterator(fadeout)));


		std::transform(std::end(result) - fadesize - 1, std::end(result),
					   std::begin(grain),
					   std::end(result) - fadesize - 1, std::plus<sample_t>());

		std::copy(std::begin(grain) + fadesize, std::begin(grain) + overlapInterval + synthesisHopSize, std::back_inserter(result));

	}






	Miqs::SoundOut out;
	auto& props = out.property;
	props.chans = 1;
	props.srate = sndIn.property.srate;
	props.samptype = PSF_SAMP_IEEE_FLOAT;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	out.createSoundFile("X1_Extenedes_SOLA.wav");
	out.writeDoubleFrames(result.data(), result.size());
}