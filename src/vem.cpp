#include <vem.hpp>

namespace vem{

vec2::vec2(void) : x(0), y(0){
}
vec2::vec2(float f) : x(f), y(f){
}
vec2::vec2(float x, float y) : x(x), y(y){
}

vec2	vec2::operator+(const vec2 &v) const{
	return (vec2(x + v.x, y + v.y));
}
vec2	vec2::operator-(const vec2 &v) const{
	return (vec2(x - v.x, y - v.y));
}
vec2	vec2::operator*(const vec2 &v) const{
	return (vec2(x * v.x, y * v.y));
}
vec2	vec2::operator/(const vec2 &v) const{
	return (vec2(x / v.x, y / v.y));
}
vec2&	vec2::operator+=(const vec2 &v){
	x += v.x; y += v.y;
	return (*this);
}
vec2&	vec2::operator-=(const vec2 &v){
	x -= v.x; y -= v.y;
	return (*this);
}
bool	vec2::operator==(const vec2 &v) const{
	return (x == v.x && y == v.y);
}
float&	vec2::operator[](int i){
	return ((i == 0) ? x : y);
}
const float&	vec2::operator[](int i) const{
	return ((i == 0) ? x : y);
}

vec3::vec3(void) : x(0), y(0), z(0){
}
vec3::vec3(float f) : x(f), y(f), z(f){
}
vec3::vec3(float x, float y, float z) : x(x), y(y), z(z){
}
vec3::vec3(const vec4 &v) : x(v.x), y(v.y), z(v.z){
}

vec3	vec3::operator+(const vec3 &v) const{
	return (vec3(x + v.x, y + v.y, z + v.z));
}
vec3	vec3::operator-(const vec3 &v) const{
	return (vec3(x - v.x, y - v.y, z - v.z));
}
vec3	vec3::operator*(const vec3 &v) const{
	return (vec3(x * v.x, y * v.y, z * v.z));
}
vec3	vec3::operator/(const vec3 &v) const{
	return (vec3(x / v.x, y / v.y, z / v.z));
}
vec3&	vec3::operator+=(const vec3 &v){
	x += v.x; y += v.y; z += v.z;
	return (*this);
}
vec3&	vec3::operator-=(const vec3 &v){
	x -= v.x; y -= v.y; z -= v.z;
	return (*this);
}
bool	vec3::operator==(const vec3 &v) const{
	return (x == v.x && y == v.y && z == v.z);
}
float&	vec3::operator[](int i){
	if (i == 0) return (x);
	else if (i == 1) return (y);
	else return (z);
}
const float&	vec3::operator[](int i) const{
	if (i == 0) return (x);
	else if (i == 1) return (y);
	else return (z);
}

vec4::vec4(void) : x(0), y(0), z(0), w(0){
}
vec4::vec4(float f) : x(f), y(f), z(f), w(f){
}
vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){
}
vec4::vec4(const vec3 &v, float f) : x(v.x), y(v.y), z(v.z), w(f){
}

vec4	vec4::operator+(const vec4 &v) const{
	return (vec4(x + v.x, y + v.y, z + v.z, w + v.w));
}
vec4	vec4::operator-(const vec4 &v) const{
	return (vec4(x - v.x, y - v.y, z - v.z, w - v.w));
}
vec4	vec4::operator*(const vec4 &v) const{
	return (vec4(x * v.x, y * v.y, z * v.z, w * v.w));
}
vec4	vec4::operator/(const vec4 &v) const{
	return (vec4(x / v.x, y / v.y, z / v.z, w / v.w));
}
vec4&	vec4::operator+=(const vec4 &v){
	x += v.x; y += v.y; z += v.z; w += v.w;
	return (*this);
}
vec4&	vec4::operator-=(const vec4 &v){
	x -= v.x; y -= v.y; z -= v.z; w -= v.w;
	return (*this);
}
bool	vec4::operator==(const vec4 &v) const{
	return (x == v.x && y == v.y && z == v.z && w == v.w);
}
float&	vec4::operator[](int i){
	if (i == 0) return (x);
	else if (i == 1) return (y);
	else if (i == 2) return (z);
	else return (w);
}
const float&	vec4::operator[](int i) const{
	if (i == 0) return (x);
	else if (i == 1) return (y);
	else if (i == 2) return (z);
	else return (w);
}

mat3::mat3(void){
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			m[i][j] = 0.f;
}
mat3::mat3(float diagonal){
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			m[i][j] = (i == j) ? diagonal : 0.f;
}
mat3::mat3(const vec3 &v1, const vec3 &v2, const vec3 &v3){
	m[0][0] = v1.x; m[0][1] = v1.y; m[0][2] = v1.z;
	m[1][0] = v2.x; m[1][1] = v2.y; m[1][2] = v2.z;
	m[2][0] = v3.x; m[2][1] = v3.y; m[2][2] = v3.z;
}

mat3	mat3::operator*(const mat3 &v) const{
	mat3	result;
	for (int i = 0; i < 3; ++i){
		for (int j = 0; j < 3; ++j){
			result.m[i][j] = 0;
			for (int k = 0; k < 3; ++k)
				result.m[i][j] += m[i][k] * v.m[k][j];
		}
	}
	return (result);
}

mat4::mat4(void){
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = 0.f;
}
mat4::mat4(float diagonal){
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = (i == j) ? diagonal : 0.f;
}
mat4::mat4(const vec4 &v1, const vec4 &v2, const vec4 &v3, const vec4 &v4){
	m[0][0] = v1.x; m[0][1] = v1.y; m[0][2] = v1.z; m[0][3] = v1.w;
	m[1][0] = v2.x; m[1][1] = v2.y; m[1][2] = v2.z; m[1][3] = v2.w;
	m[2][0] = v3.x; m[2][1] = v3.y; m[2][2] = v3.z; m[2][3] = v3.w;
	m[3][0] = v4.x; m[3][1] = v4.y; m[3][2] = v4.z; m[3][3] = v4.w;
}

mat4	mat4::operator*(const mat4 &v) const{
	mat4	result;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			for (int k = 0; k < 4; ++k)
				result.m[i][j] += m[i][k] * v.m[k][j];
	return (result);
}
vec4	mat4::operator*(const vec4 &v) const{
	return (vec4(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
		m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
	));
}
mat4&	mat4::operator=(const mat3 &mat){
	for (int i = 0; i < 3; ++i){
		for (int j = 0; j < 3; ++j)
			m[i][j] = mat.m[i][j];
		m[i][3] = 0.f;
	}
	for (int j = 0; j < 4; ++j)
		m[3][j] = (j == 3) ? 1.f : 0.f;
	return *this;
}

float	dot(const vec2 &v1, const vec2 &v2){
	return (v1.x * v2.x + v1.y * v2.y);
}
float	dot(const vec3 &v1, const vec3 &v2){
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}
float	dot(const vec4 &v1, const vec4 &v2){
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

vec2	normalize(const vec2 &v){
	float length = std::sqrt(v.x * v.x + v.y * v.y);
	return (vec2(v.x / length, v.y / length));
}
vec3	normalize(const vec3 &v){
	float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return (vec3(v.x / length, v.y / length, v.z / length));
}
vec4	normalize(const vec4 &v){
	float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	return (vec4(v.x / length, v.y / length, v.z / length, v.w / length));
}

float	mod(float x, float y){
	return (std::fmod(x, y));
}
vec2	mod(const vec2 &v, float y){
	return (vec2(std::fmod(v.x, y), std::fmod(v.y, y)));
}
vec3	mod(const vec3 &v, float y){
	return (vec3(std::fmod(v.x, y), std::fmod(v.y, y), std::fmod(v.z, y)));
}
vec4	mod(const vec4 &v, float y){
	return (vec4(std::fmod(v.x, y), std::fmod(v.y, y), std::fmod(v.z, y), std::fmod(v.w, y)));
}

float	radians(float degrees){
	return (degrees * (M_PI / 180.0f));
}

vec2	rotate(const vec2 &v, float angle){
	float	rad = radians(angle);
	float	cos_a = std::cos(rad);
	float	sin_a = std::sin(rad);
	return (vec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a));
}
vec3	rotate(const vec3 &v, float angle, const vec3 &axis){
	float	rad = radians(angle);
	float	cos_a = std::cos(rad);
	float	sin_a = std::sin(rad);
	float	one_minus_cos = 1.f - cos_a;

	vec3	norm_axis = normalize(axis);
	float	x = norm_axis.x;
	float	y = norm_axis.y;
	float	z = norm_axis.z;

	vec3	result;
	result.x = (cos_a + x * x * one_minus_cos) * v.x +
			   (x * y * one_minus_cos - z * sin_a) * v.y +
			   (x * z * one_minus_cos + y * sin_a) * v.z;
	result.y = (y * x * one_minus_cos + z * sin_a) * v.x +
			   (cos_a + y * y * one_minus_cos) * v.y +
			   (y * z * one_minus_cos - x * sin_a) * v.z;
	result.z = (z * x * one_minus_cos - y * sin_a) * v.x +
			   (z * y * one_minus_cos + x * sin_a) * v.y +
			   (cos_a + z * z * one_minus_cos) * v.z;
	return (result);
}
vec4 rotate(const vec4 &v, float angle, const vec3 &axis){
	vec3	rotated = rotate(vec3(v.x, v.y, v.z), angle, axis);
	return (vec4(rotated.x, rotated.y, rotated.z, v.w));
}
mat3 rotate(const mat3 &v, float angle, const vec3 &axis){
	float	rad = radians(angle);
	float	cos_a = std::cos(rad);
	float	sin_a = std::sin(rad);
	float	one_minus_cos = 1.f - cos_a;

	vec3	norm_axis = normalize(axis);
	float	x = norm_axis.x;
	float	y = norm_axis.y;
	float	z = norm_axis.z;

	mat3	rotation(1.f);
	rotation.m[0][0] = cos_a + x * x * one_minus_cos;
	rotation.m[0][1] = x * y * one_minus_cos - z * sin_a;
	rotation.m[0][2] = x * z * one_minus_cos + y * sin_a;
	rotation.m[1][0] = y * x * one_minus_cos + z * sin_a;
	rotation.m[1][1] = cos_a + y * y * one_minus_cos;
	rotation.m[1][2] = y * z * one_minus_cos - x * sin_a;
	rotation.m[2][0] = z * x * one_minus_cos - y * sin_a;
	rotation.m[2][1] = z * y * one_minus_cos + x * sin_a;
	rotation.m[2][2] = cos_a + z * z * one_minus_cos;
	return (v * rotation);
}
mat4 rotate(const mat4 &v, float angle, const vec3 &axis){
	float	rad = radians(angle);
	float	cos_a = std::cos(rad);
	float	sin_a = std::sin(rad);
	float	one_minus_cos = 1.f - cos_a;

	vec3	norm_axis = normalize(axis);
	float	x = norm_axis.x;
	float	y = norm_axis.y;
	float	z = norm_axis.z;

	mat4	rotation(1.f);
	rotation.m[0][0] = cos_a + x * x * one_minus_cos;
	rotation.m[0][1] = x * y * one_minus_cos - z * sin_a;
	rotation.m[0][2] = x * z * one_minus_cos + y * sin_a;
	rotation.m[1][0] = y * x * one_minus_cos + z * sin_a;
	rotation.m[1][1] = cos_a + y * y * one_minus_cos;
	rotation.m[1][2] = y * z * one_minus_cos - x * sin_a;
	rotation.m[2][0] = z * x * one_minus_cos - y * sin_a;
	rotation.m[2][1] = z * y * one_minus_cos + x * sin_a;
	rotation.m[2][2] = cos_a + z * z * one_minus_cos;
	return (v * rotation);
}

float	clamp(float x, float minVal, float maxVal){
	return (std::max(minVal, std::min(x, maxVal)));
}
vec2	clamp(const vec2 &v, float minVal, float maxVal){
	return (vec2(clamp(v.x, minVal, maxVal), clamp(v.y, minVal, maxVal)));
}
vec3	clamp(const vec3 &v, float minVal, float maxVal){
	return (vec3(
		clamp(v.x, minVal, maxVal),
		clamp(v.y, minVal, maxVal),
		clamp(v.z, minVal, maxVal)
	));
}
vec4	clamp(const vec4 &v, float minVal, float maxVal){
	return (vec4(
		clamp(v.x, minVal, maxVal),
		clamp(v.y, minVal, maxVal),
		clamp(v.z, minVal, maxVal),
		clamp(v.w, minVal, maxVal)
	));
}

}
