#include "../include/miqs_processor_pitch_shifter.h"

using namespace miqs::process;

pitch_shifter::pitch_shifter():m_win_ratio{ 0.1 }, m_win_size{ 0 }
{
	this->set_name("Pitch Shifter");
	this->set_gain(1.0);

	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });
	this->set_pitch(1.0);
}
