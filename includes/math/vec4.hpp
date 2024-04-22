#ifndef __VEC4_HPP__
# define __VEC4_HPP__

# include <iostream>
# include <math.h>
# include "vec3.hpp"

namespace math
{

class vec4
{
	public :
		float x;
		float y;
		float z;
		float w;

		vec4();
		vec4(const vec4 &src);
		vec4(const vec3 &src);
		vec4(const float &src);
		vec4(const float &x, const float &y, const float &z, const float &w);
		vec4(const float* src);
		~vec4();

		float*	getTab() const;

		vec4&	operator=(const vec4 &rhs);
		vec4&	operator=(const float &rhs);
		vec4&	operator=(const float* rhs);
		vec4	operator+(const vec4 &rhs) const;
		vec4	operator+(const float rhs) const;
		vec4	operator-(const vec4 &rhs) const;
		vec4	operator-(const float rhs) const;
		vec4	operator*(const vec4 &rhs) const;
		vec4	operator*(const float rhs) const;
		vec4	operator/(const vec4 &rhs) const;
		vec4	operator/(const float rhs) const;
		vec4&	operator+=(const vec4 &rhs);
		vec4&	operator+=(const float rhs);
		vec4&	operator-=(const vec4 &rhs);
		vec4&	operator-=(const float rhs);
		vec4&	operator*=(const vec4 &rhs);
		vec4&	operator*=(const float rhs);
		vec4&	operator/=(const vec4 &rhs);
		vec4&	operator/=(const float rhs);
		bool	operator==(const vec4 &rhs) const;
		bool	operator!=(const vec4 &rhs) const;
		float&	operator[](const int index);

		vec4	normalize();
		vec4	cross(const vec4 a, const vec4 b);
		float	magnitude(const vec4 vec);
		float	dot(const vec4 a, const vec4 b);
		float	angle(const vec4 a, const vec4 b);
		vec4	projection(const vec4 a, const vec4 b);
};

std::ostream&	operator<<(std::ostream &o, const vec4 &rhs);
vec4			operator*(const float lhs, const vec4 &rhs);
vec4			operator/(const float lhs, const vec4 &rhs);
vec4			operator+(const float lhs, const vec4 &rhs);
vec4			operator-(const float lhs, const vec4 &rhs);
}

#endif
