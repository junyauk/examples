#include "pch.h"

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>
#include <condition_variable>
#include "threadpermessage.h"

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::unique_lock;
using std::queue;
using c_v = std::condition_variable;

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::chrono::seconds;

namespace Example1
{
	class Helper
	{
	public:
		void handle(const string& filePath, const string& contents)
		{
			auto id = std::this_thread::get_id();
			lock_guard<mutex> lg(m_mutex);
			cout << "TID: " << id << filePath << " : " << contents << endl;
		}
	private:
		static mutex	m_mutex;
	};

	mutex Helper::m_mutex;

	class Host
	{
	public:
		void request(const string& filePath, const string& contents)
		{
			thread([&]()
				{
					Helper helper;
					helper.handle(filePath, contents);
				}).detach();	// calling detach() is important for this design pattern.

				// Note:
				// This thread is detached when created.
				// this means that Host can't detect
				// if something happened in the thread.
				// You can run threads that thread safe is guaranteed,
				// or Host doesn't need to care about happend in the thread.
				//
				// So the thread needs to have the following conditions.
				//  1. Errors will never happen.
				//  2. No deadlock (thread safe is guaranteed)
				//  3. The thread can work asynchronously, and fully independent.
		}
	};

	void Run_ThreadPerMessage()
	{
		Host host;
		host.request("path_a", "contents_a");
		host.request("path_b", "contents_b");
		host.request("path_c", "contents_c");

		sleep_for(seconds(3));
	}
};


namespace Example2
{
	// A message class that represents a request or a task
	class Message
	{
	public:
		Message(const int id, const string& data)
			: m_id(id)
			, m_data(data)
		{}

		int getID() const { return m_id; }
		string getData() const { return m_data; }

	private:
		int m_id;
		string m_data;
	};

	// A message queue class that stores messages in a thread safe way
	class MessageQueue
	{
	public:
		void push(Message msg)
		{
			lock_guard<mutex> lg(m_mutex);
			m_queue.push(msg);
			m_cv.notify_one();
		}

		Message pop()
		{
			unique_lock<mutex> ul(m_mutex);
			// Wait until something was pushed to m_queue.
			m_cv.wait(ul, [this]() { return !m_queue.empty(); });
			Message msg = m_queue.front();
			m_queue.pop();
			return msg;
		}

		bool empty()
		{
			unique_lock<mutex> ul(m_mutex);
			return m_queue.empty();
		}

	private:
		queue<Message> m_queue;
		mutex m_mutex;
		c_v m_cv;
	};

	// A function that prints id and data
	static void printMessage(Message msg)
	{
		cout << "Handling message(ID: " << msg.getID() << ", Data: " << msg.getData() << endl;
	}

	// A function that creates a new thread for each message in the queue and print it.
	static void threadPerMessage(MessageQueue& mq)
	{
		while (!mq.empty())
		{
			Message msg = mq.pop();
			thread th(printMessage, msg); // For printing
			// Note
			// As Thread Per Message multi threading design pattern,
			// detaching the thread is important
			th.detach();	// Detach the thread so it can run INDEPENDENTLY
		}
	}

	static void Run_ThreadPerMessage()
	{
		MessageQueue mq;

		mq.push(Message(1, "Hellow"));
		mq.push(Message(2, "World"));
		mq.push(Message(3, "Bye"));
		threadPerMessage(mq);

		sleep_for(seconds(3));

		return;
	}

};

int Run_ThreadPerMessage()
{
// Note:
// After detach() was called, the threads might not be able to 
// finish before finishing the main thread.
// This means that the literal values like "Hellow" are not available
// after the main thread finished.
// Then unexpected behaviour would happen.
// So do not run the following tests.
//	Example1::Run_ThreadPerMessage();
//	Example2::Run_ThreadPerMessage();

	return 0;
}