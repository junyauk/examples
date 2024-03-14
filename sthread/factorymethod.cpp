#include "pch.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "factorymethod.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unique_ptr;
using std::make_unique;
using std::enable_shared_from_this;

namespace FactoryMethod::Basic1
{
	class Car : public IProduct
	{
	public:
		Car(string const& name) : m_name(name) {}
		virtual void setFactoryName(string const& name) override { m_factory = name; }
		virtual string getName() { return m_name + " made by " + m_factory + " Factory"; }
	private:
		string m_factory;
		string m_name;
	};

	class Bike : public IProduct
	{
	public:
		Bike(string const& name) : m_name(name) {}
		virtual void setFactoryName(string const& name) override { m_factory = name; }
		virtual string getName() { return m_name + " made by " + m_factory + " Factory"; }
	private:
		string m_factory;
		string m_name;
	};

	// This factory is designed for producing Car
	class CarFactory : public IFactory
	{
	public:
		CarFactory(string const& name) : m_name(name) {}
		virtual unique_ptr<IProduct> produce() override
		{
			unique_ptr<Car> product = make_unique<Car>("Car");
			product->setFactoryName(m_name);
			return product;
		}
	private:
		string m_name;
	};

	// This factory is designed for producing Bike
	class BikeFactory : public IFactory
	{
	public:
		BikeFactory(string const& name) : m_name(name) {}
		virtual unique_ptr<IProduct> produce() override
		{
			unique_ptr<Bike> product = make_unique<Bike>("Bike");
			product->setFactoryName(m_name);
			return product;
		}
	private:
		string m_name;
	};

	int Tests::run()
	{
		unique_ptr<IFactory> factory;
		unique_ptr<IProduct> product;

		// User can change the factory for producing a product that the user want

		factory = make_unique<CarFactory>("Toyota");
		product = factory->produce();
		cout << product->getName() << endl;

		factory = make_unique<BikeFactory>("Honda");
		product = factory->produce();
		cout << product->getName() << endl;

		return 0;
	}
}