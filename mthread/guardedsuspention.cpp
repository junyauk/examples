#include "pch.h"
#include "guardedsuspention.h"

using namespace GuardesSuspention;

int Run_GuardedSuspention()
{
	Printer	printer(10);
	for (int i = 0; i < 10; i++)
	{
		stringstream ss;
		ss << "Hello World " << i;
		while (!printer.append(ss.str()))
		{
			sleep_for(std::chrono::milliseconds(1));
		}
	}
	return 0;
}

