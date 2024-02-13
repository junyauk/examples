#include "pch.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <map>
#include <future>
#include <functional>

using std::cout;
using std::endl;
using std::stringstream;
using std::string;
using std::to_string;
using std::vector;
using std::queue;
using std::thread;
using std::mutex;
using stdcv = std::condition_variable;
using std::unique_lock;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;
using std::atomic;
using tid = std::thread::id;
using std::future;
using std::promise;
using std::shared_future;
using std::future_status;
using std::function;

// Leader and follower multi thread design pattern
// It has got a thread pool.
// The thread pool has got the following lists
// - a list of threads
// - a list of tasks
// Tasks are added to the thread pool.
// The threads are started when constructor, and ended when destructor.
// The thread checks if there is a task in the list, and process it if exists.
// If there is no task in the list, the thread will wait for it.
// When a task was added, only one thread will be notified for processing the task.

namespace LeaderFollower::Example1
{
	class safeout
	{
	public:
		static void print(const string& str)
		{
			static mutex	m;
			{
				unique_lock<mutex>	ul(m);
				cout << str;
			}
		}
	};

	class Event
	{
	public:
		Event(const int id)
			: m_id(id)
		{}
		int get_id() const
		{
			return m_id;
		}
	private:
		int m_id;
	};

	class ThreadPool
	{
	public:
		ThreadPool(int numThreads)
			: m_leaderIndex(0)
			, m_stop(false)
		{
			for (auto i = 0; i < numThreads; i++)
			{
//				m_threads.emplace_back(&ThreadPool::worker, this, i);
				m_threads.emplace_back([this, i] { worker(i); });
			}
		}

		~ThreadPool()
		{
			safeout::print("destructor of ThreadPool ->\n");
			m_stop = true;
			m_cv.notify_all();
			for (auto& t : m_threads)
			{
				t.join();
			}
			safeout::print("destructor of ThreadPool <-\n");
		}
		void addEvents(const Event& e)
		{
			unique_lock<mutex>	ul(m_mutex);
			m_events.push_back(e);
			m_cv.notify_one();
		}

	private:
		void processEvents(thread::id const &tid)
		{
			// Simulate process events
			if (!m_events.empty())
			{
				Event e = m_events.back();
				m_events.pop_back();
				stringstream ss;
				ss << "Thread " << tid << " process event " << e.get_id() << "\n";
				safeout::print(ss.str());
			}
		}
		void worker(int index)
		{
			auto tid = std::this_thread::get_id();
			stringstream ss;
			ss << "Worker (" << tid << "]) started\n";
			ss.clear();
			while (!m_stop)
			{
				unique_lock<mutex>	ul(m_mutex);
				m_cv.wait(ul, [this]() { return !m_events.empty() || m_stop; });
				if (m_stop)
				{
					ss << "Thread " << tid << " worker exits\n";
					safeout::print(ss.str());
					ss.clear();
					return;
				}

				// The leader thread process events
				if (index == m_leaderIndex)
				{
					processEvents(tid);
					m_leaderIndex = (m_leaderIndex + 1) % m_threads.size();
				}
			}
		}
		vector<thread>	m_threads;
		vector<Event>	m_events;
		mutex			m_mutex;
		stdcv			m_cv;
		int				m_leaderIndex;
		atomic<bool>	m_stop;
	};

	void Run_LeaderFollower()
	{
		const int numThreads = 4;
		ThreadPool	tp(numThreads);

		// Add some events
		for (auto i = 0; i < 10; i++)
		{
			tp.addEvents(Event(i));
		}

		sleep_for(seconds(2));
	}
}

namespace LeaderFollower::Example2
{
	// This example uses classes, they are Event, ThreadPool.
	// The Event class is created specified ID and handler.
	// The created Event is added to ThreadPool via ThreadPool::addEvent().
	// The ThreadPool class is created specified the number of threads.
	// The class will create leader and follower threads based on the number.
	// The leader thread will pick up the front element from the Event queue,
	// and add a string and let other threads know the element is ready.
	// The follwer thread will pop the front element from the Event queue,
	// and call a handler of the Event.
	// The handler set in the Event will be called.
	
	class Event
	{
	public:
		Event(int const &id, function<void(string)> const &handler)
			: m_id(id)
			, m_handler(handler)
		{}
		int get_id() const
		{
			return m_id;
		}
		void setData(string const &str)
		{
			m_str = str;
		}
		void callHandler(string const &str)
		{
			stringstream ss;
			ss << "EventID: " << m_id << ", " << m_str << ", " << str;
			m_handler(ss.str());
		}
	private:
		int		m_id;
		string	m_str;
		function<void(string)> m_handler;
	};

	class ThreadPool
	{
	public:
		ThreadPool(int const& numThreads)
			: m_stop(false)
			, m_ready(false)
			, m_suspend(false)
		{
			for (auto i = 0; i < numThreads; i++)
			{
				m_leaders.emplace_back(&ThreadPool::leader, this, i);
				m_followers.emplace_back(&ThreadPool::follower, this, i);
			}
		}
		~ThreadPool()
		{
			{
				unique_lock<mutex>	ul(m_mutex);
				m_stop = true;
				m_cv.notify_all();
			}
			for (auto& t : m_leaders)
			{
				t.join();
			}
			for (auto& t : m_followers)
			{
				t.join();
			}
		}

		void addEvent(int const& id, function<void(string)> const & handler)
		{
			unique_lock<mutex>	ul(m_mutex);
			Event	event(id, handler);
			m_events.push(event);
			m_cv.notify_all();
		}

		void suspend()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_suspend = true;
			m_cv.notify_all();
		}
		void resume()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_suspend = false;
			m_cv.notify_all();
		}

	private:
		void leader(int const& leaderNumber)
		{
			stringstream ss;
			int tId;
			string strId;
			ss << std::this_thread::get_id();
			ss >> tId;
			strId = ss.str();
			while (!m_stop)
			{
				unique_lock<mutex>	ul(m_mutex);
				m_cv.wait(ul, [this] {return m_stop || (!m_events.empty() && !m_ready && !m_suspend); });
				if (m_stop)
				{
					return;
				}
				Event& event = m_events.front();
				event.setData("Leader ID: " + strId);
				sleep_for(milliseconds(100));
				m_ready = true;
				m_cv.notify_all();
			}
		}

		void follower(int const &followerNumber)
		{
			stringstream ss;
			int tId;
			string strId;
			ss << std::this_thread::get_id();
			ss >> tId;
			strId = ss.str();
			while (!m_stop)
			{
				unique_lock<mutex>	ul(m_mutex);
				//				m_cv.wait(ul, [this] {return m_stop || (!m_events.empty() && m_ready); });
				m_cv.wait(ul, [this] {return m_stop || (!m_events.empty() && m_ready && !m_suspend); });
				if (m_stop)
				{
					return;
				}
				Event &event = m_events.front();
				event.callHandler("Follower ID: " + strId);
				m_events.pop();
				m_ready = false;
				m_cv.notify_all();
			}
		}

		mutex			m_mutex;
		stdcv			m_cv;
		bool			m_stop;
		bool			m_ready;
		bool			m_suspend;
		queue<Event>	m_events;
		vector<thread>	m_leaders;
		vector<thread>	m_followers;
	};

	void handler(string const& str)
	{
		sleep_for(milliseconds(50));
		cout << str << endl;
	}
	void reverseHandler(string const& str)
	{
		sleep_for(milliseconds(50));
		cout << string{ str.rbegin(), str.rend() } << endl;
	}

	void Run_LeaderFollower()
	{
		ThreadPool	tp(4);

		for (auto i = 0; i < 30; i++)
		{
			tp.addEvent(i, (i % 2) ? reverseHandler : handler);
		}
		sleep_for(seconds(5));
		tp.suspend();
		cout << "Suspended..." << endl;
		for (auto i = 0; i < 20; i++)
		{
			tp.addEvent(i, (i % 2) ? reverseHandler : handler);
		}
		sleep_for(seconds(5));
		cout << "Resumed..." << endl;
		tp.resume();
		for (auto i = 0; i < 40; i++)
		{
			tp.addEvent(i, (i % 2) ? reverseHandler : handler);
		}
		sleep_for(seconds(5));
		return;
	}
}

int Run_LeaderFollower()
{
	LeaderFollower::Example1::Run_LeaderFollower();
	LeaderFollower::Example2::Run_LeaderFollower();
	return 0;
}
