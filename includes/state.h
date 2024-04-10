#pragma once
#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::weak_ptr;

namespace State::Basic1
{
	class Context;

	class IState
	{
	public:
		virtual ~IState() {}
		virtual void handle(Context& context) = 0;
	};

	class Context
	{
	public:
		Context() {}
		void setState(unique_ptr<IState> state)
		{
			m_state = std::move(state);
		}
		void request()
		{
			if (m_state)
			{
				m_state->handle(*this);
			}
			else
			{
				cout << "No state set.\n";
			}
		}
	private:
		unique_ptr<IState> m_state;
	};

	class Tests
	{
	public:
		int run();
	};
}


#endif // STATE_H_INCLUDED
