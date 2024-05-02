#include <Windows.h>
#include <iostream>

#include "general.h"
#include "processa.h"
#include "plugin.h"

using namespace Plugin;

int main(int argc, char* argv[])
{
	HANDLE hMappedFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"LOCAL//MemoryMappedFile");
	if (!hMappedFile)
	{
		auto lastError = GetLastError();
		cerr << "OpenFileMapping() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		return 1;
	}

	LPVOID pData = MapViewOfFile(hMappedFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
	if (!pData)
	{
		auto lastError = GetLastError();
		cerr << "MapViewOfFile() failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		return 1;
	}

	int* ptr = static_cast<int*>(pData);
	*ptr = 123;

	UnmapViewOfFile(pData);
	CloseHandle(hMappedFile);

	return 0;
}
