#pragma once
#ifndef TEMPLATEMETHOD_H_INCLUDED
#define TEMPLATEMETHOD_H_INCLUDED

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

// Template Method design pattern looks similar to Strategy design pattern.
// There is a difference between them.
// The Template Method design pattern has a method that processes a seriece of operations. 
// and allow derived classses to override each operation.
// The Strategy design pattern doesn't care the whole processes.
// This design pattern would override each individual operation if needed.
namespace TemplateMethod::Basic1
{
	class IBase
	{
	public:
		// What it needs to do is defined in the asbtract class as template
		string run()
		{
			string pre = preProcess();
			string main = mainProcess();
			string post = postProcess();
			return pre + " " + main + " " + post;
		}

		// This can be overridden if needed
		virtual string preProcess()
		{
			return "<IBase pre process>";
		}

		// This needs to be overritten
		virtual string mainProcess() = 0;

		// This can be overridden if needed
		virtual string postProcess()
		{
			return "<IBase post process>";
		}
	};

	class Derived1 : public IBase
	{
	public:
		virtual string mainProcess() override
		{
			return "<Derived1 main process>";
		}
	};

	class Derived2 : public IBase
	{
	public:
		virtual string mainProcess() override
		{
			return "<Derived2 main process>";
		}
		virtual string postProcess() override
		{
			return "<Derived2 post process>";
		}
	};

	class Tests
	{
	public:
		int run();
	};
}

#endif // TEMPLATEMETHOD_H_INCLUDED