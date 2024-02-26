#pragma once
#ifndef BUILDER_H_INCLUDED
#define BUILDER_H_INCLUDED

#include <string>
#include <vector>
#include <memory>


using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unique_ptr;

namespace Builder::Basic1
{
	// Product class
	class IPlasticModel
	{
	public:
		virtual ~IPlasticModel() {}
		virtual void setMaterial(string const &material ) = 0;
		virtual void setColour(string const &colour) = 0;
		virtual void setScale(int const scale) = 0;
		virtual void display() = 0;
		virtual shared_ptr<IPlasticModel> clone() const = 0;
	};

	// Builder class
	class IModelBuilder
	{
	public:
		virtual ~IModelBuilder() {}
		virtual void buildMaterial() = 0;
		virtual void buildColour() = 0;
		virtual void buildScale() = 0;
		virtual shared_ptr<IPlasticModel> getPrototype() const = 0;
	};

	class IFactory
	{
	public:
		virtual void setModelBuilder(shared_ptr<IModelBuilder> builder) = 0;
		virtual shared_ptr<IPlasticModel> getPrototype() const = 0;
		virtual void constructModel() = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Builder::Basic2
{
	class IFood
	{
	public:
		virtual ~IFood() {}
		virtual bool setIngredients(vector<string> ingredients) = 0;
		virtual void setCookingMethods(vector<string> methods) = 0;
		virtual void cook() = 0;
		virtual string getFood() const = 0;
	};

	class IChef
	{
	public:
		virtual ~IChef() {}
		virtual void prepareIngredients(vector<string>& ingredients) = 0;
		virtual void cookFood() = 0;
		virtual vector<unique_ptr<IFood>> getFood() = 0;
	};

	class IRestaurant
	{
	public:
		virtual ~IRestaurant() {}
		virtual void setChef(weak_ptr<IChef> chef) = 0;
		virtual void open() = 0;
		virtual void close() = 0;
		virtual void addOrder() = 0;
		virtual void serveFood() = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

#endif
