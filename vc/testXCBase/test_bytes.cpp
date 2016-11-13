#include "stdafx.h"
#include "CppUnitTest.h"
#include"../../bytes.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xc{		
	TEST_CLASS(test_bytes){
public:
	TEST_METHOD(construct_with_size){
		constexpr unsigned int BufSize = 100;

		xc::bytes Bytes(BufSize);

		//Size Check
		Assert::AreEqual(BufSize, Bytes.size());

		//Itr Diff Check
		Assert::AreEqual(BufSize, static_cast<unsigned int>(Bytes.end() - Bytes.begin()));

		//Initial Reference Address
		Assert::AreEqual(Bytes.begin(), &Bytes.at(0));

		//End Reference Address
		Assert::AreEqual(Bytes.end(), &Bytes.at(0) + BufSize);
	}
	TEST_METHOD(move_construct){
		constexpr unsigned int BufSize = 100;
		xc::bytes V1(BufSize);

		unsigned char* Ptr = V1.begin();

		xc::bytes V2(xc::move(V1));

		Assert::IsTrue(V1.empty());
		Assert::IsFalse(V2.empty());

		Assert::AreEqual(BufSize, V2.size());
		Assert::AreEqual(Ptr, V2.begin());
	}
	TEST_METHOD(move_operator){
		constexpr unsigned int BufSize = 100;
		xc::bytes V1(BufSize);

		unsigned char* Ptr = V1.begin();

		xc::bytes V2;
		V2 = xc::move(V1);

		Assert::IsTrue(V1.empty());
		Assert::IsFalse(V2.empty());

		Assert::AreEqual(BufSize, V2.size());
		Assert::AreEqual(Ptr, V2.begin());
	}
	TEST_METHOD(swap){
		constexpr unsigned int BufSize = 100;
		xc::bytes V1(BufSize);
		xc::bytes V2(BufSize*2);

		unsigned char *Ptr1, *Ptr2;
		Ptr1 = V1.begin();
		Ptr2 = V2.begin();

		V1.swap(V2);

		Assert::AreEqual(BufSize * 2, V1.size());
		Assert::AreEqual(BufSize, V2.size());
		Assert::AreEqual(Ptr2, V1.begin());
		Assert::AreEqual(Ptr1, V2.begin());
	}
	TEST_METHOD(deleter){
		struct buffer{
			bool Used;
			unsigned char Buffer[100];
		public:
			struct deleter{
				buffer* Ptr;
			public:
				deleter(buffer* Ptr_) :Ptr(Ptr_){}
				void operator()(void* Ptr_, unsigned int Size_){
					Ptr->Used = false;
				}
			};
		public:
			buffer() :Used(false){}
		};

		buffer Buf;

		xc::bytes Bytes(Buf.Buffer, 100, buffer::deleter(&Buf));
		Buf.Used = true;

		Assert::IsTrue(Buf.Used);

		Bytes.clear();

		Assert::IsFalse(Buf.Used);
	}
	TEST_METHOD(deleter_move){
		struct buffer{
			bool Used;
			unsigned char Buffer[100];
		public:
			struct deleter{
				buffer* Ptr;
			public:
				deleter(buffer* Ptr_) :Ptr(Ptr_){}
				void operator()(void* Ptr_, unsigned int Size_){
					Ptr->Used = false;
				}
			};
		public:
			buffer() :Used(false){}
		};

		buffer Buf;

		xc::bytes V1(Buf.Buffer, 100, buffer::deleter(&Buf));
		Buf.Used = true;

		Assert::IsTrue(Buf.Used);

		xc::bytes V2(xc::move(V1));
		V1.clear();
		Assert::IsTrue(Buf.Used);

		V2.clear();
		Assert::IsFalse(Buf.Used);
	}
	TEST_METHOD(deleter_swap){
		struct buffer{
			bool Used;
			unsigned char Buffer[100];
		public:
			struct deleter{
				buffer* Ptr;
			public:
				deleter(buffer* Ptr_) :Ptr(Ptr_){}
				void operator()(void* Ptr_, unsigned int Size_){
					Ptr->Used = false;
				}
			};
		public:
			buffer() :Used(false){}
		};

		buffer Buf1;
		buffer Buf2;

		xc::bytes V1(Buf1.Buffer, 100, buffer::deleter(&Buf1));
		Buf1.Used = true;
		Assert::IsTrue(Buf1.Used);

		xc::bytes V2(Buf2.Buffer, 100, buffer::deleter(&Buf2));
		Buf2.Used = true;
		Assert::IsTrue(Buf2.Used);

		V1.swap(V2);
		Assert::IsTrue(Buf1.Used);
		Assert::IsTrue(Buf2.Used);

		V1.clear();
		Assert::IsTrue(Buf1.Used);
		Assert::IsFalse(Buf2.Used);

		V2.clear();
		Assert::IsFalse(Buf1.Used);
		Assert::IsFalse(Buf2.Used);
	}
	};
}
