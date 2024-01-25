#include "pch.h"
#include <iostream>
#include <thread>
#include <string>
#include <functional>

using std::cout;
using std::endl;
using std::string;

using std::thread;
using std::function;

class A
{
public:
	A() : m_str(" and m_str was used"){}

	static void static_public_member_func() { cout << "static_public_member_func() was called" << endl; }
	void non_static_public_member_func() { cout << "non_static_public_member_func() was called" << endl; };

	void Run_LambdaFunc()
	{
		thread th([this]()
			{
				cout << "Lambda func was called" << m_str << endl;
				// Public
				cout << " - ";
				static_public_member_func();
				cout << " - ";
				non_static_public_member_func();

				// Private
				cout << " - ";
				static_private_member_func();
				cout << " - ";
				non_static_private_member_func();

				cout << "These member functions were called from the thread" << endl;
			});
		th.join();
		return;
	}

	void Run_Functions()
	{
		function<void(void)> f;
		f = static_public_member_func;
		thread th_static_public(f);
		th_static_public.join();

		f = static_private_member_func;
		thread th_private(f);
		th_private.join();


		// In the case of non static, functions need to be binded
		// compile error:
		//  f = std::bind(non_static_public_member_func);   <- simple
		//  f = std::bind(&non_static_public_member_func);  <- with & is not enough
		//  f = std::bind(&A::non_static_public_member_func); <- with &A:: is not enough
		f = std::bind(&A::non_static_public_member_func, this); // with &A:: and this is OK
		thread th_non_static_public(f);
		th_non_static_public.join();

		f = std::bind(&A::non_static_private_member_func, this);
		thread th_non_static_private(f);
		th_non_static_private.join();

		// Without binding
		thread th_non_static_public2(&A::non_static_public_member_func, this);
		th_non_static_public2.join();
		thread th_non_static_private2(&A::non_static_private_member_func, this);
		th_non_static_private2.join();

		return;
	}

	void Run_FunctionWithLambda()
	{
		function<void(void)> f;
		f = [&, this]()
			{
				cout << "std::function with Lambda" << m_str << endl;
				cout << " - ";
				static_public_member_func();
				cout << " - ";
				non_static_public_member_func();

				cout << " - ";
				static_private_member_func();
				cout << " - ";
				non_static_private_member_func();
				cout << "These member functions were called from the thread directly" << endl;

				cout << " - ";
				this->static_public_member_func();
				cout << " - ";
				this->non_static_public_member_func();
				cout << "These member functions were called from the thread via \"this\"" << endl;
			};
		thread th(f);

		th.join();
		return;
	}

private:
	static void static_private_member_func() { cout << "static_private_member_func() was called" << endl; }
	void non_static_private_member_func() { cout << "non_static_private_member_func() was called" << endl; };

	string m_str;
};

void Run_StaticMemberFunc()
{
	A a;
	thread th(a.static_public_member_func);
	th.join();

	return;
}

void Run_NonStaticMemberFunc()
{
	A a;
	thread th(&A::non_static_public_member_func, &a);
	th.join();

	return;
}

void Run_LambdaFunc()
{
	A a;
	a.Run_LambdaFunc();

	return;
}

void Run_Functions()
{
	A a;
	a.Run_Functions();
	a.Run_FunctionWithLambda();

	return;
}


int Run_Basic()
{
	Run_StaticMemberFunc();
	Run_NonStaticMemberFunc();
	Run_LambdaFunc();
	Run_Functions();

	return 0;
}
