#include "../include/miqs_processor_delay.h"

using namespace miqs::process;



delay::delay(size_t delay, size_t max_delay):m_delay(delay)
{
	this->set_name("Delay");
	this->reset_buffer(max_delay);
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });
}