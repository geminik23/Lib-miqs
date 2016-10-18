#include "../include/miqs_processor_noise.h"

using namespace miqs::process;

noise::noise():m_re(std::random_device()()), m_rdist(-1.0,1.0)
{
	this->set_name("Noise");
	this->set_gain(1.0);
	this->set_outputs({ {port_type::DefaultSignal, "SignalOut"} });
}
