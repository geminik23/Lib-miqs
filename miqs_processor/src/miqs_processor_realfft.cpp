#include "../include/miqs_processor_realfft.h"

using namespace miqs::process;

realfft::realfft()
{

	this->set_inputs({ { port_type::DefaultSignal,"SignalIn" } });
	this->set_outputs({ { port_type::RealFFTSignal,"FFTOut" } });

	this->set_name("Real FFT");
	this->set_fftsize(512);

}
