#include "miqs_func_schur_recursion.h"
#include "miqs_func_levinson_durbin_method.h"


double miqs::levinson_durbin_method(double* ac, double* lc, double* rc, size_t lpc_order)
{
	size_t i, m;
	double e;
	//	int errF = 1;
	double k_m;

	if ((ac[0] == 0.0) || (ac[0] == -0.0))
	{
		for (i = 0; i < lpc_order; i++) lc[i] = 0.0;
		return 0.0;
	}

	rc[0] = 1.0;
	lc[0] = 1.0;

	e = ac[0];
	for (m = 1; m <= lpc_order; m++)
	{
		double sum = (double)1.0 * ac[m];
		for (i = 1; i < m; i++)
		{
			sum += lc[i] * ac[m - i];
		}
		k_m = ((double)-1.0 / e) * sum;

		// save reflection coefficients
		rc[m] = k_m;
		lc[m] = k_m;


		for (i = 1; i <= m / 2; i++)
		{
			double x = lc[i];
			lc[i] += k_m * lc[m - i];
			if ((i < (m / 2)) || ((m & 1) == 1)) lc[m - i] += k_m * x;
		}

		// update the error:
		e *= ((double)1.0 - k_m*k_m);
		if (e == 0.0)
		{
			for (i = m; i <= lpc_order; i++)
			{
				lc[i] = 0.0;
				rc[m] = 0.0;
			}
			break;
		}
	}
	return e;
}


double miqs::schur_recursion(double* ac, double* lc, double* rc, size_t lpc_order)
{
	size_t i, j;
	double temp, alphap;

	double *y1, *y2;

	y1 = new double[lpc_order + 2];
	y2 = new double[lpc_order + 2];

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