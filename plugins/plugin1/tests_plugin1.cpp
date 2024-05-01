#include "pch.h"
#include <Windows.h>

#include "plugin.h"
#include "plugindll.h"
#include "tests_plugins.h"

namespace Plugin::LoadingPluginTests
{
	int Tests::run()
	{
		auto hPlugin = LoadLibrary(TEXT("plugin1.dll"));
		if (hPlugin != NULL)
		{
			auto func = GetProcAddress(hPlugin, "run");
			if (func != NULL)
			{
				func();
			}
			FreeLibrary(hPlugin);
		}
		else
		{
			auto lastError = GetLastError();
			cerr << "LoadLibrary() failed (" << lastError << ") " << GetLastErrorMessage(lastError) << endl;
		}
		return 0;
	}
}


