#pragma once
#ifndef RINGBUFFER_H_INCLUDED
#define RINGBUFFER_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

class CircularIndex
{
public:
	CircularIndex(unsigned int max)
		: m_idx(0)
		, m_max(max)
	{}
	CircularIndex(CircularIndex const& other) = delete;
	CircularIndex& operator=(CircularIndex const& other) = delete;

	// ++ and --
	size_t& operator++()
	{
		m_idx = add(1);
		return m_idx;
	}
	size_t& operator--()
	{
		m_idx = sub(1);
		return m_idx;
	}
	// ++(int) and --(int)
	size_t operator++(int)
	{
		size_t copy = m_idx;
		m_idx = add(1);
		return copy;
	}
	size_t operator--(int)
	{
		size_t copy = m_idx;
		m_idx = sub(1);
		return copy;
	}

	// += and -=
	size_t& operator+=(int const n)
	{
		m_idx = add(n);
		return m_idx;
	}
	size_t& operator-=(int const n)
	{
		m_idx = sub(n);
		return m_idx;
	}

	// + and -
	size_t operator+(int const n) const
	{
		return add(n);
	}
	size_t operator-(int const n) const
	{
		return sub(n);
	}

	CircularIndex& operator=(size_t const& n) { this->m_idx = 0;  this->m_idx = add(n); return *this; }

	template<typename T>
	bool operator==(T const& right) const { return m_idx == static_cast<size_t>(right); }
	bool operator==(CircularIndex const& right) const { return m_idx == right.m_idx; }



	template<typename T>
	bool operator!=(T const& right) const { return m_idx != static_cast<size_t>(right); }
	bool operator!=(CircularIndex const& right) const { return m_idx != right.m_idx; }

	friend std::ostream& operator<< (std::ostream& os, CircularIndex const& idx)
	{
		os << idx.m_idx;
		return os;
	}

	template<typename T>
	operator T() { return static_cast<T>(this->m_idx); }
	operator string() { return to_string(this->m_idx); }

private:
	size_t add(size_t const& n) const
	{
		size_t ret = m_idx;
		size_t tmp{ n % m_max };
		if (!tmp)
		{
			return ret;
		}
		if (ret + tmp >= m_max)
		{
			ret = (ret + tmp) - m_max;
		}
		else
		{
			ret += tmp;
		}
		return ret;
	}
	size_t sub(size_t const& n) const
	{
		size_t ret = m_idx;
		size_t tmp{ n % m_max };
		if (tmp == 0)
		{
			return ret;
		}
		if (ret < tmp)
		{
			ret += m_max - tmp;
		}
		else
		{
			ret -= tmp;
		}
		return ret;
	}

	size_t m_idx;
	size_t m_max;
};

template<typename T, size_t capacity>
class RingBuffer
{
public:
	RingBuffer()
		: m_head(0)
		, m_tail(0)
		, m_size(0)
		, m_buf(capacity)
	{}

	size_t getSize() { return m_size; }
	bool isEmpty() { return m_size == 0; }
	bool isFull() { return m_size == capacity; }

	void push(T const& item)
	{
		if (m_size == capacity)
		{
			throw std::overflow_error("Buffer is full");
		}
		m_buf[m_tail] = item;
		m_tail = (m_tail + 1) % capacity;
		++m_size;
	}

	T pop()
	{
		if (m_size == 0)
		{
			throw std::underflow_error("Buffer is empty");
		}
		T item = m_buf[m_head];
		m_head = (m_head + 1) % capacity;
		--m_size;
		return item;
	}

private:
	vector<T>	m_buf;
	size_t		m_head;
	size_t		m_tail;
	size_t		m_size;
};

int Run_RingBuffer();

#endif // RINGBUFFER_H_INCLUDED