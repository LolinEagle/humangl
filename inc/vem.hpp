#pragma once

#include <cmath>
#include <algorithm>

namespace vem{

class vec2;
class vec3;
class vec4;
class mat3;
class mat4;

class vec2{
public:
	float	x, y;

	vec2(void);
	vec2(float f);
	vec2(float x, float y);

	vec2	operator+(const vec2 &v) const;
	vec2	operator-(const vec2 &v) const;
	vec2	operator*(const vec2 &v) const;
	vec2	operator/(const vec2 &v) const;
	vec2&	operator+=(const vec2 &v);
	vec2&	operator-=(const vec2 &v);
	bool	operator==(const vec2 &v) const;
	float&	operator[](int i);
	const float&	operator[](int i) const;
};

class vec3{
public:
	float	x, y, z;

	vec3(void);
	vec3(float f);
	vec3(float x, float y, float z);
	vec3(const vec4 &v);

	vec3	operator+(const vec3 &v) const;
	vec3	operator-(const vec3 &v) const;
	vec3	operator*(const vec3 &v) const;
	vec3	operator/(const vec3 &v) const;
	vec3&	operator+=(const vec3 &v);
	vec3&	operator-=(const vec3 &v);
	bool	operator==(const vec3 &v) const;
	float&	operator[](int i);
	const float&	operator[](int i) const;
};

class vec4{
public:
	float	x, y, z, w;

	vec4(void);
	vec4(float f);
	vec4(float x, float y, float z, float w);
	vec4(const vec3 &v, float f);

	vec4	operator+(const vec4 &v) const;
	vec4	operator-(const vec4 &v) const;
	vec4	operator*(const vec4 &v) const;
	vec4	operator/(const vec4 &v) const;
	vec4&	operator+=(const vec4 &v);
	vec4&	operator-=(const vec4 &v);
	bool	operator==(const vec4 &v) const;
	float&	operator[](int i);
	const float&	operator[](int i) const;
};

class mat3{
public:
	float	m[3][3];

	mat3(void);
	mat3(float diagonal);
	mat3(const vec3 &v1, const vec3 &v2, const vec3 &v3);

	mat3	operator*(const mat3 &v) const;
};

class mat4{
public:
	float	m[4][4];

	mat4(void);
	mat4(float diagonal);
	mat4(const vec4 &v1, const vec4 &v2, const vec4 &v3, const vec4 &v4);

	mat4	operator*(const mat4 &v) const;
	vec4	operator*(const vec4 &v) const;
	mat4&	operator=(const mat3 &mat);
};

float	dot(const vec2 &v1, const vec2 &v2);
float	dot(const vec3 &v1, const vec3 &v2);
float	dot(const vec4 &v1, const vec4 &v2);

vec2	normalize(const vec2 &v);
vec3	normalize(const vec3 &v);
vec4	normalize(const vec4 &v);

float	mod(float x, float y);
vec2	mod(const vec2 &v, float y);
vec3	mod(const vec3 &v, float y);
vec4	mod(const vec4 &v, float y);

float	radians(float degrees);

vec2	rotate(const vec2 &v, float angle);
vec3	rotate(const vec3 &v, float angle, const vec3 &axis);
vec4	rotate(const vec4 &v, float angle, const vec3 &axis);
mat3	rotate(const mat3 &v, float angle, const vec3 &axis);
mat4	rotate(const mat4 &v, float angle, const vec3 &axis);

float	clamp(float x, float minVal, float maxVal);
vec2	clamp(const vec2 &v, float minVal, float maxVal);
vec3	clamp(const vec3 &v, float minVal, float maxVal);
vec4	clamp(const vec4 &v, float minVal, float maxVal);

}
