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

	//futureへのpromiseからのインターフェース
	template<typename T, typename Alloc = default_allocator>
	class promise_base{
		typedef function<void(T), Alloc> func_type;
	public:
		virtual ~promise_base(){}
		virtual bool can_read() = 0;
		virtual const T& read() = 0;
		virtual void end_read() = 0;
		virtual void then(const func_type&) = 0;
	};
	template<typename Alloc>
	class promise_base<void, Alloc>{
		typedef function<void(T), Alloc> func_type;
	public:
		virtual ~promise_base(){}
		virtual bool can_read() = 0;
		virtual void read() = 0;
		virtual void end_read() = 0;
		virtual void then(const func_type&) = 0;
	};

	//future: 将来取得できる値へのアクセッサ
	//	promise_baseインターフェースを受け取って生成
	template<typename T, typename Alloc = default_allocator>
	class future{
		typedef function<void(T), Alloc> func_type;
		typedef promise_base<T, Alloc> my_promise;
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
	//voidへの特殊化future
	template<typename Alloc>
	class future<void, Alloc>{
		typedef function<void(void), Alloc> func_type;
		typedef promise_base<T, Alloc> my_promise;
	public:
		struct canceler{
		private:
			my_promise* Ptr;
		public:
			canceler() :Ptr(0){}
			canceler(future& Future_)
				:Ptr(Future_.Ptr){}
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

	//標準promise：内部に変数を保持する形のpromise
	//	値のセットは、promiseに対してset_value関数を利用することで行う
	template<typename T, typename Alloc = default_allocator>
	class promise{
		typedef promise<T, Alloc> my_type;
	public:
		typedef future<T, Alloc> my_future;
	private:
		struct base :public promise_base<T, Alloc>{
			typedef function<void(T), Alloc> func_type;
		private:
			T Val;
			bool IsValid;
			bool IsWritten;
			func_type InformFunc;
		public:
			base() :IsValid(false), IsWritten(true){}
		public://promise_base overload functions
			bool can_read(){ return IsWritten; }
			const T& read(){ return Val; }
			void end_read(){
				IsValid = false;
				InformFunc.clear();
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
					tmpFunc(Val);
				}
			}
		public:
			bool can_ready_future()const{ return IsWritten && !IsValid; }
			void ready_future()const{
				IsValid = true;
				IsWritten = false;
			}
			bool can_write()const{ return !IsWritten; }
			void write(const T& val){
				Val = val;
			}
			void write(xc::rvalue_reference<T> rref){
				Val = rref;
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
		};
	private:
		base Base;
	private://コピー禁止
		promise(const my_type&){}
		const my_type& operator=(const my_type&);
	public:
		bool valid(){ return Base.can_write(); }
		void set_value(const T& val){
			Base.write(val);
			Base.end_write();
		}
		void set_value(xc::rvalue_reference<T> rref){
			Base.write(rref);
			Base.end_write();
		}
		bool can_get_future()const{ return Base.can_ready_future(); }
		my_future get_future(){
			Base.ready_future();
			return my_future(Base);
		}
	};
	//voidへの特殊化標準promise
	template<typename Alloc>
	class promise<void, Alloc>{
		typedef promise<void, Alloc> my_type;
	public:
		typedef future<void, Alloc> my_future;
	private:
		struct base :public promise_base<void, Alloc>{
			typedef function<void(void), Alloc> func_type;
		private:
			bool IsValid;
			bool IsWritten;
			func_type InformFunc;
		public:
			base() :IsValid(false), IsWritten(true){}
		public://promise_base overload functions
			bool can_read(){ return IsWritten; }
			void read(){}
			void end_read(){
				IsValid = false;
				InformFunc.clear();
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
			bool can_ready_future()const{ return IsWritten && !IsValid; }
			void ready_future()const{
				IsValid = true;
				IsWritten = false;
			}
			bool can_write()const{ return !IsWritten; }
			void write(){}
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
		};
	private:
		base Base;
	private://コピー禁止
		promise(const my_type&){}
		const my_type& operator=(const my_type&);
	public:
		bool valid(){ return Base.can_write(); }
		void set_value(){
			Base.write();
			Base.end_write();
		}
		bool can_get_future()const{ return Base.can_ready_future(); }
		my_future get_future(){
			Base.ready_future();
			return my_future(Base);
		}
	};
}
#
#endif
