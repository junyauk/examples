#include "pch.h"

#include <iostream>
#include <thread>
#include <functional>
#include <atomic>
#include <chrono>
#include <cassert>

using std::cout;
using std::endl;
using std::thread;
using std::function;
using std::atomic;
using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::chrono::milliseconds;

namespace Example1
{
	class Worker
	{
	public:
		Worker()
			: m_done(false)
		{
			m_th = thread(&Worker::run, this);
		}

		~Worker()
		{
			m_th.join();
		}

		void terminate()
		{
			// termination can be set from outside
			m_done = true;
		}

		void run()
		{
			while (!m_done)
			{
				// do some work
				cout << "working..." << endl;
				// sleep for a while
				sleep_for(seconds(1));
			}
			// do some clean up
			cout << "terminating..." << endl;
		}

	private:
		thread	m_th;
		atomic<bool>	m_done;
	};

	int Run_TwoPhaseTermination()
	{
		// create a worker object
		Worker wk;

		// wait for some time
		sleep_for(seconds(5));

		// request the worker to terminate
		wk.terminate();

		// exit the main thread
		return 0;
	}
}

namespace Example2
{
	class Worker
	{
		// This class has got a thread,
		// The thread is created at constructor, and terminated at destructor
		// The thread has got an infinite loop, and it only exits
		// when a termination flag was set at destructor before calling join().
	public:
		Worker()
			: m_isTerminationRequested(false)
		{
			// create a thread
			m_th = thread(m_main);
		}
		~Worker()
		{
			cout << "Woker destructor ->\n";
			assert(m_th.joinable());

			// 1. set the termination flag for letting the thread know it
			m_isTerminationRequested = true;
			// 2. wait for the thread
			m_th.join();
			cout << "Woker destructor <-\n";
		}
	private:
		function<void()>	m_main = [this] // for accessing members, you can use "this" or '&'
			{
				while (1)
				{
					cout << "thread is working...\n";
					// do something
					sleep_for(milliseconds(500));
					if (m_isTerminationRequested)
					{
						// it might need some tidying up tasks...
						sleep_for(seconds(5));
						cout << "thread is exiting...\n";
						return;
					}
				}
			};

		thread	m_th;
		atomic<bool>	m_isTerminationRequested;
	};

	int Run_TwoPhaseTermination()
	{
		{
			Worker	wk;
			sleep_for(seconds(10));
		}
		return 0;
	}
}

int Run_TwoPhaseTermination()
{
	// create a worker object
	Example1::Run_TwoPhaseTermination();
	Example2::Run_TwoPhaseTermination();

	return 0;
}


