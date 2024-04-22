#ifndef __MAT3_HPP__
# define __MAT3_HPP__

# include <math.h>
# include <iostream>

namespace math
{

class mat3
{
	public:
		mat3();
		~mat3();
		mat3(const mat3 &src);
		mat3(float diag);

		mat3&	translate(float x, float y, float z);
		mat3&	rotate(float angle, float x, float y, float z);

		float*	getValue();

		mat3&	operator=(const mat3 &rhs);
		mat3&	operator=(const float &rhs);
		mat3&	operator=(const float* rhs);
		mat3	operator+(const mat3 &rhs) const;
		mat3	operator+(const float rhs) const;
		mat3	operator-(const mat3 &rhs) const;
		mat3	operator-(const float rhs) const;
		mat3	operator*(const mat3 &rhs) const;
		mat3	operator*(const float rhs) const;
		mat3	operator/(const mat3 &rhs) const;
		mat3	operator/(const float rhs) const;
		mat3&	operator+=(const mat3 &rhs);
		mat3&	operator+=(const float rhs);
		mat3&	operator-=(const mat3 &rhs);
		mat3&	operator-=(const float rhs);
		mat3&	operator*=(const mat3 &rhs);
		mat3&	operator*=(const float rhs);
		mat3&	operator/=(const mat3 &rhs);
		mat3&	operator/=(const float rhs);
		bool	operator==(const mat3 &rhs) const;
		bool	operator!=(const mat3 &rhs) const;
		float*	operator[](const int index);

	private:
		float	value[4][4];
};

std::ostream&	operator<<(std::ostream &o, const mat3 &rhs);
mat3			operator*(const float lhs, const mat3 &rhs);
mat3			operator/(const float lhs, const mat3 &rhs);
mat3			operator+(const float lhs, const mat3 &rhs);
mat3			operator-(const float lhs, const mat3 &rhs);

}

#endif
