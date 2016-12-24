#pragma once


#include "base.h"
//#include <limits>

namespace miqs
{
	static const double eps = 0.0000000000000001;
	static const double pi = 3.14159265358979323846;
	static const double two_pi = pi * 2;
	static const double e = 2.71828182845904523536;
	static const double sqrt_2 = 1.4142135623730950488016;


	template <typename _Ty> inline _Ty max_value(_Ty a, _Ty b){	return (a > b) ? a : b;}
	template <typename _Ty>	inline _Ty min_value(_Ty a, _Ty b){	return (a < b) ? a : b;}

	inline double log10_x(double x, double product) { return (double)product * std::log10(x); }
	inline double sinc_pi(double i) { return (i == 0.0) ? 1.0 : (sin(i * pi) / (i * pi)); }
	inline double sinc(double i){return (i == 0.0) ? 1.0 : (sin(i) / (i));}
	//inline void exp_cos_sin(double phi, double& c, double& s){c = cos(phi);	s = sin(phi);}
	//template <typename _Ty = sample_t> inline std::pair<sample_t, sample_t> exp_cos_sin(double phi)
	//{double c, s;exp_cos_sin(phi, s, c); return std::pair<sample_t, sample_t>(static_cast<sample_t>(c), static_cast<sample_t>(s));}

	template<typename T> inline int32_t sign(T & a) { return ((a >= 0) ? 1 : (-1)); }

	// amp <->loudness(db)
	inline double amp_to_db(double amp) { return 20 * std::log10(amp); }
	inline double db_to_amp(double db) { return pow(10, db / 20); }

	// degree <-> radian
	inline double degree_to_radian(double deg) { return deg * pi / 180.0; }
	inline double radian_to_degree(double rad) { return rad * 180.0 / pi; }

	// midi <-> freq
	inline double midi_to_freq(double midi) { return std::pow(2, (midi - 69) / 12) * 440.0; }
	inline double freq_to_midi(double freq) { return 12 * std::log2(freq / 440.0) + 69; }

	// semitone <-> ratio
	inline double semitone_to_ratio(double semi) { return std::pow(2, (semi) / 12); }
	inline double ratio_to_semitone(double ratio) { return std::log2(ratio) * 12; }

	// milliseconds <-> samples
	inline uint32_t ms_to_samples(double ms, uint32_t samplerate) { return static_cast<uint32_t>((ms / 1000.0) * samplerate); }
	inline double samples_to_ms(uint32_t samples, uint32_t samplerate) { return ((static_cast<double>(samples) / samplerate * 1000)); }

	// hertz <-> mel
	inline double hertz_to_mel(double f) { return 1127.0 * std::log(1.0 + f / 700.0); }
	inline double mel_to_hertz(double mel) { return 700.0*(std::exp(mel / 1127.0) - 1.0); }

	// reflect <-> lpc
	inline void lpc_to_reflect(double* a, double* k, size_t order);
	inline void reflect_to_lpc(double* k, double* a, size_t order);

	// sign
	template<typename T>inline int32_t sign(T a) { return ((a >= 0) ? 1 : (-1)); }
	template<typename T>inline double half_sign(T a) { return ((a >= 0) ? 0.5 : (-0.5)); }

	// zero_cross
	template <typename _Iter>
	uint32_t zero_cross(_Iter biter, _Iter eiter)
	{
		uint32_t result = 0;
		for (_Iter i = biter; i != eiter - 1; i++)
			result += static_cast<uint32_t>(std::abs(half_sign(*(i + 1)) - half_sign((*i))));
		return result;
	}

	/* bounded zero-cross*/
	// bound is samples
	template <typename _Iter>
	uint32_t bounded_zero_cross(_Iter biter, _Iter eiter, std::pair<size_t, size_t> bound)
	{
		uint32_t result = 0;
		size_t count = 0;
		bool first = true;
		for (_Iter i = biter; i != eiter - 1; i++)
		{
			++count;
			if (static_cast<uint32_t>(std::abs(half_sign(*(i + 1)) - half_sign((*i)))))
			{
				if (first || (count >= bound.first && count <= bound.second))
				{
					first = false;
					++result;
					count = 0;
				}
			}
			if (count > bound.second)
			{
				first = true;
				count = 0;
			}
		}
		return result;
	}
}


inline void miqs::lpc_to_reflect(double* a, double* k, size_t order)
{
	double *b, *b1, ee;
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
		ee = 1 - (k[i] * k[i]);
		for (j = 1; j < i; j++)
			b1[j] = b[j];
		for (j = 1; j < i; j++)
			b[j] = (b1[j] - k[i] * b1[i - j]) / ee;
	}

	delete[] b;
	delete[] b1;
}


inline void miqs::reflect_to_lpc(double* k, double* a, size_t order)
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