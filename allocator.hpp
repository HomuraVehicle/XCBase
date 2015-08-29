#ifndef XCBASE_ALLOCATOR_INC
#define XCBASE_ALLOCATOR_INC 100
#
#include<xc_config.hpp>
#ifdef XC_LOCAL_ALLOCATOR
#include<local_allocator.hpp>
namespace xc{
	typedef local_allocator default_allocator;
	template<typename T>
	struct default_deleter {
	private:
		typedef typename default_allocator::template rebind<T>::other allocator;
		allocator Alloc;
	public:
		void operator()(T* ptr) {
			if (ptr == 0)return;
			Alloc.destroy(ptr);
			Alloc.deallocate(ptr, 1);
		}
	};
	template<typename T>
	struct default_deleter<T[]> {
		void operator()(T* ptr) {
			if(ptr)delete[] ptr;
		}
	};
}
#else
#include<memory>
namespace xc{
	typedef std::allocator<void> default_allocator;
}
#endif
#
#endif
