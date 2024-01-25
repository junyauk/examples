#include "pch.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>

using std::cout;
using std::endl;
using std::string;

using std::thread;
using std::vector;

class ImmutablePoint
{
public:
	ImmutablePoint(int x, int y)
		: m_x(x)
		, m_y(y)
	{}

	int getX() const { return m_x; }
	int getY() const { return m_y; }

	ImmutablePoint add(const ImmutablePoint& other) const
	{
		// Note:
		// This add method doesn't modify m_x and m_y
		// Returns a new instance of this class only.
		return ImmutablePoint(m_x + other.getX(), m_y + other.getY());
	}

private:

	// Immutable class only has got const values initialised at constructor.
	// They will never be modified
	const int m_x;
	const int m_y;
};

void PrintPoint(const ImmutablePoint& p)
{
	cout << "(" << p.getX() << ", " << p.getY() << ")" << endl;
}

int Run_Immutable()
{
	vector<thread>	threads;
	ImmutablePoint p1(1, 2);
	ImmutablePoint p2(3, 4);

	for (auto i = 0; i < 10; i++)
	{
		threads.push_back(thread(PrintPoint, p1.add(p2)));
	}

	for (auto i = 0; i < 10; i++)
	{
		threads[i].join();
	}

	return 0;
}