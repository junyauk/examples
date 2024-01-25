#pragma once
#include <xmemory>
class Vector
{
public:
	enum
	{
		INI_SIZE = 8,
	};
	Vector() : 
		m_capacity(INI_SIZE), 
		m_data(new int[INI_SIZE]), // It's an array, so use "int[size]"
		m_size(0)
	{
	};
	Vector(int sz, int d = 0): m_capacity(sz), m_size(sz), m_data(new int[sz])
	{
		for (int i = 0; i < sz; i++)
		{
			m_data[i] = d;
		}
	};
	Vector(const int* first, const int* last)
	{
		m_capacity = m_size = static_cast<int>(last - first);
		m_data = new int[m_size];
		for (int i = 0; i < m_size; i++)
		{
			m_data[i] = *(first + i);
		}
	};
	Vector(const Vector& v):
		m_capacity(v.m_capacity),
		m_size(v.m_size),
		m_data(new int[v.m_size])
	{
		memcpy(m_data, v.m_data, sizeof(int) * m_size);
	}
	~Vector()
	{
		if (m_data)
		{
			delete [] m_data; // It's an array, so use "delete []" instead of "delete"
		}
	};
	bool isEmpty() const;	// Do not change member variables
	int size() const;		// Do not change member variables
	int capacity() const;

	int front() const;
	int back() const;
	int at(int ix) const;

//	int operator[](int ix) const;
	int &operator[](int ix) const;

	void clear();
	void reserve(int sz);
	void resize(int sz);

	void push_back(int n);
	int pop_back();

	void swap(Vector& v);
	void insert(int ix, int d);
	void erase(int ix);
	void assign(int sz, int d);

	int *m_data;
	int m_capacity;
	int m_size;
};
