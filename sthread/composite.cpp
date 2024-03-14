#include "pch.h"

#include "composite.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::vector;

using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unique_ptr;
using std::make_unique;

namespace Composite::Basic1
{
	class CComponent : public IComponent
	{
	public:
		virtual void operation() const override
		{
			cout << "Component\n";
		}
	};

	class CComposite : public IComponent
	{
	public:
		void add(IComponent *component)
		{
			m_components.push_back(component);
		}
		void remove(IComponent* component)
		{
			auto it = std::find(m_components.begin(), m_components.end(), component);
			if (it != m_components.end())
			{
				m_components.erase(it);
			}
		}
		void operation() const override
		{
			cout << "CComposite operation()\n";
			for (auto& c : m_components)
			{
				c->operation();
			}
		}

	private:
		vector<IComponent*>	m_components;
	};

	int Tests::run()
	{
		CComponent c1;
		CComponent c2;
		CComposite composite;

		composite.add(&c1);
		composite.add(&c2);

		composite.operation();
		composite.remove(&c1);
		composite.remove(&c2);

		return 0;
	}
}


namespace Composite::Basic2
{
	class CFile : public IComponent
	{
	public:
		CFile(string const&name)
			: m_name(name)
		{}
		void operation() const override
		{
			cout << "File: " << m_name << endl;
		}
	private:
		string m_name;
	};

	class CDirectory : public IComponent
	{
	public:
		CDirectory(string const &name)
			: m_name(name)
		{}
		void operation() const override
		{
			cout << "Directory: " << m_name << endl;
			for (auto& component : m_children)
			{
#if 1
				auto c = component.lock();
				if (c != nullptr)
				{
					c->operation();
				}
#else
				component->operation();
#endif
			}
		}
		void add(const shared_ptr<IComponent>& component)
		{
			m_children.push_back(component);
		}
		void add(shared_ptr<IComponent>&& component)
		{
			m_children.push_back(component);
		}

		void remove(const shared_ptr<IComponent>& component)
		{
#if 1
			auto it = std::find_if(m_children.begin(), m_children.end(),
				[&](const weak_ptr<IComponent>& wp)
				{
					return !wp.expired() && wp.lock() == component;
				});
#else
			auto it = std::find(m_children.begin(), m_children.end(), component);
#endif
			if (it != m_children.end())
			{
				m_children.erase(it);
			}
		}

	private:
		string m_name;
#if 1
		// Note:
		// using weak_ptr would canse expiring issues.
		vector<weak_ptr<IComponent>> m_children;
#else
		vector<shared_ptr<IComponent>> m_children;

#endif
	};


	int Tests::run()
	{
		{
			auto file1 = make_shared<CFile>("file1.txt");
			auto file2 = make_shared<CFile>("file2.txt");
			auto file3 = make_shared<CFile>("file3.txt");
			auto file4 = make_shared<CFile>("file4.txt");

			CDirectory root("root");
			root.add(file1);
			root.add(file2);

			auto subdir = make_shared<CDirectory>("SubDir");
			auto subdir2 = make_shared<CDirectory>("SubDir2");
			subdir2->add(file4);

			// Note:
			// In the case of using weak_ptr, this shared_ptr will be expired immediately.
			// Because the created shared_ptr is temporary one.
			// This shared_ptr will be stored as weak_ptr, but it doesn't affect the ref count of the shared_ptr.
			// So after this line was done, the shared_ptr will be discarded,
			// and the stored weak_ptr is expired as well.
			subdir2->add(std::move(make_shared<CFile>("file5.txt")));

			subdir->add(file3);
			subdir->add(subdir2);

			root.add(subdir);

			root.operation(); // file5.txt wil not be displayed in the case of using weak_ptr

			root.remove(file2);

			root.operation();
		}

		{ // This test demonstrate the safe case of using weak_ptr
			// These shared_ptr will not be expired until this scope ends.
			auto file1 = make_shared<CFile>("file1.txt");
			auto file2 = make_shared<CFile>("file2.txt");
			auto file3 = make_shared<CFile>("file3.txt");
			auto file4 = make_shared<CFile>("file4.txt");
			auto file5 = make_shared<CFile>("file5.txt");
			auto subdir = make_shared<CDirectory>("SubDir");
			auto subdir2 = make_shared<CDirectory>("SubDir2");
			auto root = make_shared<CDirectory>("SubDir");

			root->add(file1);
			root->add(file2);
			subdir2->add(file4); // The shared_ptr will be stored as weak_ptr, but never expired.
			subdir2->add(file5); // The shared_ptr will be stored as weak_ptr, but never expired.
			subdir->add(file3);
			subdir->add(subdir2);

			root->add(subdir);

			root->operation();

			root->remove(file2);

			root->operation();


		}

		return 0;
	}
}