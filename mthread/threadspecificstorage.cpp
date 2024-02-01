#include "pch.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <cassert>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::unique_lock;


namespace Example1
{
	int g_var = 0;
	mutex	g_mutex;

	thread_local	int l_var = 0;

	void increment()
	{
		// lock
		lock_guard<mutex>	lg(g_mutex);
		g_var++;
		l_var++;

		stringstream ss;

		ss << "Thread ID: " << std::this_thread::get_id() << ", "
			<< "Global var: " << g_var << ", "
			<< "Local Var: " << l_var;
		cout << ss.str() << endl;
	}

	void run_threads(int n)
	{
		vector<thread>	threads;

		for (auto i = 0; i < n; i++)
		{
			threads.emplace_back(increment);
		}

		for (auto &t : threads)
		{
			t.join();
		}
	}

	int Run_ThreadSpecificStorage()
	{
		// run 10 threads
		run_threads(10);

		cout << "Final value of g_var = " << g_var << endl;

		return 0;
	}

}

int Run_ThreadSpecificStorage()
{
	Example1::Run_ThreadSpecificStorage();

	return 0;
}
