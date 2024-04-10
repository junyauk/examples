#pragma once
#ifndef PROXY_H_INCLUDED
#define PROXY_H_INCLUDED

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

namespace Proxy::Basic1
{
	class ISubject
	{
	public:
		virtual ~ISubject() {}
		virtual void request() const = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Proxy::Basic2
{
	class IFileServer
	{
	public:
		virtual string getFileContents(string const& filename) = 0;
	};

	class Tests
	{
	public:
		int run();
	};


}


#endif // PROXY_H_INCLUDED
