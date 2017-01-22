#include "stdafx.h"
#include "CppUnitTest.h"
#include"../../chain.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xc{

	TEST_CLASS(test_chain){
		struct elem : public chain_element{
			double value;
		public:
			elem() = default;
			elem(int value_) :value(value_){}
		};
		using my_chain = chain<elem>;
public:
	TEST_METHOD(value_type){
		my_chain::value_type Elm1;
		Assert::IsFalse(Elm1.owned_by_chain(), L"Elm cannot be written.");

		Elm1.value = 5.2;

		Assert::AreEqual(5.2, Elm1.value);
	}
	TEST_METHOD(construct){
		my_chain Cir;
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Assert::AreEqual(0u, Cir.size(), L"size function error.");
	}
	TEST_METHOD(push_back){
		my_chain::value_type Elm1;
		Elm1.value = 3.5;

		my_chain Cir;
		Cir.push_back(Elm1);
		Assert::IsTrue(Elm1.owned_by_chain(), L"elm can use after it is inserted");

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(1u, Cir.size(), L"size function error.");

		Assert::AreEqual(3.5, Cir.front().value, L"fron function error");

		my_chain::value_type Elm2;
		Elm2.value = 4.5;
		Cir.push_back(Elm2);

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(2u, Cir.size(), L"size function error.");

		Assert::AreEqual(3.5, Cir.front().value, L"front function error");
		Assert::AreEqual(4.5, Cir.back().value, L"back function error");
	}
	TEST_METHOD(push_front){
		my_chain::value_type Elm1;
		Elm1.value = 3.5;

		my_chain Cir;
		Cir.push_front(Elm1);
		Assert::IsTrue(Elm1.owned_by_chain(), L"elm can use after it is inserted");

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(1u, Cir.size(), L"size function error.");

		Assert::AreEqual(3.5, Cir.front().value, L"fron function error");
		Assert::AreEqual(3.5, Cir.back().value, L"back function error");

		my_chain::value_type Elm2;
		Elm2.value = 4.5;
		Cir.push_front(Elm2);

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(2u, Cir.size(), L"size function error.");

		Assert::AreEqual(4.5, Cir.front().value, L"back function error");
		Assert::AreEqual(3.5, Cir.back().value, L"front function error");
	}
	TEST_METHOD(push_back_multi){
		my_chain Cir;

		my_chain::value_type Elm[10];
		for(int i = 0; i < 10; ++i){
			Elm[i].value = i * 5;
			Cir.push_back(Elm[i]);
		}

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(10u, Cir.size(), L"size function error.");

		auto Itr = Cir.begin();
		for(int i = 0; i < 10; ++i){
			Assert::AreEqual(i * 5., Itr->value, L"");
			++Itr;
		}
		Assert::IsTrue(Itr == Cir.end(), L"itr == error");
	}
	TEST_METHOD(push_front_multi){
		my_chain Cir;

		my_chain::value_type Elm[10];
		for(int i = 0; i < 10; ++i){
			Elm[i].value = i * 5;
			Cir.push_front(Elm[i]);
		}

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(10u, Cir.size(), L"size function error.");

		auto Itr = Cir.begin();
		for(int i = 0; i < 10; ++i){
			Assert::AreEqual((10 - i - 1) * 5., Itr->value, L"");
			++Itr;
		}
		Assert::IsTrue(Itr == Cir.end(), L"itr == error");
	}
	TEST_METHOD(insert){
		my_chain Cir;

		//setup value_types
		my_chain::value_type Elm[10];
		for(int i = 0; i < 10; ++i){
			Elm[i].value = i * 5;
		}

		//push back
		for(int i = 0; i < 5; ++i){
			Cir.push_back(Elm[i * 2 + 1]);
		}

		//insert
		auto IItr = Cir.begin();
		for(int i = 0; i < 5; ++i){
			Assert::AreEqual(i * 2, std::distance(Cir.begin(), IItr), L"Place error");
			IItr = Cir.insert(IItr, Elm[i * 2]);
			Assert::AreEqual(Elm[i * 2].value, IItr->value, L"Not returned same value_type");
			std::advance(IItr, 2);
		}

		//check all value_types
		auto Itr = Cir.begin();
		for(int i = 0; i < 10; ++i){
			Assert::AreEqual(i * 5.0, Itr->value, L"Inserted order error");
			++Itr;
		}
		Assert::IsTrue(Itr == Cir.end(), L"itr == error");
	}
	TEST_METHOD(erase){
		my_chain Cir;

		//setup value_types
		my_chain::value_type Elm[10];
		for(int i = 0; i < 10; ++i){
			Elm[i].value = i * 5;
		}

		//push back
		for(int i = 0; i < 10; ++i){
			Cir.push_back(Elm[i]);
		}

		//erase
		auto IItr = Cir.begin();
		for(int i = 0; i < 5; ++i){
			Assert::AreEqual(i, std::distance(Cir.begin(), IItr), L"Place error");
			IItr = Cir.erase(IItr);
			++IItr;
		}

		//check all value_types
		auto Itr = Cir.begin();
		for(int i = 0; i < 5; ++i){
			Assert::AreEqual((2.0 * i + 1) * 5, Itr->value, L"Erased order error");
			++Itr;
		}
		Assert::IsTrue(Itr == Cir.end(), L"itr == error");

		//check value_types
		for(int i = 0; i < 10; ++i){
			Assert::AreEqual(Elm[i].owned_by_chain(), i % 2 != 0, L"Release value_type error");
		}
	}
	TEST_METHOD(clear){
		my_chain Cir;

		//setup value_types
		my_chain::value_type Elm[10];
		for(int i = 0; i < 10; ++i){
			Elm[i].value = i * 5;
		}

		//push back
		for(int i = 0; i < 10; ++i){
			Cir.push_back(Elm[i]);
		}

		//clear
		Cir.clear();

		Assert::IsTrue(Cir.empty(), L"empty error after clear.");
		Assert::AreEqual(0u, Cir.size(), L"size error after clear.");

		//check value_types
		for(int i = 0; i < 10; ++i){
			Assert::IsFalse(Elm[i].owned_by_chain(), L"Release value_type error");
		}
	}
	};
}
