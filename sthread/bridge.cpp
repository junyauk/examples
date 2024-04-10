#include "pch.h"

#include "bridge.h"

namespace Bridge::Basic1
{
	class CImplA : public Impl
	{
	public:
		virtual void operationImpl() const override
		{
			cout << "CImplA" << endl;
		}
	};

	class CImplB : public Impl
	{
	public:
		virtual void operationImpl() const override
		{
			cout << "CImplB" << endl;
		}
	};

	class CAbstract : public Abstract
	{
	public:
		CAbstract(weak_ptr<Impl> impl)
			: Abstract(impl)
		{}
		virtual void operation() const override
		{
			cout << "CAbstract's operation runs impl's operation" << endl;
			shared_ptr impl = m_impl.lock();
			if (impl != nullptr)
			{
				impl->operationImpl();
			}
		}
	};

	int Tests::run()
	{
		shared_ptr<Impl>	implA = make_shared<CImplA>();
		shared_ptr<Impl>	implB = make_shared<CImplB>();

		shared_ptr<CAbstract> absA = make_shared<CAbstract>(implA);
		shared_ptr<CAbstract> absB = make_shared<CAbstract>(implB);

		absA->operation();
		absB->operation();

		return 0;
	}
}

namespace Bridge::Basic2
{
	// The IPlayer (UI layer) has got an instance of Device
	class ClassicPlayer : public IPlayer
	{
	public:
		ClassicPlayer(unique_ptr<IDevice> device)
			: IPlayer(std::move(device))
		{}
		virtual void powerOn() override { m_device->powerOn(); }
		virtual void powerOff() override { m_device->powerOff(); }
		virtual void action() override { m_device->play(); m_device->pause(); }
	};

	class TogglePlayer : public IPlayer
	{
	public:
		TogglePlayer(unique_ptr<IDevice> device)
			: IPlayer(std::move(device))
		{}
		virtual void powerOn() override { m_device->powerOn(); }
		virtual void powerOff() override { m_device->powerOff(); }
		virtual void action(int state ){ state? m_device->play() : m_device->pause(); }
	private:
		virtual void action() override {}
	};

	class TapeDevice : public IDevice
	{
	public:
		virtual void powerOn() override { m_power = true; cout << "Tape Device Powered On" << endl; }
		virtual void powerOff() override { m_power = false; cout << "Tape Device Powered Off" << endl; }
		virtual void play() override
		{
			if (m_power)
			{
				cout << "Tape Device play music" << endl;
				m_play = true;
			}
		}
		virtual void pause() override
		{
			if (m_power)
			{
				if (m_play)
				{
					cout << "Tape Device pause music" << endl;
				}
				else
				{
					cout << "Tape Device play music" << endl;
				}
				m_play = !m_play;
			}
		}

	private:
		bool m_play{ false };
		bool m_power{ false };
	};

	class VinylDevice : public IDevice
	{
	public:
		virtual void powerOn() override { m_power = true;  cout << "Vinyl Device Powered On" << endl; }
		virtual void powerOff() override { m_power = false; cout << "Vinyl Device Powered Off" << endl; }
		virtual void play() override
		{
			if (m_power)
			{
				cout << "Vinyl Device play music" << endl;
				m_play = true;
			}
		}
		virtual void pause() override
		{
			if (m_power)
			{
				if (m_play)
				{
					cout << "Vinyl Device pause music" << endl;
				}
				else
				{
					cout << "Vinyl Device play music" << endl;
				}
				m_play = !m_play;
			}
		}

	private:
		bool m_play{ false };
		bool m_power{ false };
	};

	int Tests::run()
	{
		unique_ptr<IPlayer> classicPlayer = make_unique<ClassicPlayer>(make_unique<TapeDevice>());
		unique_ptr<TogglePlayer> togglePlayer = make_unique<TogglePlayer>(make_unique<VinylDevice>());

		classicPlayer->powerOn();
		classicPlayer->action();
		classicPlayer->powerOff();

		togglePlayer->powerOn();
		togglePlayer->action(1);
		togglePlayer->action(0);
		togglePlayer->powerOff();

		return 0;
	}
}

namespace Bridge::Basic2Step2
{
	// Concrete Device classes
	class TapeDevice : public IDevice<TapeDevice>
	{
	public:
		void powerOnImpl() { cout << "Tape Device is powered on." << endl; }
		void powerOffImpl() { cout << "Tape Device is powered off." << endl; }
		void playImpl() { cout << "Playing music tape." << endl; }
		void pauseImpl() { cout << "Pausing music tape." << endl; }
	};

	class VinylDevice : public IDevice<VinylDevice>
	{
	public:
		void powerOnImpl() { cout << "Vinyl Device is powered on." << endl; }
		void powerOffImpl() { cout << "Vinyl Device is powered off." << endl; }
		void playImpl() { cout << "Playing vinyl record." << endl; }
		void pauseImpl() { cout << "Pausing vinyl record." << endl; }
	};

	// Concrete UI classes
	class ButtonPlayer : public IPlayer<ButtonPlayer, IDeviceBase>
	{
	public:
		ButtonPlayer(std::unique_ptr<IDeviceBase> dev)
			: IPlayer<ButtonPlayer, IDeviceBase>(std::move(dev)) {}
		void actionImpl()
		{
			device->play();
			device->pause();
		}
	};

	class TogglePlayer : public IPlayer<TogglePlayer, IDeviceBase>
	{
	public:
		TogglePlayer(std::unique_ptr<IDeviceBase> dev)
			: IPlayer<TogglePlayer, IDeviceBase>(std::move(dev)) {}
		void actionImpl() { device->play(); }
	};


	int Tests::run()
	{
		// Using Button UI with Tape Device
		std::cout << "Using Button UI with Tape Device:" << std::endl;
		auto buttonPlayer = std::make_unique<ButtonPlayer>(std::make_unique<TapeDevice>());
		buttonPlayer->powerOn();
		buttonPlayer->action();
		buttonPlayer->powerOff();
		std::cout << std::endl;

		// Using Toggle UI with Tape Device
		std::cout << "Using Toggle UI with Tape Device:" << std::endl;
		auto togglePlayer1 = std::make_unique<TogglePlayer>(std::make_unique<TapeDevice>());
		togglePlayer1->powerOn();
		togglePlayer1->action();
		togglePlayer1->powerOff();
		std::cout << std::endl;

		// Using Button UI with Vinyl Device
		std::cout << "Using Button UI with Vinyl Device:" << std::endl;
		auto buttonPlayer2 = std::make_unique<ButtonPlayer>(std::make_unique<VinylDevice>());
		buttonPlayer2->powerOn();
		buttonPlayer2->action();
		buttonPlayer2->powerOff();
		std::cout << std::endl;

		// Using Toggle UI with Vinyl Device
		std::cout << "Using Toggle UI with Vinyl Device:" << std::endl;
		auto togglePlayer2 = std::make_unique<TogglePlayer>(std::make_unique<VinylDevice>());
		togglePlayer2->powerOn();
		togglePlayer2->action();
		togglePlayer2->powerOff();

		return 0;
	}
}

