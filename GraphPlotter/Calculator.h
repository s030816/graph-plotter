#pragma once

class Calculator
{
private:
	std::vector<std::pair<char, GLfloat> > operands;
	std::vector<char> postfix;
public:
	Calculator();
	virtual ~Calculator();

	void removeSpaces(std::string& func);
	inline UINT8 precedence(char c);
	void calc(std::vector<GLfloat>& stack, char op);
	GLfloat eval_expr(GLfloat x);
	void lexer(std::string& func);
	void parser(std::string& func);
};

