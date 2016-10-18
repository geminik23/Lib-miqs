#pragma once

#include <miqs>
#include "miqs_processor_app.h"

namespace miqs
{

	namespace process
	{
		//===================== types
		enum class window_type
		{
			Hanning,
			Hamming,
			Blackman
		};


		//-----------for processor's internal id
		inline size_t get_id()
		{
			static size_t _id{1};
			return _id++;
		}

		//==================== signal
		template <typename Func>
		struct signal
		{
			using container_type = std::list<Func>;
			signal() = default;
			signal(signal&& other)
			{
				this->signals = std::move(other.signals);
			}

			signal& operator=(signal&& other) { this->signals = std::move(other.signals); return *this; }
			signal& operator=(signal const& other) { this->signals = other.signals; return *this; }
			
			void connect(Func const& f) noexcept
			{
				signals.push_back(f);
			}
			void disconnect(Func const& f) noexcept
			{
				auto iter = std::find(std::begin(signals), std::end(signals), f);
				if (iter != std::end(signals)) signals.erase(iter);
			}

			template <typename ...Args>
			void emit(Args... args)
			{
				for (auto&& f : signals) f(std::forward<Args>(args)...);
			}

		private:
			container_type signals;
		};

		using CallbackExample = std::function<void(void)>;
		using CallbackSignalExample = signal<CallbackExample>;



		const static size_t DefaultFrameLength = WavetableSize;
		
		//==================== port 
		enum class port_type {
			DoubleSignal,
			ComplexSignal,
			IntegerSignal,
			Double,
			Integer,
			DefaultSignal = DoubleSignal,

			RealFFTSignal = DoubleSignal,
			ComplexFFTSignal = ComplexSignal,
			RealFFTPolarSignal = DoubleSignal,
			ComplexFFTPolarSignal = ComplexSignal,
		};

		struct port_info
		{
			port_info() = default;
			port_info(port_type type_, std::string const& name_ ="", int length_ = -1):
				type{ type_ }, name{ name_ }, length{ length_ }{}
			

			port_type type{ port_type::DefaultSignal };
			std::string name;
			int length{ -1 };
		};


		//================process

		enum class processor_type { Processor, Analyzer, Generator };

		class process_info;
		class process_proxy;
		struct iprocessor
		{
			virtual void init(process_info&) = 0;
			virtual void reset() = 0;
			virtual void tick(process_proxy& proxy) = 0;
			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) = 0;
		};


		class process_info
		{
		public:
			size_t frame_count() const noexcept { return m_nframe; }
			double samplerate() const noexcept { return m_samplerate; }

		public:
			size_t m_nframe;
			double m_samplerate;
		};
		


		class process_proxy: public process_info
		{
		public:
			void call_process()
			{
				if (m_cur_processor)
					m_cur_processor->process(*this, &in, 1, &out, 1);
			}
			void set_current_processor(iprocessor * processor) { m_cur_processor = processor; }

			sample_t *in;
			sample_t *out;

		private:
			iprocessor* m_cur_processor;
		};







		

		//==================================================
		// MODULE BASE


		template<typename... _Rest>
		struct _base_modules;

		template<typename _Mod, typename... _Rest>
		struct _base_modules<_Mod, _Rest...>: public _Mod, public _base_modules <_Rest...>{};

		template <typename _Mod>
		struct _base_modules<_Mod>: public _Mod
		{};



		//-----------------
		struct gain_module
		{
			virtual void set_gain(double gain) noexcept { this->m_gain = gain; }
			virtual double get_gain()noexcept { return m_gain; }
		protected:
			double m_gain;
		};

		template <typename _Ty, size_t N>
		struct phase_info_module
		{
			typedef phase_info<_Ty> phase_info_type;
			virtual void set_phaseinfo(size_t idx, _Ty phase, _Ty step) { m_phaseinfo[idx].phase = phase; m_phaseinfo[idx].step = step; }
			virtual phase_info_type& get_phaseinfo(size_t idx) { return m_phaseinfo[idx]; }
			virtual size_t get_phaseinfo_count() const noexcept { return N; }
		protected:
			phase_info<_Ty> m_phaseinfo[N];
		};

		struct buffer_module
		{
		protected:
			virtual void reset_buffer(size_t size) { m_buffer.reset(size); }
			virtual void reset_buffer(size_t size, sample_t&& v) { m_buffer.reset(size, std::forward<sample_t>(v)); }

			miqs::array<sample_t> m_buffer;
		};

		template <size_t N>
		struct multibuffer_module
		{
		protected:
			virtual void reset_buffers(size_t idx, size_t size) { m_buffers[idx].reset(size, 0.0); }

			miqs::array<sample_t> m_buffers[N];
		};

		struct lastsample_module
		{
		protected:
			void set_max_lastsample(size_t count) { m_lastsample.resize(count); }

			miqs::array<sample_t> m_lastsample;
		};

		
		struct internal_process_info_module
		{
		protected:
			virtual void set_internal_info(process_info& info)
			{
				this->m_samplerate = info.samplerate();
				this->m_nframe = info.frame_count();
			}

			double m_samplerate;
			size_t m_nframe;
		};
		
		struct realfft_module
		{
		protected:
			miqs::real_fft<sample_t> func_realfft;
		};

		struct realifft_module
		{
		protected:
			miqs::real_ifft<sample_t> func_realifft;
		};




		//=====================================================
		// BASE PROCESSOR

		//
		// base processor
		//
		template <processor_type _PType>
		class base_processor: public iprocessor
		{
		public:
			typedef sample_t value_type;
			typedef value_type * pointer;

			processor_type get_process_type() { return _PType; }
			virtual void init(process_info&) { this->reset(); }
			virtual void reset() {}

			std::string get_name() const noexcept { return this->m_name }


			virtual void process(process_info&, sample_t **, size_t, sample_t **, size_t) {}

			//--------port info
			size_t get_num_inputs() const noexcept { return this->m_info_inputs.size(); }
			size_t get_num_outputs() const noexcept { return this->m_info_outputs.size(); }
			
			port_type get_inport_type(size_t i) const { return this->m_info_inputs[i].type; }
			port_type get_outport_type(size_t i) const { return this->m_info_outputs[i].type; }

			std::string get_inport_name(size_t i) const { return this->m_info_inputs[i].name; }
			std::string get_outport_name(size_t i) const { return this->m_info_outputs[i].name; }

			//TODO length?
			virtual int get_inport_length(size_t i) const { return this->m_info_inputs[i].length; }
			virtual int get_outport_length(size_t i) const { return this->m_info_outputs[i].length; }


			//--------helper functions
			static sample_t low_sin(double phase) { phase /= miqs::two_pi; MIQS_PTR_MODULAR(phase, 1.0); return resource::get_instance().osc_table(OSCTYPE_SINE)[(int)phase]; }
			static sample_t low_cos(double phase) { phase /= miqs::two_pi; MIQS_PTR_MODULAR(phase, 1.0); return resource::get_instance().osc_table(OSCTYPE_SINE)[ DefaultFrameLength*1/4+ (int)phase];}


		protected:
			void set_name(std::string const& name) noexcept { this->m_name = name; }

			void set_inputs(std::initializer_list<port_info>);
			void set_outputs(std::initializer_list<port_info>);

			void set_inport_length(size_t i, int newlength) { m_info_inputs[i].length = newlength; }
			void set_outport_length(size_t i, int newlength) { m_info_outputs[i].length = newlength; }


		private:
			miqs::array<port_info> m_info_inputs;
			miqs::array<port_info> m_info_outputs;

			std::string m_name;
		};

		template <processor_type _PType, typename ..._Rest>
		class processor_with_modules:public base_processor<_PType>, public _base_modules<_Rest...>
		{
		};


		template <typename ..._Rest>
		using TypeProcessor = processor_with_modules<processor_type::Processor, _Rest...>;
		template <typename ..._Rest>
		using TypeAnalyzer = processor_with_modules<processor_type::Analyzer, _Rest...>;
		template <typename ..._Rest>
		using TypeGenerator = processor_with_modules<processor_type::Generator, _Rest...>;


		template <processor_type _PType>
		void base_processor<_PType>::set_inputs(std::initializer_list<port_info> list)
		{
			this->m_info_inputs.resize(list.size());
			size_t i{};
			for (auto&& info : list)
			{
				m_info_inputs[i++]= info;
				
			}
		}

		template <processor_type _PType>
		void base_processor<_PType>::set_outputs(std::initializer_list<port_info> list)
		{
			this->m_info_outputs.resize(list.size());
			size_t i{};
			for (auto&& info : list)
			{
				m_info_outputs[i++] = info;
			}
		}
		
		
		
	}

}