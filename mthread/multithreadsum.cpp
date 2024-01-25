// mthread.cpp : スタティック ライブラリ用の関数を定義します。
//

#include "pch.h"
#include "framework.h"
#include <Windows.h>
#include <process.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <numeric>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::size_t;
using std::vector;
using std::string;
using std::to_string;

size_t const NUM_THREADS = 4;

// struct used by the thread
template <class T>
struct Bthread_sum
{
	size_t tid;
	size_t n;			// The number of objects
	vector<T> const *x; // A vector of the target object such as float or double
	T sum;
};

// Thread function
template <class T>
static // Thread function needs to be static
unsigned int WINAPI vec_sum_thread(void* arg) // expect WINAPI is __stdcall
{
	DWORD dwThreadId = GetThreadId(GetCurrentThread());
	string str;
	str = "Entering thread(" + to_string(dwThreadId) + ")";
	cout << str << endl;

	Bthread_sum<T>* param = static_cast<Bthread_sum<T>*>(arg);
	size_t const len = (param->n + NUM_THREADS - 1) / NUM_THREADS;
	size_t const beg = param->tid + len;
	size_t const end = (param->tid == NUM_THREADS - 1) ? param->n : beg + len;

	param->sum = 0;
	for (size_t i = beg; i < end; i++)
	{
		param->sum += param->x->at(i);
	}

	str = "Leaving thread(" + to_string(dwThreadId) + ")";
	cout << str << endl;
	_endthreadex(0);
	return 0;
}

// Define vec_sum() function
// The parameters are...
// - n: The number of objects which needs to be calculated
// - x: An array of the object
// This function does...
// - call _beginthreadex() for creating threads
// - call WaitForMultipleObjects() for the created threads
// - calculate sum
// - call CloseHandle() for closing thread handles
template<class T>
T vec_sum(size_t const n, vector<T> const &x)
{
	vector<HANDLE> hThread;
	vector<Bthread_sum<T>> param(NUM_THREADS, Bthread_sum<T>());

	for (size_t i = 0; i < NUM_THREADS; i++)
	{
		param[i].tid = i;
		param[i].n = n;
		param[i].x = &x;
		HANDLE ht = (HANDLE)_beginthreadex(NULL, 0, vec_sum_thread<T>, &param[i], 0, NULL);
		hThread.push_back(ht);
	}

	WaitForMultipleObjects(NUM_THREADS, hThread.data(), TRUE, INFINITE);

	for (size_t i = 1; i < NUM_THREADS; i++)
	{
		param[0].sum += param[i].sum;
	}

	for (size_t i = 0; i < NUM_THREADS; i++)
	{
		CloseHandle(hThread[i]);
	}

	return param[0].sum;
}


void Run_MThread_Sum()
{
	size_t const n = 1023;
	vector<float> x(n, 0);
	vector<double> y(n, 0);

	srand(static_cast<unsigned>(time(NULL)));
	for (auto i = 0; i < n; i++)
	{
		x[i] = static_cast<float>(rand()) / RAND_MAX;
		y[i] = static_cast<double>(rand()) / RAND_MAX;
	}

	cout << "Single Precision" << endl;
	cout << " - Accumulate: " << std::accumulate(x.begin(), x.begin() + n, 0.0) << endl;
	cout << " - Sum : " << vec_sum(n, x) << endl;
	cout << "Double Precision" << endl;

	cout << " - Accumulate: " << std::accumulate(y.begin(), y.begin() + n, 0.0) << endl;
	cout << " - Sum : " << vec_sum(n, y) << endl;
}

