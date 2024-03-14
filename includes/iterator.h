#pragma once
#ifndef ITERATOR_H_INCLUDED
#define ITERATOR_H_INCLUDED

#include <vector>
#include <iterator>

using std::vector;

namespace Iterator::Basic1
{
	// This example adds only the limited features
	class IIterator
	{
	public:
		virtual ~IIterator() {}
		virtual bool hasNext() const = 0;
		virtual int next() = 0;
	};

	// This class produces the data with the IIterator.
	class IAggregate
	{
	public:
		virtual ~IAggregate() {}
		virtual IIterator* createIterator() = 0;
		virtual void addData(int val) = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Iterator::Basic2
{
	// simple example of customizing the existing iterator.

	// Note:
	// until C++17 -> std::iterator<std::input_iterator_tag, int>
	// since C++20 -> std::iterator_traits<vector<int>::iterator>
	// C++20 removed std::iterator. So std::iterator is no longer available 
	class CustomIterator : public std::iterator_traits<vector<int>::iterator>
	{
	public:
		CustomIterator(vector<int>::iterator it) : m_it(it) {}

		// Dereference operator
		int& operator*() { return *m_it; }

		// Increment operator
		CustomIterator& operator++() { ++m_it; return *this; }

		// Increment operator I++
		CustomIterator operator++(int) { auto t = *this; ++m_it; return t; }

		// Equality operator ==
		bool operator==(CustomIterator const& other) const { return this->m_it == other.m_it; }

		// Inequality operator !=
		bool operator!=(CustomIterator const& other) const { return this->m_it != other.m_it; }

		// and so on

	private:
		vector<int>::iterator m_it;
	};

	class Tests
	{
	public:
		int run();
	};
}


#endif // ITERATOR_H_INCLUDED
