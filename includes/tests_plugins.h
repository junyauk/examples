#pragma once
#ifndef TESTS_PLUGINS_HINCLUDED
#define TESTS_PLUGINS_HINCLUDED
#include "tests.h"

namespace Plugin::LoadingPluginTests
{
	class Tests : public ITest
	{
	public:
		int run();
	};
}

#endif // TESTS_PLUGINS_HINCLUDED
