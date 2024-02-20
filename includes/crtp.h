#pragma once
#ifndef CRTP_H_INCLUDED
#define CRTP_H_INCLUDED

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace CRTP::Basic1
{
	template<typename DerivedClass>
	class Base
	{
	public:
		string interface()
		{
			return static_cast<DerivedClass*>(this)->implementation();
		}

		string commonFunction()
		{
			return "This is a common function in Base class";
		}
	private:
	};

	class Derived : public Base<Derived>
	{
	public:
		string implementation()
		{
			return "Implementation in Derived class";
		}
	};
}

namespace CRTP::Basic2
{
	// This one demonstrate how to use CRTP with concept.
	// If applying conditions to Derived classes,
	// The Derived class will need to inherit a specialized
	// intermediate class.

	// concept
	template <class T>
	concept drawable = requires(T t)
	{
		{ t.drawLine() } -> std::same_as<std::string>;
		// multiple conditions can be define
	};
	template <class T>
	concept info = requires(T t)
	{
		{ t.info() };
		// multiple conditions can be define
	};
	// Combine multiple concept
	template <class T>
	concept drawable_and_info = drawable<T> && info<T>;

	template <class T>
	class Pen
	{
	public:
		string draw()
		{
			return static_cast<T*>(this)->drawLine();
		}
		void info()
		{
			return static_cast<T*>(this)->info();
		}
	};

	// Note:
	// This is the trick using CRTP with concept
	// Define an intermediate class, and its specialized one.
	// Derived class need to inherit the specialized one.
	template<class T> class PenBase : public Pen<T>{};
	template<drawable_and_info T> class PenBase<T> : public Pen<T>{};

	// Derived class uses the specialized version of the class.
	class SolidPen : public PenBase<SolidPen>
	{
	public:
		string drawLine()
		{
			return "Drawing a solid line";
		}
		void info()
		{
			cout << "Solid Pen\n";
		}
	};

	// Derived class uses the specialized version of the class.
	class DashPen : public PenBase<DashPen>
	{
	public:
		string drawLine()
		{
			return "Drawing a dash line";
		}
		void info()
		{
			cout << "Dash Pen\n";
		}
	};
}

namespace CRTP::Singleton
{
	// This base class's getInstance () returns the provided class's instance.
	template<class T>
	class Singleton
	{
	public:
		static T& getInstance()
		{
			cout << "getInstance() was called.\n";
			static T singleton; // Returning this static instance of the provided class
			return singleton;
		}
	protected:
		Singleton() = default;
		virtual ~Singleton() = default;

		// Disable copy constructor and operator=
		Singleton(const Singleton& t) = delete;
		Singleton& operator=(const Singleton& t) = delete;
	};

	// This class inherits the Singleton class 
	// which can create a singleton of the Delived class
	class Derived1 : public Singleton<Derived1>
	{
	public:
		Derived1() { cout << "Derived1's constructor\n";	}
		~Derived1(){ cout << "Derived1's destructor will be called when the program was completed because it's STATIC!!!\n"; }
		void run() { cout << "run()\n";	}
	};

	class Derived2 : public Singleton<Derived2>
	{
	public:
		Derived2() { cout << "Derived2's constructor\n"; }
		~Derived2(){ cout << "Derived2's destructor will be called when the program was completed because it's STATIC!!!\n"; }
		void run() { cout << "run()\n"; }
	};

}


#endif // CRTP_H_INCLUDED
