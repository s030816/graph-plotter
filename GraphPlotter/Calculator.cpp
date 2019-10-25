#include "stdafx.h"
#include "Calculator.h"


Calculator::Calculator()
{
}


Calculator::~Calculator()
{
}

void Calculator::removeSpaces(std::string & func)
{
	func.erase(std::remove(func.begin(), func.end(), ' '), func.end());
}

inline UINT8 Calculator::precedence(char c)
{
	switch (c)
	{
	case '^':
		return 3;
	case '*':
	case '/':
		return 2;
	case '+':
	case '-':
		return 1;
	default:
		return 0;
	}
	return 0;
}

void Calculator::parser(std::string & func)
{
	char tmp;
	this->removeSpaces(func);
	func += ')';
	std::vector<char> stack;
	stack.emplace_back('(');
	std::stringstream ss(func);
	while (ss >> tmp)
	{
		if (std::isalpha(static_cast<int>(tmp)))
		{
			this->postfix.emplace_back(tmp);
		}
		else
		{
			switch (tmp)
			{
			case '(':
				stack.emplace_back('(');
				break;
			case '^':
			case '/':
			case '*':
			case '+':
			case '-':
				{
					while (precedence(stack.back()) >= precedence(tmp))
					{
						this->postfix.emplace_back(stack.back());
						stack.pop_back();
					}
					stack.emplace_back(tmp);
				}
				break;
			case ')':
				{
					while (stack.back() != '(')
					{
						this->postfix.emplace_back(stack.back());
						stack.pop_back();
					}
					stack.pop_back();
				}
				break;
			default:
				MessageBoxA(nullptr, "Parser error", "Error", MB_OK);
				return;
			}
		}
	}
	std::string test;
	for (auto& i : this->postfix)
	{
		test += i;
	}
	MessageBoxA(nullptr, test.c_str(), "test", MB_OK);
}
