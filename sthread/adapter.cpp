#include "pch.h"

#include <iostream>
#include <memory>
#include <algorithm>

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

namespace Adapter::Basic2
{
	// Solution 1 (containing the ExistingOldClass)
	class CAdapter : public IAdapter
	{
		// CAdapter's interface methods are designed for the new system
		// They convert new stype value to old version,
		// so that the ExistingOldClass can work as expected.
	public:
		CAdapter(ExistingOldClass &old)
			: m_old(old)
		{}
		virtual void setVal_New(int const val)
		{
			// modify the val so that the old class can understand it
			m_old.setVal(val / 10);
		}
		virtual void setStr_New(string const str)
		{
			// modify the val so that the old class can understand it
			m_old.setStr(string(str.rbegin(), str.rend()));
		}
		virtual int getVal_New() const
		{
			int tmp = m_old.getVal();
			// modify the old stype val so that the new system can understand it
			return tmp * 10;
		}
		virtual string getStr_New() const
		{
			string tmp = m_old.getStr();
			std::reverse(tmp.begin(), tmp.end());
			// modify the old stype val so that the new system can understand it
			return tmp;
		}
	private:
		ExistingOldClass& m_old;
	};

	// Solution 1 (inheriting the ExistingOldClass with special feature)
	class SpecialEncoder
	{
		// This class has got encoding feature that ExistingOldClass can understand the encoded ones
	public:
		int encode(const int val) const { return val * 10; }
		string encode(string const str) const { return string{ str.rbegin(), str.rend()}; }
	};
	class SpecialDecoder
	{
		// This class has got decoding feature that New class can understand the decoded ones
	public:
		int decode(const int val) const { return val / 10; }
		string decode(string const str) const { return string{ str.rbegin(), str.rend() }; }
	};

	class NewClass : public ExistingOldClass, public SpecialEncoder, public SpecialDecoder
	{
	public:
		virtual void setVal(int const val) override
		{
			ExistingOldClass::setVal(encode(val));
		}
		virtual void setStr(string const str) override
		{
			ExistingOldClass::setStr(encode(str));
		}
		virtual int getVal() const override
		{
			return decode(ExistingOldClass::getVal()); 
		}
		virtual string getStr() const override
		{
			return decode(ExistingOldClass::getStr());
		}
	};

	// This class needs to be moved to unit tests
	int Tests::run()
	{
		int val;
		string str;
		{
			ExistingOldClass oldClass;
			CAdapter	adapter(oldClass);

			oldClass.setVal(123);
			oldClass.setStr("123");

			val = adapter.getVal_New();
			str = adapter.getStr_New();

			adapter.setVal_New(654);
			adapter.setStr_New("654");

			val = oldClass.getVal();
			str = oldClass.getStr();
		}

		{
			// Check on debugger
			NewClass	newClass;

			newClass.setVal(123);
			val = newClass.getVal();
			newClass.setStr("123");
			str = newClass.getStr();
		}
		return 0;
	}
}
