#pragma once
#ifndef DECORATOR_H_INCLUDED
#define DECORATOR_H_INCLUDED

#include <iostream>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;

// Decorator design pattern provides a way to dynamically add 
// additional functionality to objects.
// The key point is that the Decorator is instantiated with the original object.
// and it contains the original object.
// So the decorator can run the original object's methods.
// This is a method of wrapping object for adding feature dynamically.

namespace Decorator::Basic1
{
	class Coffee
	{
	public:
		virtual ~Coffee(){}
		virtual void brew() const = 0;
	};

	class SimpleCoffee : public Coffee
	{
	public:
		virtual void brew() const override
		{
			cout << "Brewing simple coffee" << endl;
		}
	};

	class CoffeeDecorator : public Coffee
	{
	public:
		CoffeeDecorator(unique_ptr<Coffee> coffee)
			: m_coffee(std::move(coffee))
		{}
		virtual void brew() const override
		{
			m_coffee->brew();
		}
	private:
		unique_ptr<Coffee> m_coffee;
	};

	class MilkDecorator : public CoffeeDecorator
	{
	public:
		MilkDecorator(unique_ptr<Coffee> coffee)
			: CoffeeDecorator(std::move(coffee))
		{}
		virtual void brew() const override
		{
			CoffeeDecorator::brew();
			cout << "Adding milk to coffee" << endl;
		}
	};

	class SugarDecorator : public CoffeeDecorator
	{
	public:
		SugarDecorator(unique_ptr<Coffee> coffee)
			: CoffeeDecorator(std::move(coffee))
		{}
		virtual void brew() const override
		{
			CoffeeDecorator::brew();
			cout << "Adding sugar to coffee" << endl;
		}
	};


	class Tests
	{
	public:
		int run();
	};
}

namespace Decorator::Basic2
{
	class Text
	{
	public:
		virtual ~Text() {}
		virtual string getText() const = 0;
		virtual void setText(string const& str) {}
	};

	class PlainText : public Text
	{
	public:
		virtual string getText() const override
		{
			return m_text;
		}
		virtual void setText(string const& str) override
		{
			m_text = str;
		}
	private:
		string m_text;
	};

	class TextDecorator : public Text
	{
	public:
		TextDecorator(weak_ptr<Text> text)
			: m_text (text)
		{}
		virtual string getText() const override
		{
			shared_ptr text = m_text.lock();
			if (text != nullptr)
			{
				return text->getText();
			}
			else
				return "";
		}
	private:
		weak_ptr<Text> m_text;
	};

	class BoldTextDecorator : public TextDecorator
	{
	public:
		BoldTextDecorator(weak_ptr<Text> text)
			: TextDecorator(text)
		{}
		virtual string getText() const override
		{
			return "<b>" + TextDecorator::getText() + "</b>";
		}
	};

	class ItalicTextDecorator : public TextDecorator
	{
	public:
		ItalicTextDecorator(weak_ptr<Text> text)
			: TextDecorator(text)
		{}
		virtual string getText() const override
		{
			return "<i>" + TextDecorator::getText() + "</i>";
		}
	};

	class Tests
	{
	public:
		int run();
	};
}


#endif // DECORATOR_H_INCLUDED