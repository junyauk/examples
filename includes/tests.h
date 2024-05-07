#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <memory>
#include <future>

#include "general.h"

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::to_string;
using std::stringstream;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;
using std::vector;
using std::queue;
using std::thread;
using std::mutex;
using std::unique_lock;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::future;
using std::promise;
using std::initializer_list;
using std::for_each;

class ITest
{
public:
	virtual ~ITest() {}
	virtual int run() = 0;
};

