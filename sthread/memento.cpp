#include "pch.h"

#include "memento.h"

namespace Memento::Basic1
{
	int Tests::run()
	{
		Originator originator;
		Caretaker caretaker;

		originator.setState("State 1");
		caretaker.addMemento(originator.createMemento());

		originator.setState("State 2");
		caretaker.addMemento(originator.createMemento());

		originator.setState("State 3");

		originator.restoreMemento(caretaker.getMemento(1));

		return 0;
	}
}

namespace Memento::Basic2
{
	int Tests::run()
	{
		TextEditor editor;
		History history;

		editor.addText("Hellow ");
		history.push(editor.saveToMemento());

		editor.addText("World!");
		history.push(editor.saveToMemento());

		cout << "current: [" << editor.getText() << "]\n";
		editor.restoreFromMemento(history.pop());
		cout << "after undo: [" << editor.getText() << "]\n";
		editor.restoreFromMemento(history.pop());
		cout << "after redo: [" << editor.getText() << "]\n";

		return 0;
	}
}