#pragma once
#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED

#include <iostream>
using std::cout;
using std::endl;
namespace Visitor::Basic1
{
	// They are needed for letting the IVisitor know actual elements
	class CElementA;
	class CElementB;

	class IVisitor
	{
	public:
		virtual ~IVisitor() = default;
		// The Visitor should know the actual elements,
		// and have methods for each ones.
		virtual void visitCElementA(CElementA& element) = 0;
		virtual void visitCElementB(CElementB& element) = 0;
	};

	class IElement
	{
	public:
		virtual ~IElement() = default;
		virtual void accept(IVisitor& visitor) = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Visitor::Basic2
{
	class Bakery;
	class Fishmonger;
	class PostOffice;

	// Implement this interface to access shops.
	class IVisitor
	{
	public:
		virtual ~IVisitor() {}
		virtual void visitBakery(Bakery& b) = 0;
		virtual void visitFishmonger(Fishmonger& f) = 0;
		virtual void visitPostOffice(PostOffice& p) = 0;
	};

	// Implement this interface to allow visitors to visit own shop
	class IShop
	{
	public:
		virtual ~IShop() {}
		virtual void accept(IVisitor&) = 0;
	};

	class Bakery : public IShop
	{
	public:
		Bakery()
			: m_balance(1000)
			, m_numBread(0)
		{
			cout << "Bakery is open now\n";
		}
		// Shop accepts visitor, but what the visitor does at the shop depends on the visitor
		virtual void accept(IVisitor& errand) override { errand.visitBakery(*this); }

		// Bakery will sell bread to consumer and also get ingredients from producer.
		void buyBread() { cout << "Bakery sells Bread()\n"; m_balance += 100; m_numBread--; }
		void sellFlower() { cout << "Bakery buys Flower()\n"; m_balance -= 100; m_numBread++; }
		int getBalance() const { return m_balance; }
		int getNumBread() const { return m_numBread; }
	private:
		int m_balance;
		int m_numBread;
	};

	class Fishmonger : public IShop
	{
	public:
		Fishmonger()
			: m_balance(1000)
			, m_numFish(0)
		{
			cout << "Fishmonger is open now\n";
		}
		// Shop accepts visitor, but what the visitor does at the shop depends on the visitor
		virtual void accept(IVisitor& errand) override { errand.visitFishmonger(*this); }

		// Fishmonger will sell fish to consumer and also get new fish from producer.
		void buyFish() { cout << "Fishmonger sells Fish()\n"; m_balance += 100; m_numFish--; }
		void sellFish() { cout << "Fishmonger buys Fish()\n"; m_balance -= 100; m_numFish++; }
		int getBalance() const { return m_balance; }
		int getNumFish() const { return m_numFish; }
	private:
		int m_balance;
		int m_numFish;
	};


	class PostOffice : public IShop
	{
	public:
		PostOffice()
			: m_numParcel(100)
		{
			cout << "PostOffice is open now\n";
		}
		// Shop accepts visitor, but what the visitor does at the shop depends on the visitor
		virtual void accept(IVisitor& errand) override { errand.visitPostOffice(*this); }

		// PostOffice will receive a parcel from company and pass it to consumer.
		void postParcel() { cout << "PostOffice got a parcel()\n"; m_numParcel++; }
		void receiveParcel() { cout << "PostOffice gave you a parcel()\n"; m_numParcel--; }
		int getNumParcels() { return m_numParcel; }
	private:
		int m_numParcel;
	};

	// Each visitor visit shops with different purpose.
	class CProducer : public IVisitor
	{
		// Producer will deliver different products to each shop
	public:
		void visitBakery(Bakery& b)
		{
			cout << "Errand is visiting Bakery\n";
			b.sellFlower();
			cout << "Errand visited Bakery\n";
		}
		void visitPostOffice(PostOffice& p)
		{
			cout << "Errand is visiting PostOffice\n";
			p.postParcel();
			cout << "Errand visited PostOffice\n";
		}
		void visitFishmonger(Fishmonger& f)
		{
			cout << "Errand is visiting FishMonger\n";
			f.sellFish();
			cout << "Errand visited FishMonger\n";
		}
	};

	class CConsumer : public IVisitor
	{
		// Consumer will come to each shop for getting something
	public:
		void visitBakery(Bakery& b)
		{
			cout << "Errand is visiting Bakery\n";
			b.buyBread();
			cout << "Errand visited Bakery\n";
		}
		void visitPostOffice(PostOffice& p)
		{
			cout << "Errand is visiting PostOffice\n";
			p.receiveParcel();
			cout << "Errand visited PostOffice\n";
		}
		void visitFishmonger(Fishmonger& f)
		{
			cout << "Errand is visiting FishMonger\n";
			f.buyFish();
			cout << "Errand visited FishMonger\n";
		}
	};

	class Tests
	{
	public:
		int run();
	};
}

#endif //VISITOR_H_INCLUDED