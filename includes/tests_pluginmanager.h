#pragma once
#ifndef TESTS_PLUGINMANAGER_H_INCLUDED
#define TESTS_PLUGINMANAGER_H_INCLUDED
#include "tests.h"

namespace Plugin
{
	class Tests : public ITest
	{
	public:
		int run();
	};
}


#endif // TESTS_PLUGINMANAGER_H_INCLUDED
