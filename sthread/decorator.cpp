#include "pch.h"


#include "decorator.h"

namespace Decorator::Basic1
{
	int Tests::run()
	{
		// First, the coffee is instantiated as SimpleCoffee.
		unique_ptr<Coffee> coffee = make_unique<SimpleCoffee>();
		coffee->brew(); // "Brewing simple coffee\n"

		// Next, the SimpleCoffee is wrapped with MilkDecorator
		coffee = make_unique<MilkDecorator>(std::move(coffee));
		coffee->brew(); // "Brewing simple coffee\nAdding milk to coffee\n"

		// Finally the SimpleCoffee wrapped with MilkDecorator is further wrapped with SugarDecorator
		coffee = make_unique<SugarDecorator>(std::move(coffee));
		coffee->brew(); // "Brewing simple coffee\nAdding milk to coffee\nAdding sugar to coffee\n"

		// Each decorator's brew() runs the base class's brew() first,
		// and run own code.

		return 0;
	}
}

namespace Decorator::Basic2
{
	int Tests::run()
	{
		shared_ptr<PlainText> plainText = make_shared<PlainText>();
		plainText->setText("Plain Text");
		cout << plainText->getText() << endl;

		auto boldText = make_shared<BoldTextDecorator>(plainText);
		cout << boldText->getText() << endl;

		auto italicBoldText = make_shared<ItalicTextDecorator>(boldText);
		cout << italicBoldText->getText() << endl;

		// This example shares the PlainText instance.
		// So once it was updated, boldText and italicBoldText will be affected.
		// each getText() will work for the updated new text.
		plainText->setText("New Text");
		cout << plainText->getText() << endl;
		cout << boldText->getText() << endl;
		cout << italicBoldText->getText() << endl;

		return 0;
	}
}

