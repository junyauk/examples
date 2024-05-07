#include "pch.h"
#include "framework.h"
#include <Windows.h>
#include <rpc.h>
#include <openssl/sha.h>

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
wstring GetLastErrorMessageW(DWORD lastError)
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

	wstring errorMessage{ (wchar_t*)lpMsgBuf };
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

wstring GetRunningPath()
{
	WCHAR path[MAX_PATH];
	if (!GetModuleFileName(nullptr, path, MAX_PATH))
	{
		DWORD lastError = GetLastError();
		cerr << "Error GetModuleFileName failed: (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		return TEXT("");
	}
	wstring fullpath{ path };
	auto pos = fullpath.find_last_of(TEXT('\\'));
	return fullpath.substr(0, pos);
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

wstring GetTestFolderPath()
{
	wstring path = FindFolderInPath(L"examples");
	return path + L"\\tests\\";
}

vector<char> LoadBinaryFile(wstring const& filename)
{
	wstring wsPath = GetTestFolderPath() + TEXT("\\") + filename;
	HANDLE hFile = CreateFile(
		wsPath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("Failed to open. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
	}

	DWORD dwSize = GetFileSize(hFile, NULL);
	if (dwSize == INVALID_FILE_SIZE)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("Failed to get the file size. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		CloseHandle(hFile);
		return vector<char>();
	}

	vector<char> buf(dwSize);

	DWORD dwRead;
	if (!ReadFile(hFile, buf.data(), dwSize, &dwRead, NULL) || (dwSize != dwRead))
	{
		auto lastError = GetLastError();
		wcerr << TEXT("Failed to read the file. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		CloseHandle(hFile);
		return vector<char>();
	}

	CloseHandle(hFile);

	return buf;
}

string hashToHexString(vector<UCHAR> const& hash)
{
	stringstream ss;
	for (auto& c : hash)
	{
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
	}
	return ss.str();
}


BitmapFile::BitmapFile(wstring const& filename)
{
	// Open the specified bitmap file
	HANDLE hFile = CreateFile(
		filename.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("Failed to open. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		return;
	}

	// Get the file size
	DWORD dwSize = GetFileSize(hFile, NULL);
	if (dwSize == INVALID_FILE_SIZE)
	{
		auto lastError = GetLastError();
		wcerr << TEXT("Failed to get the file size. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		CloseHandle(hFile);
		return;
	}
	m_spFile = make_shared<UCHAR[]>(dwSize);
	m_pFile = m_spFile.get();

	// Read the file
	DWORD dwRead;
	if (!ReadFile(hFile, m_pFile, dwSize, &dwRead, NULL) || (dwSize != dwRead))
	{
		auto lastError = GetLastError();
		wcerr << TEXT("Failed to read the file. (") << lastError << TEXT(") ") << GetLastErrorMessageW(lastError) << endl;
		CloseHandle(hFile);
		return;
	}
	CloseHandle(hFile);

	initialize();
}

BitmapFile::BitmapFile(LPVOID const pFile)
{
	m_spFile = nullptr;
	m_pFile = static_cast<UCHAR*>(pFile);
	initialize();
}
void BitmapFile::initialize()
{
	LPBITMAPFILEHEADER pFileHeader = bitmapFileHeader();
	LPBITMAPINFOHEADER pInfoHeader = bitmapInfoHeader();
	m_width = pInfoHeader->biWidth;
	m_height = pInfoHeader->biHeight;
	m_bitCount = pInfoHeader->biBitCount;
	m_fileSize = pFileHeader->bfSize;
	m_scanlineSize = (((pInfoHeader->biWidth * (pInfoHeader->biBitCount / 8)) + 3) / 4) * 4;
	m_imageSize = pInfoHeader->biSizeImage;
	m_image = m_pFile + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}


vector<UCHAR> BitmapFile::getScanline(const LONG y)
{
	if (m_pFile == nullptr)
	{
		return vector<UCHAR>();
	}


	if (std::abs(m_height) < std::abs(y))
	{
		return vector<UCHAR>();
	}

	vector<UCHAR> scanline(m_scanlineSize);
	UCHAR* pScanline = m_image;
	if (m_height < 0)
	{
		pScanline = m_image + (y * m_scanlineSize);
	}
	else
	{
		pScanline = m_image + ((m_height - y) * m_scanlineSize);
	}
	CopyMemory(scanline.data(), pScanline, m_scanlineSize);
	return scanline;
}

void BitmapFile::setScanline(DWORD const y, vector<UCHAR> const& scanline)
{
	UCHAR* pScanline;
	if (m_height < 0)
	{
		pScanline = m_image + (y * m_scanlineSize);
	}
	else
	{
		pScanline = m_image + ((m_height - y) * m_scanlineSize);
	}

	DWORD wSize = (m_scanlineSize < (DWORD)(scanline.size())) ? m_scanlineSize : (DWORD)(scanline.size());
	CopyMemory(pScanline, scanline.data(), wSize);
}

string BitmapFile::hash()
{
	vector<UCHAR> hs(SHA256_DIGEST_LENGTH);
	SHA256(m_pFile, m_fileSize, hs.data());

	return hashToHexString(hs);
}
