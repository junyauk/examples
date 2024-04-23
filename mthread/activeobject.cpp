#include "pch.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <chrono>

#include "activeobject.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::thread;
using std::packaged_task;
using std::queue;
using std::mutex;
using std::unique_lock;
using std::lock_guard;
using c_v = std::condition_variable;
using std::function;
using std::future;
using std::promise;
using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::chrono::milliseconds;

// About Active OBject design pattern
// Active Object class has got a queue of messages (or tasks).
// The class will start a single thread at constructor,
// and the thread runs until destructor.
// The thread will process messages (or tasks) in the queue.
// The class should have a method for adding a message (or task).


namespace ActiveObject::Example1
{
	// a constructor that creates and starts a worker thread
	ActiveObject::ActiveObject()
	{
		m_worker = thread(&ActiveObject::run, this);
	}
	// a destructor that stops the worker thread
	ActiveObject::~ActiveObject()
	{
		enqueue([this]() 
			{
				m_done = true;
			});
		m_worker.join();
	}

	// a method that performs some work
	void ActiveObject::doWork(int x)
	{
		cout << "Doing work with " << x << endl;
		sleep_for(seconds(1)); // simulate some delay
		cout << "Done work with " << x << endl;
	}

	// a method that enqueue a request to the scheduler
	void ActiveObject::enqueueWork(int x)
	{
		// send a work message
		enqueue([this, x]()
			{
				doWork(x);
			});
	}

	// a method that enqueue a message to the queue
	void ActiveObject::enqueue(function<void()> msg)
	{
		unique_lock<mutex>	ul(m_mutex);
		m_messages.push(msg);
		m_cv.notify_one();
	}
		
	// a method that dequeues a message from the queue
	function<void()> ActiveObject::dequeue()
	{
		unique_lock<mutex>	ul(m_mutex);
		m_cv.wait(ul, [this]
			{
				return !m_messages.empty();
			});
		function<void()> msg = m_messages.front();
		m_messages.pop();
		return msg;
	}

	// a method that runs the scheduler loop
	void ActiveObject::run()
	{
		while (!m_done)
		{
			function<void()>	msg = dequeue();// get the next message
			msg();	// execute the message
		}
	}
}

namespace ActiveObject::Example2
{
	ActiveObject::ActiveObject()
		: m_done(false)
	{
		// create a thread
		m_worker = thread(&ActiveObject::run, this);
	}
	ActiveObject::~ActiveObject()
	{
		// set a termination task
		enqueue([this]() {m_done = true; });
		// wait for the thread
		m_worker.join();
	}

	int ActiveObject::compute(int x)
	{
		cout << "Compute with " << x << endl;
		sleep_for(seconds(1));
		cout << "Done computing with " << x << endl;
		return x * x;
	}

	future<int>	ActiveObject::enqueueCompute(int x)
	{
		// create a promise to store the result
		shared_ptr<promise<int>> p = make_shared<promise<int>>();
		// get the future from the promise
		future<int> f = p->get_future();
		// enqueue a message that bind the compute method with the promise and the argument
		enqueue([this, p, x]()
			{
				p->set_value(compute(x));
			});
		// return the future
		return f;
	}

	void ActiveObject::enqueue(function<void()> msg)
	{
		unique_lock<mutex>	ul(m_mutex);
		m_messages.push(msg);
		m_cv.notify_one();
	}
		
	function<void()> ActiveObject::dequeue()
	{
		unique_lock<mutex>	ul(m_mutex);
		m_cv.wait(ul, [this]()
			{
				return !m_messages.empty();
			});
		function<void()> msg = m_messages.front();
		m_messages.pop();
		return msg;
	}

	// this is the thread that processes tasks in the queue of Active Object.
	void ActiveObject::run()
	{
		while (!m_done)
		{
			// pick up a task from the queue and run it
			function<void()> msg = dequeue();
			msg();
		}
	}

} // Example2

namespace ActiveObject::Example3
{
	ActiveObject::ActiveObject()
		: m_stop(false)
		, m_thread(thread(&ActiveObject::run, this)) // Start a single thread
	{}

	ActiveObject::~ActiveObject()
	{
		{
			unique_lock<mutex>	ul(m_mutex);
			m_stop = true;
			m_cv.notify_all();
		}
		m_thread.join();
	}

	void ActiveObject::enqueue(function<void()> func)
	{
		unique_lock<mutex>	ul(m_mutex);
		m_queue.push(func);
		m_cv.notify_all();
	}

	void ActiveObject::run()
	{
		// This thread works as scheduler
		// a thread should have an infinite loop
		while (true)
		{
			function<void()>	func;
			{
				// 1. lock
				unique_lock<mutex>	ul(m_mutex);
				// 2. wait for a message is queued or termination
				m_cv.wait(ul, [this]() {return !m_queue.empty() || m_stop; });
				if (m_stop)
				{
					break; // terminated
				}
				// 3. get the message (or task)
				func = m_queue.front();
				m_queue.pop();
			}
			// 4. run the message (or task)
			func();
		}
	}

} // Example3

namespace ActiveObject::Basic1
{
	ActiveObject::ActiveObject()
		: m_stop(false)
	{
		m_thread = thread(&ActiveObject::worker, this);
	}
	ActiveObject::~ActiveObject()
	{
		{
			unique_lock<mutex>	ul(m_mutex);
			m_stop = true;
			m_cv.notify_all();
		}
		m_thread.join();
	}

	void ActiveObject::run(function<void()>& task)
	{
		unique_lock<mutex>	ul(m_mutex);
		m_tasks.push(task);
		m_cv.notify_one();
	}
	void ActiveObject::worker()
	{
		while (!m_stop)
		{
			function<void()> task;
			unique_lock<mutex>	ul(m_mutex);
			m_cv.wait(ul, [this] { return !m_tasks.empty() || m_stop; });
			if (m_stop)
			{
				break;
			}
			task = m_tasks.front();
			m_tasks.pop();
			task();
		}
	}

	// Any function
	void func1()
	{
		cout << "Start running void func1()\n";
		sleep_for(seconds(1));
		cout << "Finish running void func1()\n";
	}

	auto lambda = []() -> void
		{
			cout << "Start running lambda\n";
			sleep_for(seconds(1));
			cout << "Finish running lambda\n";
		};

	void func2(string const& str)
	{
		cout << "Start running func2( " << str << ")\n";
		sleep_for(seconds(1));
		cout << "Finish running func2( " << str << ")\n";
	}

	int Tests::run()
	{
		{
			ActiveObject ao;
			function<void()> f;
			f = func1;
			ao.run(f);
			f = lambda;
			ao.run(f);
			f = std::bind(func2, "parameter");
			ao.run(f);
			sleep_for(seconds(5));

			f = std::bind(&Tests::dummy_non_static_public_func, this, "parameter");
			ao.run(f);
			f = std::bind(&Tests::dummy_non_static_private_func, this, "parameter");
			ao.run(f);
			sleep_for(seconds(5));

			f = std::bind(dummy_static_public_func, "parameter");
			ao.run(f);
			f = std::bind(dummy_static_private_func, "parameter");
			ao.run(f);
			sleep_for(seconds(5));

		} // Expect calling ActiveObject's destructor here
		return 0;
	}

	void Tests::dummy_static_public_func(string const& str)
	{
		cout << "Start running dummy_static_public_func( " << str << ")\n";
		sleep_for(seconds(1));
		cout << "Finish running dummy_static_public_func( " << str << ")\n";
	}
	void Tests::dummy_non_static_public_func(string const& str) const
	{
		cout << "Start running dummy_non_static_public_func( " << str << ")\n";
		sleep_for(seconds(1));
		cout << "Finish running dummy_non_static_public_func( " << str << ")\n";
	}

	void Tests::dummy_static_private_func(string const& str)
	{
		cout << "Start running dummy_static_private_func( " << str << ")\n";
		sleep_for(seconds(1));
		cout << "Finish running dummy_static_private_func( " << str << ")\n";
	}
	void Tests::dummy_non_static_private_func(string const& str) const
	{
		cout << "Start running dummy_non_static_private_func( " << str << ")\n";
		sleep_for(seconds(3));
		cout << "Finish running dummy_non_static_private_func( " << str << ")\n";
	}
}

namespace ActiveObject::Basic2
{
	ActiveObject::ActiveObject()
		: m_stop(false)
	{
		m_thread = thread(&ActiveObject::worker, this);
	}
		
	ActiveObject::~ActiveObject()
	{
		{
			unique_lock<mutex>	ul(m_mutex);
			m_stop = true;
			m_cv.notify_all();
		}
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

	void ActiveObject::worker()
	{
		while (!m_stop)
		{
			function<void()> task;
			{
				unique_lock<mutex>	ul(m_mutex);
				m_cv.wait(ul, [this] {return !m_tasks.empty() || m_stop; });
				if (m_stop)
				{
					return;
				}
				task = m_tasks.front();
				m_tasks.pop();
			}
			task();
		}
	}


	int add(const int a, const int b)
	{
		return a + b;
	}

	string print(string const &str1, int a)
	{
		return "Combined: " + str1 + "_" + std::to_string(a);
	}


	int Tests::run()
	{
		{ // Lambda
			ActiveObject	ao;
			auto f = ao.run([] {return 42; });
			auto ret = f.get();
			if (ret != 42)
			{
				return -1;
			}
			cout << "Result: " << ret << endl;
		}
		{ // function (int, int)
			ActiveObject ao;
			auto f = ao.run(add, 10, 20);
			auto ret = f.get();
			if (ret != 30)
			{
				return -1;
			}
			cout << "Result: " << ret << endl;
		}
		{ // function (str, int)
			ActiveObject ao;
			auto f = ao.run(print, "Print", 20);
			auto ret = f.get();
			if (ret != "Combined: Print_20")
			{
				return -1;
			}
			cout << "Result: " << ret << endl;
		}
		return 0;
	}
}
