#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;


void miqs_test::funcs::convert_samplerate() {

	/* change samplerate */
	//==================================Sound Sample
	auto sndSrcIn = Miqs::SoundIn::ReadSoundFile("moore_guitar.wav");
	auto sndSrcSize = sndSrcIn.size();
	auto inSamplerate = sndSrcIn.property.srate;

	int outSamplerate = inSamplerate == 48000 ? 44100 : 48000;
	double ratioSr = (double)inSamplerate / outSamplerate;


	// samples
	std::vector<sample_t> in(sndSrcSize);
	std::vector<sample_t> out(static_cast<size_t>(ratioSr*sndSrcSize + 0.5));

	// read source file
	sndSrcIn.readDoubleFrames(in.data(), in.size());

	//====================================

	miqs::circular_adaptor<std::vector<sample_t>, false> adaptor{ in };
	miqs::wavetable<miqs::interpolation::linear, decltype(adaptor)> wavetable(adaptor);


	//============== using generator
	miqs::phasor_step phase{ ratioSr };
	auto generator = miqs::make_generator(wavetable, phase);
	std::generate(std::begin(out), std::end(out), generator);


	//====================================Sound Out
	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = 1;
	props.srate = outSamplerate;
	props.samptype = sndSrcIn.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	sndout.createSoundFile("result_moore_changed_sr.wav");
	sndout.writeDoubleFrames(out.data(), out.size());

}

