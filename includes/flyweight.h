#pragma once
#ifndef FLYWEIGHT_H_INCLUDED
#define FLYWEIGHT_H_INCLUDED

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unique_ptr;
using std::make_unique;


namespace Flyweight::Basic1
{
	class Singer // as Flyweight
	{
	public:
		virtual ~Singer() {}
		virtual void sing() = 0;
	};

	template<typename Derived>
	class ILabel
	{
	public:
		virtual ~ILabel() {}
		Singer* getSinger(string const& type)
		{
			auto it = m_singers.find(type);
			if (it == m_singers.end())
				return nullptr;
			return it->second.get();
		}
	protected:
		template<typename T>
		void registerSinger(string const &singer)
		{
			m_singers[singer] = unique_ptr<T>();
		}
	private:
		unordered_map<string, unique_ptr<Singer>> m_singers;
	};


	class Tests
	{
	public:
		int run();
	};

}

#endif // FLYWEIGHT_H_INCLUDED