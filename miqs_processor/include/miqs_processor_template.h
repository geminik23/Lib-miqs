#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		class template_processor:
			public TypeProcessor<buffer_module>
		{
		public:
			template_processor();


			sample_t operator()(sample_t i);

			//-------- override
			virtual void init(process_info&) override;
			virtual void reset() override;
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		};

		inline void template_processor::init(process_info&info)
		{
			this->reset();
			
		}
		inline void template_processor::reset()
		{

		}
		inline void template_processor::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				*out++ = this->operator()(*in++);
			}
		}		
		inline sample_t template_processor::operator()(sample_t in)
		{
			sample_t result{};


			return result;
		}

	}
}