#include "pch.h"

#include <iostream>
#include <memory>

#include "adapter.h"

using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;

namespace Adapter::Basic1
{
	// Concrete implementation of Adaptee interface
	class CVideoPlayer : public IAdvancedMediaPlayer
	{
	public:
		void playMusic(string const& fileName)
		{
			// Nothing to do
		}
		void playVideo(string const& fileName)
		{
			cout << "Playing the video file: " << fileName << endl;
		}
	};

	// Concrete implementation of Adaptee interface
	class CMusicPlayer : public IAdvancedMediaPlayer
	{
	public:
		void playMusic(string const& fileName)
		{
			cout << "Playing the music file: " << fileName << endl;
		}
		void playVideo(string const& fileName)
		{
			// Nothing to do
		}
	};

	// Adapter which inherit the original class
	class MediaAdapter : public IMediaPlayer
	{
	public:
		MediaAdapter(string const& mediaType)
		{
			if (mediaType == "video")
			{
				m_advancedMediaPlayer = make_unique<CVideoPlayer>();
			}
			else if (mediaType == "music")
			{
				m_advancedMediaPlayer = make_unique<CMusicPlayer>();
			}
		}

		// Override the original class's method
		void play(string const& mediaType, string const& fileName) override
		{
			if (mediaType == "video")
			{
				m_advancedMediaPlayer->playVideo(fileName);
			}
			else if (mediaType == "music")
			{
				m_advancedMediaPlayer->playMusic(fileName);
			}
		}
	private:
		// Adapter hs got the extended feature's instance
		unique_ptr<IAdvancedMediaPlayer> m_advancedMediaPlayer;
	};

	int Run_Adapter()
	{
		unique_ptr<IMediaPlayer> player = make_unique<MediaAdapter>("music");
		player->play("music", "sakura.mp3");

		player = make_unique<MediaAdapter>("video");
		player->play("video", "godzilla.mp4");

		return 0;
	}
}