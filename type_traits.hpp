
#ifndef XCBASE_TYPETRAITS_INC
#define XCBASE_TYPETRAITS_INC 100
#
namespace xc{
	typedef char yes_type;
	typedef struct { char c[8]; } no_type;
#define  xc_yes_or_no(expr) (sizeof(expr) == sizeof(xc::yes_type));
	/*
	namespace{
		template<typename T>
		struct is_base_impl{
			template<typename U = T::base_identifier>
			static xc::yes_type check(U*, typename U::base_identifier* = NULL);
			template<typename U>
			static xc::no_type check(...);
		public:
			static bool const value = xc_yes_or_no(check<T>(0));
		};
	}
	template<typename T>
	struct is_base :public xc::bool_type<is_base_impl<T>::value>{};
	*/
	struct true_type {
		static bool value() { return true; }
		operator bool() { return true; }
	};
	struct false_type {
		static bool value() { return false; }
		operator bool() { return false; }
	};
	template<bool val>
	struct bool_type:public false_type{};
	template<>
	struct bool_type<true> :public true_type{};
}
#
#endif
