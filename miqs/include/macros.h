#pragma once

namespace miqs
{
	//===============modular
#define MIQS_PHASE_NEXT(PHASE_INFO) PHASE_INFO.phase += PHASE_INFO.step
#define MIQS_PHASE_MODULAR_UP(PHASE_INFO, BOUND) if(PHASE_INFO.phase>=BOUND) PHASE_INFO.phase -= BOUND
#define MIQS_PHASE_MODULAR_DOWN(PHASE_INFO, BOUND) if(PHASE_INFO.phase<0) PHASE_INFO.phase = BOUND-PHASE_INFO.step


#define MIQS_PTR_CIRCULAR_INCR_TAIL(CIRCULAR_TYPE) CIRCULAR_TYPE.tail = (CIRCULAR_TYPE.tail + 1 ==  CIRCULAR_TYPE.size) ? 0 : CIRCULAR_TYPE.tail + 1
#define MIQS_PTR_CIRCULAR_INCR_HEAD(CIRCULAR_TYPE) CIRCULAR_TYPE.head = ( CIRCULAR_TYPE.head + 1 == CIRCULAR_TYPE.size) ? 0 :  CIRCULAR_TYPE.head + 1
#define MIQS_PTR_CIRCULAR_DECR_TAIL(CIRCULAR_TYPE) CIRCULAR_TYPE.tail = (CIRCULAR_TYPE.tail == 0) ? CIRCULAR_TYPE.size - 1 : CIRCULAR_TYPE.tail - 1
#define MIQS_PTR_CIRCULAR_DECR_HEAD(CIRCULAR_TYPE)  CIRCULAR_TYPE.head = ( CIRCULAR_TYPE.head == 0) ? CIRCULAR_TYPE.size - 1 :  CIRCULAR_TYPE.head - 1
#define MIQS_PTR_CIRCULAR_AT_TAIL(CIRCULAR_TYPE) (CIRCULAR_TYPE.ptr + CIRCULAR_TYPE.tail)
#define MIQS_PTR_CIRCULAR_AT_HEAD(CIRCULAR_TYPE) (CIRCULAR_TYPE.ptr + CIRCULAR_TYPE.head)
#define MIQS_PTR_MODULAR(VALUE, SIZE) while(VALUE>=SIZE) VALUE-=SIZE;while(VALUE<0) VALUE+=SIZE
#define MIQS_PTR_MODULAR_UP(VALUE, SIZE) VALUE = (VALUE>=SIZE)?VALUE-SIZE:VALUE
#define MIQS_PTR_MODULAR_DOWN(VALUE, SIZE) VALUE = (VALUE<0)?VALUE+SIZE:VALUE

	//================cartesian <-> polar
#define MIQS_PTR_C2P_MAGNITUDE(X1, X2) (std::sqrt(*(X1)**(X1) + *(X2)**(X2)))
#define MIQS_PTR_C2P_PHASE(X1, X2) (std::atan2(*(X2), *(X1)))
#define MIQS_PTR_C2P(X1, X2, M, P) do{\
	*(M) = MIQS_PTR_C2P_MAGNITUDE(X1,X2);\
	*(P) = MIQS_PTR_C2P_PHASE(X1, X2); \
	}while(0)

#define MIQS_PTR_C2P_ITSELF(X1, X2) do{\
std::iterator_traits<decltype((X1))>::value_type __miqs_value = *(X1);\
	*(X1) = MIQS_PTR_C2P_MAGNITUDE(&__miqs_value,X2);\
	*(X2) = MIQS_PTR_C2P_PHASE(&__miqs_value, X2); \
}while(0)

#define MIQS_PTR_P2C_REAL(X1, X2) (*(X1)*std::cos(*(X2)))
#define MIQS_PTR_P2C_IMAGINARY(X1, X2) (*(X1)*std::sin(*(X2)))
#define MIQS_PTR_P2C(X1, X2, M, P) do{\
	*(M) = MIQS_PTR_P2C_REAL(X1,X2);\
	*(P) = MIQS_PTR_P2C_IMAGINARY(X1, X2); \
	}while(0)
#define MIQS_PTR_P2C_ITSELF(X1, X2) do{\
	decltype(x1) __miqs_t_ptr = (X1);\
	*(X1) = MIQS_PTR_P2C_REAL(__miqs_t_ptr,X2);\
	*(X2) = MIQS_PTR_P2C_IMAGINARY(__miqs_t_ptr, X2); \
}while(0)



	//==============================Interpolation
#define MIQS_PTR_INTERP_LINEAR(LINEAR_TYPE, DST) *(DST) =  *(LINEAR_TYPE.x1) + (*(LINEAR_TYPE.x2) - *(LINEAR_TYPE.x1)) * (LINEAR_TYPE.frac)
#define MIQS_PTR_INTERP_CUBIC(LINEAR_TYPE, DST) do{\
			LINEAR_TYPE.x0 = *(LINEAR_TYPE.x1) + (*(LINEAR_TYPE.x1) - *(LINEAR_TYPE.x2));\
			LINEAR_TYPE.a3 = LINEAR_TYPE.frac * LINEAR_TYPE.frac; LINEAR_TYPE.a3 -= 1.0; LINEAR_TYPE.a3 *= (1.0 / 6.0);\
			LINEAR_TYPE.a2 = (LINEAR_TYPE.frac + 1.0) * 0.5; LINEAR_TYPE.a0 = LINEAR_TYPE.a2 - 1.0;\
			LINEAR_TYPE.a1 = LINEAR_TYPE.a3 * 3.0; LINEAR_TYPE.a2 -= LINEAR_TYPE.a1; LINEAR_TYPE.a0 -= LINEAR_TYPE.a3; \
			LINEAR_TYPE.a1 -= LINEAR_TYPE.frac;\
			LINEAR_TYPE.a0 *= LINEAR_TYPE.frac; LINEAR_TYPE.a1 *= LINEAR_TYPE.frac; \
			LINEAR_TYPE.a2 *= LINEAR_TYPE.frac; LINEAR_TYPE.a3 *= LINEAR_TYPE.frac; LINEAR_TYPE.a1 += 1.0;\
			*(DST) =  (LINEAR_TYPE.a0*(LINEAR_TYPE.x0) +LINEAR_TYPE.a1**(LINEAR_TYPE.x1) +LINEAR_TYPE.a2**(LINEAR_TYPE.x2) +LINEAR_TYPE.a3**(LINEAR_TYPE.x3));\
		}while(0)

}