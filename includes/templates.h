#pragma once
#ifndef TEMPLATES_H_INCLUDED
#define TEMPLATES_H_INCLUDED

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
		array_iterator(Array&a, std::size_t i)
			: m_a(a)
			, m_i(i)
			, m_maxSize(m_a.size())
		{
		}

		array_iterator& operator++()
		{
			if (m_i + 1 >= m_maxSize)
			{
				throw std::out_of_range("cannot increment array iterator past end");
			}
			++m_i;
			return *this;
		}
		array_iterator operator++(int)
		{
			array_iterator copy = *this;
			++*this;
			return copy;
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
		array_iterator operator--(int)
		{
			array_iterator copy = *this;
			--*this;
			return copy;
		}

		Array::reference operator*()
		{
#if 0
			if (m_i < 0)
			{
				throw std::out_of_range("cannot decrement array iterator before begin");
			}
			else if ((m_i < 0) || (m_i >= m_maxSize))
			{
				throw std::out_of_range("cannot increment array iterator past end");
			}
#else
			if ((m_i < 0) || (m_i >= m_maxSize))
			{
				throw std::out_of_range("cannot dereference out of range array iterator");
			}
#endif
			return m_a[m_i];
		}

		bool operator==(const array_iterator& right)
		{
			return m_i == right.m_i;
		}
		bool operator!=(const array_iterator& right)
		{
			return m_i != right.m_i;
		}

		array_iterator &operator+=(std::size_t n)
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
		array_iterator operator+(std::size_t n) const
		{
			array_iterator copy = *this;
			copy += n;
			return copy;
		}
		array_iterator& operator-=(std::size_t n)
		{
			if (static_cast<signed long long>(m_i - static_cast<signed long long>(n)) < 0)
			{
				throw std::out_of_range("cannot decrement array iterator before begin");
			}
			m_i -= n;
			return *this;
		}
		array_iterator operator-(std::size_t n) const
		{
			array_iterator copy = *this;
			copy -= n;
			return copy;
		}

		typename Array::reference operator[](std::size_t i) const
		{
			auto index = static_cast<signed long long>(m_i + i);
			if ((index < 0) || (index >= static_cast<signed long long>(m_maxSize)))
			{
				throw std::out_of_range("cannot dereference out of range array iterator");
			}
			return *(*this + i); // m_a[m_i + i];
		}

		bool operator<(array_iterator const & right) const { return m_i < right.m_i; }
		bool operator>(array_iterator const & right) const { return m_i > right.m_i; }
		bool operator<=(array_iterator const& right) const { return m_i <= right.m_i; }
		bool operator>=(array_iterator const & right) const { return m_i >= right.m_i; }


	private:
		Array& m_a;
		std::size_t	m_i;
		const std::size_t m_maxSize;
	};

	template<typename Array>
	class array_const_iterator
	{
	public:
		array_const_iterator(Array const &a, std::size_t i)
			: m_a(a)
			, m_i(i)
		{}
		array_const_iterator(array_iterator<Array>::iterator const& itr)
			: m_a(itr.m_a)
			, m_i(itr.m_i)
		{}

		array_const_iterator& operator++()
		{
			++m_i;
			return *this;
		}
		array_const_iterator& operator--()
		{
			--m_i;
			return *this;
		}
		array_const_iterator operator++(int)
		{
			array_const_iterator copy = *this;
			++m_i;
			return copy;
		}
		array_const_iterator operator--(int)
		{
			array_const_iterator copy = *this;
			--m_i;
			return copy;
		}
		Array::const_reference operator*() const
		{
			return m_a[m_i];
		}
		Array::const_reference operator[](std::size_t i) const
		{
			return *(*this + i);
		}


		bool operator==(array_const_iterator const& right)
		{
			return m_i == right.m_i;
		}
		bool operator!=(array_const_iterator const& right)
		{
			return m_i != right.m_i;
		}

		array_const_iterator& operator+=(std::size_t n)
		{
			m_i += n;
			return *this;
		}
		array_const_iterator& operator-=(std::size_t n)
		{
			m_i -= n;
			return *this;
		}

		array_const_iterator operator+(std::size_t n) const
		{
			array_const_iterator copy = *this;
			copy += n;
			return copy;
		}
		array_const_iterator operator-(std::size_t n) const
		{
			array_const_iterator copy = *this;
			copy -= n;
			return copy;
		}

	private:
		Array const& m_a;
		std::size_t m_i;
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

		reference operator[](std::size_t i) { return m_storage[i]; }
		const_reference operator[](std::size_t i) const { return m_storage[i]; }

		size_type size() const { return N; }

		reference front() { return m_storage[0]; }
		const_reference front() const  { return m_storage[0]; }
		reference back() { return m_storage[N - 1]; }
		const_reference back() const { return m_storage[N - 1]; }

		iterator begin()
		{
			return iterator( *this, 0 );
		}
		iterator end()
		{
			return iterator( *this, N );
		}
		const_iterator cbegin()
		{
			return const_iterator(*this, 0);
		}
		const_iterator cend()
		{
			return const_iterator(*this, N);
		}
		void fill(T const& u)
		{
			for (auto i = 0; i < N; i++)
			{
				m_storage[i] = u;
			}
		}
	private:
		value_type m_storage[N];
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


#endif // TEMPLATES_H_INCLUDED
