#pragma once
#ifndef ADAPTER_H_INCLUDED
#define ADAPTER_H_INCLUDED

#include <string>

using std::string;

// Media Adapter design pattern uses Adapter class and Advanced class
// The Advanced class has got the feature which overrides the target class's function.
// The Adapter class inherits the target class and behaves as advanced version of the target class.
// The Adapter class has got the instance of the Advanced class,
// and the Adapter class calls the Advanced class's functions when the overridden function was called.

namespace Adapter::Basic1
{
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

#endif // ADAPTER_H_INCLUDED