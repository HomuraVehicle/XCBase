#include "stdafx.h"
#include "CppUnitTest.h"
#include <bitset.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xc{		
	TEST_CLASS(test_bitset)	{
	public:
		TEST_METHOD(set_reset_test){
			bitset<5> Bits(0);

			Bits.set();
			Assert::IsTrue(Bits.test(0));
			Assert::IsTrue(Bits.test(1));
			Assert::IsTrue(Bits.test(2));
			Assert::IsTrue(Bits.test(3));
			Assert::IsTrue(Bits.test(4));

			Bits.reset();
			Assert::IsFalse(Bits.test(0));
			Assert::IsFalse(Bits.test(1));
			Assert::IsFalse(Bits.test(2));
			Assert::IsFalse(Bits.test(3));
			Assert::IsFalse(Bits.test(4));

			Bits.set(3, true);
			Assert::IsFalse(Bits.test(0));
			Assert::IsFalse(Bits.test(1));
			Assert::IsFalse(Bits.test(2));
			Assert::IsTrue(Bits.test(3));
			Assert::IsFalse(Bits.test(4));

			Bits.set(1, true);
			Bits.set(4, true);
			Assert::IsFalse(Bits.test(0));
			Assert::IsTrue(Bits.test(1));
			Assert::IsFalse(Bits.test(2));
			Assert::IsTrue(Bits.test(3));
			Assert::IsTrue(Bits.test(4));


			Bits.set(3, false);
			Assert::IsFalse(Bits.test(0));
			Assert::IsTrue(Bits.test(1));
			Assert::IsFalse(Bits.test(2));
			Assert::IsFalse(Bits.test(3));
			Assert::IsTrue(Bits.test(4));


			Bits.reset(1);
			Assert::IsFalse(Bits.test(0));
			Assert::IsFalse(Bits.test(1));
			Assert::IsFalse(Bits.test(2));
			Assert::IsFalse(Bits.test(3));
			Assert::IsTrue(Bits.test(4));

			Bits.reset(0);
			Bits.set(3, false);
			Bits.set(4, true);
			Assert::IsFalse(Bits.test(0));
			Assert::IsFalse(Bits.test(1));
			Assert::IsFalse(Bits.test(2));
			Assert::IsFalse(Bits.test(3));
			Assert::IsTrue(Bits.test(4));
		}
		TEST_METHOD(bit_shift){
			bitset<5> Bits(0);

			Bits.set();
			Bits >>= 3;
			Assert::IsTrue(Bits.test(0));
			Assert::IsTrue(Bits.test(1));
			Assert::IsFalse(Bits.test(2));
			Assert::IsFalse(Bits.test(3));
			Assert::IsFalse(Bits.test(4));

			Bits.set();
			Bits <<= 3;
			Assert::IsFalse(Bits.test(0));
			Assert::IsFalse(Bits.test(1));
			Assert::IsFalse(Bits.test(2));
			Assert::IsTrue(Bits.test(3));
			Assert::IsTrue(Bits.test(4));
		}
		TEST_METHOD(bit_access){
			bitset<5> Bits(0);

			Assert::IsFalse(Bits[0]);
			Assert::IsFalse(Bits[1]);
			Assert::IsFalse(Bits[2]);
			Assert::IsFalse(Bits[3]);
			Assert::IsFalse(Bits[4]);

			Bits[1] = true;
			Bits[4] = true;
			Assert::IsFalse(Bits[0]);
			Assert::IsTrue(Bits[1]);
			Assert::IsFalse(Bits[2]);
			Assert::IsFalse(Bits[3]);
			Assert::IsTrue(Bits[4]);
		}
	};
}