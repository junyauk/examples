#include <Windows.h>
#include <iostream>

#include "general.h"
#include "processa.h"
#include "plugin.h"
#include "pipe.h"

using namespace Plugin;

int Run_MemoryMappedFileCode()
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

int Run_AnonymousPipe()
{
	cout << "Sending texts from Process A using cout.\n";
	cerr << "Sending texts from Process A using cerr.\n";
	return 0;
}

int Run_NamedPipe()
{
	HANDLE hPipeEnabled = NULL;

	while (!hPipeEnabled)
	{
		hPipeEnabled = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("PipeReady"));
		if (!hPipeEnabled)
		{
			Sleep(1000);
		}
	}
	WaitForSingleObject(hPipeEnabled, INFINITE);

	HANDLE hPipe;
	hPipe = CreateFile(
		TEXT("\\\\.\\pipe\\NamedPipe"),
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("CreateFile failed. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		return 1;
	}

	HANDLE hReadToRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("ReadyToRead"));


	DWORD dwTimes = 0;
	DWORD dwWritten;
	PipeData sendData = { dwTimes, "Message from Process A." };
	while (dwTimes < 10)
	{
		sendData.dwVal = dwTimes;
		if (!WriteFile(
			hPipe,
			&sendData,
			sizeof(sendData),
			&dwWritten,
			NULL))
		{
			auto lastError = GetLastError();
			wcerr << TEXT("WriteFile failed. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		}
		SetEvent(hReadToRead);
		++dwTimes;
		Sleep(1000);
	}

	PipeData lastData = { dwTimes, "Exit" };
	if (!WriteFile(
		hPipe,
		&lastData,
		sizeof(lastData),
		&dwWritten,
		NULL))
	{
		auto lastError = GetLastError();
		wcerr << TEXT("WriteFile failed. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
	}
	SetEvent(hReadToRead);

	CloseHandle(hPipe);

	return 0;
}


// This program is used by TEST(MultiProcess, MemoryMappedFile).
int wmain(int argc, wchar_t* argv[])
{
	std::wstring argStr;
	if (argc > 1) {
		argStr = argv[1];
	}
	else
	{
		wcerr << TEXT("Missing parameter.\n");
		return 1;
	}

	if (argStr == TEXT("MemoryMappedFile"))
	{
		return Run_MemoryMappedFileCode();
	}
	else if (argStr == TEXT("AnonymousPipe"))
	{
		return Run_AnonymousPipe();
	}
	else if (argStr == TEXT("NamedPipe"))
	{
		return Run_NamedPipe();
	}
	return 0;
}
