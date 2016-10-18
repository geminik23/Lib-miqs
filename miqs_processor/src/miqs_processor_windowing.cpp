#include "../include/miqs_processor_windowing.h"
#include "../include/miqs_processor_windowing_repeat.h"
#include "../include/miqs_processor_zerophase_windowing.h"


using namespace miqs::process;

windowing::windowing()
{
	this->set_name("Windowing");

	this->reset_buffer(WavetableSize);

	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });
}


windowing_repeat::windowing_repeat()
{
	this->set_name("Windowing Repeating");

	this->reset_buffer(WavetableSize);

	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });
}

zerophase_windowing::zerophase_windowing()
{
	this->set_name("ZeroPhase Windowing");

	this->reset_buffer(WavetableSize);

	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });
}