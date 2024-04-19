#include "pch.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <shared_mutex>
#include <chrono>

#include "readlockwritelock.h"

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::to_string;
using std::thread;
using std::vector;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::chrono::seconds;

// Read-Write lock multithread design pattern
// This pattern excludes the case writing.
// Read - Read : allow
// Read - Write : excluded
// Write - Write : excluded

using Lock = std::shared_mutex;				// Use shared mutex for multiple threads
using WriteLock = std::unique_lock<Lock>;	// Write: only one thread can write
using ReadLock = std::shared_lock<Lock>;	// Read: all threads can read

Lock myLock;
int sharedData = 0;

void ReadFunc(int id)
{
	ReadLock rl(myLock);
	cout << "ReadFunc(ID: " << to_string(id) << ", sharedData = " << to_string(sharedData) << ")\n";
}

void WriteFunc(int id)
{
	WriteLock wl(myLock);
	sharedData++;
	cout << "WriteFunc(ID: " << to_string(id) << ", sharedData = " << to_string(sharedData) << ")\n";
}


int Run_ReadLockWriteLock()
{
	vector<thread> readers;
	vector<thread> writers;

	for (auto i = 0; i < 5; i++)
	{
		readers.push_back(thread(ReadFunc, i));
		writers.push_back(thread(WriteFunc, i));
	}

	for (auto i = 0; i < 5; i++)
	{
		readers[i].join();
		writers[i].join();
	}

	return 0;
}


namespace ReadLockWriteLock::Singleton::Basic1
{
	// Static members
	mutex SafeOut::m_mutex;

	unique_ptr<Singleton> Singleton::m_instance;
	shared_mutex Singleton::m_rwLock;

	// Both reader/writer threads will access to the same m_data in the Singleton.
	// The Singleton will allow multiple readers to access, but allows only one writer to access.
	void readerThread()
	{
		Singleton* singleton = Singleton::getInstance();
		int value = singleton->readData();
		stringstream ss;
		ss << "Reader thread " << std::this_thread::get_id() << "read data : " << value << endl;
		SafeOut::out(ss.str());
	}

	void writerThread(int value)
	{
		Singleton* singleton = Singleton::getInstance();
		singleton->writeData(value);
		stringstream ss;
		ss << "Writer thread " << std::this_thread::get_id() << "write data: " << value << endl;
		SafeOut::out(ss.str());
	}

	constexpr int NUM_READER_THREADS = 5;
	constexpr int NUM_WRITER_THREADS = 2;

	int Tests::run()
	{
		thread readers[NUM_READER_THREADS];
		for (auto i = 0; i < NUM_READER_THREADS; ++i)
		{
			readers[i] = thread(readerThread);
		}
		thread writers[NUM_WRITER_THREADS];
		for (auto i = 0; i < NUM_WRITER_THREADS; ++i)
		{
			writers[i] = thread(writerThread, i);
		}

		sleep_for(seconds(1));

		for (auto& t : readers)
		{
			if (t.joinable())
			{
				t.join();
			}
		}

		for (auto& t : writers)
		{
			if (t.joinable())
			{
				t.join();
			}
		}

		return 0;
	}

}

namespace ReadLockWriteLock::Singleton::Basic2
{
	// Static members
	mutex SafeOut::m_mutex;

	unique_ptr<Logger> Logger::m_instance;
	shared_mutex Logger::m_rwLock;

	void reader()
	{
		Logger* logger = Logger::getInstance();
		vector<string> logs = logger->read();

		stringstream ss;
		ss << "Reader thread " << std::this_thread::get_id() << " read log: ";
		for (auto& log : logs)
		{
			ss << "\n\t" << log;
		}
		ss << endl;
		SafeOut::out(ss.str());
	}

	void writer(const string& log)
	{
		Logger* logger = Logger::getInstance();
		logger->add(log);

		stringstream ss;
		ss << "Writer thread " << std::this_thread::get_id() << " write log: " << log << endl;
		SafeOut::out(ss.str());
	}

	constexpr int NUM_READER_THREADS = 3;
	constexpr int NUM_WRITER_THREADS = 2;

	int Tests::run()
	{
		thread writers[NUM_WRITER_THREADS];
		writers[0] = thread(writer, "Error: Disk space low");
		writers[1] = thread(writer, "Info: Application started");

		thread readers[NUM_READER_THREADS];
		for (auto& r : readers)
		{
			r = thread(reader);
		}

		sleep_for(seconds(1));

		for (auto& t : readers)
		{
			if (t.joinable())
			{
				t.join();
			}
		}

		for (auto& t : writers)
		{
			if (t.joinable())
			{
				t.join();
			}
		}

		return 0;
	}

}


