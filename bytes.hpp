#ifndef XCBASE_BYTES_INC
#define XCBASE_BYTES_INC 100
#
#include"allocator.hpp"
#include"type.hpp"
#include"rvalue_reference.hpp"
#include"function.hpp"
namespace xc{
	struct bytes{
		typedef bytes this_type;
		typedef xc::rvalue_reference<this_type> this_rvalue_reference;
		typedef unsigned char* pointer;
		typedef unsigned char& reference;
		typedef unsigned int size_type;
		typedef xc::function<void(void*, size_type)> deleter;
		typedef unsigned char* iterator;
		typedef const unsigned char* const_iterator;
	private:
		pointer Ptr;
		size_type Size;
		deleter Dlt;
	public:
		bytes()
			: Ptr(0)
			, Size(0)
			, Dlt(){
		}
		template<typename alloc = default_allocator>
		explicit bytes(size_type Size_, alloc Alloc = alloc())
			: Ptr(0)
			, Size(0)
			, Dlt(){
			assign(Size_, Alloc);
		}
		template<typename deleter_>
		bytes(pointer Ptr_, size_type Size_, deleter_  Dlt_)
			: Ptr(0)
			, Size(0)
			, Dlt(){
			assign(Ptr_, Size_, Dlt_);
		}
		bytes(this_rvalue_reference rref_)
			: Ptr(0)
			, Size(0)
			, Dlt(){
			swap(rref_.ref);
		}
		this_type& operator=(this_rvalue_reference rref_){
			if(this != &(rref_.ref)){
				clear();
				swap(rref_.ref);
			}
			return *this;
		}
		~bytes(){ clear();}
	private://コピーは禁止
		bytes(const this_type& my_);
		this_type& operator=(const this_type& my_);
	public:
		//配列を解放
		void clear(){
			if(Ptr==0)return;

			if(Dlt){
				Dlt(Ptr, Size);
				Dlt.clear();
				Ptr = 0;
				Size = 0;
			}
		}
		//配列を解放し、新しく配列を保持
		template<typename deleter_>
		void assign(pointer Ptr_, size_type Size_, deleter_ Dlt_){
			clear();

			if(Ptr_ == 0 || Size_ == 0)return;

			Ptr = Ptr_;
			Size = Size_;
			Dlt = Dlt_;
		}
		//配列を開放し、新しく配列を保持
		template<typename alloc = default_allocator>
		void assign(size_type Size_, alloc Alloc=alloc()){
			clear();

			typedef typename alloc::template rebind<unsigned char>::other allocator;
			struct deleter{
				void operator()(void* ptr, bytes::size_type size){
					allocator().deallocate(static_cast<unsigned char*>(ptr), size);
				}
			};
			static deleter Deleter;

			Ptr = static_cast<unsigned char*>(allocator().allocate(Size_));
			Size = Size_;
			Dlt = xc::ref(Deleter);
		}
		//配列同士を交換
		void swap(this_type& my_){
			if(&my_ == this)return;

			pointer tmpPtr = Ptr;
			size_type tmpSize = Size;
			Ptr = my_.Ptr;
			Size = my_.Size;
			my_.Ptr = tmpPtr;
			my_.Size = tmpSize;

			Dlt.swap(my_.Dlt);
		}
		//deleterを取得
		deleter get_deleter() { return Dlt; }
		//配列は一切処理せず解放
		pointer release() {
			pointer AnsPtr=Ptr;
			Ptr=0;
			Size=0;
			Dlt.clear();
			return AnsPtr;
		}
	public:
		//配列を管理しているか
		operator bool()const{ return Ptr!=0; }
		//配列を管理しているか
		bool empty()const{ return Ptr == 0; }
		//配列先頭アドレス取得
		pointer get(){return Ptr;}
		//配列先頭アドレス取得(cosnt)
		const pointer get()const{return Ptr;}
		//配列へアクセス
		reference operator[](size_type pos_){return Ptr[pos_];}
		//配列へアクセス(const)
		const reference operator[](size_type pos_)const{return Ptr[pos_];}
		//配列へアクセス
		reference at(size_type pos_){ return Ptr[pos_]; }
		//配列へアクセス(const)
		const reference at(size_type pos_)const{ return Ptr[pos_]; }
		//サイズを取得
		size_type size()const{return Size;}
		//配列にデータを詰める
		void fill(unsigned char data_){
			for(iterator itr = begin(); itr != end(); ++itr){
				*itr = data_;
			}
		}
	public:
		//iterator取得関数
		iterator begin(){ return get(); }
		iterator end(){ return get() + size(); }
		//const_iterator取得関数
		const_iterator begin()const{ return get(); }
		const_iterator end()const{ return get() + size(); }
	};
}
#
#endif
