#include "pch.h"
#include <iostream>
using namespace std;

#ifndef __STRING_H__
#define __STRING_H__

class String
{
private:
	char *text;
	int length;

public:
	String(const char *text = "");
	String(const int number);
	String(const double number);
	String(const String &s);
	~String();

	String& operator=(const String &s);
	friend String operator+(String s1, String s2);
	friend String operator+(String s, const char *text);
	friend String operator+(const char *text, String s);
	void operator+=(const char *text);
	void operator+=(const int number);
	void operator+=(const double number);
	void operator+=(String s);
	void operator--(int);

	char& operator[](int index);

	friend istream& operator>>(istream& in, String &s);
	friend ostream& operator<<(ostream& os, String s);

	char * to_char();
	int to_int();

	void clear();
	int get_length();
};

#endif