#include "../include/miqs_processor_oscillator.h"
using namespace miqs::process;

oscillator::oscillator()
{
	this->set_name("Oscillator");
	this->set_outputs({ { port_type::DefaultSignal,"SignalOut" } });

	this->set_gain(1.0);
	this->set_osc_type(type::Sine);
}
