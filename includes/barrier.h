#pragma once
#ifndef BARRIER_H_INCLUDED
#define BARRIER_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <semaphore>
#include <thread>
#include <chrono>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::mutex;
using std::shared_mutex;
using std::counting_semaphore;
using cv = std::condition_variable;
using std::unique_lock;
using std::shared_lock;
using std::thread;
using std::this_thread::sleep_for;
using std::this_thread::get_id;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::weak_ptr;

namespace Barrier::Basic1
{
	class Barrier
	{
	public:
		explicit Barrier(size_t numThreads) : m_count(numThreads), m_reached(0) {}
		void wait()
		{
			unique_lock<mutex> ul(m_mutex);
			if (++m_reached < m_count)
			{
				m_cv.wait(ul, [this] {return m_reached >= m_count; });
			}
			else
			{
				m_cv.notify_all();
			}
		}

	private:
		size_t m_count;
		size_t m_reached;
		mutex m_mutex;
		cv m_cv;
	};

	class Tests
	{
	public:
		int run();
	};

}

namespace Barrier::Basic2
{
	class Barrier
	{
	public:
		explicit Barrier(size_t numWorkers): m_count(numWorkers), m_reached(0) {}

		void wait()
		{
			unique_lock<mutex> ul(m_mutex);
			if (++m_reached < m_count)
			{
				m_cv.wait(ul, [this] {return m_reached >= m_count; });
			}
			else
			{
				m_cv.notify_all();
			}
		}
	private:
		size_t m_count;
		size_t m_reached;
		mutex m_mutex;
		cv m_cv;
	};

	class Worker
	{
	public:
		Worker(int id, shared_ptr<Barrier> barrier) : m_id(id), m_barrier(barrier) {}
		void assembleComponent()
		{
			sleep_for(milliseconds(1000));
			cout << "Worker[" << get_id() << "] has assembled their component.\n";
			m_barrier->wait();
		}
	private:
		int m_id;
		shared_ptr<Barrier>	m_barrier;
	};


	class Tests
	{
	public:
		int run();
	};
}

#endif // BARRIER_H_INCLUDED