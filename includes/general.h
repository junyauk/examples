#pragma once
#ifndef GENERAL_H_INCLUDED
#define GENERAL_H_INCLUDED
#include "Windows.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <deque>
#include <functional>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <cassert>
#include <random>
#include <chrono>
#include <atomic>

using std::size_t;
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::wstring;
using std::to_string;
using std::stringstream;
using std::vector;
using std::queue;
using std::deque;
using std::function;
using std::thread;
using std::mutex;
using std::shared_mutex;
using std::unique_lock;
using std::lock_guard;
using cv = std::condition_variable;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::atomic;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

void general();

class ThreadSafeOut
{
public:
	static void print(const string& str)
	{
		static mutex	m;
		{
			unique_lock<mutex>	ul(m);
			cout << str;
		}
	}
};

string GetLastErrorMessage(DWORD lastError);

wstring FindFolderInPath(const wstring folderName);


#endif // GENERAL_H_INCLUDED