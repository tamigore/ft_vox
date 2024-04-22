#include "../../includes/math/math.hpp"

using namespace math;
#define PRECISION 0.000000001


float	math::radians(float degrees)
{
	return degrees * M_PI / 180;
}

//mat4

mat4	math::translate(mat4 &mat, const vec3 &vec)
{
	vec4 x = vec4(mat[0][0], mat[1][0], mat[2][0], mat[3][0]);
	vec4 y = vec4(mat[0][1], mat[1][1], mat[2][1], mat[3][1]);
	vec4 z = vec4(mat[0][2], mat[1][2], mat[2][2], mat[3][2]);
	vec4 w = vec4(mat[0][3], mat[1][3], mat[2][3], mat[3][3]);

	mat[3][0] = dot(x, vec);
	mat[3][1] = dot(y, vec);
	mat[3][2] = dot(z, vec);
	mat[3][3] = dot(w, vec);
	return mat;
}

mat4	math::rotate(mat4 &mat, float angle, const vec3 &vec)
{
	// float rad = radians(angle);
	float c = std::cos(angle);
	float s = std::sin(angle);
	float omc = 1.0f - c;

	mat[0][0] = vec.x * vec.x * omc + c;
	mat[0][1] = vec.x * vec.y * omc - vec.z * s;
	mat[0][2] = vec.x * vec.z * omc + vec.y * s;

	mat[1][0] = vec.y * vec.x * omc + vec.z * s;
	mat[1][1] = vec.y * vec.y * omc + c;
	mat[1][2] = vec.y * vec.z * omc - vec.x * s;

	mat[2][0] = vec.x * vec.z * omc - vec.y * s;
	mat[2][1] = vec.y * vec.z * omc + vec.x * s;
	mat[2][2] = vec.z * vec.z * omc + c;
	return (mat);
}

mat4	math::scale(mat4 &mat, const vec3 &vec)
{
	mat[0][0] *= vec.x;
	mat[1][1] *= vec.y;
	mat[2][2] *= vec.z;
	return (mat);
}

mat4	math::perspective(float fov, float aspect, float near, float far)
{
	mat4 mat(1.0f);
	// float rad = radians(fov);
	float tanHalfFovy = std::tan(fov / 2.0f);
	
	mat[0][0] = 1.0f / (aspect * tanHalfFovy);
	mat[1][1] = 1.0f / (tanHalfFovy);
	mat[2][2] = -(far + near) / (far - near);
	mat[2][3] = -1.0f;
	mat[3][2] = -(2.0f * far * near) / (far - near);
	return (mat);
}

mat4	math::lookAt(vec3 position, vec3 target, vec3 worldUp)
{
	// std::cout << "position: " << position << std::endl;
	// std::cout << "target: " << target << std::endl;
	// std::cout << "worldUp: " << worldUp << std::endl;
    // 1. Position = known
    // 2. Calculate cameraDirection
	// std::cout << position - target << std::endl;
    vec3 zaxis = normalize(position - target);
    // 3. Get positive right axis vector
    vec3 xaxis = normalize(cross(normalize(worldUp), zaxis));
    // 4. Calculate camera up vector
    vec3 yaxis = cross(zaxis, xaxis);

	// std::cout << "xaxis: " << xaxis << std::endl;
	// std::cout << "{" << std::endl;
	// std::cout << "    " << cross(normalize(worldUp), zaxis) << std::endl;
	// std::cout << "    " << normalize(worldUp) << std::endl;
	// std::cout << "}" << std::endl;
	// std::cout << "yaxis: " << yaxis << std::endl;
	// std::cout << "zaxis: " << zaxis << std::endl;
    // Create translation and rotation matrix 
    // In glm we access elements as mat[col][row] due to column-major layout
    mat4 translation = mat4(1.0f); // Identity matrix by default
    translation[3][0] = -position.x; // Fourth column, first row
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
	// std::cout << "translation:\n" << translation << std::endl;

    mat4 rotation = mat4(1.0f);
    rotation[0][0] = xaxis.x; // First column, first row
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x; // First column, second row
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x; // First column, third row
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z;
	// std::cout << "rotation:\n" << rotation << std::endl;

	mat4 lookAt = translation * rotation;
	// std::cout << "lookAt:\n" << lookAt << std::endl;
    // Return lookAt matrix as combination of translation and rotation matrix
    return lookAt; // Remember to read from right to left (first translation then rotation)
}

//vec4

float	math::magnitude(const vec4 vec)
{
	float res = sqrtf(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2));
	if (res <  PRECISION)
		return 0;
	return (res);
}

vec4	math::normalize(const vec4 vec)
{
	vec4 res = vec;
	float mag = magnitude(vec);
	res.x = vec.x / mag;
	res.y = vec.y / mag;
	res.z = vec.z / mag;
	return res;
}

vec4	math::cross(const vec4 a, const vec4 b)
{
	return vec4(a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x,
				1);
}

float	math::dot(const vec4 a, const vec4 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

float	math::angle(const vec4 a, const vec4 b)
{
	float angle = dot(a, b);
	angle /= (magnitude(a) * magnitude(b));
	return angle = acosf(angle);
}

vec4	math::projection(const vec4 a, const vec4 b)
{
	vec4 bn = normalize(b);
	return bn * dot(a, bn);
}

//vec3

float	math::magnitude(const vec3 vec)
{
	float res = sqrtf(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2));
	if (res == 0)
		return (PRECISION);
	return (res);
}

vec3	math::normalize(const vec3 vec)
{
	vec3 res = vec;
	float mag = magnitude(vec);
	res.x = vec.x / mag;
	res.y = vec.y / mag;
	res.z = vec.z / mag;
	return res;
}

vec3	math::cross(const vec3 a, const vec3 b)
{
	return vec3(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

float	math::dot(const vec3 a, const vec3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float	math::angle(const vec3 a, const vec3 b)
{
	float angle = dot(a, b);
	angle /= (magnitude(a) * magnitude(b));
	return angle = acosf(angle);
}

vec3	math::projection(const vec3 a, const vec3 b)
{
	vec3 bn = normalize(b);
	return bn * dot(a, bn);
}

//vec2

float	math::magnitude(const vec2 vec)
{
	float res = sqrtf(powf(vec.x, 2) + powf(vec.y, 2));
	if (res == 0)
		return (PRECISION);
	return (res);
}

vec2	math::normalize(const vec2 a)
{
	float mag = magnitude(a);
	return vec2(a.x / mag, a.y / mag);
}

float	math::cross(const vec2 a, const vec2 b)
{
	return (a.x * b.y - a.y * b.x);
}

float	math::dot(const vec2 a, const vec2 b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float	math::angle(const vec2 a, const vec2 b)
{
	float angle = dot(a, b);
	angle /= (magnitude(a) * magnitude(b));
	return angle = acosf(angle);
}

vec2	math::projection(const vec2 a, const vec2 b)
{
	vec2 bn = b / magnitude(b);
	return bn * dot(a, bn);
}
