#pragma once
#ifndef PLUGIN_H_INCLUDED
#define PLUGIN_H_INCLUDED

#include <Windows.h>
#include "general.h"

namespace Plugin
{
	class IPlugin
	{
	public:
		virtual ~IPlugin() {}
		virtual int run() = 0;
	};

	class PluginManager
	{
	public:
		PluginManager(string& prefix);
		~PluginManager();
		void execute();
	private:
		void loadPlugins();
		void releasePlugins();
		string m_prefix;
		vector<HMODULE> m_dlls;
		vector<shared_ptr<IPlugin>> m_plugins;
	};

#ifdef __cplusplus
	extern "C" {
#endif
		__declspec(dllexport) IPlugin* createPlugin();
#ifdef __cplusplus
	}
#endif
}


#endif // PLUGIN_H_INCLUDED

