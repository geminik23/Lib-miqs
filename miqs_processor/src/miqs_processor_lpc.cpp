#include "../include/miqs_processor_lpc.h"

using namespace miqs::process;

lpc::lpc():m_order(12)
{
	this->set_name("LPC Analyzer");

	this->set_inputs({ {port_type::DefaultSignal, "SignalIn"} });
	this->set_outputs({ {port_type::DoubleSignal, "Linear Pridiction Coefficients"} });
}
