#pragma once
#ifndef GENERAL_H_INCLUDED
#define GENERAL_H_INCLUDED
#include "Windows.h"

#include <iostream>
#include <string>
#include <mutex>

using std::cout;
using std::endl;
using std::string;
using std::wstring;
using std::mutex;
using std::unique_lock;

void general();

class ThreadSafeOut
{
public:
	static void print(const string& str)
	{
		static mutex	m;
		{
			unique_lock<mutex>	ul(m);
			cout << str;
		}
	}
};

string GetLastErrorMessage(DWORD lastError);

wstring FindFolderInPath(const wstring folderName);


#endif // GENERAL_H_INCLUDED