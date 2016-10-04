#pragma once
#include "miqs_test.h"

using namespace miqs_test;
using namespace miqs;







void _spectrogram(std::vector<sample_t>& samples, size_t start, size_t step, std::vector<sample_t>& window)
{
	miqs::transforms_fft<true> fft;

	size_t count = (samples.size() - start) / step;
	size_t analysis_size = window.size();
	std::vector<sample_t> segment(analysis_size);
	std::vector<sample_t> result(analysis_size);
	std::vector<std::complex<sample_t>> ana_segment(analysis_size);

	auto todbSpectrum = [analysis_size](sample_t&v) ->double{
		return miqs::log10_((v / (analysis_size / 2.0)), 20.0);
	};

	auto s_begin = std::begin(samples) + start;
	auto s_end = s_begin + analysis_size;


	for (size_t i{}; i < count; i++, s_begin = s_end, s_end = s_begin + analysis_size)
	{
		// get segment
		std::transform(s_begin, s_end, std::begin(window), std::begin(segment), std::multiplies<sample_t>());
	
		// copy to 
		miqs::complex_copy_from_single<real_part>(std::begin(segment), std::end(segment), std::begin(ana_segment));

		fft(ana_segment);
		
		// cartesian to polar
		std::transform(std::begin(ana_segment), std::end(ana_segment), std::begin(ana_segment), miqs::cartesian_to_polar<>());

		miqs::complex_copy_to_single<real_part>(std::begin(ana_segment), std::end(ana_segment), std::begin(result));
		
		// to db Spectrum
		std::transform(std::begin(result), std::end(result), std::begin(result), todbSpectrum);

		// print
		std::copy(std::begin(result), std::end(result), std::ostream_iterator<sample_t>(std::cout, " "));
		std::cout << "\n\n";
	}

	




}

#include <iomanip>
void miqs_test::funcs::spectrogram()
{
	std::cout << std::setprecision(4) << std::fixed;

	// arguments
	std::string filename{"a11wlk01.wav"};
	size_t sndSampleCount{};
	constexpr size_t windowSize = 512;
	size_t stepSize = windowSize/2;

	// sound in
	//Miqs::SoundFileIn in{ filename.c_str() };
	//sndSampleCount = in.GetNumberOfSamples();
	
	// init sampleBuffer
	std::vector<miqs::sample_t> samples(sndSampleCount);
	//in.GetSoundFileSamplesAt(std::begin(samples), std::end(samples),0);
	

	// fill window
	std::vector<miqs::sample_t> window(windowSize);
	
	miqs::phasor_index phase{ windowSize };
	miqs::generator<miqs::window::hanning_f<windowSize>, decltype(phase)> hann{phase};
	std::generate(std::begin(window), std::end(window), hann);


	_spectrogram(samples, 0, stepSize, window);

}
