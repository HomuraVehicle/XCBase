#ifndef XCBASE_SHAREDBYTES_INC
#define XCBASE_SHAREDBYTES_INC 100
#
#include"allocator.hpp"
#include"type.hpp"
#include"rvalue_reference.hpp"
#include"function.hpp"
#include"bytes.hpp"
namespace xc{
	template<typename Alloc=default_allocator>
	struct shared_bytes{
		typedef shared_bytes this_type;
		typedef xc::rvalue_reference<this_type> this_rvalue_reference;
	public:
		typedef unsigned char* pointer;
		typedef unsigned char& reference;
		typedef unsigned int size_type;
		typedef uint16 usecnt_type;
		typedef pointer iterator;
		typedef const pointer const_iterator;
		typedef typename Alloc::template rebind<unsigned char>::other allocator;
	private:
		void allocate(pointer Ptr_, size_type Size_){
			if(Ptr_ == 0){
				unsigned char* BasePtr = allocator().allocate(sizeof(usecnt_type) + Size_);
				usecnt_type* pUseCnt = reinterpret_cast<usecnt_type*>(BasePtr);
				*pUseCnt = 1;

				Ptr = BasePtr + sizeof(usecnt_type);
				Size = Size_;
			} else{
				usecnt_type* pUseCnt = reinterpret_cast<usecnt_type*>(Ptr_ - sizeof(usecnt_type));
				++(*pUseCnt);

				Ptr = Ptr_;
				Size = Size_;
			}
		}
		struct deleter{
			void operator()(void* Ptr_, size_type Size_){
				if(Ptr_ == 0)return;

				usecnt_type* pUseCnt = reinterpret_cast<usecnt_type*>(static_cast<unsigned char*>(Ptr_)-sizeof(usecnt_type));
				if(*pUseCnt == 0)return;

				if(--(*pUseCnt) == 0){
					allocator().deallocate(reinterpret_cast<unsigned char*>(pUseCnt), Size_ + sizeof(usecnt_type));
				}
			}
		};
		static deleter Deleter;
	private:
		pointer Ptr;			//[-2:0]: UseCount、[0:Size]: データアドレス
		size_type Size;
	public:
		shared_bytes()
			: Ptr(0)
			, Size(0){
		}
		explicit shared_bytes(size_type Size_)
			: Ptr(0)
			, Size(0){
			assign(Size_);
		}
		explicit shared_bytes(this_rvalue_reference rref_)
			: Ptr(0)
			, Size(0){
			swap(rref_.ref);
		}
		const this_type& operator=(this_rvalue_reference rref_){
			if(this != &(rref_.ref)){
				clear();
				swap(rref_.ref);
			}
			return *this;
		}
		shared_bytes(const this_type& my_)
			: Ptr(0)
			, Size(0){
			
			allocate(my_.Ptr, my_.Size);
		}
		const this_type& operator=(const this_type& my_){
			if(this != &my_){
				clear();
				
				allocate(my_.Ptr, my_.Size);
			}
			return *this;
		}
		~shared_bytes(){ clear(); }
	public:
		//配列を解放
		void clear(){
			if(Ptr == 0)return;

			Deleter(Ptr, Size);

			Ptr = 0;
			Size = 0;
		}
		//配列を解放し、新しく配列を保持
		void assign(size_type Size_){
			clear();

			if(Size_ == 0)return;

			allocate(0, Size_);
		}
		//配列同士を交換
		void swap(this_type& my_){
			pointer tmpPtr = Ptr;
			size_type tmpSize = Size;

			Ptr = my_.Ptr;
			Size = my_.Size;

			my_.Ptr = tmpPtr;
			my_.Size = tmpSize;
		}
		//bytesを作成
		bytes make_bytes(){
			usecnt_type* pUseCnt = reinterpret_cast<usecnt_type*>(Ptr - sizeof(usecnt_type));
			++(*pUseCnt);

			return xc::move(bytes(Ptr, Size, xc::ref(Deleter)));
		}
	public:
		//配列を管理しているか
		operator bool()const{ return Ptr != 0; }
		//配列を管理しているか
		bool empty()const{ return Ptr == 0; }
		//配列先頭アドレス取得
		pointer get(){ return Ptr; }
		//配列先頭アドレス取得(cosnt)
		const pointer get()const{ return Ptr; }
		//配列へアクセス
		reference operator[](size_type Pos_){ return Ptr[Pos_]; }
		//配列へアクセス(const)
		const reference operator[](size_type Pos_)const{ return Ptr[Pos_]; }
		//配列へアクセス
		reference at(size_type Pos_){ return Ptr[Pos_]; }
		//配列へアクセス(const)
		const reference at(size_type Pos_)const{ return Ptr[Pos_]; }
		//サイズを取得
		size_type size()const{ return Size; }
		//配列にデータを詰める
		void fill(unsigned char Data_){
			for(iterator itr = begin(); itr != end(); ++itr){
				*itr = Data_;
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
	template<typename Alloc>
	typename shared_bytes<Alloc>::deleter shared_bytes<Alloc>::Deleter;
}
#
#endif
