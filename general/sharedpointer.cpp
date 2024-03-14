#include "pch.h"

#include<tuple>

#include "sharedpinter.h"

using std::tuple;

namespace SharedPointer::Basic1
{
    class FOO : public must_be_shared<FOO>
    {
    public:
        FOO(Magic_t const& magic, int foo_value)
            : must_be_shared<FOO>(magic)
            , foo_value_(foo_value)
        {}
        void foo()
        {
            auto instance = shared_from_this();
            cout << instance->foo_value_ << endl;
        }
    private:
        int foo_value_;
    };

    class BAR : public FOO
    {
    public:
        BAR(Magic_t const& magic, int barVal = 200, int fooVal = 100)
            : FOO(magic, fooVal)
            , bar_value_(barVal)
        {}
        void bar()
        {
            auto instance = shared_from_this_as<BAR>();
            cout << instance->bar_value_ << endl;
        }
    private:
        int bar_value_;
    };

    int Tests::run()
    {
        auto foo = FOO::create(10);
        foo->foo();

        auto bar = BAR::create<BAR>();
        bar->foo();
        bar->bar();

        shared_ptr<BAR> sp = BAR::create<BAR>(5, 15);
        sp->foo();
        sp->bar();

        auto bar_as_foo = BAR::create<BAR, FOO>(7, 14);
        bar_as_foo->foo();
        bar_as_foo->shared_from_this_as<BAR>()->bar();

        return 0;
    }
}

namespace SharedPointer::Basic2
{
    int Tests::run()
    {
        shared_ptr<X> x = X::create("Hello Japan");
        cout << x->info() << endl;

        return 0;
    }
}
