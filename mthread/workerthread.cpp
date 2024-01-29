#include "pch.h"
#include "workerthread.h"

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <cassert>
#include <chrono>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::queue;
using std::deque;
using std::thread;
using std::mutex;
using std::unique_lock;
using c_v = std::condition_variable;
using std::shared_ptr;
using std::make_shared;
using std::function;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

namespace Example1
{
	template <typename T>
	class Queue
	{
		// Queue has a STL's dequeue.
		// This will keep functions.
	public:
		Queue(int size)
			: m_size(size)
		{}
		bool put(T&& data)
		{
			if (m_size <= m_dequeue.size())
			{
				return false;
			}
			m_dequeue.emplace_back(std::move(data));
			return true;
		}
		bool put(const T& data)
		{
			if (m_size <= m_dequeue.size())
			{
				return false;
			}
			m_dequeue.emplace_back(data);
			return true;
		}
		bool get(T& data)
		{
			if (m_dequeue.empty())
			{
				return false;
			}
			data = std::move(m_dequeue.front());
			m_dequeue.pop_front();
			return true;
		}
		bool empty()
		{
			return m_dequeue.empty();
		}

	private:
		int m_size;
		deque<T> m_dequeue;
	};

	class Runnable
	{
		// This class makes sure the class inheriting has got the run method.
	public:
		virtual void run() = 0;
	};

	class ThreadPool
	{
		// ThreadPool runs multiple threads at constructor.
		// The created thread will be terminated at destructor.
		// ThreadPool has got a queue of functions.
		// Functions are added via add() method of ThreadPool,
		// and added to the queue.
		// The multiple created threads checks the queue.
		// Pick up one function from the queue, and run it.

	public:
		ThreadPool(int threadCount, int queueSize)
			: m_isTerminationRequired(false)
			, m_queue(queueSize)
		{
			for (auto i = 0; i < threadCount; i++)
			{
				m_threads.emplace_back(thread(m_main));
			}
		}

		~ThreadPool()
		{
			{
				unique_lock<mutex>	ul(m_mutex);
				m_isTerminationRequired = true;
			}
			m_cv.notify_all();
			auto size = m_threads.size();
			for (auto i = 0; i < size; i++)
			{
				m_threads[i].join();
			}
		}

		bool add(shared_ptr<Runnable>&& runnable)
		{
			{
				unique_lock<mutex>	ul(m_mutex);
				if (!m_queue.put(std::move(runnable)))
				{
					return false;
				}
			}
			m_cv.notify_all();
			return true;
		}

		bool add(const shared_ptr<Runnable>& runnable)
		{
			{
				unique_lock<mutex>	ul(m_mutex);
				if (!m_queue.put(runnable))
				{
					return false;
				}
			}
			m_cv.notify_all();
			return true;
		}

	private:
		function<void()> m_main = [this]()
			{
				while (1)
				{
					shared_ptr<Runnable>	runnable;
					{
						unique_lock<mutex>	ul(m_mutex);
						while (m_queue.empty())
						{
							if (m_isTerminationRequired)
							{
								return;
							}
							m_cv.wait(ul);
						}
						const bool result = m_queue.get(runnable);
						assert(result);
					}
					runnable->run();
				}
			};

		bool m_isTerminationRequired;
		Queue<shared_ptr<Runnable>> m_queue;
		mutex	m_mutex;
		c_v	m_cv;
		vector<thread> m_threads;
	};

	class WorkerA : public Runnable
	{
	public:
		void run() final
		{
			cout << "Worker A" << endl;
		}
	};

	class WorkerB : public Runnable
	{
	public:
		WorkerB(int value)
			: m_value(value)
		{}

		void run() final
		{
			cout << "Worker B value= " << m_value << endl;
		}
	private:
		int m_value;
	};


	void Run_WorkerThread()
	{
		ThreadPool	tp(5, 1000);

		for (auto i = 0; i < 1000; i++)
		{
			while (!tp.add(make_shared<WorkerA>()))
			{
				sleep_for(milliseconds(1));
			}

			while (!tp.add(make_shared<WorkerB>(i)))
			{
				sleep_for(milliseconds(1));
			}
		}
		return;
	}

}

namespace Example2
{
	// Simpler than Example1
	class ThreadPool
	{
	public:
		ThreadPool( int numThreads, int queueSize)
			: m_isTerminationResuested(false)
			, m_maxQueueSize(queueSize)
		{
			// create threads that pick up a function from queue and run it
			for (auto i = 0; i < numThreads; i++)
			{
				m_threads.emplace_back(thread(m_main));
			}
		}
		~ThreadPool()
		{
			// 1. lock first
			{
				unique_lock<mutex>	ul(m_mutex);
				// 2. Set termination flag
				m_isTerminationResuested = true;
			}
			// 3. let threads know terminating
			m_cv.notify_all();
			// 4. terminating threads
			auto size = m_threads.size();
			for (auto i = 0; i < size; i++)
			{
				m_threads[i].join();
			}
		}

		bool add(function<void()>&& func)
		{
			// 1. lock first
			unique_lock<mutex>	ul(m_mutex);
			// 2. check the queue size
			if (m_maxQueueSize <= m_queue.size())
			{
				return false;
			}
			// 3. push the func
			m_queue.emplace(std::move(func)); // func is rref, so needs to be moved
			// 4. finally notify
			m_cv.notify_all();
			return true;
		}

		bool add(const function<void()>& func)
		{
			// 1. lock first
			unique_lock<mutex>	ul(m_mutex);
			// 2. check the queue size
			if (m_maxQueueSize <= m_queue.size())
			{
				return false;
			}
			// 3. push the func
			m_queue.emplace(func);
			// 4. finally notify
			m_cv.notify_all();
			return true;
		}

	private:
		function<void()>	m_main = [this]()
			{
				// Need to have an infinite loop
				while (1)
				{
					function<void()>	func;
					{
						unique_lock<mutex>	ul(m_mutex);
						if (m_queue.empty())
						{
							// All queued funcs have been processed
							if (m_isTerminationResuested)
							{
								// exit when termination was requested only
								return;
							}
							m_cv.wait(ul);
						}
						// make sure again if it's not empty
						// now ready to pop a function
						if (!m_queue.empty())
						{
							func = m_queue.front();
							m_queue.pop();
						}
						else
							continue;
					}
					// call the function
					func();
				}
			};

		unsigned int m_maxQueueSize;
		queue<function<void()>>	m_queue;
		bool	m_isTerminationResuested;
		c_v	m_cv;
		mutex m_mutex;
		vector<thread>	m_threads;	// Need to be locate at the end, to make sure threads are initialised last
	};

	static void Run_WorkerThread()
	{
		ThreadPool	tp(5, 1000);

		for (auto i = 0; i < 1000; i++)
		{
			// Create function objects
			auto A = [&]() { cout << "Func A" << endl; return; };
			auto B = [&]() { cout << "Func B " << "val= " << i << endl; return; };
			while (!tp.add(A))
			{
				sleep_for(milliseconds(1));
			}

			while (!tp.add(B))
			{
				sleep_for(milliseconds(1));
			}
		}
		return;
	}
}


int Run_WorkerThread()
{
	Example1::Run_WorkerThread();
	Example2::Run_WorkerThread();

	return 0;
}
