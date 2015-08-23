#ifndef XCBASE_EXCEPTIONS_INC
#define XCBASE_EXCEPTIONS_INC 100
#
#include <exception>
namespace xc {
	struct exception { 
		typedef unsigned int code_type;
	private:
		code_type Code;
	public:
		code_type code()const { return Code; }
	public:
		exception() :Code(0){}
		exception(code_type Code_) :Code(Code_) {}
	};

}
#define HMR_NOEXCEPT

#ifndef  HMR_NOEXCEPT
#	define xc_throw(except) throw except
#	define xc_assert(condition,except) if(!(condition))xc_throw(except)
#else
#	define xc_throw(except)
#	define xc_assert(condition,except)
#endif
#
#endif
