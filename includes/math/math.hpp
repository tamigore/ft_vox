#ifndef __MATH_HPP__
# define __MATH_HPP__

# include "vec2.hpp"
# include "vec3.hpp"
# include "vec4.hpp"
# include "mat2.hpp"
# include "mat3.hpp"
# include "mat4.hpp"

namespace math
{

double	fade(double t);
double	lerp(double t, double a, double b);
double	grad(int hash, double x, double y, double z);

float	radians(float degrees);	

//mat4
mat4	translate(mat4 &mat, const vec3 &vec);
mat4	rotate(mat4 &mat, float angle, const vec3 &vec);
mat4	scale(mat4 &mat, const vec3 &vec);
mat4	perspective(float fov, float aspect, float near, float far);
mat4	lookAt(math::vec3 position, math::vec3 target, math::vec3 worldUp);

//vec4
float	magnitude(const vec4 vec);
vec4	normalize(const vec4 a);
vec4	cross(const vec4 a, const vec4 b);
float	dot(const vec4 a, const vec4 b);
float	angle(const vec4 a, const vec4 b);
vec4	projection(const vec4 a, const vec4 b);

//vec3
float	magnitude(const vec3 vec);
vec3	normalize(const vec3 a);
vec3	cross(const vec3 a, const vec3 b);
float	dot(const vec3 a, const vec3 b);
float	angle(const vec3 a, const vec3 b);
vec3	projection(const vec3 a, const vec3 b);

//vec2
float	magnitude(const vec2 vec);
vec2	normalize(const vec2 a);
float	cross(const vec2 a, const vec2 b);
float	dot(const vec2 a, const vec2 b);
float	angle(const vec2 a, const vec2 b);
vec2	projection(const vec2 a, const vec2 b);
}

#endif
