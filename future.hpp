#ifndef XC_FUTURE_INC
#define XC_FUTURE_INC 100
#
#include"exceptions.hpp"
#include"function.hpp"
#include"rvalue_reference.hpp"
#include"allocator.hpp"
namespace xc{
	struct invalid_future_exception :public exception{
		invalid_future_exception() :exception(){}
		invalid_future_exception(code_type Clde_) :exception(Clde_){}
	};

	template<typename T, typename Alloc>
	class future;
	template<typename T,typename Alloc = default_allocator>
	class promise{
		friend class future<T, Alloc>;
		typedef promise<T, Alloc> my_type;
	public:
		typedef function<void(T), Alloc> func_type;
		typedef future<T, Alloc> my_future;
	private:
		T Val;
		bool IsValid;
		bool IsWritten;
		func_type InformFunc;
	public:
		promise() :IsValid(false), IsWritten(true){}
	private:
		promise(const my_type&);
		const my_type& operator=(const my_type&);
	private:
		bool can_read()const{return IsWritten;}
		const T& read()const{ return Val; }
		void end_read(){
			IsValid = false;
			InformFunc.clear();
		}
		void end_write(){
			if(IsWritten)return;
			IsWritten = true;
			if(IsValid == false)return;

			if(InformFunc){
				func_type tmpFunc;
				tmpFunc.swap(InformFunc);
				end_read();
				tmpFunc(Val);
			}
		}
		void then(const func_type& InformFunc_){
			if (!IsValid)return;

			InformFunc = InformFunc_;

			//書き込み済みなら、InformFuncを実行
			//	end_writeが割り込みで実行された場合があるので、
			//	IsValidももう一度調べる必要がある。
			if (IsWritten && IsValid){
				func_type tmpFunc;
				tmpFunc.swap(InformFunc);
				end_read();
				tmpFunc(Val);
			}
		}
	public:
		bool valid(){ return !IsWritten; }
		void set_value(const T& val){ 
			Val = val; 
			end_write();
		}
		void set_value(xc::rvalue_reference<T> rref){
			Val = rref;
			end_write();
		}
		bool can_get_future()const{ return IsWritten && !IsValid; }
		my_future get_future(){
			IsValid = true;
			IsWritten = false;
			return my_future(*this);
		}
	};
	template<typename T, typename Alloc = default_allocator>
	class future{
		typedef function<void(T), Alloc> func_type;
		typedef promise<T, Alloc> my_promise;
	public:
		struct canceler{
		private:
			my_promise* Ptr;
		public:
			canceler() :Ptr(0){}
			canceler(future& Future_)
				:Ptr(Future_.Ptr){
			}
			operator bool()const{ return Ptr != 0; }
			void operator()(void){
				if (Ptr == 0)return;
				Ptr->end_read();
				Ptr = 0;
			}
			void clear(){ Ptr = 0; }
		};
	private:
		my_promise* Ptr;
	public:
		future() :Ptr(0){}
		future(my_promise& Ref_) :Ptr(&Ref_){}
		bool valid()const{ return Ptr!=0; }
		bool can_get()const{ 
			xc_assert(valid(), invalid_future_exception(100));
			return Ptr->can_read(); 
		}
		T get(){
			xc_assert(valid(), invalid_future_exception(101));
			T Val = Ptr->read();
			Ptr->end_read();
			Ptr = 0;
			return xc::move(Val);
		}
		void cancel(){
			if(!valid())return;
			Ptr->end_read();
			Ptr = 0;
		}
		canceler then(const func_type& Function_){
			if (!valid())return canceler();

			Ptr->then(Function_);
			canceler Canceler(*this);
			Ptr = 0;
			return Canceler;
		}
	};

	//voidへの特殊化
	template<typename Alloc>
	class promise<void, Alloc>{
		typedef promise<void, Alloc> my_type;
		typedef function<void(void), Alloc> func_type;
		typedef future<void, Alloc> my_future;
		friend class future<void, Alloc>;
	private:
		bool IsValid;
		bool IsWritten;
		func_type InformFunc;
	public:
		promise() :IsValid(false), IsWritten(true){}
	private:
		promise(const my_type&);
		const my_type& operator=(const my_type&);
	private:
		bool can_read()const{ return IsWritten; }
		void end_read(){
			IsValid = false;
			InformFunc.clear();
		}
		void end_write(){
			if(IsWritten)return;
			IsWritten = true;
			if(IsValid == false)return;

			if(InformFunc){
				func_type tmpFunc;
				tmpFunc.swap(InformFunc);
				end_read();
				tmpFunc();
			}
		}
		void then(const func_type& InformFunc_){
			if(!IsValid)return;

			InformFunc = InformFunc_;

			//書き込み済みなら、InformFuncを実行
			//	end_writeが割り込みで実行された場合があるので、
			//	IsValidももう一度調べる必要がある。
			if(IsWritten && IsValid){
				func_type tmpFunc;
				tmpFunc.swap(InformFunc);
				end_read();
				tmpFunc();
			}
		}
	public:
		bool valid(){ return !IsWritten; }
		void set_value(){
			end_write();
		}
		bool can_get_future()const{ return IsWritten && !IsValid; }
		my_future get_future(){
			IsValid = true;
			IsWritten = false;
			return my_future(*this);
		}
	};
	template<typename Alloc>
	class future<void, Alloc>{
		typedef function<void(void), Alloc> func_type;
		typedef promise<void, Alloc> my_promise;
	public:
		struct canceler{
		private:
			my_promise* Ptr;
		public:
			canceler() :Ptr(0){}
			canceler(future& Future_)
				:Ptr(Future_.Ptr){
			}
			operator bool()const{ return Ptr != 0; }
			void operator()(void){
				if(Ptr == 0)return;
				Ptr->end_read();
				Ptr = 0;
			}
			void clear(){ Ptr = 0; }
		};
	private:
		my_promise* Ptr;
	public:
		future() :Ptr(0){}
		future(my_promise& Ref_) :Ptr(&Ref_){}
		bool valid()const{ return Ptr != 0; }
		bool can_get()const{
			xc_assert(valid(), invalid_future_exception(100));
			return Ptr->can_read();
		}
		void get(){
			xc_assert(valid(), invalid_future_exception(101));
			Ptr->end_read();
			Ptr = 0;
		}
		void cancel(){
			xc_assert(valid(), invalid_future_exception(102));
			Ptr->end_read();
			Ptr = 0;
		}
		canceler then(const func_type& Function_){
			if(!valid())return canceler();

			Ptr->then(Function_);
			canceler Canceler(*this);
			Ptr = 0;
			return Canceler;
		}
	};
}
#
#endif
