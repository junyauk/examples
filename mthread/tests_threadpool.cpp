#include "pch.h"
#include "Windows.h"

#include "general.h"

#include "tests.h"
#include "threadpool.h"
#include "tests_threadpool.h"

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
	void Task::process() const
	{
		stringstream ss;
		ss << "Task: " << m_id << " was processed by thread ID: " << std::this_thread::get_id() << endl;
		ThreadSafeOut::print(ss.str());
	}

	Task Request::produce()
	{
		return ++m_id;
	}

	void WebServer::processRequest(Task&& task)
	{
		// setting the Task::process with the moved instance
		m_threadPool.enqueueTask(std::bind(&Task::process, std::move(task)));
	}
	void WebServer::stop()
	{
		m_threadPool.stop();
	}

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
	int Tests::run()
	{
		unique_ptr<ExecutionStrategy> strategy;

		// in the case of using single-threading
		{
			strategy = make_unique<SingleThreadExecution>();
			strategy->execute([]()
				{
					ThreadSafeOut::print("Single Task\n");
				});
		}

		// in the case of using Multi-threading
		{
			strategy = make_unique<MultiThreadExecution>();
			for (auto i = 0; i < 10; ++i)
			{
				strategy->execute([i]()
					{
						stringstream ss;
						ss << "Task: " << i << " is being executed by thread: " << std::this_thread::get_id() << endl;
						ThreadSafeOut::print(ss.str());
						sleep_for(milliseconds(100));
					});
			}
		}

		sleep_for(seconds(1));

		return 0;
	}
}

namespace ThreadPool::Windows1
{
	int Tests::run()
	{
		PTP_CLEANUP_GROUP pCleanupGroup = CreateThreadpoolCleanupGroup();
		if (!pCleanupGroup)
		{
			cout << "Failed to create a thread pool group.\n";
			return 1;
		}

		// Initialize thraed pool environment
		TP_CALLBACK_ENVIRON env;
		InitializeThreadpoolEnvironment(&env);
		SetThreadpoolCallbackCleanupGroup(&env, pCleanupGroup, NULL);

		// Create a thread pool
		PTP_POOL pool = CreateThreadpool(NULL);
		// Set max and min numbers of threads
		SetThreadpoolThreadMaximum(pool, 2);
		SetThreadpoolThreadMinimum(pool, 2);

		// Associate thread pool with the environment
		SetThreadpoolCallbackPool(&env, pool);

		// Create work items for printing even/odd numbers
		PTP_WORK workE = CreateThreadpoolWork(PrintEvenNumbers, NULL, &env);
		PTP_WORK workO = CreateThreadpoolWork(PrintOddNumbers, NULL, &env);
		if (!workE || !workO)
		{
			cout << "Failed to create thread pool work items.\n";
			CloseThreadpoolCleanupGroup(pCleanupGroup);
			return 1;
		}

		// Submit work items to thread pool
		SubmitThreadpoolWork(workE);
		SubmitThreadpoolWork(workO);

		// Wait for work items to complete
		WaitForThreadpoolWorkCallbacks(workE, FALSE);
		WaitForThreadpoolWorkCallbacks(workO, FALSE);

		// Tydying up
		CloseThreadpoolWork(workE);
		CloseThreadpoolWork(workO);
		CloseThreadpoolCleanupGroupMembers(pCleanupGroup, FALSE, NULL);
		CloseThreadpoolCleanupGroup(pCleanupGroup);
		CloseThreadpoolCleanupGroupMembers(pCleanupGroup, TRUE, NULL);
		CloseThreadpoolCleanupGroup(pCleanupGroup);
		CloseThreadpoolCleanupGroupMembers(pCleanupGroup, FALSE, NULL);
		CloseThreadpoolCleanupGroup(pCleanupGroup);
		CloseThreadpoolCleanupGroupMembers(pCleanupGroup, TRUE, NULL);
		CloseThreadpoolCleanupGroup(pCleanupGroup);

		// Destroy the thread pool
		CloseThreadpool(pool);

		// Destroy the cleanup group
		CloseThreadpoolCleanupGroup(pCleanupGroup);

		return 0;
	}
}



