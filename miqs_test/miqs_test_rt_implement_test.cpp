#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;
#define miqs_TEST_OBJ_NAME implement_test

namespace miqs
{


}

miqs_TEST_OBJ_NAME::miqs_TEST_OBJ_NAME():
	phase(440.0, 48000)
{
	angle = 0;
}


void miqs_TEST_OBJ_NAME::process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length)
{

	(void)in; (void)nchi; (void)ncho;
	miqs::array_accessor<sample_t> i1{ in , length };
	miqs::array_accessor<sample_t> i2{ in+length , length };

	miqs::array_accessor<sample_t> o1{ out , length };
	miqs::array_accessor<sample_t> o2{ out + length , length };

}
#undef miqs_TEST_OBJ_NAME
