#pragma once



namespace miqs
{
	//===============modular
#define MIQS_PHASE_NEXT(PHASE_INFO) PHASE_INFO.phase += PHASE_INFO.step
#define MIQS_PHASE_MODULAR_UP(PHASE_INFO, BOUND) if(PHASE_INFO.phase>=BOUND) PHASE_INFO.phase -= BOUND
#define MIQS_PHASE_MODULAR_DOWN(PHASE_INFO, BOUND) if(PHASE_INFO.phase<0) PHASE_INFO.phase = BOUND-PHASE_INFO.step


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


}