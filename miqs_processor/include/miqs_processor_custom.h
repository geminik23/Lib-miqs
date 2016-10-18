#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class custom:
			public base_processor<processor_type::Processor>
		{
		public:
			typedef std::function<void(process_info&, sample_t **, size_t, sample_t **, size_t)> function_type;
			custom()
			{
				this->set_name("Custom Processor");

				this->set_inputs({ { port_type::RealFFTSignal, "SignalIn" } });
				this->set_outputs({ { port_type::RealFFTPolarSignal, "SignalOut" } });
			}

			void set_process_function(function_type&& func) { this->m_func = func; }

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			function_type m_func;
		};
		inline void custom::process(process_info& info, sample_t ** ins, size_t nin, sample_t ** outs, size_t nout)
		{
			if(m_func)	m_func(info, ins, nin, outs, nout);
		}

	}
}