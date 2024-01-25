#include "pch.h"
#include <future>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>

using std::promise;
using std::future;
using std::shared_future;
using std::future_status;

using std::async;
using std::packaged_task;

using std::runtime_error;
using std::thread;

using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::chrono::milliseconds;

using std::string;
using std::stringstream;

using std::cout;
using std::endl;

#include "futurepromise.h"

namespace Example1
{
	void Promise(promise<int> p)
	{
		sleep_for(seconds(1));
		p.set_value(0);
		// Note:
		// If this thread needs to throw exception, enable this line.
		//p.set_exception(std::make_exception_ptr(runtime_error("error")));
	}

	void Run_FuturePromise_Example1()
	{
		// future can be retrived from promise.
		// The feature is used for collecting value in the promise.
		promise<int> p;
		future<int> f = p.get_future();

		try
		{
			// The promise is passed to the thread,
			// and the thread will update the value in the promise.
			thread th(Promise, std::move(p));

			future_status st = f.wait_for(seconds(2));
			if (st != future_status::timeout)
			{
				int ret = f.get();
				// Note:
				// After once the value of future was retrieved,
				// the value in the future is undefined.
				// f.get();
			}
			th.join();
		}
		catch (const std::exception &e)
		{
			cout << "Exception from the thread: " << e.what() << endl;
		}
		catch (...)
		{
			cout << "Exception from the thread" << endl;
		}
		return;
	}
}

namespace Example2
{
	void Promise(promise<int> p)
	{
		sleep_for(seconds(1));
		p.set_value(1);
	}

	void Get(shared_future<int> sf)
	{
		cout << "get(sf: " << sf.get() << " )\n";
	}

	void Run_FuturePromise_Example2()
	{
		promise<int> p;
		future<int> f = p.get_future();
		shared_future<int> sf = f.share();

		thread th1(Promise, std::move(p));
		thread th2(Get, sf);
		thread th3(Get, sf);

		th1.join();
		th2.join();
		th3.join();
	}
}

namespace Example3
{
	int Async(int num)
	{
		cout << "Async = " << std::this_thread::get_id() << endl;
		return num;
	}

	void Run_Async()
	{
		cout << "Main = " << std::this_thread::get_id() << endl;
		future<int> f = async(Async, 100);
		cout << f.get() << endl;

		f = async(std::launch::deferred, Async, 1000);
		cout << f.get() << endl;
	}

}

namespace Example4
{
	int Async(int num)
	{
		cout << "Async = " << std::this_thread::get_id() << endl;
		sleep_for(milliseconds(num));
		return num;
	}

	string taskThread(int num)
	{
		stringstream ss;
		ss << "Thread ID: " << std::this_thread::get_id() << " - " << num;
		sleep_for(milliseconds(num));
		return ss.str();
	}

	void Run_Packaged_Tasks()
	{
		packaged_task<int(int)> task(Async);
		future<int> f = task.get_future();

		task(10);

		cout << f.get() << endl;

		packaged_task<string(int)> task2(taskThread);
		future<string> f2 = task2.get_future();

		task2(20);

		cout << f2.get() << endl;
	}
}

namespace Example5
{
	void Run_SetException()
	{
		promise<int> p;
		future<int> f = p.get_future();

		thread th([&p]()
			{
				cout << "Start thread ->\n";
				try
				{
					cout << "Throw an exception \"Example\" from the thread\n";
					// Code that may throw exception...
					throw std::runtime_error("Example");
				}
				catch (...)
				{
					cout << "The thrown exception was cought\n";
					try
					{
						cout << "set_exception of promise\n";
						// Store anything throw in the promise
						p.set_exception(std::current_exception());
						// or throw a custom exception instead
						// p.set_exception(std::make_exception_ptr(MyException("mine")));
					}
					catch (...)
					{
						cout << "Exception has been thrown...\n";
					}
				}
				cout << "End thread <-\n";
			}
		);

		try
		{
			cout << "call f.get()\n";
			cout << f.get() << endl;
		}
		catch (const std::exception& e)
		{
			cout << "Exception from the thread: " << e.what() << endl;
		}
		th.join();

		return;
	}

}

int Run_FuturePromise()
{
	Example1::Run_FuturePromise_Example1();
	Example2::Run_FuturePromise_Example2();
	Example3::Run_Async();
	Example4::Run_Packaged_Tasks();
	Example5::Run_SetException();

	return 0;
}

