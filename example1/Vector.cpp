#include "pch.h"
#include "Vector.h"

bool Vector::isEmpty() const
{
	return m_size == 0;
}

int Vector::size() const
{
	return m_size;
}

int Vector::capacity() const
{
	return m_capacity;
}

int Vector::front() const
{
	if (!m_size)
		return -1;

	return m_data[0];
}

int Vector::back() const
{
	if (!m_size)
		return -1;

	return m_data[m_size - 1];
}

int Vector::at(int ix) const
{
	if ((ix < 0) || (m_size <= ix))
		return -1;

	return m_data[ix];
}

int &Vector::operator[](int ix) const
{
//	if ((ix < 0) || (m_size <= ix))
//		return -1;

	return m_data[ix];
}

void Vector::clear()
{
	m_size = 0;
}

void Vector::reserve(int sz)
{
	if (m_capacity >= sz)
		return;
	m_capacity = INI_SIZE;
	while (m_capacity < sz)
		m_capacity *= 2;
	int* tmp = new int[m_capacity];
	for (int i = 0; i < m_size; i++)
	{
		tmp[i] = m_data[i];
	}
	delete[] m_data;
	m_data = tmp;
}

void Vector::resize(int sz)
{
	if (sz < 0)
		sz = 0;
	reserve(sz);
	m_size = sz;
}

void Vector::push_back(int n)
{
	if (++m_size > m_capacity)
		reserve(m_size);
	m_data[m_size - 1] = n;
}

int Vector::pop_back()
{
	if (!m_size)
		return -1;
	return m_data[--m_size];
}

void Vector::swap(Vector& v)
{
	std::swap(m_data, v.m_data);
	std::swap(m_capacity, v.m_capacity);
	std::swap(m_size, v.m_size);
}

void Vector::insert(int ix, int d)
{
	if (ix < 0)
		ix = 0;
	if (ix > m_size)
		ix = m_size;
	resize(m_size + 1);
	for (int i = m_size; --i >= ix;)
	{
		m_data[i + 1] = m_data[i];
	}
	m_data[ix] = d;
}

void Vector::erase(int ix)
{
	if ((ix < 0) || (ix >= m_size))
		return;
	m_size--;
	for (int i = ix; i < m_size; i++)
	{
		m_data[i] = m_data[i + 1];
	}
}

void Vector::assign(int sz, int d)
{
	resize(sz);
	for (int i = 0; i < m_size; i++)
	{
		m_data[i] = d;
	}
}

