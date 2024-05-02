#pragma once
#ifndef PLUGIN2_H_INCLUDED
#define PLUGIN3_H_INCLUDED
#include <Windows.h>
#include "general.h"
#include "plugin.h"

using namespace Plugin;

class Plugin2 : public IPlugin
{
public:
	Plugin2();
	~Plugin2();
	int run();
private:
	int m_data{ 0 };
	string m_str{ "" };
};



#endif // PLUGIN1_H_INCLUDED