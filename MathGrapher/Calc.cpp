#include "stdafx.h"
#include "Calc.h"


CCalc::CCalc()
	:m_Tab(0x9)
	,m_DIGIT(1)
	,m_expression(nullptr)
{
}


CCalc::~CCalc()
{
}


double CCalc::fun(double x, char op[], int *iop) {
	while (op[*iop - 1] < 32)//����ʹ�ú���Ƕ�׵���ʱ���ؼ�����,�� arc sin(sin(1.234)) ֻ�����arc sin sin 1.234<Enter>
	{
		switch (op[*iop - 1]) {
		case  7: x = sin(x);    (*iop)--; break;
		case  8: x = cos(x);    (*iop)--; break;
		case  9: x = tan(x);    (*iop)--; break;
		case 10: x = sqrt(x);   (*iop)--; break;
		case 11: x = asin(x);   (*iop)--; break;
		case 12: x = acos(x);   (*iop)--; break;
		case 13: x = atan(x);   (*iop)--; break;
		case 14: x = log10(x);  (*iop)--; break;
		case 15: x = log(x);    (*iop)--; break;
		case 16: x = exp(x);    (*iop)--; break;
		case 17: x = ceil(x);   (*iop)--; break;
		case 18: x = floor(x);  (*iop)--; break;
		}
	}
	return x;
}

double CCalc::calc(char *expr, char **addr, bool *errRet)
{
	static int deep; //�ݹ����
	static char *fname[] = { "sin","cos","tan","sqrt","arcsin","arccos","arctan","lg","ln","exp","ceil","floor",NULL };
	double ST[10] = { 0.0 }; //����ջ
	char op[10] = { '+' }; //�����ջ
	char c, *rexp, *pp, *pf;
	int ist = 1, iop = 1, last, i;
	bool err{ false };

	if (!deep) {
		pp = pf = expr;
		do {
			c = *pp++;
			if (c != ' '&& c != m_Tab)
				*pf++ = c;
		} while (c != '\0');
	}
	pp = expr;
	if ((c = *pp) == '-' || c == '+') {
		op[0] = c;
		pp++;
	}
	last = !m_DIGIT;
	while ((c = *pp) != '\0') {
		if (c == '(') {//��Բ����
			deep++;
			ST[ist++] = calc(++pp, addr, &err);
			if (err)
			{
				*errRet = true;
				return 0;
			}
			deep--;
			ST[ist - 1] = fun(ST[ist - 1], op, &iop);
			pp = *addr;
			last = m_DIGIT;
			if (*pp == '(' || isalpha(*pp) && _strnicmp(pp, "Pi", 2)) {//Ŀ���ǣ�����Բ����������Ϊ��Բ������������ʱ��Ĭ����Ϊ�˷�
				op[iop++] = '*';
				last = !m_DIGIT;
				c = op[--iop];
				goto operate;
			}
		}
		else if (c == ')') {//��Բ����
			pp++;
			break;
		}
		else if (isalpha(c)) {
			map<char, double>::iterator it;
			if (!_strnicmp(pp, "Pi", 2)) 
			{
				if (last == m_DIGIT) 
				{
					*errRet = true; return 0;//���������
				}
				ST[ist++] = 3.14159265358979323846264338328;
				ST[ist - 1] = fun(ST[ist - 1], op, &iop);
				pp += 2;
				last = m_DIGIT;
				if (!_strnicmp(pp, "Pi", 2)) {
					*errRet = true; return 0;//����������
				}
				if (*pp == '(') {
					*errRet = true; return 0;//���Ҳ�����
				}
			}
			else if ((it = m_UnknownVar.find(c))!= m_UnknownVar.end())
			{
				ST[ist++] = it->second;
				ST[ist - 1] = fun(ST[ist - 1], op, &iop);
				pp++;
				last = m_DIGIT;
			}
			else 
			{
				for (i = 0; (pf = fname[i]) != NULL; i++)
					if (!_strnicmp(pp, pf, strlen(pf))) break;
				map<char, double>::iterator it;
				it = m_UnknownVar.find(c);
				if (pf != NULL)
				{
					op[iop++] = 07 + i;
					pp += strlen(pf);
				}
				else if (it != m_UnknownVar.end())
				{
					op[iop++] = it->first;
					pp++;
				}
				else 
				{
					*errRet = true; return 0;//İ��������/δ֪����
				}
			}
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^') {
			char cc;
			if (last != m_DIGIT) {
				*errRet = true; return 0;//�����ճ��
			}
			pp++;
			if (c == '+' || c == '-') {
				do {
					cc = op[--iop];
					--ist;
					switch (cc) {
					case '+':  ST[ist - 1] += ST[ist]; break;
					case '-':  ST[ist - 1] -= ST[ist]; break;
					case '*':  ST[ist - 1] *= ST[ist]; break;
					case '/':  ST[ist - 1] /= ST[ist]; break;
					case '%':  ST[ist - 1] = fmod(ST[ist - 1], ST[ist]); break;
					case '^':  ST[ist - 1] = pow(ST[ist - 1], ST[ist]); break;
					}
				} while (iop);
				op[iop++] = c;
			}
			else if (c == '*' || c == '/' || c == '%') {
			operate:        cc = op[iop - 1];
				if (cc == '+' || cc == '-') {
					op[iop++] = c;
				}
				else {
					--ist;
					op[iop - 1] = c;
					switch (cc) {
					case '*':  ST[ist - 1] *= ST[ist]; break;
					case '/':  ST[ist - 1] /= ST[ist]; break;
					case '%':  ST[ist - 1] = fmod(ST[ist - 1], ST[ist]); break;
					case '^':  ST[ist - 1] = pow(ST[ist - 1], ST[ist]); break;
					}
				}
			}
			else {
				cc = op[iop - 1];
				if (cc == '^') {
					*errRet = true; return 0;//���ݷ�����
				}
				op[iop++] = c;
			}
			last = !m_DIGIT;
		}
		else {
			if (last == m_DIGIT) {
				*errRet = true; return 0;//������ճ��
			}
			ST[ist++] = strtod(pp, &rexp);
			ST[ist - 1] = fun(ST[ist - 1], op, &iop);
			if (pp == rexp) {
				*errRet = true; return 0;//�Ƿ��ַ�
			}
			pp = rexp;
			last = m_DIGIT;
			if (*pp == '(' || isalpha(*pp)) {
				op[iop++] = '*';
				last = !m_DIGIT;
				c = op[--iop];
				goto operate;
			}
		}
	}
	*addr = pp;
	if (iop >= ist) {
		*errRet = true; return 0;//���ʽ����
	}
	while (iop) {
		--ist;
		switch (op[--iop]) {
		case '+':  ST[ist - 1] += ST[ist]; break;
		case '-':  ST[ist - 1] -= ST[ist]; break;
		case '*':  ST[ist - 1] *= ST[ist]; break;
		case '/':  ST[ist - 1] /= ST[ist]; break;
		case '%':  ST[ist - 1] = fmod(ST[ist - 1], ST[ist]); break;
		case '^':  ST[ist - 1] = pow(ST[ist - 1], ST[ist]); break;
		}
	}
	return ST[0];
}

void CCalc::SetExpression(string exp)
{
	if (m_expression)
		delete[]m_expression;
	m_expression = new char[exp.length() + 1];
	stringstream ss;
	ss << exp;
	ss >> m_expression;
}

double CCalc::GetValue(bool* pRet)
{
	char* endss;
	bool ret{ false };
	double result;
	result = calc(m_expression, &endss, &ret);
	if (pRet)
		*pRet = ret;
	return result;
}

void CCalc::AddUnknownVar(char symbol, double val)
{
	if (m_UnknownVar.find(symbol) != m_UnknownVar.end())
		return;
	m_UnknownVar.insert(pair<char, double>(symbol, val));
	return;
}

void CCalc::SetUnknownValue(char symbol, double val)
{
	m_UnknownVar[symbol] = val;
	return;
}
