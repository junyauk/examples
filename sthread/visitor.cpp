#include "pch.h"
#include <iostream>
#include <vector>
#include <memory>

#include "visitor.h"

using std::cout;
using std::endl;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;


namespace Visitor::Basic1
{
	class CVisitor : public IVisitor
	{
	public:
		void visitCElementA(CElementA& element) override
		{
			cout << "Visitor is visiting CElementA\n";
		}
		void visitCElementB(CElementB& element) override
		{
			cout << "Visitor is visiting CElementB\n";
		}
	};
	class CElementA : public IElement
	{
	public:
		void accept(IVisitor& visitor) override
		{
			visitor.visitCElementA(*this);
		}
	};

	class CElementB : public IElement
	{
	public:
		void accept(IVisitor& visitor) override
		{
			visitor.visitCElementB(*this);
		}
	};

	class ObjectStructure
	{
	public:
		void attach(weak_ptr<IElement> element)
		{
			m_elements.push_back(element);
		}

		void accept(IVisitor& visitor)
		{
			for (auto &e : m_elements)
			{
				if (auto element = e.lock())
				{
					element->accept(visitor);
					// or visitor->visit(element);
				}
				else
				{
					cout << "The element no longer exists\n";
				}
			}
		}
	private:
		vector<weak_ptr<IElement>> m_elements;
	};

	int Tests::run()
	{
		CVisitor visitor;
		shared_ptr<IElement> a = make_shared<CElementA>();
		shared_ptr<IElement> b = make_shared<CElementB>();

		ObjectStructure	os;
		os.attach(a);
		os.attach(b);

		os.accept(visitor);

		return 0;
	}
}


namespace Visitor::Basic2
{
	// Note:
	// These classes will need to move to unit test file,
	// and each interface will need to be tested individually.

	// Various visitors come to shops with different purpose.
	// There are various shops and they accept visitors.
	class ShoppingMall : public IShop
	{
	public:
		void addShop(weak_ptr<IShop> shop)
		{
			m_shops.emplace_back(shop);
		}
		virtual void accept(IVisitor& errand)
		{
			for (auto w : m_shops)
			{
				if (auto shop = w.lock())
				{
					shop->accept(errand);
				}
				else
				{
					cout << "The shop no longer exists\n";
				}
			}
		}
	private:
		vector<weak_ptr<IShop>>	m_shops;
	};

	int Tests::run()
	{
		CConsumer	consumer; // as Visitor
		CProducer	producer; // as Visitor
		Bakery	bakery;
		PostOffice postoffice;
		IShop* shop;


		shop = &bakery;
		shop->accept(producer);
		shop->accept(producer);
		shop->accept(consumer);
		shop = &bakery;
		shop->accept(producer);
		shop->accept(producer);
		shop->accept(consumer);

		ShoppingMall mall;

		// Setup Shops in the shopping mall
		shared_ptr<Bakery> pBakery1 = make_shared<Bakery>();
		shared_ptr<Bakery> pBakery2 = make_shared<Bakery>();
		shared_ptr<Fishmonger> pFishmonger1 = make_shared<Fishmonger>();
		shared_ptr<PostOffice> pPostOffice = make_shared<PostOffice>();
		mall.addShop(pBakery1);
		mall.addShop(pBakery2);
		mall.addShop(pFishmonger1);
		mall.addShop(pPostOffice);

		// Deliver products
		mall.accept(producer);
		mall.accept(producer);
		mall.accept(producer);

		// Shopping mall is reado to open
		mall.accept(consumer);

		cout << "Bakery1 balance: " << pBakery1->getBalance() << endl;
		cout << "Bakery1 numBreads: " << pBakery1->getNumBread() << endl;
		cout << "Bakery2 balance: " << pBakery2->getBalance() << endl;
		cout << "Bakery2 numBreads: " << pBakery2->getNumBread() << endl;
		cout << "Fishmonger1 balance: " << pFishmonger1->getBalance() << endl;
		cout << "Fishmonger1 numFish: " << pFishmonger1->getNumFish() << endl;
		cout << "PostOffice numParcels: " << pPostOffice->getNumParcels() << endl;

		cout << "Another consumers are comming\n";
		mall.accept(consumer);
		mall.accept(consumer);
		cout << "Bakery1 balance: " << pBakery1->getBalance() << endl;
		cout << "Bakery1 numBreads: " << pBakery1->getNumBread() << endl;
		cout << "Bakery2 balance: " << pBakery2->getBalance() << endl;
		cout << "Bakery2 numBreads: " << pBakery2->getNumBread() << endl;
		cout << "Fishmonger1 balance: " << pFishmonger1->getBalance() << endl;
		cout << "Fishmonger1 numFish: " << pFishmonger1->getNumFish() << endl;
		cout << "PostOffice numParcels: " << pPostOffice->getNumParcels() << endl;

		return 0;
	}
}
