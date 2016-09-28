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
		template <typename _Ty>
		static _Ty get(std::complex<_Ty> const& complex)
		{
			return complex.real();
		}

		template <typename _Ty, typename _T2>
		static void set(std::complex<_Ty>& complex, _T2 const& v)
		{
			complex.real(v);
		}

	};

	struct imag_part
	{
		template <typename _Ty>
		static _Ty get(std::complex<_Ty> const& complex)
		{
			return complex.imag();
		}

		template <typename _Ty, typename _T2>
		static void set(std::complex<_Ty>& complex, _T2 const& v)
		{
			complex.imag(v);
		}
	};

	template <typename _Ty>
	void complex_swap(std::complex<_Ty>& c)
	{
		_Ty temp = c.imag();
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




	template <typename _Ty = sample_t>
	struct logarithmic_amplitude_20log
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		result_type operator() (const argument_type& v)
		{
			return 20 * std::log10(v) + std::numeric_limits<result_type>::epsilon();
		}
	};

	template <typename _Ty = sample_t>
	struct logarithmic_amplitude_10log
	{
		typedef _Ty argument_type;
		typedef _Ty result_type;

		result_type operator() (const argument_type& v)
		{
			return 10 * std::log10(v) + std::numeric_limits<result_type>::epsilon();
		}
	};

	template <typename _Ty = sample_t>
	struct log10 : _base_trans_arg1<_Ty>
	{
		result_type operator() (const argument_type& v)
		{
			return std::log10(v);
		}
	};

	template <typename _Ty = sample_t>
	struct log: _base_trans_arg1<_Ty>
	{
		result_type operator() (const argument_type& v)
		{
			return std::log(v);
		}
	};

	template <typename _Ty = sample_t>
	struct exp: _base_trans_arg1<_Ty>
	{
		result_type operator() (const argument_type& v)
		{
			return std::exp(v);
		}
	};


	//
	//
	//

	inline double log10_(double x, double product)
	{
		return (double)product * std::log10(x);
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

	// milliseconds <-> samples
	inline uint32_t ms_to_samples(uint32_t ms, uint32_t samplerate) { return static_cast<uint32_t>((ms / 1000.0) * samplerate); }
	inline uint32_t samples_to_ms(uint32_t samples, uint32_t samplerate) { return static_cast<uint32_t>((static_cast<double>(samples)/samplerate * 1000)); }

	// hertz <-> mel
	inline double hertzToMel(double f){return 1127.0 * std::log(1.0 + f / 700.0);}
	inline double melToHertz(double mel){return 700.0*(std::exp(mel / 1127.0) - 1.0);}

	// reflect <-> lpc
	inline void lpc_to_reflect(double* a, double* k, size_t order);
	inline void reflect_to_lpc(double* k, double* a, size_t order);
}


void miqs::lpc_to_reflect(double* a, double* k, size_t order)
{
	double *b, *b1, e;
	size_t   i, j;
	b = new double[order + 1];
	b1 = new double[order + 1];

	/* equate temporary variables (b = a) */
	for (i = 1; i <= order; i++)
		b[i] = a[i];

	/* compute reflection coefficients */
	for (i = order; i > 0; i--)
	{
		k[i] = b[i];
		e = 1 - (k[i] * k[i]);
		for (j = 1; j < i; j++)
			b1[j] = b[j];
		for (j = 1; j < i; j++)
			b[j] = (b1[j] - k[i] * b1[i - j]) / e;
	}

	delete[] b;
	delete[] b1;
}


void miqs::reflect_to_lpc(double* k, double* a, size_t order)
{
	size_t   i, j;

	double *a1;
	a1 = new double[order + 1];

	for (i = 1; i <= order; i++)
	{
		/* refl to a recursion */
		a[i] = k[i];
		for (j = 1; j < i; j++)
			a1[j] = a[j];
		for (j = 1; j < i; j++)
			a[j] = a1[j] + k[i] * a1[i - j];
	}
	delete[] a1;
}