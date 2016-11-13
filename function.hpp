#ifndef XCBASE_FUNCTION_INC
#define XCBASE_FUNCTION_INC 100
#
#include<memory>
#include"exceptions.hpp"
#include"type_traits.hpp"
#include"allocator.hpp"
#include"rvalue_reference.hpp"
using namespace std;
namespace xc{
	struct no_function_invoked_exception :public exception{
		no_function_invoked_exception() :exception(){}
		no_function_invoked_exception(code_type Clde_) :exception(Clde_){}
	};

	template<typename T>
	struct ref_wrapper{
		T& Ref;
		ref_wrapper(T& Ref_) :Ref(Ref_){}
	};
	template<typename T>
	ref_wrapper<T> ref(T& Ref_){ return ref_wrapper<T>(Ref_); }

	namespace functions{
		namespace detail{
			template<typename R, typename Alloc=default_allocator>
			class function0{
				typedef function0<R, Alloc> my_type;
			private:
				typedef R(*func_invoke_type)(void*);
				typedef void*(*func_clone_type)(void*);
				typedef void(*func_release_type)(void*);
				template<typename func>
				struct function_manager{
					typedef typename Alloc::template rebind<func>::other allocator;
					static R invoke(void* Ptr){
						R val = reinterpret_cast<func*>(Ptr)->operator()();
						return xc::move(val);
					}
					static void* clone(void* Ptr){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, *reinterpret_cast<func*>(Ptr));
						return static_cast<void*>(fPtr);
					}
					static void* create(const func& Func_){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, Func_);
						return static_cast<void*>(fPtr);
					}
					static void release(void* Ptr){
						allocator Allocator;
						Allocator.destroy(reinterpret_cast<func*>(Ptr));
						Allocator.deallocate(reinterpret_cast<func*>(Ptr), 1);
						Ptr = 0;
					}
				};
				template<typename func>
				struct function_manager < ref_wrapper<func> > {
					static R invoke(void* Ptr){
						R val = reinterpret_cast<func*>(Ptr)->operator()();
						return xc::move(val);
					}
					static void* clone(void* Ptr){
						return reinterpret_cast<void*>(Ptr);
					}
					static void* create(const ref_wrapper<func>& Ref_){
						return static_cast<void*>(&(Ref_.Ref));
					}
					static void release(void* Ptr){
						Ptr = 0;
					}
				};
			private:
				void* Ptr;
				func_invoke_type FuncInvoke;
				func_clone_type FuncClone;
				func_release_type FuncRelease;
				template<typename func>
				void set_function(const func& Func_){
					clear_function();
					Ptr = function_manager<func>::create(Func_);
					FuncInvoke = function_manager<func>::invoke;
					FuncClone = function_manager<func>::clone;
					FuncRelease = function_manager<func>::release;
				}
				void clear_function(){
					if (Ptr == 0)return;
					FuncRelease(Ptr);
					FuncInvoke = 0;
					FuncClone = 0;
					FuncRelease = 0;
				}
			public:
				function0()
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
				}
				function0(const my_type& My_)
					: Ptr(0)
					, FuncInvoke(My_.FuncInvoke)
					, FuncClone(My_.FuncClone)
					, FuncRelease(My_.FuncRelease){
					if(My_.Ptr != 0){
						Ptr = FuncClone(My_.Ptr);
					}
				}
				const my_type& operator=(const my_type& My_){
					if (this != &My_){
						clear_function();
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;
						if(My_.Ptr != 0){
							Ptr = FuncClone(My_.Ptr);
						}
					}
					return *this;
				}
				~function0(){ clear_function(); }
				template<typename func >
				function0(const func& Func)
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
					set_function(Func);
				}
				template<typename func>
				const my_type& operator=(const func& Func){
					set_function(Func);
					return *this;
				}
				void clear(){ clear_function(); }
				void swap(const my_type& My_){
					if (this != &My_){
						void* tmpPtr = Ptr;
						func_invoke_type tmpFuncInvoke = FuncInvoke;
						func_clone_type tmpFuncClone = FuncClone;
						func_release_type tmpFuncRelease = FuncRelease;

						Ptr = My_.Ptr;
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;

						My_.Ptr = tmpPtr;
						My_.FuncInvoke = tmpFuncInvoke;
						My_.FuncClone = tmpFuncClone;
						My_.FuncRelease = tmpFuncRelease;
					}
				}
			public:
				operator bool()const{ return Ptr != 0; }
				R operator()() const{
					xc_assert(*this != false, no_function_invoked_exception(100));
					return FuncInvoke(Ptr);
				}
			};
			template<typename Alloc>
			class function0<void, Alloc>{
				typedef function0<void, Alloc> my_type;
			private:
				typedef void(*func_invoke_type)(void*);
				typedef void*(*func_clone_type)(void*);
				typedef void(*func_release_type)(void*);
				template<typename func>
				struct function_manager{
					typedef typename Alloc::template rebind<func>::other allocator;
					static void invoke(void* Ptr){
						reinterpret_cast<func*>(Ptr)->operator()();
					}
					static void* clone(void* Ptr){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, *reinterpret_cast<func*>(Ptr));
						return static_cast<void*>(fPtr);
					}
					static void* create(const func& Func_){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, Func_);
						return static_cast<void*>(fPtr);
					}
					static void release(void* Ptr){
						allocator Allocator;
						Allocator.destroy(reinterpret_cast<func*>(Ptr));
						Allocator.deallocate(reinterpret_cast<func*>(Ptr), 1);
						Ptr = 0;
					}
				};
				template<typename func>
				struct function_manager < ref_wrapper<func> > {
					static void invoke(void* Ptr){
						reinterpret_cast<func*>(Ptr)->operator()();
					}
					static void* clone(void* Ptr){
						return reinterpret_cast<void*>(Ptr);
					}
					static void* create(const ref_wrapper<func>& Ref_){
						return static_cast<void*>(&(Ref_.Ref));
					}
					static void release(void* Ptr){
						Ptr = 0;
					}
				};
			private:
				void* Ptr;
				func_invoke_type FuncInvoke;
				func_clone_type FuncClone;
				func_release_type FuncRelease;
				template<typename func>
				void set_function(const func& Func_){
					clear_function();
					Ptr = function_manager<func>::create(Func_);
					FuncInvoke = function_manager<func>::invoke;
					FuncClone = function_manager<func>::clone;
					FuncRelease = function_manager<func>::release;
				}
				void clear_function(){
					if(Ptr == 0)return;
					FuncRelease(Ptr);
					FuncInvoke = 0;
					FuncClone = 0;
					FuncRelease = 0;
				}
			public:
				function0()
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
				}
				function0(const my_type& My_)
					: Ptr(0)
					, FuncInvoke(My_.FuncInvoke)
					, FuncClone(My_.FuncClone)
					, FuncRelease(My_.FuncRelease){
					if(My_.Ptr != 0){
						Ptr = FuncClone(My_.Ptr);
					}
				}
				const my_type& operator=(const my_type& My_){
					if(this != &My_){
						clear_function();
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;
						if(My_.Ptr != 0){
							Ptr = FuncClone(My_.Ptr);
						}
					}
					return *this;
				}
				~function0(){ clear_function(); }
				template<typename func >
				function0(const func& Func)
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
					set_function(Func);
				}
				template<typename func>
				const my_type& operator=(const func& Func){
					set_function(Func);
					return *this;
				}
				void clear(){ clear_function(); }
				void swap(const my_type& My_){
					if(this != &My_){
						void* tmpPtr = Ptr;
						func_invoke_type tmpFuncInvoke = FuncInvoke;
						func_clone_type tmpFuncClone = FuncClone;
						func_release_type tmpFuncRelease = FuncRelease;

						Ptr = My_.Ptr;
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;

						My_.Ptr = tmpPtr;
						My_.FuncInvoke = tmpFuncInvoke;
						My_.FuncClone = tmpFuncClone;
						My_.FuncRelease = tmpFuncRelease;
					}
				}
			public:
				operator bool()const{ return Ptr != 0; }
				void operator()() const{
					xc_assert(*this != false, no_function_invoked_exception(100));
					FuncInvoke(Ptr);
				}
			};

			template<typename R, typename T0, typename Alloc>
			class function1{
				typedef function1<R, T0, Alloc> my_type;
			private:
				typedef R(*func_invoke_type)(void*, T0);
				typedef void*(*func_clone_type)(void*);
				typedef void(*func_release_type)(void*);
				template<typename func>
				struct function_manager{
					typedef typename Alloc::template rebind<func>::other allocator;
					static R invoke(void* Ptr, T0 Arg){
						R val = reinterpret_cast<func*>(Ptr)->operator()(Arg);
						return xc::move(val);
					}
					static void* clone(void* Ptr){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, *reinterpret_cast<func*>(Ptr));
						return static_cast<void*>(fPtr);
					}
					static void* create(const func& Func_){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, Func_);
						return static_cast<void*>(fPtr);
					}
					static void release(void* Ptr){
						allocator Allocator;
						Allocator.destroy(reinterpret_cast<func*>(Ptr));
						Allocator.deallocate(reinterpret_cast<func*>(Ptr), 1);
					}
				};
				template<typename func>
				struct function_manager < ref_wrapper<func> > {
					static R invoke(void* Ptr, T0 Arg){
						R val = reinterpret_cast<func*>(Ptr)->operator()(Arg);
						return xc::move(val);
					}
					static void* clone(void* Ptr){
						return reinterpret_cast<void*>(Ptr);
					}
					static void* create(const ref_wrapper<func>& Ref_){
						return static_cast<void*>(&(Ref_.Ref));
					}
					static void release(void* Ptr){}
				};
			private:
				void* Ptr;
				func_invoke_type FuncInvoke;
				func_clone_type FuncClone;
				func_release_type FuncRelease;
				template<typename func>
				void set_function(const func& Func_){
					clear_function();
					Ptr = function_manager<func>::create(Func_);
					FuncInvoke = function_manager<func>::invoke;
					FuncClone = function_manager<func>::clone;
					FuncRelease = function_manager<func>::release;
				}
				void clear_function(){
					if (Ptr == 0)return;
					FuncRelease(Ptr);
					FuncInvoke = 0;
					FuncClone = 0;
					FuncRelease = 0;
					Ptr = 0;
				}
			public:
				function1()
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
				}
				function1(const my_type& My_)
					: Ptr(0)
					, FuncInvoke(My_.FuncInvoke)
					, FuncClone(My_.FuncClone)
					, FuncRelease(My_.FuncRelease){
					if(My_.Ptr != 0){
						Ptr = FuncClone(My_.Ptr);
					}
				}
				const my_type& operator=(const my_type& My_){
					if (this != &My_){
						clear_function();
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;
						if(My_.Ptr != 0){
							Ptr = FuncClone(My_.Ptr);
						}
					}
					return *this;
				}
				~function1(){ clear_function(); }
				template<typename func>
				function1(const func& Func)
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
					set_function(Func);
				}
				template<typename func>
				const my_type& operator=(const func& Func){
					set_function(Func);
					return *this;
				}
				void clear(){ clear_function(); }
				void swap(my_type& My_){
					if (this != &My_){
						void* tmpPtr = Ptr;
						func_invoke_type tmpFuncInvoke = FuncInvoke;
						func_clone_type tmpFuncClone = FuncClone;
						func_release_type tmpFuncRelease = FuncRelease;

						Ptr = My_.Ptr;
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;

						My_.Ptr = tmpPtr;
						My_.FuncInvoke = tmpFuncInvoke;
						My_.FuncClone = tmpFuncClone;
						My_.FuncRelease = tmpFuncRelease;
					}
				}
			public:
				operator bool()const{ return Ptr != 0; }
				R operator()(T0 Arg) const{
					xc_assert(*this != false, no_function_invoked_exception(101));
					return xc::move(FuncInvoke(Ptr, Arg));
				}
			};

			template<typename T0, typename Alloc>
			class function1<void, T0, Alloc>{
				typedef function1<void, T0, Alloc> my_type;
			private:
				typedef void(*func_invoke_type)(void*, T0);
				typedef void*(*func_clone_type)(void*);
				typedef void(*func_release_type)(void*);
				template<typename func>
				struct function_manager{
					typedef typename Alloc::template rebind<func>::other allocator;
					static void invoke(void* Ptr, T0 Arg){
						reinterpret_cast<func*>(Ptr)->operator()(Arg);
					}
					static void* clone(void* Ptr){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, *reinterpret_cast<func*>(Ptr));
						return static_cast<void*>(fPtr);
					}
					static void* create(const func& Func_){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, Func_);
						return static_cast<void*>(fPtr);
					}
					static void release(void* Ptr){
						allocator Allocator;
						Allocator.destroy(reinterpret_cast<func*>(Ptr));
						Allocator.deallocate(reinterpret_cast<func*>(Ptr), 1);
					}
				};
				template<typename func>
				struct function_manager < ref_wrapper<func> > {
					static void invoke(void* Ptr, T0 Arg1){
						reinterpret_cast<func*>(Ptr)->operator()(Arg1);
					}
					static void* clone(void* Ptr){
						return reinterpret_cast<void*>(Ptr);
					}
					static void* create(const ref_wrapper<func>& Ref_){
						return static_cast<void*>(&(Ref_.Ref));
					}
					static void release(void* Ptr){}
				};
			private:
				void* Ptr;
				func_invoke_type FuncInvoke;
				func_clone_type FuncClone;
				func_release_type FuncRelease;
				template<typename func>
				void set_function(const func& Func_){
					clear_function();
					Ptr = function_manager<func>::create(Func_);
					FuncInvoke = function_manager<func>::invoke;
					FuncClone = function_manager<func>::clone;
					FuncRelease = function_manager<func>::release;
				}
				void clear_function(){
					if(Ptr == 0)return;
					FuncRelease(Ptr);
					FuncInvoke = 0;
					FuncClone = 0;
					FuncRelease = 0;
					Ptr = 0;
				}
			public:
				function1()
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
				}
				function1(const my_type& My_)
					: Ptr(0)
					, FuncInvoke(My_.FuncInvoke)
					, FuncClone(My_.FuncClone)
					, FuncRelease(My_.FuncRelease){
					if(My_.Ptr != 0){
						Ptr = FuncClone(My_.Ptr);
					}
				}
				const my_type& operator=(const my_type& My_){
					if(this != &My_){
						clear_function();
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;
						if(My_.Ptr != 0){
							Ptr = FuncClone(My_.Ptr);
						}
					}
					return *this;
				}
				~function1(){ clear_function(); }
				template<typename func>
				function1(const func& Func)
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
					set_function(Func);
				}
				template<typename func>
				const my_type& operator=(const func& Func){
					set_function(Func);
					return *this;
				}
				void clear(){ clear_function(); }
				void swap(my_type& My_){
					if(this != &My_){
						void* tmpPtr = Ptr;
						func_invoke_type tmpFuncInvoke = FuncInvoke;
						func_clone_type tmpFuncClone = FuncClone;
						func_release_type tmpFuncRelease = FuncRelease;

						Ptr = My_.Ptr;
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;

						My_.Ptr = tmpPtr;
						My_.FuncInvoke = tmpFuncInvoke;
						My_.FuncClone = tmpFuncClone;
						My_.FuncRelease = tmpFuncRelease;
					}
				}
			public:
				operator bool()const{ return Ptr != 0; }
				void operator()(T0 Arg) const{
					xc_assert(*this != false, no_function_invoked_exception(101));
					FuncInvoke(Ptr, Arg);
				}
			};

			template<typename R, typename T0, typename T1, typename Alloc>
			class function2{
				typedef function2<R, T0, T1, Alloc> my_type;
			private:
				typedef R(*func_invoke_type)(void*, T0, T1);
				typedef void*(*func_clone_type)(void*);
				typedef void(*func_release_type)(void*);
				template<typename func>
				struct function_manager{
					typedef typename Alloc::template rebind<func>::other allocator;
					static R invoke(void* Ptr, T0 Arg0, T1 Arg1){
						R val = reinterpret_cast<func*>(Ptr)->operator()(Arg0,Arg1);
						return xc::move(val);
					}
					static void* clone(void* Ptr){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, *reinterpret_cast<func*>(Ptr));
						return static_cast<void*>(fPtr);
					}
					static void* create(const func& Func_){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, Func_);
						return static_cast<void*>(fPtr);
					}
					static void release(void* Ptr){
						allocator Allocator;
						Allocator.destroy(reinterpret_cast<func*>(Ptr));
						Allocator.deallocate(reinterpret_cast<func*>(Ptr), 1);
					}
				};
				template<typename func>
				struct function_manager < ref_wrapper<func> > {
					static R invoke(void* Ptr, T0 Arg0, T1 Arg1){
						R val = reinterpret_cast<func*>(Ptr)->operator()(Arg0, Arg1);
						return xc::move(val);
					}
					static void* clone(void* Ptr){
						return reinterpret_cast<void*>(Ptr);
					}
					static void* create(const ref_wrapper<func>& Ref_){
						return static_cast<void*>(&(Ref_.Ref));
					}
					static void release(void* Ptr){}
				};
			private:
				void* Ptr;
				func_invoke_type FuncInvoke;
				func_clone_type FuncClone;
				func_release_type FuncRelease;
				template<typename func>
				void set_function(const func& Func_){
					clear_function();
					Ptr = function_manager<func>::create(Func_);
					FuncInvoke = function_manager<func>::invoke;
					FuncClone = function_manager<func>::clone;
					FuncRelease = function_manager<func>::release;
				}
				void clear_function(){
					if (Ptr == 0)return;
					FuncRelease(Ptr);
					FuncInvoke = 0;
					FuncClone = 0;
					FuncRelease = 0;
					Ptr = 0;
				}
			public:
				function2()
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
				}
				function2(const my_type& My_)
					: Ptr(0)
					, FuncInvoke(My_.FuncInvoke)
					, FuncClone(My_.FuncClone)
					, FuncRelease(My_.FuncRelease){
					if(My_.Ptr != 0){
						Ptr = FuncClone(My_.Ptr);
					}
				}
				const my_type& operator=(const my_type& My_){
					if(this != &My_){
						clear_function();
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;
						if(My_.Ptr != 0){
							Ptr = FuncClone(My_.Ptr);
						}
					}
					return *this;
				}
				~function2(){ 
					clear_function();
				}
				template<typename func>
				function2(const func& Func)
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
					set_function(Func);
				}
				template<typename func>
				const my_type& operator=(const func& Func){
					set_function(Func);
					return *this;
				}
				void clear(){ clear_function(); }
				void swap(const my_type& My_){
					if (this != &My_){
						void* tmpPtr = Ptr;
						func_invoke_type tmpFuncInvoke = FuncInvoke;
						func_clone_type tmpFuncClone = FuncClone;
						func_release_type tmpFuncRelease = FuncRelease;

						Ptr = My_.Ptr;
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;

						My_.Ptr = tmpPtr;
						My_.FuncInvoke = tmpFuncInvoke;
						My_.FuncClone = tmpFuncClone;
						My_.FuncRelease = tmpFuncRelease;
					}
				}
			public:
				operator bool()const{ return Ptr != 0; }
				R operator()(T0 Arg0, T1 Arg1) const{
					xc_assert(*this != false, no_function_invoked_exception(102));
					return xc::move(FuncInvoke(Ptr, Arg0, Arg1));
				}
			};

			template<typename T0, typename T1, typename Alloc>
			class function2<void, T0, T1, Alloc>{
				typedef function2<void, T0, T1, Alloc> my_type;
			private:
				typedef void(*func_invoke_type)(void*, T0, T1);
				typedef void*(*func_clone_type)(void*);
				typedef void(*func_release_type)(void*);
				template<typename func>
				struct function_manager{
					typedef typename Alloc::template rebind<func>::other allocator;
					static void invoke(void* Ptr, T0 Arg0, T1 Arg1){
						reinterpret_cast<func*>(Ptr)->operator()(Arg0, Arg1);
					}
					static void* clone(void* Ptr){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, *reinterpret_cast<func*>(Ptr));
						return static_cast<void*>(fPtr);
					}
					static void* create(const func& Func_){
						allocator Allocator;
						func* fPtr = Allocator.allocate(1);
						Allocator.construct(fPtr, Func_);
						return static_cast<void*>(fPtr);
					}
					static void release(void* Ptr){
						allocator Allocator;
						Allocator.destroy(reinterpret_cast<func*>(Ptr));
						Allocator.deallocate(reinterpret_cast<func*>(Ptr), 1);
					}
				};
				template<typename func>
				struct function_manager < ref_wrapper<func> > {
					static void invoke(void* Ptr, T0 Arg0, T1 Arg1){
						reinterpret_cast<func*>(Ptr)->operator()(Arg0, Arg1);
					}
					static void* clone(void* Ptr){
						return reinterpret_cast<void*>(Ptr);
					}
					static void* create(const ref_wrapper<func>& Ref_){
						return static_cast<void*>(&(Ref_.Ref));
					}
					static void release(void* Ptr){}
				};
			private:
				void* Ptr;
				func_invoke_type FuncInvoke;
				func_clone_type FuncClone;
				func_release_type FuncRelease;
				template<typename func>
				void set_function(const func& Func_){
					clear_function();
					Ptr = function_manager<func>::create(Func_);
					FuncInvoke = function_manager<func>::invoke;
					FuncClone = function_manager<func>::clone;
					FuncRelease = function_manager<func>::release;
				}
				void clear_function(){
					if(Ptr == 0)return;
					FuncRelease(Ptr);
					FuncInvoke = 0;
					FuncClone = 0;
					FuncRelease = 0;
					Ptr = 0;
				}
			public:
				function2()
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
				}
				function2(const my_type& My_)
					: Ptr(0)
					, FuncInvoke(My_.FuncInvoke)
					, FuncClone(My_.FuncClone)
					, FuncRelease(My_.FuncRelease){
					if(My_.Ptr != 0){
						Ptr = FuncClone(My_.Ptr);
					}
				}
				const my_type& operator=(const my_type& My_){
					if(this != &My_){
						clear_function();
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;
						if(My_.Ptr != 0){
							Ptr = FuncClone(My_.Ptr);
						}
					}
					return *this;
				}
				~function2(){
					clear_function();
				}
				template<typename func>
				function2(const func& Func)
					: Ptr(0)
					, FuncInvoke(0)
					, FuncClone(0)
					, FuncRelease(0){
					set_function(Func);
				}
				template<typename func>
				const my_type& operator=(const func& Func){
					set_function(Func);
					return *this;
				}
				void clear(){ clear_function(); }
				void swap(my_type& My_){
					if(this != &My_){
						void* tmpPtr = Ptr;
						func_invoke_type tmpFuncInvoke = FuncInvoke;
						func_clone_type tmpFuncClone = FuncClone;
						func_release_type tmpFuncRelease = FuncRelease;

						Ptr = My_.Ptr;
						FuncInvoke = My_.FuncInvoke;
						FuncClone = My_.FuncClone;
						FuncRelease = My_.FuncRelease;

						My_.Ptr = tmpPtr;
						My_.FuncInvoke = tmpFuncInvoke;
						My_.FuncClone = tmpFuncClone;
						My_.FuncRelease = tmpFuncRelease;
					}
				}
			public:
				operator bool()const{ return Ptr != 0; }
				void operator()(T0 Arg0, T1 Arg1) const{
					xc_assert(*this != false, no_function_invoked_exception(102));
					FuncInvoke(Ptr, Arg0, Arg1);
				}
			};
		}
	}

	template<class T, typename Alloc = default_allocator>
	class function;


	template<typename R, typename Alloc>
	class function<R(void), Alloc > : public functions::detail::function0<R, Alloc >{
		typedef function<R(void), Alloc > my_type;
		typedef functions::detail::function0<R, Alloc> base_type;
	public:
		function():base_type(){}
		function(const my_type& My_) :base_type(static_cast<const base_type&>(My_)){}
		const my_type& operator=(const my_type& My_){
			base_type::operator=(static_cast<const base_type&>(My_));
			return *this;
		}
		template<typename func>
		function(const func& Func) :base_type(Func){}
		template<typename func >
		const my_type& operator=(const func& Func){
//			base_type::operator=<func>(Func);
			base_type::operator=(Func);
			return *this;
		} 
	};


	template<typename R, typename T0, typename Alloc>
	class function<R(T0), Alloc > : public functions::detail::function1<R, T0, Alloc >{
		typedef function<R(T0), Alloc > my_type;
		typedef functions::detail::function1<R, T0, Alloc > base_type;
	public:
		function() :base_type(){}
		function(const my_type& My_) :base_type(static_cast<const base_type&>(My_)){}
		const my_type& operator=(const my_type& My_){
			base_type::operator=(static_cast<const base_type&>(My_));
			return *this;
		}
		template<typename func>
		function(const func& Func) : base_type(Func){}
		template<typename func>
		const my_type& operator=(const func& Func){
			base_type::operator=(Func);
			return *this;
		} 
	};


	template<typename R, typename T0, typename T1, typename Alloc>
	class function<R(T0, T1), Alloc > : public functions::detail::function2<R, T0, T1, Alloc>{
		typedef function<R(T0, T1), Alloc > my_type;
		typedef functions::detail::function2<R, T0, T1, Alloc > base_type;
	public:
		function() :base_type(){}
		function(const my_type& My_) :base_type(static_cast<const base_type&>(My_)){}
		const my_type& operator=(const my_type& My_){
			base_type::operator=(static_cast<const base_type&>(My_));
			return *this;
		}
		template<typename func>
		function(const func& Func) : base_type(Func){}
		template<typename func>
		const my_type& operator=(const func& Func){
			base_type::operator=(Func);
			return *this;
		}
	};
}
#endif
