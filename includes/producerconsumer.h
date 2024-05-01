#pragma once
#ifndef PRODUCERCONSUMER_H_INCLUDED
#define PRODUCERCONSUMER_H_INCLUDED
#include "general.h"

namespace ProducerConsumer::Basic1
{
	// This example runs 2 threads. One of them are in Producer class, another one is in Consumer class.
	// The Producer class keeps pushing int values to the Queue class.
	// The Consumer class keeps popping the int values that Producer class has pushed in the Queue class.
	// The Queue class exclusively control accesses from both Producer and Consumer.

	// This class derives std::queue
	class Queue : public queue<int>
	{
	public:
		size_t size() { unique_lock<mutex> ul(m_mutex); return queue<int>::size(); }
		bool empty() { unique_lock<mutex> ul(m_mutex); return queue<int>::empty(); }
		void push(const int& val);
		int pop();
		void stop() { m_stop = true; m_cv.notify_all(); }
	private:
		const int MAX_QUEUE_SIZE = 10;
		atomic<bool> m_stop{ false };
		cv m_cv;
		mutex m_mutex;
	};

	// This class runs a thread that keeps pushing int values
	class Producer
	{
	public:
		Producer(Queue& q);
		~Producer();
	private:
		void threadFunc();

		atomic<bool> m_stop{ false };
		Queue& m_queue;
		thread m_thread;
	};

	// This class runs a thread that keeps popping int values
	class Consumer
	{
	public:
		Consumer(Queue& q);
		~Consumer();
	private:
		void threadFunc();

		atomic<bool> m_stop{ false };
		Queue& m_queue;
		thread m_thread;
	};
}

namespace ProducerConsumer::Basic2
{
	// This example uses Producer class and Consumer class
	// They share a single Queue.
	// Both classes run multiple threads.
	// Threads created by Producer class will create an Item with a string,
	// and push it to the queue.
	// Threads created by Consumer class will pop items from the queue,
	// and run the item for outputting the string.
	// A sync object is used in the Queue class only.
	// Producer and Consumer classes don't have a sync mechanism.

	class Item
	{
	public:
		void set(string const& str) { m_str = str; }
		string operator()() { return "Item( made by " + m_str + " )"; }
	private:
		string m_str;
	};

	class Queue
	{
	public:
		void push(Item const& item);
		Item pop();
		bool isEmpty() const { return m_queue.empty(); }
	private:
		queue<Item>	m_queue;
		mutex		m_mutex;
		cv			m_cv;
	};

	class Producer
	{
	public:
		Producer(Queue& queue, int const& numThreads);
		~Producer();
	private:
		void worker(int const& threadNumber);
		Queue& m_queue;
		atomic<bool>	m_stop;
		vector<thread>	m_threads;
	};

	class Consumer
	{
	public:
		Consumer(Queue& q, int const& numThreads);
		~Consumer();
	private:
		void worker(int const& threadNumber);
		Queue& m_queue;
		atomic<bool>	m_stop;
		mutex			m_mutex;
		vector<thread>	m_threads;
	};
}


namespace ProducerConsumer::Printer1
{
	template <typename T>
	class Queue
	{
	public:
		Queue(int size) : m_size(size)
		{
			cout << "Queue constructor\n";
		}

		bool put(T&& data) // data is rref, so will be moved
		{
			cout << "put ->\n";
			if (m_size <= m_deque.size())
			{
				return false;
			}
			// Use emplace_back(), it doesn't call copy constructor.
			// It's quicker than push_back()
			m_deque.emplace_back(std::move(data));
			cout << "put <-\n";
			return true;
		}

		bool put(const T& data)	// data is lref, so will be copied
		{
			cout << "put ->\n";
			if (m_size <= m_deque.size())
			{
				return false;
			}
			// Use emplace_back(), it doesn't call copy constructor.
			// It's quicker than push_back()
			m_deque.emplace_back(data);
			cout << "put <-\n";
			return true;
		}

		bool get(T& data)
		{
			cout << "get ->\n";
			if (m_deque.empty())
			{
				return false;
			}
			// data in the queue can be discarded.
			// so move the data
			data = std::move(m_deque.front());
			m_deque.pop_front();
			cout << "get <-\n";
			return true;
		}

		bool empty()
		{
			return m_deque.empty();
		}

	private:
		int m_size;
		deque<T> m_deque;
	};

	class Printer
	{
	public:
		Printer(int threadCount, int queueSize);
		~Printer();
		bool append(string&& str); // str is rref, move
		bool append(const string& str); // str is lref, copy 

	private:
		// This thread function "m_main" is defined using std::function with lambda formula
		// The function's arguments are corresponding with 
		// the actual parameters of the specified lambda
		// function<void(int) [](int num){}
		// function<double(float, int) [](float num1, int num2){ return (double)num1;}
		std::function<void(int)> m_main = // As thread function, the return value is void
			// lambda formula
			[&, this](int num)	// specify '&' for using private members. you can also specify "this" as well
			{
				auto th_id = std::this_thread::get_id();
				cout << "m_main thread ID: " << th_id << "->\n";
				while (1)
				{
					string str;
					{
						// Printer has got only one queue "m_queue".
						// So accessing the queue, you need to make sure it's thread safe.
						unique_lock<mutex> ul(m_mutex);
						cout << "mutex locked ID: " << th_id << endl;
						// Now all threads accessing m_mutex will be waited except this thread who locked.
						while (m_queue.empty())
						{
							if (m_isTerminationRequested)
							{
								cout << "m_main thread terminated <-\n";
								return;
							}
							// m_cv is signaled when something has been added to the m_queue.
							m_cv.wait(ul);
							// Now something has been added (someone has called m_cv.notify_all())
							// and ready to call get().
						}
						const bool result = m_queue.get(str);
						assert(result);
						// Now the mutex is unlocked after this
						cout << "mutex unlocked ID: " << th_id << endl;
					}
					cout << "num = " << num << " " << str << endl;
				}
				cout << "m_main thread ID: " << th_id << "<-\n";
			};

		int m_isTerminationRequested;
		mutex m_mutex;
		cv m_cv;
		Queue<string> m_queue;
		vector<thread> m_threads; // thread should be initialized finally.
	};

}

namespace ProducerConsumer::TaskQueue
{
	// This example has got a queue class named TaskQueue.
	// It has got a queue in it.
	// TaskQueue::addTask() and getTask() methods are used for accessing the queue.
	// Then a mutex is used for accessing the queue.
	// addTask() can queue a task any time,
	// but getTask() can get only when something in the queue.
	// So the caller who called getTask() will wait for that getTask() retrieves a task.

	class TaskQueue
	{
	public:
		void addTask(int const& val);
		int getTask();
	private:
		queue<int>	m_queue;
		mutex		m_mutex;
		cv			m_cv;
	};
}

namespace ProducerConsumer::Windows1
{
	class Queue
	{
	public:
		Queue()
		{
			InitializeCriticalSection(&m_cs);
		}
		~Queue()
		{
			DeleteCriticalSection(&m_cs);
		}
		size_t size()
		{
			size_t s = 0;
			EnterCriticalSection(&m_cs);
			s = m_queue.size();
			LeaveCriticalSection(&m_cs);
			return s;
		}
		BOOL empty()
		{
			BOOL e = FALSE;
			EnterCriticalSection(&m_cs);
			e = m_queue.empty();
			LeaveCriticalSection(&m_cs);
			return e;
		}

		void push(DWORD const& val)
		{
			EnterCriticalSection(&m_cs);
			m_queue.push(val);
			LeaveCriticalSection(&m_cs);
		}
		DWORD pop()
		{
			DWORD ret = 0;
			EnterCriticalSection(&m_cs);
			if (!m_queue.empty())
			{
				ret = m_queue.front();
				m_queue.pop();
			}
			LeaveCriticalSection(&m_cs);
			return ret;
		}

	private:
		CRITICAL_SECTION m_cs;
		queue<DWORD> m_queue;
	};

	class ThreadRunner
	{
	public:
		ThreadRunner()
		{
			m_hThread = CreateThread(NULL, 0, &ThreadRunner::BootThreadFunc, this, 0, NULL);
			if (!m_hThread)
			{
				auto lastError = GetLastError();
				cerr << "Creating thread failed: (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
			}
		}
		virtual ~ThreadRunner()
		{
			if (m_hThread)
			{
				WaitForSingleObject(m_hThread, INFINITE);
				CloseHandle(m_hThread);
			}
		}
	private:
		static DWORD WINAPI BootThreadFunc(LPVOID lpParam)
		{
			ThreadRunner* pDerived = static_cast<ThreadRunner*>(lpParam);
			pDerived->ThreadFunc();
			return 0;
		}
		virtual void ThreadFunc() {}
		virtual void stop() {}

		HANDLE m_hThread;
	};

	class Producer : public ThreadRunner
	{
	public:
		Producer(shared_ptr<Queue> q)
			: m_queue(q){}
		~Producer()
		{
			m_stop = true; // Let the thread knows terminating.
		}
	private:
		shared_ptr<Queue> m_queue; // Note: the Queue needs to be initialized first.
		BOOL m_stop = FALSE;
		Random32 m_random;
		void ThreadFunc() override;
	};

	class Consumer : public ThreadRunner
	{
	public:
		Consumer(shared_ptr<Queue> q)
			: m_queue(q){}
		~Consumer()
		{
			m_stop = true; // Let the thread knows terminating.
		}
	private:
		shared_ptr<Queue> m_queue; // Note: the Queue needs to be initialized first.
		BOOL m_stop = FALSE;
		void ThreadFunc() override;
	};
}


#endif // PRODUCERCONSUMER_H_INCLUDED
