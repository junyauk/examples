#pragma once
#ifndef ACTIVEOBJECT_H_INCLUDED
#define ACTIVEOBJECT_H_INCLUDED

#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <future>

using std::string;
using std::queue;
using std::thread;
using std::mutex;
using std::unique_lock;
using std::function;
using c_v = std::condition_variable;
using std::shared_ptr;
using std::make_shared;
using std::packaged_task;
using std::future;

namespace ActiveObject::Example1
{
	// a class that represents an active object
	class ActiveObject
	{
	public:
		// a constructor that creates and starts a worker thread
		ActiveObject();
		// a destructor that stops the worker thread
		~ActiveObject();

		// a method that performs some work
		void doWork(int x);

		// a method that enqueue a request to the scheduler
		void enqueueWork(int x);

	private:
		// a method that enqueue a message to the queue
		void enqueue(function<void()> msg);
		// a method that dequeues a message from the queue
		function<void()> dequeue();
		// a method that runs the scheduler loop
		void run();

		queue<function<void()>>	m_messages;	// a queue of messages
		mutex	m_mutex;	// a mutex to protect the queue
		c_v	m_cv;	// a condition variable to signal the worker thread
		bool	m_done = false;	// a flag to indicate the active object is done
		thread	m_worker;	// a worker thread that executes the messages
	};

}

namespace ActiveObject::Example2
{
	// a class that represents active object
	class ActiveObject
	{
	public:
		ActiveObject();
		~ActiveObject();

		int compute(int x);
		future<int>	enqueueCompute(int x);
	private:
		void enqueue(function<void()> msg);
		function<void()> dequeue();

		// this is the thread that processes tasks in the queue of Active Object.
		void run();

		queue<function<void()>>	m_messages;
		mutex	m_mutex;
		c_v		m_cv;
		bool	m_done;
		thread	m_worker;
	};
}

namespace ActiveObject::Example3
{
	class ActiveObject
	{
	public:
		ActiveObject();
		~ActiveObject();
		void enqueue(function<void()> func);
	private:
		void run();

		queue<function<void()>> m_queue;
		mutex m_mutex;	// Only one thread runs
		c_v	m_cv;		// waiting for something is queued
		bool m_stop;	// a flag for terminating
		thread	m_thread;
	};
}

namespace ActiveObject::Basic1
{
	class ActiveObject
	{
	public:
		ActiveObject();
		~ActiveObject();

		void run(function<void()>& task);
	private:
		void worker();

		queue<function<void()>> m_tasks;
		mutex m_mutex;
		c_v m_cv;
		bool m_stop;
		thread	m_thread;
	};

	class Tests
	{
	public:
		int run();

		static void dummy_static_public_func(string const& str);
		void dummy_non_static_public_func(string const& str) const;

	private:
		static void dummy_static_private_func(string const& str);
		void dummy_non_static_private_func(string const& str) const;
	};
}

namespace ActiveObject::Basic2
{
	// This ActiveObject runs functions with multiple parameters.
	class ActiveObject
	{
	public:
		ActiveObject();
		~ActiveObject();

		template<typename Func, typename... Args>
		auto run(Func func, Args... args) -> future<decltype(func(args...))>
		{
			auto task = make_shared<packaged_task<decltype(func(args...))()>>(std::bind(func, args...));
			future<decltype(func(args...))> f = task->get_future();
			{
				unique_lock<mutex>	ul(m_mutex);
				m_tasks.push([task] {(*task)(); });
			}
			m_cv.notify_one();
			return f;
		}

	private:
		void worker();

		bool	m_stop;
		mutex	m_mutex;
		c_v		m_cv;
		queue<function<void()>>	m_tasks;
		thread	m_thread;
	};

	class Tests
	{
	public:
		int run();
	};
}
#endif // ACTIVEOBJECT_H_INCLUDED