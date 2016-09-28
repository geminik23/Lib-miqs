#pragma once
#include "miqs_basictype.h"

namespace miqs
{

	inline int32_t _int32_reverse(int32_t i)
	{
		i = (i & 0x55555555) << 1 | (int32_t)((uint32_t)i >> 1) & 0x55555555;
		i = (i & 0x33333333) << 2 | (int32_t)((uint32_t)i >> 2) & 0x33333333;
		i = (i & 0x0f0f0f0f) << 4 | (int32_t)((uint32_t)i >> 4) & 0x0f0f0f0f;
		i = (i << 24) | ((i & 0xff00) << 8) |
			((int32_t)((uint32_t)i >> 8) & 0xff00) | (int32_t)((uint32_t)i >> 24);
		return i;
	}

	template <typename _T>
	void tramsform_radix2(std::complex<_T>* complex, size_t length)
	{
		typedef _T complex_sample_type;
		int levels = (int)std::floor(std::log2((double)length));

		if ((size_t)(1 << levels) != length)
			throw std::invalid_argument("Length is not a power of 2");

		// TODO: keep those tables in memory
		complex_sample_type *cos_table = new complex_sample_type[length / 2];
		complex_sample_type *sin_table = new complex_sample_type[length / 2];
		for (uint32_t i = 0; i < length / 2; i++)
		{
			cos_table[i] = static_cast<complex_sample_type>(std::cos(2 * miqs::PI * i / (complex_sample_type)length));
			sin_table[i] = static_cast<complex_sample_type>(std::sin(2 * miqs::PI * i / (complex_sample_type)length));
		}


		// Bit-reversed addressing permutation
		for (int i = 0; i < (int)length; i++)
		{
			int j = ((int)((uint32_t)_int32_reverse(i) >> (32 - levels)));

			if (j > i)
			{
				auto temp = complex[i];
				complex[i] = complex[j];
				complex[j] = temp;
			}
		}


		// Cooley-Tukey decimation-in-time radix-2 FFT
		for (size_t size = 2; size <= length; size *= 2)
		{
			int halfsize = size / 2;
			int tablestep = length / size;

			for (int i = 0; i < (int)length; i += size)
			{
				for (int j = i, k = 0; j < i + halfsize; j++, k += tablestep)
				{
					int h = j + halfsize;
					complex_sample_type re = complex[h].real();
					complex_sample_type im = complex[h].imag();

					complex_sample_type tpre = +re * cos_table[k] + im * sin_table[k];
					complex_sample_type tpim = -re * sin_table[k] + im * cos_table[k];

					complex_sample_type rej = complex[j].real();
					complex_sample_type imj = complex[j].imag();

					complex[h].real(rej - tpre);
					complex[h].imag(imj - tpim);
					complex[j].real(rej + tpre);
					complex[j].imag(imj + tpim);
				}
			}

			// Prevent overflow in 'size *= 2'
			if (size == length)
				break;
		}

		delete[] cos_table;
		delete[] sin_table;
	}



	template <bool _FORWARD = true>
	struct transforms_fft
	{
		template < typename _DST>
		void operator()(_DST&dst)
		{
			size_t length = dst.size();

			if (length == 0)
				return;
			if ((length & (length - 1)) != 0)
				return;

			_DST::value_type * d = dst.data();

			if (!_FORWARD)
			{
				for (size_t i = 0; i < length; i++)
					complex_swap(d[i]);
			}
			//// Is power of 2
			tramsform_radix2(d, length);

			if (!_FORWARD)
			{
				for (size_t i = 0; i < length; i++)
				{
					d[i] /= (double)length;
					complex_swap(d[i]);
				}
			}
		}

		template <typename _SRC, typename _DST>
		void operator()(_SRC& src, _DST&dst)
		{
			if (dst != src)
				dst = src;

			this->operator()(dst);
		}
	};

	typedef transforms_fft<true> transforms_forward_fft;
	typedef transforms_fft<false> transforms_backward_fft;
}
//#pragma once
//#include "miqs_basictype.h"
//
//namespace miqs
//{
//
//	inline int32_t _int32_reverse(int32_t i)
//	{
//		i = (i & 0x55555555) << 1 | (int32_t)((uint32_t)i >> 1) & 0x55555555;
//		i = (i & 0x33333333) << 2 | (int32_t)((uint32_t)i >> 2) & 0x33333333;
//		i = (i & 0x0f0f0f0f) << 4 | (int32_t)((uint32_t)i >> 4) & 0x0f0f0f0f;
//		i = (i << 24) | ((i & 0xff00) << 8) |
//			((int32_t)((uint32_t)i >> 8) & 0xff00) | (int32_t)((uint32_t)i >> 24);
//		return i;
//	}
//
//	template <typename _Ty>
//	void tramsform_radix2(std::complex<_Ty>* complex, size_t length)
//	{
//		typedef _Ty complex_sample_type;
//		int levels = (int)std::floor(std::log2((double)length));
//
//		if ((size_t)(1 << levels) != length)
//			throw std::invalid_argument("Length is not a power of 2");
//
//		// TODO: keep those tables in memory
//		complex_sample_type *cos_table = new complex_sample_type[length / 2];
//		complex_sample_type *sin_table = new complex_sample_type[length / 2];
//		for (uint32_t i = 0; i < length / 2; i++)
//		{
//			cos_table[i] = static_cast<complex_sample_type>(std::cos(2 * miqs::PI * i / (complex_sample_type)length));
//			sin_table[i] = static_cast<complex_sample_type>(std::sin(2 * miqs::PI * i / (complex_sample_type)length));
//		}
//
//
//		// Bit-reversed addressing permutation
//		for (int i = 0; i < (int)length; i++)
//		{
//			int j = ((int)((uint32_t)_int32_reverse(i) >> (32 - levels)));
//
//			if (j > i)
//			{
//				auto temp = complex[i];
//				complex[i] = complex[j];
//				complex[j] = temp;
//			}
//		}
//
//
//		// Cooley-Tukey decimation-in-time radix-2 FFT
//		for (size_t size = 2; size <= length; size *= 2)
//		{
//			int halfsize = size / 2;
//			int tablestep = length / size;
//
//			for (int i = 0; i < (int)length; i += size)
//			{
//				for (int j = i, k = 0; j < i + halfsize; j++, k += tablestep)
//				{
//					int h = j + halfsize;
//					complex_sample_type re = complex[h].real();
//					complex_sample_type im = complex[h].imag();
//
//					complex_sample_type tpre = +re * cos_table[k] + im * sin_table[k];
//					complex_sample_type tpim = -re * sin_table[k] + im * cos_table[k];
//
//					complex_sample_type rej = complex[j].real();
//					complex_sample_type imj = complex[j].imag();
//
//					complex[h].real(rej - tpre);
//					complex[h].imag(imj - tpim);
//					complex[j].real(rej + tpre);
//					complex[j].imag(imj + tpim);
//				}
//			}
//
//			// Prevent overflow in 'size *= 2'
//			if (size == length)
//				break;
//		}
//
//		delete[] cos_table;
//		delete[] sin_table;
//	}
//
//
//
//
//
//	template <bool _FORWARD = true>
//	struct transforms_fft
//	{
//
//		template <typename _ContType, typename _Type = _ContType::value_type>
//		struct container_type_traits
//		{
//			typedef _Type value_type;
//			static value_type* get_data(_ContType& cont){return cont.data();}
//			static size_t size(_ContType& cont) noexcept {return cont.size() / 2;}
//		};
//
//		template <typename _ContType, typename _Type>
//		struct container_type_traits <_ContType, std::complex<_Type>>
//		{
//			typedef _Type value_type;
//			static value_type* get_data(_ContType& cont) { return reinterpret_cast<value_type*>(cont.data()); }
//			static size_t size(_ContType& cont) noexcept { return cont.size(); }
//		};
//
//		
//
//
//		template < typename _DST>
//		void operator()(_DST&dst)
//		{
//			using ContainerTraits = container_type_traits<_DST>;
//			using value_type = ContainerTraits::value_type;
//
//			size_t length = ContainerTraits::size(dst);
//			value_type * d = ContainerTraits::get_data(dst);
//
//			if (length == 0)
//				return;
//			if ((length & (length - 1)) != 0)
//				return;
//
//
//			if (!_FORWARD)
//			{
//				for (size_t i = 0; i < length; i++)
//					std::swap(d[i * 2], d[i * 2 + 1]);
//			}
//
//			//// Is power of 2
//			tramsform_radix2(reinterpret_cast<std::complex<value_type> *>(d), length);
//
//			if (!_FORWARD)
//			{
//				for (size_t i = 0; i < length; i++)
//				{
//					d[i] /= (double)length;
//					std::swap(d[i * 2], d[i * 2 + 1]);
//				}
//			}
//		}
//
//		template <typename _SRC, typename _DST>
//		void operator()(_SRC& src, _DST&dst)
//		{
//			if (dst != src)
//				dst = src;
//
//			this->operator()(dst);
//		}
//	};
//
//	typedef transforms_fft<true> transforms_forward_fft;
//	typedef transforms_fft<false> transforms_backward_fft;
//}