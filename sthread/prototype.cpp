#include "pch.h"

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <memory>

#include "prototype.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::make_unique;

namespace Prototype::Basic1
{
	class CPrototype : public IPrototype
	{
	public:
		CPrototype(string const& name) : m_name(name) {}
		CPrototype(CPrototype const& other) : m_name(other.m_name) {}
		virtual unique_ptr<IPrototype> clone() const override
		{
			return make_unique<CPrototype>(*this);
		}
		virtual string info() const override
		{
			return m_name;
		}
	private:
		string m_name;
	};

	int Tests::run()
	{
		CPrototype original("Original prototype");
		cout << original.info() << endl;
		
		unique_ptr<IPrototype> cloned = original.clone();
		cout << cloned->info() << endl;

		return 0;
	}
}


namespace Prototype::Basic2
{
	class CPrototype : public IPrototype
	{
	public:
		CPrototype(string const& name) : m_name(name) {}
		CPrototype(CPrototype const& other) : m_name(other.m_name) {}
		virtual IPrototype* clone() const override
		{
			// create a new instance
			return new CPrototype(*this);
		}
		virtual void printInfo() const override
		{
			cout << "CPrototype: " << m_name << endl;
		}
	private:
		string m_name;
	};

	// This class is just managing prototypes
	class PrototypeRegistry
	{
	public:
		void addPrototype(string const& name, IPrototype* prototype)
		{
			m_prototypes[name] = prototype;
		}
		IPrototype* getPrototype( string const&key)
		{
			if (m_prototypes.find(key) != m_prototypes.end())
			{
				return m_prototypes[key]->clone();
			}
			return nullptr;
		}
	private:
		unordered_map<string, IPrototype*> m_prototypes;
	};

	int Tests::run()
	{
		PrototypeRegistry	registry;

		registry.addPrototype("Fyana", new CPrototype("Fyana"));
		registry.addPrototype("Ypsilon", new CPrototype("Ypsilon"));

		// clone prototype from the registry
		IPrototype* clone1 = registry.getPrototype("Fyana");
		IPrototype* clone2 = registry.getPrototype("Ypsilon");

		if (clone1)
		{
			clone1->printInfo();
		}
		if (clone2)
		{
			clone2->printInfo();
		}

		return 0;
	}
}