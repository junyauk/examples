#include "pch.h"
#include <Windows.h>

#include "general.h"
#include "plugin.h"
#include "tests_pluginmanager.h"


namespace Plugin
{
	int Tests::run()
	{
		string prefix = "plugin";
		PluginManager pluginManager(prefix);
		pluginManager.execute();
		return 0;
	}
}