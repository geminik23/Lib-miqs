#pragma once
#include "miqs_basictype.h"

namespace miqs
{

	template <typename _GEN>
	struct trans_wrapper
	{
		trans_wrapper(_GEN gen):m_gen{ gen } {}

		sample_t operator()(sample_t v)
		{
			return v*m_gen();
		}

		_GEN m_gen;
	};


	template <typename _GEN>
	trans_wrapper<_GEN> make_trans_wrapper(_GEN gen)
	{
		return trans_wrapper<_GEN>(gen);
	}

}