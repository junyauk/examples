#pragma once
#ifndef FACADE_H_INCLUDED
#define FACADE_H_INCLUDED

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

namespace Facade::Basic1
{
	class SubSystemA
	{
	public:
		void operation() { cout << "SubSystem A operation\n"; }
	};

	class SubSystemB
	{
	public:
		void operation() { cout << "SubSystem B operation\n"; }
	};

	class SubSystemC
	{
	public:
		void operation() { cout << "SubSystem C operation\n"; }
	};

	// Facade is a kind of wrap class that contains subsystems or libraries.
	// This class provide a simple interface set 
	// instead of using a set of sybsystems (or libraries API).
	class Facade
	{
	public:
		void operation1()
		{
			m_a.operation();
			m_b.operation();
		}
		void operation2()
		{
			m_b.operation();
			m_c.operation();
		}

	private:
		SubSystemA m_a;
		SubSystemB m_b;
		SubSystemC m_c;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Facade::Basic2
{
	template<typename Derived>
	class MultimediaFacadeBase
	{
	public:
		void playMedia( string const &name, string const &type)
		{
			static_cast<Derived*>(this)->playMediaImpl(name, type);
		}
	};

	class MultimediaFacade : public MultimediaFacadeBase<MultimediaFacade>
	{
	public:
		void playMediaImpl(string const& name, string const& type)
		{
			if (type == "audio"){ playAudio(name); }
			else if (type == "video") { playVideo(name); }
			else if (type == "image") {	playImage(name); }
			else { cout << "unsupported media type\n"; }
		}
	private:
		// These methods would use subsystems or libraries features.
		void playAudio(string const& name) { cout << "Play audio\n"; }
		void playVideo(string const& name) { cout << "Play video\n"; }
		void playImage(string const& name) { cout << "Play image\n"; }
	};

	class Tests
	{
	public:
		int run();
	};

}



#endif // FACADE_H_INCLUDED