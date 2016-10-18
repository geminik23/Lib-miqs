#pragma once
#include "miqs_basictype.h"
#include "miqs_math.h"

namespace miqs
{

	template <typename _Ty>
	inline _Ty schur_recursion(_Ty* ac, _Ty* lc, _Ty* rc, size_t lpc_order)
	{
		size_t i, j;
		_Ty temp, alphap;

		_Ty *y1, *y2;

		y1 = new _Ty[lpc_order + 2];
		y2 = new _Ty[lpc_order + 2];

		rc[1] = -ac[1] / ac[0];
		alphap = ac[0] * (1 - (rc[1] * rc[1]));

		y2[1] = ac[1];
		y2[2] = ac[0] + rc[1] * ac[1];

		for (i = 2; i <= lpc_order; i++)
		{
			y1[1] = temp = ac[i];

			for (j = 1; j < i; j++)
			{
				y1[j + 1] = y2[j] + rc[j] * temp;
				temp += rc[j] * y2[j];
				y2[j] = y1[j];
			}

			rc[i] = -temp / y2[i];
			y2[i + 1] = y2[i] + rc[i] * temp;
			y2[i] = y1[i];

			alphap *= 1 - (rc[i] * rc[i]);
		}

		reflect_to_lpc(rc, lc, lpc_order);

		delete[] y1;
		delete[] y2;
		return(alphap);
	}


}
