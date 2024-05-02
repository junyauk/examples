#pragma once
#ifndef PLUGIN1_H_INCLUDED
#define PLUGIN1_H_INCLUDED
#include <Windows.h>
#include "general.h"
#include "plugin.h"

using namespace Plugin;

class Plugin1 : public IPlugin
{
public:
	Plugin1();
	~Plugin1();
	int run();
private:
	int m_data{ 0 };
	string m_str{ "" };
};



#endif // PLUGIN1_H_INCLUDED