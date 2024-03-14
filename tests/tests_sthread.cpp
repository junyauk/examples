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
