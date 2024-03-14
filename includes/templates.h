#pragma once
#ifndef TEMPLATES_H_INCLUDED
#define TEMPLATES_H_INCLUDED

#include <string>
#include <vector>
#include <concepts>
using std::string;
using std::vector;

namespace TemplateExamples::IsSameType
{
	template<typename T, typename U>
	struct isSameImpl
	{
		using value = std::false_type;
	};

	template<typename T>
	struct isSameImpl<T, T>
	{
		using value = std::true_type;
	};

	template<typename T, typename U>
	struct isSame : public isSameImpl<T, U>::value
	{
	};
}

namespace TemplateExamples::MyArray
{
	template<typename Array>
	class array_iterator
	{
	public:
		using iterator = array_iterator;

		array_iterator(Array&a, std::size_t const& i)
			: m_a(a)
			, m_i(i)
			, m_maxSize(m_a.size())
		{}

		// * and [] returning reference of the actual element
		Array::reference operator*() const
		{
			if ((m_i < 0) || (m_i >= m_maxSize))
			{
				throw std::out_of_range("cannot dereference out of range array iterator");
			}
			return m_a[m_i];
		}
		Array::reference operator[](std::size_t const &i) const
		{
			auto index = static_cast<signed long long>(m_i + i);
			if ((index < 0) || (index >= static_cast<signed long long>(m_maxSize)))
			{
				throw std::out_of_range("cannot dereference out of range array iterator");
			}
			return *(*this + i); // m_a[m_i + i];
		}

		// ++ and -- returning reference
		array_iterator& operator++()
		{
			if (m_i + 1 >= m_maxSize)
			{
				throw std::out_of_range("cannot increment array iterator past end");
			}
			++m_i;
			return *this;
		}
		array_iterator& operator--()
		{
			if (m_i == 0)
			{
				throw std::out_of_range("cannot decrement array iterator before begin");
			}
			--m_i;
			return *this;
		}

		// ++(int) and --(int) returning copy
		array_iterator operator++(int)
		{
			array_iterator copy = *this;
			++*this;
			return copy;
		}
		array_iterator operator--(int)
		{
			array_iterator copy = *this;
			--*this;
			return copy;
		}

		// += and -= returning reference
		array_iterator &operator+=(std::size_t const &n)
		{
			auto index = static_cast<signed long long>(m_i + n);
			if (index >= static_cast<signed long long>(m_maxSize))
			{
				throw std::out_of_range("cannot increment array iterator past end");
			}
			else if (index < 0)
			{
				throw std::out_of_range("cannot decrement array iterator before begin");
			}
			m_i += n;
			return *this;
		}
		array_iterator& operator-=(std::size_t const &n)
		{
			if (static_cast<signed long long>(m_i - n) < 0)
			{
				throw std::out_of_range("cannot decrement array iterator before begin");
			}
			m_i -= n;
			return *this;
		}

		// + and - returning copy
		array_iterator operator+(std::size_t const& n) const
		{
			array_iterator copy = *this;
			copy += n;
			return copy;
		}
		array_iterator operator-(std::size_t const& n) const
		{
			array_iterator copy = *this;
			copy -= n;
			return copy;
		}

		// ==, !=, <=, >=, < and > returing bool
		bool operator==(array_iterator const& right) { return m_i == right.m_i; }
		bool operator!=(array_iterator const& right) { return m_i != right.m_i; }
		bool operator<(array_iterator const& right) const { return m_i < right.m_i; }
		bool operator>(array_iterator const& right) const { return m_i > right.m_i; }
		bool operator<=(array_iterator const& right) const { return m_i <= right.m_i; }
		bool operator>=(array_iterator const& right) const { return m_i >= right.m_i; }

	private:
		Array& m_a;
		std::size_t	m_i;
		const std::size_t m_maxSize;
	};

	template<typename Array>
	class array_const_iterator
	{
	public:

		// normal constructor
		array_const_iterator(Array const& a, std::size_t const& i)
			: m_a(a)
			, m_i(i)
			, m_maxSize(a.size())
		{}
		// copy constructor
		array_const_iterator(array_iterator<Array>::iterator const& itr)
			: m_a(itr)
			, m_i(itr.m_i)
			, m_maxSize(itr.m_maxSize)
		{}

		// * and [] returning reference of the actual element
		Array::const_reference operator*() const
		{
			if ((m_i < 0) || (m_i >= m_maxSize))
			{
				throw std::out_of_range("cannot dereference out of range array iterator");
			}
			return m_a[m_i];
		}
		Array::const_reference operator[] (std::size_t const &i) const
		{
			auto index = static_cast<signed long long>(m_i + i);
			if ((index < 0) || (index >= static_cast<signed long long>(m_maxSize)))
			{
				throw std::out_of_range("cannot dereference out of range array iterator");
			}
			return m_a[m_i + i];
		}

		// ++ and -- returing reference
		array_const_iterator& operator++()
		{
			if (m_i == m_maxSize)
			{
				throw std::out_of_range("cannot increment array iterator past end");
			}
			m_i++;
			return *this;
		}
		array_const_iterator& operator--()
		{
			if (m_i == 0)
			{
				throw std::out_of_range("cannot decrement array iterator before begin");
			}
			m_i--;
			return *this;
		}
		// ++(int) and --(int) returing copy
		array_const_iterator operator++(int)
		{
			array_const_iterator copy = *this;
			++*this;
			return copy;
		}
		array_const_iterator operator--(int)
		{
			array_const_iterator copy = *this;
			--*this;
			return copy;
		}
		// += and -= returning reference
		array_const_iterator &operator+=(std::size_t const n)
		{
			if (m_i + n >= m_maxSize)
			{
				throw std::out_of_range("cannot increment array iterator past end");
			}
			m_i += n;
			return *this;
		}
		array_const_iterator &operator-=(std::size_t const n)
		{
			if (static_cast<signed long long>(m_i - n) < 0)
			{
				throw std::out_of_range("cannot decrement array iterator before begin");
			}
			m_i -= n;
			return *this;
		}
		// + and - returning copy
		array_const_iterator operator+(std::size_t const n) const
		{
			array_const_iterator copy = *this;
			copy += n;
			return copy;
		}
		array_const_iterator operator-(std::size_t const n) const
		{
			array_const_iterator copy = *this;
			copy -= n;
			return copy;
		}
		// ==, !=, >=, <=, >, < returning bool
		bool operator==(array_const_iterator const& right) const { return m_i == right.m_i; }
		bool operator!=(array_const_iterator const& right) const { return m_i != right.m_i; }
		bool operator>(array_const_iterator const& right) const { return m_i > right.m_i;  }
		bool operator>=(array_const_iterator const& right) const { return m_i >= right.m_i; }
		bool operator<(array_const_iterator const& right) const { return m_i < right.m_i; }
		bool operator<=(array_const_iterator const& right)const { return m_i <= right.m_i; }

	private:
		Array const& m_a;
		std::size_t	m_i;
		std::size_t m_maxSize;
	};

	template<typename T, std::size_t N>
	class MyArray
	{
	public:
		using value_type = T;
		using reference = value_type&;
		using size_type = std::size_t;
		using const_reference = T const &;
		using iterator = array_iterator<MyArray>;
		using const_iterator = array_const_iterator<MyArray>;

		// [] returing reference
		reference operator[](size_type const& i)
		{
			if ((static_cast<signed long long>(i) < 0) ||
				(static_cast<signed long long>(i) >= N))
			{
				throw std::out_of_range("cannot dereference out of range array iterator");
			}
			return m_buf[i];
		}
		// [] (const version)
		const_reference operator[](size_type const& i) const
		{
			if ((static_cast<signed long long>(i) < 0) ||
				(static_cast<signed long long>(i) >= N))
			{
				throw std::out_of_range("cannot dereference out of range array iterator");
			}
			return m_buf[i];
		}

		size_type size() const { return N; }

		reference front() { return m_buf[0]; }
		reference back() { return m_buf[N - 1]; }
		iterator begin() { return iterator(*this, 0); }
		iterator end() { return iterator(*this, N); }

		// const version
		const_reference front() const  { return m_buf[0]; }
		const_reference back() const { return m_buf[N - 1]; }
		const_iterator cbegin() const { return const_iterator(*this, 0); }
		const_iterator cend() const { return const_iterator(*this, N); }

		void fill(T const& u)
		{
			for (auto i = 0; i < N; i++)
			{
				m_buf[i] = u;
			}
		}
	private:
		value_type m_buf[N];
	};

	template<typename Container>
	void print(const Container &a)
	{
		for (auto i = 0; i < a.size(); i++)
		{
			std::cout << a[i];
		}
		std::cout << std::endl;
	}
}

namespace TemplateExamples::Concept1
{
	using std::cout;
	using std::endl;

	template<typename T>
	concept Numeric = std::is_arithmetic_v<T>;
	template<typename T>
	concept Float = std::is_floating_point_v<T>;
	template<typename T>
	concept StringType = std::is_convertible_v<T, string>;
	template<typename T>
	concept ValidArg = Numeric<T> || Float<T> || StringType<T>;
	template<typename...Args>
	concept AllValidArgs = (ValidArg<Args> && ...); // using fold expression (since C++17)

	class X
	{
	public:
		// Constructor taking multiple arguments with concepts
		template<typename...Args>
		X(Args...args)
		{
			(processValues(std::forward<Args>(args)), ...); // using fold expression (since C++17)
		}
		// Function to display values stored in the vectors
		void displayValues() const {
			cout << "Integer values: ";
			for (const auto& value : m_intValues)
			{
				cout << value << " ";
			}
			cout << endl;

			cout << "Float values: ";
			for (const auto& value : m_floatValues)
			{
				cout << value << " ";
			}
			cout << endl;

			cout << "String values: ";
			for (const auto& value : m_stringValues)
			{
				cout << value << " ";
			}
			cout << endl;
		}

	private:
		// Function to process each argument based on its type
#if 0 // using constexpr
		template<typename T>
		void processValues(T&& value)
		{
			if constexpr (Float<T>)
			{
				m_floatValues.emplace_back(value);
			}
			else if constexpr (StringType<T>)
			{
				m_stringValues.emplace_back(value);
			}
			else if constexpr (Numeric<T>)
			{
				m_intValues.emplace_back(static_cast<int>(value));
			}
		}
#else // using polymorphism
		void processValues(int&& value)
		{
			m_intValues.emplace_back(value);
		}
		void processValues(float&& value)
		{
			m_floatValues.emplace_back(value);
		}
		void processValues(string&& value)
		{
			m_stringValues.emplace_back(value);
		}
#endif

		vector<int> m_intValues;
		vector<float> m_floatValues;
		vector<string> m_stringValues;
	};
}


#endif // TEMPLATES_H_INCLUDED
