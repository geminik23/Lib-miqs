#pragma once

#include "miqs_basictype.h"
#include "miqs_math.h"

namespace miqs
{

	/* cartesian & polar coordinates */
	struct cartesian_to_polar
	{
		template <typename _Ty>
		std::complex<_Ty> operator()(const std::complex<_Ty>& c)
		{
			return std::complex<_Ty>(std::sqrt(c.real() * c.real() + c.imag() * c.imag()), std::atan2(c.imag(), c.real()));
		}
	};

	struct polar_to_cartesian
	{
		template <typename _Ty>
		std::complex<_Ty> operator()(const std::complex<_Ty>& c)
		{
			return std::complex<_Ty>(c.real()*std::cos(c.imag()), c.real()*std::sin(c.imag()));
		}
	};








}