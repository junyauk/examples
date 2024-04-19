#pragma once
#ifndef READLOCKWRITELOCK_H_INCLUDED
#define READLOCKWRITELOCK_H_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <memory>


int Run_ReadLockWriteLock();

using std::cout;
using std::endl;
using std::stringstream;
using std::string;
using std::vector;
using std::mutex;
using std::unique_ptr;
using std::make_unique;
using std::shared_mutex;
using std::unique_lock;
using std::shared_lock;

namespace ReadLockWriteLock::Singleton::Basic1
{
	class SafeOut
	{
	public:
		static void out(string str)
		{
			unique_lock<mutex> ul(m_mutex);
			cout << str;
		}
	private:
		static mutex m_mutex;
	};

	class Singleton
	{
	public:
		// Disable copy constructor and assignment operator
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

		static Singleton* getInstance()
		{
			if (m_instance == nullptr)
			{
				unique_lock<shared_mutex> ul;
				m_instance.reset(new Singleton());
			}
			return m_instance.get();
		}

		int readData()
		{
			shared_lock<shared_mutex> sl(m_rwLock);
			return m_data;
		}

		void writeData(int data)
		{
			unique_lock<shared_mutex> sl(m_rwLock);
			m_data = data;
		}

	private:
		// Disable constructor
		Singleton() : m_data(0) {}

		static unique_ptr<Singleton> m_instance;
		static shared_mutex m_rwLock;
		int m_data;
	};

	class Tests
	{
	public:
		int run();
	};

}

namespace ReadLockWriteLock::Singleton::Basic2
{
	class SafeOut
	{
	public:
		static void out(string str)
		{
			unique_lock<mutex> ul(m_mutex);
			cout << str;
		}
	private:
		static mutex m_mutex;
	};

	class Logger
	{
	public:
		// Disable copy constructor and assignment operator
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		static Logger* getInstance()
		{
			if (m_instance == nullptr)
			{
				unique_lock<shared_mutex> ul(m_rwLock);
				m_instance.reset(new Logger());
			}
			return m_instance.get();
		}

		vector<string> read()
		{
			shared_lock<shared_mutex> sl(m_rwLock);
			return m_logs;
		}

		void add(const string& log)
		{
			unique_lock<shared_mutex> ul(m_rwLock);
			m_logs.push_back(log);
		}

	private:
		static unique_ptr<Logger> m_instance;
		static shared_mutex m_rwLock;
		vector<string> m_logs;

		// Disable constructor
		Logger() {};
	};

	class Tests
	{
	public:
		int run();
	};
}

#endif // READLOCKWRITELOCK_H_INCLUDED