#pragma once
#ifndef __MIQS_UTIL_HEADER_H
#define __MIQS_UTIL_HEADER_H

#include "miqs_basictype.h"

namespace miqs
{


	static union
	{
		int s = 1;
		char c[sizeof(int)];
	} __big_endian;

	// check : is big endian system?
	template <typename None = miqs::none>
	struct is_big_endian_system
	{
		static const bool value;
	};
	template <typename None>
	const bool is_big_endian_system<None>::value = (__big_endian.c[0] != 1);


	// swap bytes
	template <int N>
	void swap_bytes(byte_t *p)
	{
		register byte_t t;
		for (size_t i{}; i < N / 2; ++i)
		{
			t = p[i];
			p[i] = p[N - 1 - i];
			p[N - 1 - i] = t;
		}
	}

}


#endif