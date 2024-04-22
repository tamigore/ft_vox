#include "../../includes/math/mat2.hpp"

using namespace math;

mat2::mat2()
{
	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			this->value[i][y] = 0;
}

mat2::~mat2()
{
}

mat2::mat2(const mat2 &src)
{
	*this = src;
}

mat2::mat2(float diag) : mat2()
{
	for (int i = 0; i < 2; i++)
		this->value[i][i] = diag;
}

float*	mat2::getValue()
{
	return (&this->value[0][0]);
}

mat2&	mat2::operator=(const mat2 &rhs)
{
	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			this->value[i][y] = rhs.value[i][y];
	return (*this);
}

mat2&	mat2::operator=(const float &rhs)
{
	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			this->value[i][y] = rhs;
	return (*this);
}

mat2&	mat2::operator=(const float* rhs)
{
	if (!rhs)
	{
		for (int i = 0; i < 2; i++)
			for (int y = 0; y < 2; y++)
				this->value[i][y] = 0;
		return (*this);
	}
	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			this->value[i][y] = rhs[i * 2 + y];
	return (*this);
}

mat2	mat2::operator+(const mat2 &rhs) const
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			for (int x = 0; x < 2; x++)
				result.value[i][y] = this->value[x][y] + rhs.value[i][x];
	return (result);
}

mat2	mat2::operator+(const float rhs) const
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			result.value[i][y] = this->value[i][y] + rhs;
	return (result);
}

mat2	mat2::operator-(const mat2 &rhs) const
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			for (int x = 0; x < 2; x++)
				result.value[i][y] = this->value[x][y] - rhs.value[i][x];
	return (result);
}

mat2	mat2::operator-(const float rhs) const
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			result.value[i][y] = this->value[i][y] - rhs;
	return (result);
}

mat2	mat2::operator*(const mat2 &rhs) const
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			for (int x = 0; x < 2; x++)
				result.value[i][y] = this->value[x][y] * rhs.value[i][x];
	return (result);
}

mat2	mat2::operator*(const float rhs) const
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			result.value[i][y] = this->value[i][y] * rhs;
	return (result);
}

mat2	mat2::operator/(const mat2 &rhs) const
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			for (int x = 0; x < 2; x++)
				result.value[i][y] = this->value[x][y] * rhs.value[i][x];
	return (result);
}

mat2	mat2::operator/(const float rhs) const
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			result.value[i][y] = this->value[i][y] / rhs;
	return (result);
}

mat2&	mat2::operator+=(const mat2 &rhs)
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			for (int x = 0; x < 2; x++)
				this->value[i][y] += rhs.value[i][x];
	return (*this);
}

mat2&	mat2::operator+=(const float rhs)
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			this->value[i][y] += rhs;
	return (*this);
}

mat2&	mat2::operator-=(const mat2 &rhs)
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			for (int x = 0; x < 2; x++)
				this->value[i][y] -= rhs.value[i][x];
	return (*this);
}

mat2&	mat2::operator-=(const float rhs)
{
	mat2 result;

	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			this->value[i][y] -= rhs;
	return (*this);
}

mat2&	mat2::operator*=(const mat2 &rhs)
{
	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			for (int x = 0; x < 2; x++)
				this->value[i][y] *= rhs.value[i][x];
	return (*this);
}

mat2&	mat2::operator*=(const float rhs)
{
	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			this->value[i][y] *= rhs;
	return (*this);
}

mat2&	mat2::operator/=(const mat2 &rhs)
{
	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			for (int x = 0; x < 2; x++)
				this->value[i][y] /= rhs.value[i][x];
	return (*this);
}

mat2&	mat2::operator/=(const float rhs)
{
	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			this->value[i][y] /= rhs;
	return (*this);
}

bool	mat2::operator==(const mat2 &rhs) const
{
	for (int i = 0; i < 2; i++)
		for (int y = 0; y < 2; y++)
			for (int x = 0; x < 2; x++)
				if (this->value[i][y] != rhs.value[i][x])
					return (false);
	return (true);
}

bool	mat2::operator!=(const mat2 &rhs) const
{
	if (*this == rhs)
		return (false);
	return (true);
}

float*	mat2::operator[](const int index)
{
	return (this->value[index]);
}

std::ostream&	operator<<(std::ostream &o, const mat2 &rhs)
{
	mat2 tmp(rhs);
	for (int i = 0; i < 2; i++)
	{
		o << "| ";
		for (int y = 0; y < 2; y++)
		{
			o << tmp[y][i] << " ";
		}
		o << "|" << std::endl;
	}
	o << std::endl;
	return o;
}
