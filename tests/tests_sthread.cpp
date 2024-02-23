#include "pch.h"

#include "abstractfactory.h"
#include "adapter.h"
#include "visitor.h"

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
