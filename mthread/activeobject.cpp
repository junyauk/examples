#include "pch.h"
#include <iostream>
#include <string>
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

namespace Example1
{
	// a class that represents an active object
	class ActiveObject
	{
	public:
		// a constructor that creates and starts a worker thread
		ActiveObject()
		{
			m_worker = thread(&ActiveObject::run, this);
		}
		// a destructor that stops the worker thread
		~ActiveObject()
		{
			enqueue([this]() 
				{
					m_done = true;
				});
			m_worker.join();
		}

		// a method that performs some work
		void doWork(int x)
		{
			cout << "Doing work with " << x << endl;
			sleep_for(seconds(1)); // simulate some delay
			cout << "Done work with " << x << endl;
		}

		// a method that enqueue a request to the scheduler
		void enqueueWork(int x)
		{
			// send a work message
			enqueue([this, x]()
				{
					doWork(x);
				});
		}

	private:
		// a method that enqueue a message to the queue
		void enqueue(function<void()> msg)
		{
			unique_lock<mutex>	ul(m_mutex);
			m_messages.push(msg);
			m_cv.notify_one();
		}
		
		// a method that dequeues a message from the queue
		function<void()> dequeue()
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
		void run()
		{
			while (!m_done)
			{
				function<void()>	msg = dequeue();// get the next message
				msg();	// execute the message
			}
		}
		
		queue<function<void()>>	m_messages;	// a queue of messages
		mutex	m_mutex;	// a mutex to protect the queue
		c_v	m_cv;	// a condition variable to signal the worker thread
		bool	m_done = false;	// a flag to indicate the active object is done
		thread	m_worker;	// a worker thread that executes the messages
	};

	int Run_ActiveObject()
	{
		// create an active object
		ActiveObject	ao;

		// enqueue some work requests
		ao.enqueueWork(1);
		ao.enqueueWork(2);
		ao.enqueueWork(3);

		// do other things
		cout << "Main thread is doing other things" << endl;
		sleep_for(seconds(5));

		return 0;
	}
}

namespace Example2
{
	// a class that represents active object
	class ActiveObject
	{
	public:
		ActiveObject()
			: m_done(false)
		{
			// create a thread
			m_worker = thread(&ActiveObject::run, this);
		}
		~ActiveObject()
		{
			// set a termination task
			enqueue([this]() {m_done = true; });
			// wait for the thread
			m_worker.join();
		}

		int compute(int x)
		{
			cout << "Compute with " << x << endl;
			sleep_for(seconds(1));
			cout << "Done computing with " << x << endl;
			return x * x;
		}

		future<int>	enqueueCompute(int x)
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

	private:
		void enqueue(function<void()> msg)
		{
			unique_lock<mutex>	ul(m_mutex);
			m_messages.push(msg);
			m_cv.notify_one();
		}
		
		function<void()> dequeue()
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
		void run()
		{
			while (!m_done)
			{
				// pick up a task from the queue and run it
				function<void()> msg = dequeue();
				msg();
			}
		}

		queue<function<void()>>	m_messages;
		mutex	m_mutex;
		c_v		m_cv;
		bool	m_done;
		thread	m_worker;
	};

	int Run_ActiveObject()
	{
		// create an active object
		ActiveObject	ao;

		// enqueue some compute results and get futures
		future<int>	f1 = ao.enqueueCompute(1);
		future<int>	f2 = ao.enqueueCompute(2);
		future<int>	f3 = ao.enqueueCompute(3);

		// do other things
		cout << "Main thread is doing other things...\n";
		sleep_for(seconds(5));

		// get the results from the futures.
		cout << "Result of f1: " << f1.get() << endl;
		cout << "Result of f2: " << f2.get() << endl;
		cout << "Result of f3: " << f3.get() << endl;

		return 0;
	}
}



int Run_ActiveObject()
{
//	Example1::Run_ActiveObject();
	Example2::Run_ActiveObject();

	return 0;
}
