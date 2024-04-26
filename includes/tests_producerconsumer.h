#pragma once
#ifndef TESTS_PRODUCERCONSUMER_H_INCLUDED
#define TESTS_PRODUCERCONSUMER_H_INCLUDED
#include "tests.h"
#include "producerconsumer.h"

namespace ProducerConsumer::Basic1
{
	class Tests : public ITest
	{
	public:
		int run();
	};
}

namespace ProducerConsumer::Basic2
{
	class Tests : public ITest
	{
	public:
		int run();
	};
}

namespace ProducerConsumer::Printer1
{
	class Tests : public ITest
	{
	public:
		int run();
	};
}

namespace ProducerConsumer::TaskQueue
{
	class Tests : public ITest
	{
	public:
		int run();
	};
}

#endif
