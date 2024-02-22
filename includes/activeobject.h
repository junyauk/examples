#pragma once
#ifndef ACTIVEOBJECT_H_INCLUDED
#define ACTIVEOBJECT_H_INCLUDED

#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <future>

using std::string;
using std::queue;
using std::thread;
using std::mutex;
using std::unique_lock;
using std::function;
using c_v = std::condition_variable;
using std::shared_ptr;
using std::make_shared;
using std::packaged_task;
using std::future;

int Run_ActiveObject();

namespace ActiveObject::Basic1
{
	class ActiveObject
	{
	public:
		ActiveObject();
		~ActiveObject();

		void run(function<void()>& task);
	private:
		void worker();

		queue<function<void()>> m_tasks;
		mutex m_mutex;
		c_v m_cv;
		bool m_stop;
		thread	m_thread;
	};

	class Tests
	{
	public:
		int run();

		static void dummy_static_public_func(string const& str);
		void dummy_non_static_public_func(string const& str) const;

	private:
		static void dummy_static_private_func(string const& str);
		void dummy_non_static_private_func(string const& str) const;
	};
}

namespace ActiveObject::Basic2
{
	// This ActiveObject runs functions with multiple parameters.
	class ActiveObject
	{
	public:
		ActiveObject();
		~ActiveObject();

		template<typename Func, typename... Args>
		auto run(Func func, Args... args) -> future<decltype(func(args...))>
		{
			auto task = make_shared<packaged_task<decltype(func(args...))()>>(std::bind(func, args...));
			future<decltype(func(args...))> f = task->get_future();
			{
				unique_lock<mutex>	ul(m_mutex);
				m_tasks.push([task] {(*task)(); });
			}
			m_cv.notify_one();
			return f;
		}

	private:
		void worker();

		bool	m_stop;
		mutex	m_mutex;
		c_v		m_cv;
		queue<function<void()>>	m_tasks;
		thread	m_thread;
	};

	class Tests
	{
	public:
		int run();
	};
}
#endif // ACTIVEOBJECT_H_INCLUDED