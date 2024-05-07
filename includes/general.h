#pragma once
#ifndef GENERAL_H_INCLUDED
#define GENERAL_H_INCLUDED
#include <Windows.h>
#include <wincrypt.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <deque>
#include <functional>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <cassert>
#include <random>
#include <chrono>
#include <atomic>
#include <memory>

using std::size_t;
using std::cout;
using std::wcout;
using std::endl;
using std::cerr;
using std::wcerr;
using std::string;
using std::wstring;
using std::to_string;
using std::to_wstring;
using std::stringstream;
using std::vector;
using std::queue;
using std::deque;
using std::function;
using std::thread;
using std::mutex;
using std::shared_mutex;
using std::unique_lock;
using std::lock_guard;
using cv = std::condition_variable;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::atomic;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;

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
wstring GetLastErrorMessageW(DWORD lastError);

wstring FindFolderInPath(const wstring folderName);
wstring GetRunningPath();
wstring GetTestFolderPath();

vector<char> LoadBinaryFile(wstring const& filename);

class Random32
{
public:
	Random32();
	~Random32();
	DWORD gen();

private:
	BYTE m_data[4] = { 0 };
	HCRYPTPROV m_hProv;
};

class BitmapFile
{
public:
	BitmapFile(wstring const& filename);
	BitmapFile(LPVOID const pFile);

	LPBITMAPFILEHEADER bitmapFileHeader()
	{
		return (LPBITMAPFILEHEADER)(m_pFile);
	}

	LPBITMAPINFOHEADER bitmapInfoHeader()
	{
		return (LPBITMAPINFOHEADER)(m_pFile + sizeof(BITMAPFILEHEADER));
	}
	UCHAR* image() { return m_image; }

	LONG width() { return m_width; }
	LONG height(){ return m_height; }
	LONG bitsCount() { return m_bitCount; }
	DWORD fileSize() { return m_fileSize; }
	DWORD imageSize() { return m_imageSize; }
	DWORD scanlineSize() { return m_scanlineSize; }

	vector<UCHAR> getScanline(const LONG y);
	void setScanline(DWORD const y, vector<UCHAR> const& scanline);

	string hash();
private:
	void initialize();
	LONG m_width{ 0 };
	LONG m_height{ 0 };
	LONG m_bitCount{ 0 };
	DWORD m_scanlineSize{ 0 };
	DWORD m_fileSize{ 0 };
	DWORD m_imageSize{ 0 };

	UCHAR* m_pFile;
	UCHAR* m_image{ NULL };
	shared_ptr<UCHAR[]> m_spFile;
};



#endif // GENERAL_H_INCLUDED