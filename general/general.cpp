#include "pch.h"
#include "framework.h"
#include <Windows.h>
#include <rpc.h>

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <map>

#include "general.h"

using namespace std;

void general()
{
	// This is used for running simple programs for debugging.
}

string GetLastErrorMessage(DWORD lastError)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		lastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Convert lpMsgBuf to string
	int size = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)lpMsgBuf, -1, NULL, 0, NULL, NULL);
	string errorMessage(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)lpMsgBuf, -1, &errorMessage[0], size, NULL, NULL);
	LocalFree(lpMsgBuf);

	return errorMessage;
}

wstring FindFolderInPath(const wstring folderName)
{
	WCHAR path[MAX_PATH];
	if (!GetModuleFileName(nullptr, path, MAX_PATH))
	{
		DWORD lastError = GetLastError();
		cerr << "Error getting file name: (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		return L"";
	}

	std::wstring fullPath{ path };
	auto pos = fullPath.rfind(folderName);
	if (pos == std::wstring::npos)
	{
		cerr << "Invalid directory path format\n";
		return L"";
	}

	if ((pos > 0) && (fullPath[pos - 1] == '\\'))
	{
		return fullPath.substr(0, pos + folderName.size());
	}

	cout << "The specified folder was not found.\n";
	return L"";
}


Random32::Random32()
{
	if (!CryptAcquireContext(&m_hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		auto lastError = GetLastError();
		cerr << "CryptAcquireContext() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
	}

}
Random32::~Random32()
{
	if (m_hProv)
	{
		if (!CryptReleaseContext(m_hProv, 0))
		{
			auto lastError = GetLastError();
			cerr << "CryptReleaseContext() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		}
	}
}
DWORD Random32::gen()
{
	DWORD dwRet = 0;
	if (m_hProv)
	{
		if (!CryptGenRandom(m_hProv, sizeof(DWORD), m_data))
		{
			auto lastError = GetLastError();
			cerr << "CryptGenRandom() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		}
		else
		{
			CopyMemory((PVOID)&dwRet, (PVOID)m_data, sizeof(DWORD));
		}
	}
	return dwRet;
}

