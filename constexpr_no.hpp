#ifndef XCBASE_CONSTEXPRNO_INC
#define XCBASE_CONSTEXPRNO_INC 100
#
namespace xc {
	template<int no_>
	struct constexpr_no {
		static const int No;
	};
	template<int no_>
	const int constexpr_no<no_>::No=no_;
}
#
#endif
