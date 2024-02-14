#include "pch.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <queue>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::thread;
using std::mutex;
using std::unique_lock;
using stdcv = std::condition_variable;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;
using std::vector;
using std::queue;
using std::string;
using std::to_string;
using std::stringstream;

namespace MonitorObject::Example1
{
	class Monitor
	{
	public:
		void waitForCondition()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_cv.wait(ul, [this] {return m_matched; });
		}
		void NotifyCondition()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_matched = true;
			m_cv.notify_all();
		}
	private:
		mutex	m_mutex;
		stdcv	m_cv;
		bool	m_matched{ false };
	};

	void Run_MonitorObject()
	{
		Monitor	m;

		thread	t1{ [&m] {
				cout << "Thread 1 waiting for condition..." << endl;
				m.waitForCondition();
				cout << "Thread 1 condition met!" << endl;
			} };

		thread t2{ [&m] {
				sleep_for(seconds(3));
				cout << "Thread 2 notifying condition..." << endl;
				m.NotifyCondition();
			} };

		t1.join();
		t2.join();

		return;

	}
}

namespace MonitorObject::Example2
{
	class safeout
	{
	public:
		static void print(string const& str)
		{
			static mutex	m;
			{
				unique_lock<mutex>	ul(m);
				cout << str;
			}

		}
	};

	class PrinterQueue
	{
	public:
		void addJob(string const& str)
		{
			unique_lock<mutex>	ul(m_mutex);
			m_jobs.push(str);
			m_cv.notify_all();
		}
		string getNextJob()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_cv.wait(ul, [this] { return !m_jobs.empty() || m_terminate; });
			if (m_terminate)
			{
				return "";
			}
			string job = m_jobs.front();
			m_jobs.pop();
			return job;
		}
		std::size_t size()
		{
			unique_lock<mutex>	ul(m_mutex);
			return m_jobs.size();
		}
		void terminate()
		{
			m_terminate = true;
			m_cv.notify_all();
		}
	private:
		bool			m_terminate{ false };
		mutex			m_mutex;
		stdcv			m_cv;
		queue<string>	m_jobs;
	};

	void producer(PrinterQueue& pq, int const& id)
	{
		for (auto i = 0; i < 5; i++)
		{
			stringstream ss;
			ss << "Job: " << i << " from Producer " << id << endl;
			pq.addJob(ss.str());
			stringstream out;
			out << "Producer " << id << " added " << ss.str() << " to the queue\n";
			safeout::print(out.str());
			sleep_for(milliseconds(100));
		}
	}

	void consumer(PrinterQueue& pq, int const& id)
	{
		while(true)
		{
			stringstream ss;
			string job = pq.getNextJob();
			if (job.empty())
			{
				break;
			}
			stringstream out;
			out << "Consumer " << id << " printer " << job << " from the queue\n";
			safeout::print(out.str());
			sleep_for(milliseconds(200));
		}
	}


	void Run_MonitorObject()
	{
		PrinterQueue pq;

		thread	p1(producer, std::ref(pq), 1);
		thread	p2(producer, std::ref(pq), 2);
		thread	c1(consumer, std::ref(pq), 1);
		thread	c2(consumer, std::ref(pq), 2);

		sleep_for(seconds(5));

		pq.terminate();

		p1.join();
		p2.join();
		c1.join();
		c2.join();

		stringstream out;
		out << "The final size of the queue is " << pq.size() << endl;
		safeout::print(out.str());

		return;
	}
}

int Run_MonitorObject()
{
//	MonitorObject::Example1::Run_MonitorObject();
	MonitorObject::Example2::Run_MonitorObject();
	return 0;
}
