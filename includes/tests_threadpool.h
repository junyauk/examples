#pragma once
#ifndef TEST_THREADPOOL_H
#define TEST_THREADPOOL_H

#include "tests.h"
#include "threadpool.h"
namespace ThreadPool::Basic1
{
	class Tests : public ITest
	{
	public:
		int run() override;
	};
}
namespace ThreadPool::ProducerConsumer
{
	class Task
	{
	public:
		Task(int id) : m_id(id) {}
		void process() const;
	private:
		int m_id;
	};

	class Request
	{
	public:
		Task produce();
	private:
		int m_id{ 0 };
	};

	class WebServer
	{
	public:
		void processRequest(Task&& task);
		void stop();
	private:
		ThreadPool m_threadPool{ 5 };
	};

	class Tests : public ITest
	{
	public:
		int run() override;
	};
}
namespace ThreadPool::Strategy
{
	class Tests : public ITest
	{
	public:
		int run() override;
	};
}
namespace ThreadPool::Windows1
{
	class Tests : public ITest
	{
	public:
		int run() override;
	};
}


#endif // TEST_THREADPOOL_H
