#pragma once
#ifndef COMPOSITE_H_INCLUDED
#define COMPOSITE_H_INCLUDED

namespace Composite::Basic1
{
	class IComponent
	{
	public:
		virtual ~IComponent() = default;
		virtual void operation() const = 0;
	};

	class Tests
	{
	public:
		int run();
	};
}

namespace Composite::Basic2
{
	class IComponent
	{
	public:
		~IComponent() = default;
		virtual void operation() const = 0;
	};

	class Tests
	{
	public:
		int run();
	};


}

#endif


