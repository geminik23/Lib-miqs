#pragma once

// core

#include "miqs_basictype.h"
#include "miqs_utils.h"
#include "miqs_math.h"
#include "miqs_phase_info.h"
#include "miqs_interpolation.h"
#include "miqs_generator.h"
#include "miqs_basic_algorithm.h"
#include "miqs_circular_adaptor.h"


// envelope
#include "miqs_envelopes.h"

// window
#include "miqs_windows.h"

// iterators
#include "miqs_iter_generate.h"

// operators
#include "miqs_ops_basic.h"
#include "miqs_ops_oscillators.h"
#include "miqs_ops_envelopes.h"

// containers
#include "miqs_cont_array_accessor.h"
#include "miqs_cont_sample_wrapper.h"

// delay ::[require container (circular_adaptor) ]
#include "miqs_delay_basic.h"
#include "miqs_delay_dynamic.h"
#include "miqs_delay_fixed.h"
#include "miqs_delay_fraction.h"

// filters
#include "miqs_filter_basic.h"
#include "miqs_filter_all_zero.h"
#include "miqs_filter_all_pole.h"
#include "miqs_filter_1st_allpass.h"
#include "miqs_filter_based_1st_allpass.h"
#include "miqs_filter_2nd_allpass.h"
#include "miqs_filter_based_2nd_allpass.h"
#include "miqs_filter_canonical.h"
#include "miqs_filter_fir_comb.h"
#include "miqs_filter_iir_comb.h"
#include "miqs_filter_universal_comb.h"

// transform
#include "miqs_trans_functions.h"
#include "miqs_trans_dft.h"
#include "miqs_trans_fft.h"


// functions
#include "miqs_functions.h"
#include "miqs_func_panning.h"
#include "miqs_func_cascade_filter.h"
#include "miqs_func_combines.h"
#include "miqs_func_filter_coeff_allpass.h"
#include "miqs_func_filter_coeff_canonical.h"

#include "miqs_func_bind_func2.h"
#include "miqs_func_bind_complex_func1.h"
#include "miqs_func_cascades.h"
#include "miqs_func_wrap_trans_func.h"
#include "miqs_func_fft_two_signal.h"
#include "miqs_func_fast_convolution.h"
#include "miqs_func_linear_combination.h"



