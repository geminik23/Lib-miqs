#include "../include/miqs_processor_biquad_filter.h"

using namespace miqs::process;

biquad_filter::biquad_filter() : m_filtetype{miqs::filter_type::lowpass}
{
	this->set_name("Biquad Filter");
	this->reset_buffer(8);
	this->set_gain(1.0);
	this->set_inputs({ {port_type::DefaultSignal,"SignalIn"} });
	this->set_outputs({ {port_type::DefaultSignal, "SignalOut"} });

	// set to sample_wrapper
	auto ptr = m_buffer.data();
	m_dx.assign(ptr);
	ptr += 2;
	m_a.assign(ptr);
	ptr += 3;
	m_b.assign(ptr);



}
