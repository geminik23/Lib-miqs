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
		static void set(std::complex<_T>& complex,  _T2 const& v)
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
	void complex_part_copy_to(_Comp_Iter _First, _Comp_Iter _End, _Single_Iter _Dst)
	{
		size_t size = _End - _First;
		for (size_t i{}; i < size; ++i, ++_Dst, ++_First)
			*_Dst = _Part::get(*_First);
	}


	template <typename _Part, typename _Single_Iter, typename _Comp_Iter>
	void complex_part_copy_from(_Single_Iter _First, _Single_Iter _End, _Comp_Iter _Dst)
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
}