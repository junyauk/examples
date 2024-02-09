#include "pch.h"

#include <string>
#include <map>
#include <functional>

#include "templates.h"
#include "typeerasure.h"

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::function;

TEST(General, IsSame)
{
	using namespace TemplateExamples::IsSameType;
	cout << isSame<int, int>::value << endl;
	cout << isSame<int, short>::value << endl;
	cout << isSame<int, char>::value << endl;
	cout << isSame<unsigned int, unsigned long>::value << endl;
	cout << isSame<std::string, std::wstring>::value << endl;
}

using namespace TemplateExamples::MyArray;

class MyArrayTests : public ::testing::Test
{
protected:
	MyArray<int, 10> myarray;

	virtual void SetUp() { reset(); }
//	virtual void TearDown(){}

	void reset()
	{
		for (auto i = 0; i < 10; i++)
		{
			// operator[] of MyArray
			myarray[i] = i;
		}
	}

	void rangeExceptionTest(function<void()> const& func, const string& expectedMessage)
	{
		EXPECT_THROW(func(), std::out_of_range);
		try { func(); }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), expectedMessage.c_str());
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}
	}
};

TEST_F(MyArrayTests, MyArrayBasicTests)
{
	// size()
	EXPECT_EQ(myarray.size(), 10);
	// []
	int ret = myarray[6];
	EXPECT_EQ(ret, 6);

	// front()/back()
	EXPECT_EQ(myarray.front(), 0);
	EXPECT_EQ(myarray.back(), 9);

	// fill()
	myarray.fill(4);
	for (auto i = 0; i < 10; i++)
	{
		EXPECT_EQ(myarray[i], 4);
	}

}

#if 1
TEST_F(MyArrayTests, MyArrayIteratorTests)
{
	{	// begin(), end() and operator++ and operator++(int)
		auto itr = myarray.begin();
		// operator[] and operator*
		for (auto i = 0; i < myarray.size(); i++)
		{
			// operator[]
			EXPECT_EQ(itr[i], i);		// [] read
			itr[i] = i * 2;
			EXPECT_EQ(itr[i], i * 2);	// [] write

			auto itr2 = &itr[i];
			// operator*
			EXPECT_EQ(*itr2, i * 2);		// * read
			*itr2 = i * 3;
			EXPECT_EQ(*itr2, i * 3);		// * write
		}
	}
	reset();

	{	// operator-- and operator--(int)
		auto itr = myarray.end();
		auto val = *--itr;
		EXPECT_EQ(val, 9);
		----itr;
		val = *itr--;
		EXPECT_EQ(val, 7);
	}
	{	// operator++ and operator++(int)
		auto itr = myarray.begin();
		auto val = *itr++;
		EXPECT_EQ(val, 0);
		val = *++itr;
		EXPECT_EQ(val, 2);
	}
	{	// multiple
		auto itr = myarray.begin();
		auto val = *--++++++++++++itr; // + 6 - 1
		EXPECT_EQ(val, 5);
		// Note:
		// forward can be specified multiple times, but backward can't
		val = *itr++;
		EXPECT_EQ(val, 5);
		val = *itr--;
		EXPECT_EQ(*itr, 5);
	}
	{	// operator+=, operator-=, operator+ and operator-
		auto itr = myarray.begin();
		itr += 8;
		EXPECT_EQ(*itr, 8);
		itr -= 3;
		EXPECT_EQ(*itr, 5);
		EXPECT_EQ(*(itr + 2), 7);
		EXPECT_EQ(*(itr - 5), 0);
	}

	{	// operator==, operator!=, operator<, operator<=, operator> and operator>=  
		auto b = myarray.begin();
		auto e = myarray.end();
		auto itr = b + 9;
		EXPECT_TRUE(b == myarray.begin());
		EXPECT_FALSE(b == e);
		EXPECT_TRUE(e - 1 == itr);
		EXPECT_TRUE(b != itr);

		auto m = b + 5;	// centre
		EXPECT_TRUE((b + 3) < m);
		EXPECT_FALSE((b + 3) >= m);
		EXPECT_TRUE((b + 7) > m);
		EXPECT_FALSE((b + 7) <= m);
	}
}
#endif

#if 1
TEST_F(MyArrayTests, MyArrayConstIteratorTests)
{
	{	// begin(), end() and operator++ and operator++(int)
		auto itr = myarray.cbegin();
		// operator[] and operator*
		for (auto i = 0; i < myarray.size(); i++)
		{
			// operator[]
			EXPECT_EQ(itr[i], i);		// [] read
			auto itr2 = &itr[i];
			// operator*
			EXPECT_EQ(*itr2, i);		// * read
		}
	}
	reset();

	{	// operator-- and operator--(int)
		auto itr = myarray.cend();
		auto val = *--itr;
		EXPECT_EQ(val, 9);
		----itr;
		val = *itr--;
		EXPECT_EQ(val, 7);
	}
	{	// operator++ and operator++(int)
		auto itr = myarray.cbegin();
		auto val = *itr++;
		EXPECT_EQ(val, 0);
		val = *++itr;
		EXPECT_EQ(val, 2);
	}
	{	// multiple
		auto itr = myarray.cbegin();
		auto val = *--++++++++++++itr; // + 6 - 1
		EXPECT_EQ(val, 5);
		// Note:
		// forward can be specified multiple times, but backward can't
		val = *itr++;
		EXPECT_EQ(val, 5);
		val = *itr--;
		EXPECT_EQ(*itr, 5);
	}
	{	// operator+=, operator-=, operator+ and operator-
		auto itr = myarray.cbegin();
		itr += 8;
		EXPECT_EQ(*itr, 8);
		itr -= 3;
		EXPECT_EQ(*itr, 5);
		EXPECT_EQ(*(itr + 2), 7);
		EXPECT_EQ(*(itr - 5), 0);
	}

	{	// operator==, operator!=, operator<, operator<=, operator> and operator>=  
		auto b = myarray.cbegin();
		auto e = myarray.cend();
		auto itr = b + 9;
		auto b2 = myarray.cbegin();
		EXPECT_TRUE(b == b2);
		EXPECT_FALSE(b == e);
		EXPECT_TRUE(e - 1 == itr);
		EXPECT_TRUE(b != itr);

		auto m = b + 5;	// centre
		EXPECT_TRUE((b + 3) < m);
		EXPECT_FALSE((b + 3) >= m);
		EXPECT_TRUE((b + 7) > m);
		EXPECT_FALSE((b + 7) <= m);
	}
}
#endif

#if 1
TEST_F(MyArrayTests, MyArrayOutOfRangeTests)
{
	// our of range check
	map<string, string>	expectedMessage
	{
		{"dereference", "cannot dereference out of range array iterator"},
		{"increment",   "cannot increment array iterator past end"},
		{"decrement",   "cannot decrement array iterator before begin"}
	};

	{	// [] and *
		rangeExceptionTest([&] { auto ret = myarray[-1]; }, expectedMessage["dereference"]);
		rangeExceptionTest([&] { auto ret = myarray[10]; }, expectedMessage["dereference"]);
		rangeExceptionTest([&] { auto ret = myarray[SIZE_MAX]; }, expectedMessage["dereference"]);
	}
}
#endif

#if 1
TEST_F(MyArrayTests, MyArrayIteratorExceptionTests)
{
	// exception check
	auto b = myarray.begin();
	auto e = myarray.end();
	map<string, string>	expectedMessage
	{
		{"dereference", "cannot dereference out of range array iterator"},
		{"increment",   "cannot increment array iterator past end"},
		{"decrement",   "cannot decrement array iterator before begin"}
	};

	{	// [] and *
		rangeExceptionTest([&] { auto itr = b[-1];}, expectedMessage["dereference"]);
		rangeExceptionTest([&] { auto itr = e[0];}, expectedMessage["dereference"]);
		rangeExceptionTest([&] { auto val = *e; }, expectedMessage["dereference"]);
	}

	{	// ++ and --
		rangeExceptionTest([&] { e++; }, expectedMessage["increment"]);
		rangeExceptionTest([&] { b--; }, expectedMessage["decrement"]);
	}

	{	// += and -=
		rangeExceptionTest([&] { e += 1; }, expectedMessage["increment"]);
		rangeExceptionTest([&] { b -= 1; }, expectedMessage["decrement"]);
	}

	{	// + and -
		rangeExceptionTest([&] { auto itr = e + 1; }, expectedMessage["increment"]);
		rangeExceptionTest([&] { auto itr = b - 1; }, expectedMessage["decrement"]);
		rangeExceptionTest([&] { auto itr = b + 10; }, expectedMessage["increment"]);
		rangeExceptionTest([&] { auto itr = e - 11; }, expectedMessage["decrement"]);
	}
}
#endif

#if 1
TEST_F(MyArrayTests, MyArrayConstIteratorExceptionTests)
{
	// exception check
	auto b = myarray.cbegin();
	auto e = myarray.cend();
	map<string, string>	expectedMessage
	{
		{"dereference", "cannot dereference out of range array iterator"},
		{"increment",   "cannot increment array iterator past end"},
		{"decrement",   "cannot decrement array iterator before begin"}
	};

	{	// [] and *
		rangeExceptionTest([&] { auto itr = b[-1]; }, expectedMessage["dereference"]);
		rangeExceptionTest([&] { auto itr = e[0]; }, expectedMessage["dereference"]);
		rangeExceptionTest([&] { auto val = *e; }, expectedMessage["dereference"]);
	}

	{	// ++ and --
		rangeExceptionTest([&] { e++; }, expectedMessage["increment"]);
		rangeExceptionTest([&] { b--; }, expectedMessage["decrement"]);
	}

	{	// += and -=
		rangeExceptionTest([&] { e += 1; }, expectedMessage["increment"]);
		rangeExceptionTest([&] { b -= 1; }, expectedMessage["decrement"]);
	}

	{	// + and -
		rangeExceptionTest([&] { auto itr = e + 1; }, expectedMessage["increment"]);
		rangeExceptionTest([&] { auto itr = b - 1; }, expectedMessage["decrement"]);
		rangeExceptionTest([&] { auto itr = b + 10; }, expectedMessage["increment"]);
		rangeExceptionTest([&] { auto itr = e - 11; }, expectedMessage["decrement"]);
	}
}
#endif

TEST(General, TypeErasure)
{
	EXPECT_NO_THROW(Run_TypeErasureStep1());
	EXPECT_NO_THROW(Run_TypeErasureStep2());
	EXPECT_NO_THROW(Run_TypeErasureStep3());
	EXPECT_NO_THROW(Run_TypeErasureStep4());
	EXPECT_NO_THROW(Run_TypeErasureStep5());
	EXPECT_NO_THROW(Run_TypeErasureStep6());
	EXPECT_NO_THROW(Run_TypeErasureExercise1());
}

