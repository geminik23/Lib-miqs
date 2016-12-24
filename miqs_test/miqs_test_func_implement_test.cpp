
#include "miqs_test.h"
#include "miqs_temp_impl.h"

using namespace miqs_test;
using namespace miqs;

#define __USE_SOUNDFILE 1


namespace miqs
{
	namespace process
	{
	

		/* 
			1. arrays (how many?) default number
			2. from base_processor -> ready (keep or reuse? call with parameter)
			3. 
			4. 
		*/

		//class base_port
		//{
		//public:
		//	virtual size_t nchannels() { return 0; }
		//	virtual process_info info() { return m_info; }

		//	miqs::array_ref<sample_t> operator[](int) { return{ data.data(), data.size() }; }
		//	//----------------------

		//private:
		//	miqs::array<sample_t> data;
		//	process_info m_info;
		//};


		//class inport : public base_port
		//{
		//public:
		//	typedef inport self_type;

		//};

		//class outport : public base_port
		//{
		//public:
		//	typedef outport self_type;
		//	friend class port_adaptor < self_type>;

		//};

		//template <typename _PortTy>
		//struct port_adaptor;

		//template <>
		//struct port_adaptor<inport>
		//{
		//	port_adaptor(inport& p):port{ p } {}


		//	void fill_zero(int i = 0) {}
		//	

		//	inport& port;
		//};

		//template <>
		//struct port_adaptor<outport>
		//{

		//};

		struct port_adapt
		{
			template <typename _Port>
			static void fill_zero(_Port& , int)
			{
				
			}
		};

		template <typename _PortTy>
		auto make_port_adaptor(_PortTy& port)
		{
			return port_adaptor<_PortTy>(port);
		}

		//void d()
		//{
		//	inport in;

		//	size_t nch = in.nchannels();
		//	in[0].size();

		//	port_adapt::fill_zero(in, 0);

		//	auto aport = make_port_adaptor(in);
		//	aport.fill_zero();

		//}
	}


	template <typename _Ptr>
	inline void ptr_print_samples(_Ptr first, _Ptr last)
	{
		std::copy(first, last, std::ostream_iterator<std::iterator_traits<_Ptr>::value_type>(std::cout, " "));
		std::cout << "\n";
	}


}





void miqs_test::funcs::implement_test()
{


	std::string infilename{ "Toms_diner.wav" };
	std::string outfilename{ "testing.wav" };

	//===============================in
	auto sndSrcIn = Miqs::SoundIn::ReadSoundFile(infilename);
	auto sndSrcSize = sndSrcIn.size();
	auto inSamplerate = sndSrcIn.property.srate;
	auto inCh = sndSrcIn.property.chans;

	//================================out

	int outCh = inCh;



	// samples
	miqs::array<> in(sndSrcSize);
	miqs::array<> out(sndSrcSize*outCh);

	// read source file
	sndSrcIn.readDoubleFrames(in, in.size());

	/*
		1. zerophase_shifter
		2. zerophase_unshifter
		3. zerophase_windowing
	*/

	//============properties

	std::cout << std::fixed;
	size_t nframe = 16;
	//auto ptr = std::begin(in);
	//auto ptr_e = ptr + (sndSrcSize / nframe)*nframe;
	//auto dst_ptr = std::begin(out);


	miqs::process::process_proxy px;
	px.m_samplerate = inSamplerate;
	px.m_nframe = nframe;

	//--------------------------

	size_t winsize{ 8 };
	size_t fftsize{ nframe };
	size_t hopsize{  };
	
	miqs::array<> d(nframe, 1.0);
	miqs::array<> d2(nframe, 1.0);
	miqs::array<> result(nframe, 1.0);


	miqs::process::windowing_repeat pwin;
	pwin.init(px);
	pwin.set_windowtype(process::window_type::Hanning);
	pwin.set_windowsize(winsize);


	miqs::process::zerophase_shifter pshifter;
	miqs::process::zerophase_unshifter punshifter;
	miqs::process::zerophase_windowing pshiftWindowing;
	pshifter.init(px);
	punshifter.init(px);
	pshiftWindowing.init(px);
	pshiftWindowing.set_windowsize(winsize);
	pshifter.set_windowsize(winsize);
	punshifter.set_windowsize(winsize);
	pshiftWindowing.set_windowtype(process::window_type::Hanning);


	px.in = d;
	px.out = result;
	px.set_current_processor(&pwin);
	pwin.tick(px);
	miqs::ptr_print_samples(std::begin(result), std::end(result));


	px.in = result;
	px.out = d;
	px.set_current_processor(&pshifter);
	pshifter.tick(px);
	miqs::ptr_print_samples(std::begin(d), std::end(d));


	px.in = d;
	px.out = result;
	px.set_current_processor(&punshifter);
	punshifter.tick(px);
	miqs::ptr_print_samples(std::begin(result), std::end(result));


	px.in = d2;
	px.out = result;
	px.set_current_processor(&pshiftWindowing);
	pshiftWindowing.tick(px);
	miqs::ptr_print_samples(std::begin(result), std::end(result));



	//===============================out
#if __USE_SOUNDFILE
	Miqs::SoundOut sndout;
	auto& props = sndout.property;
	props.chans = outCh;
	props.srate = inSamplerate;
	props.samptype = sndSrcIn.property.samptype;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	sndout.createSoundFile(outfilename);
	sndout.writeDoubleFrames(out, out.size() / outCh);
#endif
}




