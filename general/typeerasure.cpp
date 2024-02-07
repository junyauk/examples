#include "pch.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;
using std::function;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::destroy_at;

namespace TypeErasureStep1
{
	auto func(int value) -> float
	{
		return value * 2.0f;
	}

	struct functor
	{
		auto operator()(int value) const -> float
		{
			return value * 3.0f;
		}
	};

	auto const lambda = [](int value) {return static_cast<float>(value * 4.0f); };

	void test()
	{
		stringstream ss;

		function<auto(int) ->float> f;
		f = func;
		ss << "func(" << f(10) << ")" << endl;
		f = functor{};
		ss << "functor(" << f(10) << ")" << endl;
		f = lambda;
		ss << "lambda(" << f(10) << ")" << endl;
		cout << ss.str();

		// Note:
		// std::function can treat various cases.
		// But it runs memory allocation when runtime
		// It can be a performance problem
	}
} // TypeErasureStep1

namespace TypeErasureStep2	// use template
{
	template<typename T>
	class Function
	{
	public:
		Function(T f) : m_f(std::move(f)) {}

		auto operator()(int value) -> float
		{
			return m_f(value);
		}
	private:
		T m_f;
	};

	class Type
	{
	public:
		auto operator()(int value) -> float
		{
			return value * 5.0f;
		}
	};

	auto test() -> void
	{
		stringstream ss;
		Function<Type> f{ Type{} }; // Still using "<Type>"
		ss << "Templated Function(" << f(10) << ")" << endl;
		cout << ss.str();

		// Note:
		// This Function can treat classes which have got "operator()".
		// Once f was instanced, it is not reusable anymore.
		// It can't be array or list.
	}
} // TypeErasureStep2

namespace TypeErasureStep3 // invisible type information
{
	class Function
	{
		// This class keeps the original class in void*
		// It is newed at constructor and deleted at destructor.
		// This class has got operator() for running like a typical function.
		// 
	public:
		template<typename T>
		Function(T f)
			: p_{ new T{std::move(f)} }
			, call_{&Function::do_call_<T>}
			, delete_{&Function::do_delete_<T>}
		{}
		~Function()
		{
			delete_(p_);
		}
		auto operator()(int value) -> float
		{
			return call_(p_, value);
		}

		// Copy constructor is unsupported
		Function(Function const&) = delete;
		auto operator=(Function const&) -> Function& = delete;

	private:
		// This is the pointer holing the specified class
		void* p_{};

		// These are function pointers initialised at constructor;
		auto (*call_)(void*, int) -> float {};
		auto (*delete_)(void*) noexcept -> void {};

		// This is the function for processing operator().
		template<typename F>
		static auto do_call_(void* p, int value) -> float
		{
			return (*static_cast<F*>(p))(value);
		}

		// This is the function for terminating the class
		template<typename F>
		static auto do_delete_(void* p) noexcept -> void
		{
			delete static_cast<F*>(p);
		}
	};

	class Type
	{
	public:
		auto operator()(int value) -> float
		{
			return value * 6.0f;
		}
	};

	auto test() -> void
	{
		stringstream ss;
		Function f{ Type{} };	// used be "Function<Type>"
		ss << "TypeErasured(" << f(10) << ")" << endl;
		cout << ss.str();
	}

} // TypeErasureStep3

namespace TypeErasureStep4 // using inheritance
{
	class Function
	{
	public:
		// Use template for constructor
		// The original class will be newed and stored in Base_ pinter
		// Once it is stored as Base_, its type information is erased.
		// But Derived class still has got the original class type information,
		// and methods and destructor of Derived ones will be called
		template<typename T>
		Function(T f)
			:p_{ new Derived_<T>{std::move(f)} }
		{}

		~Function()
		{
			// this pointer is Base_, but Derived_'s destructor will be called
			delete p_;
		}

		// return value and parameters should be the same as functions
		// that you'd like to type erase class's ones.
		// This example is the case of "float func(int value)".
		auto operator()(int value) -> float
		{
			// simply call the method
			// this pointer is Base_, but Derived_'s call will be called
			return p_->call(value);
		}

		// copy constructor is not supported
		Function(Function const&) = delete;
		auto operator=(Function const&)->Function & = delete;

	private:
		// base struct (class) that is used for keeping 
		// the original class without class type information
		struct Base_
		{
			virtual ~Base_() {}					// the derived class's destructor will be called
			virtual auto call(int) -> float = 0;// derived class needs to inherit
		};

		// derived struct (class) that is used for 
		template<typename T>
		struct Derived_ : Base_
		{
			T f_;	// Keep the riginal class type information here
			Derived_(T f) : f_{ std::move(f) }
			{}

			// support the function calling the original function
			// Function class will call Base_'s call, 
			// but this Derived_'s call will be called actually
			auto call(int value) -> float override
			{
				return f_(value);
			}
		};

		// This is the pointer
		Base_* p_{nullptr}; // Declaration and initialization at once
	};

	// any function, class or lambda which can run with a parameter "int"
	// function
	auto func(int value) -> float
	{
		return value * 2.0f;
	}
	// class
	class functor
	{
	public:
		auto operator()(int value) const -> float
		{
			return value * 3.0f;
		}
	};
	// lambda
	auto const lambda = [](int value) {return static_cast<float>(value * 4.0f); };


	auto test() -> void
	{
		stringstream ss;
		{
			Function f{ func };
			ss << "TypeErasure using inheritance running with function(" << f(10) << ")" << endl;
		}

		{
			Function f{ functor{} };
			ss << "TypeErasure using inheritance running with class(" << f(10) << ")" << endl;
		}

		{
			Function f{ lambda };
			ss << "TypeErasure using inheritance running with lambda(" << f(10) << ")" << endl;
		}

		cout << ss.str();
	}

} // TypeErasureStep4

namespace TypeErasureStep5 // Removing new and delete for avoiding runtime memory allocation
{
	template<typename T, std::size_t Size, std::size_t Align>
	class Function
	{
	public:
		template<
			typename T, 
			std::enable_if_t<(sizeof(T) <= Size)> * = nullptr,
			std::enable_if_t<Align % alignof(T) == 0u> * = nullptr>
		Function(T f)
		: m_pBase(::new(m_storage) Derived<T>(std::move(f)))
		{
			// this constructor will create a Derived class instance first 
			// using the original class "T".
			// the created class will be stored in m_pBase which
			// is allocated from m_storage.
			// in other words, m_storage and m_pBase are the same address.
			cout << "m_storage: " << m_storage[0] << endl;
			cout << "m_pBase: " << m_pBase << endl;
		}
		
		~Function()
		{
			// The actual memory "m_storage" can't be freed,
			// but destructor might need to be called.
			// so calling destroy_at() wil call Derived's destructor.
			destroy_at(m_pBase);
		}

		// support function, class and lambda type call.
		// their parameter is int and return float.
		auto operator()(int value) -> float
		{
			return m_pBase->call(value);
		}

		// copy constructor is unsupported 
		Function(const Function&) = delete;
		auto operator=(Function const&) -> Function & = delete;

	private:
		struct Base
		{
		public:
			virtual ~Base()
			{
				cout << "Base's destructor\n";	// will be called next
			}
			virtual auto call(int) -> float = 0;
		};

		template<typename T>
		struct Derived : Base
		{
		public:
			T m_t;
			Derived(T t)
				: m_t(std::move(t)) {}
			~Derived()
			{
				cout << "Derived's destructor\n";	// will be called first
			}
			auto call(int value) -> float override
			{
				return m_t(value);
			}
		};

		// allocate a fixed memory, that the size (32u) is specified when compiling
		// m_pBase will be newed from this memory.
		// in other words, create a strage as a class member and placement new using this memory,
		// instead of allocating from heap.
//		__declspec(align(Align))	char m_storage[Size];	// VC
//		char m_storage[Size] __attribute(aligned(Align));	// gcc
		alignas(Align) char m_storage[Size];	// C++
		Base	*m_pBase{};
	};

	using Task = Function<auto (int) -> float, 32u, alignof(max_align_t)>;


	// any function, class or lambda which can run with a parameter "int"
	// function
	auto func(int value) -> float
	{
		return value * 2.0f;
	}
	// class
	class functor
	{
	public:
		auto operator()(int value) const -> float
		{
			return value * 3.0f;
		}
	};

	// lambda
	auto const lambda = [](int value) {return static_cast<float>(value * 4.0f); };

	auto test() -> void
	{
		stringstream ss;
		{
			Task f{ func };
			ss << "TypeErasure avoiding runtime allocation running with function(" << f(10) << ")" << endl;
		}

		{
			Task f{ functor{} };
			ss << "TypeErasure avoiding runtime allocation running with class(" << f(10) << ")" << endl;
		}

		{
			Task f{ lambda };
			ss << "TypeErasure avoiding runtime allocation running with lambda(" << f(10) << ")" << endl;
		}

		cout << ss.str();
	}


} // TypeErasureStep5

namespace TypeErasureStep6 // Simplified and class separated example for typical use with smart pointer
{
	class Base
	{
	public:
		Base() = default;
		virtual ~Base() = default; // virtual destructor makes sure derived class's destrunctor is called
		virtual auto call(int value) -> float = 0;
	};

	template<class T>
	class Derived : public Base
	{
	public:
		Derived( T t)
			: m_t(t){} // Keep the original class type informatino
		virtual ~Derived() = default;
		virtual auto call(int value) -> float override
		{
			cout << "Derived class's call\n";
			return m_t(value);
		}

	private:
		T	m_t; // Keep the original class type information
	};

	class Function
	{
	public:
		template<class T>
		Function(T t)
			: m_pBase(make_unique<Derived<T>>(t)) {}

		auto operator()(int value) -> float
		{
			cout << "Function's operator() calls m_pBase->call()\n";
			return m_pBase->call(value);
		}

	private:
		// Keep a pointer of the Base class
		std::unique_ptr<Base>	m_pBase;
	};

	// any function, class or lambda which can run with a parameter "int"
	// function
	auto func(int value) -> float
	{
		return value * 2.0f;
	}
	// class
	class functor
	{
	public:
		auto operator()(int value) const -> float
		{
			return value * 3.0f;
		}
	};

	// lambda
	auto const lambda = [](int value) {return static_cast<float>(value * 4.0f); };

	auto test() -> void
	{
		stringstream ss;
		{
			Function f{ func };
			ss << "Simplified TypeErasure running with function(" << f(10) << ")" << endl;
		}

		{
			Function f{ functor{} };
			ss << "Simplified TypeErasure running with class(" << f(10) << ")" << endl;
		}

		{
			Function f{ lambda };
			ss << "Simplified TypeErasure running with lambda(" << f(10) << ")" << endl;
		}

		cout << ss.str();
	}
} // TypeErasureStep6

namespace TypeErasureExercise1
{
	struct Something
	{
		int m_value;
		string m_str;
	};

	class Base
	{
	public:
		// needs to have ...
		//  1. the virtual destructor
		//  2. the method which can process using Something
		virtual ~Base() = default;
		virtual auto run(const Something &obj) -> Something = 0;
	};

	template<class T>
	class Derived : public Base
	{
	public:
		Derived(T t)
			: m_t(t)
		{}
		~Derived()
		{
			cout << "Derived's destructor\n";
		}
		auto run(const Something& obj) -> Something override
		{
			return m_t(obj);
		}
	private:
		// needs to have an instance of the original class
		T	m_t;
	};

	class Function
	{
	public:
		template<class T>
		Function(T t)
			: m_pBase(make_unique < Derived<T>>(t)) {}
		auto operator()(const Something& obj) -> Something
		{
			return m_pBase->run(obj);
		}
	private:
		unique_ptr<Base> m_pBase;
	};

	Something func(const Something& obj)
	{
		return { obj.m_value + 1, "called from function"};
	}

	class functor
	{
	public:
		auto operator()(const Something& obj) -> Something
		{
			return { obj.m_value + 1, "called from operator"};
		}
	};

	auto lambda = [](const Something& obj) -> Something
		{
			return { obj.m_value + 1, "called from lambda"};
		};

	auto test() -> void
	{
		stringstream ss;
		{
			Function f{ func };
			Something obj{ 10, "the original value is 10" };
			Something ret = f(obj);
			auto result = "Value: " + std::to_string(ret.m_value) + ", str: " + ret.m_str;
			ss << "Exercise1 TypeErasure running with function(" << result << ")" << endl;
		}

		{
			Function f{ functor{} };
			Something obj{ 20, "the original value is 20" };
			Something ret = f(obj);
			auto result = "Value: " + std::to_string(ret.m_value) + ", str: " + ret.m_str;
			ss << "Exercise1 TypeErasure running with class(" << result << ")" << endl;
		}

		{
			Function f{ lambda };
			Something obj{ 30, "the original value is 30" };
			Something ret = f(obj);
			auto result = "Value: " + std::to_string(ret.m_value) + ", str: " + ret.m_str;
			ss << "Exercise1 TypeErasure running with lambda(" << result << ")" << endl;
		}

		cout << ss.str();
	}
} // TypeErasureExercise1

namespace TypeErasureExercise2
{
	struct Something
	{
		int m_value;
		string m_str;
	};

	class Function
	{
	public:
		template<class T>
		Function(T t)
			: m_pBase(make_unique<Derived<T>>(t)) {}
		auto operator()(const Something& obj) -> Something
		{
			return m_pBase->run(obj);
		}
	private:
		class Base
		{
		public:
			virtual ~Base() = default;
			virtual Something run(const Something& obj) = 0;
		};

		template<class T>
		class Derived : public Base
		{
		public:
			Derived(T t) : m_t(t) {}
			Something run(const Something& obj) override
			{
				return m_t(obj);
			}
		private:
			T	m_t;
		};

		unique_ptr<Base>	m_pBase;
	};

	Something func(const Something& obj)
	{
		return { obj.m_value + 1, "called from function" };
	}

	class functor
	{
	public:
		auto operator()(const Something& obj) -> Something
		{
			return { obj.m_value + 1, "called from operator" };
		}
	};

	auto lambda = [](const Something& obj) -> Something
		{
			return { obj.m_value + 1, "called from lambda" };
		};


	auto test() -> void
	{
		stringstream ss;
		{
			Function f{ func };
			Something obj{ 10, "the original value is 10" };
			Something ret = f(obj);
			auto result = "Value: " + std::to_string(ret.m_value) + ", str: " + ret.m_str;
			ss << "Exercise1 TypeErasure running with function(" << result << ")" << endl;
		}

		{
			Function f{ functor{} };
			Something obj{ 20, "the original value is 20" };
			Something ret = f(obj);
			auto result = "Value: " + std::to_string(ret.m_value) + ", str: " + ret.m_str;
			ss << "Exercise1 TypeErasure running with class(" << result << ")" << endl;
		}

		{
			Function f{ lambda };
			Something obj{ 30, "the original value is 30" };
			Something ret = f(obj);
			auto result = "Value: " + std::to_string(ret.m_value) + ", str: " + ret.m_str;
			ss << "Exercise1 TypeErasure running with lambda(" << result << ")" << endl;
		}

		cout << ss.str();
	}

}

int Run_TypeErasureStep1()
{
	cout << "** Run_TypeErasureStep1" << endl;
	TypeErasureStep1::test();
	cout << endl;
	return 0;
}

int Run_TypeErasureStep2()
{
	cout << "** Run_TypeErasureStep2" << endl;
	TypeErasureStep2::test();
	cout << endl;
	return 0;
}

int Run_TypeErasureStep3()
{
	cout << "** Run_TypeErasureStep3" << endl;
	TypeErasureStep3::test();
	cout << endl;
	return 0;
}

int Run_TypeErasureStep4()
{
	cout << "** Run_TypeErasureStep4" << endl;
	TypeErasureStep4::test();
	cout << endl;
	return 0;
}

int Run_TypeErasureStep5()
{
	cout << "** Run_TypeErasureStep5" << endl;
	TypeErasureStep5::test();
	cout << endl;
	return 0;
}

int Run_TypeErasureStep6()
{
	cout << "** Run_TypeErasureStep6" << endl;
	TypeErasureStep6::test();
	cout << endl;
	return 0;
}

int Run_TypeErasureExercise1()
{
	cout << "** Run_TypeErasureExercise2" << endl;
	TypeErasureExercise2::test();
	cout << endl;
	return 0;
}
