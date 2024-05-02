#include "pch.h"
#include "plugin.h"

namespace Plugin
{
	PluginManager::PluginManager(string& prefix)
		: m_prefix(prefix)
	{
		loadPlugins();
	}
	PluginManager::~PluginManager()
	{
		releasePlugins();
	}
	void PluginManager::execute()
	{
		for (auto& h : m_plugins)
		{
			h->run();
		}
	}

	void PluginManager::loadPlugins()
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
					auto createPluginFunc = reinterpret_cast<IPlugin * (*)()>(GetProcAddress(hDll, "createPlugin"));
					if (createPluginFunc)
					{
						IPlugin* rawPtr = createPluginFunc(); // It returns a raw pointer created by new
						shared_ptr<IPlugin> plugin{ rawPtr };
						m_plugins.push_back(std::move(plugin));
					}
					else
					{
						std::wcerr << TEXT("Invalid plugin dll. ") << filePath << endl;
					}
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

	void PluginManager::releasePlugins()
	{
		// Plugins need to be freed intentionally before freeing libraries.
		while (!m_plugins.empty())
		{
			m_plugins.pop_back();
		}

		// Finally free libraries.
		for (auto& h : m_dlls)
		{
			FreeLibrary(h);
		}
	}
}
