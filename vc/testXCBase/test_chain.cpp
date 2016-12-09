#include "stdafx.h"
#include "CppUnitTest.h"
#include"../../chain.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xc{
	TEST_CLASS(test_chain){
		struct elem : public chain_element{
			int value;
		public:
			elem()=default;
			elem(int value_):value(value_){}
		};
public:
	TEST_METHOD(construct){
		xc::chain<elem> Chain;
		Assert::IsTrue(Chain.empty());
		Assert::AreEqual(0u,Chain.size());

		elem Element;

		chain_base::next(Element);
	}
	};
	TEST_CLASS(test_sorted_chain){
		struct elem : public sorted_chain_element{
			int value;
		public:
			elem() = default;
			elem(int value_) :value(value_){}
		};
public:
	TEST_METHOD(construct){
		xc::sorted_chain<elem> Chain;
		Assert::IsTrue(Chain.empty());
	}
	};
}
