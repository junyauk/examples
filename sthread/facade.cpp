#include "pch.h"

#include "facade.h"

namespace Facade::Basic1
{
	int Tests::run()
	{
		Facade facade;

		facade.operation1();

		facade.operation2();

		return 0;
	}
}

namespace Facade::Basic2
{
	int Tests::run()
	{
		MultimediaFacade facade;

		// Specifying a simple parameter set for using subsystems or libraries,
		// instead of calling various APIs of subsystems or libraries.
		facade.playMedia("aaa.mp3", "audio");
		facade.playMedia("bbb.mp4", "video");
		facade.playMedia("ccc.jpb", "image");
		return 0;
	}
}
