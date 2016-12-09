#include "stdafx.h"
#include "CppUnitTest.h"
#include"../../chain.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xc{
	TEST_CLASS(test_chain){
		struct elem : public chain_element{
			int value;
		};
public:
	TEST_METHOD(construct){
		elem Elem;
		xc::chain<elem> Chain;

	}
	};
}
