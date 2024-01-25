#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>

using std::cout;
using std::endl;

using std::thread;

using std::atomic;

using std::mutex;
using std::lock_guard;

using std::ref;

int Run_SingleThreadExecution_Atomic()
{
	auto f = [&](atomic<int>& count)
		{
			for (int i = 0; i < 1000000; i++)
			{
				++count;
			}
		};

	atomic<int> count = 0;
	thread ta(f, ref(count));
	thread tb(f, ref(count));

	ta.join();
	tb.join();

	return count.load();
}


int Run_SingleThreadExecution_Mutex()
{
	auto f = [](int& count, mutex& mtx)
		{
			for (int i = 0; i < 1000000; i++)
			{
				lock_guard<mutex> lg(mtx);
				count++;
			}
		};
	
	int count = 0;
	mutex mtx;

	std::thread ta(f, std::ref(count), std::ref(mtx));
	std::thread tb(f, std::ref(count), std::ref(mtx));

	ta.join();
	tb.join();

	return count;
}

int Run_SingleThreadExecution()
{
	int count = 0;

	count = Run_SingleThreadExecution_Atomic();
	cout << count << endl;
	count = Run_SingleThreadExecution_Mutex();
	cout << count << endl;

	return 0;
}