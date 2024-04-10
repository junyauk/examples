#pragma once
#ifndef CHAINOFRESPONSIBILITY_H_INCLUDED
#define CHAINOFRESPONSIBILITY_H_INCLUDED

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

namespace ChainOfResponsibility::Basic1
{
	class Handler
	{
	public:
		virtual ~Handler() {}
		virtual void handleRequest(string const& request) = 0;
		void setSuccessor(shared_ptr<Handler> successor)
		{
			m_successor = successor;
		}
	protected:
		shared_ptr<Handler> m_successor;
	};

	class Tests
	{
	public:
		int run();
	};
}

#endif
