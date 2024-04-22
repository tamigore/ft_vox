#include "../../includes/math/mat3.hpp"

using namespace math;

mat3::mat3()
{
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			this->value[i][y] = 0;
}

mat3::~mat3()
{
}

mat3::mat3(const mat3 &src)
{
	*this = src;
}

mat3::mat3(float diag) : mat3()
{
	for (int i = 0; i < 3; i++)
		this->value[i][i] = diag;
}

mat3&	mat3::translate(float x, float y, float z)
{
	this->value[2][0] = x;
	this->value[2][1] = y;
	this->value[2][2] = z;
	return (*this);
}

mat3&	mat3::rotate(float angle, float x, float y, float z)
{
	float rad = angle * M_PI / 180;
	float c = std::cos(rad);
	float s = std::sin(rad);
	float omc = 1.0f - c;

	this->value[0][0] = x * x * omc + c;
	this->value[0][1] = x * y * omc - z * s;
	this->value[0][2] = x * z * omc + y * s;

	this->value[1][0] = y * x * omc + z * s;
	this->value[1][1] = y * y * omc + c;
	this->value[1][2] = y * z * omc - x * s;

	this->value[2][0] = x * z * omc - y * s;
	this->value[2][1] = y * z * omc + x * s;
	this->value[2][2] = z * z * omc + c;
	return (*this);
}

float*	mat3::getValue()
{
	return (&this->value[0][0]);
}

mat3&	mat3::operator=(const mat3 &rhs)
{
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			this->value[i][y] = rhs.value[i][y];
	return (*this);
}

mat3&	mat3::operator=(const float &rhs)
{
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			this->value[i][y] = rhs;
	return (*this);
}

mat3&	mat3::operator=(const float* rhs)
{
	if (!rhs)
	{
		for (int i = 0; i < 3; i++)
			for (int y = 0; y < 3; y++)
				this->value[i][y] = 0;
		return (*this);
	}
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			this->value[i][y] = rhs[i * 3 + y];
	return (*this);
}

mat3	mat3::operator+(const mat3 &rhs) const
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				result.value[i][y] = this->value[x][y] + rhs.value[i][x];
	return (result);
}

mat3	mat3::operator+(const float rhs) const
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			result.value[i][y] = this->value[i][y] + rhs;
	return (result);
}

mat3	mat3::operator-(const mat3 &rhs) const
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				result.value[i][y] = this->value[x][y] - rhs.value[i][x];
	return (result);
}

mat3	mat3::operator-(const float rhs) const
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			result.value[i][y] = this->value[i][y] - rhs;
	return (result);
}

mat3	mat3::operator*(const mat3 &rhs) const
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				result.value[i][y] = this->value[x][y] * rhs.value[i][x];
	return (result);
}

mat3	mat3::operator*(const float rhs) const
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			result.value[i][y] = this->value[i][y] * rhs;
	return (result);
}

mat3	mat3::operator/(const mat3 &rhs) const
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				result.value[i][y] = this->value[x][y] * rhs.value[i][x];
	return (result);
}

mat3	mat3::operator/(const float rhs) const
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			result.value[i][y] = this->value[i][y] / rhs;
	return (result);
}

mat3&	mat3::operator+=(const mat3 &rhs)
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				this->value[i][y] += rhs.value[i][x];
	return (*this);
}

mat3&	mat3::operator+=(const float rhs)
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			this->value[i][y] += rhs;
	return (*this);
}

mat3&	mat3::operator-=(const mat3 &rhs)
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				this->value[i][y] -= rhs.value[i][x];
	return (*this);
}

mat3&	mat3::operator-=(const float rhs)
{
	mat3 result;

	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			this->value[i][y] -= rhs;
	return (*this);
}

mat3&	mat3::operator*=(const mat3 &rhs)
{
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				this->value[i][y] *= rhs.value[i][x];
	return (*this);
}

mat3&	mat3::operator*=(const float rhs)
{
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			this->value[i][y] *= rhs;
	return (*this);
}

mat3&	mat3::operator/=(const mat3 &rhs)
{
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				this->value[i][y] /= rhs.value[i][x];
	return (*this);
}

mat3&	mat3::operator/=(const float rhs)
{
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			this->value[i][y] /= rhs;
	return (*this);
}

bool	mat3::operator==(const mat3 &rhs) const
{
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				if (this->value[i][y] != rhs.value[i][x])
					return (false);
	return (true);
}

bool	mat3::operator!=(const mat3 &rhs) const
{
	if (*this == rhs)
		return (false);
	return (true);
}

float*	mat3::operator[](const int index)
{
	return (this->value[index]);
}

std::ostream&	operator<<(std::ostream &o, const mat3 &rhs)
{
	mat3 tmp(rhs);
	for (int i = 0; i < 3; i++)
	{
		o << "| ";
		for (int y = 0; y < 3; y++)
		{
			o << tmp[y][i] << " ";
		}
		o << "|" << std::endl;
	}
	o << std::endl;
	return o;
}
