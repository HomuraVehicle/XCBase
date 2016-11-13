#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../shared_bytes.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xc{		
	TEST_CLASS(test_shared_bytes){
	public:
		TEST_METHOD(construct){
			xc::shared_bytes<> Bytes;

			Assert::IsFalse(static_cast<bool>(Bytes));
			Assert::IsTrue(Bytes.empty());
			Assert::AreEqual(0u, Bytes.size());
		}
		TEST_METHOD(assign_construct){
			xc::shared_bytes<> Bytes(100);

			Assert::IsTrue(static_cast<bool>(Bytes));
			Assert::IsFalse(Bytes.empty());
			Assert::AreEqual(100u, Bytes.size());
			Assert::AreEqual(100u, static_cast<unsigned int>(Bytes.end() - Bytes.begin()));
		}
		TEST_METHOD(assign){
			xc::shared_bytes<> Bytes;

			Bytes.assign(100);

			Assert::IsTrue(static_cast<bool>(Bytes));
			Assert::IsFalse(Bytes.empty());
			Assert::AreEqual(100u, Bytes.size());
			Assert::AreEqual(100u, static_cast<unsigned int>(Bytes.end() - Bytes.begin()));
		}
		TEST_METHOD(clear){
			xc::shared_bytes<> Bytes(100);

			Assert::IsTrue(static_cast<bool>(Bytes));
			Assert::IsFalse(Bytes.empty());
			Assert::AreEqual(100u, Bytes.size());
			Assert::AreEqual(100u, static_cast<unsigned int>(Bytes.end() - Bytes.begin()));

			Bytes.clear();

			Assert::IsFalse(static_cast<bool>(Bytes));
			Assert::IsTrue(Bytes.empty());
			Assert::AreEqual(0u, Bytes.size());
		}
		TEST_METHOD(copy_construct){
			xc::shared_bytes<> B1(100);
			xc::shared_bytes<> B2(B1);

			Assert::IsFalse(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(100u, B1.size());
			Assert::AreEqual(100u, B2.size());
			Assert::AreEqual(100u, static_cast<unsigned int>(B1.end() - B1.begin()));
			Assert::IsTrue(B1.begin() == B2.begin());
			Assert::IsTrue(B1.end() == B2.end());

			B1.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(100u, B2.size());

			B2.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsTrue(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(0u, B2.size());
		}
		TEST_METHOD(copy_operator){
			xc::shared_bytes<> B1(100);
			xc::shared_bytes<> B2;

			B2 = B1;

			Assert::IsFalse(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(100u, B1.size());
			Assert::AreEqual(100u, B2.size());
			Assert::AreEqual(100u, static_cast<unsigned int>(B1.end() - B1.begin()));
			Assert::IsTrue(B1.begin() == B2.begin());
			Assert::IsTrue(B1.end() == B2.end());

			B1.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(100u, B2.size());

			B2.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsTrue(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(0u, B2.size());
		}
		TEST_METHOD(move_construct){
			xc::shared_bytes<> B1(100);
			xc::shared_bytes<> B2(xc::move(B1));

			Assert::IsTrue(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(100u, B2.size());

			B2.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsTrue(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(0u, B2.size());
		}
		TEST_METHOD(move_operator){
			xc::shared_bytes<> B1(100);
			xc::shared_bytes<> B2;

			B2 = xc::move(B1);

			Assert::IsTrue(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(100u, B2.size());

			B2.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsTrue(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(0u, B2.size());
		}
		TEST_METHOD(make_bytes_sharedFirst){
			xc::shared_bytes<> B1(100);
			xc::bytes B2(xc::move(B1.make_bytes()));

			Assert::IsFalse(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(100u, B1.size());
			Assert::AreEqual(100u, B2.size());
			Assert::AreEqual(100u, static_cast<unsigned int>(B1.end() - B1.begin()));
			Assert::IsTrue(B1.begin() == B2.begin());
			Assert::IsTrue(B1.end() == B2.end());

			B1.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(100u, B2.size());

			B2.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsTrue(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(0u, B2.size());
		}
		TEST_METHOD(make_bytes_nonsharedFirst){
			xc::bytes B1;
			xc::shared_bytes<> B2(100);
			B1 = xc::move(B2.make_bytes());

			Assert::IsFalse(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(100u, B1.size());
			Assert::AreEqual(100u, B2.size());
			Assert::AreEqual(100u, static_cast<unsigned int>(B1.end() - B1.begin()));
			Assert::IsTrue(B1.begin() == B2.begin());
			Assert::IsTrue(B1.end() == B2.end());

			B1.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(100u, B2.size());

			B2.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsTrue(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(0u, B2.size());
		}
		TEST_METHOD(swap){
			constexpr unsigned int BufSize = 100;
			xc::shared_bytes<> B1(BufSize);
			xc::shared_bytes<> B2(BufSize * 2);

			unsigned char *Ptr1, *Ptr2;
			Ptr1 = B1.begin();
			Ptr2 = B2.begin();

			B1.swap(B2);

			Assert::IsFalse(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(BufSize * 2, B1.size());
			Assert::AreEqual(BufSize, B2.size());

			Assert::AreEqual(Ptr2, B1.begin());
			Assert::AreEqual(Ptr1, B2.begin());


			B1.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsFalse(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(BufSize, B2.size());

			B2.clear();

			Assert::IsTrue(B1.empty());
			Assert::IsTrue(B2.empty());
			Assert::AreEqual(0u, B1.size());
			Assert::AreEqual(0u, B2.size());
		}
	};
}