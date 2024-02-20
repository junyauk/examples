#include "pch.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <memory>
#include <atomic>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::queue;
using std::map;
using std::thread;
using std::mutex;
using std::unique_lock;
using stdcv = std::condition_variable;
using std::function;
using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;
using std::weak_ptr;
using std::pair;
using std::make_pair;
using std::atomic;


namespace Reactor::Example1
{
	class safeout
	{
	public:
		static void print(string const& str)
		{
			static mutex m;
			{
				unique_lock<mutex>	ul(m);
				cout << str;
			}
		}
	};

	class Reactor
	{
	public:
		Reactor()
			: m_stop(false)
			, m_suspend(true)
		{}

		void registerEventHandler(function<void()> handler)
		{
			unique_lock<mutex>	ul(m_mutex);
			m_eventhandlers.push(handler);
			m_cv.notify_one();
		}

		void suspend()
		{
			m_suspend = true;
		}

		void resume()
		{
			m_suspend = false;
		}

		void stop()
		{
			m_stop = true;
		}

	private:
		void dispatchEvent()
		{
			while (!m_stop)
			{
				function<void()> handler;
				{
					unique_lock<mutex>	ul(m_mutex);
					m_cv.wait(ul, [this]() { return m_stop || (!m_eventhandlers.empty() || !m_suspend); });
					if (m_stop)
					{
						return;
					}
					handler = m_eventhandlers.front();
					m_eventhandlers.pop();
				}
				handler();
			}
		}

		queue<function<void()>> m_eventhandlers;
		thread m_dispatchThread;
		mutex m_mutex;
		stdcv m_cv;
		atomic<bool> m_suspend;
		atomic<bool> m_stop;
	};

	void Run_Reactor()
	{
		Reactor r;

		r.registerEventHandler([]
			{
				stringstream ss;
				ss << "Event 1 handled by thread : " << std::this_thread::get_id() << endl;
				safeout::print(ss.str());
			});

		r.registerEventHandler([]
			{
				stringstream ss;
				ss << "Event 2 handled by thread : " << std::this_thread::get_id() << endl;
				safeout::print(ss.str());
			});

		r.suspend();

		r.registerEventHandler([]
			{
				stringstream ss;
				ss << "Event 3 handled by thread : " << std::this_thread::get_id() << endl;
				safeout::print(ss.str());
			});
		r.registerEventHandler([]
			{
				stringstream ss;
				ss << "Event 4 handled by thread : " << std::this_thread::get_id() << endl;
				safeout::print(ss.str());
			});

		r.resume();

		sleep_for(seconds(2));
		r.stop();
		return;
	}
} // Reactor::Example1

namespace Reactor::Example2
{
}

int Run_Reactor()
{
	Reactor::Example1::Run_Reactor();
	return 0;
}
