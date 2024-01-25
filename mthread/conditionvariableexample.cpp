#include "pch.h"
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>

#include "conditionvariableexample.h"

using std::cout;
using std::endl;
using std::string;
using std::mutex;
using std::unique_lock;
using std::thread;
using c_v = std::condition_variable;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

void Run_ConditionVariableExample_Simple()
{
	mutex mtx;
	c_v cv;

	// Create a sub thread with the lambda formula
	thread th(
		[&]()
		{
			cout << "Sub thread ->\n";
			unique_lock<mutex> ul(mtx);
			cv.wait(ul);
			cout << "Sub thread <-\n";
		}
	);

	// Main thread
	sleep_for(milliseconds(1)); // This expect 1 second would be enough
	cv.notify_one();	// Letting the sub thread know it's time to wake up
	th.join();			// Wait for the sub thread finishes

	// Note:
	// This function expect cv.wait() runs first, and cv.notify_one() comes next.
	// However, if the sub thread took long time before calling cv.wait(),
	// this assumption would not work.
	// Then cv.notify_one() would run first, and cv.wait() runs next.
	// This means that cv.wait() would never return. This is a dead lock.	
}

void Run_ConditionVariableExample_DeadLock()
{
	// Basically this function is the same as _Simple() version.
	// The difference between them is processing time in the sub thread.
	// Main thread is still expecting the sub thread would finish quickly in 1 milli second.
	// However this function takes longer than _Simple() one.
	// Then cv.notify_one() will be called first, and cv.wait() is called next.
	// This is the deadlock version.
	mutex mtx;
	c_v cv;

	thread th(
		[&]()
		{
			cout << "Sub thread ->\n";
			// Note:
			// This function sleep 1 milli second.
			// It breaks the asumption that this sub thread will finish quickly.
			sleep_for(milliseconds(1));

			unique_lock<mutex> ul(mtx);
			cv.wait(ul);
			cout << "Sub thread <-\n";
		}
	);

	// Main thread
	sleep_for(milliseconds(1));
	cv.notify_one();
	th.join();
}


void Run_ConditionVariableExample_Solved()
{
	mutex mtx;
	c_v cv;

	bool isMainReady = false;
	bool isSubReady = false;

	thread th(
		[&]()
		{
			cout << "Sub thread ->\n";
			sleep_for(milliseconds(2));
			{
				unique_lock<mutex> ul(mtx);
				isSubReady = true;
				if (isMainReady)
				{
					cout << "[Sub] wakes main up\n";
					cv.notify_one();
				}
				else
				{
					cout << "[Sub] sleeps until main is ready\n";
					while (!isMainReady)
						cv.wait(ul);
				}
			}
			cout << "Sub thread <-\n";
		}
	);

	// Main thread
	sleep_for(milliseconds(1));
	{
		unique_lock<mutex> ul(mtx);
		isMainReady = true;
		if (isSubReady)
		{
			cout << "[Main] wakes sub up\n";
			cv.notify_one();
		}
		else
		{
			cout << "[Main] sleeps until sub is ready\n";
			while (!isSubReady)
				cv.wait(ul);
		}
	}
	cout << "[Main] started\n";

	th.join();

}


int Run_ConditionVariable()
{
//	Run_ConditionVariableExample_Simple();
//	Run_ConditionVariableExample_DeadLock();
	Run_ConditionVariableExample_Solved();
	return 0;
}
