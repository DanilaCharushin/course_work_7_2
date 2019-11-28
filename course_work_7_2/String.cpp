#include "pch.h"
#include "String.h"


String::String(const char * text)
{
	this->length = strlen(text);
	this->text = new char[this->length + 1];
	strcpy(this->text, text);
}

String::String(const int number)
{
	char buf[256];
	itoa(number, buf, 10);
	*this = buf;
}

String::String(const double number)
{
	char buf[256];
	
}

String::String(const String & s)
{
	length = strlen(s.text);
	text = new char[length + 1];
	strcpy(text, s.text);
}

String::~String()
{
	clear();
}

String & String::operator=(const String & s)
{
	clear();
	length = strlen(s.text);
	text = new char[length + 1];
	strcpy(text, s.text);
	return *this;
}

void String::operator+=(const char * text)
{
	int len = strlen(text);
	String tmp(*this);
	clear();
	this->text = new char[tmp.length + len + 1];
	strcpy(this->text, tmp.text);
	strcat(this->text, text);
	length = tmp.length + len;
}

void String::operator+=(const int number)
{
	String s = number;
	*this += s;
}

void String::operator+=(const double number)
{
}

void String::operator+=(String s)
{
	(*this) += s.text;
}

void String::operator--(int)
{
	char *tmp = new char[strlen(text)];
	strcpy(tmp, text);
	length--;
	tmp[length] = '\0';
	delete[] text;
	text = new char[length + 1];
	strcpy(text, tmp);
	delete[] tmp;
	tmp = nullptr;
}

char & String::operator[](int index)
{
	return text[length % index];
}

char * String::to_char()
{
	return text;
}

int String::to_int()
{
	return atoi(text);
}

void String::clear()
{
	length = 0;
	if (text)
		delete[] text;
}

int String::get_length()
{
	return length;
}

String operator+(String s1, String s2)
{
	String s = s1;
	s += s2;
	return s;
}

String operator+(String s, const char * text)
{
	String res = s;
	s += text;
	return res;
}

String operator+(const char * text, String s)
{
	String res(text);
	res += s;
	return res;
}

istream & operator>>(istream & in, String & s)
{
	char buf[256];
	in.getline(buf, 256);
	String res(buf);
	s = res;
	return in;
}

ostream & operator<<(ostream & os, String s)
{
	os << s.text;
	return os;
}
