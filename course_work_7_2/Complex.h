#include "pch.h"
#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;

#ifndef __COMPLEX_H__
#define __COMPLEX_H__

class Complex
{
private:
	double real; //����������� ������������ ����� (a)
	double img;  //����������� ������ ����� (b)
public:
	Complex(int max = 10) : real(rand() % (max + 1)), img(rand() % (max + 1)) {};
	Complex(double real, double img) : real(real), img(img) {};
	Complex(double ro, double fi, bool flag);
	void set_real(double real);
	void set_img(double img);
	double get_real();
	double get_img();
	double get_fi();
	double get_ro();
	string to_string();
	void exp();
	void alg();
	bool operator>(Complex c);
	bool operator>=(Complex c);
	bool operator<(Complex c);
	bool operator<=(Complex c);
	bool operator==(Complex c);
	bool operator!=(Complex c);
	friend Complex operator+(Complex c1, Complex c2);
	friend Complex operator-(Complex c1, Complex c2);
	friend Complex operator*(Complex c1, Complex c2);
	friend Complex operator/(Complex c1, Complex c2);
	friend ostream& operator<<(ostream& os, Complex c);
	friend istream& operator>>(istream& in, Complex &c);
};

#endif