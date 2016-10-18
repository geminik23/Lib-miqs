#include "../include/miqs_processor_car2pol.h"

using namespace miqs::process;

cartesian_to_polar::cartesian_to_polar()
{
	this->set_name("Cartesian to Polar");

	this->set_inputs({ {port_type::RealFFTSignal, "CartesianFormSignal"} });
	this->set_outputs({ {port_type::RealFFTPolarSignal, "PolarFormSignal"} });
}
