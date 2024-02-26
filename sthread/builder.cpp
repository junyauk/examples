#include "pch.h"

#include <iostream>
#include <string>
#include <memory>

#include "builder.h"

using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;

namespace Builder::Basic1
{
	// Product class
	class CarModel : public IPlasticModel
	{
	public:
		CarModel() {}
		CarModel(CarModel const& other) // copy constructor
		{
			m_material = other.m_material;
			m_colour = other.m_colour;
			m_scale = other.m_scale;
		}
		virtual ~CarModel(){}
		virtual shared_ptr<IPlasticModel> clone() const override
		{
			return make_shared<CarModel>(*this); // clone using copy constructor
		}

		virtual void setMaterial(string const& material) override { m_material = material; }
		virtual void setColour(string const& colour) override { m_colour = colour; }
		virtual void setScale(int const scale) override { m_scale = scale; };
		virtual void display() override
		{
			cout << "Plastic car model Made of: " << m_material << " with colour: " << m_colour << ", scale 1/" << m_scale << endl;
		};
	private:
		string m_material;
		string m_colour;
		int m_scale;
	};

	class ShipModel : public IPlasticModel
	{
	public:
		ShipModel() {}
		ShipModel(ShipModel const& other) // copy constructor
		{
			m_material = other.m_material;
			m_colour = other.m_colour;
			m_scale = other.m_scale;
		}
		virtual ~ShipModel() {}
		virtual shared_ptr<IPlasticModel> clone() const override
		{
			return make_shared<ShipModel>(*this); // clone using copy constructor
		}

		virtual void setMaterial(string const& material) override { m_material = material; }
		virtual void setColour(string const& colour) override { m_colour = colour; }
		virtual void setScale(int const scale) override { m_scale = scale; };
		virtual void display() override
		{
			cout << "Plastic ship model Made of: " << m_material << " with colour: " << m_colour << ", scale 1/" << m_scale << endl;
		};
	private:
		string m_material;
		string m_colour;
		int m_scale;
	};

	// Builder class
	class CCarModelBuilder : public IModelBuilder
	{
	public:
		CCarModelBuilder()
		{
			m_model = make_shared<CarModel>();
		}
		virtual ~CCarModelBuilder()	{}
		virtual void buildMaterial() override { m_model->setMaterial("PS"); }
		virtual void buildColour() override { m_model->setColour("Red"); }
		virtual void buildScale() override { m_model->setScale(12); }
		virtual shared_ptr<IPlasticModel> getPrototype() const override { return m_model; }
	private:
		shared_ptr<CarModel> m_model;
	};
	class CShipModelBuilder : public IModelBuilder
	{
	public:
		CShipModelBuilder()
		{
			m_model = make_shared<ShipModel>();
		}
		virtual ~CShipModelBuilder() {}
		virtual void buildMaterial() override { m_model->setMaterial("ABS"); }
		virtual void buildColour() override { m_model->setColour("Grey"); }
		virtual void buildScale() override { m_model->setScale(1700); }
		virtual shared_ptr<IPlasticModel> getPrototype() const override { return m_model; }
	private:
		shared_ptr<ShipModel> m_model;
	};

	class Tamiya: public IFactory
	{
	public:
		virtual void setModelBuilder(shared_ptr<IModelBuilder> builder) override { m_builder = builder; }
		virtual shared_ptr<IPlasticModel> getPrototype() const override
		{
			if (m_builder != nullptr)
			{
				return m_builder->getPrototype();
			}
			else
			{
				cout << "Builder needs to be set before getting a model.\n";
				return nullptr;
			}
		}
		virtual void constructModel() override
		{
			if (m_builder != nullptr)
			{
				m_builder->buildMaterial();
				m_builder->buildColour();
				m_builder->buildScale();
			}
			else
			{
				cout << "Builder needs to be set befure constructing.\n";
			}
		}
		shared_ptr<IModelBuilder> m_builder;
	};

	class Hasegawa : public IFactory
	{
	public:
		virtual void setModelBuilder(shared_ptr<IModelBuilder> builder) override { m_builder = builder; }
		virtual shared_ptr<IPlasticModel> getPrototype() const override
		{
			if (m_builder != nullptr)
			{
				return m_builder->getPrototype();
			}
			else
			{
				cout << "Builder needs to be set before getting a model.\n";
				return nullptr;
			}
		}
		virtual void constructModel() override
		{
			if (m_builder != nullptr)
			{
				m_builder->buildMaterial();
				m_builder->buildColour();
				m_builder->buildScale();
			}
			else
			{
				cout << "Builder needs to be set befure constructing.\n";
			}
		}
		shared_ptr<IModelBuilder> m_builder;
	};

	int Tests::run()
	{
		Tamiya	tamiya;
		Hasegawa hasegawa;
		shared_ptr<CCarModelBuilder> carBuilder = make_shared<CCarModelBuilder>();
		shared_ptr<CShipModelBuilder> shipBuilder = make_shared<CShipModelBuilder>();


		tamiya.setModelBuilder(carBuilder);
		tamiya.constructModel();
		// Note: the prototype is actually still in the builder
		shared_ptr<CarModel> carTamiyaPrototype = std::static_pointer_cast<CarModel>(tamiya.getPrototype());
		_ASSERT(carTamiyaPrototype == carBuilder->getPrototype());
		CarModel carTamiya(*carTamiyaPrototype);

		tamiya.setModelBuilder(shipBuilder);
		tamiya.constructModel();
		// Note: the prototype is actually still in the builder
		shared_ptr<ShipModel> shipTamiyaPrototype = std::static_pointer_cast<ShipModel>(tamiya.getPrototype());
		_ASSERT(shipTamiyaPrototype == shipBuilder->getPrototype());
		ShipModel shipTamiya(*shipTamiyaPrototype);

		hasegawa.setModelBuilder(carBuilder);
		hasegawa.constructModel();
		shared_ptr<CarModel> carHasegawaPrototype = std::static_pointer_cast<CarModel>(hasegawa.getPrototype());
		_ASSERT(carHasegawaPrototype == carBuilder->getPrototype());
		CarModel carHasegawa(*carHasegawaPrototype);

		hasegawa.setModelBuilder(shipBuilder);
		hasegawa.constructModel();
		shared_ptr<ShipModel> shipHasegawaPrototype = std::static_pointer_cast<ShipModel>(hasegawa.getPrototype());
		_ASSERT(shipHasegawaPrototype == shipBuilder->getPrototype());
		ShipModel shipHasegawa(*shipHasegawaPrototype);

		carTamiya.display();
		shipTamiya.display();
		carHasegawa.display();
		shipHasegawa.display();

		// further tests checking NG cases need to be implemented

		return 0;
	}
}
