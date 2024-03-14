#pragma once
#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

namespace Singleton::Basic1
{
	// Quite simple example
	class Singleton
	{
	public:
		// copy constructor and assign operator are deleted to prevent cloning
		Singleton(Singleton const&) = delete;
		void operator=(Singleton const&) = delete;

		static Singleton& getInstance()
		{
			// static local variable makes sure it is create only once.
			static Singleton instance;

			return instance;
		}

		string info()
		{
			return "Valid instance";
		}
	private:
		// only class method can use this
		Singleton() {}
	};

	class Tests
	{
	public:
		int run();
	};
}

#endif
