#include "pch.h"


#include <iostream>

#include "strategy.h"

using std::cout;
using std::endl;

namespace Strategy::Basic1
{
	class SolidPen : public IPen
	{
	public:
		virtual string draw() const override
		{
			return "----------";
		}
	};

	class DotPen : public IPen
	{
	public:
		virtual string draw() const override
		{
			return "- - - - - ";
		}
	};

	class CContext : public IContext
	{
	public:
		CContext() : m_pen(nullptr) {}
		virtual void setPen(IPen * pen) override
		{
			this->m_pen = pen;
		}
		virtual string draw() const override
		{
			return this->m_pen->draw();
		}

	private:
		IPen* m_pen;
	};

	int Tests::run()
	{
		SolidPen solid;
		DotPen dot;
		CContext context;

		context.setPen(&solid);
		cout << context.draw() << endl;

		context.setPen(&dot);
		cout << context.draw() << endl;

		return 0;
	}
}
#if 1
namespace Strategy::Basic1Step2
{
	// This example supports shared pointer and enable_shared_from_this.
	class CPen : public IPen, public std::enable_shared_from_this<CPen>
	{
	public:
		CPen(CPen const&) = delete;
		CPen(CPen&&) = delete;

		template<typename Derived>
		static shared_ptr<Derived> create()
		{
			return make_shared<Helper<Derived>>();
		}
		virtual string draw() const override { return "CPen's draw"; }

	private:
		template<typename Base>
		class Helper : public Base
		{
		public:
			template<typename...Args>
			Helper(Args&&...args) : Base(std::forward<Args>(args)...) {}
		};

	protected:
		CPen() {} // Note: Need to be called from derived class via Helper
	};

	class SolidPen : public CPen
	{
	public:
		SolidPen() {}
		virtual string draw() const override
		{
			return "----------";
		}
	private:
	};

	class DotPen : public CPen
	{
	public:
		virtual string draw() const override
		{
			return "- - - - - ";
		}
	private:
	};

	class CContext : public IContext
	{
	public:
		virtual void setPen(shared_ptr<IPen> pen) override
		{
			this->m_pen = pen;
		}
		virtual string draw() const override
		{
			return this->m_pen->draw();
		}

	private:
		shared_ptr<IPen> m_pen;
	};

	int Tests::run()
	{
		shared_ptr<IPen> solid = CPen::create<SolidPen>();  // can be SolidPen::create<SolidPen>();
		shared_ptr<IPen> dot = CPen::create<DotPen>();		// can be DotPen::create<DotPen>();
		CContext context;

		context.setPen(solid);
		cout << context.draw() << endl;

		context.setPen(dot);
		cout << context.draw() << endl;

		return 0;
	}
}

#endif
