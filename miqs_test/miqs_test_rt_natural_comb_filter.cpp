#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;
#define miqs_TEST_OBJ_NAME natural_comb_filter

miqs_TEST_OBJ_NAME::miqs_TEST_OBJ_NAME() {
}


void miqs_TEST_OBJ_NAME::process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length)
{
	(void)in; (void)nchi; (void)ncho; (void)length; (void)out;

}
#undef miqs_TEST_OBJ_NAME
