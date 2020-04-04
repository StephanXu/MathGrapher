#pragma once

#include <string>
#include <sstream>
#include <map>

using namespace std;

class CCalc
{
public:
	CCalc();
	~CCalc();

#define MAXLEN 16384
	const char m_Tab = 0x9;
	const int m_DIGIT = 1;
	char* m_expression;

	map<char, double> m_UnknownVar;

	double fun(double x, char op[], int *iop);
	double calc(char *expr, char **addr, bool *errRet);
	void SetExpression(string exp);
	double GetValue(bool* pRet = nullptr);

	void AddUnknownVar(char symbol, double val = 0);
	void SetUnknownValue(char symbol, double val);
};

