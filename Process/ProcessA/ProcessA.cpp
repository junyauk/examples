#include <Windows.h>
#include <iostream>

#include "general.h"
#include "processa.h"
#include "plugin.h"

using namespace Plugin;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cerr << "usage: program.exe <plugin_prefix>\n";
		return 1;
	}

	string prefix{ argv[1] };
	PluginManager pluginManager(prefix);

	pluginManager.execute();

	return 0;
}
