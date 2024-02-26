#include "pch.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "builder.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;

namespace Builder::Basic1
{
	// Product class
	class CarModel : public IPlasticModel
	{
	public:
		CarModel() : m_scale(12) {}
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
		ShipModel() : m_scale(700) {}
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

namespace Builder::Basic2
{
#if 1
	static string buildString(vector<string>& list)
	{
		stringstream ss;
		for (auto &s: list)
		{
			ss << ", " << s;
		}
		string ret = ss.str();
		return ret.substr(2);
	}

	static vector<string> findingIngredients(vector<string>& actual, vector<string>& expected)
	{
		vector<string> missing;
		for (auto& e : expected)
		{
			bool found = false;
			for (auto& a : actual)
			{
				if (a == e)
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				missing.push_back(e);
			}
		}
		return missing;
	}

	class OnionSoup : public IFood
	{
	public:
		virtual bool setIngredients(vector<string> ingredients) override
		{
			vector<string> missing = findingIngredients(ingredients, m_expectedIngredients);
			if (missing.size())
			{
				cout << "Not enough ingredients [" << buildString(missing) << "]\n";
				return false;
			}
			m_ingredients = ingredients;
			cout << "Ingredients for OnionSoup [ " << buildString(ingredients) << " ] were set\n";
			return true;
		}
		virtual void setCookingMethods(vector<string> methods) override
		{
			m_methods = methods;
		}
		virtual void cook() override
		{
			cout << " Cooking onion soup...\n";
			for (auto& s : m_methods)
			{
				cout << "  - " << s << endl;
			}
			cout << " Finished cooking. Ready to serve!\n";
			m_soup = "Onion Soup";
		}
		virtual string getFood() const override
		{
			return m_soup;
		}
	private:
		vector<string> m_expectedIngredients = { "Onion" };
		vector<string> m_ingredients;
		vector<string> m_methods;
		string m_soup;
	};

	class Cassoulet : public IFood
	{
	public:
		virtual bool setIngredients(vector<string> ingredients) override
		{
			vector<string> missing = findingIngredients(ingredients, m_expectedIngredients);
			if (missing.size())
			{
				cout << "Not enough ingredients [" << buildString(missing) << "]\n";
				return false;
			}
			m_ingredients = ingredients;
			cout << "Ingredients for Cassoulet [ " << buildString(ingredients) << " ] were set\n";
			return true;
		}
		virtual void setCookingMethods(vector<string> methods) override
		{
			m_methods = methods;
		}
		virtual void cook() override
		{
			cout << " Cooking cassoulet...\n";
			for (auto& s : m_methods)
			{
				cout << "  - " << s << endl;
			}
			cout << " Finished cooking. Ready to serve!\n";
			m_Cassoulet = "Casseoulet";
		}
		virtual string getFood() const override
		{
			return m_Cassoulet;
		}
	private:
		vector<string> m_expectedIngredients = { "Sausage", "Beans" };
		vector<string> m_ingredients;
		vector<string> m_methods;
		string m_Cassoulet;
	};

	class Ratatouille : public IFood
	{
	public:
		virtual bool setIngredients(vector<string> ingredients) override
		{
			vector<string> missing = findingIngredients(ingredients, m_expectedIngredients);
			if (missing.size())
			{
				cout << "Not enough ingredients [" << buildString(missing) << "]\n";
				return false;
			}
			m_ingredients = ingredients;
			cout << "Ingredients for Ratatouille [ " << buildString(ingredients) << " ] were set\n";
			return true;
		}
		virtual void setCookingMethods(vector<string> methods) override
		{
			m_methods = methods;
		}
		virtual void cook() override
		{
			cout << " Cooking ratatouille...\n";
			for (auto& s : m_methods)
			{
				cout << "  - " << s << endl;
			}
			cout << " Finished cooking. Ready to serve!\n";
			m_ratatouille = "Ratatouille";
		}
		virtual string getFood() const override
		{
			return m_ratatouille;
		}
	private:
		vector<string> m_expectedIngredients = { "Onion", "Pepper", "Courgette", "Aubergine" };
		vector<string> m_ingredients;
		vector<string> m_methods;
		string m_ratatouille;
	};

	class FrenchChef : public IChef
	{
	public:
		virtual void prepareIngredients(vector<string>& ingredients) override
		{
			// Check today's ingredients
			vector<string> missing = findingIngredients(ingredients, m_ingredientsForTodaysMenu);
			if (missing.size())
			{
				cout << "Not enough ingredients [" << buildString(missing) << "]\n";
				return;
			}
			vector<string> onionIngredients = { "Onion" };
			unique_ptr<IFood> onionSoup = make_unique<OnionSoup>();
			if (onionSoup->setIngredients(onionIngredients))
			{
				vector<string> cookingMethods = { "Cut onions", "Melt butter in a pan", "Fry onions", "Add stock", "Simmer"};
				onionSoup->setCookingMethods(cookingMethods);
				m_food.emplace_back(std::move(onionSoup));
			}

			vector<string> cassouletIngredients = { "Sausage", "Beans" };
			unique_ptr<IFood> cassoulet = make_unique<Cassoulet>();
			if (cassoulet->setIngredients(cassouletIngredients))
			{
				vector<string> cookingMethods = { "Cut ingredients", "Heat olive oil in a pan", "Fry ingredients", "Add stock", "Simmer long time" };
				cassoulet->setCookingMethods(cookingMethods);
				m_food.emplace_back(std::move(cassoulet));
			}

			vector<string> ratatouilleIngredients = { "Onion", "Pepper", "Courgette", "Aubergine" };
			unique_ptr<IFood> ratatouille = make_unique<Ratatouille>();
			if (ratatouille->setIngredients(ratatouilleIngredients))
			{
				vector<string> cookingMethods = { "Cut ingredients", "Heat olive oil in a pan", "Fry ingredients", "Add stock", "Simmer" };
				ratatouille->setCookingMethods(cookingMethods);
				m_food.emplace_back(std::move(ratatouille));
			}
		}
		virtual void cookFood() override
		{
			for (auto& f : m_food)
			{
				f->cook();
			}
		}
		virtual vector<unique_ptr<IFood>> getFood() override
		{
			vector<unique_ptr<IFood>> served;

			for (auto& f : m_food)
			{
				served.emplace_back(std::move(f));
			}
			m_food.clear();
			return served;
		}

	private:
		vector<string> m_ingredientsForTodaysMenu = { "Onion", "Sausage", "Beans", "Peppter", "Courgette", "Aubergine" };
		string m_menu;
		vector<unique_ptr<IFood>> m_food;
		vector<string> m_dbg;
	};

	class FrenchRestaurant : public IRestaurant
	{
	public:
		virtual void setChef(weak_ptr<IChef> chef) override
		{
			m_chef = std::static_pointer_cast<FrenchChef>(chef.lock());
		}
		virtual void open() override
		{
			shared_ptr<FrenchChef> chef = m_chef.lock();
			if (chef != nullptr)
			{
				chef->prepareIngredients(m_ingredients);
			}
			cout << "French restaurant is open\n";
		}
		virtual void close() override
		{
			cout << "French restaurant is close\n";
		}
		virtual void addOrder()
		{
			cout << "Order was added, our chef starts cooking now.\n";
			shared_ptr<FrenchChef> chef = m_chef.lock();
			if (chef != nullptr)
			{
				chef->cookFood();
			}
		}
		virtual void serveFood()
		{
			shared_ptr<FrenchChef> chef = m_chef.lock();
			if (chef != nullptr)
			{
				vector<unique_ptr<IFood>> food = chef->getFood();
				cout << "serving...\n";
				for (auto& f : food)
				{
					cout << " - " << f->getFood() << endl;
				}
			}
			cout << "Bon appetit!\n";
		}
	private:
		vector<string> m_ingredients = { "Onion", "Sausage", "Beans", "Peppter", "Courgette", "Aubergine" };
		weak_ptr<FrenchChef>	m_chef;
	};
#else
	class FrenchChef : public IChef
	{
	public:
		virtual void prepareIngredients(vector<string>& ingredients) override
		{
			
		}
		virtual void cookFood() override {}
		virtual vector<unique_ptr<IFood>> getFood() override
		{
			vector<unique_ptr<IFood>> served;
			
			for (auto& f : m_food)
			{
				served.emplace_back(std::move(f));
			}
			m_food.clear();
			return served;
		}
	private:
		vector<unique_ptr<IFood>> m_food;
	};
	class FrenchRestaurant : public IRestaurant
	{
	public:
		virtual void setChef(weak_ptr<IChef> chef) override {}
		virtual void open() override {}
		virtual void close() override {}
		virtual void addOrder() override {}
		virtual void serveFood() override {}
	};

#endif
int Tests::run()
	{
		FrenchRestaurant restaurant;
		shared_ptr<FrenchChef>	chef = make_shared<FrenchChef>();

		restaurant.setChef(chef);
		restaurant.open();
		restaurant.addOrder();
		restaurant.serveFood();

		// further tests needs to be implemented here

		return 0;
	}
}

