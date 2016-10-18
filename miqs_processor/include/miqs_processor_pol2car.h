#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class polar_to_cartesian:
			public base_processor<processor_type::Processor>
		{
		public:
			polar_to_cartesian();


			void operator()(sample_t* i, sample_t * o);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		};


		inline void polar_to_cartesian::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in < in_end; in+=2, out+=2)
			{
				MIQS_PTR_P2C(in, in + 1, out, out + 1);
			}
		}

		inline void polar_to_cartesian::operator()(sample_t* i, sample_t * o)
		{
			MIQS_PTR_P2C(i, i + 1, o, o + 1);
		}

	}
}