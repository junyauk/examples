#include "pch.h"
#include "mediator.h"



namespace Mediator::Basic1
{
	class CColleagueA : public IColleague
	{
	public:
		explicit CColleagueA(shared_ptr<IMediator> const& mediator) : IColleague(mediator) {}

		virtual void send(string const& msg) override
		{
			// Use the mediator for communicating others.
			auto mediator = m_mediator.lock();
			if (mediator != nullptr)
			{
				mediator->sendMessage(shared_from_this(), msg);
			}
		}
		virtual void receive(string const& msg) override
		{
			cout << "CColleagueA received: [" << msg << "]\n";
		}
	};

	class CColleagueB : public IColleague
	{
	public:
		explicit CColleagueB(shared_ptr<IMediator> const& mediator) : IColleague(mediator) {}

		virtual void send(string const& msg) override
		{
			// Use the mediator for communicating others.
			auto mediator = m_mediator.lock();
			if (mediator != nullptr)
			{
				mediator->sendMessage(shared_from_this(), msg);
			}
		}
		virtual void receive(string const& msg) override
		{
			cout << "CColleagueB received: [" << msg << "]\n";
		}
	};

	class CMediator : public IMediator
	{
	public:
		void setColleagueA(shared_ptr<CColleagueA> colleague) { m_colA = colleague; }
		void setColleagueB(shared_ptr<CColleagueB> colleague) { m_colB = colleague; }
		virtual void sendMessage(shared_ptr<IColleague> sender, string const& msg) const override
		{
			if (auto a = m_colA.lock())
			{
				if (sender != a)
				{
					a->receive(msg);
				}
			}
			if (auto b = m_colB.lock())
			{
				if (sender != b)
				{
					b->receive(msg);
				}
			}
		}

	private:
		weak_ptr<CColleagueA> m_colA;
		weak_ptr<CColleagueB> m_colB;
	};

	int Tests::run()
	{
		shared_ptr<CMediator> mediator = make_shared<CMediator>();
		shared_ptr<CColleagueA> colA = make_shared<CColleagueA>(mediator);
		shared_ptr<CColleagueB> colB = make_shared<CColleagueB>(mediator);

		mediator->setColleagueA(colA);
		mediator->setColleagueB(colB);

		colA->send("Hello message from Colleague A!");
		colB->send("Hi from Colleague B!");

		return 0;
	}
}


namespace Mediator::Basic1Step2
{
	class ConcreteColleagueA : public Colleague<ConcreteColleagueA> {
	public:
		explicit ConcreteColleagueA(const std::shared_ptr<Mediator<ConcreteColleagueA>>& mediator) : Colleague<ConcreteColleagueA>(mediator) {}

		void receive(const std::string& message) override {
			std::cout << "ConcreteColleagueA received: " << message << std::endl;
		}
	};

	class ConcreteColleagueB : public Colleague<ConcreteColleagueB> {
	public:
		explicit ConcreteColleagueB(const std::shared_ptr<Mediator<ConcreteColleagueB>>& mediator) : Colleague<ConcreteColleagueB>(mediator) {}

		void receive(const std::string& message) override {
			std::cout << "ConcreteColleagueB received: " << message << std::endl;
		}
	};

	class ConcreteMediatorA : public Mediator<ConcreteColleagueA> {
	private:
		std::weak_ptr<ConcreteColleagueB> colleagueB;

	public:
		void setColleagueB(const std::shared_ptr<ConcreteColleagueB>& colleague) {
			colleagueB = colleague;
		}

		void sendMessage(const std::shared_ptr<ConcreteColleagueA>& sender, const std::string& message) override {
			if (auto b = colleagueB.lock()) {
				b->receive(message);
			}
		}
	};

	class ConcreteMediatorB : public Mediator<ConcreteColleagueB> {
	private:
		std::weak_ptr<ConcreteColleagueA> colleagueA;

	public:
		void setColleagueA(const std::shared_ptr<ConcreteColleagueA>& colleague) {
			colleagueA = colleague;
		}

		void sendMessage(const std::shared_ptr<ConcreteColleagueB>& sender, const std::string& message) override {
			if (auto a = colleagueA.lock()) {
				a->receive(message);
			}
		}
	};



	int Tests::run()
	{
		auto mediatorA = std::make_shared<ConcreteMediatorA>();
		auto mediatorB = std::make_shared<ConcreteMediatorB>();

		auto colleagueA = std::make_shared<ConcreteColleagueA>(mediatorA);
		auto colleagueB = std::make_shared<ConcreteColleagueB>(mediatorB);

		mediatorA->setColleagueB(colleagueB);
		mediatorB->setColleagueA(colleagueA);

		colleagueA->send("Hello from Colleague A!");
		colleagueB->send("Hi from Colleague B!");

		return 0;
	}

}