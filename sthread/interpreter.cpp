#include "pch.h"
#include <numeric>
#include <sstream>

#include "interpreter.h"

using std::accumulate;
using std::istringstream;

namespace Interpreter::Basic1
{
	class NumberExpression : public IExpression
	{
	public:
		NumberExpression(int number) : m_number(number) {}
		virtual int interpret(unordered_map<char, int>& context) override
		{
			return m_number;
		}
	private:
		int m_number;
	};

	class VariableExpression : public IExpression
	{
	public:
		VariableExpression(char variable) : m_variable(variable) {}
		virtual int interpret(unordered_map<char, int>& context) override
		{
			return context[m_variable];
		}
	private:
		char m_variable;
	};

	class AdditionExpression : public IExpression
	{
	public:
		AdditionExpression(unique_ptr<IExpression> left, unique_ptr<IExpression> right) : m_left(std::move(left)), m_right(std::move(right)) {}
		virtual int interpret(unordered_map<char, int>& context) override
		{
			return m_left->interpret(context) + m_right->interpret(context);
		}

	private:
		unique_ptr<IExpression> m_left;
		unique_ptr<IExpression> m_right;
	};

	int Tests::run()
	{
		unordered_map<char, int> context;
		context['x'] = 5;
		context['y'] = 7;

		unique_ptr<IExpression> expression = make_unique<AdditionExpression>(
			make_unique<VariableExpression>('x'),
			make_unique<AdditionExpression>(
				make_unique<VariableExpression>('y'),
				make_unique<NumberExpression>(9)
			)
		);

		int result = expression->interpret(context);
		cout << "Result: " << result << endl;

		return 0;
	}
}
namespace Interpreter::Basic2
{
	class sumExpression : public IExpression
	{
	public:
		virtual int interpret(vector<int> const& content) const override
		{
			return accumulate(content.begin(), content.end(), 0);
		}
	};

	class averageExpression : public IExpression
	{
	public:
		virtual int interpret(vector<int> const& content) const override
		{
			if (content.empty())
			{
				return 0;
			}
			int sum = accumulate(content.begin(), content.end(), 0);
			return sum / static_cast<int>(content.size());
		}
	};

	class countExpression : public IExpression
	{
	public:
		virtual int interpret(vector<int> const& content) const override
		{
			return static_cast<int>(content.size());
		}
	};

	class contextParser
	{
	public:
		static vector<int> parse(string const& input)
		{
			vector<int> context;
			istringstream iss(input);

			int num = 0;
			while (iss >> num)
			{
				context.push_back(num);
			}
			return context;
		}
	};

	int Tests::run()
	{
		string input = "1 2 3 4 5";
		vector<int> context = contextParser::parse(input);

		unique_ptr<IExpression> sumExp = make_unique<sumExpression>();
		unique_ptr<IExpression> aveExp = make_unique<averageExpression>();
		unique_ptr<IExpression> cntExp = make_unique<countExpression>();

		int sum = sumExp->interpret(context);
		int ave = aveExp->interpret(context);
		int cnt = cntExp->interpret(context);

		cout << "Sum: " << sum << ", Ave: " << ave << ", Cnt: " << cnt << endl;

		return 0;
	}

}
