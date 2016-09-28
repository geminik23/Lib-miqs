#include "miqs_test.h"



using namespace miqs_test;
using namespace miqs;

void miqs_test::funcs::cepstrum()
{

	/* cepstrum_cross */

	std::cout << std::setprecision(4) << std::fixed;
	/////////////////////////
	size_t fftLength{ 128 };
	size_t cutvalue{ 10 };
	size_t offset{ 1000 };
	size_t halfLength{ fftLength / 2 };

	//////////////////////
	/* original samples */
	//////////////////////
	auto sndIn = Miqs::SoundIn::ReadSoundFile("la.wav");
	std::vector<sample_t> samples(sndIn.size());
	sndIn.readDoubleFrames(samples.data(), samples.size());

	// complex 
	std::vector<std::complex<sample_t>> fftcomp(fftLength);
	miqs::multichannel_array_accessor<sample_t> fftsamples(reinterpret_cast<sample_t *>(fftcomp.data()), 2, fftLength);
	std::vector<sample_t> frame_smp(fftLength);

	// window
	std::vector<sample_t> win(fftLength);

	// fill windows
	miqs::phasor_index wphase;
	miqs::window::hanning han{ fftLength };
	miqs::generator_ref<miqs::window::hanning, miqs::phasor_index> hangen{ han, wphase };
	std::generate(std::begin(win), std::end(win), hangen);


	auto s_begin = std::begin(samples) + offset;
	auto s_end = s_begin + fftLength;




	// windowing
	std::fill(miqs::begin(fftsamples, 1), miqs::end(fftsamples, 1), 0.0);
	std::transform(s_begin, s_end, std::begin(win), miqs::begin(fftsamples, 0), std::multiplies<sample_t>());

	// fft
	miqs::transforms_fft<true> fft;
	fft(fftcomp);

	//std::transform(std::begin(fftcomp), std::end(fftcomp),
	//			   std::begin(fftcomp),
	//			   miqs::divides_const<std::complex<sample_t>>(std::complex

	// cart to polar
	std::transform(std::begin(fftcomp), std::end(fftcomp), std::begin(fftcomp), miqs::cartesian_to_polar());


	std::transform(miqs::begin(fftsamples,0), miqs::end(fftsamples, 0), miqs::begin(fftsamples, 0),
				   miqs::make_cascades_functions<sample_t>(miqs::divides_const<sample_t>(halfLength),
														   miqs::plus_const<sample_t>(std::numeric_limits<sample_t>::epsilon()),
														   miqs::log10<>(),
														   miqs::multiplies_const<>(20)));


	std::cout << "::FFT magnitude::\n";
	std::copy(miqs::begin(fftsamples, 0), miqs::end(fftsamples, 0), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";

	// remove imaginary part
	std::copy(miqs::begin(fftsamples, 1), miqs::end(fftsamples, 1), std::begin(frame_smp));
	std::fill(miqs::begin(fftsamples, 1), miqs::end(fftsamples, 1), 0.0);

	// ifft
	miqs::transforms_fft<false> ifft;
	ifft(fftcomp);



	/////////////////////////////
	/////result is cepstrum//////
	/////////////////////////////

	//liftering : use envelope 
	//liftering variables
	std::vector<size_t> interval{ 0,1,cutvalue + 1 };
	std::vector<sample_t> weight{ 1.0, 2.0 };
	miqs::envelope::piecewise_constant<sample_t> pw{ std::begin(interval), std::end(interval), std::begin(weight) };

	miqs::phasor_index phase;
	auto gen = make_generator(pw, phase);
	miqs::generator_ref<miqs::envelope::piecewise_constant<sample_t>, miqs::phasor_index> genn(pw, phase);


	std::transform(miqs::begin(fftsamples, 0), miqs::end(fftsamples, 0), miqs::begin(fftsamples, 0),
				   miqs::make_multiplies_iter(make_generate_iterator(genn)));


	std::cout << "::Liftering Cepstrum::\n";
	std::copy(miqs::begin(fftsamples, 0), miqs::end(fftsamples, 0), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


	fft(fftcomp);


	std::cout << "::Result FFT::\n";
	std::copy(miqs::begin(fftsamples, 0), miqs::end(fftsamples, 0), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n";


}