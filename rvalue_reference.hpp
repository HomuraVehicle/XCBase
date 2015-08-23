#ifndef XCBASE_RVALUEREFERENCE_INC
#define XCBASE_RVALUEREFERENCE_INC 100
#
namespace xc{
	template<typename T>
	struct rvalue_reference {
		T& ref;
		explicit rvalue_reference(T& ref_) :ref(ref_) {}
		operator T&(){ return ref; }
	};
	template<typename T>
	inline rvalue_reference<T> move(T& Ptr_) { return rvalue_reference<T>(Ptr_); }
}
#
#endif
