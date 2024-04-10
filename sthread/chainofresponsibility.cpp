#include "pch.h"

#include "chainofresponsibility.h"

namespace ChainOfResponsibility::Basic1
{
	class CHandler1 : public Handler
	{
	public:
		void handleRequest(string const& request) override
		{
			if (request == "Handler1")
			{
				cout << "CHandler1 handled the request\n";
			}
			else
			{
				m_successor->handleRequest(request);
			}
		}
	};

	class CHandler2 : public Handler
	{
	public:
		void handleRequest(string const& request) override
		{
			if (request == "Handler1")
			{
				cout << "CHandler2 handled the request\n";
			}
			else if (m_successor != nullptr)
			{
				m_successor->handleRequest(request);
			}
		}
	};

	int Tests::run()
	{
		shared_ptr<Handler> handler1 = make_shared<CHandler1>();
		shared_ptr<Handler> handler2 = make_shared<CHandler2>();

		handler1->setSuccessor(handler2);

		handler1->handleRequest("Handler1");
		handler1->handleRequest("Handler2");
		handler1->handleRequest("Handler3");

		return 0;
	}
}
