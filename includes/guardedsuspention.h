#pragma once
#ifndef GUARDEDSUSPENTION_H_INCLUDED
#define GUARDEDSUSPENTION_H_INCLUDED

#include <deque>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>
#include <cassert>
#include <sstream>

using std::deque;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::mutex;
using std::unique_lock;
using std::condition_variable;
using std::thread;
using std::this_thread::sleep_for;

namespace GuardesSuspention
{
	template<typename T>
	class Queue
	{
	public:
		Queue(int size) :m_size(size) {}
		bool put(T&& data)
		{
			cout << "Queue.put(T&& data) ->\n";

			if (m_size <= m_deque.size())
			{
				return false;
			}
			m_deque.emplace_back(std::move(data));
			cout << "Queue.put(T&& data) <-\n";
			return true;
		}

		bool put(const T& data)
		{
			cout << "Queue.put(const T& data) ->\n";
			if (m_size <= m_deque.size())
			{
				return false;
			}
			m_deque.emplace_back(data);
			cout << "Queue.put(const T& data) <-\n";
			return true;
		}

		bool get(T& data)
		{
			cout << "Queue.get(T& data) ->\n";
			if (m_deque.empty())
			{
				return false;
			}
			data = std::move(m_deque.front());
			m_deque.pop_front();
			cout << "Queue.get(T& data) <-\n";
			return true;
		}

		bool empty()
		{
			return m_deque.empty();
		}

	private:
		int m_size;
		deque<T>	m_deque;
	};

	class Printer
	{
	public:
		Printer(int queueSize) :
			m_isTerminationRequested(false),
			m_queue(queueSize),
			m_thread(m_main)
		{
			// The specified thread function will be kicked off.
			cout << "Printer constructor\n";
		}
		~Printer()
		{
			cout << "Printer destructor ->\n";
			{
				unique_lock<mutex> ul(m_mutex);
				m_isTerminationRequested = true;
				// !!! Note !!!
				// The mutex needs to be unlocked as soon as possible,
				// otherwise unique_lock<mutex> ul(m_mtx) in the thread
				// would wait for until it's unlocked.
				// This means the thread can't finish!
				// So you can't leave m_mutex locked when calling join().
				// It causes a dead lock.
			}
			m_cv.notify_one();
			m_thread.join();
			cout << "Printer destructor <-\n";
		}

		bool append(string&& str)
		{
			unique_lock<mutex> ul(m_mutex);
			if (!m_queue.put(move(str)))
			{
				return false;
			}
			m_cv.notify_one();
			return true;
		}

		bool append(const string& str)
		{
			unique_lock<mutex> ul(m_mutex);
			if (!m_queue.put(str))
			{
				return false;
			}
			m_cv.notify_one();
			return true;
		}

	private:
		std::function<void()> m_main = [this]
			{
				cout << "m_main() ->\n";
				while (1)
				{
					string str;
					{
						unique_lock<mutex> ul(m_mutex);
						while (m_queue.empty())
						{
							if (m_isTerminationRequested)
							{
								return;
							}
							cout << "start waiting\n";
							m_cv.wait(ul);
							cout << "finished waiting\n";
						}
						const bool result = m_queue.get(str);
						assert(result);
					}
					cout << str << endl;
				}
				cout << "m_main() <-\n";
			};

		// Initilization will run in this order.
		bool m_isTerminationRequested;
		Queue<string> m_queue;
		mutex	m_mutex;
		condition_variable m_cv;
		// And finally m_thread is initialised.
		// It is to make sure other members are initialised first
		// before running m_thread;
		thread m_thread;
	};
};

int Run_GuardedSuspention();


#endif // GUARDEDSUSPENTION_H_INCLUDED