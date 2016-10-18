#pragma once

#include "miqs_basictype.h"

namespace miqs{

	// generator
	template <typename _OSC, typename _PHASOR>
	struct generator
	{
		typedef sample_t result_type;

		generator(_PHASOR& phasor):m_phasor{ phasor } {}

		sample_t operator()()
		{
			sample_t t = m_osc(m_phasor());
			return t;
		}

	private:
		_OSC m_osc;
		_PHASOR& m_phasor;
	};


	// generator_ref
	template <typename _OSC, typename _PHASOR>
	struct generator_ref
	{
		typedef sample_t result_type;

		generator_ref(_OSC& osc, _PHASOR& phasor):m_osc{ osc }, m_phasor{ phasor } {}

		sample_t operator()()
		{
			sample_t t = m_osc(m_phasor());
			return t;
		}

	private:
		_OSC& m_osc;
		_PHASOR& m_phasor;
	};





	template <typename _OSC, typename _Phase>
	auto make_generator(_Phase& phase) -> generator<_OSC, _Phase>
	{
		return generator<_OSC, _Phase>(std::ref(phase));
	}

	template <typename _OSC, typename _Phase>
	auto make_generator(_OSC& osc, _Phase& phase) -> generator_ref<_OSC, _Phase>
	{
		return generator_ref<_OSC, _Phase>(std::ref(osc), std::ref(phase));
	}

}