#pragma once

#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		//======================realpart_mag2db
		class realpart_mag2db:
			public base_processor<processor_type::Processor>
		{
		public:
			realpart_mag2db();
			void operator()(sample_t* i, sample_t* o);
			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		};


		inline void realpart_mag2db::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in < in_end; in += 2, out += 2)
			{
				*out = 20 * std::log10(*in + std::numeric_limits<sample_t>::epsilon());
			}
		}

		inline void realpart_mag2db::operator()(sample_t* in, sample_t* out)
		{
			*out = 20 * std::log10(*in + std::numeric_limits<sample_t>::epsilon());
		}




		//
		//======================realpart_db2mag
		//
		class realpart_db2mag:
			public base_processor<processor_type::Processor>
		{
		public:
			realpart_db2mag();
			void operator()(sample_t* i, sample_t* o);
			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		};

		inline void realpart_db2mag::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in < in_end; in += 2, out += 2)
			{
				*out = std::pow(10, *in* 0.05);
			}
		}

		inline void realpart_db2mag::operator()(sample_t* in, sample_t* out)
		{
			*out = std::pow(10, *in* 0.05);
		}




		//
		//======================realpart_zero
		//
		class realpart_zero:
			public base_processor<processor_type::Processor>
		{
		public:
			realpart_zero();
			void operator()(sample_t* i, sample_t* o);
			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		};


		inline void realpart_zero::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in < in_end;)
			{
				*out++ = 0.0; 
				++in;
				*out++ = *in++;
			}
		}

		inline void realpart_zero::operator()(sample_t* in, sample_t* out)
		{
			*out++ = 0.0;
			++in;
			*out++ = *in++;
		}
		

		//
		//======================imagpart_zero
		//
		class imagpart_zero:
			public base_processor<processor_type::Processor>
		{
		public:
			imagpart_zero();
			void operator()(sample_t* i, sample_t* o);
			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
		};


		inline void imagpart_zero::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in < in_end; )
			{
				*out++ = *in++;
				*out++ = 0.0;
				++in;
			}
		}

		inline void imagpart_zero::operator()(sample_t* in, sample_t* out)
		{
			*out++ = *in++;
			*out++ = 0.0;
			++in;
		}
	}
}