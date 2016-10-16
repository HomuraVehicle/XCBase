#include "stdafx.h"
#include "CppUnitTest.h"
#include <future.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace xc{
	TEST_CLASS(test_future){
public:
	TEST_METHOD(normal_use){
		promise<double> Promise;
		future<double> Future;
		Assert::IsTrue(Promise.can_get_future(), L"cannot get future");
		Assert::IsFalse(Promise.is_wait_value(), L"Promise valid error");
		Assert::IsFalse(Future.valid(), L"Future valid error");
		
		Future = Promise.get_future();
		Assert::IsFalse(Promise.can_get_future(), L"cannot get future");
		Assert::IsTrue(Promise.is_wait_value(), L"Promise valid error");
		Assert::IsTrue(Future.valid(), L"Future valid error");
		Assert::IsFalse(Future.can_get(), L"cannot get future");

		Promise.set_value(4.5);
		Assert::IsFalse(Promise.can_get_future(), L"cannot get future");
		Assert::IsFalse(Promise.is_wait_value(), L"Promise valid error");
		Assert::IsTrue(Future.valid(), L"Future valid error");
		Assert::IsTrue(Future.can_get(), L"cannot get future");

		double Value = Future.get();
		Assert::AreEqual(4.5, Value, L"fail to read correct value.");

		Assert::IsTrue(Promise.can_get_future(), L"cannot get future");
		Assert::IsFalse(Promise.is_wait_value(), L"Promise valid error");
		Assert::IsFalse(Future.valid(), L"Future valid error");
	}
	TEST_METHOD(cancel){
		promise<double> Promise;
		future<double> Future = Promise.get_future();

		Future.cancel();
		Assert::IsFalse(Promise.can_get_future(), L"cannot get future");
		Assert::IsTrue(Promise.is_wait_value(), L"Promise valid error");
		Assert::IsFalse(Future.valid(), L"Future valid error");

		Promise.set_value(4.4);
		Assert::IsTrue(Promise.can_get_future(), L"cannot get future");
		Assert::IsFalse(Promise.is_wait_value(), L"Promise valid error");
		Assert::IsFalse(Future.valid(), L"Future valid error");
	}
	TEST_METHOD(then){
		promise<double> Promise;
		future<double> Future = Promise.get_future();

		double Value = 0.0;
		Future.then([&](double val){Value = val; });

		Promise.set_value(4.5);
		Assert::AreEqual(4.5, Value, L"fail to read correct value.");

		Assert::IsTrue(Promise.can_get_future(), L"cannot get future");
		Assert::IsFalse(Promise.is_wait_value(), L"Promise valid error");
		Assert::IsFalse(Future.valid(), L"Future valid error");
	}
	TEST_METHOD(then_cancel){
		promise<double> Promise;
		future<double> Future = Promise.get_future();

		double Value = 0.0;
		auto Canceller = Future.then([&](double val){Value = val; });
		Assert::IsTrue(static_cast<bool>(Canceller), L"Fail in Canceller");

		Canceller();
		Assert::IsFalse(static_cast<bool>(Canceller), L"Fail in Canceller");
		Assert::IsFalse(Promise.can_get_future(), L"cannot get future");
		Assert::IsTrue(Promise.is_wait_value(), L"Promise valid error");
		Assert::IsFalse(Future.valid(), L"Future valid error");

		Promise.set_value(4.5);
		Assert::IsTrue(Promise.can_get_future(), L"cannot get future");
		Assert::IsFalse(Promise.is_wait_value(), L"Promise valid error");
		Assert::IsFalse(Future.valid(), L"Future valid error");
	}
	};
}
