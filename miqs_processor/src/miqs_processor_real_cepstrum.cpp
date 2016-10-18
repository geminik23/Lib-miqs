#include "../include/miqs_processor_real_cespstrum.h"

using namespace miqs::process;

real_cepstrum::real_cepstrum()
{
	this->set_name("Real Cepstrum");

	this->set_inputs({ {port_type::DefaultSignal, "SignalIn"} });
	this->set_outputs({ {port_type::DefaultSignal,"SignalOut"},{ port_type::RealFFTSignal, "FFTSignal" } });
	this->set_fftsize(512);
	this->set_pre_method(method::log10_20);

}
