#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	double levinson_durbin_method(double* ac, double* lc, double* rc, size_t lpc_order);

}
