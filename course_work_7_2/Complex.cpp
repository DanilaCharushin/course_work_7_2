#include "pch.h"
#include "Complex.h"

Complex::Complex(double ro, double fi, bool flag)
{
	real = ro * cos(fi * 3.1415926 / 180);
	img = ro * sin(fi * 3.1415926 / 180);
}
void Complex::set_real(double real)
{
	this->real = real;
}
void Complex::set_img(double img)
{
	this->img = img;
}
double Complex::get_real()
{
	return real;
}
double Complex::get_img()
{
	return img;
}
double Complex::get_fi()
{
	return atan(img / real) * 180 / 3.1415926;
}
double Complex::get_ro()
{
	return sqrt(real * real + img * img);
}
string Complex::to_string()
{
	string str = "";

	if (img > 0)
	{
		if (real != 0)
		{
			stringstream ss;
			ss << real;
			str += ss.str();
			str += " + ";

			stringstream sss;
			sss << img;
			str += sss.str();
			str += "i";
		}
		else
		{
			stringstream ss;
			ss << img;
			str += ss.str();
			str += "i";
		}
	}
	else 
		if (img < 0)
		{
			if (real != 0)
			{
				stringstream ss;
				ss << real;
				str += ss.str();
				str += " - ";
				stringstream sss;
				sss << -img;
				str += sss.str();
				str += "i";
			}
			else
			{
				stringstream ss;
				ss << img;
				str += ss.str();
				str += "i";
			}
		}
		else
		{
			if (real != 0)
			{
				stringstream ss;
				ss << real;
				str += ss.str();
			}
			else
			{
				str += "<point>";
			}
		}
	return str;
}
void Complex::exp()
{
	cout << get_ro() << "\\_" << get_fi() << endl;
}
void Complex::alg()
{
	cout << *this << endl;
}
bool Complex::operator>(Complex c)
{
	return get_ro() > c.get_ro();
}
bool Complex::operator>=(Complex c)
{
	return get_ro() >= c.get_ro();
}
bool Complex::operator<(Complex c)
{
	return get_ro() < c.get_ro();
}
bool Complex::operator<=(Complex c)
{
	return get_ro() <= c.get_ro();
}
bool Complex::operator==(Complex c)
{
	return get_ro() == c.get_ro();
}
bool Complex::operator!=(Complex c)
{
	return get_ro() != c.get_ro();
}
Complex operator+(Complex c1, Complex c2)
{
	Complex c(c1.real + c2.real, c1.img + c2.img);
	return c;
}
Complex operator-(Complex c1, Complex c2)
{
	Complex c(c1.real - c2.real, c1.img - c2.img);
	return c;
}
Complex operator*(Complex c1, Complex c2)
{
	Complex c(c1.real * c2.real - c1.img * c2.img, c1.real * c2.img + c1.img * c2.real);
	return c;
}
Complex operator/(Complex c1, Complex c2)
{
	if (!c2.get_ro())
		return c1;
	double znam = c2.get_ro() * c2.get_ro();
	Complex c((c1.real * c2.real - c1.img * c2.img) / znam, (c1.real * c2.img + c1.img * c2.real) / znam);
	return c;
}
ostream & operator<<(ostream & os, Complex c)
{
	os << c.to_string();
	return os;
}
istream & operator>>(istream & in, Complex &c)
{
	cout << "Real (a): ";
	in >> c.real;
	cout << "Img (bi): ";
	in >> c.img;
	return in;
}
