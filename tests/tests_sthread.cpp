#include "pch.h"

#include "abstractfactory.h"
#include "adapter.h"
#include "visitor.h"
#include "builder.h"
#include "command.h"
#include "composite.h"
#include "factorymethod.h"
#include "iterator.h"
#include "observer.h"
#include "prototype.h"
#include "singleton.h"
#include "strategy.h"
#include "templatemethod.h"
#include "decorator.h"
#include "bridge.h"
#include "facade.h"
#include "flyweight.h"
#include "proxy.h"
#include "chainofresponsibility.h"
#include "interpreter.h"
#include "mediator.h"
#include "memento.h"
#include "state.h"

// Note:
// Most of tests run only a single test. Because they are not currently testing details.
// Will be updated.

TEST(SingleThread, AbstactFactory)
{
	using namespace AbstractFactory::Basic1;
	int ret = 0;
	EXPECT_NO_THROW(ret = Run_AbstractFactory());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, AdapterBasic1)
{
	using namespace Adapter::Basic1;
	int ret = 0;
	EXPECT_NO_THROW(ret = Run_Adapter());
	EXPECT_EQ(ret, 0);
}
TEST(SingleThread, AdapterBasic2)
{
	using namespace Adapter::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, VisitorBasic1)
{
	using namespace Visitor::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, VisitorBasic2)
{
	using namespace Visitor::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, BuilderBasic1)
{
	using namespace Builder::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, BuilderBasic2)
{
	using namespace Builder::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, CommandBasic1)
{
	using namespace Command::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, CommandBasic2)
{
	using namespace Command::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, CompositeBasic1)
{
	using namespace Composite::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, CompositeBasic2)
{
	using namespace Composite::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, FactoryMethodBasic1)
{
	using namespace FactoryMethod::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, IteratorBasic1)
{
	using namespace Iterator::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, IteratorBasic2)
{
	using namespace Iterator::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, ObserverBasic1)
{
	using namespace Observer::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, ObserverBasic2)
{
	using namespace Observer::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, PrototypeBasic1)
{
	using namespace Prototype::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, SingletonBasic1)
{
	using namespace Singleton::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}
TEST(SingleThread, StrategyBasic1)
{
	using namespace Strategy::Basic1;
	Tests tests;
	string result;
	int ret = 0;
	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(ret = tests.run());
	result = testing::internal::GetCapturedStdout();
	char expected[] = "----------\n- - - - - \n";
	EXPECT_STREQ(expected, result.c_str());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, StrategyBasic1Step2)
{
	using namespace Strategy::Basic1Step2;
	Tests tests;
	string result;
	int ret = 0;
	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(ret = tests.run());
	result = testing::internal::GetCapturedStdout();
	char expected[] = "----------\n- - - - - \n";
	EXPECT_STREQ(expected, result.c_str());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, TemplateMethodBasic1)
{
	using namespace TemplateMethod::Basic1;
	Tests tests;
	string result;
	int ret = 0;
	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(ret = tests.run());
	result = testing::internal::GetCapturedStdout();
	char expected[] = "<IBase pre process> <Derived1 main process> <IBase post process> <IBase pre process> <Derived2 main process> <Derived2 post process>\n";
	EXPECT_STREQ(expected, result.c_str());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, DecoratorBasic1)
{
	using namespace Decorator::Basic1;
	Tests tests;
	string result;
	int ret = 0;
	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(ret = tests.run());
	result = testing::internal::GetCapturedStdout();
	char expected[] = "Brewing simple coffee\nBrewing simple coffee\nAdding milk to coffee\nBrewing simple coffee\nAdding milk to coffee\nAdding sugar to coffee\n";
	EXPECT_STREQ(expected, result.c_str());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, DecoratorBasic2)
{
	using namespace Decorator::Basic2;
	Tests tests;
	string result;
	int ret = 0;
	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(ret = tests.run());
	result = testing::internal::GetCapturedStdout();
	char expected[] = "Plain Text\n<b>Plain Text</b>\n<i><b>Plain Text</b></i>\nNew Text\n<b>New Text</b>\n<i><b>New Text</b></i>\n";
	EXPECT_STREQ(expected, result.c_str());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, BridgeBasic1)
{
	using namespace Bridge::Basic1;
	Tests tests;
	string result;
	int ret = 0;
	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(ret = tests.run());
	result = testing::internal::GetCapturedStdout();
	char expected[] = "CAbstract's operation runs impl's operation\nCImplA\nCAbstract's operation runs impl's operation\nCImplB\n";
	EXPECT_STREQ(expected, result.c_str());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, BridgeBasic2)
{
	using namespace Bridge::Basic2;
	Tests tests;
	string result;
	int ret = 0;
	testing::internal::CaptureStdout();
	EXPECT_NO_THROW(ret = tests.run());
	result = testing::internal::GetCapturedStdout();
	char expected[] = 
		"Tape Device Powered On\n"
		"Tape Device play music\n"
		"Tape Device pause music\n"
		"Tape Device Powered Off\n"
		"Vinyl Device Powered On\n"
		"Vinyl Device play music\n"
		"Vinyl Device pause music\n"
		"Vinyl Device Powered Off\n";
	EXPECT_STREQ(expected, result.c_str());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, BridgeBasic2Step2)
{
	using namespace Bridge::Basic2Step2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, FacadeBasic1)
{
	using namespace Facade::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, FacadeBasic2)
{
	using namespace Facade::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, FlyweightBasic1)
{
	using namespace Flyweight::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, ProxyBasic1)
{
	using namespace Proxy::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, ProxyBasic2)
{
	using namespace Proxy::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, ChainOfResponsibilityBasic1)
{
	using namespace ChainOfResponsibility::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, InterpreterBasic1)
{
	using namespace Interpreter::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, InterpreterBasic2)
{
	using namespace Interpreter::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, MediatorBasic1)
{
	using namespace Mediator::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, MediatorBasic1Step2)
{
	using namespace Mediator::Basic1Step2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, MementoBasic1)
{
	using namespace Memento::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, MementoBasic2)
{
	using namespace Memento::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(SingleThread, StateBasic1)
{
	using namespace State::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}
