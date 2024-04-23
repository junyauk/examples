#include "pch.h"
#include "Windows.h"

#include <sstream>
#include <memory>
#include "threadpool.h"

using std::stringstream;
using std::unique_ptr;
using std::make_unique;

namespace ThreadPool::Basic1
{
	ThreadPool::ThreadPool(int numThreads)
		: m_stop(false)
	{
		for (auto i = 0; i < numThreads; ++i)
		{
			m_threads.emplace_back(std::move(thread(&ThreadPool::processTask, this)));
		}
	}

	ThreadPool::~ThreadPool()
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

	void ThreadPool::enqueueTask(function<void()> task)
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
	void ThreadPool::stop()
	{
		unique_lock<mutex> ul(m_mutex);
		m_stop = true;
	}

	void ThreadPool::processTask()
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

}

namespace ThreadPool::ProducerConsumer
{
	// The class is defined in threadpool.h
}

namespace ThreadPool::Strategy
{
	MultiThreadExecution::MultiThreadExecution()
	{
		for (auto i = 0; i < MAX_QUEUE_SIZE; ++i)
		{
			m_threads.emplace_back(&MultiThreadExecution::threadFunc, this);
		}
	}
	MultiThreadExecution::~MultiThreadExecution()
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

	void MultiThreadExecution::execute(function<void()> task)
	{
		{
			unique_lock<mutex> ul(m_mutex);
			m_tasks.emplace(std::move(task));
		}
		m_cv.notify_one();
	}

	void MultiThreadExecution::stop()
	{
		unique_lock<mutex> ul(m_mutex);
		m_exit = true;
	}

	void MultiThreadExecution::threadFunc()
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
}

namespace ThreadPool::Windows1
{
	VOID CALLBACK PrintEvenNumbers(
		PTP_CALLBACK_INSTANCE instance,
		PVOID context,
		PTP_WORK work)
	{
		for (auto i = 0; i < 10; i += 2)
		{
			cout << "Even: " << i << endl;
			Sleep(500);
		}
	}

	VOID CALLBACK PrintOddNumbers(
		PTP_CALLBACK_INSTANCE instance,
		PVOID context,
		PTP_WORK work)
	{
		for (auto i = 1; i < 10; i += 2)
		{
			cout << "Odd: " << i << endl;
			Sleep(500);
		}
	}

}


