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


#endif // THREADPOOL_H_INCLUDED

