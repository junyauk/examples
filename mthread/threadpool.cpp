#include "pch.h"

#include <sstream>
#include "threadpool.h"

using std::stringstream;


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

