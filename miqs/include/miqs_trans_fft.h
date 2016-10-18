#pragma once
#include "miqs_basictype.h"

namespace miqs
{




	/*fft_radix2_twiddle */
	template <typename _Ty = sample_t>
	struct fft_twiddle
	{
		typedef _Ty value_type;

		void operator()(_Ty *twiddle, int size)
		{
			/* pre-compute radix-2 twiddle factors in one array of length n */
			/* re[0], re[1], ..., re[n/2-1], im[0], im[1], ..., im[n/2-1] */
			int i;
			int hsize = size / 2;
			for (i = 0; i < hsize; i++)
			{
				twiddle[i] = std::cos(miqs::pi*2.0 / hsize*i);
				twiddle[hsize + i] = std::sin(miqs::pi * 2.0 / hsize*i);
			}

		}
	};


	template <typename _Ty>
	struct realfft_data:_base_value_type<_Ty>
	{
		//set
		void set_fftsize(size_t size)
		{
			fftsize = size;
			twiddle.reset(size);
			miqs::fft_twiddle<typename realfft_data<_Ty>::value_type>()(twiddle, fftsize);
		}

		//get
		size_t get_fftsize() const noexcept
		{
			return fftsize;
		}

		size_t fftsize{ 512 };
		miqs::array<value_type> twiddle;
	};



	template <typename _Ty = sample_t>
	struct real_fft
	{
		typedef _Ty value_type;
		typedef value_type* pointer;

		static void unshuffle(value_type *data, int size)
		{

			int i, j, k, l, m;
			value_type re, im;

			l = size - 1;
			m = size >> 1;
			for (i = 0, j = 0; i < l; i++)
			{
				if (i < j)
				{
					re = data[j + j]; im = data[j + j + 1];
					data[j + j] = data[i + i]; data[j + j + 1] = data[i + i + 1];
					data[i + i] = re; data[i + i + 1] = im;
				}
				k = m;
				while (k <= j)
				{
					j -= k;
					k >>= 1;
				}
				j += k;
			}
		}




		//===============================
		static void dif_butterfly(value_type *data, int size, value_type *twiddle)
		{

			int angle, astep, dl;
			value_type xr, yr, xi, yi, wr, wi, dr, di;
			value_type *l1, *l2, *end, *ol2;

			astep = 1;
			end = data + size + size;
			for (dl = size; dl > 1; dl >>= 1, astep += astep)
			{
				l1 = data;
				l2 = data + dl;
				for (; l2 < end; l1 = l2, l2 = l2 + dl)
				{
					ol2 = l2;
					for (angle = 0; l1 < ol2; l1 += 2, l2 += 2)
					{
						wr = twiddle[angle];
						wi = -twiddle[size + angle]; /* size here is half the FFT size */
						xr = *l1 + *l2;
						xi = *(l1 + 1) + *(l2 + 1);
						dr = *l1 - *l2;
						di = *(l1 + 1) - *(l2 + 1);
						yr = dr * wr - di * wi;
						yi = dr * wi + di * wr;
						*(l1) = xr; *(l1 + 1) = xi;
						*(l2) = yr; *(l2 + 1) = yi;
						angle += astep;
					}
				}
			}
		}

		static void realize(value_type *data, int size)
		{

			value_type xr, yr, xi, yi, wr, wi, dr, di, ang, astep;
			value_type *l1, *l2;

			l1 = data;
			l2 = data + size + size - 2;
			xr = *l1;
			xi = *(l1 + 1);
			*l1 = xr + xi;
			*(l1 + 1) = xr - xi;
			l1 += 2;
			astep = miqs::pi / size;
			for (ang = astep; l1 <= l2; l1 += 2, l2 -= 2, ang += astep)
			{
				xr = (*l1 + *l2) / 2;
				yi = (-(*l1) + (*l2)) / 2;
				yr = (*(l1 + 1) + *(l2 + 1)) / 2;
				xi = (*(l1 + 1) - *(l2 + 1)) / 2;
				wr = std::cos(ang);
				wi = -std::sin(ang);
				dr = yr * wr - yi * wi;
				di = yr * wi + yi * wr;
				*l1 = xr + dr;
				*(l1 + 1) = xi + di;
				*l2 = xr - dr;
				*(l2 + 1) = -xi + di;
			}
		}



		void operator()(pointer first, pointer last, pointer twiddle)
		{
			size_t size = std::distance(first, last);
			//size_t i{};
			size >>= 1;
			dif_butterfly(first, size, twiddle);
			unshuffle(first, size);
			realize(first, size);

			//size <<= 1;
			//for (i = 0; i < size; i++)
			//	first[i] = first[i] / (size / 2);
		}

	};

	template <typename _Ty = sample_t>
	struct real_ifft
	{
		typedef _Ty value_type;
		typedef value_type* pointer;


		static void unshuffle(value_type *data, int size)
		{

			int i, j, k, l, m;
			value_type re, im;

			l = size - 1;
			m = size >> 1;
			for (i = 0, j = 0; i < l; i++)
			{
				if (i < j)
				{
					re = data[j + j]; im = data[j + j + 1];
					data[j + j] = data[i + i]; data[j + j + 1] = data[i + i + 1];
					data[i + i] = re; data[i + i + 1] = im;
				}
				k = m;
				while (k <= j)
				{
					j -= k;
					k >>= 1;
				}
				j += k;
			}
		}


		static void inverse_dit_butterfly(value_type *data, int size, value_type *twiddle)
		{

			int angle, astep, dl;
			value_type xr, yr, xi, yi, wr, wi, dr, di;
			value_type *l1, *l2, *end, *ol2;

			astep = size >> 1;
			end = data + size + size;
			for (dl = 2; astep > 0; dl += dl, astep >>= 1)
			{
				l1 = data;
				l2 = data + dl;
				for (; l2 < end; l1 = l2, l2 = l2 + dl)
				{
					ol2 = l2;
					for (angle = 0; l1 < ol2; l1 += 2, l2 += 2)
					{
						wr = twiddle[angle];
						wi = twiddle[size + angle]; /* size here is half the FFT size */
						xr = *l1;
						xi = *(l1 + 1);
						yr = *l2;
						yi = *(l2 + 1);
						dr = yr * wr - yi * wi;
						di = yr * wi + yi * wr;
						*(l1) = xr + dr; *(l1 + 1) = xi + di;
						*(l2) = xr - dr; *(l2 + 1) = xi - di;
						angle += astep;
					}
				}
			}
		}

		static void unrealize(value_type *data, int size)
		{

			value_type xr, yr, xi, yi, wr, wi, dr, di, ang, astep;
			value_type *l1, *l2;

			l1 = data;
			l2 = data + size + size - 2;
			xr = (*l1) / 2;
			xi = (*(l1 + 1)) / 2;
			*l1 = xr + xi;
			*(l1 + 1) = xr - xi;
			l1 += 2;
			astep = miqs::pi / size;
			for (ang = astep; l1 <= l2; l1 += 2, l2 -= 2, ang += astep)
			{
				xr = (*l1 + *l2) / 2;
				yi = -(-(*l1) + (*l2)) / 2;
				yr = (*(l1 + 1) + *(l2 + 1)) / 2;
				xi = (*(l1 + 1) - *(l2 + 1)) / 2;
				wr = std::cos(ang);
				wi = -std::sin(ang);
				dr = yr * wr - yi * wi;
				di = yr * wi + yi * wr;
				*l2 = xr + dr;
				*(l1 + 1) = xi + di;
				*l1 = xr - dr;
				*(l2 + 1) = -xi + di;
			}
		}


		void operator()(pointer first, pointer last, pointer twiddle)
		{
			size_t size = std::distance(first, last);
			size_t i{};

			size >>= 1;
			unrealize(first, size);
			unshuffle(first, size);
			inverse_dit_butterfly(first, size, twiddle);

			size <<= 1;
			for (i = 0; i<size; i++)
				first[i] = first[i]*2*size;//if( normalized using /size ) then use this
		}
	};

}