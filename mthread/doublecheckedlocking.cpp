#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>

#include "doublecheckedlocking.h"


using std::cout;
using std::endl;
using std::thread;
using std::mutex;
using std::unique_lock;
using std::lock_guard;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::weak_ptr;
using std::atomic;


namespace Example1
{
	// a singleton class that uses double-checked locking
	class Singleton
	{
	public:
		// a public method to access the instance
		static Singleton* getInstance()
		{
			// 1. CHECK,  first check without locking
			if (!m_instance)
			{
				// 2. LOCK, lock the mutex
				unique_lock<mutex> ul(m_mutex);
				// 3. CHECK, again
				if (!m_instance)
				{
					// then, finally create the instance
					m_instance = make_unique<Singleton>();
				}
			}
			return m_instance.get();
		}
	private:
		static unique_ptr<Singleton>	m_instance;
		static mutex	m_mutex;
	};

	// initialize the static members
	unique_ptr<Singleton> Singleton::m_instance = nullptr;
	mutex	Singleton::m_mutex;

	// a function that calls the getInstance() method
	void func()
	{
		static mutex	coutmtx;
		Singleton* s = Singleton::getInstance();
		{
			unique_lock<mutex>	ul(coutmtx);
			cout << "Thread " << std::this_thread::get_id() << " got instance" << s << endl;
		}
	}

	int Run_DoubleCheckedLocking()
	{
		// create threads that calls the func()
		thread	th1(func);
		thread	th2(func);
		thread	th3(func);

		th1.join();
		th2.join();
		th3.join();

		return 0;
	}
}

namespace Example2
{
	// a singleton class that uses double-checked locking
	class Singleton
	{
	public:
		// a public method to access the instance
		static Singleton* getInstance()
		{
			// Same meaning as Example1.
			// The static variable would be initialised once.
			// Even if multiple threads call getInstance,
			// they would use the same value.
			static Singleton singleton;
			return &singleton;
		}
	private:
		static mutex	m_mutex;
	};

	// a function that calls the getInstance() method
	void func()
	{
		static mutex	coutmtx;
		Singleton* s = Singleton::getInstance();
		{
			unique_lock<mutex>	ul(coutmtx);
			cout << "Thread " << std::this_thread::get_id() << " got instance" << s << endl;
		}
	}

	int Run_DoubleCheckedLocking()
	{
		// create threads that calls the func()
		thread	th1(func);
		thread	th2(func);
		thread	th3(func);

		th1.join();
		th2.join();
		th3.join();

		return 0;
	}

}


int Run_DoubleCheckedLocking()
{
	Example1::Run_DoubleCheckedLocking();
	Example2::Run_DoubleCheckedLocking();
	return 0;
}
