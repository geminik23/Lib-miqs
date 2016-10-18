#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class interleaved_separator:
			public base_processor<processor_type::Processor>
		{
		public:
			interleaved_separator();



			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		};

		inline void interleaved_separator::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out1 = outs[0];
			auto out2 = outs[1];
			for (; in != in_end; )
			{
				*out1++ = *in++;
				*out2++ = *in++;
			}
		}


		class interleaved_merger:
			public base_processor<processor_type::Processor>
		{
		public:
			interleaved_merger();

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		};

		inline void interleaved_merger::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in1 = ins[0];
			auto in2 = ins[1];

			auto out = outs[0];
			auto out_end = out + info.frame_count()*2;
			for (; out != out_end; )
			{
				*out++ = *in1++;
				*out++ = *in2++;
			}
		}
	}
}