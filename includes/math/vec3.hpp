#ifndef __VEC3_HPP__
# define __VEC3_HPP__

# include <iostream>
# include <math.h>

namespace math
{

class vec3
{
	public :
		float x;
		float y;
		float z;

		vec3();
		vec3(const vec3 &src);
		vec3(const float &src);
		vec3(const float &x, const float &y, const float &z);
		vec3(const float* src);
		~vec3();

		float*	getTab() const;

		vec3&	operator=(const vec3 &rhs);
		vec3&	operator=(const float &rhs);
		vec3&	operator=(const float* rhs);
		vec3	operator+(const vec3 &rhs) const;
		vec3	operator+(const float rhs) const;
		vec3	operator-(const vec3 &rhs) const;
		vec3	operator-(const float rhs) const;
		vec3	operator*(const vec3 &rhs) const;
		vec3	operator*(const float rhs) const;
		vec3	operator/(const vec3 &rhs) const;
		vec3	operator/(const float rhs) const;
		vec3&	operator+=(const vec3 &rhs);
		vec3&	operator+=(const float rhs);
		vec3&	operator-=(const vec3 &rhs);
		vec3&	operator-=(const float rhs);
		vec3&	operator*=(const vec3 &rhs);
		vec3&	operator*=(const float rhs);
		vec3&	operator/=(const vec3 &rhs);
		vec3&	operator/=(const float rhs);
		bool	operator==(const vec3 &rhs) const;
		bool	operator!=(const vec3 &rhs) const;
		float&	operator[](const int index);

		float	magnitude();
		void	normalize();
		vec3	cross(const vec3 vec);
		float	dot(const vec3 vec);
		float	angle(const vec3 vec);
		vec3	projection(const vec3 vec);
};

std::ostream&	operator<<(std::ostream &o, const vec3 &rhs);
vec3			operator*(const float lhs, const vec3 &rhs);
vec3			operator/(const float lhs, const vec3 &rhs);
vec3			operator+(const float lhs, const vec3 &rhs);
vec3			operator-(const float lhs, const vec3 &rhs);

// vec3			operator+(const vec3 &rhs, const vec3 &lhs);
// vec3			operator+(const vec3 &rhs, const float lhs);
// vec3			operator-(const vec3 &rhs, const vec3 &lhs);
// vec3			operator-(const vec3 &rhs, const float lhs);
// vec3			operator*(const vec3 &rhs, const vec3 &lhs);
// vec3			operator*(const vec3 &rhs, const float lhs);
// vec3			operator/(const vec3 &rhs, const vec3 &lhs);
// vec3			operator/(const vec3 &rhs, const float lhs);
// vec3			operator+=(const vec3 &rhs, const vec3 &lhs);
// vec3			operator+=(const vec3 &rhs, const float lhs);
// vec3			operator-=(const vec3 &rhs, const vec3 &lhs);
// vec3			operator-=(const vec3 &rhs, const float lhs);
// vec3			operator*=(const vec3 &rhs, const vec3 &lhs);
// vec3			operator*=(const vec3 &rhs, const float lhs);
// vec3			operator/=(const vec3 &rhs, const vec3 &lhs);
// vec3			operator/=(const vec3 &rhs, const float lhs);
// bool			operator==(const vec3 &rhs, const vec3 &lhs);
// bool			operator!=(const vec3 &rhs, const vec3 &lhs);

}

#endif
