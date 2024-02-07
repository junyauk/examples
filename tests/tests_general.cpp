#include "pch.h"

#include "templates.h"
#include "typeerasure.h"

using std::cout;
using std::endl;
using std::string;

TEST(General, IsSame)
{
	using namespace TemplateExamples::IsSameType;
	cout << isSame<int, int>::value << endl;
	cout << isSame<int, short>::value << endl;
	cout << isSame<int, char>::value << endl;
	cout << isSame<unsigned int, unsigned long>::value << endl;
	cout << isSame<std::string, std::wstring>::value << endl;
}
TEST(General, TemplatesBasics)
{
	using namespace TemplateExamples::MyArray;
	MyArray<int, 10> myarray;

	auto reset = [&]() {
		for (auto i = 0; i < 10; i++)
		{
			// operator[] of MyArray
			myarray[i] = i;
		}
		};

	// Initialize
	reset();
	int ret = myarray[6];
	EXPECT_EQ(ret, 6);

	// front()/back()
	print(myarray);
	EXPECT_EQ(myarray.front(), 0);
	EXPECT_EQ(myarray.back(), 9);

	// fill()
	myarray.fill(4);
	for (auto i = 0; i < 10; i++)
	{
		EXPECT_EQ(myarray[i], 4);
	}
	reset();

	// -- iterator tests ---------------------------

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

TEST(General, TemplatesExceptions)
{
	using namespace TemplateExamples::MyArray;
	MyArray<int, 10> myarray;

	auto reset = [&]() {
		for (auto i = 0; i < 10; i++)
		{
			// operator[] of MyArray
			myarray[i] = i;
		}
		};
	reset();

	// exception check
	auto b = myarray.begin();
	auto e = myarray.end();

	{	// [] and *
		EXPECT_THROW(auto itr = b[-1], std::out_of_range);
		try { auto itr = b[-1]; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot dereference out of range array iterator");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}

		EXPECT_THROW(auto itr = e[0], std::out_of_range);
		try { auto itr = e[0]; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot dereference out of range array iterator");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}
		EXPECT_THROW(auto val = *e, std::out_of_range);
		try { auto val = *e; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot dereference out of range array iterator");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}
	}

	{	// ++ and --
		EXPECT_THROW(e++, std::out_of_range);
		try { e++; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot increment array iterator past end");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}
		EXPECT_THROW(b--, std::out_of_range);
		try { b--; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot decrement array iterator before begin");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}
	}

	{	// += and -=
		EXPECT_THROW(e += 1, std::out_of_range);
		try { e += 1; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot increment array iterator past end");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}
		EXPECT_THROW(b -= 1, std::out_of_range);
		try { b -= 1; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot decrement array iterator before begin");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}
	}

	{	// + and -
		EXPECT_THROW(auto itr = e + 1, std::out_of_range);
		try { auto itr = e + 1; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot increment array iterator past end");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}
		EXPECT_THROW(auto itr = b - 1, std::out_of_range);
		try { auto itr = b - 1; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot decrement array iterator before begin");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}

		EXPECT_THROW(auto itr = b + 10, std::out_of_range);
		try { auto itr = b + 10; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot increment array iterator past end");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}
		EXPECT_THROW(auto itr = e - 11, std::out_of_range);
		try { auto itr = e - 11; }
		catch (std::out_of_range& o)
		{
			EXPECT_STREQ(o.what(), "cannot decrement array iterator before begin");
		}
		catch (...)
		{
			ADD_FAILURE() << "unexpected exception";
		}
	}
}


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

