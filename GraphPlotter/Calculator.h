#pragma once

class Calculator
{
private:
	std::vector<char> postfix;
public:
	Calculator();
	virtual ~Calculator();

	void removeSpaces(std::string& func);
	inline UINT8 precedence(char c);
	void parser(std::string& func);
};

