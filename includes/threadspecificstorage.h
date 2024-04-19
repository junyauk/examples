#pragma once
#ifndef THREADSPECIFICSTORAGE_H_INCLUDED
#define THREADSPECIFICSTORAGE_H_INCLUDED

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <map>
#include <cassert>
#include <fstream>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::to_string;
using std::thread;
using std::mutex;
using std::unique_lock;
using std::stringstream;
using std::unique_ptr;
using std::make_unique;
using std::map;
using std::ofstream;

int Run_ThreadSpecificStorage();

namespace ThreadSpecificStorage::FactoryMethod::Basic1
{
	class SafeOut
	{
	public:
		static void out(const string& str)
		{
			unique_lock<mutex> ul(m_mutex);
			cout << str;
		}
	private:
		static mutex m_mutex;
	};

	// My object class
	class MyObject
	{
	public:
		MyObject(int id) :m_id(id) {}
		unique_ptr<MyObject> clone()
		{
			return make_unique<MyObject>(*this);
		}
		void print()
		{
			stringstream ss;
			ss << "MyObject: " << m_id << " belongs to thread " << std::this_thread::get_id() << endl;
			SafeOut::out(ss.str());
		}
	private:
		int m_id;
	};

	// Factory class
	class MyObjectFactory
	{
	public:
		static unique_ptr<MyObject> createObject(int id)
		{
			return make_unique<MyObject>(id);
		}
	};

	// Thread specific storage class
	class ThreadLocal
	{
	public:
		MyObject* get()
		{
			unique_lock<mutex> m_mutex;
			auto it = m_storage.find(std::this_thread::get_id());
			if (it != m_storage.end())
			{
				return it->second.get();
			}
			return nullptr;
		}
		void set(unique_ptr<MyObject> obj)
		{
			unique_lock<mutex> ul(m_mutex);
			m_storage[std::this_thread::get_id()] = std::move(obj);
		}
	private:
		mutex m_mutex;
		map<thread::id, unique_ptr<MyObject>> m_storage;
	};

	class Tests
	{
	public:
		int run();
	};
}


namespace ThreadSpecificStorage::FactoryMethod::Basic2
{
	class SafeOut
	{
	public:
		static void out(const string& str)
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
		Logger(int id) : m_threadId(id)
		{
			string filename = "log_thread_ "
							+ to_string(m_threadId)
							+ "_"
							+ to_string(reinterpret_cast<uintptr_t>(this))
							+ ".txt";
			m_file.open(filename);
		}

		Logger(const Logger& other) :m_threadId(other.m_threadId)
		{
			string filename = "log_thread_ " 
							+ to_string(m_threadId) 
							+ "_"
							+ to_string(reinterpret_cast<uintptr_t>(this))
							+ ".txt";
			m_file.open(filename);
		}

		~Logger()
		{
			m_file.close();
		}
		void log(const string& message)
		{
			m_file << message << endl;
		}

		unique_ptr<Logger> clone()
		{
			return make_unique<Logger>(*this);
		}
	private:
		ofstream m_file;
		int m_threadId;
	};

	// Factory class
	class LoggerFactory
	{
	public:
		static unique_ptr<Logger> createLogger(int id)
		{
			return make_unique<Logger>(id);
		}
	};

	class ThreadLocal
	{
	public:
		Logger* get()
		{
			unique_lock<mutex> ul(m_mutex);
			auto it = m_storage.find(std::this_thread::get_id());
			if (it != m_storage.end())
			{
				return it->second.get();
			}
			return nullptr;
		}

		void set(unique_ptr<Logger> logger)
		{
			unique_lock<mutex> ul(m_mutex);
			m_storage[std::this_thread::get_id()] = std::move(logger);
		}

	private:
		mutex m_mutex;
		map<thread::id, unique_ptr<Logger>> m_storage;
	};

	class Tests
	{
	public:
		int run();
	};

}


#endif //THREADSPECIFICSTORAGE_H_INCLUDED
