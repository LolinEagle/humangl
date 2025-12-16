#include <VeCamera.hpp>

VeCamera::VeCamera(void){
}

VeCamera::~VeCamera(){
}

void			VeCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far){
	const float	tanHalfFovy = tan(fovy / 2.f);

	_projectionMatrix = vem::mat4{0.0f};
	_projectionMatrix.m[0][0] = 1.f / (aspect * tanHalfFovy);
	_projectionMatrix.m[1][1] = 1.f / (tanHalfFovy);
	_projectionMatrix.m[2][2] = far / (far - near);
	_projectionMatrix.m[2][3] = 1.f;
	_projectionMatrix.m[3][2] = -(far * near) / (far - near);
}

void			VeCamera::setViewYXZ(vem::vec3 position, vem::vec3 rotation){
	const float	c3 = cos(rotation.z);
	const float	s3 = sin(rotation.z);
	const float	c2 = cos(rotation.x);
	const float	s2 = sin(rotation.x);
	const float	c1 = cos(rotation.y);
	const float	s1 = sin(rotation.y);
	const vem::vec3	u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
	const vem::vec3	v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
	const vem::vec3	w{(c2 * s1), (-s2), (c1 * c2)};

	_viewMatrix = vem::mat4{1.f};
	_viewMatrix.m[0][0] = u.x;
	_viewMatrix.m[1][0] = u.y;
	_viewMatrix.m[2][0] = u.z;
	_viewMatrix.m[0][1] = v.x;
	_viewMatrix.m[1][1] = v.y;
	_viewMatrix.m[2][1] = v.z;
	_viewMatrix.m[0][2] = w.x;
	_viewMatrix.m[1][2] = w.y;
	_viewMatrix.m[2][2] = w.z;
	_viewMatrix.m[3][0] = -vem::dot(u, position);
	_viewMatrix.m[3][1] = -vem::dot(v, position);
	_viewMatrix.m[3][2] = -vem::dot(w, position);

	_inverseViewMatrix = vem::mat4{1.f};
	_inverseViewMatrix.m[0][0] = u.x;
	_inverseViewMatrix.m[0][1] = u.y;
	_inverseViewMatrix.m[0][2] = u.z;
	_inverseViewMatrix.m[1][0] = v.x;
	_inverseViewMatrix.m[1][1] = v.y;
	_inverseViewMatrix.m[1][2] = v.z;
	_inverseViewMatrix.m[2][0] = w.x;
	_inverseViewMatrix.m[2][1] = w.y;
	_inverseViewMatrix.m[2][2] = w.z;
	_inverseViewMatrix.m[3][0] = position.x;
	_inverseViewMatrix.m[3][1] = position.y;
	_inverseViewMatrix.m[3][2] = position.z;
}

const vem::mat4	&VeCamera::getProjection(void) const {
	return (_projectionMatrix);
}

const vem::mat4	&VeCamera::getView(void) const {
	return (_viewMatrix);
}

const vem::mat4	&VeCamera::getInverseView(void) const {
	return (_inverseViewMatrix);
}

const vem::vec3	VeCamera::getPosition(void) const {
	return (vem::vec3(
		_inverseViewMatrix.m[3][0], _inverseViewMatrix.m[3][1], _inverseViewMatrix.m[3][2]
	));
}
