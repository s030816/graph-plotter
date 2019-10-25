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

void Calculator::calc(std::vector<GLfloat>& stack, char op)
{
	GLfloat fNum = stack.back();
	stack.pop_back();
	GLfloat sNum = stack.back();
	stack.pop_back();
	switch (op)
	{
	case '+':
		stack.emplace_back(sNum + fNum);
		break;
	case '-':
		stack.emplace_back(sNum - fNum);
		break;
	case '*':
		stack.emplace_back(sNum*fNum);
		break;
	case '/':
		stack.emplace_back(sNum/fNum);
		break;
	case '^':
		stack.emplace_back(std::powf(sNum,fNum));
		break;
	default:
		MessageBoxA(nullptr, "calculation error", "Error", MB_OK);
	}
}

GLfloat Calculator::eval_expr(GLfloat x)
{
	std::vector<GLfloat> stack;
	for (auto& i : this->postfix)
	{
		if (std::isalpha(static_cast<int>(i)))
		{
			auto iter = std::find_if(this->operands.begin(),
				this->operands.end(), [&](const auto& s) { return s.first == i; });
			if (iter == operands.end())
			{
				stack.emplace_back(x);
			}
			else
			{
				stack.emplace_back(iter->second);
			}
		}
		else
		{
			this->calc(stack, i);
		}
	}
	return stack.back();
}

void Calculator::lexer(std::string & func)
{
	std::stringstream ss(func);
	std::string tmpStr;
	GLfloat tmpFloat;
	char curChar = 'A';
	char tmpChar;
	while (!ss.eof())
	{
		if (ss >> tmpFloat)
		{
			tmpStr += curChar;
			this->operands.emplace_back(std::make_pair(curChar++,tmpFloat));
		}
		else
		{
			ss.clear();
		}
		while (ss >> tmpChar)
		{
			tmpStr += tmpChar;
			if (tmpChar != ')')
				break;
		}
			
	}
	//MessageBoxA(nullptr, tmpStr.c_str(), "test", MB_OK);
	func = tmpStr;
}

void Calculator::parser(std::string & func)
{
	char tmp;
	this->removeSpaces(func);
	this->lexer(func);
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
	MessageBoxA(nullptr, std::to_string(this->eval_expr(3)).c_str(), "test", MB_OK);
}
