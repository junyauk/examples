#include "pch.h"
#include <Windows.h>

#include "tests_producerconsumer.h"

namespace ProducerConsumer::Basic1
{
	int Tests::run()
	{
		Queue q;
		Producer producer(q);
		Consumer consumer(q);

		sleep_for(milliseconds(3000));
		return 0;
	}
}

namespace ProducerConsumer::Basic2
{
	int Tests::run()
	{
		Queue	q;
		Producer	p(q, 5);
		Consumer	c(q, 10);

		sleep_for(milliseconds(3000));
		return 0;
	}
}

namespace ProducerConsumer::Printer1
{
	int Tests::run()
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

		return 0;
	}
}
namespace ProducerConsumer::TaskQueue
{
	void producer(TaskQueue& queue, int producerId)
	{
		for (auto i = 0; i < 5; i++)
		{
			queue.addTask(producerId);
			sleep_for(milliseconds(100));
		}
	}
	void consumer(TaskQueue& queue)
	{
		for (auto i = 0; i < 10; i++)
		{
			int taskid = queue.getTask();
			cout << "Consumer Thread,  Task ID: " << taskid << endl;
		}
	}

	int Tests::run()
	{
		int numThreads = 3;
		TaskQueue tq;

		// multiple producers will run
		vector<thread>	producers;
		for (auto i = 0; i < numThreads; i++)
		{
			producers.emplace_back(producer, std::ref(tq), i);
			// producers.emplace_back([&] {producer(tq, i); });
		}

		// only one single consumer get produced tasks
		thread	consumerThread(consumer, std::ref(tq));
		// thread  consumerThread([&] {consumer(tq); });

		// wait for all threads to finish
		for (auto& t : producers)
		{
			t.join();
		}

		consumerThread.join();

		return 0;
	}
}

