#include "pch.h"

#include "state.h"

namespace State::Basic1
{
	class StateA : public IState
	{
	public:
		virtual void handle(Context& context) override;
	};

	class StateB : public IState
	{
	public:
		virtual void handle(Context& context) override;
	};

	class StateC : public IState
	{
	public:
		virtual void handle(Context& context) override;
	};

	void StateA::handle(Context& context)
	{
		cout << "Handling request in State A.\n";
		context.setState(unique_ptr<StateB>());
	}

	void StateB::handle(Context& context)
	{
		cout << "Handling request in State B.\n";
		context.setState(unique_ptr<StateC>());
	}

	void StateC::handle(Context& context)
	{
		cout << "Handling request in State C.\n";
		context.setState(unique_ptr<StateA>());
	}

	int Tests::run()
	{
		Context context;
		context.setState(make_unique<StateA>());

		context.request();
		context.request();
		context.request();
		context.request();

		return 0;
	}
}