#pragma once
#include <miqs.h>

#include <iomanip>
#include <MiqsSound.h>
#define q_interface struct



using namespace miqs;
namespace miqs_test
{

	q_interface iprocessor{
		virtual void process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length) = 0;
		virtual void set_info(miqs::uint32_t size, miqs::uint32_t samplerate) = 0;
	};
	q_interface idata{};

	q_interface ianalysis_processor{
		virtual void* result() = 0;
	virtual void process() = 0;
	};


	struct audio_info
	{
		size_t vector_size;
		miqs::uint32_t samplerate;
	};


#define PROCESSOR(NAME, TYPES) \
class NAME : public iprocessor{\
public:\
	NAME();\
	virtual void set_info(miqs::uint32_t size=512, miqs::uint32_t samplerate=48000){ _info.vector_size = size;_info.samplerate = samplerate; }\
	audio_info& get_info(){return _info;}\
	virtual void process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length);\
private:\
	audio_info _info;\
public:\
	TYPES\
}




	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// testing functions
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct funcs
	{
		static void implement_test();
		static void linear_line();
		static void sola();
		static void lpc_analysis();
		static void lpc_cross();
		static void cepstrum();
		static void simple_granular();
		static void allpassfilter();
		static void applywindow();
		static void basicfilter();
		static void biquadfilter();
		static void combfilter();
		static void convert_samplerate();
		static void delay();
		static void fft();
		static void pitchestimation1();
		//static void pitchestimation2();
		static void spectrogram();
		static void fft_2real_signal();
		static void fast_convolution();
		static void envelope1();
		static void cepstrum_cross();
		static void cepstrum_formant_move();
		static void cepstrum_formant_move_directly();
		static void spectral_interpolation();
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Realtime Implement Test
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Test
	PROCESSOR(implement_test, miqs::phasor phase; double angle;);




	// generate sinewave basic.
	PROCESSOR(generator, miqs::phasor phasor;);
	// basic operators
	PROCESSOR(basic_ops, miqs::phasor phasor;);

	/* delay and comb based effects */
	// vibrato
	PROCESSOR(delay_effect_vibrato,
			  double width;
	miqs::phasor phase;
	miqs::phasor lfphase;
	miqs::delay buffer;);

	// chorus
	PROCESSOR(delay_effect_chorus,
			  double width;
	miqs::phasor phase;
	miqs::phasor lfphase;
	miqs::delay buffer;);

	// stereo panning
	PROCESSOR(stereopanning, miqs::phasor phase; double angle;);

	// flanger, chorus, slapback, echo   // p97
	PROCESSOR(comb_effects,
			  miqs::delay delay;
	miqs::iir_comb_filter<miqs::delay> iircomb;);

	// natural sounding comb filter
	PROCESSOR(natural_comb_filter, );

	// modulations
	PROCESSOR(modulations, miqs::phasor phase1; miqs::phasor phase2;);


}




//#include "miqs_test.h"
//using namespace miqs_test;
//using namespace miqs;
//#define miqs_TEST_OBJ_NAME 
//miqs_TEST_OBJ_NAME::miqs_TEST_OBJ_NAME() {
//}
//
//
//void miqs_TEST_OBJ_NAME::process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length) {
//
//}
//#undef miqs_TEST_OBJ_NAME
