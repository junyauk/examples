#pragma once
#ifndef BASIC_H_INLCLUDED
#define BASIC_H_INLCLUDED
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <functional>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::vector;

using std::thread;
using std::mutex;
using std::unique_lock;
using std::function;
using std::shared_ptr;
using std::make_shared;
using std::enable_shared_from_this;

int Run_Basic();

namespace MultiThread::Observer
{
	class IObserver : public enable_shared_from_this<IObserver>
	{
	public:
		virtual void update(int newState) = 0;
	};

	class ISubject
	{
	public:
		virtual void addObserver(shared_ptr<IObserver> observer) = 0;
		virtual void removeObserver(shared_ptr<IObserver> observer) = 0;
		virtual void notifyObservers() = 0;
		virtual void setState(int state) = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

#endif // BASIC_H_INLCLUDED
