#include "pch.h"
#include "templatemethod.h"

namespace TemplateMethod::Basic1
{
	int Tests::run()
	{
		string result;
		Derived1 derived1;
		Derived2 derived2;
		result = derived1.run() + " " + derived2.run();
		cout << result << endl;

		return 0;
	}
}
