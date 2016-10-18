#include "../include/miqs_processor_ops.h"

using namespace miqs::process;

fill_value::fill_value()
{
	this->set_name("Fill Value");
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });

	this->set_value(0.0);
}

multiplies::multiplies()
{
	this->set_name("Multiplies");
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });

	this->set_value(0.0);
}

divides::divides()
{
	this->set_name("Divides");
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });

	this->set_value(0.0);
}

plus::plus()
{
	this->set_name("Plus");
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });

	this->set_value(0.0);
}

minus::minus()
{
	this->set_name("Minus");
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });

	this->set_value(0.0);
}



amp_to_db::amp_to_db()
{
	this->set_name("Amplitude to dB");
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });

}

db_to_amp::db_to_amp()
{
	this->set_name("Db to Amplitude");
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });

}


ln::ln()
{
	this->set_name("Logarithmic e");
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });

}

exp::exp()
{
	this->set_name("Exponential");
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });

}
