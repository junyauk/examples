#include "pch.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <shared_mutex>

#include "readlockwritelock.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::thread;
using std::vector;

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

