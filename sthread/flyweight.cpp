#include "pch.h"

#include "flyweight.h"

namespace Flyweight::Basic1
{
    // Concrete Singer classes as Flyweight
    class PopSinger : public Singer
    {
    public:
        void sing() override { cout << "Singing pop song\n"; }
    };

    class RockSinger : public Singer
    {
    public:
        void sing() override { cout << "Singing rock song\n"; }
    };

    class OperaSinger : public Singer
    {
    public:
        void sing() override { cout << "Singing opera song\n"; }
    };

    // Label class as factory, responsible for managing singers
    class Label : public ILabel<Label>
    {
    public:
        Label()
        {
            // calling the protected method of the base class
            registerSinger<PopSinger>("pop");
            registerSinger<RockSinger>("rock");
            registerSinger<OperaSinger>("opera");
        }
    };

    int Tests::run()
	{
		Label emi;

		Singer* popSinger1 = emi.getSinger("pop");
		Singer* popSinger2 = emi.getSinger("pop");
		Singer* rockSinger = emi.getSinger("rock");
		Singer* operaSinger = emi.getSinger("opera");

		if (popSinger1) popSinger1->sing();
		if (popSinger2) popSinger2->sing();
		if (rockSinger) rockSinger->sing();
		if (operaSinger) operaSinger->sing();

		return 0;
	}
}

