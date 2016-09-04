#pragma once
#include <miqs.h>


#define q_interface struct

using namespace miqs;
namespace miqs_test
{

	q_interface iprocessor{
		virtual void process(sample_t * samples, size_t length) = 0;
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


#define PROCESSOR_DATA(NAME, TYPES) \
class NAME##_data : public idata{\
public:\
	TYPES\
}

#define PROCESSOR(NAME) \
class NAME : public iprocessor{\
public:\
	NAME(miqs::uint32_t size=512, miqs::uint32_t samplerate=48000){_info.vector_size = size;_info.samplerate = samplerate;_data = std::make_unique<NAME##_data>();this->initialize();}\
	void initialize();\
	audio_info& get_info(){return _info;}\
	virtual void process(sample_t * samples, size_t length);\
	NAME##_data* get_data(){\
		return reinterpret_cast<NAME##_data*>(_data.get());\
	}\
private:\
	std::unique_ptr<idata> _data;\
	audio_info _info;\
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



	//TESTs
	// generate sinewave basic.
	PROCESSOR_DATA(generator, miqs::phasor phasor;);
	PROCESSOR(generator);

	//// basic operators
	PROCESSOR_DATA(basic_ops, miqs::phasor phasor;);
	PROCESSOR(basic_ops);


	//// filter allpole
	//PROCESSOR_DATA(allpole_filter, );
	//PROCESSOR(allpole_filter);
	//// filter allzero
	//PROCESSOR_DATA(allzero_filter, );
	//PROCESSOR(allzero_filter);


	//// ifft{fft{sin}}
	//PROCESSOR_DATA(simple_fft, std::unique_ptr<miqs::generator<sine_wave>> osc;);
	//PROCESSOR(simple_fft);

	//// ifft{fft{sin}} : calc magnitude and phase
	//PROCESSOR_DATA(simple_fft2, std::unique_ptr<miqs::generator<sine_wave>> osc;);
	//PROCESSOR(simple_fft2);





	//
	// Analysis Processor

	// apply window
	PROCESSOR_DATA(ana_apply_window,);
	ANALYSIS_PROCESSOR(ana_apply_window);

	// tranform_fft
	PROCESSOR_DATA(ana_fft, );
	ANALYSIS_PROCESSOR(ana_fft);

	// pitch using zerocross
	//PROCESSOR_DATA(pitch_estimation1, double pitch;);
	//ANALYSIS_PROCESSOR(pitch_estimation1);




}




//#include "miqs_test.h"
//using namespace miqs_test;
//using namespace miqs;
//#define miqs_TEST_OBJ_NAME 
//void miqs_TEST_OBJ_NAME::initialize() {
//}
//
//
//void miqs_TEST_OBJ_NAME::process(sample_t * samples, size_t length) {
//
//}
//#undef miqs_TEST_OBJ_NAME
