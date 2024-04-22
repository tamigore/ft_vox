#ifndef __VEC2_HPP__
# define __VEC2_HPP__

# include <iostream>
# include <math.h>

namespace math
{

class vec2
{
	public :
		float x;
		float y;

		vec2();
		vec2(const vec2 &src);
		vec2(const float &src);
		vec2(const float &x, const float &y);
		vec2(const float* src);
		~vec2();

		float*	getTab() const;

		vec2&	operator=(const vec2 &rhs);
		vec2&	operator=(const float &rhs);
		vec2&	operator=(const float* rhs);
		vec2	operator+(const vec2 &rhs) const;
		vec2	operator+(const float rhs) const;
		vec2	operator-(const vec2 &rhs) const;
		vec2	operator-(const float rhs) const;
		vec2	operator*(const vec2 &rhs) const;
		vec2	operator*(const float rhs) const;
		vec2	operator/(const vec2 &rhs) const;
		vec2	operator/(const float rhs) const;
		vec2&	operator+=(const vec2 &rhs);
		vec2&	operator+=(const float rhs);
		vec2&	operator-=(const vec2 &rhs);
		vec2&	operator-=(const float rhs);
		vec2&	operator*=(const vec2 &rhs);
		vec2&	operator*=(const float rhs);
		vec2&	operator/=(const vec2 &rhs);
		vec2&	operator/=(const float rhs);
		bool	operator==(const vec2 &rhs) const;
		bool	operator!=(const vec2 &rhs) const;
		float&	operator[](const int index);

		float	magnitude();
		void	normalize();
		float	cross(const vec2 vec);
		float	dot(const vec2 vec);
		float	angle(const vec2 vec);
		vec2	projection(const vec2 vec);
};

std::ostream&	operator<<(std::ostream &o, const vec2 &rhs);
vec2			operator*(const float lhs, const vec2 &rhs);
vec2			operator/(const float lhs, const vec2 &rhs);
vec2			operator+(const float lhs, const vec2 &rhs);
vec2			operator-(const float lhs, const vec2 &rhs);
}

#endif