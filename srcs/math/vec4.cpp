#include "../../includes/math/vec4.hpp"

using namespace math;

vec4::vec4()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

vec4::vec4(const vec4 &src)
{
	this->x = src.x;
	this->y = src.y;
	this->z = src.z;
	this->w = src.w;
}

vec4::vec4(const vec3 &src)
{
	this->x = src.x;
	this->y = src.y;
	this->z = src.z;
	this->w = 1;
}

vec4::vec4(const float &src)
{
	this->x = src;
	this->y = src;
	this->z = src;
	this->w = src;
}

vec4::vec4(const float &x, const float &y, const float &z, const float &w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

vec4::vec4(const float* src): vec4::vec4()
{
	if (!src)
		return ;
	try
	{
		this->x = src[0];
		this->y = src[1];
		this->z = src[2];
		this->z = src[4];
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}

vec4::~vec4() {}

float*	vec4::getTab() const
{
	float* tab = new float(4);
	tab[0] = this->x;
	tab[1] = this->y;
	tab[2] = this->z;
	tab[3] = this->w;
	return (tab);
}

vec4&	vec4::operator=(const vec4 &rhs) 
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	this->w = rhs.w;
	return (*this);
}

vec4&	vec4::operator=(const float &rhs) 
{
	this->x = rhs;
	this->y = rhs;
	this->z = rhs;
	this->w = rhs;
	return (*this);
}

vec4&	vec4::operator=(const float* rhs) 
{
	if (!rhs)
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
		return (*this);
	}
	this->x = rhs[0];
	this->y = rhs[1];
	this->z = rhs[2];
	this->w = rhs[3];
	return (*this);
}

vec4	vec4::operator+(const vec4 &rhs) const
{
	return (vec4(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w));
}

vec4	vec4::operator+(const float rhs) const
{
	return (vec4(this->x + rhs, this->y + rhs, this->z + rhs, this->w + rhs));
}

vec4	vec4::operator-(const vec4 &rhs) const
{
	return (vec4(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w));
}

vec4	vec4::operator-(const float rhs) const
{
	return (vec4(this->x - rhs, this->y - rhs, this->z - rhs, this->w - rhs));
}

vec4	vec4::operator*(const vec4 &rhs) const
{
	return (vec4(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z, this->w * rhs.w));
}

vec4	vec4::operator*(const float rhs) const
{
	return (vec4(this->x * rhs, this->y * rhs, this->z * rhs, this->w * rhs));
}

vec4	vec4::operator/(const vec4 &rhs) const
{
	return (vec4(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z, this->w / rhs.w));
}

vec4	vec4::operator/(const float rhs) const
{
	return (vec4(this->x / rhs, this->y / rhs, this->z / rhs, this->w / rhs));
}

vec4&	vec4::operator+=(const vec4 &rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	this->w += rhs.w;
	return (*this);
}

vec4&	vec4::operator+=(const float rhs)
{
	this->x += rhs;
	this->y += rhs;
	this->z += rhs;
	this->w += rhs;
	return (*this);
}

vec4&	vec4::operator-=(const vec4 &rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	this->w -= rhs.w;
	return (*this);
}

vec4&	vec4::operator-=(const float rhs)
{
	this->x -= rhs;
	this->y -= rhs;
	this->z -= rhs;
	this->w -= rhs;
	return (*this);
}

vec4&	vec4::operator*=(const vec4 &rhs)
{
	this->x *= rhs.x;
	this->y *= rhs.y;
	this->z *= rhs.z;
	this->w *= rhs.w;
	return (*this);
}

vec4&	vec4::operator*=(const float rhs)
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;
	this->w *= rhs;
	return (*this);
}

vec4&	vec4::vec4::operator/=(const vec4 &rhs)
{
	this->x /= rhs.x;
	this->y /= rhs.y;
	this->z /= rhs.z;
	this->w /= rhs.w;
	return (*this);
}

vec4&	vec4::operator/=(const float rhs)
{
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;
	this->w /= rhs;
	return (*this);
}

bool	vec4::operator==(const vec4 &rhs) const
{
	return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w);
}
bool	vec4::operator!=(const vec4 &rhs) const
{
	return (this->x != rhs.x || this->y != rhs.y || this->z != rhs.z || this->w != rhs.w);
}

float&	vec4::operator[](const int index)
{
	if (index == 0)
		return (this->x);
	else if (index == 1)
		return (this->y);
	else if (index == 2)
		return (this->z);
	else if (index == 3)
		return (this->w);
	else
		throw std::out_of_range("vec4 index out of range");
}

vec4	vec4::normalize()
{
	float mag = vec4::magnitude(*this);
	return vec4(this->x / mag, this->y / mag, this->z / mag, this->w / mag);
}

vec4	vec4::cross(const vec4 a, const vec4 b)
{
	return vec4(a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x,
				1);
}

float	vec4::magnitude(const vec4 vec)
{
	return (sqrtf(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2) + powf(vec.w, 2)));
}

float	vec4::dot(const vec4 a, const vec4 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

float	vec4::angle(const vec4 a, const vec4 b)
{
	float angle = dot(a, b);
	angle /= (magnitude(a) * magnitude(b));
	return angle = acosf(angle);
}

vec4	vec4::projection(const vec4 a, const vec4 b)
{
	vec4 bn = b / magnitude(b);
	return bn * dot(a, bn);
}

namespace math
{
	std::ostream&	operator<<(std::ostream &out, const vec4 &rhs)
	{
		out << "vec4(" << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << ")";
		return (out);
	}

	vec4			operator*(const float lhs, const vec4 &rhs)
	{
		return (vec4(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs, rhs.w * lhs));
	}
	
	vec4			operator/(const float lhs, const vec4 &rhs)
	{
		return (vec4(rhs.x / lhs, rhs.y / lhs, rhs.z / lhs, rhs.w / lhs));
	}

	vec4			operator+(const float lhs, const vec4 &rhs)
	{
		return (vec4(rhs.x + lhs, rhs.y + lhs, rhs.z + lhs, rhs.w + lhs));
	}

	vec4			operator-(const float lhs, const vec4 &rhs)
	{
		return (vec4(rhs.x - lhs, rhs.y - lhs, rhs.z - lhs, rhs.w - lhs));
	}
}
