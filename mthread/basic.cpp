#include "pch.h"
#include <chrono>
#include "basic.h"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::chrono::seconds;

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


namespace MultiThread::Observer
{
	class CSubject : public ISubject
	{
	public:
		virtual void addObserver(shared_ptr<IObserver> observer) override
		{
			unique_lock<mutex> ul(m_mutex);
			m_observers.push_back(observer);
		}
		virtual void removeObserver(shared_ptr<IObserver> observer) override
		{
			unique_lock<mutex> ul(m_mutex);
			m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
		}
		virtual void notifyObservers() override
		{
			unique_lock<mutex> ul(m_mutex);
			for (auto& o : m_observers)
			{
				o->update(m_state);
			}
		}

		void setState(int state)
		{
			{
				unique_lock<mutex> ul(m_mutex);
				m_state = state;
			}
			notifyObservers();
		}

	private:
		vector<shared_ptr<IObserver>> m_observers;
		int m_state;
		mutex m_mutex;
	};

	class CObserver : public IObserver
	{
	public:
		CObserver(string name, shared_ptr<ISubject> subject) : m_name(name), m_subject(subject) {}
		~CObserver()
		{
			detachFromObserver();
		}

		void attachToObserver()
		{
			if (m_subject)
			{
				m_subject->addObserver(shared_from_this());
			}
		}
		void detachFromObserver()
		{
			if (m_subject)
			{
				m_subject->removeObserver(shared_from_this());
			}
		}

		void update(int state) override
		{
			cout << "Observer [" << m_name << "] received state: " << state << endl;
		}

	private:
		string m_name;
		shared_ptr<ISubject> m_subject;
	};


	void func(int state, shared_ptr<ISubject> subject)
	{
		subject->setState(state);
		sleep_for(seconds(1));
	}

	int Tests::run()
	{
		auto subject = make_shared<CSubject>();
		auto observer1 = make_shared<CObserver>("Observer 1", subject);
		auto observer2 = make_shared<CObserver>("Observer 2", subject);

		observer1->attachToObserver();
		observer2->attachToObserver();

		thread t1(std::bind(func, 1, subject));
		thread t2(std::bind(func, 2, subject));

		sleep_for(seconds(3));

		if (t1.joinable())
		{
			t1.join();
		}
		if (t2.joinable())
		{
			t2.join();
		}

		return 0;
	}

}

