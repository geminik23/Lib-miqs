
#include "miqs_test.h"
#include "miqs_temp_impl.h"

using namespace miqs_test;
using namespace miqs;


static void pitch_tracker_fft();
static void pitch_shift_with_formant_preservation();


void miqs_test::funcs::implement_test()
{ 
	//pitch_tracker_fft();
	//pitch_shift_with_formant_preservation();

}

static void pitch_shift_with_formant_preservation()
{
	
}

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


	size_t signalLength{n1 - n0 - 1 + hopSizeFFT}; // signal length
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
	std::vector<sample_t> in(n1-n0);

	{// dump prev samples [0~n0)
		std::vector<sample_t> dump(n0);
		sndIn.readDoubleFrames(dump.data(), dump.size());
	}
	sndIn.readDoubleFrames(in.data(), in.size());


	std::vector<sample_t> blockSamples(fftLength + hopSizeFFT);


	auto ibegin = std::begin(in);
	auto iend = ibegin + fftLength + hopSizeFFT;
	//=======================================pitch extraction per block
	for (size_t i{}; i < blocks; ++i, ibegin+=hopSizeTime, iend+=hopSizeTime)
	{
		// copy from sample to block
		std::copy(ibegin, iend, std::begin(blockSamples));

		// TODO find pitch using fft


		//take candidate
		pitches[i] = 0.0;

	}

	//========================================post-processing


	//========================================voiced/unvoiced detection





}