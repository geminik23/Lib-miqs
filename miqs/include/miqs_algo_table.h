#pragma once

#include "miqs_algo_basics.h"

namespace miqs
{
	template <typename _PtrOut, typename _PtrTable, typename _PhaseInfo>
	void ptr_table_lookup(_PtrOut first, _PtrOut last, _PtrTable table, _PhaseInfo& info)
	{
		int idx;
		for (; first < last; ++first)
		{
			*first = table[static_cast<int>(info.phase + 0.5)];
			info.next();
		}
	}


	template <typename _PtrOut, typename _PtrTable, typename _PhaseInfo>
	void ptr_table_lookup(_PtrOut first, _PtrOut last, int oi, _PtrTable table, _PhaseInfo& info)
	{
		int idx;
		for (; first < last; first += oi)
		{
			*first = table[static_cast<int>(info.phase + 0.5)];
			info.next();
		}
	}


	template <typename _Interpolator, typename _PtrOut, typename _PtrTable, typename _PhaseInfo>
	void ptr_table_lookup(_PtrOut first, _PtrOut last, _PtrTable table, _PhaseInfo& info)
	{
		_Interpolator interp;
		int idx;
		for (; first < last; ++first)
		{
			idx = static_cast<int>(info.phase);
			*first = interp(table + (idx- _Interpolator::REQUIRE_PRE_SIZE), info.phase - idx);
			info.next();
		}
	}


	template <typename _Interpolator, typename _PtrOut, typename _PtrTable, typename _PhaseInfo>
	void ptr_table_lookup(_PtrOut first, _PtrOut last, int oi, _PtrTable table, _PhaseInfo& info)
	{
		_Interpolator interp;
		int idx;
		for (; first < last; first+=oi)
		{
			idx = static_cast<int>(info.phase);
			*first = interp(table + (idx - _Interpolator::REQUIRE_PRE_SIZE), info.phase - idx);
			info.next();
		}
	}

	//-------------------rotating
	// use phase_modular
	template <typename _Interpolator, typename _PtrOut, typename _PtrTable, typename _PhaseInfo>
	void ptr_table_lookup_rotating(_PtrOut first, _PtrOut last, _PtrTable tfirst, _PtrTable tlast, _PhaseInfo& info)
	{
		_Interpolator interp;
		int i,idx, k;
		size_t tsize = std::distance(tfirst, tlast);
		std::iterator_traits<_PtrTable>::value_type param[_Interpolator::REQUIRE_PARAM_SIZE];

		for (; first != last; ++first)
		{
			idx = static_cast<int>(info.phase);
			if (idx + _Interpolator::REQUIRE_PARAM_SIZE - -_Interpolator::REQUIRE_PRE_SIZE >= (int)tsize)
			{
				k = (idx + _Interpolator::REQUIRE_PARAM_SIZE - -_Interpolator::REQUIRE_PRE_SIZE - tsize);
				for (i= 0; i < (_Interpolator::REQUIRE_PARAM_SIZE - k); ++i)
				{
					param[i] = *(tfirst + (idx - _Interpolator::REQUIRE_PRE_SIZE));
				}
				for (; i < _Interpolator::REQUIRE_PARAM_SIZE; ++i,++k)
				{
					param[i] = *(tfirst + k);
				}

				*first = interp(param, info.phase - idx);
			}
			else
			{
				*first = interp(tfirst + (idx - _Interpolator::REQUIRE_PRE_SIZE), info.phase - idx);
			}
			info.next();
		}
	}


}