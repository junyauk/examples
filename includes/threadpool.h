#pragma once
#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED
#include "Windows.h"

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>

#include "tests.h"

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
		ThreadPool(int numThreads);
		~ThreadPool();

		void enqueueTask(function<void()> task);
		void stop();

	private:
		void processTask();

		bool m_stop;
		mutex m_mutex;
		cv m_cv;
		vector<thread>	m_threads;
		queue < function<void()>> m_tasks;
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
		MultiThreadExecution();
		~MultiThreadExecution();
		virtual void execute(function<void()> task) override;
		void stop();
	private:
		void threadFunc();

		bool m_exit{ false };
		mutex m_mutex;
		cv m_cv;
		queue<function<void()>> m_tasks;
		vector<thread> m_threads;
	};
}

namespace ThreadPool::Windows1
{
	VOID CALLBACK PrintEvenNumbers(
		PTP_CALLBACK_INSTANCE instance,
		PVOID context,
		PTP_WORK work);

	VOID CALLBACK PrintOddNumbers(
		PTP_CALLBACK_INSTANCE instance,
		PVOID context,
		PTP_WORK work);
}

#endif // THREADPOOL_H_INCLUDED

