#pragma once
#ifndef TESTS_PLUGINS_HINCLUDED
#define TESTS_PLUGINS_HINCLUDED
#include "tests.h"

namespace Plugin::LoadingPlugin1Tests
{
	class Tests : public ITest
	{
	public:
		int run();
	};
}

namespace Plugin::LoadingPlugin2Tests
{
	class Tests : public ITest
	{
	public:
		int run();
	};
}

#endif // TESTS_PLUGINS_HINCLUDED
