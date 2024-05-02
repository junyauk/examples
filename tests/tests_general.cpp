#include "pch.h"

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <sstream>
#include <utility>

#include "templates.h"
#include "typeerasure.h"
#include "ringbuffer.h"
#include "crtp.h"
#include "sharedpinter.h"

#include "hackerrank.h"

#include "general.h"

#include "tests_plugins.h"
#include "tests_pluginmanager.h"

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::function;
using std::stringstream;
using std::pair;

TEST(General, General)
{
	general();
}

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

TEST(General, CircularIndex)
{
	CircularIndex	ci1{ 5 };
	CircularIndex	ci2{ 14 };

	// operator =
	ci1 = 0;
	EXPECT_EQ(ci1, 0);
	ci1 = 5;
	EXPECT_EQ(ci1, 0);
	ci1 = 14;
	EXPECT_EQ(ci1, 4);
	ci1 = -3;		// Note: size_t is unsigned, so -3 will be treated as 18446744073709551613.
	ci2 = -3;
	EXPECT_EQ(ci1, 3);
	size_t val = -1;// Note: -1 will be treated as max size 18446744073709551615
	ci1 = val;
	ci2 = val;
	EXPECT_EQ(ci1, 0);
	EXPECT_EQ(ci2, 1);
	ci1 = 7;
	ci2 = 8;

	// Note:
	// Copy constructor and operator= is deleted
	// ci1 = ci2;

	// ++ and --
	ci1 = 0; ci2 = 0;
	for (auto i = 0; i < 100; ++i)
	{
		EXPECT_EQ(ci1, i % 5);
		EXPECT_EQ(ci2, i % 14);
		++ci1;
		++ci2;
	}
	ci1 = 99; ci2 = 99;
	for (auto i = 99; i >=  0; --i)
	{
		EXPECT_EQ(ci1, i % 5);
		EXPECT_EQ(ci2, i % 14);
		--ci1;
		--ci2;
	}
	// ++(int) and --(int)
	ci1 = 0; ci2 = 0;
	for (auto i = 0; i < 100; ++i)
	{
		EXPECT_EQ(ci1, i % 5);
		EXPECT_EQ(ci2, i % 14);
		ci1++;
		ci2++;
	}
	ci1 = 99; ci2 = 99;
	for (auto i = 99; i >= 0; --i)
	{
		EXPECT_EQ(ci1, i % 5);
		EXPECT_EQ(ci2, i % 14);
		ci1--;
		ci2--;
	}
	ci1 = 0; ci2 = 0;
	for (auto i = 0; i < 100; ++i)
	{
		int expected1 = ci1;
		EXPECT_EQ(expected1, ci1++);
		int expected2 = ci2;
		EXPECT_EQ(expected2, ci2--);
	}

	// + and -
	ci1 = 4;
	size_t tmp;
	tmp = ci1 + 1;
	EXPECT_EQ(tmp, 0);
	tmp = ci1 + 13;
	EXPECT_EQ(tmp, 2);
	tmp = ci1 - 6;
	EXPECT_EQ(tmp, 3);
	tmp = ci1 - 23;
	EXPECT_EQ(tmp, 1);

	// += and -=
	ci1 = 0;
	ci1 += 3;
	EXPECT_EQ(ci1, 3);
	ci1 += 11;
	EXPECT_EQ(ci1, 4);
	ci1 += 26;
	EXPECT_EQ(ci1, 0);
	ci2 = 0;
	ci2 -= 1;
	EXPECT_EQ(ci2, 13);
	ci2 -= 10;
	EXPECT_EQ(ci2, 3);
	ci2 -= 29;
	EXPECT_EQ(ci2, 2);

	// == and !=
	ci1 = 3;
	ci2 = 3;
	EXPECT_TRUE(ci1 == ci2);
	ci1 = 1;
	EXPECT_TRUE(ci1 != ci2);

	// =
	ci1 = 2;
	int val2 = ci1;
	EXPECT_EQ(val2, 2);

	// <<
	ci2 = 12;
	string str = ci2;
	EXPECT_STREQ(str.c_str(), "12");

	stringstream ss;
	ss << ci2;
	EXPECT_STREQ(ss.str().c_str(), "12");


}


TEST(General, RingBuffer)
{
	{ // int ring buffer
		constexpr int bufSize = 10;
		RingBuffer<int, 10>	intBuf;
		EXPECT_TRUE(intBuf.isEmpty());
		EXPECT_FALSE(intBuf.isFull());
		for (auto i = 0; i < bufSize; ++i)
		{
			intBuf.push(i);
		}
		EXPECT_EQ(intBuf.getSize(), bufSize);
		EXPECT_FALSE(intBuf.isEmpty());
		EXPECT_TRUE(intBuf.isFull());
		EXPECT_THROW(intBuf.push(100), std::overflow_error);
		for (auto i = 0; i < bufSize; ++i)
		{
			EXPECT_EQ(intBuf.pop(), i);
		}
		EXPECT_TRUE(intBuf.isEmpty());
		EXPECT_FALSE(intBuf.isFull());
		EXPECT_THROW(intBuf.pop(), std::underflow_error);
	}

	{ // string ring buffer
		constexpr int bufSize = 7;
		RingBuffer<string, bufSize>	strBuf;
		EXPECT_TRUE(strBuf.isEmpty());
		EXPECT_FALSE(strBuf.isFull());
		for (auto i = 0; i < bufSize; ++i)
		{
			strBuf.push(std::to_string(i));
		}
		EXPECT_EQ(strBuf.getSize(), bufSize);
		EXPECT_FALSE(strBuf.isEmpty());
		EXPECT_TRUE(strBuf.isFull());
		EXPECT_THROW(strBuf.push("10"), std::overflow_error);
		for (auto i = 0; i < bufSize; ++i)
		{
			EXPECT_EQ(strBuf.pop(), std::to_string(i));
		}
		EXPECT_TRUE(strBuf.isEmpty());
		EXPECT_FALSE(strBuf.isFull());
		EXPECT_THROW(strBuf.pop(), std::underflow_error);
	}
}


TEST(General, SharedPointerBasic1)
{
	using namespace SharedPointer::Basic1;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(General, SharedPointerBasic2)
{
	using namespace SharedPointer::Basic2;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(General, TemplateConcept1)
{
	using namespace TemplateExamples::Concept1;

	string result;
	{
		X x1(10, 5.5f, std::string("Hello")); // No explicit template arguments
		testing::internal::CaptureStdout();
		// Displaying initialized values
		x1.displayValues();
		result = testing::internal::GetCapturedStdout();
		char expected[] = "Integer values: 10 \nFloat values: 5.5 \nString values: Hello \n";
		EXPECT_STREQ(expected, result.c_str());
	}
	
	{
		X x2("Hello", 10, 5.5f); // No explicit template arguments
		testing::internal::CaptureStdout();
		x2.displayValues();
		result = testing::internal::GetCapturedStdout();
		char expected[] = "Integer values: 10 \nFloat values: 5.5 \nString values: Hello \n";
		EXPECT_STREQ(expected, result.c_str());
	}

	{
		X x3("Hello", 10, "world", 5.5f, 152, 99, 32.9f); // No explicit template arguments
		testing::internal::CaptureStdout();
		x3.displayValues();
		result = testing::internal::GetCapturedStdout();
		char expected[] = "Integer values: 10 152 99 \nFloat values: 5.5 32.9 \nString values: Hello world \n";
		EXPECT_STREQ(expected, result.c_str());
	}

}

TEST(General, HackerrankBitArray)
{
	using namespace Hackerrank::BitArray;

	int ret;
	testing::internal::CaptureStdout();
	ret = BitArray(3, 1, 1, 1);
	string result = testing::internal::GetCapturedStdout();
	char expected[] = "3";
	EXPECT_STREQ(expected, result.c_str());
}

TEST(Plugin, LoadingPluginTests)
{
	{
		using namespace Plugin::LoadingPlugin1Tests;
		Tests tests;
		int ret = 0;
		EXPECT_NO_THROW(ret = tests.run());
		EXPECT_EQ(ret, 0);
	}

	{
		using namespace Plugin::LoadingPlugin2Tests;
		Tests tests;
		int ret = 0;
		EXPECT_NO_THROW(ret = tests.run());
		EXPECT_EQ(ret, 0);
	}
}

TEST(Plugin, PluginManager)
{
	using namespace Plugin;
	Tests tests;
	int ret = 0;
	EXPECT_NO_THROW(ret = tests.run());
	EXPECT_EQ(ret, 0);
}

TEST(MultiProcess, MemoryMappedFile)
{
	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(int),
		L"LOCAL//MemoryMappedFile");
	if (!hMapFile)
	{
		auto lastError = GetLastError();
		cerr << "CreateFileMapping() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		return;
	}

	LPVOID pData = MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(int));
	if (!pData)
	{
		auto lastError = GetLastError();
		cerr << "MapViewOfFile() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		CloseHandle(hMapFile);
		return;
	}
	int* pValue = static_cast<int*>(pData);
	*pValue = 3;

	wstring path = GetRunningPath() + TEXT("\\") + TEXT("ProcessA.exe");

	PROCESS_INFORMATION pi;
	STARTUPINFO si = { sizeof(si) };
	if (!CreateProcess(
		path.c_str(),
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED, // for debugging
		NULL,
		NULL,
		&si,
		&pi))
	{
		auto lastError = GetLastError();
		cerr << "MapViewOfFile() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		UnmapViewOfFile(pData);
		CloseHandle(hMapFile);
		return;
	}
	ResumeThread(pi.hThread); // for debugging. Attach ProcessA before running this.

	WaitForSingleObject(pi.hProcess, INFINITE);

	EXPECT_EQ(*pValue, 123);

	UnmapViewOfFile(pData);
	CloseHandle(hMapFile);
}
