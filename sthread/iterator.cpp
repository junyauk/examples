#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "iterator.h"


using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unique_ptr;
using std::make_unique;

namespace Iterator::Basic1
{
	class CIterator : public IIterator
	{
	public:
		CIterator(vector<int> const& data) : m_data(data) , m_pos(0) {}
		virtual bool hasNext() const override { return m_pos < m_data.size(); }
		virtual int next() override { return m_data[m_pos++]; }
	private:
		vector<int> m_data;
		size_t m_pos;
	};

	class CAggregate : public IAggregate
	{
	public:
		virtual IIterator* createIterator() override { return new CIterator(m_data); }
		virtual void addData(int val) override { m_data.push_back(val); }
	private:
		vector<int> m_data;
	};

	int Tests::run()
	{
		CAggregate aggregate;
		aggregate.addData(10);
		aggregate.addData(20);
		aggregate.addData(30);

		IIterator* it = aggregate.createIterator();

		while (it->hasNext())
		{
			cout << it->next() << endl;
		}

		delete it;

		return 0;
	}
}

namespace Iterator::Basic2
{
	int Tests::run()
	{
		vector<int> vec = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		CustomIterator begin(vec.begin());
		CustomIterator end(vec.end());

		for (CustomIterator it = begin; it != end; ++it)
		{
			cout << *it << " ";
		}
		cout << endl;

		return 0;
	}
}
