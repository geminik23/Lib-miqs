#include "../include/miqs_processor_buffer_pitch_shifter.h"

using namespace miqs::process;

buffer_pitch_shifter::buffer_pitch_shifter():m_win_ratio{ 0.1 }, m_win_size{}
{
	this->set_name("Buffer Pitch Shifter");
	this->set_outputs({ port_type::DefaultSignal });
	this->set_gain(1.0);
	this->set_pitch(1.0);
}
