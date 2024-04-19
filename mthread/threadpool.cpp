#include "pch.h"

#include <sstream>
#include <memory>
#include "threadpool.h"

using std::stringstream;
using std::unique_ptr;
using std::make_unique;

namespace ThreadPool::Basic1
{
	int Tests::run()
	{
		ThreadPool	threadPool(4);

		for (auto i = 0; i < 50; ++i)
		{
			threadPool.enqueueTask([i]()
				{
					stringstream ss;
					ss << "Task: " << i << " processed by Thread: " << std::this_thread::get_id() << endl;
					cout << ss.str();
				});
		}

		sleep_for(seconds(5));

		return 0;
	}
}

namespace ThreadPool::ProducerConsumer
{
	class SafeOut
	{
	public:
		void out(const string str)
		{
			unique_lock<mutex> ul(m_mutex);
			cout << str;
		}
	private:
		mutex m_mutex;
	};

	class Task
	{
	public:
		Task(int id) : m_id(id) {}
		void process()
		{
			static SafeOut out;
			stringstream ss;
			ss << "Task: " << m_id << " was processed by thread ID: " << std::this_thread::get_id() << endl;
			out.out(ss.str());
		}
	private:
		int m_id;
	};

	class Request
	{
	public:
		Task produce()
		{
			return ++m_id;
		}
	private:
		int m_id{ 0 };
	};

	class WebServer
	{
	public:
		void processRequest(Task&& task)
		{
			// setting the Task::process with the moved instance
			m_threadPool.enqueueTask(std::bind(&Task::process, std::move(task)));
		}
		void stop()
		{
			m_threadPool.stop();
		}
	private:

		ThreadPool m_threadPool{ 5 };
	};

	int Tests::run()
	{
		WebServer webServer;
		Request	request;

		for (auto i = 0; i < 20; ++i)
		{
			webServer.processRequest(request.produce());
		}

		sleep_for(seconds(5));

		webServer.stop();

		return 0;
	}
}

namespace ThreadPool::Strategy
{
	class SafeOut
	{
	public:
		void out(const string str)
		{
			unique_lock<mutex> ul(m_mutex);
			cout << str;
		}
	private:
		mutex m_mutex;
	};

	int Tests::run()
	{
		unique_ptr<ExecutionStrategy> strategy;

		// in the case of using single-threading
		{
			strategy = make_unique<SingleThreadExecution>();
			strategy->execute([]()
				{
					static SafeOut sout;
					sout.out("Single Task\n");
				});
		}

		// in the case of using Multi-threading
		{
			strategy = make_unique<MultiThreadExecution>();
			for (auto i = 0; i < 10; ++i)
			{
				strategy->execute([i]()
					{
						static SafeOut sout;
						stringstream ss;
						ss << "Task: " << i << " is being executed by thread: " << std::this_thread::get_id() << endl;
						sout.out(ss.str());
						sleep_for(milliseconds(100));
					});
			}
		}

		sleep_for(seconds(1));

		return 0;
	}
}