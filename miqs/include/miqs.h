#pragma once




/**

[  ] equalizer p.80

[  ] sound file testing

[  ] granular implements

[  ] wavetable -> samplerate

[  ] need safe container

[  ] vibrato fraction delay version.

[  ] delay ref version.

[  ] delay seperate the files

[ O ] PROBLEM universal_comb_filter <- fraction_delay  
	- minimum delay set to 1 not 0
*/

#include "miqs_basictype.h"
#include "miqs_utils.h"
#include "miqs_math.h"
#include "miqs_phasors.h"
#include "miqs_interpolation.h"


// operators
#include "miqs_ops_basic.h"
#include "miqs_ops_bind.h"

// functions
#include "miqs_functions.h"
#include "miqs_func_cascade_filter.h"
#include "miqs_func_combines.h"
#include "miqs_func_filter_coeff_allpass.h"
#include "miqs_func_filter_coeff_canonical.h"

// containers
#include "miqs_cont_array_accessor.h"
#include "miqs_cont_sample_wrapper.h"
#include "miqs_cont_circular_adaptor.h"
#include "miqs_cont_circular_block.h"

// delay ::[require container (circular_adaptor) ]
#include "miqs_delay_basic.h"
#include "miqs_delay_fraction.h"


// window
#include "miqs_windows.h"

// generator
#include "miqs_gen_generator.h"
#include "miqs_gen_wavetable.h"

// filters
#include "miqs_filter_basic.h"
#include "miqs_filter_all_zero.h"
#include "miqs_filter_all_pole.h"
#include "miqs_filter_1st_allpass.h"
#include "miqs_filter_based_1st_allpass.h"
#include "miqs_filter_2nd_allpass.h"
#include "miqs_filter_based_2nd_allpass.h"
#include "miqs_filter_canonical.h"

// transform
#include "miqs_trans_functions.h"
#include "miqs_trans_fft.h"

#include "miqs_implements.h"