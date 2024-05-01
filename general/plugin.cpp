#include "pch.h"
#include "plugin.h"

namespace Plugin
{
	PluginManager::PluginManager(string& prefix)
		: m_prefix(prefix)
	{
		WCHAR path[MAX_PATH];
		GetModuleFileNameW(NULL, path, MAX_PATH);
		std::wstring folderPath{ path };
		auto lastSeparator = folderPath.find_last_of(L"\\");
		folderPath = folderPath.substr(0, lastSeparator + 1);

		auto searchName = folderPath + std::wstring(m_prefix.begin(), m_prefix.end()) + TEXT("*.dll");
		WIN32_FIND_DATA findFileData;
		HANDLE hFile = FindFirstFileW(searchName.c_str(), &findFileData);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			do
			{
				wstring filePath = folderPath + findFileData.cFileName;

				HMODULE hDll = LoadLibraryW(filePath.c_str());
				if (hDll)
				{
					m_dlls.push_back(hDll);
				}
				else
				{
					auto lastError = GetLastError();
					cerr << "LoadLibrary failed. (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
				}
			} while (FindNextFileW(hFile, &findFileData));
			FindClose(hFile);
		}
		else
		{
			cerr << "No dlls found with prefix: " << m_prefix << endl;
		}
	}
	PluginManager::~PluginManager()
	{
		for (auto& h : m_dlls)
		{
			FreeLibrary(h);
		}
	}
	void PluginManager::execute()
	{
		for (auto& h : m_dlls)
		{
			auto run = GetProcAddress(h, "run");
			if (run)
			{
				run();
			}
		}
	}
}
