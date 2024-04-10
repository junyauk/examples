#pragma once
#ifndef MEDIATOR_H_INCLUDED
#define MEDIATOR_H_INCLUDED

#include <iostream>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::weak_ptr;

namespace Mediator::Basic1
{
	class IColleague;

	class IMediator
	{
	public:
		virtual ~IMediator() {}
		virtual void sendMessage(shared_ptr<IColleague> sender, string const& msg) const = 0;
	};

	class IColleague : public std::enable_shared_from_this<IColleague>
	{
	public:
		explicit IColleague(shared_ptr<IMediator> mediator) : m_mediator(mediator) {}
		virtual void send(string const& msg) = 0;
		virtual void receive(string const& msg) = 0;
	protected:
		weak_ptr<IMediator> m_mediator;
	};

	class Tests
	{
	public:
		int run();
	};

}

namespace Mediator::Basic1Step2
{
	// This example uses CRTP, but the code looks more complex than the original one.

	// Forward declarations
	class ConcreteColleagueA;
	class ConcreteColleagueB;

	template<typename T>
	class Mediator {
	public:
		virtual void sendMessage(const std::shared_ptr<T>& sender, const std::string& message) = 0;
	};

	template<typename T>
	class Colleague : public std::enable_shared_from_this<Colleague<T>> {
	protected:
		std::weak_ptr<Mediator<T>> mediator;

	public:
		explicit Colleague(const std::shared_ptr<Mediator<T>>& mediator) : mediator(mediator) {}

		void send(const std::string& message) {
			if (auto med = mediator.lock()) {
				med->sendMessage(std::static_pointer_cast<T>(this->shared_from_this()), message);
			}
			else {
				std::cout << "Mediator is expired!" << std::endl;
			}
		}

		virtual void receive(const std::string& message) = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}



#endif // MEDIATOR_H_INCLUDED
