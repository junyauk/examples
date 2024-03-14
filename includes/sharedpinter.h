#pragma once
#ifndef SHAREDPOINTER_H_INCLUDED
#define SHAREDPOINTER_H_INCLUDED
#include <iostream>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::enable_shared_from_this;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;

namespace SharedPointer::Basic1
{
    // must_be_shared class forces using create() method to create an instance
    template<typename T>
    class must_be_shared : public enable_shared_from_this<T>
    {
    private:
        // These private structs are used for creating an instance of must_be_shared
        // So only must_be_shared class can create own instance.
        class MagicSeed {};
        class Magic { public: Magic(MagicSeed const&) noexcept {} };
    protected:
        using Magic_t = typename must_be_shared<T>::Magic;
        must_be_shared(Magic_t const&) noexcept {}
    public:
        virtual ~must_be_shared() {}

        template<typename...Args>
        static auto create(Args&&...args) noexcept
        {
            return make_shared<T>(Magic{ MagicSeed{} }, std::forward<Args>(args)...);
        }
        template<typename Derived, typename OutType = Derived, typename...Args>
        static auto create(Args&&...args) noexcept
        {
            return static_pointer_cast<OutType>
                (
                    make_shared<Derived>(Magic{ MagicSeed{} }, std::forward<Args>(args)...)
                );
        }

        template<typename X>
        auto shared_from_this_as() noexcept
        {
            return dynamic_pointer_cast<X>(must_be_shared<T>::shared_from_this());
        }
    };

    class Tests
    {
    public:
        int run();
    };
}

namespace SharedPointer::Basic2
{
    // This example denies direct instantiation
    // Object needs to be created by create() method
    class X : public enable_shared_from_this<X>
    {
    public:
        X(X const&) = delete;
        X(X&&) = delete;

        static shared_ptr<X> create(string str)
        {
            return make_shared<Helper<X>>(str);
        }
        string info() const
        {
            return "Instance of " + m_str;
        }

    private:
        X() {}
        X(string str) : m_str(str) {}

        template<typename Base>
        class Helper : public Base
        {
        public:
            template<typename...Args>
            explicit Helper(Args...args) : Base(std::forward<Args>(args)...) {}
        };
        string m_str;
    };

    class Tests
    {
    public:
        int run();
    };

}



#endif // SHAREDPOINTER_H_INCLUDED
