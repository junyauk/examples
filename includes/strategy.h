#pragma once
#ifndef STRATEGY_H_INCLUDED
#define STRATEGY_H_INCLUDED

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unique_ptr;
using std::make_unique;

namespace Strategy::Basic1
{
	class IPen
	{
	public:
		virtual ~IPen() {}
		virtual string draw() const = 0;
	};

	class IContext
	{
	public:
		virtual ~IContext() {}
		virtual void setPen(IPen* pen) = 0;
		virtual string draw() const = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Strategy::Basic1Step2
{
	class IPen
	{
	public:
		virtual ~IPen() {}
		virtual string draw() const = 0;
	};

	class IContext
	{
	public:
		virtual ~IContext() {}
		virtual void setPen(shared_ptr<IPen> pen) = 0;
		virtual string draw() const = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}


#endif // STRATEGY_H_INCLUDED