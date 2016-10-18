#include "../include/miqs_processor_template.h"

using namespace miqs::process;

template_processor::template_processor()
{
	this->set_name("");
	//this->set_gain(1.0);

	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });
}
