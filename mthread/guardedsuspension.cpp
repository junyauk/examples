#include "pch.h"

#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::thread;
using std::mutex;
using std::unique_lock;
using std::lock_guard;
using c_v = std::condition_variable;
using std::queue;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::chrono::seconds;

class safeout
{
public:
	static void print(const string& str)
	{
		static mutex	m;
		{
			unique_lock<mutex>	ul(m);
			cout << str << endl;
		}
	}
};

namespace Example1
{
	class Resource
	{
	public:
		Resource()
			: m_done(false)
		{}

		// A method to produce data and push it to the queue
		void produce(int item)
		{
			lock_guard<mutex>	lg(m_mutex);
			m_data.push(item);
			stringstream ss;
			ss << "Produced " << item;
			safeout::print(ss.str());
		}

		int consume()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_cv.wait(ul, [this]() {return !m_data.empty() || m_done; });
			if (m_data.empty() && m_done)
			{
				return -1;
			}
			int item = m_data.front();
			m_data.pop();
			stringstream ss;
			ss << "Consumed " << item;
			safeout::print(ss.str());
			return item;
		}

		void set_done()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_done = true;
			m_cv.notify_all();
		}
	private:
		mutex	m_mutex;
		c_v		m_cv;
		queue<int>	m_data;
		bool	m_done;
	};

	// a function that simulates a producer thread
	void producer(Resource& r)
	{
		// produce 10 items and push them to the resource
		for (auto i = 0; i < 10; i++)
		{
			r.produce(i);
			sleep_for(seconds(1));
		}
		// set the resource to done
		r.set_done();
	}

	// a function that simulate a consumer thread
	void consumer(Resource& r)
	{
		// consume items from the resource until -is is returned
		while (true)
		{
			int item = r.consume();
			if (item == -1)
			{
				break;
			}
			// sleep for a random amout of time
			sleep_for(seconds(1));
		}
	}

	void Run_GuardedSuspension()
	{
		// create a resource object
		Resource	r;
		
		// create a producer thread
		thread	p(producer, std::ref(r));
		thread	c(consumer, std::ref(r));

		// join the threads.
		p.join();
		c.join();

		return;
	}
}

namespace Example2
{
	class GuardedQueue
	{
	public:
		void enqueue(const int data)
		{
			unique_lock<mutex>	ul(m_mutex);	// 1. lock
			m_queue.push(data);					// 2. push
			m_cv.notify_one();					// 3. notify
		}

		int dequeue()
		{
			unique_lock<mutex>	ul(m_mutex);						// 1. lock
			m_cv.wait(ul, [this]() { return !m_queue.empty(); });	// 2. wait
			int data = m_queue.front();								// 3. pop
			m_queue.pop();
			m_cv.notify_one();										// 4. notify
			return data;											// 5. return
		}
	private:
		queue<int>	m_queue;
		mutex		m_mutex;
		c_v			m_cv;
	};

	void producer(GuardedQueue& gq)
	{
		for (auto i = 0; i < 10; i++)
		{
			// generate some data
			int data = i * i;
			// enqueue it
			gq.enqueue(data);
			stringstream ss;
			ss << "Producer enqueued\n";
			safeout::print(ss.str());
			sleep_for(milliseconds(100));
		}
	}
	
	void consumer(GuardedQueue& gq)
	{
		for (auto i = 0; i < 10; i++)
		{
			// dequeue it
			int data = gq.dequeue();
			stringstream ss;
			ss << "Consumer dequeued: " << data;
			safeout::print(ss.str());
			sleep_for(milliseconds(200));
		}
	}

	void Run_GuardedSuspension()
	{
		GuardedQueue	q;

		thread	p(producer, std::ref(q));
		thread	c(consumer, std::ref(q));

		p.join();
		c.join();

		return;
	}
}

int Run_GuardedSuspension()
{
//	Example1::Run_GuardedSuspension();
	Example2::Run_GuardedSuspension();
	return 0;
}