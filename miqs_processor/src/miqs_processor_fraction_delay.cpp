#include "../include/miqs_processor_fraction_delay.h"

using namespace miqs;
using namespace miqs::process;



base_fraction_delay::base_fraction_delay(double delay, size_t max_delay): m_delay(delay)
{
	this->set_name("Fraction Delay");
	this->reset_buffer(max_delay);
	this->set_inputs({ port_type::DefaultSignal });
	this->set_outputs({ port_type::DefaultSignal });
	this->set_gain(1.0);


}






//-----------------------------





delay_linear::value_type miqs::process::delay_linear::operator()(value_type in)
{
	value_type result = this->operator()();

	this->push(in);


	return result;
}

delay_linear::value_type miqs::process::delay_linear::operator()()
{
	size_t ipos;
	value_type result{};

	auto ppos = m_circular_buffer.tail - (m_delay - 1.0);
	MIQS_PTR_MODULAR_DOWN(ppos, m_circular_buffer.size);

	ipos = static_cast<size_t>(ppos);
	m_interp.frac = ppos - ipos;
	m_interp.x1 = m_circular_buffer.ptr + ipos;
	m_interp.x2 = m_circular_buffer.ptr + ((ipos + 1 >= m_circular_buffer.size) ? 0 : ipos + 1);
	MIQS_PTR_INTERP_LINEAR(m_interp, &result);

	return result * m_gain;
}

//-------------------------------------------------
void miqs::process::delay_linear::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
{
	auto nframe = info.frame_count();
	auto iptr = ins[0];
	auto iptr_end = iptr + nframe;
	auto optr = outs[0];
	size_t ipos;
	for (; iptr != iptr_end; ++iptr, ++optr)
	{
		auto ppos = m_circular_buffer.tail - (m_delay - 1.0);
		MIQS_PTR_MODULAR_DOWN(ppos, m_circular_buffer.size);

		ipos = static_cast<size_t>(ppos);
		m_interp.frac = ppos - ipos;
		m_interp.x1 = m_circular_buffer.ptr + ipos;
		m_interp.x2 = m_circular_buffer.ptr + ((ipos + 1 >= m_circular_buffer.size) ? 0 : ipos + 1);
		MIQS_PTR_INTERP_LINEAR(m_interp, optr);

		*optr *= m_gain;


		m_circular_buffer.increase_head();
		m_circular_buffer.increase_tail();
		*(m_circular_buffer.at_tail()) = *iptr;
	}
}


//-------------------------------

delay_cubic::value_type miqs::process::delay_cubic::operator()(value_type in)
{
	value_type result = this->operator()();

	this->push(in);

	return result;
}

delay_cubic::value_type miqs::process::delay_cubic::operator()()
{
	size_t ipos;
	value_type result{};

	auto ppos = m_circular_buffer.tail - (m_delay - 1.0);
	MIQS_PTR_MODULAR_DOWN(ppos, m_circular_buffer.size);

	ipos = static_cast<size_t>(ppos);
	m_interp.frac = ppos - ipos;
	m_interp.x1 = m_circular_buffer.ptr + ipos;
	m_interp.x2 = m_circular_buffer.ptr + ((ipos + 1 >= m_circular_buffer.size) ? 0 : ipos + 1);
	m_interp.x3 = m_circular_buffer.ptr + ((ipos + 2 >= m_circular_buffer.size) ? 0 : ipos + 2);

	MIQS_PTR_INTERP_CUBIC(m_interp, &result);

	return result*m_gain;
}

void miqs::process::delay_cubic::process(process_info& info, sample_t ** ins, size_t, sample_t ** outs, size_t)
{
	auto nframe = info.frame_count();
	auto iptr = ins[0];
	auto iptr_end = iptr + nframe;
	auto optr = outs[0];
	size_t ipos;
	for (; iptr != iptr_end; ++iptr, ++optr)
	{

		auto ppos = m_circular_buffer.tail - (m_delay - 1.0);
		MIQS_PTR_MODULAR_DOWN(ppos, m_circular_buffer.size);

		ipos = static_cast<size_t>(ppos);
		m_interp.frac = ppos - ipos;
		m_interp.x1 = m_circular_buffer.ptr + ipos;
		m_interp.x2 = m_circular_buffer.ptr + ((ipos + 1 >= m_circular_buffer.size) ? 0 : ipos + 1);
		m_interp.x3 = m_circular_buffer.ptr + ((ipos + 2 >= m_circular_buffer.size) ? 0 : ipos + 2);

		MIQS_PTR_INTERP_CUBIC(m_interp, optr);


		*optr *= m_gain;

		m_circular_buffer.increase_head();
		m_circular_buffer.increase_tail();
		*(m_circular_buffer.at_tail()) = *iptr;
	}


}


//-------------------------------------------------------------------------------
