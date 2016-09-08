#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;

#define miqs_TEST_OBJ_NAME implement_test



namespace miqs
{

	
	/* fir comb filter 
		y[n] = x[n] + g*x[n-M]
	*/
	template <typename _Delay>
	struct fir_comb_filter
	{
		fir_comb_filter(_Delay& delay): m_delay{ delay } {}

		fir_comb_filter(double g, _Delay& delay):gain{ g }, m_delay{ delay } {}

		void reset() noexcept { m_delay.reset(); }


		_Delay& _Get_container() noexcept { return m_delay; }

		sample_t operator()(sample_t in) noexcept
		{
			return in + gain*m_delay.operator()(in);
		}

		double gain;
	private:
		_Delay& m_delay;
	};

	/* iir comb filter 
		y[n] = c*x[n] + g*y[n-M]
	*/
	template <typename _Delay>
	struct iir_comb_filter
	{
		iir_comb_filter(_Delay& delay):m_delay{ delay } {}
		iir_comb_filter(double c, double g, _Delay& delay):scale{ c }, gain{ g }, m_delay{ delay } {}

		void reset() { m_delay.reset(); }

		_Delay& _Get_container() { return m_delay; }

		sample_t operator()(sample_t in)
		{
			sample_t out = scale*in + gain*m_delay.peek();
			m_delay.push(out);
			return out;
		}

		double gain{ 1.0 };
		double scale{ 1.0 };
	private:
		_Delay& m_delay;
	};




	//
	//TODO!!!!!!!!!!! fraction_delay HAVE PROBLEM::PUSH AND PEEK ORDER
	//
	/** universal_comb_filter
	* bl (blend param)
	* fb (feedback param)
	* ff (feedforward param)
	* 
	* xh = x[n] + fb*xh[n-M];
	* y[n] = ff*xh[n-M] + bl*xh;
	*                      BL   FB   FF
	* FIR comb filter -    1    0    g
	* IIR comb filter -    c    g    0
	* Allpass         -    a    -a   1
	* Delay           -    0    0    1
	* _Delay::reset(), _Delay::peek()
	*/
	template <typename _Delay>
	struct universal_comb_filter
	{
		universal_comb_filter(_Delay& delay):m_delay{ delay } {}

		universal_comb_filter(double param_blend, double param_feedback, double param_feedforward, _Delay& delay)
			:param_blend{ param_blend }
			, param_feedback{ param_feedback }
			, param_feedforward{ param_feedforward }
			, m_delay{ delay }
		{}

		void set_fir_comb_filter(double gain) { param_blend = 1.0; param_feedback = 0.0; param_feedforward = gain; }
		void set_iir_comb_filter(double scale, double gain) { param_blend = scale; param_feedback = gain; param_feedforward = 0.0; }
		void set_allpass_filter(double c) { param_blend = c; param_feedback = -c; param_feedforward = 1.0; }
		void set_delay() { param_blend = 0.0; param_feedback = 0.0; param_feedforward = 1.0; }

		void reset() { m_delay.reset(); }

		_Delay& _Get_container() { return m_delay; }

		sample_t operator()(sample_t in)
		{
			sample_t x_new = in + param_feedback*m_delay.peek();
			sample_t out = param_feedforward*m_delay.peek() + param_blend*x_new;
			m_delay.push(x_new);
			return out;
		}

		double param_blend{}; // blend
		double param_feedback{}; // feedback
		double param_feedforward{}; // feedforward

	private:
		_Delay& m_delay;
	};
}




void miqs_TEST_OBJ_NAME::process()
{
	std::cout << std::fixed << std::setprecision(3);

	std::vector<sample_t> original(12, 0.0);
	original[0] = 1.0;


	std::vector<sample_t> samples = original;
	
	
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	/* fir comb filter */
	std::vector<sample_t> dbuff( 10,0.0);
	miqs::fraction_delay<miqs::interpolation::linear> fir_delay(2.0, dbuff);
	miqs::fir_comb_filter<decltype(fir_delay)> fircomb{ 0.9, fir_delay };


	std::cout << "::FIR COMB FILTER - fraction_delay(2.0)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), fircomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";



	//reset
	samples = original;


	/* iir comb filter */
	miqs::one_sample_delay iir_delay;
	miqs::iir_comb_filter<decltype(iir_delay)> iircomb{ 0.6,0.6, iir_delay };

	std::cout << "::IIR COMB FILTER - delay(1)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), iircomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";



	//reset
	samples = original;


	fir_delay.reset();

	/* universal comb filter */
	miqs::universal_comb_filter <decltype(fir_delay)> ucomb{ fir_delay};
	// fir comb filter
	ucomb.set_fir_comb_filter(0.9);

	std::cout << "::UNIVERSAL COMB FILTER [FIR COMB FILTER]- delay(2.0)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), ucomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";



	//reset
	samples = original;
	fir_delay.reset();
	fir_delay.set_delay(1.0);
	// iir comb filter
	ucomb.set_iir_comb_filter(0.6, 0.6);
	
	std::cout << "::UNIVERSAL COMB FILTER [IIR COMB FILTER]- delay(1.0)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), ucomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	//reset
	samples = original;
	fir_delay.reset();
	fir_delay.set_delay(4.0);

	// delay
	ucomb.set_delay();
	std::cout << "::UNIVERSAL COMB FILTER [Delay]- delay(4.0)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), ucomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	//reset
	samples = original;
	fir_delay.reset();
	fir_delay.set_delay(1.0);

	// allpass
	sample_t c = 0.9;

	ucomb.set_allpass_filter(c);
	std::cout << "::UNIVERSAL COMB FILTER [Allpass]- delay(1.0)::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), ucomb);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


	//reset
	samples = original;
	fir_delay.reset();

	miqs::first_order_allpass_filter allpass{&c, fir_delay._Get_container()._Get_container()};
	std::cout << "::1st ORDER ALLPASS FILTER::\n";
	std::transform(std::begin(samples), std::end(samples), std::begin(samples), allpass);
	std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<sample_t>(std::cout, " "));
	std::cout << "\n\n";


}
#undef miqs_TEST_OBJ_NAME