#include "pch.h"
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

using std::cout;
using std::endl;
using std::string;

using std::thread;
using std::mutex;
using std::unique_lock;
using c_v = std::condition_variable;

// The balking pattern is a way of preventing an object from executing an action 
// if it is not in a particular state 1. 
// For example, if an object reads files and a calling method 
// invokes a get method on the object when the ZIP file is not open, 
// the object would “balk” at the request 

// This design pattern is useful if letting threads work with only a particular condition.
// Open: Only works if m_isOpen is false.
// Read: Only works if m_isOpen is true.
// Write: Only works if m_isOoen is true, and will wait for file until it's got true.
// Read: Only works if m_isOpen is true, and will wait for file until it's got true.

class File
{
public:
	File()
		: m_isOpen(false)
		, m_content("")
	{}

	void open()
	{
		unique_lock<mutex> ul(m_mutex);
		if (m_isOpen)
		{
			cout << "File is already open" << endl;
			return;
		}
		m_isOpen = true;
		m_content = "Hello World!";
		cout << "File was opened" << endl;
		m_cv.notify_all();
	}

	void close()
	{
		unique_lock<mutex> ul(m_mutex);
		if (!m_isOpen)
		{
			cout << "File is already closed" << endl;
			return;
		}
		m_isOpen = false;
		m_content = "";
		cout << "File was closed" << endl;
		m_cv.notify_all();
	}

	void read()
	{
		unique_lock<mutex> ul(m_mutex);
		while (!m_isOpen)
		{
			cout << "Waiting for file to open" << endl;
			m_cv.wait(ul);
		}
		cout << "Reading file: " << m_content << endl;
	}

	void write( const string & newContent)
	{
		unique_lock<mutex> ul(m_mutex);
		while (!m_isOpen)
		{
			cout << "Waiting for file to open" << endl;
			m_cv.wait(ul);
		}
		m_content = newContent;
		cout << "Writing file: " << m_content << endl;
	}

private:
	mutex m_mutex;
	c_v m_cv;	// This is used for letting threads know the status that file is open/close.
	bool m_isOpen;
	string m_content;
};

int Run_Balking()
{
	File f;
	thread	th_read(&File::read, &f);
	thread	th_write(&File::write, &f, "Hello C++!");
	thread	th_open(&File::open, &f);
	thread	th_close(&File::close, &f);

	th_read.join();
	th_write.join();
	th_open.join();
	th_close.join();

	return 0;
}

