#pragma once
#ifndef BUILDER_H_INCLUDED
#define BUILDER_H_INCLUDED

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;
using std::make_shared;

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


#endif
