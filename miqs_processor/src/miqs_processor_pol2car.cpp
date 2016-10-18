#include "../include/miqs_processor_pol2car.h"

using namespace miqs::process;

polar_to_cartesian::polar_to_cartesian()
{
	this->set_name("Polar to Cartesian");
	this->set_inputs({ {port_type::RealFFTPolarSignal, "PolarFormSignal"} });
	this->set_outputs({ {port_type::RealFFTSignal, "CartesianFormSignal"} });
}
