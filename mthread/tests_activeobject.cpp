#include "pch.h"
#include "tests_activeobject.h"



namespace ActiveObject::Example1
{
	int Tests::run()
	{
		// create an active object
		ActiveObject	ao;

		// enqueue some work requests
		ao.enqueueWork(1);
		ao.enqueueWork(2);
		ao.enqueueWork(3);

		// do other things
		cout << "Main thread is doing other things" << endl;
		sleep_for(seconds(5));

		return 0;
	}
}

namespace ActiveObject::Example2
{
	int Tests::run()
	{
		// create an active object
		ActiveObject	ao;

		// enqueue some compute results and get futures
		future<int>	f1 = ao.enqueueCompute(1);
		future<int>	f2 = ao.enqueueCompute(2);
		future<int>	f3 = ao.enqueueCompute(3);

		// do other things
		cout << "Main thread is doing other things...\n";
		sleep_for(seconds(5));

		// get the results from the futures.
		cout << "Result of f1: " << f1.get() << endl;
		cout << "Result of f2: " << f2.get() << endl;
		cout << "Result of f3: " << f3.get() << endl;

		return 0;
	}
}

namespace ActiveObject::Example3
{
	void Message(int id, string& message, promise<string>& prom)
	{
		stringstream ss;
		ss << "Message ID:" << id << ", [" << message << "]\n";
		prom.set_value(ss.str());
	}

	int Tests::run()
	{
		// This example of Active Object uses promise and future classes
		// The thread run by Active Object will set results to the promises
		// After all messages have processed, 
		// the main thread can retrieve the results stored in the promise.
		ActiveObject	ao;
		vector<string>	str = { "Beef", "Pork", "Chicken", "Fish" };
		vector<promise<string>>	promises(4);

		for (int i = 0; i < 4; i++)
		{
			ao.enqueue(std::bind(Message, i, str[i], std::ref(promises[i])));
		}

		// get the results
		for (auto& p : promises)
		{
			future	f = p.get_future();
			cout << f.get();
		}

		return 0;
	}

}