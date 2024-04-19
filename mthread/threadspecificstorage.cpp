#include "pch.h"
#include "threadspecificstorage.h"

#include <chrono>


using std::lock_guard;
using std::unique_lock;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;

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

namespace ThreadSpecificStorage::FactoryMethod::Basic1
{
	// Static members
	mutex SafeOut::m_mutex;

	void threadFunc(ThreadLocal& thl, int id)
	{
		unique_ptr<MyObject> obj = thl.get() ? std::move(thl.get()->clone()) : MyObjectFactory::createObject(id);
		thl.set(std::move(obj));
		thl.get()->print();
	}

	int Tests::run()
	{
		constexpr int NUM_THREADS = 5;
		thread threads[NUM_THREADS];
		ThreadLocal thl;

		for (auto i = 0; i < NUM_THREADS; ++i)
		{
			threads[i] = thread(threadFunc, std::ref(thl), i+1);
		}

		sleep_for(seconds(1));

		for (auto& t : threads)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
		return 0;
	}


}


namespace ThreadSpecificStorage::FactoryMethod::Basic2
{
	void threadFunc(ThreadLocal& thl, int id)
	{
		unique_ptr<Logger> logger = thl.get() ? std::move(thl.get()->clone()) : LoggerFactory::createLogger(id);
		thl.set(std::move(logger));

		Logger* loggerPtr = thl.get();
		if (loggerPtr)
		{
			loggerPtr->log("Thread " + std::to_string(id) + " is logging.");
		}
	}

	int Tests::run()
	{
		constexpr int NUM_THREADS = 5;
		thread threads[NUM_THREADS];
		ThreadLocal thl;

		// Create and start threads
		for (auto i = 0; i < NUM_THREADS; ++i)
		{
			threads[i] = thread(threadFunc, std::ref(thl), i + 1);
		}

		for (auto& t : threads)
		{
			if (t.joinable())
			{
				t.join();
			}
		}

		return 0;
	}

}

