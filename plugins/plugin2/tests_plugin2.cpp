#include "pch.h"
#include <Windows.h>

#include "plugin.h"
#include "tests_plugins.h"

namespace Plugin::LoadingPlugin2Tests
{
	int Tests::run()
	{
		auto hPlugin = LoadLibrary(TEXT("plugin2.dll"));
		if (hPlugin != NULL)
		{
			auto func = reinterpret_cast<IPlugin * (*)()>(GetProcAddress(hPlugin, "createPlugin"));
			if (func != NULL)
			{
				IPlugin* rawPtr = func(); // It returns a raw pointer created by new
				shared_ptr<IPlugin> sharedPtr(rawPtr);
				sharedPtr->run();
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


