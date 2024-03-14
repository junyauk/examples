#pragma once
#ifndef PROTOTYPE_H_INCLUDE
#define PROTOTYPE_H_INCLUDE

#include <string>
#include <memory>

using std::string;
using std::unique_ptr;

namespace Prototype::Basic1
{
	class IPrototype
	{
	public:
		virtual ~IPrototype() {}
		virtual string info() const = 0;
		virtual unique_ptr<IPrototype> clone() const = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}
namespace Prototype::Basic2
{

	// This prototype class should have a clone feature
	// that produce a new instance.
	class IPrototype
	{
	public:
		virtual ~IPrototype() {}
		virtual IPrototype* clone() const = 0; // This cloning feature is the important point
		virtual void printInfo() const = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}


#endif
