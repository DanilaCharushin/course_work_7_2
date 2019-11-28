#include "pch.h"
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

#ifndef __MATRIX_H__
#define __MATRIX_H__

const int MAX_M = 9;
const int MIN_M = 0;

class Matrix
{
private:
	double **value;
	int N;
	int M;
public:
	Matrix(int N = 0, int M = 0, double = 0);
	Matrix(const Matrix &m);
	Matrix & operator=(const Matrix &m);
	~Matrix();
	void fill_const(double value);
	void fill_random(int min = MIN_M, int max = MAX_M);
	double & get_value(int i = 0, int j = 0);
	long double determinant();
	int get_N();
	int get_M();
	Matrix strike_out(int i, int j);
	Matrix inverse();
	Matrix transposed();
	Matrix & operator*(double value);
	Matrix & operator/(double value);
	friend Matrix operator+(Matrix m1, Matrix m2);
	friend Matrix operator-(Matrix m1, Matrix m2);
	friend Matrix operator*(Matrix m1, Matrix m2);
	friend ostream& operator<<(ostream& os, Matrix m);
	friend istream& operator>>(istream& in, Matrix &m);
};

#endif