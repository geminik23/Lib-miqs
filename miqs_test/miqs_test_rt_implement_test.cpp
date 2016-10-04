#include "miqs_test.h"
#include "miqs_temp_impl.h"
using namespace miqs_test;
using namespace miqs;
#define miqs_TEST_OBJ_NAME implement_test

namespace miqs
{

}

constexpr size_t length = 512;
constexpr size_t step_size = 300;




miqs_TEST_OBJ_NAME::miqs_TEST_OBJ_NAME():
	m_cur_position{}, m_ratio{1.7},
	m_window(length),
	m_index1(length), m_index2(length)
{
	
	//Sound File
	auto sndIn = Miqs::SoundIn::ReadSoundFile("Toms_diner.wav");
	auto sndsize = sndIn.size();
	m_samples.reset(sndsize);
	sndIn.readDoubleFrames(miqs::data(m_samples), sndsize);
	

	// fill window
	auto win_arr = miqs::make_array_accessor(m_window);
	miqs::phasor_index win_ph;
	auto win_gen = miqs::make_generator<miqs::window::hanning_f<512>>(win_ph);
	std::generate(std::begin(win_arr), std::end(win_arr), win_gen);



	auto acc_i1 = miqs::make_array_accessor(m_index1);
	auto acc_i2 = miqs::make_array_accessor(m_index2);

	std::iota(m_index1.data(), m_index1.data()+512, 0.0);
	std::iota(std::begin(acc_i2), std::end(acc_i2), 0.0);

	std::transform(std::begin(acc_i1), std::end(acc_i1), std::begin(acc_i1), miqs::multiplies_const<sample_t>(m_ratio));
	std::transform(std::begin(acc_i2), std::end(acc_i2), std::begin(acc_i2), miqs::multiplies_const<sample_t>(m_ratio));

	//std::rotate(std::begin(acc_i2), std::begin(acc_i2) + m_index1.size() / 2, std::end(acc_i2));

}


void miqs_TEST_OBJ_NAME::process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length)
{
	miqs::array<size_t> temp(512);

	(void)in; (void)nchi; (void)ncho;
	miqs::array_accessor<sample_t> i1{ in , length };
	miqs::array_accessor<sample_t> i2{ in+length , length };

	miqs::array_accessor<sample_t> o1{ out , length };
	miqs::array_accessor<sample_t> o2{ out + length , length };

	// check
	if (m_samples.size() - (length * 2) < m_cur_position)
	{
		std::copy(std::begin(i1), std::end(i1), std::begin(o1));
		std::copy(std::begin(i1), std::end(i1), std::begin(o2));

		return;
	}

	miqs::array<> grain1(length);
	miqs::array<> grain2(length);

	
	
	std::fill(std::begin(o1), std::end(o1), 0.0);
	std::fill(std::begin(o2), std::end(o2), 0.0);

	auto samples = miqs::make_array_accessor(m_samples);
	auto c_samples = miqs::circular_adaptor<miqs::array_accessor<sample_t>, false>(samples);
	auto wavetable = miqs::make_wavetable<miqs::interpolation::cubic>(c_samples);
	auto window = miqs::make_array_accessor(m_window);
	auto acci1 = miqs::make_array_accessor(m_index1);
	auto acci2 = miqs::make_array_accessor(m_index2);

	//std::transform(std::begin(acci1), std::end(acci1),
	//			   std::begin(window),
	//			   std::begin(grain),
	//			   miqs::bind_func_2arg<LEFT>(std::multiplies<sample_t>(), miqs::make_cascades_functions<sample_t>(miqs::plus_const<sample_t>(static_cast<sample_t>(m_cur_position)), wavetable)));

	miqs::ptr_transform(miqs::ptr_begin(acci1),miqs::ptr_end(acci1),
				   miqs::ptr_begin(grain1),
				   miqs::make_cascades_functions<sample_t>(miqs::plus_const<sample_t>(static_cast<sample_t>(m_cur_position)), wavetable));

	miqs::ptr_transform(miqs::ptr_begin(o1), miqs::ptr_end(o1), miqs::ptr_begin(grain1), miqs::ptr_begin(o1), std::plus<sample_t>());

	//std::transform(std::begin(acci2), std::end(acci2),
	//			   std::begin(window),
	//			   std::begin(grain),
	//			   miqs::bind_func_2arg<LEFT>(std::multiplies<sample_t>(), miqs::make_cascades_functions<sample_t>(miqs::plus_const<sample_t>(static_cast<sample_t>(m_cur_position + length/2)), wavetable)));
	//std::transform(std::begin(o1), std::end(o1), std::begin(grain), std::begin(o1), std::plus<sample_t>());
	//

	miqs::ptr_transform(miqs::ptr_begin(o1), miqs::ptr_end(o1), miqs::ptr_begin(grain1), miqs::ptr_begin(o1), std::plus<sample_t>());



	miqs::ptr_normalize(miqs::ptr_begin(o1), miqs::ptr_end(o1), 0.89);
	miqs::ptr_copy(miqs::ptr_begin(o1), miqs::ptr_end(o2), miqs::ptr_begin(o2));
	m_cur_position += length;






}
#undef miqs_TEST_OBJ_NAME
