#include "../include/miqs_processor_realifft.h"

using namespace miqs::process;

realifft::realifft()
{

	this->set_inputs({ { port_type::RealFFTSignal,"RealFFTIn" } });
	this->set_outputs({ { port_type::DefaultSignal,"SignalOut" } });

	this->set_name("Real Inverse FFT");
	this->set_fftsize(512);

}
