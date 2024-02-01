#include "pch.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>
#include <atomic>
#include <random>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::stringstream;
using std::vector;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::unique_lock;
using c_v = std::condition_variable;
using std::queue;
using std::function;
using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::atomic;
using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;

class safeout
{
public:
	static void print(const string& str)
	{
		static mutex	m;
		{
			unique_lock<mutex>	ul(m);
			cout << str << endl;
		}
	}
};


namespace Example1
{
	// a thread-safe quee that holds tasks to be executed asyncronously
	class TaskQueue
	{
	public:
		void push(function<void()> task)
		{
			unique_lock<mutex>	ul(m_mutex);
			m_queue.push(task);
			m_cv.notify_one();
		}

		void pop_and_run()
		{
			function<void()>	task;
			{
				unique_lock<mutex>	ul(m_mutex);
				m_cv.wait(ul, [this]() {return !m_queue.empty(); });
				task = m_queue.front();
				m_queue.pop();
			}
			task();
		}
		
		bool empty()
		{
			unique_lock<mutex>	ul(m_mutex);
			return m_queue.empty();
		}

	private:
		mutex	m_mutex;
		c_v		m_cv;
		queue<function<void()>>	m_queue;
	};

	// a thread pool that manages a fixed number of worker threads
	class ThreadPool
	{
	public:
		ThreadPool(size_t num_threads)
			: m_stop(false)
		{
			for (auto i = 0; i < num_threads; i++)
			{
				m_threads.emplace_back([this]()
					{
						while (true)
						{
							m_taskQueue.pop_and_run();
							unique_lock<mutex>	ul(m_mutex);
							if (m_stop && m_taskQueue.empty())
							{
								break;
							}
						}
					});
			}
		}

		~ThreadPool()
		{
			{
				unique_lock<mutex>	ul(m_mutex);
				m_stop = true;
			}
			for (auto& t : m_threads)
			{
				t.join();
			}
		}
		// submit a new task to the threaad pool
		template <class F, class... Args>
		void submit(F&& f, Args&& ... args)
		{
			// push the task to the queue
			m_taskQueue.push(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		}
	private:
		vector<thread>	m_threads;
		TaskQueue		m_taskQueue;
		mutex			m_mutex;
		bool			m_stop;
	};

	class PrintAndSleep
	{
	public:
		static void print_and_sleep(const string& str, int ms)
		{
			static mutex	m;
			{
				unique_lock<mutex>	ul(m);
				cout << str << endl;
			}
			sleep_for(milliseconds(ms));
		}
	};

	void Run_HalfSyncHalfAsync()
	{
		ThreadPool	tp(4);

		tp.submit(PrintAndSleep::print_and_sleep, "Hello from thread 1", 1000);
		tp.submit(PrintAndSleep::print_and_sleep, "Hello from thread 2", 2000);
		tp.submit(PrintAndSleep::print_and_sleep, "Hello from thread 3", 3000);
		tp.submit(PrintAndSleep::print_and_sleep, "Hello from thread 4", 4000);
		tp.submit(PrintAndSleep::print_and_sleep, "Hello from thread 5", 5000);
		tp.submit(PrintAndSleep::print_and_sleep, "Hello from thread 6", 6000);

		return;
	}
}

namespace Example2
{
	// a request class that contains some data and a result
	class Request
	{
	public:
		Request(int data)
			: m_data(data)
			, m_result(0)
		{}
		int data() const { return m_data; }
		int result() const { return m_result; }
		void setResult(int result) { m_result = result; }
	private:
		int m_data;
		int m_result;
	};

	// a queue class that it thread-safe and supports blocking operation
	class Queue
	{
	public:
		Queue(): m_release(false) {}
		Queue(const Queue& queue) : m_release(false)
		{
			unique_lock<mutex>	uls(queue.m_mutex);
			unique_lock<mutex>	uld(m_mutex);
			m_queue = queue.m_queue;
		}
		void push(Request *request)
		{
			{
				unique_lock<mutex>	ul(m_mutex);
				m_queue.push(request);
			}
			m_cv.notify_one();
		}

		Request *pop()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_cv.wait(ul, [this]() {return !m_queue.empty() || m_release; });
			if (m_release)
			{
				return nullptr;
			}
			Request *request	= m_queue.front();
			m_queue.pop();
			ul.unlock();
			return request;
		}

		void release()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_release = true;
			m_cv.notify_all();
		}
		bool empty()
		{
			unique_lock<mutex>	m_mutex;
			return m_queue.empty();
		}

	private:
		queue<Request*>	m_queue;
		mutable	mutex	m_mutex;
		c_v				m_cv;
		bool			m_release;
	};

	// a synchronous layer class that handles requests from clients
	class SynchLayer
	{
	public:
		SynchLayer(const Queue& queue)
			: m_queue(queue)
		{}

		void receiveRequest(int data)
		{
			stringstream	ss;
			cout << "Received request with data: " << data << endl;
			m_queue.push(new Request(data));
		}

		void sendResult()
		{
			Request *request = m_queue.pop();
			stringstream ss;
			cout << "Sent result with data: " << request->data()
				<< " and result: " << request->result() << endl;
			delete request;
		}

	private:
		Queue	m_queue;
	};

	// an Asynchronous layer class that processes requests from the queue
	class AsynchLayer
	{
	public:
		AsynchLayer(Queue& queue, int numThreads)
			:m_queue(queue)
			, m_numThread(numThreads)
			, m_stop(false)
		{}

		void start()
		{
			for (auto i = 0; i < m_numThread; i++)
			{
				m_threads.push_back(thread(&AsynchLayer::run, this));
			}
		}
		void stop()
		{
			{
				unique_lock<mutex>	ul(m_mutex);
				m_queue.release();
				m_stop = true;
			}
			for (auto& t : m_threads)
			{
				t.join();
			}
		}

		void run()
		{
			auto id = std::this_thread::get_id();
			stringstream ss;
			ss << "run(" << id << ") ->";
			safeout::print(ss.str());
			ss.clear();
			while (true)
			{
				if (m_stop)
				{
					break;
				}
				Request	*request = m_queue.pop();
				if (request != nullptr)
				{
					ss << " - Thread (" << id << ") popped a request " << request->data();
					process_request(request);
					m_queue.push(request);
				}
			}
			ss << "run(" << id << ") <-";
		}

		// process a request by performing some computation
		void process_request(Request *request)
		{
			cout << "Processing request with data: " << request->data() << endl;
			int result = request->data() * 2;
			sleep_for(seconds(1));
			request->setResult(result);
			cout << "Processed request with data: " << request->data()
				<< " and result: " << request->result() << endl;
		}

	private:
		mutex	m_mutex;
		Queue& m_queue;
		int m_numThread;
		bool	m_stop;
		vector<thread>	m_threads;
	};

	void Run_HalfSyncHalfAsync()
	{
		Queue	queue;
		SynchLayer	sync(queue);
		AsynchLayer	async(queue, 4);

		async.start();

		sleep_for(seconds(10));

		// simulate some requests from clients
		for (auto i = 1; i <= 10; i++)
		{
			sync.receiveRequest(i);
			sync.sendResult();
		}
		sleep_for(seconds(10));

		async.stop();

		return;
	}
}

namespace Example3
{
	// a task class that represents a unit of task
	class Task
	{
	public:
		Task(int id)
			: m_id(id)
		{}
		void execute()
		{
			auto tid = std::this_thread::get_id();
			// simulate some work
			stringstream	ss;
			ss << "Task(" << m_id << ") on thread(" << tid << ")";
			safeout::print(ss.str());
			sleep_for(seconds(1));
		}
	private:
		int m_id;
	};

	// a queue class that stores tasks and synchronizes access
	class Queue
	{
	public:
		Queue() : m_done(false) {}
		void push(Task* task)
		{
			unique_lock<mutex>	ul(m_mutex);
			m_queue.push(task);
			m_cv.notify_one();
		}
		Task* pop()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_cv.wait(ul, [this]() {return !m_queue.empty() || m_done; });
			if (m_done)
			{
				return nullptr;
			}
			Task* task = m_queue.front();
			m_queue.pop();
			return task;
		}

		void done()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_done = true;
			m_cv.notify_all();
		}
	private:
		queue<Task*>	m_queue;
		mutex	m_mutex;
		c_v		m_cv;
		bool	m_done;
	};

	// a worker class that runs tasks from the queue
	class Worker
	{
	public:
		Worker(Queue* queue) : m_queue(queue) {}
		void operator()()
		{
			// loop until the queue is done
			while (true)
			{
				// pop a task from the queue
				Task* task = m_queue->pop();
				// if null, break the loop
				if (task == nullptr)
				{
					break;
				}
				// otherwise execute the task
				task->execute();
				// delete the task
				delete task;
			}
		}
	private:
		Queue	*m_queue;
	};

	// a service class that manages the queue and the workers
	class Service
	{
	public:
		Service(int numWorkers)
			: m_numWorkers(numWorkers)
		{
			m_queue = new Queue();
			// create workers
			for (auto i = 0; i < m_numWorkers; i++)
			{
				m_workers.emplace_back(new thread(Worker(m_queue)));
			}
		}

		~Service()
		{
			// delete the queue
			delete m_queue;
			// delete the workers
			for (auto i = 0; i < m_numWorkers; i++)
			{
				delete m_workers[i];
			}
		}

		void submit(Task *task)
		{
			// push the task to the queue
			m_queue->push(task);
		}

		void shutdown()
		{
			m_queue->done();
			for (auto& w : m_workers)
			{
				w->join();
			}
		}

	private:
		Queue* m_queue;
		int m_numWorkers;
		vector<thread*>	m_workers;
	};

	void Run_HalfSyncHalfAsync()
	{
		// create a service with 4 workers
		Service service(4);

		// submit 10 tasks to the service
		for (auto i = 0; i < 4; i++)
		{
			service.submit(new Task(i));
		}


		sleep_for(seconds(10));
		// shutdown the service
		service.shutdown();

		return;
	}
}

namespace Example4
{
	// a class that represents a client request
	class Request
	{
	public:
		Request(int id, int data)
			: m_id(id)
			, m_data(data)
		{}
		int getId() const { return m_id; }
		int getData() const { return m_data; }
	private:
		int m_id;
		int m_data;
	};

	// a class that represents a queue of requests
	class RequestQueue
	{
	public:
		RequestQueue()
			: m_release(false)
		{}

		void enqueue(Request *request)
		{
			unique_lock<mutex>	ul(m_mutex);
			m_queue.push(request);
			m_cv.notify_one();
		}
		Request *dequeue()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_cv.wait(ul, [this]() {return !m_queue.empty() || m_release; });
			if (m_release)
			{
				return nullptr;
			}
			Request *request = m_queue.front();
			m_queue.pop();
			return request;
		}
		void release()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_release = true;
			m_cv.notify_all();
		}
	private:
		queue<Request*>	m_queue;
		mutex			m_mutex;
		c_v				m_cv;
		bool			m_release;
	};

	// a class that represents a half-sync/half-async server
	class Server
	{
	public:
		Server(int numThreads)
			: m_numThreads(numThreads)
			, m_stop(false)
		{
			for (auto i = 0; i < m_numThreads; i++)
			{
				m_threads.emplace_back(thread(&Server::worker, this, i));
			}
		}
		~Server()
		{
			cout << "Destructor of Server ->" << endl;
			// stop workers and join them
			m_queue.release();
			m_stop = true;
			for (auto& t : m_threads)
			{
				cout << "join" << endl;
				t.join();
			}
			cout << "Destructor of Server <-" << endl;
		}

		// handle a client request asynchronously
		void handleRequest(Request *request)
		{
			// add the request to the queue
			m_queue.enqueue(request);
		}

	private:
		// a function that processs a request
		void processRequest(int id, Request *request)
		{
			// simulate some work by sleep for a random duration
			mt19937 rng(random_device{}());
			uniform_int_distribution<int> dist(100, 500);
			sleep_for(milliseconds(dist(rng)));
			// print the result
			cout << "Thread" << id << " processed request " << request->getId() << " with Data " << request->getData() << endl;
		}

		void worker(int id)
		{
			while (!m_stop)
			{
				// get a request from the queue
				Request *request = m_queue.dequeue();
				if (request == nullptr)
				{
					break;
				}
				// process the request
				processRequest(id, request);
				delete request;
			}
		}

		int m_numThreads;
		vector<thread>	m_threads;
		RequestQueue	m_queue;
		bool			m_stop;
	};

	// a function that simulates a client that sends requests to the server
	void client(Server& server, int numRequests)
	{
		// generate and send random requests
		mt19937	rng(random_device{}());
		uniform_int_distribution<int>	dist(1, 100);
		for (auto i = 0; i < numRequests; i++)
		{
			Request *request = new Request(i, dist(rng));
			server.handleRequest(request);
		}
	}

	void Run_HalfSyncHalfAsync()
	{
		// create a server with 4 threads
		Server server(4);
		// create a client that sends 10 requests
		client(server, 10);
		sleep_for(seconds(10));
		return;
	}
}

namespace Example5
{
	class Task
	{
	public:
		Task(int id, int data)
			: m_id(id)
			, m_data(data)
		{
			m_str = "TaskID(" + to_string(m_id) + "), data: " + to_string(m_data);
		}
		string get()
		{
			return m_str;
		}
	private:
		int m_id;
		int m_data;
		string m_str;
	};
	using TaskPtr = shared_ptr<Task>;

	class Queue
	{
	public:
		Queue() : m_release(false) {}
		void push(TaskPtr task)
		{
			unique_lock<mutex>	ul(m_mutex);
			m_queue.push(task);
			m_cv.notify_one();
		}
		TaskPtr pop()
		{
			TaskPtr task;
			{
				unique_lock<mutex>	ul(m_mutex);
				m_cv.wait(ul, [this]() { return !m_queue.empty() || m_release; });
				if (m_release)
				{
					return nullptr;
				}
				task = m_queue.front();
				m_queue.pop();
			}
			return task;
		}
		void release()
		{
			unique_lock<mutex>	ul(m_mutex);
			m_release = true;
			m_cv.notify_all();
		}
	private:
		queue<TaskPtr>	m_queue;
		mutex			m_mutex;
		c_v				m_cv;
		bool			m_release;
	};
	using QueuePtr = shared_ptr<Queue>;

	class Async
	{
	public:
		Async(QueuePtr queue, int numThreads)
			: m_numThreads(numThreads)
			, m_queue(queue)
			, m_stop(false)
		{
			for (auto i = 0; i < m_numThreads; i++)
			{
				m_threads.emplace_back(thread(&Async::process, this));
			}
		}

		~Async()
		{
			m_queue->release();
			m_stop = true;
			for (auto& t : m_threads)
			{
				t.join();
			}
		}

		void process()
		{
			stringstream	ss;
			auto tid = std::this_thread::get_id();
			ss << "Thread[" << tid << "] ->";
			safeout::print(ss.str());
			ss.clear();
			while (true)
			{
				if (m_stop)
				{
					break;
				}
				TaskPtr task = m_queue->pop();
				if (task == nullptr)
				{
					break;
				}
				ss << "Thread(" << tid << ") [" << task.get() << "]";
				safeout::print(ss.str());
			}
		}

	private:
		int			m_numThreads;
		QueuePtr	m_queue;
		atomic<bool>	m_stop;
		vector<thread>	m_threads;
	};

	class Sync
	{
	public:
		Sync(QueuePtr queue)
			: m_queue(queue)
		{}

		void setTask(int id, int data)
		{
			m_queue->push(make_shared<Task>(id, data));
		}

	private:
		QueuePtr	m_queue;
	};

	void Run_HalfSyncHalfAsync()
	{
		// Create a queue, it is shared with both sync and async
		QueuePtr q = make_shared<Queue>();

		Async	a(q, 4);
		Sync	s(q);

		sleep_for(seconds(5));

		for (auto i = 0; i < 10; i++)
		{
			s.setTask(i, i*2);
		}

		sleep_for(seconds(5));
		return;
	}

}

int Run_HalfSyncHalfAsync()
{
	// NG
	// Example1::Run_HalfSyncHalfAsync();
	// Example2::Run_HalfSyncHalfAsync();
	// OK
	// Example3::Run_HalfSyncHalfAsync();
	// Example4::Run_HalfSyncHalfAsync();
	Example5::Run_HalfSyncHalfAsync();
	return 0;
}

