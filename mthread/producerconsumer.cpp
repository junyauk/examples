#include "pch.h"
#include "general.h"

#include "producerconsumer.h"

namespace ProducerConsumer::Basic1
{
	void Queue::push(const int& val)
	{
		{
			unique_lock<mutex> ul(m_mutex);
			m_cv.wait(ul, [&] {return m_stop || queue<int>::size() < MAX_QUEUE_SIZE; });
			if (!m_stop)
			{
				queue<int>::push(val);
			}
		}
		m_cv.notify_one();
	}
	int Queue::pop()
	{
		int ret = 0;
		{
			unique_lock<mutex> ul(m_mutex);
			m_cv.wait(ul, [&] {return !queue<int>::empty(); });
			ret = queue<int>::front();
			queue<int>::pop();
		}
		m_cv.notify_one();
		return ret;
	}

	Producer::Producer(Queue& q)
		: m_queue(q)
	{
		m_thread = thread(&Producer::threadFunc, this);
	}
	Producer::~Producer()
	{
		m_stop = true;

		if (m_thread.joinable())
		{
			m_queue.stop();
			m_thread.join();
		}
	}

	void Producer::threadFunc()
	{
		random_device rd;
		mt19937	gen(rd());
		uniform_int_distribution<> dis(1, 50);

		while (!m_stop)
		{
			int item = dis(gen);
			m_queue.push(dis(gen));
			stringstream ss;
			ss << "Producer produced " << item << endl;
			ThreadSafeOut::print(ss.str());
			// sleep for a random time
			sleep_for(milliseconds(100));
		}
	}

	Consumer::Consumer(Queue& q)
		: m_queue(q)
	{
		m_thread = thread(&Consumer::threadFunc, this);
	}
	Consumer::~Consumer()
	{
		m_stop = true;
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}
	void Consumer::threadFunc()
	{
		while (!m_stop)
		{
			int item = m_queue.pop();
			stringstream ss;
			ss << "Consumer consumed " << item << endl;
			ThreadSafeOut::print(ss.str());
			// sleep for a random time
			sleep_for(milliseconds(50));
		}
	}
};

namespace ProducerConsumer::Basic2
{
	void Queue::push(Item const& item)
	{
		unique_lock<mutex>	ul(m_mutex);
		m_queue.push(item);
		m_cv.notify_one();
	}

	Item Queue::pop()
	{
		unique_lock<mutex>	ul(m_mutex);
		m_cv.wait(ul, [this] {return !m_queue.empty(); });
		Item item = std::move(m_queue.front());
		m_queue.pop();
		return item;
	}

	Producer::Producer(Queue& queue, int const& numThreads)
		: m_queue(queue)
		, m_stop(false)
	{
		for (auto i = 0; i < numThreads; i++)
		{
			m_threads.emplace_back(&Producer::worker, this, i);
		}
	}
	Producer::~Producer()
	{
		m_stop = true;
		for (auto& t : m_threads)
		{
			t.join();
		}
	}
	void Producer::worker(int const& threadNumber)
	{
		while (!m_stop)
		{
			sleep_for(milliseconds(100));
			Item item;
			item.set("P" + std::to_string(threadNumber));
			m_queue.push(item);
		}
	}

	Consumer::Consumer(Queue& q, int const& numThreads)
		: m_queue(q)
		, m_stop(false)
	{
		for (auto i = 0; i < numThreads; i++)
		{
			m_threads.emplace_back(&Consumer::worker, this, i);
		}
	}
	Consumer::~Consumer()
	{
		m_stop = true;
		for (auto& t : m_threads)
		{
			t.join();
		}
	}
	void Consumer::worker(int const& threadNumber)
	{
		while (!m_stop)
		{
			Item item = m_queue.pop();
			{
				unique_lock<mutex> ul(m_mutex);
				cout << "C" << threadNumber << ": [" << item() << "]" << endl;
			}
		}
	}
}

namespace ProducerConsumer::Printer1
{
	Printer::Printer(int threadCount, int queueSize)
		: m_isTerminationRequested(false)
		, m_queue(queueSize)
	{
		cout << "Printer constructor ->\n";
		for (int i = 0; i < threadCount; i++)
		{
			// function object "m_main" requires one parameter.
			m_threads.emplace_back(thread(m_main, i));
		}
		cout << "Printer constructor <-\n";
	}

	Printer::~Printer()
	{
		cout << "Printer destructor ->\n";
		{
			unique_lock<mutex> ul(m_mutex);
			m_isTerminationRequested = true;
			m_cv.notify_all();
		}
		auto size = m_threads.size();
		for (auto i = 0; i < size; i++)
		{
			m_threads[i].join();
		}
		cout << "Printer destructor <-\n";
	}

	bool Printer::append(string&& str) // str is rref, move
	{
		cout << "Printer append ->\n";
		{
			unique_lock<mutex> ul(m_mutex);
			if (!m_queue.put(std::move(str)))
			{
				return false;
			}
			m_cv.notify_all();
		}
		cout << "Printer append <-\n";
		return true;
	}

	bool Printer::append(const string& str) // str is lref, copy 
	{
		cout << "Printer append ->\n";
		{
			unique_lock<mutex> ul(m_mutex);
			if (!m_queue.put(str))
			{
				return false;
			}
			m_cv.notify_all();
		}
		cout << "Printer append <-\n";
		return true;
	}
};

namespace ProducerConsumer::TaskQueue
{
	void TaskQueue::addTask(int const& val)
	{
		{
			unique_lock<mutex>	ul(m_mutex);
			m_queue.push(val);
		}
		m_cv.notify_one();
	}
	int TaskQueue::getTask()
	{
		int val = 0;
		{
			unique_lock<mutex> ul(m_mutex);
			m_cv.wait(ul, [this]() { return !m_queue.empty(); });
			val = m_queue.front();
			m_queue.pop();
		}
		m_cv.notify_one();
		return val;
	}
}

namespace ProducerConsumer::Windows1
{
	void Producer::ThreadFunc()
	{
		while (!m_stop)
		{
			int item = m_random.gen();
			m_queue->push(item);
			stringstream ss;
			ss << "Producer produced " << item << endl;
			ThreadSafeOut::print(ss.str());
			// sleep for a random time
			Sleep(100);
		}
	}

	void Consumer::ThreadFunc()
	{
		while (!m_stop)
		{
			int item = m_queue->pop();
			stringstream ss;
			ss << "Consumer consumed " << item << endl;
			ThreadSafeOut::print(ss.str());
			// sleep for a random time
			Sleep(50);
		}
	}

}
