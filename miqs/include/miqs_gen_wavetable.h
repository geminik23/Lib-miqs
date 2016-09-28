#pragma once
#include "miqs_basictype.h"

namespace miqs
{


	/* need safe container */
	template <typename _Interpolator, typename _Cont>
	struct wavetable
	{
		wavetable(_Cont& cont):m_table{ cont }
		{}
		size_t size() const noexcept { return m_table.size(); }
		_Cont& _Get_container() noexcept { return m_table; }

		sample_t operator()(double idx)
		{
			auto tap = static_cast<size_t>(idx);
			
			int offset = ((tap%this->size()) - _Interpolator::REQUIRE_PRE_SIZE);
			auto iter = std::begin(m_table) + offset;

			for (size_t i{}; i < _Interpolator::REQUIRE_PARAM_SIZE; ++i)
			{
				samples[i] = *iter++;
			}

			return interpolator(samples, static_cast<double>(idx - tap));
		}
		
	_Interpolator interpolator;
	private:
		sample_t samples[_Interpolator::REQUIRE_PARAM_SIZE];
		_Cont& m_table;
	};

	template <typename _Interpolator, typename _Cont>
	auto make_wavetable(_Cont& cont) -> wavetable<_Interpolator, _Cont>
	{
		return wavetable<_Interpolator, _Cont>(cont);
	}

}