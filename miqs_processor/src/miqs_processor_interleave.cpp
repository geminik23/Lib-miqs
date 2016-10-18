#include "../include/miqs_processor_interleave.h"

using namespace miqs::process;

interleaved_separator::interleaved_separator()
{
	this->set_name("Interleaved Separator");

	this->set_inputs({ { port_type::DefaultSignal,"SignalIn" } });
	this->set_outputs({ { port_type::DefaultSignal,"SignalOut1",-2 },{ port_type::DefaultSignal,"SignalOut2",-2 } });
}
interleaved_merger::interleaved_merger()
{
	this->set_name("Interleaved Merger");

	this->set_inputs({ { port_type::DefaultSignal,"SignalIn1" ,-2},{ port_type::DefaultSignal,"SignalIn2" ,-2 } });
	this->set_outputs({ { port_type::DefaultSignal,"SignalOut" } });
}
