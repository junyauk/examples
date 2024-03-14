#pragma once
#ifndef OBSERVER_H_INCLUDED
#define OBSERVER_H_INCLUDED

#include <memory>

using std::shared_ptr;

namespace Observer::Basic1
{
	class IObserver;

	// Subject class will have a list of observer
	// The notify() will call update() of observers in the list
	class ISubject
	{
	public:
		virtual ~ISubject() {}
		virtual void attach(shared_ptr<IObserver> const& observer) = 0;
		virtual void detach(shared_ptr<IObserver> const& observer) = 0;
		virtual void setState(const int val) = 0;
		virtual int getState() const = 0;
		virtual void notify() = 0;
		virtual void someEvent() = 0;
	};

	// Observer class will have update() method to receive something's happened.
	class IObserver
	{
	public:
		virtual ~IObserver() {}
		virtual void update(const shared_ptr<ISubject> & subject) = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Observer::Basic2
{
	class Tests
	{
	public:
		int run();
	};
}


#endif // OBSERVER_H_INCLUDED