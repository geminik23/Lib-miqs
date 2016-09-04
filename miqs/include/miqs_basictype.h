#pragma once

#include "miqs_headers.h"


namespace miqs
{

	typedef std::int8_t sbyte_t;
	typedef std::int16_t int16_t;
	typedef std::int32_t int32_t;
	typedef std::int64_t int64_t;

	typedef std::uint8_t byte_t;
	typedef std::uint16_t uint16_t;
	typedef std::uint32_t uint32_t;
	typedef std::uint64_t uint64_t;

	typedef float float32_t;
	typedef double float64_t;

	typedef double sample_t;

	enum class type_format
	{
		int_8bit = 0,
		int_16bit,
		int_24bit,
		int_32bit,
		float_32bit,
		float_64bit
	};

}