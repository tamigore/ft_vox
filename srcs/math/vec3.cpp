#include "../../includes/math/vec3.hpp"

using namespace math;

vec3::vec3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

vec3::vec3(const vec3 &src)
{
	this->x = src.x;
	this->y = src.y;
	this->z = src.z;
}

vec3::vec3(const float &src)
{
	this->x = src;
	this->y = src;
	this->z = src;
}

vec3::vec3(const float &xx, const float &yy, const float &zz)
{
	this->x = xx;
	this->y = yy;
	this->z = zz;
}

vec3::vec3(const float* src)
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	if (!src)
		return ;
	try
	{
		this->x = src[0];
		this->y = src[1];
		this->z = src[2];
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}

vec3::~vec3() {}

float*	vec3::getTab() const
{
	float* tab = new float(3);
	tab[0] = this->x;
	tab[1] = this->y;
	tab[2] = this->z;
	return (tab);
}

vec3&	vec3::operator=(const vec3 &rhs) 
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	return (*this);
}

vec3&	vec3::operator=(const float &rhs) 
{
	this->x = rhs;
	this->y = rhs;
	this->z = rhs;
	return (*this);
}

vec3&	vec3::operator=(const float* rhs) 
{
	if (!rhs)
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
		return (*this);
	}
	this->x = rhs[0];
	this->y = rhs[1];
	this->z = rhs[2];
	return (*this);
}

vec3	vec3::operator+(const vec3 &rhs) const
{
	return (vec3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z));
}

vec3	vec3::operator+(const float rhs) const
{
	return (vec3(this->x + rhs, this->y + rhs, this->z + rhs));
}

vec3	vec3::operator-(const vec3 &rhs) const
{
	return (vec3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z));
}

vec3	vec3::operator-(const float rhs) const
{
	return (vec3(this->x - rhs, this->y - rhs, this->z - rhs));
}

vec3	vec3::operator*(const vec3 &rhs) const
{
	return (vec3(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z));
}

vec3	vec3::operator*(const float rhs) const
{
	return (vec3(this->x * rhs, this->y * rhs, this->z * rhs));
}

vec3	vec3::operator/(const vec3 &rhs) const
{
	return (vec3(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z));
}

vec3	vec3::operator/(const float rhs) const
{
	return (vec3(this->x / rhs, this->y / rhs, this->z / rhs));
}

vec3&	vec3::operator+=(const vec3 &rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	return (*this);
}

vec3&	vec3::operator+=(const float rhs)
{
	this->x += rhs;
	this->y += rhs;
	this->z += rhs;
	return (*this);
}

vec3&	vec3::operator-=(const vec3 &rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	return (*this);
}

vec3&	vec3::operator-=(const float rhs)
{
	this->x -= rhs;
	this->y -= rhs;
	this->z -= rhs;
	return (*this);
}

vec3&	vec3::operator*=(const vec3 &rhs)
{
	this->x *= rhs.x;
	this->y *= rhs.y;
	this->z *= rhs.z;
	return (*this);
}

vec3&	vec3::operator*=(const float rhs)
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;
	return (*this);
}

vec3&	vec3::vec3::operator/=(const vec3 &rhs)
{
	this->x /= rhs.x;
	this->y /= rhs.y;
	this->z /= rhs.z;
	return (*this);
}

vec3&	vec3::operator/=(const float rhs)
{
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;
	return (*this);
}

bool	vec3::operator==(const vec3 &rhs) const
{
	return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
}
bool	vec3::operator!=(const vec3 &rhs) const
{
	return (this->x != rhs.x || this->y != rhs.y || this->z != rhs.z);
}

float&	vec3::operator[](const int index)
{
	if (index == 0)
		return (this->x);
	else if (index == 1)
		return (this->y);
	else if (index == 2)
		return (this->z);
	else
		throw std::out_of_range("vec3 index out of range");
}

float	vec3::magnitude()
{
	return (sqrtf(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2)));
}

void	vec3::normalize()
{
	float mag = this->magnitude();
	this->x = this->x / mag;
	this->y = this->y / mag;
	this->z = this->z / mag;
}

vec3	vec3::cross(const vec3 vec)
{
	return vec3(this->y * vec.z - this->z * vec.y,
		this->z * vec.x - this->x * vec.z,
		this->x * vec.y - this->y * vec.x);
}

float	vec3::dot(const vec3 vec)
{
	return (this->x * vec.x) + (this->y * vec.y) + (this->z * vec.z);
}

float	vec3::angle(const vec3 vec)
{
	vec3 tmp = vec;
	float angle = this->dot(tmp);
	angle /= (this->magnitude() * tmp.magnitude());
	return angle = acosf(angle);
}

vec3	vec3::projection(const vec3 vec)
{
	vec3 tmp = vec;
	vec3 bn = tmp / tmp.magnitude();
	return bn * this->dot(bn);
}

namespace math
{
	std::ostream&	operator<<(std::ostream &out, const vec3 &rhs)
	{
		out << "vec3(" << rhs.x << ", " << rhs.y << ", " << rhs.z << ")";
		return (out);
	}

	vec3			operator*(const float lhs, const vec3 &rhs)
	{
		return (vec3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs));
	}

	vec3			operator/(const float lhs, const vec3 &rhs)
	{
		return (vec3(rhs.x / lhs, rhs.y / lhs, rhs.z / lhs));
	}

	vec3			operator+(const float lhs, const vec3 &rhs)
	{
		return (vec3(rhs.x + lhs, rhs.y + lhs, rhs.z + lhs));
	}

	vec3			operator-(const float lhs, const vec3 &rhs)
	{
		return (vec3(rhs.x - lhs, rhs.y - lhs, rhs.z - lhs));
	}
}