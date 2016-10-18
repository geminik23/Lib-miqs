#pragma once



#include "miqs_processor_base.h"

namespace miqs
{
	namespace process
	{

		//template <typename _Interpolator>
		class lpc:
			public base_processor<processor_type::Analyzer>
		{
		public:
			lpc();

			void set_order(size_t order) noexcept { m_order = order; this->set_outport_length(0, order + 1); }
			size_t get_order() const noexcept { return m_order; }

			//-------- override
			virtual void tick(process_proxy& proxy) { proxy.call_process(); }
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) override;

		private:
			size_t m_order;
		};




		inline void lpc::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
		{
			auto in = ins[0];
			auto in_end = in + info.frame_count();
			auto out = outs[0];

			miqs::array_ref<double> lpc(out,  this->m_order + 1);
			miqs::array<double> refl(this->m_order+1);
			double gain = miqs::lpc_levinson_durbin_method(in, in_end, lpc, refl, m_order);
			out[0] = gain;
		}



	}
}