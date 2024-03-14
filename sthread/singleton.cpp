#include "pch.h"

#include <iostream>
#include <string>


#include "singleton.h"

namespace Singleton::Basic1
{
	int Tests::run()
	{
		Singleton& instance = Singleton::getInstance();
		cout << instance.info() << endl;
		return 0;
	}
}


