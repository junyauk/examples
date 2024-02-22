#pragma once
#ifndef ABSTRACTFACTORY_H_INCLUDED
#define ABSTRACTFACTORY_H_INCLUDED

#include <string>
#include <memory>

using std::string;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;

// This Abstract Factory design pattern uses 2 classes
// They are Product and Factory.
// 

namespace AbstractFactory::Basic1
{
	class IProductA
	{
	public:
		virtual ~IProductA() {};
		virtual string FuncA() const = 0;
	};
	using IProductAuptr = unique_ptr<IProductA>;
	using IProductAsptr = shared_ptr<IProductA>;

	class IProductB
	{
	public:
		virtual ~IProductB() {};
		virtual string FuncB() const = 0;
		virtual string FuncBwithProductA(IProductA const& a) const = 0;
	};
	using IProductBuptr = unique_ptr<IProductB>;
	using IProductBsptr = shared_ptr<IProductB>;

	class IFactory
	{
	public:
		virtual ~IFactory() {};
		virtual IProductAuptr createProductA() const = 0;
		virtual IProductBuptr createProductB() const = 0;
	};
	using IFactoryuptr = unique_ptr<IFactory>;
	using IFactorysptr = shared_ptr<IFactory>;

	int Run_AbstractFactory();
}


#endif // ABSTRACTFACTORY_H_INCLUDED