#include "pch.h"

#include "proxy.h"
#include "unordered_map"

using std::unordered_map;

namespace Proxy::Basic1
{
	class CSubject : public ISubject
	{
	public:
		virtual void request() const
		{
			cout << "CSubject's request\n";
		}
	};

	class Proxy : public ISubject
	{
	public:
		Proxy() : m_subject(nullptr) {}
		virtual void request() const override
		{
			init();
			m_subject->request();
		}

	private:
		void init() const
		{
			if (m_subject == nullptr)
			{
				const_cast<Proxy*>(this)->m_subject = make_shared<CSubject>();
			}
		}
		shared_ptr<ISubject> m_subject;
	};

	int Tests::run()
	{
		cout << "directly use CSubject\n";
		unique_ptr<ISubject> subject = make_unique<CSubject>();
		subject->request();

		cout << "Use CSubject from Proxy\n";
		unique_ptr<ISubject> proxy = make_unique<Proxy>();
		proxy->request();

		return 0;
	}

}

namespace Proxy::Basic2
{
	class RemoteFileServer : public IFileServer
	{
	public:
		virtual string getFileContents(string const& filename) override
		{
			cout << "Fetching file " << filename << " from remote server\n";
			return "Contents of file <" + filename + ">";
		}
	};

	class FileServerProxy : public IFileServer
	{
	public:
		FileServerProxy(shared_ptr<RemoteFileServer> server) : m_fileServer(server) {}
		virtual string getFileContents(string const& filename) override
		{
			if (m_fileCache.find(filename) == m_fileCache.end())
			{
				// file hasn't been cached. create a new one.
				string contents = m_fileServer->getFileContents(filename);
				m_fileCache[filename] = contents;
				cout << "Cached file <" << filename << "> locally\n";
				return contents;
			}
			else
			{
				cout << "Retrieved file <" << filename << "> from local cache\n";
				return m_fileCache[filename];
			}

			return "";
		}

	private:
		shared_ptr<RemoteFileServer> m_fileServer;
		unordered_map<string, string> m_fileCache;
	};

	int Tests::run()
	{
		shared_ptr<RemoteFileServer> remote = make_shared<RemoteFileServer>();
		unique_ptr<FileServerProxy> proxy = make_unique<FileServerProxy>(remote);

		// accessing contents via proxy.
		cout << proxy->getFileContents("doc1.txt") << endl;
		cout << proxy->getFileContents("doc2.txt") << endl;
		cout << proxy->getFileContents("doc1.txt") << endl;

		return 0;
	}
}

#include "proxy.h"



