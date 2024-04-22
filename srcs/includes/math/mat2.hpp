#ifndef __MAT2_HPP__
# define __MAT2_HPP__

# include <math.h>
# include <iostream>

namespace math
{

class mat2
{
	public:
		mat2();
		~mat2();
		mat2(const mat2 &src);
		mat2(float diag);

		float*	getValue();

		mat2&	operator=(const mat2 &rhs);
		mat2&	operator=(const float &rhs);
		mat2&	operator=(const float* rhs);
		mat2	operator+(const mat2 &rhs) const;
		mat2	operator+(const float rhs) const;
		mat2	operator-(const mat2 &rhs) const;
		mat2	operator-(const float rhs) const;
		mat2	operator*(const mat2 &rhs) const;
		mat2	operator*(const float rhs) const;
		mat2	operator/(const mat2 &rhs) const;
		mat2	operator/(const float rhs) const;
		mat2&	operator+=(const mat2 &rhs);
		mat2&	operator+=(const float rhs);
		mat2&	operator-=(const mat2 &rhs);
		mat2&	operator-=(const float rhs);
		mat2&	operator*=(const mat2 &rhs);
		mat2&	operator*=(const float rhs);
		mat2&	operator/=(const mat2 &rhs);
		mat2&	operator/=(const float rhs);
		bool	operator==(const mat2 &rhs) const;
		bool	operator!=(const mat2 &rhs) const;
		float*	operator[](const int index);

	private:
		float	value[4][4];
};

std::ostream&	operator<<(std::ostream &o, const mat2 &rhs);
mat2			operator*(const float lhs, const mat2 &rhs);
mat2			operator/(const float lhs, const mat2 &rhs);
mat2			operator+(const float lhs, const mat2 &rhs);
mat2			operator-(const float lhs, const mat2 &rhs);

}

#endif
