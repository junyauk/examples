#pragma once
#ifndef FACTORYMETHOD_H_INCLUDED
#define FACTORYMETHOD_H_INCLUDED

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unique_ptr;
using std::make_unique;

namespace FactoryMethod::Basic1
{
	class IProduct
	{
	public:
		virtual ~IProduct() {};
		virtual void setFactoryName(string const& name) = 0;
		virtual string getName() = 0;
	};
	class IFactory
	{
	public:
		virtual ~IFactory() {}
		virtual unique_ptr<IProduct> produce() = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

#endif // FACTORYMETHOD_H_INCLUDED
