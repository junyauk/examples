#include "pch.h"

#include <iostream>

#include "abstractfactory.h"

using std::cout;
using std::endl;

namespace AbstractFactory::Basic1
{
    class CProductA1 : public IProductA
    {
    public:
        ~CProductA1()
        {
            cout << "ProductA1 destructor\n";
        }
        string FuncA() const override
        {
            return "Result of the Product A1";
        }
    };

    class CProductA2 : public IProductA
    {
    public:
        ~CProductA2()
        {
            cout << "ProductA2 destructor\n";
        }
        string FuncA() const override
        {
            return "Result of the ProductA2";
        }
    };


    class CProductB1 : public IProductB
    {
    public:
        ~CProductB1()
        {
            cout << "ProductB1 destructor\n";
        }
        string FuncB() const override
        {
            return "Result of the ProductB1";
        }
        string FuncBwithProductA(IProductA const& a) const override
        {
            return "Result of the ProductB1 with [ " + a.FuncA() + " ]";
        }
    };

    class CProductB2 : public IProductB
    {
    public:
        ~CProductB2()
        {
            cout << "ProductB2 destructor\n";
        }
        string FuncB() const override
        {
            return "Result of the ProductB2";
        }
        string FuncBwithProductA(IProductA const& a) const override
        {
            return "Result of the ProductB2 with [ " + a.FuncA() + " ]";
        }
    };

    class CFactory1 :public IFactory
    {
    public:
        ~CFactory1()
        {
            cout << "CFactory1 destructor\n";
        }
        IProductAuptr createProductA() const override
        {
            return make_unique<CProductA1>();
        }
        IProductBuptr createProductB() const override
        {
            return make_unique<CProductB1>();
        }
    };

    class CFactory2 :public IFactory
    {
    public:
        ~CFactory2()
        {
            cout << "CFactory2 destructor\n";
        }
        IProductAuptr createProductA() const override
        {
            return make_unique<CProductA2>();
        }
        IProductBuptr createProductB() const override
        {
            return make_unique<CProductB2>();
        }
    };

    // for local test
    static void run(IFactory& factory)
    {
        IProductAuptr a = factory.createProductA();
        IProductBuptr b = factory.createProductB();
        cout << a->FuncA() << endl;
        cout << b->FuncB() << endl;
        cout << b->FuncBwithProductA(*a);
    }

    int Run_AbstractFactory()
    {
        cout << "Client : Testing client code with the 1st factory type: " << endl;
        {
            unique_ptr<CFactory1> f1 = make_unique<CFactory1>();
            run(*f1);
        }
        // Check if f1's destructor was called.

        {
            unique_ptr<CFactory2> f2 = make_unique<CFactory2>();
            run(*f2);
        }
        // Check if f2's destructor was called.

        cout << "Client : Finish" << endl;
        return 0;
    }
}


