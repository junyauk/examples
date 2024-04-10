#pragma once
#ifndef BRIDGE_H_INCLUDED
#define BRIDGE_H_INCLUDED

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unique_ptr;
using std::make_unique;

namespace Bridge::Basic1
{
	class Impl
	{
	public:
		virtual ~Impl() {}
		virtual void operationImpl() const = 0;
	};

	class Abstract
	{
	public:
		Abstract(weak_ptr<Impl> impl)
			: m_impl(impl)
		{}
		virtual ~Abstract() {}
		virtual void operation() const = 0;
	protected:
		weak_ptr<Impl> m_impl;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Bridge::Basic2
{
	class IDevice
	{
	public:
		virtual void powerOn() = 0;
		virtual void powerOff() = 0;
		virtual void play() = 0;
		virtual void pause() = 0;
	};

	class IPlayer
	{
	public:
		IPlayer(std::unique_ptr<IDevice> dev)
			: m_device(std::move(dev)) {}
		virtual ~IPlayer() {}
		virtual void powerOn()  = 0;
		virtual void powerOff() = 0;
		virtual void action() = 0;
	protected:
		unique_ptr<IDevice> m_device;
	};

	class Tests
	{
	public:
		int run();
	};
}


namespace Bridge::Basic2Step2
{
    // Common base class for all device types
    class IDeviceBase
    {
    public:
        virtual ~IDeviceBase() {}
        virtual void powerOn() = 0;
        virtual void powerOff() = 0;
        virtual void play() = 0;
        virtual void pause() = 0;
    };

    // Abstract Device class
    // This template allows IDevice to use DeviceType's methods.
    template<typename DeviceType>
    class IDevice : public IDeviceBase
    {
    public:
        void powerOn() override { static_cast<DeviceType*>(this)->powerOnImpl(); }
        void powerOff() override { static_cast<DeviceType*>(this)->powerOffImpl(); }
        void play() override { static_cast<DeviceType*>(this)->playImpl(); }
        void pause() override { static_cast<DeviceType*>(this)->pauseImpl(); }
    };

    // Abstract UI class
    template<typename PlayerType, typename DeviceType>
    class IPlayer
    {
    protected:
        std::unique_ptr<DeviceType> device;

    public:
        IPlayer(std::unique_ptr<DeviceType> dev)
            : device(std::move(dev)) {}
        void powerOn() { device->powerOn(); }
        void powerOff() { device->powerOff(); }
        void action() { static_cast<PlayerType*>(this)->actionImpl(); }
    };

    class Tests
	{
	public:
		int run();
	};
}



#endif // BRIDGE_H_INCLUDED
