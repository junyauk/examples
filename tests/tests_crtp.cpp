#include "pch.h"

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <sstream>

#include "crtp.h"

TEST(CRTP, Basic1) // Simple one
{
	using namespace CRTP::Basic1;
	Derived	d;
	EXPECT_STREQ(d.interface().c_str(), "Implementation in Derived class");
	EXPECT_STREQ(d.implementation().c_str(), "Implementation in Derived class");
	EXPECT_STREQ(d.commonFunction().c_str(), "This is a common function in Base class");
}

TEST(CRTP, Basic2) // CRTP with concept
{
	using namespace CRTP::Basic2;
	SolidPen	solid;
	DashPen		dash;
	EXPECT_STREQ(solid.draw().c_str(), "Drawing a solid line");
	EXPECT_STREQ(dash.draw().c_str(), "Drawing a dash line");
}


TEST(CRTP, CRTPSingleton)
{
	using namespace CRTP::Singleton;
	cout << "*** CRTP: Singleton case 1 *****\n";
	{
		// The Derived class inherit the Singleton class
		Derived1& d1 = Derived1::getInstance();
		d1.run(); // shows "run()"
	}

	cout << "*** CRTP: Singleton case 2 *****\n";
	{
		Derived2& d2 = Derived2::getInstance();
		d2.run(); // shows "run()"
	}
}
