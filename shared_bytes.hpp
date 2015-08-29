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
		typedef shared_bytes my_type;
		typedef my_type& my_reference;
		typedef xc::rvalue_reference<my_type> my_rvalue_reference;
		typedef unsigned char* pointer;
		typedef unsigned char& reference;
		typedef uint16 size_type;
		typedef uint16 usecnt_type;
		typedef pointer iterator;
		typedef const pointer const_iterator;
		typedef typename Alloc::template rebind<unsigned char>::other allocator;
	private:
		void allocate(size_type Size_){
			unsigned char* base_ptr = allocator().allocate(sizeof(usecnt_type) + Size_);
			pUseCnt = reinterpret_cast<usecnt_type*>(base_ptr);
			*pUseCnt = 1;
			Ptr = base_ptr + sizeof(usecnt_type);
			Size = Size_;
		}
		struct deleter{
			void operator()(void* ptr_, size_type size_){
				if(ptr_ == 0)return;

				usecnt_type* pUseCnt = reinterpret_cast<usecnt_type*>(static_cast<unsigned char*>(ptr_)-sizeof(usecnt_type));
				if(*pUseCnt == 0)return;

				if(--(*pUseCnt) == 0){
					allocator().deallocate(reinterpret_cast<unsigned char*>(pUseCnt), size_ + sizeof(usecnt_type));
				}
			}
		};
		static deleter Deleter;
	private:
		pointer Ptr;
		size_type Size;
		usecnt_type* pUseCnt;
	public:
		shared_bytes()
			: Ptr(0)
			, Size(0)
			, pUseCnt(0){
		}
		shared_bytes(size_type Size_)
			: Ptr(0)
			, Size(0)
			, pUseCnt(0){
			assign(Size_);
		}
		explicit shared_bytes(my_rvalue_reference rref_)
			: Ptr(0)
			, Size(0)
			, pUseCnt(0){
			swap(rref_.ref);
		}
		const my_reference operator=(my_rvalue_reference rref_){
			if(this != &(rref_.ref)){
				clear();
				swap(rref_.ref);
			}
			return *this;
		}
		shared_bytes(const my_reference my_)
			: Ptr(0)
			, Size(0)
			, pUseCnt(0){
			*this = my_;
		}
		const my_reference operator=(const my_reference my_){
			if(this != &my_){
				clear();
				if(my_.Ptr != 0){
					++(*(my_.pUseCnt));	//share
					Ptr = my_.Ptr;
					Size = my_.Size;
					pUseCnt = my_.pUseCnt;
				}
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
			pUseCnt = 0;
		}
		//配列を解放し、新しく配列を保持
		void assign(size_type size_){
			clear();

			if(size_ == 0)return;

			allocate(size_);
		}
		//配列同士を交換
		void swap(my_reference my_){
			pointer tmpPtr = Ptr;
			size_type tmpSize = Size;
			usecnt_type* tmppUseCnt = pUseCnt;

			Ptr = my_.Ptr;
			Size = my_.Size;
			pUseCnt = my_.pUseCnt;

			my_.Ptr = tmpPtr;
			my_.Size = tmpSize;
			my_.pUseCnt = tmppUseCnt;
		}
		//bytesを作成
		bytes make_bytes(){
			++(*pUseCnt);
			return xc::move(bytes(Ptr, Size, xc::ref(Deleter)));
		}
	public:
		//配列を管理しているか
		operator bool()const{ return Ptr != 0; }
		//配列を管理しているか
		bool empty()const{ return Ptr != 0; }
		//配列先頭アドレス取得
		pointer get(){ return Ptr; }
		//配列先頭アドレス取得(cosnt)
		const pointer get()const{ return Ptr; }
		//配列へアクセス
		reference operator[](size_type size_){ return Ptr[size_]; }
		//配列へアクセス(const)
		const reference operator[](size_type size_)const{ return Ptr[size_]; }
		//配列へアクセス
		reference at(size_type size_){ return Ptr[size_]; }
		//配列へアクセス(const)
		const reference at(size_type size_)const{ return Ptr[size_]; }
		//サイズを取得
		size_type size()const{ return Size; }
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
	template<typename Alloc>
	typename shared_bytes<Alloc>::deleter shared_bytes<Alloc>::Deleter;
}
#
#endif
