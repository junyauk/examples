#include "pch.h"
#include "producerconsumer.h"

#include <deque>
#include <queue>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>
#include <cassert>
#include <random>
#include <chrono>
#include <sstream>

using std::cout;
using std::endl;


using std::deque;
using std::queue;
using std::string;
using std::vector;
using std::stringstream;

using std::thread;
using std::mutex;
using c_v = std::condition_variable;
using std::unique_lock;

using std::this_thread::sleep_for;
using std::chrono::milliseconds;


namespace Example1
{
	template <typename T>
	class Queue
	{
	public:
		Queue(int size) : m_size(size)
		{
			cout << "Queue constructor\n";
		}

		bool put(T&& data) // data is rref, so will be moved
		{
			cout << "put ->\n";
			if (m_size <= m_deque.size())
			{
				return false;
			}
			// Use emplace_back(), it doesn't call copy constructor.
			// It's quicker than push_back()
			m_deque.emplace_back(std::move(data));
			cout << "put <-\n";
			return true;
		}

		bool put(const T& data)	// data is lref, so will be copied
		{
			cout << "put ->\n";
			if (m_size <= m_deque.size())
			{
				return false;
			}
			// Use emplace_back(), it doesn't call copy constructor.
			// It's quicker than push_back()
			m_deque.emplace_back(data);
			cout << "put <-\n";
			return true;
		}

		bool get(T& data)
		{
			cout << "get ->\n";
			if (m_deque.empty())
			{
				return false;
			}
			// data in the queue can be discarded.
			// so move the data
			data = std::move(m_deque.front());
			m_deque.pop_front();
			cout << "get <-\n";
			return true;
		}

		bool empty()
		{
			return m_deque.empty();
		}

	private:
		int m_size;
		deque<T> m_deque;
	};

	class Printer
	{
	public:
		Printer(int threadCount, int queueSize)
			: m_isTerminationRequested(false)
			, m_queue(queueSize)
		{
			cout << "Printer constructor ->\n";
			for (int i = 0; i < threadCount; i++)
			{
				// function object "m_main" requires one parameter.
				m_threads.emplace_back(thread(m_main, i));
			}
			cout << "Printer constructor <-\n";
		}

		~Printer()
		{
			cout << "Printer destructor ->\n";
			{
				unique_lock<mutex> ul(m_mutex);
				m_isTerminationRequested = true;
			}
			m_cv.notify_all();
			auto size = m_threads.size();
			for (auto i = 0; i < size; i++)
			{
				m_threads[i].join();
			}
			cout << "Printer destructor <-\n";
		}

		bool append(string&& str) // str is rref, move
		{
			cout << "Printer append ->\n";
			{
				unique_lock<mutex> ul(m_mutex);
				if (!m_queue.put(std::move(str)))
				{
					return false;
				}
			}
			m_cv.notify_all();
			cout << "Printer append <-\n";
			return true;
		}

		bool append(const string& str) // str is lref, copy 
		{
			cout << "Printer append ->\n";
			{
				unique_lock<mutex> ul(m_mutex);
				if (!m_queue.put(str))
				{
					return false;
				}
			}
			m_cv.notify_all();
			cout << "Printer append <-\n";
			return true;
		}

	private:
		// This thread function "m_main" is defined using std::function with lambda formula
		// The function's arguments are corresponding with 
		// the actual parameters of the specified lambda
		// function<void(int) [](int num){}
		// function<double(float, int) [](float num1, int num2){ return (double)num1;}
		std::function<void(int)> m_main = // As thread function, the return value is void
			// lambda formula
			[&, this](int num)	// specify '&' for using private members. you can also specify "this" as well
			{
				auto th_id = std::this_thread::get_id();
				cout << "m_main thread ID: " << th_id << "->\n";
				while (1)
				{
					string str;
					{
						// Printer has got only one queue "m_queue".
						// So accessing the queue, you need to make sure it's thread safe.
						unique_lock<mutex> ul(m_mutex);
						cout << "mutex locked ID: " << th_id << endl;
						// Now all threads accessing m_mutex will be waited except this thread who locked.
						while (m_queue.empty())
						{
							if (m_isTerminationRequested)
							{
								cout << "m_main thread terminated <-\n";
								return;
							}
							// m_cv is signaled when something has been added to the m_queue.
							m_cv.wait(ul);
							// Now something has been added (someone has called m_cv.notify_all())
							// and ready to call get().
						}
						const bool result = m_queue.get(str);
						assert(result);
						// Now the mutex is unlocked after this
						cout << "mutex unlocked ID: " << th_id << endl;
					}
					cout << "num = " << num << " " << str << endl;
				}
				cout << "m_main thread ID: " << th_id << "<-\n";
			};

		int m_isTerminationRequested;
		Queue<string> m_queue;
		mutex m_mutex;
		c_v m_cv;
		vector<thread> m_threads;
	};

	void Run_ProducerConsumer()
	{
		// This Printer class will create 5 threads at constructor
		Printer printer(5, 10);

		for (int i = 0; i < 50; i++)
		{
			// Create a string
			stringstream ss;
			ss << "hello world " << i;

			// The created string is moved to the queue in the printer.
			while (!printer.append(std::move(ss.str())))
			{
				sleep_for(milliseconds(1));
			}
		}
	}

};

namespace Example2
{
	using std::random_device;
	using std::mt19937;
	using std::uniform_int_distribution;
	const int BUFFER_SIZE = 10;
	queue<int> g_queue;
	mutex	g_mutex;
	c_v		g_cv;
	bool	g_finish = false;

	static void producer()
	{
		random_device rd;
		mt19937	gen(rd());
		uniform_int_distribution<> dis(1, 100);

		while (1)
		{
			// generate a random item
			int item = dis(gen);
			// lock the mutex
			{ // Critical Section ->
				unique_lock<mutex> ul(g_mutex);
				if (g_finish)
				{
					return;
				}
				// wait until the queue is not full
				g_cv.wait(ul, [&] {return g_queue.size() < BUFFER_SIZE; });
				// push the item to the queue
				g_queue.push(item);
				// unlock the mutex
				ul.unlock();
				// notify the consumer that an item is available
				g_cv.notify_one();
			} // Critical Section <-
			// print the item
			cout << "Producer produced " << item << endl;
			// sleep for a random time
			sleep_for(milliseconds(dis(gen)));
		}
	}

	static void consumer()
	{
		random_device rd;
		mt19937	gen(rd());
		uniform_int_distribution<> dis(1, 100);

		while (1)
		{
			int item = 0;
			{ // Critical Section ->
				// lock the mutex
				unique_lock<mutex> ul(g_mutex);
				if (g_finish)
				{
					return;
				}
				// wait until the queue is not empty
				g_cv.wait(ul, [&] { return !g_queue.empty(); });
				// pop an item from the queue
				item = g_queue.front();
				g_queue.pop();
				// notify the producer that an item was consumed
				g_cv.notify_one();
			} // Critical Section <-
			// print the item
			cout << "Consumer consumed " << item << endl;
			// sleep for a random time
			sleep_for(milliseconds(dis(gen)));
		}
	}

	static void Run_ProducerConsumer()
	{
		// create and start both Producer and Consumer threads
		thread th_producer(producer);
		thread th_consumer(consumer);

		sleep_for(milliseconds(3000));

		{
			unique_lock<mutex> ul(g_mutex);
			g_finish = true;
			g_cv.notify_all();
		}

		// join threads
		th_producer.join();
		th_consumer.join();

		return;
	}
};


int Run_ProducerConsumer()
{
	Example1::Run_ProducerConsumer();
	Example2::Run_ProducerConsumer();

	return 0;
}
