#include "../include/miqs_processor_zerophase_shifter.h"
using namespace miqs::process;

zerophase_shifter::zerophase_shifter()
{
	this->set_name("ZeroPhase Shifter");

	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });
}

zerophase_unshifter::zerophase_unshifter()
{
	this->set_name("ZeroPhase UnShifter");

	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });
}
