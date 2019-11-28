#include "pch.h"
#include "Matrix.h"

Matrix::Matrix(int N, int M, double value) : N(N), M(M)
{
	this->value = new double*[N];
	for (int i = 0; i < N; i++)
		this->value[i] = new double[M];
	if (value == 0)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				if (i == j)
					this->value[i][j] = 1;
				else
					this->value[i][j] = 0;
	else
		fill_const(value);
}
Matrix::Matrix(const Matrix & m) : N(m.N), M(m.M)
{
	value = new double*[m.N];
	for (int i = 0; i < m.N; i++)
	{
		value[i] = new double[m.M];
		for (int j = 0; j < m.M; j++)
			value[i][j] = m.value[i][j];
	}
}
Matrix & Matrix::operator=(const Matrix & m)
{
	for (int i = 0; i < N; i++)
		if (value[i])
			delete[] value[i];
	if (value)
		delete[] value;

	N = m.N;
	M = m.M;
	value = new double*[N];
	for (int i = 0; i < N; i++)
	{
		value[i] = new double[M];
		for (int j = 0; j < M; j++)
			value[i][j] = m.value[i][j];
	}
	return *this;
}
Matrix::~Matrix()
{
	for (int i = 0; i < N; i++)
		if (value[i])
			delete[] value[i];
	if (value)
		delete[] value;
}
void Matrix::fill_const(double value)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			this->value[i][j] = value;
}
void Matrix::fill_random(int min, int max)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			value[i][j] = rand() % (max - min + 1) + min;
}
double & Matrix::get_value(int i, int j)
{
	return value[i][j];
}
long double Matrix::determinant()
{
	if (N != M)
		return NAN;

	if (N == 2)
		return value[0][0] * value[1][1] - value[0][1] * value[1][0];

	if (N == 3)
		return value[0][0] * (value[1][1] * value[2][2] - value[1][2] * value[2][1])
			 - value[0][1] * (value[1][0] * value[2][2] - value[1][2] * value[2][0])
			 + value[0][2] * (value[1][0] * value[2][1] - value[1][1] * value[2][0]);


	double **A = new double*[N];
	for (int i = 0; i < N; i++)
	{
		A[i] = new double[M];
		for (int j = 0; j < M; j++)
			A[i][j] = value[i][j];
	}
	long double value = 1;
	double koef = 0;
	int strok = 0;
	for (int i = 0, j = 0; i < N; i++, j++)
	{
		if (A[i][j] == 0)
		{
			strok = i;
			while (A[strok][j] == 0)
			{
				strok++;
				if (strok == N)
					return 0;
			}
			for (int stolb = j; stolb < N; stolb++)
				A[i][stolb] += A[strok][stolb];
		}
		if (A[i][j] != 0)
			value *= A[i][j];

		for (int stolb = j + 1; stolb < N; stolb++)
			A[i][stolb] /= A[i][j];

		for (int stroka = i + 1; stroka < N; stroka++)
		{
			koef = A[stroka][j];
			A[stroka][j] = 0;
			for (int stolb = j + 1; stolb < N; stolb++)
				A[stroka][stolb] -= (A[i][stolb] * koef);
		}
	}

	for (int i = 0; i < N; i++)
		if (A[i])
			delete[] A[i];
	if (A)
		delete[] A;

	return value;
}
int Matrix::get_N()
{
	return N;
}
int Matrix::get_M()
{
	return M;
}
Matrix Matrix::strike_out(int I, int J)
{
	if (I >= N || J >= M || J < 0 || I < 0)
		return Matrix();
	int n = N - 1;
	int m = M - 1;
	int iN = 0;
	int iM = 0;
	Matrix res(n, m);
	for (int i = 0; i < n; i++)
	{
		if (i == I)
			iN = 1;
		iM = 0;
		for (int j = 0; j < m; j++)
		{
			if (j == J)
				iM = 1;
			res.value[i][j] = value[i + iN][j + iM];
		}
	}
	return res;
}
Matrix Matrix::inverse()
{
	if (N != M)
		return NAN;

	Matrix res(N, M);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			res.value[i][j] = strike_out(i, j).determinant() * pow(-1, i + j);
	return res.transposed() / determinant();

}
Matrix Matrix::transposed()
{
	Matrix res(M, N);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			res.value[j][i] = value[i][j];
	return res;
}
Matrix & Matrix::operator*(double value)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			this->value[i][j] *= value;
	return *this;
}
Matrix & Matrix::operator/(double value)
{
	if (value == 0)
		return *this;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			this->value[i][j] /= value;
	return *this;
}
Matrix operator+(Matrix m1, Matrix m2)
{
	if (m1.N == m2.N && m1.M == m2.M)
	{
		int N = m1.N;
		int M = m2.M;
		Matrix res(N, M);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				res.value[i][j] = m1.value[i][j] + m2.value[i][j];
		return res;
	}
	else
		return NAN;
}
Matrix operator-(Matrix m1, Matrix m2)
{
	if (m1.N == m2.N && m1.M == m2.M)
	{
		int N = m1.N;
		int M = m2.M;
		Matrix res(N, M);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				res.value[i][j] = m1.value[i][j] - m2.value[i][j];
		return res;
	}
	else 
		return NAN;
}
Matrix operator*(Matrix m1, Matrix m2)
{
	if (m1.M == m2.N)
	{
		int N = m1.N;
		int M = m2.M;
		Matrix res(N, M);
		res.fill_const(0);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				for (int k = 0; k < m1.M; k++)
					res.value[i][j] += m1.value[i][k] * m2.value[k][j];
		return res;
	}
	else
		return NAN;
}
ostream & operator<<(ostream & os, Matrix m)
{
	for (int i = 0; i < m.N; i++)
	{
		for (int j = 0; j < m.M; j++)
			os << "\t" << m.value[i][j];
		os << endl;
	}
	return os;
}
istream & operator>>(istream & in, Matrix &m)
{
	for (int i = 0; i < m.N; i++)
		for (int j = 0; j < m.M; j++)
			in >> m.value[i][j];
	return in;
}
