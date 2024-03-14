#include "pch.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "observer.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unique_ptr;
using std::make_unique;

namespace Observer::Basic1
{
	class CSubject : public ISubject, public std::enable_shared_from_this<CSubject>
	{
	public:
		virtual ~CSubject() {}
		virtual void attach(shared_ptr<IObserver> const &observer) override
		{
			m_observers.push_back(observer);
		}
		virtual void detach(shared_ptr<IObserver> const &observer) override
		{
			m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
		}
		virtual void notify() override
		{
			notifyObservers();
		}
		virtual void setState(const int val) override
		{
			m_val = val;
			notifyObservers();
		}
		virtual int getState() const override { return m_val; }
		virtual void someEvent() override
		{
			cout << "CSubject: Event occurred.\n";
			notifyObservers();
		}
	private:
		void notifyObservers()
		{
			shared_ptr<ISubject> subject = shared_from_this();
			for (auto& o : m_observers)
			{
				o->update(subject);
			}
		}

		void notify(const shared_ptr<ISubject>& subject) const
		{
			for (auto& o : m_observers)
			{
				o->update(subject);
			}
		}
		vector<shared_ptr<IObserver>> m_observers;
		int m_val{-1};
	};

	class CObserver : public IObserver
	{
	public:
		virtual ~CObserver() {}
		virtual void update(const shared_ptr<ISubject>& subject) override
		{
			// you can use subject as well
			cout << "CObserver: Received update from subject state[ " << subject->getState() << " ].\n";
		}
	};

	int Tests::run()
	{
		auto sub = make_shared<CSubject>();
		auto ob1 = make_shared<CObserver>();
		auto ob2 = make_shared<CObserver>();

		sub->attach(ob1);
		sub->attach(ob2);

		sub->setState(15);
		sub->someEvent();

		sub->setState(721);

		sub->detach(ob1);
		sub->someEvent();

		return 0;
	}
}

namespace Observer::Basic2
{

	int Tests::run() {
		return 0;
	}

}

