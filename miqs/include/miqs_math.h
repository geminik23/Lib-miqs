#pragma once


#include "miqs_basictype.h"
#include <limits>

namespace miqs
{

	static const double Eps = 0.0000000000000001;
	static const double PI = 3.14159265358979323846;
	static const double E = 2.71828182845904523536;
	static const double Sqrt_2 = 1.4142135623730950488016;


	//
	/* complex */
	//
	struct real_part
	{
		template <typename _T>
		static _T get(std::complex<_T> const& complex)
		{
			return complex.real();
		}

		template <typename _T, typename _T2>
		static void set(std::complex<_T>& complex, _T2 const& v)
		{
			complex.real(v);
		}

	};

	struct imag_part
	{
		template <typename _T>
		static _T get(std::complex<_T> const& complex)
		{
			return complex.imag();
		}

		template <typename _T, typename _T2>
		static void set(std::complex<_T>& complex, _T2 const& v)
		{
			complex.imag(v);
		}
	};

	template <typename _T>
	void complex_swap(std::complex<_T>& c)
	{
		_T temp = c.imag();
		c.imag(c.real());
		c.real(temp);
	}

	template <typename _Part, typename _Comp_Iter, typename _Single_Iter>
	void complex_copy_to_single(_Comp_Iter _First, _Comp_Iter _End, _Single_Iter _Dst)
	{
		size_t size = _End - _First;
		for (size_t i{}; i < size; ++i, ++_Dst, ++_First)
			*_Dst = _Part::get(*_First);
	}


	template <typename _Part, typename _Single_Iter, typename _Comp_Iter>
	void complex_copy_from_single(_Single_Iter _First, _Single_Iter _End, _Comp_Iter _Dst)
	{
		size_t size = _End - _First;
		for (size_t i{}; i < size; ++i, ++_Dst, ++_First)
			_Part::set(*_Dst, *_First);
	}


	template <typename _Part, typename _Comp_Iter, typename _Func>
	void complex_part_apply(_Comp_Iter _First, _Comp_Iter _End, _Func func)
	{
		size_t size = _End - _First;
		for (size_t i{}; i < size; ++i, ++_First)
			_Part::set(*_First, func(_Part::get(*_First)));
	}

	template <typename _Part, typename _Comp_Iter, typename _Func>
	void complex_part_transform(_Comp_Iter _First, _Comp_Iter _End, _Comp_Iter _Dst, _Func func)
	{
		size_t size = _End - _First;
		for (size_t i{}; i < size; ++i, ++_Dst, ++_First)
			_Part::set(*_Dst, func(_Part::get(*_First)));
	}






	//
	//
	//

	inline double log10_(double x, double product)
	{
		return (double)product * log10(x);
	}

	inline double sinc_pi(double i)
	{
		return (i == 0.0) ? 1.0 : (sin(i * M_PI) / (i * M_PI));
	}

	inline double sinc(double i)
	{
		return (i == 0.0) ? 1.0 : (sin(i) / (i));
	}

	inline void exp_cos_sin(double phi, double& c, double& s)
	{
		c = cos(phi);
		s = sin(phi);
	}

	inline std::complex<sample_t> exp_cos_sin(double phi)
	{
		double c, s;
		exp_cos_sin(phi, s, c);
		return std::complex<sample_t>(static_cast<sample_t>(c), static_cast<sample_t>(s));
	}

	template<typename T>
	inline int32_t sign(T & a) { return ((a >= 0) ? 1 : (-1)); }

	// degree <-> radian
	inline double degree_to_radian(double deg) { return deg * PI / 180.0; }
	inline double radian_to_degree(double rad) { return rad * 180.0 / PI; }
	
	// midi <-> freq
	inline double midi_to_freq(double midi) { return std::pow(2, (midi - 69) / 12) * 440.0; }
	inline double freq_to_midi(double freq) { return 12 * std::log2(freq / 440.0) + 69; }
}