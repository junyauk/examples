#pragma once
#ifndef TESTS_JOBOBJECT_H_INCLUDED
#define TESTS_JOBOBJECT_H_INCLUDED
#include "tests.h"
#include "jobobject.h"

namespace JobObject::Windows1
{
	class Tests : public ITest
	{
	public:
		int run();
	};
}


#endif // TESTS_JOBOBJECT_H_INCLUDED