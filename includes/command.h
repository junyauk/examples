#pragma once
#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

namespace Command::Basic1
{
	class IToggle
	{
	public:
		virtual ~IToggle() {}
		virtual void turnOn() = 0;
		virtual void turnOff() = 0;
	};

	class ICommand
	{
	public:
		virtual ~ICommand() {}
		virtual void execute() = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Command::Basic2
{
	class IDocument
	{
	public:
		virtual ~IDocument() {};
		virtual void open() = 0;
		virtual void close() = 0;
		virtual void read() = 0;
	};

	class ICommand
	{
	public:
		virtual ~ICommand() {}
		virtual void execute() = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

#endif // COMMAND_H_INCLUDED