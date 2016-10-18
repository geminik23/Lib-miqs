#include "../include/miqs_processor_realpart_ops.h"

using namespace miqs::process;

realpart_mag2db::realpart_mag2db()
{
	this->set_name("RealPart Magnitude to DeciBel");

	this->set_inputs({ {port_type::RealFFTPolarSignal,"PolarFormSignal" } });
	this->set_outputs({ {port_type::RealFFTPolarSignal,"PolarFormSignal"} });
}

realpart_db2mag::realpart_db2mag()
{
	this->set_name("RealPart DeciBel to Magnitude");

	this->set_inputs({ { port_type::RealFFTPolarSignal,"PolarFormSignal" } });
	this->set_outputs({ { port_type::RealFFTPolarSignal,"PolarFormSignal" } });
}

realpart_zero::realpart_zero()
{
	this->set_name("RealPart Zero");

	this->set_inputs({ { port_type::RealFFTPolarSignal,"PolarFormSignal" } });
	this->set_outputs({ { port_type::RealFFTPolarSignal,"PolarFormSignal" } });
}

imagpart_zero::imagpart_zero()
{
	this->set_name("ImagPart Zero");

	this->set_inputs({ { port_type::RealFFTPolarSignal,"PolarFormSignal" } });
	this->set_outputs({ { port_type::RealFFTPolarSignal,"PolarFormSignal" } });
}