#include "pch.h"

#include "readerwriter.h"

namespace ReaderWriter::Basic1
{
	void readerThread(Data& data)
	{
		for (int i = 0; i < 5; ++i)
		{
			int value = data.read();
			cout << "Reader Thread: " << std::this_thread::get_id() << " read: " << value << endl;
			sleep_for(milliseconds(100));
		}
	}

	void writerThread(Data& data)
	{
		for (int i = 0; i < 5; ++i)
		{
			data.write(i);
			cout << "Writer Thread: " << std::this_thread::get_id() << " wrote: " << i << endl;
			sleep_for(milliseconds(100));
		}
	}

	int Tests::run()
	{
		Data sharedData;

		// The essense of the Reader-Writer design pattern exists in this run().
		// This run() uses multiple readers, but only a single writer.
		// The Data class dosn't have any element of the design pattern.

		thread readers[3];
		for (auto i = 0; i < 3; ++i)
		{
			readers[i] = thread(readerThread, std::ref(sharedData));
		}

		thread writer(writerThread, std::ref(sharedData));

		for (auto i = 0; i < 3; ++i)
		{
			readers[i].join();
		}

		writer.join();

		return 0;
	}
}

namespace ReaderWriter::Basic1Step2
{
	void readerThread(Data& data)
	{
		for (int i = 0; i < 5; ++i)
		{
			int value = data.read();
			cout << "Reader Thread: " << std::this_thread::get_id() << " read: " << value << endl;
			sleep_for(milliseconds(100));
		}
	}

	void writerThread(Data& data)
	{
		for (int i = 0; i < 5; ++i)
		{
			data.write(i);
			cout << "Writer Thread: " << std::this_thread::get_id() << " wrote: " << i << endl;
			sleep_for(milliseconds(100));
		}
	}

	int Tests::run()
	{
		Data sharedData;

		thread readers[5];
		for (auto i = 0; i < 5; ++i)
		{
			readers[i] = thread(readerThread, std::ref(sharedData));
		}

		thread writers[3];
		for (auto i = 0; i < 3; ++i)
		{
			writers[i] = thread(writerThread, std::ref(sharedData));
		}

		for (auto& r : readers)
		{
			r.join();
		}

		for (auto& w : writers)
		{
			w.join();
		}

		return 0;
	}

}
