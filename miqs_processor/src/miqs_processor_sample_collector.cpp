#include "../include/miqs_processor_sample_collector.h"

using namespace miqs::process;

sample_collector::sample_collector():m_counter{}
{
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });

	this->set_name("Sample Collector");
	this->set_collect_size(DefaultFrameLength);


}
