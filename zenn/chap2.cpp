#include "pch.h"
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <exception>
#include <map>
#include <mutex>

using std::cout;
using std::endl;
using std::thread;
using std::vector;

//*******************************************
void task_chap2_1()
{
	cout << "Worker thread task" << endl;
}

void chap2_1()
{
	cout << "chap2_1 ->" << endl;
	thread th{ task_chap2_1 };

	cout << "main thread" << endl;

	th.join();

	cout << "chap2_1 <-" << endl;
}

//*******************************************
void chap2_2()
{
	cout << "chap2_2 ->" << endl;

	vector<thread> thds;

	for (int id = 0; id < 5; id++)
	{
		thds.emplace_back(
			[&,	// others are as reference
			id]	// only id is as value
			{
				cout << "worker thread #" << id << endl;
			}
		);
	}

	for (auto &t : thds)
	{
		t.join();
	}

	cout << "chap2_2 <-" << endl;
}

//*******************************************

class Adder
{
public:
	Adder(int a, int b) : m_a(a), m_b(b), m_r(0)
	{}
	int result()
	{
		return m_r;
	}
	void process()
	{
		m_r = m_a + m_b;
	}
private:
	int m_a;
	int m_b;
	int m_r;
};

void chap2_3()
{
	cout << "chap2_3 ->" << endl;

	Adder ad(1, 2);

	// Create a thread

#if 0
	// Solution 1
	// non-static class method can be called
	// with pointer(or ref) of the instance
	thread th{ &Adder::process, &ad };
#else
	// Solution 2 (better)
	thread th{
		[&]	// variables are passed as ref
		{
			ad.process(); // call the class method in the lambda formula
		}
	};
#endif

	cout << "main thread" << endl;

	th.join();

	cout << "chap2_3 <-" << endl;
}

//*******************************************

using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;

// Note:
// only pass by value param is allowed for thread
void task_chap2_4(shared_ptr<int>sp, unique_ptr<int> up)
{
	cout << "thread ->" << endl;

	cout
		<< *sp << " " << sp.use_count() << endl  // 100 2
		<< *up << endl;                          // 200

	cout << "thread <-" << endl;
}


//*******************************************
void chap2_4()
{
	cout << "chap2_4 ->" << endl;

	shared_ptr<int> sp = make_shared<int>(100);
	unique_ptr<int> up = make_unique<int>(200);

	thread thd
	{
		task_chap2_4,	// thread func

		sp,				// sp is copied
		move(up)		// up is moved
	};

	// Note:
	// After move(up), up will be empty.

	cout << "main thread" << endl;

	thd.join();

	cout << "chap2_4 <-" << endl;
}

//*******************************************
using std::string;
using std::ref;
struct Item
{
	string name;
	int price;
};

void bake(Item& item)
{
	cout << "bake ->" << endl;
	item.name = "Baked-" + item.name;
	item.price *= 2;
	cout << "bake <-" << endl;
}


void chap2_5()
{
	cout << "chap2_5 ->" << endl;
	Item item{ "Apple", 150 };

	// build error
	//	thread thd{ bake, item }; // item can't be passed
	thread thd{ bake, ref(item) }; // item can be passed as ref using ref().


	cout << "main thread" << endl;

	thd.join();

	cout << "chap2_5 <-" << endl;
}

//*******************************************


string greeting()
{
	return "hello";
}

void chap2_6()
{
	cout << "chap2_6 ->" << endl;

	string ret;

	thread thd
	{
		[&] {ret = greeting(); }
	};

	cout << "main thread" << endl;

	thd.join();

	cout << "chap2_6 <-" << endl;
}

//*******************************************

using std::exception_ptr;

int chap2_7_task()
{
// Some exceptions and there are more
//	throw std::logic_error("logic error");				defined in stdexcept
//	throw std::runtime_error("runtime error");			defined in stdexcept
//	throw std::bad_alloc();								defined in new
//	throw std::bad_cast();								defined in typeinfo
//	throw std::out_of_range("out of range");			defined in stdexcept
//	throw std::invalid_argument("invalid argument");	defined in stdexcept
	throw std::exception("general exception");		//  defined in exception.h
	return 1;
}

void chap2_7()
{
	cout << "chap2_7 ->" << endl;

	int result = 0;
	exception_ptr ep;
	string ret;

#if 1
	// OK
	thread thd
	{
		[&]	// ref all local variables
		{
			// !! IMPORTANT !!
			// exceptionsneed to be cought in the thread
			// if a thrown exception was not cought, terminate() will be called
			try
			{
				result = chap2_7_task();
			}
			catch(...)
			{
				ep = std::current_exception();
			}
		}
	};
#else
	// NG
	// terminate() will be called when an exception was thrown,
	// then this program will be terminated after thd.join().
	thread thd{ chap2_7_task };
#endif

	cout << "main thread" << endl;

	thd.join();

	if (ep)
	{
		// Do something for the exception
	}
	else
	{
		// OK
	}

	cout << "chap2_7 <-" << endl;
}

//*******************************************
using std::map;
using std::mutex;
using std::function;
using std::lock_guard;

void chap2_8_task(string &name)
{
	cout << "chap2_8_task ->" << endl;
	cout << "  task name: " << name << endl;
	name += "(done)";
	cout << "chap2_8_task <-" << endl;
}

mutex mtx_guard;

thread fork_task(map<thread::id, string> tbl, function<void(string&)> task, string &name)
{
	thread thd{ move(task), ref(name) };
	{
		lock_guard<mutex> lg(mtx_guard);
		tbl[thd.get_id()] = name;
	}
	return thd;
}

void chap2_8()
{
	cout << "chap2_8 ->" << endl;

	map<thread::id, string> thrd_tbl;
	vector<thread> thrds;
	vector<string> names;
	names.push_back("1st");
	names.push_back("2nd");
	names.push_back("3rd");
	names.push_back("4th");
	thrds.emplace_back(fork_task(thrd_tbl, chap2_8_task, names[0]));
	thrds.emplace_back(fork_task(thrd_tbl, chap2_8_task, names[1]));
	thrds.emplace_back(fork_task(thrd_tbl, chap2_8_task, names[2]));
	thrds.emplace_back(fork_task(thrd_tbl, chap2_8_task, names[3]));

	for (auto& t : thrds)
	{
		t.join();
	}

	cout << "chap2_8 <-" << endl;
}
