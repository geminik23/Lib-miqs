#pragma once
#include <miqs.h>

#include <iomanip>

#define q_interface struct



using namespace miqs;
namespace miqs_test
{

	q_interface iprocessor{
		virtual void process(sample_t *in, sample_t * samples, size_t length) = 0;
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
	virtual void process(sample_t* in, sample_t * out, size_t length);\
private:\
	audio_info _info;\
	TYPES\
}

#define ANALYSIS_DATA(NAME, TYPE)\
struct NAME##_data: public idata{\
TYPE\
}


#define ANALYSIS_PROCESSOR(NAME) \
	class NAME : public ianalysis_processor\
	{\
	public:\
		typedef NAME##_data data_type;\
		virtual void* result() { return (void*)&this->_data; }\
		virtual void process();\
		data_type* get_data() { return &_data; }\
		data_type _data;\
	}



	// TESTs
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

	// flanger, chorus, slapback, echo   // p97
	PROCESSOR(comb_effects, );

	// natural sounding comb filter
	PROCESSOR(natural_comb_filter, );








	
	//
	// Analysis Processor
	//
	// apply window
	ANALYSIS_DATA(ana_apply_window,);
	ANALYSIS_PROCESSOR(ana_apply_window);

	// tranform_fft
	ANALYSIS_DATA(ana_fft, );
	ANALYSIS_PROCESSOR(ana_fft);

	// Spectrogram //TODO  check sound
	ANALYSIS_DATA(ana_spectrogram, );
	ANALYSIS_PROCESSOR(ana_spectrogram);


	/** TODO
		FILTERS
	**/

	// Basic Filter 
	ANALYSIS_DATA(ana_basicfilter, );
	ANALYSIS_PROCESSOR(ana_basicfilter);

	// Allpass Filter 
	ANALYSIS_DATA(ana_allpassfilter, );
	ANALYSIS_PROCESSOR(ana_allpassfilter);

	// Biquad Filter (iir_df2) direct-form 2.
	ANALYSIS_DATA(ana_biquadfilter, );
	ANALYSIS_PROCESSOR(ana_biquadfilter);

	// Delay and fraction delay
	ANALYSIS_DATA(ana_delay, );
	ANALYSIS_PROCESSOR(ana_delay); 

	// Comb Filter
	ANALYSIS_DATA(ana_comb_filter, );
	ANALYSIS_PROCESSOR(ana_comb_filter);


	/** TODO
		MODULATIONS [DAFX p101]
	**/


	/** TODO
		SPATIAL EFFECTS [DAFX p156]
	**/



	/* TODO
	Time Segment p201 
	1. time stretching SOLA, PSOLA, 
	2. pitch shifting
	3. 
	*/

	/** TODO
		Time Shuffling and granulation [DAFX p226]

	**/


	/** TODO
	Source-Filter processing [DAFX p294]
	1. LPC
	2. Cepstrum
	3. Cross Synthesis ? 
	4. Formant changing
	5. Spectrum interpolation
	**/

	/**
		Adaptive digital audio	effects [DAFX 336]
	** PITCH
	** GRANULAR
	**/

	/**??
	SPECTRAL PROCESSING [DAFX 407]
	**/


	/**??
	 Time-Freq Warping [DAFX 460]
	**/

	



	// pitch using zerocross
	ANALYSIS_DATA(pitch_estimation1,);
	ANALYSIS_PROCESSOR(pitch_estimation1);

	// pitch using autocorrelation
	ANALYSIS_DATA(pitch_estimation2,);
	ANALYSIS_PROCESSOR(pitch_estimation2);





	// pitch using autocorrelation
	ANALYSIS_DATA(implement_test, );
	ANALYSIS_PROCESSOR(implement_test);

}




//#include "miqs_test.h"
//using namespace miqs_test;
//using namespace miqs;
//miqs_TEST_OBJ_NAME::miqs_TEST_OBJ_NAME() {
//}
//
//
//void miqs_TEST_OBJ_NAME::process(sample_t * in, sample_t * out, size_t length) {
//
//}
//#undef miqs_TEST_OBJ_NAME
