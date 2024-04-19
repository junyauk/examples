#pragma once
#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::queue;
using std::function;
using std::mutex;
using std::unique_lock;
using std::thread;
using std::this_thread::sleep_for;
using cv = std::condition_variable;
using std::chrono::seconds;
using std::chrono::milliseconds;

namespace ThreadPool::Basic1
{
	constexpr int MAX_QUEUE_SIZE = 20;

	class ThreadPool
	{
	public:
		ThreadPool(int numThreads)
			: m_stop(false)
		{
			for (auto i = 0; i < numThreads; ++i)
			{
				m_threads.emplace_back(std::move(thread(&ThreadPool::processTask, this)));
			}
		}

		~ThreadPool()
		{
			stop();
			m_cv.notify_all(); // Let threads know they needs to exit
			for (auto& t : m_threads)
			{
				if (t.joinable())
				{
					t.join();
				}
			}
		}

		void enqueueTask(function<void()> task)
		{
			{
				unique_lock<mutex> ul(m_mutex);
				m_cv.wait(ul, [this] {return m_stop || m_tasks.size() < MAX_QUEUE_SIZE; });
				if (m_stop)
				{
					return;
				}
				m_tasks.push(task);
			}
			m_cv.notify_one(); // Let threads know one task was pushed.
		}
		void stop()
		{
			unique_lock<mutex> ul(m_mutex);
			m_stop = true;
		}

	private:
		void processTask()
		{
			while (true)
			{
				function<void()> task;
				{
					unique_lock<mutex> ul(m_mutex);
					m_cv.wait(ul, [this] {return m_stop || !m_tasks.empty(); });
					if (m_stop)
					{
						return;
					}
					task = m_tasks.front();
					m_tasks.pop();
				}
				m_cv.notify_all(); // let other threads know a task was popped.
				task();
			}
		}

		bool m_stop;
		mutex m_mutex;
		cv m_cv;
		vector<thread>	m_threads;
		queue < function<void()>> m_tasks;
	};

	class Tests
	{
	public:
		int run();
	};

}

namespace ThreadPool::ProducerConsumer
{
	// Define the same one as Basic1

	constexpr int MAX_QUEUE_SIZE = 20;

	class ThreadPool
	{
	public:
		ThreadPool(int const numThreads)
		{
			for (auto i = 0; i < numThreads; ++i)
			{
				m_threads.emplace_back(&ThreadPool::threadFunc, this);
			}
		}

		~ThreadPool()
		{
			stop();
			m_cv.notify_all();

			for (auto& t : m_threads)
			{
				if (t.joinable())
				{
					t.join();
				}
			}
		}

		template<class T>
		void enqueueTask(T&& task)
		{
			{
				unique_lock<mutex> ul(m_mutex);
				m_cv.wait(ul, [this] {return (m_exit || m_queue.size() < MAX_QUEUE_SIZE); });
				if (m_exit)
				{
					return;
				}
				m_queue.push(task);
			}
			m_cv.notify_one();
		}
		void stop()
		{
			unique_lock<mutex> ul(m_mutex);
			m_exit = true;
		}

	private:

		void threadFunc()
		{
			while (true)
			{
				function<void()> func;
				{
					unique_lock<mutex> ul(m_mutex);
					m_cv.wait(ul, [this] {return !m_queue.empty() || m_exit; });
					if (m_exit)
					{
						return;
					}
					func = m_queue.front();
					m_queue.pop();
				}
				m_cv.notify_all();
				func();
			}
		}

		mutex m_mutex;
		cv m_cv;
		bool m_exit{ false };
		queue< function<void()>> m_queue;
		vector<thread> m_threads;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace ThreadPool::Strategy
{
	class ExecutionStrategy
	{
	public:
		virtual void execute(function<void()> task) = 0;
		virtual ~ExecutionStrategy() = default;
	};

	class SingleThreadExecution : public ExecutionStrategy
	{
	public:
		virtual void execute(function<void()> task) override
		{
			task();
		}
	};

	constexpr int MAX_QUEUE_SIZE = 20;

	class MultiThreadExecution : public ExecutionStrategy
	{
	public:
		MultiThreadExecution()
		{
			for (auto i = 0; i < MAX_QUEUE_SIZE; ++i)
			{
				m_threads.emplace_back(&MultiThreadExecution::threadFunc, this);
			}
		}
		~MultiThreadExecution()
		{
			stop();
			m_cv.notify_all();

			for (auto& t : m_threads)
			{
				if (t.joinable())
				{
					t.join();
				}
			}
		}
		virtual void execute(function<void()> task) override
		{
			{
				unique_lock<mutex> ul(m_mutex);
				m_tasks.emplace(std::move(task));
			}
			m_cv.notify_one();
		}

		void stop()
		{
			unique_lock<mutex> ul(m_mutex);
			m_exit = true;
		}


	private:
		void threadFunc()
		{
			while (true)
			{
				function<void()> task;
				{
					unique_lock<mutex> ul(m_mutex);
					m_cv.wait(ul, [this] {return m_exit || !m_tasks.empty(); });
					if (m_exit && m_tasks.empty())
					{
						return;
					}
					task = m_tasks.front();
					m_tasks.pop();
				}
				m_cv.notify_all();
				task();
			}
		}

		bool m_exit{ false };
		mutex m_mutex;
		cv m_cv;
		queue<function<void()>> m_tasks;
		vector<thread> m_threads;
	};

	class Tests
	{
	public:
		int run();
	};
}


#endif // THREADPOOL_H_INCLUDED

