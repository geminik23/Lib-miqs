#include "miqs_test.h"


using namespace miqs;
using namespace miqs_test;


void miqs_test::funcs::spectral_interpolation()
{

	/* spectral interpolation */
	// divide two signal to E[f] - excitation, H[f] - spectral envelope 

	using SampleComplex = std::complex<sample_t>;
	//=================================Properties
	std::string sound1{ "claire_oubli_voix.wav" };
	std::string sound2{ "claire_oubli_flute.wav" };

	auto sndIn1 = Miqs::SoundIn::ReadSoundFile(sound1);
	auto sndIn2 = Miqs::SoundIn::ReadSoundFile(sound2);


	size_t samplerate = static_cast<size_t>(sndIn1.property.srate);

	size_t analHopsize{ 512 },
		synHopSize{ analHopsize },
		sizeOfWin{ 2048 },
		cutoff_cue = 50;

	std::vector<sample_t> dump(sizeOfWin);
	//=================================Read Two Sound

	size_t sndIn1Size{ sndIn1.size() }, sndIn2Size{ sndIn2.size() };
	size_t l = std::min(sndIn1Size, sndIn2Size);
	size_t extra_l = (sizeOfWin - l%analHopsize);

	std::vector<sample_t> in1(sndIn1Size + extra_l), in2(sndIn1Size + extra_l);

	sndIn1.readDoubleFrames(in1.data(), in1.size());
	sndIn2.readDoubleFrames(in2.data(), in2.size());

	// property
	std::vector<sample_t> out(in1.size());


	//=================================Window
	std::vector<sample_t> analWindow(sizeOfWin), synWindow(sizeOfWin);
	miqs::phasor_index idxPhase;
	miqs::window::hanning han(sizeOfWin);
	auto winGen = miqs::make_generator(han, idxPhase);

	std::generate(std::begin(analWindow), std::end(analWindow), winGen);
	std::copy(std::begin(analWindow), std::end(analWindow), std::begin(synWindow));


	// normalize input samples
	miqs::normalize_with_max(std::begin(in1), std::end(in1));
	miqs::normalize_with_max(std::begin(in2), std::end(in2));
	//===============================
	std::vector<SampleComplex>
		grain1(sizeOfWin), grain2(sizeOfWin),
		spec1(sizeOfWin), spec2(sizeOfWin),
		spec(sizeOfWin);

	miqs::multichannel_array_accessor<sample_t>
		acc_grain1(miqs::data<sample_t>(grain1), 2, sizeOfWin),
		acc_grain2(miqs::data<sample_t>(grain2), 2, sizeOfWin),
		acc_spec1(miqs::data<sample_t>(spec1), 2, sizeOfWin),
		acc_spec2(miqs::data<sample_t>(spec2), 2, sizeOfWin),
		acc_spec(miqs::data<sample_t>(spec), 2, sizeOfWin);


	//======Cepstrum filter
	std::vector<size_t> interval{ 0,1,cutoff_cue + 1 };
	std::vector<sample_t> weight{ 0.5, 1.0 };
	miqs::envelope::piecewise_constant<sample_t> cepfilt{ std::begin(interval), std::end(interval), std::begin(weight) };
	auto gen_cepfilt = miqs::make_generator(cepfilt, idxPhase);

	//===========================================================================
	size_t pin{}, pout{}, pend = l - sizeOfWin;
	std::vector<sample_t> k(2);
	auto s1begin = std::begin(in1);
	auto s2begin = std::begin(in2);
	auto s1end = s1begin + sizeOfWin;
	auto s2end = s2begin + sizeOfWin;
	auto obegin = std::begin(out);
	auto oend = obegin + sizeOfWin;


	miqs::transforms_forward_fft fft;
	miqs::transforms_backward_fft ifft;


	auto op_log = miqs::make_cascades_functions<sample_t>(
		miqs::plus_const<sample_t>(std::numeric_limits<sample_t>::epsilon()),
		miqs::log<sample_t>()
		);
	auto op_mag = miqs::make_cascades_functions<SampleComplex>(divides_const<SampleComplex>({ static_cast<sample_t>(sizeOfWin / 2) ,0.0 }), miqs::cartesian_to_polar<>());
	auto op_rev_mag = miqs::make_cascades_functions<SampleComplex>(miqs::polar_to_cartesian<>(), miqs::multiplies_const<SampleComplex>({ static_cast<sample_t>(sizeOfWin / 2) ,0 }));


	while (pin < pend)
	{
		//=====ratio
		k[1] = ((double)pin / pend);
		k[0] = (1.0 - k[1]);

		//======extract input signal
		std::transform(s1begin, s1end, std::begin(analWindow), miqs::begin(acc_grain1, 0), std::multiplies<sample_t>());
		std::transform(s2begin, s2end, std::begin(analWindow), miqs::begin(acc_grain2, 0), std::multiplies<sample_t>());
		std::fill(miqs::begin(acc_grain1, 1), miqs::end(acc_grain1, 1), 0.0);
		std::fill(miqs::begin(acc_grain2, 1), miqs::end(acc_grain2, 1), 0.0);

		// fft
		fft(grain1);
		fft(grain2);
		std::transform(std::begin(grain1), std::end(grain1), std::begin(grain1), op_mag);
		std::transform(std::begin(grain2), std::end(grain2), std::begin(grain2), op_mag);


		//==================Computing Spectral Shape
		spec1 = grain1;
		spec2 = grain2;

		/* sample 1 */
		std::transform(miqs::begin(acc_spec1, 0), miqs::end(acc_spec1, 0), miqs::begin(acc_spec1, 0), op_log); //log
		std::fill(miqs::begin(acc_spec1, 1), miqs::end(acc_spec1, 1), 0.0);
		ifft(spec1);

		fft(spec1);
		std::transform(miqs::begin(acc_spec1, 0), miqs::end(acc_spec1, 0), miqs::begin(acc_spec1, 0), miqs::multiplies_const<sample_t>(2.0));

		/* sample 2 */
		std::transform(miqs::begin(acc_spec2, 0), miqs::end(acc_spec2, 0), miqs::begin(acc_spec2, 0), op_log); //log
		std::fill(miqs::begin(acc_spec2, 1), miqs::end(acc_spec2, 1), 0.0);
		ifft(spec2);

		fft(spec2);
		std::transform(miqs::begin(acc_spec2, 0), miqs::end(acc_spec2, 0), miqs::begin(acc_spec2, 0), miqs::multiplies_const<sample_t>(2.0));


		std::fill(std::begin(spec), std::end(spec), SampleComplex(0, 0));

		miqs::linear_combination_with_func(miqs::begin(acc_spec, 0), miqs::end(acc_spec, 0), miqs::begin(acc_spec, 0),
										   std::begin(k),
										   miqs::exp<>(),
										   miqs::begin(acc_spec1, 0), miqs::begin(acc_spec2, 0));


		//====================get output
		std::transform(miqs::begin(acc_spec1, 0), miqs::end(acc_spec1, 0), miqs::begin(acc_spec1, 0), miqs::exp<>());
		std::transform(miqs::begin(acc_spec2, 0), miqs::end(acc_spec2, 0), miqs::begin(acc_spec2, 0), miqs::exp<>());



		std::fill(std::begin(dump), std::end(dump), 0.0);

		std::transform(miqs::begin(acc_grain1, 0), miqs::end(acc_grain1, 0), miqs::begin(acc_spec1, 0), miqs::begin(acc_grain1, 0), std::divides<sample_t>());
		std::transform(miqs::begin(acc_grain2, 0), miqs::end(acc_grain2, 0), miqs::begin(acc_spec2, 0), miqs::begin(acc_grain2, 0), std::divides<sample_t>());

		miqs::linear_combination(std::begin(dump), std::end(dump), miqs::begin(acc_grain1, 0),
								 std::begin(k),
								 miqs::begin(acc_grain1, 0), miqs::begin(acc_grain2, 0));


		std::transform(miqs::begin(acc_grain1, 0), miqs::end(acc_grain1, 0), miqs::begin(acc_spec, 0), miqs::begin(acc_grain1, 0), std::multiplies<sample_t>());
		//for (size_t i{}; i < sizeOfWin; ++i)
		//{
		//	auto t = grain1[i].real() / spec1[i].real();
		//	auto t2 = grain2[i].real() / spec2[i].real();

		//	grain1[i].real((t * k[0] + t2 * k[1])*spec[i].real());
		//}

		std::transform(std::begin(grain1), std::end(grain1), std::begin(grain1), op_rev_mag);
		ifft(grain1);

		// synth
		std::transform(miqs::begin(acc_grain1, 0), miqs::end(acc_grain1, 0), std::begin(synWindow), miqs::begin(acc_grain1, 0), std::multiplies<sample_t>());
		std::transform(obegin, oend, miqs::begin(acc_grain1, 0), obegin, std::plus<sample_t>());



		//==============================
		s1begin += analHopsize;
		s2begin += analHopsize;
		s1end += analHopsize;
		s2end += analHopsize;
		obegin += synHopSize;
		oend += synHopSize;


		pin += analHopsize;
		pout += synHopSize;

	}


	// normalize out samples
	miqs::normalize_with_max(std::begin(out), std::end(out));

	//=================================Save to file
	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = 1;
	props.srate = samplerate;
	props.samptype = sndIn1.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	sndout.createSoundFile("result_spec_interp.wav");
	sndout.writeDoubleFrames(out.data(), out.size());

}