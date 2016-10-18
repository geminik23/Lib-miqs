#include "../include/miqs_processor_panning.h"

using namespace miqs::process;

stereo_panning::stereo_panning()
{
	this->set_name("Stereo Panning");
	this->set_gain(1.0);
	this->set_panning(0.5);

	this->set_inputs({ {port_type::DefaultSignal, "SignalIn"} });
	this->set_outputs({ {port_type::DefaultSignal, "Left"},{ port_type::DefaultSignal, "Right" } });
	
}
