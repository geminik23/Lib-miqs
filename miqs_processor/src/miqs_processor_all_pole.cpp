#include "../include/miqs_processor_all_pole.h"

using namespace miqs::process;

all_pole_filter::all_pole_filter()
{
	this->set_name("All Pole Filter");
	this->reset_buffer(128);
	this->set_gain(1.0);

	this->set_inputs({ { port_type::DefaultSignal, "SignalIn" } });
	this->set_outputs({ { port_type::DefaultSignal,"SignalOut" } });
	this->set_order(1);
}

