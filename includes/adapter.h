#pragma once
#ifndef ADAPTER_H_INCLUDED
#define ADAPTER_H_INCLUDED

#include <string>

using std::string;

// Adapter design pattern is a kind of wrapper to convert something via interface methods.

namespace Adapter::Basic1
{
	// Media Adapter design pattern uses Adapter class and Advanced class
	// The Advanced class has got the feature which overrides the target class's function.
	// The Adapter class inherits the target class and behaves as advanced version of the target class.
	// The Adapter class has got the instance of the Advanced class,
	// and the Adapter class calls the Advanced class's functions when the overridden function was called.
		
	// Target interface
	class IMediaPlayer
	{
	public:
		// The method which needs to be overridden
		virtual void play(string const& mediaType, string const& fileName) = 0;
	};

	// Adapter interface
	class IAdvancedMediaPlayer
	{
	public:
		// Extended features. These will be called when the target class's play() was called.
		virtual void playMusic(string const& fileName) = 0;
		virtual void playVideo(string const& fileName) = 0;
	};

	int Run_Adapter();
}


namespace Adapter::Basic2
{
	// This is the target class that need the adapter.
	class ExistingOldClass
	{
	public:
		virtual ~ExistingOldClass() = default;
		virtual void setVal(int const val) { m_val = val; }
		virtual void setStr(string const str) { m_str = str; }
		virtual int getVal() const { return m_val; }
		virtual string getStr() const { return m_str; }
	private:
		int m_val;
		string m_str;
	};

	// Interface that the client expects to work with
	class IAdapter
	{
	public:
		virtual void setVal_New(int const val) = 0;
		virtual void setStr_New(string const str) = 0;
		virtual int getVal_New() const = 0;
		virtual string getStr_New() const = 0;
	};

	class Tests
	{
	public:
		int run();
	};

}

#endif // ADAPTER_H_INCLUDED