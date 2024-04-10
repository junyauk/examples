#pragma once
#ifndef READERWRITER_H_INCLUDED
#define READERWRITER_H_INCLUDED

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
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::weak_ptr;

namespace ReaderWriter::Basic1
{
	class Data
	{
	public:
		Data() : m_data(0) {}

		int read()
		{
			shared_lock<shared_mutex> ul(m_mutex);
			return m_data;
		}

		void write(int value)
		{
			unique_lock<shared_mutex> ul(m_mutex);
			m_data = value;
		}

	private:
		int m_data;
		mutable shared_mutex m_mutex;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace ReaderWriter::Basic1Step2
{
	class Data
	{
#if 0 // using semaphore for maximizing concurrency, not data consistency
	public:
		Data() : m_data(0), m_r_semaphore(3), m_w_semaphore(1) {}
		int read()
		{
			m_r_semaphore.acquire();
			int value = m_data;
			m_r_semaphore.release();
			return value;
		}
		void write(int data)
		{
			m_w_semaphore.acquire();
			m_data = data;
			m_w_semaphore.release();
		}
	private:
		int m_data;
		counting_semaphore<3>	m_r_semaphore;
		counting_semaphore<1>	m_w_semaphore;
#else // using mutex and condition_variable for maximizing data consistency, not concurrency
	public:
		Data() : m_data(0), m_readerCount(0) {}
		int read()
		{
			unique_lock<mutex> ul(m_mutex);
			m_cv.wait(ul, [this] {return m_readerCount < 3; });
			++m_readerCount;
			int value = m_data;
			--m_readerCount;
			return value;
		}
		void write(int data)
		{
			unique_lock<mutex> ul(m_mutex);
			m_cv.wait(ul, [this] {return m_readerCount == 0; });// make sure no reader threads when writing
			m_data = data;
			m_cv.notify_all();
		}
	private:
		int m_data;
		mutex	m_mutex;
		cv m_cv;
		int m_readerCount;
#endif
	};

	class Tests
	{
	public:
		int run();
	};
}

#endif // READERWRITER_H_INCLUDED