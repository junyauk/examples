#pragma once
#ifndef MEMENTO_H_INCLUDED
#define MEMENTO_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::weak_ptr;

namespace Memento::Basic1
{

	class Memento
	{
	public:
		Memento(string const& state) : m_state(state) {}
		string const& getState() const { return m_state; }
	private:
		string m_state;
	};

	class Originator
	{
	public:
		void setState(string const& state)
		{
			cout << "Setting state to: " << state << endl;
			m_state = state;
		}
		string const& getState() const { return m_state; }

		Memento createMemento() const
		{
			cout << "Creating memento...\n";
			return Memento(m_state);
		}

		void restoreMemento(Memento const& memento)
		{
			cout << "Restoring state from memento...\n";
			m_state = memento.getState();
			cout << "State restored to: " << m_state << endl;
		}

	private:
		string m_state;
	};

	class Caretaker
	{
	public:
		void addMemento(Memento const& memento)
		{
			m_mementos.push_back(memento);
		}

		Memento const& getMemento(int index) const
		{
			return m_mementos[index];
		}

	private:
		vector<Memento> m_mementos;
	};

	class Tests
	{
	public:
		int run();
	};


}

namespace Memento::Basic2
{
	class TextMemento
	{
	public:
		TextMemento(string const& text) : m_text(text) {}
		string const& getText() const { return m_text; }
	private:
		string m_text;
	};

	class TextEditor
	{
	public:
		void addText(string const& text) { m_text += text; }
		string const& getText() const { return m_text; }

		TextMemento saveToMemento() const { return TextMemento(m_text); }
		void restoreFromMemento(TextMemento const& memento) { m_text = memento.getText(); }

	private:
		string m_text;
	};

	class History
	{
	public:
		void push(TextMemento const& memento) { m_mementos.push_back(memento); }
		TextMemento pop()
		{
			if (!m_mementos.empty())
			{
				TextMemento m = m_mementos.back();
				m_mementos.pop_back();
				return m;
			}
			return TextMemento(""); // Return an empty memento
		}
	private:
		vector<TextMemento> m_mementos;
	};

	class Tests
	{
	public:
		int run();
	};
}


#endif // MEMENTO_H_INCLUDED