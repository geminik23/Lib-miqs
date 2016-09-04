#pragma once

#include "miqs_basictype.h"

namespace miqs{

	// generator
	template <typename _OSC, typename _PHASOR>
	struct generator
	{
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



}