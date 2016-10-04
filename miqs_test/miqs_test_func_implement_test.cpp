
#include "miqs_test.h"
#include "miqs_temp_impl.h"

using namespace miqs_test;
using namespace miqs;


static void pitch_tracker_fft();

static void pitch_shift_with_formant_preservation();

static void pitch_trackings();

void miqs_test::funcs::implement_test()
{
	

	//pitch_tracker_fft();
	//pitch_shift_with_formant_preservation();
}



static void pitch_trackings()
{

	constexpr int ALGO_YIN = 0, ALGO_LONGTERM = 1;
	int tracking_method{};

	switch (tracking_method)
	{
	case ALGO_YIN:
		//miqs::pitch_track_yin();
		break;
	case ALGO_LONGTERM:
		break;
	}


}


static void pitch_shift_with_formant_preservation() // to implement this function require information about Phase Vocoder.
{

}



struct find_pitch_fft
{
	template <typename _ContSample, typename _ContWindow>
	void operator()(_ContSample const& sample, _ContWindow const& window)
	{

		this->result_idx.clear();
		this->result_corrected_freq.clear();
		this->result_estimated_freq.clear();
		//------------ copy samples
		miqs::multichannel_array_accessor<sample_t> acc_s1(miqs::data<sample_t>(signal1), 2, fft_length);
		miqs::multichannel_array_accessor<sample_t> acc_s2(miqs::data<sample_t>(signal2), 2, fft_length);

		auto s_begin = std::begin(sample);

		std::transform(s_begin, s_begin + fft_length, std::begin(window),
					   miqs::begin(acc_s1, 0), std::multiplies<sample_t>());
		std::transform(s_begin + fft_hopsize, s_begin + fft_hopsize + fft_length, std::begin(window),
					   miqs::begin(acc_s2, 0), std::multiplies<sample_t>());

		std::fill(miqs::begin(acc_s1, 1), miqs::end(acc_s1, 1), 0.0);
		std::fill(miqs::begin(acc_s2, 1), miqs::end(acc_s2, 1), 0.0);

		//--- get fft
		miqs::fft_two_real_signal<sample_t>(
			miqs::begin(acc_s1, 0), miqs::end(acc_s1, 0),
			miqs::begin(acc_s2, 0),
			std::begin(signal1), std::begin(signal2)
			);

		//--- to db(magnitude) and phase
		auto op = miqs::bind_complex_func_1arg<RESULT, real_part>(
			miqs::cartesian_to_polar<>(),
			miqs::make_cascades_functions<sample_t>(miqs::divides_const<sample_t>(static_cast<sample_t>(fft_length/2)), miqs::logarithmic_amplitude_20log<>())
			); // combine operators

		std::transform(std::begin(signal1), std::end(signal1), std::begin(signal1), op);
		std::transform(std::begin(signal2), std::end(signal2), std::begin(signal2), op);

		// resize
		this->signal1.resize(kp_max + 1);
		this->signal2.resize(kp_max + 1);
		acc_s1.set_size(kp_max + 1);
		acc_s2.set_size(kp_max + 1);

		//------------find max location
		fml(miqs::begin(acc_s1, 0), miqs::end(acc_s1, 0));
		// result is  fml.idx
		// find max from -> fml.idx;

		sample_t v_max{};
		for (auto&& i : fml.idx)
		{
			v_max = std::max(v_max, acc_s1(0, i));
		}
		
		std::vector<size_t> ii;
		ii.reserve(kp_max+1);

		for (size_t i{}; i < fml.idx.size(); ++i)
		{
			if (acc_s1(0, fml.idx[i]) - v_max > - ((int)threshold))
				ii.push_back(i);
		}


		//==============
		double maxerr = (double)fft_hopsize / fft_length * 1.2;
		size_t k;
		double phi1, phi2_t, phi2, phi2_err, phi2_unwrap, dphi;
		for (auto&& i : ii)
		{
			k = fml.idx[i] - 1; // fft bin with maximum
			phi1 = acc_s1(1, k); // max phase diff error/pi
			phi2_t = phi1 + 2 * miqs::Pi / fft_length*k*fft_hopsize; // expected target phase after hop size
			phi2 = acc_s2(1, k);
			// puts an arvitrary phase value into [-pi,pi]
			phi2_err = miqs::phase_wrapping(phi2 - phi2_t);
			phi2_unwrap = phi2_t + phi2_err;
			dphi = phi2_unwrap - phi1; // phase diff
			if (k > kp_min && (std::abs(phi2_err) / miqs::Pi) < maxerr)
			{
				result_idx.push_back(k);
				result_corrected_freq.push_back(dphi / (2 * miqs::Pi*dt));
				result_estimated_freq.push_back(k*df);
			}

		}


	}


	size_t samplerate{ 48000 };
	size_t fft_length{ 512 };
	size_t fft_hopsize{ 1 };
	size_t threshold{ 50 };
	double freq_min{ 50 };
	double freq_max{ 800 };

	void update()
	{
		dt = (double)fft_hopsize / samplerate;
		df = (double)samplerate / fft_length;
		kp_min = (size_t)std::round(freq_min / df);
		kp_max = (size_t)std::round(freq_max / df);

		signal1.resize(fft_length);
		signal2.resize(fft_length);
	}


	std::vector<size_t> result_idx;
	std::vector<double> result_estimated_freq;
	std::vector<double> result_corrected_freq;

private:

	find_max_locations<sample_t> fml;

	double dt; // time diff between FFTs
	double df; // freq resolution
	size_t kp_min;
	size_t kp_max;

	std::vector<std::complex<sample_t>> signal1;
	std::vector<std::complex<sample_t>> signal2;


};


static void pitch_tracker_fft()//357
{
	/* Pitch Tracker based on FFT analysis */

	//========================================PROPERTIES
	std::string sndFileName{ "Toms_diner.wav" };
	size_t n0{ 2000 }, n1{ 210000 };

	size_t fftLength{ 1024 },
		hopSizeFFT{ 1 }, //  for pitch estimation
		hopSizeTime{ 200 },
		threshold{ 50 }; // threshold for fft maxima

	double freqMin = 50, freqMax = 800;
	double threshold_voiceDectect = 1.05; // deviation of pitch from mean value


	size_t signalLength{ n1 - n0 - 1 + hopSizeFFT }; // signal length
	size_t blocks = static_cast<size_t>(std::floor((double)signalLength / hopSizeTime));

	signalLength = (blocks - 1)*hopSizeFFT + fftLength + hopSizeTime;
	n1 = n0 + signalLength;


	// Result Block
	std::vector<sample_t> pitches(blocks);

	//===============================Hanning Window
	std::vector<sample_t> window(fftLength);
	miqs::phasor_index idxPhase;
	miqs::window::hanning han(fftLength);
	auto winGen = miqs::make_generator(han, idxPhase);
	std::generate(std::begin(window), std::end(window), winGen);




	//===============================SampleSound
	auto sndIn = Miqs::SoundIn::ReadSoundFile(sndFileName);
	size_t sndInSize = sndIn.size();
	(void)sndInSize;
	size_t samplerate = sndIn.property.srate;
	std::vector<sample_t> in(n1 - n0);

	{// dump prev samples [0~n0)
		std::vector<sample_t> dump(n0);
		sndIn.readDoubleFrames(dump.data(), dump.size());
	}
	sndIn.readDoubleFrames(in.data(), in.size());


	std::vector<sample_t> blockSamples(fftLength + hopSizeFFT);


	auto ibegin = std::begin(in);
	auto iend = ibegin + fftLength + hopSizeFFT;

	find_pitch_fft find_pitch;
	find_pitch.fft_hopsize = hopSizeFFT;
	find_pitch.fft_length = fftLength;
	find_pitch.freq_max = freqMax;
	find_pitch.freq_min = freqMin;
	find_pitch.samplerate = samplerate;
	find_pitch.threshold = threshold;

	find_pitch.update();

	//=======================================pitch extraction per block
	for (size_t i{}; i < blocks; ++i, ibegin += hopSizeTime, iend += hopSizeTime)
	{
		// copy from sample to block
		std::copy(ibegin, iend, std::begin(blockSamples));

		// find pitch using fft
		find_pitch(blockSamples, window);
		
		//take candidate
		if (find_pitch.result_corrected_freq.size() != 0)
			pitches[i] = find_pitch.result_corrected_freq[0];
		else pitches[i] = 0.0;

	}

	//========================================post-processing
	//TODO 357

	//========================================mirror beginning and end for 'non-causal' filtering

	
	//========================================voiced/unvoiced detection





}