#pragma once
#ifndef PLUGIN_H_INCLUDED
#define PLUGIN_H_INCLUDED

#include <Windows.h>
#include "general.h"

namespace Plugin
{
	class PluginManager
	{
	public:
		PluginManager(string& prefix);
		~PluginManager();
		void execute();
	private:
		string m_prefix;
		vector<HMODULE> m_dlls;
	};
}

#endif // PLUGIN_H_INCLUDED

