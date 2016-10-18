#pragma once



#include "miqs_processor_base.h"
namespace miqs
{
	namespace process
	{

		class noise:
			public TypeGenerator<gain_module>
		{
		public:
			noise();

			sample_t operator()();

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;
			
		private:
			std::default_random_engine m_re;
			std::uniform_real_distribution<sample_t> m_rdist;
		};


		inline void noise::process(process_info& info, sample_t **, size_t, sample_t ** outs, size_t)
		{
			auto out = outs[0];
			size_t nframe = info.frame_count();
			for (size_t i{}; i<nframe; ++i )
			{
				*out++ = m_gain* m_rdist(m_re);
			}
		}



		inline sample_t noise::operator()()
		{
			return m_gain* m_rdist(m_re);
		}

	}
}