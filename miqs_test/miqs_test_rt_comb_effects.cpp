#include "miqs_test.h"
using namespace miqs_test;
using namespace miqs;
#include <random>

#define WHITENOISE 1
#define FLANGE 0

static std::default_random_engine& get_re()
{
	static std::random_device rd;
	static std::default_random_engine re{ rd()};
	return re;
}

static std::uniform_real_distribution<>& get_distribution()
{
	static std::uniform_real_distribution<> dist{ -1,1.0 };
	return dist;
}

#define miqs_TEST_OBJ_NAME comb_effects
miqs_TEST_OBJ_NAME::miqs_TEST_OBJ_NAME():
	delay{ 512 },
	iircomb{0.76, 0.99, delay }{

#if WHITENOISE

	double frequency = 110;
	size_t d = static_cast<size_t>(std::round(48000 / frequency));
	delay.set_delay(d);

	
#elif FLANGE

	

#endif



}

void miqs_TEST_OBJ_NAME::process(sample_t* in, size_t nchi, sample_t * out, size_t ncho, size_t length)
{
	(void)in; (void)nchi; (void)ncho;

	

#if WHITENOISE
	std::vector<sample_t> noise(length);
	std::generate(std::begin(noise), std::end(noise), []() {
		return get_distribution()(get_re());
	});

	miqs::array_accessor<sample_t> arr(out, length);

	std::transform(std::begin(noise), std::end(noise), std::begin(arr), this->iircomb);



#elif FLANGE

#endif

}

#undef miqs_TEST_OBJ_NAME
