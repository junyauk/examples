#include <Windows.h>
#include <iostream>

#include "general.h"
#include "processa.h"
#include "plugin.h"

using namespace Plugin;

// This program is used by TEST(MultiProcess, MemoryMappedFile).
int wmain(int argc, wchar_t* argv[])
{
	HANDLE hMappedFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, TEXT("LOCAL//MemoryMappedFile"));
	if (!hMappedFile)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("OpenFileMapping() failed. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		return 1;
	}

	// Theck the bitmap file size first
	LPBITMAPFILEHEADER pBitmapFileHeader = (LPBITMAPFILEHEADER)MapViewOfFile(hMappedFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(BITMAPFILEHEADER));
	if (!pBitmapFileHeader)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("MapViewOfFile() failed. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		return 1;
	}
	DWORD dwSize = pBitmapFileHeader->bfSize;
	UnmapViewOfFile(pBitmapFileHeader);

	// Mapping whole file size of the bitmap
	LPVOID pData = MapViewOfFile(hMappedFile, FILE_MAP_ALL_ACCESS, 0, 0, dwSize);
	if (!pData)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("MapViewOfFile() failed. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		CloseHandle(hMappedFile);
		return 1;
	}

	BitmapFile bitmapFile(pData);
	string expected = "aa629fbf81affc709e46dd2c69c2b3464e3f6e7adf6f7eb179094319d7e6550a";
	string actual = bitmapFile.hash();
	if (expected != actual)
	{
		wcerr << "BitmapFile class was not initialized correctly.\n";
		MessageBox(NULL, L"BitmapFile class was not initialized with pData correctly.", L"ProcessA", MB_OK);
	}

	auto pixels = 30;
	auto colour = bitmapFile.bitsCount() / 8; // will be 1 or 3;
	auto x = 5 * colour;
	auto y = bitmapFile.height();

	vector<UCHAR> scanline = bitmapFile.getScanline(bitmapFile.height());

	for (auto pixel = 0; pixel < pixels; ++pixel)
	{
		for (auto c = 0; c < colour; ++c)
		{
			scanline.at(x + c) = 0x00;
		}
		x += colour; // x pos
	}
	// Overwrite the scanline
	bitmapFile.setScanline(y, scanline);

	string expected2 = "b8e325daea4a99f870f7c24f74eb0495a65e7f62a78649ef8948ecf7d44595ad";
	string actual2 = bitmapFile.hash();
	if (expected2 != actual2)
	{
		wcerr << "setScanline didn't work.\n";
		MessageBox(NULL, L"setScanline didn't work.", L"ProcessA", MB_OK);
	}

	UnmapViewOfFile(pData);
	CloseHandle(hMappedFile);

	return 0;
}
