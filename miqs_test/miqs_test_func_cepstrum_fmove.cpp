#include "miqs_test.h"



using namespace miqs_test;
using namespace miqs;

void miqs_test::funcs::cepstrum_formant_move()
{
	/* formant move cepstrum*/

	using SampleComplex = std::complex<sample_t>;

	std::cout << std::setprecision(4) << std::fixed;
	////////////////////////
	sample_t warpingCoef = 2.0;
	size_t analysisHopsize = 512;
	size_t synthesisHopsize = analysisHopsize;
	size_t sWin = 2048;
	size_t halfSWin = sWin / 2;
	size_t order = 50;
	sample_t r = 0.76;

	// window(Hanning)
	std::vector<sample_t> window(sWin);
	miqs::phasor_index idxPhase;
	miqs::window::hanning han(sWin);
	auto winGen = miqs::make_generator(han, idxPhase);
	std::generate(std::begin(window), std::end(window), winGen);


	//////////////////////
	/* original samples */
	//////////////////////
	auto sndIn = Miqs::SoundIn::ReadSoundFile("la.wav");
	size_t sndInSize = sndIn.size();
	std::vector<sample_t> samples(sndInSize + (sWin - sndInSize%analysisHopsize));
	std::vector<sample_t> out(samples.size(), 0.0);

	sndIn.readDoubleFrames(samples.data(), samples.size());

	// normalize soundIn
	miqs::normalize_with_max(std::begin(samples), std::end(samples));


	//  interpolation
	miqs::circular_adaptor<std::vector<sample_t>, false> adaptor{ samples };

	auto linWavetable = miqs::make_wavetable<miqs::interpolation::linear>(adaptor);


	// cepstrum filter
	std::vector<size_t> interval{ 0,1, order + 1 };
	std::vector<sample_t> weight{ 0.5, 1.0 };
	miqs::envelope::piecewise_constant<sample_t> srcpw{ std::begin(interval), std::end(interval), std::begin(weight) };
	auto fil_gen = miqs::make_generator(srcpw, idxPhase);


	std::vector<double> warpingIndice(sWin);
	size_t idx{};
	std::generate(std::begin(warpingIndice), std::end(warpingIndice),
				  [&idx, warpingCoef]() {
		return (idx++*warpingCoef) + 1;
	});


	size_t pin{ 0 },
		pout{ 0 },
		pend{ sndInSize - sWin };

	std::vector<SampleComplex> grain(sWin);
	std::vector<SampleComplex> com_fft1(sWin);
	std::vector<SampleComplex> com_fft2(sWin);


	miqs::multichannel_array_accessor<sample_t> acc_grain(reinterpret_cast<sample_t*>(grain.data()), 2, sWin);
	miqs::multichannel_array_accessor<sample_t> acc_fft1(reinterpret_cast<sample_t*>(com_fft1.data()), 2, sWin);
	miqs::multichannel_array_accessor<sample_t> acc_fft2(reinterpret_cast<sample_t*>(com_fft2.data()), 2, sWin);


	auto sbegin = std::begin(samples);
	auto send = std::begin(samples) + sWin;
	auto obegin = std::begin(out);
	auto oend = std::begin(out) + sWin;

	miqs::transforms_forward_fft fft;
	miqs::transforms_backward_fft ifft;

	// operators
	auto op_log_mag = miqs::make_cascades_functions<sample_t>(miqs::plus_const<sample_t>(std::numeric_limits<sample_t>::epsilon()),
															  miqs::log<>());
	while (pin < pend)
	{
		//=============== fill samples
		std::transform(sbegin, send, std::begin(window), miqs::begin(acc_grain, 0), std::multiplies<sample_t>());
		std::fill(miqs::begin(acc_grain, 1), miqs::end(acc_grain, 1), 0.0);
		std::transform(std::begin(warpingIndice), std::end(warpingIndice),
					   miqs::begin(acc_fft2, 0),
					   miqs::make_cascades_functions<sample_t>(miqs::plus_const<sample_t>(static_cast<sample_t>(pin)), linWavetable));
		std::transform(miqs::begin(acc_fft2, 0), miqs::end(acc_fft2, 0), std::begin(window), miqs::begin(acc_fft2, 0), std::multiplies<sample_t>());
		std::fill(miqs::begin(acc_fft2, 1), miqs::end(acc_fft2, 1), 0.0);

		//==============fft
		fft(grain);
		fft(com_fft2);


		//=================== magnitude and log 
		std::transform(std::begin(grain), std::end(grain), std::begin(grain), miqs::divides_const<SampleComplex>(SampleComplex(halfSWin, 0.0)));
		std::transform(std::begin(grain), std::end(grain), std::begin(grain), miqs::cartesian_to_polar<>());

		std::copy(std::begin(grain), std::end(grain), std::begin(com_fft1));
		std::transform(miqs::begin(acc_fft1, 0), miqs::end(acc_fft1, 0), miqs::begin(acc_fft1, 0), op_log_mag);


		std::transform(std::begin(com_fft2), std::end(com_fft2), std::begin(com_fft2), miqs::divides_const<SampleComplex>(SampleComplex(halfSWin, 0.0)));
		std::transform(std::begin(com_fft2), std::end(com_fft2), std::begin(com_fft2), miqs::cartesian_to_polar<>());
		std::transform(miqs::begin(acc_fft2, 0), miqs::end(acc_fft2, 0), miqs::begin(acc_fft2, 0), op_log_mag);



		//===================
		std::transform(miqs::begin(acc_fft2, 0), miqs::end(acc_fft2, 0), miqs::begin(acc_fft1, 0), miqs::begin(acc_fft2, 0), std::minus<sample_t>());
		std::fill(miqs::begin(acc_fft2, 1), miqs::end(acc_fft2, 1), 0.0);

		//================================ get cepstrum
		ifft(com_fft2);
		idxPhase.reset();
		std::transform(miqs::begin(acc_fft2, 0), miqs::end(acc_fft2, 0), miqs::begin(acc_fft2, 0),
					   miqs::make_multiplies_iter(miqs::make_generate_iterator(fil_gen)));

		fft(com_fft2);

		std::transform(miqs::begin(acc_fft2, 0), miqs::end(acc_fft2, 0), miqs::begin(acc_fft2, 0),
					   miqs::make_cascades_filter(miqs::multiplies_const<sample_t>(2.0), miqs::exp<>()));



		std::transform(miqs::begin(acc_grain, 0), miqs::end(acc_grain, 0), miqs::begin(acc_fft2, 0), miqs::begin(acc_grain, 0),
					   std::multiplies<sample_t>());

		std::transform(std::begin(grain), std::end(grain), std::begin(grain), miqs::polar_to_cartesian<>());
		std::transform(std::begin(grain), std::end(grain), std::begin(grain), miqs::multiplies_const<SampleComplex>(SampleComplex(halfSWin, 0.0)));

		ifft(grain);


		//window and copy to out sample

		std::transform(miqs::begin(acc_grain, 0), miqs::end(acc_grain, 0), std::begin(window), miqs::begin(acc_grain, 0), std::multiplies<sample_t>());
		std::transform(obegin, oend, miqs::begin(acc_grain, 0), obegin, std::plus<sample_t>());


		//====================================
		pin += analysisHopsize;
		pout += synthesisHopsize;

		sbegin += analysisHopsize;
		send += analysisHopsize;

		obegin += synthesisHopsize;
		oend += synthesisHopsize;

	}

	//normalize
	auto m = std::max_element(std::begin(out), std::end(out));
	std::transform(std::begin(out), std::end(out), std::begin(out),
				   miqs::make_cascades_filter(miqs::divides_const<sample_t>(std::abs(*m)), miqs::multiplies_const<sample_t>(r)));





	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = 1;
	props.srate = sndIn.property.srate;
	props.samptype = sndIn.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	sndout.createSoundFile("la_fmove.wav");
	sndout.writeDoubleFrames(out.data(), sndInSize);
}