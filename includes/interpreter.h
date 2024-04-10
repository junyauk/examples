#pragma once
#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::unordered_map;

namespace Interpreter::Basic1
{
	class IExpression
	{
	public:
		virtual ~IExpression() {}
		virtual int interpret(unordered_map<char, int>& context) = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Interpreter::Basic2
{
	class IExpression
	{
	public:
		virtual ~IExpression() {}
		virtual int interpret(vector<int>const& content) const = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}
#endif
