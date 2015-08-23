#ifndef XCBASE_TYPE_INC
#define XCBASE_TYPE_INC 100
#
namespace xc {
	typedef signed char sint8;
	typedef unsigned char uint8;
	typedef char int8;

	typedef signed short sint16;
	typedef unsigned short uint16;
	typedef short int16;

	typedef signed int sint32;
	typedef unsigned int uint32;
	typedef int int32;

	typedef signed long long sint64;
	typedef unsigned long long uint64;
	typedef long long int64;

	typedef float float32;
	typedef double float64;

	//functions
	typedef void(*vFp_v)(void);

	typedef void(*vFp_c)(char);
	typedef char(*cFp_v)(void);
	typedef void(*vFp_s)(short);
	typedef short(*sFp_v)(void);
	typedef void(*vFp_i)(int);
	typedef int(*iFp_v)(void);
	typedef void(*vFp_l)(long);
	typedef long(*lFp_v)(void);
	typedef void(*vFp_f)(float);
	typedef float(*fFp_v)(void);
	typedef void(*vFp_lf)(double);
	typedef double(*lfFp_v)(void);

	typedef void(*vFp_b)(bool);
	typedef bool(*bFp_v)(void);
	typedef void(*vFp_p)(void*);
	typedef void*(*pFp_v)(void);

	typedef void(*vFp_u8)(uint8);
	typedef void(*vFp_s8)(sint8);
	typedef uint8(*u8Fp_v)(void);
	typedef sint8(*s8Fp_v)(void);
	typedef void(*vFp_u16)(uint16);
	typedef void(*vFp_s16)(sint16);
	typedef uint16(*u16Fp_v)(void);
	typedef sint16(*s16Fp_v)(void);
	typedef void(*vFp_u32)(uint32);
	typedef void(*vFp_s32)(sint32);
	typedef uint32(*u32Fp_v)(void);
	typedef sint32(*s32Fp_v)(void);
	typedef void(*vFp_u64)(uint64);
	typedef void(*vFp_s64)(sint64);
	typedef uint64(*u64Fp_v)(void);
	typedef sint64(*s64Fp_v)(void);
	typedef void(*vFp_f32)(float32);
	typedef float32(*f32Fp_v)(void);
	typedef void(*vFp_f64)(float64);
	typedef float64(*f64Fp_v)(void);
}
#
#endif
