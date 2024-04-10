#include "pch.h"

#include "barrier.h"


namespace Barrier::Basic1
{
	void task(Barrier& barrier)
	{
		sleep_for(milliseconds(1000));
		cout << "Thread[" << get_id() << "] reached the barrier\n";
		barrier.wait();
		cout << "Thread[" << get_id() << "] passed the barrier\n";
	}

	int Tests::run()
	{
		size_t numThreads = 5;
		Barrier barrier(numThreads);

		vector<thread> threads;
		for (auto i = 0; i < numThreads; ++i)
		{
			threads.push_back(thread(task, std::ref(barrier)));
		}

		for (auto& t : threads)
		{
			t.join();
		}

		return 0;
	}
}

namespace Barrier::Basic2
{
	int Tests::run()
	{
		auto numThreads = 5;
		shared_ptr<Barrier> barrier = make_shared<Barrier>(numThreads);
		vector<thread> threads;

		for (auto i = 0; i < numThreads; ++i)
		{
			threads.emplace_back([&, i]
				{
					Worker w(i, barrier);
					w.assembleComponent();
				});
		}

		for (auto& t : threads)
		{
			t.join();
		}

		cout << "All workers have assembled their components. Proceeding to assemble the product.\n";
		cout << "Product assembly completed\n";

		return 0;
	}

}