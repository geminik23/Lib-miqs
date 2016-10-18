#pragma once
#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		//
		//=========================== fill value
		//
		class fill_value:
			public base_processor<processor_type::Processor>
		{
		public:
			fill_value();

			void set_value(sample_t v) noexcept { m_value = v; }
			sample_t get_value() const noexcept { return m_value; }
			sample_t operator()(sample_t i);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			sample_t m_value;
		};

		inline void fill_value::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				*out++ = m_value;
			}
		}
		inline sample_t fill_value::operator()(sample_t)
		{
			return m_value;
		}

		//
		//==========================
		//
		class multiplies:
			public base_processor<processor_type::Processor>
		{
		public:
			multiplies();

			void set_value(sample_t v) noexcept { m_value = v; }
			sample_t get_value() const noexcept { return m_value; }
			sample_t operator()(sample_t i);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			sample_t m_value;
		};

		inline void multiplies::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				*out++ = *in++*m_value;
			}
		}
		inline sample_t multiplies::operator()(sample_t in)
		{
			return in*m_value;
		}

		//
		//==========================
		//
		class divides:
			public base_processor<processor_type::Processor>
		{
		public:
			divides();

			void set_value(sample_t v) noexcept { m_value = v; }
			sample_t get_value() const noexcept { return m_value; }
			sample_t operator()(sample_t i);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			sample_t m_value;
		};

		inline void divides::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				*out++ = *in++ /m_value;
			}
		}
		inline sample_t divides::operator()(sample_t in)
		{
			return in/m_value;
		}


		//
		//==========================
		//
		class plus:
			public base_processor<processor_type::Processor>
		{
		public:
			plus();

			void set_value(sample_t v) noexcept { m_value = v; }
			sample_t get_value() const noexcept { return m_value; }
			sample_t operator()(sample_t i);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			sample_t m_value;
		};

		inline void plus::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				*out++ = *in++ +m_value;
			}
		}
		inline sample_t plus::operator()(sample_t in)
		{
			return in+m_value;
		}


		//
		//==========================
		//
		class minus:
			public base_processor<processor_type::Processor>
		{
		public:
			minus();

			void set_value(sample_t v) noexcept { m_value = v; }
			sample_t get_value() const noexcept { return m_value; }
			sample_t operator()(sample_t i);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			sample_t m_value;
		};

		inline void minus::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				*out++ = *in++ -m_value;
			}
		}
		inline sample_t minus::operator()(sample_t in)
		{
			return in - m_value;
		}


		//
		//=========================== 
		//
		class amp_to_db:
			public base_processor<processor_type::Processor>
		{
		public:
			amp_to_db();

			sample_t operator()(sample_t i);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		};

		inline void amp_to_db::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				*out++ = 20 * std::log10(*in++ + std::numeric_limits<sample_t>::epsilon());
			}
		}
		inline sample_t amp_to_db::operator()(sample_t in)
		{
			return 20 * std::log10(in + std::numeric_limits<sample_t>::epsilon());
		}


		//
		//=========================== 
		//
		class db_to_amp:
			public base_processor<processor_type::Processor>
		{
		public:
			db_to_amp();

			sample_t operator()(sample_t i);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		};

		inline void db_to_amp::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				
				*out++ = std::pow(10, (*in++ / 20.0));
			}
		}
		inline sample_t db_to_amp::operator()(sample_t in)
		{
			return std::pow(10, (in / 20.0));
		}



		//
		//=========================== 
		//
		class ln:
			public base_processor<processor_type::Processor>
		{
		public:
			ln();

			sample_t operator()(sample_t i);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		};

		inline void ln::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				*out = std::log(*in + std::numeric_limits<sample_t>::epsilon());
			}
		}
		inline sample_t ln::operator()(sample_t in)
		{
			return std::log(in + std::numeric_limits<sample_t>::epsilon());
		}

		//
		//=========================== 
		//
		class exp:
			public base_processor<processor_type::Processor>
		{
		public:
			exp();

			sample_t operator()(sample_t i);

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		};

		inline void exp::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			for (; in != in_end; )
			{
				*out = std::exp(*in);
			}
		}
		inline sample_t exp::operator()(sample_t in)
		{
			return std::exp(in);
		}
	}
}