#include "pch.h"

#include <iostream>
#include <string>
#include <memory>

#include "command.h"

using std::cout;
using std::endl;
using std::string;

using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::weak_ptr;

namespace Command::Basic1
{
	// Define target objects and what to do for it.

	class Light : public IToggle
	{
	public:
		virtual void turnOn()
		{
			cout << "turn the light on\n";
		}
		virtual void turnOff()
		{
			cout << "turn the light off\n";
		}
	};

	class Motor : public IToggle
	{
	public:
		virtual void turnOn()
		{
			cout << "turn the motor on\n";
		}
		virtual void turnOff()
		{
			cout << "turn the motor off\n";
		}
	};

	class TurnOnCommand : public ICommand
	{
	public:
		TurnOnCommand(IToggle& toggle)
			: m_toggle(toggle)
		{}
		virtual void execute() override
		{
			m_toggle.turnOn();
		}
	private:
		IToggle& m_toggle;
	};

	class TurnOffCommand : public ICommand
	{
	public:
		TurnOffCommand(IToggle& toggle)
			: m_toggle(toggle)
		{}
		virtual void execute() override
		{
			m_toggle.turnOff();
		}
	private:
		IToggle& m_toggle;
	};

	class RemoteControl
	{
	public:
		void setCommand(shared_ptr<ICommand> command)
		{
			m_command = command;
		}
		void pressButton()
		{
			m_command->execute();
		}

	private:
		shared_ptr<ICommand>	m_command;
	};

	int Tests::run()
	{
		Light light;
		shared_ptr<ICommand> lightOn = make_shared<TurnOnCommand>(light);
		shared_ptr<ICommand> lightOff = make_shared<TurnOffCommand>(light);
		Motor motor;
		shared_ptr<ICommand> motorOn = make_shared<TurnOnCommand>(motor);
		shared_ptr<ICommand> motorOff = make_shared<TurnOffCommand>(motor);

		// Client code only do is setCommand and pressButton.
		RemoteControl	rc;
		rc.setCommand(lightOn);
		rc.pressButton();
		rc.setCommand(lightOff);
		rc.pressButton();

		rc.setCommand(motorOn);
		rc.pressButton();
		rc.setCommand(motorOff);
		rc.pressButton();

		return 0;
	}
}


namespace Command::Basic2
{
	class Manual : public IDocument
	{
	public:
		Manual(string path) // constructor
			: m_path(path)
		{}
		Manual(Manual const & manual) // copy constructor
			: m_path(manual.m_path)
		{}
		Manual& operator=(Manual const& manual) // operator=
		{
			this->m_path = manual.m_path;
			return *this;
		}

		virtual void open() override
		{
			cout << "Open: " << m_path << endl;
		}
		virtual void close() override
		{
			cout << "Close: " << m_path << endl;
		}
		virtual void read() override
		{
			cout << "Read: " << m_path << endl;
		}
	private:
		string m_path;
	};

	class OpenCommand : public ICommand
	{
	public:
		OpenCommand(IDocument& doc) :m_doc(doc)	{}
		virtual void execute() override { m_doc.open(); }
	private:
		IDocument	&m_doc;
	};

	class CloseCommand : public ICommand
	{
	public:
		CloseCommand(IDocument& doc) :m_doc(doc) {}
		virtual void execute() override { m_doc.close(); }
	private:
		IDocument& m_doc;
	};

	class ReadCommand : public ICommand
	{
	public:
		ReadCommand(IDocument& doc) :m_doc(doc) {}
		virtual void execute() override { m_doc.read(); }
	private:
		IDocument& m_doc;
	};

	class Menu
	{
	public:
		void setCommand(weak_ptr<ICommand> command)
		{
			m_command = command;
		}
		void click()
		{
			shared_ptr<ICommand> cmd = m_command.lock();
			if (cmd != nullptr)
			{
				cmd->execute();
			}
			else
			{
				cout << "Command needs to be set\n";
			}
		}

	private:
		weak_ptr<ICommand> m_command;
	};

	int Tests::run()
	{
		Manual	manual("c:\\Document\\Manual\\Tech01.pdf");

		shared_ptr<OpenCommand> open = make_shared<OpenCommand>(manual);
		shared_ptr<CloseCommand> close = make_shared<CloseCommand>(manual);
		shared_ptr<ReadCommand> read = make_shared<ReadCommand>(manual);

		Menu	touchPanel;
		touchPanel.setCommand(open);
		touchPanel.click();
		touchPanel.setCommand(read);
		touchPanel.click();
		touchPanel.setCommand(close);
		touchPanel.click();

		return 0;
	}
}
