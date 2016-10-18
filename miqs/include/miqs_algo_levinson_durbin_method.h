#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	template <typename _Ty>
	inline _Ty levinson_durbin_method(_Ty* ac, _Ty* lc, _Ty* rc, size_t lpc_order)
	{
		size_t i, m;
		_Ty ee;
		//	int errF = 1;
		_Ty k_m;

		if ((ac[0] == 0.0) || (ac[0] == -0.0))
		{
			for (i = 0; i < lpc_order; i++) lc[i] = 0.0;
			return 0.0;
		}

		rc[0] = 1.0;
		lc[0] = 1.0;

		ee = ac[0];
		for (m = 1; m <= lpc_order; m++)
		{
			_Ty sum = (_Ty)1.0 * ac[m];
			for (i = 1; i < m; i++)
			{
				sum += lc[i] * ac[m - i];
			}
			k_m = ((_Ty)-1.0 / ee) * sum;

			// save reflection coefficients
			rc[m] = k_m;
			lc[m] = k_m;


			for (i = 1; i <= m / 2; i++)
			{
				_Ty x = lc[i];
				lc[i] += k_m * lc[m - i];
				if ((i < (m / 2)) || ((m & 1) == 1)) lc[m - i] += k_m * x;
			}

			// update the error:
			ee *= ((_Ty)1.0 - k_m*k_m);
			if (ee == 0.0)
			{
				for (i = m; i <= lpc_order; i++)
				{
					lc[i] = 0.0;
					rc[m] = 0.0;
				}
				break;
			}
		}
		return ee;
	}

}
