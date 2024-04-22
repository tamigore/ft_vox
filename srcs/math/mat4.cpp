#include "../../includes/math/mat4.hpp"

using namespace math;

mat4::mat4()
{
	for (int i = 0; i < 4; i++)
		for (int y = 0; y < 4; y++)
			this->value[i][y] = 0;
}

mat4::~mat4()
{
}

mat4::mat4(const mat4 &src)
{
	*this = src;
}

mat4::mat4(float diag) : mat4()
{
	for (int i = 0; i < 4; i++)
		this->value[i][i] = diag;
}

mat4&	mat4::translate(float x, float y, float z)
{
	this->value[3][0] = x;
	this->value[3][1] = y;
	this->value[3][2] = z;
	return (*this);
}

mat4&	mat4::rotate(float angle, float x, float y, float z)
{
	float c = std::cos(angle);
	float s = std::sin(angle);
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

mat4&	mat4::rotate(float angle, const vec3 &vec)
{
	float c = std::cos(angle);
	float s = std::sin(angle);
	float omc = 1.0f - c;

	this->value[0][0] = vec.x * vec.x * omc + c;
	this->value[0][1] = vec.x * vec.y * omc - vec.z * s;
	this->value[0][2] = vec.x * vec.z * omc + vec.y * s;

	this->value[1][0] = vec.y * vec.x * omc + vec.z * s;
	this->value[1][1] = vec.y * vec.y * omc + c;
	this->value[1][2] = vec.y * vec.z * omc - vec.x * s;

	this->value[2][0] = vec.x * vec.z * omc - vec.y * s;
	this->value[2][1] = vec.y * vec.z * omc + vec.x * s;
	this->value[2][2] = vec.z * vec.z * omc + c;
	return (*this);
}

mat4&	mat4::scale(const vec3 &vec)
{
	this->value[0][0] *= vec.x;
	this->value[1][1] *= vec.y;
	this->value[2][2] *= vec.z;
	return (*this);
}

mat4&	mat4::perspective(float fov, float aspect, float near, float far)
{
	float tanHalfFovy = std::tan(fov / 2.0f);
	
	this->value[0][0] = 1.0f / (aspect * tanHalfFovy);
	this->value[1][1] = 1.0f / (tanHalfFovy);
	this->value[2][2] = -(far + near) / (far - near);
	this->value[2][3] = -1.0f;
	this->value[3][2] = -(2.0f * far * near) / (far - near);
	return (*this);
}

float*	mat4::getValue()
{
	return (&this->value[0][0]);
}

mat4&	mat4::operator=(const mat4 &rhs)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			this->value[x][y] = rhs.value[x][y];
	return (*this);
}

mat4&	mat4::operator=(const float &rhs)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			this->value[x][y] = rhs;
	return (*this);
}

mat4&	mat4::operator=(const float* rhs)
{
	if (!rhs)
	{
		for (int x = 0; x < 4; x++)
			for (int y = 0; y < 4; y++)
				this->value[x][y] = 0;
		return (*this);
	}
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			this->value[x][y] = rhs[x * 4 + y];
	return (*this);
}

mat4	mat4::operator+(const mat4 &rhs) const
{
	mat4 result;

	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			result.value[x][y] = this->value[x][y] + rhs.value[x][y];
	return (result);
}

mat4	mat4::operator+(const float rhs) const
{
	mat4 result;

	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			result.value[x][y] = this->value[x][y] + rhs;
	return (result);
}

mat4	mat4::operator-(const mat4 &rhs) const
{
	mat4 result;

	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			result.value[x][y] = this->value[x][y] - rhs.value[x][y];
	return (result);
}

mat4	mat4::operator-(const float rhs) const
{
	mat4 result;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			result.value[x][y] = this->value[x][y] - rhs;
	return (result);
}

mat4	mat4::operator*(const mat4 &rhs) const
{
    mat4    mat;
    mat4    tmp(rhs);
    int        x;
    int        y;

    y = 0;
    while (y < 4)
    {
        x = 0;
        while (x < 4)
        {
            mat[y][x] = this->value[y][0] * tmp[0][x]
                + this->value[y][1] * tmp[1][x]
                + this->value[y][2] * tmp[2][x]
                + this->value[y][3] * tmp[3][x];
            x++;
        }
        y++;
    }
    return (mat);
}

vec3	mat4::operator*(const vec3 &rhs) const
{
	vec3	tmp(rhs);
	vec3	res;
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			res[x] += this->value[x][y] * tmp[y];
	return (res);
}

vec4	mat4::operator*(const vec4 &rhs) const
{
	vec4	tmp(rhs);
	vec4	res;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			res[x] += this->value[x][y] * tmp[y];
	return (res);
}

mat4	mat4::operator*(const float rhs) const
{
	mat4 result;

	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			result.value[x][y] = this->value[x][y] * rhs;
	return (result);
}

mat4	mat4::operator/(const mat4 &rhs) const
{
	mat4 result;

	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
				result.value[x][y] = this->value[x][y] * rhs.value[x][y];
	return (result);
}

mat4	mat4::operator/(const float rhs) const
{
	mat4 result;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			result.value[x][y] = this->value[x][y] / rhs;
	return (result);
}

mat4&	mat4::operator+=(const mat4 &rhs)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
				this->value[x][y] += rhs.value[x][y];
	return (*this);
}

mat4&	mat4::operator+=(const float rhs)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			this->value[x][y] += rhs;
	return (*this);
}

mat4&	mat4::operator-=(const mat4 &rhs)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
				this->value[x][y] -= rhs.value[x][y];
	return (*this);
}

mat4&	mat4::operator-=(const float rhs)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			this->value[x][y] -= rhs;
	return (*this);
}

mat4&	mat4::operator*=(const mat4 &rhs)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			this->value[x][y] *= rhs.value[x][x];
	return (*this);
}

mat4&	mat4::operator*=(const float rhs)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			this->value[x][y] *= rhs;
	return (*this);
}

mat4&	mat4::operator/=(const mat4 &rhs)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
				this->value[x][y] /= rhs.value[x][y];
	return (*this);
}

mat4&	mat4::operator/=(const float rhs)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			this->value[x][y] /= rhs;
	return (*this);
}

bool	mat4::operator==(const mat4 &rhs) const
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			if (this->value[x][y] != rhs.value[x][y])
				return (false);
	return (true);
}

bool	mat4::operator!=(const mat4 &rhs) const
{
	if (*this == rhs)
		return (false);
	return (true);
}

float*	mat4::operator[](const int index)
{
	return (this->value[index]);
}

namespace math
{
	std::ostream&	operator<<(std::ostream &o, const mat4 &rhs)
	{
		mat4 tmp(rhs);
		for (int i = 0; i < 4; i++)
		{
			o << "| ";
			for (int y = 0; y < 4; y++)
			{
				o << tmp[y][i] << " ";
			}
			o << "|" << std::endl;
		}
		o << std::endl;
		return o;
	}

	// mat4	operator+(const mat4 &rhs, const mat4 &lhs)
	// {
	// 	mat4 result;
	// 	mat4 l(lhs);
	// 	mat4 r(rhs);
	// 	for (int x = 0; x < 4; x++)
	// 		for (int y = 0; y < 4; y++)
	// 			result[x][y] = r[x][y] + l[x][y];
	// 	return (result);
	// }

	// mat4	operator+(const mat4 &rhs, const float lhs)
	// {
	// 	mat4 result;
	// 	mat4 r(rhs);
	// 	for (int x = 0; x < 4; x++)
	// 		for (int y = 0; y < 4; y++)
	// 			result[x][y] = r[x][y] + lhs;
	// 	return (result);
	// }

	// mat4	operator-(const mat4 &rhs, const mat4 &lhs)
	// {
	// 	mat4 result;
	// 	mat4 l(lhs);
	// 	mat4 r(rhs);
	// 	for (int x = 0; x < 4; x++)
	// 		for (int y = 0; y < 4; y++)
	// 			result[x][y] = r[x][y] - l[x][y];
	// 	return (result);
	// }

	// mat4	operator-(const mat4 &rhs, const float lhs)
	// {
	// 	mat4 result;
	// 	mat4 r(rhs);
	// 	for (int x = 0; x < 4; x++)
	// 		for (int y = 0; y < 4; y++)
	// 			result[x][y] = r[x][y] - lhs;
	// 	return (result);
	// }

	// mat4	operator*(const mat4 &rhs, const mat4 &lhs)
	// {
	// 	mat4	result;
	// 	mat4	r(rhs);
	// 	mat4	l(lhs);
	// 	for (int y = 0; y < 4; y++)
	// 	{
	// 		for (int x = 0; x < 4; x++)
	// 		{
	// 			result[y][x] = r[y][0] * l[0][x]
	// 				+ r[y][1] * l[1][x]
	// 				+ r[y][2] * l[2][x]
	// 				+ r[y][3] * l[3][x];
	// 		}
	// 	}
	// 	return (result);
	// }

	// vec3	operator*(const mat4 &rhs, const vec3 &lhs)
	// {
	// 	vec3	result;
	// 	mat4	r(rhs);
	// 	vec3	l(lhs);
	// 	for (int x = 0; x < 3; x++)
	// 		for (int y = 0; y < 3; y++)
	// 			result[x] += r[x][y] * l[y];
	// 	return (result);
	// }

	// vec4	operator*(const mat4 &rhs, const vec4 &lhs)
	// {
	// 	vec4	result;
	// 	mat4	r(rhs);
	// 	vec4	l(lhs);
	// 	for (int x = 0; x < 4; x++)
	// 		for (int y = 0; y < 4; y++)
	// 			result[x] += r[x][y] * l[y];
	// 	return (result);
	// }

	// mat4	operator*(const mat4 &rhs, const float lhs)
	// {
	// 	mat4	result;
	// 	mat4	r(rhs);
	// 	for (int x = 0; x < 4; x++)
	// 		for (int y = 0; y < 4; y++)
	// 			result[x][y] = r[x][y] * lhs;
	// 	return (result);
	// }

	// mat4	operator/(const mat4 &rhs, const mat4 &lhs)
	// {
	// 	mat4	result;
	// 	mat4	r(rhs);
	// 	mat4	l(lhs);
	// 	for (int x = 0; x < 4; x++)
	// 		for (int y = 0; y < 4; y++)
	// 				result[x][y] = r[x][y] / l[x][x];
	// 	return (result);
	// }

	// mat4	operator/(const mat4 &rhs, const float lhs)
	// {
	// 	mat4	result;
	// 	mat4	r(rhs);
	// 	for (int x = 0; x < 4; x++)
	// 		for (int y = 0; y < 4; y++)
	// 			result[x][y] = r[x][y] / lhs;
	// 	return (result);
	// }

	// mat4	operator+=(const mat4 &rhs, const mat4 &lhs)
	// {
	// 	mat4	result = rhs + lhs;
	// 	return (result);
	// }

	// mat4	operator+=(const mat4 &rhs, const float lhs)
	// {
	// 	mat4	result = rhs + lhs;
	// 	return (result);
	// }

	// mat4	operator-=(const mat4 &rhs, const mat4 &lhs)
	// {
	// 	mat4	result = rhs - lhs;
	// 	return (result);
	// }

	// mat4	operator-=(const mat4 &rhs, const float lhs)
	// {
	// 	mat4	result = rhs - lhs;
	// 	return (result);
	// }

	// mat4	operator*=(const mat4 &rhs, const mat4 &lhs)
	// {
	// 	mat4	result = rhs * lhs;
	// 	return (result);
	// }

	// mat4	operator*=(const mat4 &rhs, const float lhs)
	// {
	// 	mat4	result = rhs * lhs;
	// 	return (result);
	// }

	// mat4	operator/=(const mat4 &rhs, const mat4 &lhs)
	// {
	// 	mat4	result = rhs / lhs;
	// 	return (result);
	// }

	// mat4	operator/=(const mat4 &rhs, const float lhs)
	// {
	// 	mat4	result = rhs / lhs;
	// 	return (result);
	// }

	// bool	operator==(const mat4 &rhs, const mat4 &lhs)
	// {
	// 	mat4	r(rhs);
	// 	mat4	l(lhs);
	// 	for (int y = 0; y < 4; y++)
	// 		for (int x = 0; x < 4; x++)
	// 			if (r[x][y] != l[x][y])
	// 				return (false);
	// 	return (true);
	// }

	// bool	operator!=(const mat4 &rhs, const mat4 &lhs)
	// {
	// 	if (rhs == lhs)
	// 		return (false);
	// 	return (true);
	// }
}
