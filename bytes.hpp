#ifndef XCBASE_BYTES_INC
#define XCBASE_BYTES_INC 100
#
#include"allocator.hpp"
#include"type.hpp"
#include"rvalue_reference.hpp"
#include"function.hpp"
namespace xc{
	struct bytes{
		typedef bytes my_type;
		typedef my_type& my_reference;
		typedef xc::rvalue_reference<my_type> my_rvalue_reference;
		typedef unsigned char* pointer;
		typedef unsigned char& reference;
		typedef uint16 size_type;
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
			typedef typename alloc::template rebind<unsigned char>::other allocator;
			struct deleter{
				void operator()(void* ptr, bytes::size_type size){
					allocator().deallocate(static_cast<unsigned char*>(ptr), size);
				}
			};
			static deleter Deleter;

			Ptr = static_cast<unsigned char*>(Alloc.allocate(Size_));
			Size = Size_;
			Dlt = xc::ref(Deleter);
		}
		bytes(pointer Ptr_, size_type Size_, const deleter&  Dlt_)
			: Ptr(0)
			, Size(0)
			, Dlt(){
			assign(Ptr_, Size_, Dlt_);
		}
		bytes(my_rvalue_reference rref_)
			: Ptr(0)
			, Size(0)
			, Dlt(){
			swap(rref_.ref);
		}
		const my_reference operator=(my_rvalue_reference rref_){
			if(this != &(rref_.ref)){
				clear();
				swap(rref_.ref);
			}
			return *this;
		}
		~bytes(){ 
			clear();
		}
	private://�R�s�[�͋֎~
		bytes(const my_reference my_);
		const my_reference operator=(const my_reference my_);
	public:
		//�z������
		void clear(){
			if(Ptr==0)return;

			if(Dlt){
				Dlt(Ptr, Size);
				Dlt.clear();
				Ptr = 0;
				Size = 0;
			}
		}
		//�z���������A�V�����z���ێ�
		void assign(pointer ptr_, size_type size_, const deleter& dlt_){
			clear();

			if(ptr_ == 0 || size_ == 0)return;

			Ptr = ptr_;
			Size = size_;
			Dlt = dlt_;
		}
		//�z����J�����A�V�����z���ێ�
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

			Ptr = static_cast<unsigned char*>(Alloc.allocate(Size_));
			Size = Size_;
			Dlt = xc::ref(Deleter);
		}
		//�z�񓯎m������
		void swap(my_reference my_){
			if(&my_ == this)return;

			pointer tmpPtr = Ptr;
			size_type tmpSize = Size;
			deleter tmpDlt = Dlt;

			Ptr = my_.Ptr;
			Size = my_.Size;
			Dlt = my_.Dlt;

			my_.Ptr = tmpPtr;
			my_.Size = tmpSize;
			my_.Dlt = tmpDlt;
		}
		//deleter���擾
		deleter get_deleter() { return Dlt; }
		//�z��͈�؏����������
		pointer release() {
			pointer AnsPtr=Ptr;
			Ptr=0;
			Size=0;
			Dlt.clear();
			return AnsPtr;
		}
	public:
		//�z����Ǘ����Ă��邩
		operator bool()const{ return Ptr!=0; }
		//�z����Ǘ����Ă��邩
		bool empty()const{ return Ptr == 0; }
		//�z��擪�A�h���X�擾
		pointer get(){return Ptr;}
		//�z��擪�A�h���X�擾(cosnt)
		const pointer get()const{return Ptr;}
		//�z��փA�N�Z�X
		reference operator[](size_type size_){return Ptr[size_];}
		//�z��փA�N�Z�X(const)
		const reference operator[](size_type size_)const{return Ptr[size_];}
		//�z��փA�N�Z�X
		reference at(size_type size_){ return Ptr[size_]; }
		//�z��փA�N�Z�X(const)
		const reference at(size_type size_)const{ return Ptr[size_]; }
		//�T�C�Y���擾
		size_type size()const{return Size;}
		//�z��Ƀf�[�^���l�߂�
		void fill(unsigned char data_){
			for(iterator itr = begin(); itr != end(); ++itr){
				*itr = data_;
			}
		}
	public:
		//iterator�擾�֐�
		iterator begin(){ return get(); }
		iterator end(){ return get() + size(); }
		//const_iterator�擾�֐�
		const_iterator begin()const{ return get(); }
		const_iterator end()const{ return get() + size(); }
	};
}
#
#endif
